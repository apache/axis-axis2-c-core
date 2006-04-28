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
 * This class represents group of WSDL extension properties (i.e. extension
 * elements or attributes) related by their non-WSDL namespace. 
 * The only behaviour of this class is to return the namespace.
 * Implementations may extend this class to define behaviour specific to 
 * their required extensions.
 * 
 */

#include <axis2_allocator.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2.h>
#include <axis2_hash.h>
#include <axis2_qname.h>
#include <axis2_url.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axis2_woden_component_exts axis2_woden_component_exts_t;
typedef struct axis2_woden_component_exts_ops axis2_woden_component_exts_ops_t;
struct axis2_woden_wsdl_element;

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
      
    axis2_url_t *(AXIS2_CALL *
    get_namespace) (void *component_exts,
                  axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    init) (void *component_exts,
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
axis2_woden_component_exts_resolve_methods(axis2_woden_component_exts_t *component_exts,
                                axis2_env_t **env,
                                axis2_woden_component_exts_t *component_exts_impl,
                                axis2_hash_t *methods);

#define AXIS2_WODEN_COMPONENT_EXTS_FREE(component_exts, env) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->free (component_exts, env))

#define AXIS2_WODEN_COMPONENT_EXTS_INIT(component_exts, env, parent_el, namespc) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->init(component_exts, env, \
            parent_el, namespc))

#define AXIS2_WODEN_COMPONENT_EXTS_GET_NAMESPACE(component_exts, env) \
		(((axis2_woden_component_exts_t *) component_exts)->ops->\
         get_namespace(component_exts, env))


/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_COMPONENT_EXTS_H */
