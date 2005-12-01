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

#include <axis2_conf_ctx.h>
#include <axis2.h>

typedef struct axis2_conf_ctx_impl
{
    /** configuration context interface struct */
    axis2_conf_ctx_t conf_ctx;
    /** base context struct */
    axis2_ctx_t *base;
    /** engine configuration */
    axis2_engine_config_t *engine_config;
    /** root dir */
    /* should be handled as a URL string ? */
    axis2_char_t *root_dir;
    /**
     * axis2_hash_t *containing <code>MessageID</code> to
     * <code>OperationContext</code> mapping.
     */
    axis2_hash_t *operation_ctx_map;

    axis2_hash_t *svc_ctx_map;

    axis2_hash_t *svc_grp_ctx_map;    
} axis2_conf_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(conf_ctx) ((axis2_conf_ctx_impl_t *)conf_ctx)

axis2_status_t AXIS2_CALL axis2_conf_ctx_set_engine_config(struct axis2_conf_ctx *conf_ctx, 
    axis2_env_t **env, 
    axis2_engine_config_t *engine_config);
axis2_ctx_t* AXIS2_CALL axis2_conf_ctx_get_base(struct axis2_conf_ctx *conf_ctx, 
    axis2_env_t **env);
axis2_engine_config_t* AXIS2_CALL axis2_conf_ctx_get_engine_config(struct axis2_conf_ctx *conf_ctx, 
    axis2_env_t **env);
axis2_hash_t* AXIS2_CALL axis2_conf_ctx_get_operation_ctx_map(struct axis2_conf_ctx *conf_ctx, 
    axis2_env_t **env);
axis2_hash_t* AXIS2_CALL axis2_conf_ctx_get_svc_ctx_map(struct axis2_conf_ctx *conf_ctx, 
    axis2_env_t **env);
axis2_hash_t* AXIS2_CALL axis2_conf_ctx_get_svc_grp_ctx_map(struct axis2_conf_ctx *conf_ctx, 
                                                            axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_conf_ctx_register_operation_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                axis2_env_t **env,
                                                                axis2_char_t *message_id,
                                                                axis2_operation_ctx_t *operation_ctx);
axis2_operation_ctx_t* AXIS2_CALL axis2_conf_ctx_get_operation_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                        axis2_env_t **env,
                                                                        axis2_char_t *message_id);
axis2_status_t AXIS2_CALL axis2_conf_ctx_register_svc_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                axis2_env_t **env,
                                                                axis2_char_t *svc_id,
                                                                axis2_svc_ctx_t *svc_ctx);
axis2_svc_ctx_t* AXIS2_CALL axis2_conf_ctx_get_svc_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                        axis2_env_t **env,
                                                                        axis2_char_t *svc_id);
axis2_status_t AXIS2_CALL axis2_conf_ctx_register_svc_grp_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                axis2_env_t **env,
                                                                axis2_char_t *svc_grp_id,
                                                                axis2_svc_grp_ctx_t *svc_grp_ctx);
axis2_svc_grp_ctx_t* AXIS2_CALL axis2_conf_ctx_get_svc_grp_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                        axis2_env_t **env,
                                                                        axis2_char_t *svc_grp_id);
axis2_char_t* AXIS2_CALL axis2_conf_ctx_get_root_dir(struct axis2_conf_ctx *conf_ctx, 
                                                        axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_conf_ctx_set_root_dir(struct axis2_conf_ctx *conf_ctx, 
                                                        axis2_env_t **env,
                                                        axis2_char_t *path);
axis2_status_t AXIS2_CALL axis2_conf_ctx_init(struct axis2_conf_ctx *conf_ctx, 
                                                        axis2_env_t **env,
                                                        axis2_engine_config_t *engine_config);
axis2_status_t AXIS2_CALL axis2_conf_ctx_free(struct axis2_conf_ctx *conf_ctx, 
                                   axis2_env_t **env);
axis2_svc_grp_ctx_t* AXIS2_CALL axis2_conf_ctx_fill_ctxs(struct axis2_conf_ctx *conf_ctx, 
                                               axis2_env_t **env,
                                               axis2_msg_ctx_t *msg_ctx);


axis2_conf_ctx_t* AXIS2_CALL axis2_conf_ctx_create(axis2_env_t **env, axis2_engine_config_t *engine_config) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    conf_ctx_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_conf_ctx_impl_t) );
    if (!conf_ctx_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    conf_ctx_impl->conf_ctx.ops = NULL;
    conf_ctx_impl->base = NULL;
    conf_ctx_impl->engine_config = NULL;
    conf_ctx_impl->root_dir = NULL;
    conf_ctx_impl->operation_ctx_map = NULL;
    conf_ctx_impl->svc_ctx_map = NULL;
    conf_ctx_impl->svc_grp_ctx_map = NULL;
    
    if (engine_config)
        conf_ctx_impl->engine_config = engine_config;
    
    conf_ctx_impl->base = axis2_ctx_create(env);
    if (!(conf_ctx_impl->base))
    {
        axis2_conf_ctx_free(&(conf_ctx_impl->conf_ctx), env);
        return NULL;
    }
    
    conf_ctx_impl->operation_ctx_map = axis2_hash_make(env);
    if (!(conf_ctx_impl->operation_ctx_map))
    {
        axis2_conf_ctx_free(&(conf_ctx_impl->conf_ctx), env);
        return NULL;
    }
    
	conf_ctx_impl->svc_ctx_map = axis2_hash_make(env);
    if (!(conf_ctx_impl->svc_ctx_map))
    {
        axis2_conf_ctx_free(&(conf_ctx_impl->conf_ctx), env);
        return NULL;
    }
	
    conf_ctx_impl->svc_grp_ctx_map = axis2_hash_make(env);
    if (!(conf_ctx_impl->svc_grp_ctx_map))
    {
        axis2_conf_ctx_free(&(conf_ctx_impl->conf_ctx), env);
        return NULL;
    }
    
    /* initialize operations */    
    conf_ctx_impl->conf_ctx.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_conf_ctx_ops_t) );
    if (!conf_ctx_impl->conf_ctx.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_conf_ctx_free(&(conf_ctx_impl->conf_ctx), env);
        return NULL;        
    }

    conf_ctx_impl->conf_ctx.ops->free = axis2_conf_ctx_free;

    conf_ctx_impl->conf_ctx.ops->set_engine_config = axis2_conf_ctx_set_engine_config;
    conf_ctx_impl->conf_ctx.ops->get_base = axis2_conf_ctx_get_base;
    conf_ctx_impl->conf_ctx.ops->get_engine_config = axis2_conf_ctx_get_engine_config;
    conf_ctx_impl->conf_ctx.ops->get_operation_ctx_map = axis2_conf_ctx_get_operation_ctx_map;
    conf_ctx_impl->conf_ctx.ops->get_svc_ctx_map = axis2_conf_ctx_get_svc_ctx_map;
    conf_ctx_impl->conf_ctx.ops->get_svc_grp_ctx_map = axis2_conf_ctx_get_svc_grp_ctx_map;
    conf_ctx_impl->conf_ctx.ops->register_operation_ctx = axis2_conf_ctx_register_operation_ctx;
    conf_ctx_impl->conf_ctx.ops->get_operation_ctx = axis2_conf_ctx_get_operation_ctx;
    conf_ctx_impl->conf_ctx.ops->register_svc_ctx = axis2_conf_ctx_register_svc_ctx;
    conf_ctx_impl->conf_ctx.ops->get_svc_ctx = axis2_conf_ctx_get_svc_ctx;
    conf_ctx_impl->conf_ctx.ops->register_svc_grp_ctx = axis2_conf_ctx_register_svc_grp_ctx;
    conf_ctx_impl->conf_ctx.ops->get_svc_grp_ctx = axis2_conf_ctx_get_svc_grp_ctx;
    conf_ctx_impl->conf_ctx.ops->get_root_dir = axis2_conf_ctx_get_root_dir;
    conf_ctx_impl->conf_ctx.ops->set_root_dir = axis2_conf_ctx_set_root_dir;
    conf_ctx_impl->conf_ctx.ops->init = axis2_conf_ctx_init;
    conf_ctx_impl->conf_ctx.ops->free = axis2_conf_ctx_free;
    conf_ctx_impl->conf_ctx.ops->fill_ctxs = axis2_conf_ctx_fill_ctxs;

    return &(conf_ctx_impl->conf_ctx);
}

/**
 * @param configuration
 */
axis2_status_t AXIS2_CALL axis2_conf_ctx_set_engine_config(struct axis2_conf_ctx *conf_ctx, axis2_env_t **env, axis2_engine_config_t *engine_config) 
{
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(conf_ctx)->engine_config = engine_config; /* we just maitain a shalow copy here */
    return AXIS2_SUCCESS;
}

/**
 */
axis2_ctx_t* AXIS2_CALL axis2_conf_ctx_get_base(struct axis2_conf_ctx *conf_ctx, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf_ctx)->base;
}

/**
 */
axis2_engine_config_t* AXIS2_CALL axis2_conf_ctx_get_engine_config(struct axis2_conf_ctx *conf_ctx, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf_ctx)->engine_config;
}

axis2_hash_t* AXIS2_CALL axis2_conf_ctx_get_operation_ctx_map(struct axis2_conf_ctx *conf_ctx, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf_ctx)->operation_ctx_map;
}

axis2_hash_t* AXIS2_CALL axis2_conf_ctx_get_svc_ctx_map(struct axis2_conf_ctx *conf_ctx, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf_ctx)->svc_ctx_map;
}

axis2_hash_t* AXIS2_CALL axis2_conf_ctx_get_svc_grp_ctx_map(struct axis2_conf_ctx *conf_ctx, 
                                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf_ctx)->svc_grp_ctx_map;
}

/**
 * Register a struct axis2_operation_ctx against a given Message ID.
 *
 * @param messageID
 * @param mepContext
 */
axis2_status_t AXIS2_CALL axis2_conf_ctx_register_operation_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                axis2_env_t **env,
                                                                axis2_char_t *message_id,
                                                                axis2_operation_ctx_t *operation_ctx) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, AXIS2_FAILURE);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->operation_ctx_map)
    {
        axis2_hash_set(conf_ctx_impl->operation_ctx_map, message_id, AXIS2_HASH_KEY_STRING, operation_ctx);
    }
    
    return AXIS2_SUCCESS;
}

/**
 * get axis2_operation_ctx struct given a Message ID
 *
 * @param message_id
 * @return struct axis2_operation_ctx * <code>struct axis2_operation_ctx *<code>
 */
axis2_operation_ctx_t* AXIS2_CALL axis2_conf_ctx_get_operation_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                        axis2_env_t **env,
                                                                        axis2_char_t *message_id) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->operation_ctx_map)
    {
        return (axis2_operation_ctx_t*)axis2_hash_get(conf_ctx_impl->operation_ctx_map, message_id, AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

/**
 * Register a struct axis2_svc_ctx against a given Message ID.
 *
 * @param svc_id
 * @param mepContext
 */
axis2_status_t AXIS2_CALL axis2_conf_ctx_register_svc_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                axis2_env_t **env,
                                                                axis2_char_t *svc_id,
                                                                axis2_svc_ctx_t *svc_ctx) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, AXIS2_FAILURE);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->svc_ctx_map)
    {
        axis2_hash_set(conf_ctx_impl->svc_ctx_map, svc_id, AXIS2_HASH_KEY_STRING, svc_ctx);
    }
    
    return AXIS2_SUCCESS;
}

/**
 * get axis2_svc_ctx struct given a Message ID
 *
 * @param svc_id
 * @return struct axis2_svc_ctx * <code>struct axis2_svc_ctx *<code>
 */
axis2_svc_ctx_t* AXIS2_CALL axis2_conf_ctx_get_svc_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                        axis2_env_t **env,
                                                                        axis2_char_t *svc_id) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->svc_ctx_map)
    {
        return (axis2_svc_ctx_t*)axis2_hash_get(conf_ctx_impl->svc_ctx_map, svc_id, AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

/**
 * Register a struct axis2_svc_grp_ctx against a given Message ID.
 *
 * @param svc_grp_id
 * @param mepContext
 */
axis2_status_t AXIS2_CALL axis2_conf_ctx_register_svc_grp_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                axis2_env_t **env,
                                                                axis2_char_t *svc_grp_id,
                                                                axis2_svc_grp_ctx_t *svc_grp_ctx) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, AXIS2_FAILURE);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->svc_grp_ctx_map)
    {
        axis2_hash_set(conf_ctx_impl->svc_grp_ctx_map, svc_grp_id, AXIS2_HASH_KEY_STRING, svc_grp_ctx);
    }
    
    return AXIS2_SUCCESS;
}

/**
 * get axis2_svc_grp_ctx struct given a service group ID
 *
 * @param svc_grp_id
 * @return struct axis2_svc_grp_ctx * <code>struct axis2_svc_grp_ctx *<code>
 */
axis2_svc_grp_ctx_t* AXIS2_CALL axis2_conf_ctx_get_svc_grp_ctx(struct axis2_conf_ctx *conf_ctx, 
                                                                        axis2_env_t **env,
                                                                        axis2_char_t *svc_grp_id) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->svc_grp_ctx_map)
    {
        return (axis2_svc_grp_ctx_t*)axis2_hash_get(conf_ctx_impl->svc_grp_ctx_map, svc_grp_id, AXIS2_HASH_KEY_STRING);
    }
    return NULL;
}

/**
 * This method allows users to reolve the paths relative to the
 * root diretory
 *
 * @param path
 */
axis2_char_t* AXIS2_CALL axis2_conf_ctx_get_root_dir(struct axis2_conf_ctx *conf_ctx, 
                                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);    
    return AXIS2_INTF_TO_IMPL(conf_ctx)->root_dir;
}

/**
 * @param file
 */
axis2_status_t AXIS2_CALL axis2_conf_ctx_set_root_dir(struct axis2_conf_ctx *conf_ctx, 
                                                        axis2_env_t **env,
                                                        axis2_char_t *path) 
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, AXIS2_FAILURE);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->root_dir)
    {
        AXIS2_FREE((*env)->allocator, conf_ctx_impl->root_dir);
        conf_ctx_impl->root_dir = NULL;
    } 
    
    if (path)
    {
        conf_ctx_impl->root_dir = AXIS2_STRDUP(path, env);
        if (!(conf_ctx_impl->root_dir))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_conf_ctx_init(struct axis2_conf_ctx *conf_ctx, 
                                                        axis2_env_t **env,
                                                        axis2_engine_config_t *engine_config)
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    void* ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, AXIS2_FAILURE);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);

    conf_ctx_impl->engine_config = engine_config;
    
    for (hi = axis2_hash_first (conf_ctx_impl->operation_ctx_map, env);
             hi; hi = axis2_hash_next (env, hi))
    {
        axis2_hash_this (hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_operation_ctx_t *operation_ctx = (axis2_operation_ctx_t*) ctx;
            AXIS2_OPERATION_CTX_INIT(operation_ctx, env, engine_config);
        }
    }

    for (hi = axis2_hash_first (conf_ctx_impl->svc_ctx_map, env);
             hi; hi = axis2_hash_next (env, hi))
    {
        axis2_hash_this (hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_svc_ctx_t *svc_ctx = (axis2_svc_ctx_t*) ctx;
            AXIS2_SVC_CTX_INIT(svc_ctx, env, engine_config);
        }
    }
    
    for (hi = axis2_hash_first (conf_ctx_impl->svc_grp_ctx_map, env);
             hi; hi = axis2_hash_next (env, hi))
    {
        axis2_hash_this (hi, NULL, NULL, &ctx);
        if (ctx)
        {
            axis2_svc_grp_ctx_t *svc_grp_ctx = (axis2_svc_grp_ctx_t*) ctx;
            AXIS2_SVC_GRP_CTX_INIT(svc_grp_ctx, env, engine_config);
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_conf_ctx_free (struct axis2_conf_ctx *conf_ctx, 
                                   axis2_env_t **env)
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, AXIS2_FAILURE);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    if (conf_ctx_impl->conf_ctx.ops)
    {
        AXIS2_FREE((*env)->allocator, conf_ctx_impl->conf_ctx.ops);
        conf_ctx_impl->conf_ctx.ops = NULL;
    }
    
    if (conf_ctx_impl->operation_ctx_map)
    {
        axis2_hash_free(conf_ctx_impl->operation_ctx_map, env);
        conf_ctx_impl->operation_ctx_map = NULL;
    }    
    
    if (conf_ctx_impl->svc_ctx_map)
    {
        axis2_hash_free(conf_ctx_impl->svc_ctx_map, env);
        conf_ctx_impl->svc_ctx_map = NULL;
    }    
    
    if (conf_ctx_impl->svc_grp_ctx_map)
    {
        axis2_hash_free(conf_ctx_impl->svc_grp_ctx_map, env);
        conf_ctx_impl->svc_grp_ctx_map = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, conf_ctx_impl);
    conf_ctx_impl = NULL;
    
    return AXIS2_SUCCESS;
}

/**
 * This method should search for a service group context in the map with given id as the key.
 * If(key != null && found)
 * check for a service context for the intended service.
 * if (!found)
 * create one and hook up to service group context
 * else
 * create new service group context with the given key or if key is null with a new key
 * create a new service context for the service
 *
 * @param messageContext
 */
axis2_svc_grp_ctx_t* AXIS2_CALL axis2_conf_ctx_fill_ctxs(struct axis2_conf_ctx *conf_ctx, 
                                               axis2_env_t **env,
                                               axis2_msg_ctx_t *msg_ctx)
{
    axis2_conf_ctx_impl_t *conf_ctx_impl = NULL;
    axis2_char_t *svc_grp_ctx_id = NULL;
    axis2_svc_grp_ctx_t *svc_grp_ctx;
    axis2_svc_ctx_t *svc_ctx;
    axis2_svc_t *svc = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *svc_id = NULL;
    axis2_operation_ctx_t *operation_ctx = NULL;

    
    AXIS2_FUNC_PARAM_CHECK(conf_ctx, env, NULL);
    
    conf_ctx_impl = AXIS2_INTF_TO_IMPL(conf_ctx);
    
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, NULL);

    svc = AXIS2_MSG_CTX_GET_SVC(msg_ctx, env);
    if (!svc)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SERVICE_NOT_YET_FOUND, AXIS2_FAILURE);
        return NULL;
    }
    
    qname = AXIS2_SVC_GET_NAME(svc, env);
    if (!qname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC, AXIS2_FAILURE);
        return NULL;
    }
    
    svc_id = AXIS2_QNAME_GET_LOCALPART(qname, env);
    if (!svc_id)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC, AXIS2_FAILURE);
        return NULL;
    }
    
    svc_grp_ctx_id = AXIS2_MSG_CTX_GET_SVC_GRP_CTX_ID(msg_ctx, env);
    
    /* by this time service group context id must have a value, 
    either from transport or from addressing */
    if (svc_grp_ctx_id)
    {
        svc_grp_ctx = (axis2_svc_grp_ctx_t*) axis2_hash_get(conf_ctx_impl->svc_grp_ctx_map, 
                                                                 svc_grp_ctx_id, AXIS2_HASH_KEY_STRING);
        if (svc_grp_ctx)
        {
            svc_ctx = AXIS2_SVC_GRP_CTX_GET_SVC_CTX(svc_grp_ctx, env, svc_id);
            if (!svc_ctx)
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC_GRP, AXIS2_FAILURE);
                return NULL;
            }
        }
    }
    
    if (!svc_grp_ctx_id)
    {
        /** TODO generate ID and set it*/
        /*
        svc_grp_ctx_id = UUIDGenerator.getUUID();
        msg_ctx.setServiceGroupContextId(svc_grp_ctx_id);
        */
    }
    
    if (!svc_grp_ctx)
    {
        axis2_svc_grp_t *svc_grp = NULL;
        svc_grp = AXIS2_SVC_GET_PARENT(svc, env);
        /*svc_grp_ctx = AXIS2_SVC_GRP_GET_SVC_CTX(svc_grp, env);
        svc_ctx = AXIS2_SVC_GRP_CTX_GET_SVC_CTX(svc_grp_ctx, env, svc_id);*/        
        if (!svc_ctx)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC_GRP, AXIS2_FAILURE);
            return NULL;
        }
        AXIS2_SVC_GRP_CTX_SET_ID(svc_grp_ctx, env, svc_grp_ctx_id);
        axis2_conf_ctx_register_svc_grp_ctx(conf_ctx, env, svc_grp_ctx_id, svc_grp_ctx);        
    }
    
    /* when you come here operation context MUST already been assigned to the message context */
    operation_ctx = AXIS2_MSG_CTX_GET_OPERATION_CTX(msg_ctx, env);
    if (!operation_ctx)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_MSG_CTX, AXIS2_FAILURE);
        return NULL;
    }
    
    AXIS2_OPERATION_CTX_SET_PARENT(operation_ctx, env, svc_ctx);
    AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx);
    AXIS2_MSG_CTX_SET_SVC_GRP_CTX(msg_ctx, env, svc_grp_ctx);
    return svc_grp_ctx;
}
