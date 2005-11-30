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

#include <axis2_svc_grp_ctx.h>
#include <axis2.h>
#include <axis2_hash.h>

typedef struct axis2_svc_grp_ctx_impl
{
    /** context base struct */
    axis2_svc_grp_ctx_t svc_grp_ctx;
    /** base context struct */
    axis2_ctx_t *base;
    /** parent of service group context is a configuration context instance */
    struct axis2_conf_ctx *parent;
    /** service group context ID */
    axis2_char_t *id;
    /** map of service contexts belonging to this service contecxt group */
    axis2_hash_t *svc_ctx_map;
    /** service group associated with this service group context */
    axis2_svc_grp_t *svc_grp;
    /** name of the service group associated with this context */
    axis2_char_t *svc_grp_name;
} axis2_svc_grp_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(svc_grp_ctx) ((axis2_svc_grp_ctx_impl_t *)svc_grp_ctx)

axis2_ctx_t* AXIS2_CALL axis2_svc_grp_ctx_get_base(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                            axis2_env_t **env);
struct axis2_conf_ctx* AXIS2_CALL axis2_svc_grp_ctx_get_parent(struct axis2_svc_grp_ctx *svc_grp_ctx, 
       axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_free(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                   axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_init(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                            axis2_env_t **env,
                                            axis2_engine_config_t *engine_config);
axis2_char_t* AXIS2_CALL axis2_svc_grp_ctx_get_id(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_set_id(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env, axis2_char_t *id);
axis2_svc_ctx_t* AXIS2_CALL axis2_svc_grp_ctx_get_svc_ctx(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env,
                        axis2_char_t *svc_name);
axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_fill_svc_ctx_map(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env);
axis2_svc_grp_t* AXIS2_CALL axis2_svc_grp_ctx_get_svc_grp(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env);
axis2_hash_t* AXIS2_CALL axis2_svc_grp_ctx_get_svc_ctx_map(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env);


axis2_svc_grp_ctx_t* AXIS2_CALL axis2_svc_grp_ctx_create(axis2_env_t **env, 
    axis2_svc_grp_t *svc_grp, 
    struct axis2_conf_ctx *conf_ctx) 
{
    axis2_svc_grp_ctx_impl_t *svc_grp_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    svc_grp_ctx_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_grp_ctx_impl_t) );
    if (!svc_grp_ctx_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    svc_grp_ctx_impl->svc_grp_ctx.ops = NULL;
    svc_grp_ctx_impl->base = NULL;
    svc_grp_ctx_impl->parent = NULL;
    svc_grp_ctx_impl->id = NULL;
    svc_grp_ctx_impl->svc_ctx_map = NULL;
    svc_grp_ctx_impl->svc_grp = NULL;
    svc_grp_ctx_impl->svc_grp_name = NULL;

    
    svc_grp_ctx_impl->base = axis2_ctx_create(env);
    if (!(svc_grp_ctx_impl->base))
    {
        axis2_svc_grp_ctx_free(&(svc_grp_ctx_impl->svc_grp_ctx), env);
        return NULL;
    }

    if (svc_grp)
    {
        svc_grp_ctx_impl->svc_grp = svc_grp;
        svc_grp_ctx_impl->svc_grp_name = AXIS2_SVC_GRP_GET_NAME(svc_grp_ctx_impl->svc_grp, env);
    }
    
    if (conf_ctx)
    {
        svc_grp_ctx_impl->parent = conf_ctx;
    }
    
    svc_grp_ctx_impl->svc_ctx_map = axis2_hash_make(env);
    if (!(svc_grp_ctx_impl->svc_ctx_map))
    {
        axis2_svc_grp_ctx_free(&(svc_grp_ctx_impl->svc_grp_ctx), env);
        return NULL;
    }
    
    axis2_svc_grp_ctx_fill_svc_ctx_map((&(svc_grp_ctx_impl->svc_grp_ctx)), env);
    
    /* initialize operations */
    
    svc_grp_ctx_impl->svc_grp_ctx.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_svc_grp_ctx_ops_t) );
    if (!svc_grp_ctx_impl->svc_grp_ctx.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_svc_grp_ctx_free(&(svc_grp_ctx_impl->svc_grp_ctx), env);
        return NULL;        
    }

    svc_grp_ctx_impl->svc_grp_ctx.ops->get_base = axis2_svc_grp_ctx_get_base;
    svc_grp_ctx_impl->svc_grp_ctx.ops->get_parent = axis2_svc_grp_ctx_get_parent;
    svc_grp_ctx_impl->svc_grp_ctx.ops->free = axis2_svc_grp_ctx_free;
    svc_grp_ctx_impl->svc_grp_ctx.ops->init = axis2_svc_grp_ctx_init;
    svc_grp_ctx_impl->svc_grp_ctx.ops->get_id = axis2_svc_grp_ctx_get_id;
    svc_grp_ctx_impl->svc_grp_ctx.ops->set_id = axis2_svc_grp_ctx_set_id;
    
    svc_grp_ctx_impl->svc_grp_ctx.ops->get_svc_ctx = axis2_svc_grp_ctx_get_svc_ctx;
    svc_grp_ctx_impl->svc_grp_ctx.ops->fill_svc_ctx_map = axis2_svc_grp_ctx_fill_svc_ctx_map;
    svc_grp_ctx_impl->svc_grp_ctx.ops->get_svc_grp = axis2_svc_grp_ctx_get_svc_grp;
    svc_grp_ctx_impl->svc_grp_ctx.ops->get_svc_ctx_map = axis2_svc_grp_ctx_get_svc_ctx_map;
    
    return &(svc_grp_ctx_impl->svc_grp_ctx);
}

axis2_ctx_t* AXIS2_CALL axis2_svc_grp_ctx_get_base(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_grp_ctx)->base;
}


struct axis2_conf_ctx* AXIS2_CALL axis2_svc_grp_ctx_get_parent(struct axis2_svc_grp_ctx *svc_grp_ctx, axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_grp_ctx)->parent;
}



axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_free (struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                   axis2_env_t **env)
{
    axis2_svc_grp_ctx_impl_t *svc_grp_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, AXIS2_FAILURE);
    
    svc_grp_ctx_impl = AXIS2_INTF_TO_IMPL(svc_grp_ctx);
    
    if (svc_grp_ctx_impl->svc_grp_ctx.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_grp_ctx_impl->svc_grp_ctx.ops);
        svc_grp_ctx_impl->svc_grp_ctx.ops = NULL;
    }
    
    if (svc_grp_ctx_impl->base)
    {
        AXIS2_CTX_FREE(svc_grp_ctx_impl->base, env);
        svc_grp_ctx_impl->base = NULL;
    }    
    
    if (svc_grp_ctx_impl->svc_ctx_map)
    {
        axis2_hash_free(svc_grp_ctx_impl->svc_ctx_map, env);
        svc_grp_ctx_impl->base = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, svc_grp_ctx_impl);
    svc_grp_ctx_impl = NULL;
    
    return AXIS2_SUCCESS;
}


/**
 * The method is used to do the intialization of the EngineContext
 */
axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_init(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                            axis2_env_t **env,
                    axis2_engine_config_t *engine_config) 
{
    axis2_svc_grp_ctx_impl_t *svc_grp_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, AXIS2_FAILURE);
    
    svc_grp_ctx_impl = AXIS2_INTF_TO_IMPL(svc_grp_ctx);

    if (svc_grp_ctx_impl->svc_grp_name)
    {        
        svc_grp_ctx_impl->svc_grp = AXIS2_ENGINE_CONFIG_GET_SVC_GRP(engine_config, env, svc_grp_ctx_impl->svc_grp_name);
    }
    
    return AXIS2_SUCCESS;
}


axis2_char_t* AXIS2_CALL axis2_svc_grp_ctx_get_id(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_grp_ctx)->id;
}

axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_set_id(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env, axis2_char_t *id) 
{
    axis2_svc_grp_ctx_impl_t *svc_grp_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, AXIS2_FAILURE);
    
    svc_grp_ctx_impl = AXIS2_INTF_TO_IMPL(svc_grp_ctx);
    
    if (svc_grp_ctx_impl->id)
    {
        AXIS2_FREE((*env)->allocator, svc_grp_ctx_impl->id);
        svc_grp_ctx_impl->id = NULL;
    }
    
    if (id)
    {
        svc_grp_ctx_impl->id = AXIS2_STRDUP(id, env);
    }
    
    return AXIS2_SUCCESS;
}

/** if the servic name is foo:bar , you should pass only bar */
axis2_svc_ctx_t* AXIS2_CALL axis2_svc_grp_ctx_get_svc_ctx(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env,
                        axis2_char_t *svc_name) 
{
    axis2_svc_grp_ctx_impl_t *svc_grp_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, NULL);
    
    svc_grp_ctx_impl = AXIS2_INTF_TO_IMPL(svc_grp_ctx);
    
    return (axis2_svc_ctx_t *) axis2_hash_get(svc_grp_ctx_impl->svc_ctx_map, svc_name, AXIS2_HASH_KEY_STRING);
}

/**
 * This will create one service context per each service in descrpition
 * if service group has 2 service init , then two service contexts will be
 * created
 */
axis2_status_t AXIS2_CALL axis2_svc_grp_ctx_fill_svc_ctx_map(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env)
{
    axis2_svc_grp_ctx_impl_t *svc_grp_ctx_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    void *next_svc = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, AXIS2_SUCCESS);
    
    svc_grp_ctx_impl = AXIS2_INTF_TO_IMPL(svc_grp_ctx);

    if (svc_grp_ctx_impl->svc_grp)
    {
        /*axis2_hash_t *service_map = AXIS2_SVC_GRP_GET_SVC_MAP(svc_grp_ctx_impl->svc_grp, env);
        if (service_map)
        {
            for (hi = axis2_hash_first (service_map, env);
             hi; hi = axis2_hash_next (env, hi))
            {
                axis2_hash_this (hi, NULL, NULL, &next_svc);
                if (next_svc)
                {
                    axis2_svc_t *svc = NULL;
                    axis2_svc_ctx_t *svc_ctx = NULL;
                    axis2_char_t *svc_name = NULL; 
                    svc = (axis2_svc_t*) next_svc;
                    svc_ctx = axis2_svc_create(env, svc, svc_grp_ctx);
                    svc_name = AXIS2_SVC_GET_NAME(svc, name);
                    if (svc_name)
                        AXIS2_HASH_SET(svc_grp_ctx_impl->svc_ctx_map, svc_name, AXIS2_HASH_KEY_STRING, svc_ctx);
                }
            }
        }*/
    }
}

axis2_svc_grp_t* AXIS2_CALL axis2_svc_grp_ctx_get_svc_grp(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_grp_ctx)->svc_grp;
}

axis2_hash_t* AXIS2_CALL axis2_svc_grp_ctx_get_svc_ctx_map(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_grp_ctx)->svc_ctx_map;
}
