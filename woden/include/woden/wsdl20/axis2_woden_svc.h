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

#ifndef AXIS2_WODEN_SVC_H
#define AXIS2_WODEN_SVC_H

/**
 * @file axis2_woden_svc.h
 * @brief Axis2 Service Interface
 * This class represents the Service component and the 
 * &lt;service&gt; element.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_svc_element.h>
#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_configurable.h>
#include <woden/wsdl20/axis2_woden_configurable_component.h>
#include <woden/types/woden_nc_name.h>

/** @defgroup axis2_woden_svc Service
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_svc_base axis2_woden_svc_base_t;
typedef struct axis2_woden_svc axis2_woden_svc_t;
typedef struct axis2_woden_svc_ops axis2_woden_svc_ops_t;
struct axis2_woden_documentation_element;
struct axis2_woden_wsdl_component;
struct axis2_woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_svc_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *svc,
            axis2_env_t **env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *svc,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *svc,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *svc,
            axis2_env_t **env);
    /* ************************************************************
     *  Service interface methods (the WSDL Component model)
     * ************************************************************/

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *svc,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_interface) (
            void *svc,
            axis2_env_t **env);

    axis2_array_list_t *(AXIS2_CALL *
    get_endpoints) (
            void *svc,
            axis2_env_t **env);
    /******************************************************************************
     *  Non-API implementation methods
     *****************************************************************************/  

    axis2_status_t (AXIS2_CALL *
    set_interface_element) (
            void *svc,
            axis2_env_t **env,
            void *interface);


};

union axis2_woden_svc_base
{
    axis2_woden_configurable_t configurable;
    axis2_woden_svc_element_t svc_element;
    axis2_woden_configurable_component_t configurable_component;
};

struct axis2_woden_svc
{
    axis2_woden_svc_base_t base;
    axis2_woden_svc_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_svc_element(
        void *svc,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_documentable_element(
        void *svc,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_configurable(
        void *svc,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_configurable_component(
        void *svc,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_wsdl_component(
        void *svc,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_svc_resolve_methods(
        axis2_woden_svc_t *svc,
        axis2_env_t **env,
        axis2_woden_svc_t *svc_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SVC_FREE(svc, env) \
		(((axis2_woden_svc_t *) svc)->ops->free(svc, env))

#define AXIS2_WODEN_SVC_SUPER_OBJS(svc, env) \
		(((axis2_woden_svc_t *) svc)->ops->super_objs(svc, env))

#define AXIS2_WODEN_SVC_TYPE(svc, env) \
		(((axis2_woden_svc_t *) svc)->ops->type(svc, env))

#define AXIS2_WODEN_SVC_GET_BASE_IMPL(svc, env) \
		(((axis2_woden_svc_t *) svc)->ops->get_base_impl(svc, env))

#define AXIS2_WODEN_SVC_GET_QNAME(svc, env) \
		(((axis2_woden_svc_t *) svc)->\
         get_qname(svc, env))

#define AXIS2_WODEN_SVC_GET_INTERFACE(svc, env) \
		(((axis2_woden_svc_t *) svc)->\
         get_interface(svc, env))

#define AXIS2_WODEN_SVC_GET_ENDPOINTS(svc, env) \
		(((axis2_woden_svc_t *) svc)->ops->\
         get_endpoints(svc, env))

#define AXIS2_WODEN_SVC_SET_INTERFACE_ELEMENT(svc, env, interface) \
		(((axis2_woden_svc_t *) svc)->ops->\
         set_interface_element(svc, env, interface))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SVC_H */
