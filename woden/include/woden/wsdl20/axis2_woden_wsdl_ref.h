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

#ifndef AXIS2_WODEN_WSDL_REF_H
#define AXIS2_WODEN_WSDL_REF_H

/**
 * @file axis2_woden_wsdl_ref.h
 * @brief Axis2 Wsdl Ref Interface
 * This abstract class defines the common behaviour for referencing WSDL
 * documents via the &lt;wsdl:import&gt; and &lt;wsdl:include&gt; elements.
 * It is extended by the concrete implementation classes for those two elements.
 * 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_wsdl_element.h>
#include <woden/wsdl20/axis2_woden_wsdl_component.h>

/** @defgroup axis2_woden_wsdl_ref Wsdl Ref
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_wsdl_ref_base axis2_woden_wsdl_ref_base_t;
typedef struct axis2_woden_wsdl_ref axis2_woden_wsdl_ref_t;
typedef struct axis2_woden_wsdl_ref_ops axis2_woden_wsdl_ref_ops_t;
struct axis2_woden_component_exts;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_wsdl_ref_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *wsdl_ref,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *wsdl_ref,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *wsdl_ref,
            const axis2_env_t *env);

    /**
     * @return the base implementation class
     */
    axis2_woden_wsdl_element_t *(AXIS2_CALL *
    get_base_impl) (
            void *wsdl_ref,
            const axis2_env_t *env);

    axis2_uri_t *(AXIS2_CALL *
    get_location) (
            void *wsdl_ref,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_location) (
            void *wsdl_ref,
            const axis2_env_t *env,
            axis2_uri_t *loc_uri);

    void *(AXIS2_CALL *
    get_desc_element) (
            void *wsdl_ref,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_desc_element) (
            void *wsdl_ref,
            const axis2_env_t *env,
            void *desc);

    axis2_status_t (AXIS2_CALL *
    add_documentation_element) (
            void *wsdl_ref,
            const axis2_env_t *env,
            void *doc_el);

    axis2_array_list_t *(AXIS2_CALL *
    get_documentation_elements) (
            void *wsdl_ref,
            const axis2_env_t *env);


};

struct axis2_woden_wsdl_ref
{
    axis2_woden_wsdl_element_t wsdl_element;
    axis2_woden_wsdl_ref_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_wsdl_ref_t * AXIS2_CALL
axis2_woden_wsdl_ref_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_wsdl_ref_resolve_methods(
        axis2_woden_wsdl_ref_t *wsdl_ref,
        const axis2_env_t *env,
        axis2_woden_wsdl_ref_t *wsdl_ref_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_WSDL_REF_FREE(wsdl_ref, env) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->free(wsdl_ref, env))

#define AXIS2_WODEN_WSDL_REF_SUPER_OBJS(wsdl_ref, env) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->super_objs(wsdl_ref, env))

#define AXIS2_WODEN_WSDL_REF_TYPE(wsdl_ref, env) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->type(wsdl_ref, env))

#define AXIS2_WODEN_WSDL_REF_GET_BASE_IMPL(wsdl_ref, env) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->get_base_impl(wsdl_ref, env))

#define AXIS2_WODEN_WSDL_REF_GET_LOCATION(wsdl_ref, env) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->\
        get_location (wsdl_ref, env))

#define AXIS2_WODEN_WSDL_REF_SET_LOCATION(wsdl_ref, env, loc_uri) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->\
         set_location(wsdl_ref, env, loc_uri))

#define AXIS2_WODEN_WSDL_REF_GET_DESC_ELEMENT(wsdl_ref, env) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->\
        get_desc_element (wsdl_ref, env))

#define AXIS2_WODEN_WSDL_REF_SET_DESC_ELEMENT(wsdl_ref, env, desc) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->\
         set_desc_element(wsdl_ref, env, desc))

#define AXIS2_WODEN_WSDL_REF_ADD_DOCUMENTATION_ELEMENT(wsdl_ref, env, doc_el) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->\
        add_documentation_element (wsdl_ref, env, doc_el))

#define AXIS2_WODEN_WSDL_REF_GET_DOCUMENTATION_ELEMENTS(wsdl_ref, env) \
      (((axis2_woden_wsdl_ref_t *) wsdl_ref)->ops->\
         get_documentation_elements(wsdl_ref, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_WSDL_REF_H */
