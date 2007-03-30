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

#ifndef WODEN_NESTED_ELEMENT_H
#define WODEN_NESTED_ELEMENT_H

/**
 * @file woden_nested_element.h
 * @brief Axis2 Nested Element Interface
 * Elements which are nested within a 'top-level' WSDL element will
 * extend this interface. These are &lt;feature&gt;, &lt;property&gt;
 * and the child elements of &lt;interface&gt;, &lt;binding&gt; and
 * &lt;service&gt;.  
 * <p>
 * The elements which are not nested are &lt;description&gt;, 
 * &lt;import&gt;, &lt;include&gt;, &lt;types&gt;, &lt;interface&gt;, 
 * &lt;binding&gt; and &lt;service&gt;.
 * 
 *
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axutil_string.h>
#include <axutil_utils.h>
#include <axutil_hash.h>
#include <axutil_qname.h>
#include <woden.h>
#include <woden_wsdl_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_nested_element woden_nested_element_t;
typedef struct woden_nested_element_ops woden_nested_element_ops_t;
struct woden_wsdl_element;

/** @defgroup woden_nested_element Nested Element
  * @ingroup woden
  * @{
  */

struct woden_nested_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *nested_el,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *nested_el,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_parent_element) (
            void *nested_el,
            const axutil_env_t *env,
            struct woden_wsdl_element *parent);

    void *(AXIS2_CALL *
    get_parent_element) (
            void *nested_el,
            const axutil_env_t *env);

};

struct woden_nested_element
{
    woden_wsdl_element_t wsdl_element;
    woden_nested_element_ops_t *ops;
};

/************************Woden C Internal Methods*****************************/
axis2_status_t AXIS2_CALL
woden_nested_element_resolve_methods(
        woden_nested_element_t *nested_element,
        const axutil_env_t *env,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_NESTED_ELEMENT_FREE(nested_el, env) \
      (((woden_nested_element_t *) nested_el)->ops->\
         free (nested_el, env))

#define WODEN_NESTED_ELEMENT_TYPE(nested_el, env) \
      (((woden_nested_element_t *) nested_el)->ops->\
         type (nested_el, env))

#define WODEN_NESTED_ELEMENT_SET_PARENT_ELEMENT(nested_el, env, parent) \
      (((woden_nested_element_t *) nested_el)->ops->\
         set_parent_element(nested_el, env, parent))

#define WODEN_NESTED_ELEMENT_GET_PARENT_ELEMENT(nested_el, env) \
      (((woden_nested_element_t *) nested_el)->ops->\
         get_parent_element(nested_el, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_NESTED_ELEMENT_H */
