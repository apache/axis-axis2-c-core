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

#ifndef WODEN_DOCUMENTATION_ELEMENT_H
#define WODEN_DOCUMENTATION_ELEMENT_H

/**
 * @file woden_documentation_element.h
 * @brief Axis2 Documentation Element Interface
 * This interface represents the WSDL 2.0 &lt;wsdl:documentation&gt; XML element.
 * The &lt;wsdl:documentation&gt; element may contain mixed content, but this 
 * interface does not attempt to understand that content. Instead it just wraps
 * the &lt;wsdl:documentation&gt; element's content model as a void *. 
 *
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axutil_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <woden_wsdl_element.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_documentation_element 
        woden_documentation_element_t;
typedef struct woden_documentation_element_ops 
        woden_documentation_element_ops_t;

/** @defgroup woden_documentation_element Documentation Element
  * @ingroup woden
  * @{
  */

struct woden_documentation_element_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *doc_el,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *doc_el,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    set_content) (void *doc_el,
            const axutil_env_t *env,
            void *doc_elem);

    void *(AXIS2_CALL *
    get_content) (
            void *doc_el,
            const axutil_env_t *env);

};

struct woden_documentation_element
{
    woden_wsdl_element_t wsdl_element;
    woden_documentation_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_documentation_element_resolve_methods(
        woden_documentation_element_t *documentation_element,
        const axutil_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_DOCUMENTATION_ELEMENT_FREE(doc_el, env) \
      (((woden_documentation_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define WODEN_DOCUMENTATION_ELEMENT_TYPE(doc_el, env) \
      (((woden_documentation_element_t *) doc_el)->ops->\
         type (doc_el, env))

#define WODEN_DOCUMENTATION_ELEMENT_SET_CONTENT(doc_el, env, doc_elem) \
      (((woden_documentation_element_t *) doc_el)->ops->\
         set_content(doc_el, env, doc_elem))

#define WODEN_DOCUMENTATION_ELEMENT_GET_CONTENT(doc_el, env) \
      (((woden_documentation_element_t *) doc_el)->ops->\
         get_content(doc_el, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_DOCUMENTATION_ELEMENT_H */
