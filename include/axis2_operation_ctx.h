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

#ifndef AXIS2_OPERATION_CTX_H
#define AXIS2_OPERATION_CTX_H


/**
  * @file axis2_operation_ctx.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_hash.h>
#include <axis2_env.h>
#include <axis2_msg_ctx.h>
#include <axis2_operation.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * @defgroup axis2_operation_ctx Operation Context 
 * An operation context represents a running "instance" of an operation, which is
 * represented by an axis2_operation struct. This concept is needed to allow
 * messages to be grouped into operations as in WSDL 2.0-speak operations are
 * essentially arbitrary message exchange patterns. So as messages are being
 * exchanged the operation context remembers the state of where in the message
 * exchange pattern it is in.
 * <p/>
 * The base implementation of operation context
 * supports MEPs which have one input message and/or one output message. That
 * is, it supports the all the MEPs that are in the WSDL 2.0 specification. In
 * order to support another MEP one must extend this struct.
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_operation_ctx_ops axis2_operation_ctx_ops_t;
typedef struct axis2_operation_ctx axis2_operation_ctx_t; 
struct axis2_svc_ctx;
    
/** 
 * @brief Message Context operations struct
 * Encapsulator struct for operations of axis2_operation_ctx
 */  
struct axis2_operation_ctx_ops
{
    axis2_ctx_t* (AXIS2_CALL *get_base)(struct axis2_operation_ctx *operation_ctx, 
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *free)(struct axis2_operation_ctx *operation_ctx, 
                                       axis2_env_t **env);
    
    /**
     * The method is used to do the intialization of the axis2_operation_ctx
     */
    axis2_status_t (AXIS2_CALL *init)(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env, struct axis2_engine_config *engine_config);
    
    /**
     * @return Returns the operation.
     */
    axis2_operation_t* (AXIS2_CALL *get_operation)(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env);
    
    /**
     * Return the struct axis2_svc_ctx * in which this operation_ctx lives.
     *
     * @return parent struct axis2_svc_ctx *
     */
    struct axis2_svc_ctx* (AXIS2_CALL *get_parent)(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env);
    
    /**
     * When a new message is added to the <code>MEPContext</code> the logic
     * should be included remove the MEPContext from the table in the
     * <code>axis2_conf_ctx</code>. Example: IN_IN_OUT At the second IN
     * message the MEPContext should be removed from the AxisOperation
     *
     * @param msgContext
     */
    axis2_status_t (AXIS2_CALL *add_msg_ctx)(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
    
    /**
     * @param message_id
     * @return
     * @throws AxisFault
     */
    axis2_msg_ctx_t* (AXIS2_CALL *get_msg_ctx)(struct axis2_operation_ctx *operation_ctx, 
        axis2_env_t **env, axis2_char_t *message_id);
    
    /**
     * Checks to see if the MEP is complete. i.e. whether all the messages that
     * are associated with the MEP has arrived and MEP is complete.
     *
     */
    axis2_bool_t (AXIS2_CALL *get_is_complete)(struct axis2_operation_ctx *operation_ctx, 
        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_complete)(struct axis2_operation_ctx *operation_ctx, 
        axis2_env_t **env, 
        axis2_bool_t is_complete);
    
    /**
     * Removes the pointers to this <code>operation_ctx</code> in the
     * <code>axis2_conf_ctx</code>'s operation_ctxaxis2_hash_t *so that this
     * <code>operation_ctx</code> will eventually get garbage collected
     * along with the <code>axis2_msg_ctx_t *</code>'s it contains. Note that if
     * the caller wants to make sure its safe to clean up this operation_ctx
     * he should call is_complete() first. However, in cases like IN_OPTIONAL_OUT
     * and OUT_OPTIONAL_IN, it is possibe this will get called without the MEP
     * being complete due to the optional nature of the MEP.
     */
    axis2_status_t (AXIS2_CALL *cleanup)(struct axis2_operation_ctx *operation_ctx, 
        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_parent)(struct axis2_operation_ctx *operation_ctx, 
        axis2_env_t **env, 
        struct axis2_svc_ctx *svc_ctx);
    
    axis2_hash_t* (AXIS2_CALL *get_msg_ctx_map)(struct axis2_operation_ctx *operation_ctx, 
        axis2_env_t **env);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_operation_ctx
{
    axis2_operation_ctx_ops_t *ops;    
};

axis2_operation_ctx_t* AXIS2_CALL axis2_operation_ctx_create(axis2_env_t **env, 
    axis2_operation_t *operation,
    struct axis2_svc_ctx * svc_ctx);
    
/************************** Start of function macros **************************/
#define OPERATION_CTX_GET_BASE(operation_ctx, env) ((operation_ctx)->ops->get_base(operation_ctx, env))
#define AXIS2_OPERATION_CTX_FREE(operation_ctx, env) ((operation_ctx)->ops->free(operation_ctx, env))
#define AXIS2_OPERATION_CTX_INIT(operation_ctx, env, engine_config) ((operation_ctx)->ops->init(operation_ctx, env, engine_config))
#define AXIS2_OPERATION_CTX_GET_OPERATION(operation_ctx, env) ((operation_ctx)->ops->get_operation(operation_ctx, env))
#define AXIS2_OPERATION_CTX_GET_PARENT(operation_ctx, env) ((operation_ctx)->ops->get_parent(operation_ctx, env))
#define AXIS2_OPERATION_CTX_ADD_MSG_CTX(operation_ctx, env, msg_ctx) ((operation_ctx)->ops->add_msg_ctx(operation_ctx, env, msg_ctx))
#define AXIS2_OPERATION_CTX_GET_MSG_CTX(operation_ctx, env, message_id) ((operation_ctx)->ops->get_msg_ctx(operation_ctx, env, message_id))
#define AXIS2_OPERATION_CTX_GET_IS_COMPLETE(operation_ctx, env) ((operation_ctx)->ops->get_is_complete(operation_ctx, env))
#define AXIS2_OPERATION_CTX_SET_IS_COMPLETE(operation_ctx, env, is_complete) ((operation_ctx)->ops->set_is_complete(operation_ctx, env, is_complete))
#define AXIS2_OPERATION_CTX_CLEANUP(operation_ctx, env) ((operation_ctx)->ops->cleanup(operation_ctx, env))
#define AXIS2_OPERATION_CTX_SET_PARENT(operation_ctx, env, svc_ctx) ((operation_ctx)->ops->set_parent(operation_ctx, env, svc_ctx))
#define AXIS2_OPERATION_CTX_GET_MSG_CTX_MAP(operation_ctx, env) ((operation_ctx)->ops->get_msg_ctx_map(operation_ctx, env))    

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OPERATION_CTX_H */
