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

#ifndef AXIS2_SVC_GRP_CTX_H
#define AXIS2_SVC_GRP_CTX_H

/** @defgroup axis2_svc_grp_ctx service group context
 * @ingroup axis2_context
 * @{
 */

/**
 * @file axis2_svc_grp_ctx.h
 * @brief axis2 service group context interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_svc_ctx.h>
#include <axis2_svc_grp.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_svc_grp;
    /** Type name for struct svc_grp_ctx_ops */
    typedef struct axis2_svc_grp_ctx_ops axis2_svc_grp_ctx_ops_t;
    /** Type name for struct svc_grp_ctx */
    typedef struct axis2_svc_grp_ctx axis2_svc_grp_ctx_t;


    /**
     * service group context ops struct
     * Encapsulator struct for ops of axis2_svc_grp_ctx
     */
    struct axis2_svc_grp_ctx_ops
    {
        /**
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment
         */
        axis2_ctx_t *(AXIS2_CALL *
                get_base)(
                    const axis2_svc_grp_ctx_t *svc_grp_ctx,
                    const axis2_env_t *env);


        /**
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         */
        struct axis2_conf_ctx *(AXIS2_CALL *
                get_parent)(
                    const axis2_svc_grp_ctx_t *svc_grp_ctx,
                    const axis2_env_t *env);


        /**
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_svc_grp_ctx *svc_grp_ctx,
                    const axis2_env_t *env);

        /**
         * The method is used to do the intialization of the EngineContext
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         * @param conf pointer to conf
         */
        axis2_status_t (AXIS2_CALL *
                init)(
                    struct axis2_svc_grp_ctx *svc_grp_ctx,
                    const axis2_env_t *env,
                    struct axis2_conf *conf);


        /**
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_id)(
                    const axis2_svc_grp_ctx_t *svc_grp_ctx,
                    const axis2_env_t *env);


        /**
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         * @param id pointer to id
         */
        axis2_status_t (AXIS2_CALL *
                set_id)(
                    struct axis2_svc_grp_ctx *svc_grp_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *id);


        /** 
         * if the servic name is foo:bar , you should pass only bar 
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         * @param svc_name pointer to service name
         */
        struct axis2_svc_ctx *(AXIS2_CALL *
                get_svc_ctx)(
                    const axis2_svc_grp_ctx_t *svc_grp_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *svc_name);

        /**
         * This will create one service context per each service in descrpition
         * if service group has 2 service init , then two service contexts will be
         * created
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                fill_svc_ctx_map)(
                    struct axis2_svc_grp_ctx *svc_grp_ctx,
                    const axis2_env_t *env);


        /**
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         */
        struct axis2_svc_grp *(AXIS2_CALL *
                get_svc_grp)(
                    const axis2_svc_grp_ctx_t *svc_grp_ctx,
                    const axis2_env_t *env);


        /**
         * @param svc_grp_ctx pointer to service group context
         * @param env pointer to environment struct
         */
        axis2_hash_t *(AXIS2_CALL *
                get_svc_ctx_map)(
                    const axis2_svc_grp_ctx_t *svc_grp_ctx,
                    const axis2_env_t *env);
    };

    /**
     * service group context struct
     */
    struct axis2_svc_grp_ctx
    {
        /** operations of service group context */
        axis2_svc_grp_ctx_ops_t *ops;
    };


    /**
     * Creates a service group context struct
     * @param env pointer to environment struct
     * @param svc_grp pointer to service group
     * @param conf_ctx pointer to configuration context
     */
    AXIS2_EXTERN axis2_svc_grp_ctx_t *AXIS2_CALL
    axis2_svc_grp_ctx_create(
        const axis2_env_t *env,
        struct axis2_svc_grp *svc_grp,
        struct axis2_conf_ctx *conf_ctx);

/****************** Start of function macros **************************/

/** Gets the base.
    @sa axis2_grp_ctx_ops#get_base */
#define AXIS2_SVC_GRP_CTX_GET_BASE(svc_grp_ctx, env) \
      ((svc_grp_ctx)->ops->get_base(svc_grp_ctx, env))

/** Gets the parent.
    @sa axis2_grp_ctx_ops#get_parent */
#define AXIS2_SVC_GRP_CTX_GET_PARENT(svc_grp_ctx, env) \
      ((svc_grp_ctx)->ops->get_parent(svc_grp_ctx, env))

/** Frees the service group context.
    @sa axis2_grp_ctx_ops#free */
#define AXIS2_SVC_GRP_CTX_FREE(svc_grp_ctx, env)\
      ((svc_grp_ctx)->ops->free(svc_grp_ctx, env))

/** Initialize the service group context.
    @sa axis2_grp_ctx_ops#init */
#define AXIS2_SVC_GRP_CTX_INIT(svc_grp_ctx, env, conf)\
      ((svc_grp_ctx)->ops->init(svc_grp_ctx, env, conf))

/** Gets the id.
    @sa axis2_grp_ctx_ops#get_id */
#define AXIS2_SVC_GRP_CTX_GET_ID(svc_grp_ctx, env) \
      ((svc_grp_ctx)->ops->get_id(svc_grp_ctx, env))

/** Set id.
    @sa axis2_grp_ctx_ops#set id */
#define AXIS2_SVC_GRP_CTX_SET_ID(svc_grp_ctx, env, id) \
      ((svc_grp_ctx)->ops->set_id(svc_grp_ctx, env, id))

/** Gets the service context.
    @sa axis2_grp_ctx_ops#get_svc_ctx */
#define AXIS2_SVC_GRP_CTX_GET_SVC_CTX(svc_grp_ctx, env, svc_name) \
      ((svc_grp_ctx)->ops->get_svc_ctx(svc_grp_ctx, env, svc_name))

/** Gets the service context map.
    @sa axis2_grp_ctx_ops#fill_svc_ctx_map */
#define AXIS2_SVC_GRP_CTX_FILL_SVC_CTX_MAP(svc_grp_ctx, env) \
      ((svc_grp_ctx)->ops->fill_svc_ctx_map(svc_grp_ctx, env))

/** Gets the service group.
    @sa axis2_grp_ctx_ops#get_svc_grp */
#define AXIS2_SVC_GRP_GET_SVC_GRP(svc_grp_ctx, env) \
      ((svc_grp_ctx)->ops->get_svc_grp(svc_grp_ctx, env))

/** Gets the service context map.
    @sa axis2_grp_ctx_ops#get_svc_ctx_map */
#define AXIS2_SVC_GRP_GET_SVC_CTX_MAP(svc_grp_ctx, env) \
      ((svc_grp_ctx)->ops->get_svc_ctx_map(svc_grp_ctx, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_SVC_GRP_CTX_H */
