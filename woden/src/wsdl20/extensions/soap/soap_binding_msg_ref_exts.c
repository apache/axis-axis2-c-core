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

#include <woden_soap_binding_msg_ref_exts.h>
#include <woden_component_exts.h>
#include <woden_wsdl_element.h>
#include <woden_string_attr.h>
#include <woden_qname_or_token_any_attr.h>
#include <woden_qname_list_or_token_any_attr.h>
#include "woden_soap_constants.h"

typedef struct woden_soap_binding_msg_ref_exts_impl woden_soap_binding_msg_ref_exts_impl_t;

/**
 * @brief Soap Binding Message Reference Extensions Struct Impl
 *   Axis2 Soap Binding Message Reference Extensions
 */
struct woden_soap_binding_msg_ref_exts_impl
{
    woden_soap_binding_msg_ref_exts_t binding_msg_ref_exts;
    woden_component_exts_t *component_exts;
    axis2_hash_t *methods;
    axis2_hash_t *super;
    woden_obj_types_t obj_type;

    axis2_qname_t *qname;
};

#define INTF_TO_IMPL(binding_msg_ref_exts) ((woden_soap_binding_msg_ref_exts_impl_t *) binding_msg_ref_exts)

axis2_status_t AXIS2_CALL
woden_soap_binding_msg_ref_exts_free(
    void *binding_msg_ref_exts,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_super_objs(
    void *binding_msg_ref_exts,
    const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_soap_binding_msg_ref_exts_type(
    void *binding_msg_ref_exts,
    const axis2_env_t *env);

woden_component_exts_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_get_base_impl(
    void *binding_msg_ref_exts,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_get_soap_modules(
    void *binding_msg_ref_exts,
    const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_get_soap_headers(
    void *binding_msg_ref_exts,
    const axis2_env_t *env);

static woden_soap_binding_msg_ref_exts_t *
create(
    const axis2_env_t *env);

static axis2_status_t
woden_soap_binding_msg_ref_exts_free_ops(
    void *binding_msg_ref_exts,
    const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_soap_binding_msg_ref_exts_t * AXIS2_CALL
woden_soap_binding_msg_ref_exts_to_component_exts(
    void *binding_msg_ref_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_msg_ref_exts)
    {
        binding_msg_ref_exts_impl = (woden_soap_binding_msg_ref_exts_impl_t *) create(env);
    }
    else
        binding_msg_ref_exts_impl = (woden_soap_binding_msg_ref_exts_impl_t *) binding_msg_ref_exts;
    woden_soap_binding_msg_ref_exts_free_ops(binding_msg_ref_exts, env);

    binding_msg_ref_exts_impl->binding_msg_ref_exts.component_exts.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_component_exts_ops_t));
    woden_component_exts_resolve_methods(&(binding_msg_ref_exts_impl->binding_msg_ref_exts.
            component_exts), env, binding_msg_ref_exts_impl->methods);
    return binding_msg_ref_exts;
}

/************************End of Woden C Internal Methods***********************/
static woden_soap_binding_msg_ref_exts_t *
create(const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_msg_ref_exts_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_soap_binding_msg_ref_exts_impl_t));

    binding_msg_ref_exts_impl->obj_type = WODEN_SOAP_BINDING_MSG_REF_EXTS;
    binding_msg_ref_exts_impl->super = NULL;
    binding_msg_ref_exts_impl->methods = NULL;

    binding_msg_ref_exts_impl->component_exts = NULL;
    binding_msg_ref_exts_impl->qname = NULL;

    binding_msg_ref_exts_impl->binding_msg_ref_exts.component_exts.ops = NULL;

    binding_msg_ref_exts_impl->binding_msg_ref_exts.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_soap_binding_msg_ref_exts_ops_t));

    binding_msg_ref_exts_impl->binding_msg_ref_exts.ops->free = woden_soap_binding_msg_ref_exts_free;
    binding_msg_ref_exts_impl->binding_msg_ref_exts.ops->super_objs =
        woden_soap_binding_msg_ref_exts_super_objs;
    binding_msg_ref_exts_impl->binding_msg_ref_exts.ops->type =
        woden_soap_binding_msg_ref_exts_type;
    binding_msg_ref_exts_impl->binding_msg_ref_exts.ops->get_base_impl =
        woden_soap_binding_msg_ref_exts_get_base_impl;

    binding_msg_ref_exts_impl->binding_msg_ref_exts.ops->get_soap_modules =
        woden_soap_binding_msg_ref_exts_get_soap_modules;
    binding_msg_ref_exts_impl->binding_msg_ref_exts.ops->get_soap_headers =
        woden_soap_binding_msg_ref_exts_get_soap_headers;

    binding_msg_ref_exts_impl->methods = axis2_hash_make(env);
    if (!binding_msg_ref_exts_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_msg_ref_exts_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_soap_binding_msg_ref_exts_free);
    axis2_hash_set(binding_msg_ref_exts_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_soap_binding_msg_ref_exts_super_objs);
    axis2_hash_set(binding_msg_ref_exts_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_soap_binding_msg_ref_exts_type);

    axis2_hash_set(binding_msg_ref_exts_impl->methods, "get_modules",
            AXIS2_HASH_KEY_STRING,
            woden_soap_binding_msg_ref_exts_get_soap_modules);
    axis2_hash_set(binding_msg_ref_exts_impl->methods, "get_soap_headers",
            AXIS2_HASH_KEY_STRING,
            woden_soap_binding_msg_ref_exts_get_soap_headers);

    return &(binding_msg_ref_exts_impl->binding_msg_ref_exts);
}

AXIS2_EXTERN woden_soap_binding_msg_ref_exts_t * AXIS2_CALL
woden_soap_binding_msg_ref_exts_create(const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_msg_ref_exts_impl = (woden_soap_binding_msg_ref_exts_impl_t *) create(env);

    binding_msg_ref_exts_impl->component_exts = woden_component_exts_create(env);

    binding_msg_ref_exts_impl->super = axis2_hash_make(env);
    if (!binding_msg_ref_exts_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_msg_ref_exts_impl->super, "WODEN_SOAP_BINDING_MSG_REF_EXTS", AXIS2_HASH_KEY_STRING,
            &(binding_msg_ref_exts_impl->binding_msg_ref_exts));
    axis2_hash_set(binding_msg_ref_exts_impl->super, "WODEN_COMPONENT_EXTS", AXIS2_HASH_KEY_STRING,
            binding_msg_ref_exts_impl->component_exts);

    return &(binding_msg_ref_exts_impl->binding_msg_ref_exts);
}

static axis2_status_t
woden_soap_binding_msg_ref_exts_free_ops(
    void *binding_msg_ref_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_exts_impl = INTF_TO_IMPL(binding_msg_ref_exts);

    if (binding_msg_ref_exts_impl->binding_msg_ref_exts.component_exts.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_exts_impl->binding_msg_ref_exts.
                component_exts.ops);
        binding_msg_ref_exts_impl->binding_msg_ref_exts.component_exts.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_soap_binding_msg_ref_exts_free(void *binding_msg_ref_exts,
        const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_exts_impl = INTF_TO_IMPL(binding_msg_ref_exts);

    if (binding_msg_ref_exts_impl->super)
    {
        axis2_hash_free(binding_msg_ref_exts_impl->super, env);
        binding_msg_ref_exts_impl->super = NULL;
    }

    if (binding_msg_ref_exts_impl->methods)
    {
        axis2_hash_free(binding_msg_ref_exts_impl->methods, env);
        binding_msg_ref_exts_impl->methods = NULL;
    }

    if (binding_msg_ref_exts_impl->component_exts)
    {
        WODEN_COMPONENT_EXTS_FREE(binding_msg_ref_exts_impl->component_exts, env);
        binding_msg_ref_exts_impl->component_exts = NULL;
    }

    if (binding_msg_ref_exts_impl->qname)
    {
        AXIS2_QNAME_FREE(binding_msg_ref_exts_impl->qname, env);
        binding_msg_ref_exts_impl->qname = NULL;
    }
    woden_soap_binding_msg_ref_exts_free_ops(binding_msg_ref_exts, env);

    if ((&(binding_msg_ref_exts_impl->binding_msg_ref_exts))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_msg_ref_exts_impl->binding_msg_ref_exts))->ops);
        (&(binding_msg_ref_exts_impl->binding_msg_ref_exts))->ops = NULL;
    }

    if (binding_msg_ref_exts_impl)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_exts_impl);
        binding_msg_ref_exts_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_super_objs(
    void *binding_msg_ref_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_msg_ref_exts_impl = INTF_TO_IMPL(binding_msg_ref_exts);

    return binding_msg_ref_exts_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_soap_binding_msg_ref_exts_type(
    void *binding_msg_ref_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_exts_impl = INTF_TO_IMPL(binding_msg_ref_exts);

    return binding_msg_ref_exts_impl->obj_type;
}

woden_component_exts_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_get_base_impl(
    void *binding_msg_ref_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_msg_ref_exts_impl = INTF_TO_IMPL(binding_msg_ref_exts);

    return binding_msg_ref_exts_impl->component_exts;
}

axis2_status_t AXIS2_CALL
woden_soap_binding_msg_ref_exts_resolve_methods(
    woden_soap_binding_msg_ref_exts_t *binding_msg_ref_exts,
    const axis2_env_t *env,
    woden_soap_binding_msg_ref_exts_t *binding_msg_ref_exts_impl,
    axis2_hash_t *methods)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_msg_ref_exts_impl_l = INTF_TO_IMPL(binding_msg_ref_exts_impl);

    binding_msg_ref_exts->ops->free = axis2_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    binding_msg_ref_exts->ops->super_objs = axis2_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    binding_msg_ref_exts->ops->type = axis2_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    binding_msg_ref_exts->ops->get_soap_modules = axis2_hash_get(methods,
            "get_soap_modules", AXIS2_HASH_KEY_STRING);
    if (!binding_msg_ref_exts->ops->get_soap_modules && binding_msg_ref_exts_impl_l)
        binding_msg_ref_exts->ops->get_soap_modules =
            binding_msg_ref_exts_impl_l->binding_msg_ref_exts.ops->get_soap_modules;

    binding_msg_ref_exts->ops->get_soap_headers = axis2_hash_get(methods,
            "get_soap_headers", AXIS2_HASH_KEY_STRING);
    if (!binding_msg_ref_exts->ops->get_soap_headers && binding_msg_ref_exts_impl_l)
        binding_msg_ref_exts->ops->get_soap_headers =
            binding_msg_ref_exts_impl_l->binding_msg_ref_exts.ops->get_soap_headers;

    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_get_soap_modules(
    void *binding_msg_ref_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;
    axis2_array_list_t *soap_mods = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_msg_ref_exts_impl = INTF_TO_IMPL(binding_msg_ref_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_msg_ref_exts_impl->component_exts, env);
    parent_element = woden_wsdl_element_to_element_extensible(parent_element,
            env);
    binding_msg_ref_exts_impl->qname = axis2_qname_create_from_string(env,
            WODEN_Q_ELEM_SOAP_MODULE);
    soap_mods = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
                parent_element, env, binding_msg_ref_exts_impl->qname);

    return soap_mods;
}

axis2_array_list_t *AXIS2_CALL
woden_soap_binding_msg_ref_exts_get_soap_headers(
    void *binding_msg_ref_exts,
    const axis2_env_t *env)
{
    woden_soap_binding_msg_ref_exts_impl_t *binding_msg_ref_exts_impl = NULL;
    axis2_array_list_t *soap_headers = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_msg_ref_exts_impl = INTF_TO_IMPL(binding_msg_ref_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_msg_ref_exts_impl->component_exts, env);
    parent_element = woden_wsdl_element_to_element_extensible(parent_element,
            env);
    binding_msg_ref_exts_impl->qname = axis2_qname_create_from_string(env,
            WODEN_Q_ELEM_SOAP_HEADER);
    soap_headers = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
                parent_element, env, binding_msg_ref_exts_impl->qname);

    return soap_headers;
}

