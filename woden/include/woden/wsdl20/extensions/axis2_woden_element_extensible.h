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

#ifndef AXIS2_WODEN_ELEMENT_EXTENSIBLE_H
#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_H

/**
 * @file axis2_woden_element_extensible.h
 * @brief Axis2 Element Extensible Interface
 *          Common code for handling extension elements. 
 *          Can be reused by inheritance or by delegation.
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <axis2_array_list.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_element_extensible axis2_woden_element_extensible_t;
typedef struct axis2_woden_element_extensible_ops axis2_woden_element_extensible_ops_t;
struct axis2_woden_ext_element;

/** @defgroup axis2_woden_element_extensible Element Extensible
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_element_extensible_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *
            element_extensible,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *
            element_extensible,
            axis2_env_t **env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *element_extensible,
            axis2_env_t **env);
 
    axis2_status_t (AXIS2_CALL *
    add_ext_element) (
            void *extensible,
            axis2_env_t **env,
            struct axis2_woden_ext_element *ext_el); 

    axis2_status_t (AXIS2_CALL *
    remove_ext_element) (
            void *extensible,
            axis2_env_t **env,
            struct axis2_woden_ext_element *ext_el); 

    axis2_array_list_t *(AXIS2_CALL *
    get_ext_elements) (
            void *extensible,
            axis2_env_t **env); 

    axis2_array_list_t *(AXIS2_CALL *
    get_ext_elements_of_type) (
            void *extensible,
            axis2_env_t **env,
            axis2_qname_t *ext_type);

    axis2_bool_t (AXIS2_CALL *
    has_ext_elements_for_namespace) (
            void *extensible,
            axis2_env_t **env,
            axis2_url_t *namespc);
 
};

struct axis2_woden_element_extensible
{
    axis2_woden_element_extensible_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_element_extensible_t *)
axis2_woden_element_extensible_create(axis2_env_t **env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_DECLARE(axis2_status_t)
axis2_woden_element_extensible_resolve_methods(
        axis2_woden_element_extensible_t *extensible,
        axis2_env_t **env,
        axis2_woden_element_extensible_t *extensible_impl,
        axis2_hash_t *methods);

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_FREE(extensible, env) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         free (extensible, env))

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_SUPER_OBJS(extensible, env) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         super_objs (extensible, env))

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_TYPE(extensible, env) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         type (extensible, env))

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_ADD_EXT_ELEMENT(extensible, env, ext_el) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         add_ext_element(extensible, env, ext_el))

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_REMOVE_EXT_ELEMENT(extensible, env, \
        ext_el) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         remove_ext_element(extensible, env, ext_el))

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS(extensible, env) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         get_ext_elements(extensible, env))

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(extensible, \
        env, ext_type) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         get_ext_elements_of_type(extensible, env, ext_type))

#define AXIS2_WODEN_ELEMENT_EXTENSIBLE_HAS_EXT_ELEMENTS_FOR_NAMESPACE(\
        extensible, env, namespc) \
		(((axis2_woden_element_extensible_t *) extensible)->ops->\
         has_ext_elements_for_namespace(extensible, env, namespc))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_ELEMENT_EXTENSIBLE_H */
