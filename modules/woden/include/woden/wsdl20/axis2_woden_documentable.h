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

#ifndef AXIS2_WODEN_DOCUMENTABLE_H
#define AXIS2_WODEN_DOCUMENTABLE_H

/**
 * @file axis2_woden_documentable.h
 * @brief Axis2 Documentable Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/axis2_woden_wsdl_obj.h>

/** @defgroup axis2_woden_documentable Documentable
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_documentable_base axis2_woden_documentable_base_t;
typedef struct axis2_woden_documentable axis2_woden_documentable_t;
typedef struct axis2_woden_documentable_ops axis2_woden_documentable_ops_t;
struct axis2_woden_documentation_element;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_documentable_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *documentable,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    to_documentable_free) (
            void *documentable,
            axis2_env_t **env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *documentable,
            axis2_env_t **env);
    
    /**
     * @return the base implementation class
     */
    axis2_woden_wsdl_obj_t *(AXIS2_CALL *
    get_base_impl) (
            void *documentable,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (
            void *types,
            axis2_env_t **env,
            struct axis2_woden_documentation_element *documentation);

    axis2_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (
            void *types,
            axis2_env_t **env);


};

union axis2_woden_documentable_base
{
    axis2_woden_wsdl_obj_t wsdl_obj;
    axis2_woden_documentable_element_t documentable_elem;
};

struct axis2_woden_documentable
{
    axis2_woden_documentable_base_t base;
    axis2_woden_documentable_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_documentable_t *)
axis2_woden_documentable_create(
        axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_status_t)
axis2_woden_documentable_resolve_methods(
        axis2_woden_documentable_t *documentable,
        axis2_env_t **env,
        axis2_woden_documentable_t *documentable_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/


#define AXIS2_WODEN_DOCUMENTABLE_FREE(documentable, env) \
		(((axis2_woden_documentable_t *) documentable)->ops->\
         free(documentable, env))

#define AXIS2_WODEN_DOCUMENTABLE_TO_DOCUMENTABLE_FREE(documentable, env) \
		(((axis2_woden_documentable_t *) documentable)->ops->\
         to_documentable_free(documentable, env))

#define AXIS2_WODEN_DOCUMENTABLE_SUPER_OBJS(documentable, env) \
		(((axis2_woden_documentable_t *) documentable)->ops->\
         super_objs(documentable, env))

#define AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(documentable, env) \
		(((axis2_woden_documentable_t *) documentable)->ops->\
         get_base_impl(documentable, env))

#define AXIS2_WODEN_DOCUMENTABLE_ADD_DOCUMENTATION_ELEMENT(documentable, env, \
        documentation) \
		(((axis2_woden_documentable_t *) documentable)->ops->\
         add_documentation_element(documentable, env, documentation))

#define AXIS2_WODEN_DOCUMENTABLE_GET_DOCUMENTATION_ELEMENTS(documentable, env) \
		(((axis2_woden_documentable_t *) documentable)->ops->\
         get_documentation_elements(documentable, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_DOCUMENTABLE_H */
