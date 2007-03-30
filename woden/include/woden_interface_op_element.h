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

#ifndef WODEN_INTERFACE_OP_ELEMENT_H
#define WODEN_INTERFACE_OP_ELEMENT_H

/**
 * @file woden_interface_op_element.h
 * @brief Axis2 Interface Msg Reference Element Interface
 * This interface represents an &lt;operation&gt; child element 
 * of the WSDL &lt;interface&gt; element. 
 * It declares the behaviour required to support parsing, 
 * creating and manipulating an &lt;operation&gt; element.
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axutil_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axutil_array_list.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_configurable_element.h>
#include <woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_interface_op_element woden_interface_op_element_t;
typedef struct woden_interface_op_element_ops woden_interface_op_element_ops_t;
typedef union woden_interface_op_element_base woden_interface_op_element_base_t;

/** @defgroup woden_interface_op_element Interface Msg Reference Element
  * @ingroup axis2_interface
  * @{
  */

struct woden_interface_op_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *interface_op_element,
            const axutil_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *interface_op_element,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *iface_op_ele,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *iface_op_ele,
            const axutil_env_t *env);
 
    axis2_status_t (AXIS2_CALL *
    set_pattern) (
            void *iface_op_ele,
            const axutil_env_t *env,
            axis2_uri_t *uri);
    
    axis2_uri_t *(AXIS2_CALL *
    get_pattern) (
            void *iface_op_ele,
            const axutil_env_t *env);
 
    axis2_status_t (AXIS2_CALL *
    add_style_uri) (
            void *iface_op_ele,
            const axutil_env_t *env,
            axis2_uri_t *uri);

    axis2_status_t (AXIS2_CALL *
    remove_style_uri) (
            void *iface_op_ele,
            const axutil_env_t *env,
            axis2_uri_t *uri);
    
    axis2_uri_t *(AXIS2_CALL *
    get_style) (
            void *iface_op_ele,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    add_interface_msg_ref_element) (
            void *iface_op_ele,
            const axutil_env_t *env,
            void *msg_ref);

    axis2_status_t (AXIS2_CALL *
    remove_interface_msg_ref_element) (
            void *iface_op_ele,
            const axutil_env_t *env,
            void *msg_ref);
 
    axutil_array_list_t *(AXIS2_CALL *
    get_interface_msg_ref_elements) (
            void *iface_op_ele,
            const axutil_env_t *env);
     
    axis2_status_t (AXIS2_CALL *
    add_interface_fault_ref_element) (
            void *iface_op_ele,
            const axutil_env_t *env,
            void *fault_ref);

    axis2_status_t (AXIS2_CALL *
    remove_interface_fault_ref_element) (
            void *iface_op_ele,
            const axutil_env_t *env,
            void *fault_ref);
 
    axutil_array_list_t *(AXIS2_CALL *
    get_interface_fault_ref_elements) (
            void *iface_op_ele,
            const axutil_env_t *env);
    

};

union woden_interface_op_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
    woden_nested_element_t nested_element;
};

struct woden_interface_op_element
{
    woden_interface_op_element_base_t base;
    woden_interface_op_element_ops_t *ops;
};

AXIS2_EXTERN woden_interface_op_element_t * AXIS2_CALL
woden_interface_op_element_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_interface_op_element_resolve_methods(
        woden_interface_op_element_t *interface_op_element,
        const axutil_env_t *env,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INTERFACE_OP_ELEMENT_FREE(interface_op_element, env) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         free (interface_op_element, env))

#define WODEN_INTERFACE_OP_ELEMENT_TYPE(interface_op_element, env) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         type (interface_op_element, env))

#define WODEN_INTERFACE_OP_ELEMENT_SET_QNAME(interface_op_element, env, qname) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         set_qname (interface_op_element, env, qname))

#define WODEN_INTERFACE_OP_ELEMENT_GET_QNAME(interface_op_element, env) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
        get_qname  (interface_op_element, env))

#define WODEN_INTERFACE_OP_ELEMENT_SET_PATTERN(interface_op_element, env, uri) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         set_pattern (interface_op_element, env, uri))

#define WODEN_INTERFACE_OP_ELEMENT_GET_PATTERN(interface_op_element, env) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         get_pattern (interface_op_element, env))

#define WODEN_INTERFACE_OP_ELEMENT_ADD_STYLE_URI(interface_op_element, env, uri) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
        add_style_uri  (interface_op_element, env, uri))

#define WODEN_INTERFACE_OP_ELEMENT_REMOVE_STYLE_URI(interface_op_element, env, uri) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
        remove_style_uri  (interface_op_element, env, uri))

#define WODEN_INTERFACE_OP_ELEMENT_GET_STYLE(interface_op_element, env) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
        get_style  (interface_op_element, env))

#define WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_MSG_REF_ELEMENT(interface_op_element, env, msg_ref) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         add_interface_msg_ref_element (interface_op_element, env, msg_ref))

#define WODEN_INTERFACE_OP_ELEMENT_REMOVE_MSG_REF_ELEMENT(interface_op_element, env, msg_ref) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         remove_msg_ref_element (interface_op_element, env, msg_ref))

#define WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_MSG_REF_ELEMENTS(interface_op_element, env) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         get_interface_msg_ref_elements (interface_op_element, env))

#define WODEN_INTERFACE_OP_ELEMENT_ADD_INTERFACE_FAULT_REF_ELEMENT(interface_op_element, env, fault_ref) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         add_interface_fault_ref_element (interface_op_element, env, fault_ref))

#define WODEN_INTERFACE_OP_ELEMENT_REMOVE_FAULT_REF_ELEMENT(interface_op_element, env, fault_ref) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         remove_fault_ref_element (interface_op_element, env, fault_ref))

#define WODEN_INTERFACE_OP_ELEMENT_GET_INTERFACE_FAULT_REF_ELEMENTS(interface_op_element, env) \
      (((woden_interface_op_element_t *) interface_op_element)->ops->\
         get_interface_fault_ref_elements (interface_op_element, env))





/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INTERFACE_OP_ELEMENT_H */
