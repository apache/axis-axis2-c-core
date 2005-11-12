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

#ifndef AXIS2_MSG_CTX_H
#define AXIS2_MSG_CTX_H


/**
  * @file axis2_msg_ctx.h
  * @brief axis2 Message Context interface
  */

#include <axis2_core.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_msg_ctx Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_msg_ctx_ops axis2_msg_ctx_ops_t;
typedef struct axis2_msg_ctx axis2_msg_ctx_t;    
    
/** 
 * @brief Message Context operations struct
 * Encapsulator struct for operations of axis2_msg_ctx
 */  
struct axis2_msg_ctx_ops
{
    /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free)(axis2_msg_ctx_t *msg_ctx,
                                        axis2_env_t **env); 
   /**
    *
    */
    axis2_status_t (AXIS2_CALL *set_paused_phase_name)(axis2_msg_ctx_t *msg_ctx,
                                        axis2_env_t **env, axis2_char_t *name); 
   /**
    *
    */    
    axis2_bool_t (AXIS2_CALL *is_paused)(axis2_msg_ctx_t *msg_ctx,
                                         axis2_env_t **env); 
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_msg_ctx
{
    axis2_msg_ctx_ops_t *ops;    
};

AXIS2_DECLARE(axis2_msg_ctx_t *)
axis2_msg_ctx_create (axis2_env_t **env);
    
/************************** Start of function macros **************************/

#define AXIS2_MSG_CTX_FREE(msg_ctx, env) ((msg_ctx)->ops->free (msg_ctx, env))
#define AXIS2_MSG_CTX_SET_PAUSED_PHASE_NAME(msg_ctx, env, name) ((msg_ctx)->ops->set_paused_phase_name(msg_ctx, env, name))
#define AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env) ((msg_ctx)->ops->is_paused (msg_ctx, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_MSG_CTX_H */
