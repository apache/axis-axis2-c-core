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

#ifndef AXIS2_CALLBACK_RECV_H
#define AXIS2_CALLBACK_RECV_H


/**
  * @file axis2_callback_recv.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_msg_recv.h>
#include <axis2_callback.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_callback_recv Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
#define AXIS2_CALLBACK_RECV_SVC_NAME "ClientService"
    
typedef struct axis2_callback_recv_ops axis2_callback_recv_ops_t;
typedef struct axis2_callback_recv axis2_callback_recv_t; 

    
/** 
 * @brief Message Context ops struct
 * Encapsulator struct for ops of axis2_callback_recv
 */  
struct axis2_callback_recv_ops
{
    axis2_msg_recv_t* (AXIS2_CALL *get_base)(struct axis2_callback_recv *callback_recv, 
                                                axis2_env_t **env);
    axis2_status_t (AXIS2_CALL *free)(struct axis2_callback_recv *callback_recv, 
                                       axis2_env_t **env);
    axis2_status_t (AXIS2_CALL *add_callback)(struct axis2_callback_recv *callback_recv, 
        axis2_env_t **env,
        axis2_char_t *msg_id, 
        axis2_callback_t *callback);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_callback_recv
{
    axis2_callback_recv_ops_t *ops;    
};

AXIS2_DECLARE(axis2_callback_recv_t*) axis2_callback_recv_create(axis2_env_t **env);
    
/************************** Start of function macros **************************/

#define AXIS2_CALLBACK_RECV_GET_BASE(callback_recv, env) ((callback_recv)->ops->get_base(callback_recv, env))
#define AXIS2_CALLBACK_RECV_FREE(callback_recv, env) ((callback_recv)->ops->free(callback_recv, env))
#define AXIS2_CALLBACK_RECV_ADD_CALLBACK(callback_recv, env, msg_id, callback) ((callback_recv)->ops->add_callback(callback_recv, env, msg_id, callback))
    
/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CALLBACK_RECV_H */
