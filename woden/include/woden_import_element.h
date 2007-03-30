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

#ifndef WODEN_IMPORT_ELEMENT_H
#define WODEN_IMPORT_ELEMENT_H

/**
 * @file woden_import_element.h
 * @brief Axis2 Import Element Interface
 * This interface represents a &lt;import&gt; XML element 
 * information item. It declares the behaviour required to support 
 * parsing, creating and manipulating a &lt;import&gt; element.
 *
 */

#include <axutil_allocator.h>
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

typedef struct woden_import_element woden_import_element_t;
typedef struct woden_import_element_ops woden_import_element_ops_t;
typedef union woden_import_element_base woden_import_element_base_t;

/** @defgroup woden_import_element Import Element
  * @ingroup woden
  * @{
  */

struct woden_import_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *import_el,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (void *import_el,
            const axis2_env_t *env);
     
    axis2_status_t (AXIS2_CALL *
    set_namespace) (void *import_el,
            const axis2_env_t *env,
            axis2_uri_t *ns_uri);

    axis2_uri_t *(AXIS2_CALL *
    get_namespace) (void *import_el,
            const axis2_env_t *env);

   
    axis2_status_t (AXIS2_CALL *
    set_location) (void *import_el,
            const axis2_env_t *env,
            axis2_uri_t *loc_uri);

    axis2_uri_t *(AXIS2_CALL *
    get_location) (void *import_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_desc_element) (void *import_el,
            const axis2_env_t *env,
            void *desc);

    void *(AXIS2_CALL *
    get_desc_element) (void *import_el,
            const axis2_env_t *env);

};


struct woden_import_element
{
    woden_documentable_element_t documentable_element;
    woden_import_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_import_element_resolve_methods(
        woden_import_element_t *import_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_IMPORT_ELEMENT_FREE(import_el, env) \
      (((woden_import_element_t *) import_el)->ops->\
         free (import_el, env))

#define WODEN_IMPORT_ELEMENT_TYPE(import_el, env) \
      (((woden_import_element_t *) import_el)->ops->\
         type (import_el, env))

#define WODEN_IMPORT_ELEMENT_SET_NAMESPACE(import_el, env, ns_uri) \
      (((woden_import_element_t *) import_el)->ops->\
         set_namespace(import_el, env, ns_uri))

#define WODEN_IMPORT_ELEMENT_GET_NAMESPACE(import_el, env) \
      (((woden_import_element_t *) import_el)->ops->\
         get_namespace(import_el, env))

#define WODEN_IMPORT_ELEMENT_SET_LOCATION(import_el, env, loc_uri) \
      (((woden_import_element_t *) import_el)->ops->\
         set_location(import_el, env, loc_uri))

#define WODEN_IMPORT_ELEMENT_GET_LOCATION(import_el, env) \
      (((woden_import_element_t *) import_el)->ops->\
         get_location(import_el, env))

#define WODEN_IMPORT_ELEMENT_SET_DESC_ELEMENT(import_el, env, desc) \
      (((woden_import_element_t *) import_el)->ops->\
         set_desc_element(import_el, env, desc))

#define WODEN_IMPORT_ELEMENT_GET_DESC_ELEMENT(import_el, env) \
      (((woden_import_element_t *) import_el)->ops->\
         get_desc_element(import_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_IMPORT_ELEMENT_H */
