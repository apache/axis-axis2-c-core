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

#ifndef AXIS2_WODEN_ENDPOINT_H
#define AXIS2_WODEN_ENDPOINT_H

/**
 * @file axis2_woden_endpoint.h
 * @brief Axis2 Endpoint Interface
 * This class represents the Endpoint component and the &lt;endpoint&gt; element.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_endpoint_element.h>
#include <woden/wsdl20/axis2_woden_nested_configurable.h>
#include <woden/wsdl20/axis2_woden_nested_component.h>
#include <woden/wsdl20/axis2_woden_configurable_component.h>
#include <woden/types/woden_nc_name.h>

/** @defgroup axis2_woden_endpoint Endpoint
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_endpoint_base axis2_woden_endpoint_base_t;
typedef struct axis2_woden_endpoint axis2_woden_endpoint_t;
typedef struct axis2_woden_endpoint_ops axis2_woden_endpoint_ops_t;
struct axis2_woden_documentation_element;
struct axis2_woden_wsdl_component;
struct axis2_woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_endpoint_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *endpoint,
            axis2_env_t **env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *endpoint,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *endpoint,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_nested_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *endpoint,
            axis2_env_t **env);

    /* ************************************************************
     *  Endpoint interface methods (the WSDL Component model)
     * ************************************************************/

    woden_nc_name_t *(AXIS2_CALL *
    get_name) (
            void *endpoint,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_binding) (
            void *endpoint,
            axis2_env_t **env);

    axis2_url_t *(AXIS2_CALL *
    get_address) (
            void *endpoint,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_binding_element) (
        void *endpoint,
        axis2_env_t **env,
        void *binding);

};

union axis2_woden_endpoint_base
{
    axis2_woden_nested_configurable_t nested_configurable;
    axis2_woden_endpoint_element_t endpoint_element;
    axis2_woden_configurable_component_t configurable_component;
    axis2_woden_configurable_component_t nested_component;
};

struct axis2_woden_endpoint
{
    axis2_woden_endpoint_base_t base;
    axis2_woden_endpoint_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_endpoint_element(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_nested_element(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_documentable_element(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_nested_configurable(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_configurable(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_nested_component(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_configurable_component(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_endpoint_t *)
axis2_woden_endpoint_to_wsdl_component(
        void *endpoint,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_endpoint_resolve_methods(
        axis2_woden_endpoint_t *endpoint,
        axis2_env_t **env,
        axis2_woden_endpoint_t *endpoint_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_ENDPOINT_FREE(endpoint, env) \
		(((axis2_woden_endpoint_t *) endpoint)->ops->free(endpoint, env))

#define AXIS2_WODEN_ENDPOINT_SUPER_OBJS(endpoint, env) \
		(((axis2_woden_endpoint_t *) endpoint)->ops->super_objs(endpoint, env))

#define AXIS2_WODEN_ENDPOINT_TYPE(endpoint, env) \
		(((axis2_woden_endpoint_t *) endpoint)->ops->type(endpoint, env))

#define AXIS2_WODEN_ENDPOINT_GET_BASE_IMPL(endpoint, env) \
		(((axis2_woden_endpoint_t *) endpoint)->ops->get_base_impl(endpoint, env))

#define AXIS2_WODEN_ENDPOINT_GET_NAME(endpoint, env) \
		(((axis2_woden_endpoint_t *) endpoint)->\
         get_name(endpoint, env))

#define AXIS2_WODEN_ENDPOINT_GET_BINDING(endpoint, env) \
		(((axis2_woden_endpoint_t *) endpoint)->\
         get_binding(endpoint, env))

#define AXIS2_WODEN_ENDPOINT_GET_ADDRESS(endpoint, env) \
		(((axis2_woden_endpoint_t *) endpoint)->ops->\
         get_address(endpoint, env))

#define AXIS2_WODEN_ENDPOINT_SET_BINDING_ELEMENT(endpoint, env, binding) \
		(((axis2_woden_endpoint_t *) endpoint)->ops->\
         set_binding_element(endpoint, env, binding))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_ENDPOINT_H */
