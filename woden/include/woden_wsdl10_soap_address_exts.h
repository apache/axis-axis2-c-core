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

#ifndef WODEN_WSDL10_SOAP_ADDRESS_EXTS_H
#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_H

/**
 * @file woden_wsdl10_soap_address_exts.h
 * @brief Axis2 Soap Binding Operation Extensions Interface
 * This interface represents the properties from the SOAP namespace
 * added to the WSDL 2.0 <code>address</code> component as part 
 * of the SOAP binding extension.
 * <p>
 * These include:
 * <ul>
 * <li>{soap mep}</li>
 * <li>{soap address}</li>
 * <li>{soap modules}</li>
 * </ul> 
 * 
 */

#include <woden.h>
#include <woden_component_exts.h>
#include <axis2_array_list.h>

/** @defgroup woden_wsdl10_soap_address_exts Soap Binding Operation Extensions
  * @ingroup woden
  * @{
  */

typedef union woden_wsdl10_soap_address_exts_base woden_wsdl10_soap_address_exts_base_t;
typedef struct woden_wsdl10_soap_address_exts woden_wsdl10_soap_address_exts_t;
typedef struct woden_wsdl10_soap_address_exts_ops woden_wsdl10_soap_address_exts_ops_t;


#ifdef __cplusplus
extern "C"
{
#endif

struct woden_wsdl10_soap_address_exts_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *soap_address_exts,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *soap_address_exts,
            const axis2_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *soap_address_exts,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_component_exts *(AXIS2_CALL *
    get_base_impl) (
            void *soap_address_exts,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    set_soap_address) (
            void *address_exts,
            const axis2_env_t *env,
            axis2_uri_t *address);

    axis2_uri_t *(AXIS2_CALL *
    get_soap_address) (
            void *address_exts,
            const axis2_env_t *env); 

    axis2_array_list_t *(AXIS2_CALL *
    get_soap_modules) (
            void *address_exts,
            const axis2_env_t *env);

};

struct woden_wsdl10_soap_address_exts
{
    woden_component_exts_t component_exts;
    woden_wsdl10_soap_address_exts_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl10_soap_address_exts_t * AXIS2_CALL
woden_wsdl10_soap_address_exts_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_wsdl10_soap_address_exts_t * AXIS2_CALL
woden_wsdl10_soap_address_exts_to_component_exts(
        void *soap_address_exts,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl10_soap_address_exts_resolve_methods(
        woden_wsdl10_soap_address_exts_t *soap_address_exts,
        const axis2_env_t *env,
        woden_wsdl10_soap_address_exts_t *soap_address_exts_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_FREE(soap_address_exts, env) \
      (((woden_wsdl10_soap_address_exts_t *) soap_address_exts)->ops->free(soap_address_exts, env))

#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_SUPER_OBJS(soap_address_exts, env) \
      (((woden_wsdl10_soap_address_exts_t *) soap_address_exts)->ops->super_objs(soap_address_exts, env))

#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_TYPE(soap_address_exts, env) \
      (((woden_wsdl10_soap_address_exts_t *) soap_address_exts)->ops->type(soap_address_exts, env))

#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_GET_BASE_IMPL(soap_address_exts, env) \
      (((woden_wsdl10_soap_address_exts_t *) soap_address_exts)->ops->get_base_impl(soap_address_exts, env))

#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_SET_SOAP_ADDRESS(soap_address_exts, env, address) \
      (((woden_wsdl10_soap_address_exts_t *) soap_address_exts)->ops->\
        set_soap_address (soap_address_exts, env, address))

#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_GET_SOAP_ADDRESS(soap_address_exts, env) \
      (((woden_wsdl10_soap_address_exts_t *) soap_address_exts)->ops->\
        get_soap_address (soap_address_exts, env))

#define WODEN_WSDL10_SOAP_ADDRESS_EXTS_GET_SOAP_MODULES(soap_address_exts, env) \
      (((woden_wsdl10_soap_address_exts_t *) soap_address_exts)->ops->\
        get_soap_modules (soap_address_exts, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL10_SOAP_ADDRESS_EXTS_H */

