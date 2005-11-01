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
 
#include <axis2_description_param_include.h>

/** @struct axis2_description_param_include
  * @brief DESCRIPTION param_include struct
  *	Holder for parameters
  *  
*/  
struct axis2_description_param_include_s
{
	axis2_description_param_include_ops_t *ops;
    axis2_hash_t *params;
    axis2_bool_t is_locked;	
};

/************************* Function prototypes ********************************/

axis2_status_t axis2_description_param_include_ops_free 
	(axis2_description_param_include_t *param_include);

axis2_status_t axis2_description_param_include_ops_add_param
    (axis2_env_t *environment
	, axis2_description_param_include_t *param_include
    , const axis2_description_param_t *param);
	 
axis2_description_param_t *axis2_description_param_include_ops_get_param
    (axis2_env_t *env, axis2_description_param_include_t *param_include
	, const axis2_char_t *name);

axis2_hash_t *axis2_description_param_include_ops_get_params
    (axis2_env_t *env
	, axis2_description_param_include_t *param_include);

axis2_status_t axis2_description_param_include_ops_is_param_locked
    (axis2_env_t *env, axis2_description_param_include_t *param_include
	, const axis2_char_t *param_name);

/************************** End of function prototypes ************************/

axis2_description_param_include_ops_t *axis2_description_param_include_get_ops
		(axis2_env_t *env
		, axis2_description_param_include_t *param_include)
{
	if(!param_include)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return param_include->ops;
}

axis2_description_param_include_t *axis2_description_param_include_create (
		axis2_env_t *env)
{
	axis2_description_param_include_t *param_include = 
		(axis2_description_param_include_t *)
		malloc(sizeof(axis2_description_param_include_t));
	if(!param_include)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_description_param_include_ops_t *ops = 
		malloc(sizeof(axis2_description_param_include_ops_t));
	if(!ops)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	ops->free = (axis2_description_param_include_free_t) 
				axis2_description_param_include_ops_free;
	ops->add_param = (axis2_description_param_include_add_param_t) 
				axis2_description_param_include_ops_add_param;
	ops->get_param = (axis2_description_param_include_get_param_t) 
				axis2_description_param_include_ops_get_param;
	ops->get_params = (axis2_description_param_include_get_params_t) 
				axis2_description_param_include_ops_get_params;
	ops->is_param_locked = (axis2_description_param_include_is_param_locked_t) 
				axis2_description_param_include_ops_is_param_locked;
	param_include->ops = ops;
				
	param_include->params = axis2_hash_make (env);				
	if(!param_include->params)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;			
	}	
	param_include->is_locked = AXIS2_FALSE;
	return param_include;
}

/*************************** Start of operation impls *************************/

axis2_status_t axis2_description_param_include_ops_free 
		(axis2_description_param_include_t *param_include)
{
	if(param_include)
		free(param_include);
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_description_param_include_ops_add_param
    	(axis2_env_t *env
		, axis2_description_param_include_t *param_include
    	, const axis2_description_param_t *param)
{
	if(!param_include || !param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	if (!(param_include->params))
	{                    
		param_include->params = axis2_hash_make (env);
	}	
	axis2_hash_set (param_include->params		
		, axis2_description_param_get_name(env, param)
		, AXIS2_HASH_KEY_STRING, param);
	
	return AXIS2_SUCCESS;

}

axis2_description_param_t *axis2_description_param_include_ops_get_param
    	(axis2_env_t* env
		, axis2_description_param_include_t *param_include
		, const axis2_char_t *name)
{
	if(!param_include)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return (axis2_description_param_t *)(axis2_hash_get 
		(param_include->params, axis2_strdup(name)
		, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t *axis2_description_param_include_ops_get_params
    	(axis2_env_t *env
		, axis2_description_param_include_t *param_include)
{
	if(!param_include)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return param_include->params;
}

axis2_status_t axis2_description_param_include_ops_is_param_locked
    	(axis2_env_t *env
		, axis2_description_param_include_t *param_include
		, const axis2_char_t *param_name)
{
	axis2_description_param_t *param = (axis2_description_param_t *)
		(axis2_hash_get (param_include->params
		, axis2_strdup(param_name)
		, AXIS2_HASH_KEY_STRING));
	
	if(!param)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;	
	return axis2_description_param_is_locked(env, param);
}
