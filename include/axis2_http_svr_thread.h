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

#ifndef AXIS2_HTTP_SVR_THREAD_H
#define AXIS2_HTTP_SVR_THREAD_H

/**
 * @defgroup axis2_http_svr_thread http server thread 
 * @ingroup axis2_core_trans_http
 * @{
 */

/**
 * @file axis2_http_svr_thread.h
 * @brief axis2 HTTP server listning thread implementation
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_http_worker.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axist_http_svr_thread_ops */
    typedef struct axis2_http_svr_thread_ops axis2_http_svr_thread_ops_t;
    /** Type name for struct axist_http_svr_thread */
    typedef struct axis2_http_svr_thread axis2_http_svr_thread_t;

    extern int axis2_http_socket_read_timeout;

    /**
     * HTTP Server Thread ops struct
     * Encapsulator struct for ops of axis2_http_svr_thread
     */
    struct axis2_http_svr_thread_ops
    {
        /**
         * @param svr_thread pointer to server thread
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                run)(
                    axis2_http_svr_thread_t *svr_thread,
                    const axis2_env_t *env);

        /**
         * @param svr_thread pointer to server thread
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                destroy)(
                    axis2_http_svr_thread_t *svr_thread,
                    const axis2_env_t *env);

        /**
         * @param svr_thread pointer to server thread
         * @param env pointer to environment struct
         */
        int (AXIS2_CALL *
                get_local_port)(
                    const axis2_http_svr_thread_t *svr_thread,
                    const axis2_env_t *env);

        /**
         * @param svr_thread pointer to server thread
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_running)(
                    axis2_http_svr_thread_t *svr_thread,
                    const axis2_env_t *env);

        /**
         * @param svr_thread pointer to server thread
         * @param env pointer to environment struct
         * @param worker pointer to worker
         */
        axis2_status_t (AXIS2_CALL *
                set_worker)(
                    axis2_http_svr_thread_t *svr_thread,
                    const axis2_env_t *env,
                    axis2_http_worker_t *worker);

        /**
         * @param svr_thread pointer to server thread
         * @param env pointer to environment struct
         */
        void (AXIS2_CALL *
                free)(
                    axis2_http_svr_thread_t *svr_thread,
                    const axis2_env_t *env);
    };

    /**
     * axis2 http server thread
     */
    struct axis2_http_svr_thread
    {
        /** operations of axis2 http server thread */
        axis2_http_svr_thread_ops_t *ops;
    };

    /**
     * @param env pointer to environment struct
     * @param port
     */
    AXIS2_EXTERN axis2_http_svr_thread_t *AXIS2_CALL
    axis2_http_svr_thread_create (
        const axis2_env_t *env,
        int port);

/************************** Start of function macros **************************/

/** Run.
    @sa axis2_http_svr_thread_ops#run */
#define AXIS2_HTTP_SVR_THREAD_RUN(svr_thread, env) \
                  ((svr_thread)->ops->run(svr_thread, env))

/** Destroy.
    @sa axis2_http_svr_thread_ops#destroy */
#define AXIS2_HTTP_SVR_THREAD_DESTROY(svr_thread, env) \
                        ((svr_thread)->ops->destroy(svr_thread, env))

/** Gets the local port.
    @sa axis2_http_svr_thread_ops#get_local_port */
#define AXIS2_HTTP_SVR_THREAD_GET_LOCAL_PORT(svr_thread, env) \
                        ((svr_thread)->ops->get_local_port(svr_thread,\
                  env))

/** Is running.
    @sa axis2_http_svr_thread_ops#is_running */
#define AXIS2_HTTP_SVR_THREAD_IS_RUNNING(svr_thread, env) \
                        ((svr_thread)->ops->is_running(svr_thread, env))

/** Set worker.
    @sa axis2_http_svr_thread_ops#set_worker */
#define AXIS2_HTTP_SVR_THREAD_SET_WORKER(svr_thread, env, worker) \
                        ((svr_thread)->ops->set_worker(svr_thread, env,\
                  worker))

/** Frees the http_svr_thread.
    @sa axis2_http_svr_thread_ops#free */
#define AXIS2_HTTP_SVR_THREAD_FREE(svr_thread, env) \
                ((svr_thread)->ops->free(svr_thread, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SVR_THREAD_H */
