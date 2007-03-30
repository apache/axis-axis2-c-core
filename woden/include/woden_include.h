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

#ifndef WODEN_INCLUDE_H
#define WODEN_INCLUDE_H

/**
 * @file woden_include.h
 * @brief Axis2 Include Interface
 * This class implements the &lt;wsdl:include&gt; element. 
 * 
 */

#include <woden.h>
#include <woden_include_element.h>
#include <woden_wsdl_obj.h>
#include <woden_wsdl_ref.h>

/** @defgroup woden_include Include
  * @ingroup woden
  * @{
  */

typedef union woden_include_base woden_include_base_t;
typedef struct woden_include woden_include_t;
typedef struct woden_include_ops woden_include_ops_t;
struct woden_wsdl_ref;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_include_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *include,
            const axutil_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *include,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *include,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_wsdl_ref *(AXIS2_CALL *
    get_base_impl) (
            void *include,
            const axutil_env_t *env);

    /* No additional definitions required. This class inherits all of its behaviour 
     * from woden_wsdl_ref. We just need this subclass so we can create an
     * object representing include_element, which maps to <wsdl:include>.
     */
};

union woden_include_base
{
    woden_include_element_t include_element;
    woden_wsdl_ref_t wsdl_ref;
};

struct woden_include
{
    woden_include_base_t base;
    woden_include_ops_t *ops;
};

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_include_element(
        void *include,
        const axutil_env_t *env);

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_wsdl_ref(
        void *include,
        const axutil_env_t *env);

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_attr_extensible(
        void *include,
        const axutil_env_t *env);

AXIS2_EXTERN woden_include_t * AXIS2_CALL
woden_include_to_element_extensible(
        void *include,
        const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_include_resolve_methods(
        woden_include_t *include,
        const axutil_env_t *env,
        woden_include_t *include_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_INCLUDE_FREE(include, env) \
      (((woden_include_t *) include)->ops->free(include, env))

#define WODEN_INCLUDE_SUPER_OBJS(include, env) \
      (((woden_include_t *) include)->ops->super_objs(include, env))

#define WODEN_INCLUDE_TYPE(include, env) \
      (((woden_include_t *) include)->ops->type(include, env))

#define WODEN_INCLUDE_GET_BASE_IMPL(include, env) \
      (((woden_include_t *) include)->ops->get_base_impl(include, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_INCLUDE_H */
