# HTTP/2 JSON Client

`axis2_h2_json_client.h` is the client half of the HTTP/2 JSON transport: it
POSTs a JSON body to one operation of an Axis2/C service and returns the JSON
response. Until it existed, Axis2/C could serve JSON over HTTP/2 (mod_axis2
under mod_http2) but could not call such a service; the HTTP/2 transport
sender in `src/core/transport/h2/sender/axis2_h2_transport_sender.c` is still
a placeholder.

The library is `libaxis2_h2_client`, built with `--enable-http2 --enable-json`.
It depends on **axutil, nghttp2 and OpenSSL only** — no engine, no axiom, no
`axis2.xml` — so an embedded or mobile caller links what it uses. On Android it
cross-compiles with the NDK against the same nghttp2 and OpenSSL the server
build uses.

## Use

```c
#include <axis2_h2_json_client.h>

axis2_h2_json_client_options_t o = {0};
o.host        = "192.168.1.20";        /* connect address */
o.port        = 8443;
o.verify_name = "service.local";       /* name the server cert carries */
o.ca_file     = "ca.crt";
o.cert_file   = "client.crt";          /* mTLS; omit for none */
o.key_file    = "client.key";

axis2_h2_json_client_t *c = axis2_h2_json_client_create(env, &o);

const char *body = "{\"n_assets\":2,\"weights\":[0.5,0.5],"
                   "\"covariance_matrix\":[[0.04,0.01],[0.01,0.09]]}";
axis2_char_t *resp = NULL;
size_t len = 0;
int status = 0;

if (axis2_h2_json_client_post(c, env,
        "/services/FinancialBenchmarkService/portfolioVariance",
        body, strlen(body), &resp, &len, &status) == AXIS2_SUCCESS) {
    /* status is the HTTP status; resp is the JSON body, NUL-terminated */
    AXIS2_FREE(env->allocator, resp);
} else {
    printf("%s\n", axis2_h2_json_client_get_error(c));
}

axis2_h2_json_client_free(c, env);
```

One client holds one connection, opened on the first post and reused by the
next. A client is not thread-safe; use one per thread.

An operation that works for a long time before it answers -- one that
starts a file transfer and replies when it finishes, say -- sends nothing on
the stream meanwhile, so it needs both `io_timeout_ms` and
`request_timeout_ms` raised to its longest expected run. Set them on a client
kept for that operation; other clients keep the short defaults.

## Rules it enforces

| Rule | Why |
|---|---|
| ALPN must select `h2`; an HTTP/1.1-only server is refused | Axis2/C serves JSON over HTTP/2 only. A silent downgrade would fail later and somewhere less obvious. |
| TLS 1.2 or later, no compression, no renegotiation | RFC 9113 §9.2. |
| The server certificate is always verified, including its name | There is no option to skip it. |
| `verify_name` is separate from `host` | Connecting by IP to a server whose certificate names it by DNS name — the equivalent of `curl --resolve`. An IP-literal `verify_name` is matched against IP SANs and never sent as SNI. |
| Every wait has a deadline | Connect + handshake share `connect_timeout_ms` (default 5 s); each wait for the server is bounded by `io_timeout_ms` (default 30 s), and the whole request and response by `request_timeout_ms` (default 120 s), so a server trickling bytes cannot hold a post. |
| A response over `max_response_bytes` is abandoned, not truncated | Default 16 MB, the server's JSON transport buffer. The stream is reset. |
| Nothing is retried | A POST may not be safe to repeat. After a failure the connection is dropped and the next post opens a new one. |
| A connection the server closed while idle is replaced before the next request | Servers close idle HTTP/2 connections (httpd's keep-alive timeout is 5 s by default). The client checks, without waiting, for a GOAWAY or FIN before it sends; nothing has been sent, so a new connection is not a retry. |
| Any HTTP status is a success | A 500 from the service is still an answer; FAILURE means no complete response arrived. |

Socket I/O runs under OpenSSL memory BIOs, so a write to a closed peer cannot
raise `SIGPIPE` and the library never changes the process's signal handling.

## Test

`make check` in `src/core/transport/h2/test` runs `h2_json_client_test`.
Its offline checks always run. The live checks need a real server and run
only when one is named:

```sh
AXIS2_H2_TEST_HOST=192.168.1.20 AXIS2_H2_TEST_PORT=8443 \
AXIS2_H2_TEST_CA=ca.crt AXIS2_H2_TEST_CERT=client.crt AXIS2_H2_TEST_KEY=client.key \
AXIS2_H2_TEST_NAME=service.local \
  ./h2_json_client_test
```

The default path is the financial benchmark sample's `portfolioVariance`;
`AXIS2_H2_TEST_PATH` names another. The live checks cover three posts on one
connection, a response over the size limit, and a certificate that does not
carry `verify_name`. With `AXIS2_H2_TEST_IDLE_SECS` set longer than the
server's keep-alive timeout they also post after the server has closed the idle
connection.

## Not yet

- A transport sender behind `axis2_svc_client` built on this client, so
  existing client code can reach HTTP/2 JSON services unchanged.
- Concurrent streams on one connection. The client sends one request at a
  time; HTTP/2 multiplexing is available underneath if a caller needs it.
