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
#include <woden_binding_fault_ref.h>
#include <woden_interface_fault_ref.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_msg_label.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>
#include <woden_direction.h>

typedef struct woden_binding_fault_ref_impl woden_binding_fault_ref_impl_t;

/** 
 * @brief Binding Fault Reference Struct Impl
 *   Axis2 Binding Fault Reference  
 */ 
struct woden_binding_fault_ref_impl
{
    woden_binding_fault_ref_t binding_fault_ref;
    woden_nested_configurable_t *nested_configurable;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_qname_t *f_ref;
    void *f_direction;
    void *f_msg_label;
    void *f_interface_fault_ref;
};

#define INTF_TO_IMPL(binding_fault_ref) ((woden_binding_fault_ref_impl_t *) binding_fault_ref)

axis2_status_t AXIS2_CALL 
woden_binding_fault_ref_free(
        void *binding_fault_ref,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
woden_binding_fault_ref_super_objs(
        void *binding_fault_ref,
        const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL 
woden_binding_fault_ref_type(
        void *binding_fault_ref,
        const axis2_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_binding_fault_ref_get_base_impl(
        void *binding_fault_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Binding Fault Reference  methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
woden_binding_fault_ref_get_interface_fault_ref(
        void *binding_fault_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_binding_fault_ref_get_direction(
        void *binding_fault_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_binding_fault_ref_to_element(
        void *binding_fault_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Binding Fault Reference Element methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_ref(
        void *binding_fault_ref,
        const axis2_env_t *env,
        axis2_qname_t *fault_qname);

axis2_qname_t *AXIS2_CALL
woden_binding_fault_ref_get_ref(
        void *binding_fault_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_binding_fault_ref_get_interface_fault_ref_element(
        void *binding_fault_ref,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_msg_label(
        void *binding_fault_ref,
        const axis2_env_t *env,
        void *msg_label);

void *AXIS2_CALL
woden_binding_fault_ref_get_msg_label(
        void *binding_fault_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_interface_fault_ref_element(
        void *binding_fault_ref,
        const axis2_env_t *env,
        void *fault_ref);

axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_interface_fault_ref(
        void *binding_fault_ref,
        const axis2_env_t *env,
        void *fault_ref);

static woden_binding_fault_ref_t *
create(const axis2_env_t *env);
 
static axis2_status_t
woden_binding_fault_ref_free_ops(
        void *binding_fault_ref,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_binding_fault_ref_element(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;

    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_binding_fault_ref_element_ops_t));
    woden_binding_fault_ref_element_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            binding_fault_ref_element), env, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_nested_configurable(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_nested_configurable_ops_t));
    woden_nested_configurable_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            nested_configurable), env, binding_fault_ref_impl->nested_configurable, 
            binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_configurable(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_configurable_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_fault_ref_impl->nested_configurable, env);
    woden_configurable_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            nested_configurable.base.configurable), env, configurable, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_wsdl_obj(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    void *configurable = NULL;
    void *documentable = NULL;
    void *wsdl_obj = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.configurable.
        base.documentable.base.wsdl_obj.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_wsdl_obj_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_fault_ref_impl->nested_configurable, env);
    documentable = WODEN_CONFIGURABLE_GET_BASE_IMPL(
            configurable, env);
    wsdl_obj = WODEN_DOCUMENTABLE_GET_BASE_IMPL(
            documentable, env);
    woden_wsdl_obj_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            nested_configurable.base.configurable.base.documentable.base.wsdl_obj), 
            env, wsdl_obj, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_nested_element(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
        nested_element.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            nested_configurable.base.nested_element), 
            env, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_wsdl_component(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
        nested_component.wsdl_component.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_wsdl_component_ops_t));
    woden_wsdl_component_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            nested_configurable.base.nested_component.wsdl_component), 
            env, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_configurable_element(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;

    woden_binding_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.configurable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            binding_fault_ref_element.base.configurable_element), env, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_documentable_element(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;

    woden_binding_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.documentable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            binding_fault_ref_element.base.documentable_element), env, 
            binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_documentable(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;

    woden_binding_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
        configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            nested_configurable.base.configurable.base.documentable), env, NULL,
            binding_fault_ref_impl->methods);
    return binding_fault_ref;
}



AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_attr_extensible(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.
        base.documentable_element.wsdl_element.base.attr_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_attr_extensible_ops_t));
    woden_element_ext_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            binding_fault_ref_element.base.documentable_element.
            wsdl_element.base.attr_extensible), env, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_to_element_extensible(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_fault_ref)
    {
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);
    }
    else
        binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) binding_fault_ref;
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);

    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.
        base.documentable_element.wsdl_element.base.element_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_element_extensible_ops_t));
    woden_element_ext_resolve_methods(&(binding_fault_ref_impl->binding_fault_ref.base.
            binding_fault_ref_element.base.documentable_element.
            wsdl_element.base.element_extensible), env, binding_fault_ref_impl->methods);
    return binding_fault_ref;
}


/************************End of Woden C Internal Methods***********************/
static woden_binding_fault_ref_t *
create(const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_ref_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_binding_fault_ref_impl_t));

    binding_fault_ref_impl->obj_type= WODEN_BINDING_FAULT_REF;
    binding_fault_ref_impl->super = NULL;
    binding_fault_ref_impl->methods = NULL;
    binding_fault_ref_impl->f_ref = NULL;
    binding_fault_ref_impl->f_direction = NULL;
    binding_fault_ref_impl->f_msg_label = NULL;
    binding_fault_ref_impl->f_interface_fault_ref = NULL;
    
    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.ops = NULL;
    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.ops = NULL;
    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.configurable.ops = 
            NULL;
    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.
        nested_element.ops = NULL;
    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.
        configurable_element.ops = NULL;
    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.
        documentable_element.ops = NULL;
    binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.configurable.base.
        documentable.ops = NULL;
    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;


    binding_fault_ref_impl->binding_fault_ref.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_binding_fault_ref_ops_t));

    binding_fault_ref_impl->binding_fault_ref.ops->free = woden_binding_fault_ref_free;
    binding_fault_ref_impl->binding_fault_ref.ops->super_objs = woden_binding_fault_ref_super_objs;
    binding_fault_ref_impl->binding_fault_ref.ops->type = woden_binding_fault_ref_type;
    binding_fault_ref_impl->binding_fault_ref.ops->get_base_impl = woden_binding_fault_ref_get_base_impl;
    
    binding_fault_ref_impl->binding_fault_ref.ops->get_interface_fault_ref = 
        woden_binding_fault_ref_get_interface_fault_ref;
    binding_fault_ref_impl->binding_fault_ref.ops->to_element = 
        woden_binding_fault_ref_to_element;
    binding_fault_ref_impl->binding_fault_ref.ops->set_interface_fault_ref_element = 
        woden_binding_fault_ref_set_interface_fault_ref_element;
    binding_fault_ref_impl->binding_fault_ref.ops->set_interface_fault_ref = 
        woden_binding_fault_ref_set_interface_fault_ref;
 
    binding_fault_ref_impl->methods = axis2_hash_make(env);
    if(!binding_fault_ref_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_fault_ref_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_free);
    axis2_hash_set(binding_fault_ref_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, woden_binding_fault_ref_super_objs);
    axis2_hash_set(binding_fault_ref_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, woden_binding_fault_ref_type);

    axis2_hash_set(binding_fault_ref_impl->methods, "get_interface_fault_ref", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_get_interface_fault_ref);
    axis2_hash_set(binding_fault_ref_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_to_element);
    axis2_hash_set(binding_fault_ref_impl->methods, "set_ref", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_set_ref);
    axis2_hash_set(binding_fault_ref_impl->methods, "get_ref", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_get_ref);
    axis2_hash_set(binding_fault_ref_impl->methods, "get_interface_fault_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_get_interface_fault_ref_element);
    axis2_hash_set(binding_fault_ref_impl->methods, "set_msg_label", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_set_msg_label);
    axis2_hash_set(binding_fault_ref_impl->methods, "get_msg_label", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_get_msg_label);
    axis2_hash_set(binding_fault_ref_impl->methods, "set_interface_fault_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_set_interface_fault_ref_element);
    axis2_hash_set(binding_fault_ref_impl->methods, "set_interface_fault_ref", 
            AXIS2_HASH_KEY_STRING, 
            woden_binding_fault_ref_set_interface_fault_ref);

    return &(binding_fault_ref_impl->binding_fault_ref);
}

AXIS2_EXTERN woden_binding_fault_ref_t * AXIS2_CALL
woden_binding_fault_ref_create(const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_ref_impl = (woden_binding_fault_ref_impl_t *) create(env);

    binding_fault_ref_impl->nested_configurable = woden_nested_configurable_create(env);

    binding_fault_ref_impl->super = axis2_hash_make(env);
    if(!binding_fault_ref_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_fault_ref_impl->super, "WODEN_BINDING_FAULT_REF", 
            AXIS2_HASH_KEY_STRING, 
            &(binding_fault_ref_impl->binding_fault_ref));
    axis2_hash_set(binding_fault_ref_impl->super, "WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            binding_fault_ref_impl->nested_configurable);
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_fault_ref_impl->nested_configurable, env);
    axis2_hash_set(binding_fault_ref_impl->super, "WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(binding_fault_ref_impl->binding_fault_ref);
}
 
static axis2_status_t
woden_binding_fault_ref_free_ops(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_ref_impl = INTF_TO_IMPL(binding_fault_ref);

    if(binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                binding_fault_ref_element.ops);
        binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.ops = NULL;
    }

    if(binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                nested_configurable.ops);
        binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.ops = 
            NULL;
    }
    
    if(binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                nested_configurable.base.configurable.ops);
        binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.configurable.ops = 
            NULL;
    }
     
    if(binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            configurable.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                nested_configurable.base.configurable.base.documentable.base.wsdl_obj.ops);
        binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            configurable.base.documentable.base.wsdl_obj.ops = NULL;
    }
    
    if(binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            nested_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                nested_configurable.base.nested_component.wsdl_component.ops);
        binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            nested_component.wsdl_component.ops = NULL;
    }
    
    if(binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                nested_configurable.base.nested_element.ops);
        binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            nested_element.ops = NULL;
    }
  
    if(binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                binding_fault_ref_element.base.documentable_element.ops);
        binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.base.
            documentable_element.ops = NULL;
    }
     
    if(binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.base.
                nested_configurable.base.configurable.base.documentable.ops);
        binding_fault_ref_impl->binding_fault_ref.base.nested_configurable.base.
            configurable.base.documentable.ops = NULL;
    }
      

   
    if(binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.
            base.documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.
                base.binding_fault_ref_element.base.documentable_element.
                wsdl_element.base.attr_extensible.ops );
        binding_fault_ref_impl->binding_fault_ref.
                base.binding_fault_ref_element.base.documentable_element.
                wsdl_element.base.attr_extensible.ops = NULL;
    }
    
    if(binding_fault_ref_impl->binding_fault_ref.base.binding_fault_ref_element.
            base.documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl->binding_fault_ref.
                base.binding_fault_ref_element.base.documentable_element.
                wsdl_element.base.element_extensible.ops );
        binding_fault_ref_impl->binding_fault_ref.
                base.binding_fault_ref_element.base.documentable_element.
                wsdl_element.base.element_extensible.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_binding_fault_ref_free(void *binding_fault_ref,
                        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_ref_impl = INTF_TO_IMPL(binding_fault_ref);

    if(binding_fault_ref_impl->f_ref)
    {
        AXIS2_QNAME_FREE(binding_fault_ref_impl->f_ref, env);
        binding_fault_ref_impl->f_ref = NULL;
    }

    if(binding_fault_ref_impl->f_direction)
    {
        WODEN_DIRECTION_FREE(binding_fault_ref_impl->f_direction, env);
        binding_fault_ref_impl->f_direction = NULL;
    }

    if(binding_fault_ref_impl->f_msg_label)
    {
        WODEN_MSG_LABEL_FREE(binding_fault_ref_impl->
                f_msg_label, env);
        binding_fault_ref_impl->f_msg_label = NULL;
    }

    if(binding_fault_ref_impl->f_interface_fault_ref)
    {
        WODEN_INTERFACE_FAULT_REF_FREE(binding_fault_ref_impl->
                f_interface_fault_ref, env);
        binding_fault_ref_impl->f_interface_fault_ref = NULL;
    }

    if(binding_fault_ref_impl->super)
    {
        axis2_hash_free(binding_fault_ref_impl->super, env);
        binding_fault_ref_impl->super = NULL;
    }
    
    if(binding_fault_ref_impl->methods)
    {
        axis2_hash_free(binding_fault_ref_impl->methods, env);
        binding_fault_ref_impl->methods = NULL;
    }

    if(binding_fault_ref_impl->nested_configurable)
    {
        WODEN_NESTED_CONFIGURABLE_FREE(binding_fault_ref_impl->
                nested_configurable, env);
        binding_fault_ref_impl->nested_configurable = NULL;
    }
    woden_binding_fault_ref_free_ops(binding_fault_ref, env);
   
    if((&(binding_fault_ref_impl->binding_fault_ref))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_fault_ref_impl->binding_fault_ref))->ops);
        (&(binding_fault_ref_impl->binding_fault_ref))->ops = NULL;
    }
    
    if(binding_fault_ref_impl)
    {
        AXIS2_FREE(env->allocator, binding_fault_ref_impl);
        binding_fault_ref_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_binding_fault_ref_super_objs(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_ref_impl = INTF_TO_IMPL(binding_fault_ref);

    return binding_fault_ref_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_binding_fault_ref_type(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_fault_ref_impl = INTF_TO_IMPL(binding_fault_ref);

    return binding_fault_ref_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_binding_fault_ref_get_base_impl(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_fault_ref_impl = INTF_TO_IMPL(binding_fault_ref);

    return binding_fault_ref_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_binding_fault_ref_resolve_methods(
        woden_binding_fault_ref_t *binding_fault_ref,
        const axis2_env_t *env,
        woden_binding_fault_ref_t *binding_fault_ref_impl,
        axis2_hash_t *methods)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_fault_ref_impl_l = INTF_TO_IMPL(binding_fault_ref_impl);
    
    binding_fault_ref->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    binding_fault_ref->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    binding_fault_ref->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    binding_fault_ref->ops->get_interface_fault_ref = axis2_hash_get(methods, 
            "get_interface_fault_ref", AXIS2_HASH_KEY_STRING);
    if(!binding_fault_ref->ops->get_interface_fault_ref && binding_fault_ref_impl_l)
            binding_fault_ref->ops->get_interface_fault_ref = 
            binding_fault_ref_impl_l->binding_fault_ref.ops->get_interface_fault_ref;
    
    binding_fault_ref->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!binding_fault_ref->ops->to_element && binding_fault_ref_impl_l)
            binding_fault_ref->ops->to_element = 
            binding_fault_ref_impl_l->binding_fault_ref.ops->to_element;
    
    binding_fault_ref->ops->set_interface_fault_ref_element = axis2_hash_get(methods, 
            "set_interface_fault_ref_element", AXIS2_HASH_KEY_STRING);
    if(!binding_fault_ref->ops->set_interface_fault_ref_element && binding_fault_ref_impl_l)
            binding_fault_ref->ops->set_interface_fault_ref_element = 
            binding_fault_ref_impl_l->binding_fault_ref.ops->set_interface_fault_ref_element;
    
    binding_fault_ref->ops->set_interface_fault_ref = axis2_hash_get(methods, 
            "set_interface_fault_ref", AXIS2_HASH_KEY_STRING);
    if(!binding_fault_ref->ops->set_interface_fault_ref && binding_fault_ref_impl_l)
            binding_fault_ref->ops->set_interface_fault_ref = 
            binding_fault_ref_impl_l->binding_fault_ref.ops->set_interface_fault_ref;
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Binding Fault Reference  methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
woden_binding_fault_ref_get_interface_fault_ref(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));
    
    return binding_fault_ref_impl->f_interface_fault_ref;
}

void *AXIS2_CALL
woden_binding_fault_ref_get_direction(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return binding_fault_ref_impl->f_direction;
}
void *AXIS2_CALL
woden_binding_fault_ref_to_element(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));
    
    return &(binding_fault_ref_impl->binding_fault_ref);
}

/* ************************************************************
 *  Binding Fault Reference Element methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_ref(
        void *binding_fault_ref,
        const axis2_env_t *env,
        axis2_qname_t *fault_qname)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_qname, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(binding_fault_ref_impl->f_ref)
    {
        AXIS2_QNAME_FREE(binding_fault_ref_impl->f_ref, env);
    }
    binding_fault_ref_impl->f_ref = AXIS2_QNAME_CLONE(fault_qname, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_binding_fault_ref_get_ref(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return binding_fault_ref_impl->f_ref;
}

void *AXIS2_CALL
woden_binding_fault_ref_get_interface_fault_ref_element(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return NULL;
}

axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_msg_label(
        void *binding_fault_ref,
        const axis2_env_t *env,
        void *msg_label)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_label, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(!binding_fault_ref_impl->f_msg_label)
    {
        /* TODO */
    }
    binding_fault_ref_impl->f_msg_label = msg_label;


    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
woden_binding_fault_ref_get_msg_label(
        void *binding_fault_ref,
        const axis2_env_t *env)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return binding_fault_ref_impl->f_msg_label;
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_interface_fault_ref_element(
        void *binding_fault_ref,
        const axis2_env_t *env,
        void *fault_ref)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_ref, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(!binding_fault_ref_impl->f_interface_fault_ref)
    {
        /* TODO */
    }
    binding_fault_ref_impl->f_interface_fault_ref = fault_ref;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_binding_fault_ref_set_interface_fault_ref(
        void *binding_fault_ref,
        const axis2_env_t *env,
        void *fault_ref)
{
    woden_binding_fault_ref_impl_t *binding_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_ref, AXIS2_FAILURE);
    super = WODEN_BINDING_FAULT_REF_SUPER_OBJS(binding_fault_ref, env);
    binding_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_BINDING_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(!binding_fault_ref_impl->f_interface_fault_ref)
    {
        /* TODO */
    }
    binding_fault_ref_impl->f_interface_fault_ref = fault_ref;
    
    return AXIS2_SUCCESS;
}

