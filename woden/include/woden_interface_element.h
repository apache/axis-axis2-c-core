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

#ifndef WODEN_INTERFACE_ELEMENT_H
#define WODEN_INTERFACE_ELEMENT_H

/**
 * @file woden_interface_element.h
 * @brief Axis2 Interface Element Interface
 * This interface represents a &lt;interface&gt; XML element 
 * information item. It declares the behaviour required to support 
 * parsing, creating and manipulating a &lt;interface&gt; element.
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axutil_array_list.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_configurable_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_interface_element woden_interface_element_t;
typedef struct woden_interface_element_ops woden_interface_element_ops_t;
typedef union woden_interface_element_base woden_interface_element_base_t;

/** @defgroup woden_interface_element Interface Element
  * @ingroup axis2_interface
  * @{
  */

struct woden_interface_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *interface_element,
            const axutil_env_t *env);
 
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *interface_element,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *interface_element,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *interface_element,
            const axutil_env_t *env); 
 
    axis2_status_t (AXIS2_CALL *
    add_extends_qname) (
            void *interface_element,
            const axutil_env_t *env,
            axis2_qname_t *qname);

    axutil_array_list_t *(AXIS2_CALL *
    get_extends_qnames) (
            void *interface_element,
            const axutil_env_t *env); 
  
    axis2_status_t (AXIS2_CALL *
    add_style_default_uri) (
            void *interface_element,
            const axutil_env_t *env,
            axis2_uri_t *uri);

    axis2_uri_t *(AXIS2_CALL *
    get_style_default) (
            void *interface_element,
            const axutil_env_t *env); 
  
    axis2_status_t (AXIS2_CALL *
    add_interface_fault_element) (
            void *interface_element,
            const axutil_env_t *env,
            void *fault);

    axutil_array_list_t *(AXIS2_CALL *
    get_interface_fault_elements) (
            void *interface_element,
            const axutil_env_t *env);
  
    axis2_status_t (AXIS2_CALL *
    add_interface_op_element) (
            void *interface_element,
            const axutil_env_t *env,
            void *op);

    axutil_array_list_t *(AXIS2_CALL *
    get_interface_op_elements) (
            void *interface_element,
            const axutil_env_t *env);

};

union woden_interface_element_base
{
    woden_documentable_element_t documentable_element;
    woden_configurable_element_t configurable_element;
};

struct woden_interface_element
{
    woden_interface_element_base_t base;
    woden_interface_element_ops_t *ops;
};

AXIS2_EXTERN woden_interface_element_t * AXIS2_CALL
woden_interface_element_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_interface_element_resolve_methods(
        woden_interface_element_t *interface_element,
        const axutil_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INTERFACE_ELEMENT_FREE(interface_element, env) \
      (((woden_interface_element_t *) interface_element)->ops->\
         free (interface_element, env))

#define WODEN_INTERFACE_ELEMENT_TYPE(interface_element, env) \
      (((woden_interface_element_t *) interface_element)->ops->\
         type (interface_element, env))

#define WODEN_INTERFACE_ELEMENT_SET_QNAME(interface_element, env, qname) \
      (((woden_interface_element_t *) interface_element)->ops->\
         set_qname (interface_element, env, qname))

#define WODEN_INTERFACE_ELEMENT_GET_QNAME(interface_element, env) \
      (((woden_interface_element_t *) interface_element)->ops->\
        get_qname  (interface_element, env))

#define WODEN_INTERFACE_ELEMENT_ADD_EXTENDS_QNAME(interface_element, env, qname) \
      (((woden_interface_element_t *) interface_element)->ops->\
         add_extends_qname (interface_element, env, qname))

#define WODEN_INTERFACE_ELEMENT_GET_EXTENDS_QNAMES(interface_element, env) \
      (((woden_interface_element_t *) interface_element)->ops->\
         get_extends_qnames (interface_element, env))

#define WODEN_INTERFACE_ELEMENT_ADD_STYLE_DEFAULT_URI(interface_element, env, uri) \
      (((woden_interface_element_t *) interface_element)->ops->\
         add_style_default_uri (interface_element, env, uri))

#define WODEN_INTERFACE_ELEMENT_GET_STYLE_DEFAULT(interface_element, env) \
      (((woden_interface_element_t *) interface_element)->ops->\
        get_style_default  (interface_element, env))

#define WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_FAULT_ELEMENT(interface_element, env, fault) \
      (((woden_interface_element_t *) interface_element)->ops->\
        add_interface_fault_element  (interface_element, env, fault))

#define WODEN_INTERFACE_ELEMENT_GET_INTERFACE_FAULT_ELEMENTS(interface_element, env) \
      (((woden_interface_element_t *) interface_element)->ops->\
        get_interface_fault_elements  (interface_element, env))

#define WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_OP_ELEMENT(interface_element, env, op) \
      (((woden_interface_element_t *) interface_element)->ops->\
         add_interface_op_element (interface_element, env, op))

#define WODEN_INTERFACE_ELEMENT_GET_INTERFACE_OP_ELEMENTS(interface_element, env) \
      (((woden_interface_element_t *) interface_element)->ops->\
         get_interface_op_elements (interface_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INTERFACE_ELEMENT_H */
