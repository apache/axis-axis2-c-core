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
#include <woden/wsdl20/axis2_woden_interface_op.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>
#include <xml_schema/axis2_xml_schema_element.h>

typedef struct axis2_woden_interface_op_impl axis2_woden_interface_op_impl_t;

/** 
 * @brief Interface Operation Struct Impl
 *	Axis2 Interface Operation  
 */ 
struct axis2_woden_interface_op_impl
{
    axis2_woden_interface_op_t interface_op;
    axis2_woden_nested_configurable_t *nested_configurable;
    axis2_woden_obj_types_t obj_type;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    axis2_qname_t *f_qname;
    axis2_url_t *f_msg_exchange_pattern;
    axis2_array_list_t *f_style;
    axis2_array_list_t *f_msg_refs;
    axis2_array_list_t *f_fault_refs;
};

#define INTF_TO_IMPL(interface_op) ((axis2_woden_interface_op_impl_t *) interface_op)

axis2_status_t AXIS2_CALL 
axis2_woden_interface_op_free(
        void *interface_op,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_interface_op_super_objs(
        void *interface_op,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_interface_op_type(
        void *interface_op,
        axis2_env_t **env);

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_interface_op_get_base_impl(
        void *interface_op,
        axis2_env_t **env);

/* ************************************************************
 *  Interface Operation  methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
axis2_woden_interface_op_get_qname(
        void *interface_op,
        axis2_env_t **env);

axis2_url_t *AXIS2_CALL
axis2_woden_interface_op_get_msg_exchange_pattern(
        void *interface_op,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_msg_refs(
        void *interface_op,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_fault_refs(
        void *interface_op,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_style(
        void *interface_op,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_interface_op_to_element(
        void *interface_op,
        axis2_env_t **env);

/* ************************************************************
 *  Interface Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_set_qname(
        void *interface_op,
        axis2_env_t **env,
        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_set_pattern(
        void *interface_op,
        axis2_env_t **env,
        axis2_url_t *uri);

axis2_url_t *AXIS2_CALL
axis2_woden_interface_op_get_pattern(
        void *interface_op,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_add_style_uri(
        void *interface_op,
        axis2_env_t **env,
        axis2_url_t *uri);

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_remove_style_uri(
        void *interface_op,
        axis2_env_t **env,
        axis2_url_t *uri);

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_add_interface_msg_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *msg_ref);

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_remove_interface_msg_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *msg_ref);

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_msg_ref_elements(
        void *interface_op,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_add_interface_fault_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *fault_ref);

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_remove_interface_fault_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *fault_ref);

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_fault_ref_elements(
        void *interface_op,
        axis2_env_t **env);

static axis2_woden_interface_op_t *
create(axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_interface_op_t *)
axis2_woden_interface_op_to_interface_op_element(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_op)
    {
        interface_op_impl = (axis2_woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (axis2_woden_interface_op_impl_t *) interface_op;

    interface_op_impl->interface_op.base.interface_op_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_interface_op_element_ops_t));
    axis2_woden_interface_op_element_resolve_methods(&(interface_op_impl->interface_op.base.
            interface_op_element), env, interface_op_impl->methods);
    return interface_op;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_to_interface_op_element_free(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    if(interface_op_impl->interface_op.base.interface_op_element.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_op_impl->interface_op.base.
                interface_op_element.ops);
        interface_op_impl->interface_op.base.interface_op_element.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_interface_op_t *)
axis2_woden_interface_op_to_nested_configurable(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_op)
    {
        interface_op_impl = (axis2_woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (axis2_woden_interface_op_impl_t *) interface_op;

    interface_op_impl->interface_op.base.nested_configurable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_configurable_ops_t));
    axis2_woden_nested_configurable_resolve_methods(&(interface_op_impl->interface_op.base.
            nested_configurable), env, interface_op_impl->nested_configurable, 
            interface_op_impl->methods);
    return interface_op;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_to_nested_configurable_free(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    if(interface_op_impl->interface_op.base.nested_configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_op_impl->interface_op.base.
                nested_configurable.ops);
        interface_op_impl->interface_op.base.nested_configurable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


AXIS2_DECLARE(axis2_woden_interface_op_t *)
axis2_woden_interface_op_to_configurable(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!interface_op)
    {
        interface_op_impl = (axis2_woden_interface_op_impl_t *) create(env);
    }
    else
        interface_op_impl = (axis2_woden_interface_op_impl_t *) interface_op;

    interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_op_impl->nested_configurable, env);
    axis2_woden_configurable_resolve_methods(&(interface_op_impl->interface_op.base.
            nested_configurable.base.configurable), env, configurable, interface_op_impl->methods);
    return interface_op;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_to_configurable_free(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    if(interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_op_impl->interface_op.base.
                nested_configurable.base.configurable.ops);
        interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_interface_op_t *
create(axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_interface_op_impl_t));

    interface_op_impl->obj_type= AXIS2_WODEN_INTERFACE_OP;
    interface_op_impl->super = NULL;
    interface_op_impl->methods = NULL;
    interface_op_impl->f_qname = NULL;
    interface_op_impl->f_msg_exchange_pattern = NULL;
    interface_op_impl->f_style = NULL;
    interface_op_impl->f_msg_refs = NULL;
    interface_op_impl->f_fault_refs = NULL;
    
    interface_op_impl->interface_op.base.interface_op_element.ops = NULL;
    interface_op_impl->interface_op.base.nested_configurable.ops = NULL;
    interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops = 
            NULL;
    
    interface_op_impl->interface_op.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_interface_op_ops_t));

    interface_op_impl->interface_op.ops->free = axis2_woden_interface_op_free;
    interface_op_impl->interface_op.ops->super_objs = axis2_woden_interface_op_super_objs;
    interface_op_impl->interface_op.ops->type = axis2_woden_interface_op_type;
    interface_op_impl->interface_op.ops->get_base_impl = axis2_woden_interface_op_get_base_impl;
    
    interface_op_impl->interface_op.ops->get_qname = 
        axis2_woden_interface_op_get_qname;
    interface_op_impl->interface_op.ops->get_msg_exchange_pattern = 
        axis2_woden_interface_op_get_msg_exchange_pattern;
    interface_op_impl->interface_op.ops->get_interface_msg_refs = 
        axis2_woden_interface_op_get_interface_msg_refs;
    interface_op_impl->interface_op.ops->get_interface_fault_refs = 
        axis2_woden_interface_op_get_interface_fault_refs;
    interface_op_impl->interface_op.ops->get_style = 
        axis2_woden_interface_op_get_style;
    interface_op_impl->interface_op.ops->to_element = 
        axis2_woden_interface_op_to_element;
 
    interface_op_impl->methods = axis2_hash_make(env);
    if(!interface_op_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_op_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_free);
    axis2_hash_set(interface_op_impl->methods, "to_interface_op_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_to_interface_op_element_free);
    axis2_hash_set(interface_op_impl->methods, "to_nested_configurable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_to_nested_configurable_free);
    axis2_hash_set(interface_op_impl->methods, "to_configurable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_to_configurable_free);
    axis2_hash_set(interface_op_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_interface_op_super_objs);
    axis2_hash_set(interface_op_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_interface_op_type);

    axis2_hash_set(interface_op_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_qname);
    axis2_hash_set(interface_op_impl->methods, "get_msg_exchange_pattern", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_msg_exchange_pattern);
    axis2_hash_set(interface_op_impl->methods, "get_interface_msg_refs", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_interface_msg_refs);
    axis2_hash_set(interface_op_impl->methods, "get_interface_fault_refs", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_interface_fault_refs);
    axis2_hash_set(interface_op_impl->methods, "get_style", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_style);
    axis2_hash_set(interface_op_impl->methods, "to_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_to_element);
    axis2_hash_set(interface_op_impl->methods, "set_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_set_qname);
    axis2_hash_set(interface_op_impl->methods, "set_pattern", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_set_pattern);
    axis2_hash_set(interface_op_impl->methods, "get_pattern", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_pattern);
    axis2_hash_set(interface_op_impl->methods, "add_style_uri", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_add_style_uri);
    axis2_hash_set(interface_op_impl->methods, "remove_style_uri", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_remove_style_uri);
    axis2_hash_set(interface_op_impl->methods, "add_interface_msg_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_add_interface_msg_ref_element);
    axis2_hash_set(interface_op_impl->methods, "remove_interface_msg_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_remove_interface_msg_ref_element);
    axis2_hash_set(interface_op_impl->methods, "get_interface_msg_ref_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_interface_msg_ref_elements);
    axis2_hash_set(interface_op_impl->methods, "add_interface_fault_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_add_interface_fault_ref_element);
    axis2_hash_set(interface_op_impl->methods, "remove_interface_fault_ref_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_remove_interface_fault_ref_element);
    axis2_hash_set(interface_op_impl->methods, "get_interface_fault_ref_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_interface_op_get_interface_fault_ref_elements);

    return &(interface_op_impl->interface_op);
}

AXIS2_DECLARE(axis2_woden_interface_op_t *)
axis2_woden_interface_op_create(axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    void *configurable = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = (axis2_woden_interface_op_impl_t *) create(env);

    interface_op_impl->nested_configurable = axis2_woden_nested_configurable_create(env);

    interface_op_impl->super = axis2_hash_make(env);
    if(!interface_op_impl->super) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(interface_op_impl->super, "AXIS2_WODEN_INTERFACE_OP", 
            AXIS2_HASH_KEY_STRING, 
            &(interface_op_impl->interface_op));
    axis2_hash_set(interface_op_impl->super, "AXIS2_WODEN_NESTED_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, 
            interface_op_impl->nested_configurable);
    configurable = AXIS2_WODEN_NESTED_CONFIGURABLE_GET_BASE_IMPL(
            interface_op_impl->nested_configurable, env);
    axis2_hash_set(interface_op_impl->super, "AXIS2_WODEN_CONFIGURABLE", 
            AXIS2_HASH_KEY_STRING, configurable);
 
    return &(interface_op_impl->interface_op);
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_free(void *interface_op,
                        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    if(interface_op_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_op_impl->f_qname, env);
        interface_op_impl->f_qname = NULL;
    }

    if(interface_op_impl->f_msg_exchange_pattern)
    {
        AXIS2_URL_FREE(interface_op_impl->f_msg_exchange_pattern, env);
        interface_op_impl->f_msg_exchange_pattern = NULL;
    }

    if(interface_op_impl->f_style)
    {
        AXIS2_ARRAY_LIST_FREE(interface_op_impl->f_style, env);
        interface_op_impl->f_style = NULL;
    }

    if(interface_op_impl->f_msg_refs)
    {
        AXIS2_ARRAY_LIST_FREE(interface_op_impl->f_msg_refs, env);
        interface_op_impl->f_msg_refs = NULL;
    }

    if(interface_op_impl->f_fault_refs)
    {
        AXIS2_ARRAY_LIST_FREE(interface_op_impl->f_fault_refs, env);
        interface_op_impl->f_fault_refs = NULL;
    }

    if(interface_op_impl->super)
    {
        axis2_hash_free(interface_op_impl->super, env);
        interface_op_impl->super = NULL;
    }
    
    if(interface_op_impl->methods)
    {
        axis2_hash_free(interface_op_impl->methods, env);
        interface_op_impl->methods = NULL;
    }

    if(interface_op_impl->nested_configurable)
    {
        AXIS2_WODEN_NESTED_CONFIGURABLE_FREE(interface_op_impl->
                nested_configurable, env);
        interface_op_impl->nested_configurable = NULL;
    }

    if(interface_op_impl->interface_op.base.interface_op_element.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_op_impl->interface_op.base.
                interface_op_element.ops);
        interface_op_impl->interface_op.base.interface_op_element.ops = NULL;
    }

    if(interface_op_impl->interface_op.base.nested_configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_op_impl->interface_op.base.
                nested_configurable.ops);
        interface_op_impl->interface_op.base.nested_configurable.ops = 
            NULL;
    }
    
    if(interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, interface_op_impl->interface_op.base.
                nested_configurable.base.configurable.ops);
        interface_op_impl->interface_op.base.nested_configurable.base.configurable.ops = 
            NULL;
    }
    
    if((&(interface_op_impl->interface_op))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(interface_op_impl->interface_op))->ops);
        (&(interface_op_impl->interface_op))->ops = NULL;
    }
    
    if(interface_op_impl)
    {
        AXIS2_FREE((*env)->allocator, interface_op_impl);
        interface_op_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_interface_op_super_objs(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    return interface_op_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_interface_op_type(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    return interface_op_impl->obj_type;
}

axis2_woden_nested_configurable_t *AXIS2_CALL
axis2_woden_interface_op_get_base_impl(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    interface_op_impl = INTF_TO_IMPL(interface_op);

    return interface_op_impl->nested_configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_resolve_methods(
        axis2_woden_interface_op_t *interface_op,
        axis2_env_t **env,
        axis2_woden_interface_op_t *interface_op_impl,
        axis2_hash_t *methods)
{
    axis2_woden_interface_op_impl_t *interface_op_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    interface_op_impl_l = INTF_TO_IMPL(interface_op_impl);
    
    interface_op->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    interface_op->ops->to_interface_op_free = 
            axis2_hash_get(methods, "to_interface_op_free", 
            AXIS2_HASH_KEY_STRING);
    interface_op->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    interface_op->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    interface_op->ops->get_qname = axis2_hash_get(methods, 
            "get_qname", AXIS2_HASH_KEY_STRING);
    if(!interface_op->ops->get_qname && interface_op_impl_l)
            interface_op->ops->get_qname = 
            interface_op_impl_l->interface_op.ops->get_qname;
    
    interface_op->ops->get_msg_exchange_pattern = axis2_hash_get(methods, 
            "get_msg_exchange_pattern", AXIS2_HASH_KEY_STRING);
    if(!interface_op->ops->get_msg_exchange_pattern && interface_op_impl_l)
            interface_op->ops->get_msg_exchange_pattern = 
            interface_op_impl_l->interface_op.ops->get_msg_exchange_pattern;
    
    interface_op->ops->get_interface_msg_refs = axis2_hash_get(methods, 
            "get_interface_msg_refs", AXIS2_HASH_KEY_STRING);
    if(!interface_op->ops->get_interface_msg_refs && interface_op_impl_l)
            interface_op->ops->get_interface_msg_refs = 
            interface_op_impl_l->interface_op.ops->get_interface_msg_refs;
    
    interface_op->ops->get_interface_fault_refs = axis2_hash_get(methods, 
            "get_interface_fault_refs", AXIS2_HASH_KEY_STRING);
    if(!interface_op->ops->get_interface_fault_refs && interface_op_impl_l)
            interface_op->ops->get_interface_fault_refs = 
            interface_op_impl_l->interface_op.ops->get_interface_fault_refs;
    
    interface_op->ops->get_style = axis2_hash_get(methods, 
            "get_style", AXIS2_HASH_KEY_STRING);
    if(!interface_op->ops->get_style && interface_op_impl_l)
            interface_op->ops->get_style = 
            interface_op_impl_l->interface_op.ops->get_style;
    
    interface_op->ops->to_element = axis2_hash_get(methods, 
            "to_element", AXIS2_HASH_KEY_STRING);
    if(!interface_op->ops->to_element && interface_op_impl_l)
            interface_op->ops->to_element = 
            interface_op_impl_l->interface_op.ops->to_element;
   
    return AXIS2_SUCCESS;
}

/* ************************************************************
 *  Interface Operation  methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
axis2_woden_interface_op_get_qname(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_qname;
}

axis2_url_t *AXIS2_CALL
axis2_woden_interface_op_get_msg_exchange_pattern(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_msg_exchange_pattern;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_msg_refs(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
    
    return interface_op_impl->f_msg_refs;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_fault_refs(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
    
    return interface_op_impl->f_fault_refs;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_style(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
    
    return interface_op_impl->f_style;
}

void *AXIS2_CALL
axis2_woden_interface_op_to_element(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
    
    return &(interface_op_impl->interface_op);
}

/* ************************************************************
 *  Interface Operation Element methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_set_qname(
        void *interface_op,
        axis2_env_t **env,
        axis2_qname_t *qname)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    if(!interface_op_impl->f_qname)
    {
        AXIS2_QNAME_FREE(interface_op_impl->f_qname, env);
        interface_op_impl->f_qname = NULL;
    }
    interface_op_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_set_pattern(
        void *interface_op,
        axis2_env_t **env,
        axis2_url_t *uri)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    if(!interface_op_impl->f_msg_exchange_pattern)
    {
        /* TODO */
    }
    interface_op_impl->f_msg_exchange_pattern = uri;


    return AXIS2_SUCCESS;
}

axis2_url_t *AXIS2_CALL
axis2_woden_interface_op_get_pattern(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_msg_exchange_pattern;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_add_style_uri(
        void *interface_op,
        axis2_env_t **env,
        axis2_url_t *uri)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
 
    if(!interface_op_impl->f_style)
    {
        interface_op_impl->f_style = axis2_array_list_create(env, 0);
        if(!interface_op_impl->f_style)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(interface_op_impl->f_style, env, uri);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_remove_style_uri(
        void *interface_op,
        axis2_env_t **env,
        axis2_url_t *uri)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
 
    index = AXIS2_ARRAY_LIST_INDEX_OF(interface_op_impl->f_style, env, uri);
    AXIS2_ARRAY_LIST_REMOVE(interface_op_impl->f_style, env, index);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_add_interface_msg_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *msg_ref)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
 
    if(!interface_op_impl->f_msg_refs)
    {
        interface_op_impl->f_msg_refs = axis2_array_list_create(env, 0);
        if(!interface_op_impl->f_msg_refs)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(interface_op_impl->f_msg_refs, env, msg_ref);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_remove_interface_msg_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *msg_ref)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
 
    index = AXIS2_ARRAY_LIST_INDEX_OF(interface_op_impl->f_msg_refs, env, msg_ref);
    AXIS2_ARRAY_LIST_REMOVE(interface_op_impl->f_msg_refs, env, index);
    return AXIS2_SUCCESS;
}


axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_msg_ref_elements(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_msg_refs;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_add_interface_fault_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *fault_ref)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
 
    if(!interface_op_impl->f_fault_refs)
    {
        interface_op_impl->f_fault_refs = axis2_array_list_create(env, 0);
        if(!interface_op_impl->f_fault_refs)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    AXIS2_ARRAY_LIST_ADD(interface_op_impl->f_fault_refs, env, fault_ref);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_interface_op_remove_interface_fault_ref_element(
        void *interface_op,
        axis2_env_t **env,
        void *fault_ref)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;
    int index = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_ref, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));
 
    index = AXIS2_ARRAY_LIST_INDEX_OF(interface_op_impl->f_fault_refs, env, fault_ref);
    AXIS2_ARRAY_LIST_REMOVE(interface_op_impl->f_fault_refs, env, index);
    return AXIS2_SUCCESS;
}


axis2_array_list_t *AXIS2_CALL
axis2_woden_interface_op_get_interface_fault_ref_elements(
        void *interface_op,
        axis2_env_t **env)
{
    axis2_woden_interface_op_impl_t *interface_op_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_INTERFACE_OP_SUPER_OBJS(interface_op, env);
    interface_op_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_INTERFACE_OP", AXIS2_HASH_KEY_STRING));

    return interface_op_impl->f_fault_refs;
}



