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

#include <axis2_ctx.h>
#include <axis2.h>
#include <axis2_hash.h>

typedef struct axis2_ctx_impl
{
    /** context base struct */
    axis2_ctx_t ctx;
    /** non persistent map */
    axis2_hash_t *non_persistent_map;
    /** persistent map */
    axis2_hash_t *persistent_map;
    /*axis2_ctx_t *parent; This will not be required as the responsibility of 
    knwong the parent is  deligated to impl structs */
} axis2_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(ctx) ((axis2_ctx_impl_t *)ctx)

axis2_char_t* AXIS2_CALL axis2_ctx_get_property(struct axis2_ctx *ctx, axis2_env_t **env, axis2_char_t *key, axis2_bool_t persistent);
axis2_hash_t* AXIS2_CALL axis2_ctx_get_non_persistent_map(struct axis2_ctx *ctx, axis2_env_t **env);
axis2_hash_t* AXIS2_CALL axis2_ctx_get_persistent_map(struct axis2_ctx *ctx, axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_ctx_free (struct axis2_ctx *ctx, 
                                   axis2_env_t **env);

axis2_ctx_t* AXIS2_CALL axis2_ctx_create(axis2_env_t **env) 
{
    axis2_ctx_impl_t *ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    ctx_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_ctx_impl_t) );
    if (!ctx_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    ctx_impl->persistent_map = NULL;
	ctx_impl->non_persistent_map = NULL;
    
    ctx_impl->persistent_map = axis2_hash_make(env);
    if (!(ctx_impl->persistent_map))
    {
        axis2_ctx_free(&(ctx_impl->ctx), env);
        return NULL;
    }
    
	ctx_impl->non_persistent_map = axis2_hash_make(env);
    if (!(ctx_impl->non_persistent_map))
    {
        axis2_ctx_free(&(ctx_impl->ctx), env);
        return NULL;
    }
	
    /* initialize operations */
    ctx_impl->ctx.ops = NULL;
    ctx_impl->ctx.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_ctx_ops_t) );
    if (!ctx_impl->ctx.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_ctx_free(&(ctx_impl->ctx), env);
        return NULL;        
    }

    ctx_impl->ctx.ops->get_property = axis2_ctx_get_property;
    ctx_impl->ctx.ops->get_non_persistent_map = axis2_ctx_get_non_persistent_map;
    ctx_impl->ctx.ops->get_persistent_map = axis2_ctx_get_persistent_map;
    ctx_impl->ctx.ops->free = axis2_ctx_free;

    return &(ctx_impl->ctx);
}

axis2_status_t AXIS2_CALL axis2_ctx_set_property(struct axis2_ctx *ctx, axis2_env_t **env, axis2_char_t *key, axis2_status_t *value, axis2_bool_t persistent) 
{
    axis2_ctx_impl_t *ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(ctx, env, AXIS2_FAILURE);
    
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    if (persistent) 
    {
        if (ctx_impl->persistent_map)
            axis2_hash_set(ctx_impl->persistent_map, key, AXIS2_HASH_KEY_STRING, value);
    } 
    else 
    {
        if (ctx_impl->non_persistent_map)
            axis2_hash_set(ctx_impl->non_persistent_map, key, AXIS2_HASH_KEY_STRING, value);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL axis2_ctx_get_property(struct axis2_ctx *ctx, axis2_env_t **env, axis2_char_t *key, axis2_bool_t persistent) 
{
    axis2_ctx_impl_t *ctx_impl = NULL;
    axis2_char_t *ret = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(ctx, env, NULL);
    
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    
    if (persistent) 
    {
        if (ctx_impl->persistent_map)
            ret = axis2_hash_get(ctx_impl->persistent_map, key, AXIS2_HASH_KEY_STRING);
    }
    
    if (!ret) 
    {
        if (ctx_impl->non_persistent_map)
            ret = axis2_hash_get(ctx_impl->non_persistent_map, key, AXIS2_HASH_KEY_STRING);
    }
    /** it is the responsibility of the caller to lookup parent if key is not found here 
        Note that in C implementation it is the responsibility of the deriving struct to 
        handle the parent as we do not have the inheritance facility. Im case of 
        context it is not worth trying to mimic inheritance. */
    
    return ret;
}

axis2_hash_t* AXIS2_CALL axis2_ctx_get_non_persistent_map(struct axis2_ctx *ctx, axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(ctx)->non_persistent_map;
}

axis2_hash_t* AXIS2_CALL axis2_ctx_get_persistent_map(struct axis2_ctx *ctx, axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(ctx)->persistent_map;
}

axis2_status_t AXIS2_CALL axis2_ctx_free (struct axis2_ctx *ctx, 
                                   axis2_env_t **env)
{
    axis2_ctx_impl_t *ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(ctx, env, AXIS2_FAILURE);
    
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    
    if (ctx_impl->ctx.ops)
    {
        AXIS2_FREE((*env)->allocator, ctx_impl->ctx.ops);
        ctx_impl->ctx.ops = NULL;
    }
    
    if (ctx_impl->non_persistent_map)
    {
        axis2_hash_free(ctx_impl->non_persistent_map, env);
        ctx_impl->non_persistent_map = NULL;
    }    
    
    if (ctx_impl->persistent_map)
    {
        axis2_hash_free(ctx_impl->persistent_map, env);
        ctx_impl->persistent_map = NULL;
    }    
    
    AXIS2_FREE((*env)->allocator, ctx_impl);
    ctx_impl = NULL;
    
    return AXIS2_SUCCESS;
}
