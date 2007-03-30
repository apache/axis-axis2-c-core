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

#include <woden_soap_binding_exts.h>
#include <woden_component_exts.h>
#include <woden_wsdl_element.h>
#include <woden_string_attr.h>
#include <woden_uri_attr.h>
#include "woden_soap_constants.h"

typedef struct woden_soap_binding_exts_impl woden_soap_binding_exts_impl_t;

/**
 * @brief Soap Binding Extensions Struct Impl
 *   Axis2 Soap Binding Extensions
 */
struct woden_soap_binding_exts_impl
{
    woden_soap_binding_exts_t binding_exts;
    woden_component_exts_t *component_exts;
    axis2_hash_t *methods;
    axis2_hash_t *super;
    woden_obj_types_t obj_type;

    axis2_qname_t *qname;
};

#define INTF_TO_IMPL(binding_exts) ((woden_soap_binding_exts_impl_t *) binding_exts)

axis2_status_t AXIS2_CALL
woden_soap_binding_exts_free(
    void *binding_exts,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_soap_binding_exts_super_objs(
    void *binding_exts,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_soap_binding_exts_type(
    void *binding_exts,
    const axis2_env_t *env);

woden_component_exts_t *AXIS2_CALL
woden_soap_binding_exts_get_base_impl(
    void *binding_exts,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_version(
    void *binding_exts,
    const axis2_env_t *env);

axis2_uri_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_underlying_protocol(
    void *binding_exts,
    const axis2_env_t *env);

axis2_uri_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_mep_default(
    void *binding_exts,
    const axis2_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_modules(
    void *binding_exts,
    const axis2_env_t *env);


static woden_soap_binding_exts_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_soap_binding_exts_free_ops(
    void *binding_exts,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_soap_binding_exts_t * AXIS2_CALL
woden_soap_binding_exts_to_component_exts(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_exts)
    {
        binding_exts_impl = (woden_soap_binding_exts_impl_t *) create(env);
    }
    else
        binding_exts_impl = (woden_soap_binding_exts_impl_t *) binding_exts;
    woden_soap_binding_exts_free_ops(binding_exts, env);

    binding_exts_impl->binding_exts.component_exts.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_component_exts_ops_t));
    woden_component_exts_resolve_methods(&(binding_exts_impl->binding_exts.
            component_exts), env, binding_exts_impl->methods);
    return binding_exts;
}

/************************End of Woden C Internal Methods***********************/
static woden_soap_binding_exts_t *
create(const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_soap_binding_exts_impl_t));

    binding_exts_impl->obj_type = WODEN_SOAP_BINDING_EXTS;
    binding_exts_impl->super = NULL;
    binding_exts_impl->methods = NULL;

    binding_exts_impl->component_exts = NULL;
    binding_exts_impl->qname = NULL;

    binding_exts_impl->binding_exts.component_exts.ops = NULL;

    binding_exts_impl->binding_exts.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_soap_binding_exts_ops_t));

    binding_exts_impl->binding_exts.ops->free = woden_soap_binding_exts_free;
    binding_exts_impl->binding_exts.ops->super_objs =
        woden_soap_binding_exts_super_objs;
    binding_exts_impl->binding_exts.ops->type =
        woden_soap_binding_exts_type;
    binding_exts_impl->binding_exts.ops->get_base_impl =
        woden_soap_binding_exts_get_base_impl;

    binding_exts_impl->binding_exts.ops->get_soap_version =
        woden_soap_binding_exts_get_soap_version;
    binding_exts_impl->binding_exts.ops->get_soap_underlying_protocol =
        woden_soap_binding_exts_get_soap_underlying_protocol;
    binding_exts_impl->binding_exts.ops->get_soap_mep_default =
        woden_soap_binding_exts_get_soap_mep_default;
    binding_exts_impl->binding_exts.ops->get_soap_modules =
        woden_soap_binding_exts_get_soap_modules;

    binding_exts_impl->methods = axis2_hash_make(env);
    if (!binding_exts_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_exts_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_soap_binding_exts_free);
    axis2_hash_set(binding_exts_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_soap_binding_exts_super_objs);
    axis2_hash_set(binding_exts_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_soap_binding_exts_type);

    axis2_hash_set(binding_exts_impl->methods, "get_soap_version",
            AXIS2_HASH_KEY_STRING,
            woden_soap_binding_exts_get_soap_version);
    axis2_hash_set(binding_exts_impl->methods, "get_soap_underlying_protocol",
            AXIS2_HASH_KEY_STRING,
            woden_soap_binding_exts_get_soap_underlying_protocol);
    axis2_hash_set(binding_exts_impl->methods, "get_soap_mep_default",
            AXIS2_HASH_KEY_STRING,
            woden_soap_binding_exts_get_soap_mep_default);
    axis2_hash_set(binding_exts_impl->methods, "get_soap_modules",
            AXIS2_HASH_KEY_STRING,
            woden_soap_binding_exts_get_soap_modules);

    return &(binding_exts_impl->binding_exts);
}

AXIS2_EXTERN woden_soap_binding_exts_t * AXIS2_CALL
woden_soap_binding_exts_create(const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = (woden_soap_binding_exts_impl_t *) create(env);

    binding_exts_impl->component_exts = woden_component_exts_create(env);

    binding_exts_impl->super = axis2_hash_make(env);
    if (!binding_exts_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_exts_impl->super, "WODEN_SOAP_BINDING_EXTS", AXIS2_HASH_KEY_STRING,
            &(binding_exts_impl->binding_exts));
    axis2_hash_set(binding_exts_impl->super, "WODEN_COMPONENT_EXTS", AXIS2_HASH_KEY_STRING,
            binding_exts_impl->component_exts);

    return &(binding_exts_impl->binding_exts);
}

static axis2_status_t
woden_soap_binding_exts_free_ops(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    if (binding_exts_impl->binding_exts.component_exts.ops)
    {
        AXIS2_FREE(env->allocator, binding_exts_impl->binding_exts.
                component_exts.ops);
        binding_exts_impl->binding_exts.component_exts.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_soap_binding_exts_free(void *binding_exts,
        const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    if (binding_exts_impl->super)
    {
        axis2_hash_free(binding_exts_impl->super, env);
        binding_exts_impl->super = NULL;
    }

    if (binding_exts_impl->methods)
    {
        axis2_hash_free(binding_exts_impl->methods, env);
        binding_exts_impl->methods = NULL;
    }

    if (binding_exts_impl->component_exts)
    {
        WODEN_COMPONENT_EXTS_FREE(binding_exts_impl->component_exts, env);
        binding_exts_impl->component_exts = NULL;
    }

    if (binding_exts_impl->qname)
    {
        axis2_qname_free(binding_exts_impl->qname, env);
        binding_exts_impl->qname = NULL;
    }
    woden_soap_binding_exts_free_ops(binding_exts, env);

    if ((&(binding_exts_impl->binding_exts))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_exts_impl->binding_exts))->ops);
        (&(binding_exts_impl->binding_exts))->ops = NULL;
    }

    if (binding_exts_impl)
    {
        AXIS2_FREE(env->allocator, binding_exts_impl);
        binding_exts_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_soap_binding_exts_super_objs(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    return binding_exts_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_soap_binding_exts_type(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    return binding_exts_impl->obj_type;
}

woden_component_exts_t *AXIS2_CALL
woden_soap_binding_exts_get_base_impl(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    return binding_exts_impl->component_exts;
}

axis2_status_t AXIS2_CALL
woden_soap_binding_exts_resolve_methods(
    woden_soap_binding_exts_t *binding_exts,
    const axis2_env_t *env,
    woden_soap_binding_exts_t *binding_exts_impl,
    axis2_hash_t *methods)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_exts_impl_l = INTF_TO_IMPL(binding_exts_impl);

    binding_exts->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    binding_exts->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    binding_exts->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    binding_exts->ops->get_soap_version = axis2_hash_get(methods,
            "get_soap_version", AXIS2_HASH_KEY_STRING);
    if (!binding_exts->ops->get_soap_version && binding_exts_impl_l)
        binding_exts->ops->get_soap_version =
            binding_exts_impl_l->binding_exts.ops->get_soap_version;

    binding_exts->ops->get_soap_underlying_protocol = axis2_hash_get(methods,
            "get_soap_underlying_protocol", AXIS2_HASH_KEY_STRING);
    if (!binding_exts->ops->get_soap_underlying_protocol && binding_exts_impl_l)
        binding_exts->ops->get_soap_underlying_protocol =
            binding_exts_impl_l->binding_exts.ops->get_soap_underlying_protocol;

    binding_exts->ops->get_soap_mep_default = axis2_hash_get(methods,
            "get_soap_mep_default", AXIS2_HASH_KEY_STRING);
    if (!binding_exts->ops->get_soap_mep_default && binding_exts_impl_l)
        binding_exts->ops->get_soap_mep_default =
            binding_exts_impl_l->binding_exts.ops->get_soap_mep_default;

    binding_exts->ops->get_soap_modules = axis2_hash_get(methods,
            "get_soap_modules", AXIS2_HASH_KEY_STRING);
    if (!binding_exts->ops->get_soap_modules && binding_exts_impl_l)
        binding_exts->ops->get_soap_modules =
            binding_exts_impl_l->binding_exts.ops->get_soap_modules;

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_version(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;
    woden_string_attr_t *version = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_exts_impl->component_exts, env);
    parent_element = woden_wsdl_element_to_attr_extensible(parent_element,
            env);
    binding_exts_impl->qname = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_VERSION);
    version = (woden_string_attr_t *)
            WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(parent_element, env, binding_exts_impl->qname);
    return version  ? WODEN_STRING_ATTR_GET_STRING(version, env) :
            NULL;
}

axis2_uri_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_underlying_protocol(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;
    woden_uri_attr_t *protocol = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_exts_impl->component_exts, env);

    parent_element = woden_wsdl_element_to_attr_extensible(parent_element,
            env);
    binding_exts_impl->qname = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_PROTOCOL);
    protocol = (woden_uri_attr_t *) WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(
                parent_element, env, binding_exts_impl->qname);
    return protocol  ? WODEN_URI_ATTR_GET_URI(protocol, env) : NULL;
}

axis2_uri_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_mep_default(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;
    woden_uri_attr_t *mep_default = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_exts_impl->component_exts, env);

    parent_element = woden_wsdl_element_to_attr_extensible(parent_element,
            env);
    binding_exts_impl->qname = axis2_qname_create_from_string(env, WODEN_Q_ATTR_SOAP_MEPDEFAULT);
    mep_default = (woden_uri_attr_t *)
            WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(parent_element, env, binding_exts_impl->qname);
    return mep_default  ? WODEN_URI_ATTR_GET_URI(mep_default, env) :
            NULL;
}

axutil_array_list_t *AXIS2_CALL
woden_soap_binding_exts_get_soap_modules(
    void *binding_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_exts_impl_t *binding_exts_impl = NULL;
    axutil_array_list_t *ext_els = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_exts_impl = INTF_TO_IMPL(binding_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_exts_impl->component_exts, env);
    parent_element = woden_wsdl_element_to_attr_extensible(parent_element,
            env);
    binding_exts_impl->qname = axis2_qname_create_from_string(env, WODEN_Q_ELEM_SOAP_MODULE);
    ext_els = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
                parent_element, env, binding_exts_impl->qname);
    return ext_els;
}

