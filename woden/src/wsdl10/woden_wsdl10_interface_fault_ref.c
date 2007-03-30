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
#include <woden_wsdl10_interface_fault_ref.h>
#include <woden_types.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>
#include <woden_element_decl.h>
#include <woden_direction.h>

typedef struct woden_wsdl10_interface_fault_ref_impl woden_wsdl10_interface_fault_ref_impl_t;

/**
 * @brief Interface Message Reference Struct Impl
 *   Axis2 Interface Message Reference
 */
struct woden_wsdl10_interface_fault_ref_impl
{
    woden_wsdl10_interface_fault_ref_t interface_fault_ref;
    woden_nested_configurable_t *nested_configurable;
    woden_obj_types_t obj_type;
    axutil_hash_t *super;
    axutil_hash_t *methods;

    void *f_direction;
    axis2_qname_t *f_msg_qname;
    axis2_qname_t *f_qname;
};

#define INTF_TO_IMPL(interface_fault_ref) ((woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref)

axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_free(
    void *interface_fault_ref,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_super_objs(
    void *interface_fault_ref,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_type(
    void *interface_fault_ref,
    const axutil_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_base_impl(
    void *interface_fault_ref,
    const axutil_env_t *env);

/* ************************************************************
 *  Interface Message Reference  methods (the WSDL Component model)
 * ************************************************************/

void *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_direction(
    void *interface_fault_ref,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_element(
    void *interface_fault_ref,
    const axutil_env_t *env);

/* ************************************************************
 *  Interface Message Reference Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_set_direction(
    void *interface_fault_ref,
    const axutil_env_t *env,
    void *dir);

axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_set_qname(
    void *interface_fault_ref,
    const axutil_env_t *env,
    axis2_qname_t *qname);

axis2_qname_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_qname(
    void *interface_fault_ref,
    const axutil_env_t *env);


axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_set_msg_qname(
    void *interface_fault_ref,
    const axutil_env_t *env,
    axis2_qname_t *element);

axis2_qname_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_msg_qname(
    void *interface_fault_ref,
    const axutil_env_t *env);

static woden_wsdl10_interface_fault_ref_t *
create(const axutil_env_t *env);

static axis2_status_t
woden_wsdl10_interface_fault_ref_free_ops(
    void *interface_fault_ref,
    const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_interface_fault_ref_element(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) create(env);
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_wsdl10_interface_fault_ref_element_ops_t));
    woden_wsdl10_interface_fault_ref_element_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            interface_fault_ref_element), env, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_nested_configurable(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_configurable_ops_t));
    woden_nested_configurable_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            nested_configurable), env, interface_fault_ref_impl->nested_configurable,
            interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_configurable(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    void *configurable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                interface_fault_ref_impl->nested_configurable, env);
    woden_configurable_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            nested_configurable.base.configurable), env, configurable, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_nested_element(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.nested_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            interface_fault_ref_element.base.nested_element), env, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}


AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_configurable_element(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.configurable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            interface_fault_ref_element.base.configurable_element), env, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_documentable_element(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            interface_fault_ref_element.base.documentable_element), env,
            interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_documentable(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.
    configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            nested_configurable.base.configurable.base.documentable), env, NULL,
            interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_attr_extensible(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.documentable_element.
    wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            interface_fault_ref_element.base.documentable_element.wsdl_element.base.
            attr_extensible), env, NULL, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}


AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_element_extensible(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!interface_fault_ref)
    {
	  interface_fault_ref = (void *) create (env);
	  interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *)interface_fault_ref;
    }
    else
        interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) interface_fault_ref;

    if (interface_fault_ref)
	  woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.documentable_element.
    wsdl_element.base.element_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            interface_fault_ref_element.base.documentable_element.wsdl_element.base.
            element_extensible), env, NULL, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}


/************************End of Woden C Internal Methods***********************/
static woden_wsdl10_interface_fault_ref_t *
create(const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_fault_ref_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_interface_fault_ref_impl_t));

    interface_fault_ref_impl->obj_type = WODEN_WSDL10_INTERFACE_FAULT_REF;
    interface_fault_ref_impl->super = NULL;
    interface_fault_ref_impl->methods = NULL;
    interface_fault_ref_impl->f_direction = NULL;
    interface_fault_ref_impl->f_qname = NULL;
    interface_fault_ref_impl->f_msg_qname = NULL;

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops =
        NULL;
    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
    nested_element.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
    configurable_element.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
    documentable_element.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.base.
    documentable.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
    documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
    documentable_element.wsdl_element.base.element_extensible.ops = NULL;


    interface_fault_ref_impl->interface_fault_ref.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_interface_fault_ref_ops_t));

    interface_fault_ref_impl->interface_fault_ref.ops->free = woden_wsdl10_interface_fault_ref_free;
    interface_fault_ref_impl->interface_fault_ref.ops->super_objs = woden_wsdl10_interface_fault_ref_super_objs;
    interface_fault_ref_impl->interface_fault_ref.ops->type = woden_wsdl10_interface_fault_ref_type;
    interface_fault_ref_impl->interface_fault_ref.ops->get_base_impl = woden_wsdl10_interface_fault_ref_get_base_impl;

    interface_fault_ref_impl->interface_fault_ref.ops->get_direction =
        woden_wsdl10_interface_fault_ref_get_direction;
    interface_fault_ref_impl->interface_fault_ref.ops->to_element =
        woden_wsdl10_interface_fault_ref_to_element;

    interface_fault_ref_impl->methods = axutil_hash_make(env);
    if (!interface_fault_ref_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(interface_fault_ref_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_free);
    axutil_hash_set(interface_fault_ref_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_wsdl10_interface_fault_ref_super_objs);
    axutil_hash_set(interface_fault_ref_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_wsdl10_interface_fault_ref_type);

    axutil_hash_set(interface_fault_ref_impl->methods, "get_direction",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_get_direction);
    axutil_hash_set(interface_fault_ref_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_to_element);
    axutil_hash_set(interface_fault_ref_impl->methods, "set_qname",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_set_qname);
    axutil_hash_set(interface_fault_ref_impl->methods, "get_qname",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_get_qname);
    axutil_hash_set(interface_fault_ref_impl->methods, "set_msg_qname",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_set_msg_qname);
    axutil_hash_set(interface_fault_ref_impl->methods, "get_msg_qname",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_get_msg_qname);
    axutil_hash_set(interface_fault_ref_impl->methods, "set_direction",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_interface_fault_ref_set_direction);

    return &(interface_fault_ref_impl->interface_fault_ref);
}

AXIS2_EXTERN woden_wsdl10_interface_fault_ref_t * AXIS2_CALL
woden_wsdl10_interface_fault_ref_create(const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    void *configurable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_fault_ref_impl = (woden_wsdl10_interface_fault_ref_impl_t *) create(env);

    interface_fault_ref_impl->nested_configurable = woden_nested_configurable_create(env);

    interface_fault_ref_impl->super = axutil_hash_make(env);
    if (!interface_fault_ref_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(interface_fault_ref_impl->super, "WODEN_WSDL10_INTERFACE_FAULT_REF",
            AXIS2_HASH_KEY_STRING,
            &(interface_fault_ref_impl->interface_fault_ref));
    axutil_hash_set(interface_fault_ref_impl->super, "WODEN_NESTED_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING,
            interface_fault_ref_impl->nested_configurable);
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                interface_fault_ref_impl->nested_configurable, env);
    axutil_hash_set(interface_fault_ref_impl->super, "WODEN_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING, configurable);

    return &(interface_fault_ref_impl->interface_fault_ref);
}

static axis2_status_t
woden_wsdl10_interface_fault_ref_free_ops(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    if (interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                interface_fault_ref_element.ops);
        interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops = NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                nested_configurable.ops);
        interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops =
            NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                nested_configurable.base.configurable.ops);
        interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops =
            NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                interface_fault_ref_element.base.nested_element.ops);
        interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
        nested_element.ops = NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
            configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                interface_fault_ref_element.base.configurable_element.ops);
        interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
        configurable_element.ops = NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                interface_fault_ref_element.base.documentable_element.ops);
        interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
        documentable_element.ops = NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                nested_configurable.base.configurable.base.documentable.ops);
        interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.
        configurable.base.documentable.ops = NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                interface_fault_ref_element.base.documentable_element.wsdl_element.base.
                attr_extensible.ops);
        interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    }

    if (interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                interface_fault_ref_element.base.documentable_element.wsdl_element.base.
                element_extensible.ops);
        interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_free(void *interface_fault_ref,
        const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    if (interface_fault_ref_impl->f_direction)
    {
        WODEN_DIRECTION_FREE(interface_fault_ref_impl->f_direction, env);
        interface_fault_ref_impl->f_direction = NULL;
    }

    if (interface_fault_ref_impl->f_qname)
    {
        axis2_qname_free(interface_fault_ref_impl->f_qname, env);
        interface_fault_ref_impl->f_qname = NULL;
    }

    if (interface_fault_ref_impl->f_msg_qname)
    {
        axis2_qname_free(interface_fault_ref_impl->f_msg_qname, env);
        interface_fault_ref_impl->f_msg_qname = NULL;
    }

    if (interface_fault_ref_impl->super)
    {
        axutil_hash_free(interface_fault_ref_impl->super, env);
        interface_fault_ref_impl->super = NULL;
    }

    if (interface_fault_ref_impl->methods)
    {
        axutil_hash_free(interface_fault_ref_impl->methods, env);
        interface_fault_ref_impl->methods = NULL;
    }

    if (interface_fault_ref_impl->nested_configurable)
    {
        WODEN_NESTED_CONFIGURABLE_FREE(interface_fault_ref_impl->
                nested_configurable, env);
        interface_fault_ref_impl->nested_configurable = NULL;
    }

    woden_wsdl10_interface_fault_ref_free_ops(interface_fault_ref, env);

    if ((&(interface_fault_ref_impl->interface_fault_ref))->ops)
    {
        AXIS2_FREE(env->allocator, (&(interface_fault_ref_impl->interface_fault_ref))->ops);
        (&(interface_fault_ref_impl->interface_fault_ref))->ops = NULL;
    }

    if (interface_fault_ref_impl)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl);
        interface_fault_ref_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_super_objs(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    return interface_fault_ref_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_type(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    return interface_fault_ref_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_base_impl(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    return interface_fault_ref_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_resolve_methods(
    woden_wsdl10_interface_fault_ref_t *interface_fault_ref,
    const axutil_env_t *env,
    woden_wsdl10_interface_fault_ref_t *interface_fault_ref_impl,
    axutil_hash_t *methods)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    interface_fault_ref_impl_l = INTF_TO_IMPL(interface_fault_ref_impl);

    interface_fault_ref->ops->free = axutil_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    interface_fault_ref->ops->super_objs = axutil_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    interface_fault_ref->ops->type = axutil_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    interface_fault_ref->ops->get_direction = axutil_hash_get(methods,
            "get_direction", AXIS2_HASH_KEY_STRING);
    if (!interface_fault_ref->ops->get_direction && interface_fault_ref_impl_l)
        interface_fault_ref->ops->get_direction =
            interface_fault_ref_impl_l->interface_fault_ref.ops->get_direction;

    interface_fault_ref->ops->to_element = axutil_hash_get(methods,
            "to_element", AXIS2_HASH_KEY_STRING);
    if (!interface_fault_ref->ops->to_element && interface_fault_ref_impl_l)
        interface_fault_ref->ops->to_element =
            interface_fault_ref_impl_l->interface_fault_ref.ops->to_element;

    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Message Reference  methods (the WSDL Component model)
 * ************************************************************/

void *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_direction(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL10_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL10_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return interface_fault_ref_impl->f_direction;
}

void *AXIS2_CALL
woden_wsdl10_interface_fault_ref_to_element(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL10_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL10_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return &(interface_fault_ref_impl->interface_fault_ref);
}

/* ************************************************************
 *  Interface Message Reference Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_set_direction(
    void *interface_fault_ref,
    const axutil_env_t *env,
    void *dir)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dir, AXIS2_FAILURE);
    super = WODEN_WSDL10_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL10_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if (!interface_fault_ref_impl->f_direction)
    {
        /* TODO */
    }
    interface_fault_ref_impl->f_direction = dir;


    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_set_qname(
    void *interface_fault_ref,
    const axutil_env_t *env,
    axis2_qname_t *qname)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_WSDL10_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL10_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if (interface_fault_ref_impl->f_qname)
    {
        axis2_qname_free(interface_fault_ref_impl->f_qname, env);
    }
    interface_fault_ref_impl->f_qname = axis2_qname_clone(qname, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_qname(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL10_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL10_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return interface_fault_ref_impl->f_qname;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_interface_fault_ref_set_msg_qname(
    void *interface_fault_ref,
    const axutil_env_t *env,
    axis2_qname_t *element)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element, AXIS2_FAILURE);
    super = WODEN_WSDL10_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL10_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if (interface_fault_ref_impl->f_msg_qname)
    {
        axis2_qname_free(interface_fault_ref_impl->f_msg_qname, env);
    }
    interface_fault_ref_impl->f_msg_qname = axis2_qname_clone(element, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_wsdl10_interface_fault_ref_get_msg_qname(
    void *interface_fault_ref,
    const axutil_env_t *env)
{
    woden_wsdl10_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_WSDL10_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_WSDL10_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return interface_fault_ref_impl->f_msg_qname;
}


