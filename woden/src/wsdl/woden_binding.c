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
#include <woden_binding.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <woden_binding_fault_element.h>
#include <woden_interface.h>
#include <woden_binding_op_element.h>

typedef struct woden_binding_impl woden_binding_impl_t;

/**
 * @brief Binding Struct Impl
 *   Axis2 Binding
 */
struct woden_binding_impl
{
    woden_binding_t binding;
    woden_configurable_t *configurable;
    woden_obj_types_t obj_type;
    axutil_hash_t *super;
    axutil_hash_t *methods;
    axutil_array_list_t *f_extended_bindings;
    axis2_qname_t *f_qname;
    axis2_qname_t *f_interface_qname;
    void *f_interface;
    axis2_uri_t *f_type;
    /*
     * Binding faults and operations may be referred to specifically by the qname represented by
     * their 'ref' attribute. This typically suggests a Map implementation keyed by 'ref' qname. 
     * However if the validation feature is disabled, it is possible that the 'ref' attribute may 
     * be missing, which will cause a null key problem if a Map is used.
     * To avoid this problem, binding faults and operations will be stored in Lists instead. 
     * This will avoid any null key issues, however it will make the implementation of 'ref' based
     * access more complicated. 
     */
    axutil_array_list_t *f_faults;
    axutil_array_list_t *f_ops;
};

#define INTF_TO_IMPL(binding) ((woden_binding_impl_t *) binding)

axis2_status_t AXIS2_CALL
woden_binding_free(
    void *binding,
    const axutil_env_t *env);

axutil_hash_t *AXIS2_CALL
woden_binding_super_objs(
    void *binding,
    const axutil_env_t *env);

woden_obj_types_t AXIS2_CALL
woden_binding_type(
    void *binding,
    const axutil_env_t *env);

woden_configurable_t *AXIS2_CALL
woden_binding_get_base_impl(
    void *binding,
    const axutil_env_t *env);

/* ************************************************************
 *  Binding interface methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
woden_binding_get_qname(
    void *binding,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_binding_get_interface(
    void *binding,
    const axutil_env_t *env);

axis2_uri_t *AXIS2_CALL
woden_binding_get_type(
    void *binding,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_faults(
    void *binding,
    const axutil_env_t *env);

axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_ops(
    void *binding,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_binding_to_element(
    void *binding,
    const axutil_env_t *env);

/* ************************************************************
 *  Binding Element interface methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_set_qname(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_binding_set_interface_qname(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname);

axis2_qname_t *AXIS2_CALL
woden_binding_get_interface_qname(
    void *binding,
    const axutil_env_t *env);

void *AXIS2_CALL
woden_binding_get_interface_element(
    void *binding,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_binding_set_type(
    void *binding,
    const axutil_env_t *env,
    axis2_uri_t *type);

axis2_status_t AXIS2_CALL
woden_binding_add_binding_fault_element(
    void *binding,
    const axutil_env_t *env,
    void *fault);

axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_fault_elements(
    void *binding,
    const axutil_env_t *env);

axis2_status_t AXIS2_CALL
woden_binding_add_binding_op_element(
    void *binding,
    const axutil_env_t *env,
    void *op);

axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_op_elements(
    void *binding,
    const axutil_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
void *AXIS2_CALL
woden_binding_get_binding_fault_element(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname);

void *AXIS2_CALL
woden_binding_get_binding_op_element(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_binding_set_interface_element(
    void *binding,
    const axutil_env_t *env,
    void *interface);

static woden_binding_t *
create(const axutil_env_t *env);

static axis2_status_t
woden_binding_free_ops(
    void *binding,
    const axutil_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_binding_element(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding)
    {
        binding_impl = (woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (woden_binding_impl_t *) binding;
    /*
	woden_binding_free_ops(binding, env);
	*/
    binding_impl->binding.base.binding_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_binding_element_ops_t));
    woden_binding_element_resolve_methods(&(binding_impl->binding.base.
            binding_element), env, binding_impl->methods);
    return binding;
}

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_configurable_element(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding)
    {
        binding_impl = (woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (woden_binding_impl_t *) binding;
    woden_binding_free_ops(binding, env);

    binding_impl->binding.base.binding_element.base.configurable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(binding_impl->binding.base.
            binding_element.base.configurable_element), env, binding_impl->methods);
    return binding;
}

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_documentable_element(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding)
    {
        binding_impl = (woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (woden_binding_impl_t *) binding;
    woden_binding_free_ops(binding, env);

    binding_impl->binding.base.binding_element.base.documentable_element.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(binding_impl->binding.base.
            binding_element.base.documentable_element), env,
            binding_impl->methods);
    return binding;
}

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_documentable(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding)
    {
        binding_impl = (woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (woden_binding_impl_t *) binding;

    woden_binding_free_ops(binding, env);

    binding_impl->binding.base.configurable.base.documentable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(binding_impl->binding.base.
            configurable.base.documentable), env, NULL,
            binding_impl->methods);
    return binding;
}

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_configurable(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding)
    {
        binding_impl = (woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (woden_binding_impl_t *) binding;
    woden_binding_free_ops(binding, env);

    binding_impl->binding.base.configurable.ops =
        AXIS2_MALLOC(env->allocator,
                sizeof(woden_configurable_ops_t));
    woden_configurable_resolve_methods(&(binding_impl->binding.base.
            configurable), env, binding_impl->configurable,
            binding_impl->methods);
    return binding;
}

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_wsdl_obj(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    void *documentable = NULL;
    void *wsdl_obj = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding)
    {
        binding_impl = (woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (woden_binding_impl_t *) binding;
    woden_binding_free_ops(binding, env);

    binding_impl->binding.base.configurable.
    base.documentable.base.wsdl_obj.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl_obj_ops_t));
    documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(
                binding_impl->configurable, env);
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(
                documentable, env);
    woden_wsdl_obj_resolve_methods(&(binding_impl->binding.base.
            configurable.base.documentable.base.wsdl_obj),
            env, wsdl_obj, binding_impl->methods);
    return binding;
}

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_to_wsdl_component(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    if (!binding)
    {
        binding_impl = (woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (woden_binding_impl_t *) binding;
    woden_binding_free_ops(binding, env);

    binding_impl->binding.base.configurable.base.
    configurable_component.wsdl_component.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_wsdl_component_ops_t));
    woden_wsdl_component_resolve_methods(&(binding_impl->binding.base.
            configurable.base.configurable_component.wsdl_component),
            env, binding_impl->methods);
    return binding;
}

/************************End of Woden C Internal Methods***********************/
static woden_binding_t *
create(const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_binding_impl_t));

    binding_impl->obj_type = WODEN_BINDING;
    binding_impl->super = NULL;
    binding_impl->methods = NULL;
    binding_impl->f_qname = NULL;
    binding_impl->f_interface_qname = NULL;
    binding_impl->f_interface = NULL;
    binding_impl->f_type = NULL;
    binding_impl->f_faults = NULL;
    binding_impl->f_ops = NULL;

    binding_impl->binding.base.binding_element.ops = NULL;
    binding_impl->binding.base.binding_element.base.documentable_element.ops = NULL;
    binding_impl->binding.base.configurable.base.documentable.ops = NULL;
    binding_impl->binding.base.binding_element.base.configurable_element.ops = NULL;
    binding_impl->binding.base.configurable.ops = NULL;

    binding_impl->binding.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_binding_ops_t));

    binding_impl->binding.ops->free = woden_binding_free;
    binding_impl->binding.ops->super_objs = woden_binding_super_objs;
    binding_impl->binding.ops->type = woden_binding_type;
    binding_impl->binding.ops->get_base_impl = woden_binding_get_base_impl;

    binding_impl->binding.ops->get_qname = woden_binding_get_qname;
    binding_impl->binding.ops->get_interface = woden_binding_get_interface;
    binding_impl->binding.ops->get_type = woden_binding_get_type;
    binding_impl->binding.ops->get_binding_faults =
        woden_binding_get_binding_faults;
    binding_impl->binding.ops->get_binding_ops =
        woden_binding_get_binding_ops;
    binding_impl->binding.ops->to_element = woden_binding_to_element;
    binding_impl->binding.ops->get_binding_fault_element =
        woden_binding_get_binding_fault_element;
    binding_impl->binding.ops->get_binding_op_element =
        woden_binding_get_binding_op_element;
    binding_impl->binding.ops->set_interface_element =
        woden_binding_set_interface_element;

    binding_impl->methods = axutil_hash_make(env);
    if (!binding_impl->methods)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(binding_impl->methods, "free", AXIS2_HASH_KEY_STRING,
            woden_binding_free);
    axutil_hash_set(binding_impl->methods, "super_objs",
            AXIS2_HASH_KEY_STRING, woden_binding_super_objs);
    axutil_hash_set(binding_impl->methods, "type",
            AXIS2_HASH_KEY_STRING, woden_binding_type);

    axutil_hash_set(binding_impl->methods, "get_qname",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_qname);
    axutil_hash_set(binding_impl->methods, "get_interface",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_interface);
    axutil_hash_set(binding_impl->methods, "get_type",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_type);
    axutil_hash_set(binding_impl->methods, "get_binding_faults",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_binding_faults);
    axutil_hash_set(binding_impl->methods, "get_binding_ops",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_binding_ops);
    axutil_hash_set(binding_impl->methods, "to_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_to_element);
    axutil_hash_set(binding_impl->methods, "set_qname",
            AXIS2_HASH_KEY_STRING,
            woden_binding_set_qname);
    axutil_hash_set(binding_impl->methods, "set_interface_qname",
            AXIS2_HASH_KEY_STRING,
            woden_binding_set_interface_qname);
    axutil_hash_set(binding_impl->methods, "get_interface_qname",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_interface_qname);
    axutil_hash_set(binding_impl->methods, "get_interface_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_interface_element);
    axutil_hash_set(binding_impl->methods, "set_type",
            AXIS2_HASH_KEY_STRING,
            woden_binding_set_type);
    axutil_hash_set(binding_impl->methods, "add_binding_fault_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_add_binding_fault_element);
    axutil_hash_set(binding_impl->methods, "get_binding_fault_elements",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_binding_fault_elements);
    axutil_hash_set(binding_impl->methods, "add_binding_op_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_add_binding_op_element);
    axutil_hash_set(binding_impl->methods, "get_binding_op_elements",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_binding_op_elements);
    axutil_hash_set(binding_impl->methods, "get_binding_fault_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_binding_fault_element);
    axutil_hash_set(binding_impl->methods, "get_binding_op_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_get_binding_op_element);
    axutil_hash_set(binding_impl->methods, "set_interface_element",
            AXIS2_HASH_KEY_STRING,
            woden_binding_set_interface_element);

    return &(binding_impl->binding);
}

AXIS2_EXTERN woden_binding_t * AXIS2_CALL
woden_binding_create(const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_impl = (woden_binding_impl_t *) create(env);

    binding_impl->configurable = woden_configurable_create(env);

    binding_impl->super = axutil_hash_make(env);
    if (!binding_impl->super)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axutil_hash_set(binding_impl->super, "WODEN_BINDING", AXIS2_HASH_KEY_STRING,
            &(binding_impl->binding));
    axutil_hash_set(binding_impl->super, "WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING,
            binding_impl->configurable);

    return &(binding_impl->binding);
}

static axis2_status_t
woden_binding_free_ops(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    if (binding_impl->binding.base.binding_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_impl->binding.base.
                binding_element.ops);
        binding_impl->binding.base.binding_element.ops = NULL;
    }

    if (binding_impl->binding.base.binding_element.base.configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_impl->binding.base.
                binding_element.base.configurable_element.ops);
        binding_impl->binding.base.binding_element.base.configurable_element.ops =
            NULL;
    }

    if (binding_impl->binding.base.binding_element.base.documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_impl->binding.base.
                binding_element.base.documentable_element.ops);
        binding_impl->binding.base.binding_element.base.documentable_element.ops =
            NULL;
    }

    if (binding_impl->binding.base.configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, binding_impl->binding.base.
                configurable.base.documentable.ops);
        binding_impl->binding.base.configurable.base.documentable.ops =
            NULL;
    }

    if (binding_impl->binding.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_impl->binding.base.
                configurable.ops);
        binding_impl->binding.base.configurable.ops =
            NULL;
    }

    if (binding_impl->binding.base.
            configurable.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, binding_impl->binding.base.
                configurable.base.documentable.base.wsdl_obj.ops);
        binding_impl->binding.base.configurable.base.documentable.base.wsdl_obj.ops = NULL;
    }

    if (binding_impl->binding.base.configurable.base.
            configurable_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, binding_impl->binding.base.
                configurable.base.configurable_component.wsdl_component.ops);
        binding_impl->binding.base.configurable.base.
        configurable_component.wsdl_component.ops = NULL;
    }

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_binding_free(void *binding,
        const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    if (binding_impl->f_qname)
    {
        axis2_qname_free(binding_impl->f_qname, env);
        binding_impl->f_qname = NULL;
    }

    if (binding_impl->f_interface_qname)
    {
        axis2_qname_free(binding_impl->f_interface_qname, env);
        binding_impl->f_interface_qname = NULL;
    }

    if (binding_impl->f_interface)
    {
        WODEN_INTERFACE_FREE(binding_impl->f_interface, env);
        binding_impl->f_interface = NULL;
    }

    if (binding_impl->f_type)
    {
        axis2_uri_free(binding_impl->f_type, env);
        binding_impl->f_type = NULL;
    }

    if (binding_impl->f_faults)
    {
        axutil_array_list_free(binding_impl->f_faults, env);
        binding_impl->f_faults = NULL;
    }

    if (binding_impl->f_ops)
    {
        axutil_array_list_free(binding_impl->f_ops, env);
        binding_impl->f_ops = NULL;
    }

    if (binding_impl->super)
    {
        axutil_hash_free(binding_impl->super, env);
        binding_impl->super = NULL;
    }

    if (binding_impl->methods)
    {
        axutil_hash_free(binding_impl->methods, env);
        binding_impl->methods = NULL;
    }

    if (binding_impl->configurable)
    {
        WODEN_CONFIGURABLE_FREE(binding_impl->configurable, env);
        binding_impl->configurable = NULL;
    }
    woden_binding_free_ops(binding, env);

    if ((&(binding_impl->binding))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_impl->binding))->ops);
        (&(binding_impl->binding))->ops = NULL;
    }

    if (binding_impl)
    {
        AXIS2_FREE(env->allocator, binding_impl);
        binding_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axutil_hash_t *AXIS2_CALL
woden_binding_super_objs(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_impl = INTF_TO_IMPL(binding);

    return binding_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_binding_type(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    return binding_impl->obj_type;
}

woden_configurable_t *AXIS2_CALL
woden_binding_get_base_impl(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_impl = INTF_TO_IMPL(binding);

    return binding_impl->configurable;
}

axis2_status_t AXIS2_CALL
woden_binding_resolve_methods(
    woden_binding_t *binding,
    const axutil_env_t *env,
    woden_binding_t *binding_impl,
    axutil_hash_t *methods)
{
    woden_binding_impl_t *binding_impl_l = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_impl_l = INTF_TO_IMPL(binding_impl);

    binding->ops->free = axutil_hash_get(methods, "free",
            AXIS2_HASH_KEY_STRING);
    binding->ops->super_objs = axutil_hash_get(methods, "super_objs",
            AXIS2_HASH_KEY_STRING);
    binding->ops->type = axutil_hash_get(methods, "type",
            AXIS2_HASH_KEY_STRING);

    binding->ops->get_qname = axutil_hash_get(methods,
            "get_qname", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->get_qname && binding_impl_l)
        binding->ops->get_qname =
            binding_impl_l->binding.ops->get_qname;

    binding->ops->get_interface = axutil_hash_get(methods,
            "get_interface", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->get_interface && binding_impl_l)
        binding->ops->get_interface =
            binding_impl_l->binding.ops->get_interface;

    binding->ops->get_type = axutil_hash_get(methods,
            "get_type", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->get_type && binding_impl_l)
        binding->ops->get_type =
            binding_impl_l->binding.ops->get_type;

    binding->ops->get_binding_faults = axutil_hash_get(methods,
            "get_binding_faults", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->get_binding_faults && binding_impl_l)
        binding->ops->get_binding_faults =
            binding_impl_l->binding.ops->get_binding_faults;

    binding->ops->get_binding_ops = axutil_hash_get(methods,
            "get_binding_ops", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->get_binding_ops && binding_impl_l)
        binding->ops->get_binding_ops =
            binding_impl_l->binding.ops->get_binding_ops;

    binding->ops->to_element = axutil_hash_get(methods,
            "to_element", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->to_element && binding_impl_l)
        binding->ops->to_element =
            binding_impl_l->binding.ops->to_element;

    binding->ops->get_binding_fault_element = axutil_hash_get(methods,
            "get_binding_fault_element", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->get_binding_fault_element && binding_impl_l)
        binding->ops->get_binding_fault_element =
            binding_impl_l->binding.ops->get_binding_fault_element;

    binding->ops->get_binding_op_element = axutil_hash_get(methods,
            "get_binding_op_element", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->get_binding_op_element && binding_impl_l)
        binding->ops->get_binding_op_element =
            binding_impl_l->binding.ops->get_binding_op_element;

    binding->ops->set_interface_element = axutil_hash_get(methods,
            "set_interface_element", AXIS2_HASH_KEY_STRING);
    if (!binding->ops->set_interface_element && binding_impl_l)
        binding->ops->set_interface_element =
            binding_impl_l->binding.ops->set_interface_element;


    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Binding interface methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
woden_binding_get_qname(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_qname;
}

void *AXIS2_CALL
woden_binding_get_interface(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_interface;
}

axis2_uri_t *AXIS2_CALL
woden_binding_get_type(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_type;
}


axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_faults(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_faults;
}

axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_ops(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_ops;
}

void *AXIS2_CALL
woden_binding_to_element(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return &(binding_impl->binding);
}

/* ************************************************************
 *  Binding Element interface methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_set_qname(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if (binding_impl->f_qname)
    {
        axis2_qname_free(binding_impl->f_qname, env);
    }
    binding_impl->f_qname = axis2_qname_clone(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_binding_set_interface_qname(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if (binding_impl->f_interface_qname)
    {
        axis2_qname_free(binding_impl->f_interface_qname, env);
    }
    binding_impl->f_interface_qname = axis2_qname_clone(qname, env);

    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_binding_get_interface_qname(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_interface_qname;
}

void *AXIS2_CALL
woden_binding_get_interface_element(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_interface;
}


axis2_status_t AXIS2_CALL
woden_binding_set_type(
    void *binding,
    const axutil_env_t *env,
    axis2_uri_t *type)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, type, AXIS2_FAILURE);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if (binding_impl->f_type)
    {
        axis2_uri_free(binding_impl->f_type, env);
        binding_impl->f_type = NULL;
    }
    binding_impl->f_type = axis2_uri_clone(type, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_binding_add_binding_fault_element(
    void *binding,
    const axutil_env_t *env,
    void *fault)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault, AXIS2_FAILURE);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if (!binding_impl->f_faults)
    {
        binding_impl->f_faults = axutil_array_list_create(env, 0);
        if (!binding_impl->f_faults)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axutil_array_list_add(binding_impl->f_faults, env, fault);

    return AXIS2_SUCCESS;
}

axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_fault_elements(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_faults;
}

axis2_status_t AXIS2_CALL
woden_binding_add_binding_op_element(
    void *binding,
    const axutil_env_t *env,
    void *op)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if (!binding_impl->f_ops)
    {
        binding_impl->f_ops = axutil_array_list_create(env, 0);
        if (!binding_impl->f_ops)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    axutil_array_list_add(binding_impl->f_ops, env, op);

    return AXIS2_SUCCESS;
}

axutil_array_list_t *AXIS2_CALL
woden_binding_get_binding_op_elements(
    void *binding,
    const axutil_env_t *env)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_ops;
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
void *AXIS2_CALL
woden_binding_get_binding_fault_element(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;
    int i = 0, size = 0;
    void *fault = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    size = axutil_array_list_size(binding_impl->f_faults, env);
    for (i = 0; i < size; i++)
    {
        void *bind_fault = NULL;
        axis2_qname_t *qname_l = NULL;

        bind_fault = axutil_array_list_get(binding_impl->f_faults,
                env, i);
        qname_l = (axis2_qname_t *) WODEN_BINDING_FAULT_ELEMENT_GET_REF(
                    bind_fault, env);
        if (AXIS2_TRUE == axis2_qname_equals(qname, env, qname_l))
        {
            fault = bind_fault;
            break;
        }
    }

    return fault;
}

void *AXIS2_CALL
woden_binding_get_binding_op_element(
    void *binding,
    const axutil_env_t *env,
    axis2_qname_t *qname)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;
    int i = 0, size = 0;
    void *op = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    size = axutil_array_list_size(binding_impl->f_ops, env);
    for (i = 0; i < size; i++)
    {
        void *bind_op = NULL;
        axis2_qname_t *qname_l = NULL;

        bind_op = axutil_array_list_get(binding_impl->f_ops,
                env, i);
        qname_l = (axis2_qname_t *) WODEN_BINDING_OP_ELEMENT_GET_REF(
                    bind_op, env);
        if (AXIS2_TRUE == axis2_qname_equals(qname, env, qname_l))
        {
            op = bind_op;
            break;
        }
    }

    return op;
}

axis2_status_t AXIS2_CALL
woden_binding_set_interface_element(
    void *binding,
    const axutil_env_t *env,
    void *interface)
{
    woden_binding_impl_t *binding_impl = NULL;
    axutil_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, interface, AXIS2_FAILURE);
    super = WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axutil_hash_get(super,
            "WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if (binding_impl->f_interface)
    {
        /* TODO */
    }
    binding_impl->f_interface = interface;

    return AXIS2_SUCCESS;
}

