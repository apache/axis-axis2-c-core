/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WODEN_SVC_H
#define WODEN_SVC_H

/**
 * @file woden_svc.h
 * @brief Axis2 Service Interface
 * This class represents the Service component and the 
 * &lt;service&gt; element.
 * 
 */

#include <woden.h>
#include <woden_svc_element.h>
#include <woden_wsdl_obj.h>
#include <woden_configurable.h>
#include <woden_configurable_component.h>
#include <woden_nc_name.h>

/** @defgroup woden_svc Service
  * @ingroup woden
  * @{
  */

typedef union woden_svc_base woden_svc_base_t;
typedef struct woden_svc woden_svc_t;
typedef struct woden_svc_ops woden_svc_ops_t;
struct woden_documentation_element;
struct woden_wsdl_component;
struct woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_svc_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *svc,
            const axutil_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *svc,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *svc,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_configurable *(AXIS2_CALL *
    get_base_impl) (
            void *svc,
            const axutil_env_t *env);
    /* ************************************************************
     *  Service interface methods (the WSDL Component model)
     * ************************************************************/

    axis2_qname_t *(AXIS2_CALL *
    get_qname) (
            void *svc,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_interface) (
            void *svc,
            const axutil_env_t *env);

    axutil_array_list_t *(AXIS2_CALL *
    get_endpoints) (
            void *svc,
            const axutil_env_t *env);
    /******************************************************************************
     *  Non-API implementation methods
     *****************************************************************************/  

    axis2_status_t (AXIS2_CALL *
    set_interface_element) (
            void *svc,
            const axutil_env_t *env,
            void *winterface);


};

union woden_svc_base
{
    woden_configurable_t configurable;
    woden_svc_element_t svc_element;
    woden_configurable_component_t configurable_component;
};

struct woden_svc
{
    woden_svc_base_t base;
    woden_svc_ops_t *ops;
};

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_svc_element(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_documentable_element(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_configurable(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_configurable_component(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_wsdl_component(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_configurable_element(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_documentable(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_attr_extensible(
        void *svc,
        const axutil_env_t *env);

AXIS2_EXTERN woden_svc_t * AXIS2_CALL
woden_svc_to_element_extensible(
        void *svc,
        const axutil_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_svc_resolve_methods(
        woden_svc_t *svc,
        const axutil_env_t *env,
        woden_svc_t *svc_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_SVC_FREE(svc, env) \
      (((woden_svc_t *) svc)->ops->free(svc, env))

#define WODEN_SVC_SUPER_OBJS(svc, env) \
      (((woden_svc_t *) svc)->ops->super_objs(svc, env))

#define WODEN_SVC_TYPE(svc, env) \
      (((woden_svc_t *) svc)->ops->type(svc, env))

#define WODEN_SVC_GET_BASE_IMPL(svc, env) \
      (((woden_svc_t *) svc)->ops->get_base_impl(svc, env))

#define WODEN_SVC_GET_QNAME(svc, env) \
      (((woden_svc_t *) svc)->ops->\
         get_qname(svc, env))

#define WODEN_SVC_GET_INTERFACE(svc, env) \
      (((woden_svc_t *) svc)->ops->\
         get_interface(svc, env))

#define WODEN_SVC_GET_ENDPOINTS(svc, env) \
      (((woden_svc_t *) svc)->ops->\
         get_endpoints(svc, env))

#define WODEN_SVC_SET_INTERFACE_ELEMENT(svc, env, interface) \
      (((woden_svc_t *) svc)->ops->\
         set_interface_element(svc, env, interface))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_SVC_H */
