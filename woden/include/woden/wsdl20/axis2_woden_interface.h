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

#ifndef AXIS2_WODEN_INTERFACE_H
#define AXIS2_WODEN_INTERFACE_H

/**
 * @file axis2_woden_interface.h
 * @brief Axis2 Interface Interface
 * Represents the Interface component from the WSDL 2.0 Component model.
 * This component provides a read-only, abstract view of the WSDL 
 * interface, including any interface information defined within
 * imported or included WSDL documents. 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_interface_element.h>
#include <woden/wsdl20/axis2_woden_configurable.h>

/** @defgroup axis2_woden_interface Interface
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_interface_base axis2_woden_interface_base_t;
typedef struct axis2_woden_interface axis2_woden_interface_t;
typedef struct axis2_woden_interface_ops axis2_woden_interface_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_interface_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *interface,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *interface,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *interface,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *interface,
            const axis2_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *interface,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_extended_interfaces) (
            void *interface,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_faults) (
            void *interface,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_interface_fault) (
            void *interface,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_ops) (
            void *interface,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *interface,
            const axis2_env_t *env);

};

union axis2_woden_interface_base
{
    axis2_woden_configurable_t configurable;
    axis2_woden_interface_element_t interface_element;
};

struct axis2_woden_interface
{
    axis2_woden_interface_base_t base;
    axis2_woden_interface_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_interface_t * AXIS2_CALL
axis2_woden_interface_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN axis2_woden_interface_t * AXIS2_CALL
axis2_woden_interface_to_interface_element(
        void *interface,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_t * AXIS2_CALL
axis2_woden_interface_to_configurable_element(
        void *interface,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_t * AXIS2_CALL
axis2_woden_interface_to_documentable_element(
        void *interface,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_t * AXIS2_CALL
axis2_woden_interface_to_configurable(
        void *interface,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_interface_resolve_methods(
        axis2_woden_interface_t *interface,
        const axis2_env_t *env,
        axis2_woden_interface_t *interface_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INTERFACE_FREE(interface, env) \
		(((axis2_woden_interface_t *) interface)->ops->free(interface, env))

#define AXIS2_WODEN_INTERFACE_SUPER_OBJS(interface, env) \
		(((axis2_woden_interface_t *) interface)->ops->super_objs(interface, env))

#define AXIS2_WODEN_INTERFACE_TYPE(interface, env) \
		(((axis2_woden_interface_t *) interface)->ops->type(interface, env))

#define AXIS2_WODEN_INTERFACE_GET_BASE_IMPL(interface, env) \
		(((axis2_woden_interface_t *) interface)->ops->get_base_impl(interface, env))

#define AXIS2_WODEN_INTERFACE_GET_QNAME(interface, env) \
		(((axis2_woden_interface_t *) interface)->\
         get_qname(interface, env))

#define AXIS2_WODEN_INTERFACE_GET_EXTENDED_INTERFACES(interface, env) \
		(((axis2_woden_interface_t *) interface)->\
         get_extended_interfaces(interface, env))

#define AXIS2_WODEN_INTERFACE_GET_INTERFACE_FAULTS(interface, env) \
		(((axis2_woden_interface_t *) interface)->ops->\
         get_interface_faults(interface, env))

#define AXIS2_WODEN_INTERFACE_GET_INTERFACE_FAULT(interface, env, qname) \
		(((axis2_woden_interface_t *) interface)->ops->\
         get_interface_fault(interface, env, qname))

#define AXIS2_WODEN_INTERFACE_GET_INTERFACE_OPS(interface, env) \
		(((axis2_woden_interface_t *) interface)->ops->\
         get_interface_ops(interface, env))

#define AXIS2_WODEN_INTERFACE_TO_ELEMENT(interface, env) \
		(((axis2_woden_interface_t *) interface)->ops->\
         to_element(interface, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INTERFACE_H */
