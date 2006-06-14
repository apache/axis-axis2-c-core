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

#ifndef AXIS2_WODEN_WSDL_OBJ_H
#define AXIS2_WODEN_WSDL_OBJ_H

/**
 * @file axis2_woden_wsdl_obj.h
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

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_wsdl_element.h>
#include <woden/wsdl20/axis2_woden_wsdl_component.h>

/** @defgroup axis2_woden_wsdl_obj Wsdl Object
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_wsdl_obj_base axis2_woden_wsdl_obj_base_t;
typedef struct axis2_woden_wsdl_obj axis2_woden_wsdl_obj_t;
typedef struct axis2_woden_wsdl_obj_ops axis2_woden_wsdl_obj_ops_t;
struct axis2_woden_component_exts;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_wsdl_obj_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (
            void *wsdl_obj,
            const axis2_env_t *env);
    
    axis2_hash_t *(AXIS2_CALL *
    super_objs) (
            void *wsdl_obj,
            const axis2_env_t *env);
    
    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (
            void *wsdl_obj,
            const axis2_env_t *env);

    /**
     * @return the base implementation class
     */
    axis2_woden_wsdl_element_t *(AXIS2_CALL *
    get_base_impl) (
            void *wsdl_obj,
            const axis2_env_t *env);

    struct axis2_woden_component_exts *(AXIS2_CALL *
    get_component_exts_for_namespace) (
            void *wsdl_obj,
            const axis2_env_t *env,
            axis2_uri_t *namespc);
    
    /*
     * Store the extensions in a map using the namespace string as the key.
     * If the extensions value is null, delete any existing entry in the map
     * for this namespace. If the nsmespace string is null, do nothing.
     */
    axis2_status_t (AXIS2_CALL *
    set_component_exts) (
            void *wsdl_obj,
            const axis2_env_t *env,
            axis2_uri_t *namespc,
            struct axis2_woden_component_exts *exts);
};

union axis2_woden_wsdl_obj_base
{
    axis2_woden_wsdl_element_t wsdl_element;
    axis2_woden_wsdl_component_t wsdl_component;
};

struct axis2_woden_wsdl_obj
{
    axis2_woden_wsdl_obj_base_t base;
    axis2_woden_wsdl_obj_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_wsdl_obj_t * AXIS2_CALL
axis2_woden_wsdl_obj_create(
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_wsdl_obj_resolve_methods(
        axis2_woden_wsdl_obj_t *wsdl_obj,
        const axis2_env_t *env,
        axis2_woden_wsdl_obj_t *wsdl_obj_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_WSDL_OBJ_FREE(wsdl_obj, env) \
      (((axis2_woden_wsdl_obj_t *) wsdl_obj)->ops->free(wsdl_obj, env))

#define AXIS2_WODEN_WSDL_OBJ_SUPER_OBJS(wsdl_obj, env) \
      (((axis2_woden_wsdl_obj_t *) wsdl_obj)->ops->super_objs(wsdl_obj, env))

#define AXIS2_WODEN_WSDL_OBJ_TYPE(wsdl_obj, env) \
      (((axis2_woden_wsdl_obj_t *) wsdl_obj)->ops->type(wsdl_obj, env))

#define AXIS2_WODEN_WSDL_OBJ_GET_BASE_IMPL(wsdl_obj, env) \
      (((axis2_woden_wsdl_obj_t *) wsdl_obj)->ops->get_base_impl(wsdl_obj, env))

#define AXIS2_WODEN_WSDL_OBJ_GET_COMPONENT_EXTS_FOR_NAMESPACE(wsdl_obj, env, \
                        namespc) \
      (((axis2_woden_wsdl_obj_t *) wsdl_obj)->ops->\
         get_component_exts_for_namespace(wsdl_obj, env, namespc))

#define AXIS2_WODEN_WSDL_OBJ_SET_COMPONENT_EXTS(wsdl_obj, env, namespc, exts) \
      (((axis2_woden_wsdl_obj_t *) wsdl_obj)->ops->set_component_exts(\
        wsdl_obj, env, namespc, exts))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_WSDL_OBJ_H */
