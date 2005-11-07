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
	axis2_svc_t *parent;
	axis2_qname_t *qname;
	axis2_msg_recv_t *msg_recv;
	axis2_char_t* msg_exchange_pattern;
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

axis2_hash_t * AXIS2_CALL
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
		
axis2_qname_t * AXIS2_CALL
axis2_operation_get_name (axis2_operation_t *operation, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env,
                                            axis2_char_t *pattern);
		
axis2_char_t * AXIS2_CALL
axis2_operation_get_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_operation_set_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env,
                                axis2_engine_msg_recv_t *msg_recv);

axis2_engine_msg_recv_t * AXIS2_CALL
axis2_operation_get_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env);
		
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
		AXIS2_ERROR_HANDLE(env, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	operation_impl->ops->free = axis2_operation_free;
	operation_impl->ops->add_param = axis2_operation_add_param;
	operation_impl->ops->get_param = axis2_operation_get_param;
	operation_impl->ops->get_params = axis2_operation_get_params;
	operation_impl->ops->set_parent = axis2_operation_set_parent;
	operation_impl->ops->get_parent = axis2_operation_get_parent;
	operation_impl->ops->get_name = axis2_operation_get_name;
	operation_impl->ops->set_msg_exchange_pattern 
		= axis2_operation_set_msg_exchange_pattern;
	operation_impl->ops->get_msg_exchange_pattern
		= axis2_operation_get_msg_exchange_pattern;
	operation_impl->ops->set_msg_recv = axis2_operation_set_msg_recv;
	operation_impl->ops->get_msg_recv = axis2_operation_get_msg_recv;
	
	axis2_param_container_t *param_container = (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == param_container)
	{
        AXIS2_ERROR_HANDLE(env, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	operation_impl->param_container = param_container;
	
	operation_impl->parent = NULL;
	
	operation_impl->qname = NULL;
	
	operation_impl->msg_recv = NULL;
	
	operation_impl->msg_exchange_pattern = MEP_URI_IN_OUT;
						
	return &(operation_impl->operation);
}

axis2_operation_t * AXIS2_CALL
axis2_operation_create_with_name (axis2_env_t **env, axis2_qname_t *qname)
{
	axis2_operation_impl_t *operation_impl = 
        AXIS2_INTF_TO_IMPL(axis2_operation_create(env));
    
	if(NULL == operation_impl)
	{
		AXIS2_ERROR_HANDLE(env, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    AXIS2_PARAM_CHECK(env, operation_impl, AXIS2_FAILURE);	
	
    operation_impl->qname = qname;    
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
    
/*    if(NULL != AXIS2_INTF_TO_IMPL(operation)->parent)
	    AXIS2_SVC_FREE(AXIS2_INTF_TO_IMPL(operation)->parent, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->qname)
	    AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(operation)->qname, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->msg_recv)
	    AXIS2_MSG_RECV_FREE(AXIS2_INTF_TO_IMPL(operation)->msg_recv, env);
*/    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->msg_exchange_pattern)
	    AXIS2_FREE((*env)->allocator, 
        AXIS2_INTF_TO_IMPL(operation)->msg_exchange_pattern);
        
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(operation));
    
    return AXIS2_SUCCESS;
}	
	
axis2_status_t AXIS2_CALL 
axis2_operation_add_param (axis2_operation_t *operation, 
                                axis2_env_t **env,       
		                        axis2_param_t *param)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env, AXIS2_INTF_TO_IMPL(operation)->param_container, 
        AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env, param, AXIS2_FALSE);
	
	axis2_hash_set 
        (AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(operation)->
        param_container, env), AXIS2_PARAM_GET_NAME(param, env), 
        AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
	
}

axis2_param_t * AXIS2_CALL
axis2_operation_get_param (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        const axis2_char_t *name)
{
	AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env, AXIS2_INTF_TO_IMPL(operation)->param_container, 
        AXIS2_FALSE);
	
	axis2_char_t *tempname = axis2_strdup(name);
	if(NULL == tempname)
        AXIS2_ERROR_HANDLE(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FALSE);
		
	return (axis2_param_t *)(axis2_hash_get (
        AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(operation)->
        param_container, env), tempname, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL
axis2_operation_get_params(axis2_operation_t *operation, 
                                axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env, AXIS2_INTF_TO_IMPL(operation)->param_container, 
        AXIS2_FALSE);
	
	return AXIS2_PARAM_CONTAINER_GET_PARAMS
        (AXIS2_INTF_TO_IMPL(operation)->param_container, env);
}

axis2_bool_t AXIS2_CALL 
axis2_operation_is_param_locked(axis2_operation_t *operation, 
                                    axis2_env_t **env,
		                            const axis2_char_t *param_name)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env, AXIS2_INTF_TO_IMPL(operation)->param_container, 
        AXIS2_FALSE);
	
	axis2_char_t *tempname = axis2_strdup(param_name);
	if(NULL == tempname)
        AXIS2_ERROR_HANDLE(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FALSE); 
		
	return AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED
		(AXIS2_INTF_TO_IMPL(operation)->param_container, env, tempname); 
	
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_parent (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        axis2_svc_t *svc)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env, msg_recv, AXIS2_FAILURE);
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

axis2_qname_t * AXIS2_CALL
axis2_operation_get_name (axis2_operation_t *operation, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
	
    return AXIS2_INTF_TO_IMPL(operation)->name;
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env,
		                                    axis2_char_t *pattern)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
	AXIS2_INTF_TO_IMPL(operation)->msg_exchange_pattern = axis2_strdup(pattern);
	if(NULL == AXIS2_INTF_TO_IMPL(operation)->msg_exchange_pattern)
        AXIS2_ERROR_HANDLE(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		
	return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_operation_get_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
	return AXIS2_INTF_TO_IMPL(operation)->msg_exchange_pattern;
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        axis2_engine_msg_recv_t *msg_recv)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env, msg_recv, AXIS2_FAILURE);
	AXIS2_INTF_TO_IMPL(operation)->msg_recv = msg_recv;
	
	return AXIS2_SUCCESS;
}

axis2_engine_msg_recv_t * AXIS2_CALL
axis2_operation_get_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->msg_recv;
}
