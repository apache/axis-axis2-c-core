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

#ifndef WODEN_WSDL10_SVC_ELEMENT_H
#define WODEN_WSDL10_SVC_ELEMENT_H

/**
 * @file woden_wsdl10_svc_element.h
 * @brief Axis2 Service Element Interface
 * This interface represents the &lt;service&gt; element. 
 *
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
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

typedef struct woden_wsdl10_svc_element woden_wsdl10_svc_element_t;
typedef struct woden_wsdl10_svc_element_ops woden_wsdl10_svc_element_ops_t;
typedef union woden_wsdl10_svc_element_base woden_wsdl10_svc_element_base_t;

/** @defgroup woden_wsdl10_svc_element Service Element
  * @ingroup woden
  * @{
  */

struct woden_wsdl10_svc_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *svc_el,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *svc_el,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *svc_el,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *svc_el,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_interface_qname) (
            void *svc_el,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_interface_qname) (
            void *svc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_interface_element) (
            void *svc_el,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    add_endpoint_element) (
            void *svc_el,
            const axutil_env_t *env,
            void *endpoint);

    axutil_array_list_t *(AXIS2_CALL *
    get_endpoint_elements) (
            void *svc_el,
            const axutil_env_t *env);

};

union woden_wsdl10_svc_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
    woden_nested_element_t nested_element;
};

struct woden_wsdl10_svc_element
{
    woden_wsdl10_svc_element_base_t base;
    woden_wsdl10_svc_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_wsdl10_svc_element_resolve_methods(
        woden_wsdl10_svc_element_t *svc_element,
        const axutil_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_SVC_ELEMENT_FREE(svc_el, env) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         free (svc_el, env))

#define WODEN_WSDL10_SVC_ELEMENT_TYPE(svc_el, env) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         type (svc_el, env))

#define WODEN_WSDL10_SVC_ELEMENT_SET_QNAME(svc_el, env, qname) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         set_qname(svc_el, env, qname))

#define WODEN_WSDL10_SVC_ELEMENT_GET_QNAME(svc_el, env) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         get_qname(svc_el, env))

#define WODEN_WSDL10_SVC_ELEMENT_SET_INTERFACE_QNAME(svc_el, env, qname) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         set_interface_qname(svc_el, env, qname))

#define WODEN_WSDL10_SVC_ELEMENT_GET_INTERFACE_QNAME(svc_el, env) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         get_interface_qname(svc_el, env))

#define WODEN_WSDL10_SVC_ELEMENT_GET_INTERFACE_ELEMENT(svc_el, env) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         get_interface_element(svc_el, env))

#define WODEN_WSDL10_SVC_ELEMENT_ADD_ENDPOINT_ELEMENT(svc_el, env, endpoint) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         add_endpoint_element(svc_el, env, endpoint))

#define WODEN_WSDL10_SVC_ELEMENT_GET_ENDPOINT_ELEMENTS(svc_el, env) \
      (((woden_wsdl10_svc_element_t *) svc_el)->ops->\
         get_endpoint_elements(svc_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_SVC_ELEMENT_H */
