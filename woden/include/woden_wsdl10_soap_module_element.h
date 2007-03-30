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

#ifndef WODEN_WSDL10_SOAP_MODULE_ELEMENT_H
#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_H

/**
 * @file woden_wsdl10_soap_module_element.h
 * @brief Axis2 Soap Module Element Soap Module
 * This interface represents the &lt;wsoap:module&gt; extension element that 
 * can appear within a Binding, Binding Fault, Binding Operation, Binding
 * Fault Reference or Binding Message Reference.
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axutil_string.h>
#include <axis2_utils.h>
#include <axutil_hash.h>
#include <axutil_qname.h>
#include <axis2_uri.h>
#include <axutil_array_list.h>
#include <woden.h>
#include <woden_ext_element.h>
#include <woden_attr_extensible.h>
#include <woden_element_extensible.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_wsdl10_soap_module_element woden_wsdl10_soap_module_element_t;
typedef struct woden_wsdl10_soap_module_element_ops woden_wsdl10_soap_module_element_ops_t;
typedef union woden_wsdl10_soap_module_element_base woden_wsdl10_soap_module_element_base_t;

/** @defgroup woden_wsdl10_soap_module_element Soap Module Element
  * @ingroup axiom_soap_module
  * @{
  */

struct woden_wsdl10_soap_module_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *soap_module_element,
            const axutil_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *soap_module_element,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_ref) (
            void *soap_module_element,
            const axutil_env_t *env,
            axis2_uri_t *uri);

    axis2_uri_t *(AXIS2_CALL *
    get_ref) (
            void *soap_module_element,
            const axutil_env_t *env); 
 
    axis2_status_t (AXIS2_CALL *
    set_parent_element) (
            void *soap_module_element,
            const axutil_env_t *env,
            void *wsdl_el);

    void *(AXIS2_CALL *
    get_parent_element) (
            void *soap_module_element,
            const axutil_env_t *env); 
  
    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (
            void *soap_module_element,
            const axutil_env_t *env,
            void *doc_el);
  
    axis2_status_t (AXIS2_CALL *
    set_soap_binding_op_exts) (
            void *soap_module_element,
            const axutil_env_t *env,
            void *soap_binding_op_exts);

    void *(AXIS2_CALL *
    get_soap_binding_op_exts) (
            void *soap_module_element,
            const axutil_env_t *env); 
 
    axis2_status_t (AXIS2_CALL *
    set_soap_address_exts) (
            void *soap_module_element,
            const axutil_env_t *env,
            void *soap_address_exts);

    void *(AXIS2_CALL *
    get_soap_address_exts) (
            void *soap_module_element,
            const axutil_env_t *env); 


    axutil_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (
            void *soap_module_element,
            const axutil_env_t *env);
 
};

union woden_wsdl10_soap_module_element_base
{
    woden_ext_element_t ext_element;
    woden_attr_extensible_t attr_extensible;
    woden_element_extensible_t element_extensible;
};

struct woden_wsdl10_soap_module_element
{
    woden_wsdl10_soap_module_element_base_t base;
    woden_wsdl10_soap_module_element_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_soap_module_element_t * AXIS2_CALL
woden_wsdl10_soap_module_element_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl10_soap_module_element_resolve_methods(
        woden_wsdl10_soap_module_element_t *soap_module_element,
        const axutil_env_t *env,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_FREE(soap_module_element, env) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
         free (soap_module_element, env))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_TYPE(soap_module_element, env) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
         type (soap_module_element, env))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_REF(soap_module_element, env, uri) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
         set_ref (soap_module_element, env, uri))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_REF(soap_module_element, env) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        get_ref  (soap_module_element, env))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_PARENT_ELEMENT(soap_module_element, env, wsdl_el) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
         set_parent_element (soap_module_element, env, wsdl_el))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_PARENT_ELEMENT(soap_module_element, env) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        get_parent_element  (soap_module_element, env))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_ADD_DOCUMENTATION_ELEMENT(soap_module_element, env, doc_el) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        add_documentation_element  (soap_module_element, env, doc_el))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_DOCUMENTATION_ELEMENTS(soap_module_element, env) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        get_documentation_elements  (soap_module_element, env))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_SOAP_ADDRESS_EXTS(soap_module_element, env, doc_el) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        set_soap_address_exts  (soap_module_element, env, doc_el))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_SOAP_ADDRESS_EXTS(soap_module_element, env) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        get_soap_address_exts  (soap_module_element, env))


#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_SET_SOAP_BINDING_OP_EXTS(soap_module_element, env, doc_el) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        set_soap_binding_op_exts  (soap_module_element, env, doc_el))

#define WODEN_WSDL10_SOAP_MODULE_ELEMENT_GET_SOAP_BINDING_OP_EXTS(soap_module_element, env) \
      (((woden_wsdl10_soap_module_element_t *) soap_module_element)->ops->\
        get_soap_binding_op_exts  (soap_module_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_SOAP_MODULE_ELEMENT_H */
