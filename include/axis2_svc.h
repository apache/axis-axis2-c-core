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

#ifndef AXIS2_SVC_H
#define AXIS2_SVC_H

/**
  * @file axis2_svc.h
  * @brief axis2 service interface
  */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_flow_container.h>
#include <axis2_wsdl_svc.h>
#include <axis2_operation.h>
#include <axis2_svc_grp.h>
#include <axis2_qname.h>
#include <axis2_error.h>
#include <axis2_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc Service description
  * @ingroup axis2_core_description
  * @{
  */

struct axis2_svc_grp;
struct axis2_operation;
struct axis2_flow_container;
struct axis2_param_container;
struct axis2_wsdl_svc;
struct axis2_wsdl_interface;    
typedef struct axis2_svc_ops axis2_svc_ops_t;   
typedef struct axis2_svc axis2_svc_t;
    
/** 
 * @brief Service operations struct
 * Encapsulator struct for operations of axis2_svc
 */    
struct axis2_svc_ops
{
	axis2_status_t (AXIS2_CALL *free) (axis2_svc_t *svc, axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *add_operation) (axis2_svc_t *svc, 
                                                axis2_env_t **env,
	 	                                        struct axis2_operation *operation);

	struct axis2_operation *(AXIS2_CALL *get_operation_with_qname) (
                                                  axis2_svc_t *svc, 
                                                  axis2_env_t **env,
	 	                                          axis2_qname_t *operation_qname);

	struct axis2_operation *(AXIS2_CALL *get_operation_with_name) (
                                            axis2_svc_t *svc, 
                                            axis2_env_t **env,
	 	                                    const axis2_char_t * operation_name);

	axis2_hash_t *(AXIS2_CALL *get_operations) (axis2_svc_t *svc, 
                                                    axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *set_parent) (axis2_svc_t *svc, 
                                                axis2_env_t **env,
	 	                                        struct axis2_svc_grp *svc_grp);

	struct axis2_svc_grp *(AXIS2_CALL *get_parent) (axis2_svc_t *svc, 
                                                axis2_env_t **env);
	
    axis2_status_t (AXIS2_CALL *set_name) (axis2_svc_t *svc, 
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);
    
	axis2_qname_t *(AXIS2_CALL *get_name) (const axis2_svc_t *svc, 
                                            axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *add_param) (axis2_svc_t *svc, 
                                                axis2_env_t **env,
		                                        axis2_param_t *param);

	axis2_param_t *(AXIS2_CALL *get_param) (axis2_svc_t *svc, 
                                                axis2_env_t **env,
		                                        const axis2_char_t *name);

	axis2_hash_t *(AXIS2_CALL *get_params) (axis2_svc_t *svc, 
                                                axis2_env_t **env);
	
	axis2_bool_t (AXIS2_CALL *is_param_locked) (axis2_svc_t *svc, 
                                                axis2_env_t **env,
		                                        const axis2_char_t *param_name);
                                                
    axis2_status_t (AXIS2_CALL *
    set_svc_interface) (axis2_svc_t *svc,
                                axis2_env_t **env,
                                struct axis2_wsdl_interface *svc_interface);                                                

};

/** 
 * @brief Service struct
  *	Axis2 Service  
 */
struct axis2_svc
{
	axis2_svc_ops_t *ops; 
    struct axis2_param_container *param_container;
    struct axis2_flow_container *flow_container;
    struct axis2_wsdl_svc *wsdl_svc;    

};

/** 
 * Creates service struct
 * @return pointer to newly created service
 */
AXIS2_DECLARE(axis2_svc_t *)
axis2_svc_create (axis2_env_t **env);

/** 
 * Creates service struct with qname
 * @param qname ualified name
 * @return pointer to newly created service
 */
AXIS2_DECLARE(axis2_svc_t *)
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname);

/** 
 * Creates service struct with wsdl service
 * @param wsdl_svc wsdl service
 * @return pointer to newly created service
 */
axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_wsdl_svc (axis2_env_t **env, 
                                struct axis2_wsdl_svc *wsdl_svc);

/**************************** Start of function macros ************************/

#define AXIS2_SVC_FREE(svc, env) (svc->ops->free (svc, env))

#define AXIS2_SVC_ADD_OPERATION(svc, env, operation) \
		(svc->ops->add_operation (svc, env, operation))

#define AXIS2_SVC_GET_OPERATION_WITH_QNAME(svc, env, operation_qname) \
		(svc->ops->get_operation_with_qname (svc, env, operation_qname))

#define AXIS2_SVC_GET_OPERATION_WITH_NAME(svc, env, operation_name) \
		(svc->ops->get_operation_with_name (svc, env, operation_name))

#define AXIS2_SVC_GET_OPERATIONS(svc, env) (svc->ops->get_operations (svc, env))

#define AXIS2_SVC_SET_PARENT(svc, env , svc_grp) (svc->ops->set_parent \
        (svc, env, svc_grp))

#define AXIS2_SVC_GET_PARENT(svc, env) (svc->ops->get_parent (svc, env))

#define AXIS2_SVC_SET_NAME(svc, env, qname) (svc->ops->set_name(svc , env, qname))

#define AXIS2_SVC_GET_NAME(svc, env) (svc->ops->get_name(svc , env))
		
#define AXIS2_SVC_ADD_PARAM(svc, env, param) (svc->ops->add_param(svc , env, param))
		
#define AXIS2_SVC_GET_PARAM(svc, env, name) (svc->ops->get_param(svc , env, name))
		
#define AXIS2_SVC_GET_PARAMS(svc, env) (svc->ops->get_params(svc , env))
		
#define AXIS2_SVC_IS_PARAM_LOCKED(svc, env, param_name) \
        (svc->ops->is_param_locked(svc, env, param_name))

#define AXIS2_SVC_SET_SVC_INTERFACE(svc, env, svc_interface) \
        (svc->ops->set_svc_interface(svc, env, svc_interface))
        
/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_SVC_H */
