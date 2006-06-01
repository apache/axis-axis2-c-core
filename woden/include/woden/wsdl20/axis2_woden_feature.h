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

#ifndef AXIS2_WODEN_FEATURE_H
#define AXIS2_WODEN_FEATURE_H

/**
 * @file axis2_woden_feature.h
 * @brief Axis2 Feature Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_feature_element.h>
#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_documentable.h>
#include <woden/wsdl20/axis2_woden_nested_component.h>

/** @defgroup axis2_woden_feature Feature
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_feature_base axis2_woden_feature_base_t;
typedef struct axis2_woden_feature axis2_woden_feature_t;
typedef struct axis2_woden_feature_ops axis2_woden_feature_ops_t;
struct axis2_woden_documentation_element;
struct axis2_woden_wsdl_component;
struct axis2_woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_feature_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *feature,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *feature,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *feature,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *feature,
            const axis2_env_t *env);

    axis2_url_t *(AXIS2_CALL *
    get_ref) (
            void *feature,
            const axis2_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    is_required) (
            void *feature,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *feature,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *feature,
            const axis2_env_t *env);

};

union axis2_woden_feature_base
{
    axis2_woden_documentable_t documentable;
    axis2_woden_feature_element_t feature_element;
    axis2_woden_nested_component_t nested_component;
};

struct axis2_woden_feature
{
    axis2_woden_feature_base_t base;
    axis2_woden_feature_ops_t *ops;
};

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_feature_element(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_nested_element(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_documentable_element(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_documentable(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_wsdl_obj(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_nested_component(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_wsdl_component(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_element_extensible(
        void *feature,
        const axis2_env_t *env);

AXIS2_EXTERN axis2_woden_feature_t * AXIS2_CALL
axis2_woden_feature_to_attr_extensible(
        void *feature,
        const axis2_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_woden_feature_resolve_methods(
        axis2_woden_feature_t *feature,
        const axis2_env_t *env,
        axis2_woden_feature_t *feature_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_FEATURE_FREE(feature, env) \
		(((axis2_woden_feature_t *) feature)->ops->free(feature, env))

#define AXIS2_WODEN_FEATURE_SUPER_OBJS(feature, env) \
		(((axis2_woden_feature_t *) feature)->ops->super_objs(feature, env))

#define AXIS2_WODEN_FEATURE_TYPE(feature, env) \
		(((axis2_woden_feature_t *) feature)->ops->type(feature, env))

#define AXIS2_WODEN_FEATURE_GET_BASE_IMPL(feature, env) \
		(((axis2_woden_feature_t *) feature)->ops->get_base_impl(feature, env))

#define AXIS2_WODEN_FEATURE_GET_REF(feature, env) \
		(((axis2_woden_feature_t *) feature)->\
         get_ref(feature, env))

#define AXIS2_WODEN_FEATURE_IS_REQUIRED(feature, env) \
		(((axis2_woden_feature_t *) feature)->\
         is_required(feature, env))

#define AXIS2_WODEN_FEATURE_GET_PARENT(feature, env) \
		(((axis2_woden_feature_t *) feature)->ops->\
         get_parent(feature, env))

#define AXIS2_WODEN_FEATURE_TO_ELEMENT(feature, env) \
		(((axis2_woden_feature_t *) feature)->ops->\
         to_element(feature, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_FEATURE_H */
