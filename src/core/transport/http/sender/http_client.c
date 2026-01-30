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

#include <axis2_http_client.h>
#include <axis2_http_transport.h>
#include <axutil_stream.h>
#include <axutil_string.h>
#include <axutil_network_handler.h>
#include <axis2_http_request_line.h>
#include <axis2_http_header.h>
#include <axis2_http_status_line.h>
#include <axutil_http_chunked_stream.h>
#include <platforms/axutil_platform_auto_sense.h>
#include <axiom_mime_part.h>
#include <axis2_http_transport_utils.h>

#ifdef AXIS2_SSL_ENABLED
#include "ssl/ssl_stream.h"
#endif

#define AXIS2_HTTP_HEADER_LENGTH 4096
#define AXIS2_HTTP_STATUS_LINE_LENGTH 512

/*
 * AXIS2C-1480: Read buffer size for efficient header parsing.
 * Instead of reading 1 byte at a time (expensive syscall per byte),
 * we read chunks and scan for CRLF in memory.
 */
#define AXIS2_HTTP_READ_BUFFER_SIZE 4096

struct axis2_http_client
{
    int sockfd;
    axutil_stream_t *data_stream;
    axutil_url_t *url;
    axis2_http_simple_response_t *response;
    axis2_bool_t request_sent;
    int timeout;
    axis2_bool_t proxy_enabled;
    axis2_char_t *proxy_host;
    int proxy_port;
    axis2_char_t *proxy_host_port;
    axis2_bool_t dump_input_msg;
    axis2_char_t *server_cert;
    axis2_char_t *key_file;
    axis2_char_t *req_body;
    size_t req_body_size;
    axis2_bool_t validate_ssl_hostname;

    /* AXIS2C-1312/1555: Proxy authentication for HTTPS CONNECT */
    axis2_char_t *proxy_auth_username;
    axis2_char_t *proxy_auth_password;

    /* These are for mtom case */
    axutil_array_list_t *mime_parts;
    axis2_bool_t doing_mtom;
    axis2_char_t *mtom_sending_callback_name;
};

/*
 * AXIS2C-1480: Prepend stream wrapper for efficient header reading.
 *
 * When reading HTTP headers in chunks (instead of byte-by-byte), we may
 * read beyond the header boundary into the body. This wrapper stream
 * serves that "leftover" data first before delegating to the underlying
 * socket/SSL stream.
 */
typedef struct axis2_prepend_stream_impl
{
    axutil_stream_t stream;         /* Must be first - allows casting */
    axutil_stream_t *underlying;    /* Original socket/SSL stream */
    axis2_char_t *prepend_data;     /* Leftover data from header reading */
    size_t prepend_pos;             /* Current position in prepend buffer */
    size_t prepend_len;             /* Total length of prepend data */
} axis2_prepend_stream_impl_t;

static int AXIS2_CALL
axis2_prepend_stream_read(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    void *buffer,
    size_t count)
{
    axis2_prepend_stream_impl_t *impl = (axis2_prepend_stream_impl_t *)stream;
    size_t total_read = 0;
    axis2_char_t *buf = (axis2_char_t *)buffer;

    /* First, return any prepend data */
    if (impl->prepend_data && impl->prepend_pos < impl->prepend_len)
    {
        size_t available = impl->prepend_len - impl->prepend_pos;
        size_t to_copy = (available < count) ? available : count;

        memcpy(buf, impl->prepend_data + impl->prepend_pos, to_copy);
        impl->prepend_pos += to_copy;
        total_read = to_copy;

        /* If we've satisfied the request from prepend, return */
        if (total_read >= count)
        {
            return (int)total_read;
        }

        /* Otherwise, read remainder from underlying stream */
        buf += to_copy;
        count -= to_copy;
    }

    /* Read from underlying stream */
    if (impl->underlying && impl->underlying->read)
    {
        int underlying_read = impl->underlying->read(impl->underlying, env, buf, count);
        if (underlying_read > 0)
        {
            total_read += (size_t)underlying_read;
        }
        else if (total_read == 0)
        {
            /* No prepend data and underlying returned error/eof */
            return underlying_read;
        }
    }

    return (int)total_read;
}

static int AXIS2_CALL
axis2_prepend_stream_write(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    const void *buffer,
    size_t count)
{
    axis2_prepend_stream_impl_t *impl = (axis2_prepend_stream_impl_t *)stream;

    /* Delegate write to underlying stream */
    if (impl->underlying && impl->underlying->write)
    {
        return impl->underlying->write(impl->underlying, env, buffer, count);
    }
    return -1;
}

static int AXIS2_CALL
axis2_prepend_stream_skip(
    axutil_stream_t *stream,
    const axutil_env_t *env,
    int count)
{
    axis2_prepend_stream_impl_t *impl = (axis2_prepend_stream_impl_t *)stream;
    int total_skipped = 0;

    /* Skip prepend data first */
    if (impl->prepend_data && impl->prepend_pos < impl->prepend_len)
    {
        int available = impl->prepend_len - impl->prepend_pos;
        int to_skip = (available < count) ? available : count;

        impl->prepend_pos += to_skip;
        total_skipped = to_skip;
        count -= to_skip;
    }

    /* Skip from underlying stream if needed */
    if (count > 0 && impl->underlying && impl->underlying->skip)
    {
        int underlying_skipped = impl->underlying->skip(impl->underlying, env, count);
        if (underlying_skipped > 0)
        {
            total_skipped += underlying_skipped;
        }
    }

    return total_skipped;
}

static void AXIS2_CALL
axis2_prepend_stream_free(
    axutil_stream_t *stream,
    const axutil_env_t *env)
{
    axis2_prepend_stream_impl_t *impl = (axis2_prepend_stream_impl_t *)stream;

    if (impl)
    {
        if (impl->prepend_data)
        {
            AXIS2_FREE(env->allocator, impl->prepend_data);
            impl->prepend_data = NULL;
        }
        /* Free the underlying stream */
        if (impl->underlying)
        {
            axutil_stream_free(impl->underlying, env);
            impl->underlying = NULL;
        }
        AXIS2_FREE(env->allocator, impl);
    }
}

/*
 * Create a prepend stream wrapper that serves prepend_data first,
 * then delegates to underlying stream.
 * Takes ownership of both prepend_data and underlying stream.
 *
 * Memory layout for cleanup:
 * - buffer_head: points to prepend_data (freed by MANAGED handler)
 * - buffer: current read position in prepend_data
 * - len: remaining prepend bytes
 * - fp: stores underlying stream pointer (freed by MANAGED handler)
 */
static axutil_stream_t *
axis2_prepend_stream_create(
    const axutil_env_t *env,
    axis2_char_t *prepend_data,
    size_t prepend_len,
    axutil_stream_t *underlying)
{
    axis2_prepend_stream_impl_t *impl;

    impl = (axis2_prepend_stream_impl_t *)AXIS2_MALLOC(env->allocator,
            sizeof(axis2_prepend_stream_impl_t));
    if (!impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(impl, 0, sizeof(axis2_prepend_stream_impl_t));

    impl->stream.stream_type = AXIS2_STREAM_MANAGED;
    impl->stream.read = axis2_prepend_stream_read;
    impl->stream.write = axis2_prepend_stream_write;
    impl->stream.skip = axis2_prepend_stream_skip;
    impl->stream.axis2_eof = EOF;

    /*
     * Store cleanup info in base stream fields for axutil_stream_free():
     * - buffer_head: prepend_data allocation (freed by MANAGED handler)
     * - buffer: current position (not separately freed)
     * - len: remaining prepend bytes
     * - fp: underlying stream as void* (freed as stream by MANAGED handler)
     */
    impl->stream.buffer_head = prepend_data;
    impl->stream.buffer = prepend_data;
    impl->stream.len = prepend_len;
    impl->stream.max_len = prepend_len;
    impl->stream.fp = (FILE *)underlying;  /* Store stream ptr for cleanup */

    /* Also store in our extended fields for the read/write/skip functions */
    impl->underlying = underlying;
    impl->prepend_data = prepend_data;
    impl->prepend_pos = 0;
    impl->prepend_len = prepend_len;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "Created prepend stream with %d bytes of prepend data (AXIS2C-1480)", prepend_len);

    return (axutil_stream_t *)impl;
}

AXIS2_EXTERN axis2_http_client_t *AXIS2_CALL
axis2_http_client_create(
    const axutil_env_t * env,
    axutil_url_t * url)
{
    axis2_http_client_t *http_client = NULL;

    http_client = (axis2_http_client_t *)AXIS2_MALLOC(env->allocator, sizeof(axis2_http_client_t));

    if(!http_client)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset(http_client, 0, sizeof(axis2_http_client_t));

    http_client->url = url;
    http_client->data_stream = NULL;
    http_client->sockfd = -1;
    http_client->response = NULL;
    http_client->request_sent = AXIS2_FALSE;

    /* default timeout is 60000 milliseconds */
    http_client->timeout = AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT;
    http_client->proxy_enabled = AXIS2_FALSE;
    http_client->proxy_port = 0;
    http_client->proxy_host = NULL;
    http_client->proxy_host_port = NULL;
    http_client->dump_input_msg = AXIS2_FALSE;
    http_client->server_cert = NULL;
    http_client->key_file = NULL;
    http_client->req_body = NULL;
    http_client->req_body_size = 0;
    http_client->mime_parts = NULL;
    http_client->doing_mtom = AXIS2_FALSE;
    http_client->mtom_sending_callback_name = NULL;

    /*
     * Enable SSL/TLS hostname validation by default (AXIS2C-1700).
     * This verifies that the server certificate CN/SAN matches the hostname,
     * preventing man-in-the-middle attacks. Users with self-signed certificates
     * or hostname mismatches can disable via AXIS2_SSL_VERIFY_HOST property.
     */
    http_client->validate_ssl_hostname = AXIS2_TRUE;

    return http_client;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_client_free(
    axis2_http_client_t * http_client,
    const axutil_env_t * env)
{
    if(http_client->proxy_host)
    {
        AXIS2_FREE(env->allocator, http_client->proxy_host);
    }
    if(http_client->proxy_host_port)
    {
        AXIS2_FREE(env->allocator, http_client->proxy_host_port);
    }
    /* AXIS2C-1312/1555: Free proxy authentication credentials */
    if(http_client->proxy_auth_username)
    {
        AXIS2_FREE(env->allocator, http_client->proxy_auth_username);
    }
    if(http_client->proxy_auth_password)
    {
        AXIS2_FREE(env->allocator, http_client->proxy_auth_password);
    }
    if(http_client->url)
    {
        axutil_url_free(http_client->url, env);
    }
    if(http_client->response)
    {
        axis2_http_simple_response_free(http_client->response, env);
    }
    if(-1 != http_client->sockfd)
    {
        if(http_client->data_stream)
        {
            /* AXIS2C-1258: SSL streams need special cleanup to free SSL/SSL_CTX.
             * axutil_stream_free() doesn't handle AXIS2_STREAM_MANAGED properly. */
#ifdef AXIS2_SSL_ENABLED
            if(http_client->data_stream->stream_type == AXIS2_STREAM_SOCKET)
            {
                axutil_stream_free(http_client->data_stream, env);
            }
            else
            {
                axis2_ssl_stream_free(http_client->data_stream, env);
            }
#else
            axutil_stream_free(http_client->data_stream, env);
#endif
            http_client->data_stream = NULL;
        }
		axutil_network_handler_close_socket(env, http_client->sockfd);
        http_client->sockfd = -1;
    }

    if(http_client->req_body)
    {
        AXIS2_FREE(env->allocator, http_client->req_body);
    }

    /* There is no other appropriate place to free the mime_part list when a 
     * particular client send requests. */

    if(http_client->mime_parts)
    {
        int i = 0;
        for(i = 0; i < axutil_array_list_size(http_client->mime_parts, env); i++)
        {
            axiom_mime_part_t *mime_part = NULL;
            mime_part = (axiom_mime_part_t *)axutil_array_list_get(http_client->mime_parts, env, i);
            if(mime_part)
            {
                axiom_mime_part_free(mime_part, env);
            }
        }
        axutil_array_list_free(http_client->mime_parts, env);
    }

    AXIS2_FREE(env->allocator, http_client);
    return;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_client_free_void_arg(
    void *client,
    const axutil_env_t * env)
{
    axis2_http_client_t *client_l = NULL;

    client_l = (axis2_http_client_t *)client;
    axis2_http_client_free(client_l, env);
    return;
}

/* This is the main method which writes to the socket in the case of a client 
 * sends an http_request. Previously this method does not distinguish between a 
 * mtom request and non mtom request. Because what finally it had was the 
 * complete buffer with the request. But now MTOM invocations are done 
 * differently in order to support large attachments so this method should 
 * distinguish those invocations */

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_send(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_http_simple_request_t * request,
    axis2_char_t * ssl_pp)
{
    char *wire_format = NULL;
    axutil_array_list_t *headers = NULL;
    char *str_header = NULL;
    char *str_request_line = NULL;
    int written = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_bool_t chunking_enabled = AXIS2_FALSE;
    axis2_char_t *host = NULL;
    unsigned int port = 0;

    /* In the MTOM case request body is not set. Instead mime_parts
     array_list is there */

    /*if(client->req_body)
    {
        AXIS2_FREE(env->allocator, client->req_body);
        client->req_body = NULL;
    }*/
    if(!client->req_body && !(client->doing_mtom))
    {
        client->req_body_size = axis2_http_simple_request_get_body_bytes(request, env,
            &client->req_body);
    }

    if(client->dump_input_msg == AXIS2_TRUE)
    {
        return AXIS2_SUCCESS;
    }

    if(!client->url)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_URL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Request url not set");
        return AXIS2_FAILURE;
    }

    host = axutil_url_get_host(client->url, env);
    port = axutil_url_get_port(client->url, env);

    if(client->proxy_enabled)
    {
        if(!client->proxy_host || client->proxy_port <= 0)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Proxy port or Host not set");
            return AXIS2_FAILURE;
        }

        if(client->sockfd < 0)
        {
            client->sockfd = (int)axutil_network_handler_open_socket(env, client->proxy_host,
                client->proxy_port);
        }
    }
    else
    {
        /*Proxy is not enabled*/

        if(client->sockfd < 0)
        {
            client->sockfd = (int)axutil_network_handler_open_socket(env, host, port);
        }
    }

    if(client->sockfd < 0)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Socket Creation failed.");
        return AXIS2_FAILURE;
    }

    if(client->timeout > 0)
    {
        /*Set the receiving time out*/
        axutil_network_handler_set_sock_option(env, client->sockfd, SO_RCVTIMEO, client->timeout);
        /*Set the sending time out*/

        axutil_network_handler_set_sock_option(env, client->sockfd, SO_SNDTIMEO, client->timeout);
    }

    if(0 == axutil_strcasecmp(axutil_url_get_protocol(client->url, env), AXIS2_TRANSPORT_URL_HTTPS))
    {
#ifdef AXIS2_SSL_ENABLED
        if (client->proxy_enabled)
        {
            if (AXIS2_SUCCESS != axis2_http_client_connect_ssl_host(client, env, host, port))
            {
                axutil_network_handler_close_socket(env, client->sockfd);
                client->sockfd = -1;
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "HTTPS connection creation failed");
                return AXIS2_FAILURE;
            }
        }
		if(!client->data_stream)
			client->data_stream =
			axutil_stream_create_ssl(env, client->sockfd, axis2_http_client_get_server_cert(client,
                env), axis2_http_client_get_key_file(client, env), ssl_pp,
                    client->validate_ssl_hostname == AXIS2_TRUE ? host : NULL);
#else
        axutil_network_handler_close_socket(env, client->sockfd);
        client->sockfd = -1;
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_INVALID_TRANSPORT_PROTOCOL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Invalid Transport Protocol, HTTPS transport not enabled.");

        return AXIS2_FAILURE;
#endif
    }
    else
    {
        if(!client->data_stream)
            client->data_stream = axutil_stream_create_socket(env, client->sockfd);
    }

    if(!client->data_stream)
    {
        axutil_network_handler_close_socket(env, client->sockfd);
        client->sockfd = -1;
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Data stream creation failed for Host %s and %d port", host, port);

        return AXIS2_FAILURE;
    }

    /*Accessing HTTP headers*/

    headers = axis2_http_simple_request_get_headers(request, env);
    if(headers)
    {
        int header_count = axutil_array_list_size(headers, env);
        int i = 0;
        char *str_header2 = NULL;
        for(i = 0; i < header_count; i++)
        {
            axis2_char_t *header_ext_form = NULL;
            axis2_http_header_t *tmp_header = (axis2_http_header_t *)axutil_array_list_get(headers,
                env, i);

            if(!tmp_header)
            {
                /* This continue is added as a safey mechanism,
                 * However I see a problem with this logic, AFAIC
                 * see there can't be null headers in the headers
                 * array list, because number of headers in "headers"
                 * array list count with axutil_array_list_size,
                 * therefore this check and continue might not have a
                 * real effect.*/

                continue;
            }

            /* check whether we have transfer encoding and then see whether the
             * value is "chunked" */
            if(!axutil_strcmp(axis2_http_header_get_name(tmp_header, env),
                AXIS2_HTTP_HEADER_TRANSFER_ENCODING) && !axutil_strcmp(axis2_http_header_get_value(
                tmp_header, env), AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                chunking_enabled = AXIS2_TRUE;
            }

            header_ext_form = axis2_http_header_to_external_form(tmp_header, env);

            /* str_header2 is to hold intermediate value of str_header */
            str_header2 = axutil_stracat(env, str_header, header_ext_form);
            if(str_header)
            {
                AXIS2_FREE(env->allocator, str_header);
                str_header = NULL;
            }
            if(header_ext_form)
            {
                AXIS2_FREE(env->allocator, header_ext_form);
                header_ext_form = NULL;
            }

            /* str_header has all HTTP headers to send. */
            str_header = str_header2;
        }
    }

    if(AXIS2_FALSE == client->proxy_enabled)
    {
        str_request_line = axis2_http_request_line_to_string(
            axis2_http_simple_request_get_request_line(request, env), env);
    }
    else
    {
        /* proxy enabled case */

        /* we need the request line in the format
         * POST http://host:port/path HTTP/1.x if we have enabled proxies
         */
        axis2_char_t *host_port_str = NULL;
        axis2_char_t *host = axutil_url_get_host(client->url, env);
        axis2_http_request_line_t *request_line = axis2_http_simple_request_get_request_line(
            request, env);
        axis2_char_t *path = axis2_http_request_line_get_uri(request_line, env);

        /* Buffer size: "http://" (7) + host + ":" (1) + port (max 5) + path + null (1) + margin */
        {
            size_t host_port_size = axutil_strlen(host) + axutil_strlen(path) + 20;
            host_port_str = AXIS2_MALLOC(env->allocator, host_port_size);

            if(!host_port_str)
            {
                axutil_network_handler_close_socket(env, client->sockfd);
                client->sockfd = -1;
                AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Memory allocation failed for host %s and %s path", host, path);

                return AXIS2_FAILURE;
            }

            snprintf(host_port_str, host_port_size, "http://%s:%d%s",
                     host, axutil_url_get_port(client->url, env), path);
        }
        str_request_line = AXIS2_MALLOC(env->allocator, axutil_strlen(host_port_str) + 20);

        if(!str_request_line)
        {
            axutil_network_handler_close_socket(env, client->sockfd);
            client->sockfd = -1;
            AXIS2_FREE(env->allocator, host_port_str);
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "memory allocation failed for host %s and %s path", host, path);

            return AXIS2_FAILURE;
        }

        {
            size_t request_line_size = axutil_strlen(host_port_str) + 20;
            snprintf(str_request_line, request_line_size, "%s %s %s\r\n",
                     axis2_http_request_line_get_method(request_line, env),
                     host_port_str,
                     axis2_http_request_line_get_http_version(request_line, env));
        }

        AXIS2_FREE(env->allocator, host_port_str);
        host_port_str = NULL;

    }

    /* Here first we send the http header part */

    wire_format = axutil_stracat(env, str_request_line, str_header);
    AXIS2_FREE(env->allocator, str_header);
    str_header = NULL;
    AXIS2_FREE(env->allocator, str_request_line);
    str_request_line = NULL;
    written
        = axutil_stream_write(client->data_stream, env, wire_format, axutil_strlen(wire_format));
    AXIS2_FREE(env->allocator, wire_format);
    wire_format = NULL;

    /* Then we write the two new line charaters before the http body*/

    written = axutil_stream_write(client->data_stream, env, AXIS2_HTTP_CRLF, 2);

    /* When sending MTOM it is bit different. We keep the attachment + other
     mime headers in an array_list and send them one by one */

    if(client->doing_mtom)
    {
        /*axis2_status_t status = AXIS2_SUCCESS; */
        axutil_http_chunked_stream_t *chunked_stream = NULL;

        /* If the callback name is not there, then we will check whether there 
         * is any mime_parts which has type callback. If we found then no point 
         * of continuing we should return a failure */

        if(!(client->mtom_sending_callback_name))
        {
            if(axis2_http_transport_utils_is_callback_required(env, client->mime_parts))
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Sender callback not specified");
                return AXIS2_FAILURE;
            }
        }

        /* For MTOM we automatically enabled chunking */
        chunked_stream = axutil_http_chunked_stream_create(env, client->data_stream);

        /* This method will write the Attachment + data to the wire */

        status = axis2_http_transport_utils_send_mtom_message(chunked_stream, env,
            client->mime_parts, client->mtom_sending_callback_name);

        axutil_http_chunked_stream_free(chunked_stream, env);
        chunked_stream = NULL;

    }
    /* Non MTOM case */
    else if(client->req_body_size > 0 && client->req_body)
    {
        int len = 0;
        written = 0;

        /* Keep on writing data in a loop until we finised 
         with all the data in the buffer */

        if(!chunking_enabled)
        {
            status = AXIS2_SUCCESS;
            while(written < client->req_body_size)
            {
                len = 0;
                len = axutil_stream_write(client->data_stream, env, client->req_body + written,
                    client->req_body_size - written);
                if(-1 == len)
                {
                    status = AXIS2_FAILURE;
                    break;
                }
                else
                {
                    written += len;
                }
            }
        }
        else
        {
            /* Not MTOM but chunking is enabled */
            axutil_http_chunked_stream_t *chunked_stream = NULL;
            chunked_stream = axutil_http_chunked_stream_create(env, client->data_stream);
            status = AXIS2_SUCCESS;
            if(!chunked_stream)
            {
                axutil_network_handler_close_socket(env, client->sockfd);
                client->sockfd = -1;
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Creatoin of chunked stream failed");
                return AXIS2_FAILURE;
            }

            while(written < client->req_body_size)
            {
                written = axutil_http_chunked_stream_write(chunked_stream, env, client->req_body,
                    client->req_body_size);

                if(-1 == written)
                {
                    status = AXIS2_FAILURE;
                    break;
                }
            }

            if(AXIS2_SUCCESS == status)
            {
                /* Writing the trailing null charactor */
                axutil_http_chunked_stream_write_last_chunk(chunked_stream, env);
            }

            axutil_http_chunked_stream_free(chunked_stream, env);
        }
    }
    else
    {
        /* No body to send (e.g., GET or POST with Content-Length: 0).
         * Headers were already written successfully, so return SUCCESS.
         */
        status = AXIS2_SUCCESS;
    }

    client->request_sent = AXIS2_TRUE;
    return status;
}

/* Following is deprecated and should be removed after 1.8 - spelling of API is not correct */
AXIS2_EXTERN int AXIS2_CALL
axis2_http_client_recieve_header(
    axis2_http_client_t * client,
    const axutil_env_t * env)
{
    return axis2_http_client_receive_header(client, env);
}

/*
 * AXIS2C-1480: Buffered HTTP header reading implementation.
 *
 * Previous implementation read 1 byte at a time, causing excessive syscalls.
 * This implementation reads in chunks (4KB) and scans for CRLF in memory,
 * dramatically reducing syscall overhead.
 *
 * Algorithm:
 * 1. Read chunks into a local buffer
 * 2. Scan buffer for CRLF line endings
 * 3. Extract and process complete lines (status line, headers)
 * 4. When headers end (\r\n\r\n), any remaining data is body content
 * 5. Wrap leftover data in a prepend stream for body reading
 */
AXIS2_EXTERN int AXIS2_CALL
axis2_http_client_receive_header(
    axis2_http_client_t * client,
    const axutil_env_t * env)
{
    int status_code = -1;
    axis2_http_status_line_t *status_line = NULL;
    axis2_char_t str_line[AXIS2_HTTP_HEADER_LENGTH];  /* Buffer for current line */
    size_t str_line_len = 0;
    int http_status = 0;
    axis2_bool_t end_of_headers = AXIS2_FALSE;
    axis2_bool_t status_line_parsed = AXIS2_FALSE;

    /* AXIS2C-1480: Read buffer for efficient chunk-based reading */
    axis2_char_t read_buffer[AXIS2_HTTP_READ_BUFFER_SIZE];
    size_t buf_pos = 0;   /* Current read position in buffer */
    size_t buf_len = 0;   /* Amount of valid data in buffer */

    if(-1 == client->sockfd || !client->data_stream || AXIS2_FALSE == client->request_sent)
    {
        axis2_char_t *host;
        unsigned int port;
        host = axutil_url_get_host(client->url, env);
        port = axutil_url_get_port(client->url, env);

        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Client data stream null or socket error for host %s and %d port", host, port);
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_REQUEST_NOT_SENT, AXIS2_FAILURE);
        return -1;
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "AXIS2C-1480: Using buffered header reading (chunk size: %d)", AXIS2_HTTP_READ_BUFFER_SIZE);

    /*
     * Main parsing loop: read chunks, extract lines, process headers
     */
    while(!end_of_headers)
    {
        /* If buffer is exhausted, read more data */
        if(buf_pos >= buf_len)
        {
            int bytes_read = axutil_stream_read(client->data_stream, env,
                                                read_buffer, AXIS2_HTTP_READ_BUFFER_SIZE);

            if(bytes_read < 0)
            {
                /* AXIS2C-1568: Distinguish timeout from other errors */
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "http client, response timed out");
                AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_RESPONSE_TIMED_OUT, AXIS2_FAILURE);
                if(status_line)
                {
                    axis2_http_status_line_free(status_line, env);
                }
                return AXIS2_HTTP_CLIENT_STATUS_TIMEOUT;
            }
            else if(bytes_read == 0)
            {
                AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_RESPONSE_SERVER_SHUTDOWN, AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Response error, Server Shutdown");
                if(status_line)
                {
                    axis2_http_status_line_free(status_line, env);
                }
                return 0;
            }

            buf_pos = 0;
            buf_len = (size_t)bytes_read;
        }

        /* Scan buffer for CRLF, building current line */
        while(buf_pos < buf_len && !end_of_headers)
        {
            axis2_char_t c = read_buffer[buf_pos++];

            /* Append character to current line */
            if(str_line_len < AXIS2_HTTP_HEADER_LENGTH - 1)
            {
                str_line[str_line_len++] = c;
                str_line[str_line_len] = '\0';
            }
            else
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Header line exceeds maximum length %d", AXIS2_HTTP_HEADER_LENGTH);
                /* Continue to find line end, but truncate */
            }

            /* Check for CRLF (end of line) */
            if(str_line_len >= 2 &&
               str_line[str_line_len - 2] == '\r' &&
               str_line[str_line_len - 1] == '\n')
            {
                /* Complete line found */
                if(str_line_len == 2)
                {
                    /* Empty line (\r\n) = end of headers */
                    end_of_headers = AXIS2_TRUE;
                }
                else if(!status_line_parsed)
                {
                    /* First line is the status line */
                    status_line = axis2_http_status_line_create(env, str_line);
                    if(!status_line)
                    {
                        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "axis2_http_status_line_create failed for: %s", str_line);
                        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE, AXIS2_FAILURE);
                        http_status = 0;
                    }
                    else
                    {
                        http_status = axis2_http_status_line_get_status_code(status_line, env);
                        /* Handle 1xx informational responses - continue reading */
                        if(http_status >= AXIS2_HTTP_RESPONSE_OK_CODE_VAL)
                        {
                            status_line_parsed = AXIS2_TRUE;
                        }
                        else
                        {
                            /* 1xx response, free and continue */
                            axis2_http_status_line_free(status_line, env);
                            status_line = NULL;
                        }
                    }
                }
                else
                {
                    /* Header line */
                    axis2_http_header_t *tmp_header = axis2_http_header_create_by_str(env, str_line);
                    if(tmp_header)
                    {
                        if(!client->response)
                        {
                            client->response = axis2_http_simple_response_create_default(env);
                            if(status_line)
                            {
                                axis2_http_simple_response_set_status_line(client->response, env,
                                    axis2_http_status_line_get_http_version(status_line, env),
                                    axis2_http_status_line_get_status_code(status_line, env),
                                    axis2_http_status_line_get_reason_phrase(status_line, env));
                            }
                        }
                        axis2_http_simple_response_set_header(client->response, env, tmp_header);
                    }
                }

                /* Reset line buffer for next line */
                str_line_len = 0;
                str_line[0] = '\0';
            }
        }
    }

    /* Create response if not already created */
    if(!client->response && status_line)
    {
        client->response = axis2_http_simple_response_create_default(env);
        axis2_http_simple_response_set_status_line(client->response, env,
            axis2_http_status_line_get_http_version(status_line, env),
            axis2_http_status_line_get_status_code(status_line, env),
            axis2_http_status_line_get_reason_phrase(status_line, env));
    }

    /* Handle case where status line parsing failed - response will be NULL */
    if(!client->response)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Failed to create HTTP response - status line may be malformed");
        if(status_line)
        {
            axis2_http_status_line_free(status_line, env);
        }
        return -1;
    }

    /*
     * AXIS2C-1480: Handle leftover data (body bytes read during header parsing)
     *
     * If buf_pos < buf_len, we've read beyond the headers into the body.
     * Create a prepend stream to serve this data first before reading
     * from the underlying socket stream.
     */
    if(buf_pos < buf_len)
    {
        size_t leftover_len = buf_len - buf_pos;
        axis2_char_t *leftover_data = (axis2_char_t *)AXIS2_MALLOC(env->allocator, leftover_len);

        if(leftover_data)
        {
            memcpy(leftover_data, read_buffer + buf_pos, leftover_len);

            axutil_stream_t *body_stream = axis2_prepend_stream_create(env,
                leftover_data, leftover_len, client->data_stream);

            if(body_stream)
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "AXIS2C-1480: %zu bytes of body data read during header parsing", leftover_len);
                axis2_http_simple_response_set_body_stream(client->response, env, body_stream);
                /* Clear our reference - prepend stream now owns the underlying stream */
                client->data_stream = NULL;
            }
            else
            {
                /* Failed to create prepend stream, free leftover and fall back */
                AXIS2_FREE(env->allocator, leftover_data);
                axis2_http_simple_response_set_body_stream(client->response, env, client->data_stream);
                client->data_stream = NULL;
            }
        }
        else
        {
            /* Memory allocation failed, fall back to original behavior */
            AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI,
                "AXIS2C-1480: Failed to allocate leftover buffer, body may be incomplete");
            axis2_http_simple_response_set_body_stream(client->response, env, client->data_stream);
            client->data_stream = NULL;
        }
    }
    else
    {
        /* No leftover data, pass stream directly */
        axis2_http_simple_response_set_body_stream(client->response, env, client->data_stream);
        client->data_stream = NULL;
    }

    if(status_line)
    {
        status_code = axis2_http_status_line_get_status_code(status_line, env);
        axis2_http_status_line_free(status_line, env);
        status_line = NULL;
    }

    if(AXIS2_FALSE == axis2_http_simple_response_contains_header(client->response, env,
        AXIS2_HTTP_HEADER_CONTENT_TYPE) && 202 != status_code
        && axis2_http_simple_response_get_content_length(client->response, env) > 0)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_RESPONSE_CONTENT_TYPE_MISSING, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Response does not contain Content-Type");
        return -1;
    }

    return status_code;
}

AXIS2_EXTERN axis2_http_simple_response_t *AXIS2_CALL
axis2_http_client_get_response(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    return client->response;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_url(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axutil_url_t * url)
{
    AXIS2_PARAM_CHECK(env->error, url, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);

    if(client->url)
    {
        axutil_url_free(client->url, env);
        client->url = NULL;
    }
    client->url = url;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_url_t *AXIS2_CALL
axis2_http_client_get_url(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    AXIS2_PARAM_CHECK(env->error, client, NULL);
    return client->url;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_timeout(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    int timeout_ms)
{
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);
    client->timeout = timeout_ms;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_http_client_get_timeout(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);
    return client->timeout;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_proxy(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_char_t * proxy_host,
    int proxy_port)
{
    AXIS2_PARAM_CHECK(env->error, proxy_host, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);

    if(proxy_port <= 0)
    {
        return AXIS2_FAILURE;
    }

    client->proxy_port = proxy_port;
    if(client->proxy_host)
    {
        AXIS2_FREE(env->allocator, client->proxy_host);
        client->proxy_host = NULL;
    }

    if(client->proxy_host_port)
    {
        AXIS2_FREE(env->allocator, client->proxy_host_port);
        client->proxy_host_port = NULL;
    }

    client->proxy_host = axutil_strdup(env, proxy_host);
    if(!client->proxy_host)
    {
        return AXIS2_FAILURE;
    }

    /* Buffer size: host + ":" + max 5-digit port + null terminator
     * Max port is 65535 (5 digits), so we need host_len + 1 + 5 + 1 = host_len + 7
     * Using 10 extra bytes for safety margin */
    {
        size_t buf_size = axutil_strlen(proxy_host) + 10;
        client->proxy_host_port = AXIS2_MALLOC(env->allocator, buf_size);
        if (client->proxy_host_port)
        {
            snprintf(client->proxy_host_port, buf_size, "%s:%d", proxy_host, proxy_port);
        }
    }
    client->proxy_enabled = AXIS2_TRUE;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_client_get_proxy(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    AXIS2_PARAM_CHECK(env->error, client, NULL);
    return client->proxy_host_port;
}

/* AXIS2C-1312/1555: Set proxy authentication credentials for HTTPS CONNECT */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_proxy_auth(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    const axis2_char_t * username,
    const axis2_char_t * password)
{
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);

    if(client->proxy_auth_username)
    {
        AXIS2_FREE(env->allocator, client->proxy_auth_username);
        client->proxy_auth_username = NULL;
    }
    if(client->proxy_auth_password)
    {
        AXIS2_FREE(env->allocator, client->proxy_auth_password);
        client->proxy_auth_password = NULL;
    }

    if(username)
    {
        client->proxy_auth_username = axutil_strdup(env, username);
        if(!client->proxy_auth_username)
        {
            return AXIS2_FAILURE;
        }
    }
    if(password)
    {
        client->proxy_auth_password = axutil_strdup(env, password);
        if(!client->proxy_auth_password)
        {
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_connect_ssl_host(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_char_t * host,
    int port)
{
    axutil_stream_t *tmp_stream = NULL;
    axis2_char_t *connect_string = NULL;
    axis2_char_t str_status_line[AXIS2_HTTP_STATUS_LINE_LENGTH];
    axis2_char_t str_header[AXIS2_HTTP_HEADER_LENGTH];
    axis2_char_t tmp_buf[3];
    int read = 0;
    axis2_bool_t end_of_line = AXIS2_FALSE;
    axis2_bool_t end_of_response = AXIS2_FALSE;
    axis2_http_status_line_t *status_line = NULL;

    AXIS2_PARAM_CHECK(env->error, host, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, client->url, AXIS2_FAILURE);
    /* This host and port will use for give undersandable log messages
     * */

    if(port <= 0)
    {
        return AXIS2_FAILURE;
    }

    tmp_stream = axutil_stream_create_socket(env, client->sockfd);
    if(!tmp_stream)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Unable to create socket for SSL host %s and %d port", host, port);
        return AXIS2_FAILURE;
    }

    /* AXIS2C-1312/1555: Build CONNECT request with optional proxy authentication
     *
     * HTTP/1.0 is intentionally used here (not HTTP/1.1) for the CONNECT tunnel:
     *
     * 1. CONNECT establishes a raw TCP tunnel through the proxy - once the proxy
     *    returns "200 Connection established", all subsequent bytes are passed
     *    through verbatim as encrypted SSL/TLS traffic. HTTP semantics end here.
     *
     * 2. HTTP/1.0 is simpler and more compatible:
     *    - No Host header required (HTTP/1.1 mandates it)
     *    - No chunked transfer encoding complications
     *    - No persistent connection semantics to manage
     *    - Better compatibility with older/simpler proxy servers
     *
     * 3. HTTP/1.1 features (keep-alive, pipelining, chunked encoding) provide
     *    zero benefit for a tunnel that immediately carries encrypted traffic.
     *
     * Using HTTP/1.1 would require: "CONNECT host:port HTTP/1.1\r\nHost: host:port\r\n\r\n"
     * with no advantage over the simpler HTTP/1.0 form.
     */
    if(client->proxy_auth_username && client->proxy_auth_password)
    {
        /* Build Basic authentication header for proxy */
        axis2_char_t *credentials = NULL;
        axis2_char_t *encoded = NULL;
        size_t credentials_len;
        size_t encoded_len;

        credentials_len = axutil_strlen(client->proxy_auth_username) +
                          axutil_strlen(client->proxy_auth_password) + 2;
        credentials = AXIS2_MALLOC(env->allocator, credentials_len);
        if (credentials)
        {
            snprintf(credentials, credentials_len, "%s:%s",
                     client->proxy_auth_username, client->proxy_auth_password);
        }

        /* Base64 encode the credentials */
        encoded_len = axutil_base64_encode_len(axutil_strlen(credentials));
        encoded = AXIS2_MALLOC(env->allocator, encoded_len + 1);
        if(encoded)
        {
            axutil_base64_encode(encoded, credentials, axutil_strlen(credentials));
            encoded[encoded_len] = '\0';
        }
        AXIS2_FREE(env->allocator, credentials);

        if(encoded)
        {
            /* size_t (C89/ANSI C) is used for buffer sizes instead of int because:
             * 1. It's unsigned - buffer sizes are never negative
             * 2. It matches malloc/strlen return types - no truncation on 64-bit
             * 3. Prevents integer overflow vulnerabilities where large int values
             *    wrap negative, causing small allocations followed by buffer overflow
             */
            size_t connect_len = axutil_strlen(host) + axutil_strlen(encoded) + 100;
            connect_string = AXIS2_MALLOC(env->allocator, connect_len);
            if (connect_string)
            {
                snprintf(connect_string, connect_len,
                         "CONNECT %s:%d HTTP/1.0\r\nProxy-Authorization: Basic %s\r\n\r\n",
                         host, port, encoded);
            }
            AXIS2_FREE(env->allocator, encoded);
        }
        else
        {
            /* Fallback to no auth if encoding fails */
            size_t fallback_len = axutil_strlen(host) + 50;
            connect_string = AXIS2_MALLOC(env->allocator, fallback_len);
            if (connect_string)
            {
                snprintf(connect_string, fallback_len, "CONNECT %s:%d HTTP/1.0\r\n\r\n", host, port);
            }
        }
    }
    else
    {
        size_t connect_len = axutil_strlen(host) + 50;
        connect_string = AXIS2_MALLOC(env->allocator, connect_len);
        if (connect_string)
        {
            snprintf(connect_string, connect_len, "CONNECT %s:%d HTTP/1.0\r\n\r\n", host, port);
        }
    }
    axutil_stream_write(tmp_stream, env, connect_string, axutil_strlen(connect_string)
        * sizeof(axis2_char_t));

    memset(str_status_line, 0, AXIS2_HTTP_STATUS_LINE_LENGTH);
    size_t str_status_line_length = 0;
    end_of_line = AXIS2_FALSE;
    while((read = axutil_stream_read(tmp_stream, env, tmp_buf, 1)) > 0)
    {
        tmp_buf[read] = '\0';
        str_status_line_length += (size_t)read;
        if (str_status_line_length + 1 > AXIS2_HTTP_STATUS_LINE_LENGTH)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "reached maximum status line length %d",
                    AXIS2_HTTP_STATUS_LINE_LENGTH);
            end_of_line = AXIS2_TRUE;
            break;
        }
        strcat(str_status_line, tmp_buf);
        if(0 != strstr(str_status_line, AXIS2_HTTP_CRLF))
        {
            end_of_line = AXIS2_TRUE;
            break;
        }
    }
    if(read < 0)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_RESPONSE_TIMED_OUT, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, connect_string);
        axutil_stream_free(tmp_stream, env);
        return AXIS2_FAILURE;
    }
    status_line = axis2_http_status_line_create(env, str_status_line);
    if(!status_line)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, connect_string);
        axutil_stream_free(tmp_stream, env);
        return AXIS2_FAILURE;
    }
    /* AXIS2C-1312/1555: Handle proxy authentication response */
    {
        int status_code = axis2_http_status_line_get_status_code(status_line, env);
        if(200 != status_code)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Proxy CONNECT failed with status code %d", status_code);
            AXIS2_FREE(env->allocator, connect_string);
            axis2_http_status_line_free(status_line, env);
            axutil_stream_free(tmp_stream, env);
            /* Return 407 status code so caller can handle proxy auth */
            if(AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL == status_code)
            {
                return AXIS2_HTTP_RESPONSE_PROXY_AUTHENTICATION_REQUIRED_CODE_VAL;
            }
            return AXIS2_FAILURE;
        }
    }
    /* We need to empty the stream before we return
     */
    memset(str_header, 0, AXIS2_HTTP_HEADER_LENGTH);
    size_t str_header_length = 0;
    while(AXIS2_FALSE == end_of_response)
    {
        while((read = axutil_stream_read(tmp_stream, env, tmp_buf, 1)) > 0)
        {
            tmp_buf[read] = '\0';
            str_header_length += (size_t)read;
            if (str_header_length + 1 > AXIS2_HTTP_HEADER_LENGTH)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "reached maximum header line length %d",
                        AXIS2_HTTP_HEADER_LENGTH);
                end_of_line = AXIS2_TRUE;
                break;
            }
            strcat(str_header, tmp_buf);
            if(0 != strstr(str_header, AXIS2_HTTP_CRLF))
            {
                end_of_line = AXIS2_TRUE;
                break;
            }
        }
        if(AXIS2_TRUE == end_of_line)
        {
            if(0 == axutil_strcmp(str_header, AXIS2_HTTP_CRLF))
            {
                end_of_response = AXIS2_TRUE;
            }
            else
            {
                end_of_line = AXIS2_FALSE;
                memset(str_header, 0, AXIS2_HTTP_HEADER_LENGTH);
            }

        }
    }
    AXIS2_FREE(env->allocator, connect_string);
    axis2_http_status_line_free(status_line, env);
    axutil_stream_free(tmp_stream, env);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_dump_input_msg(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_bool_t dump_input_msg)
{
    client->dump_input_msg = dump_input_msg;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_server_cert(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_char_t * server_cert)
{
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, server_cert, AXIS2_FAILURE);
    client->server_cert = server_cert;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_client_get_server_cert(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    return client->server_cert;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_key_file(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_char_t * key_file)
{
    AXIS2_PARAM_CHECK(env->error, client, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, key_file, AXIS2_FAILURE);
    client->key_file = key_file;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_client_get_key_file(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    return client->key_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_mime_parts(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axutil_array_list_t *mime_parts)
{
    client->mime_parts = mime_parts;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_http_client_get_mime_parts(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    return client->mime_parts;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_doing_mtom(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_bool_t doing_mtom)
{
    client->doing_mtom = doing_mtom;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_client_get_doing_mtom(
    const axis2_http_client_t * client,
    const axutil_env_t * env)
{
    return client->doing_mtom;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_mtom_sending_callback_name(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_char_t *callback_name)
{
    client->mtom_sending_callback_name = callback_name;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_consume_stream(
    axis2_http_client_t * client,
    const axutil_env_t * env)
{
    /*axutil_stream_close(client->data_stream, env);*/
    axis2_char_t tmp_buffer[512];
    int read;
    
    while((read = axutil_stream_read(client->data_stream, env, tmp_buffer, 511)) == 511)
    {
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_reset(
    axis2_http_client_t * client,
    const axutil_env_t * env)
{
    if(client->req_body)
    {
        AXIS2_FREE(env->allocator, client->req_body);
        client->req_body = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_set_validate_ssl_hostname(
        axis2_http_client_t * client,
        const axutil_env_t * env,
        axis2_bool_t validate_ssl_hostname)
{
    client->validate_ssl_hostname = validate_ssl_hostname;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_client_get_validate_ssl_hostname(
        const axis2_http_client_t * client,
        const axutil_env_t * env)
{
    return client->validate_ssl_hostname;
}

