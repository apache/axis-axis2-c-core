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

#ifndef WODEN_FEATURE_H
#define WODEN_FEATURE_H

/**
 * @file woden_feature.h
 * @brief Axis2 Feature Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden.h>
#include <woden_feature_element.h>
#include <woden_wsdl_obj.h>
#include <woden_documentable.h>
#include <woden_nested_component.h>

/** @defgroup woden_feature Feature
  * @ingroup woden
  * @{
  */

typedef union woden_feature_base woden_feature_base_t;
typedef struct woden_feature woden_feature_t;
typedef struct woden_feature_ops woden_feature_ops_t;
struct woden_documentation_element;
struct woden_wsdl_component;
struct woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_feature_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *feature,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs) (void *feature,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *feature,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *feature,
            const axutil_env_t *env);

    axis2_uri_t *(AXIS2_CALL *
    get_ref) (
            void *feature,
            const axutil_env_t *env);

    axis2_bool_t (AXIS2_CALL *
    is_required) (
            void *feature,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *feature,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element)  (
            void *feature,
            const axutil_env_t *env);

};

union woden_feature_base
{
    woden_documentable_t documentable;
    woden_feature_element_t feature_element;
    woden_nested_component_t nested_component;
};

struct woden_feature
{
    woden_feature_base_t base;
    woden_feature_ops_t *ops;
};

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_feature_element(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_nested_element(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_documentable_element(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_documentable(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_wsdl_obj(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_nested_component(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_wsdl_component(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_element_extensible(
        void *feature,
        const axutil_env_t *env);

AXIS2_EXTERN woden_feature_t * AXIS2_CALL
woden_feature_to_attr_extensible(
        void *feature,
        const axutil_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_feature_resolve_methods(
        woden_feature_t *feature,
        const axutil_env_t *env,
        woden_feature_t *feature_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_FEATURE_FREE(feature, env) \
      (((woden_feature_t *) feature)->ops->free(feature, env))

#define WODEN_FEATURE_SUPER_OBJS(feature, env) \
      (((woden_feature_t *) feature)->ops->super_objs(feature, env))

#define WODEN_FEATURE_TYPE(feature, env) \
      (((woden_feature_t *) feature)->ops->type(feature, env))

#define WODEN_FEATURE_GET_BASE_IMPL(feature, env) \
      (((woden_feature_t *) feature)->ops->get_base_impl(feature, env))

#define WODEN_FEATURE_GET_REF(feature, env) \
      (((woden_feature_t *) feature)->\
         get_ref(feature, env))

#define WODEN_FEATURE_IS_REQUIRED(feature, env) \
      (((woden_feature_t *) feature)->\
         is_required(feature, env))

#define WODEN_FEATURE_GET_PARENT(feature, env) \
      (((woden_feature_t *) feature)->ops->\
         get_parent(feature, env))

#define WODEN_FEATURE_TO_ELEMENT(feature, env) \
      (((woden_feature_t *) feature)->ops->\
         to_element(feature, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_FEATURE_H */
