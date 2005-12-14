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


/**
  * @file axis2_svc_grp_ctx.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_svc_ctx.h>
#include <axis2_svc_grp.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc_grp_ctx Message Context 
 * @ingroup axis2_core_context
 * @{
 */

struct axis2_svc_grp;    
typedef struct axis2_svc_grp_ctx_ops axis2_svc_grp_ctx_ops_t;
typedef struct axis2_svc_grp_ctx axis2_svc_grp_ctx_t; 

    
/** 
 * @brief Message Context ops struct
 * Encapsulator struct for ops of axis2_svc_grp_ctx
 */  
struct axis2_svc_grp_ctx_ops
{
    axis2_ctx_t* (AXIS2_CALL *get_base)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                                axis2_env_t **env);
    struct axis2_conf_ctx* (AXIS2_CALL *get_parent)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
           axis2_env_t **env);
    axis2_status_t (AXIS2_CALL *free)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                       axis2_env_t **env);
    /**
     * The method is used to do the intialization of the EngineContext
     */
    axis2_status_t (AXIS2_CALL *init)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                                        axis2_env_t **env,
                                        axis2_conf_t *conf);
    axis2_char_t* (AXIS2_CALL *get_id)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                            axis2_env_t **env);
    axis2_status_t (AXIS2_CALL *set_id)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                            axis2_env_t **env, axis2_char_t *id);
    /** if the servic name is foo:bar , you should pass only bar */
    axis2_svc_ctx_t* (AXIS2_CALL *get_svc_ctx)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                            axis2_env_t **env,
                            axis2_char_t *svc_name);
    /**
     * This will create one service context per each service in descrpition
     * if service group has 2 service init , then two service contexts will be
     * created
     */
    axis2_status_t (AXIS2_CALL *fill_svc_ctx_map)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                            axis2_env_t **env);
    struct axis2_svc_grp* (AXIS2_CALL *get_svc_grp)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                            axis2_env_t **env);
    axis2_hash_t* (AXIS2_CALL *get_svc_ctx_map)(struct axis2_svc_grp_ctx *svc_grp_ctx, 
                            axis2_env_t **env);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_svc_grp_ctx
{
    axis2_svc_grp_ctx_ops_t *ops;    
};

AXIS2_DECLARE(axis2_svc_grp_ctx_t*) axis2_svc_grp_ctx_create(axis2_env_t **env, 
    struct axis2_svc_grp *svc_grp, 
    struct axis2_conf_ctx *conf_ctx);

/****************** Start of function macros **************************/

#define AXIS2_SVC_GRP_CTX_GET_BASE(svc_grp_ctx, env) ((svc_grp_ctx)->ops->get_base(svc_grp_ctx, env))
#define AXIS2_SVC_GRP_CTX_GET_PARENT(svc_grp_ctx, env) ((svc_grp_ctx)->ops->get_parent(svc_grp_ctx, env))
#define AXIS2_SVC_GRP_CTX_FREE(svc_grp_ctx, env) ((svc_grp_ctx)->ops->free(svc_grp_ctx, env))
#define AXIS2_SVC_GRP_CTX_INIT(svc_grp_ctx, env, conf) ((svc_grp_ctx)->ops->init(svc_grp_ctx, env, conf))
#define AXIS2_SVC_GRP_CTX_GET_ID(svc_grp_ctx, env) ((svc_grp_ctx)->ops->get_id(svc_grp_ctx, env))
#define AXIS2_SVC_GRP_CTX_SET_ID(svc_grp_ctx, env, id) ((svc_grp_ctx)->ops->set_id(svc_grp_ctx, env, id))
#define AXIS2_SVC_GRP_CTX_GET_SVC_CTX(svc_grp_ctx, env, svc_name) ((svc_grp_ctx)->ops->get_svc_ctx(svc_grp_ctx, env, svc_name))
#define AXIS2_SVC_GRP_CTX_FILL_SVC_CTX_MAP(svc_grp_ctx, env) ((svc_grp_ctx)->ops->fill_svc_ctx_map(svc_grp_ctx, env))
#define AXIS2_SVC_GRP_GET_SVC_GRP(svc_grp_ctx, env) ((svc_grp_ctx)->ops->get_svc_grp(svc_grp_ctx, env))
#define AXIS2_SVC_GRP_GET_SVC_CTX_MAP(svc_grp_ctx, env) ((svc_grp_ctx)->ops->get_svc_ctx_map(svc_grp_ctx, env))
    
/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_SVC_GRP_CTX_H */
