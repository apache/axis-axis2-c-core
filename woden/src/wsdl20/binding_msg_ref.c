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
#include <woden/wsdl20/axis2_woden_binding_msg_ref.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <xml_schema/axis2_xml_schema_element.h>

typedef struct axis2_woden_binding_msg_ref_impl axis2_woden_binding_msg_ref_impl_t;

/** 
 * @brief Binding Message Reference Struct Impl
 *	Axis2 Binding Message Reference  
 */ 
struct axis2_woden_binding_msg_ref_impl
{
    axis2_woden_binding_msg_ref_t binding_msg_ref;
    axis2_woden_nested_configurable_t *nested_configurable;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    void *f_direction;
    void *f_msg_label;
    void *f_interface_msg_ref;
};

#define INTF_TO_IMPL(binding_msg_ref) ((axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref)

axis2_status_t AXIS2_CALL 
axis2_woden_binding_msg_ref_free(
        void *binding_msg_ref,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_binding_msg_ref_super_objs(
        void *binding_msg_ref,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_binding_msg_ref_type(
        void *binding_msg_ref,
        const axis2_env_t *env);

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_binding_msg_ref_get_base_impl(
        void *binding_msg_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Binding Message Reference  methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
axis2_woden_binding_msg_ref_get_interface_msg_ref(
        void *binding_msg_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_binding_msg_ref_to_element(
        void *binding_msg_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Binding Message Reference Element methods (the XML Element model)
 * ************************************************************/


axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_set_direction(
        void *binding_msg_ref,
        const axis2_env_t *env,
        void *dir);

void *AXIS2_CALL
axis2_woden_binding_msg_ref_get_direction(
        void *binding_msg_ref,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_set_msg_label(
        void *binding_msg_ref,
        const axis2_env_t *env,
        void *msg_label);

void *AXIS2_CALL
axis2_woden_binding_msg_ref_get_msg_label(
        void *binding_msg_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_set_interface_msg_ref_element(
        void *binding_msg_ref,
        const axis2_env_t *env,
        void *int_msg_ref);


static axis2_woden_binding_msg_ref_t *
create(const axis2_env_t *env);

static axis2_status_t
axis2_woden_binding_msg_ref_free_ops(
        void *binding_msg_ref,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_binding_msg_ref_element(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_binding_msg_ref_element_ops_t));
    axis2_woden_binding_msg_ref_element_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            binding_msg_ref_element), env, binding_msg_ref_impl->methods);
    return binding_msg_ref;
}

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_nested_configurable(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_configurable_ops_t));
    axis2_woden_nested_configurable_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            nested_configurable), env, binding_msg_ref_impl->nested_configurable, 
            binding_msg_ref_impl->methods);
    return binding_msg_ref;
}

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_configurable(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_msg_ref_impl->nested_configurable, env);
    axis2_woden_configurable_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            nested_configurable.base.configurable), env, configurable, binding_msg_ref_impl->methods);
    return binding_msg_ref;
}

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_wsdl_obj(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    void *configurable = NULL;
    void *documentable = NULL;
    void *wsdl_obj = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.configurable.
        base.documentable.base.wsdl_obj.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_wsdl_obj_ops_t));
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_msg_ref_impl->nested_configurable, env);
    documentable = AXIS2_WODEN_CONFIGURABLE_GET_BASE_IMPL(
            configurable, env);
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(
            documentable, env);
    axis2_woden_wsdl_obj_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            nested_configurable.base.configurable.base.documentable.base.wsdl_obj), 
            env, wsdl_obj, binding_msg_ref_impl->methods);
    return binding_msg_ref;
}

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_nested_element(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
        nested_element.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_element_ops_t));
    axis2_woden_nested_element_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            nested_configurable.base.nested_element), 
            env, binding_msg_ref_impl->methods);
    return binding_msg_ref;
}

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_wsdl_component(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
        nested_component.wsdl_component.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_wsdl_component_ops_t));
    axis2_woden_wsdl_component_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            nested_configurable.base.nested_component.wsdl_component), 
            env, binding_msg_ref_impl->methods);
    return binding_msg_ref;
}


AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_attr_extensible(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.
        base.documentable_element.wsdl_element.base.attr_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_element_ext_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            binding_msg_ref_element.base.documentable_element.
            wsdl_element.base.attr_extensible), env, binding_msg_ref_impl->methods);
    return binding_msg_ref;
}

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_to_element_extensible(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_msg_ref)
    {
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);
    }
    else
        binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) binding_msg_ref;
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.
        base.documentable_element.wsdl_element.base.element_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_ext_resolve_methods(&(binding_msg_ref_impl->binding_msg_ref.base.
            binding_msg_ref_element.base.documentable_element.
            wsdl_element.base.element_extensible), env, binding_msg_ref_impl->methods);
    return binding_msg_ref;
}


/************************End of Woden C Internal Methods***********************/
static axis2_woden_binding_msg_ref_t *
create(const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_binding_msg_ref_impl_t));

    binding_msg_ref_impl->obj_type= AXIS2_WODEN_BINDING_MSG_REF;
    binding_msg_ref_impl->super = NULL;
    binding_msg_ref_impl->methods = NULL;
    binding_msg_ref_impl->f_direction = NULL;
    binding_msg_ref_impl->f_msg_label = NULL;
    binding_msg_ref_impl->f_interface_msg_ref = NULL;
    
    binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.ops = NULL;
    binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.ops = NULL;
    binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.configurable.ops = 
            NULL;
    
    binding_msg_ref_impl->binding_msg_ref.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_binding_msg_ref_ops_t));

    binding_msg_ref_impl->binding_msg_ref.ops->free = axis2_woden_binding_msg_ref_free;
    binding_msg_ref_impl->binding_msg_ref.ops->super_objs = axis2_woden_binding_msg_ref_super_objs;
    binding_msg_ref_impl->binding_msg_ref.ops->type = axis2_woden_binding_msg_ref_type;
    binding_msg_ref_impl->binding_msg_ref.ops->get_base_impl = axis2_woden_binding_msg_ref_get_base_impl;
    
    binding_msg_ref_impl->binding_msg_ref.ops->get_interface_msg_ref = 
        axis2_woden_binding_msg_ref_get_interface_msg_ref;
    binding_msg_ref_impl->binding_msg_ref.ops->to_element = 
        axis2_woden_binding_msg_ref_to_element;
    binding_msg_ref_impl->binding_msg_ref.ops->set_interface_msg_ref_element = 
        axis2_woden_binding_msg_ref_set_interface_msg_ref_element;
 
    binding_msg_ref_impl->methods = axis2_hash_make(env);
    if(!binding_msg_ref_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_msg_ref_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_free);
    axis2_hash_set(binding_msg_ref_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_binding_msg_ref_super_objs);
    axis2_hash_set(binding_msg_ref_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_binding_msg_ref_type);

    axis2_hash_set(binding_msg_ref_impl->methods, "get_interface_msg_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_get_interface_msg_ref);
    axis2_hash_set(binding_msg_ref_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_to_element);
    axis2_hash_set(binding_msg_ref_impl->methods, "set_direction", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_set_direction);
    axis2_hash_set(binding_msg_ref_impl->methods, "get_direction", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_get_direction);
    axis2_hash_set(binding_msg_ref_impl->methods, "set_msg_label", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_set_msg_label);
    axis2_hash_set(binding_msg_ref_impl->methods, "get_msg_label", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_get_msg_label);
    axis2_hash_set(binding_msg_ref_impl->methods, "set_interface_msg_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_msg_ref_set_interface_msg_ref_element);

    return &(binding_msg_ref_impl->binding_msg_ref);
}

AXIS2_DECLARE(axis2_woden_binding_msg_ref_t *)
axis2_woden_binding_msg_ref_create(const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_impl = (axis2_woden_binding_msg_ref_impl_t *) create(env);

    binding_msg_ref_impl->nested_configurable = axis2_woden_nested_configurable_create(env);

    binding_msg_ref_impl->super = axis2_hash_make(env);
    if(!binding_msg_ref_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_msg_ref_impl->super, "AXIS2_WODEN_BINDING_MSG_REF", 
            AXIS2_HASH_KEY_STRING, 
            &(binding_msg_ref_impl->binding_msg_ref));
    axis2_hash_set(binding_msg_ref_impl->super, "AXIS2_WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            binding_msg_ref_impl->nested_configurable);
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_msg_ref_impl->nested_configurable, env);
    axis2_hash_set(binding_msg_ref_impl->super, "AXIS2_WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(binding_msg_ref_impl->binding_msg_ref);
}

static axis2_status_t
axis2_woden_binding_msg_ref_free_ops(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_impl = INTF_TO_IMPL(binding_msg_ref);

    if(binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.base.
                binding_msg_ref_element.ops);
        binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.ops = NULL;
    }

    if(binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.base.
                nested_configurable.ops);
        binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.ops = 
            NULL;
    }
    
    if(binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.base.
                nested_configurable.base.configurable.ops);
        binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.configurable.ops = 
            NULL;
    }
        
    if(binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
            configurable.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.base.
                nested_configurable.base.configurable.base.documentable.base.wsdl_obj.ops);
        binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
            configurable.base.documentable.base.wsdl_obj.ops = NULL;
    }
    
    if(binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
            nested_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.base.
                nested_configurable.base.nested_component.wsdl_component.ops);
        binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
            nested_component.wsdl_component.ops = NULL;
    }
    
    if(binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.base.
                nested_configurable.base.nested_element.ops);
        binding_msg_ref_impl->binding_msg_ref.base.nested_configurable.base.
            nested_element.ops = NULL;
    }
 
    if(binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.
            base.documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.
                base.binding_msg_ref_element.base.documentable_element.
                wsdl_element.base.attr_extensible.ops );
        binding_msg_ref_impl->binding_msg_ref.
                base.binding_msg_ref_element.base.documentable_element.
                wsdl_element.base.attr_extensible.ops = NULL;
    }
    
    if(binding_msg_ref_impl->binding_msg_ref.base.binding_msg_ref_element.
            base.documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->binding_msg_ref.
                base.binding_msg_ref_element.base.documentable_element.
                wsdl_element.base.element_extensible.ops );
        binding_msg_ref_impl->binding_msg_ref.
                base.binding_msg_ref_element.base.documentable_element.
                wsdl_element.base.element_extensible.ops = NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_free(void *binding_msg_ref,
                        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_impl = INTF_TO_IMPL(binding_msg_ref);

    if(binding_msg_ref_impl->f_direction)
    {
        AXIS2_WODEN_DIRECTION_FREE(binding_msg_ref_impl->f_direction, env);
        binding_msg_ref_impl->f_direction = NULL;
    }

    if(binding_msg_ref_impl->f_msg_label)
    {
        AXIS2_WODEN_MSG_LABEL_FREE(binding_msg_ref_impl->
                f_msg_label, env);
        binding_msg_ref_impl->f_msg_label = NULL;
    }

    if(binding_msg_ref_impl->f_interface_msg_ref)
    {
        AXIS2_INTERFACE_MSG_REF_FREE(binding_msg_ref_impl->f_interface_msg_ref, env);
        binding_msg_ref_impl->f_interface_msg_ref = NULL;
    }

    if(binding_msg_ref_impl->super)
    {
        axis2_hash_free(binding_msg_ref_impl->super, env);
        binding_msg_ref_impl->super = NULL;
    }
    
    if(binding_msg_ref_impl->methods)
    {
        axis2_hash_free(binding_msg_ref_impl->methods, env);
        binding_msg_ref_impl->methods = NULL;
    }

    if(binding_msg_ref_impl->nested_configurable)
    {
        AXIS2_WODEN_NESTED_CONFIGURABLE_FREE(binding_msg_ref_impl->
                nested_configurable, env);
        binding_msg_ref_impl->nested_configurable = NULL;
    }
    axis2_woden_binding_msg_ref_free_ops(binding_msg_ref, env);

    if((&(binding_msg_ref_impl->binding_msg_ref))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_msg_ref_impl->binding_msg_ref))->ops);
        (&(binding_msg_ref_impl->binding_msg_ref))->ops = NULL;
    }
    
    if(binding_msg_ref_impl)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl);
        binding_msg_ref_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_binding_msg_ref_super_objs(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_impl = INTF_TO_IMPL(binding_msg_ref);

    return binding_msg_ref_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_binding_msg_ref_type(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_msg_ref_impl = INTF_TO_IMPL(binding_msg_ref);

    return binding_msg_ref_impl->obj_type;
}

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_binding_msg_ref_get_base_impl(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_msg_ref_impl = INTF_TO_IMPL(binding_msg_ref);

    return binding_msg_ref_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_resolve_methods(
        axis2_woden_binding_msg_ref_t *binding_msg_ref,
        const axis2_env_t *env,
        axis2_woden_binding_msg_ref_t *binding_msg_ref_impl,
        axis2_hash_t *methods)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_msg_ref_impl_l = INTF_TO_IMPL(binding_msg_ref_impl);
    
    binding_msg_ref->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    binding_msg_ref->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    binding_msg_ref->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    binding_msg_ref->ops->get_interface_msg_ref = axis2_hash_get(methods, 
            "get_interface_msg_ref", AXIS2_HASH_KEY_STRING);
    if(!binding_msg_ref->ops->get_interface_msg_ref && binding_msg_ref_impl_l)
            binding_msg_ref->ops->get_interface_msg_ref = 
            binding_msg_ref_impl_l->binding_msg_ref.ops->get_interface_msg_ref;
    
    binding_msg_ref->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!binding_msg_ref->ops->to_element && binding_msg_ref_impl_l)
            binding_msg_ref->ops->to_element = 
            binding_msg_ref_impl_l->binding_msg_ref.ops->to_element;
    
    binding_msg_ref->ops->set_interface_msg_ref_element = axis2_hash_get(methods, 
            "set_interface_msg_ref_element", AXIS2_HASH_KEY_STRING);
    if(!binding_msg_ref->ops->set_interface_msg_ref_element && binding_msg_ref_impl_l)
            binding_msg_ref->ops->set_interface_msg_ref_element = 
            binding_msg_ref_impl_l->binding_msg_ref.ops->set_interface_msg_ref_element;
    
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Binding Message Reference  methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
axis2_woden_binding_msg_ref_get_interface_msg_ref(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env);
    binding_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_MSG_REF", AXIS2_HASH_KEY_STRING));

    return binding_msg_ref_impl->f_interface_msg_ref;
}

void *AXIS2_CALL
axis2_woden_binding_msg_ref_to_element(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env);
    binding_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_MSG_REF", AXIS2_HASH_KEY_STRING));
    
    return &(binding_msg_ref_impl->binding_msg_ref);
}

/* ************************************************************
 *  Binding Message Reference Element methods (the XML Element model)
 * ************************************************************/


axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_set_direction(
        void *binding_msg_ref,
        const axis2_env_t *env,
        void *dir)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dir, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env);
    binding_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_MSG_REF", AXIS2_HASH_KEY_STRING));

    if(!binding_msg_ref_impl->f_direction)
    {
        /* TODO */
    }
    binding_msg_ref_impl->f_direction = dir;


    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_binding_msg_ref_get_direction(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env);
    binding_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_MSG_REF", AXIS2_HASH_KEY_STRING));

    return binding_msg_ref_impl->f_direction;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_set_msg_label(
        void *binding_msg_ref,
        const axis2_env_t *env,
        void *msg_label)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_label, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env);
    binding_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_MSG_REF", AXIS2_HASH_KEY_STRING));

    if(!binding_msg_ref_impl->f_msg_label)
    {
        /* TODO */
    }
    binding_msg_ref_impl->f_msg_label = msg_label;


    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_binding_msg_ref_get_msg_label(
        void *binding_msg_ref,
        const axis2_env_t *env)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env);
    binding_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_MSG_REF", AXIS2_HASH_KEY_STRING));

    return binding_msg_ref_impl->f_msg_label;
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_binding_msg_ref_set_interface_msg_ref_element(
        void *binding_msg_ref,
        const axis2_env_t *env,
        void *int_msg_ref)
{
    axis2_woden_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, int_msg_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_MSG_REF_SUPER_OBJS(binding_msg_ref, env);
    binding_msg_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_MSG_REF", AXIS2_HASH_KEY_STRING));

    if(!binding_msg_ref_impl->f_interface_msg_ref)
    {
        /* TODO */
    }
    binding_msg_ref_impl->f_interface_msg_ref = int_msg_ref;
    
    return AXIS2_SUCCESS;
}

