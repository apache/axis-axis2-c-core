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

#ifndef WODEN_SOAP_MODULE_H
#define WODEN_SOAP_MODULE_H

/**
 * @file woden_soap_module.h
 * @brief Axis2 Soap Module Interface
 * This interface represents the &lt;wsoap:header&gt; extension element that 
 * can appear within a Binding Fault or Binding Message Reference.
 * 
 */

#include <woden.h>
#include <woden_soap_module_element.h>

/** @defgroup woden_soap_module Soap Module
  * @ingroup woden
  * @{
  */

typedef union woden_soap_module_base woden_soap_module_base_t;
typedef struct woden_soap_module woden_soap_module_t;
typedef struct woden_soap_module_ops woden_soap_module_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_soap_module_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL * 
    free) (
            void *module,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL * 
    super_objs) (
            void *module,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL * 
    type) (
            void *module,
            const axutil_env_t *env);

    struct woden_component_exts *(AXIS2_CALL *
    get_base_impl) (
            void *module,
            const axutil_env_t *env);


    /* ***********************************************************************
     *  Component model methods (SOAPHeaderBlock interface), some shared with Element model
     * ***********************************************************************/

    axutil_uri_t *(AXIS2_CALL *
    get_ref) (
            void *module,
            axutil_env_t *env);

    axis2_bool_t (AXIS2_CALL * 
    is_required) (
            void *module,
            axutil_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *module,
            axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element) (
            void *module,
            axutil_env_t *env);

};

struct woden_soap_module
{
    woden_soap_module_element_t module_element;
    woden_soap_module_ops_t *ops;
};

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_soap_module_element(
        void *module,
        const axutil_env_t *env);

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_ext_element(
        void *module,
        const axutil_env_t *env);

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_attr_extensible(
        void *module,
        const axutil_env_t *env);

AXIS2_EXTERN woden_soap_module_t * AXIS2_CALL
woden_soap_module_to_element_extensible(
        void *module,
        const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_soap_module_resolve_methods(
        woden_soap_module_t *soap_module,
        const axutil_env_t *env,
        woden_soap_module_t *soap_module_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_SOAP_MODULE_FREE(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->ops->free(soap_module, env))

#define WODEN_SOAP_MODULE_SUPER_OBJS(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->ops->super_objs(soap_module, env))

#define WODEN_SOAP_MODULE_TYPE(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->ops->type(soap_module, env))

#define WODEN_SOAP_MODULE_GET_BASE_IMPL(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->ops->get_base_impl(soap_module, env))

#define WODEN_SOAP_MODULE_GET_REF(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->\
        get_ref (soap_module, env))

#define WODEN_SOAP_MODULE_IS_REQUIRED(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->\
        is_required (soap_module, env))

#define WODEN_SOAP_MODULE_GET_PARENT(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->\
        get_parent (soap_module, env))

#define WODEN_SOAP_MODULE_TO_ELEMENT(soap_module, env) \
      (((woden_soap_module_t *) soap_module)->\
        to_element (soap_module, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_SOAP_MODULE_H */
