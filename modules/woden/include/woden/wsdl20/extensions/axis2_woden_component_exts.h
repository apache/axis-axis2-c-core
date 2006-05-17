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

#ifndef AXIS2_WODEN_COMPONENT_EXTS_H
#define AXIS2_WODEN_COMPONENT_EXTS_H

/**
 * @file axis2_woden_component_exts.h
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
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>
#include <woden/axis2_woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_component_exts axis2_woden_component_exts_t;
typedef struct axis2_woden_component_exts_ops axis2_woden_component_exts_ops_t;
struct axis2_woden_wsdl_element;

    
/**
 * Namespace URIs for extensions defined by WSDL 2.0 Specification.
 */
#define AXIS2_WODEN_URI_NS_SOAP "http://www.w3.org/2006/01/wsdl/soap"
#define AXIS2_WODEN_URI_NS_HTTP "http://www.w3.org/2006/01/wsdl/http"
    
/** @defgroup axis2_woden_component_exts Component Extensions
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_component_exts_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *component_exts,
            axis2_env_t **env);
      
    axis2_status_t (AXIS2_CALL *
    to_component_exts_free) (void *component_exts,
            axis2_env_t **env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *component_exts,
            axis2_env_t **env);
    
    axis2_url_t *(AXIS2_CALL *
    get_namespace) (
            void *component_exts,
            axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    init) (
            void *component_exts,
            axis2_env_t **env,
            struct axis2_woden_wsdl_element *parent_el,
            axis2_url_t *namespc);

};

struct axis2_woden_component_exts
{
    axis2_woden_component_exts_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_component_exts_t *)
axis2_woden_component_exts_create(axis2_env_t **env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_DECLARE(axis2_status_t)
axis2_woden_component_exts_resolve_methods(
        axis2_woden_component_exts_t *component_exts,
        axis2_env_t **env,
        axis2_hash_t *methods);

#define AXIS2_WODEN_COMPONENT_EXTS_FREE(component_exts, env) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->\
         free (component_exts, env))

#define AXIS2_WODEN_COMPONENT_EXTS_TO_COMPONENT_EXTS_FREE(component_exts, env) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->\
         to_component_exts_free (component_exts, env))

#define AXIS2_WODEN_COMPONENT_EXTS_TYPE(component_exts, env) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->\
         type (component_exts, env))

#define AXIS2_WODEN_COMPONENT_EXTS_INIT(component_exts, env, parent_el, namespc) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->\
         init(component_exts, env, \
            parent_el, namespc))

#define AXIS2_WODEN_COMPONENT_EXTS_GET_NAMESPACE(component_exts, env) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->\
         get_namespace(component_exts, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_COMPONENT_EXTS_H */
