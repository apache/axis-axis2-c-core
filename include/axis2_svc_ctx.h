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

#ifndef AXIS2_SVC_CTX_H
#define AXIS2_SVC_CTX_H


/**
  * @file axis2_svc_ctx.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_op_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc_ctx Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_svc_ctx_ops axis2_svc_ctx_ops_t;
typedef struct axis2_svc_ctx axis2_svc_ctx_t; 

    
/** 
 * @brief Message Context ops struct
 * Encapsulator struct for ops of axis2_svc_ctx
 */  
struct axis2_svc_ctx_ops
{
    axis2_ctx_t* (AXIS2_CALL *
	get_base)(struct axis2_svc_ctx *svc_ctx,
			  const axis2_env_t *env);
	
	
    struct axis2_svc_grp_ctx* (AXIS2_CALL *
	get_parent)(struct axis2_svc_ctx *svc_ctx,
				const axis2_env_t *env);
	
	
    axis2_status_t (AXIS2_CALL *
	free)(struct axis2_svc_ctx *svc_ctx,
		  const axis2_env_t *env);
	
	
    /**
     * The method is used to do the intialization of the EngineContext
     */
    axis2_status_t (AXIS2_CALL *
	init)(struct axis2_svc_ctx *svc_ctx, 
          const axis2_env_t *env,
	      struct axis2_conf *conf);
			  
		  
    /**
     * @return Returns the svc_id.
     */
    axis2_char_t* (AXIS2_CALL *
	get_svc_id)(struct axis2_svc_ctx *svc_ctx,
		        const axis2_env_t *env);
	
	
    struct axis2_svc* (AXIS2_CALL *
	get_svc)(struct axis2_svc_ctx *svc_ctx,
             const axis2_env_t *env);
	
    axis2_status_t (AXIS2_CALL *
    set_svc) (axis2_svc_ctx_t *svc_ctx,
              const axis2_env_t *env,
              struct axis2_svc *svc);
				  

    struct axis2_conf_ctx* (AXIS2_CALL *
	get_conf_ctx)(struct axis2_svc_ctx *svc_ctx, 
                  const axis2_env_t *env);
	
	
    struct axis2_op_ctx* (AXIS2_CALL *
	create_op_ctx)(struct axis2_svc_ctx *svc_ctx, 
                   const axis2_env_t *env, 
				   axis2_qname_t *qname);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_svc_ctx
{
    axis2_svc_ctx_ops_t *ops;    
};

AXIS2_EXTERN axis2_svc_ctx_t* AXIS2_CALL axis2_svc_ctx_create(const axis2_env_t *env, 
    struct axis2_svc *svc,
    struct axis2_svc_grp_ctx *svc_grp_ctx);
    
/************************** Start of function macros **************************/

#define AXIS2_SVC_CTX_GET_BASE(svc_ctx, env) \
		((svc_ctx)->ops->get_base(svc_ctx, env))
	
#define AXIS2_SVC_CTX_GET_PARENT(svc_ctx, env) \
		((svc_ctx)->ops->get_parent(svc_ctx, env))
	
#define AXIS2_SVC_CTX_FREE(svc_ctx, env) \
		((svc_ctx)->ops->free(svc_ctx, env))
	
#define AXIS2_SVC_CTX_INIT(svc_ctx, env, conf) \
		((svc_ctx)->ops->init(svc_ctx, env, conf))
	
#define AXIS2_SVC_CTX_GET_SVC_ID(svc_ctx, env) \
		((svc_ctx)->ops->get_svc_id(svc_ctx, env))
	
#define AXIS2_SVC_CTX_GET_SVC(svc_ctx, env) \
		((svc_ctx)->ops->get_svc(svc_ctx, env))
	
#define AXIS2_SVC_CTX_SET_SVC(svc_ctx, env, svc) \
        ((svc_ctx)->ops->set_svc(svc_ctx, env, svc))
		
#define AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env) \
		((svc_ctx)->ops->get_conf_ctx(svc_ctx, env))
	
#define AXIS2_SVC_CTX_CREATE_OP_CTX(svc_ctx, env, qname) \
		((svc_ctx)->ops->create_op_ctx(svc_ctx, env, qname))
    
/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_SVC_CTX_H */
