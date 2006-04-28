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

#ifndef AXIS2_WODEN_DOCUMENTABLE_ELEMENT_H
#define AXIS2_WODEN_DOCUMENTABLE_ELEMENT_H

/**
 * @file axis2_woden_documentable_element.h
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
#include <axis2.h>
#include <axis2_hash.h>
#include <axis2_array_list.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_documentable_element 
        axis2_woden_documentable_element_t;
typedef struct axis2_woden_documentable_element_ops 
        axis2_woden_documentable_element_ops_t;
struct axis2_woden_documentation_element;

/** @defgroup axis2_woden_documentable_element Documentable Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_documentable_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *doc_el,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (void *doc_el,
                        axis2_env_t **env,
                        struct axis2_woden_documentation_element *doc_elem);

    axis2_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (void *doc_el,
                        axis2_env_t **env);

};

struct axis2_woden_documentable_element
{
    axis2_woden_documentable_element_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_documentable_element_t *)
axis2_woden_documentable_element_create(axis2_env_t **env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_DECLARE(axis2_status_t)
axis2_woden_documentable_element_resolve_methods(
                                axis2_woden_documentable_element_t *doc_el,
                                axis2_env_t **env,
                                axis2_woden_documentable_element_t *doc_el_impl,
                                axis2_hash_t *methods);

#define AXIS2_WODEN_DOCUMENTABLE_ELEMENT_FREE(doc_el, env) \
		(((axis2_woden_documentable_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define AXIS2_WODEN_DOCUMENTABLE_ELEMENT_ADD_DOCUMENTATION_ELEMENT(doc_el, env, doc_elem) \
		(((axis2_woden_documentable_element_t *) doc_el)->ops->\
         add_documentation_element(doc_el, env, doc_elem))

#define AXIS2_WODEN_DOCUMENTABLE_ELEMENT_GET_DOCUMENTATOIN_ELEMENTS(doc_el, env) \
		(((axis2_woden_documentable_element_t *) doc_el)->ops->\
         get_documentation_elements(doc_el, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_DOCUMENTABLE_ELEMENT_H */
