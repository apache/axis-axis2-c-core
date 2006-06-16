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

#ifndef WODEN_INTERFACE_H
#define WODEN_INTERFACE_H

/**
 * @file woden_interface.h
 * @brief Axis2 Interface Interface
 * Represents the Interface component from the WSDL 2.0 Component model.
 * This component provides a read-only, abstract view of the WSDL 
 * interface, including any interface information defined within
 * imported or included WSDL documents. 
 * 
 */

#include <woden.h>
#include <woden_interface_element.h>
#include <woden_configurable.h>

/** @defgroup woden_interface Interface
  * @ingroup axis2_wsdl
  * @{
  */

typedef union woden_interface_base woden_interface_base_t;
typedef struct woden_interface woden_interface_t;
typedef struct woden_interface_ops woden_interface_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_interface_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *woden_interface,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *woden_interface,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *woden_interface,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *woden_interface,
            const axis2_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *woden_interface,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_extended_interfaces) (
            void *woden_interface,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_faults) (
            void *woden_interface,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_interface_fault) (
            void *woden_interface,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_ops) (
            void *woden_interface,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *woden_interface,
            const axis2_env_t *env);

};

union woden_interface_base
{
    woden_configurable_t configurable;
    woden_interface_element_t interface_element;
};

struct woden_interface
{
    woden_interface_base_t base;
    woden_interface_ops_t *ops;
};

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_interface_element(
        void *woden_interface,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_documentable(
        void *woden_interface,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_configurable(
        void *woden_interface,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_attr_extensible(
        void *woden_interface,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_t * AXIS2_CALL
woden_interface_to_element_extensible(
        void *woden_interface,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_interface_resolve_methods(
        woden_interface_t *woden_interface,
        const axis2_env_t *env,
        woden_interface_t *interface_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INTERFACE_FREE(interface, env) \
      (((woden_interface_t *) interface)->ops->free(interface, env))

#define WODEN_INTERFACE_SUPER_OBJS(interface, env) \
      (((woden_interface_t *) interface)->ops->super_objs(interface, env))

#define WODEN_INTERFACE_TYPE(interface, env) \
      (((woden_interface_t *) interface)->ops->type(interface, env))

#define WODEN_INTERFACE_GET_BASE_IMPL(interface, env) \
      (((woden_interface_t *) interface)->ops->get_base_impl(interface, env))

#define WODEN_INTERFACE_GET_QNAME(interface, env) \
      (((woden_interface_t *) interface)->ops->\
         get_qname(interface, env))

#define WODEN_INTERFACE_GET_EXTENDED_INTERFACES(interface, env) \
      (((woden_interface_t *) interface)->ops->\
         get_extended_interfaces(interface, env))

#define WODEN_INTERFACE_GET_INTERFACE_FAULTS(interface, env) \
      (((woden_interface_t *) interface)->ops->\
         get_interface_faults(interface, env))

#define WODEN_INTERFACE_GET_INTERFACE_FAULT(interface, env, qname) \
      (((woden_interface_t *) interface)->ops->\
         get_interface_fault(interface, env, qname))

#define WODEN_INTERFACE_GET_INTERFACE_OPS(interface, env) \
      (((woden_interface_t *) interface)->ops->\
         get_interface_ops(interface, env))

#define WODEN_INTERFACE_TO_ELEMENT(interface, env) \
      (((woden_interface_t *) interface)->ops->\
         to_element(interface, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INTERFACE_H */
