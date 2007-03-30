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

#ifndef WODEN_WSDL10_PART_H
#define WODEN_WSDL10_PART_H

/**
 * @file woden_wsdl10_part.h
 * @brief Woden Part Interface Message Reference
 * 
 */

#include <woden.h>
#include <woden_wsdl10_part_element.h>
#include <woden_nested_configurable.h>
#include <axis2_qname.h>

/** @defgroup woden_wsdl10_part Interface Message Reference
  * @ingroup woden
  * @{
  */

typedef union woden_wsdl10_part_base woden_wsdl10_part_base_t;
typedef struct woden_wsdl10_part woden_wsdl10_part_t;
typedef struct woden_wsdl10_part_ops woden_wsdl10_part_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_wsdl10_part_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *part,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs) (void *part,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *part,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *part,
            const axutil_env_t *env);

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *part,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_element_declaration) (
            void *part,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *part,
            const axutil_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
    axis2_status_t (AXIS2_CALL *
    set_element_declaration) (
            void *part,
            const axutil_env_t *env,
            void *element);

    axis2_status_t (AXIS2_CALL *
    set_types) (
            void *part,
            const axutil_env_t *env,
            void *types);
};

union woden_wsdl10_part_base
{
    woden_nested_configurable_t nested_configurable;
    woden_wsdl10_part_element_t part_element;
};

struct woden_wsdl10_part
{
    woden_wsdl10_part_base_t base;
    woden_wsdl10_part_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_part_element(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_nested_configurable(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_configurable(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_nested_element(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_configurable_element(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_documentable_element(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_documentable(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_attr_extensible(
        void *part,
        const axutil_env_t *env);

AXIS2_EXTERN woden_wsdl10_part_t * AXIS2_CALL
woden_wsdl10_part_to_element_extensible(
        void *part,
        const axutil_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl10_part_resolve_methods(
        woden_wsdl10_part_t *part,
        const axutil_env_t *env,
        woden_wsdl10_part_t *part_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_PART_FREE(part, env) \
      (((woden_wsdl10_part_t *) part)->ops->\
       free(part, env))

#define WODEN_WSDL10_PART_SUPER_OBJS(part, env) \
      (((woden_wsdl10_part_t *) part)->ops->\
       super_objs(part, env))

#define WODEN_WSDL10_PART_TYPE(part, env) \
      (((woden_wsdl10_part_t *) part)->ops->\
       type(part, env))

#define WODEN_WSDL10_PART_GET_BASE_IMPL(part, env) \
      (((woden_wsdl10_part_t *) part)->ops->\
       get_base_impl(part, env))

#define WODEN_WSDL10_PART_GET_QNAME(part, env) \
      (((woden_wsdl10_part_t *) part)->ops->\
         get_qname(part, env))

#define WODEN_WSDL10_PART_GET_ELEMENT_DECLARATION(part, env) \
      (((woden_wsdl10_part_t *) part)->ops->\
         get_element_declaration(part, env))

#define WODEN_WSDL10_PART_TO_ELEMENT(part, env) \
      (((woden_wsdl10_part_t *) part)->ops->\
         to_element(part, env))

#define WODEN_WSDL10_PART_SET_ELEMENT_DECLARATION(part, env, element) \
      (((woden_wsdl10_part_t *) part)->ops->\
         set_element_declaration(part, env, element))

#define WODEN_WSDL10_PART_SET_TYPES(part, env, types) \
      (((woden_wsdl10_part_t *) part)->ops->\
         set_types(part, env, types))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_PART_H */
