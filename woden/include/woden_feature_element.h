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

#ifndef WODEN_FEATURE_ELEMENT_H
#define WODEN_FEATURE_ELEMENT_H

/**
 * @file woden_feature_element.h
 * @brief Axis2 Feature Element Interface
 * Represents the &lt;feature&gt; element.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_feature_element woden_feature_element_t;
typedef struct woden_feature_element_ops woden_feature_element_ops_t;
typedef union woden_feature_element_base woden_feature_element_base_t;

/** @defgroup woden_feature_element Feature Element
  * @ingroup woden
  * @{
  */

struct woden_feature_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *doc_el,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (void *doc_el,
            const axis2_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_ref) (void *doc_el,
            const axis2_env_t *env,
            axis2_uri_t *uri);

    axis2_uri_t *(AXIS2_CALL *
    get_ref) (void *doc_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_required) (void *doc_el,
            const axis2_env_t *env,
            axis2_bool_t required);

    axis2_bool_t (AXIS2_CALL *
    is_required) (void *doc_el,
            const axis2_env_t *env);

};

union woden_feature_element_base
{
    woden_documentable_element_t documentable_element;
    woden_nested_element_t nested_element;
};

struct woden_feature_element
{
    woden_feature_element_base_t base;
    woden_feature_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_feature_element_resolve_methods(
        woden_feature_element_t *feature_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_FEATURE_ELEMENT_FREE(doc_el, env) \
      (((woden_feature_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define WODEN_FEATURE_ELEMENT_TYPE(doc_el, env) \
      (((woden_feature_element_t *) doc_el)->ops->\
         type (doc_el, env))

#define WODEN_FEATURE_ELEMENT_SET_REF(doc_el, env, uri) \
      (((woden_feature_element_t *) doc_el)->ops->\
         set_ref(doc_el, env, uri))

#define WODEN_FEATURE_ELEMENT_GET_REF(doc_el, env) \
      (((woden_feature_element_t *) doc_el)->ops->\
         get_ref(doc_el, env))

#define WODEN_FEATURE_ELEMENT_SET_REQUIRED(doc_el, env, required) \
      (((woden_feature_element_t *) doc_el)->ops->\
         set_required(doc_el, env, required))

#define WODEN_FEATURE_ELEMENT_IS_REQUIRED(doc_el, env) \
      (((woden_feature_element_t *) doc_el)->ops->\
         is_required(doc_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_FEATURE_ELEMENT_H */
