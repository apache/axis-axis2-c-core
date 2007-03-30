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

#ifndef WODEN_INTERFACE_FAULT_H
#define WODEN_INTERFACE_FAULT_H

/**
 * @file woden_interface_fault.h
 * @brief Axis2 Interface Fault Interface Fault
 * This class represents the InterfaceFault component from the WSDL 2.0 Component 
 * Model and the &lt;fault&gt; child element of the &lt;interface&gt; element. 
 * 
 */

#include <woden.h>
#include <woden_interface_fault_element.h>
#include <woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup woden_interface_fault Interface Fault
  * @ingroup woden
  * @{
  */

typedef union woden_interface_fault_base woden_interface_fault_base_t;
typedef struct woden_interface_fault woden_interface_fault_t;
typedef struct woden_interface_fault_ops woden_interface_fault_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_interface_fault_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *interface_fault,
            const axutil_env_t *env);
    
    axutil_hash_t *(AXIS2_CALL *
    super_objs) (void *interface_fault,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *interface_fault,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *interface_fault,
            const axutil_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *interface_fault,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_element_decl) (
            void *interface_fault,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *interface_fault,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_element_decl) (
            void *interface_fault,
            const axutil_env_t *env,
            void *element_decl);

    axis2_status_t (AXIS2_CALL *
    set_types) (
            void *interface_fault,
            const axutil_env_t *env,
            void *types);
};

union woden_interface_fault_base
{
    woden_nested_configurable_t nested_configurable;
    woden_interface_fault_element_t interface_fault_element;
};

struct woden_interface_fault
{
    woden_interface_fault_base_t base;
    woden_interface_fault_ops_t *ops;
};

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_interface_fault_element(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_nested_configurable(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_configurable(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_configurable_element(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_documentable_element(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_documentable(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_attr_extensible(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_element_extensible(
        void *interface_fault,
        const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_interface_fault_resolve_methods(
        woden_interface_fault_t *interface_fault,
        const axutil_env_t *env,
        woden_interface_fault_t *interface_fault_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INTERFACE_FAULT_FREE(interface_fault, env) \
      (((woden_interface_fault_t *) interface_fault)->ops->free(interface_fault, env))

#define WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env) \
      (((woden_interface_fault_t *) interface_fault)->ops->super_objs(interface_fault, env))

#define WODEN_INTERFACE_FAULT_TYPE(interface_fault, env) \
      (((woden_interface_fault_t *) interface_fault)->ops->type(interface_fault, env))

#define WODEN_INTERFACE_FAULT_GET_BASE_IMPL(interface_fault, env) \
      (((woden_interface_fault_t *) interface_fault)->ops->get_base_impl(interface_fault, env))

#define WODEN_INTERFACE_FAULT_GET_QNAME(interface_fault, env) \
      (((woden_interface_fault_t *) interface_fault)->ops->\
         get_qname(interface_fault, env))

#define WODEN_INTERFACE_FAULT_GET_ELEMENT_DECLARATION(interface_fault, env) \
      (((woden_interface_fault_t *) interface_fault)->\
         get_element_declaration(interface_fault, env))

#define WODEN_INTERFACE_FAULT_TO_ELEMENT(interface_fault, env) \
      (((woden_interface_fault_t *) interface_fault)->ops->\
         to_element(interface_fault, env))

#define WODEN_INTERFACE_FAULT_SET_ELEMENT_DECL(interface_fault, env, element_decl) \
      (((woden_interface_fault_t *) interface_fault)->ops->\
        set_element_decl (interface_fault, env, element_decl))

#define WODEN_INTERFACE_FAULT_SET_TYPES(interface_fault, env, types) \
      (((woden_interface_fault_t *) interface_fault)->ops->\
        set_types (interface_fault, env, types))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INTERFACE_FAULT_H */
