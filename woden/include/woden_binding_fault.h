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

#ifndef WODEN_BINDING_FAULT_H
#define WODEN_BINDING_FAULT_H

/**
 * @file woden_binding_fault.h
 * @brief Axis2 Binding Fault Binding Fault
 * This class represents the Binding Fault component from the WSDL 2.0 Component Model 
 * and the &lt;fault&gt; child element of the WSDL &lt;binding&gt; element.
 * 
 */

#include <woden.h>
#include <woden_binding_fault_element.h>
#include <woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup woden_binding_fault Binding Fault
  * @ingroup woden
  * @{
  */

typedef union woden_binding_fault_base woden_binding_fault_base_t;
typedef struct woden_binding_fault woden_binding_fault_t;
typedef struct woden_binding_fault_ops woden_binding_fault_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_binding_fault_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *binding_fault,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *binding_fault,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *binding_fault,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *binding_fault,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_interface_fault) (
            void *binding_fault,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *binding_fault,
            const axis2_env_t *env);

    /**************************************************************************
     *                      Non-API implementation methods
     **************************************************************************/
    axis2_status_t (AXIS2_CALL *
    set_interface_fault_element) (
            void *binding_fault,
            const axis2_env_t *env,
            void *in_fault);
};

union woden_binding_fault_base
{
    woden_nested_configurable_t nested_configurable;
    woden_binding_fault_element_t binding_fault_element;
};

struct woden_binding_fault
{
    woden_binding_fault_base_t base;
    woden_binding_fault_ops_t *ops;
};

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_binding_fault_element(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_nested_configurable(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_configurable(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_wsdl_obj(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_documentable_element(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_documentable(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_nested_element(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_wsdl_component(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_configurable_element(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_attr_extensible(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_element_extensible(
        void *binding_fault,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_binding_fault_resolve_methods(
        woden_binding_fault_t *binding_fault,
        const axis2_env_t *env,
        woden_binding_fault_t *binding_fault_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_BINDING_FAULT_FREE(binding_fault, env) \
      (((woden_binding_fault_t *) binding_fault)->ops->free(binding_fault, env))

#define WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env) \
      (((woden_binding_fault_t *) binding_fault)->ops->super_objs(binding_fault, env))

#define WODEN_BINDING_FAULT_TYPE(binding_fault, env) \
      (((woden_binding_fault_t *) binding_fault)->ops->type(binding_fault, env))

#define WODEN_BINDING_FAULT_GET_BASE_IMPL(binding_fault, env) \
      (((woden_binding_fault_t *) binding_fault)->ops->get_base_impl(binding_fault, env))

#define WODEN_BINDING_FAULT_GET_INTERFACE_FAULT(binding_fault, env) \
      (((woden_binding_fault_t *) binding_fault)->ops->\
         get_interface_fault(binding_fault, env))

#define WODEN_BINDING_FAULT_TO_ELEMENT(binding_fault, env) \
      (((woden_binding_fault_t *) binding_fault)->ops->\
         to_element(binding_fault, env))

#define WODEN_BINDING_FAULT_SET_INTERFACE_FAULT_ELEMENT(binding_fault, env, in_fault) \
      (((woden_binding_fault_t *) binding_fault)->ops->\
         set_interface_fault_element(binding_fault, env, in_fault))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_BINDING_FAULT_H */
