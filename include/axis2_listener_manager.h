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

#ifndef AXIS2_LISTENER_MANAGER_H
#define AXIS2_LISTENER_MANAGER_H

/**
 * @defgroup axis2_listener_manager listener manager
 * @ingroup axis2_core_context
 * listener manager manages the listeners in case of dual channel invocations.
 * In case of a dual channel invocation, request is sent along one channel
 * and the response is received on another channel. When the response is
 * expected to be received along another transport channel, it has to be
 * made sure that the listener socket is up in anticipation of the response
 * and also that listener must be closed once the response is received.
 * listener manager is responsible for dealing with these tasks.
 * @{
 */


/**
  * @file axis2_listener_manager.h
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_conf_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_listener_manager_ops */
    typedef struct axis2_listener_manager_ops axis2_listener_manager_ops_t;
    /** Type name for struct axis2_listener_manager */
    typedef struct axis2_listener_manager axis2_listener_manager_t;


    /**
     * listener manager ops struct.
     * Encapsulator struct for ops of axis2_listener_manager.
     */
    struct axis2_listener_manager_ops
    {
        /**
         * Ensures that the named transport's listener is started. Starts a listener 
         * if it is not already started. Only one listener would be started for a given 
         * transport.
         * @param listener_manager pointer to listener manager struct
         * @param env pointer to environment struct         
         * @param transport name of the transport
         * @param conf_ctx configuration context to pick transport info for the 
         * named transport
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                make_sure_started)(
                    axis2_listener_manager_t *listener_manager,
                    const axis2_env_t *env,
                    const axis2_char_t *transport,
                    axis2_conf_ctx_t *conf_ctx);

       /**
         * Stops the named listener transport.
         * @param listener_manager pointer to listener manager struct
         * @param env pointer to environment struct         
         * @param transport name of the transport whose listener is to be stopped
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                stop)(
                    axis2_listener_manager_t *listener_manager,
                    const axis2_env_t *env,
                    const axis2_char_t *transport);
        /**
         * Gets reply to end point reference. The engine will direct the 
         * response for the message to this reply to address.
         * @param listener_manager pointer to listener manager struct
         * @param env pointer to environment struct         
         * @param svc_name name of the service for which the epr is to be returned
         * @param transport name of the transport corresponding to the endpoint
         * @return a pointer to endpoint reference struct representing the reply 
         * endpoint
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *
                get_reply_to_epr)(
                const axis2_listener_manager_t *listener_manager,
                const axis2_env_t *env,
                const axis2_char_t *svc_name,
                const axis2_char_t *transport);

        /**
         * Gets the configuration context that holds information on the transports 
         * managed by the listener manager.
         * @param listener_manager pointer to listener manager struct
         * @param env pointer to environment struct         
         */
        axis2_conf_ctx_t *(AXIS2_CALL *
                get_conf_ctx)(
                    const axis2_listener_manager_t *listener_manager,
                    const axis2_env_t *env);
       /**
         * Frees listener manager struct.
         * @param listener_manager pointer to listener manager struct
         * @param env pointer to environment struct         
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_listener_manager_t *listener_manager,
                    const axis2_env_t *env);

    };

    /**
     * listener manager struct.
     */
    struct axis2_listener_manager
    {
        /** operations of listener manager */
        axis2_listener_manager_ops_t *ops;
    };

    /**
     * Creates a listener manager struct instance.
     * @param env pointer to environment struct
     * @return a pointer to newly created listener manager struct,
     *         or NULL on error with error code set in environment's error
     */
    AXIS2_EXTERN axis2_listener_manager_t *AXIS2_CALL
    axis2_listener_manager_create(
        const axis2_env_t *env);

/** Makes sure the named transport is started. 
    @sa axis2_listener_manager_ops#make_sure_started */
#define AXIS2_LISTNER_MANAGER_MAKE_SURE_STARTED(listener_manager, env, transport, conf_ctx)\
        ((listener_manager)->ops->make_sure_started(listener_manager, env, transport, conf_ctx))

/** Stops the named transport.
    @sa axis2_listener_manager_ops#stop */
#define AXIS2_LISTENER_MANAGER_STOP(listener_manager, env, transport)\
        ((listener_manager)->ops->stop(listener_manager, env, transport))

/** Gets reply to endpoint reference.
    @sa axis2_listener_manager_ops#get_reply_to_epr */
#define AXIS2_LISTNER_MANAGER_GET_REPLY_TO_EPR(listener_manager, env, svc_name, transport) \
        ((listener_manager)->ops->get_reply_to_epr(listener_manager, env, svc_name, transport))

/** Gets configuration context.
    @sa axis2_listener_manager_ops#get_conf_ctx */
#define AXIS2_LISTNER_MANAGER_GET_CONF_CTX(listener_manager, env) \
        ((listener_manager)->ops->get_conf_ctx(listener_manager, env))

/** Frees listener manager.
    @sa axis2_listener_manager_ops#free */
#define AXIS2_LISTNER_MANAGER_FREE(listener_manager, env) \
        ((listener_manager)->ops->free(listener_manager, env))

    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_LISTENER_MANAGER_H */
