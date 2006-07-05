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

#ifndef WODEN_ENDPOINT_H
#define WODEN_ENDPOINT_H

/**
 * @file woden_endpoint.h
 * @brief Axis2 Endpoint Interface
 * This class represents the Endpoint component and the &lt;endpoint&gt; element.
 * 
 */

#include <woden.h>
#include <woden_endpoint_element.h>
#include <woden_nested_configurable.h>
#include <woden_nested_component.h>
#include <woden_configurable_component.h>
#include <woden_nc_name.h>

/** @defgroup woden_endpoint Endpoint
  * @ingroup woden
  * @{
  */

typedef union woden_endpoint_base woden_endpoint_base_t;
typedef struct woden_endpoint woden_endpoint_t;
typedef struct woden_endpoint_ops woden_endpoint_ops_t;
struct woden_documentation_element;
struct woden_wsdl_component;
struct woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_endpoint_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *endpoint,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *endpoint,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *endpoint,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *endpoint,
            const axis2_env_t *env);

    /* ************************************************************
     *  Endpoint interface methods (the WSDL Component model)
     * ************************************************************/

    woden_nc_name_t *(AXIS2_CALL *
    get_name) (
            void *endpoint,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_binding) (
            void *endpoint,
            const axis2_env_t *env);

    axis2_uri_t *(AXIS2_CALL *
    get_address) (
            void *endpoint,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_binding_element) (
        void *endpoint,
        const axis2_env_t *env,
        void *binding);

};

union woden_endpoint_base
{
    woden_nested_configurable_t nested_configurable;
    woden_endpoint_element_t endpoint_element;
    woden_configurable_component_t configurable_component;
    woden_configurable_component_t nested_component;
};

struct woden_endpoint
{
    woden_endpoint_base_t base;
    woden_endpoint_ops_t *ops;
};

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_endpoint_element(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_nested_element(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_documentable_element(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_nested_configurable(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_configurable(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_nested_component(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_configurable_component(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_wsdl_component(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_configurable_element(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_documentable(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_attr_extensible(
        void *endpoint,
        const axis2_env_t *env);

AXIS2_EXTERN woden_endpoint_t * AXIS2_CALL
woden_endpoint_to_element_extensible(
        void *endpoint,
        const axis2_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_endpoint_resolve_methods(
        woden_endpoint_t *endpoint,
        const axis2_env_t *env,
        woden_endpoint_t *endpoint_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_ENDPOINT_FREE(endpoint, env) \
      (((woden_endpoint_t *) endpoint)->ops->free(endpoint, env))

#define WODEN_ENDPOINT_SUPER_OBJS(endpoint, env) \
      (((woden_endpoint_t *) endpoint)->ops->super_objs(endpoint, env))

#define WODEN_ENDPOINT_TYPE(endpoint, env) \
      (((woden_endpoint_t *) endpoint)->ops->type(endpoint, env))

#define WODEN_ENDPOINT_GET_BASE_IMPL(endpoint, env) \
      (((woden_endpoint_t *) endpoint)->ops->get_base_impl(endpoint, env))

#define WODEN_ENDPOINT_GET_NAME(endpoint, env) \
      (((woden_endpoint_t *) endpoint)->ops->\
         get_name(endpoint, env))

#define WODEN_ENDPOINT_GET_BINDING(endpoint, env) \
      (((woden_endpoint_t *) endpoint)->ops->\
         get_binding(endpoint, env))

#define WODEN_ENDPOINT_GET_ADDRESS(endpoint, env) \
      (((woden_endpoint_t *) endpoint)->ops->\
         get_address(endpoint, env))

#define WODEN_ENDPOINT_SET_BINDING_ELEMENT(endpoint, env, binding) \
      (((woden_endpoint_t *) endpoint)->ops->\
         set_binding_element(endpoint, env, binding))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_ENDPOINT_H */
