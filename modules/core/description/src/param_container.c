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
 
#include <axis2_param_container.h>

/** 
 * @brief Param container struct impl
 *	Container for params  
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
	axis2_param_container_free (axis2_param_container_t *param_container,
									axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_param_container_add_param (axis2_param_container_t *param_container, 
						axis2_env_t **env, 
						axis2_param_t *param);
	 
axis2_param_t * AXIS2_CALL 
axis2_param_container_get_param (axis2_param_container_t *param_container, 
						axis2_env_t **env, 
						const axis2_char_t *name);

axis2_hash_t * AXIS2_CALL 
axis2_param_container_get_params (axis2_param_container_t *param_container, 
						axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_param_container_is_param_locked (axis2_param_container_t *param_container, 
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
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
	
	param_container_impl->param_container.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_param_container_ops_t));
	if(NULL == param_container_impl->param_container.ops)
    {
        AXIS2_FREE((*env)->allocator, param_container_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
	param_container_impl->param_container.ops->free =  axis2_param_container_free;
	param_container_impl->param_container.ops->add_param =  
        axis2_param_container_add_param;
	param_container_impl->param_container.ops->get_param =  
        axis2_param_container_get_param;
	param_container_impl->param_container.ops->get_params = 
        axis2_param_container_get_params;
	param_container_impl->param_container.ops->is_param_locked = 
        axis2_param_container_is_param_locked;
				
	param_container_impl->params = axis2_hash_make (env);
	if(NULL == param_container_impl->params)
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	
	return &(param_container_impl->param_container);
}

/*************************** Start of operation impls *************************/

axis2_status_t AXIS2_CALL 
axis2_param_container_free (axis2_param_container_t *param_container,
								axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(param_container, env, AXIS2_FAILURE);
	if(NULL != param_container->ops)
		AXIS2_FREE((*env)->allocator, param_container->ops);
	
	if(NULL != AXIS2_INTF_TO_IMPL(param_container)->params)
		axis2_hash_free(AXIS2_INTF_TO_IMPL(param_container)->params, env);
	
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(param_container));    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_param_container_add_param (axis2_param_container_t *param_container, 
					axis2_env_t **env, 
					axis2_param_t *param)
{
	AXIS2_FUNC_PARAM_CHECK(param_container, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
	
	if (NULL == (AXIS2_INTF_TO_IMPL(param_container)->params))
	{                    
		AXIS2_INTF_TO_IMPL(param_container)->params = axis2_hash_make (env);
	}	
	axis2_hash_set (AXIS2_INTF_TO_IMPL(param_container)->params	
		, AXIS2_PARAM_GET_NAME(param, env)
		, AXIS2_HASH_KEY_STRING, param);
	
	return AXIS2_SUCCESS;

}

axis2_param_t * AXIS2_CALL
axis2_param_container_get_param (axis2_param_container_t *param_container, 
					axis2_env_t **env, 
					const axis2_char_t *name)
{
	AXIS2_FUNC_PARAM_CHECK(param_container, env, NULL);
	
	return (axis2_param_t *)(axis2_hash_get (AXIS2_INTF_TO_IMPL(param_container)->
		params, AXIS2_STRDUP(name, env), AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL 
axis2_param_container_get_params (axis2_param_container_t *param_container, 
		axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(param_container, env, NULL);
	
	return AXIS2_INTF_TO_IMPL(param_container)->params;
}

axis2_status_t AXIS2_CALL 
axis2_param_container_is_param_locked (axis2_param_container_t *param_container, 
						axis2_env_t **env, 
						const axis2_char_t *param_name)
{
	AXIS2_FUNC_PARAM_CHECK(param_container, env, AXIS2_FAILURE);
	
	axis2_param_t *param = (axis2_param_t *)
		(axis2_hash_get (AXIS2_INTF_TO_IMPL(param_container)->params
			, AXIS2_STRDUP(param_name, env), AXIS2_HASH_KEY_STRING));
	
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
	
	return AXIS2_PARAM_IS_LOCKED(param, env);
}
