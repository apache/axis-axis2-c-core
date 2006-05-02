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

#ifndef AXIS2_SVR_CALLBACK_H
#define AXIS2_SVR_CALLBACK_H

/**
  * @file axis2_svr_callback.h
  * @brief axis Server Callback interface
  */

#ifdef __cplusplus
extern "C"
{
#endif
    
/** @defgroup axis2_svr_callback Server Callback
  * @ingroup axis2_core_receivers
  * @{
  */

#include <axis2_defines.h>
#include <axis2_const.h>
#include <axis2_msg_ctx.h>

typedef struct axis2_svr_callback axis2_svr_callback_t;
typedef struct axis2_svr_callback_ops axis2_svr_callback_ops_t;

/** 
 * @brief Message Receiver ops struct
 * Encapsulator struct for ops of axis2_svr_callback
 */     
struct axis2_svr_callback_ops
{
    /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_svr_callback_t *svr_callback,
            axis2_env_t **env);

    /**
     * Handle result
     * @param msg_ctx
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    handle_result) (axis2_svr_callback_t *svr_callback,
                        axis2_env_t **env,
                        axis2_msg_ctx_t *msg_ctx);
    
    /** 
     * Handle fault
     * @param msg_ctx
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    handle_fault) (axis2_svr_callback_t *svr_callback,
                    axis2_env_t **env,
                    axis2_msg_ctx_t *msg_ctx);               
};

/** 
 * @brief Server Callback struct
  *	Axis2 Server Callback
 */
struct axis2_svr_callback
{
    axis2_svr_callback_ops_t *ops;    
};

/**
 * Create Server Callback struct
 * @return newly created server callback object
 */
AXIS2_DECLARE(axis2_svr_callback_t *)
axis2_svr_callback_create (axis2_env_t **env);

/************************** Start of function macros **************************/

#define AXIS2_SVR_CALLBACK_FREE(svr_callback, env) \
        ((svr_callback)->ops->free (svr_callback, env))

#define AXIS2_SVR_CALLBACK_HANDLER_RESULT(svr_callback, env, msg_ctx) \
		((svr_callback)->ops->handle_result (svr_callback, env, msg_ctx))

#define AXIS2_SVR_CALLBACK_HANDLER_FAULT(svr_callback, env, msg_ctx) \
		((svr_callback)->ops->handle_fault (svr_callback, env, msg_ctx))

/************************** End of function macros ****************************/
    
/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_SVR_CALLBACK_H */
