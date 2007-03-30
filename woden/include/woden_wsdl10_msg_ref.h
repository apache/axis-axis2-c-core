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

#ifndef WODEN_WSDL10_MSG_REF_H
#define WODEN_WSDL10_MSG_REF_H

/**
 * @file woden_wsdl10_msg_ref.h
 * @brief Axis2 Interface Message Reference Interface Message Reference
 * 
 */

#include <woden.h>
#include <woden_wsdl10_msg_ref_element.h>
#include <woden_nested_configurable.h>
#include <axutil_qname.h>

/** @defgroup woden_wsdl10_msg_ref Interface Message Reference
  * @ingroup woden
  * @{
  */

typedef union woden_wsdl10_msg_ref_base woden_wsdl10_msg_ref_base_t;
typedef struct woden_wsdl10_msg_ref woden_wsdl10_msg_ref_t;
typedef struct woden_wsdl10_msg_ref_ops woden_wsdl10_msg_ref_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_wsdl10_msg_ref_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *msg_ref,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs) (void *msg_ref,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *msg_ref,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *msg_ref,
            const axutil_env_t *env);

    axutil_qname_t *(AXIS2_CALL *
    get_qname) (
            void *interface_op,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element) (
            void *interface_op,
            const axutil_env_t *env);

};

union woden_wsdl10_msg_ref_base
{
    woden_nested_configurable_t nested_configurable;
    woden_wsdl10_msg_ref_element_t msg_ref_element;
};

struct woden_wsdl10_msg_ref
{
    woden_wsdl10_msg_ref_base_t base;
    woden_wsdl10_msg_ref_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_msg_ref_element(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_nested_configurable(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_configurable(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_nested_element(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_configurable_element(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_documentable_element(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_documentable(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_attr_extensible(
        void *msg_ref,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_msg_ref_t * AXIS2_CALL
woden_wsdl10_msg_ref_to_element_extensible(
        void *msg_ref,
        const axutil_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl10_msg_ref_resolve_methods(
        woden_wsdl10_msg_ref_t *msg_ref,
        const axutil_env_t *env,
        woden_wsdl10_msg_ref_t *msg_ref_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_MSG_REF_FREE(msg_ref, env) \
      (((woden_wsdl10_msg_ref_t *) msg_ref)->ops->\
       free(msg_ref, env))

#define WODEN_WSDL10_MSG_REF_SUPER_OBJS(msg_ref, env) \
      (((woden_wsdl10_msg_ref_t *) msg_ref)->ops->\
       super_objs(msg_ref, env))

#define WODEN_WSDL10_MSG_REF_TYPE(msg_ref, env) \
      (((woden_wsdl10_msg_ref_t *) msg_ref)->ops->\
       type(msg_ref, env))

#define WODEN_WSDL10_MSG_REF_GET_BASE_IMPL(msg_ref, env) \
      (((woden_wsdl10_msg_ref_t *) msg_ref)->ops->\
       get_base_impl(msg_ref, env))

#define WODEN_WSDL10_MSG_REF_GET_QNAME(msg_ref, env) \
      (((woden_wsdl10_msg_ref_t *) msg_ref)->ops->\
         get_qname(msg_ref, env))

#define WODEN_WSDL10_MSG_REF_TO_ELEMENT(msg_ref, env) \
      (((woden_wsdl10_msg_ref_t *) msg_ref)->ops->\
         to_element(msg_ref, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_MSG_REF_H */
