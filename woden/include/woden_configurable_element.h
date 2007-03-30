/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WODEN_CONFIGURABLE_ELEMENT_H
#define WODEN_CONFIGURABLE_ELEMENT_H

/**
 * @file woden_configurable_element.h
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

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <woden.h>
#include <woden_wsdl_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_configurable_element 
        woden_configurable_element_t;
typedef struct woden_configurable_element_ops 
        woden_configurable_element_ops_t;
struct woden_feature_element;
struct woden_property_element;

/** @defgroup woden_configurable_element Configurable Element
  * @ingroup woden
  * @{
  */

struct woden_configurable_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *conf_el,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *conf_el,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    add_feature_element) (
            void *conf_el,
            const axutil_env_t *env,
            struct woden_feature_element *feature);

    axutil_array_list_t *(AXIS2_CALL *
    get_feature_elements) (
            void *conf_el,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    add_property_element) (
            void *conf_el,
            const axutil_env_t *env,
            struct woden_property_element *feature);

    axutil_array_list_t *(AXIS2_CALL *
    get_property_elements) (
            void *conf_el,
            const axutil_env_t *env);


};

struct woden_configurable_element
{
    woden_wsdl_element_t wsdl_element;
    woden_configurable_element_ops_t *ops;
};

/************************Woden C Internal Methods*****************************/
axis2_status_t AXIS2_CALL
woden_configurable_element_resolve_methods(
        woden_configurable_element_t *configurable_element,
        const axutil_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_CONFIGURABLE_ELEMENT_FREE(conf_el, env) \
      (((woden_configurable_element_t *) conf_el)->ops->\
         free (conf_el, env))

#define WODEN_CONFIGURABLE_ELEMENT_TYPE(conf_el, env) \
      (((woden_configurable_element_t *) conf_el)->ops->\
         type (conf_el, env))

#define WODEN_CONFIGURABLE_ELEMENT_ADD_FEATURE_ELEMENT(conf_el, env, \
        feature) \
      (((woden_configurable_element_t *) conf_el)->ops->\
         add_feature_element(conf_el, env, feature))

#define WODEN_CONFIGURABLE_ELEMENT_GET_FEATURE_ELEMENTS(conf_el, env) \
      (((woden_configurable_element_t *) conf_el)->ops->\
         get_feature_elements(conf_el, env))

#define WODEN_CONFIGURABLE_ELEMENT_ADD_PROPERTY_ELEMENT(conf_el, env, \
        property) \
      (((woden_configurable_element_t *) conf_el)->ops->\
         add_property_element(conf_el, env, property))

#define WODEN_CONFIGURABLE_ELEMENT_GET_PROPERTY_ELEMENTS(conf_el, env) \
      (((woden_configurable_element_t *) conf_el)->ops->\
         get_property_elements(conf_el, env))



/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_CONFIGURABLE_ELEMENT_H */
