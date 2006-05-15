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

#ifndef AXIS2_WODEN_INTERFACE_ELEMENT_H
#define AXIS2_WODEN_INTERFACE_ELEMENT_H

/**
 * @file axis2_woden_interface_element.h
 * @brief Axis2 Interface Element Interface
 * This interface represents a &lt;interface&gt; XML element 
 * information item. It declares the behaviour required to support 
 * parsing, creating and manipulating a &lt;interface&gt; element.
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

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_interface_element axis2_woden_interface_element_t;
typedef struct axis2_woden_interface_element_ops axis2_woden_interface_element_ops_t;
typedef union axis2_woden_interface_element_base axis2_woden_interface_element_base_t;

/** @defgroup axis2_woden_interface_element Interface Element
  * @ingroup axis2_interface
  * @{
  */

struct axis2_woden_interface_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *interface_element,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    to_interface_element_free) (
            void *interface_element,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *interface_element,
            axis2_env_t **env);
 
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *interface_element,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_qname) (
            void *interface_element,
            axis2_env_t **env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *interface_element,
            axis2_env_t **env); 
 
    axis2_status_t (AXIS2_CALL *
    add_extends_qname) (
            void *interface_element,
            axis2_env_t **env,
            axis2_qname_t *qname);

    axis2_array_list_t *(AXIS2_CALL *
    get_extends_qnames) (
            void *interface_element,
            axis2_env_t **env); 
  
    axis2_status_t (AXIS2_CALL *
    add_style_default_uri) (
            void *interface_element,
            axis2_env_t **env,
            axis2_url_t *uri);

    axis2_url_t *(AXIS2_CALL *
    get_style_default) (
            void *interface_element,
            axis2_env_t **env); 
  
    axis2_status_t (AXIS2_CALL *
    add_interface_fault_element) (
            void *interface_element,
            axis2_env_t **env,
            void *fault);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_fault_elements) (
            void *interface_element,
            axis2_env_t **env);
  
    axis2_status_t (AXIS2_CALL *
    add_interface_op_element) (
            void *interface_element,
            axis2_env_t **env,
            void *op);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_op_elements) (
            void *interface_element,
            axis2_env_t **env);

};

union axis2_woden_interface_element_base
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_configurable_element_t configurable_element;
};

struct axis2_woden_interface_element
{
    axis2_woden_interface_element_base_t base;
    axis2_woden_interface_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_interface_element_t *)
axis2_woden_interface_element_create(
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_status_t)
axis2_woden_interface_element_resolve_methods(
        axis2_woden_interface_element_t *interface_element,
        axis2_env_t **env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INTERFACE_ELEMENT_FREE(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         free (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_TO_INTERFACE_ELEMENT_FREE(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         to_interface_element_free (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_SUPER_OBJS(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         super_objs (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_TYPE(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         type (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_SET_QNAME(interface_element, env, qname) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         set_qname (interface_element, env, qname))

#define AXIS2_WODEN_INTERFACE_ELEMENT_GET_QNAME(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
        get_qname  (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_ADD_EXTENDS_QNAME(interface_element, env, qname) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         add_extends_qname (interface_element, env, qname))

#define AXIS2_WODEN_INTERFACE_ELEMENT_GET_EXTENDS_QNAMES(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         get_extends_qnames (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_ADD_STYLE_DEFAULT_URI(interface_element, env, uri) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         add_style_default_uri (interface_element, env, uri))

#define AXIS2_WODEN_INTERFACE_ELEMENT_GET_STYLE_DEFAULT(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
        get_style_default  (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_FAULT_ELEMENT(interface_element, env, fault) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
        add_interface_fault_element  (interface_element, env, fault))

#define AXIS2_WODEN_INTERFACE_ELEMENT_GET_INTERFACE_FAULT_ELEMENTS(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
        get_interface_fault_elements  (interface_element, env))

#define AXIS2_WODEN_INTERFACE_ELEMENT_ADD_INTERFACE_OP_ELEMENT(interface_element, env, op) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         add_interface_op_element (interface_element, env, op))

#define AXIS2_WODEN_INTERFACE_ELEMENT_GET_INTERFACE_OP_ELEMENTS(interface_element, env) \
		(((axis2_woden_interface_element_t *) interface_element)->ops->\
         get_interface_op_elements (interface_element, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INTERFACE_ELEMENT_H */
