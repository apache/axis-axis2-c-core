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

#ifndef AXIS2_WODEN_WSDL_COMPONENT_H
#define AXIS2_WODEN_WSDL_COMPONENT_H

/**
 * @file axis2_woden_wsdl_component.h
 * @brief Axis2 Wsdl Component Interface
 * All components directly or indirectly extend this interface, so it provides 
 * a common term of reference for all components.
 *
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_url.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_wsdl_component axis2_woden_wsdl_component_t;
typedef struct axis2_woden_wsdl_component_ops axis2_woden_wsdl_component_ops_t;
struct axis2_xml_wsdl_component;

/** @defgroup axis2_woden_wsdl_component Wsdl Component
  * @ingroup axis2_wsdl
  * @{
  */

struct axis2_woden_wsdl_component_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *wsdl_component,
            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    get_component_exts_for_namespace) (void *wsdl_component,
                        axis2_env_t **env,
                        axis2_url_t *namespc);

};

struct axis2_woden_wsdl_component
{
    axis2_woden_wsdl_component_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_wsdl_component_t *)
axis2_woden_wsdl_component_create(axis2_env_t **env);

/**
 * This is an Axis2 C internal method. This is used only from constructor
 * of the child class
 */
AXIS2_DECLARE(axis2_status_t)
axis2_woden_wsdl_component_resolve_methods(axis2_woden_wsdl_component_t *wsdl_component,
                                axis2_env_t **env,
                                axis2_woden_wsdl_component_t *wsdl_component_impl,
                                axis2_hash_t *methods);

#define AXIS2_WODEN_WSDL_COMPONENT_FREE(wsdl_component, env) \
		(((axis2_woden_wsdl_component_t *) wsdl_component)->ops->free (wsdl_component, env))

#define AXIS2_WODEN_WSDL_COMPONENT_GET_COMPONENT_EXTS_FOR_NAMESPACE(\
        wsdl_component, env, namespc) \
		(((axis2_woden_wsdl_component_t *) wsdl_component)->ops->\
         get_component_exts_for_namespace(wsdl_component, env, namespc))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_WSDL_COMPONENT_H */
