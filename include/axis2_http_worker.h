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
  * @file axis2_http_worker.h
  * @brief axis2 HTTP Worker
  */

#include <axis2.h>
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

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

    typedef struct axis2_http_worker_ops axis2_http_worker_ops_t;
    typedef struct axis2_http_worker axis2_http_worker_t; 
    
/** 
 * @brief HTTP Worker ops struct
 * Encapsulator struct for ops of axis2_http_worker
 */  
AXIS2_DECLARE_DATA struct axis2_http_worker_ops
{
    axis2_bool_t (AXIS2_CALL *process_request)
                    (axis2_http_worker_t *http_worker, 
                    axis2_env_t **env, 
                    axis2_simple_http_svr_conn_t *svr_conn, 
                    axis2_http_simple_request_t *simple_request);
    
    axis2_status_t (AXIS2_CALL *free)
                    (axis2_http_worker_t *http_worker, 
                    axis2_env_t **env);
};

/** 
 * @brief HTTP Worker struct
  *    Axis2 HTTP Worker
 */
AXIS2_DECLARE_DATA struct axis2_http_worker
{
    axis2_http_worker_ops_t *ops;    
};


AXIS2_DECLARE(axis2_http_worker_t *) 
axis2_http_worker_create (axis2_env_t **env, axis2_conf_ctx_t *conf_ctx);
    
/************************** Start of function macros **************************/


#define AXIS2_HTTP_WORKER_PROCESS_REQUEST\
                (http_worker, env, svr_conn, simple_request) \
                ((http_worker)->ops->process_request(http_worker, env, svr_conn\
                ,simple_request))
#define AXIS2_HTTP_WORKER_FREE\
                (http_worker, env) \
                ((http_worker)->ops->free(http_worker, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_HTTP_WORKER_H */
