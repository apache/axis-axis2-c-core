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

#ifndef AXIS2_WODEN_INTERFACE_OP_H
#define AXIS2_WODEN_INTERFACE_OP_H

/**
 * @file axis2_woden_interface_op.h
 * @brief Axis2 Interface Operation Interface Operation
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_interface_op_element.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup axis2_woden_interface_op Interface Operation
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_interface_op_base axis2_woden_interface_op_base_t;
typedef struct axis2_woden_interface_op axis2_woden_interface_op_t;
typedef struct axis2_woden_interface_op_ops axis2_woden_interface_op_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_interface_op_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *interface_op,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *interface_op,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *interface_op,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *interface_op,
            const axis2_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *interface_op,
            const axis2_env_t *env);

    axis2_uri_t *(AXIS2_CALL *
    get_msg_exchange_pattern) (
            void *interface_op,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_msg_refs) (
            void *interface_op,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_interface_fault_refs) (
            void *interface_op,
            const axis2_env_t *env);

    axis2_array_list_t *(AXIS2_CALL *
    get_style) (
            void *interface_op,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *interface_op,
            const axis2_env_t *env);

};

union axis2_woden_interface_op_base
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_interface_op_element_t interface_op_element;
};

struct axis2_woden_interface_op
{
    axis2_woden_interface_op_base_t base;
    axis2_woden_interface_op_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_interface_op_element(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_nested_configurable(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_configurable(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_nested_element(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_configurable_element(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_documentable_element(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_documentable(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_attr_extensible(
        void *interface_op,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_interface_op_t * AXIS2_CALL
axis2_woden_interface_op_to_element_extensible(
        void *interface_op,
        const axis2_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_interface_op_resolve_methods(
        axis2_woden_interface_op_t *interface_op,
        const axis2_env_t *env,
        axis2_woden_interface_op_t *interface_op_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INTERFACE_OP_FREE(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->ops->free(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->ops->super_objs(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_TYPE(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->ops->type(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_GET_BASE_IMPL(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->ops->get_base_impl(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_GET_QNAME(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->\
         get_qname(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_GET_MSG_EXCHANGE_PATTERN(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->\
         get_msg_exchange_pattern(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_GET_INTERFACE_MSG_REFS(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->\
         get_interface_msg_refs(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_GET_INTERFACE_FAULT_REFS(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->\
         get_interface_fault_refs(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_GET_STYLE(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->\
         get_style(interface_op, env))

#define AXIS2_WODEN_INTERFACE_OP_TO_ELEMENT(interface_op, env) \
      (((axis2_woden_interface_op_t *) interface_op)->ops->\
         to_element(interface_op, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INTERFACE_OP_H */
