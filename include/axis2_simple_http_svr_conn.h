/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_SIMPLE_HTTP_SVR_CONN_H
#define AXIS2_SIMPLE_HTTP_SVR_CONN_H

/**
 * @ingroup axis2_core_transport_http
 * @{
 */


/**
 * @file axis2_simple_http_svr_conn.h
 * @brief Axis2 simple http server connection
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_http_simple_request.h>
#include <axis2_http_simple_response.h>
#include <axis2_http_response_writer.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_simple_http_svr_conn_ops */
    typedef struct axis2_simple_http_svr_conn_ops
                axis2_simple_http_svr_conn_ops_t;
    /** Type name for struct axis2_simple_http_svr_conn_ops_t */
    typedef struct axis2_simple_http_svr_conn axis2_simple_http_svr_conn_t;

    /**
     * Simple HTTP Server Connection ops struct
     */
    struct axis2_simple_http_svr_conn_ops
    {
        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                close)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_open)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         * @param keep_alive 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_keep_alive)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env,
                    axis2_bool_t keep_alive);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_keep_alive)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         */
        axis2_stream_t* (AXIS2_CALL *
                get_stream)(
                    const axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         */
        axis2_http_response_writer_t *(AXIS2_CALL *
                get_writer)(
                    const axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         */
        axis2_http_simple_request_t *(AXIS2_CALL *
                read_request)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         * @param response response
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                write_response)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env,
                    axis2_http_simple_response_t *response);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         * @param timeout timeout
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_rcv_timeout)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env,
                    int timeout);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         * @param timeout timeout
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_snd_timeout)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env,
                    int timeout);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_svr_ip)(
                    const axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);

        /**
         * @param svr_conn pointer to server connection struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_simple_http_svr_conn_t *svr_conn,
                    const axis2_env_t *env);
    };

    /**
    * Simple HTTP Server Connection struct
    */
    struct axis2_simple_http_svr_conn
    {
        /** Simple HTTP Server Connection related ops */
        axis2_simple_http_svr_conn_ops_t *ops;
    };


    /**
     * creates axis2_simple_http_svr_conn struct
     * @param env pointer to environment struct
     * @param sockfd sockfd
     */
    AXIS2_EXTERN axis2_simple_http_svr_conn_t *AXIS2_CALL
    axis2_simple_http_svr_conn_create(
        const axis2_env_t *env, 
        int sockfd);

/** Close.
    @sa axis2_simple_http_svr_conn_ops#close */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_CLOSE(svr_conn, env) \
                    ((svr_conn)->ops->close(svr_conn, env))

/** Is open.
    @sa axis2_simple_http_svr_conn_ops#is_open */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_IS_OPEN(svr_conn, env) \
                    ((svr_conn)->ops->is_open(svr_conn, env))

/** Keep alive.
    @sa axis2_simple_http_svr_conn_ops#set_keep_alive */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, keep_alive)\
                    ((svr_conn)->ops->set_keep_alive(svr_conn, env, keep_alive))

/** Is keep alive.
    @sa axis2_simple_http_svr_conn_ops#is_keep_alive */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_IS_KEEP_ALIVE(svr_conn, env) \
                    ((svr_conn)->ops->is_keep_alive(svr_conn, env))

/** Get stream.
    @sa axis2_simple_http_svr_conn_ops#get_stream */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_GET_STREAM(svr_conn, env) \
                    ((svr_conn)->ops->get_stream(svr_conn, env))

/** Set writer.
    @sa axis2_simple_http_svr_conn_ops#set_writer */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_WRITER(svr_conn, env, writer) \
                    ((svr_conn)->ops->set_writer(svr_conn, env, writer))

/** Read request.
    @sa axis2_simple_http_svr_conn_ops#read_request */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_READ_REQUEST(svr_conn, env) \
                    ((svr_conn)->ops->read_request(svr_conn, env))

/** Write response.
    @sa axis2_simple_http_svr_conn_ops#write_response */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env, response) \
                    ((svr_conn)->ops->write_response(svr_conn, env, response))

/** Set snd timeout.
    @sa axis2_simple_http_svr_conn_ops#set_snd_timeout */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_SND_TIMEOUT(svr_conn, env, timeout) \
                    ((svr_conn)->ops->set_snd_timeout(svr_conn, env, timeout))

/** Set rcv timeout.
    @sa axis2_simple_http_svr_conn_ops#set_rcv_timeout */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_RCV_TIMEOUT(svr_conn, env, timeout) \
                    ((svr_conn)->ops->set_rcv_timeout(svr_conn, env, timeout))

/** Get svr ip.
    @sa axis2_simple_http_svr_conn_ops#get_svr_ip */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_GET_SVR_IP(svr_conn, env) \
                    ((svr_conn)->ops->get_svr_ip(svr_conn, env))

/** Frees the sime http server connection.
    @sa axis2_simple_http_svr_conn_ops#free */
#define AXIS2_SIMPLE_HTTP_SVR_CONN_FREE(svr_conn, env) \
                    ((svr_conn)->ops->free(svr_conn, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_SIMPLE_HTTP_SVR_CONN_H */
