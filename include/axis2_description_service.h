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
 
#ifndef AXIS2_DESCRIPTION_SERVICE_H
#define AXIS2_DESCRIPTION_SERVICE_H

/**
  * @file axis2_description_service.h
  * @brief axis2 DESCRIPTION CORE service
  */
 
#include <axis2_core.h>
#include <axis2_description_param_include.h>
#include <axis2_description_operation.h>
#include <axis2_description_servicegroup.h>
#include <axis2_qname.h>

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
 * @defgroup axis2_description_service DESCRIPTION Service
 * @ingroup axis2_description 
 * @{
 */

/**************************** Start of function macros ************************/

/**************************** End of function macros **************************/	
/**************************** Function pointers *******************************/
	

typedef axis2_status_t (*axis2_description_service_free_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

typedef axis2_status_t (*axis2_description_service_add_param_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_param_t *param);

typedef axis2_description_param_t *(*axis2_description_service_get_param_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t *name);

typedef axis2_hash_t *(*axis2_description_service_get_params_t) 
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

typedef axis2_status_t (*axis2_description_service_add_operation_t) 
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_operation_t *operation_desc);
		
typedef axis2_description_operation_t *(*axis2_description_service_get_operation_with_qname_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_qname_t *operation_name);
		
typedef axis2_description_operation_t 
		*(*axis2_description_service_get_operation_with_name_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t* operation_name);

typedef axis2_hash_t *(*axis2_description_service_get_operations_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc);
	
typedef axis2_status_t (*axis2_description_service_set_parent_t) 
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		,axis2_description_servicegroup_t *servicegroup_desc);

typedef axis2_description_servicegroup_t 
		*(*axis2_description_service_get_parent_t)
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

/************************ End function pointers *******************************/

/** @struct axis2_description_service_ops_s
  * @brief operations for the axis2_description_service_t
  * This contain all the operations for axis2_description_service struct
  */
struct axis2_description_service_ops_s
{
	axis2_description_service_free_t free;
	
	axis2_description_service_add_operation_t add_operation;

	axis2_description_service_add_param_t add_param;

	axis2_description_service_get_param_t get_param;

	axis2_description_service_get_params_t get_params;

	axis2_description_service_get_operation_with_qname_t 
		get_operation_with_qname;

	axis2_description_service_get_operation_with_name_t get_operation_with_name;

	axis2_description_service_get_operations_t get_operations;
	
	axis2_description_service_set_parent_t set_parent;

	axis2_description_service_get_parent_t get_parent;

};

/** To get the operation struct for axis_description_service_t call this
  * function
  * @return operation struct for service
  */
axis2_description_service_ops_t * axis2_description_service_get_ops
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

/** Create a service with qualified name
  * @param qualified name
  * @return service struct
  */
axis2_description_service_t *axis2_description_service_create_with_qname
		(axis2_environment_t *env, axis2_qname_t *qname);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_DESCRIPTION_SERVICE_H */
