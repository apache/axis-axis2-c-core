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

#ifndef AXIS2_WODEN_SVC_ELEMENT_H
#define AXIS2_WODEN_SVC_ELEMENT_H

/**
 * @file axis2_woden_endpoint_element.h
 * @brief Axis2 Endpoint Element Interface
 * This interface represents an &lt;endpoint&gt; element 
 * information item, a child element of WSDL &lt;service&gt;.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_documentable_element.h>
#include <woden/wsdl20/xml/axis2_woden_configurable_element.h>
#include <woden/wsdl20/xml/axis2_woden_nested_element.h>
#include <woden/types/woden_nc_name.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_endpoint_element axis2_woden_endpoint_element_t;
typedef struct axis2_woden_endpoint_element_ops axis2_woden_endpoint_element_ops_t;
typedef union axis2_woden_endpoint_element_base axis2_woden_endpoint_element_base_t;

/** @defgroup axis2_woden_endpoint_element Endpoint Element
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_endpoint_element_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *endpoint_el,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    to_endpoint_element_free) (
            void *endpoint_el,
            axis2_env_t **env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *endpoint_el,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_name) (
            void *endpoint_el,
            axis2_env_t **env,
            woden_nc_name_t *name);

    woden_nc_name_t *(AXIS2_CALL *
    get_name) (
            void *endpoint_el,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_binding_qname) (
            void *endpoint_el,
            axis2_env_t **env,
            axis2_qname_t *qname);

    axis2_qname_t *(AXIS2_CALL *
    get_binding_qname) (
            void *endpoint_el,
            axis2_env_t **env);

    void *(AXIS2_CALL *
    get_binding_element) (
            void *endpoint_el,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_address) (
            void *endpoint_el,
            axis2_env_t **env,
            axis2_url_t *uri);

    axis2_url_t *(AXIS2_CALL *
    get_address) (
            void *endpoint_el,
            axis2_env_t **env);

};

union axis2_woden_endpoint_element_base
{
    axis2_woden_documentable_element_t documentable_element;
    axis2_woden_configurable_element_t configurable_element;
    axis2_woden_nested_element_t nested_element;
};

struct axis2_woden_endpoint_element
{
    axis2_woden_endpoint_element_base_t base;
    axis2_woden_endpoint_element_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
axis2_woden_endpoint_element_resolve_methods(
        axis2_woden_endpoint_element_t *endpoint_element,
        axis2_env_t **env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_SVC_ELEMENT_FREE(endpoint_el, env) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         free (endpoint_el, env))

#define AXIS2_WODEN_SVC_ELEMENT_TO_SVC_ELEMENT_FREE(endpoint_el, env) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         to_endpoint_element_free (endpoint_el, env))

#define AXIS2_WODEN_SVC_ELEMENT_TYPE(endpoint_el, env) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         type (endpoint_el, env))

#define AXIS2_WODEN_SVC_ELEMENT_SET_NAME(endpoint_el, env, name) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         set_name(endpoint_el, env, name))

#define AXIS2_WODEN_SVC_ELEMENT_GET_NAME(endpoint_el, env) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         get_name(endpoint_el, env))

#define AXIS2_WODEN_SVC_ELEMENT_SET_BINDING_QNAME(endpoint_el, env, qname) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         set_binding_qname(endpoint_el, env, qname))

#define AXIS2_WODEN_SVC_ELEMENT_GET_BINDING_QNAME(endpoint_el, env) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         get_binding_qname(endpoint_el, env))

#define AXIS2_WODEN_SVC_ELEMENT_GET_BINDING_ELEMENT(endpoint_el, env) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         get_binding_element(endpoint_el, env))

#define AXIS2_WODEN_SVC_ELEMENT_SET_ADDRESS(endpoint_el, env, uri) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         set_address(endpoint_el, env, uri))

#define AXIS2_WODEN_SVC_ELEMENT_GET_ADDRESS(endpoint_el, env) \
		(((axis2_woden_endpoint_element_t *) endpoint_el)->ops->\
         get_address(endpoint_el, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_SVC_ELEMENT_H */
