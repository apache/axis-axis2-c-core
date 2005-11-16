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
 
#include <axis2_operation.h>

/** 
 * @brief Operaton struct impl
 *	Axis2 Operations  
 */ 
typedef struct axis2_operation_impl_s
{
	axis2_operation_t operation;
	axis2_param_container_t *param_container;
    axis2_wsdl_operation_t *wsdl_operation;
	struct axis2_svc *parent;
	axis2_msg_recv_t *msg_recv;
} axis2_operation_impl_t;

#define AXIS2_INTF_TO_IMPL(operation) ((axis2_operation_impl_t *)operation)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_operation_free (axis2_operation_t *operation, 
                        axis2_env_t **env);	

axis2_status_t AXIS2_CALL 
axis2_operation_add_param (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_operation_get_param (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            const axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_operation_get_params (axis2_operation_t *operation, 
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_operation_is_param_locked(axis2_operation_t *operation, 
                                    axis2_env_t **env
    ,                               const axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_operation_set_parent (axis2_operation_t *operation, 
                                axis2_env_t **env,
                                axis2_svc_t *svc);

axis2_svc_t * AXIS2_CALL
axis2_operation_get_parent (axis2_operation_t *operation, 
                                axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_operation_set_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env,
                                axis2_msg_recv_t *msg_recv);

axis2_msg_recv_t * AXIS2_CALL
axis2_operation_get_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_name (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            axis2_qname_t *qname);
                            
axis2_qname_t * AXIS2_CALL
axis2_operation_get_name (axis2_operation_t *operation, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env,
                                            const axis2_char_t *pattern);
		
axis2_char_t * AXIS2_CALL
axis2_operation_get_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env);

axis2_hash_t * AXIS2_CALL
axis2_operation_get_component_properties(axis2_operation_t *operation,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_component_properties(axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        axis2_hash_t *properties);

axis2_wsdl_component_t * AXIS2_CALL
axis2_operation_get_component_property(axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        const axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_operation_set_component_property (axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        const void *key,
                                        void *value);

axis2_char_t * AXIS2_CALL
axis2_operation_get_style (axis2_operation_t *operation,
                            axis2_env_t **env);

axis2_status_t  AXIS2_CALL
axis2_operation_set_style (axis2_operation_t *operation,
                            axis2_env_t **env,
                            axis2_char_t *style);

/************************* End of function headers ****************************/	

axis2_operation_t * AXIS2_CALL
axis2_operation_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	axis2_operation_impl_t *operation_impl = 
		(axis2_operation_impl_t *) AXIS2_MALLOC ((*env)->allocator,
		sizeof (axis2_operation_impl_t));
     
	if(NULL == operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    operation_impl->param_container = (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == operation_impl->param_container)
	{
        AXIS2_FREE((*env)->allocator, operation_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	operation_impl->wsdl_operation = (axis2_wsdl_operation_t *)
		axis2_wsdl_operation_create(env);		
	if(NULL == operation_impl->wsdl_operation)
	{
        AXIS2_PARAM_CONTAINER_FREE(operation_impl->param_container, env);
        AXIS2_FREE((*env)->allocator, operation_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

    AXIS2_WSDL_OPERATION_SET_MSG_EXCHANGE_PATTERN(operation_impl->wsdl_operation,
        env, MEP_URI_IN_OUT);    
    
    operation_impl->operation.ops = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_operation_ops_t));
	if(NULL == operation_impl->operation.ops)
	{
        AXIS2_PARAM_CONTAINER_FREE(operation_impl->param_container, env);
        AXIS2_WSDL_OPERATION_FREE(operation_impl->wsdl_operation, env);
        AXIS2_FREE((*env)->allocator, operation_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	operation_impl->operation.ops->free = axis2_operation_free;
	operation_impl->operation.ops->add_param = axis2_operation_add_param;
	operation_impl->operation.ops->get_param = axis2_operation_get_param;
	operation_impl->operation.ops->get_params = axis2_operation_get_params;
	operation_impl->operation.ops->set_parent = axis2_operation_set_parent;
	operation_impl->operation.ops->get_parent = axis2_operation_get_parent;
  
	operation_impl->operation.ops->set_msg_recv = axis2_operation_set_msg_recv;
	operation_impl->operation.ops->get_msg_recv = axis2_operation_get_msg_recv;
    
    operation_impl->operation.ops->set_name = axis2_operation_set_name;
	operation_impl->operation.ops->get_name = axis2_operation_get_name;
    
	operation_impl->operation.ops->set_msg_exchange_pattern 
		= axis2_operation_set_msg_exchange_pattern;
    
	operation_impl->operation.ops->get_msg_exchange_pattern
		= axis2_operation_get_msg_exchange_pattern;
    
    operation_impl->operation.ops->get_component_properties = 
        axis2_operation_get_component_properties;
        
    operation_impl->operation.ops->set_component_properties = 
        axis2_operation_set_component_properties;
        
    operation_impl->operation.ops->get_component_property = 
        axis2_operation_get_component_property;
        
    operation_impl->operation.ops->set_component_property = 
        axis2_operation_set_component_property;
        
    operation_impl->operation.ops->set_style = axis2_operation_set_style;
    
	operation_impl->operation.ops->get_style = axis2_operation_get_style; 
	
	operation_impl->parent = NULL;
	
	operation_impl->msg_recv = NULL;
						
	return &(operation_impl->operation);
}

axis2_operation_t * AXIS2_CALL
axis2_operation_create_with_name (axis2_env_t **env, axis2_qname_t *qname)
{
	axis2_operation_impl_t *operation_impl = 
        AXIS2_INTF_TO_IMPL(axis2_operation_create(env));
    
	if(NULL == operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);	
	axis2_operation_set_name(&(operation_impl->operation), env, qname);
       
	return &(operation_impl->operation);	
}

axis2_operation_t * AXIS2_CALL
axis2_operation_create_with_wsdl_operation (axis2_env_t **env, 
                                            axis2_wsdl_operation_t *wsdl_operation)
{
	axis2_operation_impl_t *operation_impl = 
        AXIS2_INTF_TO_IMPL(axis2_operation_create(env));
    
	if(NULL == operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    AXIS2_PARAM_CHECK((*env)->error, wsdl_operation, NULL);	
	
    operation_impl->wsdl_operation = wsdl_operation;
       
	return &(operation_impl->operation);	
}

/*************************** Start of operation impls *************************/

axis2_status_t AXIS2_CALL 
axis2_operation_free (axis2_operation_t *operation, axis2_env_t **env)
{ 
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    
	if(NULL != operation->ops)
		AXIS2_FREE((*env)->allocator, operation->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->param_container)
	    AXIS2_PARAM_CONTAINER_FREE(AXIS2_INTF_TO_IMPL(operation)->param_container
        , env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->wsdl_operation)
	    AXIS2_WSDL_OPERATION_FREE(AXIS2_INTF_TO_IMPL(operation)->wsdl_operation
        , env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->parent)
	    AXIS2_SVC_FREE(AXIS2_INTF_TO_IMPL(operation)->parent, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->msg_recv)
	    AXIS2_MSG_RECV_FREE(AXIS2_INTF_TO_IMPL(operation)->msg_recv, env);
        
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(operation));
    
    return AXIS2_SUCCESS;
}	
	
axis2_status_t AXIS2_CALL 
axis2_operation_add_param (axis2_operation_t *operation, 
                                axis2_env_t **env,       
		                        axis2_param_t *param)
{
    axis2_param_container_t *param_container_l = NULL;
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FALSE);
	
    if(AXIS2_TRUE == axis2_operation_is_param_locked(operation, env, 
            AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
    }
    else
    {
        param_container_l = (axis2_param_container_t *) 
            axis2_operation_get_component_property(operation, env, 
                (axis2_char_t *) PARAMETER_KEY);
        return AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container_l, env, param);
    }
    
    return AXIS2_SUCCESS;
}

axis2_param_t * AXIS2_CALL
axis2_operation_get_param (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        const axis2_char_t *param_name)
{
    axis2_param_container_t *param_container_l = NULL;
	AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, param_name, NULL);
    if(AXIS2_INTF_TO_IMPL(operation)->param_container)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_OPERATION, NULL);
	
    param_container_l = (axis2_param_container_t *) 
        axis2_operation_get_component_property(operation, env, (axis2_char_t *) 
            PARAMETER_KEY);
	
	return AXIS2_PARAM_CONTAINER_GET_PARAM(param_container_l, env, param_name);
}

axis2_array_list_t * AXIS2_CALL
axis2_operation_get_params(axis2_operation_t *operation, 
                                axis2_env_t **env)
{
    axis2_param_container_t *param_container_l = NULL;
	AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, AXIS2_INTF_TO_IMPL(operation)->param_container, 
        AXIS2_FALSE);
	
    param_container_l = (axis2_param_container_t *) 
        axis2_operation_get_component_property(operation, env, (axis2_char_t *) 
            PARAMETER_KEY);
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(param_container_l, env);
}

axis2_bool_t AXIS2_CALL 
axis2_operation_is_param_locked(axis2_operation_t *operation, 
                                    axis2_env_t **env,
		                            const axis2_char_t *param_name)
{
    axis2_svc_t *parent_l = NULL;
    axis2_param_t *param_l = NULL;
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    if(AXIS2_INTF_TO_IMPL(operation)->param_container)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_OPERATION, 
            AXIS2_FALSE);
	
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
	
    axis2_bool_t locked = AXIS2_FALSE;
    /* checking the locked value of parent*/
    parent_l = axis2_operation_get_parent(operation, env);
    if(NULL != parent_l)
    {
        locked = AXIS2_SVC_IS_PARAM_LOCKED(parent_l, env, param_name);
    }
    if(AXIS2_TRUE == locked)
    {
        return AXIS2_TRUE;
    }        
    else
    {
        param_l = axis2_operation_get_param(operation, env, param_name);   
    }
	return AXIS2_PARAM_IS_LOCKED(param_l, env);
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_parent (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        axis2_svc_t *svc)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(operation)->parent = svc;
	return AXIS2_SUCCESS;
}

axis2_svc_t * AXIS2_CALL
axis2_operation_get_parent (axis2_operation_t *operation, 
                                axis2_env_t **env)
{           
	AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
	return AXIS2_INTF_TO_IMPL(operation)->parent;
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        axis2_msg_recv_t *msg_recv)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_recv, AXIS2_FAILURE);
	AXIS2_INTF_TO_IMPL(operation)->msg_recv = msg_recv;
	
	return AXIS2_SUCCESS;
}

axis2_msg_recv_t * AXIS2_CALL
axis2_operation_get_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->msg_recv;
}
 
axis2_status_t AXIS2_CALL
axis2_operation_set_name (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            axis2_qname_t *qname)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
	
    return AXIS2_WSDL_OPERATION_SET_NAME(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env, qname);
}
    
axis2_qname_t * AXIS2_CALL
axis2_operation_get_name (axis2_operation_t *operation, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
	
    return AXIS2_WSDL_OPERATION_GET_NAME(AXIS2_INTF_TO_IMPL(operation)->wsdl_operation,
        env);
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env,
		                                    const axis2_char_t *pattern)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_MSG_EXCHANGE_PATTERN(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env, pattern);
}

axis2_char_t * AXIS2_CALL
axis2_operation_get_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    
    return AXIS2_WSDL_OPERATION_GET_MSG_EXCHANGE_PATTERN
        (AXIS2_INTF_TO_IMPL(operation)->wsdl_operation, env);
}

axis2_hash_t * AXIS2_CALL
axis2_operation_get_component_properties(axis2_operation_t *operation,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    
    return AXIS2_WSDL_OPERATION_GET_COMPONENT_PROPERTIES(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env);
}

axis2_status_t AXIS2_CALL
axis2_operation_set_component_properties(axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        axis2_hash_t *properties)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_COMPONENT_PROPERTIES(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env, properties);
}

axis2_wsdl_component_t * AXIS2_CALL
axis2_operation_get_component_property(axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        const axis2_char_t *key)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, key, NULL);
    
    return AXIS2_WSDL_OPERATION_GET_COMPONENT_PROPERTY(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env, key); 
}

axis2_status_t AXIS2_CALL
axis2_operation_set_component_property (axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        const void *key,
                                        void *value)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_COMPONENT_PROPERTY(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env, (axis2_char_t *) key, value);
}

axis2_char_t * AXIS2_CALL
axis2_operation_get_style (axis2_operation_t *operation,
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    
    return AXIS2_WSDL_OPERATION_GET_STYLE(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env);
}

axis2_status_t  AXIS2_CALL
axis2_operation_set_style (axis2_operation_t *operation,
                            axis2_env_t **env,
                            axis2_char_t *style)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_STYLE(AXIS2_INTF_TO_IMPL(operation)->
        wsdl_operation, env, style);
}
