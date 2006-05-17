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

#ifndef AXIS2_WODEN_BINDING_MSG_REF_H
#define AXIS2_WODEN_BINDING_MSG_REF_H

/**
 * @file axis2_woden_binding_msg_ref.h
 * @brief Axis2 Binding Message Reference Binding Message Reference
 * This class represents the Binding Message Reference component of the 
 * WSDL 2.0 Component model and the &lt;input&gt; and &lt;output&gt; 
 * child elements of a WSDL binding &lt;operation&gt;. 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_binding_msg_ref_element.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup axis2_woden_binding_msg_ref Binding Message Reference
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_binding_msg_ref_base axis2_woden_binding_msg_ref_base_t;
typedef struct axis2_woden_binding_msg_ref axis2_woden_binding_msg_ref_t;
typedef struct axis2_woden_binding_msg_ref_ops axis2_woden_binding_msg_ref_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_binding_msg_ref_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *binding_msg_ref,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    to_binding_msg_ref_free) (void *binding_msg_ref,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *binding_msg_ref,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *binding_msg_ref,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *binding_msg_ref,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_interface_msg_ref) (
            void *binding_msg_ref,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    to_element)  (
            void *binding_msg_ref,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_interface_msg_ref_element) (
            void *binding_msg_ref,
            axis2_env_t **env,
            void *int_msg_ref);

};

union axis2_woden_binding_msg_ref_base
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_binding_msg_ref_element_t binding_msg_ref_element;
};

struct axis2_woden_binding_msg_ref
{
    axis2_woden_binding_msg_ref_base_t base;
    axis2_woden_binding_msg_ref_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_binding_msg_ref_element(
        void *binding_msg_ref,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_nested_configurable(
        void *binding_msg_ref,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_configurable(
        void *binding_msg_ref,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_binding_msg_ref_resolve_methods(
        axis2_woden_binding_msg_ref_t *binding_msg_ref,
        axis2_env_t **env,
        axis2_woden_binding_msg_ref_t *binding_msg_ref_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_BINDING_MSG_REF_FREE(binding_msg_ref, env) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->ops->free(binding_msg_ref, env))

#define AXIS2_WODEN_BINDING_MSG_REF_TO_BINDING_MSG_REF_FREE(binding_msg_ref, env) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->ops->to_binding_msg_ref_free(binding_msg_ref, env))

#define AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->ops->super_objs(binding_msg_ref, env))

#define AXIS2_WODEN_BINDING_MSG_REF_TYPE(binding_msg_ref, env) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->ops->type(binding_msg_ref, env))

#define AXIS2_WODEN_BINDING_MSG_REF_GET_BASE_IMPL(binding_msg_ref, env) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->ops->get_base_impl(binding_msg_ref, env))

#define AXIS2_WODEN_BINDING_MSG_REF_GET_INTERFACE_MSG_REF(binding_msg_ref, env) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->\
         get_interface_msg_ref(binding_msg_ref, env))

#define AXIS2_WODEN_BINDING_MSG_REF_TO_ELEMENT(binding_msg_ref, env) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->ops->\
         to_element(binding_msg_ref, env))

#define AXIS2_WODEN_BINDING_MSG_REF_SET_INTERFACE_MSG_REF_ELEMENT(binding_msg_ref, env, int_msg_ref) \
		(((axis2_woden_binding_msg_ref_t *) binding_msg_ref)->\
         set_interface_msg_ref_element(binding_msg_ref, env, int_msg_ref))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_BINDING_MSG_REF_H */
