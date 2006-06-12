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
#include <woden/wsdl20/axis2_woden_binding_op.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <xml_schema/axis2_xml_schema_element.h>

typedef struct axis2_woden_binding_op_impl axis2_woden_binding_op_impl_t;

/** 
 * @brief Binding Operation Struct Impl
 *   Axis2 Binding Operation  
 */ 
struct axis2_woden_binding_op_impl
{
    axis2_woden_binding_op_t binding_op;
    axis2_woden_nested_configurable_t *nested_configurable;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_qname_t *f_ref;
    axis2_array_list_t *f_msg_refs;
    axis2_array_list_t *f_fault_refs;
    void *f_interface_op;
};

#define INTF_TO_IMPL(binding_op) ((axis2_woden_binding_op_impl_t *) binding_op)

axis2_status_t AXIS2_CALL 
axis2_woden_binding_op_free(
        void *binding_op,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_binding_op_super_objs(
        void *binding_op,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_binding_op_type(
        void *binding_op,
        const axis2_env_t *env);

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_binding_op_get_base_impl(
        void *binding_op,
        const axis2_env_t *env);
/* ************************************************************
 *  Binding Operation  methods (the WSDL Component model)
 * ************************************************************/

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_msg_refs(
        void *binding_op,
        const axis2_env_t *env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_fault_refs(
        void *binding_op,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_binding_op_to_element(
        void *binding_op,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_binding_op_get_interface_op(
        void *binding_op,
        const axis2_env_t *env);
/* ************************************************************
 *  Binding Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_set_ref(
        void *binding_op,
        const axis2_env_t *env,
        axis2_qname_t *qname);

axis2_qname_t *AXIS2_CALL
axis2_woden_binding_op_get_ref(
        void *binding_op,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_binding_op_get_interface_op_element(
        void *binding_op,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_add_binding_msg_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *msg_ref);

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_remove_binding_msg_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *msg_ref);


axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_msg_ref_elements(
        void *binding_op,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_add_binding_fault_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *fault_ref);

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_remove_binding_fault_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *fault_ref);

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_fault_ref_elements(
        void *binding_op,
        const axis2_env_t *env);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_set_interface_op_element(
        void *binding_op,
        const axis2_env_t *env,
        void *int_op);

static axis2_woden_binding_op_t *
create(const axis2_env_t *env);

static axis2_status_t
axis2_woden_binding_op_free_ops(
        void *binding_op,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_binding_op_element(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.binding_op_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_binding_op_element_ops_t));
    axis2_woden_binding_op_element_resolve_methods(&(binding_op_impl->binding_op.base.
            binding_op_element), env, binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_nested_configurable(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.nested_configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_configurable_ops_t));
    axis2_woden_nested_configurable_resolve_methods(&(binding_op_impl->binding_op.base.
            nested_configurable), env, binding_op_impl->nested_configurable, 
            binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_configurable(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_op_impl->nested_configurable, env);
    axis2_woden_configurable_resolve_methods(&(binding_op_impl->binding_op.base.
            nested_configurable.base.configurable), env, configurable, binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_wsdl_obj(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    void *configurable = NULL;
    void *documentable = NULL;
    void *wsdl_obj = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.nested_configurable.base.configurable.
        base.documentable.base.wsdl_obj.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_wsdl_obj_ops_t));
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_op_impl->nested_configurable, env);
    documentable = AXIS2_WODEN_CONFIGURABLE_GET_BASE_IMPL(
            configurable, env);
    wsdl_obj = AXIS2_WODEN_DOCUMENTABLE_GET_BASE_IMPL(
            documentable, env);
    axis2_woden_wsdl_obj_resolve_methods(&(binding_op_impl->binding_op.base.
            nested_configurable.base.configurable.base.documentable.base.wsdl_obj), 
            env, wsdl_obj, binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_nested_element(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.nested_configurable.base.
        nested_element.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_element_ops_t));
    axis2_woden_nested_element_resolve_methods(&(binding_op_impl->binding_op.base.
            nested_configurable.base.nested_element), 
            env, binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_wsdl_component(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.nested_configurable.base.
        nested_component.wsdl_component.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_wsdl_component_ops_t));
    axis2_woden_wsdl_component_resolve_methods(&(binding_op_impl->binding_op.base.
            nested_configurable.base.nested_component.wsdl_component), 
            env, binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_configurable_element(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;

    axis2_woden_binding_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.binding_op_element.base.configurable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_configurable_element_ops_t));
    axis2_woden_configurable_element_resolve_methods(&(binding_op_impl->binding_op.base.
            binding_op_element.base.configurable_element), env, binding_op_impl->methods);
    return binding_op;
}


AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_documentable_element(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;

    axis2_woden_binding_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.binding_op_element.base.documentable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_documentable_element_ops_t));
    axis2_woden_documentable_element_resolve_methods(&(binding_op_impl->binding_op.base.
            binding_op_element.base.documentable_element), env, 
            binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_documentable(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;

    axis2_woden_binding_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.nested_configurable.base.
        configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_documentable_ops_t));
    axis2_woden_documentable_resolve_methods(&(binding_op_impl->binding_op.base.
            nested_configurable.base.configurable.base.documentable), env, NULL,
            binding_op_impl->methods);
    return binding_op;
}



AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_attr_extensible(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.binding_op_element.
        base.documentable_element.wsdl_element.base.attr_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_attr_extensible_ops_t));
    axis2_woden_element_ext_resolve_methods(&(binding_op_impl->binding_op.base.
            binding_op_element.base.documentable_element.
            wsdl_element.base.attr_extensible), env, binding_op_impl->methods);
    return binding_op;
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_to_element_extensible(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!binding_op)
    {
        binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);
    }
    else
        binding_op_impl = (axis2_woden_binding_op_impl_t *) binding_op;
    axis2_woden_binding_op_free_ops(binding_op, env);

    binding_op_impl->binding_op.base.binding_op_element.
        base.documentable_element.wsdl_element.base.element_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_element_extensible_ops_t));
    axis2_woden_element_ext_resolve_methods(&(binding_op_impl->binding_op.base.
            binding_op_element.base.documentable_element.
            wsdl_element.base.element_extensible), env, binding_op_impl->methods);
    return binding_op;
}


/************************End of Woden C Internal Methods***********************/
static axis2_woden_binding_op_t *
create(const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_binding_op_impl_t));

    binding_op_impl->obj_type= AXIS2_WODEN_BINDING_OP;
    binding_op_impl->super = NULL;
    binding_op_impl->methods = NULL;
    binding_op_impl->f_ref = NULL;
    binding_op_impl->f_msg_refs = NULL;
    binding_op_impl->f_fault_refs = NULL;
    binding_op_impl->f_interface_op = NULL;
    
    binding_op_impl->binding_op.base.binding_op_element.ops = NULL;
    binding_op_impl->binding_op.base.nested_configurable.ops = NULL;
    binding_op_impl->binding_op.base.nested_configurable.base.configurable.ops = 
            NULL;
    binding_op_impl->binding_op.base.binding_op_element.base.
        nested_element.ops = NULL;
    binding_op_impl->binding_op.base.binding_op_element.base.
        configurable_element.ops = NULL;
    binding_op_impl->binding_op.base.binding_op_element.base.
        documentable_element.ops = NULL;
    binding_op_impl->binding_op.base.nested_configurable.base.configurable.base.
        documentable.ops = NULL;
    binding_op_impl->binding_op.base.binding_op_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    binding_op_impl->binding_op.base.binding_op_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    
    binding_op_impl->binding_op.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_binding_op_ops_t));

    binding_op_impl->binding_op.ops->free = axis2_woden_binding_op_free;
    binding_op_impl->binding_op.ops->super_objs = axis2_woden_binding_op_super_objs;
    binding_op_impl->binding_op.ops->type = axis2_woden_binding_op_type;
    binding_op_impl->binding_op.ops->get_base_impl = axis2_woden_binding_op_get_base_impl;
    
    binding_op_impl->binding_op.ops->get_binding_msg_refs = 
        axis2_woden_binding_op_get_binding_msg_refs;
    binding_op_impl->binding_op.ops->get_binding_fault_refs = 
        axis2_woden_binding_op_get_binding_fault_refs;
    binding_op_impl->binding_op.ops->to_element = 
        axis2_woden_binding_op_to_element;
    binding_op_impl->binding_op.ops->set_interface_op_element = 
        axis2_woden_binding_op_set_interface_op_element;
 
    binding_op_impl->methods = axis2_hash_make(env);
    if(!binding_op_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_op_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_free);
    axis2_hash_set(binding_op_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_binding_op_super_objs);
    axis2_hash_set(binding_op_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_binding_op_type);

    axis2_hash_set(binding_op_impl->methods, "get_interface_op", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_get_interface_op);
    axis2_hash_set(binding_op_impl->methods, "get_binding_msg_refs", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_get_binding_msg_refs);
    axis2_hash_set(binding_op_impl->methods, "get_binding_fault_refs", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_get_binding_fault_refs);
    axis2_hash_set(binding_op_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_to_element);
    axis2_hash_set(binding_op_impl->methods, "set_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_set_ref);
    axis2_hash_set(binding_op_impl->methods, "get_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_get_ref);
    axis2_hash_set(binding_op_impl->methods, "get_interface_op_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_get_interface_op_element);
    axis2_hash_set(binding_op_impl->methods, "add_binding_msg_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_add_binding_msg_ref_element);
    axis2_hash_set(binding_op_impl->methods, "remove_binding_msg_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_remove_binding_msg_ref_element);
    axis2_hash_set(binding_op_impl->methods, "get_binding_msg_ref_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_get_binding_msg_ref_elements);
    axis2_hash_set(binding_op_impl->methods, "add_binding_fault_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_add_binding_fault_ref_element);
    axis2_hash_set(binding_op_impl->methods, "remove_binding_fault_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_remove_binding_fault_ref_element);
    axis2_hash_set(binding_op_impl->methods, "get_binding_fault_ref_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_get_binding_fault_ref_elements);
    axis2_hash_set(binding_op_impl->methods, "set_interface_op_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_binding_op_set_interface_op_element);

    return &(binding_op_impl->binding_op);
}

AXIS2_EXTERN axis2_woden_binding_op_t * AXIS2_CALL
axis2_woden_binding_op_create(const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_impl = (axis2_woden_binding_op_impl_t *) create(env);

    binding_op_impl->nested_configurable = axis2_woden_nested_configurable_create(env);

    binding_op_impl->super = axis2_hash_make(env);
    if(!binding_op_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(binding_op_impl->super, "AXIS2_WODEN_BINDING_OP", 
            AXIS2_HASH_KEY_STRING, 
            &(binding_op_impl->binding_op));
    axis2_hash_set(binding_op_impl->super, "AXIS2_WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            binding_op_impl->nested_configurable);
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            binding_op_impl->nested_configurable, env);
    axis2_hash_set(binding_op_impl->super, "AXIS2_WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(binding_op_impl->binding_op);
}

static axis2_status_t
axis2_woden_binding_op_free_ops(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_impl = INTF_TO_IMPL(binding_op);

    if(binding_op_impl->binding_op.base.binding_op_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                binding_op_element.ops);
        binding_op_impl->binding_op.base.binding_op_element.ops = NULL;
    }

    if(binding_op_impl->binding_op.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                nested_configurable.ops);
        binding_op_impl->binding_op.base.nested_configurable.ops = 
            NULL;
    }
    
    if(binding_op_impl->binding_op.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                nested_configurable.base.configurable.ops);
        binding_op_impl->binding_op.base.nested_configurable.base.configurable.ops = 
            NULL;
    }
      
    if(binding_op_impl->binding_op.base.nested_configurable.base.
            configurable.base.documentable.base.wsdl_obj.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                nested_configurable.base.configurable.base.documentable.base.wsdl_obj.ops);
        binding_op_impl->binding_op.base.nested_configurable.base.
            configurable.base.documentable.base.wsdl_obj.ops = NULL;
    }
    
    if(binding_op_impl->binding_op.base.nested_configurable.base.
            nested_component.wsdl_component.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                nested_configurable.base.nested_component.wsdl_component.ops);
        binding_op_impl->binding_op.base.nested_configurable.base.
            nested_component.wsdl_component.ops = NULL;
    }
    
    if(binding_op_impl->binding_op.base.nested_configurable.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                nested_configurable.base.nested_element.ops);
        binding_op_impl->binding_op.base.nested_configurable.base.
            nested_element.ops = NULL;
    }
  
    if(binding_op_impl->binding_op.base.binding_op_element.base.
            configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                binding_op_element.base.configurable_element.ops);
        binding_op_impl->binding_op.base.binding_op_element.base.
            configurable_element.ops = NULL;
    }
    

    if(binding_op_impl->binding_op.base.binding_op_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                binding_op_element.base.documentable_element.ops);
        binding_op_impl->binding_op.base.binding_op_element.base.
            documentable_element.ops = NULL;
    }
     
    if(binding_op_impl->binding_op.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.base.
                nested_configurable.base.configurable.base.documentable.ops);
        binding_op_impl->binding_op.base.nested_configurable.base.
            configurable.base.documentable.ops = NULL;
    }
    if(binding_op_impl->binding_op.base.binding_op_element.
            base.documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.
                base.binding_op_element.base.documentable_element.
                wsdl_element.base.attr_extensible.ops );
        binding_op_impl->binding_op.
                base.binding_op_element.base.documentable_element.
                wsdl_element.base.attr_extensible.ops = NULL;
    }
    
    if(binding_op_impl->binding_op.base.binding_op_element.
            base.documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, binding_op_impl->binding_op.
                base.binding_op_element.base.documentable_element.
                wsdl_element.base.element_extensible.ops );
        binding_op_impl->binding_op.
                base.binding_op_element.base.documentable_element.
                wsdl_element.base.element_extensible.ops = NULL;
    }
    

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_woden_binding_op_free(void *binding_op,
                        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_impl = INTF_TO_IMPL(binding_op);

    if(binding_op_impl->f_ref)
    {
        AXIS2_QNAME_FREE(binding_op_impl->f_ref, env);
        binding_op_impl->f_ref = NULL;
    }

    if(binding_op_impl->f_msg_refs)
    {
        AXIS2_ARRAY_LIST_FREE(binding_op_impl->f_msg_refs, env);
        binding_op_impl->f_msg_refs = NULL;
    }

    if(binding_op_impl->f_fault_refs)
    {
        AXIS2_ARRAY_LIST_FREE(binding_op_impl->f_fault_refs, env);
        binding_op_impl->f_fault_refs = NULL;
    }

    if(binding_op_impl->f_interface_op)
    {
        AXIS2_INTERFACE_OP_FREE(binding_op_impl->f_interface_op, env);
        binding_op_impl->f_interface_op = NULL;
    }

    if(binding_op_impl->super)
    {
        axis2_hash_free(binding_op_impl->super, env);
        binding_op_impl->super = NULL;
    }
    
    if(binding_op_impl->methods)
    {
        axis2_hash_free(binding_op_impl->methods, env);
        binding_op_impl->methods = NULL;
    }

    if(binding_op_impl->nested_configurable)
    {
        AXIS2_WODEN_NESTED_CONFIGURABLE_FREE(binding_op_impl->
                nested_configurable, env);
        binding_op_impl->nested_configurable = NULL;
    }
    axis2_woden_binding_op_free_ops(binding_op, env);

    if((&(binding_op_impl->binding_op))->ops)
    {
        AXIS2_FREE(env->allocator, (&(binding_op_impl->binding_op))->ops);
        (&(binding_op_impl->binding_op))->ops = NULL;
    }
    
    if(binding_op_impl)
    {
        AXIS2_FREE(env->allocator, binding_op_impl);
        binding_op_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_binding_op_super_objs(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_impl = INTF_TO_IMPL(binding_op);

    return binding_op_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_binding_op_type(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    binding_op_impl = INTF_TO_IMPL(binding_op);

    return binding_op_impl->obj_type;
}

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_binding_op_get_base_impl(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    binding_op_impl = INTF_TO_IMPL(binding_op);

    return binding_op_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_resolve_methods(
        axis2_woden_binding_op_t *binding_op,
        const axis2_env_t *env,
        axis2_woden_binding_op_t *binding_op_impl,
        axis2_hash_t *methods)
{
    axis2_woden_binding_op_impl_t *binding_op_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    binding_op_impl_l = INTF_TO_IMPL(binding_op_impl);
    
    binding_op->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    binding_op->ops->to_binding_op_free = 
            axis2_hash_get(methods, "to_binding_op_free", 
            AXIS2_HASH_KEY_STRING);
    binding_op->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    binding_op->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    binding_op->ops->get_interface_op = axis2_hash_get(methods, 
            "get_interface_op", AXIS2_HASH_KEY_STRING);
    if(!binding_op->ops->get_interface_op && binding_op_impl_l)
            binding_op->ops->get_interface_op = 
            binding_op_impl_l->binding_op.ops->get_interface_op;
       
    binding_op->ops->get_binding_msg_refs = axis2_hash_get(methods, 
            "get_binding_msg_refs", AXIS2_HASH_KEY_STRING);
    if(!binding_op->ops->get_binding_msg_refs && binding_op_impl_l)
            binding_op->ops->get_binding_msg_refs = 
            binding_op_impl_l->binding_op.ops->get_binding_msg_refs;
    
    binding_op->ops->get_binding_fault_refs = axis2_hash_get(methods, 
            "get_binding_fault_refs", AXIS2_HASH_KEY_STRING);
    if(!binding_op->ops->get_binding_fault_refs && binding_op_impl_l)
            binding_op->ops->get_binding_fault_refs = 
            binding_op_impl_l->binding_op.ops->get_binding_fault_refs;
    
    
    binding_op->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!binding_op->ops->to_element && binding_op_impl_l)
            binding_op->ops->to_element = 
            binding_op_impl_l->binding_op.ops->to_element;
    
    binding_op->ops->set_interface_op_element = axis2_hash_get(methods, 
            "set_interface_op_element", AXIS2_HASH_KEY_STRING);
    if(!binding_op->ops->set_interface_op_element && binding_op_impl_l)
            binding_op->ops->set_interface_op_element = 
            binding_op_impl_l->binding_op.ops->set_interface_op_element;
    

    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Binding Operation  methods (the WSDL Component model)
 * ************************************************************/

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_msg_refs(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));
    
    return binding_op_impl->f_msg_refs;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_fault_refs(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));
    
    return binding_op_impl->f_fault_refs;
}

void *AXIS2_CALL
axis2_woden_binding_op_to_element(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));
    
    return &(binding_op_impl->binding_op);
}

void *AXIS2_CALL
axis2_woden_binding_op_get_interface_op(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));

    return binding_op_impl->f_interface_op;
}
/* ************************************************************
 *  Binding Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_set_ref(
        void *binding_op,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));

    if(!binding_op_impl->f_ref)
    {
        AXIS2_QNAME_FREE(binding_op_impl->f_ref, env);
        binding_op_impl->f_ref = NULL;
    }
    binding_op_impl->f_ref = AXIS2_QNAME_CLONE(qname, env);

    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_woden_binding_op_get_ref(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));

    return binding_op_impl->f_ref;
}

void *AXIS2_CALL
axis2_woden_binding_op_get_interface_op_element(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));

    return binding_op_impl->f_interface_op;
}


axis2_status_t AXIS2_CALL
axis2_woden_binding_op_add_binding_msg_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *msg_ref)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));
 
    if(!binding_op_impl->f_msg_refs)
    {
        binding_op_impl->f_msg_refs = axis2_array_list_create(env, 0);
        if(!binding_op_impl->f_msg_refs)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(binding_op_impl->f_msg_refs, env, msg_ref);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_remove_binding_msg_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *msg_ref)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));
 
    index = AXIS2_ARRAY_LIST_INDEX_OF(binding_op_impl->f_msg_refs, env, msg_ref);
    AXIS2_ARRAY_LIST_REMOVE(binding_op_impl->f_msg_refs, env, index);
    return AXIS2_SUCCESS;
}


axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_msg_ref_elements(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));

    return binding_op_impl->f_msg_refs;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_add_binding_fault_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *fault_ref)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));
 
    if(!binding_op_impl->f_fault_refs)
    {
        binding_op_impl->f_fault_refs = axis2_array_list_create(env, 0);
        if(!binding_op_impl->f_fault_refs)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(binding_op_impl->f_fault_refs, env, fault_ref);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_remove_binding_fault_ref_element(
        void *binding_op,
        const axis2_env_t *env,
        void *fault_ref)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));
 
    index = AXIS2_ARRAY_LIST_INDEX_OF(binding_op_impl->f_fault_refs, env, fault_ref);
    AXIS2_ARRAY_LIST_REMOVE(binding_op_impl->f_fault_refs, env, index);
    return AXIS2_SUCCESS;
}


axis2_array_list_t *AXIS2_CALL
axis2_woden_binding_op_get_binding_fault_ref_elements(
        void *binding_op,
        const axis2_env_t *env)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));

    return binding_op_impl->f_fault_refs;
}



/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_binding_op_set_interface_op_element(
        void *binding_op,
        const axis2_env_t *env,
        void *int_op)
{
    axis2_woden_binding_op_impl_t *binding_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, int_op, AXIS2_FAILURE);
    super = AXIS2_WODEN_BINDING_OP_SUPER_OBJS(binding_op, env);
    binding_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_BINDING_OP", AXIS2_HASH_KEY_STRING));

    if(!binding_op_impl->f_interface_op)
    {
        /* TODO */
    }
    binding_op_impl->f_interface_op = int_op;


    return AXIS2_SUCCESS;
}

