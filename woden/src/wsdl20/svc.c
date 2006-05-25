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
#include <woden/wsdl20/axis2_woden_svc.h>
#include <woden/wsdl20/extensions/axis2_woden_ext_element.h>
#include <woden/wsdl20/extensions/axis2_woden_component_exts.h>
#include <woden/xml/axis2_woden_xml_attr.h>
#include <woden/wsdl20/xml/axis2_woden_documentation_element.h>

typedef struct axis2_woden_svc_impl axis2_woden_svc_impl_t;

/** 
 * @brief Service Struct Impl
 *	Axis2 Service  
 */ 
struct axis2_woden_svc_impl
{
    axis2_woden_svc_t svc;
    axis2_woden_obj_types_t obj_type;
    axis2_woden_configurable_t *configurable;
    axis2_hash_t *super;
    axis2_hash_t *methods;
    woden_nc_name_t *f_qname;
    axis2_qname_t *f_interface_qname;
    void *f_interface;
    axis2_array_list_t *f_endpoints;
};

#define INTF_TO_IMPL(svc) ((axis2_woden_svc_impl_t *) svc)

axis2_status_t AXIS2_CALL 
axis2_woden_svc_free(
        void *svc,
        axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_woden_svc_super_objs(
        void *svc,
        axis2_env_t **env);

axis2_woden_obj_types_t AXIS2_CALL 
axis2_woden_svc_type(
        void *svc,
        axis2_env_t **env);

axis2_woden_configurable_t *AXIS2_CALL
axis2_woden_svc_get_base_impl(
        void *svc,
        axis2_env_t **env);
/* ************************************************************
 *  Service interface methods (the WSDL Component model)
 * ************************************************************/

axis2_qname_t *AXIS2_CALL
axis2_woden_svc_get_qname(
        void *svc,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_svc_get_interface(
        void *svc,
        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_woden_svc_get_endpoints(
        void *svc,
        axis2_env_t **env);
/* ************************************************************
 *  Service Element interface methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_svc_set_qname(
        void *svc,
        axis2_env_t **env,
        woden_nc_name_t *qname);

axis2_status_t AXIS2_CALL
axis2_woden_svc_set_interface_qname(
        void *svc,
        axis2_env_t **env,
        axis2_qname_t *interface_qname);

axis2_qname_t *AXIS2_CALL
axis2_woden_svc_get_interface_qname(
        void *svc,
        axis2_env_t **env);

void *AXIS2_CALL
axis2_woden_svc_get_interface_element(
        void *svc,
        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_woden_svc_add_endpoint_element(
        void *svc,
        axis2_env_t **env,
        void *endpoint);

axis2_array_list_t *AXIS2_CALL
axis2_woden_svc_get_endpoint_elements(
        void *svc,
        axis2_env_t **env);

/******************************************************************************
 *  Non-API implementation methods
 *****************************************************************************/  

axis2_status_t AXIS2_CALL
axis2_woden_svc_set_interface_element(
        void *svc,
        axis2_env_t **env,
        void *interface);


static axis2_woden_svc_t *
create(axis2_env_t **env);

/************************Woden C Internal Methods******************************/
AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_svc_element(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!svc)
    {
        svc_impl = (axis2_woden_svc_impl_t *) create(env);
    }
    else
        svc_impl = (axis2_woden_svc_impl_t *) svc;

    svc_impl->svc.base.svc_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_svc_element_ops_t));
    axis2_woden_svc_element_resolve_methods(&(svc_impl->svc.base.
            svc_element), env, svc_impl->methods);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_to_svc_element_free(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->svc.base.svc_element.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                svc_element.ops);
        svc_impl->svc.base.svc_element.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_nested_element(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!svc)
    {
        svc_impl = (axis2_woden_svc_impl_t *) create(env);
    }
    else
        svc_impl = (axis2_woden_svc_impl_t *) svc;

    svc_impl->svc.base.svc_element.base.nested_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_element_ops_t));
    axis2_woden_nested_element_resolve_methods(&(svc_impl->svc.base.
            svc_element.base.nested_element), env, svc_impl->methods);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_to_nested_element_free(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->svc.base.svc_element.base.nested_element.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                svc_element.base.nested_element.ops);
        svc_impl->svc.base.svc_element.base.nested_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_documentable_element(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!svc)
    {
        svc_impl = (axis2_woden_svc_impl_t *) create(env);
    }
    else
        svc_impl = (axis2_woden_svc_impl_t *) svc;

    svc_impl->svc.base.svc_element.base.documentable_element.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_documentable_element_ops_t));
    axis2_woden_documentable_element_resolve_methods(&(svc_impl->svc.base.
            svc_element.base.documentable_element), env, 
            svc_impl->methods);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_to_documentable_element_free(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->svc.base.svc_element.base.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                svc_element.base.documentable_element.ops);
        svc_impl->svc.base.svc_element.base.documentable_element.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_configurable(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!svc)
    {
        svc_impl = (axis2_woden_svc_impl_t *) create(env);
    }
    else
        svc_impl = (axis2_woden_svc_impl_t *) svc;

    svc_impl->svc.base.configurable.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_configurable_ops_t));
    axis2_woden_configurable_resolve_methods(&(svc_impl->svc.base.
            configurable), env, svc_impl->configurable, 
            svc_impl->methods);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_to_configurable_free(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->svc.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable.ops);
        svc_impl->svc.base.configurable.ops = 
            NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_nested_component(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!svc)
    {
        svc_impl = (axis2_woden_svc_impl_t *) create(env);
    }
    else
        svc_impl = (axis2_woden_svc_impl_t *) svc;
  
    svc_impl->svc.base.configurable.base.nested_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_nested_component_ops_t));
    axis2_woden_nested_component_resolve_methods(&(svc_impl->svc.base.
            configurable.base.nested_component), env, svc_impl->methods);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_to_nested_component_free(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->svc.base.configurable.base.nested_component.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable.base.nested_component.ops);
        svc_impl->svc.base.configurable.base.nested_component.
            ops = NULL;
    }
    return AXIS2_SUCCESS;
}

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_configurable_component(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!svc)
    {
        svc_impl = (axis2_woden_svc_impl_t *) create(env);
    }
    else
        svc_impl = (axis2_woden_svc_impl_t *) svc;
  
    svc_impl->svc.base.configurable_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
                sizeof(axis2_woden_configurable_component_ops_t));
    axis2_woden_configurable_component_resolve_methods(&(svc_impl->svc.base.
            configurable_component), env, svc_impl->methods);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_to_configurable_component_free(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->svc.base.configurable_component.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable_component.ops);
        svc_impl->svc.base.configurable_component.ops = NULL;
    }
    return AXIS2_SUCCESS;
}


AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_to_wsdl_component(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!svc)
    {
        svc_impl = (axis2_woden_svc_impl_t *) create(env);
    }
    else
        svc_impl = (axis2_woden_svc_impl_t *) svc;
    svc_impl->svc.base.configurable_component.wsdl_component.ops = 
        AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_woden_wsdl_component_ops_t));
    axis2_woden_wsdl_component_resolve_methods(&(svc_impl->svc.base.
            configurable_component.wsdl_component), env, svc_impl->methods);
    return svc;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_to_wsdl_component_free(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->svc.base.configurable_component.wsdl_component.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable_component.wsdl_component.ops);
        svc_impl->svc.base.configurable_component.wsdl_component.ops = NULL;
    }
    return AXIS2_SUCCESS;
}

/************************End of Woden C Internal Methods***********************/
static axis2_woden_svc_t *
create(axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = AXIS2_MALLOC((*env)->allocator, 
                    sizeof(axis2_woden_svc_impl_t));

    svc_impl->obj_type= AXIS2_WODEN_SVC;
    svc_impl->super = NULL;
    svc_impl->methods = NULL;
    svc_impl->f_qname = NULL;
    svc_impl->f_interface_qname = NULL;
    svc_impl->f_interface = NULL;
    svc_impl->f_endpoints = NULL;
    
    svc_impl->svc.base.svc_element.ops = NULL;
    svc_impl->svc.base.svc_element.base.nested_element.ops = NULL;
    svc_impl->svc.base.svc_element.base.documentable_element.ops = 
            NULL;
    svc_impl->svc.base.configurable.ops = NULL;
    svc_impl->svc.base.configurable.base.configurable.ops = NULL;
    svc_impl->svc.base.configurable.base.nested_component.ops = NULL; 
    svc_impl->svc.base.configurable_component.ops = NULL;
    svc_impl->svc.base.configurable_component.wsdl_component.ops = NULL;
    
    svc_impl->svc.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_woden_svc_ops_t));

    svc_impl->svc.ops->free = axis2_woden_svc_free;
    svc_impl->svc.ops->super_objs = axis2_woden_svc_super_objs;
    svc_impl->svc.ops->type = axis2_woden_svc_type;
    svc_impl->svc.ops->get_base_impl = axis2_woden_svc_get_base_impl;

    svc_impl->svc.ops->get_qname = axis2_woden_svc_get_qname;
    svc_impl->svc.ops->get_interface = axis2_woden_svc_get_interface;
    svc_impl->svc.ops->get_endpoints = 
        axis2_woden_svc_get_endpoints;
    svc_impl->svc.ops->set_interface_element = 
        axis2_woden_svc_set_interface_element;
 
    svc_impl->methods = axis2_hash_make(env);
    if(!svc_impl->methods) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(svc_impl->methods, "free", AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_free);
    axis2_hash_set(svc_impl->methods, "to_svc_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_svc_element_free);
    axis2_hash_set(svc_impl->methods, "to_nested_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_nested_element_free);
    axis2_hash_set(svc_impl->methods, "to_documentable_element_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_documentable_element_free);
    axis2_hash_set(svc_impl->methods, "to_configurable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_configurable_free);
    axis2_hash_set(svc_impl->methods, "to_configurable_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_configurable_free);
    axis2_hash_set(svc_impl->methods, "to_nested_component_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_nested_component_free);
    axis2_hash_set(svc_impl->methods, "to_configurable_component_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_configurable_component_free);
    axis2_hash_set(svc_impl->methods, "to_wsdl_component_free", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_to_wsdl_component_free);
    axis2_hash_set(svc_impl->methods, "super_objs", 
            AXIS2_HASH_KEY_STRING, axis2_woden_svc_super_objs);
    axis2_hash_set(svc_impl->methods, "type", 
            AXIS2_HASH_KEY_STRING, axis2_woden_svc_type);

    axis2_hash_set(svc_impl->methods, "get_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_get_qname);
    axis2_hash_set(svc_impl->methods, "get_interface", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_get_interface);
    axis2_hash_set(svc_impl->methods, "get_endpoints", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_get_endpoints);
    axis2_hash_set(svc_impl->methods, "set_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_set_qname);
    axis2_hash_set(svc_impl->methods, "set_interface_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_set_interface_qname);
    axis2_hash_set(svc_impl->methods, "get_interface_qname", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_get_interface_qname);
    axis2_hash_set(svc_impl->methods, "get_interface_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_get_interface_element);
    axis2_hash_set(svc_impl->methods, "add_endpoint_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_add_enpoint_element);
    axis2_hash_set(svc_impl->methods, "get_endpoint_elements", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_get_endpoint_elements);
    axis2_hash_set(svc_impl->methods, "set_interface_element", 
            AXIS2_HASH_KEY_STRING, 
            axis2_woden_svc_set_interface_element);

    return &(svc_impl->svc);
}

AXIS2_DECLARE(axis2_woden_svc_t *)
axis2_woden_svc_create(axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = (axis2_woden_svc_impl_t *) create(env);

    svc_impl->configurable = axis2_woden_configurable_create(env);

    svc_impl->super = axis2_hash_make(env);
    if(!svc_impl->super) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    axis2_hash_set(svc_impl->super, "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING, 
            &(svc_impl->svc));
    axis2_hash_set(svc_impl->super, "AXIS2_WODEN_NESTED_CONFIGURABLE", AXIS2_HASH_KEY_STRING, 
            svc_impl->configurable);
 
    return &(svc_impl->svc);
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_free(void *svc,
                        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    if(svc_impl->f_qname)
    {
        WODEN_QNAME_FREE(svc_impl->f_qname, env);
        svc_impl->f_qname = NULL;
    }

    if(svc_impl->f_interface_qname)
    {
        WODEN_QNAME_FREE(svc_impl->f_interface_qname, env);
        svc_impl->f_interface_qname = NULL;
    }

    if(svc_impl->f_interface)
    {
        AXIS2_WODEN_INTERFACE_FREE(svc_impl->f_interface, env);
        svc_impl->f_interface = NULL;
    }

    if(svc_impl->f_endpoints)
    {
        AXIS2_ARRAY_LIST_FREE(svc_impl->f_endpoints, env);
        svc_impl->f_endpoints = NULL;
    }
   
    /* TODO free f_parent */
    
    if(svc_impl->super)
    {
        axis2_hash_free(svc_impl->super, env);
        svc_impl->super = NULL;
    }
    
    if(svc_impl->methods)
    {
        axis2_hash_free(svc_impl->methods, env);
        svc_impl->methods = NULL;
    }

    if(svc_impl->configurable)
    {
        AXIS2_WODEN_NESTED_CONFIGURABLE_FREE(svc_impl->configurable, env);
        svc_impl->configurable = NULL;
    }

    if(svc_impl->svc.base.svc_element.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                svc_element.ops);
        svc_impl->svc.base.svc_element.ops = NULL;
    }

    if(svc_impl->svc.base.svc_element.base.nested_element.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                svc_element.base.nested_element.ops);
        svc_impl->svc.base.svc_element.base.nested_element.ops = 
            NULL;
    }
    
    if(svc_impl->svc.base.svc_element.base.documentable_element.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                svc_element.base.documentable_element.ops);
        svc_impl->svc.base.svc_element.base.documentable_element.ops = 
            NULL;
    }
    
    if(svc_impl->svc.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable.ops);
        svc_impl->svc.base.configurable.ops = 
            NULL;
    }
    
    if(svc_impl->svc.base.configurable.base.configurable.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable.base.configurable.ops);
        svc_impl->svc.base.configurable.base.configurable.ops = 
            NULL;
    }

    if(svc_impl->svc.base.configurable.base.nested_component.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable.base.nested_component.ops);
        svc_impl->svc.base.configurable.base.nested_component.ops = NULL;
    }

    if(svc_impl->svc.base.configurable_component.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable_component.ops);
        svc_impl->svc.base.configurable_component.ops = NULL;
    }

    if(svc_impl->svc.base.configurable_component.wsdl_component.ops)
    {
        AXIS2_FREE((*env)->allocator, svc_impl->svc.base.
                configurable_component.wsdl_component.ops);
        svc_impl->svc.base.configurable_component.wsdl_component.ops = NULL;
    }

    if((&(svc_impl->svc))->ops)
    {
        AXIS2_FREE((*env)->allocator, (&(svc_impl->svc))->ops);
        (&(svc_impl->svc))->ops = NULL;
    }
    
    if(svc_impl)
    {
        AXIS2_FREE((*env)->allocator, svc_impl);
        svc_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_woden_svc_super_objs(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    return svc_impl->super;
}

axis2_woden_obj_types_t AXIS2_CALL
axis2_woden_svc_type(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    svc_impl = INTF_TO_IMPL(svc);

    return svc_impl->obj_type;
}

axis2_woden_configurable_t *AXIS2_CALL
axis2_woden_svc_get_base_impl(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    svc_impl = INTF_TO_IMPL(svc);

    return svc_impl->configurable;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_resolve_methods(
        axis2_woden_svc_t *svc,
        axis2_env_t **env,
        axis2_woden_svc_t *svc_impl,
        axis2_hash_t *methods)
{
    axis2_woden_svc_impl_t *svc_impl_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, methods, AXIS2_FAILURE);
    svc_impl_l = INTF_TO_IMPL(svc_impl);
    
    svc->ops->free = axis2_hash_get(methods, "free", 
            AXIS2_HASH_KEY_STRING);
    svc->ops->to_svc_free = axis2_hash_get(methods, "to_svc_free", 
            AXIS2_HASH_KEY_STRING);
    svc->ops->super_objs = axis2_hash_get(methods, "super_objs", 
            AXIS2_HASH_KEY_STRING);
    svc->ops->type = axis2_hash_get(methods, "type", 
            AXIS2_HASH_KEY_STRING);
    
    svc->ops->get_qname = axis2_hash_get(methods, 
            "get_qname", AXIS2_HASH_KEY_STRING);
    if(!svc->ops->get_qname && svc_impl_l)
            svc->ops->get_qname = 
            svc_impl_l->svc.ops->get_qname;
    
    svc->ops->get_interface = axis2_hash_get(methods, 
            "get_interface", AXIS2_HASH_KEY_STRING);
    if(!svc->ops->get_interface && svc_impl_l)
            svc->ops->get_interface = 
            svc_impl_l->svc.ops->get_interface;
    
    svc->ops->get_endpoints = axis2_hash_get(methods, 
            "get_endpoints", AXIS2_HASH_KEY_STRING);
    if(!svc->ops->get_endpoints && svc_impl_l)
            svc->ops->get_endpoints = 
            svc_impl_l->svc.ops->get_endpoints;
    
    svc->ops->set_interface_element = axis2_hash_get(methods, 
            "set_interface_element", AXIS2_HASH_KEY_STRING);
    if(!svc->ops->set_interface_element && svc_impl_l)
            svc->ops->set_interface_element = 
            svc_impl_l->svc.ops->set_interface_element;

    return AXIS2_SUCCESS;
}
/* ************************************************************
 *  Service interface methods (the WSDL Component model)
 * ************************************************************/

woden_qname_t *AXIS2_CALL
axis2_woden_svc_get_qname(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));
    
    return svc_impl->f_qname;
}

void *AXIS2_CALL
axis2_woden_svc_get_interface(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));
    
    return svc_impl->f_interface;
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_svc_get_endpoints(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));
    
    return svc_impl->f_endpoints;
}
/* ************************************************************
 *  Service Element interface methods (the XML Element model)
 * ************************************************************/

axis2_status_t AXIS2_CALL
axis2_woden_svc_set_qname(
        void *svc,
        axis2_env_t **env,
        woden_nc_name_t *qname)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));

    if(svc_impl->f_qname)
    {
        AXIS2_QNAME_FREE(svc_impl->f_qname, env);
    }
    svc_impl->f_qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_set_interface_qname(
        void *svc,
        axis2_env_t **env,
        axis2_qname_t *interface_qname)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));

    if(svc_impl->f_interface_qname)
    {
        AXIS2_QNAME_FREE(svc_impl->f_interface_qname, env);
    }

    svc_impl->f_interface_qname = AXIS2_QNAME_CLONE(interface_qname, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_woden_svc_get_interface_qname(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));

    return svc_impl->f_interface_qname;
}

void *AXIS2_CALL
axis2_woden_svc_get_interface_element(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));

    return svc_impl->f_interface;
}

axis2_status_t AXIS2_CALL
axis2_woden_svc_add_endpoint_element(
        void *svc,
        axis2_env_t **env,
        void *endpoint)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));

    if(!svc_impl->f_endpoints)
    {
        svc_impl->f_endpoints = axis2_array_list_create(env, 0);
        if(!svc_impl->f_endpoints)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }

    return AXIS2_ARRAY_LIST_ADD(svc_impl->f_endpoints, env, endpoint);
}

axis2_array_list_t *AXIS2_CALL
axis2_woden_svc_get_endpoint_elements(
        void *svc,
        axis2_env_t **env)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));
    
    return svc_impl->f_endpoints;
}
/******************************************************************************
 *  Non-API implementation methods
 *****************************************************************************/  

axis2_status_t AXIS2_CALL
axis2_woden_svc_set_interface_element(
        void *svc,
        axis2_env_t **env,
        void *interface)
{
    axis2_woden_svc_impl_t *svc_impl = NULL;
    axis2_hash_t *super = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    super = AXIS2_WODEN_SVC_SUPER_OBJS(svc, env);
    svc_impl = INTF_TO_IMPL(axis2_hash_get(super, 
                "AXIS2_WODEN_SVC", AXIS2_HASH_KEY_STRING));

    if(svc_impl->f_interface)
    {
        /* TODO */
    }

    svc_impl->f_interface = interface;
    return AXIS2_SUCCESS;
}


