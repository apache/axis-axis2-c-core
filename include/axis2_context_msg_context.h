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

#ifndef AXIS2_ENGINE_MSG_CONTEXT_H
#define AXIS2_ENGINE_MSG_CONTEXT_H


/**
  * @file axis2_context_msg_context.h
  * @brief axis2 ENGINE CORE msg_context
  */

#include <axis2_core.h>
#include <axis2_context_msg_context.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_context ENGINE (Axis2 Information model)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @defgroup axis2_context_msg_context ENGINE Msg_context
 * @ingroup axis2_context 
 * @{
 */

/************************** Start of function macros **************************/

#define axis2_context_msg_ctx_free(env, msg_ctx) \
		(axis2_context_msg_ctx_get_ops(env, \
		msg_ctx)->free (env, msg_ctx));

/************************** End of function macros ****************************/

/************************** Start of function pointers ************************/

    typedef axis2_status_t (*axis2_context_msg_ctx_free_t)
        (axis2_env_t * env, axis2_context_msg_ctx_t * msg_ctx);

/**************************** End of function pointers ************************/

    struct axis2_context_msg_ctx_ops_s
    {
        axis2_context_msg_ctx_free_t free;
    };

    axis2_context_msg_ctx_t *axis2_context_msg_ctx_get_ops
        (axis2_env_t * env, axis2_context_msg_ctx_t * msg_ctx);

      axis2_context_msg_ctx_t
        * axis2_context_msg_ctx_create (axis2_env_t * env);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENGINE_MSG_CONTEXT_H */
