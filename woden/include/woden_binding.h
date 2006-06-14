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

#ifndef WODEN_BINDING_H
#define WODEN_BINDING_H

/**
 * @file woden_binding.h
 * @brief Axis2 Binding Interface
 * This class represents the Binding component from the WSDL 2.0 Component Model 
 * and the WSDL &lt;binding&gt; element.
 * 
 */

#include <woden.h>
#include <woden_binding_element.h>
#include <woden_configurable.h>

/** @defgroup woden_binding Binding
  * @ingroup axis2_wsdl
  * @{
  */

typedef union woden_binding_base woden_binding_base_t;
typedef struct woden_binding woden_binding_t;
typedef struct woden_binding_ops woden_binding_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_binding_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *binding,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *binding,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *binding,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *binding,
            const axis2_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *binding,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_interface) (
            void *binding,
            const axis2_env_t *env);

    axis2_uri_t *(AXIS2_CALL *
    get_type) (
            void *binding,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_binding_faults) (
            void *binding,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_binding_ops) (
            void *binding,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *binding,
            const axis2_env_t *env);
    /**************************************************************************
     *                      Non-API implementation methods
     **************************************************************************/
    /* 
     * Get the binding fault with the specified 'ref' attribute qname.
     * 
     * TODO decide if this type of qname-based accessor is needed, either internally or on API.
     *  
     * Note that for this type of key-based access, the choice of List rather than Map makes
     * the implementation more complicated. However, the advantage of List is that it avoids the
     * null key problem that arises when the binding fault is missing its 'ref' attribute qname.
     */
    void *(AXIS2_CALL *
    get_binding_fault_element) (
            void *binding,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    /*
     * Get the binding operation with the specified 'ref' attribute qname.
     * 
     * TODO decide if this type of qname-based accessor is needed, either internally or on API.
     * 
     * Note that for this type of key-based access, the choice of List rather than Map makes
     * the implementation more complicated. However, the advantage of List is that it avoids the
     * null key problem that arises when the binding operation is missing its 'ref' attribute qname.
     */
    void *(AXIS2_CALL *
    get_binding_op_element) (
            void *binding,
            const axis2_env_t *env,
            axis2_qname_t *qname);

    axis2_status_t (AXIS2_CALL *
    set_interface_element) (
            void *binding,
            const axis2_env_t *env,
            void *interface);
};

union woden_binding_base
{
    woden_configurable_t configurable;
    woden_binding_element_t binding_element;
};

struct woden_binding
{
    woden_binding_base_t base;
    woden_binding_ops_t *ops;
};

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_binding_element(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_configurable_element(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_documentable_element(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_documentable(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_configurable(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_wsdl_obj(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_nested_element(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_wsdl_component(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_attr_extensible(
        void *binding,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_element_extensible(
        void *binding,
        const axis2_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_binding_resolve_methods(
        woden_binding_t *binding,
        const axis2_env_t *env,
        woden_binding_t *binding_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_BINDING_FREE(binding, env) \
      (((woden_binding_t *) binding)->ops->free(binding, env))

#define WODEN_BINDING_SUPER_OBJS(binding, env) \
      (((woden_binding_t *) binding)->ops->super_objs(binding, env))

#define WODEN_BINDING_TYPE(binding, env) \
      (((woden_binding_t *) binding)->ops->type(binding, env))

#define WODEN_BINDING_GET_BASE_IMPL(binding, env) \
      (((woden_binding_t *) binding)->ops->get_base_impl(binding, env))

#define WODEN_BINDING_GET_QNAME(binding, env) \
      (((woden_binding_t *) binding)->\
         get_qname(binding, env))

#define WODEN_BINDING_GET_INTERFACE(binding, env) \
      (((woden_binding_t *) binding)->\
         get_interface(binding, env))

#define WODEN_BINDING_GET_TYPE(binding, env) \
      (((woden_binding_t *) binding)->ops->\
         get_type(binding, env))

#define WODEN_BINDING_GET_BINDING_FAULTS(binding, env, qname) \
      (((woden_binding_t *) binding)->ops->\
         get_binding_faults(binding, env, qname))

#define WODEN_BINDING_GET_BINDING_OPS(binding, env) \
      (((woden_binding_t *) binding)->ops->\
         get_binding_ops(binding, env))

#define WODEN_BINDING_TO_ELEMENT(binding, env) \
      (((woden_binding_t *) binding)->ops->\
         to_element(binding, env))

#define WODEN_BINDING_GET_BINDING_FAULT_ELEMENT(binding, env, qname) \
      (((woden_binding_t *) binding)->ops->\
       get_binding_fault_element  (binding, env, qname))

#define WODEN_BINDING_GET_BINDING_OP_ELEMENT(binding, env, qname) \
      (((woden_binding_t *) binding)->ops->\
        get_binding_op_element (binding, env, qname))

#define WODEN_BINDING_SET_INTERFACE_ELEMENT(binding, env, interface) \
      (((woden_binding_t *) binding)->ops->\
        set_interface_element (binding, env, interface))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_BINDING_H */
