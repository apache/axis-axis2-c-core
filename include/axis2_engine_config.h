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
 * @defgroup axis2_context_ CONTEXT engine_config
 * @ingroup axis2_engine_config 
 * @{
 */
	
/**
 * The palce where all the global states of Axis is accessed.
 * All the global states kept in the <code>EngineRegistry</code> and all the
 * Service states kept in the <code>MessageContext</code>. Other runtime
 * artifacts does not keep states forward from the execution.
 */

/**
  * @struct axis2_engine_config
  * @brief CONTEXT engine_config operations
  * This holds the information about engine.
  */
typedef struct
{
    axis2_description_param_include_t * param_include;
    axis2_qname_t *name;
    axis2_hash_t *service_groups;
    
} axis2_engine_config_t;

void axis2_engine_config_add_service_group(axis2_engine_config_t *engine_config
    , axis2_description_servicegroup_t *service_group_desc);

void axis2_engine_config_add_service(axis2_engine_config_t *engine_config, 
    axis2_description_service_t *service_desc);

void axis2_engine_config_get_service(axis2_engine_config_t *engine_config,
    char* service_name);

void axis2_engine_config_remove_service(axis2_engine_config_t *engine_config,
    char *name);

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_ENGINE_CONFIG_H */
