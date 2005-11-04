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

#ifndef AXIS2_DESCRIPTION_PARAM_INCLUDE_H
#define AXIS2_DESCRIPTION_PARAM_INCLUDE_H

/**
 * @file axis2_parameter_container.h
 * @brief Parameter handling
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_environment.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_hash.h>

/*#include <axis2_om_element.h>*/
#include <axis2_qname.h>
#include <axis2_parameter.h>

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
 * @defgroup axis2_description_parameter_include DESCRIPTION ParameterInclude
 * @ingroup axis2_description 
 * @{
 */


/**
  * Paramter can be any thing it can be XML element with number of child 
  * elements
  */
struct axis2_param_container_ops_s
{
	/** Deallocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(axis2_param_container_t *param_container,
										axis2_env_t **env);

	/** Add a parameter
     * @param parameters
     * @return status code
     */
	axis2_status_t (AXIS2_CALL *add_param)
									(axis2_param_container_t *param_container,
										axis2_env_t **env,
		 								axis2_param_t *param);

	/** To get a parameter in a given description 
     * @param parameter name
     * @return parameter
     */
	axis2_param_t *(AXIS2_CALL *get_param) 
									(axis2_param_container_t *param_container, 
										axis2_env_t **env,
										const axis2_char_t *name);


	/** To get all the parameters in a given description
	 * @return all the parameters contained
	 */
	axis2_hash_t *(AXIS2_CALL *get_params)
									(axis2_param_container_t *param_container, 
										axis2_env_t **env);
	
	/** To check whether the paramter is locked at any level
	 * @param parameter name
	 * @return whether parameter is locked
	 */
	axis2_bool_t (AXIS2_CALL *is_param_locked)
									(axis2_param_container_t *param_container, 
										axis2_env_t **env,
										const axis2_char_t *param_name) ;

};

/** @struct axis2_description_param_include
  * @brief DESCRIPTION param_include struct
  *	Holder for parameters
  *  
*/  
struct axis2_param_container_s
{
	axis2_param_container_ops_t *ops;
};

/**
  *	Create axis2_param_container_t
  * @return axis2_param_container_t
  */
axis2_param_container_t 
*axis2_param_container_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_PARAMETER_CONTAINER_FREE(param_container, env) \
		((param_container->ops)->free (env, param_container))

#define AXIS2_PARAMETER_CONTAINER_ADD_PARAMETER(param_container, env, param) \
		((param_container->ops)->add_param (param_container, env, param))

#define AXIS2_PARAMETER_CONTAINER_GET_PARAMETER(param_container, env) \
		((param_container->ops)->get_param (param_container, env))

#define AXIS2_PARAMETER_CONTAINER_GET_PARAMETERS(param_container, env) \
		((param_container->ops)->get_params (param_container, env))

#define AXIS2_PARAMETER_CONTAINER_IS_PARAMETER_LOCKED(param_container, env, \
		param_name) \
		((param_container->ops)->is_param_locked (param_container, env, \
		param_name))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_DESCRIPTION_PARAM_INCLUDE_H */
