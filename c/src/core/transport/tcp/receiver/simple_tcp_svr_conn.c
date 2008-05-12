
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

#include <axis2_simple_tcp_svr_conn.h>
#include <axis2_tcp_transport.h>
#include <axutil_string.h>
#include <axutil_network_handler.h>
#include <platforms/axutil_platform_auto_sense.h>

struct axis2_simple_tcp_svr_conn
{
    int socket;
    axutil_stream_t *stream;
    axis2_char_t *buffer;
};

AXIS2_EXTERN axis2_simple_tcp_svr_conn_t *AXIS2_CALL
axis2_simple_tcp_svr_conn_create(
    const axutil_env_t * env,
    int sockfd)
{
    axis2_simple_tcp_svr_conn_t *svr_conn = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    svr_conn = (axis2_simple_tcp_svr_conn_t *)
        AXIS2_MALLOC(env->allocator, sizeof(axis2_simple_tcp_svr_conn_t));

    if (!svr_conn)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    svr_conn->socket = sockfd;
    svr_conn->stream = NULL;
    svr_conn->buffer = NULL;

    if (-1 != svr_conn->socket)
    {
        svr_conn->stream = axutil_stream_create_socket(env, svr_conn->socket);
        if (!svr_conn->stream)
        {
            axis2_simple_tcp_svr_conn_free((axis2_simple_tcp_svr_conn_t *)
                                           svr_conn, env);
            return NULL;
        }
    }
    return svr_conn;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_simple_tcp_svr_conn_free(
    axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env)
{
    axis2_simple_tcp_svr_conn_close(svr_conn, env);

    AXIS2_FREE(env->allocator, svr_conn);

    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_simple_tcp_svr_conn_close(
    axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    axutil_stream_free(svr_conn->stream, env);
    if (-1 != svr_conn->socket)
    {
        axutil_network_handler_close_socket(env, svr_conn->socket);
        svr_conn->socket = -1;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_simple_tcp_svr_conn_is_open(
    axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (-1 != svr_conn->socket)
    {
        return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}

AXIS2_EXTERN axutil_stream_t *AXIS2_CALL
axis2_simple_tcp_svr_conn_get_stream(
    const axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env)
{
    return svr_conn->stream;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_simple_tcp_svr_conn_read_request(
    axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env)
{
    int size = 32000;
    axis2_char_t str_line[32000];
    axis2_char_t tmp_buf[32000];
    int read = -1;

    AXIS2_ENV_CHECK(env, NULL);

    memset(str_line, 0, size);
    while ((read =
            axutil_stream_peek_socket(svr_conn->stream, env, tmp_buf,
                                      size - 1)) > 0)
    {
        tmp_buf[read] = '\0';
        if (read > 0)
        {
            read = axutil_stream_read(svr_conn->stream, env, tmp_buf, size - 1);
            if (read > 0)
            {
                tmp_buf[read] = '\0';
                strcat(str_line, tmp_buf);
                break;
            }
            else
            {
                break;
            }
        }
    }
    if (str_line > 0)
    {
        svr_conn->buffer = str_line;
    }
    return svr_conn->buffer;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_simple_tcp_svr_conn_set_rcv_timeout(
    axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env,
    int timeout)
{
    return axutil_network_handler_set_sock_option(env,
                                                  svr_conn->socket, SO_RCVTIMEO,
                                                  timeout);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_simple_tcp_svr_conn_set_snd_timeout(
    axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env,
    int timeout)
{
    return axutil_network_handler_set_sock_option(env,
                                                  svr_conn->socket, SO_SNDTIMEO,
                                                  timeout);
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_simple_tcp_svr_conn_get_svr_ip(
    const axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env)
{
    return axutil_network_handler_get_svr_ip(env, svr_conn->socket);
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_simple_tcp_svr_conn_get_peer_ip(
    const axis2_simple_tcp_svr_conn_t * svr_conn,
    const axutil_env_t * env)
{
    return axutil_network_handler_get_peer_ip(env, svr_conn->socket);
}
