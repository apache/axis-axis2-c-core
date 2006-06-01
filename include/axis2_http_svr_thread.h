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

#ifndef AXIS2_HTTP_SVR_THREAD_H
#define AXIS2_HTTP_SVR_THREAD_H


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

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

typedef struct axis2_http_svr_thread_ops axis2_http_svr_thread_ops_t;
typedef struct axis2_http_svr_thread axis2_http_svr_thread_t;

extern int axis2_http_socket_read_timeout;
    
/** 
 * @brief HTTP Server Thread ops struct
 * Encapsulator struct for ops of axis2_http_svr_thread
 */  
AXIS2_DECLARE_DATA struct axis2_http_svr_thread_ops
{
    axis2_status_t (AXIS2_CALL *run)
						(axis2_http_svr_thread_t *svr_thread, 
						const axis2_env_t *env);
	axis2_status_t (AXIS2_CALL *destroy) 
						(axis2_http_svr_thread_t *svr_thread, 
						const axis2_env_t *env);
	int (AXIS2_CALL *get_local_port)
						(axis2_http_svr_thread_t *svr_thread, 
						const axis2_env_t *env);
    axis2_bool_t (AXIS2_CALL *is_running) 
						(axis2_http_svr_thread_t *svr_thread, 
                    	const axis2_env_t *env);
	axis2_status_t (AXIS2_CALL *set_worker) 
						(axis2_http_svr_thread_t *svr_thread, 
                    	const axis2_env_t *env, axis2_http_worker_t *worker);
	axis2_status_t (AXIS2_CALL *free)
                    (axis2_http_svr_thread_t *svr_thread, 
                    const axis2_env_t *env);
};

/** 
 * @brief HTTP Server Thread struct
 *    Axis2 HTTP Server Thread
 */
AXIS2_DECLARE_DATA struct axis2_http_svr_thread
{
    axis2_http_svr_thread_ops_t *ops;    
};


AXIS2_EXTERN axis2_http_svr_thread_t * AXIS2_CALL 
axis2_http_svr_thread_create (const axis2_env_t *env, int port);

/************************** Start of function macros **************************/

#define AXIS2_HTTP_SVR_THREAD_RUN(svr_thread, env) \
						((svr_thread)->ops->run(svr_thread, env))
#define AXIS2_HTTP_SVR_THREAD_DESTROY(svr_thread, env) \
                        ((svr_thread)->ops->destroy(svr_thread, env))
#define AXIS2_HTTP_SVR_THREAD_GET_LOCAL_PORT(svr_thread, env) \
                        ((svr_thread)->ops->get_local_port(svr_thread,\
						env))
#define AXIS2_HTTP_SVR_THREAD_IS_RUNNING(svr_thread, env) \
                        ((svr_thread)->ops->is_running(svr_thread, env))
#define AXIS2_HTTP_SVR_THREAD_SET_WORKER(svr_thread, env, worker) \
                        ((svr_thread)->ops->set_worker(svr_thread, env,\
						worker))
#define AXIS2_HTTP_SVR_THREAD_FREE(svr_thread, env) \
                ((svr_thread)->ops->free(svr_thread, env))
/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_SVR_THREAD_H */
