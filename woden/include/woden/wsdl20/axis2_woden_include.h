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

#ifndef AXIS2_WODEN_INCLUDE_H
#define AXIS2_WODEN_INCLUDE_H

/**
 * @file axis2_woden_include.h
 * @brief Axis2 Include Interface
 * This class implements the &lt;wsdl:include&gt; element. 
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_include_element.h>
#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_wsdl_ref.h>

/** @defgroup axis2_woden_include Include
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_include_base axis2_woden_include_base_t;
typedef struct axis2_woden_include axis2_woden_include_t;
typedef struct axis2_woden_include_ops axis2_woden_include_ops_t;
struct axis2_woden_wsdl_ref;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_include_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *include,
            axis2_env_t **env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *include,
            axis2_env_t **env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *include,
            axis2_env_t **env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_wsdl_ref *(AXIS2_CALL *
    get_base_impl) (
            void *include,
            axis2_env_t **env);

    /* No additional definitions required. This class inherits all of its behaviour 
     * from axis2_woden_wsdl_ref. We just need this subclass so we can create an
     * object representing include_element, which maps to <wsdl:include>.
     */
};

union axis2_woden_include_base
{
    axis2_woden_include_element_t include_element;
    axis2_woden_wsdl_ref_t wsdl_ref;
};

struct axis2_woden_include
{
    axis2_woden_include_base_t base;
    axis2_woden_include_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_include_t *)
axis2_woden_include_create(
        axis2_env_t **env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_include_t *)
axis2_woden_include_to_include_element(
        void *include,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_include_t *)
axis2_woden_include_to_wsdl_ref(
        void *include,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_include_t *)
axis2_woden_include_to_attr_extensible(
        void *include,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_woden_include_t *)
axis2_woden_include_to_element_extensible(
        void *include,
        axis2_env_t **env);

AXIS2_DECLARE(axis2_status_t)
axis2_woden_include_resolve_methods(
        axis2_woden_include_t *include,
        axis2_env_t **env,
        axis2_woden_include_t *include_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_INCLUDE_FREE(include, env) \
		(((axis2_woden_include_t *) include)->ops->free(include, env))

#define AXIS2_WODEN_INCLUDE_SUPER_OBJS(include, env) \
		(((axis2_woden_include_t *) include)->ops->super_objs(include, env))

#define AXIS2_WODEN_INCLUDE_TYPE(include, env) \
		(((axis2_woden_include_t *) include)->ops->type(include, env))

#define AXIS2_WODEN_INCLUDE_GET_BASE_IMPL(include, env) \
		(((axis2_woden_include_t *) include)->ops->get_base_impl(include, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_INCLUDE_H */
