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

#ifndef AXIS2_WODEN_SOAP_MODULE_ELEMENT_H
#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_H

/**
 * @file axis2_woden_soap_module_element.h
 * @brief Axis2 Soap Module Element Soap Module
 * This interface represents the &lt;wsoap:module&gt; extension element that 
 * can appear within a Binding, Binding Fault, Binding Operation, Binding
 * Fault Reference or Binding Message Reference.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <axis2_array_list.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_attr_extensible.h>
#include <woden/wsdl20/extensions/axis2_woden_element_extensible.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_soap_module_element axis2_woden_soap_module_element_t;
typedef struct axis2_woden_soap_module_element_ops axis2_woden_soap_module_element_ops_t;
typedef union axis2_woden_soap_module_element_base axis2_woden_soap_module_element_base_t;

/** @defgroup axis2_woden_soap_module_element Soap Module Element
  * @ingroup axis2_soap_module
  * @{
  */

struct axis2_woden_soap_module_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *soap_module_element,
            const axis2_env_t *env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *soap_module_element,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_ref) (
            void *soap_module_element,
            const axis2_env_t *env,
            axis2_url_t *uri);

    axis2_url_t *(AXIS2_CALL *
    get_ref) (
            void *soap_module_element,
            const axis2_env_t *env); 
 
    axis2_status_t (AXIS2_CALL *
    set_parent_element) (
            void *soap_module_element,
            const axis2_env_t *env,
            void *wsdl_el);

    void *(AXIS2_CALL *
    get_parent_element) (
            void *soap_module_element,
            const axis2_env_t *env); 
  
    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (
            void *soap_module_element,
            const axis2_env_t *env,
            void *doc_el);

    axis2_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (
            void *soap_module_element,
            const axis2_env_t *env);
 
};

union axis2_woden_soap_module_element_base
{
    axis2_woden_ext_element_t ext_element;
    axis2_woden_attr_extensible_t attr_extensible;
    axis2_woden_element_extensible_t element_extensible;
};

struct axis2_woden_soap_module_element
{
    axis2_woden_soap_module_element_base_t base;
    axis2_woden_soap_module_element_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_soap_module_element_t * AXIS2_CALL
axis2_woden_soap_module_element_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_soap_module_element_resolve_methods(
        axis2_woden_soap_module_element_t *soap_module_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_FREE(soap_module_element, env) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
         free (soap_module_element, env))

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_TYPE(soap_module_element, env) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
         type (soap_module_element, env))

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_SET_REF(soap_module_element, env, uri) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
         set_ref (soap_module_element, env, uri))

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_GET_REF(soap_module_element, env) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
        get_ref  (soap_module_element, env))

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_SET_PARENT_ELEMENT(soap_module_element, env, wsdl_el) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
         set_parent_element (soap_module_element, env, wsdl_el))

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_GET_PARENT_ELEMENT(soap_module_element, env) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
        get_parent_element  (soap_module_element, env))

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_ADD_DOCUMENTATION_ELEMENT(soap_module_element, env, doc_el) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
        add_documentation_element  (soap_module_element, env, doc_el))

#define AXIS2_WODEN_SOAP_MODULE_ELEMENT_GET_DOCUMENTATION_ELEMENTS(soap_module_element, env) \
      (((axis2_woden_soap_module_element_t *) soap_module_element)->ops->\
        get_documentation_elements  (soap_module_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SOAP_MODULE_ELEMENT_H */
