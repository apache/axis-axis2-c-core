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

#ifndef WODEN_WSDL10_DESC_ELEMENT_H
#define WODEN_WSDL10_DESC_ELEMENT_H

/**
 * @file woden_wsdl10_desc_element.h
 * @brief Axis2 Description Element Interface
 * This interface represents a &lt;wsdl:description&gt; element. 
 * It declares the behaviour required to support 
 * parsing, creating and manipulating a &lt;description&gt; element.
 * 
 * TODO initially, have added a method to get the WSDL component
 * model from the xml instance (i.e. get a Description from this 
 * instance of Description Element). Still need to decide 
 * where on the API to put this. Maybe in WSDL Reader? 
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
#include <woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_wsdl10_desc_element woden_wsdl10_desc_element_t;
typedef struct woden_wsdl10_desc_element_ops woden_wsdl10_desc_element_ops_t;
typedef union woden_wsdl10_desc_element_base woden_wsdl10_desc_element_base_t;

/** @defgroup woden_wsdl10_desc_element Description Element
  * @ingroup woden
  * @{
  */

struct woden_wsdl10_desc_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *desc_el,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (void *desc_el,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_document_base_uri) (
            void *desc_ele,
            const axutil_env_t *env,
            axis2_uri_t *document_base_uri);

    axis2_uri_t *(AXIS2_CALL *
    get_document_base_uri) (
            void *desc_ele,
            const axutil_env_t *env);
    
    /*
     * Attributes and namespaces
     */
    axis2_status_t (AXIS2_CALL *
    set_target_namespace) (
            void *desc_ele,
            const axutil_env_t *env,
            axis2_uri_t *namespace_uri);

    axis2_uri_t *(AXIS2_CALL *
    get_target_namespace) (
            void *desc_ele,
            const axutil_env_t *env);

    /**
     * This is a way to add a namespace association to a definition.
     * It is similar to adding a namespace prefix declaration to the
     * top of a &lt;wsdl:definition&gt; element. This has nothing to do
     * with the &lt;wsdl:import&gt; element; there are separate methods for
     * dealing with information described by &lt;wsdl:import&gt; elements.
     *
     * @param prefix the prefix to use for this namespace (when
     * rendering this information as XML). Use null or an empty string
     * to describe the default namespace (i.e. xmlns="...").
     * @param namespaceURI the namespace URI to associate the prefix
     * with. If you use null, the namespace association will be removed.
     */
    axis2_status_t (AXIS2_CALL *
    add_namespace) (
            void *desc_el,
            const axutil_env_t *env,
            axis2_char_t *prefix,
            axis2_uri_t *namespc);
    
    axis2_status_t (AXIS2_CALL *
    remove_namespace) (
            void *desc_el,
            const axutil_env_t *env,
            axis2_char_t *prefix);
    
    /**
     * Get the namespace URI associated with this prefix. Or null if
     * there is no namespace URI associated with this prefix. This is
     * unrelated to the &lt;wsdl:import&gt; element.
     *
     * @see #add_namespace(axis2_char_t , axis2_uri_t)
     * @see #get_prefix(axis2_char_t)
     */
    axis2_uri_t *(AXIS2_CALL * 
    get_namespace) (
            void *desc_el,
            const axutil_env_t *env,
            axis2_char_t *prefix);
    
    axis2_hash_t *(AXIS2_CALL * 
    get_namespaces) (
            void *desc_el,
            const axutil_env_t *env);/* TODO return arrays instead of Map? */
    
    /*
     * Element factory methods
     */
    
    void *(AXIS2_CALL *
    create_documentation_element) (
            void *desc_el,
            const axutil_env_t *env);
    
    void *(AXIS2_CALL *
    create_import_element) (
            void *desc_el,
            const axutil_env_t *env);
    
    void *(AXIS2_CALL *
    create_include_element) (
            void *desc_el,
            const axutil_env_t *env);
    
    void *(AXIS2_CALL *
    create_types_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_msg_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_part_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_interface_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_interface_fault_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_interface_op_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_interface_fault_ref_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_interface_msg_ref_element) (
            void *desc_el,
            const axutil_env_t *env);
    
    void *(AXIS2_CALL *
    create_binding_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_binding_fault_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_binding_op_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_binding_fault_ref_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_binding_msg_ref_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_feature_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_svc_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_endpoint_element) (
            void *desc_el,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    create_property_element) (
            void *desc_el,
            const axutil_env_t *env);
    
    /*
     * Element accessor and modifier methods
     * 
     * TODO removeXXX(obj), getXXX(key) methods
     * 
     */
    axis2_status_t (AXIS2_CALL *
    add_import_element) (
            void *desc_ele,
            const axutil_env_t *env,
            void *import_el);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_import_elements) (
            void *desc_ele,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    add_include_element) (
            void *desc_ele,
            const axutil_env_t *env,
            void *include_el);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_include_elements) (
            void *desc_ele,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_types_element) (
            void *desc_ele,
            const axutil_env_t *env,
            void *types_el);
    
    void *(AXIS2_CALL *
    get_types_element) (
            void *desc_ele,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    add_msg_element) (
            void *desc_ele,
            const axutil_env_t *env,
            void *msg_el);
    
    axis2_status_t (AXIS2_CALL *
    add_interface_element) (
            void *desc_ele,
            const axutil_env_t *env,
            void *interface_el);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_msg_elements) (
            void *desc_ele,
            const axutil_env_t *env);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_interface_elements) (
            void *desc_ele,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    add_binding_element) (
            void *desc_ele,
            const axutil_env_t *env,
            void *binding_el);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_binding_elements) (
            void *desc_ele,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    add_svc_element) (
            void *desc_ele,
            const axutil_env_t *env,
            void *svc_el);
    
    axutil_array_list_t *(AXIS2_CALL *
    get_svc_elements) (
            void *desc_ele,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_ext_registry) (
            void *desc_ele,
            const axutil_env_t *env,
            void *ext_reg);
    
    void *(AXIS2_CALL *
    get_ext_registry) (
            void *desc_ele,
            const axutil_env_t *env);

    /**
     * @return the Description component for this Description Element.
     */
    void *(AXIS2_CALL *
    to_component) (
            void *desc_ele,
            const axutil_env_t *env);
    

};

struct woden_wsdl10_desc_element
{
    woden_documentable_element_t documentable_element;
    woden_wsdl10_desc_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_wsdl10_desc_element_resolve_methods(
        woden_wsdl10_desc_element_t *desc_element,
        const axutil_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_DESC_ELEMENT_FREE(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
         free (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_TYPE(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
         type (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_SET_DOCUMENT_BASE_URI(desc_el, env, document_base_uri) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
         set_document_base_uri(desc_el, env, document_base_uri))

#define WODEN_WSDL10_DESC_ELEMENT_GET_DOCUMENT_BASE_URI(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
         get_document_base_uri(desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_SET_TARGET_NAMESPACE(desc_el, env, namespc_uri) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
         set_target_namespace(desc_el, env, namespc_uri))

#define WODEN_WSDL10_DESC_ELEMENT_GET_TARGET_NAMESPACE(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
         get_target_namespace(desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_ADD_NAMESPACE(desc_el, env, prefix, namespc) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        add_namespace (desc_el, env, prefix, namespc))

#define WODEN_WSDL10_DESC_ELEMENT_REMOVE_NAMESPACE(desc_el, env, prefix) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        remove_namespace (desc_el, env, prefix))

#define WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACE(desc_el, env, prefix) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_namespace (desc_el, env, prefix))

#define WODEN_WSDL10_DESC_ELEMENT_GET_NAMESPACES(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_namespaces (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_DOCUMENTATION_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_documentation_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_IMPORT_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_import_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_INCLUDE_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_include_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_TYPES_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_types_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_MSG_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_msg_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_PART_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_part_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_interface_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_FAULT_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_interface_fault_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_OP_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_interface_op_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_FAULT_REF_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_interface_fault_ref_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_INTERFACE_MSG_REF_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_interface_msg_ref_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_binding_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_FAULT_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_binding_fault_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_OP_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_binding_op_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_FAULT_REF_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_binding_fault_ref_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_BINDING_MSG_REF_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_binding_msg_ref_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_FEATURE_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_feature_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_SVC_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_svc_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_ENDPOINT_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_endpoint_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_CREATE_PROPERTY_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        create_property_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_ADD_IMPORT_ELEMENT(desc_el, env, import_el) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        add_import_element (desc_el, env, import_el))

#define WODEN_WSDL10_DESC_ELEMENT_GET_IMPORT_ELEMENTS(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_import_elements (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_ADD_INCLUDE_ELEMENT(desc_el, env, include_el) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        add_include_element (desc_el, env, include_el))

#define WODEN_WSDL10_DESC_ELEMENT_GET_INCLUDE_ELEMENTS(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_include_elements (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_SET_TYPES_ELEMENT(desc_el, env, types_el) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        set_types_element (desc_el, env, types_el))

#define WODEN_WSDL10_DESC_ELEMENT_GET_TYPES_ELEMENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_types_element (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_ADD_MSG_ELEMENT(desc_el, env, msg_el) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        add_msg_element (desc_el, env, msg_el))

#define WODEN_WSDL10_DESC_ELEMENT_ADD_INTERFACE_ELEMENT(desc_el, env, interface_el) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        add_interface_element (desc_el, env, interface_el))

#define WODEN_WSDL10_DESC_ELEMENT_GET_MSG_ELEMENTS(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_msg_elements (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_GET_INTERFACE_ELEMENTS(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_interface_elements (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_ADD_BINDING_ELEMENT(desc_el, env, binding_el) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        add_binding_element (desc_el, env, binding_el))

#define WODEN_WSDL10_DESC_ELEMENT_GET_BINDING_ELEMENTS(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_binding_elements (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_ADD_SVC_ELEMENT(desc_el, env, svc_el) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        add_svc_element (desc_el, env, svc_el))

#define WODEN_WSDL10_DESC_ELEMENT_GET_SVC_ELEMENTS(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_svc_elements (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_SET_EXT_REGISTRY(desc_el, env, ext_reg) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        set_ext_registry (desc_el, env, ext_reg))

#define WODEN_WSDL10_DESC_ELEMENT_GET_EXT_REGISTRY(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        get_ext_registry (desc_el, env))

#define WODEN_WSDL10_DESC_ELEMENT_TO_COMPONENT(desc_el, env) \
      (((woden_wsdl10_desc_element_t *) desc_el)->ops->\
        to_component (desc_el, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_DESC_ELEMENT_H */

