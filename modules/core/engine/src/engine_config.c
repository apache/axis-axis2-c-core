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
    axis2_array_list_t *outphases;
    axis2_array_list_t *in_faultphases;
    axis2_array_list_t *out_faultphases;

    axis2_array_list_t *inphases_upto_and_including_post_dispatch;
    struct axis2_phases_info *phases_info;
    axis2_hash_t *all_svcs;
    axis2_hash_t *msg_recvs;
    axis2_hash_t *faulty_svcs;
    axis2_hash_t *faulty_modules;

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
		                            axis2_char_t *svc_grp_name);

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_svc_grps(axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        struct axis2_svc *svc);

struct axis2_svc * AXIS2_CALL 
axis2_engine_config_get_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
                                axis2_char_t* svc_name);

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
		                                axis2_char_t *param_name);
                            
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
                axis2_char_t *svc_name, 
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
axis2_engine_config_get_inphases_upto_and_including_post_dispatch(
                                            axis2_engine_config_t *engine_config,
                                            axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_outflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_in_faultflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_out_faultflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env);                                        
 
axis2_hash_t *AXIS2_CALL
axis2_engine_config_get_faulty_svcs(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_engine_config_get_faulty_modules(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env);
    
/*to get all the services in the system */
axis2_hash_t *AXIS2_CALL
axis2_engine_config_get_svcs(axis2_engine_config_t *engine_config,
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_engine_config_is_engaged(axis2_engine_config_t *engine_config,
                                axis2_env_t **env,
                                axis2_qname_t *module_name);

struct axis2_phases_info *AXIS2_CALL
axis2_engine_config_get_phases_info(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_engine_config_set_phases_info(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    struct axis2_phases_info *phases_info);
axis2_status_t AXIS2_CALL
axis2_engine_config_add_msg_recv(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    axis2_char_t *key,
                                    struct axis2_msg_recv *msg_recv);

struct axis2_msg_recv *AXIS2_CALL
axis2_engine_config_get_msg_recv(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_engine_config_set_outphases(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    axis2_array_list_t *outphases);

    /**
 * @param list
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_set_in_faultphases(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list);

/**
 * @param list
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_set_out_faultphases(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list);
    
/**
 * @return HashMap
 */
axis2_hash_t *AXIS2_CALL
axis2_engine_config_get_modules(axis2_engine_config_t *engine_config,
                                axis2_env_t **env); 

/**
 * Method addMdoule
 *
 * @param module
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_add_module(axis2_engine_config_t *engine_config,
                                axis2_env_t **env,
                                struct axis2_module_desc *module);
 
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
	
    config_impl->engine_config.param_container = NULL;
    config_impl->svc_grps = NULL;
    config_impl->modules = NULL;
    config_impl->engaged_modules = NULL;
    config_impl->inphases_upto_and_including_post_dispatch = NULL;
    struct axis2_phase *phase = NULL;
    config_impl->outphases = NULL;
    config_impl->in_faultphases = NULL;
    config_impl->out_faultphases = NULL;
    config_impl->phases_info = NULL;
    config_impl->all_svcs = NULL;
    config_impl->msg_recvs = NULL;
    config_impl->faulty_svcs = NULL;
    config_impl->faulty_modules = NULL;
    config_impl->engine_config.ops = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    config_impl->engine_config.param_container = (axis2_param_container_t *) 
        axis2_param_container_create(env);		
	if(NULL == config_impl->engine_config.param_container)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    config_impl->transports_in = axis2_hash_make(env);		
	if(NULL == config_impl->transports_in)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}

	config_impl->transports_out = axis2_hash_make(env);		
	if(NULL == config_impl->transports_out)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    config_impl->modules = axis2_hash_make(env);		
	if(NULL == config_impl->modules)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    config_impl->engaged_modules = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->engaged_modules)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    config_impl->inphases_upto_and_including_post_dispatch = 
        axis2_array_list_create(env, 0);		
	if(NULL == config_impl->inphases_upto_and_including_post_dispatch)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    else
    {
        phase = axis2_phase_create(env, AXIS2_PHASE_TRANSPORTIN);
        if(NULL == phase)
        {
            axis2_engine_config_free(&(config_impl->engine_config), env);
            return NULL;
        }
        status = AXIS2_ARRAY_LIST_ADD(config_impl->
            inphases_upto_and_including_post_dispatch, env, phase);
        if(AXIS2_FAILURE == status)
        {
            axis2_engine_config_free(&(config_impl->engine_config), env);
            return NULL;
            
        }
        phase = axis2_phase_create(env, AXIS2_PHASE_PRE_DISPATCH);
        if(NULL == phase)
        {
            axis2_engine_config_free(&(config_impl->engine_config), env);
            return NULL;
        }
        status = AXIS2_ARRAY_LIST_ADD(config_impl->
            inphases_upto_and_including_post_dispatch, env, phase);
        if(AXIS2_FAILURE == status)
        {
            axis2_engine_config_free(&(config_impl->engine_config), env);
            return NULL;   
        }
    }
    
    config_impl->outphases = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->outphases)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    config_impl->in_faultphases = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->in_faultphases)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
    
    config_impl->out_faultphases = axis2_array_list_create(env, 0);		
	if(NULL == config_impl->out_faultphases)
	{
        axis2_engine_config_free(&(config_impl->engine_config), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
	}
	
    config_impl->msg_recvs = axis2_hash_make(env);
    if(NULL == config_impl->msg_recvs)
    {
        axis2_engine_config_free(&(config_impl->engine_config), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 

    config_impl->faulty_svcs = axis2_hash_make(env);
    if(NULL == config_impl->faulty_svcs)
    {
        axis2_engine_config_free(&(config_impl->engine_config), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 

    config_impl->faulty_modules = axis2_hash_make(env);
    if(NULL == config_impl->faulty_modules)
    {
        axis2_engine_config_free(&(config_impl->engine_config), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    
    config_impl->engine_config.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_engine_config_ops_t));
    if(NULL == config_impl->engine_config.ops)
    {
        axis2_engine_config_free(&(config_impl->engine_config), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
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
    
    config_impl->engine_config.ops->get_inphases_upto_and_including_post_dispatch =
            axis2_engine_config_get_inphases_upto_and_including_post_dispatch;
    
    config_impl->engine_config.ops->get_outflow =
            axis2_engine_config_get_outflow;
    
    config_impl->engine_config.ops->get_in_faultflow =
            axis2_engine_config_get_in_faultflow;
    
    config_impl->engine_config.ops->get_out_faultflow =
            axis2_engine_config_get_out_faultflow;

    config_impl->engine_config.ops->get_faulty_svcs =
            axis2_engine_config_get_faulty_svcs;
    
    config_impl->engine_config.ops->get_faulty_modules =
            axis2_engine_config_get_faulty_modules;
    
    config_impl->engine_config.ops->get_svcs =
            axis2_engine_config_get_svcs;
    
    config_impl->engine_config.ops->is_engaged =
            axis2_engine_config_is_engaged;
    
    config_impl->engine_config.ops->get_phases_info =
            axis2_engine_config_get_phases_info;
    
    config_impl->engine_config.ops->set_phases_info =
            axis2_engine_config_set_phases_info;
    
    config_impl->engine_config.ops->add_msg_recv =
            axis2_engine_config_add_msg_recv;
    
    config_impl->engine_config.ops->get_msg_recv =
            axis2_engine_config_get_msg_recv;
    
    config_impl->engine_config.ops->set_outphases =
            axis2_engine_config_set_outphases;
    
    config_impl->engine_config.ops->set_in_faultphases =
            axis2_engine_config_set_in_faultphases;
    
    config_impl->engine_config.ops->set_out_faultphases =
            axis2_engine_config_set_out_faultphases;
    
    config_impl->engine_config.ops->get_modules =
            axis2_engine_config_get_modules;
    
    config_impl->engine_config.ops->add_module =
            axis2_engine_config_add_module;
    
	return &(config_impl->engine_config);	
}	

/**********************Start of operation impls********************************/

axis2_status_t AXIS2_CALL 
axis2_engine_config_free (axis2_engine_config_t *engine_config, 
                            axis2_env_t **env)
{
    axis2_engine_config_impl_t *config_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    
    if(NULL != engine_config->ops)
    {
		AXIS2_FREE((*env)->allocator, engine_config->ops);
        engine_config->ops = NULL;
    }
    
	if(NULL != engine_config->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(engine_config->param_container, env);
        engine_config->param_container = NULL;
    }
    
    if(config_impl->svc_grps)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (config_impl->svc_grps, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_svc_grp *svc_grp = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            svc_grp = (struct axis2_svc_grp *) val;
            if (svc_grp)
               AXIS2_SVC_GRP_FREE (svc_grp, env);
            
            val = NULL;
            svc_grp = NULL;
               
        }
        axis2_hash_free(config_impl->svc_grps, env);
        config_impl->svc_grps = NULL;
    }
    
    if(config_impl->transports_in)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (config_impl->transports_in, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_transport_in_desc *transport_in = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            transport_in = (struct axis2_transport_in_desc *) val;
            if (transport_in)
               AXIS2_TRANSPORT_IN_DESC_FREE (transport_in, env);
            
            val = NULL;
            transport_in = NULL;
               
        }
        axis2_hash_free(config_impl->transports_in, env);
        config_impl->transports_in = NULL;
    }
    
    if(config_impl->transports_out)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (config_impl->transports_out, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_transport_out_desc *transport_out = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            transport_out = (struct axis2_transport_out_desc *) val;
            if (transport_out)
                AXIS2_TRANSPORT_IN_DESC_FREE (transport_out, env);
            
            val = NULL;
            transport_out = NULL;
               
        }
        axis2_hash_free(config_impl->transports_out, env);
        config_impl->transports_out = NULL;
    }
    
    if(config_impl->modules)
    {
        axis2_hash_free(config_impl->modules, env);
        config_impl->modules = NULL;
    }
    
    if(config_impl->engaged_modules)
    {
        AXIS2_ARRAY_LIST_FREE(config_impl->engaged_modules, env);
        config_impl->engaged_modules = NULL;
    }
    
    if(config_impl->outphases)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(config_impl->outphases, env); i++)
        {
            struct axis2_phase *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->outphases, env, i);
            
            phase = (struct axis2_phase *) val;
            if (phase)
               AXIS2_PHASE_FREE (phase, env);
            
            val = NULL;
            phase = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(config_impl->outphases, env);
        config_impl->outphases = NULL;
    }
    
    if(config_impl->in_faultphases)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(config_impl->in_faultphases, env); i++)
        {
            struct axis2_phase *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->in_faultphases, env, i);
            
            phase = (struct axis2_phase *) val;
            if (phase)
               AXIS2_PHASE_FREE (phase, env);
            
            val = NULL;
            phase = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(config_impl->in_faultphases, env);
        config_impl->in_faultphases = NULL;
    }
    
    if(config_impl->out_faultphases)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(config_impl->out_faultphases, env); i++)
        {
            struct axis2_phase *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->out_faultphases, env, i);
            
            phase = (struct axis2_phase *) val;
            if (phase)
               AXIS2_PHASE_FREE (phase, env);
            
            val = NULL;
            phase = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(config_impl->out_faultphases, env);
        config_impl->out_faultphases = NULL;
    }
    
    if(config_impl->inphases_upto_and_including_post_dispatch)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(config_impl->
                inphases_upto_and_including_post_dispatch, env); i++)
        {
            struct axis2_phase *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->
                inphases_upto_and_including_post_dispatch, env, i);
            
            phase = (struct axis2_phase *) val;
            if (phase)
               AXIS2_PHASE_FREE (phase, env);
            
            val = NULL;
            phase = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(config_impl->
            inphases_upto_and_including_post_dispatch, env);
        config_impl->inphases_upto_and_including_post_dispatch = NULL;
    }
    
    if(config_impl->all_svcs)
    {
        axis2_hash_free(config_impl->all_svcs, env);
        config_impl->all_svcs = NULL;
    }
    
    if(config_impl->msg_recvs)
    {
        axis2_hash_free(config_impl->msg_recvs, env);
        config_impl->msg_recvs = NULL;
    }
    
    if(config_impl->faulty_svcs)
    {
        axis2_hash_free(config_impl->faulty_svcs, env);
        config_impl->faulty_svcs = NULL;
    }
    
    if(config_impl->faulty_modules)
    {
        axis2_hash_free(config_impl->faulty_modules, env);
        config_impl->faulty_modules = NULL;
    }
	AXIS2_FREE((*env)->allocator, config_impl);
    config_impl = NULL;
    
	return status;
}

axis2_status_t AXIS2_CALL 
axis2_engine_config_add_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
    	                            struct axis2_svc_grp *svc_grp)
{
    axis2_engine_config_impl_t *config_impl = NULL;
    axis2_hash_t *svcs = NULL;
    struct axis2_svc *desc = NULL;
    axis2_hash_index_t *index_i = NULL;
    void *value = NULL;
    axis2_char_t *svc_name = NULL;
    axis2_char_t *svc_name2 = NULL;
    axis2_qname_t *module_desc = NULL;
    axis2_char_t *svc_grp_name = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    svcs = AXIS2_SVC_GRP_GET_SVCS(svc_grp, env);
    if(!config_impl->all_svcs)
    {
        config_impl->all_svcs = axis2_hash_make(env);
        if(!config_impl->all_svcs)
            return AXIS2_FAILURE;
    }
    index_i = axis2_hash_first (svcs, env);
    while(index_i)
    {
        axis2_hash_this (index_i, NULL, NULL, &value);
        desc = (struct axis2_svc *) value;
        svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_NAME(desc, env), env);
        
        svc_name2 = axis2_hash_get(config_impl->all_svcs, svc_name, 
                AXIS2_HASH_KEY_STRING);
        if(NULL != svc_name2)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;            
        }
        index_i = axis2_hash_next(env, index_i);
    }
    
    if(svcs)
    {
        axis2_hash_free(svcs, env);
        svcs = NULL;
    }
    
    svcs = AXIS2_SVC_GRP_GET_SVCS(svc_grp, env);
    index_i = axis2_hash_first (svcs, env);
    while(NULL != index_i)
    {
        axis2_hash_this (index_i, NULL, NULL, &value);
        desc = (struct axis2_svc *) value;
        svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_NAME(desc, env), env);
        axis2_hash_set(config_impl->all_svcs, svc_name, AXIS2_HASH_KEY_STRING,
            desc);
        /* notifyObservers(AxisEvent.SERVICE_DEPLOY ,description); */
        index_i = axis2_hash_next(env, index_i);
    }
   
    while(i < AXIS2_ARRAY_LIST_SIZE(config_impl->engaged_modules, env))
    {
        
        module_desc = (axis2_qname_t *) AXIS2_ARRAY_LIST_GET(config_impl->
            engaged_modules, env, i);
        status = AXIS2_SVC_GRP_ADD_MODULE(svc_grp, env, module_desc);
        if(AXIS2_FAILURE == status)
        {
            return AXIS2_FAILURE;
        }
        i++;    
    }

    svc_grp_name = AXIS2_SVC_GRP_GET_NAME(svc_grp, env);
    if(!config_impl->svc_grps)
    {
        config_impl->svc_grps = axis2_hash_make(env);
        if(!config_impl->svc_grps)
            return AXIS2_FAILURE;
    }
    axis2_hash_set (config_impl->svc_grps, svc_grp_name, 
        AXIS2_HASH_KEY_STRING, svc_grp);
	
	return AXIS2_SUCCESS;
}

struct axis2_svc_grp * AXIS2_CALL 
axis2_engine_config_get_svc_grp (axis2_engine_config_t *engine_config, 
                                    axis2_env_t **env,
		                            axis2_char_t *svc_grp_name)
{
    axis2_engine_config_impl_t *config_impl = NULL;
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, svc_grp_name, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    if(!config_impl->svc_grps)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_ENGINE_CONFIG, NULL);
        return NULL;
    }
	return (struct axis2_svc_grp *) (axis2_hash_get (config_impl->svc_grps, 
            svc_grp_name, AXIS2_HASH_KEY_STRING));
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
    struct axis2_svc_grp *svc_grp = NULL;
    axis2_qname_t *svc_grp_qname = NULL;
    axis2_char_t *svc_grp_name = NULL;
    axis2_status_t status = AXIS2_FAILURE;
        
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
	
	svc_grp = axis2_svc_grp_create(env);
	
	if(NULL == svc_grp)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
        return AXIS2_FAILURE;
    }
		
	
	svc_grp_qname = AXIS2_SVC_GET_NAME(svc, env);
    if(NULL == svc_grp_qname)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC, 
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
	
	svc_grp_name = AXIS2_QNAME_GET_LOCALPART(svc_grp_qname, env);
	if(!svc_grp_name)
    {
        return AXIS2_FAILURE;
    }
	status = AXIS2_SVC_GRP_SET_NAME(svc_grp, env, svc_grp_name);
    if(AXIS2_FAILURE == status)
    {
        return status;
    }
	status = AXIS2_SVC_GRP_SET_PARENT(svc_grp, env, engine_config);
    if(AXIS2_FAILURE == status)
    {
        return status;
    }
	status = AXIS2_SVC_GRP_ADD_SVC(svc_grp, env, svc);
    if(AXIS2_FAILURE == status)
    {
        return status;
    }
    status = axis2_engine_config_add_svc_grp(engine_config, env, svc_grp);
	
	return status;
}

struct axis2_svc * AXIS2_CALL 
axis2_engine_config_get_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        axis2_char_t* svc_name)
{   
	AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, NULL);
	
    return axis2_hash_get(AXIS2_INTF_TO_IMPL(engine_config)->all_svcs, svc_name,
        AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL 
axis2_engine_config_remove_svc (axis2_engine_config_t *engine_config, 
                                axis2_env_t **env,
		                        const axis2_char_t *svc_name)
{
    axis2_engine_config_impl_t *config_impl = NULL;
        
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, AXIS2_FAILURE);
    
    axis2_hash_set(config_impl->all_svcs, svc_name, AXIS2_HASH_KEY_STRING,
        NULL);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_engine_config_add_param (axis2_engine_config_t *engine_config, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
	if(AXIS2_TRUE == axis2_engine_config_is_param_locked(engine_config, env, 
        AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
	else
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
		                    axis2_char_t *param_name)
{
    struct axis2_param *param = NULL;
        
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
    param = axis2_engine_config_get_param(engine_config, env, param_name);
    return (NULL != param  && AXIS2_PARAM_IS_LOCKED(param, env));
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
    
    if(!config_impl->transports_in)
    {
        config_impl->transports_in = (axis2_hash_t *) axis2_hash_make(env);
        if(!transports_in)
            return AXIS2_FAILURE;
    }
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
    
    if(!config_impl->transports_in)
    {
        config_impl->transports_out = axis2_hash_make(env);
        if(!transports_out)
            return AXIS2_FAILURE;
    }
    
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
axis2_engine_config_get_inphases_upto_and_including_post_dispatch(
                                            axis2_engine_config_t *engine_config,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->
        inphases_upto_and_including_post_dispatch;
}

axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_outflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->outphases;
}


/**
 * @return ArrayList
 */
axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_in_faultflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->in_faultphases;
}

/**
 * @return ArrayList
 */
axis2_array_list_t * AXIS2_CALL
axis2_engine_config_get_out_faultflow(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->out_faultphases;
}

axis2_hash_t * AXIS2_CALL
axis2_engine_config_get_transports_out(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->transports_out;
}	

/*axis2_status_t 
split_svc_name (axis2_env_t **env, 
                axis2_char_t *svc_name, 
                axis2_char_t **svc_name_st)
{
    axis2_char_t *svc_name_l = NULL;
    axis2_char_t *grp_name = NULL;
	
    svc_name_l = strpbrk(svc_name, SERVICE_NAME_SPLIT_CHAR);
    if(NULL == svc_name_l)
    {
        *(svc_name_st + 1) = AXIS2_STRDUP((*env)->allocator, svc_name);
        *(svc_name_st + 2) = AXIS2_STRDUP((*env)->allocator, svc_name);
        return AXIS2_SUCCESS;
    }
    svc_name_l[0] = AXIS2_EOLN;
    grp_name = AXIS2_MALLOC((*env)->allocator, strlen(svc_name));
    sscanf(svc_name, "%s", grp_name);
    svc_name_l = svc_name_l + 1;
    *(svc_name_st + 1) = AXIS2_STRDUP((*env)->allocator, svc_name_l);
    *(svc_name_st + 2) = grp_name;
	
    return AXIS2_SUCCESS;	
}
*/

axis2_hash_t *AXIS2_CALL
axis2_engine_config_get_faulty_svcs(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(engine_config)->faulty_svcs;
}

axis2_hash_t *AXIS2_CALL 
axis2_engine_config_get_faulty_modules(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    return AXIS2_INTF_TO_IMPL(engine_config)->faulty_modules;
}
    
/*to get all the services in the system */
axis2_hash_t *AXIS2_CALL
axis2_engine_config_get_svcs(axis2_engine_config_t *engine_config,
                                axis2_env_t **env) 
{
    axis2_engine_config_impl_t *config_impl = NULL;
    axis2_hash_t *sgs = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_hash_index_t *index_j = NULL;
    void *value = NULL;
    void *value2 = NULL;
    struct axis2_svc_grp *axis_svc_grp = NULL;
    axis2_hash_t *svcs = NULL;
    struct axis2_svc *svc =NULL;
    axis2_char_t *svc_name = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    sgs = axis2_engine_config_get_svc_grps(engine_config, env);
    index_i = axis2_hash_first(sgs, env);
    while(NULL != index_i)
    {
        axis2_hash_this(index_i, NULL, NULL, &value);
        axis_svc_grp = (struct axis2_svc_grp *) value;
        svcs = AXIS2_SVC_GRP_GET_SVCS(axis_svc_grp, env);
        index_j = axis2_hash_first(svcs, env);
        while(NULL != index_j)
        {
            axis2_hash_this(index_j, NULL, NULL, &value2);
            svc = (struct axis2_svc *) value2;
            svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_NAME(svc, env), env);
            axis2_hash_set(config_impl->all_svcs, svc_name,
                AXIS2_HASH_KEY_STRING, svc);    
                            
            index_j = axis2_hash_next(env, index_j);
        }
        
        index_i = axis2_hash_next(env, index_i);
    }
    return config_impl->all_svcs;
}

axis2_bool_t AXIS2_CALL
axis2_engine_config_is_engaged(axis2_engine_config_t *engine_config,
                                axis2_env_t **env,
                                axis2_qname_t *module_name) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FALSE);
    
    return AXIS2_ARRAY_LIST_CONTAINS(AXIS2_INTF_TO_IMPL(engine_config)->
        engaged_modules, env, module_name);
}

struct axis2_phases_info *AXIS2_CALL
axis2_engine_config_get_phases_info(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(engine_config)->phases_info;
}

axis2_status_t AXIS2_CALL
axis2_engine_config_set_phases_info(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    struct axis2_phases_info *phases_info) 
{
    axis2_engine_config_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, phases_info, AXIS2_FAILURE);
    
    if(config_impl->phases_info)
    {
        AXIS2_PHASES_INFO_FREE(phases_info, env);
        config_impl->phases_info = NULL;
    }
    config_impl->phases_info = phases_info;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_engine_config_add_msg_recv(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    axis2_char_t *key,
                                    struct axis2_msg_recv *msg_recv) 
{
    axis2_engine_config_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_recv, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    
    if(!config_impl->msg_recvs)
    {
        config_impl->msg_recvs = axis2_hash_make(env);
        if(!config_impl->msg_recvs)
            return AXIS2_FAILURE;
    }
    axis2_hash_set(config_impl->msg_recvs, key, AXIS2_HASH_KEY_STRING, msg_recv);
    return AXIS2_SUCCESS;
}

struct axis2_msg_recv *AXIS2_CALL
axis2_engine_config_get_msg_recv(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    axis2_char_t *key) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    
    return (struct axis2_msg_recv *) axis2_hash_get(AXIS2_INTF_TO_IMPL(
        engine_config)->msg_recvs, key, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_engine_config_set_outphases(axis2_engine_config_t *engine_config,
                                    axis2_env_t **env,
                                    axis2_array_list_t *outphases) 
{
    axis2_engine_config_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outphases, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    if(config_impl->outphases)
    {
        AXIS2_ARRAY_LIST_FREE(config_impl->outphases, env);
        config_impl->outphases = NULL;
    }
    config_impl->outphases = outphases;
    return AXIS2_SUCCESS;
}

    /**
 * @param list
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_set_in_faultphases(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list) 
{
    axis2_engine_config_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    if(config_impl->in_faultphases)
    {
        AXIS2_ARRAY_LIST_FREE(config_impl->in_faultphases, env);
        config_impl->in_faultphases = NULL;
    }
    config_impl->in_faultphases = list;
    return AXIS2_SUCCESS;
}

/**
 * @param list
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_set_out_faultphases(axis2_engine_config_t *engine_config,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list) 
{
    axis2_engine_config_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    if(config_impl->out_faultphases)
    {
        AXIS2_ARRAY_LIST_FREE(config_impl->out_faultphases, env);
        config_impl->out_faultphases = NULL;
    }
    config_impl->out_faultphases = list;
    return AXIS2_SUCCESS;
}
    
/**
 * @return HashMap
 */
axis2_hash_t *AXIS2_CALL
axis2_engine_config_get_modules(axis2_engine_config_t *engine_config,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(engine_config)->modules;
}  

/**
 * Method addMdoule
 *
 * @param module
 */
axis2_status_t AXIS2_CALL
axis2_engine_config_add_module(axis2_engine_config_t *engine_config,
                                axis2_env_t **env,
                                struct axis2_module_desc *module) 
{
    axis2_engine_config_impl_t *config_impl = NULL;
    
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_FUNC_PARAM_CHECK(engine_config, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(engine_config);
    
    status = AXIS2_MODULE_DESC_SET_PARENT(module, env, engine_config);
    if(AXIS2_FAILURE == status)
    {
        return status;
    }
    if(!config_impl->modules)
    {
        config_impl->modules = axis2_hash_make(env);
        if(!config_impl->modules)
            return AXIS2_FAILURE;
    }
    axis2_hash_set(config_impl->modules, AXIS2_MODULE_DESC_GET_NAME(module, env), 
        sizeof(axis2_qname_t), module);
    
    return AXIS2_SUCCESS;
}
