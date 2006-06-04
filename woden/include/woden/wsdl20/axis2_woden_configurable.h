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

#ifndef AXIS2_WODEN_CONFIGURABLE_H
#define AXIS2_WODEN_CONFIGURABLE_H

/**
 * @file axis2_woden_configurable.h
 * @brief Axis2 Configurable Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_configurable_element.h>
#include <woden/wsdl20/axis2_woden_configurable_component.h>
#include <woden/wsdl20/axis2_woden_documentable.h>

/** @defgroup axis2_woden_configurable Configurable
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_configurable_base axis2_woden_configurable_base_t;
typedef struct axis2_woden_configurable axis2_woden_configurable_t;
typedef struct axis2_woden_configurable_ops axis2_woden_configurable_ops_t;
struct axis2_woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_configurable_ops
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
    struct axis2_woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *configurable,
            const axis2_env_t *env);


};

union axis2_woden_configurable_base
{
    axis2_woden_documentable_t documentable;
    axis2_woden_configurable_element_t configurable_elem;
    axis2_woden_configurable_component_t configurable_component;
};

struct axis2_woden_configurable
{
    axis2_woden_configurable_base_t base;
    axis2_woden_configurable_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_configurable_t * AXIS2_CALL
axis2_woden_configurable_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_configurable_resolve_methods(
        axis2_woden_configurable_t *configurable,
        const axis2_env_t *env,
        axis2_woden_configurable_t *configurable_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/


#define AXIS2_WODEN_CONFIGURABLE_FREE(configurable, env) \
      (((axis2_woden_configurable_t *) configurable)->ops->\
         free(configurable, env))

#define AXIS2_WODEN_CONFIGURABLE_TO_CONFIGURABLE_FREE(configurable, env) \
      (((axis2_woden_configurable_t *) configurable)->ops->\
         to_configurable_free(configurable, env))

#define AXIS2_WODEN_CONFIGURABLE_SUPER_OBJS(configurable, env) \
      (((axis2_woden_configurable_t *) configurable)->ops->\
         super_objs(configurable, env))

#define AXIS2_WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env) \
      (((axis2_woden_configurable_t *) configurable)->ops->\
         get_base_impl(configurable, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_CONFIGURABLE_H */
