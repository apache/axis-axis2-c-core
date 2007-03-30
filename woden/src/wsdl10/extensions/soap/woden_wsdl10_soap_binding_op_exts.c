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

#include <woden_wsdl10_soap_binding_op_exts.h>
#include <woden_component_exts.h>
#include <woden_wsdl_element.h>
#include <woden_string_attr.h>
#include <woden_qname_or_token_any_attr.h>
#include <woden_qname_list_or_token_any_attr.h>
#include <woden_uri_attr.h>
#include "woden_wsdl10_soap_constants.h"

typedef struct woden_wsdl10_soap_binding_op_exts_impl woden_wsdl10_soap_binding_op_exts_impl_t;

/**
 * @brief Soap Binding Operation Extensions Struct Impl
 *   Axis2 Soap Binding Operation Extensions
 */
struct woden_wsdl10_soap_binding_op_exts_impl
{
    woden_wsdl10_soap_binding_op_exts_t binding_op_exts;
    woden_component_exts_t *component_exts;
    axutil_hash_t *methods;
    axutil_hash_t *super;
    woden_obj_types_t obj_type;

    axis2_qname_t *qname;
    axis2_uri_t *action;
};

#define INTF_TO_IMPL(binding_op_exts) ((woden_wsdl10_soap_binding_op_exts_impl_t *) binding_op_exts)

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_free(
    void *binding_op_exts,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_super_objs(
    void *binding_op_exts,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_type(
    void *binding_op_exts,
    const axutil_env_t *env);

woden_component_exts_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_base_impl(
    void *binding_op_exts,
    const axutil_env_t *env);

axis2_uri_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_soap_mep(
    void *binding_op_exts,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_set_soap_action(
    void *binding_op_exts,
    const axutil_env_t *env,
    axis2_uri_t *action);

axis2_uri_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_soap_action(
    void *binding_op_exts,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_soap_modules(
    void *binding_op_exts,
    const axutil_env_t *env);

static woden_wsdl10_soap_binding_op_exts_t *
create(
    const axutil_env_t *env);

static axis2_status_t
woden_wsdl10_soap_binding_op_exts_free_ops(
    void *binding_op_exts,
    const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_wsdl10_soap_binding_op_exts_t * AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_to_component_exts(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_op_exts)
    {
        binding_op_exts_impl = (woden_wsdl10_soap_binding_op_exts_impl_t *)
                create(env);
    }
    else
        binding_op_exts_impl = (woden_wsdl10_soap_binding_op_exts_impl_t *)
                binding_op_exts;
    woden_wsdl10_soap_binding_op_exts_free_ops(binding_op_exts, env);

    binding_op_exts_impl->binding_op_exts.component_exts.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_component_exts_ops_t));
    woden_component_exts_resolve_methods(&(binding_op_exts_impl->binding_op_exts.
            component_exts), env, binding_op_exts_impl->methods);
    return binding_op_exts;
}

/************************End of Woden C Internal Methods***********************/
static woden_wsdl10_soap_binding_op_exts_t *
create(const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_exts_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_soap_binding_op_exts_impl_t));

    binding_op_exts_impl->obj_type = WODEN_WSDL10_SOAP_BINDING_OP_EXTS;
    binding_op_exts_impl->super = NULL;
    binding_op_exts_impl->methods = NULL;

    binding_op_exts_impl->component_exts = NULL;
    binding_op_exts_impl->qname = NULL;
    binding_op_exts_impl->action = NULL;

    binding_op_exts_impl->binding_op_exts.component_exts.ops = NULL;

    binding_op_exts_impl->binding_op_exts.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl10_soap_binding_op_exts_ops_t));

    binding_op_exts_impl->binding_op_exts.ops->free = woden_wsdl10_soap_binding_op_exts_free;
    binding_op_exts_impl->binding_op_exts.ops->super_objs =
        woden_wsdl10_soap_binding_op_exts_super_objs;
    binding_op_exts_impl->binding_op_exts.ops->type =
        woden_wsdl10_soap_binding_op_exts_type;
    binding_op_exts_impl->binding_op_exts.ops->get_base_impl =
        woden_wsdl10_soap_binding_op_exts_get_base_impl;

    binding_op_exts_impl->binding_op_exts.ops->get_soap_modules =
        woden_wsdl10_soap_binding_op_exts_get_soap_modules;
    binding_op_exts_impl->binding_op_exts.ops->get_soap_mep =
        woden_wsdl10_soap_binding_op_exts_get_soap_mep;
    binding_op_exts_impl->binding_op_exts.ops->set_soap_action =
        woden_wsdl10_soap_binding_op_exts_set_soap_action;
    binding_op_exts_impl->binding_op_exts.ops->get_soap_action =
        woden_wsdl10_soap_binding_op_exts_get_soap_action;

    binding_op_exts_impl->methods = axutil_hash_make(env);
    if (!binding_op_exts_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(binding_op_exts_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_binding_op_exts_free);
    axutil_hash_set(binding_op_exts_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_wsdl10_soap_binding_op_exts_super_objs);
    axutil_hash_set(binding_op_exts_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_wsdl10_soap_binding_op_exts_type);

    axutil_hash_set(binding_op_exts_impl->methods, "get_soap_modules",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_binding_op_exts_get_soap_modules);
    axutil_hash_set(binding_op_exts_impl->methods, "get_soap_mep",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_binding_op_exts_get_soap_mep);
    axutil_hash_set(binding_op_exts_impl->methods, "set_soap_action",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_binding_op_exts_set_soap_action);
    axutil_hash_set(binding_op_exts_impl->methods, "get_soap_action",
            AXIS2_HASH_KEY_STRING,
            woden_wsdl10_soap_binding_op_exts_get_soap_action);

    return &(binding_op_exts_impl->binding_op_exts);
}

AXIS2_EXTERN woden_wsdl10_soap_binding_op_exts_t * AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_create(const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_exts_impl = (woden_wsdl10_soap_binding_op_exts_impl_t *) create(env);

    binding_op_exts_impl->component_exts = woden_component_exts_create(env);

    binding_op_exts_impl->super = axutil_hash_make(env);
    if (!binding_op_exts_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(binding_op_exts_impl->super, "WODEN_WSDL10_SOAP_BINDING_OP_EXTS", AXIS2_HASH_KEY_STRING,
            &(binding_op_exts_impl->binding_op_exts));
    axutil_hash_set(binding_op_exts_impl->super, "WODEN_COMPONENT_EXTS", AXIS2_HASH_KEY_STRING,
            binding_op_exts_impl->component_exts);

    return &(binding_op_exts_impl->binding_op_exts);
}

static axis2_status_t
woden_wsdl10_soap_binding_op_exts_free_ops(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    if (binding_op_exts_impl->binding_op_exts.component_exts.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_exts_impl->binding_op_exts.
                component_exts.ops);
        binding_op_exts_impl->binding_op_exts.component_exts.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_free(void *binding_op_exts,
        const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    if (binding_op_exts_impl->super)
    {
        axutil_hash_free(binding_op_exts_impl->super, env);
        binding_op_exts_impl->super = NULL;
    }

    if (binding_op_exts_impl->methods)
    {
        axutil_hash_free(binding_op_exts_impl->methods, env);
        binding_op_exts_impl->methods = NULL;
    }

    if (binding_op_exts_impl->component_exts)
    {
        WODEN_COMPONENT_EXTS_FREE(binding_op_exts_impl->component_exts, env);
        binding_op_exts_impl->component_exts = NULL;
    }

    if (binding_op_exts_impl->action)
    {
        axis2_uri_free(binding_op_exts_impl->action, env);
        binding_op_exts_impl->action = NULL;
    }

    if (binding_op_exts_impl->qname)
    {
        axis2_qname_free(binding_op_exts_impl->qname, env);
        binding_op_exts_impl->qname = NULL;
    }
    woden_wsdl10_soap_binding_op_exts_free_ops(binding_op_exts, env);

    if ((&(binding_op_exts_impl->binding_op_exts))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_op_exts_impl->binding_op_exts))->ops);
        (&(binding_op_exts_impl->binding_op_exts))->ops = NULL;
    }

    if (binding_op_exts_impl)
    {
        AXIS2_FREE(env->allocator, binding_op_exts_impl);
        binding_op_exts_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_super_objs(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    return binding_op_exts_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_type(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    return binding_op_exts_impl->obj_type;
}

woden_component_exts_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_base_impl(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    return binding_op_exts_impl->component_exts;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_resolve_methods(
    woden_wsdl10_soap_binding_op_exts_t *binding_op_exts,
    const axutil_env_t *env,
    woden_wsdl10_soap_binding_op_exts_t *binding_op_exts_impl,
    axutil_hash_t *methods)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_op_exts_impl_l = INTF_TO_IMPL(binding_op_exts_impl);

    binding_op_exts->ops->free = axutil_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    binding_op_exts->ops->super_objs = axutil_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    binding_op_exts->ops->type = axutil_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    binding_op_exts->ops->get_soap_modules = axutil_hash_get(methods,
            "get_soap_modules", AXIS2_HASH_KEY_STRING);
    if (!binding_op_exts->ops->get_soap_modules && binding_op_exts_impl_l)
        binding_op_exts->ops->get_soap_modules =
            binding_op_exts_impl_l->binding_op_exts.ops->get_soap_modules;

    return AXIS2_SUCCESS;
}

axis2_uri_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_soap_mep(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;
    woden_uri_attr_t *mep = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_op_exts_impl->component_exts, env);
    parent_element = woden_wsdl_element_to_attr_extensible(parent_element,
            env);
    binding_op_exts_impl->qname = axis2_qname_create_from_string(env,
            WODEN_WSDL10_Q_ATTR_SOAP_MEP);
    mep = WODEN_ATTR_EXTENSIBLE_GET_EXT_ATTR(
                parent_element, env, binding_op_exts_impl->qname);

    return mep  ? WODEN_URI_ATTR_GET_URI(mep, env) : NULL;
}

axis2_status_t AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_set_soap_action(
    void *binding_op_exts,
    const axutil_env_t *env,
    axis2_uri_t *action)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, action, AXIS2_FAILURE);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    binding_op_exts_impl->action = axis2_uri_clone(action, env);

    return AXIS2_SUCCESS;
}


axis2_uri_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_soap_action(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    return binding_op_exts_impl->action;
}

axutil_array_list_t *AXIS2_CALL
woden_wsdl10_soap_binding_op_exts_get_soap_modules(
    void *binding_op_exts,
    const axutil_env_t *env)
{
    woden_wsdl10_soap_binding_op_exts_impl_t *binding_op_exts_impl = NULL;
    axutil_array_list_t *soap_mods = NULL;
    void *parent_element = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_exts_impl = INTF_TO_IMPL(binding_op_exts);

    parent_element = WODEN_COMPONENT_EXTS_GET_PARENT_ELEMENT(
                binding_op_exts_impl->component_exts, env);
    parent_element = woden_wsdl_element_to_element_extensible(parent_element,
            env);
    binding_op_exts_impl->qname = axis2_qname_create_from_string(env,
            WODEN_WSDL10_Q_ELEM_SOAP_MODULE);
    soap_mods = WODEN_ELEMENT_EXTENSIBLE_GET_EXT_ELEMENTS_OF_TYPE(
                parent_element, env, binding_op_exts_impl->qname);

    return soap_mods;
}

