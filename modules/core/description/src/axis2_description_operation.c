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
 
#include <axis2_description_operation.h>

/**
  * @struct axis2_description_operation_s
  * @brief DESCRIPTION operation
  * This holds the information about operation.
  */
struct axis2_description_operation_s
{
	axis2_description_operation_ops_t *ops;
	axis2_description_param_include_t *param_include;
	axis2_description_service_t *parent;
	axis2_qname_t *name;
	axis2_engine_msg_receiver_t *msg_receiver;
	axis2_char_t* msg_exchange_pattern;
};

	
/*************************** Function headers *********************************/

axis2_status_t axis2_description_operation_ops_free (axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);	

axis2_status_t axis2_description_operation_ops_add_param
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_description_param_t *param);

axis2_description_param_t *axis2_description_operation_ops_get_param
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *name);

axis2_hash_t *axis2_description_operation_ops_get_params
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);

axis2_bool_t axis2_description_operation_ops_is_param_locked(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *param_name);

axis2_status_t axis2_description_operation_ops_set_parent
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_description_service_t *service_desc);

axis2_description_service_t *axis2_description_operation_ops_get_parent
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);
		
axis2_qname_t *axis2_description_operation_ops_get_name
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc);

axis2_status_t axis2_description_operation_ops_set_msg_exchange_pattern 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_char_t *pattern);
		
axis2_char_t *axis2_description_operation_ops_get_msg_exchange_pattern 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc);
		
axis2_status_t axis2_description_operation_ops_set_msg_receiver 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_engine_msg_receiver_t *msg_receiver);

axis2_engine_msg_receiver_t *axis2_description_operation_ops_get_msg_receiver 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc);
		
/************************* End of function headers ****************************/	

axis2_description_operation_ops_t *axis2_description_operation_get_ops
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc)
{
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;	
	}
	return (axis2_description_operation_ops_t *) operation_desc->ops;
}

axis2_description_operation_t *axis2_description_operation_create 
		(axis2_environment_t *env)
{
	axis2_description_operation_ops_t *ops = NULL;
	axis2_description_operation_t *operation_desc = 
		(axis2_description_operation_t *) axis2_malloc (env->allocator
		, sizeof (axis2_description_operation_t));
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	ops = (axis2_description_operation_ops_t *) axis2_malloc(env->allocator,
		sizeof(axis2_description_operation_ops_t));
	if(!ops)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}	
	ops->free = axis2_description_operation_ops_free;
	ops->add_param = axis2_description_operation_ops_add_param;
	ops->get_param = axis2_description_operation_ops_get_param;
	ops->get_params = axis2_description_operation_ops_get_params;
	ops->set_parent = axis2_description_operation_ops_set_parent;
	ops->get_parent = axis2_description_operation_ops_get_parent;
	ops->get_name = axis2_description_operation_ops_get_name;
	ops->set_msg_exchange_pattern 
		= axis2_description_operation_ops_set_msg_exchange_pattern;
	ops->get_msg_exchange_pattern
		= axis2_description_operation_ops_get_msg_exchange_pattern;
	ops->set_msg_receiver = axis2_description_operation_ops_set_msg_receiver;
	ops->get_msg_receiver = axis2_description_operation_ops_get_msg_receiver;
	
	operation_desc->ops = ops;
	
	axis2_description_param_include_t *param_include 
		= (axis2_description_param_include_t *)
		axis2_description_param_include_create(env);		
	if(!param_include)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}

	operation_desc->param_include = param_include;
	
	operation_desc->parent = NULL;
	
	operation_desc->name = NULL;
	
	operation_desc->msg_receiver = NULL;
	
	operation_desc->msg_exchange_pattern = MEP_URI_IN_OUT;
						
	return operation_desc;
}

axis2_description_operation_t *axis2_description_operation_create_with_name 
		(axis2_environment_t *env, axis2_qname_t *name)
{
	axis2_description_operation_t *operation_desc 
		= axis2_description_operation_create(env);
	
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	if(!name)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}		
						
	return operation_desc;	
}

/******************************************************************************/

axis2_status_t axis2_description_operation_ops_free (axis2_environment_t *env
		, axis2_description_operation_t *operation_desc)
{
	if(operation_desc)
	{
		axis2_free(env->allocator, operation_desc);
		return AXIS2_SUCCESS;
	}
	return AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED;
}	
	
axis2_status_t axis2_description_operation_ops_add_param
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_description_param_t *param)
{
	if(!operation_desc || !operation_desc->param_include || !param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	axis2_hash_set (axis2_description_param_include_get_params(env
		, operation_desc->param_include), axis2_description_param_get_name(env
		, param)
		, AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
	
}

axis2_description_param_t *axis2_description_operation_ops_get_param(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *name)
{
	if(!operation_desc || !operation_desc->param_include)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	axis2_char_t *tempname = axis2_strdup(env->string, name);
	if(!tempname)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
		
	return (axis2_description_param_t *)(axis2_hash_get 
		(axis2_description_param_include_get_params(env
		, operation_desc->param_include), tempname, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t *axis2_description_operation_ops_get_params(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc)
{
	if(!operation_desc || !operation_desc->param_include)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	
	return axis2_description_param_include_get_params(env
		, operation_desc->param_include);
}

axis2_bool_t axis2_description_operation_ops_is_param_locked(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *param_name)
{
	if(!env || !operation_desc || !operation_desc->param_include)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return AXIS2_FALSE;
	}
	axis2_char_t *tempname = axis2_strdup(env->string, param_name);
	if(!tempname)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return AXIS2_FALSE;
	}
		
	return axis2_description_param_include_is_param_locked (env
		, operation_desc->param_include, param_name); 
	
}

axis2_status_t axis2_description_operation_ops_set_parent
		(axis2_environment_t *env ,axis2_description_operation_t *operation_desc
		, axis2_description_service_t *service_desc)
{
	if(!operation_desc || !service_desc)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	operation_desc->parent = service_desc;
	return AXIS2_SUCCESS;
}

axis2_description_service_t *axis2_description_operation_ops_get_parent
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc)
{
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return operation_desc->parent;
}

axis2_qname_t *axis2_description_operation_ops_get_name
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc)
{
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return operation_desc->name;
}

axis2_status_t axis2_description_operation_ops_set_msg_exchange_pattern 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_char_t *pattern)
{
	if(!operation_desc)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;		
	}
	operation_desc->msg_exchange_pattern = axis2_strdup(env->string, pattern);
	if(!operation_desc->msg_exchange_pattern)
		return AXIS2_ERROR_NO_MEMORY;
	return AXIS2_SUCCESS;
}

axis2_char_t *axis2_description_operation_ops_get_msg_exchange_pattern 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc)
{
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return operation_desc->msg_exchange_pattern;
}

axis2_status_t axis2_description_operation_ops_set_msg_receiver 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_engine_msg_receiver_t *msg_receiver)
{
	if(!operation_desc)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;		
	}
	operation_desc->msg_receiver = msg_receiver;
	if(!operation_desc->msg_receiver)
		return AXIS2_ERROR_NO_MEMORY;
	return AXIS2_SUCCESS;
}

axis2_engine_msg_receiver_t *axis2_description_operation_ops_get_msg_receiver 
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc)
{
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return operation_desc->msg_receiver;
}
