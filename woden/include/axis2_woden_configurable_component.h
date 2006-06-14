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

#ifndef AXIS2_WODEN_CONFIGURABLE_COMPONENT_H
#define AXIS2_WODEN_CONFIGURABLE_COMPONENT_H

/**
 * @file axis2_woden_configurable_component.h
 * @brief Axis2 Configurable Component Interface
 * All components directly or indirectly extend this interface, so it provides 
 * a common term of reference for all components.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_uri.h>
#include <axis2_array_list.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/axis2_woden_wsdl_component.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_configurable_component axis2_woden_configurable_component_t;
typedef struct axis2_woden_configurable_component_ops axis2_woden_configurable_component_ops_t;

/** @defgroup axis2_woden_configurable_component Configurable Component
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_configurable_component_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *configurable_component,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_configurable_component_free) (
            void *configurable_component,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *configurable_component,
            const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_features) (
            void *configurable_component,
            const axis2_env_t *env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_properties) (
            void *configurable_component,
            const axis2_env_t *env);

};

struct axis2_woden_configurable_component
{
    axis2_woden_wsdl_component_t wsdl_component;
    axis2_woden_configurable_component_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
axis2_woden_configurable_component_resolve_methods(
        axis2_woden_configurable_component_t *configurable_component,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_CONFIGURABLE_COMPONENT_FREE(configurable_component, env) \
      (((axis2_woden_configurable_component_t *) configurable_component)->ops->\
         free (configurable_component, env))

#define AXIS2_WODEN_CONFIGURABLE_COMPONENT_TO_CONFIGURABLE_COMPONENT_FREE(configurable_component, env) \
      (((axis2_woden_configurable_component_t *) configurable_component)->ops->\
         to_configurable_component_free (configurable_component, env))

#define AXIS2_WODEN_CONFIGURABLE_COMPONENT_TYPE(configurable_component, env) \
      (((axis2_woden_configurable_component_t *) configurable_component)->ops->\
         type (configurable_component, env))

#define AXIS2_WODEN_CONFIGURABLE_COMPONENT_GET_FEATURES(\
        configurable_component, env) \
      (((axis2_woden_configurable_component_t *) configurable_component)->ops->\
         get_features(configurable_component, env))

#define AXIS2_WODEN_CONFIGURABLE_COMPONENT_GET_PROPERTIES(\
        configurable_component, env) \
      (((axis2_woden_configurable_component_t *) configurable_component)->ops->\
         get_properties(configurable_component, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_CONFIGURABLE_COMPONENT_H */
