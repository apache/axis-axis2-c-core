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
     * Field ops
     */
    axis2_hash_t *ops;

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
axis2_wsdl_interface_get_defined_ops(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_interface_get_faults(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_interface_get_name(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env);

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_ops(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env);

void *AXIS2_CALL
axis2_wsdl_interface_get_op(axis2_wsdl_interface_t *wsdl_interface,
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
axis2_wsdl_interface_set_ops(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_hash_t *list);

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_op(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    void *op);

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
    axis2_wsdl_interface_impl_t *wsdl_interface_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	wsdl_interface_impl = (axis2_wsdl_interface_impl_t *) AXIS2_MALLOC ((*env)->
        allocator, sizeof (axis2_wsdl_interface_impl_t));
     
	if(NULL == wsdl_interface_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    wsdl_interface_impl->wsdl_interface.extensible_component = NULL;
    wsdl_interface_impl->qname = NULL;
	wsdl_interface_impl->style_default = NULL;
    wsdl_interface_impl->faults = NULL;
    wsdl_interface_impl->super_interfaces = NULL;
    wsdl_interface_impl->ops = NULL;
    wsdl_interface_impl->wsdl_interface.ops = NULL;
    
    wsdl_interface_impl->wsdl_interface.extensible_component = 
        axis2_wsdl_extensible_component_create(env);
    
	if(NULL == wsdl_interface_impl->wsdl_interface.extensible_component)
	{
        axis2_wsdl_interface_free(&(wsdl_interface_impl->wsdl_interface), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
	}    
    
    wsdl_interface_impl->faults = axis2_linked_list_create(env);		
	if(NULL == wsdl_interface_impl->faults)
	{
        axis2_wsdl_interface_free(&(wsdl_interface_impl->wsdl_interface), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;		
	}

	wsdl_interface_impl->super_interfaces = axis2_hash_make(env);		
	if(NULL == wsdl_interface_impl->super_interfaces)
	{
        axis2_wsdl_interface_free(&(wsdl_interface_impl->wsdl_interface), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;		
	}
    
    wsdl_interface_impl->ops = axis2_hash_make(env);		
	if(NULL == wsdl_interface_impl->ops)
	{
        axis2_wsdl_interface_free(&(wsdl_interface_impl->wsdl_interface), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;		
	}  
    
    wsdl_interface_impl->wsdl_interface.ops = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_wsdl_interface_ops_t));
	if(NULL == wsdl_interface_impl->wsdl_interface.ops)
	{
        axis2_wsdl_interface_free(&(wsdl_interface_impl->wsdl_interface), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
	
	wsdl_interface_impl->wsdl_interface.ops->free = axis2_wsdl_interface_free;
    
	wsdl_interface_impl->wsdl_interface.ops->get_defined_ops = 
        axis2_wsdl_interface_get_defined_ops;
    
	wsdl_interface_impl->wsdl_interface.ops->get_faults = 
        axis2_wsdl_interface_get_faults;
    
	wsdl_interface_impl->wsdl_interface.ops->get_name = 
        axis2_wsdl_interface_get_name;
    
	wsdl_interface_impl->wsdl_interface.ops->get_ops = 
        axis2_wsdl_interface_get_ops;
    
    wsdl_interface_impl->wsdl_interface.ops->get_op =
        axis2_wsdl_interface_get_op;
    
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
    
	wsdl_interface_impl->wsdl_interface.ops->set_ops = 
        axis2_wsdl_interface_set_ops;
    
	wsdl_interface_impl->wsdl_interface.ops->set_op = 
        axis2_wsdl_interface_set_op;
    
    wsdl_interface_impl->wsdl_interface.ops->set_super_interfaces = 
        axis2_wsdl_interface_set_super_interfaces;
        
    wsdl_interface_impl->wsdl_interface.ops->add_super_interface = 
        axis2_wsdl_interface_add_super_interface;
        
    wsdl_interface_impl->wsdl_interface.ops->get_style_default = 
        axis2_wsdl_interface_get_style_default;
        
    wsdl_interface_impl->wsdl_interface.ops->set_style_default = 
        axis2_wsdl_interface_set_style_default;
						
	return &(wsdl_interface_impl->wsdl_interface);
}

/*************************Function implementations*****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_interface_free (
                        axis2_wsdl_interface_t *wsdl_interface, 
                        axis2_env_t **env)
{
    axis2_wsdl_interface_impl_t *wsdl_interface_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    wsdl_interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    
	if(NULL != wsdl_interface->ops)
    {
        AXIS2_FREE((*env)->allocator, wsdl_interface->ops);
        wsdl_interface->ops = NULL;
    }
    
    if(NULL != wsdl_interface_impl->super_interfaces)
    {
        axis2_hash_free(wsdl_interface_impl->super_interfaces, env);
        wsdl_interface_impl->super_interfaces = NULL;
    }
    
    if(NULL != wsdl_interface_impl->ops)
    {
        /*axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (wsdl_interface_impl->ops, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_hash_this (hi, NULL, NULL, &val);
            AXIS2_WSDL_OP_FREE(val, env);
            val = NULL;
               
        }*/
        axis2_hash_free(wsdl_interface_impl->ops, env);
        wsdl_interface_impl->ops = NULL;
    }
    
    if(NULL != wsdl_interface_impl->faults)
    {
        AXIS2_LINKED_LIST_FREE(wsdl_interface_impl->faults, env);
        wsdl_interface_impl->faults = NULL;
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_interface)->qname)
    {
        AXIS2_QNAME_FREE(wsdl_interface_impl->qname, env);
        wsdl_interface_impl->qname = NULL;
    }
    
    if(NULL != wsdl_interface_impl->style_default)
    {
        AXIS2_FREE((*env)->allocator, wsdl_interface_impl->style_default);
        wsdl_interface_impl->style_default = NULL;
    }
    
    if(NULL != wsdl_interface->extensible_component)
    {
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(wsdl_interface->
            extensible_component, env);
        wsdl_interface->extensible_component = NULL;
    }
    
    if(wsdl_interface_impl)
        AXIS2_FREE((*env)->allocator, wsdl_interface_impl);
    wsdl_interface_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_defined_ops(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->ops;
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_interface_get_faults(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->faults;
}

axis2_hash_t *AXIS2_CALL
axis2_wsdl_interface_get_ops(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->ops;
}

void *AXIS2_CALL
axis2_wsdl_interface_get_op(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_char_t *nc_name) 
{
    return axis2_hash_get(AXIS2_INTF_TO_IMPL(
        wsdl_interface)->ops, nc_name, AXIS2_HASH_KEY_STRING);
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
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);

    name = AXIS2_QNAME_TO_STRING(qname, env);
    return (axis2_wsdl_interface_t *) axis2_hash_get(
        interface_impl->super_interfaces, name, AXIS2_HASH_KEY_STRING);
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
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    
    if(interface_impl->faults)
    {
        AXIS2_LINKED_LIST_FREE(interface_impl->faults, env);
        interface_impl->faults = NULL;
    }
    interface_impl->faults = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_name(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env,
                                axis2_qname_t *qName) 
{
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qName, AXIS2_FAILURE);
    
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    
    if(interface_impl->qname)
    {
        AXIS2_QNAME_FREE(interface_impl->qname, env);
        interface_impl->qname = NULL;
    }
    interface_impl->qname = qName;
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_interface_get_name(axis2_wsdl_interface_t *wsdl_interface,
                                axis2_env_t **env) 
{
    return AXIS2_INTF_TO_IMPL(wsdl_interface)->qname;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_ops(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_hash_t *list) 
{
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    if(interface_impl->ops)
    {
        axis2_hash_free(interface_impl->ops, env);
        interface_impl->ops = NULL;
    }
    interface_impl->ops = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_op(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    void *op) 
{
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    axis2_qname_t *wsdl_op_name = NULL;
    axis2_char_t *op_name = NULL;
        
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op, AXIS2_FAILURE); 
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    
    wsdl_op_name = AXIS2_WSDL_OP_GET_QNAME(op, env);    
    if (!wsdl_op_name) 
    {
        /* The Operation name cannot be null (required) */
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_WSDL_OP, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    op_name = AXIS2_QNAME_GET_LOCALPART(wsdl_op_name, env);
    
    axis2_hash_set(interface_impl->ops, op_name, AXIS2_HASH_KEY_STRING, op);
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_set_super_interfaces(axis2_wsdl_interface_t *wsdl_interface,
                                            axis2_env_t **env,
                                            axis2_hash_t *list) 
{
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    
    if(interface_impl->super_interfaces)
    {
        axis2_hash_free(interface_impl->super_interfaces, env);
        interface_impl->super_interfaces = NULL;
    }
    interface_impl->super_interfaces = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_interface_add_super_interface(axis2_wsdl_interface_t *wsdl_interface,
                                    axis2_env_t **env,
                                    axis2_wsdl_interface_t *interface_component) 
{
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, interface_component, AXIS2_FAILURE);
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    
    qname = axis2_wsdl_interface_get_name(interface_component, env);
    if(!qname)
    {
        return AXIS2_FAILURE;
    }

    name = AXIS2_QNAME_TO_STRING(qname, env);
    axis2_hash_set(interface_impl->super_interfaces, name,
        AXIS2_HASH_KEY_STRING, interface_component);
    
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
    axis2_wsdl_interface_impl_t *interface_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style_default, AXIS2_FAILURE);
    
    interface_impl = AXIS2_INTF_TO_IMPL(wsdl_interface);
    
    if(interface_impl->style_default)
    {
        AXIS2_FREE((*env)->allocator, interface_impl->style_default);
        interface_impl->style_default = NULL;
    }
    interface_impl->style_default = style_default;
    return AXIS2_SUCCESS;
}
