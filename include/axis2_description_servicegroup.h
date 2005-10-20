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
	
/**************************** Function pointers *******************************/

typedef axis2_status_t (*axis2_description_servicegroup_add_service_t)
		(axis2_environment_t *env
		, axis2_description_servicegroup_t* service_group);

/*************************** End of function pointers *************************/

struct axis2_description_servicegroup_ops_s
{
	axis2_description_servicegroup_add_service_t add_service;	
};

axis2_description_servicegroup_ops_t *axis2_description_servicegroup_get_ops
		(axis2_environment_t *env);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_DESCRIPTION_SERVICEGROUP_H	*/
