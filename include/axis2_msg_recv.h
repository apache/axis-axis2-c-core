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

#ifndef AXIS2_ENGINE_MSG_RECEIVER_H
#define AXIS2_ENGINE_MSG_RECEIVER_H


/**
  * @file axis2_msg_recv.h
  * @brief axis2 ENGINE CORE msg_receiver
  */

#include <axis2_core.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_engine ENGINE (Axis2 Information model)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @defgroup axis2_engine_msg_receiver ENGINE Msg_receiver
 * @ingroup axis2_engine 
 * @{
 */

/************************** Start of function macros **************************/

#define axis2_engine_msg_receiver_free(env, msg_receiver) \
		(axis2_engine_msg_receiver_get_ops(env, \
		msg_receiver)->free (env, msg_receiver));

#define axis2_engine_msg_receiver_receive(env, msg_receiver, msg_ctx) \
		(axis2_engine_msg_receiver_get_ops(env, \
		msg_receiver)->receive (env, msg_receiver, msg_ctx));

/************************** End of function macros ****************************/

/************************** Start of function pointers ************************/

    typedef axis2_status_t (*axis2_engine_msg_receiver_free_t)
        (axis2_env_t * env,
         axis2_engine_msg_receiver_t * msg_receiver);

    typedef axis2_status_t (*axis2_engine_msg_receiver_receive_t)
        (axis2_env_t * env,
         axis2_engine_msg_receiver_t * msg_receiver,
         axis2_context_msg_ctx_t * msg_ctx);

/**************************** End of function pointers ************************/

    struct axis2_engine_msg_receiver_ops_s
    {
        axis2_engine_msg_receiver_free_t free;

        axis2_engine_msg_receiver_receive_t receive;
    };

    axis2_engine_msg_receiver_t *axis2_engine_msg_receiver_get_ops
        (axis2_env_t * env,
         axis2_engine_msg_receiver_t * msg_receiver);

      axis2_engine_msg_receiver_t
        * axis2_engine_msg_receiver_create (axis2_env_t * env);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENGINE_MSG_RECEIVER_H */
