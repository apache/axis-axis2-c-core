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
 * An operation client is the way an advanced user interacts with Axis2. Actual
 * operation clients understand a specific MEP and hence their behavior is
 * defined by their MEP. To interact with an operation client, you first get one
 * from a specific axis2_op_t. Then you set the messages into it one by one
 * (whatever is available). Then, when you call execute() the client will
 * execute what it can at that point. If executing the operation client results
 * in a new message being created, then if a message receiver is registered with
 * the client then the message will be delivered to that client.
 */

/**
  * @file axis2_op_client.h
  * @brief axis2 Operation Client interface
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

/** @defgroup axis2_op_client Operation Client 
 * @ingroup axis2_core_clientapi
 * @{
 */
    
typedef struct axis2_op_client_ops axis2_op_client_ops_t;
typedef struct axis2_op_client axis2_op_client_t; 
struct axis2_callback_recv;
    
/** 
 * @brief Operation Client ops struct
 * Encapsulator struct for ops of axis2_op_client
 */  
struct axis2_op_client_ops
{

   /**
     * Sets the options that should be used for this particular client. This
     * resets the entire set of options to use the new options - so you'd lose
     * any option cascading that may have been set up.
     *
     * @param options the options
     */
   axis2_status_t (AXIS2_CALL *
   set_options)(struct axis2_op_client *op_client,
               const axis2_env_t *env,
               axis2_options_t *options);


    /**
     * Return the options used by this client. If you want to set a single
     * option, then the right way is to do get_options() and set specific
     * options.
     *
     * @return the options, which will never be null.
     */
   axis2_options_t* (AXIS2_CALL *
   get_options)(struct axis2_op_client *op_client,
               const axis2_env_t *env);

    /**
     * Add a message context to the client for processing. This method must not
     * process the message - it only records it in the operation client.
     * Processing only occurs when execute() is called.
     *
     * @param mc the message context
     */
   axis2_status_t (AXIS2_CALL *
   add_msg_ctx)(struct axis2_op_client *op_client,
                  const axis2_env_t *env,
                  axis2_msg_ctx_t *mc);

   /**
     * Return a message from the client - will return null if the requested
     * message is not available.
     *
     * @param message_label the message label of the desired message context
     * @return the desired message context or null if its not available.
     */
   axis2_msg_ctx_t* (AXIS2_CALL *
   get_msg_ctx)(struct axis2_op_client *op_client,
                     const axis2_env_t *env,
                     const axis2_char_t *message_label);

    /**
     * Set the callback to be executed when a message comes into the MEP and the
     * operation client is executed. This is the way the operation client
     * provides notification that a message has been received by it. Exactly
     * when its executed and under what conditions is a function of the specific
     * operation client.
     *
     * @param callback the callback to be used when the client decides its time to
     *                 use it
     */
   axis2_status_t (AXIS2_CALL *
   set_callback)(struct axis2_op_client *op_client,
               const axis2_env_t *env,
               axis2_callback_t *callback);

    /**
     * Execute the MEP. What this does depends on the specific operation client.
     * The basic idea is to have the operation client execute and do something
     * with the messages that have been added to it so far. For example, if its
     * an Out-In MEP, then if the Out message has been set, then executing the
     * client asks it to send the message and get the In message, possibly using
     * a different thread.
     *
     * @param block Indicates whether execution should block or return ASAP. What
     *              block means is of course a function of the specific operation
     *              client.
     */
   axis2_status_t (AXIS2_CALL *
   execute)(struct axis2_op_client *op_client,
            const axis2_env_t *env,
            axis2_bool_t block);

    /**
     * Reset the operation client to a clean status after the MEP has completed.
     * This is how you can reuse an operation client. NOTE: this does not reset
     * the options; only the internal state so the client can be used again.
     *
     */
   axis2_status_t (AXIS2_CALL *
   reset)(struct axis2_op_client *op_client,
            const axis2_env_t *env);

    /**
     * To close the tranport if necessary , can call this method. The most usage of thie method
     * when clinet use two tarnport for sending and receiving , there we need to remove entry from
     * waitings calls in the transport listener queue
     * Note : DO NOT call this method if you are not using two transport send and receive
     *
     * @param mc : axis2_msg_ctx_t# which have all the trnport information
     */
   axis2_status_t (AXIS2_CALL *
   compelete)(struct axis2_op_client *op_client,
            const axis2_env_t *env,
            axis2_msg_ctx_t *mc);

   /**
     * To get the operation context of the operation cleint
     * @return Operation Context
     */
   axis2_op_ctx_t* (AXIS2_CALL *
   get_operation_context)(struct axis2_op_client *op_client);

    axis2_status_t (AXIS2_CALL *
    set_callback_recv)(
        struct axis2_op_client *op_client, 
        const axis2_env_t *env,
        struct axis2_callback_recv *callback_recv);

   axis2_status_t (AXIS2_CALL *
   free)(struct axis2_op_client *op_client,
            const axis2_env_t *env);
};

/** 
 * @brief Operation Client struct
  *   Axis2 Operation Client
 */
struct axis2_op_client
{
    axis2_op_client_ops_t *ops;    
};

AXIS2_EXTERN axis2_op_client_t* AXIS2_CALL axis2_op_client_create(const axis2_env_t *env,
                                          axis2_op_t *op,
                                          axis2_svc_ctx_t *svc_ctx,
                                          axis2_options_t *options);
    
/************************** Start of function macros **************************/

#define AXIS2_OP_CLIENT_SET_OPTIONS(op_client, env, options) \
      ((op_client)->ops->set_options(op_client, env, options))

#define AXIS2_OP_CLIENT_GET_OPTIONS(op_client, env) \
      ((op_client)->ops->get_options(op_client, env))

#define AXIS2_OP_CLIENT_ADD_MSG_CTX(op_client, env, mc) \
      ((op_client)->ops->add_msg_ctx(op_client, env, mc))

#define AXIS2_OP_CLIENT_GET_MSG_CTX(op_client, env, message_label) \
      ((op_client)->ops->get_msg_ctx(op_client, env, message_label))

#define AXIS2_OP_CLIENT_SET_CALLBACK(op_client, env, callback) \
      ((op_client)->ops->set_callback(op_client, env, callback))

#define AXIS2_OP_CLIENT_EXECUTE(op_client, env, block) \
      ((op_client)->ops->execute(op_client, env, block))

#define AXIS2_OP_CLIENT_RESET(op_client, env) \
      ((op_client)->ops->reset(op_client, env))

#define AXIS2_OP_CLIENT_COMPLETE(op_client, env, mc) \
      ((op_client)->ops->compelete(op_client, env, mc))

#define AXIS2_OP_CLIENT_GET_OPERATION_CONTEXT(op_client, env) \
      ((op_client)->ops->get_operation_context(op_client, env))

#define AXIS2_OP_CLIENT_FREE(op_client, env) \
      ((op_client)->ops->free(op_client, env))

#define AXIS2_OP_CLIENT_SET_CALLBACK_RECV(op_client, env, callback_recv) \
      ((op_client)->ops->set_callback_recv(op_client, env, callback_recv))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CALLBACK_H */
