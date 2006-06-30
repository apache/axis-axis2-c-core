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

#ifndef WODEN_DOCUMENTABLE_ELEMENT_H
#define WODEN_DOCUMENTABLE_ELEMENT_H

/**
 * @file woden_documentable_element.h
 * @brief Axis2 Documentable Element Interface
 * Interfaces for WSDL 2.0 elements which may have &lt;documentation&gt; 
 * child elements will extend this interface. That is, all WSDL 2.0 elements
 * except the &lt;documentation&gt; element itself.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_array_list.h>
#include <woden_wsdl_element.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_documentable_element 
        woden_documentable_element_t;
typedef struct woden_documentable_element_ops 
        woden_documentable_element_ops_t;
struct woden_documentation_element;

/** @defgroup woden_documentable_element Documentable Element
  * @ingroup woden
  * @{
  */

struct woden_documentable_element_ops
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
    type) (
            void *doc_el,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (void *doc_el,
                        const axis2_env_t *env,
                        struct woden_documentation_element *doc_elem);

    axis2_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (void *doc_el,
                        const axis2_env_t *env);

};

struct woden_documentable_element
{
    woden_wsdl_element_t wsdl_element;
    woden_documentable_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_documentable_element_resolve_methods(
        woden_documentable_element_t *documentable_element,
        const axis2_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_DOCUMENTABLE_ELEMENT_FREE(doc_el, env) \
      (((woden_documentable_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define WODEN_DOCUMENTABLE_ELEMENT_TYPE(doc_el, env) \
      (((woden_documentable_element_t *) doc_el)->ops->\
         type (doc_el, env))

#define WODEN_DOCUMENTABLE_ELEMENT_ADD_DOCUMENTATION_ELEMENT(doc_el, env, doc_elem) \
      (((woden_documentable_element_t *) doc_el)->ops->\
         add_documentation_element(doc_el, env, doc_elem))

#define WODEN_DOCUMENTABLE_ELEMENT_GET_DOCUMENTATOIN_ELEMENTS(doc_el, env) \
      (((woden_documentable_element_t *) doc_el)->ops->\
         get_documentation_elements(doc_el, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_DOCUMENTABLE_ELEMENT_H */
