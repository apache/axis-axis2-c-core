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
#include <woden/wsdl20/axis2_woden_interface_fault.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <xml_schema/axis2_xml_schema_element.h>

typedef struct axis2_woden_interface_fault_impl axis2_woden_interface_fault_impl_t;

/** 
 * @brief Interface Fault Struct Impl
 *	Axis2 Interface Fault  
 */ 
struct axis2_woden_interface_fault_impl
{
    axis2_woden_interface_fault_t interface_fault;
    axis2_woden_nested_configurable_t *nested_configurable;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_qname_t *f_qname;
    void *f_element_decl;
    axis2_qname_t *f_element_qname;
    void *f_types;
};

#define INTF_TO_IMPL(interface_fault) ((axis2_woden_interface_fault_impl_t *) interface_fault)

axis2_status_t AXIS2_CALL 
axis2_woden_interface_fault_free(
        void *interface_fault,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_interface_fault_super_objs(
        void *interface_fault,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_interface_fault_type(
        void *interface_fault,
        axis2_env_t **env);

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_interface_fault_get_base_impl(
        void *interface_fault,
        axis2_env_t **env);

/* ************************************************************
 *  Interface Fault methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
axis2_woden_interface_fault_get_qname(
        void *interface_fault,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_interface_fault_get_element_declaration(
        void *interface_fault,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_interface_fault_to_element(
        void *interface_fault,
        axis2_env_t **env);

/* ************************************************************
 *  Interface FaultElement interface_fault methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_qname(
        void *interface_fault,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_element_qname(
        void *interface_fault,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_qname_t *AXIS2_CALL
axis2_woden_interface_fault_get_element_qname(
        void *interface_fault,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_interface_fault_get_element(
        void *interface_fault,
        axis2_env_t **env);

/* ******************************************************************
 * Non-API implementation methods
 * ******************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_element_declaration(
        void *interface_fault,
        axis2_env_t **env,
        void *element_decl);

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_types(
        void *interface_fault,
        axis2_env_t **env,
        void *types);

static axis2_woden_interface_fault_t *
create(axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_interface_fault_t *)
axis2_woden_interface_fault_to_interface_fault_element(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (axis2_woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (axis2_woden_interface_fault_impl_t *) interface_fault;

    interface_fault_impl->interface_fault.base.interface_fault_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_interface_fault_element_ops_t));
    axis2_woden_interface_fault_element_resolve_methods(&(interface_fault_impl->interface_fault.base.
            interface_fault_element), env, interface_fault_impl->methods);
    return interface_fault;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_to_interface_fault_element_free(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    if(interface_fault_impl->interface_fault.base.interface_fault_element.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_interface_fault_t *)
axis2_woden_interface_fault_to_nested_configurable(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (axis2_woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (axis2_woden_interface_fault_impl_t *) interface_fault;

    interface_fault_impl->interface_fault.base.nested_configurable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_configurable_ops_t));
    axis2_woden_nested_configurable_resolve_methods(&(interface_fault_impl->interface_fault.base.
            nested_configurable), env, interface_fault_impl->nested_configurable, 
            interface_fault_impl->methods);
    return interface_fault;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_to_nested_configurable_free(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    if(interface_fault_impl->interface_fault.base.nested_configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_fault_impl->interface_fault.base.
                nested_configurable.ops);
        interface_fault_impl->interface_fault.base.nested_configurable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


AXIS2_DECLARE(axis2_woden_interface_fault_t *)
axis2_woden_interface_fault_to_configurable(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (axis2_woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (axis2_woden_interface_fault_impl_t *) interface_fault;

    interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_fault_impl->nested_configurable, env);
    axis2_woden_configurable_resolve_methods(&(interface_fault_impl->interface_fault.base.
            nested_configurable.base.configurable), env, configurable, interface_fault_impl->methods);
    return interface_fault;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_to_configurable_free(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    if(interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_fault_impl->interface_fault.base.
                nested_configurable.base.configurable.ops);
        interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_interface_fault_t *
create(axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_interface_fault_impl_t));

    interface_fault_impl->obj_type= AXIS2_WODEN_INTERFACE_FAULT;
    interface_fault_impl->super = NULL;
    interface_fault_impl->methods = NULL;
    interface_fault_impl->f_qname = NULL;
    interface_fault_impl->f_element_decl = NULL;
    interface_fault_impl->f_element_qname = NULL;
    interface_fault_impl->f_types = NULL;
    
    interface_fault_impl->interface_fault.base.interface_fault_element.ops = NULL;
    interface_fault_impl->interface_fault.base.nested_configurable.ops = NULL;
    interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.ops = 
            NULL;
    
    interface_fault_impl->interface_fault.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_interface_fault_ops_t));

    interface_fault_impl->interface_fault.ops->free = axis2_woden_interface_fault_free;
    interface_fault_impl->interface_fault.ops->super_objs = axis2_woden_interface_fault_super_objs;
    interface_fault_impl->interface_fault.ops->type = axis2_woden_interface_fault_type;
    interface_fault_impl->interface_fault.ops->get_base_impl = axis2_woden_interface_fault_get_base_impl;
    
    interface_fault_impl->interface_fault.ops->get_qname = axis2_woden_interface_fault_get_qname;
    interface_fault_impl->interface_fault.ops->get_element_declaration = 
        axis2_woden_interface_fault_get_element_declaration;
    interface_fault_impl->interface_fault.ops->to_element = 
        axis2_woden_interface_fault_to_element;
 
    interface_fault_impl->methods = axis2_hash_make(env);
    if(!interface_fault_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_fault_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_free);
    axis2_hash_set(interface_fault_impl->methods, "to_interface_fault_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_to_interface_fault_element_free);
    axis2_hash_set(interface_fault_impl->methods, "to_nested_configurable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_to_nested_configurable_free);
    axis2_hash_set(interface_fault_impl->methods, "to_configurable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_to_configurable_free);
    axis2_hash_set(interface_fault_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_interface_fault_super_objs);
    axis2_hash_set(interface_fault_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_interface_fault_type);

    axis2_hash_set(interface_fault_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_get_qname);
    axis2_hash_set(interface_fault_impl->methods, "get_element_declaration", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_get_element_declaration);
    axis2_hash_set(interface_fault_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_to_element);
    axis2_hash_set(interface_fault_impl->methods, "set_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_set_qname);
    axis2_hash_set(interface_fault_impl->methods, "set_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_set_element_qname);
    axis2_hash_set(interface_fault_impl->methods, "get_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_get_element_qname);
    axis2_hash_set(interface_fault_impl->methods, "get_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_get_element);
    axis2_hash_set(interface_fault_impl->methods, "set_element_declaration", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_set_element_declaration);
    axis2_hash_set(interface_fault_impl->methods, "set_types", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_fault_set_types);

    return &(interface_fault_impl->interface_fault);
}

AXIS2_DECLARE(axis2_woden_interface_fault_t *)
axis2_woden_interface_fault_create(axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = (axis2_woden_interface_fault_impl_t *) create(env);

    interface_fault_impl->nested_configurable = axis2_woden_nested_configurable_create(env);

    interface_fault_impl->super = axis2_hash_make(env);
    if(!interface_fault_impl->super) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_fault_impl->super, "AXIS2_WODEN_INTERFACE_FAULT", 
            AXIS2_HASH_KEY_STRING, 
            &(interface_fault_impl->interface_fault));
    axis2_hash_set(interface_fault_impl->super, "AXIS2_WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            interface_fault_impl->nested_configurable);
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_fault_impl->nested_configurable, env);
    axis2_hash_set(interface_fault_impl->super, "AXIS2_WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(interface_fault_impl->interface_fault);
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_free(void *interface_fault,
                        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    if(interface_fault_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_fault_impl->f_qname, env);
        interface_fault_impl->f_qname = NULL;
    }

    if(interface_fault_impl->f_element_decl)
    {
        AXIS2_WODEN_ELEMENT_DECLARATION_FREE(interface_fault_impl->
                f_element_decl, env);
        interface_fault_impl->f_element_decl = NULL;
    }

    if(interface_fault_impl->f_element_qname)
    {
        AXIS2_QNAME_FREE(interface_fault_impl->f_element_qname, env);
        interface_fault_impl->f_element_qname = NULL;
    }

    if(interface_fault_impl->f_types)
    {
        AXIS2_WODEN_TYPES_IMPL_FREE(interface_fault_impl->f_types, env);
        interface_fault_impl->f_types = NULL;
    }

    if(interface_fault_impl->super)
    {
        axis2_hash_free(interface_fault_impl->super, env);
        interface_fault_impl->super = NULL;
    }
    
    if(interface_fault_impl->methods)
    {
        axis2_hash_free(interface_fault_impl->methods, env);
        interface_fault_impl->methods = NULL;
    }

    if(interface_fault_impl->nested_configurable)
    {
        AXIS2_WODEN_NESTED_CONFIGURABLE_FREE(interface_fault_impl->
                nested_configurable, env);
        interface_fault_impl->nested_configurable = NULL;
    }

    if(interface_fault_impl->interface_fault.base.interface_fault_element.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.ops = NULL;
    }

    if(interface_fault_impl->interface_fault.base.nested_configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_fault_impl->interface_fault.base.
                nested_configurable.ops);
        interface_fault_impl->interface_fault.base.nested_configurable.ops = 
            NULL;
    }
    
    if(interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_fault_impl->interface_fault.base.
                nested_configurable.base.configurable.ops);
        interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.ops = 
            NULL;
    }
    
    if((&(interface_fault_impl->interface_fault))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(interface_fault_impl->interface_fault))->ops);
        (&(interface_fault_impl->interface_fault))->ops = NULL;
    }
    
    if(interface_fault_impl)
    {
        AXIS2_FREE((*env)->allocator, interface_fault_impl);
        interface_fault_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_interface_fault_super_objs(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    return interface_fault_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_interface_fault_type(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    return interface_fault_impl->obj_type;
}

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_interface_fault_get_base_impl(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    return interface_fault_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_resolve_methods(
        axis2_woden_interface_fault_t *interface_fault,
        axis2_env_t **env,
        axis2_woden_interface_fault_t *interface_fault_impl,
        axis2_hash_t *methods)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    interface_fault_impl_l = INTF_TO_IMPL(interface_fault_impl);
    
    interface_fault->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    interface_fault->ops->to_interface_fault_free = 
            axis2_hash_get(methods, "to_interface_fault_free", 
            AXIS2_HASH_KEY_STRING);
    interface_fault->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    interface_fault->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    interface_fault->ops->get_qname = axis2_hash_get(methods, 
            "get_qname", AXIS2_HASH_KEY_STRING);
    if(!interface_fault->ops->get_qname && interface_fault_impl_l)
            interface_fault->ops->get_qname = 
            interface_fault_impl_l->interface_fault.ops->get_qname;
    
    interface_fault->ops->get_element_declaration = axis2_hash_get(methods, 
            "get_element_declaration", AXIS2_HASH_KEY_STRING);
    if(!interface_fault->ops->get_element_declaration && interface_fault_impl_l)
            interface_fault->ops->get_element_declaration = 
            interface_fault_impl_l->interface_fault.ops->get_element_declaration;
    
    interface_fault->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!interface_fault->ops->to_element && interface_fault_impl_l)
            interface_fault->ops->to_element = 
            interface_fault_impl_l->interface_fault.ops->to_element;
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Fault interface_fault methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
axis2_woden_interface_fault_get_qname(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));
    
    return interface_fault_impl->f_qname;
}

void *AXIS2_CALL
axis2_woden_interface_fault_get_element_declaration(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    return interface_fault_impl->f_element_decl;
}

void *AXIS2_CALL
axis2_woden_interface_fault_to_element(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));
    
    return &(interface_fault_impl->interface_fault);
}

/* ************************************************************
 *  Interface Fault Element interface_fault methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_qname(
        void *interface_fault,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(interface_fault_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_fault_impl->f_qname, env);
    }
    interface_fault_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_element_qname(
        void *interface_fault,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_impl->f_element_qname)
    {
        AXIS2_QNAME_FREE(interface_fault_impl->f_element_qname, env);
    }
    interface_fault_impl->f_element_qname = AXIS2_QNAME_CLONE(qname, env);


    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_woden_interface_fault_get_element_qname(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    return interface_fault_impl->f_element_qname;
}

void *AXIS2_CALL
axis2_woden_interface_fault_get_element(
        void *interface_fault,
        axis2_env_t **env)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;
    axis2_xml_schema_element_t *xse = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(interface_fault_impl->f_types)
    {
        xse = (axis2_xml_schema_element_t *) AXIS2_WODEN_TYPES_IMPL_GET_ELEMENT_DECLARATION(
                interface_fault_impl->f_element_qname, env);
    }
    return xse;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_element_declaration(
        void *interface_fault,
        axis2_env_t **env,
        void *element_decl)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, element_decl, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_impl->f_element_decl)
    {
        /* TODO */
    }
    interface_fault_impl->f_element_decl = element_decl;
    

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_fault_set_types(
        void *interface_fault,
        axis2_env_t **env,
        void *types)
{
    axis2_woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, types, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_impl->f_types)
    {
        /* TODO */
    }
    interface_fault_impl->f_types = types;
    
    return AXIS2_SUCCESS;
}

