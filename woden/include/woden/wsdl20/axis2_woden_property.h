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

#ifndef AXIS2_WODEN_PROPERTY_H
#define AXIS2_WODEN_PROPERTY_H

/**
 * @file axis2_woden_property.h
 * @brief Axis2 Property Interface
 * An abstract superclass for WSDL 2.0 elements which can have &lt;documentation&gt; 
 * child elements. That is, all WSDL 2.0 elements except the &lt;documentation&gt; 
 * element itself. Defines accessor methods for manipulating DocumentationElements.
 * 
 */

#include <woden/axis2_woden.h>
#include <woden/wsdl20/xml/axis2_woden_property_element.h>
#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_documentable.h>
#include <woden/wsdl20/axis2_woden_nested_component.h>

/** @defgroup axis2_woden_property Property
  * @ingroup axis2_wsdl
  * @{
  */

typedef union axis2_woden_property_base axis2_woden_property_base_t;
typedef struct axis2_woden_property axis2_woden_property_t;
typedef struct axis2_woden_property_ops axis2_woden_property_ops_t;
struct axis2_woden_documentation_element;
struct axis2_woden_wsdl_component;
struct axis2_woden_documentable;

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_woden_property_ops
{
	/** 
     * Deallocate memory
     * @return status code
     */
    axis2_status_t (AXIS2_CALL *
    free) (void *property,
            const axis2_env_t *env);

    axis2_hash_t *(AXIS2_CALL *
    super_objs) (void *property,
            const axis2_env_t *env);

    axis2_woden_obj_types_t (AXIS2_CALL *
    type) (void *property,
            const axis2_env_t *env);
    /**
     * @return the base implementation class
     */
    struct axis2_woden_documentable *(AXIS2_CALL *
    get_base_impl) (
            void *property,
            const axis2_env_t *env);

    axis2_url_t *(AXIS2_CALL *
    get_ref) (
            void *property,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_value_constraint) (
            void *property,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_value) (
            void *property,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    get_parent) (
            void *property,
            const axis2_env_t *env);

    void *(AXIS2_CALL *
    to_element) (
            void *property,
            const axis2_env_t *env);

};

union axis2_woden_property_base
{
    axis2_woden_documentable_t documentable;
    axis2_woden_property_element_t property_element;
    axis2_woden_nested_component_t nested_component;
};

struct axis2_woden_property
{
    axis2_woden_property_base_t base;
    axis2_woden_property_ops_t *ops;
};

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_create(
        const axis2_env_t *env);


/***************************Woden C Internal Methods***************************/
AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_property_element(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_nested_element(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_documentable_element(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_documentable(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_wsdl_obj(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_nested_component(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_wsdl_component(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_element_extensible(
        void *property,
        const axis2_env_t *env);

AXIS2_DECLARE(axis2_woden_property_t *)
axis2_woden_property_to_attr_extensible(
        void *property,
        const axis2_env_t *env);


AXIS2_DECLARE(axis2_status_t)
axis2_woden_property_resolve_methods(
        axis2_woden_property_t *property,
        const axis2_env_t *env,
        axis2_woden_property_t *property_impl,
        axis2_hash_t *methods);
/************************End of Woden C Internal Methods***********************/

#define AXIS2_WODEN_PROPERTY_FREE(property, env) \
		(((axis2_woden_property_t *) property)->ops->free(property, env))

#define AXIS2_WODEN_PROPERTY_SUPER_OBJS(property, env) \
		(((axis2_woden_property_t *) property)->ops->super_objs(property, env))

#define AXIS2_WODEN_PROPERTY_TYPE(property, env) \
		(((axis2_woden_property_t *) property)->ops->type(property, env))

#define AXIS2_WODEN_PROPERTY_GET_BASE_IMPL(property, env) \
		(((axis2_woden_property_t *) property)->ops->get_base_impl(property, env))

#define AXIS2_WODEN_PROPERTY_GET_REF(property, env) \
		(((axis2_woden_property_t *) property)->\
         get_ref(property, env))

#define AXIS2_WODEN_PROPERTY_GET_VALUE_CONSTRAINT(property, env) \
		(((axis2_woden_property_t *) property)->\
         get_value_constraint(property, env))

#define AXIS2_WODEN_PROPERTY_GET_VALUE(property, env) \
		(((axis2_woden_property_t *) property)->\
         get_value(property, env))

#define AXIS2_WODEN_PROPERTY_GET_PARENT(property, env) \
		(((axis2_woden_property_t *) property)->ops->\
         get_parent(property, env))

#define AXIS2_WODEN_PROPERTY_TO_ELEMENT(property, env) \
		(((axis2_woden_property_t *) property)->ops->\
         to_element(property, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif /* AXIS2_WODEN_PROPERTY_H */
