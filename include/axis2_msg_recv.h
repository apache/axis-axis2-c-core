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
  * @brief axis2 ENGINE CORE msg_recv
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
 * @defgroup axis2_engine_msg_recv ENGINE Msg_receiver
 * @ingroup axis2_engine 
 * @{
 */

/************************** Start of function macros **************************/

#define axis2_engine_msg_recv_free(env, msg_recv) \
		(axis2_engine_msg_recv_get_ops(env, \
		msg_recv)->free (env, msg_recv));

#define axis2_engine_msg_recv_receive(env, msg_recv, msg_ctx) \
		(axis2_engine_msg_recv_get_ops(env, \
		msg_recv)->receive (env, msg_recv, msg_ctx));

/************************** End of function macros ****************************/

/************************** Start of function pointers ************************/

    typedef axis2_status_t (*axis2_engine_msg_recv_free_t)
        (axis2_env_t * env,
         axis2_msg_recv_t * msg_recv);

    typedef axis2_status_t (*axis2_engine_msg_recv_receive_t)
        (axis2_env_t * env,
         axis2_msg_recv_t * msg_recv,
         axis2_context_msg_ctx_t * msg_ctx);

/**************************** End of function pointers ************************/

    struct axis2_engine_msg_recv_ops_s
    {
        axis2_engine_msg_recv_free_t free;

        axis2_engine_msg_recv_receive_t receive;
    };

    axis2_msg_recv_t *axis2_engine_msg_recv_get_ops
        (axis2_env_t * env,
         axis2_msg_recv_t * msg_recv);

      axis2_msg_recv_t
        * axis2_engine_msg_recv_create (axis2_env_t * env);

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ENGINE_MSG_RECEIVER_H */
