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

#ifndef AXIS2_PARAM_CONTAINER_H
#define AXIS2_PARAM_CONTAINER_H

/**
 * @file axis2_param_container.h
 * @brief Axis2 Param container interface
 */

#include <axis2.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_hash.h>

/*#include <axis2_om_element.h>*/
#include <axis2_qname.h>
#include <axis2_param.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_param_container;
struct axis2_param_container_ops;
 
/**
 * each type which is passed as a param value to a parameter, must have this 
 * type of function implemented. When the param value is set this function  
 * should also be assigned to param free function
 */
typedef axis2_status_t (*AXIS2_PARAM_VALUE_FREE) (void *param, axis2_env_t **env);    

/** @defgroup axis2_param_container Parameter Container
 * @ingroup axis2_description
 * @{
 */

/** 
 * @brief Param Container ops struct
 * Encapsulator struct for ops of axis2_param_container
 */
AXIS2_DECLARE_DATA typedef struct axis2_param_container_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(struct axis2_param_container *param_container,
										axis2_env_t **env);

	/** Add a param
     * @param param param to be added
     * @return status code
     */
	axis2_status_t (AXIS2_CALL *add_param)
									(struct axis2_param_container *param_container,
										axis2_env_t **env,
		 								axis2_param_t *param);

	/** To get a param in a given description 
     * @param name param name
     * @return param
     */
	axis2_param_t *(AXIS2_CALL *get_param) 
									(struct axis2_param_container *param_container, 
										axis2_env_t **env,
										const axis2_char_t *name);


	/** To get all the params in a given description
	 * @return all the params contained
	 */
	axis2_array_list_t *(AXIS2_CALL *get_params)
									(struct axis2_param_container *param_container, 
										axis2_env_t **env);
	
	/** To check whether the paramter is locked at any level
	 * @param param_name name of the param
	 * @return whether param is locked
	 */
	axis2_bool_t (AXIS2_CALL *is_param_locked)
									(struct axis2_param_container *param_container, 
										axis2_env_t **env,
										const axis2_char_t *param_name) ;

}axis2_param_container_ops_t;

/** 
 * @brief Param container struct
 *	Container for params  
 */  
AXIS2_DECLARE_DATA typedef struct axis2_param_container
{
	axis2_param_container_ops_t *ops;
}axis2_param_container_t;

/**
 * Creates param container struct
 * @return pointer to newly created param container
 */
AXIS2_DECLARE(axis2_param_container_t *) axis2_param_container_create (axis2_env_t **env);

/*************************** Function macros **********************************/

#define AXIS2_PARAM_CONTAINER_FREE(param_container, env) \
		((param_container)->ops->free (param_container, env))

#define AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container, env, param) \
		((param_container)->ops->add_param (param_container, env, param))

#define AXIS2_PARAM_CONTAINER_GET_PARAM(param_container, env, name) \
		((param_container)->ops->get_param (param_container, env, name))

#define AXIS2_PARAM_CONTAINER_GET_PARAMS(param_container, env) \
		((param_container)->ops->get_params (param_container, env))

#define AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(param_container, env, param_name) \
		((param_container)->ops->is_param_locked (param_container, env, \
		param_name))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_PARAM_CONTAINER_H */
