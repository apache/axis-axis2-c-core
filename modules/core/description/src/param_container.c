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
 
#include <axis2_parameter_container.h>

/** @struct axis2_param_container_impl
  * @brief DESCRIPTION axis2_param_container_impl struct
  *	Container of parameters. 
  *  
*/  
typedef struct axis2_param_container_impl_s
{
	axis2_param_container_t param_container;
    axis2_hash_t *params;	
} axis2_param_container_impl_t;

#define AXIS2_INTF_TO_IMPL(param_container) \
		((axis2_param_container_impl_t *)param_container)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_parameter_container_free (axis2_param_container_t *param_container,
									axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_param_container_add_parameter (axis2_param_container_t *param_container, 
						axis2_env_t **env, 
						axis2_param_t *param);
	 
axis2_param_t * AXIS2_CALL 
axis2_param_container_get_parameter (axis2_param_container_t *param_container, 
						axis2_env_t **env, 
						const axis2_char_t *name);

axis2_hash_t * AXIS2_CALL 
axis2_param_container_get_parameters (axis2_param_container_t *param_container, 
						axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_param_container_is_parameter_locked (axis2_param_container_t *param_container, 
							axis2_env_t **env, 
							const axis2_char_t *param_name);

/************************** End of function prototypes ************************/

axis2_param_container_t * AXIS2_CALL 
axis2_param_container_create (axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, NULL);
	
	axis2_param_container_impl_t *param_container_impl = 
		(axis2_param_container_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_param_container_impl_t));
	
	
	if(NULL == param_container_impl)
	{
		AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
		return NULL;
	}
	
	axis2_param_container_ops_t *ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_param_container_ops_t));
	if(NULL == ops)
	{
		AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
		return NULL;
	}
	ops->free =  axis2_parameter_container_free;
	ops->add_param =  axis2_param_container_add_parameter;
	ops->get_param =  axis2_param_container_get_parameter;
	ops->get_params = axis2_param_container_get_parameters;
	ops->is_param_locked = axis2_param_container_is_parameter_locked;
	
	param_container_impl->param_container.ops = ops;
				
	param_container_impl->params = axis2_hash_make (env);
	
	if(NULL == param_container_impl->params)
	{
		AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
		return NULL;			
	}	
	
	return &(param_container_impl->param_container);
}

/*************************** Start of operation impls *************************/

axis2_status_t AXIS2_CALL 
axis2_parameter_container_free (axis2_param_container_t *param_container,
								axis2_env_t **env)
{
	AXIS2_FUNC_PARAMETER_CHECK(param_container, env, AXIS2_FAILURE);
	if(NULL != param_container->ops)
		AXIS2_FREE((*env)->allocator, param_container->ops);
	
	if(NULL != AXIS2_INTF_TO_IMPL(param_container)->params)
		AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(param_container)->params);
	
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(param_container));    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_param_container_add_parameter (axis2_param_container_t *param_container, 
					axis2_env_t **env, 
					axis2_param_t *param)
{
	AXIS2_FUNC_PARAMETER_CHECK(param_container, env, AXIS2_FAILURE);
	if(NULL == param)
	{
		AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error
			, AXIS2_ERROR_INVALID_NULL_PARAMETER);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	if (NULL == (AXIS2_INTF_TO_IMPL(param_container)->params))
	{                    
		AXIS2_INTF_TO_IMPL(param_container)->params = axis2_hash_make (env);
	}	
	axis2_hash_set (AXIS2_INTF_TO_IMPL(param_container)->params	
		, AXIS2_PARAMETER_GET_NAME(param, env)
		, AXIS2_HASH_KEY_STRING, param);
	
	return AXIS2_SUCCESS;

}

axis2_param_t * AXIS2_CALL
axis2_param_container_get_parameter (axis2_param_container_t *param_container, 
					axis2_env_t **env, 
					const axis2_char_t *name)
{
	AXIS2_FUNC_PARAMETER_CHECK(param_container, env, NULL);
	
	return (axis2_param_t *)(axis2_hash_get (AXIS2_INTF_TO_IMPL(param_container)->
		params, axis2_strdup(name), AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL 
axis2_param_container_get_parameters (axis2_param_container_t *param_container, 
		axis2_env_t **env)
{
	AXIS2_FUNC_PARAMETER_CHECK(param_container, env, NULL);
	
	return AXIS2_INTF_TO_IMPL(param_container)->params;
}

axis2_status_t AXIS2_CALL 
axis2_param_container_is_parameter_locked (axis2_param_container_t *param_container, 
						axis2_env_t **env, 
						const axis2_char_t *param_name)
{
	AXIS2_FUNC_PARAMETER_CHECK(param_container, env, AXIS2_FAILURE);
	
	axis2_param_t *param = (axis2_param_t *)
		(axis2_hash_get (AXIS2_INTF_TO_IMPL(param_container)->params
			, axis2_strdup(param_name), AXIS2_HASH_KEY_STRING));
	
	if(NULL == param)
	{
		AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error
			, AXIS2_ERROR_INVALID_NULL_PARAMETER);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
		
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;	
	}
	
	return AXIS2_PARAMETER_IS_LOCKED(param, env);
}
