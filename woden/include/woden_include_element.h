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

#ifndef WODEN_INCLUDE_ELEMENT_H
#define WODEN_INCLUDE_ELEMENT_H

/**
 * @file woden_include_element.h
 * @brief Axis2 Include Element Interface
 * This interface represents a &lt;include&gt; XML element 
 * information item. It declares the behaviour required to support 
 * parsing, creating and manipulating a &lt;include&gt; element.
 *
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axutil_string.h>
#include <axis2_utils.h>
#include <axutil_hash.h>
#include <axutil_qname.h>
#include <axis2_uri.h>
#include <woden.h>
#include <woden_documentable_element.h>
#include <woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_include_element woden_include_element_t;
typedef struct woden_include_element_ops woden_include_element_ops_t;
typedef union woden_include_element_base woden_include_element_base_t;

/** @defgroup woden_include_element Include Element
  * @ingroup woden
  * @{
  */

struct woden_include_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *include_el,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (void *include_el,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_location) (void *include_el,
            const axutil_env_t *env,
            axis2_uri_t *loc_uri);

    axis2_uri_t *(AXIS2_CALL *
    get_location) (void *include_el,
            const axutil_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_desc_element) (void *include_el,
            const axutil_env_t *env,
            void *desc);

    void *(AXIS2_CALL *
    get_desc_element) (void *include_el,
            const axutil_env_t *env);

};


struct woden_include_element
{
    woden_documentable_element_t documentable_element;
    woden_include_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_include_element_resolve_methods(
        woden_include_element_t *include_element,
        const axutil_env_t *env,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INCLUDE_ELEMENT_FREE(include_el, env) \
      (((woden_include_element_t *) include_el)->ops->\
         free (include_el, env))

#define WODEN_INCLUDE_ELEMENT_TYPE(include_el, env) \
      (((woden_include_element_t *) include_el)->ops->\
         type (include_el, env))

#define WODEN_INCLUDE_ELEMENT_SET_LOCATION(include_el, env, loc_uri) \
      (((woden_include_element_t *) include_el)->ops->\
         set_location(include_el, env, loc_uri))

#define WODEN_INCLUDE_ELEMENT_GET_LOCATION(include_el, env) \
      (((woden_include_element_t *) include_el)->ops->\
         get_location(include_el, env))

#define WODEN_INCLUDE_ELEMENT_SET_DESC_ELEMENT(include_el, env, desc) \
      (((woden_include_element_t *) include_el)->ops->\
         set_desc_element(include_el, env, desc))

#define WODEN_INCLUDE_ELEMENT_GET_DESC_ELEMENT(include_el, env) \
      (((woden_include_element_t *) include_el)->ops->\
         get_desc_element(include_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INCLUDE_ELEMENT_H */
