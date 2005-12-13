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
 * @file axis2_simple_http_svr_conn.h
 * @brief Axis2 simple http server connection
 */

#include <axis2.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_http_simple_request.h>
#include <axis2_http_simple_response.h>
#include <axis2_http_response_writer.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_simple_http_svr_conn_ops 
                    axis2_simple_http_svr_conn_ops_t;
    typedef struct axis2_simple_http_svr_conn axis2_simple_http_svr_conn_t;
    
/**
 * @ingroup axis2_core_transport_http
 * @{
 */

/**
 *   \brief Simple HTTP Server Connection operations struct
 */
AXIS2_DECLARE_DATA struct axis2_simple_http_svr_conn_ops
{        
        axis2_status_t (AXIS2_CALL *close) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        axis2_bool_t (AXIS2_CALL *is_open) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *set_keep_alive) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env, axis2_bool_t keep_alive);
        axis2_bool_t (AXIS2_CALL *is_keep_alive)
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        axis2_stream_t* (AXIS2_CALL *get_stream) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        axis2_http_response_writer_t* (AXIS2_CALL *get_writer)
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        axis2_http_simple_request_t* (AXIS2_CALL *read_request)
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *write_response)
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env, 
                        const axis2_http_simple_response_t *response);
        int (AXIS2_CALL *get_snd_timeout) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        int (AXIS2_CALL *get_rcv_timeout) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *set_rcv_timeout)
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env, int timeout);
        axis2_status_t (AXIS2_CALL *set_snd_timeout) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env, int timeout);
        axis2_status_t (AXIS2_CALL *free) 
                        (axis2_simple_http_svr_conn_t *svr_conn, 
                        axis2_env_t **env);
 };
    
/** 
* \brief Simple HTTP Server Connection struct
*/
AXIS2_DECLARE_DATA struct axis2_simple_http_svr_conn
{
    /** Simple HTTP Server Connection related operations */
    axis2_simple_http_svr_conn_ops_t *ops;
};


/**
 * creates axis2_simple_http_svr_conn struct
 */
AXIS2_DECLARE(axis2_simple_http_svr_conn_t*) AXIS2_CALL
axis2_simple_http_svr_conn_create(axis2_env_t **env, int sockfd);
  
#define AXIS2_SIMPLE_HTTP_SVR_CONN_CLOSE(svr_conn, env) \
                    ((svr_conn)->ops->close(svr_conn, env))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_IS_OPEN(svr_conn, env) \
                    ((svr_conn)->ops->is_open(svr_conn, env))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, keep_alive)\
                    ((svr_conn)->ops->set_keep_alive(svr_conn, env, keep_alive))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_IS_KEEP_ALIVE(svr_conn, env) \
                    ((svr_conn)->ops->is_keep_alive(svr_conn, env))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_GET_STREAM(svr_conn, env) \
                    ((svr_conn)->ops->get_stream(svr_conn, env))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_WRITER(svr_conn, env, writer) \
                    ((svr_conn)->ops->set_writer(svr_conn, env, writer))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_READ_REQUEST(svr_conn, env) \
                    ((svr_conn)->ops->is_open(svr_conn, env))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env, response) \
                    ((svr_conn)->ops->write_response(svr_conn, env, response))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_GET_SND_TIMEOUT(svr_conn, env)\
                    ((svr_conn)->ops->get_snd_timeout(svr_conn, env))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_GET_RCV_TIMEOUT(svr_conn, env) \
                    ((svr_conn)->ops->get_rcv_timeout(svr_conn, env))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_SND_TIMEOUT(svr_conn, env, timeout) \
                    ((svr_conn)->ops->set_snd_timeout(svr_conn, env, timeout))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_SET_RCV_TIMEOUT(svr_conn, env, timeout) \
                    ((svr_conn)->ops->set_rcv_timeout(svr_conn, env, timeout))
#define AXIS2_SIMPLE_HTTP_SVR_CONN_FREE(svr_conn, env) \
                    ((svr_conn)->ops->is_open(svr_conn, env))
    
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_SIMPLE_HTTP_SVR_CONN_H */
