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

#ifndef AXIS2_WODEN_DESC_ELEMENT_H
#define AXIS2_WODEN_DESC_ELEMENT_H

/**
 * @file axis2_woden_desc_element.h
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

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/wsdl20/xml/axis2_woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_desc_element axis2_woden_desc_element_t;
typedef struct axis2_woden_desc_element_ops axis2_woden_desc_element_ops_t;
typedef union axis2_woden_desc_element_base axis2_woden_desc_element_base_t;

/** @defgroup axis2_woden_desc_element Description Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_desc_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *doc_el,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    to_desc_element_free) (
            void *doc_el,
            axis2_env_t **env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *doc_el,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_document_base_uri) (
            void *desc_ele,
            axis2_env_t **env,
            axis2_url_t *document_base_uri);

    axis2_url_t *(AXIS2_CALL *
    get_document_base_uri) (
            void *desc_ele,
            axis2_env_t **env);
    
    /*
     * Attributes and namespaces
     */
    axis2_status_t (AXIS2_CALL *
    set_target_namespace) (
            void *desc_ele,
            axis2_env_t **env,
            axis2_url_t *namespace_uri);

    axis2_url_t *(AXIS2_CALL *
    get_target_namespace) (
            void *desc_ele,
            axis2_env_t **env);

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
            axis2_env_t **env,
            axis2_char_t *prefix,
            axis2_url_t *namespc);
    
    axis2_status_t (AXIS2_CALL *
    remove_namespace) (
            void *desc_el,
            axis2_env_t **env,
            axis2_char_t *prefix);
    
    /**
     * Get the namespace URI associated with this prefix. Or null if
     * there is no namespace URI associated with this prefix. This is
     * unrelated to the &lt;wsdl:import&gt; element.
     *
     * @see #add_namespace(axis2_char_t , axis2_url_t)
     * @see #get_prefix(axis2_char_t)
     */
    axis2_url_t *(AXIS2_CALL * 
    get_namespace) (
            void *desc_el,
            axis2_env_t **env,
            axis2_char_t *prefix);
    
    axis2_hash_t *(AXIS2_CALL * 
    get_namespaces) (
            void *desc_el,
            axis2_env_t **env);/* TODO return arrays instead of Map? */
    
    /*
     * Element factory methods
     */
    
    void *(AXIS2_CALL *
    create_documentation_element) (
            void *desc_el,
            axis2_env_t **env);
    
    void *(AXIS2_CALL *
    create_import_element) (
            void *desc_el,
            axis2_env_t **env);
    
    void *(AXIS2_CALL *
    create_include_element) (
            void *desc_el,
            axis2_env_t **env);
    
    void *(AXIS2_CALL *
    create_types_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_interface_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_interface_fault_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_interface_op_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_interface_fault_ref_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_interface_msg_ref_element) (
            void *desc_el,
            axis2_env_t **env);
    
    void *(AXIS2_CALL *
    create_binding_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_binding_fault_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_binding_op_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_binding_fault_ref_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_binding_msg_ref_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_feature_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_svc_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_endpoint_element) (
            void *desc_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    create_property_element) (
            void *desc_el,
            axis2_env_t **env);
    
    /*
     * Element accessor and modifier methods
     * 
     * TODO removeXXX(obj), getXXX(key) methods
     * 
     */
    axis2_status_t (AXIS2_CALL *
    add_import_element) (
            void *desc_ele,
            axis2_env_t **env,
            void *import_el);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_import_elements) (
            void *desc_ele,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_include_element) (
            void *desc_ele,
            axis2_env_t **env,
            void *include_el);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_include_elements) (
            void *desc_ele,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_types_element) (
            void *desc_ele,
            axis2_env_t **env,
            void *types_el);
    
    void *(AXIS2_CALL *
    get_types_element) (
            void *desc_ele,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_interface_element) (
            void *desc_ele,
            axis2_env_t **env,
            void *interface_el);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_interface_elements) (
            void *desc_ele,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_binding_element) (
            void *desc_ele,
            axis2_env_t **env,
            void *binding_el);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_binding_elements) (
            void *desc_ele,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_svc_element) (
            void *desc_ele,
            axis2_env_t **env,
            void *svc_el);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_svc_elements) (
            void *desc_ele,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    add_extension_registry) (
            void *desc_ele,
            axis2_env_t **env,
            void *ext_reg);
    
    void *(AXIS2_CALL *
    get_extension_registry) (
            void *desc_ele,
            axis2_env_t **env);

    /**
     * @return the Description component for this Description Element.
     */
    void *(AXIS2_CALL *
    to_component) (
            void *desc_ele,
            axis2_env_t **env);
    

};

struct axis2_woden_desc_element
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_desc_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
axis2_woden_desc_element_resolve_methods(
        axis2_woden_desc_element_t *desc_element,
        axis2_env_t **env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_DESC_ELEMENT_FREE(doc_el, env) \
		(((axis2_woden_desc_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define AXIS2_WODEN_DESC_ELEMENT_TO_DESC_ELEMENT_FREE(doc_el, env) \
		(((axis2_woden_desc_element_t *) doc_el)->ops->\
         to_desc_element_free (doc_el, env))

#define AXIS2_WODEN_DESC_ELEMENT_TYPE(doc_el, env) \
		(((axis2_woden_desc_element_t *) doc_el)->ops->\
         type (doc_el, env))

#define AXIS2_WODEN_DESC_ELEMENT_SET_REF(doc_el, env, uri) \
		(((axis2_woden_desc_element_t *) doc_el)->ops->\
         set_ref(doc_el, env, uri))

#define AXIS2_WODEN_DESC_ELEMENT_GET_REF(doc_el, env) \
		(((axis2_woden_desc_element_t *) doc_el)->ops->\
         get_ref(doc_el, env))

#define AXIS2_WODEN_DESC_ELEMENT_SET_REQUIRED(doc_el, env, required) \
		(((axis2_woden_desc_element_t *) doc_el)->ops->\
         set_required(doc_el, env, required))

#define AXIS2_WODEN_DESC_ELEMENT_IS_REQUIRED(doc_el, env) \
		(((axis2_woden_desc_element_t *) doc_el)->ops->\
         is_required(doc_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_DESC_ELEMENT_H */

