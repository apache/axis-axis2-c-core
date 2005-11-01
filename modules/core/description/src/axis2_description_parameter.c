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
 
#include <axis2_description_parameter.h>

/** @struct axis2_description_parameter
  * @brief DESCRIPTION parameter struct
  *	Paramter can be any thing it can be XML element with number of child 
  * elements
  *  
*/  
struct axis2_description_param_s
{
	axis2_description_param_ops_t *ops;
    axis2_char_t *name;
    void *value;
    /*axis2_om_element_t *param_element;*/
    int type;
    axis2_bool_t is_locked;
};

/***************************** Function prototypes ****************************/

axis2_status_t axis2_description_param_ops_free (axis2_env_t *env
	, axis2_description_param_t * param);

axis2_status_t axis2_description_param_ops_set_name (axis2_env_t *env
	, axis2_description_param_t * param, const axis2_char_t *name);

axis2_char_t *axis2_description_param_ops_get_name (axis2_env_t *env
	, axis2_description_param_t * param);

axis2_status_t axis2_description_param_ops_set_value (axis2_env_t *env
	, axis2_description_param_t * param, void *value);

void *axis2_description_param_ops_get_value (axis2_env_t *env
	, axis2_description_param_t * param);

axis2_bool_t axis2_description_param_ops_is_locked (axis2_env_t *env
	, axis2_description_param_t * param);

axis2_status_t axis2_description_param_ops_set_locked (axis2_env_t *env
	, axis2_description_param_t * param, const axis2_bool_t locked);

/*void axis2_description_param_ops_set_param_element (axis2_env_t *env
	, axis2_description_param_t * param, const axis2_om_element_t * om_element);
*/										 

/*axis2_description_param_t *axis2_description_param_ops_get_param_element 
	(axis2_env_t *env, axis2_description_param_t *param);*/

axis2_status_t axis2_description_param_ops_set_type (axis2_env_t *env
	, axis2_description_param_t * param, const int type);

int axis2_description_param_ops_get_type (axis2_env_t *env
		, axis2_description_param_t * param);

/************************** End of function prototypes ************************/

axis2_description_param_ops_t *axis2_description_param_get_ops
	(axis2_env_t *env, axis2_description_param_t *param)
{
	if(!param)
	{
		/* set error code*/
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	return (axis2_description_param_ops_t *) param->ops;	
}


axis2_description_param_t *axis2_description_param_create 
		(axis2_env_t *env)
{
	axis2_description_param_t *param 
	= (axis2_description_param_t *) AXIS2_MALLOC (env->allocator
	, sizeof(axis2_description_param_t));
	if(!param)
	{
		/* set the error code*/
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_description_param_ops_t *ops
		= (axis2_description_param_ops_t *) AXIS2_MALLOC (env->allocator
		, sizeof(axis2_description_param_ops_t));
	if(!ops)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		AXIS2_FREE(env->allocator, param);
		return NULL;		
	}
	ops->free = axis2_description_param_ops_free;
	ops->set_name = axis2_description_param_ops_set_name;
	ops->get_name = axis2_description_param_ops_get_name;
	ops->set_value = axis2_description_param_ops_set_value;
	ops->get_value = axis2_description_param_ops_get_value;
	ops->is_locked = axis2_description_param_ops_is_locked;
	ops->set_locked = axis2_description_param_ops_set_locked;
	/*ops->set_param_element = axis2_description_param_ops_set_param_element;*/
	/*ops->get_param_element = axis2_description_param_ops_get_param_element;*/
	ops->set_type = axis2_description_param_ops_set_type;
	ops->get_type = axis2_description_param_ops_get_type;
	
	param->ops = ops;	
	param->name = NULL;
	param->value = NULL;
	/*param->param_element = NULL;*/
	param->type = AXIS2_PARAMETER_TEXT;
	param->is_locked = AXIS2_FALSE;
	return param;	
}	


axis2_description_param_t *axis2_description_param_create_with_name_value 
		(axis2_env_t *env, const axis2_char_t *name
		, const axis2_char_t *value)
{
	
	
	axis2_description_param_t *param = axis2_description_param_create(env);
	if(!param)
	{
		/* set the error code*/
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
	}
	
	param->name = axis2_strdup (name);
	if(!param->name)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		AXIS2_FREE(env->allocator, param);
	}
	param->value = axis2_strdup (value);
	if(!param->value)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		AXIS2_FREE(env->allocator, param);
		return NULL;
	}
	/*param->param_element = NULL;*/
	param->type = AXIS2_PARAMETER_TEXT;
	param->is_locked = AXIS2_FALSE;
	return param;
}

/**********************Start of operation impls********************************/

axis2_status_t axis2_description_param_ops_free (axis2_env_t *env
		,axis2_description_param_t * param)
{
	if(param)
	{
		AXIS2_FREE(env->allocator, param);
		return AXIS2_SUCCESS;
	}
	return AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED;
}

axis2_status_t axis2_description_param_ops_set_name (axis2_env_t *env
		, axis2_description_param_t * param, const axis2_char_t *name)
{
	if(!param)
	{
		/* return the error code*/
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;		
	}
	param->name = axis2_strdup(name);
	return AXIS2_SUCCESS;	
}

axis2_char_t *axis2_description_param_ops_get_name (axis2_env_t *env
		, axis2_description_param_t *param)
{
	if(!param)
	{
		/* set error code*/	
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;	
		return NULL;
	}
	return param->name;
}

axis2_status_t axis2_description_param_ops_set_value (axis2_env_t *env
		, axis2_description_param_t * param, void *value)
{
	if(!param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	param->value = value;
	return AXIS2_SUCCESS;
}	

void *axis2_description_param_ops_get_value (axis2_env_t *env
		, axis2_description_param_t * param)
{
	if(!param)
	{
		/* set error code*/	
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;		
	}
	return param->value;
}

axis2_bool_t axis2_description_param_ops_is_locked (axis2_env_t *env
		, axis2_description_param_t * param)
{
	if(!param)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	return param->is_locked;
}

axis2_status_t axis2_description_param_ops_set_locked (axis2_env_t *env
		, axis2_description_param_t * param, const axis2_bool_t is_locked)
{
	if(!param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	param->is_locked = is_locked;	
	return AXIS2_SUCCESS;
}

/*axis2_status_t axis2_description_param_ops_set_param_element 
		(axis2_env_t *env, axis2_description_param_t * param
        , const axis2_om_element_t * om_element)
{
	if(!param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	param->param_element = om_element;	
	return AXIS2_SUCCESS;
	
}

axis2_description_param_t *axis2_description_param_get_param_element 
		(axis2_env_t *env, axis2_description_param_t *param)
{
	if(!param)
	{
		env->error.error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return (axis2_description_param_t *) param->param_element;
}*/

axis2_status_t axis2_description_param_ops_set_type (axis2_env_t *env
		, axis2_description_param_t * param, const int type)
{
	if(!param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	param->type = type;	
	return AXIS2_SUCCESS;
}

int axis2_description_param_ops_get_type (axis2_env_t *env
		, axis2_description_param_t * param)
{
	if(!param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	return param->type;
}
