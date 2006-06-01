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

#ifndef AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_H
#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_H

/**
 * @file axis2_woden_soap_header_block_element.h
 * @brief Axis2 Soap Header Block Element Soap Header Block
 * This interface represents the &lt;wsoap:header&gt; extension element that 
 * can appear within a Binding Fault or Binding Message Reference.
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

typedef struct axis2_woden_soap_header_block_element axis2_woden_soap_header_block_element_t;
typedef struct axis2_woden_soap_header_block_element_ops axis2_woden_soap_header_block_element_ops_t;
typedef union axis2_woden_soap_header_block_element_base axis2_woden_soap_header_block_element_base_t;

/** @defgroup axis2_woden_soap_header_block_element Soap Header Block Element
  * @ingroup axis2_soap_header_block
  * @{
  */

struct axis2_woden_soap_header_block_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *soap_header_block_element,
            const axis2_env_t *env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *soap_header_block_element,
            const axis2_env_t *env);

    /**
     * Set the QName that identifies the Schema element declaration
     * relating to this soap header.
     * 
     * @param qname the QName that identifies a Schema element declaration
     */
    axis2_status_t (AXIS2_CALL *
    set_element_qname) (
            void *soap_header_block_element,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_element_qname) (
            void *soap_header_block_element,
            const axis2_env_t *env); 
 
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
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_must_understand) (
            void *soap_header_block_element,
            const axis2_env_t *env,
            axis2_bool_t understood); 

    axis2_bool_t (AXIS2_CALL *
    must_understand) (
            void *soap_header_block_element,
            const axis2_env_t *env);
  
    axis2_status_t (AXIS2_CALL *
    set_parent_element) (
            void *soap_header_block_element,
            const axis2_env_t *env,
            void *wsdl_el);

    void *(AXIS2_CALL *
    get_parent_element) (
            void *soap_header_block_element,
            const axis2_env_t *env); 
  
    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (
            void *soap_header_block_element,
            const axis2_env_t *env,
            void *doc_el);

    axis2_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (
            void *soap_header_block_element,
            const axis2_env_t *env);
 
};

union axis2_woden_soap_header_block_element_base
{
    axis2_woden_ext_element_t ext_element;
    axis2_woden_attr_extensible_t attr_extensible;
    axis2_woden_element_extensible_t element_extensible;
};

struct axis2_woden_soap_header_block_element
{
    axis2_woden_soap_header_block_element_base_t base;
    axis2_woden_soap_header_block_element_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_soap_header_block_element_t * AXIS2_CALL
axis2_woden_soap_header_block_element_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_soap_header_block_element_resolve_methods(
        axis2_woden_soap_header_block_element_t *soap_header_block_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_FREE(soap_header_block_element, env) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
         free (soap_header_block_element, env))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_TYPE(soap_header_block_element, env) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
         type (soap_header_block_element, env))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_SET_ELEMENT_QNAME(soap_header_block_element, env, qname) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
         set_element_qname (soap_header_block_element, env, qname))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_GET_ELEMENT_QNAME(soap_header_block_element, env) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
        get_element_qname  (soap_header_block_element, env))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_GET_ELEMENT(soap_header_block_element, env) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
         get_element (soap_header_block_element, env))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_SET_MUST_UNDERSTAND(soap_header_block_element, env, understood) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
         set_must_understand (soap_header_block_element, env, understood))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_MUST_UNDERSTAND(soap_header_block_element, env) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
        must_understand  (soap_header_block_element, env))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_SET_PARENT_ELEMENT(soap_header_block_element, env, wsdl_el) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
         set_parent_element (soap_header_block_element, env, wsdl_el))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_GET_PARENT_ELEMENT(soap_header_block_element, env) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
        get_parent_element  (soap_header_block_element, env))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_ADD_DOCUMENTATION_ELEMENT(soap_header_block_element, env, doc_el) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
        add_documentation_element  (soap_header_block_element, env, doc_el))

#define AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_GET_DOCUMENTATION_ELEMENTS(soap_header_block_element, env) \
		(((axis2_woden_soap_header_block_element_t *) soap_header_block_element)->ops->\
        get_documentation_elements  (soap_header_block_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SOAP_HEADER_BLOCK_ELEMENT_H */
