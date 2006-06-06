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

#ifndef AXIS2_WODEN_INTERFACE_FAULT_H
#define AXIS2_WODEN_INTERFACE_FAULT_H

/**
 * @file axis2_woden_interface_fault.h
 * @brief Axis2 Interface Fault Interface Fault
 * This class represents the InterfaceFault component from the WSDL 2.0 Component 
 * Model and the &lt;fault&gt; child element of the &lt;interface&gt; element. 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_interface_fault_element.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup axis2_woden_interface_fault Interface Fault
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_interface_fault_base axis2_woden_interface_fault_base_t;
typedef struct axis2_woden_interface_fault axis2_woden_interface_fault_t;
typedef struct axis2_woden_interface_fault_ops axis2_woden_interface_fault_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_interface_fault_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *interface_fault,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *interface_fault,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *interface_fault,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *interface_fault,
            const axis2_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *interface_fault,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_element_declaration) (
            void *interface_fault,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *interface_fault,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_element_declaration) (
            void *interface_fault,
            const axis2_env_t *env,
            void *element_decl);

    axis2_status_t (AXIS2_CALL *
    set_types) (
            void *interface_fault,
            const axis2_env_t *env,
            void *types);


};

union axis2_woden_interface_fault_base
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_interface_fault_element_t interface_fault_element;
};

struct axis2_woden_interface_fault
{
    axis2_woden_interface_fault_base_t base;
    axis2_woden_interface_fault_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_interface_fault_t * AXIS2_CALL
axis2_woden_interface_fault_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN axis2_woden_interface_fault_t * AXIS2_CALL
axis2_woden_interface_fault_to_interface_fault_element(
        void *interface_fault,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_fault_t * AXIS2_CALL
axis2_woden_interface_fault_to_nested_element(
        void *interface_fault,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_fault_t * AXIS2_CALL
axis2_woden_interface_fault_to_nested_configurable(
        void *interface_fault,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_fault_t * AXIS2_CALL
axis2_woden_interface_fault_to_configurable(
        void *interface_fault,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_resolve_methods(
        axis2_woden_interface_fault_t *interface_fault,
        const axis2_env_t *env,
        axis2_woden_interface_fault_t *interface_fault_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INTERFACE_FAULT_FREE(interface_fault, env) \
      (((axis2_woden_interface_fault_t *) interface_fault)->ops->free(interface_fault, env))

#define AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env) \
      (((axis2_woden_interface_fault_t *) interface_fault)->ops->super_objs(interface_fault, env))

#define AXIS2_WODEN_INTERFACE_FAULT_TYPE(interface_fault, env) \
      (((axis2_woden_interface_fault_t *) interface_fault)->ops->type(interface_fault, env))

#define AXIS2_WODEN_INTERFACE_FAULT_GET_BASE_IMPL(interface_fault, env) \
      (((axis2_woden_interface_fault_t *) interface_fault)->ops->get_base_impl(interface_fault, env))

#define AXIS2_WODEN_INTERFACE_FAULT_GET_QNAME(interface_fault, env) \
      (((axis2_woden_interface_fault_t *) interface_fault)->\
         get_qname(interface_fault, env))

#define AXIS2_WODEN_INTERFACE_FAULT_GET_ELEMENT_DECLARATION(interface_fault, env) \
      (((axis2_woden_interface_fault_t *) interface_fault)->\
         get_element_declaration(interface_fault, env))

#define AXIS2_WODEN_INTERFACE_FAULT_TO_ELEMENT(interface_fault, env) \
      (((axis2_woden_interface_fault_t *) interface_fault)->ops->\
         to_element(interface_fault, env))

#define AXIS2_WODEN_INTERFACE_FAULT_SET_ELEMENT_DECLARATION(interface_fault, env, element_decl) \
		(((axis2_woden_interface_fault_t *) interface_fault)->ops->\
        set_element_declaration (interface_fault, env, element_decl))

#define AXIS2_WODEN_INTERFACE_FAULT_SET_TYPES(interface_fault, env, types) \
		(((axis2_woden_interface_fault_t *) interface_fault)->ops->\
        set_types (interface_fault, env, types))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INTERFACE_FAULT_H */
