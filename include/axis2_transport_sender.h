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

#ifndef AXIS2_TRANSPORT_SENDER_H
#define AXIS2_TRANSPORT_SENDER_H

/** @defgroup axis2_transport_sender transport sender
 * @ingroup axis2_transport
 * Description
 * @{
 */

/**
 * @file axis2_transport_sender.h
 * @brief Axis2 description transport sender interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_transport_out_desc.h>
#include <axis2_ctx.h>
#include <axis2_msg_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_transport_out_desc;
    struct axis2_ctx;
    struct axis2_msg_ctx;
    struct axis2_handler;
    /** Type name for struct axis2_transport_sender */
    typedef struct axis2_transport_sender axis2_transport_sender_t;
    /** Type name for struct axis2_transport_sender_ops */
    typedef struct axis2_transport_sender_ops axis2_transport_sender_ops_t;

    /**
     * @brief Description Transport Sender ops struct
     * Encapsulator struct for ops of axis2_transport_sender
     */
    AXIS2_DECLARE_DATA struct axis2_transport_sender_ops
    {
        /** De-allocate memory
    * @param transport_sender pointer to transport sender
    * @param env pointer to environment struct
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_transport_sender_t *transport_sender,
                    const axis2_env_t *env);

        /**
         * Initialize
         * @param transport_sender pointer to transport sender
    * @param env pointer to environment
    * @param conf_ctx pointer to configuration context
    * @param transport_out pointer to transport_out
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    axis2_transport_sender_t *transport_sender,
                    const axis2_env_t *env,
                    struct axis2_conf_ctx *conf_ctx,
                    struct axis2_transport_out_desc *transport_out);

        /**
         * Clean up
         * @param transport_sender pointer to transport sender
    * @param env pointer to environmnet struct
    * @param msg_ctx pointer to message context
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                cleanup)(
                    axis2_transport_sender_t *transport_sender,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

        /**
         * Invoke
         * @param transport_sender pointer to transport sender
    * @param env pointer to environment struct
    * @param msg_ctx pointer to message context
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke)(
                    axis2_transport_sender_t *transport_sender,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

    };

    /**
     * Transport Sender struct
     * This send the SOAP Message to other SOAP nodes and this alone write the SOAP 
     * Message to the wire. Out flow must be end with one of this kind
     */
    AXIS2_DECLARE_DATA struct axis2_transport_sender
    {
   /** operations of axis transport sender */
        axis2_transport_sender_ops_t *ops;
    };

    /**
     * Creates phase holder struct
     * @param env pointer to environment struct
     * @return pointer to newly created transport sender
     */
    AXIS2_EXTERN axis2_transport_sender_t * AXIS2_CALL
    axis2_transport_sender_create (
        const axis2_env_t *env);

/*************************** Function macros **********************************/

/** Frees the axis2 transport sender.
    @sa axis2_transport_sender_ops#free */
#define AXIS2_TRANSPORT_SENDER_FREE(transport_sender, env) \
      ((transport_sender->ops)->free (transport_sender, env))

/** Initialize the axis2 transport sender.
    @sa axis2_transport_sender_ops#init */
#define AXIS2_TRANSPORT_SENDER_INIT(transport_sender, env, conf_context, transport_out) \
      ((transport_sender->ops)->init (transport_sender, env, conf_context, transport_out))

/** Invoke.
    @sa axis2_transport_sender_ops#invoke */
#define AXIS2_TRANSPORT_SENDER_INVOKE(transport_sender, env, msg_ctx) \
      ((transport_sender->ops)->invoke (transport_sender, env, msg_ctx))

/** Cleanup.
    @sa axis2_transport_sender_ops#cleanup */
#define AXIS2_TRANSPORT_SENDER_CLEANUP(transport_sender, env, msg_ctx) \
      ((transport_sender->ops)->cleanup (transport_sender, env, msg_ctx))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_SENDER_H */
