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

#ifndef AXIS2_CONF_CTX_H
#define AXIS2_CONF_CTX_H


/**
  * @file axis2_conf_ctx.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_hash.h>
#include <axis2_env.h>
#include <axis2_ctx.h>
#include <axis2_svc_grp_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_conf_ctx Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_conf_ctx_ops axis2_conf_ctx_ops_t;
typedef struct axis2_conf_ctx axis2_conf_ctx_t; 
struct axis2_engine_config;
    
/** 
 * @brief Message Context operations struct
 * Encapsulator struct for operations of axis2_conf_ctx
 */  
struct axis2_conf_ctx_ops
{
    /**
     * @param configuration
     */
    axis2_status_t (AXIS2_CALL *set_engine_config)(struct axis2_conf_ctx *conf_ctx, 
        axis2_env_t **env, 
        struct axis2_engine_config *engine_config);
    /**
     */
    axis2_ctx_t* (AXIS2_CALL *get_base)(struct axis2_conf_ctx *conf_ctx, 
        axis2_env_t **env);
    /**
     */
    struct axis2_engine_config* (AXIS2_CALL *get_engine_config)(struct axis2_conf_ctx *conf_ctx, 
        axis2_env_t **env);
    axis2_hash_t* (AXIS2_CALL *get_operation_ctx_map)(struct axis2_conf_ctx *conf_ctx, 
        axis2_env_t **env);
    axis2_hash_t* (AXIS2_CALL *get_svc_ctx_map)(struct axis2_conf_ctx *conf_ctx, 
        axis2_env_t **env);
    axis2_hash_t* (AXIS2_CALL *get_svc_grp_ctx_map)(struct axis2_conf_ctx *conf_ctx, 
                                                                axis2_env_t **env);
    /**
     * Register a struct axis2_operation_ctx against a given Message ID.
     *
     * @param messageID
     * @param mepContext
     */
    axis2_status_t (AXIS2_CALL *register_operation_ctx)(struct axis2_conf_ctx *conf_ctx, 
                                                                    axis2_env_t **env,
                                                                    axis2_char_t *message_id,
                                                                    axis2_operation_ctx_t *operation_ctx);
    /**
     * get axis2_operation_ctx struct given a Message ID
     *
     * @param message_id
     * @return struct axis2_operation_ctx * <code>struct axis2_operation_ctx *<code>
     */
    axis2_operation_ctx_t* (AXIS2_CALL *get_operation_ctx)(struct axis2_conf_ctx *conf_ctx, 
                                                                            axis2_env_t **env,
                                                                            axis2_char_t *message_id);
    /**
     * Register a struct axis2_svc_ctx against a given Message ID.
     *
     * @param svc_id
     * @param mepContext
     */
    axis2_status_t (AXIS2_CALL *register_svc_ctx)(struct axis2_conf_ctx *conf_ctx, 
                                                                    axis2_env_t **env,
                                                                    axis2_char_t *svc_id,
                                                                    axis2_svc_ctx_t *svc_ctx);
    /**
     * get axis2_svc_ctx struct given a Message ID
     *
     * @param svc_id
     * @return struct axis2_svc_ctx * <code>struct axis2_svc_ctx *<code>
     */
    axis2_svc_ctx_t* (AXIS2_CALL *get_svc_ctx)(struct axis2_conf_ctx *conf_ctx, 
                                                                            axis2_env_t **env,
                                                                            axis2_char_t *svc_id);
    /**
     * Register a struct axis2_svc_grp_ctx against a given Message ID.
     *
     * @param svc_grp_id
     * @param mepContext
     */
    axis2_status_t (AXIS2_CALL *register_svc_grp_ctx)(struct axis2_conf_ctx *conf_ctx, 
                                                                    axis2_env_t **env,
                                                                    axis2_char_t *svc_grp_id,
                                                                    axis2_svc_grp_ctx_t *svc_grp_ctx);
    /**
     * get axis2_svc_grp_ctx struct given a service group ID
     *
     * @param svc_grp_id
     * @return struct axis2_svc_grp_ctx * <code>struct axis2_svc_grp_ctx *<code>
     */
    axis2_svc_grp_ctx_t* (AXIS2_CALL *get_svc_grp_ctx)(struct axis2_conf_ctx *conf_ctx, 
                                                                            axis2_env_t **env,
                                                                            axis2_char_t *svc_grp_id);
    /**
     * This method allows users to reolve the paths relative to the
     * root diretory
     *
     * @param path
     */
    axis2_char_t* (AXIS2_CALL *get_root_dir)(struct axis2_conf_ctx *conf_ctx, 
                                                            axis2_env_t **env);
    /**
     * @param file
     */
    axis2_status_t (AXIS2_CALL *set_root_dir)(struct axis2_conf_ctx *conf_ctx, 
                                                            axis2_env_t **env,
                                                            axis2_char_t *path);
    axis2_status_t (AXIS2_CALL *init)(struct axis2_conf_ctx *conf_ctx, 
                                                            axis2_env_t **env,
                                                            struct axis2_engine_config *engine_config);
    axis2_status_t (AXIS2_CALL *free)(struct axis2_conf_ctx *conf_ctx, 
                                       axis2_env_t **env);
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
    axis2_svc_grp_ctx_t* (AXIS2_CALL *fill_ctxs)(struct axis2_conf_ctx *conf_ctx, 
                                                   axis2_env_t **env,
                                                   axis2_msg_ctx_t *msg_ctx);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_conf_ctx
{
    axis2_conf_ctx_ops_t *ops;    
};

AXIS2_DECLARE(axis2_conf_ctx_t*) AXIS2_CALL create(axis2_env_t **env, struct axis2_engine_config *engine_config);
    
/************************** Start of function macros **************************/

#define AXIS2_CONF_CTX_SET_ENGINE_CONFIG(conf_ctx, env, engine_config) ((conf_ctx)->ops->set_engine_config(conf_ctx, env, engine_config))
#define AXIS2_CONF_CTX_GET_BASE(conf_ctx, env) ((conf_ctx)->ops->get_base(conf_ctx, env))
#define AXIS2_CONF_CTX_GET_ENGINE_CONFIG(conf_ctx, env) ((conf_ctx)->ops->get_engine_config(conf_ctx, env))
#define AXIS2_CONF_CTX_GET_OPERATION_CTX_MAP(conf_ctx, env) ((conf_ctx)->ops->get_operation_ctx_map(conf_ctx, env))
#define AXIS2_CONF_CTX_GET_SVC_CTX_MAP(conf_ctx, env) ((conf_ctx)->ops->get_svc_ctx_map(conf_ctx, env))
#define AXIS2_CONF_CTX_GET_SVC_GRP_CTX_MAP(conf_ctx, env) ((conf_ctx)->ops->get_svc_grp_ctx_map(conf_ctx, env))
#define AXIS2_CONF_CTX_REGISTER_OPERATION_CTX(conf_ctx, env, message_id, operation_ctx) ((conf_ctx)->ops->register_operation_ctx(conf_ctx, env, message_id, operation_ctx))
#define AXIS2_CONF_CTX_GET_OPERATION_CTX(conf_ctx, env, message_id) ((conf_ctx)->ops->get_operation_ctx(conf_ctx, env, message_id))
#define AXIS2_CONF_CTX_REGISTER_SVC_CTX(conf_ctx, env, svc_id, svc_ctx) ((conf_ctx)->ops->register_svc_ctx(conf_ctx, env, svc_id, svc_ctx))
#define AXIS2_CONF_CTX_GET_SVC_CTX(conf_ctx, env, svc_id) ((conf_ctx)->ops->get_svc_ctx(conf_ctx, env, svc_id))
#define AXIS2_CONF_CTX_REGISTER_SVC_GRP_CTX(conf_ctx, env, svc_grp_id, svc_grp_ctx) ((conf_ctx)->ops->register_svc_grp_ctx(conf_ctx, env, svc_grp_id, svc_grp_ctx))
#define AXIS2_CONF_CTX_GET_SVC_GRP_CTX(conf_ctx, env, svc_grp_id) ((conf_ctx)->ops->get_svc_grp_ctx(conf_ctx, env, svc_grp_id))
#define AXIS2_CONF_CTX_GET_ROOT_DIR(conf_ctx, env) ((conf_ctx)->ops->get_root_dir(conf_ctx, env))
#define AXIS2_CONF_CTX_SET_ROOT_DIR(conf_ctx, env, path) ((conf_ctx)->ops->set_root_dir(conf_ctx, env, path))
#define AXIS2_CONF_CTX_INIT(conf_ctx, env, engine_config) ((conf_ctx)->ops->init(conf_ctx, env, engine_config))
#define AXIS2_CONF_CTX_FREE(conf_ctx, env) ((conf_ctx)->ops->free(conf_ctx, env))
#define AXIS2_CONF_CTX_FILL_CXTS(conf_ctx, env, msg_ctx) ((conf_ctx)->ops->fill_ctxs(conf_ctx, env, msg_ctx))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CONF_CTX_H */
