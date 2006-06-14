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

#ifndef WODEN_ENDPOINT_ELEMENT_H
#define WODEN_ENDPOINT_ELEMENT_H

/**
 * @file woden_endpoint_element.h
 * @brief Axis2 Endpoint Element Interface
 * This interface represents an &lt;endpoint&gt; element 
 * information item, a child element of WSDL &lt;service&gt;.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_configurable_element.h>
#include <woden_nested_element.h>
#include <woden_nc_name.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_endpoint_element woden_endpoint_element_t;
typedef struct woden_endpoint_element_ops woden_endpoint_element_ops_t;
typedef union woden_endpoint_element_base woden_endpoint_element_base_t;

/** @defgroup woden_endpoint_element Endpoint Element
  * @ingroup axis2_wsdl
  * @{
  */

struct woden_endpoint_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *endpoint_el,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *endpoint_el,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_name) (
            void *endpoint_el,
            const axis2_env_t *env,
            woden_nc_name_t *name);

    woden_nc_name_t *(AXIS2_CALL *
    get_name) (
            void *endpoint_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_binding_qname) (
            void *endpoint_el,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_binding_qname) (
            void *endpoint_el,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_binding_element) (
            void *endpoint_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_address) (
            void *endpoint_el,
            const axis2_env_t *env,
            axis2_uri_t *uri);

    axis2_uri_t *(AXIS2_CALL *
    get_address) (
            void *endpoint_el,
            const axis2_env_t *env);

};

union woden_endpoint_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
    woden_nested_element_t nested_element;
};

struct woden_endpoint_element
{
    woden_endpoint_element_base_t base;
    woden_endpoint_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_endpoint_element_resolve_methods(
        woden_endpoint_element_t *endpoint_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_ENDPOINT_ELEMENT_FREE(endpoint_el, env) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         free (endpoint_el, env))

#define WODEN_ENDPOINT_ELEMENT_TYPE(endpoint_el, env) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         type (endpoint_el, env))

#define WODEN_ENDPOINT_ELEMENT_SET_NAME(endpoint_el, env, name) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         set_name(endpoint_el, env, name))

#define WODEN_ENDPOINT_ELEMENT_GET_NAME(endpoint_el, env) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         get_name(endpoint_el, env))

#define WODEN_ENDPOINT_ELEMENT_SET_BINDING_QNAME(endpoint_el, env, qname) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         set_binding_qname(endpoint_el, env, qname))

#define WODEN_ENDPOINT_ELEMENT_GET_BINDING_QNAME(endpoint_el, env) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         get_binding_qname(endpoint_el, env))

#define WODEN_ENDPOINT_ELEMENT_GET_BINDING_ELEMENT(endpoint_el, env) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         get_binding_element(endpoint_el, env))

#define WODEN_ENDPOINT_ELEMENT_SET_ADDRESS(endpoint_el, env, uri) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         set_address(endpoint_el, env, uri))

#define WODEN_ENDPOINT_ELEMENT_GET_ADDRESS(endpoint_el, env) \
      (((woden_endpoint_element_t *) endpoint_el)->ops->\
         get_address(endpoint_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_ENDPOINT_ELEMENT_H */
