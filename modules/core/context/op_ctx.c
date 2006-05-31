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

#include <axis2_op_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_op.h>
#include <axis2_const.h>
#include <axis2_hash.h>

typedef struct axis2_op_ctx_impl
{
    /** context base struct */
    axis2_op_ctx_t op_ctx;
    /** base context struct */
    axis2_ctx_t *base;
    /** parent of op context is a service context instance */
    struct axis2_svc_ctx *parent;
    /** message context map */
    axis2_hash_t *msg_ctx_map;
    /** the op of which this is a running instance. The MEP of this
     * op must be one of the 8 predefined ones in WSDL 2.0.
     */
    axis2_op_t *op;
    /** op Message Exchange Pattern */
    int op_mep;
    /** is complete? */
    axis2_bool_t is_complete;
    /** the global message_id -> op_ctx map which is stored in
     * the axis2_conf_ctx. We're caching it here for faster acccess.
     */
    axis2_hash_t *op_ctx_map;
    /** op qname */
    axis2_qname_t *op_qname;
    /** service qname */
    axis2_qname_t *svc_qname;
	/* Mutex to syncronize the read/write operations */
	axis2_thread_mutex_t *mutex;
} axis2_op_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(op_ctx) ((axis2_op_ctx_impl_t *)op_ctx)


axis2_ctx_t* AXIS2_CALL 
axis2_op_ctx_get_base(struct axis2_op_ctx *op_ctx, 
                      const axis2_env_t *env);
                      
axis2_status_t AXIS2_CALL 
axis2_op_ctx_free (struct axis2_op_ctx *op_ctx, 
                   const axis2_env_t *env);
                   
axis2_status_t AXIS2_CALL 
axis2_op_ctx_init(struct axis2_op_ctx *op_ctx, 
                 const axis2_env_t *env, 
                 struct axis2_conf *conf);
                 
axis2_op_t* AXIS2_CALL 
axis2_op_ctx_getAxisOperation(struct axis2_op_ctx *op_ctx, 
                             const axis2_env_t *env);

                             
struct axis2_svc_ctx * AXIS2_CALL 
axis2_op_ctx_get_parent(struct axis2_op_ctx *op_ctx, 
                        const axis2_env_t *env);

                        
axis2_status_t AXIS2_CALL 
axis2_op_ctx_add_msg_ctx(struct axis2_op_ctx *op_ctx, 
                        const axis2_env_t *env, 
                        axis2_msg_ctx_t *msg_ctx);

                        
axis2_msg_ctx_t* AXIS2_CALL 
axis2_op_ctx_get_msg_ctx(struct axis2_op_ctx *op_ctx, 
                        const axis2_env_t *env, 
                        axis2_char_t *messageLabel);
                        
axis2_bool_t AXIS2_CALL 
axis2_op_ctx_get_is_complete(struct axis2_op_ctx *op_ctx, 
                            const axis2_env_t *env);
                            
axis2_status_t AXIS2_CALL 
axis2_op_ctx_set_complete(struct axis2_op_ctx *op_ctx, 
                          const axis2_env_t *env, 
                          axis2_bool_t is_complete);
                          
axis2_status_t AXIS2_CALL 
axis2_op_ctx_cleanup(struct axis2_op_ctx *op_ctx, 
                     const axis2_env_t *env);
                     
axis2_status_t AXIS2_CALL 
axis2_op_ctx_set_parent(struct axis2_op_ctx *op_ctx, 
                        const axis2_env_t *env, 
                        struct axis2_svc_ctx* svc_ctx);

                        
axis2_op_t* AXIS2_CALL 
axis2_op_ctx_get_op(struct axis2_op_ctx *op_ctx, 
                    const axis2_env_t *env);
                    
axis2_hash_t* AXIS2_CALL 
axis2_op_ctx_get_msg_ctx_map(struct axis2_op_ctx *op_ctx, 
                             const axis2_env_t *env);



AXIS2_DECLARE(axis2_op_ctx_t*)
axis2_op_ctx_create(const axis2_env_t *env, 
                    axis2_op_t *op,
                    struct axis2_svc_ctx* svc_ctx) 
{
    axis2_op_ctx_impl_t *op_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    op_ctx_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_op_ctx_impl_t) );
    if (!op_ctx_impl)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    op_ctx_impl->op_ctx.ops = NULL;
    op_ctx_impl->base = NULL;
    op_ctx_impl->parent = NULL;
    op_ctx_impl->msg_ctx_map = NULL;
    op_ctx_impl->op = NULL;
    op_ctx_impl->op_mep = 0;
    op_ctx_impl->is_complete = AXIS2_FALSE;
    op_ctx_impl->op_ctx_map = NULL;
    op_ctx_impl->op_qname = NULL;
    op_ctx_impl->svc_qname = NULL;
    op_ctx_impl->mutex = axis2_thread_mutex_create(env->allocator, 
						AXIS2_THREAD_MUTEX_DEFAULT);
	
	if(NULL == op_ctx_impl->mutex)
	{
		axis2_op_ctx_free(&(op_ctx_impl->op_ctx), env);
        return NULL;
	}
	
    op_ctx_impl->base = axis2_ctx_create(env);
    if (!(op_ctx_impl->base))
    {
        axis2_op_ctx_free(&(op_ctx_impl->op_ctx), env);
        return NULL;
    }
    
    if (op)
    {
        op_ctx_impl->op = op;
    }
    
    
    op_ctx_impl->msg_ctx_map = axis2_hash_make(env);
    if (!(op_ctx_impl->msg_ctx_map))
    {
        axis2_op_ctx_free(&(op_ctx_impl->op_ctx), env);
        return NULL;
    }
    
    if (op_ctx_impl->op)
    {
        op_ctx_impl->op_qname = AXIS2_OP_GET_QNAME(op_ctx_impl->op, env);
        op_ctx_impl->op_mep = AXIS2_OP_GET_AXIS_SPECIFIC_MEP_CONST(op_ctx_impl->op, env);
    }
    
    axis2_op_ctx_set_parent(&(op_ctx_impl->op_ctx), env, svc_ctx);
    
    /* initialize ops */    
    op_ctx_impl->op_ctx.ops  = AXIS2_MALLOC( env->allocator, sizeof(axis2_op_ctx_ops_t) );
    if (!op_ctx_impl->op_ctx.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_op_ctx_free(&(op_ctx_impl->op_ctx), env);
        return NULL;        
    }

    op_ctx_impl->op_ctx.ops->get_base = 
        axis2_op_ctx_get_base;
        
    op_ctx_impl->op_ctx.ops->free = 
        axis2_op_ctx_free;
        
    op_ctx_impl->op_ctx.ops->init = 
        axis2_op_ctx_init;
        
    op_ctx_impl->op_ctx.ops->get_op = 
        axis2_op_ctx_get_op;
        
    op_ctx_impl->op_ctx.ops->get_parent = 
        axis2_op_ctx_get_parent;
        
    op_ctx_impl->op_ctx.ops->add_msg_ctx = 
        axis2_op_ctx_add_msg_ctx;
        
    op_ctx_impl->op_ctx.ops->get_msg_ctx = 
        axis2_op_ctx_get_msg_ctx;
        
    op_ctx_impl->op_ctx.ops->get_is_complete = 
        axis2_op_ctx_get_is_complete;
        
    op_ctx_impl->op_ctx.ops->set_complete = 
        axis2_op_ctx_set_complete;
        
    op_ctx_impl->op_ctx.ops->cleanup = 
        axis2_op_ctx_cleanup;
        
    op_ctx_impl->op_ctx.ops->set_parent = 
        axis2_op_ctx_set_parent;
        
    op_ctx_impl->op_ctx.ops->get_msg_ctx_map = 
        axis2_op_ctx_get_msg_ctx_map;
        

    return &(op_ctx_impl->op_ctx);
}

axis2_ctx_t* AXIS2_CALL 
axis2_op_ctx_get_base(struct axis2_op_ctx *op_ctx, 
                      const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op_ctx)->base;
}


axis2_status_t AXIS2_CALL 
axis2_op_ctx_free (struct axis2_op_ctx *op_ctx, 
                   const axis2_env_t *env)
{
    axis2_op_ctx_impl_t *op_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_ctx_impl = AXIS2_INTF_TO_IMPL(op_ctx);
    
    if (op_ctx_impl->op_ctx.ops)
    {
        AXIS2_FREE(env->allocator, op_ctx_impl->op_ctx.ops);
        op_ctx_impl->op_ctx.ops = NULL;
    }
    
    if (op_ctx_impl->base)
    {
        AXIS2_CTX_FREE(op_ctx_impl->base, env);
        op_ctx_impl->base = NULL;
    }
    
    if (op_ctx_impl->msg_ctx_map)
    {
        axis2_hash_free(op_ctx_impl->msg_ctx_map, env);
        op_ctx_impl->msg_ctx_map = NULL;
    }    
    if(op_ctx_impl->mutex)
    {
        axis2_thread_mutex_destroy(op_ctx_impl->mutex);
        op_ctx_impl->mutex = NULL;
    }
    
    AXIS2_FREE(env->allocator, op_ctx_impl);
    op_ctx_impl = NULL;
    
    return AXIS2_SUCCESS;
}

/**
 * The method is used to do the intialization of the axis2_op_ctx
 */
axis2_status_t AXIS2_CALL 
axis2_op_ctx_init(struct axis2_op_ctx *op_ctx, 
                  const axis2_env_t *env, 
                  struct axis2_conf *conf)
{
    axis2_op_ctx_impl_t *op_ctx_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    void *ctx = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_ctx_impl = AXIS2_INTF_TO_IMPL(op_ctx);
    
    if (op_ctx_impl->op_qname && op_ctx_impl->svc_qname)
    {
        axis2_svc_t *svc = NULL;
        axis2_char_t *svc_name = NULL;
        
        svc_name = AXIS2_QNAME_GET_LOCALPART(op_ctx_impl->svc_qname, env);
        
        if (svc_name)
        {
            svc = AXIS2_CONF_GET_SVC(conf, env, svc_name);
            
            if (svc)
            {
                op_ctx_impl->op = 
                    AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_ctx_impl->op_qname);
            }
        }
    }

    if (!(op_ctx_impl->msg_ctx_map))
        return AXIS2_SUCCESS;
    
    for (hi = axis2_hash_first (op_ctx_impl->msg_ctx_map, env);
             hi; hi = axis2_hash_next (env, hi))
    {
        axis2_hash_this (hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_msg_ctx_t *msg_ctx = (axis2_msg_ctx_t*)ctx;
            AXIS2_MSG_CTX_INIT(msg_ctx, env, conf);
        }
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @return Returns the op.
 */
axis2_op_t* AXIS2_CALL 
axis2_op_ctx_get_op(struct axis2_op_ctx *op_ctx, 
                    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op_ctx)->op;
}

/**
 * Return the struct axis2_svc_ctx * in which this op_ctx lives.
 *
 * @return parent struct axis2_svc_ctx *
 */
struct axis2_svc_ctx * AXIS2_CALL 
axis2_op_ctx_get_parent(struct axis2_op_ctx *op_ctx, 
                        const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op_ctx)->parent;
}

/**
 * When a new message is added to the <code>MEPContext</code> the logic
 * should be included remove the MEPContext from the table in the
 * <code>axis2_conf_ctx</code>. Example: IN_IN_OUT At the second IN
 * message the MEPContext should be removed from the AxisOperation
 *
 * @param msgContext
 */
axis2_status_t AXIS2_CALL 
axis2_op_ctx_add_msg_ctx(struct axis2_op_ctx *op_ctx, 
                        const axis2_env_t *env, 
                        axis2_msg_ctx_t *msg_ctx)  
{
    axis2_op_ctx_impl_t *op_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_ctx_impl = AXIS2_INTF_TO_IMPL(op_ctx);
	
    axis2_thread_mutex_lock(op_ctx_impl->mutex);
    if (op_ctx_impl->msg_ctx_map)
    {
        axis2_char_t *message_id = AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env);
        if (message_id)
        {
            axis2_hash_set(op_ctx_impl->msg_ctx_map, 
                message_id, AXIS2_HASH_KEY_STRING, msg_ctx); 
        }
    }
    axis2_thread_mutex_unlock(op_ctx_impl->mutex);
    return AXIS2_SUCCESS;
}

/**
 * @param messageLabel
 * @return
 * @
 */

axis2_msg_ctx_t* AXIS2_CALL 
axis2_op_ctx_get_msg_ctx(struct axis2_op_ctx *op_ctx, 
                         const axis2_env_t *env, 
                         axis2_char_t *message_id)
{
    axis2_op_ctx_impl_t *op_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    op_ctx_impl = AXIS2_INTF_TO_IMPL(op_ctx);
    
	axis2_thread_mutex_lock(op_ctx_impl->mutex);
    if (op_ctx_impl->msg_ctx_map)
    {
		axis2_msg_ctx_t *rv = NULL;
		rv = axis2_hash_get(op_ctx_impl->msg_ctx_map, message_id, 
						AXIS2_HASH_KEY_STRING); 
		axis2_thread_mutex_unlock(op_ctx_impl->mutex);
        return rv;
    }
    axis2_thread_mutex_unlock(op_ctx_impl->mutex);
    return NULL;
}

/**
 * Checks to see if the MEP is complete. i.e. whether all the messages that
 * are associated with the MEP has arrived and MEP is complete.
 *
 */
axis2_bool_t AXIS2_CALL 
axis2_op_ctx_get_is_complete(struct axis2_op_ctx *op_ctx, 
                             const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(op_ctx)->is_complete;
}

axis2_status_t AXIS2_CALL 
axis2_op_ctx_set_complete(struct axis2_op_ctx *op_ctx, 
                          const axis2_env_t *env, 
                          axis2_bool_t is_complete) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(op_ctx)->is_complete = is_complete;
    return AXIS2_SUCCESS;
}

/**
 * Removes the pointers to this <code>op_ctx</code> in the
 * <code>axis2_conf_ctx</code>'s op_ctxaxis2_hash_t *so that this
 * <code>op_ctx</code> will eventually get garbage collected
 * along with the <code>axis2_msg_ctx_t *</code>'s it contains. Note that if
 * the caller wants to make sure its safe to clean up this op_ctx
 * he should call is_complete() first. However, in cases like IN_OPTIONAL_OUT
 * and OUT_OPTIONAL_IN, it is possibe this will get called without the MEP
 * being complete due to the optional nature of the MEP.
 */
axis2_status_t AXIS2_CALL 
axis2_op_ctx_cleanup(struct axis2_op_ctx *op_ctx, 
                    const axis2_env_t *env) 
{
    axis2_op_ctx_impl_t *op_ctx_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    void *ctx = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_ctx_impl = AXIS2_INTF_TO_IMPL(op_ctx);

    if (!(op_ctx_impl->msg_ctx_map))
        return AXIS2_SUCCESS;
    
    for (hi = axis2_hash_first (op_ctx_impl->msg_ctx_map, env);
             hi; hi = axis2_hash_next (env, hi))
    {
        axis2_hash_this (hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_msg_ctx_t *msg_ctx = (axis2_msg_ctx_t*)ctx;
            axis2_char_t *message_id = AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env);
            if (message_id)
            {
                axis2_hash_set(op_ctx_impl->msg_ctx_map, 
                    message_id, AXIS2_HASH_KEY_STRING, NULL);
                return AXIS2_MSG_CTX_FREE(msg_ctx, env);
            }
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_op_ctx_set_parent(struct axis2_op_ctx *op_ctx, 
                        const axis2_env_t *env, 
                        struct axis2_svc_ctx* svc_ctx) 
{
    axis2_op_ctx_impl_t *op_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_ctx_impl = AXIS2_INTF_TO_IMPL(op_ctx);
    
    if (svc_ctx)
    {
        op_ctx_impl->parent = svc_ctx;
    }
    
    if (op_ctx_impl->parent) /* that is if there is a service context associated */
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(op_ctx_impl->parent, env);
        if (conf_ctx)
        {
            op_ctx_impl->op_ctx_map = 
                AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, env);
        }
        op_ctx_impl->svc_qname = 
            AXIS2_SVC_GET_QNAME(AXIS2_SVC_CTX_GET_SVC(op_ctx_impl->parent, env), env);
    }
    
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL 
axis2_op_ctx_get_msg_ctx_map(struct axis2_op_ctx *op_ctx, 
                             const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op_ctx)->msg_ctx_map;
}
