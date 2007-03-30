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

#ifndef WODEN_WSDL_COMPONENT_H
#define WODEN_WSDL_COMPONENT_H

/**
 * @file woden_wsdl_component.h
 * @brief Axis2 Wsdl Component Interface
 * All components directly or indirectly extend this interface, so it provides 
 * a common term of reference for all components.
 *
 */

#include <axutil_allocator.h>
#include <axutil_env.h>
#include <axis2_error.h>
#include <axis2_string.h>
#include <axis2_utils.h>
#include <axis2_hash.h>
#include <axis2_uri.h>
#include <woden.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct woden_wsdl_component woden_wsdl_component_t;
typedef struct woden_wsdl_component_ops woden_wsdl_component_ops_t;
struct woden_component_exts;

/** @defgroup woden_wsdl_component Wsdl Component
  * @ingroup woden
  * @{
  */

struct woden_wsdl_component_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *wsdl_component,
            const axutil_env_t *env);
    
    axis2_status_t (AXIS2_CALL *
    to_wsdl_component_free) (
            void *wsdl_component,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *wsdl_component,
            const axutil_env_t *env);
    
    void *(AXIS2_CALL *
    get_component_exts_for_namespace) (
            void *wsdl_component,
            const axutil_env_t *env,
            axis2_uri_t *namespc);

};

struct woden_wsdl_component
{
    woden_wsdl_component_ops_t *ops;
};

/************************Woden C Internal Methods******************************/
axis2_status_t AXIS2_CALL
woden_wsdl_component_resolve_methods(
        woden_wsdl_component_t *wsdl_component,
        const axutil_env_t *env,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL_COMPONENT_FREE(wsdl_component, env) \
      (((woden_wsdl_component_t *) wsdl_component)->ops->\
         free (wsdl_component, env))

#define WODEN_WSDL_COMPONENT_TO_WSDL_COMPONENT_FREE(wsdl_component, env) \
      (((woden_wsdl_component_t *) wsdl_component)->ops->\
         to_wsdl_component_free (wsdl_component, env))

#define WODEN_WSDL_COMPONENT_TYPE(wsdl_component, env) \
      (((woden_wsdl_component_t *) wsdl_component)->ops->\
         type (wsdl_component, env))

#define WODEN_WSDL_COMPONENT_GET_COMPONENT_EXTS_FOR_NAMESPACE(\
        wsdl_component, env, namespc) \
      (((woden_wsdl_component_t *) wsdl_component)->ops->\
         get_component_exts_for_namespace(wsdl_component, env, namespc))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL_COMPONENT_H */
