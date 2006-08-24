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

#ifndef AXIS2_HTTP_WORKER_H
#define AXIS2_HTTP_WORKER_H

/**
 * @defgroup axis2_http_worker http worker
 * @ingroup axis2_core_trans_http
 * @{
 */

/**
 * @file axis2_http_worker.h
 * @brief axis2 HTTP Worker
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_simple_http_svr_conn.h>
#include <axis2_http_simple_response.h>
#include <axis2_http_simple_request.h>
#include <axis2_conf_ctx.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_http_worker_ops */
    typedef struct axis2_http_worker_ops axis2_http_worker_ops_t;
    /** Type name for struct axis2_http_worker */
    typedef struct axis2_http_worker axis2_http_worker_t;

    /**
     * HTTP Worker ops struct
     * Encapsulator struct for ops of axis2_http_worker
     */
    struct axis2_http_worker_ops
    {
        /**
         * @param http_worker pointer to http worker
         * @param env pointer to environment struct
         * @param svr_conn pointer to svr conn
         * @param simple_request pointer to simple request
         */
        axis2_bool_t (AXIS2_CALL *
                process_request)(
                    axis2_http_worker_t *http_worker,
                    const axis2_env_t *env,
                    axis2_simple_http_svr_conn_t *svr_conn,
                    axis2_http_simple_request_t *simple_request);

        /**
         * @param http_worker pointer to http worker
         * @param env pointer to environment struct
         * @param port
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_svr_port)(
                    axis2_http_worker_t *http_worker,
                    const axis2_env_t *env,
                    int port);

        /**
         * @param http_worker pointer to http worker
         * @param env pointer to environment strut
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_http_worker_t *http_worker,
                    const axis2_env_t *env);
    };

    /**
     * axis2 http worker
     */
    struct axis2_http_worker
    {
        /** operations of axis2 http worker */
        axis2_http_worker_ops_t *ops;
    };


    /**
     * @param env pointer to environment struct
     * @param conf_ctx pointer to configuration context
     */
    AXIS2_EXTERN axis2_http_worker_t * AXIS2_CALL
    axis2_http_worker_create (
        const axis2_env_t *env,
        axis2_conf_ctx_t *conf_ctx);

/************************** Start of function macros **************************/

/** Process the request.
    @sa axis2_http_worker_ops#process_request */
#define AXIS2_HTTP_WORKER_PROCESS_REQUEST(http_worker, env, svr_conn,\
            simple_request) ((http_worker)->ops->process_request(\
            http_worker, env, svr_conn, simple_request))

/** Sets the server port.
    @sa axis2_http_worker_ops#set_svr_port */
#define AXIS2_HTTP_WORKER_SET_SVR_PORT(http_worker, env, port) \
                ((http_worker)->ops->set_svr_port(http_worker, env, port))

/** Frees the http worker.
    @sa axis2_http_worker_ops#free */
#define AXIS2_HTTP_WORKER_FREE(http_worker, env) \
                ((http_worker)->ops->free(http_worker, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_WORKER_H */
