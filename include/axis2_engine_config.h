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
 
#ifndef AXIS2_ENGINE_CONFIG_H
#define AXIS2_ENGINE_CONFIG_H

/**
  * @file axis2_engine_config.h
  * @brief Global states of all the Axis can
  *        can be accessed here
  */

#include <axis2_core.h>
#include <axis2_description_param_include.h>
#include <axis2_description_servicegroup.h>

#ifdef __cplusplus
extern "C" 
{
#endif
/** @defgroup axis2_context CONTEXT (Axis2 Information model)
  * @ingroup axis2
  * @{
  */

/** @} */ 

/**
 * @defgroup axis2_engine_ ENGINE engine_config
 * @ingroup axis2_engine_config 
 * @{
 */

/************************* Start of function macros	***************************/
	
#define axis2_engine_config_free(env, engine_config) (axis2_engine_get_ops(env, \
		engine_config)->free(env, engine_config);
	
#define axis2_engine_config_add_service_group(env, engine_config, \
		service_group_desc) (axis2_engine_config_get_ops(env, \
		engine_config)->add_service_group (env, engine_config, service_group_desc));
		
#define axis2_engine_config_get_service(env, engine_config, service_name) \
		(axis2_engine_config_get_ops(env, engine_config)->get_service(env, \
		engine_config, service_name));

#define axis2_engine_config_remove_service(env, engine_config, service_name) \
		(axis2_engine_config_get_ops(env, engine_config)->remove_service(env, \
		engine_config, service_name));
		
/************************* End of function macros *****************************/
/************************* Start of function pointers *************************/

typedef axis2_status_t (*axis2_engine_config_free_t)(axis2_environment_t *env,
		axis2_engine_config_t *engine_config);
		
typedef axis2_status_t (*axis2_engine_config_add_service_group_t)
		(axis2_environment_t *env, axis2_engine_config_t *engine_config
    	, axis2_description_servicegroup_t *service_group_desc);

typedef axis2_description_service_t *(*axis2_engine_config_get_service_t)
		(axis2_environment_t *env, axis2_engine_config_t *engine_config
		, const axis2_char_t* service_name);

typedef axis2_status_t (*axis2_engine_config_remove_service_t)
		(axis2_environment_t *env, axis2_engine_config_t *engine_config
		, const axis2_char_t *name);	
		
/************************* End of function pointers ***************************/
	
/**
 * The palce where all the global states of Axis is accessed.
 * All the global states kept in the <code>EngineRegistry</code> and all the
 * Service states kept in the <code>MessageContext</code>. Other runtime
 * artifacts does not keep states forward from the execution.
 */

/**
  * @struct axis2_engine_config_ops
  * @brief ENGINE engine_config operations
  * This holds the operations of the engine axis2_engine_config.
  */
struct axis2_engine_config_ops_s
{
	axis2_engine_config_free_t free;
	axis2_engine_config_add_service_group_t add_service_group;
	axis2_engine_config_get_service_t get_service;
	axis2_engine_config_remove_service_t remove_service;
};

/** function to get the operations consturct for axis2_engine_config_create
 * @return operation struct
 */
axis2_engine_config_ops_t *axis2_engine_config_get_ops(axis2_environment_t *env,
		axis2_engine_config_t *config);

/** create axis_engine_config struct
 * @return axis_engine_config struct
 */
axis2_engine_config_t *axis2_engine_config_create(axis2_environment_t *env);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_ENGINE_CONFIG_H */
