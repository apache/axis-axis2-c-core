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

#ifndef AXIS2_WODEN_INTERFACE_OP_ELEMENT_H
#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_H

/**
 * @file axis2_woden_interface_op_element.h
 * @brief Axis2 Interface Msg Reference Element Interface
 * This interface represents an &lt;operation&gt; child element 
 * of the WSDL &lt;interface&gt; element. 
 * It declares the behaviour required to support parsing, 
 * creating and manipulating an &lt;operation&gt; element.
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
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/wsdl20/xml/axis2_woden_configurable_element.h>
#include <woden/wsdl20/xml/axis2_woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_interface_op_element axis2_woden_interface_op_element_t;
typedef struct axis2_woden_interface_op_element_ops axis2_woden_interface_op_element_ops_t;
typedef union axis2_woden_interface_op_element_base axis2_woden_interface_op_element_base_t;

/** @defgroup axis2_woden_interface_op_element Interface Msg Reference Element
  * @ingroup axis2_interface
  * @{
  */

struct axis2_woden_interface_op_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *interface_op_element,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    to_interface_op_element_free) (
            void *interface_op_element,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *interface_op_element,
            axis2_env_t **env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *interface_op_element,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *iface_op_ele,
            axis2_env_t **env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *iface_op_ele,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    set_pattern) (
            void *iface_op_ele,
            axis2_env_t **env,
            axis2_url_t *uri);
    
    axis2_url_t *(AXIS2_CALL *
    get_pattern) (
            void *iface_op_ele,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    add_style_uri) (
            void *iface_op_ele,
            axis2_env_t **env,
            axis2_url_t *uri);

    axis2_status_t (AXIS2_CALL *
    remove_style_uri) (
            void *iface_op_ele,
            axis2_env_t **env,
            axis2_url_t *uri);
    
    axis2_url_t *(AXIS2_CALL *
    get_style) (
            void *iface_op_ele,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_interface_msg_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *msg_ref);

    axis2_status_t (AXIS2_CALL *
    remove_interface_msg_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *msg_ref);
 
    axis2_array_list_t *(AXIS2_CALL *
    get_interface_msg_ref_elements) (
            void *iface_op_ele,
            axis2_env_t **env);
     
    axis2_status_t (AXIS2_CALL *
    add_interface_fault_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *fault_ref);

    axis2_status_t (AXIS2_CALL *
    remove_interface_fault_ref_element) (
            void *iface_op_ele,
            axis2_env_t **env,
            void *fault_ref);
 
    axis2_array_list_t *(AXIS2_CALL *
    get_interface_fault_ref_elements) (
            void *iface_op_ele,
            axis2_env_t **env);
    

};

union axis2_woden_interface_op_element_base
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_configurable_element_t configurable_element;
    axis2_woden_nested_element_t nested_element;
};

struct axis2_woden_interface_op_element
{
    axis2_woden_interface_op_element_base_t base;
    axis2_woden_interface_op_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_interface_op_element_t *)
axis2_woden_interface_op_element_create(
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_status_t)
axis2_woden_interface_op_element_resolve_methods(
        axis2_woden_interface_op_element_t *interface_op_element,
        axis2_env_t **env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_FREE(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         free (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_TO_INTERFACE_OP_ELEMENT_FREE(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         to_interface_op_element_free (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_SUPER_OBJS(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         super_objs (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_TYPE(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         type (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_SET_QNAME(interface_op_element, env, qname) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         set_qname (interface_op_element, env, qname))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_QNAME(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
        get_qname  (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_SET_PATTERN(interface_op_element, env, uri) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         set_pattern (interface_op_element, env, uri))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_PATTERN(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         get_pattern (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_STYLE_URI(interface_op_element, env, uri) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
        add_style_uri  (interface_op_element, env, uri))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_REMOVE_STYLE_URI(interface_op_element, env, uri) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
        remove_style_uri  (interface_op_element, env, uri))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_STYLE(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
        get_style  (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_MSG_REF_ELEMENT(interface_op_element, env, msg_ref) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         add_msg_ref_element (interface_op_element, env, msg_ref))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_REMOVE_MSG_REF_ELEMENT(interface_op_element, env, msg_ref) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         remove_msg_ref_element (interface_op_element, env, msg_ref))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_MSG_REF_ELEMENTS(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         get_msg_ref_elements (interface_op_element, env))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_FAULT_REF_ELEMENT(interface_op_element, env, fault_ref) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         add_fault_ref_element (interface_op_element, env, fault_ref))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_REMOVE_FAULT_REF_ELEMENT(interface_op_element, env, fault_ref) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         remove_fault_ref_element (interface_op_element, env, fault_ref))

#define AXIS2_WODEN_INTERFACE_OP_ELEMENT_GET_FAULT_REF_ELEMENTS(interface_op_element, env) \
		(((axis2_woden_interface_op_element_t *) interface_op_element)->ops->\
         get_fault_ref_elements (interface_op_element, env))





/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INTERFACE_OP_ELEMENT_H */
