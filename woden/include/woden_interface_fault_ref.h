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

#ifndef WODEN_INTERFACE_FAULT_REF_H
#define WODEN_INTERFACE_FAULT_REF_H

/**
 * @file woden_interface_fault_ref.h
 * @brief Axis2 Interface Message Reference Interface Message Reference
 * 
 */

#include <woden.h>
#include <woden_interface_fault_ref_element.h>
#include <woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup woden_interface_fault_ref Interface Message Reference
  * @ingroup woden
  * @{
  */

typedef union woden_interface_fault_ref_base woden_interface_fault_ref_base_t;
typedef struct woden_interface_fault_ref woden_interface_fault_ref_t;
typedef struct woden_interface_fault_ref_ops woden_interface_fault_ref_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_interface_fault_ref_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *interface_fault_ref,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *interface_fault_ref,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *interface_fault_ref,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *interface_fault_ref,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_interface_fault) (
            void *interface_fault_ref,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_direction) (
            void *interface_fault_ref,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_msg_label) (
            void *interface_fault_ref,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *interface_fault_ref,
            const axis2_env_t *env);

    /* ************************************************************
     *  Non-API implementation methods
     * ************************************************************/
    axis2_status_t (AXIS2_CALL *
    set_interface_fault) (
        void *interface_fault_ref,
        const axis2_env_t *env,
        void *fault);

};

union woden_interface_fault_ref_base
{
    woden_nested_configurable_t nested_configurable;
    woden_interface_fault_ref_element_t interface_fault_ref_element;
};

struct woden_interface_fault_ref
{
    woden_interface_fault_ref_base_t base;
    woden_interface_fault_ref_ops_t *ops;
};

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_interface_fault_ref_element(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_nested_configurable(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_configurable(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_nested_element(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_configurable_element(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_documentable_element(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_documentable(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_attr_extensible(
        void *interface_fault_ref,
        const axis2_env_t *env);

AXIS2_EXTERN woden_interface_fault_ref_t * AXIS2_CALL
woden_interface_fault_ref_to_element_extensible(
        void *interface_fault_ref,
        const axis2_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_interface_fault_ref_resolve_methods(
        woden_interface_fault_ref_t *interface_fault_ref,
        const axis2_env_t *env,
        woden_interface_fault_ref_t *interface_fault_ref_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INTERFACE_FAULT_REF_FREE(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->ops->\
       free(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->ops->\
       super_objs(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_TYPE(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->ops->\
       type(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_GET_BASE_IMPL(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->ops->\
       get_base_impl(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_GET_DIRECTION(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->ops->\
         get_direction(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_TO_ELEMENT(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->ops->\
         to_element(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_GET_MSG_LABEL(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->\
         get_msg_label(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_GET_INTERFACE_FAULT(interface_fault_ref, env) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->\
         get_interface_fault(interface_fault_ref, env))

#define WODEN_INTERFACE_FAULT_REF_SET_INTERFACE_FAULT(interface_fault_ref, env, fault) \
      (((woden_interface_fault_ref_t *) interface_fault_ref)->ops->\
         set_interface_fault(interface_fault_ref, env, fault))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INTERFACE_FAULT_REF_H */
