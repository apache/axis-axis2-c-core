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
#include <woden_interface_fault.h>
#include <woden_nested_configurable.h>
#include <woden_ext_element.h>
#include <woden_component_exts.h>
#include <woden_xml_attr.h>
#include <woden_documentation_element.h>
#include <xml_schema_element.h>
#include <woden_element_decl.h>
#include <woden_types.h>

typedef struct woden_interface_fault_impl woden_interface_fault_impl_t;

/** 
 * @brief Interface Fault Struct Impl
 *   Axis2 Interface Fault  
 */ 
struct woden_interface_fault_impl
{
    woden_interface_fault_t interface_fault;
    woden_nested_configurable_t *nested_configurable;
    woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_qname_t *f_qname;
    void *f_element_decl;
    axis2_qname_t *f_element_qname;
    void *f_types;
};

#define INTF_TO_IMPL(interface_fault) ((woden_interface_fault_impl_t *) interface_fault)

axis2_status_t AXIS2_CALL 
woden_interface_fault_free(
        void *interface_fault,
        const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL 
woden_interface_fault_super_objs(
        void *interface_fault,
        const axis2_env_t *env);

woden_obj_types_t AXIS2_CALL 
woden_interface_fault_type(
        void *interface_fault,
        const axis2_env_t *env);

woden_nested_configurable_t *AXIS2_CALL
woden_interface_fault_get_base_impl(
        void *interface_fault,
        const axis2_env_t *env);

/* ************************************************************
 *  Interface Fault methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
woden_interface_fault_get_qname(
        void *interface_fault,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_fault_get_element_declaration(
        void *interface_fault,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_fault_to_element(
        void *interface_fault,
        const axis2_env_t *env);

/* ************************************************************
 *  Interface FaultElement interface_fault methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_interface_fault_set_qname(
        void *interface_fault,
        const axis2_env_t *env,
        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
woden_interface_fault_set_element_qname(
        void *interface_fault,
        const axis2_env_t *env,
        axis2_qname_t *qname);

axis2_qname_t *AXIS2_CALL
woden_interface_fault_get_element_qname(
        void *interface_fault,
        const axis2_env_t *env);

void *AXIS2_CALL
woden_interface_fault_get_element(
        void *interface_fault,
        const axis2_env_t *env);

/* ******************************************************************
 * Non-API implementation methods
 * ******************************************************************/

axis2_status_t AXIS2_CALL
woden_interface_fault_set_element_declaration(
        void *interface_fault,
        const axis2_env_t *env,
        void *element_decl);

axis2_status_t AXIS2_CALL
woden_interface_fault_set_types(
        void *interface_fault,
        const axis2_env_t *env,
        void *types);

static woden_interface_fault_t *
create(const axis2_env_t *env);

static axis2_status_t
woden_interface_fault_free_ops(
        void *interface_fault,
        const axis2_env_t *env);

/************************Woden C Internal Methods******************************/
AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_interface_fault_element(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.interface_fault_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_interface_fault_element_ops_t));
    woden_interface_fault_element_resolve_methods(&(interface_fault_impl->interface_fault.base.
            interface_fault_element), env, interface_fault_impl->methods);
    return interface_fault;
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_nested_configurable(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.nested_configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_nested_configurable_ops_t));
    woden_nested_configurable_resolve_methods(&(interface_fault_impl->interface_fault.base.
            nested_configurable), env, interface_fault_impl->nested_configurable, 
            interface_fault_impl->methods);
    return interface_fault;
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_configurable(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_configurable_ops_t));
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_fault_impl->nested_configurable, env);
    woden_configurable_resolve_methods(&(interface_fault_impl->interface_fault.base.
            nested_configurable.base.configurable), env, configurable, interface_fault_impl->methods);
    return interface_fault;
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_nested_element(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.interface_fault_element.base.nested_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_nested_element_ops_t));
    woden_nested_element_resolve_methods(&(interface_fault_impl->interface_fault.base.
            interface_fault_element.base.nested_element), env, interface_fault_impl->methods);
    return interface_fault;
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_configurable_element(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.interface_fault_element.base.configurable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_configurable_element_ops_t));
    woden_configurable_element_resolve_methods(&(interface_fault_impl->interface_fault.base.
            interface_fault_element.base.configurable_element), env, interface_fault_impl->methods);
    return interface_fault;
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_documentable_element(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.interface_fault_element.base.documentable_element.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_documentable_element_ops_t));
    woden_documentable_element_resolve_methods(&(interface_fault_impl->interface_fault.base.
            interface_fault_element.base.documentable_element), env, 
            interface_fault_impl->methods);
    return interface_fault;
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_documentable(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.nested_configurable.base.
        configurable.base.documentable.ops = AXIS2_MALLOC(env->allocator, 
                sizeof(woden_documentable_ops_t));
    woden_documentable_resolve_methods(&(interface_fault_impl->interface_fault.base.
            nested_configurable.base.configurable.base.documentable), env, NULL,
            interface_fault_impl->methods);
    return interface_fault;
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_attr_extensible(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.interface_fault_element.base.documentable_element.
        wsdl_element.base.attr_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_attr_extensible_ops_t));
    woden_attr_extensible_resolve_methods(&(interface_fault_impl->interface_fault.base.
            interface_fault_element.base.documentable_element.wsdl_element.base.
            attr_extensible), env, NULL, interface_fault_impl->methods);
    return interface_fault;
}


AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_to_element_extensible(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_fault)
    {
        interface_fault_impl = (woden_interface_fault_impl_t *) create(env);
    }
    else
        interface_fault_impl = (woden_interface_fault_impl_t *) interface_fault;

    woden_interface_fault_free_ops(interface_fault, env);

    interface_fault_impl->interface_fault.base.interface_fault_element.base.documentable_element.
        wsdl_element.base.element_extensible.ops = 
        AXIS2_MALLOC(env->allocator, 
                sizeof(woden_element_extensible_ops_t));
    woden_element_extensible_resolve_methods(&(interface_fault_impl->interface_fault.base.
            interface_fault_element.base.documentable_element.wsdl_element.base.
            element_extensible), env, NULL, interface_fault_impl->methods);
    return interface_fault;
}

/************************End of Woden C Internal Methods***********************/
static woden_interface_fault_t *
create(const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(woden_interface_fault_impl_t));

    interface_fault_impl->obj_type= WODEN_INTERFACE_FAULT;
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
    interface_fault_impl->interface_fault.base.interface_fault_element.base.
        nested_element.ops = NULL;
    interface_fault_impl->interface_fault.base.interface_fault_element.base.
        configurable_element.ops = NULL;
    interface_fault_impl->interface_fault.base.interface_fault_element.base.
        documentable_element.ops = NULL;
    interface_fault_impl->interface_fault.base.nested_configurable.base.configurable.base.
        documentable.ops = NULL;
    interface_fault_impl->interface_fault.base.interface_fault_element.base.
        documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    interface_fault_impl->interface_fault.base.interface_fault_element.base.
        documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    
    interface_fault_impl->interface_fault.ops = AXIS2_MALLOC(env->allocator, 
            sizeof(woden_interface_fault_ops_t));

    interface_fault_impl->interface_fault.ops->free = woden_interface_fault_free;
    interface_fault_impl->interface_fault.ops->super_objs = woden_interface_fault_super_objs;
    interface_fault_impl->interface_fault.ops->type = woden_interface_fault_type;
    interface_fault_impl->interface_fault.ops->get_base_impl = woden_interface_fault_get_base_impl;
    
    interface_fault_impl->interface_fault.ops->get_qname = woden_interface_fault_get_qname;
    interface_fault_impl->interface_fault.ops->get_element_declaration = 
        woden_interface_fault_get_element_declaration;
    interface_fault_impl->interface_fault.ops->to_element = 
        woden_interface_fault_to_element;
    interface_fault_impl->interface_fault.ops->set_element_declaration = 
        woden_interface_fault_set_element_declaration;
    interface_fault_impl->interface_fault.ops->set_types = 
        woden_interface_fault_set_types;
 
    interface_fault_impl->methods = axis2_hash_make(env);
    if(!interface_fault_impl->methods) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_fault_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_free);
    axis2_hash_set(interface_fault_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, woden_interface_fault_super_objs);
    axis2_hash_set(interface_fault_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, woden_interface_fault_type);

    axis2_hash_set(interface_fault_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_get_qname);
    axis2_hash_set(interface_fault_impl->methods, "get_element_declaration", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_get_element_declaration);
    axis2_hash_set(interface_fault_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_to_element);
    axis2_hash_set(interface_fault_impl->methods, "set_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_set_qname);
    axis2_hash_set(interface_fault_impl->methods, "set_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_set_element_qname);
    axis2_hash_set(interface_fault_impl->methods, "get_element_qname", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_get_element_qname);
    axis2_hash_set(interface_fault_impl->methods, "get_element", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_get_element);
    axis2_hash_set(interface_fault_impl->methods, "set_element_declaration", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_set_element_declaration);
    axis2_hash_set(interface_fault_impl->methods, "set_types", 
            AXIS2_HASH_KEY_STRING, 
            woden_interface_fault_set_types);

    return &(interface_fault_impl->interface_fault);
}

AXIS2_EXTERN woden_interface_fault_t * AXIS2_CALL
woden_interface_fault_create(const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = (woden_interface_fault_impl_t *) create(env);

    interface_fault_impl->nested_configurable = woden_nested_configurable_create(env);

    interface_fault_impl->super = axis2_hash_make(env);
    if(!interface_fault_impl->super) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_fault_impl->super, "WODEN_INTERFACE_FAULT", 
            AXIS2_HASH_KEY_STRING, 
            &(interface_fault_impl->interface_fault));
    axis2_hash_set(interface_fault_impl->super, "WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            interface_fault_impl->nested_configurable);
    configurable = WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_fault_impl->nested_configurable, env);
    axis2_hash_set(interface_fault_impl->super, "WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(interface_fault_impl->interface_fault);
}

static axis2_status_t
woden_interface_fault_free_ops(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    if(interface_fault_impl->interface_fault.base.interface_fault_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.ops = NULL;
    }

    if(interface_fault_impl->interface_fault.base.nested_configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                nested_configurable.ops);
        interface_fault_impl->interface_fault.base.nested_configurable.ops = 
            NULL;
    }
    
    if(interface_fault_impl->interface_fault.base.nested_configurable.base.
            configurable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                nested_configurable.base.configurable.ops);
        interface_fault_impl->interface_fault.base.nested_configurable.base.
            configurable.ops = 
            NULL;
    }

    if(interface_fault_impl->interface_fault.base.interface_fault_element.base.
            nested_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.base.nested_element.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.base.
            nested_element.ops = NULL;
    }
 
    if(interface_fault_impl->interface_fault.base.interface_fault_element.base.
            configurable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.base.configurable_element.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.base.
            configurable_element.ops = NULL;
    }
    
    if(interface_fault_impl->interface_fault.base.interface_fault_element.base.
            documentable_element.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.base.documentable_element.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.base.
            documentable_element.ops = NULL;
    }
     
    if(interface_fault_impl->interface_fault.base.nested_configurable.base.
            configurable.base.documentable.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                nested_configurable.base.configurable.base.documentable.ops);
        interface_fault_impl->interface_fault.base.nested_configurable.base.
            configurable.base.documentable.ops = NULL;
    }
      
    if(interface_fault_impl->interface_fault.base.interface_fault_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.base.documentable_element.wsdl_element.base.
                attr_extensible.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.base.
            documentable_element.wsdl_element.base.attr_extensible.ops = NULL;
    }
      
    if(interface_fault_impl->interface_fault.base.interface_fault_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl->interface_fault.base.
                interface_fault_element.base.documentable_element.wsdl_element.base.
                element_extensible.ops);
        interface_fault_impl->interface_fault.base.interface_fault_element.base.
            documentable_element.wsdl_element.base.element_extensible.ops = NULL;
    }
  
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
woden_interface_fault_free(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    if(interface_fault_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_fault_impl->f_qname, env);
        interface_fault_impl->f_qname = NULL;
    }

    if(interface_fault_impl->f_element_decl)
    {
        WODEN_ELEMENT_DECL_FREE(interface_fault_impl->
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
        WODEN_TYPES_FREE(interface_fault_impl->f_types, env);
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
        WODEN_NESTED_CONFIGURABLE_FREE(interface_fault_impl->
                nested_configurable, env);
        interface_fault_impl->nested_configurable = NULL;
    }

    woden_interface_fault_free_ops(interface_fault, env);

    if(interface_fault_impl)
    {
        AXIS2_FREE(env->allocator, interface_fault_impl);
        interface_fault_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
woden_interface_fault_super_objs(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    return interface_fault_impl->super;
}

woden_obj_types_t AXIS2_CALL
woden_interface_fault_type(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    return interface_fault_impl->obj_type;
}

woden_nested_configurable_t *AXIS2_CALL
woden_interface_fault_get_base_impl(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_fault_impl = INTF_TO_IMPL(interface_fault);

    return interface_fault_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
woden_interface_fault_resolve_methods(
        woden_interface_fault_t *interface_fault,
        const axis2_env_t *env,
        woden_interface_fault_t *interface_fault_impl,
        axis2_hash_t *methods)
{
    woden_interface_fault_impl_t *interface_fault_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, methods, AXIS2_FAILURE);
    interface_fault_impl_l = INTF_TO_IMPL(interface_fault_impl);
    
    interface_fault->ops->free = axis2_hash_get(methods, "free", 
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
    
    interface_fault->ops->set_element_declaration = axis2_hash_get(methods, 
            "set_element_declaration", AXIS2_HASH_KEY_STRING);
    if(!interface_fault->ops->set_element_declaration && interface_fault_impl_l)
            interface_fault->ops->set_element_declaration = 
            interface_fault_impl_l->interface_fault.ops->set_element_declaration;
    
    interface_fault->ops->set_types = axis2_hash_get(methods, 
            "set_types", AXIS2_HASH_KEY_STRING);
    if(!interface_fault->ops->set_types && interface_fault_impl_l)
            interface_fault->ops->set_types = 
            interface_fault_impl_l->interface_fault.ops->set_types;
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Fault interface_fault methods (the WSDL Component model)
 * ************************************************************/
axis2_qname_t *AXIS2_CALL
woden_interface_fault_get_qname(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));
    
    return interface_fault_impl->f_qname;
}

void *AXIS2_CALL
woden_interface_fault_get_element_declaration(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    return interface_fault_impl->f_element_decl;
}

void *AXIS2_CALL
woden_interface_fault_to_element(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));
    
    return &(interface_fault_impl->interface_fault);
}

/* ************************************************************
 *  Interface Fault Element interface_fault methods (the XML Element model)
 * ************************************************************/
axis2_status_t AXIS2_CALL
woden_interface_fault_set_qname(
        void *interface_fault,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(interface_fault_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_fault_impl->f_qname, env);
    }
    interface_fault_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_fault_set_element_qname(
        void *interface_fault,
        const axis2_env_t *env,
        axis2_qname_t *qname)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_impl->f_element_qname)
    {
        AXIS2_QNAME_FREE(interface_fault_impl->f_element_qname, env);
    }
    interface_fault_impl->f_element_qname = AXIS2_QNAME_CLONE(qname, env);


    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
woden_interface_fault_get_element_qname(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    return interface_fault_impl->f_element_qname;
}

void *AXIS2_CALL
woden_interface_fault_get_element(
        void *interface_fault,
        const axis2_env_t *env)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;
    xml_schema_element_t *xse = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(interface_fault_impl->f_types)
    {
        xse = (xml_schema_element_t *) WODEN_TYPES_IMPL_GET_ELEMENT_DECLARATION(
                interface_fault_impl->f_element_qname, env);
    }
    return xse;
}

axis2_status_t AXIS2_CALL
woden_interface_fault_set_element_declaration(
        void *interface_fault,
        const axis2_env_t *env,
        void *element_decl)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, element_decl, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_impl->f_element_decl)
    {
        /* TODO */
    }
    interface_fault_impl->f_element_decl = element_decl;
    

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
woden_interface_fault_set_types(
        void *interface_fault,
        const axis2_env_t *env,
        void *types)
{
    woden_interface_fault_impl_t *interface_fault_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, types, AXIS2_FAILURE);
    super = WODEN_INTERFACE_FAULT_SUPER_OBJS(interface_fault, env);
    interface_fault_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "WODEN_INTERFACE_FAULT", AXIS2_HASH_KEY_STRING));

    if(!interface_fault_impl->f_types)
    {
        /* TODO */
    }
    interface_fault_impl->f_types = types;
    
    return AXIS2_SUCCESS;
}

