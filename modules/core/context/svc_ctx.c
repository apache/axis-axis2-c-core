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

#include <axis2_svc_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2.h>
#include <axis2_hash.h>

typedef struct axis2_svc_ctx_impl
{
    /** context base struct */
    axis2_svc_ctx_t svc_ctx;
    /** base context struct */
    axis2_ctx_t *base;
    /** parent of op context is a service context instance */
    struct axis2_svc_grp_ctx *parent;
    /** service associated with this service context */
    axis2_svc_t *svc;
    /** id of the service associated with this context */
    axis2_char_t *svc_id;
    /** service qname */
    axis2_qname_t *svc_qname;
} axis2_svc_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(svc_ctx) ((axis2_svc_ctx_impl_t *)svc_ctx)

axis2_ctx_t* AXIS2_CALL 
axis2_svc_ctx_get_base(struct axis2_svc_ctx *svc_ctx, 
                       axis2_env_t **env);
                       
struct axis2_svc_grp_ctx * AXIS2_CALL 
axis2_svc_ctx_get_parent(struct axis2_svc_ctx *svc_ctx, 
                         axis2_env_t **env);
                         
axis2_status_t AXIS2_CALL 
axis2_svc_ctx_free(struct axis2_svc_ctx *svc_ctx, 
                   axis2_env_t **env);
                   
axis2_status_t AXIS2_CALL 
axis2_svc_ctx_init(struct axis2_svc_ctx *svc_ctx, 
                   axis2_env_t **env,
                   axis2_conf_t *conf);
                   
axis2_char_t* AXIS2_CALL 
axis2_svc_ctx_get_svc_id(struct axis2_svc_ctx *svc_ctx, 
                         axis2_env_t **env);
                         
axis2_svc_t* AXIS2_CALL 
axis2_svc_ctx_get_svc(struct axis2_svc_ctx *svc_ctx, 
                      axis2_env_t **env);
                      
axis2_status_t AXIS2_CALL
axis2_svc_ctx_set_svc(axis2_svc_ctx_t *svc_ctx,
                        axis2_env_t **env,
                        axis2_svc_t *svc);

                        
struct axis2_conf_ctx* AXIS2_CALL 
axis2_svc_ctx_get_conf_ctx(struct axis2_svc_ctx *svc_ctx, 
                           axis2_env_t **env);
                           
axis2_op_ctx_t* AXIS2_CALL 
axis2_svc_ctx_create_op_ctx(struct axis2_svc_ctx *svc_ctx, 
                            axis2_env_t **env, 
                            axis2_qname_t *qname);

AXIS2_DECLARE(axis2_svc_ctx_t*)
axis2_svc_ctx_create(axis2_env_t **env, 
    axis2_svc_t *svc,
    struct axis2_svc_grp_ctx *svc_grp_ctx) 
{
    axis2_svc_ctx_impl_t *svc_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    svc_ctx_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_ctx_impl_t) );
    if (!svc_ctx_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    svc_ctx_impl->svc_ctx.ops = NULL;
    svc_ctx_impl->base = NULL;
    svc_ctx_impl->parent = NULL;
    svc_ctx_impl->svc = NULL;
    svc_ctx_impl->svc_id = NULL;
    svc_ctx_impl->svc_qname = NULL;
    
    svc_ctx_impl->base = axis2_ctx_create(env);
    if (!(svc_ctx_impl->base))
    {
        axis2_svc_ctx_free(&(svc_ctx_impl->svc_ctx), env);
        return NULL;
    }

    if (svc)
    {
        svc_ctx_impl->svc = svc;
        svc_ctx_impl->svc_qname = AXIS2_SVC_GET_QNAME(svc, env);
        if (svc_ctx_impl->svc_qname)
        {
            svc_ctx_impl->svc_id = AXIS2_QNAME_GET_LOCALPART(svc_ctx_impl->svc_qname, env);
        }
    }
    
    if (svc_grp_ctx)
    {
        svc_ctx_impl->parent = svc_grp_ctx;
    }
    
    /* initialize ops */
    
    svc_ctx_impl->svc_ctx.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_ctx_ops_t) );
    if (!svc_ctx_impl->svc_ctx.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_svc_ctx_free(&(svc_ctx_impl->svc_ctx), env);
        return NULL;        
    }

    svc_ctx_impl->svc_ctx.ops->get_base = axis2_svc_ctx_get_base;
    svc_ctx_impl->svc_ctx.ops->get_parent = axis2_svc_ctx_get_parent;
    svc_ctx_impl->svc_ctx.ops->free = axis2_svc_ctx_free;
    svc_ctx_impl->svc_ctx.ops->init = axis2_svc_ctx_init;
    svc_ctx_impl->svc_ctx.ops->get_svc_id = axis2_svc_ctx_get_svc_id;
    svc_ctx_impl->svc_ctx.ops->get_svc = axis2_svc_ctx_get_svc;
    svc_ctx_impl->svc_ctx.ops->set_svc = axis2_svc_ctx_set_svc;
    svc_ctx_impl->svc_ctx.ops->get_conf_ctx = axis2_svc_ctx_get_conf_ctx;
    svc_ctx_impl->svc_ctx.ops->create_op_ctx = axis2_svc_ctx_create_op_ctx;
    
    return &(svc_ctx_impl->svc_ctx);
}

axis2_ctx_t* AXIS2_CALL 
axis2_svc_ctx_get_base(struct axis2_svc_ctx *svc_ctx, 
                       axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_ctx)->base;
}


struct axis2_svc_grp_ctx * AXIS2_CALL 
axis2_svc_ctx_get_parent(struct axis2_svc_ctx *svc_ctx, 
                         axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_ctx)->parent;
}



axis2_status_t AXIS2_CALL 
axis2_svc_ctx_free (struct axis2_svc_ctx *svc_ctx, 
                    axis2_env_t **env)
{
    axis2_svc_ctx_impl_t *svc_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    svc_ctx_impl = AXIS2_INTF_TO_IMPL(svc_ctx);
    
    if (svc_ctx_impl->svc_ctx.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_ctx_impl->svc_ctx.ops);
        svc_ctx_impl->svc_ctx.ops = NULL;
    }
    
    if (svc_ctx_impl->base)
    {
        AXIS2_CTX_FREE(svc_ctx_impl->base, env);
        svc_ctx_impl->base = NULL;
    }    
    
    AXIS2_FREE((*env)->allocator, svc_ctx_impl);
    svc_ctx_impl = NULL;
    
    return AXIS2_SUCCESS;
}


/**
 * The method is used to do the intialization of the EngineContext
 */
axis2_status_t AXIS2_CALL 
axis2_svc_ctx_init(struct axis2_svc_ctx *svc_ctx, 
                   axis2_env_t **env,
                   axis2_conf_t *conf) 
{
    axis2_svc_ctx_impl_t *svc_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    svc_ctx_impl = AXIS2_INTF_TO_IMPL(svc_ctx);

    if (svc_ctx_impl->svc_qname)
    {
        axis2_char_t *svc_name = AXIS2_QNAME_GET_LOCALPART(svc_ctx_impl->svc_qname, env);
        svc_ctx_impl->svc = AXIS2_CONF_GET_SVC(conf, env, svc_name);
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @return Returns the svc_id.
 */
axis2_char_t* AXIS2_CALL 
axis2_svc_ctx_get_svc_id(struct axis2_svc_ctx *svc_ctx, 
                         axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_ctx)->svc_id;
}


axis2_svc_t* AXIS2_CALL 
axis2_svc_ctx_get_svc(struct axis2_svc_ctx *svc_ctx, 
                      axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_ctx)->svc;
}

axis2_status_t AXIS2_CALL
axis2_svc_ctx_set_svc(axis2_svc_ctx_t *svc_ctx,
                        axis2_env_t **env,
                        axis2_svc_t *svc)
{
    axis2_svc_ctx_impl_t *svc_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
    svc_ctx_impl = AXIS2_INTF_TO_IMPL(svc_ctx);
     
    svc_ctx_impl->svc = svc;
    svc_ctx_impl->svc_qname = AXIS2_SVC_GET_QNAME(svc, env);
    if (svc_ctx_impl->svc_qname)
    {
        svc_ctx_impl->svc_id = AXIS2_QNAME_GET_LOCALPART(svc_ctx_impl->
            svc_qname, env);
    }
    return AXIS2_SUCCESS;
}

struct axis2_conf_ctx* AXIS2_CALL 
axis2_svc_ctx_get_conf_ctx(struct axis2_svc_ctx *svc_ctx, 
                           axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_SVC_GRP_CTX_GET_PARENT(AXIS2_INTF_TO_IMPL(svc_ctx)->parent, env);
}

axis2_op_ctx_t* AXIS2_CALL 
axis2_svc_ctx_create_op_ctx(struct axis2_svc_ctx *svc_ctx, 
                            axis2_env_t **env, 
                            axis2_qname_t *qname) 
{
    axis2_svc_ctx_impl_t *svc_ctx_impl = NULL;
    axis2_op_t *op = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    svc_ctx_impl = AXIS2_INTF_TO_IMPL(svc_ctx);
    
    if (svc_ctx_impl->svc)
    {
        op = AXIS2_SVC_GET_OP_WITH_QNAME(svc_ctx_impl->svc, env, qname);
        return axis2_op_ctx_create(env, op, svc_ctx);
    }
    
    return NULL;
}
