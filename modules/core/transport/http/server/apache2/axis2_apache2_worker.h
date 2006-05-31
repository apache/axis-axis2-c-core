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

#ifndef AXIS2_APACHE2_WORKER_H
#define AXIS2_APACHE2_WORKER_H


/**
  * @file axis2_apache2_worker.h
  * @brief axis2 Apache2 Worker
  */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_conf_ctx.h>
#include "apache2_stream.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @ingroup axis2_core_transport_http
 * @{
 */

    typedef struct axis2_apache2_worker_ops axis2_apache2_worker_ops_t;
    typedef struct axis2_apache2_worker axis2_apache2_worker_t; 
    
/** 
 * @brief Apache2 Worker ops struct
 * Encapsulator struct for ops of axis2_apache2_worker
 */  
AXIS2_DECLARE_DATA struct axis2_apache2_worker_ops
{
    int (AXIS2_CALL *process_request)
                    (axis2_apache2_worker_t *apache2_worker, 
                     const axis2_env_t *env, request_rec *r);
    
    axis2_status_t (AXIS2_CALL *free)
                    (axis2_apache2_worker_t *apache2_worker, 
                    const axis2_env_t *env);
};

/** 
 * @brief Apache2 Worker struct
  *    Axis2 Apache2 Worker
 */
AXIS2_DECLARE_DATA struct axis2_apache2_worker
{
    axis2_apache2_worker_ops_t *ops;    
};


AXIS2_DECLARE(axis2_apache2_worker_t *) 
axis2_apache2_worker_create (const axis2_env_t *env, axis2_char_t *repo_path);
    
/************************** Start of function macros **************************/
#define AXIS2_APACHE2_WORKER_PROCESS_REQUEST(apache2_worker, env, request) \
                        ((apache2_worker)->ops->process_request(\
                        apache2_worker, env, request))
#define AXIS2_APACHE2_WORKER_FREE(apache2_worker, env) \
                ((apache2_worker)->ops->free(apache2_worker, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_APACHE2_WORKER_H */
