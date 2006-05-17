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

#ifndef AXIS2_WODEN_FEATURE_ELEMENT_H
#define AXIS2_WODEN_FEATURE_ELEMENT_H

/**
 * @file axis2_woden_feature_element.h
 * @brief Axis2 Feature Element Interface
 * Represents the &lt;feature&gt; element.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/wsdl20/xml/axis2_woden_nested_element.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_feature_element axis2_woden_feature_element_t;
typedef struct axis2_woden_feature_element_ops axis2_woden_feature_element_ops_t;
typedef union axis2_woden_feature_element_base axis2_woden_feature_element_base_t;

/** @defgroup axis2_woden_feature_element Feature Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_feature_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *doc_el,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    to_feature_element_free) (
            void *doc_el,
            axis2_env_t **env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *doc_el,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_ref) (void *doc_el,
            axis2_env_t **env,
            axis2_url_t *uri);

    axis2_url_t *(AXIS2_CALL *
    get_ref) (void *doc_el,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_required) (void *doc_el,
            axis2_env_t **env,
            axis2_bool_t required);

    axis2_bool_t (AXIS2_CALL *
    is_required) (void *doc_el,
            axis2_env_t **env);

};

union axis2_woden_feature_element_base
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_nested_element_t nested_element;
};

struct axis2_woden_feature_element
{
    axis2_woden_feature_element_base_t base;
    axis2_woden_feature_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
axis2_woden_feature_element_resolve_methods(
        axis2_woden_feature_element_t *feature_element,
        axis2_env_t **env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_FEATURE_ELEMENT_FREE(doc_el, env) \
		(((axis2_woden_feature_element_t *) doc_el)->ops->\
         free (doc_el, env))

#define AXIS2_WODEN_FEATURE_ELEMENT_TO_FEATURE_ELEMENT_FREE(doc_el, env) \
		(((axis2_woden_feature_element_t *) doc_el)->ops->\
         to_feature_element_free (doc_el, env))

#define AXIS2_WODEN_FEATURE_ELEMENT_TYPE(doc_el, env) \
		(((axis2_woden_feature_element_t *) doc_el)->ops->\
         type (doc_el, env))

#define AXIS2_WODEN_FEATURE_ELEMENT_SET_REF(doc_el, env, uri) \
		(((axis2_woden_feature_element_t *) doc_el)->ops->\
         set_ref(doc_el, env, uri))

#define AXIS2_WODEN_FEATURE_ELEMENT_GET_REF(doc_el, env) \
		(((axis2_woden_feature_element_t *) doc_el)->ops->\
         get_ref(doc_el, env))

#define AXIS2_WODEN_FEATURE_ELEMENT_SET_REQUIRED(doc_el, env, required) \
		(((axis2_woden_feature_element_t *) doc_el)->ops->\
         set_required(doc_el, env, required))

#define AXIS2_WODEN_FEATURE_ELEMENT_IS_REQUIRED(doc_el, env) \
		(((axis2_woden_feature_element_t *) doc_el)->ops->\
         is_required(doc_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_FEATURE_ELEMENT_H */
