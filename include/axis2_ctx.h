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
  * @file axis2_ctx.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_hash.h>
#include <axis2_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_ctx Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_ctx_ops axis2_ctx_ops_t;
typedef struct axis2_ctx axis2_ctx_t; 

    
/** 
 * @brief Message Context operations struct
 * Encapsulator struct for operations of axis2_ctx
 */  
struct axis2_ctx_ops
{
    /**
     * Stores a key value pair depending on the persistent flag.
     *
     * @param key
     * @param value
     * @param persistent
     */
    axis2_status_t (AXIS2_CALL *set_property)(struct axis2_ctx *ctx, axis2_env_t **env, axis2_char_t *key, axis2_status_t *value, axis2_bool_t persistent);
    
    /**
     * @param key
     * @param persistent
     * @return
     */
    axis2_char_t* (AXIS2_CALL *get_property)(struct axis2_ctx *ctx, axis2_env_t **env, axis2_char_t *key, axis2_bool_t persistent);
    
    axis2_hash_t* (AXIS2_CALL *get_non_persistent_map)(struct axis2_ctx *ctx, 
                                                           axis2_env_t **env);
    
    axis2_hash_t* (AXIS2_CALL *get_persistent_map)(struct axis2_ctx *ctx, 
                                                   axis2_env_t **env);

    /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free)(axis2_ctx_t *ctx,
                                        axis2_env_t **env); 
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_ctx
{
    axis2_ctx_ops_t *ops;    
};

AXIS2_DECLARE(axis2_ctx_t*) axis2_ctx_create (axis2_env_t **env);
    
/************************** Start of function macros **************************/

#define AXIS2_CTX_SET_PROPERTY(ctx, env, key, value, persistent) ((ctx)->ops->set_property(ctx, env, key, value, persistent))
#define AXIS2_CTX_GET_PROPERTY(ctx, env, key, persistent) ((ctx)->ops->get_property(ctx, env, key, persistent))
#define AXIS2_CTX_GET_NON_PERSISTANT_MAP(ctx, env) ((ctx)->ops->get_non_persistent_map(ctx, env))
#define AXIS2_CTX_GET_PERSISTANT_MAP(ctx, env) ((ctx)->ops->get_persistent_map(ctx, env))
#define AXIS2_CTX_FREE(ctx, env) ((ctx)->ops->free (ctx, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CTX_H */
