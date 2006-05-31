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

#ifndef AXIS2_WODEN_BINDING_FAULT_REF_H
#define AXIS2_WODEN_BINDING_FAULT_REF_H

/**
 * @file axis2_woden_binding_fault_ref.h
 * @brief Axis2 Binding Fault Reference Binding Fault Reference
 * This class represents the Binding Fault Reference component of the
 * WSDL 2.0 Component model and the &lt;infault&gt; or &lt;outfault&gt; 
 * child element of a WSDL binding &lt;operation&gt;.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_binding_fault_ref_element.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup axis2_woden_binding_fault_ref Binding Fault Reference
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_binding_fault_ref_base axis2_woden_binding_fault_ref_base_t;
typedef struct axis2_woden_binding_fault_ref axis2_woden_binding_fault_ref_t;
typedef struct axis2_woden_binding_fault_ref_ops axis2_woden_binding_fault_ref_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_binding_fault_ref_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *binding_fault_ref,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *binding_fault_ref,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *binding_fault_ref,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *binding_fault_ref,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_interface_fault_ref) (
            void *binding_fault_ref,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *binding_fault_ref,
            const axis2_env_t *env);

    /**************************************************************************
     *                      Non-API implementation methods
     **************************************************************************/
    axis2_status_t (AXIS2_CALL *
    set_interface_fault_ref_element)  (
            void *binding_fault_ref,
            const axis2_env_t *env,
            void *fault_ref);

    axis2_status_t (AXIS2_CALL *
    set_interface_fault_ref)  (
            void *binding_fault_ref,
            const axis2_env_t *env,
            void *fault_ref);

};

union axis2_woden_binding_fault_ref_base
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_binding_fault_ref_element_t binding_fault_ref_element;
};

struct axis2_woden_binding_fault_ref
{
    axis2_woden_binding_fault_ref_base_t base;
    axis2_woden_binding_fault_ref_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_binding_fault_ref_element(
        void *binding_fault_ref,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_nested_configurable(
        void *binding_fault_ref,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_configurable(
        void *binding_fault_ref,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_wsdl_obj(
        void *binding_fault_ref,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_nested_element(
        void *binding_fault_ref,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_wsdl_component(
        void *binding_fault_ref,
        const axis2_env_t *env);


AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_attr_extensible(
        void *binding_fault_ref,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_binding_fault_ref_t *)
axis2_woden_binding_fault_ref_to_element_extensible(
        void *binding_fault_ref,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_binding_fault_ref_resolve_methods(
        axis2_woden_binding_fault_ref_t *binding_fault_ref,
        const axis2_env_t *env,
        axis2_woden_binding_fault_ref_t *binding_fault_ref_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_BINDING_FAULT_REF_FREE(binding_fault_ref, env) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->ops->free(binding_fault_ref, env))

#define AXIS2_WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->ops->super_objs(binding_fault_ref, env))

#define AXIS2_WODEN_BINDING_FAULT_REF_TYPE(binding_fault_ref, env) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->ops->type(binding_fault_ref, env))

#define AXIS2_WODEN_BINDING_FAULT_REF_GET_BASE_IMPL(binding_fault_ref, env) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->ops->get_base_impl(binding_fault_ref, env))

#define AXIS2_WODEN_BINDING_FAULT_REF_GET_INTERFACE_FAULT_REF(binding_fault_ref, env) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->\
         get_interface_fault_ref(binding_fault_ref, env))

#define AXIS2_WODEN_BINDING_FAULT_REF_TO_ELEMENT(binding_fault_ref, env) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->ops->\
         to_element(binding_fault_ref, env))

#define AXIS2_WODEN_BINDING_FAULT_REF_SET_INTERFACE_FAULT_REF_ELEMENT(binding_fault_ref, env, fault_ref) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->ops->\
         set_interface_fault_ref_element(binding_fault_ref, env, fault_ref))

#define AXIS2_WODEN_BINDING_FAULT_REF_SET_INTERFACE_FAULT_REF(binding_fault_ref, env, fault_ref) \
		(((axis2_woden_binding_fault_ref_t *) binding_fault_ref)->ops->\
         set_interface_fault_ref(binding_fault_ref, env, fault_ref))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_BINDING_FAULT_REF_H */
