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
#include <woden_interface_op.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>

typedef struct woden_interface_op_impl woden_interface_op_impl_t;

/**
 * @brief Interface Operation Struct Impl
 *   Axis2 Interface Operation
 */
struct woden_interface_op_impl
{
    woden_interface_op_t interface_op;
    woden_nested_configurable_t *nested_configurable;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_qname_t *f_qname;
    axis2_uri_t *f_msg_exchange_pattern;
    axis2_array_list_t *f_style;
    axis2_array_list_t *f_msg_refs;
    axis2_array_list_t *f_fault_refs;
};

#define INTF_TO_IMPL(interface_op) ((woden_interface_op_impl_t *) interface_op)

axis2_status_t AXIS2_CALL
woden_interface_op_free(
    void *interface_op,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_interface_op_super_objs(
    void *interface_op,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_interface_op_type(
    void *interface_op,
    const axis2_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_interface_op_get_base_impl(
    void *interface_op,
    const axis2_env_t *env);

/* ************************************************************
 *  Interface Operation  methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
woden_interface_op_get_qname(
    void *interface_op,
    const axis2_env_t *env);

axis2_uri_t *AXIS2_CALL
woden_interface_op_get_msg_exchange_pattern(
    void *interface_op,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_msg_refs(
    void *interface_op,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_fault_refs(
    void *interface_op,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_style(
    void *interface_op,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_op_to_element(
    void *interface_op,
    const axis2_env_t *env);

/* ************************************************************
 *  Interface Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_interface_op_set_qname(
    void *interface_op,
    const axis2_env_t *env,
    axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_interface_op_set_pattern(
    void *interface_op,
    const axis2_env_t *env,
    axis2_uri_t *uri);

axis2_uri_t *AXIS2_CALL
woden_interface_op_get_pattern(
    void *interface_op,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_interface_op_add_style_uri(
    void *interface_op,
    const axis2_env_t *env,
    axis2_uri_t *uri);

axis2_status_t AXIS2_CALL
woden_interface_op_remove_style_uri(
    void *interface_op,
    const axis2_env_t *env,
    axis2_uri_t *uri);

axis2_status_t AXIS2_CALL
woden_interface_op_add_interface_msg_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *msg_ref);

axis2_status_t AXIS2_CALL
woden_interface_op_remove_interface_msg_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *msg_ref);

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_msg_ref_elements(
    void *interface_op,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_interface_op_add_interface_fault_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *fault_ref);

axis2_status_t AXIS2_CALL
woden_interface_op_remove_interface_fault_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *fault_ref);

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_fault_ref_elements(
    void *interface_op,
    const axis2_env_t *env);

static woden_interface_op_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_interface_op_free_ops(
    void *interface_op,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_interface_op_element(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.interface_op_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_interface_op_element_ops_t));
    woden_interface_op_element_resolve_methods(&(interface_op_impl->interface_op.base.
            interface_op_element), env, interface_op_impl->methods);
    return interface_op;
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_nested_configurable(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.nested_configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_configurable_ops_t));
    woden_nested_configurable_resolve_methods(&(interface_op_impl->interface_op.base.
            nested_configurable), env, interface_op_impl->nested_configurable,
            interface_op_impl->methods);
    return interface_op;
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_nested_component(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.nested_configurable.base.nested_component.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_component_ops_t));
    woden_nested_component_resolve_methods(&(interface_op_impl->interface_op.base.
            nested_configurable.base.nested_component), env, interface_op_impl->methods);
    return interface_op;
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_configurable(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    void *configurable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                interface_op_impl->nested_configurable, env);
    woden_configurable_resolve_methods(&(interface_op_impl->interface_op.base.
            nested_configurable.base.configurable), env, configurable, interface_op_impl->methods);
    return interface_op;
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_nested_element(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.interface_op_element.base.nested_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(interface_op_impl->interface_op.base.
            interface_op_element.base.nested_element), env, interface_op_impl->methods);
    return interface_op;
}


AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_configurable_element(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.interface_op_element.base.configurable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(interface_op_impl->interface_op.base.
            interface_op_element.base.configurable_element), env, interface_op_impl->methods);
    return interface_op;
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_documentable_element(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.interface_op_element.base.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(interface_op_impl->interface_op.base.
            interface_op_element.base.documentable_element), env,
            interface_op_impl->methods);
    return interface_op;
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_documentable(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    woden_configurable_t *configurable = NULL;
    woden_documentable_t *documentable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.nested_configurable.base.
    configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_documentable_ops_t));
    if(interface_op_impl->nested_configurable)
        configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_op_impl->nested_configurable, env);
    if(configurable)
        documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
    woden_documentable_resolve_methods(&(interface_op_impl->interface_op.base.
            nested_configurable.base.configurable.base.documentable), env, 
            documentable, interface_op_impl->methods);
    return interface_op;
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_attr_extensible(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.interface_op_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(interface_op_impl->interface_op.base.
            interface_op_element.base.documentable_element.wsdl_element.base.
            attr_extensible), env, NULL, interface_op_impl->methods);
    return interface_op;
}


AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_to_element_extensible(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_op)
    {
        interface_op_impl = (woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (woden_interface_op_impl_t *) interface_op;

    woden_interface_op_free_ops(interface_op, env);

    interface_op_impl->interface_op.base.interface_op_element.base.documentable_element.
    wsdl_element.base.element_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(interface_op_impl->interface_op.base.
            interface_op_element.base.documentable_element.wsdl_element.base.
            element_extensible), env, NULL, interface_op_impl->methods);
    return interface_op;
}


/************************End of Woden C Internal Methods***********************/
static woden_interface_op_t *
create(const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_op_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_interface_op_impl_t));

    interface_op_impl->obj_type = WODEN_INTERFACE_OP;
    interface_op_impl->super = NULL;
    interface_op_impl->methods = NULL;
    interface_op_impl->f_qname = NULL;
    interface_op_impl->f_msg_exchange_pattern = NULL;
    interface_op_impl->f_style = NULL;
    interface_op_impl->f_msg_refs = NULL;
    interface_op_impl->f_fault_refs = NULL;

    interface_op_impl->interface_op.base.interface_op_element.ops = NULL;
    interface_op_impl->interface_op.base.nested_configurable.ops = NULL;
    interface_op_impl->interface_op.base.nested_configurable.base.
    nested_component.ops = NULL;
    interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops =
        NULL;
    interface_op_impl->interface_op.base.interface_op_element.base.
    nested_element.ops = NULL;
    interface_op_impl->interface_op.base.interface_op_element.base.
    configurable_element.ops = NULL;
    interface_op_impl->interface_op.base.interface_op_element.base.
    documentable_element.ops = NULL;
    interface_op_impl->interface_op.base.nested_configurable.base.configurable.base.
    documentable.ops = NULL;
    interface_op_impl->interface_op.base.interface_op_element.base.
    documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    interface_op_impl->interface_op.base.interface_op_element.base.
    documentable_element.wsdl_element.base.element_extensible.ops = NULL;

    interface_op_impl->interface_op.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_interface_op_ops_t));

    interface_op_impl->interface_op.ops->free = woden_interface_op_free;
    interface_op_impl->interface_op.ops->super_objs = woden_interface_op_super_objs;
    interface_op_impl->interface_op.ops->type = woden_interface_op_type;
    interface_op_impl->interface_op.ops->get_base_impl = woden_interface_op_get_base_impl;

    interface_op_impl->interface_op.ops->get_qname =
        woden_interface_op_get_qname;
    interface_op_impl->interface_op.ops->get_msg_exchange_pattern =
        woden_interface_op_get_msg_exchange_pattern;
    interface_op_impl->interface_op.ops->get_interface_msg_refs =
        woden_interface_op_get_interface_msg_refs;
    interface_op_impl->interface_op.ops->get_interface_fault_refs =
        woden_interface_op_get_interface_fault_refs;
    interface_op_impl->interface_op.ops->get_style =
        woden_interface_op_get_style;
    interface_op_impl->interface_op.ops->to_element =
        woden_interface_op_to_element;

    interface_op_impl->methods = axis2_hash_make(env);
    if (!interface_op_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_op_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_interface_op_free);
    axis2_hash_set(interface_op_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_interface_op_super_objs);
    axis2_hash_set(interface_op_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_interface_op_type);

    axis2_hash_set(interface_op_impl->methods, "get_qname",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_qname);
    axis2_hash_set(interface_op_impl->methods, "get_msg_exchange_pattern",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_msg_exchange_pattern);
    axis2_hash_set(interface_op_impl->methods, "get_interface_msg_refs",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_interface_msg_refs);
    axis2_hash_set(interface_op_impl->methods, "get_interface_fault_refs",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_interface_fault_refs);
    axis2_hash_set(interface_op_impl->methods, "get_style",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_style);
    axis2_hash_set(interface_op_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_to_element);
    axis2_hash_set(interface_op_impl->methods, "set_qname",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_set_qname);
    axis2_hash_set(interface_op_impl->methods, "set_pattern",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_set_pattern);
    axis2_hash_set(interface_op_impl->methods, "get_pattern",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_pattern);
    axis2_hash_set(interface_op_impl->methods, "add_style_uri",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_add_style_uri);
    axis2_hash_set(interface_op_impl->methods, "remove_style_uri",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_remove_style_uri);
    axis2_hash_set(interface_op_impl->methods, "add_interface_msg_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_add_interface_msg_ref_element);
    axis2_hash_set(interface_op_impl->methods, "remove_interface_msg_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_remove_interface_msg_ref_element);
    axis2_hash_set(interface_op_impl->methods, "get_interface_msg_ref_elements",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_interface_msg_ref_elements);
    axis2_hash_set(interface_op_impl->methods, "add_interface_fault_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_add_interface_fault_ref_element);
    axis2_hash_set(interface_op_impl->methods, "remove_interface_fault_ref_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_remove_interface_fault_ref_element);
    axis2_hash_set(interface_op_impl->methods, "get_interface_fault_ref_elements",
            AXIS2_HASH_KEY_STRING,
            woden_interface_op_get_interface_fault_ref_elements);

    return &(interface_op_impl->interface_op);
}

AXIS2_EXTERN woden_interface_op_t * AXIS2_CALL
woden_interface_op_create(const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    void *configurable = NULL;
    void *documentable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_op_impl = (woden_interface_op_impl_t *) create(env);

    interface_op_impl->nested_configurable = woden_nested_configurable_create(env);

    interface_op_impl->super = axis2_hash_make(env);
    if (!interface_op_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_op_impl->super, "WODEN_INTERFACE_OP",
            AXIS2_HASH_KEY_STRING,
            &(interface_op_impl->interface_op));
    axis2_hash_set(interface_op_impl->super, "WODEN_NESTED_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING,
            interface_op_impl->nested_configurable);
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                interface_op_impl->nested_configurable, env);
    axis2_hash_set(interface_op_impl->super, "WODEN_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING, configurable);
    documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(configurable, env);
    axis2_hash_set(interface_op_impl->super, "WODEN_DOCUMENTABLE",
            AXIS2_HASH_KEY_STRING, documentable);

    return &(interface_op_impl->interface_op);
}

static axis2_status_t
woden_interface_op_free_ops(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    if (interface_op_impl->interface_op.base.interface_op_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                interface_op_element.ops);
        interface_op_impl->interface_op.base.interface_op_element.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                nested_configurable.ops);
        interface_op_impl->interface_op.base.nested_configurable.ops =
            NULL;
    }

    if (interface_op_impl->interface_op.base.nested_configurable.base.
            nested_component.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                nested_configurable.base.nested_component.ops);
        interface_op_impl->interface_op.base.nested_configurable.base.
        nested_component.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.nested_configurable.base.
            configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                nested_configurable.base.configurable.ops);
        interface_op_impl->interface_op.base.nested_configurable.base.
        configurable.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.interface_op_element.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                interface_op_element.base.nested_element.ops);
        interface_op_impl->interface_op.base.interface_op_element.base.
        nested_element.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.interface_op_element.base.
            configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                interface_op_element.base.configurable_element.ops);
        interface_op_impl->interface_op.base.interface_op_element.base.
        configurable_element.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.interface_op_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                interface_op_element.base.documentable_element.ops);
        interface_op_impl->interface_op.base.interface_op_element.base.
        documentable_element.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                nested_configurable.base.configurable.base.documentable.ops);
        interface_op_impl->interface_op.base.nested_configurable.base.
        configurable.base.documentable.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.interface_op_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                interface_op_element.base.documentable_element.wsdl_element.base.
                attr_extensible.ops);
        interface_op_impl->interface_op.base.interface_op_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    }

    if (interface_op_impl->interface_op.base.interface_op_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_op_impl->interface_op.base.
                interface_op_element.base.documentable_element.wsdl_element.base.
                element_extensible.ops);
        interface_op_impl->interface_op.base.interface_op_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    }


    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_interface_op_free(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    if (interface_op_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_op_impl->f_qname, env);
        interface_op_impl->f_qname = NULL;
    }

    if (interface_op_impl->f_msg_exchange_pattern)
    {
        AXIS2_URI_FREE(interface_op_impl->f_msg_exchange_pattern, env);
        interface_op_impl->f_msg_exchange_pattern = NULL;
    }

    if (interface_op_impl->f_style)
    {
        AXIS2_ARRAY_LIST_FREE(interface_op_impl->f_style, env);
        interface_op_impl->f_style = NULL;
    }

    if (interface_op_impl->f_msg_refs)
    {
        AXIS2_ARRAY_LIST_FREE(interface_op_impl->f_msg_refs, env);
        interface_op_impl->f_msg_refs = NULL;
    }

    if (interface_op_impl->f_fault_refs)
    {
        AXIS2_ARRAY_LIST_FREE(interface_op_impl->f_fault_refs, env);
        interface_op_impl->f_fault_refs = NULL;
    }

    if (interface_op_impl->super)
    {
        axis2_hash_free(interface_op_impl->super, env);
        interface_op_impl->super = NULL;
    }

    if (interface_op_impl->methods)
    {
        axis2_hash_free(interface_op_impl->methods, env);
        interface_op_impl->methods = NULL;
    }

    if (interface_op_impl->nested_configurable)
    {
        WODEN_NESTED_CONFIGURABLE_FREE(interface_op_impl->
                nested_configurable, env);
        interface_op_impl->nested_configurable = NULL;
    }

    woden_interface_op_free_ops(interface_op, env);

    if ((&(interface_op_impl->interface_op))->ops)
    {
        AXIS2_FREE(env->allocator, (&(interface_op_impl->interface_op))->ops);
        (&(interface_op_impl->interface_op))->ops = NULL;
    }

    if (interface_op_impl)
    {
        AXIS2_FREE(env->allocator, interface_op_impl);
        interface_op_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_interface_op_super_objs(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    return interface_op_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_interface_op_type(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    return interface_op_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_interface_op_get_base_impl(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    return interface_op_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_interface_op_resolve_methods(
    woden_interface_op_t *interface_op,
    const axis2_env_t *env,
    woden_interface_op_t *interface_op_impl,
    axis2_hash_t *methods)
{
    woden_interface_op_impl_t *interface_op_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    interface_op_impl_l = INTF_TO_IMPL(interface_op_impl);

    interface_op->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    interface_op->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    interface_op->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    interface_op->ops->get_qname = axis2_hash_get(methods,
            "get_qname", AXIS2_HASH_KEY_STRING);
    if (!interface_op->ops->get_qname && interface_op_impl_l)
        interface_op->ops->get_qname =
            interface_op_impl_l->interface_op.ops->get_qname;

    interface_op->ops->get_msg_exchange_pattern = axis2_hash_get(methods,
            "get_msg_exchange_pattern", AXIS2_HASH_KEY_STRING);
    if (!interface_op->ops->get_msg_exchange_pattern && interface_op_impl_l)
        interface_op->ops->get_msg_exchange_pattern =
            interface_op_impl_l->interface_op.ops->get_msg_exchange_pattern;

    interface_op->ops->get_interface_msg_refs = axis2_hash_get(methods,
            "get_interface_msg_refs", AXIS2_HASH_KEY_STRING);
    if (!interface_op->ops->get_interface_msg_refs && interface_op_impl_l)
        interface_op->ops->get_interface_msg_refs =
            interface_op_impl_l->interface_op.ops->get_interface_msg_refs;

    interface_op->ops->get_interface_fault_refs = axis2_hash_get(methods,
            "get_interface_fault_refs", AXIS2_HASH_KEY_STRING);
    if (!interface_op->ops->get_interface_fault_refs && interface_op_impl_l)
        interface_op->ops->get_interface_fault_refs =
            interface_op_impl_l->interface_op.ops->get_interface_fault_refs;

    interface_op->ops->get_style = axis2_hash_get(methods,
            "get_style", AXIS2_HASH_KEY_STRING);
    if (!interface_op->ops->get_style && interface_op_impl_l)
        interface_op->ops->get_style =
            interface_op_impl_l->interface_op.ops->get_style;

    interface_op->ops->to_element = axis2_hash_get(methods,
            "to_element", AXIS2_HASH_KEY_STRING);
    if (!interface_op->ops->to_element && interface_op_impl_l)
        interface_op->ops->to_element =
            interface_op_impl_l->interface_op.ops->to_element;

    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Operation  methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
woden_interface_op_get_qname(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_qname;
}

axis2_uri_t *AXIS2_CALL
woden_interface_op_get_msg_exchange_pattern(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_msg_exchange_pattern;
}

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_msg_refs(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_msg_refs;
}

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_fault_refs(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_fault_refs;
}

axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_style(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_style;
}

void *AXIS2_CALL
woden_interface_op_to_element(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return &(interface_op_impl->interface_op);
}

/* ************************************************************
 *  Interface Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_interface_op_set_qname(
    void *interface_op,
    const axis2_env_t *env,
    axis2_qname_t *qname)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    if (interface_op_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_op_impl->f_qname, env);
        interface_op_impl->f_qname = NULL;
    }
    interface_op_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_op_set_pattern(
    void *interface_op,
    const axis2_env_t *env,
    axis2_uri_t *uri)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, uri, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    if (interface_op_impl->f_msg_exchange_pattern)
    {
        AXIS2_URI_FREE(interface_op_impl->f_msg_exchange_pattern, env);
        interface_op_impl->f_msg_exchange_pattern = NULL;
    }
    interface_op_impl->f_msg_exchange_pattern = AXIS2_URI_CLONE(uri, env);


    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_interface_op_get_pattern(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_msg_exchange_pattern;
}

axis2_status_t AXIS2_CALL
woden_interface_op_add_style_uri(
    void *interface_op,
    const axis2_env_t *env,
    axis2_uri_t *uri)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, uri, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    if (!interface_op_impl->f_style)
    {
        interface_op_impl->f_style = axis2_array_list_create(env, 0);
        if (!interface_op_impl->f_style)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(interface_op_impl->f_style, env, AXIS2_URI_CLONE(uri, env));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_op_remove_style_uri(
    void *interface_op,
    const axis2_env_t *env,
    axis2_uri_t *uri)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, uri, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    index = AXIS2_ARRAY_LIST_INDEX_OF(interface_op_impl->f_style, env, uri);
    AXIS2_ARRAY_LIST_REMOVE(interface_op_impl->f_style, env, index);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_op_add_interface_msg_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *msg_ref)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ref, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    if (!interface_op_impl->f_msg_refs)
    {
        interface_op_impl->f_msg_refs = axis2_array_list_create(env, 0);
        if (!interface_op_impl->f_msg_refs)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(interface_op_impl->f_msg_refs, env, msg_ref);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_op_remove_interface_msg_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *msg_ref)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ref, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    index = AXIS2_ARRAY_LIST_INDEX_OF(interface_op_impl->f_msg_refs, env, msg_ref);
    AXIS2_ARRAY_LIST_REMOVE(interface_op_impl->f_msg_refs, env, index);
    return AXIS2_SUCCESS;
}


axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_msg_ref_elements(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_msg_refs;
}

axis2_status_t AXIS2_CALL
woden_interface_op_add_interface_fault_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *fault_ref)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_ref, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    if (!interface_op_impl->f_fault_refs)
    {
        interface_op_impl->f_fault_refs = axis2_array_list_create(env, 0);
        if (!interface_op_impl->f_fault_refs)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(interface_op_impl->f_fault_refs, env, fault_ref);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_op_remove_interface_fault_ref_element(
    void *interface_op,
    const axis2_env_t *env,
    void *fault_ref)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_ref, AXIS2_FAILURE);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    index = AXIS2_ARRAY_LIST_INDEX_OF(interface_op_impl->f_fault_refs, env, fault_ref);
    AXIS2_ARRAY_LIST_REMOVE(interface_op_impl->f_fault_refs, env, index);
    return AXIS2_SUCCESS;
}


axis2_array_list_t *AXIS2_CALL
woden_interface_op_get_interface_fault_ref_elements(
    void *interface_op,
    const axis2_env_t *env)
{
    woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_fault_refs;
}



