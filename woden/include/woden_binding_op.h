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

#ifndef WODEN_BINDING_OP_H
#define WODEN_BINDING_OP_H

/**
 * @file woden_binding_op.h
 * @brief Axis2 Binding Operation Binding Operation
 * This class represents the Binding Operation component from the WSDL 2.0 Component Model 
 * and the &lt;operation&gt; child element of the WSDL &lt;binding&gt; element.
 * 
 */

#include <woden.h>
#include <woden_binding_op_element.h>
#include <woden_nested_configurable.h>
#include <axutil_qname.h>

/** @defgroup woden_binding_op Binding Operation
  * @ingroup woden
  * @{
  */

typedef union woden_binding_op_base woden_binding_op_base_t;
typedef struct woden_binding_op woden_binding_op_t;
typedef struct woden_binding_op_ops woden_binding_op_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_binding_op_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *binding_op,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    to_binding_op_free) (void *binding_op,
            const axutil_env_t *env);
    
    axutil_hash_t *(AXIS2_CALL *
    super_objs) (void *binding_op,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *binding_op,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *binding_op,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_interface_op) (
            void *binding_op,
            const axutil_env_t *env);

    axutil_array_list_t *(AXIS2_CALL *
    get_binding_msg_refs) (
            void *binding_op,
            const axutil_env_t *env);

    axutil_array_list_t *(AXIS2_CALL *
    get_binding_fault_refs) (
            void *binding_op,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *binding_op,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_interface_op_element) (
            void *binding_op,
            const axutil_env_t *env,
            void *int_op);

};

union woden_binding_op_base
{
    woden_nested_configurable_t nested_configurable;
    woden_binding_op_element_t binding_op_element;
};

struct woden_binding_op
{
    woden_binding_op_base_t base;
    woden_binding_op_ops_t *ops;
};

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_binding_op_element(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_nested_configurable(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_configurable(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_wsdl_obj(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_nested_element(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_wsdl_component(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_configurable_element(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_documentable_element(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_documentable(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_attr_extensible(
        void *binding_op,
        const axutil_env_t *env);

AXIS2_EXTERN woden_binding_op_t * AXIS2_CALL
woden_binding_op_to_element_extensible(
        void *binding_op,
        const axutil_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_binding_op_resolve_methods(
        woden_binding_op_t *binding_op,
        const axutil_env_t *env,
        woden_binding_op_t *binding_op_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_BINDING_OP_FREE(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->free(binding_op, env))

#define WODEN_BINDING_OP_TO_BINDING_OP_FREE(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->to_binding_op_free(binding_op, env))

#define WODEN_BINDING_OP_SUPER_OBJS(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->super_objs(binding_op, env))

#define WODEN_BINDING_OP_TYPE(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->type(binding_op, env))

#define WODEN_BINDING_OP_GET_BASE_IMPL(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->get_base_impl(binding_op, env))

#define WODEN_BINDING_OP_GET_INTERFACE_OP(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->\
         get_interface_op(binding_op, env))

#define WODEN_BINDING_OP_GET_BINDING_MSG_REFS(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->\
         get_binding_msg_refs(binding_op, env))

#define WODEN_BINDING_OP_GET_BINDING_FAULT_REFS(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->\
         get_binding_fault_refs(binding_op, env))

#define WODEN_BINDING_OP_TO_ELEMENT(binding_op, env) \
      (((woden_binding_op_t *) binding_op)->ops->\
         to_element(binding_op, env))

#define WODEN_BINDING_OP_SET_INTERFACE_OP_ELEMENT(binding_op, env, int_op) \
      (((woden_binding_op_t *) binding_op)->ops->\
         set_interface_op_element(binding_op, env, int_op))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_BINDING_OP_H */
