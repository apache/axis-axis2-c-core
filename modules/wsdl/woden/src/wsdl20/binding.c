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

#include <woden/axis2_woden_wsdl_obj.h>
#include <woden/wsdl20/axis2_woden_binding.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <woden/wsdl20/axis2_woden_binding_fault.h>

typedef struct axis2_woden_binding_impl axis2_woden_binding_impl_t;

/** 
 * @brief Binding Struct Impl
 *	Axis2 Binding  
 */ 
struct axis2_woden_binding_impl
{
    axis2_woden_binding_t binding;
    axis2_woden_configurable_t *configurable;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_array_list_t *f_extended_bindings;
    axis2_qname_t *f_qname;
    axis2_array_list_t *f_extends_qnames;
    axis2_array_list_t *f_style_default;
    axis2_array_list_t *f_binding_fault_elements;
    axis2_array_list_t *f_binding_op_elements;
};

#define INTF_TO_IMPL(binding) ((axis2_woden_binding_impl_t *) binding)

axis2_status_t AXIS2_CALL 
axis2_woden_binding_free(
        void *binding,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_binding_super_objs(
        void *binding,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_binding_type(
        void *binding,
        axis2_env_t **env);

axis2_woden_configurable_t *AXIS2_CALL
axis2_woden_binding_get_base_impl(
        void *binding,
        axis2_env_t **env);

/* ************************************************************
 *  Binding binding methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
axis2_woden_binding_get_qname(
        void *binding,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_extended_bindings(
        void *binding,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_faults(
        void *binding,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_binding_get_binding_fault(
        void *binding,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_ops(
        void *binding,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_binding_to_element(
        void *binding,
        axis2_env_t **env);

/* ************************************************************
 *  BindingElement binding methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_binding_set_qname(
        void *binding,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_style_default_uri(
        void *binding,
        axis2_env_t **env,
        axis2_url_t *uri);

void *AXIS2_CALL
axis2_woden_binding_get_style_default(
        void *binding,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_extended_qname(
        void *binding,
        axis2_env_t **env,
        axis2_woden_wsdl_element_t *qname);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_extends_qnames(
        void *binding,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_binding_fault_element(
        void *binding,
        axis2_env_t **env,
        void *fault);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_fault_elements(
        void *binding,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_binding_op_element(
        void *binding,
        axis2_env_t **env,
        void *op);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_op_elements(
        void *binding,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_extended_binding(
        void *binding,
        axis2_env_t **env,
        void *extended_binding);


static axis2_woden_binding_t *
create(axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_binding_element(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding)
    {
        binding_impl = (axis2_woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (axis2_woden_binding_impl_t *) binding;

    binding_impl->binding.base.binding_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_binding_element_ops_t));
    axis2_woden_binding_element_resolve_methods(&(binding_impl->binding.base.
            binding_element), env, binding_impl->methods);
    return binding;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_to_binding_element_free(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    if(binding_impl->binding.base.binding_element.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                binding_element.ops);
        binding_impl->binding.base.binding_element.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_configurable_element(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding)
    {
        binding_impl = (axis2_woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (axis2_woden_binding_impl_t *) binding;

    binding_impl->binding.base.binding_element.base.configurable_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_configurable_element_ops_t));
    axis2_woden_configurable_element_resolve_methods(&(binding_impl->binding.base.
            binding_element.base.configurable_element), env, binding_impl->methods);
    return binding;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_to_configurable_element_free(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    if(binding_impl->binding.base.binding_element.base.configurable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                binding_element.base.configurable_element.ops);
        binding_impl->binding.base.binding_element.base.configurable_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_documentable_element(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding)
    {
        binding_impl = (axis2_woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (axis2_woden_binding_impl_t *) binding;

    binding_impl->binding.base.binding_element.base.documentable_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_element_ops_t));
    axis2_woden_documentable_element_resolve_methods(&(binding_impl->binding.base.
            binding_element.base.documentable_element), env, 
            binding_impl->methods);
    return binding;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_to_documentable_element_free(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    if(binding_impl->binding.base.binding_element.base.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                binding_element.base.documentable_element.ops);
        binding_impl->binding.base.binding_element.base.documentable_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_to_configurable(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding)
    {
        binding_impl = (axis2_woden_binding_impl_t *) create(env);
    }
    else
        binding_impl = (axis2_woden_binding_impl_t *) binding;

    binding_impl->binding.base.configurable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    axis2_woden_configurable_resolve_methods(&(binding_impl->binding.base.
            configurable), env, binding_impl->configurable, 
            binding_impl->methods);
    return binding;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_to_configurable_free(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    if(binding_impl->binding.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                configurable.ops);
        binding_impl->binding.base.configurable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_binding_t *
create(axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_binding_impl_t));

    binding_impl->obj_type= AXIS2_WODEN_BINDING;
    binding_impl->super = NULL;
    binding_impl->methods = NULL;
    binding_impl->f_qname = NULL;
    binding_impl->f_extended_bindings = NULL;
    binding_impl->f_extends_qnames = NULL;
    binding_impl->f_style_default = NULL;
    binding_impl->f_binding_fault_elements = NULL;
    binding_impl->f_binding_op_elements = NULL;
    
    binding_impl->binding.base.binding_element.ops = NULL;
    binding_impl->binding.base.binding_element.base.documentable_element.ops = NULL;
    binding_impl->binding.base.binding_element.base.configurable_element.ops = NULL;
    binding_impl->binding.base.configurable.ops = NULL;
    
    binding_impl->binding.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_binding_ops_t));

    binding_impl->binding.ops->free = axis2_woden_binding_free;
    binding_impl->binding.ops->super_objs = axis2_woden_binding_super_objs;
    binding_impl->binding.ops->type = axis2_woden_binding_type;
    binding_impl->binding.ops->get_base_impl = axis2_woden_binding_get_base_impl;
    
    binding_impl->binding.ops->get_qname = axis2_woden_binding_get_qname;
    binding_impl->binding.ops->get_extended_bindings = 
        axis2_woden_binding_get_extended_bindings;
    binding_impl->binding.ops->get_binding_faults = 
        axis2_woden_binding_get_binding_faults;
    binding_impl->binding.ops->get_binding_fault = 
        axis2_woden_binding_get_binding_fault;
    binding_impl->binding.ops->get_binding_ops = 
        axis2_woden_binding_get_binding_ops;
    binding_impl->binding.ops->to_element = 
        axis2_woden_binding_to_element;
 
    binding_impl->methods = axis2_hash_make(env);
    if(!binding_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_free);
    axis2_hash_set(binding_impl->methods, "to_binding_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_to_binding_element_free);
    axis2_hash_set(binding_impl->methods, "to_configurable_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_to_configurable_element_free);
    axis2_hash_set(binding_impl->methods, "to_documentable_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_to_documentable_element_free);
    axis2_hash_set(binding_impl->methods, "to_configurable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_to_configurable_free);
    axis2_hash_set(binding_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_binding_super_objs);
    axis2_hash_set(binding_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_binding_type);

    axis2_hash_set(binding_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_qname);
    axis2_hash_set(binding_impl->methods, "get_extended_bindings", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_extended_bindings);
    axis2_hash_set(binding_impl->methods, "get_binding_faults", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_binding_faults);
    axis2_hash_set(binding_impl->methods, "get_binding_fault", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_binding_fault);
    axis2_hash_set(binding_impl->methods, "get_binding_ops", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_binding_ops);
    axis2_hash_set(binding_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_to_element);
    axis2_hash_set(binding_impl->methods, "set_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_set_qname);
    axis2_hash_set(binding_impl->methods, "add_style_default_uri", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_add_style_default_uri);
    axis2_hash_set(binding_impl->methods, "get_style_default", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_style_default);
    axis2_hash_set(binding_impl->methods, "add_extends_qnames", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_extends_qnames);
    axis2_hash_set(binding_impl->methods, "get_extends_qnames", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_extends_qnames);
    axis2_hash_set(binding_impl->methods, "add_binding_fault_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_add_binding_fault_element);
    axis2_hash_set(binding_impl->methods, "get_binding_fault_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_binding_fault_elements);
    axis2_hash_set(binding_impl->methods, "add_binding_op_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_add_binding_op_element);
    axis2_hash_set(binding_impl->methods, "get_binding_op_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_get_binding_op_elements);
    axis2_hash_set(binding_impl->methods, "add_extended_binding", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_add_extended_binding);

    return &(binding_impl->binding);
}

AXIS2_DECLARE(axis2_woden_binding_t *)
axis2_woden_binding_create(axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = (axis2_woden_binding_impl_t *) create(env);

    binding_impl->configurable = axis2_woden_configurable_create(env);

    binding_impl->super = axis2_hash_make(env);
    if(!binding_impl->super) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_impl->super, "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING, 
            &(binding_impl->binding));
    axis2_hash_set(binding_impl->super, "AXIS2_WODEN_CONFIGURABLE", AXIS2_HASH_KEY_STRING, 
            binding_impl->configurable);
 
    return &(binding_impl->binding);
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_free(void *binding,
                        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    if(binding_impl->f_extended_bindings)
    {
        AXIS2_ARRAY_LIST_FREE(binding_impl->f_extended_bindings, env);
        binding_impl->f_extended_bindings = NULL;
    }

    if(binding_impl->f_qname)
    {
        AXIS2_QNAME_FREE(binding_impl->f_qname, env);
        binding_impl->f_qname = NULL;
    }

    if(binding_impl->f_extends_qnames)
    {
        AXIS2_ARRAY_LIST_FREE(binding_impl->f_extends_qnames, env);
        binding_impl->f_extends_qnames = NULL;
    }

    if(binding_impl->f_style_default)
    {
        AXIS2_ARRAY_LIST_FREE(binding_impl->f_style_default, env);
        binding_impl->f_style_default = NULL;
    }

    if(binding_impl->f_binding_fault_elements)
    {
        AXIS2_ARRAY_LIST_FREE(binding_impl->f_binding_fault_elements, env);
        binding_impl->f_binding_fault_elements = NULL;
    }

    if(binding_impl->f_binding_op_elements)
    {
        AXIS2_ARRAY_LIST_FREE(binding_impl->f_binding_op_elements, env);
        binding_impl->f_binding_op_elements = NULL;
    }
   
    if(binding_impl->super)
    {
        axis2_hash_free(binding_impl->super, env);
        binding_impl->super = NULL;
    }
    
    if(binding_impl->methods)
    {
        axis2_hash_free(binding_impl->methods, env);
        binding_impl->methods = NULL;
    }

    if(binding_impl->configurable)
    {
        AXIS2_WODEN_CONFIGURABLE_FREE(binding_impl->configurable, env);
        binding_impl->configurable = NULL;
    }

    if(binding_impl->binding.base.binding_element.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                binding_element.ops);
        binding_impl->binding.base.binding_element.ops = NULL;
    }

    if(binding_impl->binding.base.binding_element.base.configurable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                binding_element.base.configurable_element.ops);
        binding_impl->binding.base.binding_element.base.configurable_element.ops = 
            NULL;
    }
    
    if(binding_impl->binding.base.binding_element.base.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                binding_element.base.documentable_element.ops);
        binding_impl->binding.base.binding_element.base.documentable_element.ops = 
            NULL;
    }
    
    if(binding_impl->binding.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, binding_impl->binding.base.
                configurable.ops);
        binding_impl->binding.base.configurable.ops = 
            NULL;
    }
    
    if((&(binding_impl->binding))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(binding_impl->binding))->ops);
        (&(binding_impl->binding))->ops = NULL;
    }
    
    if(binding_impl)
    {
        AXIS2_FREE((*env)->allocator, binding_impl);
        binding_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_binding_super_objs(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    return binding_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_binding_type(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_impl = INTF_TO_IMPL(binding);

    return binding_impl->obj_type;
}

axis2_woden_configurable_t *AXIS2_CALL
axis2_woden_binding_get_base_impl(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_impl = INTF_TO_IMPL(binding);

    return binding_impl->configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_resolve_methods(
        axis2_woden_binding_t *binding,
        axis2_env_t **env,
        axis2_woden_binding_t *binding_impl,
        axis2_hash_t *methods)
{
    axis2_woden_binding_impl_t *binding_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    binding_impl_l = INTF_TO_IMPL(binding_impl);
    
    binding->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    binding->ops->to_binding_free = axis2_hash_get(methods, "to_binding_free", 
            AXIS2_HASH_KEY_STRING);
    binding->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    binding->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    binding->ops->get_qname = axis2_hash_get(methods, 
            "get_qname", AXIS2_HASH_KEY_STRING);
    if(!binding->ops->get_qname && binding_impl_l)
            binding->ops->get_qname = 
            binding_impl_l->binding.ops->get_qname;
    
    binding->ops->get_extended_bindings = axis2_hash_get(methods, 
            "get_extended_bindings", AXIS2_HASH_KEY_STRING);
    if(!binding->ops->get_extended_bindings && binding_impl_l)
            binding->ops->get_extended_bindings = 
            binding_impl_l->binding.ops->get_extended_bindings;
    
    binding->ops->get_binding_faults = axis2_hash_get(methods, 
            "get_binding_faults", AXIS2_HASH_KEY_STRING);
    if(!binding->ops->get_binding_faults && binding_impl_l)
            binding->ops->get_binding_faults = 
            binding_impl_l->binding.ops->get_binding_faults;

    binding->ops->get_binding_fault = axis2_hash_get(methods, 
            "get_binding_fault", AXIS2_HASH_KEY_STRING);
    if(!binding->ops->get_binding_fault && binding_impl_l)
            binding->ops->get_binding_fault = 
            binding_impl_l->binding.ops->get_binding_fault;

    binding->ops->get_binding_ops = axis2_hash_get(methods, 
            "get_binding_ops", AXIS2_HASH_KEY_STRING);
    if(!binding->ops->get_binding_ops && binding_impl_l)
            binding->ops->get_binding_ops = 
            binding_impl_l->binding.ops->get_binding_ops;

    binding->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!binding->ops->to_element && binding_impl_l)
            binding->ops->to_element = 
            binding_impl_l->binding.ops->to_element;
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Binding binding methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
axis2_woden_binding_get_qname(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));
    
    return binding_impl->f_qname;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_extended_bindings(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_extended_bindings;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_faults(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_binding_fault_elements;
}

void *AXIS2_CALL
axis2_woden_binding_get_binding_fault(
        void *binding,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;
    int i = 0, size = 0;
    void *fault;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    size = AXIS2_ARRAY_LIST_SIZE(binding_impl->f_binding_fault_elements, env);
    for(i = 0; i < size; i++)
    {
        void *flt = NULL;
        axis2_qname_t *qname_l = NULL;
        
        flt = AXIS2_ARRAY_LIST_GET(binding_impl->f_binding_fault_elements, 
                env, i);
        qname_l = (axis2_qname_t *) AXIS2_BINDING_FAULT_GET_QNAME(flt, env);
        if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(qname, env, qname_l))
        {
            fault = flt;
            break;
        }
    }

    return fault;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_ops(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_binding_op_elements;
}

void *AXIS2_CALL
axis2_woden_binding_to_element(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));
    
    return &(binding_impl->binding);
}

/* ************************************************************
 *  BindingElement binding methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_binding_set_qname(
        void *binding,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if(binding_impl->f_qname)
    {
        AXIS2_QNAME_FREE(binding_impl->f_qname, env);
    }
    binding_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_style_default_uri(
        void *binding,
        axis2_env_t **env,
        axis2_url_t *uri)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if(!binding_impl->f_style_default)
    {
        binding_impl->f_style_default = axis2_array_list_create(env, 0);
        if(!binding_impl->f_style_default)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(binding_impl->f_style_default, env, uri);

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_binding_get_style_default(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_style_default;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_extended_qname(
        void *binding,
        axis2_env_t **env,
        axis2_woden_wsdl_element_t *qname)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));
    
    if(!binding_impl->f_extends_qnames)
    {
        binding_impl->f_extends_qnames = axis2_array_list_create(env, 0);
        if(!binding_impl->f_extends_qnames)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(binding_impl->f_extends_qnames, env, qname);

    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_extends_qnames(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_extends_qnames;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_binding_fault_element(
        void *binding,
        axis2_env_t **env,
        void *fault)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));
    
    if(!binding_impl->f_binding_fault_elements)
    {
        binding_impl->f_binding_fault_elements = axis2_array_list_create(env, 0);
        if(!binding_impl->f_binding_fault_elements)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(binding_impl->f_binding_fault_elements, env, fault);

    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_fault_elements(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_binding_fault_elements;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_binding_op_element(
        void *binding,
        axis2_env_t **env,
        void *op)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));
    
    if(!binding_impl->f_binding_op_elements)
    {
        binding_impl->f_binding_op_elements = axis2_array_list_create(env, 0);
        if(!binding_impl->f_binding_op_elements)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(binding_impl->f_binding_op_elements, env, op);

    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_get_binding_op_elements(
        void *binding,
        axis2_env_t **env)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    return binding_impl->f_binding_op_elements;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_add_extended_binding(
        void *binding,
        axis2_env_t **env,
        void *extended_binding)
{
    axis2_woden_binding_impl_t *binding_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, binding, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_SUPER_OBJS(binding, env);
    binding_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING", AXIS2_HASH_KEY_STRING));

    if(!binding_impl->f_extended_bindings)
    {
        binding_impl->f_extended_bindings = axis2_array_list_create(env, 0);
        if(!binding_impl->f_extended_bindings)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(binding_impl->f_extended_bindings, env, extended_binding);
    return AXIS2_SUCCESS;
}





