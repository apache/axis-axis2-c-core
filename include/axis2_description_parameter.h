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
 
#ifndef AXIS2_PARAMETER_H
#define AXIS2_PARAMETER_H

/**
 * @file axis2_parameter.h
 * @brief axis2 DESCRIPTION CORE parameter
 */

#include <axis2_core.h>
/*#include <axis2_om_element.h>*/

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
 * @defgroup axis2_description_parameter DESCRIPTION Parameter
 * @ingroup axis2_description 
 * @{
 */	
 
/************************* Start of function macros ***************************/

#define axis2_description_param_free(env, param) \
	(axis2_description_param_get_ops(env, param)->free (env, param))

#define axis2_description_param_set_name(env, param, name) \
	(axis2_description_param_get_ops(env, param)->set_name (env, param \
	, name))

#define axis2_description_param_get_name(env, param) \
	(axis2_description_param_get_ops(env, param)->get_name (env, param))
	
#define axis2_description_param_set_value(env, param, value) \
	(axis2_description_param_get_ops(env, param)->set_value (env \
	, param, value))

#define axis2_description_param_get_value(env, param) \
	(axis2_description_param_get_ops(env, param)->get_value (env, param))
	
#define axis2_description_param_is_locked(env, param) \
	(axis2_description_param_get_ops(env, param)->is_locked (env, param))
	
#define axis2_description_param_set_locked(env, param, locked) \
	(axis2_description_param_get_ops(env, param)->set_locked (env \
	, param, locked))
	
/*********************** End of function macros *******************************/

/***********************Start of function pointers ****************************/

/**
  * memeory deallocation operation
  * @return Status code
  */
typedef axis2_status_t (*axis2_description_param_free_t) 
	(axis2_environment_t *env, axis2_description_param_t * param);

/**
  * parameter name set operation
  * @param parameter name
  * @return Status code
  */
typedef axis2_status_t (*axis2_description_param_set_name_t) 
	(axis2_environment_t *env, axis2_description_param_t * param
	, const axis2_char_t *name);

/**
  * parameter name get operation
  * @return parameter name
  */
typedef axis2_char_t *(*axis2_description_param_get_name_t) 
	(axis2_environment_t *env, axis2_description_param_t * param);

/**
  * parameter value set operation
  * @param parameter value
  * @return Status code
  */
typedef axis2_status_t (*axis2_description_param_set_value_t) 
	(axis2_environment_t *env, axis2_description_param_t * param
	, void *value);

/**
  * parameter value get operation
  * @return parameter value
  */
typedef void *(*axis2_description_param_get_value_t) 
	(axis2_environment_t *env, axis2_description_param_t * param);

/**
  * parameter lock check operation
  * @return whether parameter is locked or not
  */
typedef axis2_bool_t (*axis2_description_param_is_locked_t) 
	(axis2_environment_t *env, axis2_description_param_t * param);

/**
  * parameter lock set operation
  * @param lock boolean value 
  * @return Status code
  */
typedef axis2_status_t (*axis2_description_param_set_locked_t) 
	(axis2_environment_t *env, axis2_description_param_t * param
	, const axis2_bool_t locked);

/**
  * Paramter can be any thing, it can be XML element with number of child 
  * elements , so if some one wants to access the XML elemet we need to store
  *	that , at the deployment time. To store the XMLelment of the parameter one 
  * can use this method. It will store whole
  * <parameter name="ServiceClass1" locked="false">org.apache.axis2.sample
  * .echo.EchoImpl</parameter> element
  * @param parameter element
  * @return Status code
  */
/*typedef axis2_status_t (*axis2_description_param_set_param_element_t) 
	(axis2_environment_t *env, axis2_description_param_t *param
	, axis2_om_element_t * om_element);
*/										 

/**
  * param element get operation
  * @return parameter element
  */
typedef axis2_description_param_t 
	*(*axis2_description_param_get_param_element_t) (axis2_environment_t *env
	, axis2_description_param_t *param);

/**
  * parameter type set operation
  * @param parameter type
  * @return Status code
  */
typedef axis2_status_t (*axis2_description_param_set_type_t) 
	(axis2_environment_t *env, axis2_description_param_t * param
	, const int type);

/**
  * parameter type get operation
  * @return parameter type
  */
typedef int (*axis2_description_param_get_type_t) 
	(axis2_environment_t *env, axis2_description_param_t *param);

/************************ End of function pointers ****************************/

typedef enum axis2_parameter_types {
    AXIS2_PARAMETER_INVALID = -1,
    AXIS2_PARAMETER_DOM = 10,
    AXIS2_PARAMETER_TEXT = 20,
    
} axis2_parameter_types_t;

/** @struct axis2_description_parameter_ops
    @brief DESCRIPTION parameter operations struct

    Encapsulator struct for operations of axis2_description_param_t
*/
struct axis2_description_param_ops_s
{
	axis2_description_param_free_t free;
	axis2_description_param_set_name_t set_name;
	axis2_description_param_get_name_t get_name;
	axis2_description_param_set_value_t set_value;
	axis2_description_param_get_value_t get_value;
	axis2_description_param_is_locked_t is_locked;
	axis2_description_param_set_locked_t set_locked;
	/*axis2_description_param_set_param_element_t set_param_element;*/
	axis2_description_param_get_param_element_t get_param_element;
	axis2_description_param_set_type_t set_type;
	axis2_description_param_get_type_t get_type;
};

/** This will return the operations struct of the 
  * axis2_description_param_t struct
  * @return operations for axis2_description_param_t
  */
axis2_description_param_ops_t *axis2_description_param_get_ops
	(axis2_environment_t *env, axis2_description_param_t *param);

/**
  *	Create axis2_description_param_t
  * @return axis2_description_param_t
  */
axis2_description_param_t *axis2_description_param_create 
	(axis2_environment_t *env);

/**
 * Create parameter with name and value
 * @param parameter name
 * @param parameter value
 * @return axis2_description_param_t
 */
axis2_description_param_t *axis2_description_param_create_with_name_value 
	(axis2_environment_t *env, const axis2_char_t *name
	, const axis2_char_t *value);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_PARAMETER_H */
