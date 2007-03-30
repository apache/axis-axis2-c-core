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

#include <woden_wsdl_obj.h>
#include <woden_property.h>
#include <woden_types.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <woden_type_def.h>
#include <woden_generic_obj.h>
#include <xml_schema_type.h>
#include <axutil_generic_obj.h>

typedef struct woden_property_impl woden_property_impl_t;

/**
 * @brief Feature Struct Impl
 *   Axis2 Feature
 */
struct woden_property_impl
{
    woden_property_t property;
    woden_obj_types_t obj_type;
    woden_documentable_t *documentable;
    axutil_hash_t *super;
    axutil_hash_t *methods;
    /* WSDL Component model data */
    axis2_uri_t *f_ref;
    axutil_generic_obj_t *f_value;
    woden_type_def_t *f_value_constraint;
    void *f_parent;
    /* XML Element data */
    axis2_qname_t *f_constraint_qname;
    axis2_bool_t f_has_value_token;
    void *f_types;
};

#define INTF_TO_IMPL(property) ((woden_property_impl_t *) property)

axis2_status_t AXIS2_CALL
woden_property_free(
    void *property,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
woden_property_super_objs(
    void *property,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_property_type(
    void *property,
    const axutil_env_t *env);

woden_documentable_t *AXIS2_CALL
woden_property_get_base_impl(
    void *property,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_property_set_ref(
    void *property,
    const axutil_env_t *env,
    axis2_uri_t *ref);

axis2_uri_t *AXIS2_CALL
woden_property_get_ref(
    void *property,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_property_set_parent_element(
    void *property,
    const axutil_env_t *env,
    woden_wsdl_element_t *parent);

void *AXIS2_CALL
woden_property_get_parent_element(
    void *property,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_property_get_parent(
    void *property,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_property_to_element(
    void *property,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_property_set_value(
    void *property,
    const axutil_env_t *env,
    axutil_generic_obj_t *value);

void *AXIS2_CALL
woden_property_get_value(
    void *property,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_property_get_value_constraint(
    void *property,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_property_set_constraint_qname(
    void *property,
    const axutil_env_t *env,
    axis2_qname_t *type_def);

axis2_qname_t *AXIS2_CALL
woden_property_get_constraint_qname(
    void *property,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_property_get_constraint(
    void *property,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_property_set_has_value_token(
    void *property,
    const axutil_env_t *env,
    axis2_bool_t b);

axis2_bool_t AXIS2_CALL
woden_property_has_value_token(
    void *property,
    const axutil_env_t *env);

/* ****************************************************************************
 * Non-API implementation methods
 * ***************************************************************************/
axis2_status_t AXIS2_CALL
woden_property_set_value_constraint(
    void *property,
    const axutil_env_t *env,
    void *type_def);

axis2_status_t AXIS2_CALL
woden_property_set_types(
    void *property,
    const axutil_env_t *env,
    void *types);

static woden_property_t *
create(const axutil_env_t *env);

static axis2_status_t
woden_property_free_ops(
    void *property,
    const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_property_element(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;

    woden_property_free_ops(property, env);

    property_impl->property.base.property_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_property_element_ops_t));
    woden_property_element_resolve_methods(&(property_impl->property.base.
            property_element), env, property_impl->methods);
    return property;
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_nested_element(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);

    property_impl->property.base.property_element.base.nested_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(property_impl->property.base.
            property_element.base.nested_element), env, property_impl->methods);
    return property;
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_documentable_element(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);

    property_impl->property.base.property_element.base.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(property_impl->property.base.
            property_element.base.documentable_element), env,
            property_impl->methods);
    return property;
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_documentable(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);

    property_impl->property.base.documentable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(property_impl->property.base.
            documentable), env, property_impl->documentable, property_impl->methods);
    return property;
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_wsdl_obj(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    woden_wsdl_obj_t *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);

    property_impl->property.base.documentable.base.wsdl_obj.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl_obj_ops_t));
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(
                property_impl->documentable, env);
    woden_wsdl_obj_resolve_methods(&(property_impl->property.base.
            documentable.base.wsdl_obj), env, wsdl_obj, property_impl->methods);
    return property;
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_nested_component(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);

    property_impl->property.base.nested_component.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_component_ops_t));
    woden_nested_component_resolve_methods(&(property_impl->property.base.
            nested_component), env, property_impl->methods);
    return property;
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_wsdl_component(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);
    property_impl->property.base.nested_component.wsdl_component.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl_component_ops_t));
    woden_wsdl_component_resolve_methods(&(property_impl->property.base.
            nested_component.wsdl_component), env, property_impl->methods);
    return property;
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_attr_extensible(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);

    property_impl->property.base.documentable.base.wsdl_obj.base.
    wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(property_impl->property.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.attr_extensible),
            env, NULL, property_impl->methods);
    return property;

}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_to_element_extensible(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!property)
    {
        property_impl = (woden_property_impl_t *) create(env);
    }
    else
        property_impl = (woden_property_impl_t *) property;
    woden_property_free_ops(property, env);
    property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
    base.element_extensible.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(property_impl->property.base.
            documentable.base.wsdl_obj.base.wsdl_element.base.element_extensible),
            env, NULL, property_impl->methods);
    return property;

}

/************************End of Woden C Internal Methods***********************/
static woden_property_t *
create(const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    property_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_property_impl_t));

    property_impl->obj_type = WODEN_PROPERTY;
    property_impl->super = NULL;
    property_impl->methods = NULL;
    property_impl->f_ref = NULL;
    property_impl->f_value = NULL;
    property_impl->f_value_constraint = NULL;
    property_impl->f_parent = NULL;
    property_impl->f_constraint_qname = NULL;
    property_impl->f_has_value_token = AXIS2_FALSE;
    property_impl->f_types = NULL;

    property_impl->property.base.property_element.ops = NULL;
    property_impl->property.base.property_element.base.nested_element.ops = NULL;
    property_impl->property.base.property_element.base.documentable_element.ops =
        NULL;
    property_impl->property.base.documentable.ops = NULL;
    property_impl->property.base.nested_component.ops = NULL;
    property_impl->property.base.nested_component.wsdl_component.ops = NULL;
    property_impl->property.base.documentable.base.wsdl_obj.ops = NULL;
    property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
    base.attr_extensible.ops = NULL;
    property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
    base.element_extensible.ops = NULL;

    property_impl->property.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_property_ops_t));

    property_impl->property.ops->free = woden_property_free;
    property_impl->property.ops->super_objs = woden_property_super_objs;
    property_impl->property.ops->type = woden_property_type;
    property_impl->property.ops->get_base_impl = woden_property_get_base_impl;

    property_impl->property.ops->get_ref = woden_property_get_ref;
    property_impl->property.ops->get_value_constraint =
        woden_property_get_value_constraint;
    property_impl->property.ops->get_value = woden_property_get_value;
    property_impl->property.ops->get_parent = woden_property_get_parent;
    property_impl->property.ops->to_element = woden_property_to_element;

    property_impl->methods = axutil_hash_make(env);
    if (!property_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(property_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_property_free);
    axutil_hash_set(property_impl->methods, "super_objs", AXIS2_HASH_KEY_STRING,
            woden_property_super_objs);
    axutil_hash_set(property_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_property_type);

    axutil_hash_set(property_impl->methods, "set_ref",
            AXIS2_HASH_KEY_STRING,
            woden_property_set_ref);
    axutil_hash_set(property_impl->methods, "get_ref",
            AXIS2_HASH_KEY_STRING,
            woden_property_get_ref);
    axutil_hash_set(property_impl->methods, "set_parent_element",
            AXIS2_HASH_KEY_STRING,
            woden_property_set_parent_element);
    axutil_hash_set(property_impl->methods, "get_parent_element",
            AXIS2_HASH_KEY_STRING,
            woden_property_get_parent_element);
    axutil_hash_set(property_impl->methods, "get_parent",
            AXIS2_HASH_KEY_STRING,
            woden_property_get_parent);
    axutil_hash_set(property_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_property_to_element);
    axutil_hash_set(property_impl->methods, "set_value",
            AXIS2_HASH_KEY_STRING,
            woden_property_set_value);
    axutil_hash_set(property_impl->methods, "get_value",
            AXIS2_HASH_KEY_STRING,
            woden_property_get_value);
    axutil_hash_set(property_impl->methods, "set_value_constraint",
            AXIS2_HASH_KEY_STRING,
            woden_property_set_value_constraint);
    axutil_hash_set(property_impl->methods, "get_value_constraint",
            AXIS2_HASH_KEY_STRING,
            woden_property_get_value_constraint);
    axutil_hash_set(property_impl->methods, "set_constraint_qname",
            AXIS2_HASH_KEY_STRING,
            woden_property_set_constraint_qname);
    axutil_hash_set(property_impl->methods, "get_constraint_qname",
            AXIS2_HASH_KEY_STRING,
            woden_property_get_constraint_qname);
    axutil_hash_set(property_impl->methods, "get_constraint",
            AXIS2_HASH_KEY_STRING,
            woden_property_get_constraint);
    axutil_hash_set(property_impl->methods, "set_types",
            AXIS2_HASH_KEY_STRING,
            woden_property_set_types);
    axutil_hash_set(property_impl->methods, "set_has_value_token",
            AXIS2_HASH_KEY_STRING,
            woden_property_set_has_value_token);
    axutil_hash_set(property_impl->methods, "has_value_token",
            AXIS2_HASH_KEY_STRING,
            woden_property_has_value_token);

    return &(property_impl->property);
}

AXIS2_EXTERN woden_property_t * AXIS2_CALL
woden_property_create(const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    property_impl = (woden_property_impl_t *) create(env);

    property_impl->documentable = woden_documentable_create(env);

    property_impl->super = axutil_hash_make(env);
    if (!property_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(property_impl->super, "WODEN_PROPERTY",
            AXIS2_HASH_KEY_STRING, &(property_impl->property));
    axutil_hash_set(property_impl->super, "WODEN_DOCUMENTABLE",
            AXIS2_HASH_KEY_STRING, property_impl->documentable);

    return &(property_impl->property);
}

static axis2_status_t
woden_property_free_ops(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = INTF_TO_IMPL(property);

    if (property_impl->property.base.property_element.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.
                property_element.ops);
        property_impl->property.base.property_element.ops = NULL;
    }

    if (property_impl->property.base.property_element.base.nested_element.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.
                property_element.base.nested_element.ops);
        property_impl->property.base.property_element.base.nested_element.ops =
            NULL;
    }

    if (property_impl->property.base.property_element.base.documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.
                property_element.base.documentable_element.ops);
        property_impl->property.base.property_element.base.documentable_element.ops =
            NULL;
    }

    if (property_impl->property.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.
                documentable.ops);
        property_impl->property.base.documentable.ops =
            NULL;
    }

    if (property_impl->property.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.
                documentable.base.wsdl_obj.ops);
        property_impl->property.base.documentable.base.wsdl_obj.ops =
            NULL;
    }

    if (property_impl->property.base.nested_component.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.
                nested_component.ops);
        property_impl->property.base.nested_component.ops = NULL;
    }

    if (property_impl->property.base.nested_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.
                nested_component.wsdl_component.ops);
        property_impl->property.base.nested_component.wsdl_component.ops = NULL;
    }


    if (property_impl->property.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.attr_extensible.ops);
        property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
        base.attr_extensible.ops = NULL;
    }

    if (property_impl->property.base.documentable.base.wsdl_obj.base.
            wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.base.documentable.
                base.wsdl_obj.base.wsdl_element.base.element_extensible.ops);
        property_impl->property.base.documentable.base.wsdl_obj.base.wsdl_element.
        base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_property_free(void *property,
        const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = INTF_TO_IMPL(property);

    if (property_impl->f_ref)
    {
        axis2_uri_free(property_impl->f_ref, env);
        property_impl->f_ref = NULL;
    }

    /* TODO free f_parent */

    if (property_impl->super)
    {
        axutil_hash_free(property_impl->super, env);
        property_impl->super = NULL;
    }

    if (property_impl->methods)
    {
        axutil_hash_free(property_impl->methods, env);
        property_impl->methods = NULL;
    }

    if (property_impl->documentable)
    {
        WODEN_DOCUMENTABLE_FREE(property_impl->documentable, env);
        property_impl->documentable = NULL;
    }

    woden_property_free_ops(property, env);

    if ((&(property_impl->property))->ops)
    {
        AXIS2_FREE(env->allocator, (&(property_impl->property))->ops);
        (&(property_impl->property))->ops = NULL;
    }

    if (property_impl)
    {
        AXIS2_FREE(env->allocator, property_impl);
        property_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
woden_property_super_objs(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    property_impl = INTF_TO_IMPL(property);

    return property_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_property_type(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = INTF_TO_IMPL(property);

    return property_impl->obj_type;
}

woden_documentable_t *AXIS2_CALL
woden_property_get_base_impl(void *property,
        const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    property_impl = INTF_TO_IMPL(property);

    return property_impl->documentable;
}

axis2_status_t AXIS2_CALL
woden_property_resolve_methods(
    woden_property_t *property,
    const axutil_env_t *env,
    woden_property_t *property_impl,
    axutil_hash_t *methods)
{
    woden_property_impl_t *property_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    property_impl_l = INTF_TO_IMPL(property_impl);

    property->ops->free = axutil_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    property->ops->super_objs = axutil_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    property->ops->type = axutil_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    property->ops->get_ref = axutil_hash_get(methods,
            "get_ref", AXIS2_HASH_KEY_STRING);
    if (!property->ops->get_ref && property_impl_l)
        property->ops->get_ref =
            property_impl_l->property.ops->get_ref;

    property->ops->get_value_constraint = axutil_hash_get(methods,
            "get_value_constraint", AXIS2_HASH_KEY_STRING);
    if (!property->ops->get_value_constraint && property_impl_l)
        property->ops->get_value_constraint =
            property_impl_l->property.ops->get_value_constraint;

    property->ops->get_value = axutil_hash_get(methods,
            "get_value", AXIS2_HASH_KEY_STRING);
    if (!property->ops->get_value && property_impl_l)
        property->ops->get_value =
            property_impl_l->property.ops->get_value;

    property->ops->get_parent = axutil_hash_get(methods,
            "get_parent", AXIS2_HASH_KEY_STRING);
    if (!property->ops->get_parent && property_impl_l)
        property->ops->get_parent =
            property_impl_l->property.ops->get_parent;

    property->ops->to_element = axutil_hash_get(methods,
            "to_element", AXIS2_HASH_KEY_STRING);
    if (!property->ops->to_element && property_impl_l)
        property->ops->to_element =
            property_impl_l->property.ops->to_element;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_property_set_ref(
    void *property,
    const axutil_env_t *env,
    axis2_uri_t *ref)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    if (property_impl->f_ref)
    {
        axis2_uri_free(property_impl->f_ref, env);
        property_impl->f_ref = NULL;
    }
    property_impl->f_ref = axis2_uri_clone(ref, env);
    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_property_get_ref(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_ref;
}

axis2_status_t AXIS2_CALL
woden_property_set_parent_element(
    void *property,
    const axutil_env_t *env,
    woden_wsdl_element_t *parent)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    /*if(property_impl->f_parent)
    {
        WODEN_WSDL_ELEMENT_FREE(parent, env);
    }*/
    property_impl->f_parent = parent;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_property_get_parent_element(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_parent;
}

void *AXIS2_CALL
woden_property_get_parent(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_parent;
}

void *AXIS2_CALL
woden_property_to_element(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return &(property_impl->property);
}

axis2_status_t AXIS2_CALL
woden_property_set_value(
    void *property,
    const axutil_env_t *env,
    axutil_generic_obj_t *value)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    if (property_impl->f_value)
    {
         axutil_generic_obj_free(property_impl->f_value, env);
        property_impl->f_value = NULL;
    }
    property_impl->f_value = value;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_property_get_value(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_value;
}

axis2_status_t AXIS2_CALL
woden_property_set_value_constraint(
    void *property,
    const axutil_env_t *env,
    void *type_def)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    /*if(property_impl->f_value_constraint)
    {
        WODEN_TYPE_DEFINITION_FREE(property_impl->f_value_constraint, 
                env);
        property_impl->f_value_constraint = NULL;
    }*/
    property_impl->f_value_constraint = type_def;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_property_get_value_constraint(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_value_constraint;
}

axis2_status_t AXIS2_CALL
woden_property_set_constraint_qname(
    void *property,
    const axutil_env_t *env,
    axis2_qname_t *constraint)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    if (property_impl->f_constraint_qname)
    {
        axis2_qname_free(property_impl->f_constraint_qname, env);
        property_impl->f_constraint_qname = NULL;
    }
    property_impl->f_constraint_qname = axis2_qname_clone(constraint, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_property_get_constraint_qname(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_constraint_qname;
}

void *AXIS2_CALL
woden_property_get_constraint(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    xml_schema_type_t *xst = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    if (property_impl->f_constraint_qname && property_impl->f_types)
    {
        xst = (xml_schema_type_t *)
                WODEN_TYPES_GET_TYPE_DEFINITION(property_impl->f_types, env,
                        property_impl->f_constraint_qname);
    }
    return xst;
}

axis2_status_t AXIS2_CALL
woden_property_set_types(
    void *property,
    const axutil_env_t *env,
    void *types)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    /*if(property_impl->f_types)
    {
        WODEN_TYPES_ELEMENT_FREE(property_impl->f_types, env);
        property_impl->f_types = NULL;
    }*/
    property_impl->f_types = types;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_property_set_has_value_token(
    void *property,
    const axutil_env_t *env,
    axis2_bool_t b)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    property_impl->f_has_value_token = b;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
woden_property_has_value_token(
    void *property,
    const axutil_env_t *env)
{
    woden_property_impl_t *property_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_PROPERTY_SUPER_OBJS(property, env);
    property_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_PROPERTY", AXIS2_HASH_KEY_STRING));

    return property_impl->f_has_value_token;
}


