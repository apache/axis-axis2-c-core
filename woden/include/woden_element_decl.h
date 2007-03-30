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

#ifndef WODEN_ELEMENT_DECL_H
#define WODEN_ELEMENT_DECL_H

/**
 * @file woden_element_decl.h
 * @brief Axis2 Element Declaration Interface
 * This interface represents the Element Declaration component described
 * in the WSDL 2.0 Component Model specification (within the Description 
 * Component section). An Element Declaration refers to an element, such as
 * a global element declaration in the XML Schema type system 
 * (&lt;xs:element&gt;), that describes the content of WSDL input, output
 * and fault messages.  However, it does not impose XML Schema as the type system.  
 * It returns a String representing the content model or type system 
 * (e.g. "http://www.w3.org/2001/XMLSchema") and a axis2_generic_obj_t type 
 * representing the content of the element declaration. This Object may
 * be cast to a type appropriate for the content model.
 * 
 */

#include <axutil_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axis2_array_list.h>
#include <axis2_generic_obj.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_element_decl woden_element_decl_t;
typedef struct woden_element_decl_ops woden_element_decl_ops_t;
struct woden_ext_element;

/** @defgroup woden_element_decl Element Declaration
  * @ingroup woden
  * @{
  */

struct woden_element_decl_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *
            element_decl,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_element_decl_free) (
            void *
            element_decl,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *
            element_decl,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *element_decl,
            const axis2_env_t *env);
     
    /* ************************************************************
     *  Element Declaration interface methods (the WSDL Component model)
     * ************************************************************/
    /**
     * @return the QName identifying this element declaration in the
     * underlying type system definition.
     *  
     */
    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *decl,
            const axis2_env_t *env); 

    /**
     * Indicates the underlying type system of this element declaration.
     * For example, "http://www.w3.org/2001/XMLSchema" indicates the W3 XML Schema
     * type system.
     *  
     * @return the URI identifying the type system
     */
    axis2_uri_t *(AXIS2_CALL *
    get_system) (
            void *decl,
            const axis2_env_t *env); 

    /**
     * Indicates the type of model or API used to represent components from the 
     * underlying type system identified by the get_system() method.
     * <p>
     * For example:
     * <ul>
     * <li>"org.w3c.dom" indicates that the DOM API is used, so the element declaration
     * content will be represented by a org.w3c.dom.Element.
     * <li>"org.apache.ws.commons.schema" indicates that the XmlSchema API from the
     * Apache WebServices project is used, so an XmlSchemaElement will represent the 
     * element declaration content. 
     * </ul>
     * 
     * @return a String identifying the content model or API used to represent this 
     * element declaration
     */
    axis2_char_t *(AXIS2_CALL *
    get_content_model) (
            void *decl,
            const axis2_env_t *env); 

    /**
     * Returns the content of the element declaration in an object
     * specific to the underlying content model API. The caller may then
     * cast this Object to the appropriate type, based on the content model
     * API indicated by the get_content() method.
     * 
     * @return the Object representing the content of the element declaration
     */
    axis2_generic_obj_t *(AXIS2_CALL *
    get_content) (
            void *decl,
            const axis2_env_t *env); 

    /* ************************************************************
     *  Non-API implementation methods
     * ************************************************************/

    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *decl,
            const axis2_env_t *env,
            axis2_qname_t *qname); 

    axis2_status_t (AXIS2_CALL *
    set_system) (
            void *decl,
            const axis2_env_t *env,
            axis2_uri_t *type_system_uri); 

    axis2_status_t (AXIS2_CALL *
    set_content_model) (
            void *decl,
            const axis2_env_t *env,
            axis2_char_t *content_model); 

    axis2_status_t (AXIS2_CALL *
    set_content) (
            void *decl,
            const axis2_env_t *env,
            axis2_generic_obj_t *element_content);


};

struct woden_element_decl
{
    woden_element_decl_ops_t *ops;
};

AXIS2_EXTERN woden_element_decl_t * AXIS2_CALL
woden_element_decl_create(const axis2_env_t *env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_element_decl_resolve_methods(
        woden_element_decl_t *decl,
        const axis2_env_t *env,
        woden_element_decl_t *decl_impl,
        axis2_hash_t *methods);

#define WODEN_ELEMENT_DECL_FREE(decl, env) \
      (((woden_element_decl_t *) decl)->ops->\
         free (decl, env))

#define WODEN_ELEMENT_DECL_TO_ELEMENT_DECL_FREE(decl, \
        env) \
      (((woden_element_decl_t *) decl)->ops->\
         to_element_decl_free (decl, env))

#define WODEN_ELEMENT_DECL_SUPER_OBJS(decl, env) \
      (((woden_element_decl_t *) decl)->ops->\
         super_objs (decl, env))

#define WODEN_ELEMENT_DECL_TYPE(decl, env) \
      (((woden_element_decl_t *) decl)->ops->\
         type (decl, env))

#define WODEN_ELEMENT_DECL_GET_QNAME(decl, env) \
      (((woden_element_decl_t *) decl)->ops->\
         get_qname(decl, env))

#define WODEN_ELEMENT_DECL_GET_SYSTEM(decl, env) \
      (((woden_element_decl_t *) decl)->ops->\
         get_system(decl, env))

#define WODEN_ELEMENT_DECL_GET_CONTENT_MODEL(decl, env) \
      (((woden_element_decl_t *) decl)->ops->\
         get_content_model(decl, env))

#define WODEN_ELEMENT_DECL_GET_CONTENT(decl, env) \
      (((woden_element_decl_t *) decl)->ops->\
         get_content(decl, env))

#define WODEN_ELEMENT_DECL_SET_QNAME(decl, env, qname) \
      (((woden_element_decl_t *) decl)->ops->\
         set_qname(decl, env, qname))

#define WODEN_ELEMENT_DECL_SET_SYSTEM(decl, env, type_system_uri) \
      (((woden_element_decl_t *) decl)->ops->\
         set_system(decl, env, type_system_uri))

#define WODEN_ELEMENT_DECL_SET_CONTENT_MODEL(decl, env, content_model) \
      (((woden_element_decl_t *) decl)->ops->\
         set_content_model(decl, env, content_model))

#define WODEN_ELEMENT_DECL_SET_CONTENT(decl, env, element_content) \
      (((woden_element_decl_t *) decl)->ops->\
         set_content(decl, env, element_content))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_ELEMENT_DECL_H */
