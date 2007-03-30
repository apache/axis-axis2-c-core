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

#ifndef WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_H
#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_H

/**
 * @file woden_wsdl10_soap_header_block_element.h
 * @brief Axis2 Soap Header Block Element Soap Header Block
 * This interface represents the &lt;wsoap:header&gt; extension element that 
 * can appear within a Binding Fault or Binding Message Reference.
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

typedef struct woden_wsdl10_soap_header_block_element woden_wsdl10_soap_header_block_element_t;
typedef struct woden_wsdl10_soap_header_block_element_ops woden_wsdl10_soap_header_block_element_ops_t;
typedef union woden_wsdl10_soap_header_block_element_base woden_wsdl10_soap_header_block_element_base_t;

/** @defgroup woden_wsdl10_soap_header_block_element Soap Header Block Element
  * @ingroup axiom_soap_header_block
  * @{
  */

struct woden_wsdl10_soap_header_block_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *soap_header_block_element,
            const axutil_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *soap_header_block_element,
            const axutil_env_t *env);

    /**
     * Set the QName that identifies the Schema element declaration
     * relating to this soap header.
     * 
     * @param qname the QName that identifies a Schema element declaration
     */
    axis2_status_t (AXIS2_CALL *
    set_element_qname) (
            void *soap_header_block_element,
            const axutil_env_t *env,
            axutil_qname_t *qname);

    axutil_qname_t *(AXIS2_CALL *
    get_element_qname) (
            void *soap_header_block_element,
            const axutil_env_t *env); 
 
    /**
     * Returns the Schema element declaration identified by the QName in the 'element' 
     * attribute of the &lt;wsoap:header&gt; element. 
     * If this QName does not resolve to an element declaration in a schema that is visible 
     * to the containing WSDL description, null will be returned by this method. 
     * To be visible, the Schema must have been correctly imported or inlined within 
     * the &lt;types&gt; element.
     * 
     * @return the Xml Schema Element identified by the 'element' attribute
     */
    void *(AXIS2_CALL *
    get_element) (
            void *soap_header_block_element,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_must_understand) (
            void *soap_header_block_element,
            const axutil_env_t *env,
            axis2_bool_t understood); 

    axis2_bool_t (AXIS2_CALL *
    must_understand) (
            void *soap_header_block_element,
            const axutil_env_t *env);
  
    axis2_status_t (AXIS2_CALL *
    set_parent_element) (
            void *soap_header_block_element,
            const axutil_env_t *env,
            void *wsdl_el);

    void *(AXIS2_CALL *
    get_parent_element) (
            void *soap_header_block_element,
            const axutil_env_t *env); 
  
    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (
            void *soap_header_block_element,
            const axutil_env_t *env,
            void *doc_el);

    axutil_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (
            void *soap_header_block_element,
            const axutil_env_t *env);
 
};

union woden_wsdl10_soap_header_block_element_base
{
    woden_ext_element_t ext_element;
    woden_attr_extensible_t attr_extensible;
    woden_element_extensible_t element_extensible;
};

struct woden_wsdl10_soap_header_block_element
{
    woden_wsdl10_soap_header_block_element_base_t base;
    woden_wsdl10_soap_header_block_element_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_soap_header_block_element_t * AXIS2_CALL
woden_wsdl10_soap_header_block_element_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl10_soap_header_block_element_resolve_methods(
        woden_wsdl10_soap_header_block_element_t *soap_header_block_element,
        const axutil_env_t *env,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_FREE(soap_header_block_element, env) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
         free (soap_header_block_element, env))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_TYPE(soap_header_block_element, env) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
         type (soap_header_block_element, env))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_SET_ELEMENT_QNAME(soap_header_block_element, env, qname) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
         set_element_qname (soap_header_block_element, env, qname))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_GET_ELEMENT_QNAME(soap_header_block_element, env) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
        get_element_qname  (soap_header_block_element, env))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_GET_ELEMENT(soap_header_block_element, env) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
         get_element (soap_header_block_element, env))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_SET_MUST_UNDERSTAND(soap_header_block_element, env, understood) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
         set_must_understand (soap_header_block_element, env, understood))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_MUST_UNDERSTAND(soap_header_block_element, env) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
        must_understand  (soap_header_block_element, env))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_SET_PARENT_ELEMENT(soap_header_block_element, env, wsdl_el) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
         set_parent_element (soap_header_block_element, env, wsdl_el))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_GET_PARENT_ELEMENT(soap_header_block_element, env) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
        get_parent_element  (soap_header_block_element, env))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_ADD_DOCUMENTATION_ELEMENT(soap_header_block_element, env, doc_el) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
        add_documentation_element  (soap_header_block_element, env, doc_el))

#define WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_GET_DOCUMENTATION_ELEMENTS(soap_header_block_element, env) \
      (((woden_wsdl10_soap_header_block_element_t *) soap_header_block_element)->ops->\
        get_documentation_elements  (soap_header_block_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_SOAP_HEADER_BLOCK_ELEMENT_H */
