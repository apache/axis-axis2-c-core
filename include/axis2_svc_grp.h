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

#ifndef AXIS2_SVC_GRP_H
#define AXIS2_SVC_GRP_H

/**
 * @file axis2_svc_grp.h
 * @brief axis2 Service Group interface
 */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_svc.h>
#include <axis2_svc_grp_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc_grp Service group description
  * @ingroup axis2_core_description
  * @{
  */

struct axis2_svc;
struct axis2_svc_grp_ctx;    
typedef struct axis2_svc_grp_ops axis2_svc_grp_ops_t;    
typedef struct axis2_svc_grp axis2_svc_grp_t;
    
/** 
 * @brief Service group ops struct
 * Encapsulator struct for ops of axis2_svc_grp
 */   
struct axis2_svc_grp_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (axis2_svc_grp_t *svc_grp, 
           axis2_env_t **env);
    
    /** 
     * Set name of the service group
     * @param svc_grp_name service group name
     * @return status_code
     */
    axis2_status_t (AXIS2_CALL *
    set_svc_grp_name) (axis2_svc_grp_t *svc_grp, 
                axis2_env_t **env, 
                axis2_char_t *svc_grp_name);
	/** 
     * Get the service group name
     * @return service group name
     */
    axis2_char_t *(AXIS2_CALL *
    get_svc_grp_name) (axis2_svc_grp_t *svc_grp, 
               axis2_env_t **env) ;
    /** 
     * Add a service to the serivce group
     * @param service service to be added
     * @return status code
     */
	axis2_status_t (AXIS2_CALL *
    add_svc) (axis2_svc_grp_t *svc_grp, 
                axis2_env_t **env,
                struct axis2_svc *svc) ;
    
    /** 
     * Get the service from service group
     * @param svc_name name of the service
     * @return service description
     */
	struct axis2_svc *(AXIS2_CALL *
    get_svc) (axis2_svc_grp_t *svc_grp, 
                axis2_env_t **env,
                axis2_qname_t *svc_name);
    
    axis2_hash_t *(AXIS2_CALL *
    get_svcs) (axis2_svc_grp_t *svc_grp, 
               axis2_env_t **env);
    
	axis2_status_t (AXIS2_CALL *
    remove_svc) (axis2_svc_grp_t *svc_grp, 
                 axis2_env_t **env,
                 axis2_qname_t *svc_name);
        
	axis2_status_t (AXIS2_CALL *
    add_param) (axis2_svc_grp_t *svc_grp, 
                axis2_env_t **env, 
                axis2_param_t *param);
        
	axis2_param_t *(AXIS2_CALL *
    get_param) (axis2_svc_grp_t *svc_grp, 
                axis2_env_t **env, 
                const axis2_char_t *name);
        
	axis2_array_list_t *(AXIS2_CALL *
    get_params) (axis2_svc_grp_t *svc_grp, 
                 axis2_env_t **env);
    
	axis2_bool_t (AXIS2_CALL *
    is_param_locked) (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env, 
                        axis2_char_t *param_name);
                        
    axis2_status_t (AXIS2_CALL *
    add_module) (axis2_svc_grp_t *svc_grp, 
                    axis2_env_t **env,
                    axis2_qname_t *module_qname);

    struct axis2_conf * (AXIS2_CALL *
    get_parent) (axis2_svc_grp_t *svc_grp,
                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_parent) (axis2_svc_grp_t *svc_grp,
                    axis2_env_t **env,
                    struct axis2_conf *parent);
                        
    axis2_status_t (AXIS2_CALL *
    engage_module_to_grp) (axis2_svc_grp_t *svc_grp,
                                        axis2_env_t **env,
                                        axis2_qname_t *module_name);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_svc_grp_modules) (axis2_svc_grp_t *svc_grp,
                                        axis2_env_t **env);
    
    struct axis2_conf *(AXIS2_CALL *
    get_axis_desc) (axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_axis_desc) (axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env,
                                struct axis2_conf *axis2_desc);
    
    axis2_status_t (AXIS2_CALL *
    add_moduleref) (axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env,
                                axis2_qname_t *moduleref);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_modules) (axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env); 
                                
    struct axis2_svc_grp_ctx *(AXIS2_CALL *
    get_svc_grp_ctx) (axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env,
                                struct axis2_conf_ctx *parent);                                
                                
};

/** 
 * @brief Service group struct
  *	Axis2 Service group  
 */
struct axis2_svc_grp
{
	axis2_svc_grp_ops_t *ops; 
    axis2_param_container_t *param_container;    

};

/** 
 * Creates service group struct
 * @return pointer to newly created service group
 */
AXIS2_DECLARE(axis2_svc_grp_t *)
axis2_svc_grp_create (axis2_env_t **env);

/** 
 * Creates service group struct
 * @param conf
 * @return pointer to newly created service group
 */
axis2_svc_grp_t * AXIS2_CALL 
axis2_svc_grp_create_with_conf (axis2_env_t **env,
                                         struct axis2_conf *conf);

/**************************** Start of function macros ************************/

#define AXIS2_SVC_GRP_FREE(svc_grp, env) (svc_grp->ops->free (svc_grp , env))
	
#define AXIS2_SVC_GRP_SET_NAME(svc_grp, env , svc_grp_name) \
        (svc_grp->ops->set_svc_grp_name (svc_grp, env, svc_grp_name))
		
#define AXIS2_SVC_GRP_GET_NAME(svc_grp, env) \
        (svc_grp->ops->get_svc_grp_name(svc_grp, env))

#define AXIS2_SVC_GRP_ADD_SVC(svc_grp, env , svc) \
        (svc_grp->ops->add_svc (svc_grp, env, svc))

#define AXIS2_SVC_GRP_GET_SVC(svc_grp, env , svc) \
        (svc_grp->ops->get_svc(svc_grp, env, svc))

#define AXIS2_SVC_GRP_GET_SVCS(svc_grp, env) \
        (svc_grp->ops->get_svcs(svc_grp, env))
        
#define AXIS2_SVC_GRP_REMOVE_SVC(svc_grp, env, srvc_name) \
		(svc_grp->ops->remove_svc(svc_grp, env, srvc_name))		

#define AXIS2_SVC_GRP_ADD_PARAM(svc_grp, env, param) \
		(svc_grp->ops->add_param(svc_grp, env, param))
		
#define AXIS2_SVC_GRP_GET_PARAM(svc_grp, env, name) \
		(svc_grp->ops->get_param(svc_grp, env, name))
		
#define AXIS2_SVC_GRP_GET_PARAMS(svc_grp, env) \
        (svc_grp->ops->get_params(svc_grp, env))
				
#define AXIS2_SVC_GRP_IS_PARAM_LOCKED(svc_grp, env , param_name) \
        (svc_grp->ops->is_param_locked(svc_grp, env, param_name))

#define AXIS2_SVC_GRP_ADD_MODULE(svc_grp, env , module_qname) \
        (svc_grp->ops->add_module(svc_grp, env, module_qname))

#define AXIS2_SVC_GRP_GET_PARENT(svc_grp, env) \
        (svc_grp->ops->get_parent(svc_grp, env))
        
#define AXIS2_SVC_GRP_SET_PARENT(svc_grp, env , parent) \
        (svc_grp->ops->set_parent(svc_grp, env, parent)) 

#define AXIS2_SVC_GRP_ENGAGE_MODULE_TO_GRP(svc_grp, env, module_name) \
        (svc_grp->ops->engage_module_to_grp(svc_grp, env, module_name))
        
#define AXIS2_SVC_GRP_GET_SVC_GRP_MODULES(svc_grp, env) \
        (svc_grp->ops->get_svc_grp_modules(svc_grp, env))

#define AXIS2_SVC_GRP_GET_AXIS_DESC(svc_grp, env) \
        (svc_grp->ops->get_axis_desc(svc_grp, env))
        
#define AXIS2_SVC_GRP_SET_AXIS_DESC(svc_grp, env, axis2_desc) \
        (svc_grp->ops->set_axis_desc(svc_grp, env, axis2_desc))

#define AXIS2_SVC_GRP_ADD_MODULEREF(svc_grp, env, moduleref) \
        (svc_grp->ops->add_moduleref(svc_grp, env, moduleref))
        
#define AXIS2_SVC_GRP_GET_MODULES(svc_grp, env) \
        (svc_grp->ops->get_modules(svc_grp, env)) 
 
#define AXIS2_SVC_GRP_GET_SVC_GRP_CTX(svc_grp, env) \
        (svc_grp->ops->get_svc_grp_ctx(svc_grp, env))  
        
        
                            
/**************************** End of function macros **************************/        

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_SVC_GRP_H  */
