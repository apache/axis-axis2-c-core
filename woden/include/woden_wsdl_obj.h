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

#ifndef WODEN_WSDL_OBJ_H
#define WODEN_WSDL_OBJ_H

/**
 * @file woden_wsdl_obj.h
 * @brief Axis2 Wsdl Object Interface
 * All classes implementing the WSDL 2.0 Component and Element
 * model interfaces directly or indirectly extend this 
 * abstract class. It implements the WSDL20 Component interface
 * which just provides a common reference for objects from the WSDL 2.0 
 * Component API. This class also inherits common behaviour for 
 * WSDL Elements from WSDL Element, which in turn provides a common
 * reference for objects from the the WSDL 2.0 Element API. 
 * 
 */

#include <woden.h>
#include <woden_wsdl_element.h>
#include <woden_wsdl_component.h>

/** @defgroup woden_wsdl_obj Wsdl Object
  * @ingroup woden
  * @{
  */

typedef union woden_wsdl_obj_base woden_wsdl_obj_base_t;
typedef struct woden_wsdl_obj woden_wsdl_obj_t;
typedef struct woden_wsdl_obj_ops woden_wsdl_obj_ops_t;
struct woden_component_exts;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_wsdl_obj_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *wsdl_obj,
            const axutil_env_t *env);
    
    axutil_hash_t *(AXIS2_CALL *
    super_objs) (
            void *wsdl_obj,
            const axutil_env_t *env);
    
    woden_obj_types_t (AXIS2_CALL *
    type) (
            void *wsdl_obj,
            const axutil_env_t *env);

    /**
     * @return the base implementation class
     */
    woden_wsdl_element_t *(AXIS2_CALL *
    get_base_impl) (
            void *wsdl_obj,
            const axutil_env_t *env);

    struct woden_component_exts *(AXIS2_CALL *
    get_component_exts_for_namespace) (
            void *wsdl_obj,
            const axutil_env_t *env,
            axutil_uri_t *namespc);
    
    /*
     * Store the extensions in a map using the namespace string as the key.
     * If the extensions value is null, delete any existing entry in the map
     * for this namespace. If the nsmespace string is null, do nothing.
     */
    axis2_status_t (AXIS2_CALL *
    set_component_exts) (
            void *wsdl_obj,
            const axutil_env_t *env,
            axutil_uri_t *namespc,
            struct woden_component_exts *exts);
};

union woden_wsdl_obj_base
{
    woden_wsdl_element_t wsdl_element;
    woden_wsdl_component_t wsdl_component;
};

struct woden_wsdl_obj
{
    woden_wsdl_obj_base_t base;
    woden_wsdl_obj_ops_t *ops;
};

AXIS2_EXTERN woden_wsdl_obj_t * AXIS2_CALL
woden_wsdl_obj_create(
        const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_wsdl_obj_resolve_methods(
        woden_wsdl_obj_t *wsdl_obj,
        const axutil_env_t *env,
        woden_wsdl_obj_t *wsdl_obj_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_WSDL_OBJ_FREE(wsdl_obj, env) \
      (((woden_wsdl_obj_t *) wsdl_obj)->ops->free(wsdl_obj, env))

#define WODEN_WSDL_OBJ_SUPER_OBJS(wsdl_obj, env) \
      (((woden_wsdl_obj_t *) wsdl_obj)->ops->super_objs(wsdl_obj, env))

#define WODEN_WSDL_OBJ_TYPE(wsdl_obj, env) \
      (((woden_wsdl_obj_t *) wsdl_obj)->ops->type(wsdl_obj, env))

#define WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env) \
      (((woden_wsdl_obj_t *) wsdl_obj)->ops->get_base_impl(wsdl_obj, env))

#define WODEN_WSDL_OBJ_GET_COMPONENT_EXTS_FOR_NAMESPACE(wsdl_obj, env, \
                        namespc) \
      (((woden_wsdl_obj_t *) wsdl_obj)->ops->\
         get_component_exts_for_namespace(wsdl_obj, env, namespc))

#define WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(wsdl_obj, env, namespc, exts) \
      (((woden_wsdl_obj_t *) wsdl_obj)->ops->set_component_exts(\
        wsdl_obj, env, namespc, exts))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_WSDL_OBJ_H */
