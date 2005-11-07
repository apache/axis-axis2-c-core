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
#include <axis2_operation.h>
#include <axis2_svc_grp.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc Service description
  * @ingroup axis2_core_description
  * @{
  */

/** 
 * @brief Service operations struct
 * Encapsulator struct for operations of axis2_svc
 */    
struct axis2_svc_ops_s
{
	axis2_status_t (AXIS2_CALL *free) (axis2_svc_t *srv_desc, axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *add_operation) (axis2_svc_t *srv_desc, 
                                                axis2_env_t **env,
	 	                                        axis2_operation_t *operation);

	axis2_operation_t *(AXIS2_CALL *get_operation_with_qname) (
                                                  axis2_svc_t *srv_desc, 
                                                  axis2_env_t **env,
	 	                                          axis2_qname_t *operation_name);

	axis2_operation_t *(AXIS2_CALL *get_operation_with_name) (
                                            axis2_svc_t *srv_desc, 
                                            axis2_env_t **env,
	 	                                    const axis2_char_t * operation_name);

	axis2_hash_t *(AXIS2_CALL *get_operations) (axis2_svc_t *srv_desc, 
                                                    axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *set_parent) (axis2_svc_t *srv_desc, 
                                                axis2_env_t **env,
	 	                                        axis2_svc_grp_t *svc_grp);

	axis2_svc_grp_t *(AXIS2_CALL *get_parent) (axis2_svc_t *srv_desc, 
                                                axis2_env_t **env);
	
	axis2_qname_t *(AXIS2_CALL *get_name) (const axis2_svc_t *srv_desc, 
                                            axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *add_param) (axis2_svc_t *srv_desc, 
                                                axis2_env_t **env,
		                                        axis2_param_t *param);

	axis2_param_t *(AXIS2_CALL *get_param) (axis2_svc_t *srv_desc, 
                                                axis2_env_t **env,
		                                        const axis2_char_t *name);

	axis2_hash_t *(AXIS2_CALL *get_params) (axis2_svc_t *srv_desc, 
                                                axis2_env_t **env);
	
	axis2_bool_t (AXIS2_CALL *is_param_locked) (axis2_svc_t *srv_desc, 
                                                axis2_env_t **env,
		                                        const axis2_char_t *param_name);
};

/** 
 * @brief Service struct
 *	Axis2 Service  
 */
struct axis2_svc_s
{
	axis2_svc_ops_t *ops;   

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


/**************************** Start of function macros ************************/

#define axis2_svc_free(srv_desc, env) \
		(axis2_svc_get_ops(srv_desc, env)->free \
		(srv_desc, env));

#define axis2_svc_add_operation(srv_desc, env, operation_desc) \
		(axis2_svc_get_ops(srv_desc, env)->add_operation \
		(srv_desc, env, operation_desc));

#define axis2_svc_get_operation_with_qname(srv_desc, env) \
		(axis2_svc_get_ops(srv_desc, env)->get_operation_with_qname \
		(srv_desc, env));

#define axis2_svc_get_operation_with_name(srv_desc, env) \
		(axis2_svc_get_ops(srv_desc, env)->get_operation_with_name \
		(srv_desc, env));

#define axis2_svc_get_operations(srv_desc, env) \
		(axis2_svc_get_ops(srv_desc, env)->get_operations \
		(srv_desc, env));

#define axis2_svc_set_parent(srv_desc, env \
		, svc_grp) (axis2_svc_get_ops(srv_desc \
		, env)->set_parent (srv_desc, env, svc_grp));

#define axis2_svc_get_parent(srv_desc, env) \
		(axis2_svc_get_ops(srv_desc, env)->get_parent \
		(srv_desc, env));
		
#define axis2_svc_get_name(srv_desc, env) \
		(axis2_svc_get_ops(srv_desc, env)->get_name(srv_desc \
		, env));
		
#define axis2_svc_add_param(srv_desc, env, param) \
		(axis2_svc_get_ops(srv_desc, env)->add_param(srv_desc \
		, env, param));
		

#define axis2_svc_get_param(srv_desc, env, name) \
		(axis2_svc_get_ops(srv_desc, env)->get_param(srv_desc \
		, env, name));
		

#define axis2_svc_get_params(srv_desc, env) \
		(axis2_svc_get_ops(srv_desc, env)->get_params(srv_desc \
		, env));
		
		

#define axis2_svc_is_param_locked(srv_desc, env, \
		param_name) (axis2_svc_get_ops(env \
		, srv_desc)->is_parameter_locked(srv_desc, env, param_name));
				

/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_SVC_H */
