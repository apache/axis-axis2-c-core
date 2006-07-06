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

#ifndef WODEN_IMPORT_H
#define WODEN_IMPORT_H

/**
 * @file woden_import.h
 * @brief Axis2 Import Interface
 * This class implements the &lt;wsdl:import&gt; element. 
 * 
 */

#include <woden.h>
#include <woden_import_element.h>
#include <woden_wsdl_obj.h>
#include <woden_wsdl_ref.h>

/** @defgroup woden_import Import
  * @ingroup woden
  * @{
  */

typedef union woden_import_base woden_import_base_t;
typedef struct woden_import woden_import_t;
typedef struct woden_import_ops woden_import_ops_t;
struct woden_wsdl_ref;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_import_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *import,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *import,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *import,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_wsdl_ref *(AXIS2_CALL *
    get_base_impl) (
            void *import,
            const axis2_env_t *env);

    axis2_uri_t *(AXIS2_CALL *
    get_namespace) (
            void *import,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_namespace) (
            void *import,
            const axis2_env_t *env,
            axis2_uri_t *ns_uri);

};

union woden_import_base
{
    woden_import_element_t import_element;
    woden_wsdl_ref_t wsdl_ref;
};

struct woden_import
{
    woden_import_base_t base;
    woden_import_ops_t *ops;
};

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_import_element(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_wsdl_ref(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_attr_extensible(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN woden_import_t * AXIS2_CALL
woden_import_to_element_extensible(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_import_resolve_methods(
        woden_import_t *import,
        const axis2_env_t *env,
        woden_import_t *import_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_IMPORT_FREE(import, env) \
      (((woden_import_t *) import)->ops->free(import, env))

#define WODEN_IMPORT_SUPER_OBJS(import, env) \
      (((woden_import_t *) import)->ops->super_objs(import, env))

#define WODEN_IMPORT_TYPE(import, env) \
      (((woden_import_t *) import)->ops->type(import, env))

#define WODEN_IMPORT_GET_BASE_IMPL(import, env) \
      (((woden_import_t *) import)->ops->get_base_impl(import, env))

#define WODEN_IMPORT_GET_NAMESPACE(import, env) \
      (((woden_import_t *) import)->ops->\
         get_namespace(import, env))

#define WODEN_IMPORT_SET_NAMESPACE(import, env, ns_uri) \
      (((woden_import_t *) import)->ops->\
       set_namespace(import, env, ns_uri))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_IMPORT_H */
