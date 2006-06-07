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

#ifndef AXIS2_WODEN_CONFIGURABLE_ELEMENT_H
#define AXIS2_WODEN_CONFIGURABLE_ELEMENT_H

/**
 * @file axis2_woden_configurable_element.h
 * @brief Axis2 Configurable Element Interface
 * Interfaces for elements which can have &lt;feature&gt; and &lt;property&gt;
 * child elements will extend this interface. That is, &lt;types&gt;, 
 * &lt;interface&gt; and its child elements, 
 * &lt;binding&gt; and its child elements, and
 * &lt;service&gt; and its child elements.
 * <p>
 * The WSDL elements which are not configurable are
 * &lt;description&gt;, &lt;feature&gt;, &lt;property&gt;, 
 * &lt;import&gt;, &lt;include&gt; and &lt;documentation&gt;.
 * 
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_wsdl_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_configurable_element 
        axis2_woden_configurable_element_t;
typedef struct axis2_woden_configurable_element_ops 
        axis2_woden_configurable_element_ops_t;
struct axis2_woden_feature_element;
struct axis2_woden_property_element;

/** @defgroup axis2_woden_configurable_element Configurable Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_configurable_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *conf_el,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *conf_el,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    add_feature_element) (
            void *conf_el,
            const axis2_env_t *env,
            struct axis2_woden_feature_element *feature);

    axis2_array_list_t *(AXIS2_CALL *
    get_feature_elements) (
            void *conf_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    add_property_element) (
            void *conf_el,
            const axis2_env_t *env,
            struct axis2_woden_property_element *feature);

    axis2_array_list_t *(AXIS2_CALL *
    get_property_elements) (
            void *conf_el,
            const axis2_env_t *env);


};

struct axis2_woden_configurable_element
{
    axis2_woden_wsdl_element_t wsdl_element;
    axis2_woden_configurable_element_ops_t *ops;
};

/************************Woden C Internal Methods*****************************/
axis2_status_t AXIS2_CALL
axis2_woden_configurable_element_resolve_methods(
        axis2_woden_configurable_element_t *configurable_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_CONFIGURABLE_ELEMENT_FREE(conf_el, env) \
      (((axis2_woden_configurable_element_t *) conf_el)->ops->\
         free (conf_el, env))

#define AXIS2_WODEN_CONFIGURABLE_ELEMENT_TYPE(conf_el, env) \
      (((axis2_woden_configurable_element_t *) conf_el)->ops->\
         type (conf_el, env))

#define AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(conf_el, env, \
        feature) \
      (((axis2_woden_configurable_element_t *) conf_el)->ops->\
         add_feature_element(conf_el, env, feature))

#define AXIS2_WODEN_CONFIGURABLE_ELEMENT_GET_FEATURE_ELEMENTS(conf_el, env) \
      (((axis2_woden_configurable_element_t *) conf_el)->ops->\
         get_feature_elements(conf_el, env))

#define AXIS2_WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(conf_el, env, \
        property) \
      (((axis2_woden_configurable_element_t *) conf_el)->ops->\
         add_property_element(conf_el, env, property))

#define AXIS2_WODEN_CONFIGURABLE_ELEMENT_GET_PROPERTY_ELEMENTS(conf_el, env) \
      (((axis2_woden_configurable_element_t *) conf_el)->ops->\
         get_propertyelements(conf_el, env))



/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_CONFIGURABLE_ELEMENT_H */
