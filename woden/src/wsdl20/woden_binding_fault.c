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
#include <woden_binding_fault.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>
#include <woden_interface_fault.h>
#include <woden_binding_fault_element.h>

typedef struct woden_binding_fault_impl woden_binding_fault_impl_t;

/**
 * @brief Binding Fault Struct Impl
 *   Axis2 Binding Fault
 */
struct woden_binding_fault_impl
{
    woden_binding_fault_t binding_fault;
    woden_nested_configurable_t *nested_configurable;
    woden_obj_types_t obj_type;
    axutil_hash_t *super;
    axutil_hash_t *methods;
    axutil_qname_t *f_ref;
    void *f_interface_fault;
};

#define INTF_TO_IMPL(binding_fault) ((woden_binding_fault_impl_t *) binding_fault)

axis2_status_t AXIS2_CALL
woden_binding_fault_free(
    void *binding_fault,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
woden_binding_fault_super_objs(
    void *binding_fault,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_binding_fault_type(
    void *binding_fault,
    const axutil_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_binding_fault_get_base_impl(
    void *binding_fault,
    const axutil_env_t *env);
/* ************************************************************
 *  Binding Fault interface methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
woden_binding_fault_get_interface_fault(
    void *binding_fault,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_binding_fault_to_element(
    void *binding_fault,
    const axutil_env_t *env);

/* ************************************************************
 *  Binding Fault Element interface methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_fault_set_ref(
    void *binding_fault,
    const axutil_env_t *env,
    axutil_qname_t *qname);

axutil_qname_t *AXIS2_CALL
woden_binding_fault_get_ref(
    void *binding_fault,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_binding_fault_get_interface_fault_element(
    void *binding_fault,
    const axutil_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_binding_fault_set_interface_fault_element(
    void *binding_fault,
    const axutil_env_t *env,
    void *in_fault);


static woden_binding_fault_t *
create(const axutil_env_t *env);

static axis2_status_t
woden_binding_fault_free_ops(
    void *binding_fault,
    const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_binding_fault_element(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.binding_fault_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_binding_fault_element_ops_t));
    woden_binding_fault_element_resolve_methods(&(binding_fault_impl->binding_fault.base.
            binding_fault_element), env, binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_nested_configurable(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.nested_configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_nested_configurable_ops_t));
    woden_nested_configurable_resolve_methods(&(binding_fault_impl->binding_fault.base.
            nested_configurable), env, binding_fault_impl->nested_configurable,
            binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_configurable(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    void *configurable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.nested_configurable.base.configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                binding_fault_impl->nested_configurable, env);
    woden_configurable_resolve_methods(&(binding_fault_impl->binding_fault.base.
            nested_configurable.base.configurable), env, configurable, binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_wsdl_obj(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    void *configurable = NULL;
    void *documentable = NULL;
    void *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.nested_configurable.base.configurable.
    base.documentable.base.wsdl_obj.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl_obj_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                binding_fault_impl->nested_configurable, env);
    documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(
                configurable, env);
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(
                documentable, env);
    woden_wsdl_obj_resolve_methods(&(binding_fault_impl->binding_fault.base.
            nested_configurable.base.configurable.base.documentable.base.wsdl_obj),
            env, wsdl_obj, binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_nested_element(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.nested_configurable.base.
    nested_element.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(binding_fault_impl->binding_fault.base.
            nested_configurable.base.nested_element),
            env, binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_documentable_element(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;


    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.binding_fault_element.base.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(binding_fault_impl->binding_fault.base.
            binding_fault_element.base.documentable_element), env,
            binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_documentable(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;


    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.nested_configurable.base.
    configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(binding_fault_impl->binding_fault.base.
            nested_configurable.base.configurable.base.documentable), env, NULL,
            binding_fault_impl->methods);
    return binding_fault;
}


AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_wsdl_component(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.nested_configurable.base.
    nested_component.wsdl_component.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl_component_ops_t));
    woden_wsdl_component_resolve_methods(&(binding_fault_impl->binding_fault.base.
            nested_configurable.base.nested_component.wsdl_component),
            env, binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_configurable_element(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;


    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.binding_fault_element.base.configurable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(binding_fault_impl->binding_fault.base.
            binding_fault_element.base.configurable_element), env, binding_fault_impl->methods);
    return binding_fault;
}


AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_attr_extensible(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.binding_fault_element.
    base.documentable_element.wsdl_element.base.attr_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(binding_fault_impl->binding_fault.base.
            binding_fault_element.base.documentable_element.
            wsdl_element.base.attr_extensible), env, NULL, binding_fault_impl->methods);
    return binding_fault;
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_to_element_extensible(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding_fault)
    {
	  binding_fault = (void *) create (env);
	  binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;
    }
    else
        binding_fault_impl = (woden_binding_fault_impl_t *) binding_fault;

    if (binding_fault)
	  woden_binding_fault_free_ops(binding_fault, env);

    binding_fault_impl->binding_fault.base.binding_fault_element.
    base.documentable_element.wsdl_element.base.element_extensible.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(binding_fault_impl->binding_fault.base.
            binding_fault_element.base.documentable_element.
            wsdl_element.base.element_extensible), env, NULL, binding_fault_impl->methods);
    return binding_fault;
}


/************************End of Woden C Internal Methods***********************/
static woden_binding_fault_t *
create(const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_fault_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_binding_fault_impl_t));

    binding_fault_impl->obj_type = WODEN_BINDING_FAULT;
    binding_fault_impl->super = NULL;
    binding_fault_impl->methods = NULL;
    binding_fault_impl->f_ref = NULL;
    binding_fault_impl->f_interface_fault = NULL;

    binding_fault_impl->binding_fault.base.binding_fault_element.ops = NULL;
    binding_fault_impl->binding_fault.base.nested_configurable.ops = NULL;
    binding_fault_impl->binding_fault.base.nested_configurable.base.configurable.ops =
        NULL;
    binding_fault_impl->binding_fault.base.binding_fault_element.base.
    nested_element.ops = NULL;
    binding_fault_impl->binding_fault.base.binding_fault_element.base.
    configurable_element.ops = NULL;
    binding_fault_impl->binding_fault.base.binding_fault_element.base.
    documentable_element.ops = NULL;
    binding_fault_impl->binding_fault.base.nested_configurable.base.configurable.base.
    documentable.ops = NULL;
    binding_fault_impl->binding_fault.base.binding_fault_element.base.
    configurable_element.ops = NULL;
    binding_fault_impl->binding_fault.base.binding_fault_element.base.
    documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    binding_fault_impl->binding_fault.base.binding_fault_element.base.
    documentable_element.wsdl_element.base.element_extensible.ops = NULL;

    binding_fault_impl->binding_fault.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_binding_fault_ops_t));

    binding_fault_impl->binding_fault.ops->free = woden_binding_fault_free;
    binding_fault_impl->binding_fault.ops->super_objs = woden_binding_fault_super_objs;
    binding_fault_impl->binding_fault.ops->type = woden_binding_fault_type;
    binding_fault_impl->binding_fault.ops->get_base_impl = woden_binding_fault_get_base_impl;

    binding_fault_impl->binding_fault.ops->get_interface_fault =
        woden_binding_fault_get_interface_fault;
    binding_fault_impl->binding_fault.ops->to_element =
        woden_binding_fault_to_element;
    binding_fault_impl->binding_fault.ops->set_interface_fault_element =
        woden_binding_fault_set_interface_fault_element;

    binding_fault_impl->methods = axutil_hash_make(env);
    if (!binding_fault_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(binding_fault_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_binding_fault_free);
    axutil_hash_set(binding_fault_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_binding_fault_super_objs);
    axutil_hash_set(binding_fault_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_binding_fault_type);

    axutil_hash_set(binding_fault_impl->methods, "get_interface_fault",
            AXIS2_HASH_KEY_STRING,
            woden_binding_fault_get_interface_fault);
    axutil_hash_set(binding_fault_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_fault_to_element);
    axutil_hash_set(binding_fault_impl->methods, "set_ref",
            AXIS2_HASH_KEY_STRING,
            woden_binding_fault_set_ref);
    axutil_hash_set(binding_fault_impl->methods, "get_ref",
            AXIS2_HASH_KEY_STRING,
            woden_binding_fault_get_ref);
    axutil_hash_set(binding_fault_impl->methods, "get_interface_fault_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_fault_get_interface_fault_element);
    axutil_hash_set(binding_fault_impl->methods, "set_interface_fault_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_fault_set_interface_fault_element);

    return &(binding_fault_impl->binding_fault);
}

AXIS2_EXTERN woden_binding_fault_t * AXIS2_CALL
woden_binding_fault_create(const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    void *configurable = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_fault_impl = (woden_binding_fault_impl_t *) create(env);

    binding_fault_impl->nested_configurable = woden_nested_configurable_create(env);

    binding_fault_impl->super = axutil_hash_make(env);
    if (!binding_fault_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(binding_fault_impl->super, "WODEN_BINDING_FAULT",
            AXIS2_HASH_KEY_STRING,
            &(binding_fault_impl->binding_fault));
    axutil_hash_set(binding_fault_impl->super, "WODEN_NESTED_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING,
            binding_fault_impl->nested_configurable);
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
                binding_fault_impl->nested_configurable, env);
    axutil_hash_set(binding_fault_impl->super, "WODEN_CONFIGURABLE",
            AXIS2_HASH_KEY_STRING, configurable);

    return &(binding_fault_impl->binding_fault);
}

static axis2_status_t
woden_binding_fault_free_ops(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_impl = INTF_TO_IMPL(binding_fault);

    if (binding_fault_impl->binding_fault.base.binding_fault_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                binding_fault_element.ops);
        binding_fault_impl->binding_fault.base.binding_fault_element.ops = NULL;
    }

    if (binding_fault_impl->binding_fault.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                nested_configurable.ops);
        binding_fault_impl->binding_fault.base.nested_configurable.ops =
            NULL;
    }

    if (binding_fault_impl->binding_fault.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                nested_configurable.base.configurable.ops);
        binding_fault_impl->binding_fault.base.nested_configurable.base.configurable.ops =
            NULL;
    }

    if (binding_fault_impl->binding_fault.base.nested_configurable.base.
            configurable.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                nested_configurable.base.configurable.base.documentable.base.wsdl_obj.ops);
        binding_fault_impl->binding_fault.base.nested_configurable.base.
        configurable.base.documentable.base.wsdl_obj.ops = NULL;
    }

    if (binding_fault_impl->binding_fault.base.binding_fault_element.base.
            configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                binding_fault_element.base.configurable_element.ops);
        binding_fault_impl->binding_fault.base.binding_fault_element.base.
        configurable_element.ops = NULL;
    }

    if (binding_fault_impl->binding_fault.base.binding_fault_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                binding_fault_element.base.documentable_element.ops);
        binding_fault_impl->binding_fault.base.binding_fault_element.base.
        documentable_element.ops = NULL;
    }

    if (binding_fault_impl->binding_fault.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                nested_configurable.base.configurable.base.documentable.ops);
        binding_fault_impl->binding_fault.base.nested_configurable.base.
        configurable.base.documentable.ops = NULL;
    }


    if (binding_fault_impl->binding_fault.base.nested_configurable.base.
            nested_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                nested_configurable.base.nested_component.wsdl_component.ops);
        binding_fault_impl->binding_fault.base.nested_configurable.base.
        nested_component.wsdl_component.ops = NULL;
    }

    if (binding_fault_impl->binding_fault.base.nested_configurable.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.base.
                nested_configurable.base.nested_element.ops);
        binding_fault_impl->binding_fault.base.nested_configurable.base.
        nested_element.ops = NULL;
    }

    if (binding_fault_impl->binding_fault.base.binding_fault_element.
            base.documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.
                base.binding_fault_element.base.documentable_element.
                wsdl_element.base.attr_extensible.ops);
        binding_fault_impl->binding_fault.
        base.binding_fault_element.base.documentable_element.
        wsdl_element.base.attr_extensible.ops = NULL;
    }

    if (binding_fault_impl->binding_fault.base.binding_fault_element.
            base.documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl->binding_fault.
                base.binding_fault_element.base.documentable_element.
                wsdl_element.base.element_extensible.ops);
        binding_fault_impl->binding_fault.
        base.binding_fault_element.base.documentable_element.
        wsdl_element.base.element_extensible.ops = NULL;
    }


    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_binding_fault_free(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_impl = INTF_TO_IMPL(binding_fault);

    if (binding_fault_impl->f_ref)
    {
        axutil_qname_free(binding_fault_impl->f_ref, env);
        binding_fault_impl->f_ref = NULL;
    }

    if (binding_fault_impl->f_interface_fault)
    {
        WODEN_INTERFACE_FAULT_FREE(binding_fault_impl->
                f_interface_fault, env);
        binding_fault_impl->f_interface_fault = NULL;
    }

    if (binding_fault_impl->super)
    {
        axutil_hash_free(binding_fault_impl->super, env);
        binding_fault_impl->super = NULL;
    }

    if (binding_fault_impl->methods)
    {
        axutil_hash_free(binding_fault_impl->methods, env);
        binding_fault_impl->methods = NULL;
    }

    if (binding_fault_impl->nested_configurable)
    {
        WODEN_NESTED_CONFIGURABLE_FREE(binding_fault_impl->
                nested_configurable, env);
        binding_fault_impl->nested_configurable = NULL;
    }

    woden_binding_fault_free_ops(binding_fault, env);

    if ((&(binding_fault_impl->binding_fault))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_fault_impl->binding_fault))->ops);
        (&(binding_fault_impl->binding_fault))->ops = NULL;
    }

    if (binding_fault_impl)
    {
        AXIS2_FREE(env->allocator, binding_fault_impl);
        binding_fault_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
woden_binding_fault_super_objs(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_fault_impl = INTF_TO_IMPL(binding_fault);

    return binding_fault_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_binding_fault_type(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_impl = INTF_TO_IMPL(binding_fault);

    return binding_fault_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_binding_fault_get_base_impl(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_fault_impl = INTF_TO_IMPL(binding_fault);

    return binding_fault_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_binding_fault_resolve_methods(
    woden_binding_fault_t *binding_fault,
    const axutil_env_t *env,
    woden_binding_fault_t *binding_fault_impl,
    axutil_hash_t *methods)
{
    woden_binding_fault_impl_t *binding_fault_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_fault_impl_l = INTF_TO_IMPL(binding_fault_impl);

    binding_fault->ops->free = axutil_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    binding_fault->ops->super_objs = axutil_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    binding_fault->ops->type = axutil_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    binding_fault->ops->get_interface_fault = axutil_hash_get(methods,
            "get_interface_fault", AXIS2_HASH_KEY_STRING);
    if (!binding_fault->ops->get_interface_fault && binding_fault_impl_l)
        binding_fault->ops->get_interface_fault =
            binding_fault_impl_l->binding_fault.ops->get_interface_fault;

    binding_fault->ops->to_element = axutil_hash_get(methods,
            "to_element", AXIS2_HASH_KEY_STRING);
    if (!binding_fault->ops->to_element && binding_fault_impl_l)
        binding_fault->ops->to_element =
            binding_fault_impl_l->binding_fault.ops->to_element;

    binding_fault->ops->set_interface_fault_element = axutil_hash_get(methods,
            "set_interface_fault_element", AXIS2_HASH_KEY_STRING);
    if (!binding_fault->ops->set_interface_fault_element && binding_fault_impl_l)
        binding_fault->ops->set_interface_fault_element =
            binding_fault_impl_l->binding_fault.ops->set_interface_fault_element;

    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Binding Fault interface methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
woden_binding_fault_get_interface_fault(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env);
    binding_fault_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING_FAULT", AXIS2_HASH_KEY_STRING));

    return binding_fault_impl->f_interface_fault;
}

void *AXIS2_CALL
woden_binding_fault_to_element(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env);
    binding_fault_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING_FAULT", AXIS2_HASH_KEY_STRING));

    return &(binding_fault_impl->binding_fault);
}

/* ************************************************************
 *  Binding Fault Element interface methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_fault_set_ref(
    void *binding_fault,
    const axutil_env_t *env,
    axutil_qname_t *qname)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env);
    binding_fault_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING_FAULT", AXIS2_HASH_KEY_STRING));

    if (binding_fault_impl->f_ref)
    {
        axutil_qname_free(binding_fault_impl->f_ref, env);
    }
    binding_fault_impl->f_ref = axutil_qname_clone(qname, env);
    return AXIS2_SUCCESS;
}

axutil_qname_t *AXIS2_CALL
woden_binding_fault_get_ref(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env);
    binding_fault_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING_FAULT", AXIS2_HASH_KEY_STRING));

    return binding_fault_impl->f_ref;
}

void *AXIS2_CALL
woden_binding_fault_get_interface_fault_element(
    void *binding_fault,
    const axutil_env_t *env)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env);
    binding_fault_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING_FAULT", AXIS2_HASH_KEY_STRING));

    return binding_fault_impl->f_interface_fault;
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
woden_binding_fault_set_interface_fault_element(
    void *binding_fault,
    const axutil_env_t *env,
    void *in_fault)
{
    woden_binding_fault_impl_t *binding_fault_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_fault, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_SUPER_OBJS(binding_fault, env);
    binding_fault_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING_FAULT", AXIS2_HASH_KEY_STRING));

    if (!binding_fault_impl->f_interface_fault)
    {
        /* TODO */
    }
    binding_fault_impl->f_interface_fault = in_fault;
    ;


    return AXIS2_SUCCESS;
}


