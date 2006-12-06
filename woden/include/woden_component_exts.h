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

#ifndef WODEN_COMPONENT_EXTS_H
#define WODEN_COMPONENT_EXTS_H

/**
 * @file woden_component_exts.h
 * @brief Axis2 Component Extensions Interface
 * This interface represents a group of properties that extend a <code>wsdl_component</code>.
 * These properties share the same namespace and it is different to the WSDL 2.0 namespace.
 * The XML representation of these properties are the elements and attributes
 * from outside the WSDL 2.0 namespace that extend a WSDL element.
 * For example, the elements and attributes from the SOAP namespace that extend the 
 * WSDL &lt;binding&gt; element are represented by this interface as extension
 * properties of the <code>binding</code> component. 
 * <p>
 * This interface provides a common point of reference to a WSDL component's extension
 * properties that belong to a particular namespace. 
 * The interface does not define any behaviour specific to the individual properties.
 * Woden implementations that need to support WSDL extensions from a particular namespace
 * should implement this interface and add support specific to those extensions.
 * <p>
 * For example, Woden implements this interface to support the SOAP and HTTP binding
 * extensions defined in the W3C WSDL 2.0 specification.
 * 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_uri.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_component_exts woden_component_exts_t;
typedef struct woden_component_exts_ops woden_component_exts_ops_t;
struct woden_wsdl_element;

    
/**
 * Namespace URIs for extensions defined by WSDL 2.0 Specification.
 */
#define WODEN_URI_NS_SOAP "http://www.w3.org/2006/01/wsdl/soap"
#define WODEN_URI_NS_HTTP "http://www.w3.org/2006/01/wsdl/http"
    
/** @defgroup woden_component_exts Component Extensions
  * @ingroup woden
  * @{
  */

struct woden_component_exts_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *component_exts,
            const axis2_env_t *env);
      
    axis2_status_t (AXIS2_CALL *
    to_component_exts_free) (void *component_exts,
            const axis2_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (void *component_exts,
            const axis2_env_t *env);
    
    axis2_uri_t *(AXIS2_CALL *
    get_namespace) (
            void *component_exts,
            const axis2_env_t *env);

    axis2_status_t (AXIS2_CALL *
    init) (
            void *component_exts,
            const axis2_env_t *env,
            struct woden_wsdl_element *parent_el,
            axis2_uri_t *namespc);

    void *(AXIS2_CALL *
    get_parent_element) (
           void *component_exts,
            const axis2_env_t *env);
};

struct woden_component_exts
{
    woden_component_exts_ops_t *ops;
};

AXIS2_EXTERN woden_component_exts_t * AXIS2_CALL
woden_component_exts_create(const axis2_env_t *env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_component_exts_resolve_methods(
        woden_component_exts_t *component_exts,
        const axis2_env_t *env,
        axis2_hash_t *methods);

#define WODEN_COMPONENT_EXTS_FREE(component_exts, env) \
      (((woden_component_exts_t *) component_exts)->ops->\
         free (component_exts, env))

#define WODEN_COMPONENT_EXTS_TO_COMPONENT_EXTS_FREE(component_exts, env) \
      (((woden_component_exts_t *) component_exts)->ops->\
         to_component_exts_free (component_exts, env))

#define WODEN_COMPONENT_EXTS_TYPE(component_exts, env) \
      (((woden_component_exts_t *) component_exts)->ops->\
         type (component_exts, env))

#define WODEN_COMPONENT_EXTS_INIT(component_exts, env, parent_el, namespc) \
      (((woden_component_exts_t *) component_exts)->ops->\
         init(component_exts, env, \
            parent_el, namespc))

#define WODEN_COMPONENT_EXTS_GET_NAMESPACE(component_exts, env) \
      (((woden_component_exts_t *) component_exts)->ops->\
         get_namespace(component_exts, env))

#define WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(component_exts, env) \
      (((woden_component_exts_t *) component_exts)->ops->\
         get_parent_element(component_exts, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_COMPONENT_EXTS_H */
