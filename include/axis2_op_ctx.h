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

#ifndef AXIS2_OP_CTX_H
#define AXIS2_OP_CTX_H

/**
 * @defgroup axis2_op_ctx operation context 
 * An op context represents a running "instance" of an op, which is
 * represented by an axis2_op struct. This concept is needed to allow
 * messages to be grouped into ops as in WSDL 2.0-speak ops are
 * essentially arbitrary message exchange patterns. So as messages are being
 * exchanged the op context remembers the state of where in the message
 * exchange pattern it is in.
 * The base implementation of op context
 * supports MEPs which have one input message and/or one output message. That
 * is, it supports the all the MEPs that are in the WSDL 2.0 specification. In
 * order to support another MEP one must extend this struct.
 * @ingroup axis2_context
 * @{
 */

/**
 * @file axis2_op_ctx.h
 * @brief axis2 Message Context interface
 */

#include <axis2_defines.h>
#include <axis2_hash.h>
#include <axis2_env.h>
#include <axis2_msg_ctx.h>
#include <axis2_op.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_op_ctx_ops */
    typedef struct axis2_op_ctx_ops axis2_op_ctx_ops_t;
    /** Type name for struct axis2_op_ctx */
    typedef struct axis2_op_ctx axis2_op_ctx_t;
    struct axis2_svc_ctx;

    /**
     * @brief Message Context ops struct
     * Encapsulator struct for ops of axis2_op_ctx
     */
    struct axis2_op_ctx_ops
    {
   /**
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
    */
        axis2_ctx_t* (AXIS2_CALL *
                get_base)(
          struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env);

   /**
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
    */
        axis2_status_t (AXIS2_CALL *
                free)(
          struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env);

        /**
         * The method is used to do the intialization of the axis2_op_ctx
    * @param op_ctx pointer to operation context
    * @param env pointer to enviornment struct
    * @param conf pointer to conf 
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env,
                    struct axis2_conf *conf);

        /**
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
         * @return Returns the op.
         */
        struct axis2_op* (AXIS2_CALL *
                get_op)(
                    struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env);

        /**
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
         * @return Return the struct axis2_svc_ctx * in which this op_ctx lives.
         */
        struct axis2_svc_ctx* (AXIS2_CALL *
                get_parent)(
           struct axis2_op_ctx *op_ctx,
                     const axis2_env_t *env);

        /**
         * When a new message is added to the <code>MEPContext</code> the logic
         * should be included remove the MEPContext from the table in the
         * <code>axis2_conf_ctx</code>. Example: IN_IN_OUT At the second IN
         * message the MEPContext should be removed from the AxisOperation
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
    * @param msg_ctx pointer to message context
         *
         */
        axis2_status_t (AXIS2_CALL *
                add_msg_ctx)(
               struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);

        /**
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
    * @param message_id ponter to message id
         */
        axis2_msg_ctx_t* (AXIS2_CALL *
                get_msg_ctx)(
          struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *message_id);

        /**
         * Checks to see if the MEP is complete. i.e. whether all the messages that
         * are associated with the MEP has arrived and MEP is complete.
         * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                get_is_complete)(
                    struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env);

   /**
    * @param op_ctx pointer to operatoin context
    * @param env pointer to environment struct
    * @param is_complete is_complete
    */
        axis2_status_t (AXIS2_CALL *
                set_complete)(
          struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t is_complete);

        /**
         * Removes the pointers to this <code>op_ctx</code> in the
         * <code>axis2_conf_ctx</code>'s op_ctxaxis2_hash_t *so that this
         * <code>op_ctx</code> will eventually get garbage collected
         * along with the <code>axis2_msg_ctx_t *</code>'s it contains. Note that if
         * the caller wants to make sure its safe to clean up this op_ctx
         * he should call is_complete() first. However, in cases like IN_OPTIONAL_OUT
         * and OUT_OPTIONAL_IN, it is possibe this will get called without the MEP
         * being complete due to the optional nature of the MEP.
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                cleanup)(
          struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env);

   /**
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
    * @param svc_ctx pointer to service context
    */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
          struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env,
                    struct axis2_svc_ctx *svc_ctx);

   /**
    * @param op_ctx pointer to operation context
    * @param env pointer to environment struct
    */
        axis2_hash_t* (AXIS2_CALL *
                get_msg_ctx_map)(
          struct axis2_op_ctx *op_ctx,
                    const axis2_env_t *env);

    };

    /**
     * operation context struct
     */
    struct axis2_op_ctx
    {
   /** operatoins of operation context struct */
        axis2_op_ctx_ops_t *ops;
    };

    /**
     * Creates a operation context struct
     * @param env pointer to environment struct
     * @param op pointer to operation
     * @param svc_ctx pointer to service context
     */
    AXIS2_EXTERN axis2_op_ctx_t* AXIS2_CALL
    axis2_op_ctx_create(
       const axis2_env_t *env,
            struct axis2_op *op,
            struct axis2_svc_ctx * svc_ctx);

/************************** Start of function macros **************************/

/** Gets the base.
    @sa axis2_op_ctx_ops#get_base */
#define AXIS2_OP_CTX_GET_BASE(op_ctx, env) \
      ((op_ctx)->ops->get_base(op_ctx, env))

/** Frees teh operation context.
    @sa axis2_op_ctx_ops#free */
#define AXIS2_OP_CTX_FREE(op_ctx, env)\
      ((op_ctx)->ops->free(op_ctx, env))

/** Initialize the operation context.
    @sa axis2_op_ctx_ops#init */
#define AXIS2_OP_CTX_INIT(op_ctx, env, conf) \
      ((op_ctx)->ops->init(op_ctx, env, conf))

/** Gets the operaton.
    @sa axis2_op_ctx_ops#get_op */
#define AXIS2_OP_CTX_GET_OP(op_ctx, env)\
      ((op_ctx)->ops->get_op(op_ctx, env))

/** Gets the parent.
    @sa axis2_op_ctx_ops#get_parent */
#define AXIS2_OP_CTX_GET_PARENT(op_ctx, env) \
      ((op_ctx)->ops->get_parent(op_ctx, env))

/** Adds the message context.
    @sa axis2_op_ctx_ops#add_msg_ctx */
#define AXIS2_OP_CTX_ADD_MSG_CTX(op_ctx, env, msg_ctx) \
      ((op_ctx)->ops->add_msg_ctx(op_ctx, env, msg_ctx))

/** Gets the message context.
    @sa axis2_op_ctx_ops#get_msg_ctx */
#define AXIS2_OP_CTX_GET_MSG_CTX(op_ctx, env, message_id) \
      ((op_ctx)->ops->get_msg_ctx(op_ctx, env, message_id))

/** Get is complete.
    @sa axis2_op_ctx_ops#get_is_complete */
#define AXIS2_OP_CTX_GET_IS_COMPLETE(op_ctx, env) \
      ((op_ctx)->ops->get_is_complete(op_ctx, env))

/** Set is complete.
    @sa axis2_op_ctx_ops#set_is_complete */
#define AXIS2_OP_CTX_SET_IS_COMPLETE(op_ctx, env, is_complete) \
      ((op_ctx)->ops->set_complete(op_ctx, env, is_complete))

/** Cleanup.
    @sa axis2_op_ctx_ops#cleanup */
#define AXIS2_OP_CTX_CLEANUP(op_ctx, env) \
      ((op_ctx)->ops->cleanup(op_ctx, env))

/** Set parent.
    @sa axis2_op_ctx_ops#set_parent */
#define AXIS2_OP_CTX_SET_PARENT(op_ctx, env, svc_ctx) \
      ((op_ctx)->ops->set_parent(op_ctx, env, svc_ctx))

/** Gets the message context map.
    @sa axis2_op_ctx_ops#get_msg_ctx */
#define AXIS2_OP_CTX_GET_MSG_CTX_MAP(op_ctx, env) \
      ((op_ctx)->ops->get_msg_ctx_map(op_ctx, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OP_CTX_H */
