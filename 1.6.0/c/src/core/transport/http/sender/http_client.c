
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
    int req_body_size;
    
    /* These are for mtom case */
    axutil_array_list_t *mime_parts;
    axis2_bool_t doing_mtom;
    axis2_char_t *mtom_sending_callback_name;
};

AXIS2_EXTERN axis2_http_client_t *AXIS2_CALL
axis2_http_client_create(
    const axutil_env_t * env,
    axutil_url_t * url)
{
    axis2_http_client_t *http_client = NULL;

    http_client = (axis2_http_client_t *) AXIS2_MALLOC
        (env->allocator, sizeof(axis2_http_client_t));

    if (!http_client)
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

    return http_client;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_client_free(
    axis2_http_client_t * http_client,
    const axutil_env_t * env)
{
    if (http_client->proxy_host)
    {
        AXIS2_FREE(env->allocator, http_client->proxy_host);
    }
    if (http_client->proxy_host_port)
    {
        AXIS2_FREE(env->allocator, http_client->proxy_host_port);
    }
    if (http_client->url)
    {
        axutil_url_free(http_client->url, env);
    }
    if (http_client->response)
    {
        axis2_http_simple_response_free(http_client->response, env);
    }
    if (-1 != http_client->sockfd)
    {
        axutil_network_handler_close_socket(env, http_client->sockfd);
        http_client->sockfd = -1;
    }

    if (http_client->req_body)
    {
        AXIS2_FREE(env->allocator, http_client->req_body);
    }
    
    /* There is no other appropriate place to free the mime_part list when a 
     * particular client send requests. */
    
    if (http_client->mime_parts)
    {
        int i = 0;
        for (i = 0; i < axutil_array_list_size(http_client->mime_parts, env); i++)
        {
            axiom_mime_part_t *mime_part = NULL;
            mime_part = (axiom_mime_part_t *)
                axutil_array_list_get(http_client->mime_parts, env, i);
            if (mime_part)
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

    client_l = (axis2_http_client_t *) client;
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

    if (!client->req_body && !(client->doing_mtom))
    {
        client->req_body_size = axis2_http_simple_request_get_body_bytes(request, env, 
                &client->req_body);
    }

    if (client->dump_input_msg == AXIS2_TRUE)
    {
        return AXIS2_SUCCESS;
    }

    if (!client->url)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_URL, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Request url not set");
        return AXIS2_FAILURE;
    }

    host = axutil_url_get_host(client->url, env);
    port = axutil_url_get_port(client->url, env);


    if (client->proxy_enabled)
    {
        if (!client->proxy_host || client->proxy_port <= 0)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Proxy port or Host not set");
            return AXIS2_FAILURE;
        }

        client->sockfd = (int)axutil_network_handler_open_socket(env, client->proxy_host, 
            client->proxy_port);
    }
    else
    {
        /*Proxy is not enabled*/

        client->sockfd = (int)axutil_network_handler_open_socket(env, host, port);
    }

    if (client->sockfd < 0)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_SOCKET_ERROR, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Socket Creation failed.");
        return AXIS2_FAILURE;
    }

    if (client->timeout > 0)
    {
        /*Set the receiving time out*/
        axutil_network_handler_set_sock_option(env, client->sockfd, SO_RCVTIMEO, client->timeout);
        /*Set the sending time out*/

        axutil_network_handler_set_sock_option(env, client->sockfd, SO_SNDTIMEO, client->timeout);
    }

    if (0 == axutil_strcasecmp(axutil_url_get_protocol(client->url, env), AXIS2_TRANSPORT_URL_HTTPS))
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
        client->data_stream =
            axutil_stream_create_ssl(env, client->sockfd, axis2_http_client_get_server_cert(client, 
                        env), axis2_http_client_get_key_file(client, env), ssl_pp);
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
        client->data_stream = axutil_stream_create_socket(env, client->sockfd);
    }

    if (!client->data_stream)
    {
        axutil_network_handler_close_socket(env, client->sockfd);
        client->sockfd = -1;
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Data stream creation failed for Host %s and %d port", host, port);

        return AXIS2_FAILURE;
    }

    /*Accessing HTTP headers*/

    headers = axis2_http_simple_request_get_headers(request, env);
    if (headers)
    {
        int header_count = axutil_array_list_size(headers, env);
        int i = 0;
        char *str_header2 = NULL;
        for (i = 0; i < header_count; i++)
        {
            axis2_char_t *header_ext_form = NULL;
            axis2_http_header_t *tmp_header = (axis2_http_header_t *) axutil_array_list_get(headers, 
                    env, i);

            if (!tmp_header)
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
            if (!axutil_strcmp(axis2_http_header_get_name(tmp_header, env), 
                    AXIS2_HTTP_HEADER_TRANSFER_ENCODING) && 
                    !axutil_strcmp(axis2_http_header_get_value(tmp_header, env), 
                        AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                chunking_enabled = AXIS2_TRUE;
            }

            header_ext_form = axis2_http_header_to_external_form(tmp_header, env);

            /* str_header2 is to hold intermediate value of str_header */
            str_header2 = axutil_stracat(env, str_header, header_ext_form);
            AXIS2_FREE(env->allocator, str_header);
            str_header = NULL;
            AXIS2_FREE(env->allocator, header_ext_form);
            header_ext_form = NULL;

            /* str_header has all HTTP headers to send. */
            str_header = str_header2;
        }
    }

    if (AXIS2_FALSE == client->proxy_enabled)
    {
        str_request_line = axis2_http_request_line_to_string
            (axis2_http_simple_request_get_request_line(request, env), env);
    }
    else
    {
        /* proxy enabled case */

        /* we need the request line in the format
         * POST http://host:port/path HTTP/1.x if we have enabled proxies
         */
        axis2_char_t *host_port_str = NULL;
        axis2_char_t *host = axutil_url_get_host(client->url, env);
        axis2_http_request_line_t *request_line =
            axis2_http_simple_request_get_request_line(request, env);
        axis2_char_t *path = axis2_http_request_line_get_uri(request_line, env);

        host_port_str = AXIS2_MALLOC(env->allocator, axutil_strlen(host) + axutil_strlen(path) +
            20 * sizeof(axis2_char_t));

        if (!host_port_str)
        {
            axutil_network_handler_close_socket(env, client->sockfd);
            client->sockfd = -1;
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "Memory allocation failed for host %s and %s path", host, path);

            return AXIS2_FAILURE;
        }

        sprintf(host_port_str, "http://%s:%d%s", host, axutil_url_get_port(client->url, env), path);
        str_request_line = AXIS2_MALLOC(env->allocator, axutil_strlen(host_port_str) +
                         20 * sizeof(axis2_char_t));

        if (!str_request_line)
        {
            axutil_network_handler_close_socket(env, client->sockfd);
            client->sockfd = -1;
            AXIS2_FREE(env->allocator, host_port_str);
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                    "memory allocation failed for host %s and %s path", host, path);

            return AXIS2_FAILURE;
        }

        sprintf(str_request_line, "%s %s %s\r\n", axis2_http_request_line_get_method(request_line, 
            env), host_port_str, axis2_http_request_line_get_http_version(request_line, env));

        AXIS2_FREE(env->allocator, host_port_str);
        host_port_str = NULL;

    }
    
    /* Here first we send the http header part */ 

    wire_format = axutil_stracat(env, str_request_line, str_header);
    AXIS2_FREE(env->allocator, str_header);
    str_header = NULL;
    AXIS2_FREE(env->allocator, str_request_line);
    str_request_line = NULL;
    written = axutil_stream_write(client->data_stream, env, wire_format, axutil_strlen(wire_format));
    AXIS2_FREE(env->allocator, wire_format);
    wire_format = NULL;

    /* Then we write the two new line charaters before the http body*/

    written = axutil_stream_write(client->data_stream, env, AXIS2_HTTP_CRLF, 2);
    
    /* When sending MTOM it is bit different. We keep the attachment + other
       mime headers in an array_list and send them one by one */

    if(client->doing_mtom)
    {
        axis2_status_t status = AXIS2_SUCCESS;
        axutil_http_chunked_stream_t *chunked_stream = NULL;
        
        /* If the callback name is not there, then we will check whether there 
         * is any mime_parts which has type callback. If we found then no point 
         * of continuing we should return a failure */
   
        if(!(client->mtom_sending_callback_name))
        {
            if(axis2_http_transport_utils_is_callback_required(
                env, client->mime_parts))
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Sender callback not specified");
                return AXIS2_FAILURE;
            }
        }

        /* For MTOM we automatically enabled chunking */
        chunked_stream = axutil_http_chunked_stream_create(env,
                client->data_stream);


        /* This method will write the Attachment + data to the wire */

        status = axis2_http_transport_utils_send_mtom_message(chunked_stream, env, 
                client->mime_parts, client->mtom_sending_callback_name);      

        axutil_http_chunked_stream_free(chunked_stream, env);
        chunked_stream = NULL;
          
    }
    /* Non MTOM case */
    else if (client->req_body_size > 0 && client->req_body)
    {
        int len = 0;
        written = 0;

        /* Keep on writing data in a loop until we finised 
           with all the data in the buffer */

        if (!chunking_enabled)
        {
            status = AXIS2_SUCCESS;
            while (written < client->req_body_size)
            {
                len = 0;
                len = axutil_stream_write(client->data_stream, env,
                                              client->req_body + written,
                                              client->req_body_size - written);
                if (-1 == len)
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
            if (!chunked_stream)
            {
                axutil_network_handler_close_socket(env, client->sockfd);
                client->sockfd = -1;
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Creatoin of chunked stream failed");
                return AXIS2_FAILURE;
            }

            while (written < client->req_body_size)
            {
                written = axutil_http_chunked_stream_write(chunked_stream, env, client->req_body, 
                        client->req_body_size);

                if (-1 == written)
                {
                    status = AXIS2_FAILURE;
                    break;
                }
            }

            if (AXIS2_SUCCESS == status)
            {
                /* Writing the trailing null charactor */
                axutil_http_chunked_stream_write_last_chunk(chunked_stream, env);
            }

            axutil_http_chunked_stream_free(chunked_stream, env);
        }
    }

    client->request_sent = AXIS2_TRUE;
    return status;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_http_client_recieve_header(
    axis2_http_client_t * client,
    const axutil_env_t * env)
{
    int status_code = -1;
    axis2_http_status_line_t *status_line = NULL;
    axis2_char_t str_status_line[512];
    axis2_char_t tmp_buf[3];
    axis2_char_t str_header[512];
    int read = 0;
    int http_status = 0;
    axis2_bool_t end_of_line = AXIS2_FALSE;
    axis2_bool_t end_of_headers = AXIS2_FALSE;


    if (-1 == client->sockfd || !client->data_stream ||
        AXIS2_FALSE == client->request_sent)
    {
        axis2_char_t *host;
        unsigned int port;
        host = axutil_url_get_host(client->url, env);
        port = axutil_url_get_port(client->url, env);

        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "client data stream  null or socket error for host \
%s and %d port", host, port);
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_HTTP_REQUEST_NOT_SENT,
                        AXIS2_FAILURE);
        return -1;
    }

    /* read the status line */
    do
    {
        memset(str_status_line, 0, 512);
        while ((read = axutil_stream_read(client->data_stream, env, tmp_buf,
                                          1)) > 0)
        {
            /* "read" variable is number of characters read by stream */
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
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                            "http client , response timed out");
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_RESPONSE_TIMED_OUT,
                            AXIS2_FAILURE);
            return -1;
        }
        else if (read == 0)
        {
            AXIS2_HANDLE_ERROR(env,
                            AXIS2_ERROR_RESPONSE_SERVER_SHUTDOWN,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Response error, Server Shutdown");
            return 0;
        }

        status_line = axis2_http_status_line_create(env, str_status_line);
        if (!status_line)
        {
            AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                             "axis2_http_status_line_create failed for \
str_status_line %s", str_status_line);
            AXIS2_HANDLE_ERROR(env,
                            AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE,
                            AXIS2_FAILURE);
            http_status = 0;
            continue;

        }
        http_status = axis2_http_status_line_get_status_code(status_line, env);

    }while (AXIS2_HTTP_RESPONSE_OK_CODE_VAL > http_status);

    if (client->response)
        axis2_http_simple_response_free(client->response, env);
    client->response = axis2_http_simple_response_create_default(env);
    axis2_http_simple_response_set_status_line(
        client->response, env,
        axis2_http_status_line_get_http_version (status_line, env),
        axis2_http_status_line_get_status_code (status_line, env),
        axis2_http_status_line_get_reason_phrase (status_line, env));

    /* now read the headers */
    memset(str_header, 0, 512);
    end_of_line = AXIS2_FALSE;
    while (AXIS2_FALSE == end_of_headers)
    {
        while ((read = axutil_stream_read(client->data_stream, env, tmp_buf,
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
            if (0 == axutil_strcmp(str_header, AXIS2_HTTP_CRLF))
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
                    axis2_http_simple_response_set_header(client->response,
                                                          env, tmp_header);
                }
            }
        }
        end_of_line = AXIS2_FALSE;
    }
    axis2_http_simple_response_set_body_stream(client->response, env,
                                               client->data_stream);
    if (status_line)
    {
        status_code = axis2_http_status_line_get_status_code(status_line, env);
        axis2_http_status_line_free(status_line, env);
        status_line = NULL;
    }

    if (AXIS2_FALSE ==
        axis2_http_simple_response_contains_header(
            client->response, env,
            AXIS2_HTTP_HEADER_CONTENT_TYPE)
        && 202 != status_code &&
        axis2_http_simple_response_get_content_length(client->response,
                                                      env) > 0)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_RESPONSE_CONTENT_TYPE_MISSING,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Response does not contain" " Content-Type");
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

    if (client->url)
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

    if (proxy_port <= 0)
    {
        return AXIS2_FAILURE;
    }

    client->proxy_port = proxy_port;
    if (client->proxy_host)
    {
        AXIS2_FREE(env->allocator, client->proxy_host);
        client->proxy_host = NULL;
    }

    if (client->proxy_host_port)
    {
        AXIS2_FREE(env->allocator, client->proxy_host_port);
        client->proxy_host_port = NULL;
    }

    client->proxy_host = axutil_strdup(env, proxy_host);
    if (!client->proxy_host)
    {
        return AXIS2_FAILURE;
    }

    client->proxy_host_port =
        AXIS2_MALLOC(env->allocator,
                     axutil_strlen(proxy_host) + 10 * sizeof(axis2_char_t));

    sprintf(client->proxy_host_port, "%s:%d", proxy_host, proxy_port);
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

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_client_connect_ssl_host(
    axis2_http_client_t * client,
    const axutil_env_t * env,
    axis2_char_t * host,
    int port)
{
    axutil_stream_t *tmp_stream = NULL;
    axis2_char_t *connect_string = NULL;
    axis2_char_t str_status_line[512];
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
    
    if (port <= 0)
    {
        return AXIS2_FAILURE;
    }

    tmp_stream = axutil_stream_create_socket(env, client->sockfd);
    if (!tmp_stream)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                         "unable to create open socket for ssl host %s and %d \
port", host, port);
        return AXIS2_FAILURE;
    }

    connect_string = AXIS2_MALLOC(env->allocator,
                                  axutil_strlen(host) * sizeof(axis2_char_t) +
                                  30 * sizeof(axis2_char_t));
    sprintf(connect_string, "CONNECT %s:%d HTTP/1.0\r\n\r\n", host, port);
    axutil_stream_write(tmp_stream, env, connect_string,
                        axutil_strlen(connect_string) * sizeof(axis2_char_t));

    memset(str_status_line, 0, 512);
    while ((read = axutil_stream_read(tmp_stream, env, tmp_buf, 1)) > 0)
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
        AXIS2_HANDLE_ERROR(env,
                        AXIS2_ERROR_RESPONSE_TIMED_OUT, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, connect_string);
        axutil_stream_free(tmp_stream, env);
        return AXIS2_FAILURE;
    }
    status_line = axis2_http_status_line_create(env, str_status_line);
    if (!status_line)
    {
        AXIS2_HANDLE_ERROR(env,
                        AXIS2_ERROR_INVALID_HTTP_HEADER_START_LINE,
                        AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, connect_string);
        axutil_stream_free(tmp_stream, env);
        return AXIS2_FAILURE;
    }
    if (200 != axis2_http_status_line_get_status_code(status_line, env))
    {
        AXIS2_FREE(env->allocator, connect_string);
        axutil_stream_free(tmp_stream, env);
        return AXIS2_FAILURE;
    }
    /* We need to empty the stream before we return
     */
    memset(str_status_line, 0, 512);
    while (AXIS2_FALSE == end_of_response)
    {
        while ((read = axutil_stream_read(tmp_stream, env, tmp_buf, 1)) > 0)
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
            if (0 == axutil_strcmp(str_status_line, AXIS2_HTTP_CRLF))
            {
                end_of_response = AXIS2_TRUE;
            }
        }
    }
    AXIS2_FREE(env->allocator, connect_string);
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
    client->mtom_sending_callback_name = 
        callback_name;
    return AXIS2_SUCCESS;
}
