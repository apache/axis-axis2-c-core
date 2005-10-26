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

#ifndef AXIS2_DESCRIPTION_SERVICEGROUP_H
#define AXIS2_DESCRIPTION_SERVICEGROUP_H

/**
 * @file axis2_description_servicegroup.h
 * @brief axis2 DESCRIPTION CORE servicegroup
 */

#include <axis2_core.h>
#include <axis2_description_param_include.h>
#include <axis2_description_service.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_description DESCRIPTION (Axis2 Information model)
  * @ingroup axis2
  * @{
  */

/** @} */

/**
 * @defgroup axis2_description_servicegroup DESCRIPTION Servicegroup
 * @ingroup axis2_description 
 * @{
 */

/**************************** Start of function macros ************************/

#define axis2_description_servicegroup_free(env, servicegroup_desc) \
		(axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->free (env, servicegroup_desc));
	
#define axis2_description_servicegroup_set_name(env, servicegroup_desc \
		, servicegroup_name) (axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->set_name (env, servicegroup_desc, servicegroup_name));
		
#define axis2_description_servicegroup_get_name(env, servicegroup_desc) \
		(axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->get_name(env, servicegroup_desc));

#define axis2_description_servicegroup_add_service(env, servicegroup_desc \
		, service_desc) (axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->add_service (env, servicegroup_desc, service_desc));

#define axis2_description_servicegroup_get_service(env, servicegroup_desc \
		, service_desc) (axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->get_service(env, servicegroup_desc, service_desc));
		
#define axis2_description_servicegroup_remove_service(env, servicegroup_desc \
		, service_name) (axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->remove_service(env, servicegroup_desc, service_name));		

#define axis2_description_servicegroup_add_param(env, servicegroup_desc, param) \
		(axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->add_param(env, servicegroup_desc, param));
		
		

#define axis2_description_servicegroup_get_param(env, servicegroup_desc, name) \
		(axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->get_param(env, servicegroup_desc, name));
		
#define axis2_description_servicegroup_get_params(env, servicegroup_desc) \
		(axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->get_params(env, servicegroup_desc));
		
		
#define axis2_description_servicegroup_is_param_locked(env, servicegroup_desc \
		, param_name)(axis2_description_servicegroup_get_ops(env, \
		servicegroup_desc)->is_param_locked(env, servicegroup_desc, param_name));
		
/**************************** End of function macros **************************/
/**************************** Function pointers *******************************/

/** Deallocate memory
  * @return status code
  */
typedef axis2_status_t (*axis2_description_servicegroup_free_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc);

/** Set name of the service group_member
  * @param servicegroup_name
  * @return status_code
  */
typedef axis2_status_t (*axis2_description_servicegroup_set_name_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, axis2_char_t *servicegroup_name);
		
/** get the service group name
  * @return service group name
  */
typedef axis2_char_t *(*axis2_description_servicegroup_get_name_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc);

/** Add a service to the serivce group
  * @param service to be added
  * @return status code
  */
typedef axis2_status_t (*axis2_description_servicegroup_add_service_t)
		(axis2_environment_t * env,
	 	axis2_description_servicegroup_t * servicegroup_desc,
	 	axis2_description_service_t * service_desc);

/** Get the service from service group_member
  * @param service_name
  * @return service description
  */
typedef axis2_description_service_t 
		*(*axis2_description_servicegroup_get_service_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_qname_t* service_name);

typedef axis2_status_t (*axis2_description_servicegroup_remove_service_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_qname_t* service_name);
		
typedef axis2_status_t (*axis2_description_servicegroup_add_param_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, axis2_description_param_t *param);
		

typedef axis2_description_param_t *(*axis2_description_servicegroup_get_param_t)(
		axis2_environment_t *env, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_char_t *name);
		
typedef axis2_hash_t *(*axis2_description_servicegroup_get_params_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc);
		
typedef axis2_bool_t (*axis2_description_servicegroup_is_param_locked_t)
		(axis2_environment_t *env, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_char_t *param_name);
		
/*************************** End of function pointers *************************/

struct axis2_description_servicegroup_ops_s
{
	axis2_description_servicegroup_free_t free;
	axis2_description_servicegroup_set_name_t set_name;
	axis2_description_servicegroup_get_name_t get_name;
	axis2_description_servicegroup_add_service_t add_service;
	axis2_description_servicegroup_get_service_t get_service;
	axis2_description_servicegroup_remove_service_t remove_service;
	axis2_description_servicegroup_add_param_t add_param;
	axis2_description_servicegroup_get_param_t get_param;
	axis2_description_servicegroup_get_params_t get_params;
	axis2_description_servicegroup_is_param_locked_t is_param_locked;
};

axis2_description_servicegroup_ops_t *axis2_description_servicegroup_get_ops 
		(axis2_environment_t *env
		, axis2_description_servicegroup_t *servicegroup_desc);

axis2_description_servicegroup_t *axis2_description_servicegroup_create
		(axis2_environment_t *env);

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_DESCRIPTION_SERVICEGROUP_H  */
