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

#ifndef AXIS2_WODEN_SOAP_MODULE_H
#define AXIS2_WODEN_SOAP_MODULE_H

/**
 * @file axis2_woden_soap_module.h
 * @brief Axis2 Soap Module Interface
 * This interface represents the &lt;wsoap:header&gt; extension element that 
 * can appear within a Binding Fault or Binding Message Reference.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/extensions/soap/axis2_woden_soap_module_element.h>

/** @defgroup axis2_woden_soap_module Soap Module
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_soap_module_base axis2_woden_soap_module_base_t;
typedef struct axis2_woden_soap_module axis2_woden_soap_module_t;
typedef struct axis2_woden_soap_module_ops axis2_woden_soap_module_ops_t;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_soap_module_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL * 
    free) (
            void *module,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL * 
    super_objs) (
            void *module,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL * 
    type) (
            void *module,
            const axis2_env_t *env);

    struct axis2_woden_component_exts *(AXIS2_CALL *
    get_base_impl) (
            void *module,
            const axis2_env_t *env);


    /* ***********************************************************************
     *  Component model methods (SOAPHeaderBlock interface), some shared with Element model
     * ***********************************************************************/

    axis2_uri_t *(AXIS2_CALL *
    get_ref) (
            void *module,
            axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL * 
    is_required) (
            void *module,
            axis2_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *module,
            axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element) (
            void *module,
            axis2_env_t *env);

};

union axis2_woden_soap_module_base
{
};

struct axis2_woden_soap_module
{
    axis2_woden_soap_module_element_t module_element;
    axis2_woden_soap_module_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_soap_module_t * AXIS2_CALL
axis2_woden_soap_module_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN axis2_woden_soap_module_t * AXIS2_CALL
axis2_woden_soap_module_to_soap_module_element(
        void *module,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_soap_module_t * AXIS2_CALL
axis2_woden_soap_module_to_ext_element(
        void *module,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_soap_module_t * AXIS2_CALL
axis2_woden_soap_module_to_attr_extensible(
        void *module,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_soap_module_t * AXIS2_CALL
axis2_woden_soap_module_to_element_extensible(
        void *module,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_soap_module_resolve_methods(
        axis2_woden_soap_module_t *soap_module,
        const axis2_env_t *env,
        axis2_woden_soap_module_t *soap_module_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SOAP_MODULE_FREE(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->ops->free(soap_module, env))

#define AXIS2_WODEN_SOAP_MODULE_SUPER_OBJS(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->ops->super_objs(soap_module, env))

#define AXIS2_WODEN_SOAP_MODULE_TYPE(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->ops->type(soap_module, env))

#define AXIS2_WODEN_SOAP_MODULE_GET_BASE_IMPL(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->ops->get_base_impl(soap_module, env))

#define AXIS2_WODEN_SOAP_MODULE_GET_REF(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->\
        get_ref (soap_module, env))

#define AXIS2_WODEN_SOAP_MODULE_IS_REQUIRED(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->\
        is_required (soap_module, env))

#define AXIS2_WODEN_SOAP_MODULE_GET_PARENT(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->\
        get_parent (soap_module, env))

#define AXIS2_WODEN_SOAP_MODULE_TO_ELEMENT(soap_module, env) \
      (((axis2_woden_soap_module_t *) soap_module)->\
        to_element (soap_module, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SOAP_MODULE_H */
