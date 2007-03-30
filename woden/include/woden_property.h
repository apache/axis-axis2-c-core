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

#ifndef WODEN_PROPERTY_H
#define WODEN_PROPERTY_H

/**
 * @file woden_property.h
 * @brief Axis2 Property Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden.h>
#include <woden_property_element.h>
#include <woden_wsdl_obj.h>
#include <woden_documentable.h>
#include <woden_nested_component.h>

/** @defgroup woden_property Property
  * @ingroup woden
  * @{
  */

typedef union woden_property_base woden_property_base_t;
typedef struct woden_property woden_property_t;
typedef struct woden_property_ops woden_property_ops_t;
struct woden_documentation_element;
struct woden_wsdl_component;
struct woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct woden_property_ops
{
   /** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *property,
            const axutil_env_t *env);

    axutil_hash_t *(AXIS2_CALL *
    super_objs) (void *property,
            const axutil_env_t *env);

    woden_obj_types_t (AXIS2_CALL *
    type) (void *property,
            const axutil_env_t *env);
    /**
     * @return the base implementation class
     */
    struct woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *property,
            const axutil_env_t *env);

    axutil_uri_t *(AXIS2_CALL *
    get_ref) (
            void *property,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_value_constraint) (
            void *property,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_value) (
            void *property,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *property,
            const axutil_env_t *env);

    void *(AXIS2_CALL *
    to_element) (
            void *property,
            const axutil_env_t *env);

    /* ****************************************************************************
     * Non-API implementation methods
     * ***************************************************************************/
    axis2_status_t (AXIS2_CALL *
    set_value_constraint) (
            void *property,
            const axutil_env_t *env,
            void *type_def);

    axis2_status_t (AXIS2_CALL *
    set_types) (
            void *property,
            const axutil_env_t *env,
            void *types);

};

union woden_property_base
{
    woden_documentable_t documentable;
    woden_property_element_t property_element;
    woden_nested_component_t nested_component;
};

struct woden_property
{
    woden_property_base_t base;
    woden_property_ops_t *ops;
};

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_create(
        const axutil_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_property_element(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_nested_element(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_documentable_element(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_documentable(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_wsdl_obj(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_nested_component(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_wsdl_component(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_element_extensible(
        void *property,
        const axutil_env_t *env);

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_attr_extensible(
        void *property,
        const axutil_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
woden_property_resolve_methods(
        woden_property_t *property,
        const axutil_env_t *env,
        woden_property_t *property_impl,
        axutil_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define WODEN_PROPERTY_FREE(property, env) \
      (((woden_property_t *) property)->ops->free(property, env))

#define WODEN_PROPERTY_SUPER_OBJS(property, env) \
      (((woden_property_t *) property)->ops->super_objs(property, env))

#define WODEN_PROPERTY_TYPE(property, env) \
      (((woden_property_t *) property)->ops->type(property, env))

#define WODEN_PROPERTY_GET_BASE_IMPL(property, env) \
      (((woden_property_t *) property)->ops->get_base_impl(property, env))

#define WODEN_PROPERTY_GET_REF(property, env) \
      (((woden_property_t *) property)->\
         get_ref(property, env))

#define WODEN_PROPERTY_GET_VALUE_CONSTRAINT(property, env) \
      (((woden_property_t *) property)->\
         get_value_constraint(property, env))

#define WODEN_PROPERTY_GET_VALUE(property, env) \
      (((woden_property_t *) property)->\
         get_value(property, env))

#define WODEN_PROPERTY_GET_PARENT(property, env) \
      (((woden_property_t *) property)->ops->\
         get_parent(property, env))

#define WODEN_PROPERTY_TO_ELEMENT(property, env) \
      (((woden_property_t *) property)->ops->\
         to_element(property, env))

#define WODEN_PROPERTY_SET_VALUE_CONSTRAINT(property, env, type_def) \
      (((woden_property_t *) property)->ops->\
         set_value_constraint(property, env, type_def))

#define WODEN_PROPERTY_SET_TYPES(property, env, types) \
      (((woden_property_t *) property)->ops->\
         set_types(property, env, types))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* WODEN_PROPERTY_H */
