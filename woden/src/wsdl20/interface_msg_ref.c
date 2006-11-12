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

#include <woden_wsdl_obj.h>
#include <woden_interface_msg_ref.h>
#include <woden_types.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_msg_label.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>
#include <woden_element_decl.h>
#include <woden_direction.h>

typedef struct woden_interface_msg_ref_impl woden_interface_msg_ref_impl_t;

/**
 * @brief Interface Message Reference Struct Impl
 *   Axis2 Interface Message Reference
 */
struct woden_interface_msg_ref_impl
{
    woden_interface_msg_ref_t interface_msg_ref;
    woden_nested_configurable_t *nested_configurable;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    void *f_msg_label;
    void *f_direction;
    axis2_char_t *f_msg_content_model;
    void *f_element_decl;
    axis2_qname_t *f_element_qname;
    void *f_types;
};

#define INTF_TO_IMPL(interface_msg_ref) ((woden_interface_msg_ref_impl_t *) interface_msg_ref)

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_free(
    void *interface_msg_ref,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_interface_msg_ref_super_objs(
    void *interface_msg_ref,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_interface_msg_ref_type(
    void *interface_msg_ref,
    const axis2_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_interface_msg_ref_get_base_impl(
    void *interface_msg_ref,
    const axis2_env_t *env);

/* ************************************************************
 *  Interface Message Reference  methods (the WSDL Component model)
 * ************************************************************/

void *AXIS2_CALL
woden_interface_msg_ref_get_msg_label(
    void *interface_msg_ref,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_msg_ref_get_direction(
    void *interface_msg_ref,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
woden_interface_msg_ref_get_msg_content_model(
    void *interface_msg_ref,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_msg_ref_get_element_declaration(
    void *interface_msg_ref,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_msg_ref_to_element(
    void *interface_msg_ref,
    const axis2_env_t *env);

/* ************************************************************
 *  Interface Message Reference Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_msg_label(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *msg_label);

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_direction(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *dir);


axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_msg_content_model(
    void *interface_msg_ref,
    const axis2_env_t *env,
    axis2_char_t *nm_token);

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_element_qname(
    void *interface_msg_ref,
    const axis2_env_t *env,
    axis2_qname_t *element);

axis2_qname_t *AXIS2_CALL
woden_interface_msg_ref_get_element_qname(
    void *interface_msg_ref,
    const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_msg_ref_get_element(
    void *interface_msg_ref,
    const axis2_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_element_declaration(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *element);

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_types(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *types);


static woden_interface_msg_ref_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_interface_msg_ref_free_ops(
    void *interface_msg_ref,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_interface_msg_ref_element(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_interface_msg_ref_element_ops_t));
    woden_interface_msg_ref_element_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            interface_msg_ref_element), env, interface_msg_ref_impl->methods);
    return interface_msg_ref;
}

AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_nested_configurable(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_configurable_ops_t));
    woden_nested_configurable_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            nested_configurable), env, interface_msg_ref_impl->nested_configurable,
            interface_msg_ref_impl->methods);
    return interface_msg_ref;
}

AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_configurable(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    void *configurable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                interface_msg_ref_impl->nested_configurable, env);
    woden_configurable_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            nested_configurable.base.configurable), env, configurable, interface_msg_ref_impl->methods);
    return interface_msg_ref;
}

AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_nested_element(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.nested_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            interface_msg_ref_element.base.nested_element), env, interface_msg_ref_impl->methods);
    return interface_msg_ref;
}


AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_configurable_element(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.configurable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            interface_msg_ref_element.base.configurable_element), env, interface_msg_ref_impl->methods);
    return interface_msg_ref;
}

AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_documentable_element(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            interface_msg_ref_element.base.documentable_element), env,
            interface_msg_ref_impl->methods);
    return interface_msg_ref;
}

AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_documentable(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.
    configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            nested_configurable.base.configurable.base.documentable), env, NULL,
            interface_msg_ref_impl->methods);
    return interface_msg_ref;
}

AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_attr_extensible(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.documentable_element.
    wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            interface_msg_ref_element.base.documentable_element.wsdl_element.base.
            attr_extensible), env, NULL, interface_msg_ref_impl->methods);
    return interface_msg_ref;
}


AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_to_element_extensible(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_msg_ref)
    {
	  interface_msg_ref = (void *) create (env);
	  interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;
    }
    else
        interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) interface_msg_ref;

    if (interface_msg_ref)
	  woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.documentable_element.
    wsdl_element.base.element_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(interface_msg_ref_impl->interface_msg_ref.base.
            interface_msg_ref_element.base.documentable_element.wsdl_element.base.
            element_extensible), env, NULL, interface_msg_ref_impl->methods);
    return interface_msg_ref;
}


/************************End of Woden C Internal Methods***********************/
static woden_interface_msg_ref_t *
create(const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_msg_ref_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_interface_msg_ref_impl_t));

    interface_msg_ref_impl->obj_type = WODEN_INTERFACE_MSG_REF;
    interface_msg_ref_impl->super = NULL;
    interface_msg_ref_impl->methods = NULL;
    interface_msg_ref_impl->f_msg_label = NULL;
    interface_msg_ref_impl->f_direction = NULL;
    interface_msg_ref_impl->f_msg_content_model = NULL;
    interface_msg_ref_impl->f_element_decl = NULL;
    interface_msg_ref_impl->f_types = NULL;
    interface_msg_ref_impl->f_element_qname = NULL;

    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.ops = NULL;
    interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.ops = NULL;
    interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.configurable.ops =
        NULL;
    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
    nested_element.ops = NULL;
    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
    configurable_element.ops = NULL;
    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
    documentable_element.ops = NULL;
    interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.configurable.base.
    documentable.ops = NULL;
    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
    documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
    documentable_element.wsdl_element.base.element_extensible.ops = NULL;


    interface_msg_ref_impl->interface_msg_ref.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_interface_msg_ref_ops_t));

    interface_msg_ref_impl->interface_msg_ref.ops->free = woden_interface_msg_ref_free;
    interface_msg_ref_impl->interface_msg_ref.ops->super_objs = woden_interface_msg_ref_super_objs;
    interface_msg_ref_impl->interface_msg_ref.ops->type = woden_interface_msg_ref_type;
    interface_msg_ref_impl->interface_msg_ref.ops->get_base_impl = woden_interface_msg_ref_get_base_impl;

    interface_msg_ref_impl->interface_msg_ref.ops->get_msg_label =
        woden_interface_msg_ref_get_msg_label;
    interface_msg_ref_impl->interface_msg_ref.ops->get_direction =
        woden_interface_msg_ref_get_direction;
    interface_msg_ref_impl->interface_msg_ref.ops->get_msg_content_model =
        woden_interface_msg_ref_get_msg_content_model;
    interface_msg_ref_impl->interface_msg_ref.ops->get_element_declaration =
        woden_interface_msg_ref_get_element_declaration;
    interface_msg_ref_impl->interface_msg_ref.ops->to_element =
        woden_interface_msg_ref_to_element;
    interface_msg_ref_impl->interface_msg_ref.ops->set_element_declaration =
        woden_interface_msg_ref_set_element_declaration;
    interface_msg_ref_impl->interface_msg_ref.ops->set_types =
        woden_interface_msg_ref_set_types;

    interface_msg_ref_impl->methods = axis2_hash_make(env);
    if (!interface_msg_ref_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_msg_ref_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_free);
    axis2_hash_set(interface_msg_ref_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_interface_msg_ref_super_objs);
    axis2_hash_set(interface_msg_ref_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_interface_msg_ref_type);

    axis2_hash_set(interface_msg_ref_impl->methods, "get_msg_label",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_get_msg_label);
    axis2_hash_set(interface_msg_ref_impl->methods, "get_direction",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_get_direction);
    axis2_hash_set(interface_msg_ref_impl->methods, "get_msg_content_model",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_get_msg_content_model);
    axis2_hash_set(interface_msg_ref_impl->methods, "get_element_declaration",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_get_element_declaration);
    axis2_hash_set(interface_msg_ref_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_to_element);
    axis2_hash_set(interface_msg_ref_impl->methods, "set_msg_label",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_set_msg_label);
    axis2_hash_set(interface_msg_ref_impl->methods, "set_msg_content_model",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_set_msg_content_model);
    axis2_hash_set(interface_msg_ref_impl->methods, "set_element_qname",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_set_element_qname);
    axis2_hash_set(interface_msg_ref_impl->methods, "get_element_qname",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_get_element_qname);
    axis2_hash_set(interface_msg_ref_impl->methods, "get_element",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_get_element);
    axis2_hash_set(interface_msg_ref_impl->methods, "set_direction",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_set_direction);
    axis2_hash_set(interface_msg_ref_impl->methods, "set_element_declaration",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_set_element_declaration);
    axis2_hash_set(interface_msg_ref_impl->methods, "set_types",
            AXIS2_HASH_KEY_STRING,
            woden_interface_msg_ref_set_types);

    return &(interface_msg_ref_impl->interface_msg_ref);
}

AXIS2_EXTERN woden_interface_msg_ref_t * AXIS2_CALL
woden_interface_msg_ref_create(const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    void *configurable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_msg_ref_impl = (woden_interface_msg_ref_impl_t *) create(env);

    interface_msg_ref_impl->nested_configurable = woden_nested_configurable_create(env);

    interface_msg_ref_impl->super = axis2_hash_make(env);
    if (!interface_msg_ref_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_msg_ref_impl->super, "WODEN_INTERFACE_MSG_REF",
            AXIS2_HASH_KEY_STRING,
            &(interface_msg_ref_impl->interface_msg_ref));
    axis2_hash_set(interface_msg_ref_impl->super, "WODEN_NESTED_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING,
            interface_msg_ref_impl->nested_configurable);
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                interface_msg_ref_impl->nested_configurable, env);
    axis2_hash_set(interface_msg_ref_impl->super, "WODEN_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING, configurable);

    return &(interface_msg_ref_impl->interface_msg_ref);
}

static axis2_status_t
woden_interface_msg_ref_free_ops(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_msg_ref_impl = INTF_TO_IMPL(interface_msg_ref);

    if (interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                interface_msg_ref_element.ops);
        interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.ops = NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                nested_configurable.ops);
        interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.ops =
            NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                nested_configurable.base.configurable.ops);
        interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.configurable.ops =
            NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                interface_msg_ref_element.base.nested_element.ops);
        interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
        nested_element.ops = NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
            configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                interface_msg_ref_element.base.configurable_element.ops);
        interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
        configurable_element.ops = NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                interface_msg_ref_element.base.documentable_element.ops);
        interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
        documentable_element.ops = NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                nested_configurable.base.configurable.base.documentable.ops);
        interface_msg_ref_impl->interface_msg_ref.base.nested_configurable.base.
        configurable.base.documentable.ops = NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                interface_msg_ref_element.base.documentable_element.wsdl_element.base.
                attr_extensible.ops);
        interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    }

    if (interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->interface_msg_ref.base.
                interface_msg_ref_element.base.documentable_element.wsdl_element.base.
                element_extensible.ops);
        interface_msg_ref_impl->interface_msg_ref.base.interface_msg_ref_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_interface_msg_ref_free(void *interface_msg_ref,
        const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_msg_ref_impl = INTF_TO_IMPL(interface_msg_ref);

    if (interface_msg_ref_impl->f_msg_label)
    {
        WODEN_MSG_LABEL_FREE(interface_msg_ref_impl->
                f_msg_label, env);
        interface_msg_ref_impl->f_msg_label = NULL;
    }

    if (interface_msg_ref_impl->f_direction)
    {
        WODEN_DIRECTION_FREE(interface_msg_ref_impl->f_direction, env);
        interface_msg_ref_impl->f_direction = NULL;
    }

    if (interface_msg_ref_impl->f_msg_content_model)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->f_msg_content_model);
        interface_msg_ref_impl->f_msg_content_model = NULL;
    }

    if (interface_msg_ref_impl->f_element_decl)
    {
        WODEN_ELEMENT_DECL_FREE(interface_msg_ref_impl->f_element_decl, env);
        interface_msg_ref_impl->f_element_decl = NULL;
    }

    if (interface_msg_ref_impl->f_element_qname)
    {
        AXIS2_QNAME_FREE(interface_msg_ref_impl->f_element_qname, env);
        interface_msg_ref_impl->f_element_qname = NULL;
    }

    if (interface_msg_ref_impl->f_types)
    {
        WODEN_TYPES_FREE(interface_msg_ref_impl->f_types, env);
        interface_msg_ref_impl->f_types = NULL;
    }

    if (interface_msg_ref_impl->super)
    {
        axis2_hash_free(interface_msg_ref_impl->super, env);
        interface_msg_ref_impl->super = NULL;
    }

    if (interface_msg_ref_impl->methods)
    {
        axis2_hash_free(interface_msg_ref_impl->methods, env);
        interface_msg_ref_impl->methods = NULL;
    }

    if (interface_msg_ref_impl->nested_configurable)
    {
        WODEN_NESTED_CONFIGURABLE_FREE(interface_msg_ref_impl->
                nested_configurable, env);
        interface_msg_ref_impl->nested_configurable = NULL;
    }

    woden_interface_msg_ref_free_ops(interface_msg_ref, env);

    if ((&(interface_msg_ref_impl->interface_msg_ref))->ops)
    {
        AXIS2_FREE(env->allocator, (&(interface_msg_ref_impl->interface_msg_ref))->ops);
        (&(interface_msg_ref_impl->interface_msg_ref))->ops = NULL;
    }

    if (interface_msg_ref_impl)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl);
        interface_msg_ref_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_interface_msg_ref_super_objs(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_msg_ref_impl = INTF_TO_IMPL(interface_msg_ref);

    return interface_msg_ref_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_interface_msg_ref_type(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_msg_ref_impl = INTF_TO_IMPL(interface_msg_ref);

    return interface_msg_ref_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_interface_msg_ref_get_base_impl(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_msg_ref_impl = INTF_TO_IMPL(interface_msg_ref);

    return interface_msg_ref_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_resolve_methods(
    woden_interface_msg_ref_t *interface_msg_ref,
    const axis2_env_t *env,
    woden_interface_msg_ref_t *interface_msg_ref_impl,
    axis2_hash_t *methods)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    interface_msg_ref_impl_l = INTF_TO_IMPL(interface_msg_ref_impl);

    interface_msg_ref->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    interface_msg_ref->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    interface_msg_ref->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    interface_msg_ref->ops->get_msg_label = axis2_hash_get(methods,
            "", AXIS2_HASH_KEY_STRING);
    if (!interface_msg_ref->ops->get_msg_label && interface_msg_ref_impl_l)
        interface_msg_ref->ops->get_msg_label =
            interface_msg_ref_impl_l->interface_msg_ref.ops->get_msg_label;

    interface_msg_ref->ops->get_direction = axis2_hash_get(methods,
            "get_direction", AXIS2_HASH_KEY_STRING);
    if (!interface_msg_ref->ops->get_direction && interface_msg_ref_impl_l)
        interface_msg_ref->ops->get_direction =
            interface_msg_ref_impl_l->interface_msg_ref.ops->get_direction;

    interface_msg_ref->ops->get_msg_content_model = axis2_hash_get(methods,
            "get_msg_content_model", AXIS2_HASH_KEY_STRING);
    if (!interface_msg_ref->ops->get_msg_content_model && interface_msg_ref_impl_l)
        interface_msg_ref->ops->get_msg_content_model =
            interface_msg_ref_impl_l->interface_msg_ref.ops->get_msg_content_model;

    interface_msg_ref->ops->get_element_declaration = axis2_hash_get(methods,
            "get_element_declaration", AXIS2_HASH_KEY_STRING);
    if (!interface_msg_ref->ops->get_element_declaration && interface_msg_ref_impl_l)
        interface_msg_ref->ops->get_element_declaration =
            interface_msg_ref_impl_l->interface_msg_ref.ops->get_element_declaration;

    interface_msg_ref->ops->to_element = axis2_hash_get(methods,
            "to_element", AXIS2_HASH_KEY_STRING);
    if (!interface_msg_ref->ops->to_element && interface_msg_ref_impl_l)
        interface_msg_ref->ops->to_element =
            interface_msg_ref_impl_l->interface_msg_ref.ops->to_element;

    interface_msg_ref->ops->set_element_declaration = axis2_hash_get(methods,
            "set_element_declaration", AXIS2_HASH_KEY_STRING);
    if (!interface_msg_ref->ops->set_element_declaration && interface_msg_ref_impl_l)
        interface_msg_ref->ops->set_element_declaration =
            interface_msg_ref_impl_l->interface_msg_ref.ops->set_element_declaration;

    interface_msg_ref->ops->set_types = axis2_hash_get(methods,
            "set_types", AXIS2_HASH_KEY_STRING);
    if (!interface_msg_ref->ops->set_types && interface_msg_ref_impl_l)
        interface_msg_ref->ops->set_types =
            interface_msg_ref_impl_l->interface_msg_ref.ops->set_types;

    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Message Reference  methods (the WSDL Component model)
 * ************************************************************/

void *AXIS2_CALL
woden_interface_msg_ref_get_msg_label(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    return interface_msg_ref_impl->f_msg_label;
}

void *AXIS2_CALL
woden_interface_msg_ref_get_direction(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    return interface_msg_ref_impl->f_direction;
}

axis2_char_t *AXIS2_CALL
woden_interface_msg_ref_get_msg_content_model(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    return interface_msg_ref_impl->f_msg_content_model;
}

void *AXIS2_CALL
woden_interface_msg_ref_get_element_declaration(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    return interface_msg_ref_impl->f_element_decl;
}

void *AXIS2_CALL
woden_interface_msg_ref_to_element(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    return &(interface_msg_ref_impl->interface_msg_ref);
}

/* ************************************************************
 *  Interface Message Reference Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_msg_label(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *msg_label)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_label, AXIS2_FAILURE);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    if (!interface_msg_ref_impl->f_msg_label)
    {
        /* TODO */
    }
    interface_msg_ref_impl->f_msg_label = msg_label;


    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_direction(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *dir)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dir, AXIS2_FAILURE);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    if (!interface_msg_ref_impl->f_direction)
    {
        /* TODO */
    }
    interface_msg_ref_impl->f_direction = dir;


    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_msg_content_model(
    void *interface_msg_ref,
    const axis2_env_t *env,
    axis2_char_t *nm_token)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, nm_token, AXIS2_FAILURE);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    if (interface_msg_ref_impl->f_msg_content_model)
    {
        AXIS2_FREE(env->allocator, interface_msg_ref_impl->f_msg_content_model);
    }
    interface_msg_ref_impl->f_msg_content_model = AXIS2_STRDUP(nm_token, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_element_qname(
    void *interface_msg_ref,
    const axis2_env_t *env,
    axis2_qname_t *element)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    if (interface_msg_ref_impl->f_element_qname)
    {
        AXIS2_QNAME_FREE(interface_msg_ref_impl->f_element_qname, env);
    }
    interface_msg_ref_impl->f_element_qname = AXIS2_QNAME_CLONE(element, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_interface_msg_ref_get_element_qname(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    return interface_msg_ref_impl->f_element_qname;
}

void *AXIS2_CALL
woden_interface_msg_ref_get_element(
    void *interface_msg_ref,
    const axis2_env_t *env)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;
    xml_schema_element_t *xse = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    if (interface_msg_ref_impl->f_types)
    {
        xse = (xml_schema_element_t *)
                WODEN_TYPES_GET_ELEMENT_DECLARATION(
                    interface_msg_ref_impl->f_types, env, interface_msg_ref_impl->
                    f_element_qname);
    }

    return xse;
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_element_declaration(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *element)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    if (!interface_msg_ref_impl->f_element_decl)
    {
        /* TODO */
    }
    interface_msg_ref_impl->f_element_decl = element;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_msg_ref_set_types(
    void *interface_msg_ref,
    const axis2_env_t *env,
    void *types)
{
    woden_interface_msg_ref_impl_t *interface_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, types, AXIS2_FAILURE);
    super = WODEN_INTERFACE_MSG_REF_SUPER_OBJS(interface_msg_ref, env);
    interface_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super,
            "WODEN_INTERFACE_MSG_REF", AXIS2_HASH_KEY_STRING));

    if (!interface_msg_ref_impl->f_types)
    {
        /* TODO */
    }
    interface_msg_ref_impl->f_types = types;

    return AXIS2_SUCCESS;
}

