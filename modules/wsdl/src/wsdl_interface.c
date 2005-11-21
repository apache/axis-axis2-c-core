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
 
#include <axis2_wsdl_interface.h>

/** 
 * @brief Wsdl interface struct impl
 *	Axis2 Wsdl Interface Implementation 
 */ 
typedef struct axis2_wsdl_interface_impl
{
	axis2_wsdl_interface_t wsdl_interface;
    
	/**
     * Field name
     */
    axis2_qname_t  *qname;

    /**
     * Field superInterfaces
     */
    axis2_hash_t *super_interfaces;

    /**
     * Field faults
     */
    axis2_linked_list_t *faults;

    /**
     * Field operations
     */
    axis2_hash_t *operations;

    /**
     * Field style_default
     */
    axis2_char_t *style_default;
    
} axis2_wsdl_interface_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_interface) ((axis2_wsdl_interface_impl_t *)wsdl_interface)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_free (axis2_wsdl_interface_t *wsdl_interface, 
                        axis2_env_t **env);	

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_defined_operations(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_interface_get_faults(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_interface_get_name(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env);

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_operations(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env);

struct axis2_operation *AXIS2_CALL
axis2_interface_get_operation(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_char_t *nc_name);

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_super_interfaces(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env);

axis2_wsdl_interface_t *AXIS2_CALL
axis2_wsdl_interface_get_super_interface(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);

axis2_char_t *AXIS2_CALL
axis2_wsdl_interface_get_target_namespace(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_faults(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_linked_list_t *list);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_name(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_qname_t *qName);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_operations(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_hash_t *list);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_operation(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    struct axis2_operation *operation);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_super_interfaces(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_hash_t *list);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_add_super_interface(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_wsdl_interface_t *interface_component);

axis2_char_t *AXIS2_CALL
axis2_wsdl_interface_get_style_default(axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_style_default(axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env,
                                        axis2_char_t *style_default);

/************************* End of function headers ****************************/	

axis2_wsdl_interface_t * AXIS2_CALL
axis2_wsdl_interface_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	axis2_wsdl_interface_impl_t *wsdl_interface_impl = 
		(axis2_wsdl_interface_impl_t *) AXIS2_MALLOC ((*env)->allocator,
		sizeof (axis2_wsdl_interface_impl_t));
     
	if(NULL == wsdl_interface_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    wsdl_interface_impl->faults = axis2_linked_list_create(env);		
	if(NULL == wsdl_interface_impl->faults)
	{
        AXIS2_FREE((*env)->allocator, wsdl_interface_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	wsdl_interface_impl->super_interfaces = axis2_hash_make(env);		
	if(NULL == wsdl_interface_impl->super_interfaces)
	{
        AXIS2_LINKED_LIST_FREE(wsdl_interface_impl->faults, env);
        AXIS2_FREE((*env)->allocator, wsdl_interface_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
    
    wsdl_interface_impl->operations = axis2_hash_make(env);		
	if(NULL == wsdl_interface_impl->operations)
	{
        axis2_hash_free(wsdl_interface_impl->super_interfaces, env);
        AXIS2_FREE((*env)->allocator, wsdl_interface_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}   
    
    wsdl_interface_impl->wsdl_interface.extensible_component = 
        axis2_wsdl_extensible_component_create(env);
    
	if(NULL == wsdl_interface_impl->wsdl_interface.extensible_component)
	{
        axis2_hash_free(wsdl_interface_impl->operations, env);
        axis2_hash_free(wsdl_interface_impl->super_interfaces, env);
        AXIS2_FREE((*env)->allocator, wsdl_interface_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
    wsdl_interface_impl->wsdl_interface.ops = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_wsdl_interface_ops_t));
	if(NULL == wsdl_interface_impl->wsdl_interface.ops)
	{
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(wsdl_interface_impl->wsdl_interface.
            extensible_component, env);
        axis2_hash_free(wsdl_interface_impl->super_interfaces, env);
        axis2_hash_free(wsdl_interface_impl->operations, env);
        AXIS2_LINKED_LIST_FREE(wsdl_interface_impl->faults, env);
        AXIS2_FREE((*env)->allocator, wsdl_interface_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	wsdl_interface_impl->wsdl_interface.ops->free = axis2_wsdl_interface_free;
    
	wsdl_interface_impl->wsdl_interface.ops->get_defined_operations = 
        axis2_wsdl_interface_get_defined_operations;
    
	wsdl_interface_impl->wsdl_interface.ops->get_faults = 
        axis2_wsdl_interface_get_faults;
    
	wsdl_interface_impl->wsdl_interface.ops->get_name = 
        axis2_wsdl_interface_get_name;
    
	wsdl_interface_impl->wsdl_interface.ops->get_operations = 
        axis2_wsdl_interface_get_operations;
    
	wsdl_interface_impl->wsdl_interface.ops->get_super_interfaces = 
        axis2_wsdl_interface_get_super_interfaces;
  
	wsdl_interface_impl->wsdl_interface.ops->get_super_interface = 
        axis2_wsdl_interface_get_super_interface;
    
	wsdl_interface_impl->wsdl_interface.ops->get_target_namespace = 
        axis2_wsdl_interface_get_target_namespace;
    
    wsdl_interface_impl->wsdl_interface.ops->set_faults = 
        axis2_wsdl_interface_set_faults;
        
	wsdl_interface_impl->wsdl_interface.ops->set_name = 
        axis2_wsdl_interface_set_name;
    
	wsdl_interface_impl->wsdl_interface.ops->set_operations = 
        axis2_wsdl_interface_set_operations;
    
	wsdl_interface_impl->wsdl_interface.ops->set_operation = 
        axis2_wsdl_interface_set_operation;
    
    wsdl_interface_impl->wsdl_interface.ops->set_super_interfaces = 
        axis2_wsdl_interface_set_super_interfaces;
        
    wsdl_interface_impl->wsdl_interface.ops->add_super_interface = 
        axis2_wsdl_interface_add_super_interface;
        
    wsdl_interface_impl->wsdl_interface.ops->get_style_default = 
        axis2_wsdl_interface_get_style_default;
        
    wsdl_interface_impl->wsdl_interface.ops->set_style_default = 
        axis2_wsdl_interface_set_style_default;
	
	wsdl_interface_impl->qname = NULL;
	
	wsdl_interface_impl->style_default = NULL;
						
	return &(wsdl_interface_impl->wsdl_interface);
}

/*************************Function implementations*****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_interface_free (
                        axis2_wsdl_interface_t *wsdl_interface, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_interface, env, AXIS2_FAILURE);
	if(NULL != wsdl_interface->ops)
        AXIS2_FREE((*env)->allocator, wsdl_interface->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_interface)->super_interfaces)
    {
        axis2_hash_free(AXIS2_INTF_TO_IMPL(wsdl_interface)->
            super_interfaces, env);
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_interface)->operations)
    {
        axis2_hash_free(AXIS2_INTF_TO_IMPL(wsdl_interface)->
            operations, env);
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_interface)->faults)
    {
        AXIS2_LINKED_LIST_FREE(AXIS2_INTF_TO_IMPL(wsdl_interface)->
            faults, env);
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_interface)->qname)
    {
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(wsdl_interface)->qname, env);
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_interface)->style_default)
    {
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_interface)->
            style_default);
    }
    
    if(NULL != wsdl_interface->extensible_component)
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(wsdl_interface->
            extensible_component, env);
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_interface));
    
	return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_defined_operations(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->operations;
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_interface_get_faults(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->faults;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_interface_get_name(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->qname;
}

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_operations(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->operations;
}

struct axis2_operation *AXIS2_CALL
axis2_interface_get_operation(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_char_t *nc_name) 
{
    return (struct axis2_operation *) axis2_hash_get(AXIS2_INTF_TO_IMPL(
        wsdl_interface)->operations, nc_name, AXIS2_HASH_KEY_STRING);
}

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_super_interfaces(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->super_interfaces;
}

axis2_wsdl_interface_t *AXIS2_CALL
axis2_wsdl_interface_get_super_interface(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname)
{
    return (axis2_wsdl_interface_t *) axis2_hash_get(AXIS2_INTF_TO_IMPL(
        wsdl_interface)->super_interfaces, qname, sizeof(axis2_qname_t));
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_interface_get_target_namespace(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env) 
{
    return AXIS2_QNAME_GET_URI(AXIS2_INTF_TO_IMPL(wsdl_interface)->qname, env);
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_faults(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_linked_list_t *list) 
{
    AXIS2_INTF_TO_IMPL(wsdl_interface)->faults = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_name(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_qname_t *qName) 
{
    AXIS2_INTF_TO_IMPL(wsdl_interface)->qname = qName;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_operations(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_hash_t *list) 
{
    AXIS2_INTF_TO_IMPL(wsdl_interface)->operations = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_operation(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    struct axis2_operation *operation) 
{
    
    if (NULL == AXIS2_WSDL_OPERATION_GET_NAME(operation->wsdl_operation, env)) 
    {
        /* The Operation name cannot be null (required) */
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_WSDL_OPERATION, 
            AXIS2_FAILURE);
    }
    axis2_char_t *op_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_WSDL_OPERATION_GET_NAME(
        operation->wsdl_operation, env), env);
    axis2_hash_set(AXIS2_INTF_TO_IMPL(wsdl_interface)->operations, op_name,  
        AXIS2_HASH_KEY_STRING, operation);
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_super_interfaces(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_hash_t *list) 
{
    AXIS2_INTF_TO_IMPL(wsdl_interface)->super_interfaces = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_add_super_interface(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_wsdl_interface_t *interface_component) 
{
    axis2_hash_set(AXIS2_INTF_TO_IMPL(wsdl_interface)->super_interfaces,
        axis2_wsdl_interface_get_name(interface_component, env),
        sizeof(axis2_qname_t), interface_component);
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_interface_get_style_default(axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->style_default;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_style_default(axis2_wsdl_interface_t *wsdl_interface,
                                        axis2_env_t **env,
                                        axis2_char_t *style_default) 
{
    
    AXIS2_INTF_TO_IMPL(wsdl_interface)->style_default = style_default;
    return AXIS2_SUCCESS;
}
