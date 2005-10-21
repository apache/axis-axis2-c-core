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
 * @file axis2_description_param_include.h
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
#include <axis2_description_parameter.h>

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

/*************************** Function macros **********************************/

#define axis2_description_param_include_free(env, param_include) \
	(axis2_description_param_include_get_ops(env, param_include)->free (env, \
	param_include))

#define axis2_description_param_include_add_param(env, param_include, \
	param) (axis2_description_param_include_get_ops(env, \
	param_include)->add_param (env, param_include, param))

#define axis2_description_param_include_get_param(env, param_include) \
	(axis2_description_param_include_get_ops(env, \
	param_include)->get_param (env, param_include))

#define axis2_description_param_include_get_params(env, param_include) \
	(axis2_description_param_include_get_ops(env, \
	param_include)->get_params (env, param_include))

#define axis2_description_param_include_is_param_locked(env, \
	param_include) (axis2_description_param_include_get_ops(env, \
	param_include)->is_param_locked (env, param_include))

/*************************** End of function macros ***************************/

/***************************** Function pointers ******************************/

/** Deallocate memory
  * @return status code
  */
    typedef axis2_status_t (*axis2_description_param_include_free_t)
        (axis2_environment_t * env,
         axis2_description_param_include_t * param_include);

/** Add a parameter
  * @param parameters
  * @return status code
  */
    typedef axis2_status_t (*axis2_description_param_include_add_param_t)
        (axis2_environment_t * env,
         axis2_description_param_include_t * param_include,
         const axis2_description_param_t * param);

/** To get a parameter in a given description 
  * @param parameter name
  * @return parameter
  */
    typedef axis2_description_param_t
        *(*axis2_description_param_include_get_param_t) (axis2_environment_t *
                                                         env,
                                                         axis2_description_param_include_t
                                                         * param_include,
                                                         const axis2_char_t *
                                                         name);

/** To get all the parameters in a given description
  * @return all the parameters contained
  */
    typedef axis2_hash_t *(*axis2_description_param_include_get_params_t)
        (axis2_environment_t * env,
         axis2_description_param_include_t * param_include);

/** To check whether the paramter is locked at any level
  * @param parameter name
  * @return whether parameter is locked
  */
    typedef axis2_bool_t (*axis2_description_param_include_is_param_locked_t)
        (axis2_environment_t * env,
         axis2_description_param_include_t * param_include,
         const axis2_char_t * param_name);

/****************************** End of function pointers **********************/

/**
  * Paramter can be any thing it can be XML element with number of child 
  * elements
  */
    struct axis2_description_param_include_ops_s
    {
        axis2_description_param_include_free_t free;

        axis2_description_param_include_add_param_t add_param;

        axis2_description_param_include_get_param_t get_param;

        axis2_description_param_include_get_params_t get_params;

        axis2_description_param_include_is_param_locked_t is_param_locked;

    };

/** This will return the operations struct of the 
  * axis2_description_param_include_t struct
  * @return operations for axis2_description_param_include_t
  */
    axis2_description_param_include_ops_t
        *axis2_description_param_include_get_ops (axis2_environment_t * env,
                                                  axis2_description_param_include_t
                                                  * param_include);

/**
  *	Create axis2_description_param_include_t
  * @return axis2_description_param_include_t
  */
    axis2_description_param_include_t *axis2_description_param_include_create
        (axis2_environment_t * env);

/** @} */

#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_DESCRIPTION_PARAM_INCLUDE_H */
