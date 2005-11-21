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

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc_grp Service group description
  * @ingroup axis2_core_description
  * @{
  */

struct axis2_svc_s;
typedef struct axis2_svc_grp_ops axis2_svc_grp_ops_t;    
typedef struct axis2_svc_grp axis2_svc_grp_t;
    
/** 
 * @brief Service group operations struct
 * Encapsulator struct for operations of axis2_svc_grp
 */   
struct axis2_svc_grp_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *free) (axis2_svc_grp_t *svc_grp, 
                                        axis2_env_t **env);
    
    /** 
     * Set name of the service group
     * @param svc_grp_name service group name
     * @return status_code
     */
    axis2_status_t (AXIS2_CALL *set_name) (axis2_svc_grp_t *svc_grp, 
                                            axis2_env_t **env, 
                                            axis2_char_t *svc_grp_name);
	/** 
     * Get the service group name
     * @return service group name
     */
    axis2_char_t *(AXIS2_CALL *get_name) (axis2_svc_grp_t *svc_grp, 
                                            axis2_env_t **env) ;
    /** 
     * Add a service to the serivce group
     * @param service service to be added
     * @return status code
     */
	axis2_status_t (AXIS2_CALL *add_svc) (axis2_svc_grp_t *svc_grp, 
                                            axis2_env_t **env,
	 	                                    struct axis2_svc_s *svc) ;
    
    /** 
     * Get the service from service group
     * @param svc_name name of the service
     * @return service description
     */
	struct axis2_svc_s *(AXIS2_CALL *get_svc) (axis2_svc_grp_t *svc_grp, 
                                                axis2_env_t **env,
	                                            const axis2_qname_t *svc_name);
        
	axis2_status_t (AXIS2_CALL *remove_svc) (axis2_svc_grp_t *svc_grp, 
                                             axis2_env_t **env,
		                                     const axis2_qname_t *svc_name);
        
	axis2_status_t (AXIS2_CALL *add_param) (axis2_svc_grp_t *svc_grp, 
                                                axis2_env_t **env, 
                                                axis2_param_t *param);
        
	axis2_param_t *(AXIS2_CALL *get_param) (axis2_svc_grp_t *svc_grp, 
                                                axis2_env_t **env, 
                                                const axis2_char_t *name);
        
	axis2_hash_t *(AXIS2_CALL *get_params) (axis2_svc_grp_t *svc_grp, 
                                                axis2_env_t **env);
    
	axis2_bool_t (AXIS2_CALL *is_param_locked) (axis2_svc_grp_t *svc_grp, 
                                                axis2_env_t **env, 
                                                const axis2_char_t *param_name);
};

/** 
 * @brief Service group struct
  *	Axis2 Service group  
 */
struct axis2_svc_grp
{
	axis2_svc_grp_ops_t *ops;   

};

/** 
 * Creates service group struct
 * @return pointer to newly created service group
 */
AXIS2_DECLARE(axis2_svc_grp_t *)
axis2_svc_grp_create (axis2_env_t **env);

/**************************** Start of function macros ************************/

#define AXIS2_SVC_GRP_FREE(svc_grp, env) (svc_grp->ops->free (svc_grp , env));
	
#define AXIS2_SVC_GRP_SET_NAME(svc_grp, env , svc_grp_name) \
        (svc_grp->ops->set_name (svc_grp, env, svc_grp_name));
		
#define AXIS2_SVC_GRP_GET_NAME(svc_grp, env) \
        (svc_grp->ops->get_name(svc_grp, env));

#define AXIS2_SVC_GRP_ADD_SVC(svc_grp, env , svc) \
        (svc_grp->ops->add_svc (svc_grp, env, svc));

#define AXIS2_SVC_GRP_GET_SVC(svc_grp, env , svc) \
        (svc_grp->ops->get_svc(svc_grp, env, svc));
		
#define AXIS2_SVC_GRP_REMOVE_SVC(svc_grp, env, srvc_name) \
		(svc_grp->ops->remove_svc(svc_grp, env, srvc_name));		

#define AXIS2_SVC_GRP_ADD_PARAM(svc_grp, env, param) \
		(svc_grp->ops->add_param(svc_grp, env, param));
		
#define AXIS2_SVC_GRP_GET_PARAM(svc_grp, env, name) \
		(svc_grp->ops->get_param(svc_grp, env, name));
		
#define AXIS2_SVC_GRP_GET_PARAMS(svc_grp, env) \
        (svc_grp->ops->get_params(svc_grp, env));
				
#define AXIS2_SVC_GRP_IS_PARAM_LOCKED(svc_grp, env , param_name) \
        (svc_grp->ops->is_param_locked(svc_grp, env, param_name));
        
/**************************** End of function macros **************************/        

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_SVC_GRP_H  */
