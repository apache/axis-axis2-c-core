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

#ifndef AXIS2_WODEN_IMPORT_H
#define AXIS2_WODEN_IMPORT_H

/**
 * @file axis2_woden_import.h
 * @brief Axis2 Import Interface
 * This class implements the &lt;wsdl:import&gt; element. 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_import_element.h>
#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_wsdl_ref.h>

/** @defgroup axis2_woden_import Import
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_import_base axis2_woden_import_base_t;
typedef struct axis2_woden_import axis2_woden_import_t;
typedef struct axis2_woden_import_ops axis2_woden_import_ops_t;
struct axis2_woden_wsdl_ref;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_import_ops
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

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *import,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_wsdl_ref *(AXIS2_CALL *
    get_base_impl) (
            void *import,
            const axis2_env_t *env);

    axis2_url_t *(AXIS2_CALL *
    get_namespace) (
            void *import,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_namespace) (
            void *import,
            const axis2_env_t *env,
            axis2_url_t *ns_uri);

};

union axis2_woden_import_base
{
    axis2_woden_import_element_t import_element;
    axis2_woden_wsdl_ref_t wsdl_ref;
};

struct axis2_woden_import
{
    axis2_woden_import_base_t base;
    axis2_woden_import_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_import_t * AXIS2_CALL
axis2_woden_import_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN axis2_woden_import_t * AXIS2_CALL
axis2_woden_import_to_import_element(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_import_t * AXIS2_CALL
axis2_woden_import_to_wsdl_ref(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_import_t * AXIS2_CALL
axis2_woden_import_to_attr_extensible(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_import_t * AXIS2_CALL
axis2_woden_import_to_element_extensible(
        void *import,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_import_resolve_methods(
        axis2_woden_import_t *import,
        const axis2_env_t *env,
        axis2_woden_import_t *import_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_IMPORT_FREE(import, env) \
		(((axis2_woden_import_t *) import)->ops->free(import, env))

#define AXIS2_WODEN_IMPORT_SUPER_OBJS(import, env) \
		(((axis2_woden_import_t *) import)->ops->super_objs(import, env))

#define AXIS2_WODEN_IMPORT_TYPE(import, env) \
		(((axis2_woden_import_t *) import)->ops->type(import, env))

#define AXIS2_WODEN_IMPORT_GET_BASE_IMPL(import, env) \
		(((axis2_woden_import_t *) import)->ops->get_base_impl(import, env))

#define AXIS2_WODEN_IMPORT_GET_NAMESPACE(import, env) \
		(((axis2_woden_import_t *) import)->\
         get_namespace(import, env))

#define AXIS2_WODEN_IMPORT_SET_NAMESPACE(import, env, ns_uri) \
		(((axis2_woden_import_t *) import)->\
         set_namespace(import, env, ns_uri))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_IMPORT_H */
