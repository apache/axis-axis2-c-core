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

#include <axis2_operation_ctx.h>
#include <axis2.h>
#include <axis2_hash.h>

typedef struct axis2_operation_ctx_impl
{
    /** context base struct */
    axis2_operation_ctx_t operation_ctx;
    /** base context struct */
    axis2_ctx_t *base;
    /** parent of operation context is a service context instance */
    struct axis2_svc_ctx *parent;
    /** message context map */
    axis2_hash_t *msg_ctx_map;
    /** the operation of which this is a running instance. The MEP of this
     * operation must be one of the 8 predefined ones in WSDL 2.0.
     */
    axis2_operation_t *operation;
    /** operation Message Exchange Pattern */
    int operation_mep;
    /** is complete? */
    axis2_bool_t is_complete;
    /** the global message_id -> operation_ctx map which is stored in
     * the axis2_conf_ctx. We're caching it here for faster acccess.
     */
    axis2_hash_t *operation_ctx_map;
    /** operation qname */
    axis2_qname_t *operation_qname;
    /** service qname */
    axis2_qname_t *svc_qname;
} axis2_operation_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(operation_ctx) ((axis2_operation_ctx_impl_t *)operation_ctx)

axis2_ctx_t* AXIS2_CALL axis2_operation_ctx_get_base(struct axis2_operation_ctx *operation_ctx, 
                                            axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_operation_ctx_free (struct axis2_operation_ctx *operation_ctx, 
                                   axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_operation_ctx_init(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env, 
    struct axis2_engine_config *engine_config);
axis2_operation_t* AXIS2_CALL axis2_operation_ctx_getAxisOperation(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env);
struct axis2_svc_ctx * AXIS2_CALL axis2_operation_ctx_get_parent(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_operation_ctx_add_msg_ctx(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env, 
    axis2_msg_ctx_t *msg_ctx);
axis2_msg_ctx_t* AXIS2_CALL axis2_operation_ctx_get_msg_ctx(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env, axis2_char_t *messageLabel);
axis2_bool_t AXIS2_CALL axis2_operation_ctx_get_is_complete(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_operation_ctx_set_complete(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env, 
    axis2_bool_t is_complete);
axis2_status_t AXIS2_CALL axis2_operation_ctx_cleanup(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_operation_ctx_set_parent(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env, 
    struct axis2_svc_ctx* svc_ctx);
axis2_operation_t* AXIS2_CALL axis2_operation_ctx_get_operation(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env);
axis2_hash_t* AXIS2_CALL axis2_operation_ctx_get_msg_ctx_map(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env);

axis2_operation_ctx_t* AXIS2_CALL axis2_operation_ctx_create(axis2_env_t **env, 
    axis2_operation_t *operation,
    struct axis2_svc_ctx* svc_ctx) 
{
    axis2_operation_ctx_impl_t *operation_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    operation_ctx_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_operation_ctx_impl_t) );
    if (!operation_ctx_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    operation_ctx_impl->operation_ctx.ops = NULL;
    operation_ctx_impl->base = NULL;
    operation_ctx_impl->parent = NULL;
    operation_ctx_impl->msg_ctx_map = NULL;
    operation_ctx_impl->operation = NULL;
    operation_ctx_impl->operation_mep = 0;
    operation_ctx_impl->is_complete = AXIS2_FALSE;
    operation_ctx_impl->operation_ctx_map = NULL;
    operation_ctx_impl->operation_qname = NULL;
    operation_ctx_impl->svc_qname = NULL;
    
    operation_ctx_impl->base = axis2_ctx_create(env);
    if (!(operation_ctx_impl->base))
    {
        axis2_operation_ctx_free(&(operation_ctx_impl->operation_ctx), env);
        return NULL;
    }
    
    if (operation)
    {
        operation_ctx_impl->operation = operation;
    }
    
    
    operation_ctx_impl->msg_ctx_map = axis2_hash_make(env);
    if (!(operation_ctx_impl->msg_ctx_map))
    {
        axis2_operation_ctx_free(&(operation_ctx_impl->operation_ctx), env);
        return NULL;
    }
    
    if (operation_ctx_impl->operation)
    {
        /*operation_ctx_impl->operation_qname = AXIS2_OPERATION_GET_QNAME(operation_ctx_impl->operation, env);
        operation_ctx_impl->operation_mep = AXIS2_OPERATION_GET_MEP_CONST(operation_ctx_impl->operation, env);*/
    }
    
    axis2_operation_ctx_set_parent(&(operation_ctx_impl->operation_ctx), env, svc_ctx);
    
    /* initialize operations */    
    operation_ctx_impl->operation_ctx.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_operation_ctx_ops_t) );
    if (!operation_ctx_impl->operation_ctx.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_operation_ctx_free(&(operation_ctx_impl->operation_ctx), env);
        return NULL;        
    }

    operation_ctx_impl->operation_ctx.ops->get_base = axis2_operation_ctx_get_base;
    operation_ctx_impl->operation_ctx.ops->free = axis2_operation_ctx_free;
    operation_ctx_impl->operation_ctx.ops->init = axis2_operation_ctx_init;
    operation_ctx_impl->operation_ctx.ops->get_operation = axis2_operation_ctx_get_operation;
    operation_ctx_impl->operation_ctx.ops->get_parent = axis2_operation_ctx_get_parent;
    operation_ctx_impl->operation_ctx.ops->add_msg_ctx = axis2_operation_ctx_add_msg_ctx;
    operation_ctx_impl->operation_ctx.ops->get_msg_ctx = axis2_operation_ctx_get_msg_ctx;
    operation_ctx_impl->operation_ctx.ops->get_is_complete = axis2_operation_ctx_get_is_complete;
    operation_ctx_impl->operation_ctx.ops->set_complete = axis2_operation_ctx_set_complete;
    operation_ctx_impl->operation_ctx.ops->cleanup = axis2_operation_ctx_cleanup;
    operation_ctx_impl->operation_ctx.ops->set_parent = axis2_operation_ctx_set_parent;
    operation_ctx_impl->operation_ctx.ops->get_msg_ctx_map = axis2_operation_ctx_get_msg_ctx_map;

    return &(operation_ctx_impl->operation_ctx);
}

axis2_ctx_t* AXIS2_CALL axis2_operation_ctx_get_base(struct axis2_operation_ctx *operation_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation_ctx)->base;
}


axis2_status_t AXIS2_CALL axis2_operation_ctx_free (struct axis2_operation_ctx *operation_ctx, 
                                   axis2_env_t **env)
{
    axis2_operation_ctx_impl_t *operation_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, AXIS2_FAILURE);
    
    operation_ctx_impl = AXIS2_INTF_TO_IMPL(operation_ctx);
    
    if (operation_ctx_impl->operation_ctx.ops)
    {
        AXIS2_FREE((*env)->allocator, operation_ctx_impl->operation_ctx.ops);
        operation_ctx_impl->operation_ctx.ops = NULL;
    }
    
    if (operation_ctx_impl->base)
    {
        AXIS2_CTX_FREE(operation_ctx_impl->base, env);
        operation_ctx_impl->base = NULL;
    }
    
    if (operation_ctx_impl->msg_ctx_map)
    {
        axis2_hash_free(operation_ctx_impl->msg_ctx_map, env);
        operation_ctx_impl->msg_ctx_map = NULL;
    }    
    
    AXIS2_FREE((*env)->allocator, operation_ctx_impl);
    operation_ctx_impl = NULL;
    
    return AXIS2_SUCCESS;
}

/**
 * The method is used to do the intialization of the axis2_operation_ctx
 */
axis2_status_t AXIS2_CALL axis2_operation_ctx_init(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env, struct axis2_engine_config *engine_config)
{
    axis2_operation_ctx_impl_t *operation_ctx_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    void *ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, AXIS2_FAILURE);
    
    operation_ctx_impl = AXIS2_INTF_TO_IMPL(operation_ctx);
    
    if (operation_ctx_impl->operation_qname && operation_ctx_impl->svc_qname)
    {
        axis2_svc_t *svc = NULL;
        axis2_char_t *svc_name = NULL;
        
        svc_name = AXIS2_QNAME_GET_LOCALPART(operation_ctx_impl->svc_qname, env);
        
        if (svc_name)
        {
            svc = AXIS2_ENGINE_CONFIG_GET_SVC(engine_config, env, svc_name);
            
            if (svc)
            {
               /* operation_ctx_impl->operation = AXIS2_SVC_GET_OPERATION_WITH_QNAME(svc, env, operation_ctx_impl->operation_qname);*/
            }
        }
    }

    if (!(operation_ctx_impl->msg_ctx_map))
        return AXIS2_SUCCESS;
    
    for (hi = axis2_hash_first (operation_ctx_impl->msg_ctx_map, env);
             hi; hi = axis2_hash_next (env, hi))
    {
        axis2_hash_this (hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_msg_ctx_t *msg_ctx = (axis2_msg_ctx_t*)ctx;
            AXIS2_MSG_CTX_INIT(msg_ctx, env, engine_config);
        }
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @return Returns the operation.
 */
axis2_operation_t* AXIS2_CALL axis2_operation_ctx_get_operation(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation_ctx)->operation;
}

/**
 * Return the struct axis2_svc_ctx * in which this operation_ctx lives.
 *
 * @return parent struct axis2_svc_ctx *
 */
struct axis2_svc_ctx * AXIS2_CALL axis2_operation_ctx_get_parent(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation_ctx)->parent;
}

/**
 * When a new message is added to the <code>MEPContext</code> the logic
 * should be included remove the MEPContext from the table in the
 * <code>axis2_conf_ctx</code>. Example: IN_IN_OUT At the second IN
 * message the MEPContext should be removed from the AxisOperation
 *
 * @param msgContext
 */
axis2_status_t AXIS2_CALL axis2_operation_ctx_add_msg_ctx(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)  
{
    axis2_operation_ctx_impl_t *operation_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, AXIS2_FAILURE);
    
    operation_ctx_impl = AXIS2_INTF_TO_IMPL(operation_ctx);
    
        
    if(operation_ctx_impl->operation)
    {
        /*return AXIS2_OPERATION_ADD_MSG_CTX(operation_ctx_impl->operation, env, msg_ctx, operation_ctx);*/
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @param messageLabel
 * @return
 * @
 */

axis2_msg_ctx_t* AXIS2_CALL axis2_operation_ctx_get_msg_ctx(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env, axis2_char_t *message_id)
{
    axis2_operation_ctx_impl_t *operation_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, NULL);
    
    operation_ctx_impl = AXIS2_INTF_TO_IMPL(operation_ctx);
    
    if (operation_ctx_impl->msg_ctx_map)
    {
        return axis2_hash_get(operation_ctx_impl->msg_ctx_map, message_id, AXIS2_HASH_KEY_STRING); 
    }
    
    return NULL;
}

/**
 * Checks to see if the MEP is complete. i.e. whether all the messages that
 * are associated with the MEP has arrived and MEP is complete.
 *
 */
axis2_bool_t AXIS2_CALL axis2_operation_ctx_get_is_complete(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(operation_ctx)->is_complete;
}

axis2_status_t AXIS2_CALL axis2_operation_ctx_set_complete(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env, 
    axis2_bool_t is_complete) 
{
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(operation_ctx)->is_complete = is_complete;
    return AXIS2_SUCCESS;
}

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
axis2_status_t AXIS2_CALL axis2_operation_ctx_cleanup(struct axis2_operation_ctx *operation_ctx, 
    axis2_env_t **env) 
{
    axis2_operation_ctx_impl_t *operation_ctx_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    void *ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, AXIS2_FAILURE);
    
    operation_ctx_impl = AXIS2_INTF_TO_IMPL(operation_ctx);

    if (!(operation_ctx_impl->msg_ctx_map))
        return AXIS2_SUCCESS;
    
    for (hi = axis2_hash_first (operation_ctx_impl->msg_ctx_map, env);
             hi; hi = axis2_hash_next (env, hi))
    {
        axis2_hash_this (hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_msg_ctx_t *msg_ctx = (axis2_msg_ctx_t*)ctx;
            /*axis2_char_t *message_id = AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env);*/
            /*if (message_id)
            {
                AXIS2_HASH_SET(operation_ctx_impl->msg_ctx_map, env, message_id, AXIS2_HASH_KEY_STRING, NULL);
                return AXIS2_MSG_CTX_FREE(msg_ctx, env);
            }*/
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_operation_ctx_set_parent(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env, struct axis2_svc_ctx* svc_ctx) 
{
    axis2_operation_ctx_impl_t *operation_ctx_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_msg_ctx_t *ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, AXIS2_FAILURE);
    
    operation_ctx_impl = AXIS2_INTF_TO_IMPL(operation_ctx);
    
    if (svc_ctx)
    {
        operation_ctx_impl->parent = svc_ctx;
    }
    
    if (operation_ctx_impl->parent) /* that is if there is a service context associated */
    {
        /*axis2_conf_ctx_t *conf_ctx = NULL;
        conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(operation_ctx_impl->parent, env);
        if (conf_ctx)
        {
            operation_ctx_impl->operation_ctx_map = AXIS2_CONF_CTX_GET_OPERATION_CTX_MAP(conf_ctx, env);
        }
        operation_ctx_impl->svc_qname = AXIS2_SVC_GET_QNAME(operation_ctx_impl->parent, env);*/
    }
    
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL axis2_operation_ctx_get_msg_ctx_map(struct axis2_operation_ctx *operation_ctx, axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation_ctx)->msg_ctx_map;
}
