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

#ifndef WODEN_CONFIGURABLE_H
#define WODEN_CONFIGURABLE_H

/**
 * @file woden_configurable.h
 * @brief Axis2 Configurable Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden.h>
#include <woden_configurable_element.h>
#include <woden_configurable_component.h>
#include <woden_documentable.h>

/** @defgroup woden_configurable Configurable
  * @ingroup axis2_wsdl
  * @{
  */

typedef union woden_configurable_base woden_configurable_base_t;
typedef struct woden_configurable woden_configurable_t;
typedef struct woden_configurable_ops woden_configurable_ops_t;
struct woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_configurable_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *configurable,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    to_configurable_free) (
            void *configurable,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *configurable,
            const axis2_env_t *env);
    
    /**
     * @return the base implementation class
     */
    struct woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *configurable,
            const axis2_env_t *env);


};

union woden_configurable_base
{
    woden_documentable_t documentable;
    woden_configurable_element_t configurable_elem;
    woden_configurable_component_t configurable_component;
};

struct woden_configurable
{
    woden_configurable_base_t base;
    woden_configurable_ops_t *ops;
};

AXIS2_EXTERN woden_configurable_t * AXIS2_CALL
woden_configurable_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_configurable_resolve_methods(
        woden_configurable_t *configurable,
        const axis2_env_t *env,
        woden_configurable_t *configurable_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/


#define WODEN_CONFIGURABLE_FREE(configurable, env) \
      (((woden_configurable_t *) configurable)->ops->\
         free(configurable, env))

#define WODEN_CONFIGURABLE_TO_CONFIGURABLE_FREE(configurable, env) \
      (((woden_configurable_t *) configurable)->ops->\
         to_configurable_free(configurable, env))

#define WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env) \
      (((woden_configurable_t *) configurable)->ops->\
         super_objs(configurable, env))

#define WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env) \
      (((woden_configurable_t *) configurable)->ops->\
         get_base_impl(configurable, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_CONFIGURABLE_H */
