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

#ifndef WODEN_ELEMENT_EXTENSIBLE_H
#define WODEN_ELEMENT_EXTENSIBLE_H

/**
 * @file woden_element_extensible.h
 * @brief Axis2 Element Extensible Interface
 *          Common code for handling extension elements. 
 *          Can be reused by inheritance or by delegation.
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <axutil_array_list.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_element_extensible woden_element_extensible_t;
typedef struct woden_element_extensible_ops woden_element_extensible_ops_t;
struct woden_ext_element;

/** @defgroup woden_element_extensible Element Extensible
  * @ingroup woden
  * @{
  */

struct woden_element_extensible_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *
            element_extensible,
            const axutil_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *
            element_extensible,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *element_extensible,
            const axutil_env_t *env);
 
    axis2_status_t (AXIS2_CALL *
    add_ext_element) (
            void *extensible,
            const axutil_env_t *env,
            struct woden_ext_element *ext_el); 

    axis2_status_t (AXIS2_CALL *
    remove_ext_element) (
            void *extensible,
            const axutil_env_t *env,
            struct woden_ext_element *ext_el); 

    axutil_array_list_t *(AXIS2_CALL *
    get_ext_elements) (
            void *extensible,
            const axutil_env_t *env); 

    axutil_array_list_t *(AXIS2_CALL *
    get_ext_elements_of_type) (
            void *extensible,
            const axutil_env_t *env,
            axis2_qname_t *ext_type);

    axis2_bool_t (AXIS2_CALL *
    has_ext_elements_for_namespace) (
            void *extensible,
            const axutil_env_t *env,
            axis2_uri_t *namespc);
 
};

struct woden_element_extensible
{
    woden_element_extensible_ops_t *ops;
};

AXIS2_EXTERN woden_element_extensible_t * AXIS2_CALL
woden_element_extensible_create(const axutil_env_t *env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_element_extensible_resolve_methods(
        woden_element_extensible_t *extensible,
        const axutil_env_t *env,
        woden_element_extensible_t *extensible_impl,
        axis2_hash_t *methods);

#define WODEN_ELEMENT_EXTENSIBLE_FREE(extensible, env) \
      (((woden_element_extensible_t *) extensible)->ops->\
         free (extensible, env))

#define WODEN_ELEMENT_EXTENSIBLE_SUPER_OBJS(extensible, env) \
      (((woden_element_extensible_t *) extensible)->ops->\
         super_objs (extensible, env))

#define WODEN_ELEMENT_EXTENSIBLE_TYPE(extensible, env) \
      (((woden_element_extensible_t *) extensible)->ops->\
         type (extensible, env))

#define WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(extensible, env, ext_el) \
      (((woden_element_extensible_t *) extensible)->ops->\
         add_ext_element(extensible, env, ext_el))

#define WODEN_ELEMENT_EXTENSIBLE_REMOVE_EXT_ELEMENT(extensible, env, \
        ext_el) \
      (((woden_element_extensible_t *) extensible)->ops->\
         remove_ext_element(extensible, env, ext_el))

#define WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(extensible, env) \
      (((woden_element_extensible_t *) extensible)->ops->\
         get_ext_elements(extensible, env))

#define WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(extensible, \
        env, ext_type) \
      (((woden_element_extensible_t *) extensible)->ops->\
         get_ext_elements_of_type(extensible, env, ext_type))

#define WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(\
        extensible, env, namespc) \
      (((woden_element_extensible_t *) extensible)->ops->\
         has_ext_elements_for_namespace(extensible, env, namespc))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_ELEMENT_EXTENSIBLE_H */
