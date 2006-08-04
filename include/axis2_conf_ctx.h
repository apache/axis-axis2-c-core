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

/** @defgroup axis2_conf_ctx configuration context
 * @ingroup axis2_context
 * Description.
 * @{
 */

/**
 * @file axis2_conf_ctx.h
 * @brief axis2 configuration context interface
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


    /** Type name for struct axis2_conf_ctx_ops */
    typedef struct axis2_conf_ctx_ops axis2_conf_ctx_ops_t;
    /** Type name for struct axis2_conf_ctx */
    typedef struct axis2_conf_ctx axis2_conf_ctx_t;
    struct axis2_conf;

    /**
     * configuration context ops struct
     * Encapsulator struct for ops of axis2_conf_ctx
     */
    struct axis2_conf_ctx_ops
    {
        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param conf pointer to configuration 
         */
        axis2_status_t (AXIS2_CALL * 
                set_conf)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env,
                    struct axis2_conf *conf);
        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         */
        axis2_ctx_t *(AXIS2_CALL *
                get_base)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         */
        struct axis2_conf *(AXIS2_CALL *
                get_conf)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_op_ctx_map)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_svc_ctx_map)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_svc_grp_ctx_map)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * Register a struct axis2_op_ctx against a given Message ID.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param message_id pointer to message id
         * @param op_ctx pointer to operation context
         */
        axis2_status_t (AXIS2_CALL *
                register_op_ctx)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *message_id,
                    struct axis2_op_ctx *op_ctx);

        /**
         * get axis2_op_ctx struct given a Message ID
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param message_id pointer to environment struct
         * @return struct axis2_op_ctx * <code>struct axis2_op_ctx *<code>
         */
        struct axis2_op_ctx *(AXIS2_CALL *
                get_op_ctx)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *message_id);

        /**
         * Register a struct axis2_svc_ctx against a given Message ID.
         * @param conf_ctx pointer t oconfiguration context
         * @param env pointer to environment struct
         * @param svc_id pointer to service id
         * @param svc_ctx pointer to service context
         */
        axis2_status_t (AXIS2_CALL *
                register_svc_ctx)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *svc_id,
                    struct axis2_svc_ctx *svc_ctx);

        /**
         * get axis2_svc_ctx struct given a Message ID
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param svc_id pointer to service id
         * @return struct axis2_svc_ctx * <code>struct axis2_svc_ctx *<code>
         */
        struct axis2_svc_ctx *(AXIS2_CALL *
                get_svc_ctx)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *svc_id);

        /**
         * Register a struct axis2_svc_grp_ctx against a given Message ID.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param svc_grp_id pointer to service group id
         * @param svc_grp_ctx pointer to service group context
         */
        axis2_status_t (AXIS2_CALL *
                register_svc_grp_ctx)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *svc_grp_id,
                    struct axis2_svc_grp_ctx *svc_grp_ctx);

        /**
         * get axis2_svc_grp_ctx struct given a service group ID
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param svc_grp_id pointer to service group id
         * @return struct axis2_svc_grp_ctx * <code>struct axis2_svc_grp_ctx *<code>
         */
        struct axis2_svc_grp_ctx *(AXIS2_CALL *
                get_svc_grp_ctx)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *svc_grp_id);

        /**
         * This method allows users to reolve the paths relative to the
         * root diretory
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_root_dir)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param path pointer to path
         */
        axis2_status_t (AXIS2_CALL *
                set_root_dir)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *path);

        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param conf pointer to conf
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env,
                    struct axis2_conf *conf);
   
        /**
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env);

        /**
         * This method should search for a service group context in the map with given id as the key.
         * If(key != null && found)
         * check for a service context for the intended service.
         * if (!found)
         * create one and hook up to service group context
         * else
         * create new service group context with the given key or if key is null with a new key
         * create a new service context for the service
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @param messageContext
         */
        struct axis2_svc_grp_ctx *(AXIS2_CALL *
                fill_ctxs)(
                    struct axis2_conf_ctx *conf_ctx,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
    };

    /**
     * configuration context struct
     */
    struct axis2_conf_ctx
    {
        /** operations configuration context struct */
        axis2_conf_ctx_ops_t *ops;
    };

    /**
     * Creates a configuration context struct
     * @param env pointer to environment struct
     * @param conf pointer to conf
     */
    AXIS2_EXTERN axis2_conf_ctx_t *AXIS2_CALL
    axis2_conf_ctx_create(
        const axis2_env_t *env, 
        struct axis2_conf *conf);

/************************** Start of function macros **************************/

/** Sets the configuratoin.
    @sa axis2_conf_ctx_ops#set_conf */
#define AXIS2_CONF_CTX_SET_CONF(conf_ctx, env, conf) ((conf_ctx)->ops->set_conf(conf_ctx, env, conf))

/** Gets the base.
    @sa axis2_conf_ctx_ops#get_base */
#define AXIS2_CONF_CTX_GET_BASE(conf_ctx, env) ((conf_ctx)->ops->get_base(conf_ctx, env))

/** Ges the configuration.
    @sa axis2_conf_ctx_ops#get_conf */
#define AXIS2_CONF_CTX_GET_CONF(conf_ctx, env) ((conf_ctx)->ops->get_conf(conf_ctx, env))

/** Gets the operation context map.
    @sa axis2_conf_ctx_ops#get_op_ctx_map */
#define AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, env) ((conf_ctx)->ops->get_op_ctx_map(conf_ctx, env))

/** Gets the service context map.
    @sa axis2_conf_ctx_ops#get_svc_ctx_map */
#define AXIS2_CONF_CTX_GET_SVC_CTX_MAP(conf_ctx, env) ((conf_ctx)->ops->get_svc_ctx_map(conf_ctx, env))

/** Gets the service group context map.
    @sa axis2_conf_ctx_ops#get_svc_grp_ctx_map */
#define AXIS2_CONF_CTX_GET_SVC_GRP_CTX_MAP(conf_ctx, env) ((conf_ctx)->ops->get_svc_grp_ctx_map(conf_ctx, env))
    
/** Registers the operation context.
    @sa axis2_conf_ctx_ops#register_op_ctx */
#define AXIS2_CONF_CTX_REGISTER_OP_CTX(conf_ctx, env, message_id, op_ctx) ((conf_ctx)->ops->register_op_ctx(conf_ctx, env, message_id, op_ctx))

/** Gets the opratoin context .
    @sa axis2_conf_ctx_ops#get_op_ctx */
#define AXIS2_CONF_CTX_GET_OP_CTX(conf_ctx, env, message_id) ((conf_ctx)->ops->get_op_ctx(conf_ctx, env, message_id))

/** Registers the service conetxt.
    @sa axis2_conf_ctx_ops#register_svc_ctx */
#define AXIS2_CONF_CTX_REGISTER_SVC_CTX(conf_ctx, env, svc_id, svc_ctx) ((conf_ctx)->ops->register_svc_ctx(conf_ctx, env, svc_id, svc_ctx))

/** Gets the service context.
    @sa axis2_conf_ctx_ops#get_svc_ctx */
#define AXIS2_CONF_CTX_GET_SVC_CTX(conf_ctx, env, svc_id) ((conf_ctx)->ops->get_svc_ctx(conf_ctx, env, svc_id))

/** Registers the service group context.
    @sa axis2_conf_ctx_ops#register_svc_grp_ctx */
#define AXIS2_CONF_CTX_REGISTER_SVC_GRP_CTX(conf_ctx, env, svc_grp_id, svc_grp_ctx) ((conf_ctx)->ops->register_svc_grp_ctx(conf_ctx, env, svc_grp_id, svc_grp_ctx))

/** Gets the service group context.
    @sa axis2_conf_ctx_ops#get_svc_grp_ctx */
#define AXIS2_CONF_CTX_GET_SVC_GRP_CTX(conf_ctx, env, svc_grp_id) ((conf_ctx)->ops->get_svc_grp_ctx(conf_ctx, env, svc_grp_id))

/** Gets the root dir.
    @sa axis2_conf_ctx_ops#get_root_dir */
#define AXIS2_CONF_CTX_GET_ROOT_DIR(conf_ctx, env) ((conf_ctx)->ops->get_root_dir(conf_ctx, env))

/** Sets the root dir.
    @sa axis2_conf_ctx_ops#set_root_dir */
#define AXIS2_CONF_CTX_SET_ROOT_DIR(conf_ctx, env, path) ((conf_ctx)->ops->set_root_dir(conf_ctx, env, path))

/** Initilize the configuratoin context.
    @sa axis2_conf_ctx_ops#init */
#define AXIS2_CONF_CTX_INIT(conf_ctx, env, conf) ((conf_ctx)->ops->init(conf_ctx, env, conf))

/** Frees the configuration context.
    @sa axis2_conf_ctx_ops#free */
#define AXIS2_CONF_CTX_FREE(conf_ctx, env) ((conf_ctx)->ops->free(conf_ctx, env))

/** Fill ctxs.
    @sa axis2_conf_ctx_ops#fill_ctxs */
#define AXIS2_CONF_CTX_FILL_CTXS(conf_ctx, env, msg_ctx) ((conf_ctx)->ops->fill_ctxs(conf_ctx, env, msg_ctx))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CONF_CTX_H */
