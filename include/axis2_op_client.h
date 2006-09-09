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

#ifndef AXIS2_OP_CLIENT_H
#define AXIS2_OP_CLIENT_H

/**
 * @defgroup axis2_client_api client API
 * @ingroup axis2
 * @{
 * @}
 */

/**
 * @defgroup axis2_op_client operation client
 * @ingroup axis2_client_api
 * The operation client is meant to be used by advanced users to consume services.
 * Operation client understands a specific Message Exchange Pattern (MEP) and 
 * hence the behavior is defined by the MEP. 
 * To consume services with an operation client, an operation (of type axis2_op_t) 
 * and a service context (of type axis2_svc_ctx_t)
 * has to be provided along with options to be used. The execute() function
 * can be used to send the request and get the response. 
 * The service client implementation uses the operation client and provides an
 * easy to use API for consuming services. Hence the service client 
 * implementation is a very good example of how to use the operation client API.
 * @sa axis2_svc_client
 * @{
 */


/**
 * @file axis2_op_client.h
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_options.h>
#include <axis2_msg_ctx.h>
#include <axis2_callback.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_op_client_ops */
    typedef struct axis2_op_client_ops axis2_op_client_ops_t;
    /** Type name for struct axis2_op_client */
    typedef struct axis2_op_client axis2_op_client_t;
        
    struct axis2_callback_recv;

    /**
     * operation client ops struct.
     * Encapsulator struct for ops of axis2_op_client.
     */
    struct axis2_op_client_ops
    {

        /**
         * Sets the options that is to be used by this operation client. 
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @param options pointer to options struct to be set
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_options)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    const axis2_options_t *options);

        /**
         * Gets options used by operation client. 
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @return a pointer to the options struct if options set, else NULL.
         * Returns a reference, not a cloned copy.         
         */
        const axis2_options_t* (AXIS2_CALL *
                get_options)(
                    const axis2_op_client_t *op_client,
                    const axis2_env_t *env);

        /**
         * Adds a message context to the client for processing. 
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @param msg_ctx message context to be added. operation client takes 
         * over the ownership of the message context struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_msg_ctx)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * Adds out message context to the client for processing. 
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @param msg_ctx message context to be added. operation client takes 
         * over the ownership of the message context struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_out_msg_ctx)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);


        /**
         * Gets a message corresponding to the given label.
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @param message_label the message label of the desired message context
         * @return the desired message context or NULL if its not available.
         * Returns a reference, not a cloned copy.
         */
        const axis2_msg_ctx_t* (AXIS2_CALL *
                get_msg_ctx)(
                    const axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    const axis2_char_t *message_label);

        /**
         * Sets the callback to be executed when a response message is received. 
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @param callback the callback to be used. operation client takes 
         * over the ownership of the message context struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_callback)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    axis2_callback_t *callback);

        /**
         * Execute the MEP. What this does depends on the specific operation client.
         * The basic idea is to have the operation client execute and do something
         * with the messages that have been added to it so far. For example, if its
         * an Out-In MEP, and if the Out message has been set, then executing the
         * client asks it to send the out message and get the in message
         * @param op_client pointer to operation client
         * @param env pointer to environment struct
         * @param block indicates whether execution should block or return ASAP
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                execute)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    const axis2_bool_t block);

        /**
         * Resets the operation client to a clean status after the MEP has completed.
         * This is how you can reuse an operation client. Note that this does not reset
         * the options; only the internal state so the client can be used again.
         * @param op_client pointer to operation client
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                reset)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env);

        /**
         * Completes the execution by closing the transports if necessary.
         * This method is useful when client uses two transports for sending and 
         * receiving.
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @param msg_ctx message context which contains the transport information
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                complete)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * Gets the operation context of the operation client.
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct                
         * @return operation context related to operation client
         */
        axis2_op_ctx_t* (AXIS2_CALL *
                get_operation_context)(
                    const axis2_op_client_t *op_client,
                    const axis2_env_t *env);

        /**
         * Sets callback receiver.
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @param callback_recv pointer to callback receiver struct.
         * operation client assumes ownership of the callback struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_callback_recv)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env,
                    struct axis2_callback_recv *callback_recv);

        /**
         * Frees the operation client
         * @param op_client pointer to operation client struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_op_client_t *op_client,
                    const axis2_env_t *env);
    };

    /**
     * operation client struct.
     */
    struct axis2_op_client
    {
        /** operations of operation client */
        axis2_op_client_ops_t *ops;
    };

    /**
     * Creates an operation client struct for the specified operation, service 
     * context and given options.
     * @param env pointer to environment struct
     * @param op pointer to operation struct corresponding to the operation to 
     * to be executed. Newly created client assumes ownership of the operation.
     * @param svc_ctx pointer to service context struct representing the service
     * to be consumed. Newly created client assumes ownership of the service 
     * context.
     * @param options options to be used by operation client. Newly created 
     * client assumes ownership of the options
     * context.
     * @return a pointer to newly created operation client struct,
     * or NULL on error with error code set in environment's error
     */
    AXIS2_EXTERN axis2_op_client_t* AXIS2_CALL axis2_op_client_create(
        const axis2_env_t *env,
        axis2_op_t *op,
        axis2_svc_ctx_t *svc_ctx,
        axis2_options_t *options);

     
/** Sets the options to be used by operation client. 
    @sa axis2_op_client_ops#set_options */
#define AXIS2_OP_CLIENT_SET_OPTIONS(op_client, env, options) \
      ((op_client)->ops->set_options(op_client, env, options))

/** Gets the options used by operation client. 
    @sa axis2_op_client_ops#get_options */
#define AXIS2_OP_CLIENT_GET_OPTIONS(op_client, env) \
      ((op_client)->ops->get_options(op_client, env))

/** Adds a message context. 
    @sa axis2_op_client_ops#add_msg_ctx*/
#define AXIS2_OP_CLIENT_ADD_MSG_CTX(op_client, env, msg_ctx) \
      ((op_client)->ops->add_msg_ctx(op_client, env, msg_ctx))

/** Adds out message context. 
    @sa axis2_op_client_ops#add_out_msg_ctx*/
#define AXIS2_OP_CLIENT_ADD_OUT_MSG_CTX(op_client, env, msg_ctx) \
      ((op_client)->ops->add_out_msg_ctx(op_client, env, msg_ctx))

/** Gets the message context corresponding to given label. 
    @sa axis2_op_client_ops#get_msg_ctx*/
#define AXIS2_OP_CLIENT_GET_MSG_CTX(op_client, env, message_label) \
      ((op_client)->ops->get_msg_ctx(op_client, env, message_label))

/** Sets operation client callback. 
    @sa axis2_op_client_ops#set_callback*/
#define AXIS2_OP_CLIENT_SET_CALLBACK(op_client, env, callback) \
      ((op_client)->ops->set_callback(op_client, env, callback))

/** Executes operation client. 
    @sa axis2_op_client_ops#execute*/
#define AXIS2_OP_CLIENT_EXECUTE(op_client, env, block) \
      ((op_client)->ops->execute(op_client, env, block))

/** Resets operation client. 
    @sa axis2_op_client_ops#reset*/
#define AXIS2_OP_CLIENT_RESET(op_client, env) \
      ((op_client)->ops->reset(op_client, env))

/** Completes operation client, closing open transports. 
    @sa axis2_op_client_ops#complete*/
#define AXIS2_OP_CLIENT_COMPLETE(op_client, env, msg_ctx) \
      ((op_client)->ops->complete(op_client, env, msg_ctx))

/** Gets the operation context. 
    @sa axis2_op_client_ops#get_operation_context*/
#define AXIS2_OP_CLIENT_GET_OPERATION_CONTEXT(op_client, env) \
      ((op_client)->ops->get_operation_context(op_client, env))

/** Frees the operation client. 
    @sa axis2_op_client_ops#free*/
#define AXIS2_OP_CLIENT_FREE(op_client, env) \
      ((op_client)->ops->free(op_client, env))

/** Sets the callback receiver. 
    @sa axis2_op_client_ops#set_callback_recv*/
#define AXIS2_OP_CLIENT_SET_CALLBACK_RECV(op_client, env, callback_recv) \
      ((op_client)->ops->set_callback_recv(op_client, env, callback_recv))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OP_CLIENT_H */
