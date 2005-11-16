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

typedef struct axis2_engine_config_impl_s axis2_engine_config_impl_t;

/** 
 * @brief Engine configuration struct impl
 * Axis2 Engine Configuration impl  
 */  
struct axis2_engine_config_impl_s
{
	axis2_engine_config_t engine_config;
    axis2_param_container_t *param_container;
    axis2_hash_t *svc_grps;
};

#define AXIS2_INTF_TO_IMPL(engine_config) \
        ((axis2_engine_config_impl_t *)engine_config)

/***************************** Function prototypes ****************************/

axis2_status_t AXIS2_CALL 
axis2_engine_config_free (axis2_engine_config_t *engine_config, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
    	                            struct axis2_svc_grp_s *svc_grp);

struct axis2_svc_grp_s * AXIS2_CALL 
axis2_engine_config_get_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
		                            const axis2_char_t *svc_grp_name);

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        struct axis2_svc_s *svc);

struct axis2_svc_s * 
AXIS2_CALL axis2_engine_config_get_svc (axis2_engine_config_t *engine_config, 
                                        axis2_env_t **env,
		                                const axis2_char_t* svc_name);

axis2_status_t AXIS2_CALL 
axis2_engine_config_remove_svc (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
		                            const axis2_char_t *name);

axis2_status_t AXIS2_CALL
axis2_engine_config_add_param (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env, 
                                axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_engine_config_get_param (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        const axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_params (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_engine_config_is_param_locked (axis2_engine_config_t *engine_config, 
                                        axis2_env_t **env,
		                                const axis2_char_t *param_name);
                            
/**
 * To split a given svc name into it's svc grp name and svc name.
 * if the svc name is foo:bar then svc grp name is "foo" and 
 * svc name is "bar" but if the svc name is only the "foo" we asume 
 * svc grp name is "foo" and svc name is "foo"
 * meaning foo := foo:foo
 * @param svc_name
 * @return svc name and grp name 
 */
axis2_status_t 
split_svc_name(axis2_env_t **env,
                const axis2_char_t *svc_name, 
                axis2_char_t **svc_name_st);
		

/************************** End of function prototypes ************************/

axis2_engine_config_t * AXIS2_CALL 
axis2_engine_config_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	axis2_engine_config_impl_t *engine_config_impl
		= (axis2_engine_config_impl_t *) AXIS2_MALLOC ((*env)->allocator
		, sizeof(axis2_engine_config_impl_t));
	
	if(NULL == engine_config_impl)
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	
	engine_config_impl->engine_config.ops->free = axis2_engine_config_free;
	engine_config_impl->engine_config.ops->add_svc_grp = 
        axis2_engine_config_add_svc_grp;
	engine_config_impl->engine_config.ops->get_svc_grp = 
        axis2_engine_config_get_svc_grp;
	engine_config_impl->engine_config.ops->add_svc = axis2_engine_config_add_svc;
	engine_config_impl->engine_config.ops->get_svc = axis2_engine_config_get_svc;
	engine_config_impl->engine_config.ops->remove_svc = 
        axis2_engine_config_remove_svc;
    engine_config_impl->engine_config.ops->add_param = 
        axis2_engine_config_add_param;
	engine_config_impl->engine_config.ops->get_param = 
        axis2_engine_config_get_param;
	engine_config_impl->engine_config.ops->get_params = 
        axis2_engine_config_get_params;
    engine_config_impl->engine_config.ops->is_param_locked = 
            axis2_engine_config_is_param_locked;

	axis2_param_container_t *param_container = (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == param_container)
	{
        AXIS2_FREE((*env)->allocator, engine_config_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}

	engine_config_impl->param_container = param_container;
	
	engine_config_impl->svc_grps = NULL;
	
	return &(engine_config_impl->engine_config);	
}	

/**********************Start of operation impls********************************/

axis2_status_t AXIS2_CALL 
axis2_engine_config_free (axis2_engine_config_t *engine_config, 
                            axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    
    if(NULL != engine_config->ops)
		AXIS2_FREE((*env)->allocator, engine_config->ops);
    
	if(NULL != AXIS2_INTF_TO_IMPL(engine_config)->param_container)
        AXIS2_PARAM_CONTAINER_FREE(AXIS2_INTF_TO_IMPL(engine_config)->
            param_container, env);
    
    axis2_hash_free(AXIS2_INTF_TO_IMPL(engine_config)->svc_grps, env);
    
	AXIS2_FREE((*env)->allocator, engine_config);
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
    	                            struct axis2_svc_grp_s *svc_grp)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);

	axis2_char_t *tempname = AXIS2_SVC_GRP_GET_NAME(svc_grp, env);
	axis2_hash_set (AXIS2_INTF_TO_IMPL(engine_config)->svc_grps, tempname, 
        AXIS2_HASH_KEY_STRING, svc_grp);
	
	return AXIS2_SUCCESS;
}

struct axis2_svc_grp_s * AXIS2_CALL 
axis2_engine_config_get_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
		                            const axis2_char_t *svc_grp_name)
{
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, svc_grp_name, NULL);
    
	return (struct axis2_svc_grp_s *) (axis2_hash_get (
        AXIS2_INTF_TO_IMPL(engine_config)->svc_grps, svc_grp_name, 
            AXIS2_HASH_KEY_STRING));
}

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env, 
                                struct axis2_svc_s *svc)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
	
	struct axis2_svc_grp_s *svc_grp = axis2_svc_grp_create(env);
	
	if(NULL == svc_grp)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
    }
		
	
	axis2_qname_t *svc_grp_qname = AXIS2_SVC_GET_NAME(svc, env);
    if(NULL == svc_grp_qname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC, 
            AXIS2_FAILURE);
	
	axis2_char_t *svc_grp_name = AXIS2_QNAME_GET_LOCALPART(svc_grp_qname, env);
	
	AXIS2_SVC_GRP_SET_NAME(svc_grp, env, svc_grp_name);
	
	AXIS2_SVC_GRP_ADD_SVC(svc_grp, env, svc);
	
	return AXIS2_SUCCESS;
}

struct axis2_svc_s * AXIS2_CALL 
axis2_engine_config_get_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        const axis2_char_t* svc_name)
{
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, NULL);
	
	struct axis2_svc_grp_s *sg = NULL;
	int len = strlen(svc_name);
	axis2_char_t *svc_st[2];
	svc_st[1] = (axis2_char_t*) AXIS2_MALLOC((*env)->allocator,
        len * sizeof(axis2_char_t));
	svc_st[2] = (axis2_char_t*) AXIS2_MALLOC((*env)->allocator, 
        len * sizeof(axis2_char_t));
    
	if(AXIS2_SUCCESS == split_svc_name(env, svc_name, svc_st))
	{
		axis2_char_t *grp_name = *(svc_st + 2);
		sg = AXIS2_ENGINE_CONFIG_GET_SVC_GRP(engine_config, env, grp_name);
		if(NULL == sg)
			AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC_GRP, NULL);
	}
	axis2_char_t *svc_name_l = *(svc_st + 1);
	axis2_qname_t *qname = (axis2_qname_t*) axis2_qname_create(env, svc_name_l,
        NULL, NULL); 
	
	return AXIS2_SVC_GRP_GET_SVC(sg, env, qname);		
}

axis2_status_t AXIS2_CALL 
axis2_engine_config_remove_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        const axis2_char_t *svc_name)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, AXIS2_FAILURE);
	
	struct axis2_svc_grp_s *sg = NULL;
        
	int len = strlen(svc_name);
	axis2_char_t *svc_st[2];
	svc_st[1] = (axis2_char_t*) AXIS2_MALLOC((*env)->allocator,
        len * sizeof(axis2_char_t));
	svc_st[2] = (axis2_char_t*) AXIS2_MALLOC((*env)->allocator, 
        len * sizeof(axis2_char_t));
	
	if(AXIS2_SUCCESS == split_svc_name(env, svc_name, svc_st))
	{
		axis2_char_t *grp_name = *(svc_st + 2);
		sg = AXIS2_ENGINE_CONFIG_GET_SVC_GRP(engine_config, env, grp_name);
		if(!sg)
			AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC_GRP, 
                AXIS2_FAILURE);
	}
	axis2_char_t *svc_name_l = *(svc_st + 1);
	axis2_qname_t *qname = (axis2_qname_t*) axis2_qname_create(env, svc_name_l,
		NULL, NULL);
	
	return AXIS2_SVC_GRP_REMOVE_SVC(sg, env, qname);
}

axis2_status_t AXIS2_CALL
axis2_engine_config_add_param (axis2_engine_config_t *engine_config, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
	if(NULL == AXIS2_INTF_TO_IMPL(engine_config)->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
	}
	axis2_hash_set (AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(engine_config)->
        param_container, env), AXIS2_PARAM_GET_NAME(param, env), 
        AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
}

axis2_param_t * AXIS2_CALL
axis2_engine_config_get_param (axis2_engine_config_t *engine_config, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    
	if(NULL == AXIS2_INTF_TO_IMPL(engine_config)->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
	}
    
	axis2_char_t *tempname = AXIS2_STRDUP(name, env);
	if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM,
        AXIS2_FAILURE);
		
	return (axis2_param_t *)(axis2_hash_get 
		(AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(engine_config)->
        param_container, env), tempname, AXIS2_HASH_KEY_STRING));
	
}

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_params (axis2_engine_config_t *engine_config, 
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(engine_config)->
        param_container, env);
	
}

axis2_bool_t AXIS2_CALL
axis2_engine_config_is_param_locked (axis2_engine_config_t *engine_config, 
                            axis2_env_t **env,
		                    const axis2_char_t *param_name)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FALSE);
    if(NULL == AXIS2_INTF_TO_IMPL(engine_config)->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FALSE);
	}
	
	axis2_char_t *tempname = AXIS2_STRDUP(param_name, env);
	if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM,
        AXIS2_FAILURE);
		
	return AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED
		(AXIS2_INTF_TO_IMPL(engine_config)->param_container, env, param_name); 
	
}

	
axis2_status_t 
split_svc_name (axis2_env_t **env, 
                const axis2_char_t *svc_name, 
                axis2_char_t **svc_name_st)
{
	axis2_char_t *svc_name_temp = AXIS2_STRDUP(svc_name, env);
	if(!svc_name_st)
    {
        return AXIS2_ERROR_INVALID_NULL_PARAM;
    }
    axis2_char_t *svc_name_l = strpbrk(svc_name_temp, SERVICE_NAME_SPLIT_CHAR);
    if(NULL == svc_name_l)
    {
        *(svc_name_st + 1) = svc_name_temp;
        *(svc_name_st + 2) = svc_name_temp;
        return AXIS2_SUCCESS;
    }
    svc_name_l[0] = AXIS2_EOLN;
    axis2_char_t *grp_name = AXIS2_MALLOC((*env)->allocator, strlen(svc_name_temp));
    sscanf(svc_name_temp, "%s", grp_name);
    svc_name_l = svc_name_l + 1;
    *(svc_name_st + 1) = svc_name_l;
    *(svc_name_st + 2) = grp_name;
	
    return AXIS2_SUCCESS;	
}
