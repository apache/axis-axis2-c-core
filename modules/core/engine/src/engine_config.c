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

typedef struct axis2_engine_config_impl axis2_engine_config_impl_t;

/** 
 * @brief Engine configuration struct impl
 * Axis2 Engine Configuration impl  
 */  
struct axis2_engine_config_impl
{
	axis2_engine_config_t engine_config;
    axis2_hash_t *svc_grps;
    axis2_hash_t *transports_in;
    axis2_hash_t *transports_out;
    /**
     * Field modules
     */
    axis2_array_list_t *engaged_modules;
    /**
     * Field modules
     */
    axis2_hash_t *modules;
    /** private ArrayList inPhases; */
    axis2_array_list_t *out_phases;
    axis2_array_list_t *in_fault_phases;
    axis2_array_list_t *out_fault_phases;

    axis2_array_list_t *in_phases_upto_and_including_post_dispatch;

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
    	                            struct axis2_svc_grp *svc_grp);

struct axis2_svc_grp * AXIS2_CALL 
axis2_engine_config_get_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
		                            const axis2_char_t *svc_grp_name);

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_svc_grps(axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        struct axis2_svc *svc);

struct axis2_svc * 
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
		
struct axis2_transport_in_desc * AXIS2_CALL
axis2_engine_config_get_transport_in(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_engine_config_add_transport_in(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        struct axis2_transport_in_desc *transport);

struct axis2_transport_out_desc * AXIS2_CALL
axis2_engine_config_get_transport_out(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_engine_config_add_transport_out(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        struct axis2_transport_out_desc *transport);

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_transports_in(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_transports_out(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);	
                                        
struct axis2_module_desc *AXIS2_CALL
axis2_engine_config_get_module(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_engaged_modules(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_in_phases_upto_and_including_post_dispatch(
                                            axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_outflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_in_fault_flow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_out_fault_flow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);                                        
                                        
/************************** End of function prototypes ************************/

axis2_engine_config_t * AXIS2_CALL 
axis2_engine_config_create (axis2_env_t **env)
{
    axis2_engine_config_impl_t *config_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
	config_impl
		= (axis2_engine_config_impl_t *) AXIS2_MALLOC ((*env)->allocator
		, sizeof(axis2_engine_config_impl_t));
	
	if(NULL == config_impl)
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    config_impl->svc_grps = NULL;
    config_impl->modules = NULL;
    config_impl->engaged_modules = NULL;
    config_impl->in_phases_upto_and_including_post_dispatch = NULL;
    config_impl->out_phases = NULL;
    config_impl->in_fault_phases = NULL;
    config_impl->out_fault_phases = NULL;
    
    config_impl->engine_config.ops = NULL;
	
    config_impl->engine_config.param_container = (axis2_param_container_t *) 
        axis2_param_container_create(env);		
	if(NULL == config_impl->engine_config.param_container)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    config_impl->transports_in = axis2_hash_make(env);		
	if(NULL == config_impl->transports_in)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}

	config_impl->transports_out = axis2_hash_make(env);		
	if(NULL == config_impl->transports_out)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    config_impl->modules = axis2_hash_make(env);		
	if(NULL == config_impl->modules)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    config_impl->engaged_modules = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->engaged_modules)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    config_impl->in_phases_upto_and_including_post_dispatch = 
        axis2_array_list_create(env, 0);		
	if(NULL == config_impl->in_phases_upto_and_including_post_dispatch)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    config_impl->out_phases = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->out_phases)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    config_impl->in_fault_phases = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->in_fault_phases)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    config_impl->out_fault_phases = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->out_fault_phases)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
		
    config_impl->engine_config.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_engine_config_ops_t));
    if(NULL == config_impl->engine_config.ops)
    {
        axis2_engine_config_free(&(config_impl->engine_config), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
    }
    
	config_impl->engine_config.ops->free = axis2_engine_config_free;
	config_impl->engine_config.ops->add_svc_grp = 
        axis2_engine_config_add_svc_grp;
	config_impl->engine_config.ops->get_svc_grp = 
        axis2_engine_config_get_svc_grp;
    
    config_impl->engine_config.ops->get_svc_grps = 
        axis2_engine_config_get_svc_grps;
    
	config_impl->engine_config.ops->add_svc = axis2_engine_config_add_svc;
	config_impl->engine_config.ops->get_svc = axis2_engine_config_get_svc;
	config_impl->engine_config.ops->remove_svc = 
        axis2_engine_config_remove_svc;
    config_impl->engine_config.ops->add_param = 
        axis2_engine_config_add_param;
	config_impl->engine_config.ops->get_param = 
        axis2_engine_config_get_param;
	config_impl->engine_config.ops->get_params = 
        axis2_engine_config_get_params;
    config_impl->engine_config.ops->is_param_locked = 
            axis2_engine_config_is_param_locked;
    
    config_impl->engine_config.ops->get_transport_in = 
            axis2_engine_config_get_transport_in;
      
    config_impl->engine_config.ops->add_transport_in = 
            axis2_engine_config_add_transport_in;    
     
    config_impl->engine_config.ops->get_transport_out = 
            axis2_engine_config_get_transport_out;
            
    config_impl->engine_config.ops->add_transport_out = 
            axis2_engine_config_add_transport_out;

    config_impl->engine_config.ops->get_transports_in = 
            axis2_engine_config_get_transports_in;
            
    config_impl->engine_config.ops->get_transports_out = 
            axis2_engine_config_get_transports_out;
	
    config_impl->engine_config.ops->get_module = 
            axis2_engine_config_get_module;
    
    config_impl->engine_config.ops->get_engaged_modules =
    axis2_engine_config_get_engaged_modules;
    
    config_impl->engine_config.ops->get_in_phases_upto_and_including_post_dispatch =
    axis2_engine_config_get_in_phases_upto_and_including_post_dispatch;
    
    config_impl->engine_config.ops->get_outflow =
            axis2_engine_config_get_outflow;
    
    config_impl->engine_config.ops->get_in_fault_flow =
            axis2_engine_config_get_in_fault_flow;
    
    config_impl->engine_config.ops->get_out_fault_flow =
            axis2_engine_config_get_out_fault_flow;
    
	return &(config_impl->engine_config);	
}	

/**********************Start of operation impls********************************/

axis2_status_t AXIS2_CALL 
axis2_engine_config_free (axis2_engine_config_t *engine_config, 
                            axis2_env_t **env)
{
    axis2_engine_config_impl_t *config_impl = NULL;
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    
    if(NULL != engine_config->ops)
		AXIS2_FREE((*env)->allocator, engine_config->ops);
    
	if(NULL != engine_config->param_container)
        AXIS2_PARAM_CONTAINER_FREE(engine_config->param_container, env);
    
    if(config_impl->svc_grps)
        axis2_hash_free(config_impl->svc_grps, env);
    
    if(config_impl->transports_in)
        axis2_hash_free(config_impl->transports_in, env);
    
    if(config_impl->transports_out)
        axis2_hash_free(config_impl->transports_out, env);
    
    if(config_impl->modules)
        axis2_hash_free(config_impl->modules, env);
    
    if(config_impl->engaged_modules)
        AXIS2_ARRAY_LIST_FREE(config_impl->engaged_modules, env);
    
    if(config_impl->out_phases)
        AXIS2_ARRAY_LIST_FREE(config_impl->out_phases, env);
    
    if(config_impl->in_fault_phases)
        AXIS2_ARRAY_LIST_FREE(config_impl->in_fault_phases, env);
    
    if(config_impl->out_fault_phases)
        AXIS2_ARRAY_LIST_FREE(config_impl->out_fault_phases, env);
    
    if(config_impl->in_phases_upto_and_including_post_dispatch)
        AXIS2_ARRAY_LIST_FREE(config_impl->
            in_phases_upto_and_including_post_dispatch, env);
    
	AXIS2_FREE((*env)->allocator, engine_config);
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
    	                            struct axis2_svc_grp *svc_grp)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);

	axis2_char_t *tempname = AXIS2_SVC_GRP_GET_NAME(svc_grp, env);
	axis2_hash_set (AXIS2_INTF_TO_IMPL(engine_config)->svc_grps, tempname, 
        AXIS2_HASH_KEY_STRING, svc_grp);
	
	return AXIS2_SUCCESS;
}

struct axis2_svc_grp * AXIS2_CALL 
axis2_engine_config_get_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
		                            const axis2_char_t *svc_grp_name)
{
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, svc_grp_name, NULL);
    
	return (struct axis2_svc_grp *) (axis2_hash_get (
        AXIS2_INTF_TO_IMPL(engine_config)->svc_grps, svc_grp_name, 
            AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_svc_grps(axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->svc_grps;
}

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env, 
                                struct axis2_svc *svc)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
	
	struct axis2_svc_grp *svc_grp = axis2_svc_grp_create(env);
	
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

struct axis2_svc * AXIS2_CALL 
axis2_engine_config_get_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        const axis2_char_t* svc_name)
{
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, NULL);
	
	struct axis2_svc_grp *sg = NULL;
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
	
	struct axis2_svc_grp *sg = NULL;
        
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
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
	if(NULL == engine_config->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
        return AXIS2_FAILURE;
	}
	
    if(AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(engine_config->param_container, env, 
        AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    } else
    {
        status = AXIS2_PARAM_CONTAINER_ADD_PARAM(engine_config->param_container, env, param);
    }
    return status;
}

axis2_param_t * AXIS2_CALL
axis2_engine_config_get_param (axis2_engine_config_t *engine_config, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
    
	if(NULL == engine_config->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, NULL);
        return NULL;
	}
		
	return AXIS2_PARAM_CONTAINER_GET_PARAM(engine_config->param_container, env,
        name);
	
}

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_params (axis2_engine_config_t *engine_config, 
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(engine_config->param_container, env);
	
}

axis2_bool_t AXIS2_CALL
axis2_engine_config_is_param_locked (axis2_engine_config_t *engine_config, 
                            axis2_env_t **env,
		                    const axis2_char_t *param_name)
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FALSE);
    if(NULL == engine_config->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FALSE);
	}
	
	axis2_char_t *tempname = AXIS2_STRDUP(param_name, env);
	if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM,
        AXIS2_FAILURE);
		
	return AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED
		(engine_config->param_container, env, param_name); 
	
}

struct axis2_transport_in_desc * AXIS2_CALL
axis2_engine_config_get_transport_in(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname)
{
    axis2_engine_config_impl_t *config_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    
    return (struct axis2_transport_in_desc *) axis2_hash_get(config_impl->transports_in,
            qname, sizeof(axis2_qname_t));
}

/**
 * Method addTransport
 *
 * @param transport
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_add_transport_in(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        struct axis2_transport_in_desc *transport)
{
    axis2_engine_config_impl_t *config_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, transport, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    axis2_hash_set(config_impl->transports_in,
        AXIS2_TRANSPORT_IN_DESC_GET_QNAME(transport, env), sizeof(axis2_qname_t), 
            transport);
    
    return AXIS2_SUCCESS;
    
}

struct axis2_transport_out_desc * AXIS2_CALL
axis2_engine_config_get_transport_out(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname)
{
    axis2_engine_config_impl_t *config_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    
    return (struct axis2_transport_out_desc *) axis2_hash_get(config_impl->transports_out,
            qname, sizeof(axis2_qname_t));
}

/**
 * Method addTransport
 *
 * @param transport
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_add_transport_out(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        struct axis2_transport_out_desc *transport)
{
    axis2_engine_config_impl_t *config_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, transport, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    axis2_hash_set(config_impl->transports_out,
        AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(transport, env),
            sizeof(axis2_qname_t), transport);
    return AXIS2_SUCCESS;
    
}

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_transports_in(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->transports_in;
}

/**
 * Method getModule
 *
 * @param name
 * @return ModuleDescription
 */
struct axis2_module_desc *AXIS2_CALL
axis2_engine_config_get_module(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    return (struct axis2_module_desc *) axis2_hash_get(AXIS2_INTF_TO_IMPL(
        engine_config)->modules, qname, sizeof(axis2_qname_t));
}

/**
 * Method getEngadgedModules
 *
 * @return  Collection
 */
axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_engaged_modules(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->engaged_modules;
}

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_in_phases_upto_and_including_post_dispatch(
                                            axis2_engine_config_t *engine_config,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->
        in_phases_upto_and_including_post_dispatch;
}

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_outflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->out_phases;
}


/**
 * @return ArrayList
 */
axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_in_fault_flow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->in_fault_phases;
}

/**
 * @return ArrayList
 */
axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_out_fault_flow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->out_fault_phases;
}

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_transports_out(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->transports_out;
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
