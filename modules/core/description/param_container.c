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
typedef struct axis2_param_container_impl
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

axis2_array_list_t * AXIS2_CALL 
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
    axis2_param_container_impl_t *param_container_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	param_container_impl = (axis2_param_container_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_param_container_impl_t));
	
	if(NULL == param_container_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }        
	
    param_container_impl->params = NULL;
    
    param_container_impl->params = axis2_hash_make (env);
	if(NULL == param_container_impl->params)
    {
        axis2_param_container_free(&(param_container_impl->param_container), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
	param_container_impl->param_container.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_param_container_ops_t));
	if(NULL == param_container_impl->param_container.ops)
    {
        axis2_param_container_free(&(param_container_impl->param_container), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL; 
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
	
	return &(param_container_impl->param_container);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL 
axis2_param_container_free (axis2_param_container_t *param_container,
								axis2_env_t **env)
{
    axis2_param_container_impl_t *param_container_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    
	AXIS2_FUNC_PARAM_CHECK(param_container, env, AXIS2_FAILURE);
    
    param_container_impl = AXIS2_INTF_TO_IMPL(param_container);
    
	if(NULL != param_container->ops)
		AXIS2_FREE((*env)->allocator, param_container->ops);
	
	if(NULL != param_container_impl->params)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (param_container_impl->params, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_param *param = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            param = (struct axis2_param *) val;
            if (param)
               AXIS2_PARAM_FREE (param, env);
            
            val = NULL;
            param = NULL;
               
        }
		axis2_hash_free(param_container_impl->params, env);
    }
	
    AXIS2_FREE((*env)->allocator, param_container_impl);    
    return status;
}

axis2_status_t AXIS2_CALL 
axis2_param_container_add_param (axis2_param_container_t *param_container, 
					axis2_env_t **env, 
					axis2_param_t *param)
{
    axis2_param_container_impl_t *param_container_impl = NULL;
    axis2_char_t *param_name = NULL;
    
	AXIS2_FUNC_PARAM_CHECK(param_container, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
	
    param_container_impl = AXIS2_INTF_TO_IMPL(param_container);
    
	if (NULL == (param_container_impl->params))
	{                    
		param_container_impl->params = axis2_hash_make (env);
        if(!param_container_impl->params)
        {
            return AXIS2_FAILURE;
        }
	}
    param_name = AXIS2_PARAM_GET_NAME(param, env);
	axis2_hash_set (AXIS2_INTF_TO_IMPL(param_container)->params	
		, param_name, AXIS2_HASH_KEY_STRING, param);
	
	return AXIS2_SUCCESS;

}

axis2_param_t * AXIS2_CALL
axis2_param_container_get_param (axis2_param_container_t *param_container, 
					axis2_env_t **env, 
					const axis2_char_t *name)
{
	AXIS2_FUNC_PARAM_CHECK(param_container, env, NULL);
	
	return (axis2_param_t *)(axis2_hash_get (AXIS2_INTF_TO_IMPL(param_container)->
		params, name, AXIS2_HASH_KEY_STRING));
}

axis2_array_list_t * AXIS2_CALL 
axis2_param_container_get_params (axis2_param_container_t *param_container, 
		axis2_env_t **env)
{
    axis2_param_container_impl_t *param_container_impl = NULL;
    axis2_hash_index_t *index_i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    /* create an array list with the initial default capacity 
     * Caller of this method should free this array list
     */
    axis2_array_list_t *array_list_l = axis2_array_list_create(env, 20);
    void *value = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(param_container, env, NULL);
    
    param_container_impl = AXIS2_INTF_TO_IMPL(param_container);
   
    for (index_i = axis2_hash_first (param_container_impl->params, env); index_i; 
            index_i = axis2_hash_next (env, index_i))
    {
        axis2_hash_this (index_i, NULL, NULL, &value);
        status = AXIS2_ARRAY_LIST_ADD(array_list_l, env, value);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_ARRAY_LIST_FREE(array_list_l ,env);
            return NULL;            
        }
    }
    
	return array_list_l;
}

axis2_status_t AXIS2_CALL 
axis2_param_container_is_param_locked (axis2_param_container_t *param_container, 
						axis2_env_t **env, 
						const axis2_char_t *param_name)
{
    axis2_param_t *param = NULL;
    
	AXIS2_FUNC_PARAM_CHECK(param_container, env, AXIS2_FAILURE);
    
	param = (axis2_param_t *)(axis2_hash_get (AXIS2_INTF_TO_IMPL(
        param_container)->params, param_name, AXIS2_HASH_KEY_STRING));
    if(!param)
    {
        /* In this case we consider param is not locked */
        return AXIS2_FALSE;
	}
	return AXIS2_PARAM_IS_LOCKED(param, env);
}
