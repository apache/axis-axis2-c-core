/*
* Licensed to the Apache Software Foundation (ASF) under one or more
* contributor license agreements.  See the NOTICE file distributed with
* this work for additional information regarding copyright ownership.
* The ASF licenses this file to You under the Apache License, Version 2.0
* (the "License"); you may not use this file except in compliance with
* the License.  You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef AXIS2_MSG_RECV_H
#define AXIS2_MSG_RECV_H

/**
 * @defgroup axis2_receivers receivers
 * @ingroup axis2
 * @{
 * @} 
 */

/** @defgroup axis2_msg_recv message receiver
 * @ingroup axis2_receivers
 * Description.
 * @{
 */

/**
 * @file axis2_msg_recv.h
 * @brief Axis Message Receiver interface. Message Receiver struct. 
 * This interface is extended by custom message receivers
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <axis2_defines.h>
#include <axis2_const.h>
#include <axis2_svc_skeleton.h>
#include <axis2_msg_ctx.h>
#include <axis2_op_ctx.h>
#include <axis2_svr_callback.h>

/** Sets AXIS2_SUCCESS to 1 */
#define AXIS2_SUCCESS 1
/** Sets AXIS2_FAILURE to 0 */
#define AXIS2_FAILURE 0

    struct axis2_msg_ctx;
    /** Type name for struct axis2_msg_recv ops */
    typedef struct axis2_msg_recv_ops axis2_msg_recv_ops_t;
    /** Type name for struct axis2_msg_recv*/
    typedef struct axis2_msg_recv axis2_msg_recv_t;

    /**
     * Message Receiver ops struct.
     * Encapsulator struct for ops of axis2_msg_recv
     */
    struct axis2_msg_recv_ops
    {
        /**
         * Deallocate memory
         * @param msg_recv pinter to message receiver
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free_fn)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env);

        /**
         * This method is called from axis2_engine_receive method. This method's
         * actual implementation is decided from the create method of the 
         * extended message receiver object. There depending on the synchronous or
         * asynchronous type, receive metho is assigned with the synchronous or
         * asynchronous implementation of receive.
         * @see raw_xml_in_out_msg_recv_create method where receive is assigned
         * to receive_sync
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param in_msg_ctx pointer to in message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                receive)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *in_msg_ctx,
                    void *callback_recv_param);
        /**
         * This contain synchronous receiving logic.
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param in_msg_ctx pointer to in message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                receive_sync)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *in_msg_ctx,
                    void *callback_recv_param);

        /**
         * This contain asynchronous receiving logic.
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param in_msg_ctx pointer to in message 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                receive_async)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *in_msg_ctx,
                    void *callback_recv_param);

        /**
         * This contain in only synchronous business invoke logic
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param in_msg_ctx pointer to in message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke_in_business_logic_sync)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *in_msg_ctx);

        /**
         * This contain in only asynchronous business invoke logic
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param in_msg_ctx pointer to in message context
         * @param svr_callback pointer to server callback
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke_in_business_logic_async)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *in_msg_ctx,
                    struct axis2_svr_callback *svr_callback);

        /**
         * This contain in out synchronous business invoke logic
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param in_msg_ctx pointer to in message context
         * @param out_msg_ctx pointer to out message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke_in_out_business_logic_sync)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *in_msg_ctx,
                    struct axis2_msg_ctx *out_msg_ctx);

        /**
         * This contain in out asynchronous business invoke logic
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param in_msg_ctx pinter to in message context
         * @param out_msg_ctx pointer to out message context
         * @param callback pointer to callback
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke_in_out_business_logic_async)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *in_msg_ctx,
                    struct axis2_msg_ctx *out_msg_ctx,
                    struct axis2_svr_callback *callback);


        /**
         * this will create a new service skeleton object
         * @param msg_recv pointer to message receiver
         * @param env pointer to enviornment struct
         * @param msg_ctx pointer to message context
         * @return service skeleton object
         */
        axis2_svc_skeleton_t *(AXIS2_CALL *
                make_new_svc_obj)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

        /**
         * This will return the service skeleton object
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @return service skeleton object
         */
        axis2_svc_skeleton_t *(AXIS2_CALL *
                get_impl_obj)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

        /**
         * Set the application scope
         * @param msg_recv pointer to message receiver
         * @param env pointer to environment struct
         * @param scope ointer to scope
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_scope)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    const axis2_char_t *scope);


        /**
         * Get the application scope
         * @param msg_recv pointer to message receiver
         * @env pointer to enviornment struct
         * @return scope
         */
        axis2_char_t *(AXIS2_CALL *
                get_scope)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env);

        /**
         * Delete the service skeleton object created by make_new_svc_obj
         * @param msg_recv pointer to message receiver
         * @env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                delete_svc_obj)(
                    axis2_msg_recv_t *msg_recv,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
    };

    /**
     * message receiver struct
     */
    struct axis2_msg_recv
    {
        /** operations of message receiver */
        axis2_msg_recv_ops_t *ops;
        void* derived;
    };

    /**
     * Create new message receiver object. usually this will be called from the
     * extended message receiver object.
     * @see create method of raw_xml_in_out_msg_recv
     * @param env pointer to environment struct
     * @return newly created message receiver object
     **/
    AXIS2_EXTERN axis2_msg_recv_t *AXIS2_CALL
    axis2_msg_recv_create (
        const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Frees the message receiver.
    @sa axis2_msg_recv_ops#free */
#define AXIS2_MSG_RECV_FREE(msg_recv, env) ((msg_recv)->ops->free_fn (msg_recv, env))

/** Receive.
    @sa axis2_msg_recv_ops#receive */
#define AXIS2_MSG_RECV_RECEIVE(msg_recv, env, msg_ctx, callback_recv_param) \
      ((msg_recv)->ops->receive (msg_recv, env, msg_ctx, callback_recv_param))

/** Receive sync.
    @sa axis2_msg_recv_ops#sync */
#define AXIS2_MSG_RECV_RECEIVE_SYNC(msg_recv, env, msg_ctx, callback_recv_param) \
      ((msg_recv)->ops->receive_sync (msg_recv, env, msg_ctx, callback_recv_param))

/** Receive async.
    @sa axis2_msg_recv_ops#receive_async */
#define AXIS2_MSG_RECV_RECEIVE_ASYNC(msg_recv, env, msg_ctx, callback_recv_param) \
      ((msg_recv)->ops->receive_async (msg_recv, env, msg_ctx, callback_recv_param))

/** Invoke in business logic sync.
    @sa axis2_msg_recv_ops#invoke_in_business_logic_sync */
#define AXIS2_MSG_RECV_INVOKE_IN_BUSINESS_LOGIC_SYNC(msg_recv, env, in_msg_ctx) \
        ((msg_recv)->ops->invoke_in_business_logic_sync(msg_recv, env, in_msg_ctx))

/** Invoke in business logic async.
    @sa axis2_msg_recv_ops#invoke_in_business_logic_async */
#define AXIS2_MSG_RECV_INVOKE_IN_BUSINESS_LOGIC_ASYNC(msg_recv, env, \
            in_msg_ctx, callback) \
        ((msg_recv)->ops->invoke_in_business_logic_async(msg_recv, env, \
            in_msg_ctx, callback))

/** Invoke in out business logic sync.
    @sa axis2_msg_recv_ops#invoke_in_out_business_logic_sync */
#define AXIS2_MSG_RECV_INVOKE_IN_OUT_BUSINESS_LOGIC_SYNC(msg_recv, env, \
            in_msg_ctx, out_msg_ctx) \
        ((msg_recv)->ops->invoke_in_out_business_logic_sync (msg_recv, env, \
            in_msg_ctx, out_msg_ctx))

/** Invoke in out business logic async.
    @sa axis2_msg_recv_ops#invoke_in_out_business_logic_async */
#define AXIS2_MSG_RECV_INVOKE_IN_OUT_BUSINESS_LOGIC_ASYNC(msg_recv, env, \
            in_msg_ctx, out_msg_ctx, callback) \
        ((msg_recv)->ops->invoke_in_out_business_logic_async(msg_recv, env, \
            in_msg_ctx, out_msg_ctx, callback))

/** Make new svc obj.
    @sa axis2_msg_recv_ops#make_new_svc_obj */
#define AXIS2_MSG_RECV_MAKE_NEW_SVC_OBJ(msg_recv, env, msg_ctx) \
        ((msg_recv)->ops->make_new_svc_obj(msg_recv, env, msg_ctx))

/** Get impl obj.
    @sa axis2_msg_recv_ops#get_impl_obj */
#define AXIS2_MSG_RECV_GET_IMPL_OBJ(msg_recv, env, msg_ctx) \
      ((msg_recv)->ops->get_impl_obj(msg_recv, env, msg_ctx))

/** Set scope.
    @sa axis2_msg_recv_ops#set_scope */
#define AXIS2_MSG_RECV_SET_SCOPE(msg_recv, env, scope) \
      ((msg_recv)->ops->set_scope(msg_recv, env, scope))

/** Get scope.
    @sa axis2_msg_recv_ops#get_scope */
#define AXIS2_MSG_RECV_GET_SCOPE(msg_recv, env) \
        ((msg_recv)->ops->get_scope(msg_recv, env))

/** Delete svc obj.
    @sa axis2_msg_recv_ops#delete_svc_obj */
#define AXIS2_MSG_RECV_DELETE_SVC_OBJ(msg_recv, env, msg_ctx) \
        ((msg_recv)->ops->delete_svc_obj(msg_recv, env, msg_ctx))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif  /* AXIS2_MSG_RECV_H */
