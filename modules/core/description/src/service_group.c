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

axis2_status_t axis2_description_servicegroup_ops_free
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env);

axis2_status_t axis2_description_servicegroup_ops_set_name
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, axis2_char_t *name);

axis2_char_t *axis2_description_servicegroup_ops_get_name
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env);

axis2_status_t axis2_description_servicegroup_ops_add_service
		(axis2_description_servicegroup_t* srvgrp_desc, axis2_env_t *env
		, axis2_description_service_t *service_desc);

axis2_description_service_t *axis2_description_servicegroup_ops_get_service
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, const axis2_qname_t* service_name);
		
axis2_status_t axis2_description_servicegroup_ops_remove_service
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, const axis2_qname_t* service_name);
		
axis2_status_t axis2_description_servicegroup_ops_add_param
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, axis2_description_param_t *param);
		

axis2_description_param_t *axis2_description_servicegroup_ops_get_param(
		axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, const axis2_char_t *name);
		
axis2_hash_t *axis2_description_servicegroup_ops_get_params
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env);
		
axis2_bool_t axis2_description_servicegroup_ops_is_param_locked(
		axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, const axis2_char_t *param_name);

/***************************** End of function headers ************************/

axis2_description_servicegroup_ops_t *axis2_description_servicegroup_get_ops
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env)
{
	if(!env || !srvgrp_desc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return srvgrp_desc->ops;
}

axis2_description_servicegroup_t *axis2_description_servicegroup_create
		(axis2_env_t *env)
{
	axis2_description_servicegroup_t *srvgrp_desc 
		= (axis2_description_servicegroup_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_description_servicegroup_t));
	if(!srvgrp_desc)
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
	ops->remove_service = axis2_description_servicegroup_ops_remove_service;
	ops->add_param = axis2_description_servicegroup_ops_add_param;
	ops->get_param = axis2_description_servicegroup_ops_get_param;
	ops->get_params = axis2_description_servicegroup_ops_get_params;
	ops->is_param_locked = axis2_description_servicegroup_ops_is_param_locked;
	
	srvgrp_desc->ops = ops;
	
	axis2_description_param_include_t *param_include 
		= (axis2_description_param_include_t *)
		axis2_description_param_include_create(env);		
	if(!param_include)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}

	srvgrp_desc->param_include = param_include;
	
	/*srvgrp_desc->parent = NULL;*/
	
	srvgrp_desc->services = axis2_hash_make (env);				
	if(!srvgrp_desc->services)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;			
	}
	srvgrp_desc->servicegroup_name = NULL;

	return srvgrp_desc;	
}

/******************************************************************************/

axis2_status_t axis2_description_servicegroup_ops_free
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env)
{
	if(!env || !srvgrp_desc)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	AXIS2_FREE(env->allocator, srvgrp_desc);
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_description_servicegroup_ops_set_name
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, axis2_char_t *name)
{
	if(!env || !srvgrp_desc)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	axis2_char_t *tempname = axis2_strdup(name);
	if(!tempname) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	srvgrp_desc->servicegroup_name = name;
	return AXIS2_SUCCESS;
}

axis2_char_t *axis2_description_servicegroup_ops_get_name
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env)
{
	if(!env || !srvgrp_desc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return srvgrp_desc->servicegroup_name;
}

axis2_status_t axis2_description_servicegroup_ops_add_service
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, axis2_description_service_t *service_desc)
{
	if(!env || srvgrp_desc || service_desc)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	axis2_qname_t *tempname = axis2_description_service_get_name(service_desc, env);
	axis2_hash_set (srvgrp_desc->services, tempname->localpart
		, AXIS2_HASH_KEY_STRING, service_desc);
	return AXIS2_SUCCESS;
}

axis2_description_service_t *axis2_description_servicegroup_ops_get_service
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env 
		, const axis2_qname_t* service_name)
{
	if(!env || !srvgrp_desc || ! service_name)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return (axis2_description_service_t *) (axis2_hash_get 
		(srvgrp_desc->services, service_name->localpart
		, AXIS2_HASH_KEY_STRING));
}

axis2_status_t axis2_description_servicegroup_ops_remove_service
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, const axis2_qname_t* service_name)
{
	if(!env || !srvgrp_desc || ! service_name)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	axis2_hash_set (srvgrp_desc->services, service_name->localpart
		, AXIS2_HASH_KEY_STRING, NULL);
	
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_description_servicegroup_ops_add_param
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, axis2_description_param_t *param)
{
	if(!env || !srvgrp_desc || !srvgrp_desc->param_include || !param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	axis2_hash_set (axis2_description_param_include_get_params
		(srvgrp_desc->param_include, env), axis2_description_param_get_name
		(param, env)
		, AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
}

axis2_description_param_t *axis2_description_servicegroup_ops_get_param(
		axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, const axis2_char_t *name)
{
	if(!env || !srvgrp_desc || !srvgrp_desc->param_include)
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
		(axis2_description_param_include_get_params
		(srvgrp_desc->param_include, env), tempname, AXIS2_HASH_KEY_STRING));
	
}

axis2_hash_t *axis2_description_servicegroup_ops_get_params
		(axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env)
{
	if(!env || !srvgrp_desc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;		
	}
	return axis2_description_param_include_get_params(srvgrp_desc->param_include
		, env);
	
}

axis2_bool_t axis2_description_servicegroup_ops_is_param_locked(
		axis2_description_servicegroup_t *srvgrp_desc, axis2_env_t *env
		, const axis2_char_t *param_name)
{
	if(!env || !srvgrp_desc || !srvgrp_desc->param_include)
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
		
	return axis2_description_param_include_is_param_locked
		(srvgrp_desc->param_include, env, param_name); 
	
}
