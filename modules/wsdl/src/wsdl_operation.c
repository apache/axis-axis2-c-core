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
 
#include <axis2_wsdl_operation.h>

typedef struct axis2_wsdl_operation_impl_s axis2_wsdl_operation_impl_t;

/** 
 * @brief Wsdl Component struct impl
 *	Axis2 Wsdl Component impl  
 */
struct axis2_wsdl_operation_impl_s
{
	axis2_wsdl_operation_t wsdl_operation;
	axis2_wsdl_component_t *wsdl_component;
	axis2_char_t *msg_exchange_pattern;
	axis2_char_t *style;
	axis2_qname_t *name;
		
};

#define AXIS2_INTF_TO_IMPL(wsdl_operation) \
        ((axis2_wsdl_operation_impl_t *)(wsdl_operation))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_free (axis2_wsdl_operation_t *wsdl_operation, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_msg_exchange_pattern (
		                                axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env,
                                        const axis2_char_t *msg_exchange_pattern);

axis2_char_t * AXIS2_CALL 
axis2_wsdl_operation_get_msg_exchange_pattern 
		                                (axis2_wsdl_operation_t *wsdl_operation, 
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_name (axis2_wsdl_operation_t *wsdl_operation, 
                                axis2_env_t **env,
		                        const axis2_qname_t *name);

axis2_qname_t * AXIS2_CALL 
axis2_wsdl_operation_get_name (axis2_wsdl_operation_t *wsdl_operation,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_style (axis2_wsdl_operation_t *wsdl_operation,
                                    axis2_env_t **env,
		                            const axis2_char_t *style);

axis2_char_t * AXIS2_CALL 
axis2_wsdl_operation_get_style (axis2_wsdl_operation_t *wsdl_operation,
                                    axis2_env_t **env);
		
axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_component_properties (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env,
		                                axis2_hash_t *properties);

axis2_hash_t * AXIS2_CALL 
axis2_wsdl_operation_get_component_properties (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_component_property (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env,
                                        const void *key, 
                                        void *value);
	
void * AXIS2_CALL 
axis2_wsdl_operation_get_component_property (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env,
		                                const void *key);


/***************************** End of function headers ************************/

axis2_wsdl_operation_t * AXIS2_CALL 
axis2_wsdl_operation_create (axis2_env_t **env)
{
	axis2_wsdl_operation_impl_t *wsdl_operation_impl = 
        (axis2_wsdl_operation_impl_t *) AXIS2_MALLOC (env->allocator, 
        sizeof(axis2_wsdl_operation_impl_t));
    
	if(!wsdl_operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	wsdl_operation_impl->wsdl_operation.ops = (axis2_wsdl_operation_ops_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_operation_ops_t));
    
	if(NULL == wsdl_operation_impl->wsdl_operation.ops)
	{
        AXIS2_FREE((*env)->allocator, wsdl_operation_impl->wsdl_operation.ops);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
	wsdl_operation_impl->wsdl_operation.ops->free = axis2_wsdl_operation_free;
	
    wsdl_operation_impl->wsdl_operation.ops->set_msg_exchange_pattern =
        axis2_wsdl_operation_set_msg_exchange_pattern;
    
    wsdl_operation_impl->wsdl_operation.ops->get_msg_exchange_pattern =
        axis2_wsdl_operation_get_msg_exchange_pattern;
    
    wsdl_operation_impl->wsdl_operation.ops->set_name =
        axis2_wsdl_operation_set_name;
    
    wsdl_operation_impl->wsdl_operation.ops->get_name =
        axis2_wsdl_operation_get_name;
    
    wsdl_operation_impl->wsdl_operation.ops->set_style =
        axis2_wsdl_operation_set_style;
    
    wsdl_operation_impl->wsdl_operation.ops->get_style =
        axis2_wsdl_operation_get_style;
    
    wsdl_operation_impl->wsdl_operation.ops->set_component_properties =
        axis2_wsdl_operation_set_component_properties;
        
    wsdl_operation_impl->wsdl_operation.ops->get_component_properties =
        axis2_wsdl_operation_get_component_properties;
    
    wsdl_operation_impl->wsdl_operation.ops->set_component_property =
        axis2_wsdl_operation_set_component_property;
    
    wsdl_operation_impl->wsdl_operation.ops->get_component_property =
        axis2_wsdl_operation_get_component_property;
    
	
	wsdl_operation_impl->wsdl_component = AXIS2_INTF_TO_IMPL(
        axis2_wsdl_component_create(env));
	wsdl_operation_impl->msg_exchange_pattern = NULL;
	wsdl_operation_impl->style = NULL;
	wsdl_operation_impl->name = NULL;
	
	return &(wsdl_operation_impl->wsdl_operation);	
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_free (axis2_wsdl_operation_t *wsdl_operation,
                            axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, AXIS2_FAILURE);
    
    if(NULL != wsdl_operation->ops)
        AXIS2_FREE((*env)->allocator, wsdl_operation->ops, AXIS2_FAILURE);
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_operation)->wsdl_component)
        AXIS2_WSDL_COMPONENT_FREE(AXIS2_INTF_TO_IMPL(wsdl_component)->
            wsdl_component, env);
    
    AXIS2_FREE(env->allocator, wsdl_operation);
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_msg_exchange_pattern (
		                                axis2_wsdl_operation_t *wsdl_operation, 
                                        axis2_env_t **env, 
		                                const axis2_char_t *msg_exchange_pattern)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_exchange_pattern, AXIS2_FAILURE);
	
    axis2_char_t *tempname = AXIS2_STRDUP(msg_exchange_pattern, env);
    if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
    
	(AXIS2_INTF_TO_IMPL((wsdl_operation))->msg_exchange_pattern 
		= tempname;
	
	return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL 
axis2_wsdl_operation_get_msg_exchange_pattern (
                                        axis2_wsdl_operation_t *wsdl_operation, 
                                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, NULL);
    
	return (AXIS2_INTF_TO_IMPL( wsdl_operation)->msg_exchange_pattern;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_name (axis2_wsdl_operation_t *wsdl_operation, 
                                axis2_env_t **env
		                        const axis2_qname_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
    	
	(AXIS2_INTF_TO_IMPL((wsdl_operation))->name = name;
    
	return AXIS2_SUCCESS;
}


axis2_qname_t * AXIS2_CALL 
axis2_wsdl_operation_get_name (axis2_wsdl_operation_t *wsdl_operation, 
                                axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, NULL);
    
	return (AXIS2_INTF_TO_IMPL(wsdl_operation)->name;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_style (axis2_wsdl_operation_t *wsdl_operation,
                                    axis2_env_t **env,
		                            const axis2_char_t *style)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    axis2_char_t *tempstyle = AXIS2_STRDUP(style, env);
    if(NULL == tempstyle)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
    
	(AXIS2_INTF_TO_IMPL((wsdl_operation))->style = tempstyle;
	
	return AXIS2_SUCCESS;		
}

axis2_char_t * AXIS2_CALL 
axis2_wsdl_operation_get_style (axis2_wsdl_operation_t *wsdl_operation,
                                    axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, NULL);
	
	return (AXIS2_INTF_TO_IMPL((wsdl_operation))->style;
}


axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_component_properties (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env
		                                axis2_hash_t *properties)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, properties, AXIS2_FAILURE);
    
	if(NULL == AXIS2_INTF_TO_IMPL(wsdl_operation)->wsdl_component) 
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_OPERATION);
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTIES(env
		, wsdl_operation_impl->wsdl_component, properties))
	{
		/* do error handling here */
	}
	return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL 
axis2_wsdl_operation_get_component_properties (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env)		
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, NULL);
    
	axis2_wsdl_operation_impl_t *wsdl_operation_impl = AXIS2_INTF_TO_IMPL(
		(wsdl_operation);
	if(NULL == wsdl_operation_impl) return AXIS2_ERROR_INVALID_NULL_PARAM;
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTIES(env
		, wsdl_operation_impl->wsdl_component, properties))
	{
		/* do error handling here */
	}
		
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_operation_set_component_property (
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env,
	                                    const void *key, 
                                        void *value)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    
	axis2_wsdl_operation_impl_t *wsdl_operation_impl = AXIS2_INTF_TO_IMPL(
		(wsdl_operation);
	if(NULL == wsdl_operation_impl || wsdl_operation_impl->wsdl_component || NULL == key 
		|| NULL == value) return AXIS2_ERROR_INVALID_NULL_PARAM;
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(env
		, wsdl_operation_impl->wsdl_component, key, value))
	{
		/* do error handling here */
	}
	return AXIS2_SUCCESS;
}
	
void * AXIS2_CALL 
axis2_wsdl_operation_get_component_property(
                                        axis2_wsdl_operation_t *wsdl_operation,
                                        axis2_env_t **env,
		                                const void *key)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    
	axis2_wsdl_operation_impl_t *wsdl_operation_impl = AXIS2_INTF_TO_IMPL(
		(wsdl_operation);
	if(NULL == wsdl_operation_impl || NULL == key) 
		return AXIS2_ERROR_INVALID_NULL_PARAM;
	
	if(AXIS2_SUCCESS != AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(env
		, wsdl_operation_impl->wsdl_component, key, value))
	{
		/* do error handling here */
	}
		
	return AXIS2_SUCCESS;
}
