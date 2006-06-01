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
#include <woden/wsdl20/axis2_woden_interface_fault_ref.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <xml_schema/axis2_xml_schema_element.h>

typedef struct axis2_woden_interface_fault_ref_impl axis2_woden_interface_fault_ref_impl_t;

/** 
 * @brief Interface Fault Reference Struct Impl
 *	Axis2 Interface Fault Reference  
 */ 
struct axis2_woden_interface_fault_ref_impl
{
    axis2_woden_interface_fault_ref_t interface_fault_ref;
    axis2_woden_nested_configurable_t *nested_configurable;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    void *f_interface_fault;
    void *f_msg_label;
    void *f_direction;
    axis2_qname_t *f_ref;
};

#define INTF_TO_IMPL(interface_fault_ref) ((axis2_woden_interface_fault_ref_impl_t *) interface_fault_ref)

axis2_status_t AXIS2_CALL 
axis2_woden_interface_fault_ref_free(
        void *interface_fault_ref,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_interface_fault_ref_super_objs(
        void *interface_fault_ref,
        const axis2_env_t *env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_interface_fault_ref_type(
        void *interface_fault_ref,
        const axis2_env_t *env);

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_interface_fault_ref_get_base_impl(
        void *interface_fault_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Interface Fault Reference  methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_interface_fault(
        void *interface_fault_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_msg_label(
        void *interface_fault_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_direction(
        void *interface_fault_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_interface_fault_ref_to_element(
        void *interface_fault_ref,
        const axis2_env_t *env);

/* ************************************************************
 *  Interface Fault Reference Element methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_ref(
        void *interface_fault_ref,
        const axis2_env_t *env,
        axis2_qname_t *fault_qname);

axis2_qname_t *AXIS2_CALL
axis2_woden_interface_fault_ref_get_ref(
        void *interface_fault_ref,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_interface_fault_element(
        void *interface_fault_ref,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_msg_label(
        void *interface_fault_ref,
        const axis2_env_t *env,
        void *msg_label);

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_direction(
        void *interface_fault_ref,
        const axis2_env_t *env,
        void *dir);

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_interface_fault(
        void *interface_fault_ref,
        const axis2_env_t *env,
        void *fault);

static axis2_woden_interface_fault_ref_t *
create(const axis2_env_t *env);

static axis2_status_t
axis2_woden_interface_fault_ref_free_ops(
        void *interface_fault_ref,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN axis2_woden_interface_fault_ref_t * AXIS2_CALL
axis2_woden_interface_fault_ref_to_interface_fault_ref_element(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault_ref)
    {
        interface_fault_ref_impl = (axis2_woden_interface_fault_ref_impl_t *) create(env);
    }
    else
        interface_fault_ref_impl = (axis2_woden_interface_fault_ref_impl_t *) interface_fault_ref;

    axis2_woden_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_interface_fault_ref_element_ops_t));
    axis2_woden_interface_fault_ref_element_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            interface_fault_ref_element), env, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN axis2_woden_interface_fault_ref_t * AXIS2_CALL
axis2_woden_interface_fault_ref_to_nested_configurable(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault_ref)
    {
        interface_fault_ref_impl = (axis2_woden_interface_fault_ref_impl_t *) create(env);
    }
    else
        interface_fault_ref_impl = (axis2_woden_interface_fault_ref_impl_t *) interface_fault_ref;

    axis2_woden_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_nested_configurable_ops_t));
    axis2_woden_nested_configurable_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            nested_configurable), env, interface_fault_ref_impl->nested_configurable, 
            interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

AXIS2_EXTERN axis2_woden_interface_fault_ref_t * AXIS2_CALL
axis2_woden_interface_fault_ref_to_configurable(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault_ref)
    {
        interface_fault_ref_impl = (axis2_woden_interface_fault_ref_impl_t *) create(env);
    }
    else
        interface_fault_ref_impl = (axis2_woden_interface_fault_ref_impl_t *) interface_fault_ref;

    axis2_woden_interface_fault_ref_free_ops(interface_fault_ref, env);

    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_fault_ref_impl->nested_configurable, env);
    axis2_woden_configurable_resolve_methods(&(interface_fault_ref_impl->interface_fault_ref.base.
            nested_configurable.base.configurable), env, configurable, interface_fault_ref_impl->methods);
    return interface_fault_ref;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_interface_fault_ref_t *
create(const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_interface_fault_ref_impl_t));

    interface_fault_ref_impl->obj_type= AXIS2_WODEN_INTERFACE_FAULT_REF;
    interface_fault_ref_impl->super = NULL;
    interface_fault_ref_impl->methods = NULL;
    interface_fault_ref_impl->f_interface_fault = NULL;
    interface_fault_ref_impl->f_msg_label = NULL;
    interface_fault_ref_impl->f_direction = NULL;
    interface_fault_ref_impl->f_ref = NULL;
    
    interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops = NULL;
    interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops = 
            NULL;
    
    interface_fault_ref_impl->interface_fault_ref.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(axis2_woden_interface_fault_ref_ops_t));

    interface_fault_ref_impl->interface_fault_ref.ops->free = axis2_woden_interface_fault_ref_free;
    interface_fault_ref_impl->interface_fault_ref.ops->super_objs = axis2_woden_interface_fault_ref_super_objs;
    interface_fault_ref_impl->interface_fault_ref.ops->type = axis2_woden_interface_fault_ref_type;
    interface_fault_ref_impl->interface_fault_ref.ops->get_base_impl = axis2_woden_interface_fault_ref_get_base_impl;
    
    interface_fault_ref_impl->interface_fault_ref.ops->get_interface_fault = 
        axis2_woden_interface_fault_ref_get_interface_fault;
    interface_fault_ref_impl->interface_fault_ref.ops->get_msg_label = 
        axis2_woden_interface_fault_ref_get_msg_label;
    interface_fault_ref_impl->interface_fault_ref.ops->get_direction = 
        axis2_woden_interface_fault_ref_get_direction;
    interface_fault_ref_impl->interface_fault_ref.ops->to_element = 
        axis2_woden_interface_fault_ref_to_element;
 
    interface_fault_ref_impl->methods = axis2_hash_make(env);
    if(!interface_fault_ref_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_fault_ref_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_free);
    axis2_hash_set(interface_fault_ref_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_interface_fault_ref_super_objs);
    axis2_hash_set(interface_fault_ref_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_interface_fault_ref_type);

    axis2_hash_set(interface_fault_ref_impl->methods, "get_interface_fault", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_get_interface_fault);
    axis2_hash_set(interface_fault_ref_impl->methods, "get_msg_label", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_get_msg_label);
    axis2_hash_set(interface_fault_ref_impl->methods, "get_direction", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_get_direction);
    axis2_hash_set(interface_fault_ref_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_to_element);
    axis2_hash_set(interface_fault_ref_impl->methods, "set_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_set_ref);
    axis2_hash_set(interface_fault_ref_impl->methods, "get_ref", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_get_ref);
    axis2_hash_set(interface_fault_ref_impl->methods, "get_interface_fault_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_get_interface_fault_element);
    axis2_hash_set(interface_fault_ref_impl->methods, "set_msg_label", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_set_msg_label);
    axis2_hash_set(interface_fault_ref_impl->methods, "set_direction", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_set_direction);
    axis2_hash_set(interface_fault_ref_impl->methods, "set_interface_fault", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_ref_set_interface_fault);

    return &(interface_fault_ref_impl->interface_fault_ref);
}

AXIS2_EXTERN axis2_woden_interface_fault_ref_t * AXIS2_CALL
axis2_woden_interface_fault_ref_create(const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = (axis2_woden_interface_fault_ref_impl_t *) create(env);

    interface_fault_ref_impl->nested_configurable = axis2_woden_nested_configurable_create(env);

    interface_fault_ref_impl->super = axis2_hash_make(env);
    if(!interface_fault_ref_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_fault_ref_impl->super, "AXIS2_WODEN_INTERFACE_FAULT_REF", 
            AXIS2_HASH_KEY_STRING, 
            &(interface_fault_ref_impl->interface_fault_ref));
    axis2_hash_set(interface_fault_ref_impl->super, "AXIS2_WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            interface_fault_ref_impl->nested_configurable);
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_fault_ref_impl->nested_configurable, env);
    axis2_hash_set(interface_fault_ref_impl->super, "AXIS2_WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(interface_fault_ref_impl->interface_fault_ref);
}

static axis2_status_t
axis2_woden_interface_fault_ref_free_ops(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    if(interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                interface_fault_ref_element.ops);
        interface_fault_ref_impl->interface_fault_ref.base.interface_fault_ref_element.ops = NULL;
    }

    if(interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                nested_configurable.ops);
        interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.ops = 
            NULL;
    }
    
    if(interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl->interface_fault_ref.base.
                nested_configurable.base.configurable.ops);
        interface_fault_ref_impl->interface_fault_ref.base.nested_configurable.base.configurable.ops = 
            NULL;
    }
    
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_free(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    if(interface_fault_ref_impl->f_interface_fault)
    {
        AXIS2_WODEN_INTERFACE_FAULT_FREE(interface_fault_ref_impl->
                f_interface_fault, env);
        interface_fault_ref_impl->f_interface_fault = NULL;
    }

    if(interface_fault_ref_impl->f_direction)
    {
        AXIS2_WODEN_DIRECTION_FREE(interface_fault_ref_impl->f_direction, env);
        interface_fault_ref_impl->f_direction = NULL;
    }

    if(interface_fault_ref_impl->f_ref)
    {
        AXIS2_QNAME_FREE(interface_fault_ref_impl->f_ref, env);
        interface_fault_ref_impl->f_ref = NULL;
    }

    if(interface_fault_ref_impl->super)
    {
        axis2_hash_free(interface_fault_ref_impl->super, env);
        interface_fault_ref_impl->super = NULL;
    }
    
    if(interface_fault_ref_impl->methods)
    {
        axis2_hash_free(interface_fault_ref_impl->methods, env);
        interface_fault_ref_impl->methods = NULL;
    }

    if(interface_fault_ref_impl->nested_configurable)
    {
        AXIS2_WODEN_NESTED_CONFIGURABLE_FREE(interface_fault_ref_impl->
                nested_configurable, env);
        interface_fault_ref_impl->nested_configurable = NULL;
    }

    axis2_interface_fault_ref_free_ops(interface_fault_ref, env);

    if((&(interface_fault_ref_impl->interface_fault_ref))->ops)
    {
        AXIS2_FREE(env->allocator, (&(interface_fault_ref_impl->interface_fault_ref))->ops);
        (&(interface_fault_ref_impl->interface_fault_ref))->ops = NULL;
    }
    
    if(interface_fault_ref_impl)
    {
        AXIS2_FREE(env->allocator, interface_fault_ref_impl);
        interface_fault_ref_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_interface_fault_ref_super_objs(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    return interface_fault_ref_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_interface_fault_ref_type(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    return interface_fault_ref_impl->obj_type;
}

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_interface_fault_ref_get_base_impl(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_fault_ref_impl = INTF_TO_IMPL(interface_fault_ref);

    return interface_fault_ref_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_resolve_methods(
        axis2_woden_interface_fault_ref_t *interface_fault_ref,
        const axis2_env_t *env,
        axis2_woden_interface_fault_ref_t *interface_fault_ref_impl,
        axis2_hash_t *methods)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    interface_fault_ref_impl_l = INTF_TO_IMPL(interface_fault_ref_impl);
    
    interface_fault_ref->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    interface_fault_ref->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    interface_fault_ref->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    interface_fault_ref->ops->get_interface_fault = axis2_hash_get(methods, 
            "get_interface_fault", AXIS2_HASH_KEY_STRING);
    if(!interface_fault_ref->ops->get_interface_fault && interface_fault_ref_impl_l)
            interface_fault_ref->ops->get_interface_fault = 
            interface_fault_ref_impl_l->interface_fault_ref.ops->get_interface_fault;
    
    interface_fault_ref->ops->get_msg_label = axis2_hash_get(methods, 
            "get_msg_label", AXIS2_HASH_KEY_STRING);
    if(!interface_fault_ref->ops->get_msg_label && interface_fault_ref_impl_l)
            interface_fault_ref->ops->get_msg_label = 
            interface_fault_ref_impl_l->interface_fault_ref.ops->get_msg_label;
    
    interface_fault_ref->ops->get_direction = axis2_hash_get(methods, 
            "get_direction", AXIS2_HASH_KEY_STRING);
    if(!interface_fault_ref->ops->get_direction && interface_fault_ref_impl_l)
            interface_fault_ref->ops->get_direction = 
            interface_fault_ref_impl_l->interface_fault_ref.ops->get_direction;
    
    interface_fault_ref->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!interface_fault_ref->ops->to_element && interface_fault_ref_impl_l)
            interface_fault_ref->ops->to_element = 
            interface_fault_ref_impl_l->interface_fault_ref.ops->to_element;
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Fault Reference  methods (the WSDL Component model)
 * ************************************************************/
void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_interface_fault(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));
    
    return interface_fault_ref_impl->f_interface_fault;
}

void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_msg_label(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return interface_fault_ref_impl->f_msg_label;
}

void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_direction(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return interface_fault_ref_impl->f_direction;
}
void *AXIS2_CALL
axis2_woden_interface_fault_ref_to_element(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));
    
    return &(interface_fault_ref_impl->interface_fault_ref);
}

/* ************************************************************
 *  Interface Fault Reference Element methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_ref(
        void *interface_fault_ref,
        const axis2_env_t *env,
        axis2_qname_t *fault_qname)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(interface_fault_ref_impl->f_ref)
    {
        AXIS2_QNAME_FREE(interface_fault_ref_impl->f_ref, env);
    }
    interface_fault_ref_impl->f_ref = AXIS2_QNAME_CLONE(fault_qname, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_woden_interface_fault_ref_get_ref(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return interface_fault_ref_impl->f_ref;
}

void *AXIS2_CALL
axis2_woden_interface_fault_ref_get_interface_fault_element(
        void *interface_fault_ref,
        const axis2_env_t *env)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    return NULL;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_msg_label(
        void *interface_fault_ref,
        const axis2_env_t *env,
        void *msg_label)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_label, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_ref_impl->f_msg_label)
    {
        /* TODO */
    }
    interface_fault_ref_impl->f_msg_label = msg_label;


    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_direction(
        void *interface_fault_ref,
        const axis2_env_t *env,
        void *dir)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dir, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_ref_impl->f_direction)
    {
        /* TODO */
    }
    interface_fault_ref_impl->f_direction = dir;


    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Non-API implementation methods
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_ref_set_interface_fault(
        void *interface_fault_ref,
        const axis2_env_t *env,
        void *fault)
{
    axis2_woden_interface_fault_ref_impl_t *interface_fault_ref_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_REF_SUPER_OBJS(interface_fault_ref, env);
    interface_fault_ref_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT_REF", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_ref_impl->f_interface_fault)
    {
        /* TODO */
    }
    interface_fault_ref_impl->f_interface_fault = fault;
    
    return AXIS2_SUCCESS;
}

