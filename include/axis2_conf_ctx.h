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

#ifndef AXIS2_CONF_CTX_H
#define AXIS2_CONF_CTX_H

/** 
 * @defgroup axis2_conf_ctx configuration context
 * @ingroup axis2_context
 * configuration context is the holder for all the state information 
 * related to configuration. It holds all the service group context, service
 * context and operation context that exists within an engine instance. 
 * An engine instance has only one configuration context associated with it
 * (Singleton pattern). 
 * @{
 */

/**
 * @file axis2_conf_ctx.h
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
     * configuration context ops struct.
     * Encapsulator struct for ops of axis2_conf_ctx.
     */
    struct axis2_conf_ctx_ops
    {
        /**
         * Sets the configuration associated with the engine instance.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param conf pointer to configuration 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL * 
                set_conf)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    struct axis2_conf *conf);
        /**
         * Gets the base struct, which is of type context
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @return pointer to context struct, returns a reference not a cloned
         * copy
         */
        axis2_ctx_t *(AXIS2_CALL *
                get_base)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * Gets the configuration of the engine.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @return pointer to configuration struct, returns a reference not a 
         * cloned copy
         */
        axis2_conf_t *(AXIS2_CALL *
                get_conf)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * Gets the hash map of operation context instances.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @return pointer to hash map containing all operation contexts
         */
        axis2_hash_t *(AXIS2_CALL *
                get_op_ctx_map)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * Gets the hash map of service context instances.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @return pointer to hash map containing all service contexts
         */
        axis2_hash_t *(AXIS2_CALL *
                get_svc_ctx_map)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * Gets the hash map of service group context instances.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @return pointer to hash map containing all service group contexts
         */
        axis2_hash_t *(AXIS2_CALL *
                get_svc_grp_ctx_map)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * Registers an operation context with the given message ID.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param message_id message id related to the operation context
         * @param op_ctx pointer to operation context, conf context assumes
         * ownership of the operation context 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                register_op_ctx)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *message_id,
                    axis2_op_ctx_t *op_ctx);

        /**
         * Gets operation context corresponding to the given message ID.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param message_id message ID related to the operation to be retrieved
         * @return pointer to operation context related to the given message ID
         */
        axis2_op_ctx_t *(AXIS2_CALL *
                get_op_ctx)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *message_id);

        /**
         * Registers a service context with the given service ID.
         * @param conf_ctx pointer t configuration context
         * @param env pointer to environment struct
         * @param svc_id ID of the service to be added
         * @param svc_ctx pointer to service context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                register_svc_ctx)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_id,
                    axis2_svc_ctx_t *svc_ctx);

        /**
         * Gets service context with the given service ID
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param svc_id service ID
         * @return pointer to service context with the given service ID 
         */
        struct axis2_svc_ctx *(AXIS2_CALL *
                get_svc_ctx)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_id);

        /**
         * Registers a service group context with the given service group ID.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param svc_grp_id service group id
         * @param svc_grp_ctx pointer to service group context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                register_svc_grp_ctx)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_grp_id,
                    axis2_svc_grp_ctx_t *svc_grp_ctx);

        /**
         * Gets service group with the given service group ID.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param svc_grp_id service group id
         * @return pointer to service group context with the given ID
         */
        axis2_svc_grp_ctx_t *(AXIS2_CALL *
                get_svc_grp_ctx)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_grp_id);

        /**
         * Gets the root working directory. It is in this directory that the 
         * axis2.xml configuration file is located. The services and modules 
         * sub folders too are located in this directory.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @return pointer to string containing the root folder name 
         */
        const axis2_char_t *(AXIS2_CALL *
                get_root_dir)(
                    const axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * Sets the root working directory. It is in this directory that the 
         * axis2.xml configuration file is located. The services and modules 
         * sub folders too are located in this directory.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param path string containing the path of root directory
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_root_dir)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *path);

        /**
         * Initializes the configuration context. Within this function, it would 
         * initialize all the service group context, service context and 
         * operation context instances stored within configuration context.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param conf pointer to configuration struct 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    axis2_conf_t *conf);
   
        /**
         * Frees configuration context struct.
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env);

        /**
         * This method fills the context hierarchy (service group, service and 
         * operation contexts that is) for the service and operation found in 
         * the given message context. If the context hierarchy is not already 
         * built it will create the contexts and build the context hierarchy.         
         * @param conf_ctx pointer to configuration context
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context with service and operation
         * for which the context hierarchy is to be built set
         * @return pointer to the service group context, which is the root of 
         * the context hierarchy for given service and operation
         */
        axis2_svc_grp_ctx_t *(AXIS2_CALL *
                fill_ctxs)(
                    axis2_conf_ctx_t *conf_ctx,
                    const axis2_env_t *env,
                    axis2_msg_ctx_t *msg_ctx);
    };

    /**
     * configuration context struct.
     */
    struct axis2_conf_ctx
    {
        /** operations configuration context struct */
        axis2_conf_ctx_ops_t *ops;
    };

    /**
     * Creates a configuration context struct instance.
     * @param env pointer to environment struct
     * @param conf pointer to configuration, configuration context assumes 
     * ownership of the configuration
     * @return pointer to newly created configuration context 
     */
    AXIS2_EXTERN axis2_conf_ctx_t *AXIS2_CALL
    axis2_conf_ctx_create(
        const axis2_env_t *env, 
        struct axis2_conf *conf);

/** Sets the configuration.
    @sa axis2_conf_ctx_ops#set_conf */
#define AXIS2_CONF_CTX_SET_CONF(conf_ctx, env, conf) \
        ((conf_ctx)->ops->set_conf(conf_ctx, env, conf))

/** Gets base which is of type context.
    @sa axis2_conf_ctx_ops#get_base */
#define AXIS2_CONF_CTX_GET_BASE(conf_ctx, env) \
        ((conf_ctx)->ops->get_base(conf_ctx, env))

/** Gets the configuration.
    @sa axis2_conf_ctx_ops#get_conf */
#define AXIS2_CONF_CTX_GET_CONF(conf_ctx, env) \
        ((conf_ctx)->ops->get_conf(conf_ctx, env))

/** Gets the operation context map.
    @sa axis2_conf_ctx_ops#get_op_ctx_map */
#define AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, env) \
        ((conf_ctx)->ops->get_op_ctx_map(conf_ctx, env))

/** Gets the service context map.
    @sa axis2_conf_ctx_ops#get_svc_ctx_map */
#define AXIS2_CONF_CTX_GET_SVC_CTX_MAP(conf_ctx, env) \
        ((conf_ctx)->ops->get_svc_ctx_map(conf_ctx, env))

/** Gets the service group context map.
    @sa axis2_conf_ctx_ops#get_svc_grp_ctx_map */
#define AXIS2_CONF_CTX_GET_SVC_GRP_CTX_MAP(conf_ctx, env) \
        ((conf_ctx)->ops->get_svc_grp_ctx_map(conf_ctx, env))
    
/** Registers the operation context with given message ID.
    @sa axis2_conf_ctx_ops#register_op_ctx */
#define AXIS2_CONF_CTX_REGISTER_OP_CTX(conf_ctx, env, message_id, op_ctx) \
        ((conf_ctx)->ops->register_op_ctx(conf_ctx, env, message_id, op_ctx))

/** Gets the operation context with given message ID.
    @sa axis2_conf_ctx_ops#get_op_ctx */
#define AXIS2_CONF_CTX_GET_OP_CTX(conf_ctx, env, message_id) \
        ((conf_ctx)->ops->get_op_ctx(conf_ctx, env, message_id))

/** Registers the service context with given service ID.
    @sa axis2_conf_ctx_ops#register_svc_ctx */
#define AXIS2_CONF_CTX_REGISTER_SVC_CTX(conf_ctx, env, svc_id, svc_ctx) \
        ((conf_ctx)->ops->register_svc_ctx(conf_ctx, env, svc_id, svc_ctx))

/** Gets the service context with given service ID.
    @sa axis2_conf_ctx_ops#get_svc_ctx */
#define AXIS2_CONF_CTX_GET_SVC_CTX(conf_ctx, env, svc_id) \
        ((conf_ctx)->ops->get_svc_ctx(conf_ctx, env, svc_id))

/** Registers the service group context with given service group ID.
    @sa axis2_conf_ctx_ops#register_svc_grp_ctx */
#define AXIS2_CONF_CTX_REGISTER_SVC_GRP_CTX(conf_ctx, env, svc_grp_id, svc_grp_ctx) \
        ((conf_ctx)->ops->register_svc_grp_ctx(conf_ctx, env, svc_grp_id, svc_grp_ctx))

/** Gets the service group context with given service group ID.
    @sa axis2_conf_ctx_ops#get_svc_grp_ctx */
#define AXIS2_CONF_CTX_GET_SVC_GRP_CTX(conf_ctx, env, svc_grp_id) \
        ((conf_ctx)->ops->get_svc_grp_ctx(conf_ctx, env, svc_grp_id))

/** Gets the root directory.
    @sa axis2_conf_ctx_ops#get_root_dir */
#define AXIS2_CONF_CTX_GET_ROOT_DIR(conf_ctx, env) \
        ((conf_ctx)->ops->get_root_dir(conf_ctx, env))

/** Sets the root directory.
    @sa axis2_conf_ctx_ops#set_root_dir */
#define AXIS2_CONF_CTX_SET_ROOT_DIR(conf_ctx, env, path) \
        ((conf_ctx)->ops->set_root_dir(conf_ctx, env, path))

/** Initializes the configuration context.
    @sa axis2_conf_ctx_ops#init */
#define AXIS2_CONF_CTX_INIT(conf_ctx, env, conf) \
        ((conf_ctx)->ops->init(conf_ctx, env, conf))

/** Frees the configuration context.
    @sa axis2_conf_ctx_ops#free */
#define AXIS2_CONF_CTX_FREE(conf_ctx, env) \
        ((conf_ctx)->ops->free(conf_ctx, env))

/** Fill context hierarchy related to the service and operation set in message 
    context.
    @sa axis2_conf_ctx_ops#fill_ctxs */
#define AXIS2_CONF_CTX_FILL_CTXS(conf_ctx, env, msg_ctx) \
        ((conf_ctx)->ops->fill_ctxs(conf_ctx, env, msg_ctx))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CONF_CTX_H */
