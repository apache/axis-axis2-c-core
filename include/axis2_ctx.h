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

#ifndef AXIS2_CTX_H
#define AXIS2_CTX_H

/**
 * @defgroup axis2_context context hierarchy
 * @ingroup axis2
 * @{
 * @}
 */

/** 
 * @defgroup axis2_ctx context
 * @ingroup axis2_context
 * context is the base struct of all the context related structs. This struct 
 * encapsulates the common operations and data for all context types. All the 
 * context types, configuration, service group, service and operation has the
 * base of type context. 
 * @{
 */

/**
 * @file axis2_ctx.h
 */

#include <axis2_defines.h>
#include <axis2_hash.h>
#include <axis2_env.h>
#include <axis2_property.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_ctx */
    typedef struct axis2_ctx axis2_ctx_t;

    /**
     * Creates a context struct.
     * @param env pointer to environment struct
     * @return pointer to newly created context
     */
    AXIS2_EXTERN axis2_ctx_t *AXIS2_CALL 
    axis2_ctx_create(const axis2_env_t *env);

    /**
     * Sets a property with the given key.
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @param key key string to store the property with
     * @param value pointer to property to be stored, context assumes the 
     * ownership of the property
     * @param persistent persist ency status, AXIS2_TRUE if the value is to 
     * be stored in the resistant store, AXIS2_FALSE if it is to be stored 
     * in the non-persistent store
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t  AXIS2_CALL
    axis2_ctx_set_property(struct axis2_ctx *ctx,
        const axis2_env_t *env,
        const axis2_char_t *key,
        axis2_property_t *value,
        const axis2_bool_t persistent);

    /**
     * Gets the property with the given key.
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @param key key string
     * @param persistent persistence status, AXIS2_TRUE if the value is to 
     * be retrieved from the persistent store, AXIS2_FALSE if it is to be 
     * retrieved from the non-persistent store
     * @return pointer to property struct corresponding to the given key
     */
    AXIS2_EXTERN axis2_property_t * AXIS2_CALL
    axis2_ctx_get_property(const axis2_ctx_t *ctx,
        const axis2_env_t *env,
        const axis2_char_t *key,
        const axis2_bool_t persistent);

    /**
     * Gets the non-persistent map of properties.
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @return pointer to the hash map which stores the non-persistent
     * properties
     */
    AXIS2_EXTERN axis2_hash_t * AXIS2_CALL
    axis2_ctx_get_non_persistent_map(const axis2_ctx_t *ctx,
        const axis2_env_t *env);

    /**
     * Gets the persistent map of properties.
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @return pointer to the hash map which stores the persistent
     * properties
     */
    AXIS2_EXTERN axis2_hash_t * AXIS2_CALL
    axis2_ctx_get_persistent_map(const axis2_ctx_t *ctx,
        const axis2_env_t *env);
                
    /**
     * Gets all properties stored within context. 
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @return pointer to hash table containing all properties
     */
    AXIS2_EXTERN axis2_hash_t * AXIS2_CALL
    axis2_ctx_get_all_properties(const axis2_ctx_t *ctx,
        const axis2_env_t *env);


    /**
     * Frees context struct.
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t  AXIS2_CALL
    axis2_ctx_free(axis2_ctx_t *ctx,
        const axis2_env_t *env);

    /**
     * Sets non-persistent map of properties.
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @param map pointer to hash map, context assumes ownership of the map
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */ 
    AXIS2_EXTERN axis2_status_t  AXIS2_CALL
    axis2_ctx_set_non_persistent_map(struct axis2_ctx *ctx,
        const axis2_env_t *env,
        axis2_hash_t *map);

    /**
     * Sets persistent map of properties.
     * @param ctx pointer to context struct
     * @param env pointer to environment struct
     * @param map pointer to hash map, context assumes ownership of the map
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t  AXIS2_CALL
    axis2_ctx_set_persistent_map(struct axis2_ctx *ctx,
        const axis2_env_t *env,
        axis2_hash_t *map);


/** Sets property with given key.
    @sa axis2_ctx_ops#set_property */
#define AXIS2_CTX_SET_PROPERTY(ctx, env, key, value, persistent) \
    axis2_ctx_set_property(ctx, env, key, value, persistent)

/** Gets property with given key.
    @sa axis2_ctx_ops#get_property */
#define AXIS2_CTX_GET_PROPERTY(ctx, env, key, persistent) \
    axis2_ctx_get_property(ctx, env, key, persistent)

/** Gets non persistent map of properties.
    @sa axis2_ctx_ops#get_non_persistent_map */
#define AXIS2_CTX_GET_NON_PERSISTANT_MAP(ctx, env) \
    axis2_ctx_get_non_persistent_map(ctx, env)

/** Gets all properties.
    @sa axis2_ctx_ops#get_all_properties */
#define AXIS2_CTX_GET_ALL_PROPERTIES(ctx, env) \
    axis2_ctx_get_all_properties(ctx, env)

/** Gets persistent map of properties.
    @sa axis2_ctx_ops#get_persistent_map */
#define AXIS2_CTX_GET_PERSISTANT_MAP(ctx, env) \
    axis2_ctx_get_persistent_map(ctx, env)

/** Sets non persistent map of properties.
    @sa axis2_ctx_ops#set_non_persistent_map */
#define AXIS2_CTX_SET_NON_PERSISTANT_MAP(ctx, env, map) \
    axis2_ctx_set_non_persistent_map(ctx, env, map)

/** Sets persistent map of properties.
    @sa axis2_ctx_ops#set_persistent_map */
#define AXIS2_CTX_SET_PERSISTANT_MAP(ctx, env, map) \
    axis2_ctx_set_persistent_map(ctx, env, map)

/** Frees the context.
    @sa axis2_ctx_ops#free */
#define AXIS2_CTX_FREE(ctx, env) \
    axis2_ctx_free (ctx, env)

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CTX_H */
