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
 
#include <axis2_description_servicegroup.h>

/** @struct axis2_description_servicegroup_s
  * @brief DESCRIPTION servicegroup
  * This holds the information about the servicegroup_name
  */
struct axis2_description_servicegroup_s
{
	axis2_description_servicegroup_ops_t *ops;
	axis2_description_param_include_t *param_include;
	axis2_char_t *servicegroup_name;
	axis2_hash_t *services;
	
};

/***************************** Function headers *******************************/

axis2_status_t axis2_description_servicegroup_ops_free(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc);

axis2_status_t axis2_description_servicegroup_ops_set_name
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, axis2_char_t *name);

axis2_char_t *axis2_description_servicegroup_ops_get_name
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc);

axis2_status_t axis2_description_servicegroup_ops_add_service
		(axis2_env_t *env
		, axis2_description_servicegroup_t* servicegroup_desc
		, axis2_description_service_t *service_desc);

axis2_description_service_t *axis2_description_servicegroup_ops_get_service
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_qname_t* service_name);
		
axis2_status_t axis2_description_servicegroup_ops_remove_service
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_qname_t* service_name);
		
axis2_status_t axis2_description_servicegroup_ops_add_param
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, axis2_description_param_t *param);
		

axis2_description_param_t *axis2_description_servicegroup_ops_get_param(
		axis2_env_t *env, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_char_t *name);
		
axis2_hash_t *axis2_description_servicegroup_ops_get_params
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc);
		
axis2_bool_t axis2_description_servicegroup_ops_is_param_locked(
		axis2_env_t *env, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_char_t *param_name);

/***************************** End of function headers ************************/

axis2_description_servicegroup_ops_t *axis2_description_servicegroup_get_ops
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc)
{
	if(!env || !servicegroup_desc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return servicegroup_desc->ops;
}

axis2_description_servicegroup_t *axis2_description_servicegroup_create
		(axis2_env_t *env)
{
	axis2_description_servicegroup_t *servicegroup_desc 
		= (axis2_description_servicegroup_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_description_servicegroup_t));
	if(!servicegroup_desc)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_description_servicegroup_ops_t *ops 
		= (axis2_description_servicegroup_ops_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_description_servicegroup_ops_t));
	if(!ops)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;	
	}
	ops->free = axis2_description_servicegroup_ops_free;
	ops->set_name = axis2_description_servicegroup_ops_set_name;
	ops->get_name = axis2_description_servicegroup_ops_get_name;
	ops->add_service = axis2_description_servicegroup_ops_add_service;
	ops->get_service = axis2_description_servicegroup_ops_get_service;
	
	servicegroup_desc->ops = ops;
	
	axis2_description_param_include_t *param_include 
		= (axis2_description_param_include_t *)
		axis2_description_param_include_create(env);		
	if(!param_include)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}

	servicegroup_desc->param_include = param_include;
	
	/*servicegroup_desc->parent = NULL;*/
	
	servicegroup_desc->services = axis2_hash_make (env);				
	if(!servicegroup_desc->services)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;			
	}
	servicegroup_desc->servicegroup_name = NULL;

	return servicegroup_desc;	
}

/******************************************************************************/

axis2_status_t axis2_description_servicegroup_ops_free(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc)
{
	if(!env || !servicegroup_desc)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	AXIS2_FREE(env->allocator, servicegroup_desc);
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_description_servicegroup_ops_set_name
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, axis2_char_t *name)
{
	if(!env || !servicegroup_desc)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	axis2_char_t *tempname = axis2_strdup(name);
	if(!tempname) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	servicegroup_desc->servicegroup_name = name;
	return AXIS2_SUCCESS;
}

axis2_char_t *axis2_description_servicegroup_ops_get_name
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc)
{
	if(!env || !servicegroup_desc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return servicegroup_desc->servicegroup_name;
}

axis2_status_t axis2_description_servicegroup_ops_add_service
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, axis2_description_service_t *service_desc)
{
	if(!env || servicegroup_desc || service_desc)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	axis2_qname_t *tempname = axis2_description_service_get_name(env, service_desc);
	axis2_hash_set (servicegroup_desc->services, tempname->localpart
		, AXIS2_HASH_KEY_STRING, service_desc);
	return AXIS2_SUCCESS;
}

axis2_description_service_t *axis2_description_servicegroup_ops_get_service
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_qname_t* service_name)
{
	if(!env || !servicegroup_desc || ! service_name)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return (axis2_description_service_t *) (axis2_hash_get 
		(servicegroup_desc->services, service_name->localpart
		, AXIS2_HASH_KEY_STRING));
}

axis2_status_t axis2_description_servicegroup_ops_remove_service
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_qname_t* service_name)
{
	if(!env || !servicegroup_desc || ! service_name)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	axis2_hash_set (servicegroup_desc->services, service_name->localpart
		, AXIS2_HASH_KEY_STRING, NULL);
	
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_description_servicegroup_ops_add_param
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc
		, axis2_description_param_t *param)
{
	if(!env || !servicegroup_desc || !servicegroup_desc->param_include || !param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	axis2_hash_set (axis2_description_param_include_get_params(env
		, servicegroup_desc->param_include), axis2_description_param_get_name(env
		, param)
		, AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
}

axis2_description_param_t *axis2_description_servicegroup_ops_get_param(
		axis2_env_t *env, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_char_t *name)
{
	if(!env || !servicegroup_desc || !servicegroup_desc->param_include)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	axis2_char_t *tempname = axis2_strdup(name);
	if(!tempname)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
		
	return (axis2_description_param_t *)(axis2_hash_get 
		(axis2_description_param_include_get_params(env
		, servicegroup_desc->param_include), tempname, AXIS2_HASH_KEY_STRING));
	
}

axis2_hash_t *axis2_description_servicegroup_ops_get_params
		(axis2_env_t *env
		, axis2_description_servicegroup_t *servicegroup_desc)
{
	if(!env || !servicegroup_desc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;		
	}
	return axis2_description_param_include_get_params(env
		, servicegroup_desc->param_include);
	
}

axis2_bool_t axis2_description_servicegroup_ops_is_param_locked(
		axis2_env_t *env, axis2_description_servicegroup_t *servicegroup_desc
		, const axis2_char_t *param_name)
{
	if(!env || !servicegroup_desc || !servicegroup_desc->param_include)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return AXIS2_FALSE;
	}
	axis2_char_t *tempname = axis2_strdup(param_name);
	if(!tempname)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return AXIS2_FALSE;
	}
		
	return axis2_description_param_include_is_param_locked (env
		, servicegroup_desc->param_include, param_name); 
	
}
