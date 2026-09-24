/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * JSON over HTTP/2 client. See axis2_h2_json_client.h for the contract.
 *
 * Structure: a non-blocking TCP socket, OpenSSL driven through a pair of
 * memory BIOs, and an nghttp2 client session fed with mem_send / mem_recv.
 * The memory BIOs keep every byte of socket I/O in this file, so each wait is
 * a poll() with a deadline, a write to a closed peer cannot raise SIGPIPE
 * (send() with MSG_NOSIGNAL), and nothing ever blocks without a timeout.
 *
 * Bytes move in two directions, and every function below is one step:
 *
 *   out: nghttp2_session_mem_send -> SSL_write -> wbio -> h2c_flush -> send()
 *        (h2c_pump_send drives the whole chain)
 *   in:  recv() -> h2c_fill -> rbio -> SSL_read -> nghttp2_session_mem_recv
 *        -> the h2c_on_* callbacks (h2c_pump_recv drives the whole chain)
 *
 * OpenSSL never touches the socket: it reads ciphertext from rbio and writes
 * ciphertext to wbio, and this file moves it between the BIOs and the fd.
 * nghttp2 likewise never touches TLS: it hands out and takes in plaintext
 * frames. One request is in flight at a time; its state lives in the struct.
 */

#include <axis2_h2_json_client.h>
#include <axutil_string.h>
#include <axutil_log.h>

#include <nghttp2/nghttp2.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/x509v3.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#error "The HTTP/2 JSON client requires OpenSSL 1.1.0 or later"
#endif

/* nghttp2 1.60 replaced mem_send, mem_recv, submit_request and the data
 * provider with ssize-returning "2" variants and deprecated the originals. */
#if NGHTTP2_VERSION_NUM >= 0x013c00
#define H2C_NGHTTP2_V2 1
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0          /* no such flag: SO_NOSIGPIPE is set instead */
#endif

#define H2C_DEFAULT_CONNECT_MS   5000
#define H2C_DEFAULT_IO_MS        30000
#define H2C_DEFAULT_MAX_RESPONSE ((size_t)16 * 1024 * 1024)
#define H2C_IO_CHUNK             16384
#define H2C_STREAM_WINDOW        (1 << 20)   /* per-stream receive window */
#define H2C_CONNECTION_WINDOW    (1 << 24)   /* connection receive window */

struct axis2_h2_json_client
{
    /* configuration, copied at create */
    axis2_char_t *host;
    int port;
    axis2_char_t *verify_name;
    int verify_name_is_ip;
    axis2_char_t *authority;
    int connect_timeout_ms;
    int io_timeout_ms;
    size_t max_response;
    SSL_CTX *ctx;

    /* connection */
    int fd;
    SSL *ssl;
    BIO *rbio;                  /* network -> TLS; owned by ssl */
    BIO *wbio;                  /* TLS -> network; owned by ssl */
    nghttp2_session *session;

    /* the request in flight -- written by the nghttp2 callbacks, read by
     * axis2_h2_json_client_post once the stream closes */
    const axutil_env_t *env;
    int32_t stream_id;
    int stream_closed;          /* set by h2c_on_stream_close */
    uint32_t stream_error;      /* RST_STREAM code; NGHTTP2_NO_ERROR if clean */
    int http_status;            /* from :status; 0 until the headers arrive */
    const axis2_char_t *req_body;   /* caller's buffer, not copied */
    size_t req_len;
    size_t req_off;             /* how much of req_body nghttp2 has taken */
    axis2_char_t *resp;         /* grows in h2c_on_data; handed to the caller */
    size_t resp_len;
    size_t resp_cap;
    int resp_too_large;         /* max_response hit; the stream was cancelled */

    axis2_char_t error[256];
};

/* ------------------------------------------------------------------------ */
/* errors and time                                                           */
/* ------------------------------------------------------------------------ */

static void
h2c_fail(axis2_h2_json_client_t *c, const axutil_env_t *env, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(c->error, sizeof(c->error), fmt, ap);
    va_end(ap);
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[h2_json_client] %s:%d: %s",
                    c->host, c->port, c->error);
}

/* The first queued OpenSSL error as text, or "" -- and clear the queue so a
 * stale entry cannot be reported against a later failure. */
static const char *
h2c_ssl_reason(char *buf, size_t len)
{
    unsigned long e = ERR_get_error();
    buf[0] = '\0';
    if (e)
        ERR_error_string_n(e, buf, len);
    ERR_clear_error();
    return buf;
}

/* 64-bit so the arithmetic cannot overflow where long is 32 bits: a 32-bit
 * long of milliseconds wraps after 24.8 days of uptime. */
static int64_t
h2c_now_ms(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (int64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

/* poll() one descriptor, retrying on EINTR against the same deadline.
 * 1 ready, 0 timed out, -1 error. */
static int
h2c_wait(int fd, short events, int timeout_ms)
{
    int64_t deadline = h2c_now_ms() + timeout_ms;
    for (;;)
    {
        struct pollfd p;
        int64_t left = deadline - h2c_now_ms();
        int r;
        if (left < 0)
            left = 0;
        p.fd = fd;
        p.events = events;
        p.revents = 0;
        r = poll(&p, 1, (int)left);
        if (r > 0)
            return 1;
        if (r == 0)
            return 0;
        if (errno != EINTR)
            return -1;
    }
}

/* ------------------------------------------------------------------------ */
/* socket I/O under the memory BIOs                                          */
/* ------------------------------------------------------------------------ */

/* deadline for h2c_send_all / h2c_flush: none, so each wait for the socket
 * to drain gets io_timeout_ms. The handshake passes its connect deadline
 * instead, so a server that stalls its receive window cannot stretch
 * connect_timeout_ms to io_timeout_ms. */
#define H2C_NO_DEADLINE ((int64_t)-1)

static axis2_status_t
h2c_send_all(axis2_h2_json_client_t *c, const axutil_env_t *env,
             const char *buf, size_t len, int64_t deadline)
{
    while (len > 0)
    {
        ssize_t n = send(c->fd, buf, len, MSG_NOSIGNAL);
        if (n > 0)
        {
            buf += n;
            len -= (size_t)n;
            continue;
        }
        if (n < 0 && errno == EINTR)
            continue;
        if (n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
        {
            int64_t left = deadline == H2C_NO_DEADLINE ? c->io_timeout_ms
                                                       : deadline - h2c_now_ms();
            int w = left > 0 ? h2c_wait(c->fd, POLLOUT, (int)left) : 0;
            if (w > 0)
                continue;
            h2c_fail(c, env, w == 0 ? "timed out sending to the server"
                                    : "poll failed while sending: %s", strerror(errno));
            return AXIS2_FAILURE;
        }
        h2c_fail(c, env, "send failed: %s", n < 0 ? strerror(errno) : "no progress");
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/* Send whatever TLS has produced -- the ciphertext waiting in wbio. Called
 * after anything that can make OpenSSL write: a handshake step, SSL_write,
 * SSL_shutdown, and SSL_read (which can owe the peer a reply). */
static axis2_status_t
h2c_flush(axis2_h2_json_client_t *c, const axutil_env_t *env, int64_t deadline)
{
    char buf[H2C_IO_CHUNK];
    int n;
    while ((n = BIO_read(c->wbio, buf, (int)sizeof(buf))) > 0)
    {
        if (h2c_send_all(c, env, buf, (size_t)n, deadline) != AXIS2_SUCCESS)
            return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/* Wait up to timeout_ms for bytes from the server and hand them to TLS.
 * One recv() per call: the caller asks OpenSSL for plaintext afterwards and
 * calls again only if OpenSSL wants more. timeout_ms 0 is a non-blocking
 * check (see h2c_peer_gone). */
static axis2_status_t
h2c_fill(axis2_h2_json_client_t *c, const axutil_env_t *env, int timeout_ms)
{
    char buf[H2C_IO_CHUNK];
    ssize_t n;
    int w = h2c_wait(c->fd, POLLIN, timeout_ms);
    if (w == 0)
    {
        h2c_fail(c, env, "timed out after %d ms waiting for the server", timeout_ms);
        return AXIS2_FAILURE;
    }
    if (w < 0)
    {
        h2c_fail(c, env, "poll failed while receiving: %s", strerror(errno));
        return AXIS2_FAILURE;
    }
    do
        n = recv(c->fd, buf, sizeof(buf), 0);
    while (n < 0 && errno == EINTR);
    if (n == 0)
    {
        h2c_fail(c, env, "the server closed the connection");
        return AXIS2_FAILURE;
    }
    if (n < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return AXIS2_SUCCESS;       /* spurious wakeup; the caller retries */
        h2c_fail(c, env, "recv failed: %s", strerror(errno));
        return AXIS2_FAILURE;
    }
    if (BIO_write(c->rbio, buf, (int)n) != (int)n)
    {
        h2c_fail(c, env, "could not buffer %d bytes for TLS", (int)n);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

/* ------------------------------------------------------------------------ */
/* nghttp2 callbacks                                                         */
/* ------------------------------------------------------------------------ */

/* nghttp2 calls the h2c_on_* callbacks from inside nghttp2_session_mem_recv,
 * i.e. from h2c_pump_recv. They only record into the struct; frames for any
 * stream other than the request in flight are ignored. */

/* Keep :status. A valid status is exactly three digits (RFC 9110 15);
 * anything else leaves http_status 0, which the post reports. */
static int
h2c_on_header(nghttp2_session *session, const nghttp2_frame *frame,
              const uint8_t *name, size_t namelen,
              const uint8_t *value, size_t valuelen,
              uint8_t flags, void *user_data)
{
    axis2_h2_json_client_t *c = (axis2_h2_json_client_t *)user_data;
    (void)session;
    (void)flags;
    if (frame->hd.type == NGHTTP2_HEADERS && frame->hd.stream_id == c->stream_id &&
        namelen == 7 && memcmp(name, ":status", 7) == 0 && valuelen == 3)
    {
        c->http_status = (value[0] - '0') * 100 + (value[1] - '0') * 10 + (value[2] - '0');
    }
    return 0;
}

static int
h2c_on_data(nghttp2_session *session, uint8_t flags, int32_t stream_id,
            const uint8_t *data, size_t len, void *user_data)
{
    axis2_h2_json_client_t *c = (axis2_h2_json_client_t *)user_data;
    (void)flags;
    if (stream_id != c->stream_id || c->resp_too_large)
        return 0;
    /* Written as a subtraction so it cannot overflow; resp_len never exceeds
     * max_response, so the right-hand side cannot underflow. */
    if (len > c->max_response - c->resp_len)
    {
        /* Stop the server rather than keep buffering; the post reports it.
         * Returning 0 rather than an error keeps the session parseable until
         * the stream closes, so the failure is reported as "too large" and
         * not as a protocol error. */
        c->resp_too_large = 1;
        nghttp2_submit_rst_stream(session, NGHTTP2_FLAG_NONE, stream_id, NGHTTP2_CANCEL);
        return 0;
    }
    if (c->resp_len + len + 1 > c->resp_cap)
    {
        size_t cap = c->resp_cap ? c->resp_cap : 4096;
        axis2_char_t *grown;
        /* need cannot wrap: resp_len + len <= max_response < SIZE_MAX (see
         * create). Doubling stops short of overflow and takes need instead. */
        size_t need = c->resp_len + len + 1;
        while (cap < need)
            cap = cap > SIZE_MAX / 2 ? need : cap * 2;
        grown = AXIS2_REALLOC(c->env->allocator, c->resp, cap);
        if (!grown)
            return NGHTTP2_ERR_CALLBACK_FAILURE;
        c->resp = grown;
        c->resp_cap = cap;
    }
    memcpy(c->resp + c->resp_len, data, len);
    c->resp_len += len;
    return 0;
}

static int
h2c_on_stream_close(nghttp2_session *session, int32_t stream_id,
                    uint32_t error_code, void *user_data)
{
    axis2_h2_json_client_t *c = (axis2_h2_json_client_t *)user_data;
    (void)session;
    if (stream_id == c->stream_id)
    {
        c->stream_closed = 1;
        c->stream_error = error_code;
    }
    return 0;
}

/* nghttp2 pulls the request body through this as flow control allows,
 * `length` bytes at most per call; EOF is flagged with the last chunk. */
#ifdef H2C_NGHTTP2_V2
static nghttp2_ssize
h2c_read_body(nghttp2_session *session, int32_t stream_id, uint8_t *buf,
              size_t length, uint32_t *data_flags, nghttp2_data_source *source,
              void *user_data)
#else
static ssize_t
h2c_read_body(nghttp2_session *session, int32_t stream_id, uint8_t *buf,
              size_t length, uint32_t *data_flags, nghttp2_data_source *source,
              void *user_data)
#endif
{
    axis2_h2_json_client_t *c = (axis2_h2_json_client_t *)user_data;
    size_t left = c->req_len - c->req_off;
    size_t n = left < length ? left : length;
    (void)session;
    (void)stream_id;
    (void)source;
    if (n > 0)
        memcpy(buf, c->req_body + c->req_off, n);
    c->req_off += n;
    if (c->req_off == c->req_len)
        *data_flags |= NGHTTP2_DATA_FLAG_EOF;
    return (int)n;
}

/* ------------------------------------------------------------------------ */
/* session pump                                                              */
/* ------------------------------------------------------------------------ */

/* Move everything nghttp2 wants to send through TLS onto the socket. */
static axis2_status_t
h2c_pump_send(axis2_h2_json_client_t *c, const axutil_env_t *env)
{
    for (;;)
    {
        const uint8_t *data = NULL;
#ifdef H2C_NGHTTP2_V2
        nghttp2_ssize n = nghttp2_session_mem_send2(c->session, &data);
#else
        ssize_t n = nghttp2_session_mem_send(c->session, &data);
#endif
        if (n < 0)
        {
            h2c_fail(c, env, "HTTP/2 framing error: %s", nghttp2_strerror((int)n));
            return AXIS2_FAILURE;
        }
        if (n == 0)
            break;
        if (SSL_write(c->ssl, data, (int)n) != (int)n)
        {
            char why[160];
            h2c_fail(c, env, "TLS write failed: %s", h2c_ssl_reason(why, sizeof(why)));
            return AXIS2_FAILURE;
        }
    }
    return h2c_flush(c, env, H2C_NO_DEADLINE);
}

/* Read what the server has sent -- waiting up to io_timeout for the first
 * byte -- and feed it to nghttp2. Returns once TLS has nothing more to give.
 *
 * SSL_read returning WANT_READ means "no complete record buffered". Before
 * anything has been decoded that means wait on the socket; after something
 * has, return so the caller can act on it (send WINDOW_UPDATEs, see whether
 * the stream closed) instead of blocking for more. */
static axis2_status_t
h2c_pump_recv(axis2_h2_json_client_t *c, const axutil_env_t *env)
{
    char buf[H2C_IO_CHUNK];
    int got_any = 0;
    for (;;)
    {
        int n = SSL_read(c->ssl, buf, (int)sizeof(buf));
        if (n > 0)
        {
#ifdef H2C_NGHTTP2_V2
            nghttp2_ssize rv = nghttp2_session_mem_recv2(c->session, (const uint8_t *)buf, (size_t)n);
#else
            ssize_t rv = nghttp2_session_mem_recv(c->session, (const uint8_t *)buf, (size_t)n);
#endif
            if (rv < 0)
            {
                h2c_fail(c, env, "HTTP/2 protocol error from the server: %s",
                         nghttp2_strerror((int)rv));
                return AXIS2_FAILURE;
            }
            got_any = 1;
            continue;
        }
        switch (SSL_get_error(c->ssl, n))
        {
            case SSL_ERROR_WANT_READ:
                if (got_any)
                    return AXIS2_SUCCESS;
                /* TLS may owe the server something first (a key update). */
                if (h2c_flush(c, env, H2C_NO_DEADLINE) != AXIS2_SUCCESS ||
                    h2c_fill(c, env, c->io_timeout_ms) != AXIS2_SUCCESS)
                    return AXIS2_FAILURE;
                break;
            case SSL_ERROR_ZERO_RETURN:
                h2c_fail(c, env, "the server closed the TLS session");
                return AXIS2_FAILURE;
            default:
            {
                char why[160];
                h2c_fail(c, env, "TLS read failed: %s", h2c_ssl_reason(why, sizeof(why)));
                return AXIS2_FAILURE;
            }
        }
    }
}

/* ------------------------------------------------------------------------ */
/* connect / disconnect                                                      */
/* ------------------------------------------------------------------------ */

static void
h2c_disconnect(axis2_h2_json_client_t *c, const axutil_env_t *env, int graceful)
{
    if (graceful && c->session && c->ssl)
    {
        /* Best effort: GOAWAY and close_notify, without waiting for replies.
         * Only on free(); after a failure the connection is dropped outright.
         * c->error is saved and restored so a failure here cannot overwrite
         * the error the caller is about to read. */
        axis2_char_t saved[sizeof(c->error)];
        memcpy(saved, c->error, sizeof(saved));
        nghttp2_session_terminate_session(c->session, NGHTTP2_NO_ERROR);
        if (h2c_pump_send(c, env) == AXIS2_SUCCESS && SSL_shutdown(c->ssl) >= 0)
            h2c_flush(c, env, H2C_NO_DEADLINE);
        memcpy(c->error, saved, sizeof(saved));
        ERR_clear_error();
    }
    if (c->session)
    {
        nghttp2_session_del(c->session);
        c->session = NULL;
    }
    if (c->ssl)
    {
        SSL_free(c->ssl);       /* frees both memory BIOs */
        c->ssl = NULL;
        c->rbio = NULL;
        c->wbio = NULL;
    }
    if (c->fd >= 0)
    {
        close(c->fd);
        c->fd = -1;
    }
}

/* Try each address getaddrinfo returns (IPv6 and IPv4, in its order) until
 * one connects. The socket is non-blocking from the start so the connect
 * itself honours the deadline; it stays non-blocking for the connection's
 * life, which is what lets every later wait go through h2c_wait. */
static axis2_status_t
h2c_tcp_connect(axis2_h2_json_client_t *c, const axutil_env_t *env, int64_t deadline)
{
    struct addrinfo hints, *res = NULL, *ai;
    char port[16];
    int rc, last_errno = 0;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    snprintf(port, sizeof(port), "%d", c->port);
    rc = getaddrinfo(c->host, port, &hints, &res);
    if (rc != 0)
    {
        h2c_fail(c, env, "cannot resolve %s: %s", c->host, gai_strerror(rc));
        return AXIS2_FAILURE;
    }

    for (ai = res; ai; ai = ai->ai_next)
    {
        int fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
        int one = 1, err = 0;
        socklen_t errlen = sizeof(err);
        int64_t left;
        if (fd < 0)
        {
            last_errno = errno;
            continue;
        }
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
#ifdef SO_NOSIGPIPE
        setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one));
#endif
        if (connect(fd, ai->ai_addr, ai->ai_addrlen) == 0)
            err = 0;
        else if (errno != EINPROGRESS)
            err = errno;
        else
        {
            left = deadline - h2c_now_ms();
            rc = h2c_wait(fd, POLLOUT, left > 0 ? (int)left : 0);
            if (rc == 0)
                err = ETIMEDOUT;
            else if (rc < 0)
                err = errno;
            else if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &errlen) != 0)
                err = errno;
        }
        if (err == 0)
        {
            setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
            c->fd = fd;
            freeaddrinfo(res);
            return AXIS2_SUCCESS;
        }
        last_errno = err;
        close(fd);
    }
    freeaddrinfo(res);
    h2c_fail(c, env, "cannot connect: %s", strerror(last_errno ? last_errno : ECONNREFUSED));
    return AXIS2_FAILURE;
}

static axis2_status_t
h2c_connect(axis2_h2_json_client_t *c, const axutil_env_t *env)
{
    int64_t deadline = h2c_now_ms() + c->connect_timeout_ms;
    const unsigned char *alpn = NULL;
    unsigned int alpn_len = 0;
    nghttp2_session_callbacks *cbs = NULL;
    nghttp2_settings_entry settings[2];
    char why[160];
    int rv;

    if (h2c_tcp_connect(c, env, deadline) != AXIS2_SUCCESS)
        return AXIS2_FAILURE;

    c->ssl = SSL_new(c->ctx);
    c->rbio = BIO_new(BIO_s_mem());
    c->wbio = BIO_new(BIO_s_mem());
    if (!c->ssl || !c->rbio || !c->wbio)
    {
        /* Not yet attached, so SSL_free would not release them. */
        if (c->rbio)
            BIO_free(c->rbio);
        if (c->wbio)
            BIO_free(c->wbio);
        c->rbio = c->wbio = NULL;
        h2c_fail(c, env, "out of memory setting up TLS");
        return AXIS2_FAILURE;
    }
    SSL_set_bio(c->ssl, c->rbio, c->wbio);      /* ssl owns both from here */
    SSL_set_connect_state(c->ssl);

    /* Name checks. An IP literal is matched against the certificate's IP
     * SANs and is never sent as SNI (RFC 6066 forbids it); a DNS name is
     * matched against its DNS SANs and sent as SNI. */
    if (c->verify_name_is_ip)
    {
        if (X509_VERIFY_PARAM_set1_ip_asc(SSL_get0_param(c->ssl), c->verify_name) != 1)
        {
            h2c_fail(c, env, "cannot use %s as a verify name", c->verify_name);
            return AXIS2_FAILURE;
        }
    }
    else
    {
        SSL_set_hostflags(c->ssl, X509_CHECK_FLAG_NO_PARTIAL_WILDCARDS);
        if (SSL_set1_host(c->ssl, c->verify_name) != 1 ||
            SSL_set_tlsext_host_name(c->ssl, c->verify_name) != 1)
        {
            h2c_fail(c, env, "cannot use %s as a verify name", c->verify_name);
            return AXIS2_FAILURE;
        }
    }

    /* Handshake: each SSL_do_handshake step may leave records in wbio (sent
     * now) and then want the server's reply (waited for against the same
     * deadline as the TCP connect). Certificate and name verification
     * happen inside the handshake, so a failure here names the reason. */
    for (;;)
    {
        int64_t left;
        rv = SSL_do_handshake(c->ssl);
        if (h2c_flush(c, env, deadline) != AXIS2_SUCCESS)
            return AXIS2_FAILURE;
        if (rv == 1)
            break;
        if (SSL_get_error(c->ssl, rv) != SSL_ERROR_WANT_READ)
        {
            long vr = SSL_get_verify_result(c->ssl);
            if (vr != X509_V_OK)
                h2c_fail(c, env, "server certificate rejected (verify name %s): %s",
                         c->verify_name, X509_verify_cert_error_string(vr));
            else
                h2c_fail(c, env, "TLS handshake failed: %s", h2c_ssl_reason(why, sizeof(why)));
            return AXIS2_FAILURE;
        }
        left = deadline - h2c_now_ms();
        if (left <= 0)
        {
            h2c_fail(c, env, "TLS handshake timed out after %d ms", c->connect_timeout_ms);
            return AXIS2_FAILURE;
        }
        if (h2c_fill(c, env, (int)left) != AXIS2_SUCCESS)
            return AXIS2_FAILURE;
    }

    SSL_get0_alpn_selected(c->ssl, &alpn, &alpn_len);
    if (alpn_len != 2 || memcmp(alpn, "h2", 2) != 0)
    {
        h2c_fail(c, env, "the server did not negotiate HTTP/2 (ALPN h2); "
                         "Axis2/C serves JSON over HTTP/2 only");
        return AXIS2_FAILURE;
    }

    if (nghttp2_session_callbacks_new(&cbs) != 0)
    {
        h2c_fail(c, env, "out of memory setting up HTTP/2");
        return AXIS2_FAILURE;
    }
    nghttp2_session_callbacks_set_on_header_callback(cbs, h2c_on_header);
    nghttp2_session_callbacks_set_on_data_chunk_recv_callback(cbs, h2c_on_data);
    nghttp2_session_callbacks_set_on_stream_close_callback(cbs, h2c_on_stream_close);
    rv = nghttp2_session_client_new(&c->session, cbs, c);
    nghttp2_session_callbacks_del(cbs);
    if (rv != 0)
    {
        h2c_fail(c, env, "cannot create the HTTP/2 session: %s", nghttp2_strerror(rv));
        return AXIS2_FAILURE;
    }

    /* No server push (nothing here could use it). Larger receive windows
     * than the 64 KiB default so a big response is not throttled to one
     * window per round trip; max_response still bounds what is kept. The
     * SETTINGS frame is only queued here -- the first post sends it. */
    settings[0].settings_id = NGHTTP2_SETTINGS_ENABLE_PUSH;
    settings[0].value = 0;
    settings[1].settings_id = NGHTTP2_SETTINGS_INITIAL_WINDOW_SIZE;
    settings[1].value = H2C_STREAM_WINDOW;
    rv = nghttp2_submit_settings(c->session, NGHTTP2_FLAG_NONE, settings, 2);
    if (rv == 0)
        rv = nghttp2_session_set_local_window_size(c->session, NGHTTP2_FLAG_NONE, 0,
                                                   H2C_CONNECTION_WINDOW);
    if (rv != 0)
    {
        h2c_fail(c, env, "cannot send HTTP/2 settings: %s", nghttp2_strerror(rv));
        return AXIS2_FAILURE;
    }
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[h2_json_client] connected to %s:%d (%s, %s)",
                    c->host, c->port, SSL_get_version(c->ssl), c->verify_name);
    return AXIS2_SUCCESS;
}

/* Before a new request on a reused connection: did the server end it while
 * we were idle? Servers routinely close idle HTTP/2 connections (httpd's
 * keep-alive timeout is 5 s by default) with GOAWAY and a FIN; finding that
 * out by failing the next request would fail every first call after a
 * pause. Nothing of the new request has been sent yet, so opening a fresh
 * connection here is not a retry. Returns 1 when the connection is unusable. */
static int
h2c_peer_gone(axis2_h2_json_client_t *c, const axutil_env_t *env)
{
    struct pollfd p;
    char buf[H2C_IO_CHUNK];
    ssize_t n;

    p.fd = c->fd;
    p.events = POLLIN;
    p.revents = 0;
    if (poll(&p, 1, 0) <= 0)
        return 0;                       /* nothing waiting: still open */
    do
        n = recv(c->fd, buf, 1, MSG_PEEK);
    while (n < 0 && errno == EINTR);
    if (n == 0)
        return 1;                       /* FIN */
    if (n < 0)
        return !(errno == EAGAIN || errno == EWOULDBLOCK);

    /* Bytes are waiting -- GOAWAY, PING, a TLS close_notify. Take them in
     * and let nghttp2 decide. */
    if (h2c_fill(c, env, 0) != AXIS2_SUCCESS)
        return 1;
    for (;;)
    {
        int r = SSL_read(c->ssl, buf, (int)sizeof(buf));
        if (r > 0)
        {
#ifdef H2C_NGHTTP2_V2
            if (nghttp2_session_mem_recv2(c->session, (const uint8_t *)buf, (size_t)r) < 0)
#else
            if (nghttp2_session_mem_recv(c->session, (const uint8_t *)buf, (size_t)r) < 0)
#endif
                return 1;
            continue;
        }
        if (SSL_get_error(c->ssl, r) != SSL_ERROR_WANT_READ)
            return 1;                   /* close_notify or a TLS error */
        break;
    }
    if (h2c_pump_send(c, env) != AXIS2_SUCCESS)
        return 1;
    return !nghttp2_session_want_read(c->session) && !nghttp2_session_want_write(c->session);
}

/* ------------------------------------------------------------------------ */
/* public API                                                                */
/* ------------------------------------------------------------------------ */

static int
h2c_is_ip_literal(const char *s)
{
    unsigned char buf[sizeof(struct in6_addr)];
    return inet_pton(AF_INET, s, buf) == 1 || inet_pton(AF_INET6, s, buf) == 1;
}

AXIS2_EXTERN axis2_h2_json_client_t *AXIS2_CALL
axis2_h2_json_client_create(
    const axutil_env_t *env,
    const axis2_h2_json_client_options_t *o)
{
    axis2_h2_json_client_t *c;
    static const unsigned char alpn_h2[] = { 2, 'h', '2' };
    char why[160];
    const char *missing = NULL;
    size_t alen;

    if (!env)
        return NULL;
    if (!o)
        missing = "options";
    else if (!o->host || !o->host[0])
        missing = "host";
    else if (o->port <= 0 || o->port > 65535)
        missing = "port (1-65535)";
    else if (!o->ca_file || !o->ca_file[0])
        missing = "ca_file";
    else if (o->cert_file && (!o->key_file || !o->key_file[0]))
        missing = "key_file (cert_file is set)";
    if (missing)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[h2_json_client] missing or invalid option: %s", missing);
        return NULL;
    }

    c = AXIS2_MALLOC(env->allocator, sizeof(*c));
    if (!c)
        return NULL;
    memset(c, 0, sizeof(*c));
    c->fd = -1;
    c->port = o->port;
    c->connect_timeout_ms = o->connect_timeout_ms > 0 ? o->connect_timeout_ms : H2C_DEFAULT_CONNECT_MS;
    c->io_timeout_ms = o->io_timeout_ms > 0 ? o->io_timeout_ms : H2C_DEFAULT_IO_MS;
    c->max_response = o->max_response_bytes > 0 ? o->max_response_bytes : H2C_DEFAULT_MAX_RESPONSE;
    if (c->max_response == SIZE_MAX)
        c->max_response = SIZE_MAX - 1;     /* room for the terminating NUL */
    c->host = axutil_strdup(env, o->host);
    c->verify_name = axutil_strdup(env, (o->verify_name && o->verify_name[0]) ? o->verify_name : o->host);
    if (!c->host || !c->verify_name)
        goto fail;
    c->verify_name_is_ip = h2c_is_ip_literal(c->verify_name);

    /* :authority -- bracketed when an IPv6 literal. The name the certificate
     * is checked against is also the one the request names, so a virtual
     * host behind a load balancer's address routes correctly. +16 covers
     * "[", "]", ":", five port digits and the NUL. */
    alen = strlen(c->verify_name) + 16;
    c->authority = AXIS2_MALLOC(env->allocator, alen);
    if (!c->authority)
        goto fail;
    snprintf(c->authority, alen, strchr(c->verify_name, ':') ? "[%s]:%d" : "%s:%d",
             c->verify_name, c->port);

    ERR_clear_error();
    c->ctx = SSL_CTX_new(TLS_client_method());
    if (!c->ctx)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[h2_json_client] SSL_CTX_new: %s",
                        h2c_ssl_reason(why, sizeof(why)));
        goto fail;
    }
    /* HTTP/2 requires TLS 1.2 or later (RFC 9113 section 9.2) and forbids
     * compression and renegotiation. */
    SSL_CTX_set_min_proto_version(c->ctx, TLS1_2_VERSION);
    SSL_CTX_set_options(c->ctx, SSL_OP_NO_COMPRESSION
#ifdef SSL_OP_NO_RENEGOTIATION
                                | SSL_OP_NO_RENEGOTIATION
#endif
                        );
    SSL_CTX_set_verify(c->ctx, SSL_VERIFY_PEER, NULL);
    if (SSL_CTX_load_verify_locations(c->ctx, o->ca_file, NULL) != 1)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[h2_json_client] cannot load ca_file %s: %s",
                        o->ca_file, h2c_ssl_reason(why, sizeof(why)));
        goto fail;
    }
    if (o->cert_file)
    {
        if (SSL_CTX_use_certificate_chain_file(c->ctx, o->cert_file) != 1 ||
            SSL_CTX_use_PrivateKey_file(c->ctx, o->key_file, SSL_FILETYPE_PEM) != 1 ||
            SSL_CTX_check_private_key(c->ctx) != 1)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "[h2_json_client] cannot use client cert %s / key %s: %s",
                            o->cert_file, o->key_file, h2c_ssl_reason(why, sizeof(why)));
            goto fail;
        }
    }
    /* SSL_CTX_set_alpn_protos returns 0 on success. */
    if (SSL_CTX_set_alpn_protos(c->ctx, alpn_h2, sizeof(alpn_h2)) != 0)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[h2_json_client] cannot offer ALPN h2");
        goto fail;
    }
    return c;

fail:
    axis2_h2_json_client_free(c, env);
    return NULL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_h2_json_client_post(
    axis2_h2_json_client_t *c,
    const axutil_env_t *env,
    const axis2_char_t *path,
    const axis2_char_t *json,
    size_t json_len,
    axis2_char_t **response_out,
    size_t *response_len_out,
    int *http_status_out)
{
    char clen[24];
    nghttp2_nv hdrs[7];
    int32_t sid;

    if (response_out)
        *response_out = NULL;
    if (response_len_out)
        *response_len_out = 0;
    if (http_status_out)
        *http_status_out = 0;
    if (!c || !env)
        return AXIS2_FAILURE;
    c->error[0] = '\0';
    if (!path || path[0] != '/' || !response_out || (!json && json_len > 0))
    {
        h2c_fail(c, env, "invalid arguments: path must start with '/', response_out is required");
        return AXIS2_FAILURE;
    }

    /* A session the server has finished with cannot carry a new stream;
     * open a fresh one. The error text of a closed idle connection is not
     * this call's failure, so it is cleared. */
    if (c->session && h2c_peer_gone(c, env))
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                        "[h2_json_client] %s:%d closed the idle connection; reconnecting",
                        c->host, c->port);
        h2c_disconnect(c, env, 0);
        c->error[0] = '\0';
    }
    if (!c->session && h2c_connect(c, env) != AXIS2_SUCCESS)
    {
        h2c_disconnect(c, env, 0);
        return AXIS2_FAILURE;
    }

    c->env = env;
    c->stream_closed = 0;
    c->stream_error = 0;
    c->http_status = 0;
    c->req_body = json ? json : "";
    c->req_len = json_len;
    c->req_off = 0;
    c->resp = NULL;
    c->resp_len = 0;
    c->resp_cap = 0;
    c->resp_too_large = 0;

    snprintf(clen, sizeof(clen), "%lu", (unsigned long)json_len);
#define H2C_NV(i, n, v) do { \
        hdrs[i].name = (uint8_t *)(n); hdrs[i].namelen = strlen(n); \
        hdrs[i].value = (uint8_t *)(v); hdrs[i].valuelen = strlen(v); \
        hdrs[i].flags = NGHTTP2_NV_FLAG_NONE; } while (0)
    H2C_NV(0, ":method", "POST");
    H2C_NV(1, ":scheme", "https");
    H2C_NV(2, ":authority", c->authority);
    H2C_NV(3, ":path", path);
    H2C_NV(4, "content-type", "application/json");
    H2C_NV(5, "accept", "application/json");
    H2C_NV(6, "content-length", clen);
#undef H2C_NV

    {
#ifdef H2C_NGHTTP2_V2
        nghttp2_data_provider2 body;
        body.source.ptr = NULL;
        body.read_callback = h2c_read_body;
        sid = nghttp2_submit_request2(c->session, NULL, hdrs, 7, &body, c);
#else
        nghttp2_data_provider body;
        body.source.ptr = NULL;
        body.read_callback = h2c_read_body;
        sid = nghttp2_submit_request(c->session, NULL, hdrs, 7, &body, c);
#endif
    }
    if (sid < 0)
    {
        h2c_fail(c, env, "cannot submit the request: %s", nghttp2_strerror(sid));
        goto fail;
    }
    c->stream_id = sid;

    /* Alternate: send what nghttp2 has queued (headers, body as the window
     * opens, WINDOW_UPDATEs, SETTINGS ACKs), then wait for the server,
     * until the callbacks mark the stream closed. If the session wants
     * neither to read nor write, the server has GOAWAYed us mid-request. */
    while (!c->stream_closed)
    {
        if (h2c_pump_send(c, env) != AXIS2_SUCCESS)
            goto fail;
        if (c->stream_closed)
            break;
        if (!nghttp2_session_want_read(c->session) && !nghttp2_session_want_write(c->session))
        {
            h2c_fail(c, env, "the server ended the HTTP/2 session before responding");
            goto fail;
        }
        if (h2c_pump_recv(c, env) != AXIS2_SUCCESS)
            goto fail;
    }
    /* Acknowledge what arrived with the response (settings, window updates). */
    if (h2c_pump_send(c, env) != AXIS2_SUCCESS)
        goto fail;

    if (c->resp_too_large)
    {
        h2c_fail(c, env, "response to %s exceeded the %lu-byte limit", path,
                 (unsigned long)c->max_response);
        goto fail;
    }
    if (c->stream_error != NGHTTP2_NO_ERROR)
    {
        h2c_fail(c, env, "the server reset the stream for %s: %s", path,
                 nghttp2_http2_strerror(c->stream_error));
        goto fail;
    }
    if (c->http_status == 0)
    {
        h2c_fail(c, env, "the response to %s carried no :status", path);
        goto fail;
    }

    if (!c->resp)
    {
        c->resp = AXIS2_MALLOC(env->allocator, 1);
        if (!c->resp)
        {
            h2c_fail(c, env, "out of memory");
            goto fail;
        }
    }
    /* The terminator is not counted in resp_len; h2c_on_data always leaves
     * room for it. Ownership of the buffer passes to the caller. */
    c->resp[c->resp_len] = '\0';
    *response_out = c->resp;
    if (response_len_out)
        *response_len_out = c->resp_len;
    if (http_status_out)
        *http_status_out = c->http_status;
    c->resp = NULL;
    c->req_body = NULL;
    return AXIS2_SUCCESS;

fail:
    if (c->resp)
        AXIS2_FREE(env->allocator, c->resp);
    c->resp = NULL;
    c->req_body = NULL;
    h2c_disconnect(c, env, 0);
    return AXIS2_FAILURE;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_h2_json_client_get_error(const axis2_h2_json_client_t *c)
{
    return c ? c->error : "";
}

AXIS2_EXTERN void AXIS2_CALL
axis2_h2_json_client_free(axis2_h2_json_client_t *c, const axutil_env_t *env)
{
    if (!c || !env)
        return;
    h2c_disconnect(c, env, 1);
    if (c->ctx)
        SSL_CTX_free(c->ctx);
    if (c->host)
        AXIS2_FREE(env->allocator, c->host);
    if (c->verify_name)
        AXIS2_FREE(env->allocator, c->verify_name);
    if (c->authority)
        AXIS2_FREE(env->allocator, c->authority);
    AXIS2_FREE(env->allocator, c);
}
