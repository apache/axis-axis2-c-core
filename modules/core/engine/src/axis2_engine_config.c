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

#include <string.h>

#include <axis2_engine_config.h>


/**
  * @struct axis2_engine_config
  * @brief ENGINE engine_config operations
  * This holds the information about engine.
  */
struct axis2_engine_config_s
{
	axis2_engine_config_ops_t *ops;
    axis2_description_param_include_t * param_include;
    axis2_hash_t *service_groups;
};
	
/***************************** Function prototypes ****************************/

axis2_status_t axis2_engine_config_ops_free(axis2_engine_config_t *engine_config
		,axis2_env_t *env);

axis2_status_t axis2_engine_config_ops_add_service_group
		(axis2_engine_config_t *engine_config, axis2_env_t *env
    	, axis2_description_servicegroup_t *service_group_desc);

axis2_description_servicegroup_t *axis2_engine_config_ops_get_servicegroup
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_char_t* servicegroup_name);

axis2_status_t axis2_engine_config_ops_add_service
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_description_service_t* service_desc);

axis2_description_service_t *axis2_engine_config_ops_get_service
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_char_t* service_name);

axis2_status_t axis2_engine_config_ops_remove_service
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_char_t *name);
		
axis2_status_t split_service_name(axis2_env_t *env, axis2_char_t *service_name
		, axis2_char_t **service_name_st);
		

/************************** End of function prototypes ************************/

axis2_engine_config_ops_t *axis2_engine_config_get_ops
	(axis2_env_t *env, axis2_engine_config_t *engine_config)
{
	if(!engine_config)
	{
		/* set error code*/
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	return (axis2_engine_config_ops_t *) engine_config->ops;	
}


axis2_engine_config_t *axis2_engine_config_create 
		(axis2_env_t *env)
{
	axis2_engine_config_t *engine_config
	= (axis2_engine_config_t *) AXIS2_MALLOC (env->allocator
	, sizeof(axis2_engine_config_t));
	if(!engine_config)
	{
		/* set the error code*/
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_engine_config_ops_t *ops
		= (axis2_engine_config_ops_t *) AXIS2_MALLOC (env->allocator
		, sizeof(axis2_engine_config_ops_t));
	if(!ops)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		AXIS2_FREE(env->allocator, engine_config);
		return NULL;		
	}
	ops->free = axis2_engine_config_ops_free;
	ops->add_service_group = axis2_engine_config_ops_add_service_group;
	ops->get_service = axis2_engine_config_ops_get_service;
	ops->remove_service = axis2_engine_config_ops_remove_service;
	
	engine_config->ops = ops;

	axis2_description_param_include_t *param_include 
		= (axis2_description_param_include_t *)
		axis2_description_param_include_create(env);		
	if(!param_include)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}

	engine_config->param_include = param_include;
	
	engine_config->service_groups = NULL;
	
	return engine_config;	
}	

/**********************Start of operation impls********************************/

axis2_status_t axis2_engine_config_ops_free (axis2_engine_config_t *engine_config
		, axis2_env_t *env)
{
	if(engine_config)
	{
		AXIS2_FREE(env->allocator, engine_config);
	}
	return AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED;
}

axis2_status_t axis2_engine_config_ops_add_service_group
		(axis2_engine_config_t *engine_config, axis2_env_t *env
    	, axis2_description_servicegroup_t *servicegroup_desc)
{
	if(!engine_config || !servicegroup_desc)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	
	axis2_char_t *tempname = axis2_description_servicegroup_get_name
			(servicegroup_desc, env);
	axis2_hash_set (engine_config->service_groups
		, tempname
		, AXIS2_HASH_KEY_STRING, servicegroup_desc);	
	return AXIS2_SUCCESS;
}

axis2_description_servicegroup_t *axis2_engine_config_ops_get_servicegroup
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_char_t* servicegroup_name)
{
	if(!engine_config)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	axis2_char_t *tempname = axis2_strdup(servicegroup_name);
	if(!tempname)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return (axis2_description_servicegroup_t *) (axis2_hash_get 
		(engine_config->service_groups, servicegroup_name
		, AXIS2_HASH_KEY_STRING));
}

axis2_status_t axis2_engine_config_ops_add_service
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_description_service_t *service_desc)
{
	if(!engine_config || !service_desc) 
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	axis2_description_servicegroup_t *servicegroup_desc 
		= axis2_description_servicegroup_create(env);
	
	if(!servicegroup_desc)
		return AXIS2_ERROR_NO_MEMORY;
	
	axis2_qname_t *servicegroup_qname = axis2_description_service_get_name
		(service_desc, env);
	
	axis2_char_t *servicegroup_name = servicegroup_qname->localpart;
	
	axis2_description_servicegroup_set_name(servicegroup_desc, env
		, servicegroup_name);
	
	axis2_description_servicegroup_add_service(servicegroup_desc, env
		, service_desc);
	
	return AXIS2_SUCCESS;
}

axis2_description_service_t *axis2_engine_config_ops_get_service
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_char_t* service_name)
{
	if(!env || !engine_config)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	axis2_char_t *tempname = axis2_strdup(service_name);
	if(!tempname)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	
	axis2_description_servicegroup_t *sg = NULL;
	int len = strlen(service_name);
	axis2_char_t *service_st[2];
	service_st[1] = (axis2_char_t*) AXIS2_MALLOC
		(env->allocator, len * sizeof(axis2_char_t));
	service_st[2] = (axis2_char_t*) AXIS2_MALLOC
		(env->allocator, len * sizeof(axis2_char_t));
	if(AXIS2_SUCCESS == split_service_name(env, service_name, service_st))
	{
		axis2_char_t *grp_name = *(service_st + 2);
		sg = axis2_engine_config_ops_get_servicegroup(engine_config, env
			, grp_name);
		if(!sg)
			return NULL;
	}
	axis2_char_t *srv_name = *(service_st + 1);
	axis2_qname_t *qname = (axis2_qname_t*) axis2_qname_create(env, srv_name, NULL, NULL); 
	
	return axis2_description_servicegroup_get_service(sg, env, qname);
	return NULL;
			
}

axis2_status_t axis2_engine_config_ops_remove_service
		(axis2_engine_config_t *engine_config, axis2_env_t *env
		, const axis2_char_t *service_name)
{
	if(!env || !engine_config)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	axis2_char_t *tempname = axis2_strdup(service_name);
	if(!tempname)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	axis2_description_servicegroup_t *sg = NULL;
	int len = strlen(service_name);
	axis2_char_t *service_st[2];
	service_st[1] = (axis2_char_t*) AXIS2_MALLOC
		(env->allocator, len * sizeof(axis2_char_t));
	service_st[2] = (axis2_char_t*) AXIS2_MALLOC
		(env->allocator, len * sizeof(axis2_char_t));
	
	if(AXIS2_SUCCESS == split_service_name(env, service_name, service_st))
	{
		axis2_char_t *grp_name = *(service_st + 2);
		sg = axis2_engine_config_ops_get_servicegroup(engine_config, env
			, grp_name);
		if(!sg)
			return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	axis2_char_t *srv_name = *(service_st + 1);
	axis2_qname_t *qname = (axis2_qname_t*) axis2_qname_create(srv_name, env
		, NULL, NULL);
	
	axis2_description_servicegroup_remove_service(sg, env, qname);
	
	return AXIS2_SUCCESS;
}
	
/**
 * To split a given service name into it's service group name and service name.
 * if the service name is foo:bar then service group name is "foo" and 
 * service name is "bar" but if the service name is only the "foo" we asume 
 * service group name is "foo" and service name is "foo"
 * meaning foo := foo:foo
 * @param service_name
 * @return service name and group name 
 */
axis2_status_t split_service_name
		(axis2_env_t *env, axis2_char_t *service_name, axis2_char_t **service_name_st)
{
	if(!service_name_st)
    {
        return AXIS2_ERROR_INVALID_NULL_PARAMETER;
    }
    axis2_char_t *srv_name = strpbrk(service_name, SERVICE_NAME_SPLIT_CHAR);
    if(NULL == srv_name)
    {
        *(service_name_st + 1) = service_name;
        *(service_name_st + 2) = service_name;
        return AXIS2_SUCCESS;
    }
    srv_name[0] = AXIS2_EOLN;
    axis2_char_t *grp_name = AXIS2_MALLOC(env->allocator, strlen(service_name));
    sscanf(service_name, "%s", grp_name);
    srv_name = srv_name + 1;
    *(service_name_st + 1) = srv_name;
    *(service_name_st + 2) = grp_name;
	
    return AXIS2_SUCCESS;	
}
