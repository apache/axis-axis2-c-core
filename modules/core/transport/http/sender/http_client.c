
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
#include <axis2_stream.h>
#include <axis2_string.h>
#include <axis2_network_handler.h>
#include <axis2_http_request_line.h>
#include <axis2_http_header.h>
#include <axis2_http_status_line.h>
#include <axis2_http_chunked_stream.h>
#include <platforms/axis2_platform_auto_sense.h>

#ifdef AXIS2_SSL_ENABLED
#include "ssl/ssl_stream.h"
#endif


/**
 * @brief HTTP Client struct impl
 *   Axis2 HTTP Client impl
 */

typedef struct axis2_http_client_impl
{
    axis2_http_client_t http_client;
    int sockfd;
    axis2_stream_t *data_stream;
    axis2_url_t *url;
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
}
axis2_http_client_impl_t;

#define AXIS2_INTF_TO_IMPL(http_client) \
                ((axis2_http_client_impl_t *)(http_client))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_http_client_send(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_http_simple_request_t *request,
    axis2_char_t *ssl_pp
    );

int AXIS2_CALL
axis2_http_client_recieve_header(
    axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_http_simple_response_t *AXIS2_CALL
axis2_http_client_get_response(
    const axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_client_set_url(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_url_t *url);

axis2_url_t *AXIS2_CALL
axis2_http_client_get_url(
    const axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_client_set_timeout(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    int timeout_ms);

int AXIS2_CALL
axis2_http_client_get_timeout(
    const axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_client_set_proxy(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *proxy_host,
    int proxy_port);

axis2_status_t AXIS2_CALL
axis2_http_client_set_server_cert(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *server_cert);

axis2_status_t AXIS2_CALL
axis2_http_client_set_key_file(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *key_file);


axis2_char_t *AXIS2_CALL
axis2_http_client_get_proxy(
    const axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_http_client_get_server_cert(
    const axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_http_client_get_key_file(
    const axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_client_connect_ssl_host(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *host,
    int port);

axis2_status_t AXIS2_CALL
axis2_http_client_set_dump_input_msg(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_bool_t dump_input_msg);

axis2_status_t AXIS2_CALL
axis2_http_client_free(
    axis2_http_client_t *client,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_http_client_free_void_arg(
    void *client,
    const axis2_env_t *env);

/***************************** End of function headers ************************/

AXIS2_EXTERN axis2_http_client_t *AXIS2_CALL
axis2_http_client_create(
    const axis2_env_t *env,
    axis2_url_t *url)
{
    axis2_http_client_impl_t *http_client_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    http_client_impl = (axis2_http_client_impl_t *)AXIS2_MALLOC
            (env->allocator, sizeof(axis2_http_client_impl_t));

    if (NULL == http_client_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    http_client_impl->url = url;
    http_client_impl->data_stream = NULL;
    http_client_impl->sockfd = -1;
    http_client_impl->response = NULL;
    http_client_impl->request_sent = AXIS2_FALSE;
    http_client_impl->timeout = AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT;
    http_client_impl->proxy_enabled = AXIS2_FALSE;
    http_client_impl->proxy_port = 0;
    http_client_impl->proxy_host = NULL;
    http_client_impl->proxy_host_port = NULL;
    http_client_impl->dump_input_msg = AXIS2_FALSE;
    http_client_impl->server_cert = NULL;
    http_client_impl->key_file = NULL;

    http_client_impl->http_client.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_http_client_ops_t));
    if (NULL == http_client_impl->http_client.ops)
    {
        axis2_http_client_free((axis2_http_client_t *) http_client_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    http_client_impl->http_client.ops->send = axis2_http_client_send;
    http_client_impl->http_client.ops->recieve_header =
        axis2_http_client_recieve_header;
    http_client_impl->http_client.ops->get_response =
        axis2_http_client_get_response;
    http_client_impl->http_client.ops->set_url = axis2_http_client_set_url;
    http_client_impl->http_client.ops->get_url = axis2_http_client_get_url;
    http_client_impl->http_client.ops->set_timeout =
        axis2_http_client_set_timeout;
    http_client_impl->http_client.ops->get_timeout =
        axis2_http_client_get_timeout;
    http_client_impl->http_client.ops->set_proxy =
        axis2_http_client_set_proxy;
    http_client_impl->http_client.ops->get_proxy =
        axis2_http_client_get_proxy;
    http_client_impl->http_client.ops->set_server_cert =
        axis2_http_client_set_server_cert;
    http_client_impl->http_client.ops->get_server_cert =
        axis2_http_client_get_server_cert;
    http_client_impl->http_client.ops->set_key_file =
        axis2_http_client_set_key_file;
    http_client_impl->http_client.ops->get_key_file =
        axis2_http_client_get_key_file;
    http_client_impl->http_client.ops->set_dump_input_msg = 
        axis2_http_client_set_dump_input_msg;
    http_client_impl->http_client.ops->free = axis2_http_client_free;

    return &(http_client_impl->http_client);
}


axis2_status_t AXIS2_CALL
axis2_http_client_free(
    axis2_http_client_t *client,
    const axis2_env_t *env)
{
    axis2_http_client_impl_t *http_client_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    http_client_impl = AXIS2_INTF_TO_IMPL(client);

    if (http_client_impl->url)
    {
        AXIS2_URL_FREE(http_client_impl->url, env);
        http_client_impl->url = NULL;
    }
    if (http_client_impl->response)
    {
        AXIS2_HTTP_SIMPLE_RESPONSE_FREE(http_client_impl->response, env);
        http_client_impl->response  = NULL;
    }
    if (-1 != http_client_impl->sockfd)
    {
        axis2_network_handler_close_socket(env, http_client_impl->sockfd);
        http_client_impl->sockfd = -1;
    }
    if (client->ops)
        AXIS2_FREE(env->allocator, client->ops);

    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(client));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_client_free_void_arg(
    void *client,
    const axis2_env_t *env)
{
    axis2_http_client_t *client_l = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    client_l = (axis2_http_client_t *)client;
    return axis2_http_client_free(client_l, env);
}


axis2_status_t AXIS2_CALL
axis2_http_client_send(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_http_simple_request_t *request,
    axis2_char_t *ssl_pp
    )
{
    axis2_http_client_impl_t *client_impl = NULL;
    char *wire_format = NULL;
    axis2_array_list_t *headers = NULL;
    char *str_header = NULL;
    char *str_body = NULL;
    char *str_request_line = NULL;
    int body_size = 0;
    int written = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_bool_t chunking_enabled = AXIS2_FALSE;


    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    client_impl = AXIS2_INTF_TO_IMPL(client);

    body_size = AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY_BYTES(request, env,
            &str_body);

    if(client_impl->dump_input_msg == AXIS2_TRUE)
    {
        return AXIS2_SUCCESS;
    }

    if (NULL == client_impl->url)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_URL, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if (AXIS2_TRUE == client_impl->proxy_enabled)
    {
        if (NULL == client_impl->proxy_host || client_impl->proxy_port <= 0)
        {
            return AXIS2_FAILURE;
        }
        client_impl->sockfd = axis2_network_handler_open_socket(env,
                client_impl->proxy_host, client_impl->proxy_port);
    }
    else
    {
        client_impl->sockfd = axis2_network_handler_open_socket(env,
                AXIS2_URL_GET_SERVER(client_impl->url, env),
                AXIS2_URL_GET_PORT(client_impl->url, env));
    }
    if (client_impl->sockfd < 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        if (str_body)
        {
            AXIS2_FREE(env->allocator, str_body);
        }
        return AXIS2_FAILURE;
    }
    /* ONLY FOR TESTING
     * client_impl->data_stream = axis2_stream_create_file(env, 
     *            stdout);
     */
    if (client_impl->timeout > 0)
    {
        axis2_network_handler_set_sock_option(env, client_impl->sockfd,
                SO_RCVTIMEO, client_impl->timeout);
        axis2_network_handler_set_sock_option(env, client_impl->sockfd,
                SO_SNDTIMEO, client_impl->timeout);
    }
    if (0 == AXIS2_STRCASECMP(AXIS2_URL_GET_PROTOCOL(client_impl->url, env),
            "HTTPS"))
    {
#ifdef AXIS2_SSL_ENABLED
        if (AXIS2_TRUE == client_impl->proxy_enabled)
        {
            if (AXIS2_SUCCESS != axis2_http_client_connect_ssl_host(client, env,
                    AXIS2_URL_GET_SERVER(client_impl->url, env),
                    AXIS2_URL_GET_PORT(client_impl->url, env)))
            {
                return AXIS2_FAILURE;
            }
        }
        client_impl->data_stream = axis2_stream_create_ssl(env,
                client_impl->sockfd, 
                AXIS2_HTTP_CLIENT_GET_SERVER_CERT(client, env),
                AXIS2_HTTP_CLIENT_GET_KEY_FILE(client, env),
                ssl_pp);
#else
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_TRANSPORT_PROTOCOL,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
#endif
    }
    else
    {
        client_impl->data_stream = axis2_stream_create_socket(env,
                client_impl->sockfd);
    }

    if (NULL == client_impl->data_stream)
    {
        axis2_network_handler_close_socket(env, client_impl->sockfd);
        return AXIS2_FAILURE;
    }

    headers = AXIS2_HTTP_SIMPLE_REQUEST_GET_HEADERS(request, env);
    if (headers)
    {
        int header_count = AXIS2_ARRAY_LIST_SIZE(headers, env);
        int i = 0;
        char *str_header2 = NULL;
        for (i = 0; i < header_count; i++)
        {
            axis2_char_t *header_ext_form = NULL;
            axis2_http_header_t *tmp_header = (axis2_http_header_t *)
                    AXIS2_ARRAY_LIST_GET(headers, env, i);
            if (NULL == tmp_header)
            {
                continue;
            }
            /* check whether we have transfer encoding and then see whether the
             * value is "chunked" */
            if (0 == AXIS2_STRCMP(AXIS2_HTTP_HEADER_GET_NAME(tmp_header, env),
                    AXIS2_HTTP_HEADER_TRANSFER_ENCODING) && 0 ==
                    AXIS2_STRCMP(AXIS2_HTTP_HEADER_GET_VALUE(tmp_header,
                            env), AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                chunking_enabled = AXIS2_TRUE;
            }
            header_ext_form = AXIS2_HTTP_HEADER_TO_EXTERNAL_FORM(
                        tmp_header, env);
            str_header2 = AXIS2_STRACAT(str_header, header_ext_form, env);
            AXIS2_FREE(env->allocator, str_header);
            str_header = NULL;
            AXIS2_FREE(env->allocator, header_ext_form);
            header_ext_form = NULL;
            str_header = str_header2;
        }
    }
    if (AXIS2_FALSE == client_impl->proxy_enabled)
    {
        str_request_line = AXIS2_HTTP_REQUEST_LINE_TO_STRING(
                    AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(request, env)
                    , env);
    }
    else
    {
        /* we need the request line in the format
         * POST http://host:port/path HTTP/1.x if we have enabled proxies
         */
        axis2_char_t *host_port_str = NULL;
        axis2_char_t *server = AXIS2_URL_GET_SERVER(client_impl->url, env);
        axis2_http_request_line_t *request_line =
            AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(request, env);
        axis2_char_t *path = AXIS2_HTTP_REQUEST_LINE_GET_URI(request_line, env);


        /* length = len(server) + len(:port) + len("http://") + len(path) + 1*/
        host_port_str = AXIS2_MALLOC(env->allocator, AXIS2_STRLEN(server) +
                + AXIS2_STRLEN(path) +  20 * sizeof(axis2_char_t));
        if (NULL == host_port_str)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        sprintf(host_port_str, "http://%s:%d%s", server, AXIS2_URL_GET_PORT(
                    client_impl->url, env), path);
        str_request_line = AXIS2_MALLOC(env->allocator,
                AXIS2_STRLEN(host_port_str) + 20 * sizeof(axis2_char_t));
        sprintf(str_request_line, "%s %s %s\r\n",
                AXIS2_HTTP_REQUEST_LINE_GET_METHOD(request_line, env),
                host_port_str, AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION(
                    request_line, env));
        AXIS2_FREE(env->allocator, host_port_str);
        host_port_str = NULL;

    }
    wire_format = AXIS2_STRACAT(str_request_line, str_header, env);
    AXIS2_FREE(env->allocator, str_header);
    str_header = NULL;
    AXIS2_FREE(env->allocator, str_request_line);
    str_request_line = NULL;
    written = AXIS2_STREAM_WRITE(client_impl->data_stream, env, wire_format,
            AXIS2_STRLEN(wire_format));
    AXIS2_FREE(env->allocator, wire_format);
    wire_format = NULL;
    written = AXIS2_STREAM_WRITE(client_impl->data_stream, env, AXIS2_HTTP_CRLF,
            2);
    if (body_size > 0 &&  str_body)
    {
        if (AXIS2_FALSE == chunking_enabled)
        {
            status = AXIS2_SUCCESS;
            while (written < body_size)
            {
                written = AXIS2_STREAM_WRITE(client_impl->data_stream, env,
                        str_body, body_size);
                if (-1 == written)
                {
                    status = AXIS2_FAILURE;
                    break;
                }
            }
        }
        else
        {
            axis2_http_chunked_stream_t *chunked_stream = NULL;
            chunked_stream = axis2_http_chunked_stream_create(env,
                    client_impl->data_stream);
            status = AXIS2_SUCCESS;
            if (NULL == chunked_stream)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Creatoin of chunked"
                        "stream failed");
                return AXIS2_FAILURE;
            }
            while (written < body_size)
            {
                written = AXIS2_HTTP_CHUNKED_STREAM_WRITE(chunked_stream, env,
                        str_body, body_size);
                if (-1 == written)
                {
                    status = AXIS2_FAILURE;
                    break;
                }
            }
            if (AXIS2_SUCCESS == status)
            {
                AXIS2_HTTP_CHUNKED_STREAM_WRITE_LAST_CHUNK(chunked_stream, env);
            }
            AXIS2_HTTP_CHUNKED_STREAM_FREE(chunked_stream, env);
        }
    }

    client_impl->request_sent = AXIS2_TRUE;
    if (str_body)
    {
        AXIS2_FREE(env->allocator, str_body);
        str_body = NULL;
    }
    return status;
}


int AXIS2_CALL
axis2_http_client_recieve_header(
    axis2_http_client_t *client,
    const axis2_env_t *env)
{
    int status_code = -1;
    axis2_http_client_impl_t *client_impl = NULL;
    axis2_http_status_line_t *status_line = NULL;
    axis2_char_t str_status_line[512];
    axis2_char_t tmp_buf[3];
    axis2_char_t str_header[512];
    int read = 0;
    int http_status = 0;
    axis2_bool_t end_of_line = AXIS2_FALSE;
    axis2_bool_t end_of_headers = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);

    client_impl = AXIS2_INTF_TO_IMPL(client);
    if (-1 == client_impl->sockfd || NULL == client_impl->data_stream ||
            AXIS2_FALSE == client_impl->request_sent)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_HTTP_REQUEST_NOT_SENT,
                AXIS2_FAILURE);
        return -1;
    }
    /* read the status line */
    do
    {
        memset(str_status_line, 0, 512);
        while ((read = AXIS2_STREAM_READ(client_impl->data_stream, env, tmp_buf,
                1)) > 0)
        {
            tmp_buf[read] = '\0';
            strcat(str_status_line, tmp_buf);
            if (0 != strstr(str_status_line, AXIS2_HTTP_CRLF))
            {
                end_of_line = AXIS2_TRUE;
                break;
            }
        }

        if (read < 0)
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_RESPONSE_TIMED_OUT,
                    AXIS2_FAILURE);
            return -1;
        }
		else if (read == 0)
		{
			AXIS2_ERROR_SET(env->error,
							AXIS2_ERROR_RESPONSE_SERVER_SHUTDOWN,
							AXIS2_FAILURE);
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Response error, Server Shutdown");
			return 0;
		}
        status_line = axis2_http_status_line_create(env, str_status_line);
        if (NULL == status_line)
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE,
                    AXIS2_FAILURE);
            /*return -1;*/
            http_status = 0;
            continue;

        }
        http_status = AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(status_line, env);
    }
    while (AXIS2_HTTP_RESPONSE_OK_CODE_VAL > http_status);

    client_impl->response = axis2_http_simple_response_create_default(env);
    AXIS2_HTTP_SIMPLE_RESPONSE_SET_STATUS_LINE(client_impl->response, env,
            AXIS2_HTTP_STATUS_LINE_GET_HTTP_VERSION(status_line,
                    env), AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(
                status_line, env),
            AXIS2_HTTP_STATUS_LINE_GET_REASON_PHRASE(status_line,
                    env));

    /* now read the headers */
    memset(str_header, 0, 512);
    end_of_line = AXIS2_FALSE;
    while (AXIS2_FALSE == end_of_headers)
    {
        while ((read = AXIS2_STREAM_READ(client_impl->data_stream, env, tmp_buf,
                1)) > 0)
        {
            tmp_buf[read] = '\0';
            strcat(str_header, tmp_buf);
            if (0 != strstr(str_header, AXIS2_HTTP_CRLF))
            {
                end_of_line = AXIS2_TRUE;
                break;
            }
        }
        if (AXIS2_TRUE == end_of_line)
        {
            if (0 == AXIS2_STRCMP(str_header, AXIS2_HTTP_CRLF))
            {
                end_of_headers = AXIS2_TRUE;
            }
            else
            {
                axis2_http_header_t *tmp_header =
                    axis2_http_header_create_by_str(env, str_header);
                memset(str_header, 0, 512);
                if (tmp_header)
                {
                    AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(client_impl->response,
                            env, tmp_header);
                }
            }
        }
        end_of_line = AXIS2_FALSE;
    }
    AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STREAM(client_impl->response, env,
            client_impl->data_stream);
    if (status_line)
    {
        status_code = AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(status_line, env);
        AXIS2_HTTP_STATUS_LINE_FREE(status_line, env);
        status_line = NULL;
    }
    if (AXIS2_FALSE == AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER(
                client_impl->response, env,
                AXIS2_HTTP_HEADER_CONTENT_TYPE) && 202 != status_code
            && AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_LENGTH(
                client_impl->response, env) > 0)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_RESPONSE_CONTENT_TYPE_MISSING
                , AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Response does not contain"
                " Content-Type");
        return -1;
    }
    return status_code;
}


axis2_http_simple_response_t *AXIS2_CALL
axis2_http_client_get_response(
    const axis2_http_client_t *client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(client)->response;
}

axis2_status_t AXIS2_CALL
axis2_http_client_set_url(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_url_t *url)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, url, AXIS2_FAILURE);
    if (AXIS2_INTF_TO_IMPL(client)->url)
    {
        AXIS2_URL_FREE(AXIS2_INTF_TO_IMPL(client)->url, env);
        AXIS2_INTF_TO_IMPL(client)->url = NULL;
    }
    AXIS2_INTF_TO_IMPL(client)->url = url;
    return AXIS2_SUCCESS;
}

axis2_url_t *AXIS2_CALL
axis2_http_client_get_url(
    const axis2_http_client_t *client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(client)->url;
}

axis2_status_t AXIS2_CALL
axis2_http_client_set_timeout(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    int timeout_ms)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(client)->timeout = timeout_ms;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_http_client_get_timeout(
    const axis2_http_client_t *client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    return AXIS2_INTF_TO_IMPL(client)->timeout;
}

axis2_status_t AXIS2_CALL
axis2_http_client_set_proxy(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *proxy_host,
    int proxy_port)
{
    axis2_http_client_impl_t *client_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, proxy_host, AXIS2_FAILURE);
    if (proxy_port <= 0)
    {
        return AXIS2_FAILURE;
    }
    client_impl = AXIS2_INTF_TO_IMPL(client);
    client_impl->proxy_port = proxy_port;
    if (client_impl->proxy_host)
    {
        AXIS2_FREE(env->allocator, client_impl->proxy_host);
        client_impl->proxy_host = NULL;
    }
    if (client_impl->proxy_host_port)
    {
        AXIS2_FREE(env->allocator, client_impl->proxy_host_port);
        client_impl->proxy_host_port = NULL;
    }
    client_impl->proxy_host = AXIS2_STRDUP(proxy_host, env);
    if (NULL == client_impl->proxy_host)
    {
        return AXIS2_FAILURE;
    }
    client_impl->proxy_host_port = AXIS2_MALLOC(env->allocator, AXIS2_STRLEN(
                proxy_host) + 10 * sizeof(axis2_char_t));
    sprintf(client_impl->proxy_host_port, "%s:%d", proxy_host, proxy_port);
    client_impl->proxy_enabled = AXIS2_TRUE;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_http_client_get_proxy(
    const axis2_http_client_t *client,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(client)->proxy_host_port;
}

axis2_status_t AXIS2_CALL
axis2_http_client_connect_ssl_host(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *host,
    int port)
{
    axis2_stream_t *tmp_stream = NULL;
    axis2_char_t *connect_string = NULL;
    axis2_char_t str_status_line[512];
    axis2_char_t tmp_buf[3];
    int read = 0;
    axis2_bool_t end_of_line = AXIS2_FALSE;
    axis2_bool_t end_of_response = AXIS2_FALSE;
    axis2_http_client_impl_t *client_impl = NULL;
    axis2_http_status_line_t *status_line = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, host, AXIS2_FAILURE);

    client_impl = AXIS2_INTF_TO_IMPL(client);
    if (port <= 0)
    {
        return AXIS2_FAILURE;
    }

    tmp_stream = axis2_stream_create_socket(env, client_impl->sockfd);
    if (NULL == tmp_stream)
    {
        return AXIS2_FAILURE;
    }
    connect_string = AXIS2_MALLOC(env->allocator,
            AXIS2_STRLEN(host) * sizeof(axis2_char_t) +
            30 * sizeof(axis2_char_t));
    sprintf(connect_string, "CONNECT %s:%d HTTP/1.0\r\n\r\n", host, port);
    AXIS2_STREAM_WRITE(tmp_stream, env, connect_string,
            AXIS2_STRLEN(connect_string) * sizeof(axis2_char_t));

    memset(str_status_line, 0, 512);
    while ((read = AXIS2_STREAM_READ(tmp_stream, env, tmp_buf, 1)) > 0)
    {
        tmp_buf[read] = '\0';
        strcat(str_status_line, tmp_buf);
        if (0 != strstr(str_status_line, AXIS2_HTTP_CRLF))
        {
            end_of_line = AXIS2_TRUE;
            break;
        }
    }
    if (read < 0)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_RESPONSE_TIMED_OUT,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    status_line = axis2_http_status_line_create(env, str_status_line);
    if (NULL == status_line)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if (200 != AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(status_line, env))
    {
        return AXIS2_FAILURE;
    }
    /* We need to empty the stream before we return
     */
    memset(str_status_line, 0, 512);
    while (AXIS2_FALSE == end_of_response)
    {
        while ((read = AXIS2_STREAM_READ(tmp_stream, env, tmp_buf, 1)) > 0)
        {
            tmp_buf[read] = '\0';
            strcat(str_status_line, tmp_buf);
            if (0 != strstr(str_status_line, AXIS2_HTTP_CRLF))
            {
                end_of_line = AXIS2_TRUE;
                break;
            }
        }
        if (AXIS2_TRUE == end_of_line)
        {
            if (0 == AXIS2_STRCMP(str_status_line, AXIS2_HTTP_CRLF))
            {
                end_of_response = AXIS2_TRUE;
            }
        }
    }
    AXIS2_STREAM_FREE(tmp_stream, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_client_set_dump_input_msg(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_bool_t dump_input_msg)
{
    axis2_http_client_impl_t *client_impl = NULL;
    
    client_impl = AXIS2_INTF_TO_IMPL(client);

    client_impl->dump_input_msg = dump_input_msg;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_client_set_server_cert(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *server_cert)
{
   axis2_http_client_impl_t *client_impl = NULL;
    
   client_impl = AXIS2_INTF_TO_IMPL(client);

   client_impl->server_cert = server_cert;

   return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_http_client_get_server_cert(
    const axis2_http_client_t *client,
    const axis2_env_t *env)
{
   AXIS2_ENV_CHECK(env, NULL);
   return AXIS2_INTF_TO_IMPL(client)->server_cert;
}

axis2_status_t AXIS2_CALL
axis2_http_client_set_key_file(
    axis2_http_client_t *client,
    const axis2_env_t *env,
    axis2_char_t *key_file)
{
   axis2_http_client_impl_t *client_impl = NULL;
    
   client_impl = AXIS2_INTF_TO_IMPL(client);

   client_impl->key_file = key_file;

   return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_http_client_get_key_file(
    const axis2_http_client_t *client,
    const axis2_env_t *env)
{
   AXIS2_ENV_CHECK(env, NULL);
   return AXIS2_INTF_TO_IMPL(client)->key_file;
}
