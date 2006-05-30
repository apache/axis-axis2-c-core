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

#ifndef AXIS2_WODEN_BINDING_FAULT_H
#define AXIS2_WODEN_BINDING_FAULT_H

/**
 * @file axis2_woden_binding_fault.h
 * @brief Axis2 Binding Fault Binding Fault
 * This class represents the Binding Fault component from the WSDL 2.0 Component Model 
 * and the &lt;fault&gt; child element of the WSDL &lt;binding&gt; element.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_binding_fault_element.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup axis2_woden_binding_fault Binding Fault
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_binding_fault_base axis2_woden_binding_fault_base_t;
typedef struct axis2_woden_binding_fault axis2_woden_binding_fault_t;
typedef struct axis2_woden_binding_fault_ops axis2_woden_binding_fault_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_binding_fault_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *binding_fault,
            axis2_env_t **env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *binding_fault,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *binding_fault,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *binding_fault,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_interface_fault) (
            void *binding_fault,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    to_element)  (
            void *binding_fault,
            axis2_env_t **env);

    /**************************************************************************
     *                      Non-API implementation methods
     **************************************************************************/
    axis2_status_t (AXIS2_CALL *
    set_interface_fault_element) (
            void *binding_fault,
            axis2_env_t **env,
            void *in_fault);
};

union axis2_woden_binding_fault_base
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_binding_fault_element_t binding_fault_element;
};

struct axis2_woden_binding_fault
{
    axis2_woden_binding_fault_base_t base;
    axis2_woden_binding_fault_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_binding_fault_element(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_nested_configurable(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_configurable(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_wsdl_obj(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_nested_element(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_wsdl_component(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_attr_extensible(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_fault_t *)
axis2_woden_binding_fault_to_element_extensible(
        void *binding_fault,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_binding_fault_resolve_methods(
        axis2_woden_binding_fault_t *binding_fault,
        axis2_env_t **env,
        axis2_woden_binding_fault_t *binding_fault_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_BINDING_FAULT_FREE(binding_fault, env) \
		(((axis2_woden_binding_fault_t *) binding_fault)->ops->free(binding_fault, env))

#define AXIS2_WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env) \
		(((axis2_woden_binding_fault_t *) binding_fault)->ops->super_objs(binding_fault, env))

#define AXIS2_WODEN_BINDING_FAULT_TYPE(binding_fault, env) \
		(((axis2_woden_binding_fault_t *) binding_fault)->ops->type(binding_fault, env))

#define AXIS2_WODEN_BINDING_FAULT_GET_BASE_IMPL(binding_fault, env) \
		(((axis2_woden_binding_fault_t *) binding_fault)->ops->get_base_impl(binding_fault, env))

#define AXIS2_WODEN_BINDING_FAULT_GET_INTERFACE_FAULT(binding_fault, env) \
		(((axis2_woden_binding_fault_t *) binding_fault)->\
         get_interface_fault(binding_fault, env))

#define AXIS2_WODEN_BINDING_FAULT_TO_ELEMENT(binding_fault, env) \
		(((axis2_woden_binding_fault_t *) binding_fault)->ops->\
         to_element(binding_fault, env))

#define AXIS2_WODEN_BINDING_FAULT_SET_INTERFACE_FAULT_ELEMENT(binding_fault, env, in_fault) \
		(((axis2_woden_binding_fault_t *) binding_fault)->\
         set_interface_fault_elment(binding_fault, env, in_fault))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_BINDING_FAULT_H */
