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

#ifndef AXIS2_CTX_H
#define AXIS2_CTX_H

/**
 * @defgroup axis2_context context hierarchy
 * @ingroup axis2
 * @{
 * @}
 */

/** @defgroup axis2_ctx context
 * @ingroup axis2_context
 * Description.
 * @{
 */

/**
 * @file axis2_ctx.h
 * @brief axis2 context interface
 */

#include <axis2_defines.h>
#include <axis2_hash.h>
#include <axis2_env.h>
#include <axis2_property.h>

#ifdef __cplusplus
extern "C"
{
#endif


    /** Type name for struct axis2_ctx_ops */
    typedef struct axis2_ctx_ops axis2_ctx_ops_t;
    /** Type name for struct axis2_ctx */
    typedef struct axis2_ctx axis2_ctx_t;


    /**
     * context ops struct
     * Encapsulator struct for ops of axis2_ctx
     */
    struct axis2_ctx_ops
    {
        /**
         * Stores a key value pair depending on the persistent flag.
         * @param ctx pointer to context struct
         * @param env pointer to environment struct
         * @param key pointer to key
         * @param value pointer to value
         * @param persistent persistent
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_property)(
                    struct axis2_ctx *ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *key,
                    axis2_property_t *value,
                    const axis2_bool_t persistent);

        /**
         * @param ctx pointer to context struct
         * @param env pointer to environment struct
         * @param key pointer to key
         * @param persistent persistent
         */
        axis2_property_t *(AXIS2_CALL *
                get_property)(
                    struct axis2_ctx *ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *key,
                    const axis2_bool_t persistent);

        /**
         * @param ctx pointer to context struct
         * @param env pointer to environment struct
         */
        axis2_hash_t* (AXIS2_CALL *
                get_non_persistent_map)(
                    struct axis2_ctx *ctx,
                    const axis2_env_t *env);

        /**
         * @param ctx pointer to context struct
         * @param env pointer to environment struct
         */
        axis2_hash_t* (AXIS2_CALL *
                get_persistent_map)(
                    struct axis2_ctx *ctx,
                    const axis2_env_t *env);
        /**
         * @param ctx pointer to context struct
         * @param env pointer to environment struct
         */
        axis2_hash_t* (AXIS2_CALL *
                get_properties)(
                    struct axis2_ctx *ctx,
                    const axis2_env_t *env);


        /**
         * Deallocate memory
         * @param ctx pointer to context struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_ctx_t *ctx,
                    const axis2_env_t *env);

        /**
         * @param ctx pointer to context struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */ 
        axis2_status_t (AXIS2_CALL *
                set_non_persistent_map)(
                    struct axis2_ctx *ctx,
                    const axis2_env_t *env,
                    axis2_hash_t* map);

        /**
         * @param ctx pointer to context struct
         * @param env pointer to enviromnet struct
         * @param map pointer to map
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_persistent_map)(
                    struct axis2_ctx *ctx,
                    const axis2_env_t *env,
                    axis2_hash_t* map);
    };

    /**
     * context struct
     */
    struct axis2_ctx
    {
	/** operations of context struct */
        axis2_ctx_ops_t *ops;
    };

    /**
     * Creates a context struct
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_ctx_t *AXIS2_CALL 
    axis2_ctx_create(
        const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Set property.
    @sa axis2_ctx_ops#set_property */
#define AXIS2_CTX_SET_PROPERTY(ctx, env, key, value, persistent) \
    ((ctx)->ops->set_property(ctx, env, key, value, persistent))

/** Get property.
    @sa axis2_ctx_ops#get_property */
#define AXIS2_CTX_GET_PROPERTY(ctx, env, key, persistent) \
    ((ctx)->ops->get_property(ctx, env, key, persistent))

/** Get non persistant.
    @sa axis2_ctx_ops#get_non_persistant_map */
#define AXIS2_CTX_GET_NON_PERSISTANT_MAP(ctx, env) \
    ((ctx)->ops->get_non_persistent_map(ctx, env))

/** Get properties.
    @sa axis2_ctx_ops#get_properties */
#define AXIS2_CTX_GET_PROPERTIES(ctx, env) \
    ((ctx)->ops->get_properties(ctx, env))

/** Get persistant map.
    @sa axis2_ctx_ops#get_persistant_map */
#define AXIS2_CTX_GET_PERSISTANT_MAP(ctx, env) \
    ((ctx)->ops->get_persistent_map(ctx, env))

/** Set non persistant map.
    @sa axis2_ctx_ops#set_non_persistant_map */
#define AXIS2_CTX_SET_NON_PERSISTANT_MAP(ctx, env, map) \
    ((ctx)->ops->set_non_persistent_map(ctx, env, map))

/** set persistant map.
    @sa axis2_ctx_ops#set_persistant_map */
#define AXIS2_CTX_SET_PERSISTANT_MAP(ctx, env, map) \
    ((ctx)->ops->set_persistent_map(ctx, env, map))

/** Frees the context.
    @sa axis2_ctx_ops#free */
#define AXIS2_CTX_FREE(ctx, env) \
    ((ctx)->ops->free (ctx, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CTX_H */
