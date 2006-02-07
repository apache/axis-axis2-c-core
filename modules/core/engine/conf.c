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

#include <axis2_conf.h>
#include <axis2_dir_handler.h>
#include <axis2_dep_engine.h>
#include <axis2_arch_reader.h>


typedef struct axis2_conf_impl axis2_conf_impl_t;

/** 
 * @brief Engine configuration struct impl
 * Axis2 Engine Configuration impl  
 */  
struct axis2_conf_impl
{
	axis2_conf_t conf;
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
    axis2_array_list_t *in_faultphases;
    axis2_array_list_t *out_faultphases;

    axis2_array_list_t *in_phases_upto_and_including_post_dispatch;
    axis2_phases_info_t *phases_info;
    axis2_hash_t *all_svcs;
    axis2_hash_t *msg_recvs;
    axis2_hash_t *faulty_svcs;
    axis2_hash_t *faulty_modules;
    axis2_char_t *axis2_repos;

};

#define AXIS2_INTF_TO_IMPL(conf) ((axis2_conf_impl_t *)conf)

/***************************** Function prototypes ****************************/

axis2_status_t AXIS2_CALL 
axis2_conf_free (axis2_conf_t *conf, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_conf_add_svc_grp (axis2_conf_t *conf, 
                                    axis2_env_t **env,
    	                            axis2_svc_grp_t *svc_grp);

axis2_svc_grp_t * AXIS2_CALL 
axis2_conf_get_svc_grp (axis2_conf_t *conf, 
                                    axis2_env_t **env,
		                            axis2_char_t *svc_grp_name);

axis2_hash_t * AXIS2_CALL
axis2_conf_get_svc_grps(axis2_conf_t *conf, 
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_conf_add_svc (axis2_conf_t *conf, 
                                axis2_env_t **env,
		                        axis2_svc_t *svc);

axis2_svc_t * AXIS2_CALL 
axis2_conf_get_svc (axis2_conf_t *conf, 
                                axis2_env_t **env,
                                axis2_char_t* svc_name);

axis2_status_t AXIS2_CALL 
axis2_conf_remove_svc (axis2_conf_t *conf, 
                                    axis2_env_t **env,
		                            const axis2_char_t *name);

axis2_status_t AXIS2_CALL
axis2_conf_add_param (axis2_conf_t *conf, 
                                axis2_env_t **env, 
                                axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_conf_get_param (axis2_conf_t *conf, 
                                axis2_env_t **env,
		                        const axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_params (axis2_conf_t *conf, 
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_conf_is_param_locked (axis2_conf_t *conf, 
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
static axis2_status_t 
split_svc_name(axis2_env_t **env,
                axis2_char_t *svc_name, 
                axis2_char_t **svc_name_st);
		
axis2_transport_in_desc_t * AXIS2_CALL
axis2_conf_get_transport_in(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_conf_add_transport_in(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_transport_in_desc_t *transport);

axis2_transport_out_desc_t * AXIS2_CALL
axis2_conf_get_transport_out(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_conf_add_transport_out(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_transport_out_desc_t *transport);

axis2_hash_t * AXIS2_CALL
axis2_conf_get_transports_in(axis2_conf_t *conf,
                                        axis2_env_t **env);

axis2_hash_t * AXIS2_CALL
axis2_conf_get_transports_out(axis2_conf_t *conf,
                                        axis2_env_t **env);	
                                        
axis2_module_desc_t *AXIS2_CALL
axis2_conf_get_module(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname);

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_engaged_modules(axis2_conf_t *conf,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_in_phases_upto_and_including_post_dispatch(
                                            axis2_conf_t *conf,
                                            axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_outflow(axis2_conf_t *conf,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_in_faultflow(axis2_conf_t *conf,
                                        axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_out_faultflow(axis2_conf_t *conf,
                                        axis2_env_t **env);                                        
 
axis2_hash_t *AXIS2_CALL
axis2_conf_get_faulty_svcs(axis2_conf_t *conf,
                                    axis2_env_t **env);

axis2_hash_t *AXIS2_CALL 
axis2_conf_get_faulty_modules(axis2_conf_t *conf,
                                    axis2_env_t **env);
    
/*to get all the services in the system */
axis2_hash_t *AXIS2_CALL
axis2_conf_get_svcs(axis2_conf_t *conf,
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_conf_is_engaged(axis2_conf_t *conf,
                                axis2_env_t **env,
                                axis2_qname_t *module_name);

axis2_phases_info_t *AXIS2_CALL
axis2_conf_get_phases_info(axis2_conf_t *conf,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_conf_set_phases_info(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_phases_info_t *phases_info);
axis2_status_t AXIS2_CALL
axis2_conf_add_msg_recv(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_char_t *key,
                                    axis2_msg_recv_t *msg_recv);

axis2_msg_recv_t *AXIS2_CALL
axis2_conf_get_msg_recv(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_conf_set_out_phases(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_array_list_t *out_phases);

axis2_array_list_t* AXIS2_CALL
axis2_conf_get_out_phases(axis2_conf_t *conf,
                                    axis2_env_t **env);
    /**
 * @param list
 */
axis2_status_t AXIS2_CALL
axis2_conf_set_in_faultphases(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list);

/**
 * @param list
 */
axis2_status_t AXIS2_CALL
axis2_conf_set_out_faultphases(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list);
    
/**
 * @return HashMap
 */
axis2_hash_t *AXIS2_CALL
axis2_conf_get_modules(axis2_conf_t *conf,
                                axis2_env_t **env); 

/**
 * Method addMdoule
 *
 * @param module
 */
axis2_status_t AXIS2_CALL
axis2_conf_add_module(axis2_conf_t *conf,
                                axis2_env_t **env,
                                axis2_module_desc_t *module);

axis2_status_t AXIS2_CALL
axis2_conf_set_default_dispatchers(axis2_conf_t *conf,
                                    axis2_env_t **env); 

axis2_status_t AXIS2_CALL
axis2_conf_set_dispatch_phase(axis2_conf_t *conf,
                                axis2_env_t **env,
                                axis2_phase_t *dispatch);


axis2_status_t AXIS2_CALL
axis2_conf_engage_module(axis2_conf_t *conf,
                        axis2_env_t **env,
                        axis2_qname_t *module_ref);

axis2_char_t *AXIS2_CALL
axis2_conf_get_repos(axis2_conf_t *conf,
                     axis2_env_t **env);
                            
axis2_status_t AXIS2_CALL
axis2_conf_set_repos(axis2_conf_t *conf,
                        axis2_env_t **env,
                        axis2_char_t *repos_path);

/************************** End of function prototypes ************************/

axis2_conf_t * AXIS2_CALL 
axis2_conf_create (axis2_env_t **env)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_phase_t *phase = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
	config_impl = (axis2_conf_impl_t *) AXIS2_MALLOC ((*env)->allocator
		, sizeof(axis2_conf_impl_t));
	
	if(NULL == config_impl)
    {
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
	
    config_impl->conf.param_container = NULL;
    config_impl->svc_grps = NULL;
    config_impl->modules = NULL;
    config_impl->engaged_modules = NULL;
    config_impl->in_phases_upto_and_including_post_dispatch = NULL;
   
    config_impl->out_phases = NULL;
    config_impl->in_faultphases = NULL;
    config_impl->out_faultphases = NULL;
    config_impl->phases_info = NULL;
    config_impl->all_svcs = NULL;
    config_impl->msg_recvs = NULL;
    config_impl->faulty_svcs = NULL;
    config_impl->faulty_modules = NULL;
    config_impl->axis2_repos = NULL;
    config_impl->conf.ops = NULL;
    
    config_impl->conf.param_container = (axis2_param_container_t *) 
        axis2_param_container_create(env);		
	if(NULL == config_impl->conf.param_container)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    config_impl->svc_grps = axis2_hash_make(env);		
	if(NULL == config_impl->svc_grps)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}

    config_impl->transports_in = axis2_hash_make(env);		
	if(NULL == config_impl->transports_in)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
	config_impl->transports_out = axis2_hash_make(env);		
	if(NULL == config_impl->transports_out)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    config_impl->modules = axis2_hash_make(env);		
	if(NULL == config_impl->modules)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    config_impl->engaged_modules = axis2_array_list_create(env, 20);		
	if(NULL == config_impl->engaged_modules)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    config_impl->in_phases_upto_and_including_post_dispatch = 
        axis2_array_list_create(env, 20);		
	if(NULL == config_impl->in_phases_upto_and_including_post_dispatch)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    else
    {
        phase = axis2_phase_create(env, AXIS2_PHASE_TRANSPORTIN);
        if(NULL == phase)
        {
            axis2_conf_free(&(config_impl->conf), env);
            return NULL;
        }
        status = AXIS2_ARRAY_LIST_ADD(config_impl->
            in_phases_upto_and_including_post_dispatch, env, phase);
        if(AXIS2_FAILURE == status)
        {
            axis2_conf_free(&(config_impl->conf), env);
            return NULL;
            
        }
        phase = axis2_phase_create(env, AXIS2_PHASE_PRE_DISPATCH);
        if(NULL == phase)
        {
            axis2_conf_free(&(config_impl->conf), env);
            return NULL;
        }
        status = AXIS2_ARRAY_LIST_ADD(config_impl->
            in_phases_upto_and_including_post_dispatch, env, phase);
        if(AXIS2_FAILURE == status)
        {
            axis2_conf_free(&(config_impl->conf), env);
            return NULL;   
        }
    }
    
    config_impl->out_phases = axis2_array_list_create(env, 20);		
	if(NULL == config_impl->out_phases)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    config_impl->in_faultphases = axis2_array_list_create(env, 20);		
	if(NULL == config_impl->in_faultphases)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    config_impl->out_faultphases = axis2_array_list_create(env, 20);		
	if(NULL == config_impl->out_faultphases)
	{
        axis2_conf_free(&(config_impl->conf), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
	
    config_impl->all_svcs = axis2_hash_make(env);
    if(NULL == config_impl->all_svcs)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    
    config_impl->msg_recvs = axis2_hash_make(env);
    if(NULL == config_impl->msg_recvs)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 

    config_impl->faulty_svcs = axis2_hash_make(env);
    if(NULL == config_impl->faulty_svcs)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 

    config_impl->faulty_modules = axis2_hash_make(env);
    if(NULL == config_impl->faulty_modules)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    } 
    
    config_impl->conf.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_conf_ops_t));
    if(NULL == config_impl->conf.ops)
    {
        axis2_conf_free(&(config_impl->conf), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	config_impl->conf.ops->free = axis2_conf_free;
	config_impl->conf.ops->add_svc_grp = 
        axis2_conf_add_svc_grp;
    
	config_impl->conf.ops->get_svc_grp = 
        axis2_conf_get_svc_grp;
    
    config_impl->conf.ops->get_svc_grps = 
        axis2_conf_get_svc_grps;
    
	config_impl->conf.ops->add_svc = axis2_conf_add_svc;
	config_impl->conf.ops->get_svc = axis2_conf_get_svc;
	config_impl->conf.ops->remove_svc = 
        axis2_conf_remove_svc;
    config_impl->conf.ops->add_param = 
        axis2_conf_add_param;
	config_impl->conf.ops->get_param = 
        axis2_conf_get_param;
	config_impl->conf.ops->get_params = 
        axis2_conf_get_params;
    config_impl->conf.ops->is_param_locked = 
            axis2_conf_is_param_locked;
    
    config_impl->conf.ops->get_transport_in = 
            axis2_conf_get_transport_in;
      
    config_impl->conf.ops->add_transport_in = 
            axis2_conf_add_transport_in;    
     
    config_impl->conf.ops->get_transport_out = 
            axis2_conf_get_transport_out;
            
    config_impl->conf.ops->add_transport_out = 
            axis2_conf_add_transport_out;

    config_impl->conf.ops->get_transports_in = 
            axis2_conf_get_transports_in;
            
    config_impl->conf.ops->get_transports_out = 
            axis2_conf_get_transports_out;
	
    config_impl->conf.ops->get_module = 
            axis2_conf_get_module;
    
    config_impl->conf.ops->get_engaged_modules =
            axis2_conf_get_engaged_modules;
    
    config_impl->conf.ops->get_in_phases_upto_and_including_post_dispatch =
            axis2_conf_get_in_phases_upto_and_including_post_dispatch;
    
    config_impl->conf.ops->get_outflow =
            axis2_conf_get_outflow;
    
    config_impl->conf.ops->get_in_faultflow =
            axis2_conf_get_in_faultflow;
    
    config_impl->conf.ops->get_out_faultflow =
            axis2_conf_get_out_faultflow;

    config_impl->conf.ops->get_faulty_svcs =
            axis2_conf_get_faulty_svcs;
    
    config_impl->conf.ops->get_faulty_modules =
            axis2_conf_get_faulty_modules;
    
    config_impl->conf.ops->get_svcs =
            axis2_conf_get_svcs;
    
    config_impl->conf.ops->is_engaged =
            axis2_conf_is_engaged;
    
    config_impl->conf.ops->get_phases_info =
            axis2_conf_get_phases_info;
    
    config_impl->conf.ops->set_phases_info =
            axis2_conf_set_phases_info;
    
    config_impl->conf.ops->add_msg_recv =
            axis2_conf_add_msg_recv;
    
    config_impl->conf.ops->get_msg_recv =
            axis2_conf_get_msg_recv;
    
    config_impl->conf.ops->set_out_phases =
            axis2_conf_set_out_phases;
    
    config_impl->conf.ops->get_out_phases =
            axis2_conf_get_out_phases;
            
    config_impl->conf.ops->set_in_faultphases =
            axis2_conf_set_in_faultphases;
    
    config_impl->conf.ops->set_out_faultphases =
            axis2_conf_set_out_faultphases;
    
    config_impl->conf.ops->get_modules =
            axis2_conf_get_modules;
    
    config_impl->conf.ops->add_module =
            axis2_conf_add_module;
    config_impl->conf.ops->set_default_dispatchers = 
        axis2_conf_set_default_dispatchers;
    config_impl->conf.ops->set_dispatch_phase = axis2_conf_set_dispatch_phase;
    config_impl->conf.ops->get_repos = axis2_conf_get_repos;
    config_impl->conf.ops->set_repos = axis2_conf_set_repos;
    config_impl->conf.ops->engage_module = axis2_conf_engage_module;
    
	return &(config_impl->conf);	
}	

/**********************Start of op impls********************************/

axis2_status_t AXIS2_CALL 
axis2_conf_free (axis2_conf_t *conf, 
                            axis2_env_t **env)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    
	AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    if(NULL != conf->ops)
    {
		AXIS2_FREE((*env)->allocator, conf->ops);
        conf->ops = NULL;
    }
    
	if(NULL != conf->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(conf->param_container, env);
        conf->param_container = NULL;
    }
    
    if(config_impl->svc_grps)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (config_impl->svc_grps, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_svc_grp_t *svc_grp = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            svc_grp = (axis2_svc_grp_t *) val;
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
            axis2_transport_in_desc_t *transport_in = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            transport_in = (axis2_transport_in_desc_t *) val;
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
            axis2_transport_out_desc_t *transport_out = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            transport_out = (axis2_transport_out_desc_t *) val;
            if (transport_out)
                AXIS2_TRANSPORT_OUT_DESC_FREE (transport_out, env);
            
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
    
    if(config_impl->out_phases)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(config_impl->out_phases, env); i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->out_phases, env, i);
            
            phase = (axis2_phase_t *) val;
            if (phase)
               AXIS2_PHASE_FREE (phase, env);
            
            val = NULL;
            phase = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(config_impl->out_phases, env);
        config_impl->out_phases = NULL;
    }
    
    if(config_impl->in_faultphases)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(config_impl->in_faultphases, env); i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->in_faultphases, env, i);
            
            phase = (axis2_phase_t *) val;
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
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->out_faultphases, env, i);
            
            phase = (axis2_phase_t *) val;
            if (phase)
               AXIS2_PHASE_FREE (phase, env);
            
            val = NULL;
            phase = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(config_impl->out_faultphases, env);
        config_impl->out_faultphases = NULL;
    }
    
    if(config_impl->in_phases_upto_and_including_post_dispatch)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(config_impl->
                in_phases_upto_and_including_post_dispatch, env); i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(config_impl->
                in_phases_upto_and_including_post_dispatch, env, i);
            
            phase = (axis2_phase_t *) val;
            if (phase)
               AXIS2_PHASE_FREE (phase, env);
            
            val = NULL;
            phase = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(config_impl->
            in_phases_upto_and_including_post_dispatch, env);
        config_impl->in_phases_upto_and_including_post_dispatch = NULL;
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
axis2_conf_add_svc_grp (axis2_conf_t *conf, 
                        axis2_env_t **env,
                        axis2_svc_grp_t *svc_grp)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_hash_t *svcs = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_char_t *svc_name = NULL;
    axis2_char_t *svc_name2 = NULL;
    axis2_qname_t *module_desc = NULL;
    axis2_char_t *svc_grp_name = NULL;
    int i = 0;
    int k = 0;
    int size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_grp, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    svcs = AXIS2_SVC_GRP_GET_SVCS(svc_grp, env);
    if(!config_impl->all_svcs)
    {
        config_impl->all_svcs = axis2_hash_make(env);
        if(!config_impl->all_svcs)
            return AXIS2_FAILURE;
    }
    k = axis2_hash_count(svcs);
    index_i = axis2_hash_first (svcs, env);
    while(index_i)
    {
        void *value = NULL;
        axis2_svc_t *desc = NULL;
        
        axis2_hash_this (index_i, NULL, NULL, &value);
        desc = (axis2_svc_t *) value;
        svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_QNAME(desc, env), env);
        
        svc_name2 = axis2_hash_get(config_impl->all_svcs, svc_name, 
                AXIS2_HASH_KEY_STRING);
        /* no two service names deployed in the engine can be same */
        if(NULL != svc_name2)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_TWO_SVCS_CANNOT_HAVE_SAME_NAME,
                AXIS2_FAILURE);
            return AXIS2_FAILURE;            
        }
        index_i = axis2_hash_next(env, index_i);
    }
   
    svcs = NULL;
    svcs = AXIS2_SVC_GRP_GET_SVCS(svc_grp, env);
    index_i = axis2_hash_first (svcs, env);
    while(NULL != index_i)
    {
        void *value = NULL;
        axis2_svc_t *desc = NULL;
        
        axis2_hash_this (index_i, NULL, NULL, &value);
        desc = (axis2_svc_t *) value;
        svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_QNAME(desc, env), env);
        axis2_hash_set(config_impl->all_svcs, svc_name, AXIS2_HASH_KEY_STRING,
            desc);
        /* notifyObservers(AxisEvent.SERVICE_DEPLOY ,description); */
        index_i = axis2_hash_next(env, index_i);
    }
    size = AXIS2_ARRAY_LIST_SIZE(config_impl->engaged_modules, env);
    while(i < size)
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

axis2_svc_grp_t * AXIS2_CALL 
axis2_conf_get_svc_grp (axis2_conf_t *conf, 
                                    axis2_env_t **env,
		                            axis2_char_t *svc_grp_name)
{
    axis2_conf_impl_t *config_impl = NULL;
	AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
	AXIS2_PARAM_CHECK((*env)->error, svc_grp_name, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    if(!config_impl->svc_grps)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_CONF, 
            AXIS2_FAILURE);
        return NULL;
    }
	return (axis2_svc_grp_t *) (axis2_hash_get (config_impl->svc_grps, 
            svc_grp_name, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL
axis2_conf_get_svc_grps(axis2_conf_t *conf, 
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->svc_grps;
}

axis2_status_t AXIS2_CALL 
axis2_conf_add_svc (axis2_conf_t *conf, 
                                axis2_env_t **env, 
                                axis2_svc_t *svc)
{
    axis2_svc_grp_t *svc_grp = NULL;
    axis2_qname_t *svc_grp_qname = NULL;
    axis2_char_t *svc_grp_name = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
	
	svc_grp = axis2_svc_grp_create(env);
	
	if(NULL == svc_grp)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE)
        return AXIS2_FAILURE;
    }
		
	
	svc_grp_qname = AXIS2_SVC_GET_QNAME(svc, env);
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
	status = AXIS2_SVC_GRP_SET_PARENT(svc_grp, env, conf);
    if(AXIS2_FAILURE == status)
    {
        return status;
    }
	status = AXIS2_SVC_GRP_ADD_SVC(svc_grp, env, svc);
    if(AXIS2_FAILURE == status)
    {
        return status;
    }
    status = axis2_conf_add_svc_grp(conf, env, svc_grp);
	
	return status;
}

axis2_svc_t * AXIS2_CALL 
axis2_conf_get_svc (axis2_conf_t *conf, 
                                axis2_env_t **env,
		                        axis2_char_t* svc_name)
{   
	AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, NULL);
	
    return axis2_hash_get(AXIS2_INTF_TO_IMPL(conf)->all_svcs, svc_name,
        AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL 
axis2_conf_remove_svc (axis2_conf_t *conf, 
                                axis2_env_t **env,
		                        const axis2_char_t *svc_name)
{
    axis2_conf_impl_t *config_impl = NULL;
        
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, AXIS2_FAILURE);
    
    axis2_hash_set(config_impl->all_svcs, svc_name, AXIS2_HASH_KEY_STRING,
        NULL);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_add_param (axis2_conf_t *conf, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
	if(AXIS2_TRUE == axis2_conf_is_param_locked(conf, env, 
        AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
	else
    {
        status = AXIS2_PARAM_CONTAINER_ADD_PARAM(conf->param_container, env, 
            param);
    }
    return status;
}

axis2_param_t * AXIS2_CALL
axis2_conf_get_param (axis2_conf_t *conf, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
    
	if(NULL == conf->param_container)
	{
		AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
        return NULL;
	}
		
	return AXIS2_PARAM_CONTAINER_GET_PARAM(conf->param_container, env,
        name);
	
}

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_params (axis2_conf_t *conf, 
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(conf->param_container, env);
	
}

axis2_bool_t AXIS2_CALL
axis2_conf_is_param_locked (axis2_conf_t *conf, 
                            axis2_env_t **env,
		                    axis2_char_t *param_name)
{
    axis2_param_t *param = NULL;
        
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
    param = axis2_conf_get_param(conf, env, param_name);
    return (NULL != param  && AXIS2_PARAM_IS_LOCKED(param, env));
}

axis2_transport_in_desc_t * AXIS2_CALL
axis2_conf_get_transport_in(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname)
{
    axis2_conf_impl_t *config_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    return (axis2_transport_in_desc_t *) axis2_hash_get(config_impl->
            transports_in, AXIS2_QNAME_TO_STRING(qname, env), AXIS2_HASH_KEY_STRING);
}

/**
 * Method addTransport
 *
 * @param transport
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL
axis2_conf_add_transport_in(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_transport_in_desc_t *transport)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_qname_t *qname = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, transport, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    if(!config_impl->transports_in)
    {
        config_impl->transports_in = (axis2_hash_t *) axis2_hash_make(env);
        if(!transports_in)
            return AXIS2_FAILURE;
    }
    qname = AXIS2_TRANSPORT_IN_DESC_GET_QNAME(transport, env);
    if (!qname)
        return AXIS2_FAILURE;
        
    axis2_hash_set(config_impl->transports_in,
        AXIS2_QNAME_TO_STRING(qname, env), AXIS2_HASH_KEY_STRING, 
            transport);
    
    return AXIS2_SUCCESS;
    
}

axis2_transport_out_desc_t * AXIS2_CALL
axis2_conf_get_transport_out(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname)
{
    axis2_conf_impl_t *config_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    return (axis2_transport_out_desc_t *) axis2_hash_get(config_impl->
            transports_out, AXIS2_QNAME_TO_STRING(qname, env), AXIS2_HASH_KEY_STRING);
}

/**
 * Method addTransport
 *
 * @param transport
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL
axis2_conf_add_transport_out(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_transport_out_desc_t *transport)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_qname_t *qname = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, transport, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    if(!config_impl->transports_out)
    {
        config_impl->transports_out = axis2_hash_make(env);
        if(!transports_out)
            return AXIS2_FAILURE;
    }
    
    qname = AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(transport, env);
    if (!qname)
        return AXIS2_FAILURE;
    axis2_hash_set(config_impl->transports_out,
        AXIS2_QNAME_TO_STRING(qname, env),
            AXIS2_HASH_KEY_STRING, transport);
    return AXIS2_SUCCESS;
    
}

axis2_hash_t * AXIS2_CALL
axis2_conf_get_transports_in(axis2_conf_t *conf,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->transports_in;
}

/**
 * Method getModule
 *
 * @param name
 * @return ModuleDescription
 */
axis2_module_desc_t *AXIS2_CALL
axis2_conf_get_module(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_qname_t *qname) 
{
    axis2_conf_impl_t *conf_impl = NULL;
    axis2_char_t *name = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    conf_impl = AXIS2_INTF_TO_IMPL(conf);

    name = AXIS2_QNAME_TO_STRING(qname, env);
    return (axis2_module_desc_t *) axis2_hash_get(conf_impl->modules, 
        name, AXIS2_HASH_KEY_STRING);
}

/**
 * Method getEngadgedModules
 *
 * @return  Collection
 */
axis2_array_list_t * AXIS2_CALL
axis2_conf_get_engaged_modules(axis2_conf_t *conf,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->engaged_modules;
}

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_in_phases_upto_and_including_post_dispatch(
                                            axis2_conf_t *conf,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->
        in_phases_upto_and_including_post_dispatch;
}

axis2_array_list_t * AXIS2_CALL
axis2_conf_get_outflow(axis2_conf_t *conf,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->out_phases;
}


/**
 * @return ArrayList
 */
axis2_array_list_t * AXIS2_CALL
axis2_conf_get_in_faultflow(axis2_conf_t *conf,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->in_faultphases;
}

/**
 * @return ArrayList
 */
axis2_array_list_t * AXIS2_CALL
axis2_conf_get_out_faultflow(axis2_conf_t *conf,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->out_faultphases;
}

axis2_hash_t * AXIS2_CALL
axis2_conf_get_transports_out(axis2_conf_t *conf,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->transports_out;
}	

/*static axis2_status_t 
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
axis2_conf_get_faulty_svcs(axis2_conf_t *conf,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(conf)->faulty_svcs;
}

axis2_hash_t *AXIS2_CALL 
axis2_conf_get_faulty_modules(axis2_conf_t *conf,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->faulty_modules;
}
    
/*to get all the services in the system */
axis2_hash_t *AXIS2_CALL
axis2_conf_get_svcs(axis2_conf_t *conf,
                                axis2_env_t **env) 
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_hash_t *sgs = NULL;
    axis2_hash_index_t *index_i = NULL;
    axis2_hash_index_t *index_j = NULL;
    void *value = NULL;
    void *value2 = NULL;
    axis2_svc_grp_t *axis_svc_grp = NULL;
    axis2_hash_t *svcs = NULL;
    axis2_svc_t *svc =NULL;
    axis2_char_t *svc_name = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    sgs = axis2_conf_get_svc_grps(conf, env);
    index_i = axis2_hash_first(sgs, env);
    while(NULL != index_i)
    {
        axis2_hash_this(index_i, NULL, NULL, &value);
        axis_svc_grp = (axis2_svc_grp_t *) value;
        svcs = AXIS2_SVC_GRP_GET_SVCS(axis_svc_grp, env);
        index_j = axis2_hash_first(svcs, env);
        while(NULL != index_j)
        {
            axis2_hash_this(index_j, NULL, NULL, &value2);
            svc = (axis2_svc_t *) value2;
            svc_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_QNAME(svc, env), env);
            axis2_hash_set(config_impl->all_svcs, svc_name,
                AXIS2_HASH_KEY_STRING, svc);    
                            
            index_j = axis2_hash_next(env, index_j);
        }
        
        index_i = axis2_hash_next(env, index_i);
    }
    return config_impl->all_svcs;
}

axis2_bool_t AXIS2_CALL
axis2_conf_is_engaged(axis2_conf_t *conf,
                                axis2_env_t **env,
                                axis2_qname_t *module_name) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FALSE);
    
    return AXIS2_ARRAY_LIST_CONTAINS(AXIS2_INTF_TO_IMPL(conf)->
        engaged_modules, env, module_name);
}

axis2_phases_info_t *AXIS2_CALL
axis2_conf_get_phases_info(axis2_conf_t *conf,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    
    return (AXIS2_INTF_TO_IMPL(conf))->phases_info;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_phases_info(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_phases_info_t *phases_info) 
{
    axis2_conf_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, phases_info, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    if(config_impl->phases_info)
    {
        AXIS2_PHASES_INFO_FREE(phases_info, env);
        config_impl->phases_info = NULL;
    }
    config_impl->phases_info = phases_info;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_add_msg_recv(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_char_t *key,
                                    axis2_msg_recv_t *msg_recv) 
{
    axis2_conf_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_recv, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    if(!config_impl->msg_recvs)
    {
        config_impl->msg_recvs = axis2_hash_make(env);
        if(!config_impl->msg_recvs)
            return AXIS2_FAILURE;
    }
    axis2_hash_set(config_impl->msg_recvs, key, AXIS2_HASH_KEY_STRING, msg_recv);
    return AXIS2_SUCCESS;
}

axis2_msg_recv_t *AXIS2_CALL
axis2_conf_get_msg_recv(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_char_t *key) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    
    return (axis2_msg_recv_t *) axis2_hash_get(AXIS2_INTF_TO_IMPL(
        conf)->msg_recvs, key, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_conf_set_out_phases(axis2_conf_t *conf,
                                    axis2_env_t **env,
                                    axis2_array_list_t *out_phases) 
{
    axis2_conf_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, out_phases, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    if(config_impl->out_phases)
    {
        AXIS2_ARRAY_LIST_FREE(config_impl->out_phases, env);
        config_impl->out_phases = NULL;
    }
    config_impl->out_phases = out_phases;
    return AXIS2_SUCCESS;
}

axis2_array_list_t* AXIS2_CALL
axis2_conf_get_out_phases(axis2_conf_t *conf,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    return AXIS2_INTF_TO_IMPL(conf)->out_phases;
}
    
    /**
 * @param list
 */
axis2_status_t AXIS2_CALL
axis2_conf_set_in_faultphases(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list) 
{
    axis2_conf_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
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
axis2_conf_set_out_faultphases(axis2_conf_t *conf,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list) 
{
    axis2_conf_impl_t *config_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
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
axis2_conf_get_modules(axis2_conf_t *conf,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(conf, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(conf)->modules;
}  

/**
 * Method addMdoule
 *
 * @param module
 */
axis2_status_t AXIS2_CALL
axis2_conf_add_module(axis2_conf_t *conf,
                                axis2_env_t **env,
                                axis2_module_desc_t *module) 
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_qname_t *module_qname = NULL;
    
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module, AXIS2_FAILURE);
    
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    status = AXIS2_MODULE_DESC_SET_PARENT(module, env, conf);
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

    module_qname = AXIS2_MODULE_DESC_GET_NAME(module, env);
    if (module_qname)
        axis2_hash_set(config_impl->modules, AXIS2_QNAME_TO_STRING(module_qname, env), 
            AXIS2_HASH_KEY_STRING, module);
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_default_dispatchers(axis2_conf_t *conf,
                                    axis2_env_t **env)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_phase_t *dispatch = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_disp_t *add_dispatch = NULL;
    axis2_disp_t *uri_dispatch = NULL;
    axis2_disp_t *soap_action_based_dispatch = NULL;
    axis2_disp_t *soap_msg_body_based_dispatch = NULL;
    axis2_handler_t *handler = NULL;
    axis2_phase_t *post_dispatch = NULL;
    axis2_disp_checker_t *disp_checker = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);

    dispatch = axis2_phase_create(env, AXIS2_PHASE_DISPATCH);
    if(!dispatch)
    {
        return AXIS2_FAILURE;
    }
    add_dispatch = axis2_addr_disp_create(env);
    if(!add_dispatch)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return AXIS2_FAILURE;
    }
    handler = AXIS2_DISP_GET_BASE(add_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 0, handler);
    handler = NULL;
    
    uri_dispatch = axis2_req_uri_disp_create(env);
    if(!uri_dispatch)
    {
        return AXIS2_FAILURE;
    }
    handler = AXIS2_DISP_GET_BASE(uri_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 1, handler);
    handler = NULL;
    
    soap_action_based_dispatch = axis2_soap_action_disp_create(env);
    if(!soap_action_based_dispatch)
    {
        return AXIS2_FAILURE;
    }
    handler = AXIS2_DISP_GET_BASE(soap_action_based_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 2, handler);
    handler = NULL;
    
    soap_msg_body_based_dispatch = axis2_soap_body_disp_create(env);
    if(!soap_msg_body_based_dispatch)
    {
        return AXIS2_FAILURE;
    }
    handler = AXIS2_DISP_GET_BASE(soap_msg_body_based_dispatch, env);
    AXIS2_PHASE_ADD_HANDLER_AT(dispatch, env, 3, handler);
    handler = NULL;
    
    status = AXIS2_ARRAY_LIST_ADD(config_impl->
            in_phases_upto_and_including_post_dispatch, env, dispatch);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return AXIS2_FAILURE;   
    }
    
    post_dispatch = axis2_phase_create(env, AXIS2_PHASE_POST_DISPATCH);
    if(NULL == post_dispatch)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return AXIS2_FAILURE;
    }
    
    disp_checker = axis2_disp_checker_create(env, NULL);
    handler = AXIS2_DISP_CHECKER_GET_BASE(disp_checker, env);
    AXIS2_PHASE_ADD_HANDLER_AT(post_dispatch, env, 0, handler);
    handler = NULL;

    handler = axis2_ctx_handler_create(env, NULL);
    AXIS2_PHASE_ADD_HANDLER_AT(post_dispatch, env, 1, handler);
    
    status = AXIS2_ARRAY_LIST_ADD(config_impl->
            in_phases_upto_and_including_post_dispatch, env, post_dispatch);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        AXIS2_PHASE_FREE(post_dispatch, env);
        AXIS2_DISP_CHECKER_FREE(disp_checker, env);
        return AXIS2_FAILURE;   
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_set_dispatch_phase(axis2_conf_t *conf,
                                axis2_env_t **env,
                                axis2_phase_t *dispatch)
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_handler_t *handler = NULL;
    axis2_phase_t *post_dispatch = NULL;
    axis2_disp_checker_t *disp_checker = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, dispatch, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    status = AXIS2_ARRAY_LIST_ADD(config_impl->
            in_phases_upto_and_including_post_dispatch, env, dispatch);
    if(AXIS2_FAILURE == status)
    {
        return AXIS2_FAILURE;   
    }
    
    post_dispatch = axis2_phase_create(env, AXIS2_PHASE_POST_DISPATCH);
    if(NULL == post_dispatch)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        return AXIS2_FAILURE;
    }
    
    disp_checker = axis2_disp_checker_create(env, NULL);
    
    /* TODO uncomment this when instant dispatcher is available */
    /*
    InstanceDispatcher instanceDispatcher = new InstanceDispatcher();
    instanceDispatcher.getHandlerDesc().setParent(this);
    */
    
    handler = AXIS2_DISP_CHECKER_GET_BASE(disp_checker, env);
    AXIS2_PHASE_ADD_HANDLER_AT(post_dispatch, env, 0, handler);
 
    /*postDispatch.addHandler(instanceDispatcher,1); */
    status = AXIS2_ARRAY_LIST_ADD(config_impl->
            in_phases_upto_and_including_post_dispatch, env, post_dispatch);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_PHASE_FREE(dispatch, env);
        AXIS2_PHASE_FREE(post_dispatch, env);
        AXIS2_DISP_CHECKER_FREE(disp_checker, env);
        return AXIS2_FAILURE;   
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_conf_engage_module(axis2_conf_t *conf,
                        axis2_env_t **env,
                        axis2_qname_t *module_ref) 
{
    axis2_conf_impl_t *config_impl = NULL;
    axis2_module_desc_t *module_desc = NULL;
    axis2_bool_t is_new_module = AXIS2_FAILURE;
    axis2_bool_t to_be_engaged = AXIS2_TRUE;
    axis2_dep_engine_t *dep_engine = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(conf, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_ref, AXIS2_FAILURE);
    config_impl = AXIS2_INTF_TO_IMPL(conf);
    
    module_desc = axis2_conf_get_module(conf, env, module_ref);
    if(NULL == module_desc)
    {
        axis2_file_t *file = NULL;
        axis2_char_t *file_name = NULL;
        axis2_arch_reader_t *arch_reader = NULL;
        axis2_char_t *repos_path = NULL;
        axis2_arch_file_data_t *file_data = NULL;
        axis2_char_t *temp_path1 = NULL;
        axis2_char_t *temp_path2 = NULL;
        axis2_char_t *temp_path3 = NULL;
        axis2_char_t *path = NULL;
        
        arch_reader = axis2_arch_reader_create(env);
        if(!arch_reader)
        {
            return AXIS2_FAILURE;
        }
        file_name = AXIS2_QNAME_GET_LOCALPART(module_ref, env);
        file = (axis2_file_t *) AXIS2_ARCH_READER_CREATE_MODULE_ARCH(
            arch_reader, env, file_name) ;
        repos_path = AXIS2_CONF_GET_REPOS(conf, env);
        temp_path1 = AXIS2_STRACAT(repos_path, AXIS2_PATH_SEP_STR, env);
        temp_path2 = AXIS2_STRACAT(temp_path1, AXIS2_MODULE_FOLDER, env);
        temp_path3 = AXIS2_STRACAT(temp_path2, AXIS2_PATH_SEP_STR, env);
        path = AXIS2_STRACAT(temp_path3, file_name, env);
        AXIS2_FREE((*env)->allocator, temp_path1);
        AXIS2_FREE((*env)->allocator, temp_path2);
        AXIS2_FREE((*env)->allocator, temp_path3);
        AXIS2_FILE_SET_PATH(file, env, path);
        file_data = axis2_arch_file_data_create_with_type_and_file(env,
                AXIS2_MODULE, file);

        dep_engine = axis2_dep_engine_create_with_repos_name(env, repos_path);
        AXIS2_DEP_ENGINE_SET_CURRENT_FILE_ITEM(dep_engine, env, file_data);
        module_desc = AXIS2_DEP_ENGINE_BUILD_MODULE(dep_engine, env, file, conf);
        is_new_module = AXIS2_TRUE;
    }
    if(NULL != module_desc)
    {
        int size = 0;
        int i = 0;
        
        size = AXIS2_ARRAY_LIST_SIZE(config_impl->engaged_modules, env);
        for(i = 0; i < size; i++)
        {
            axis2_qname_t *qname = NULL;
            
            qname = (axis2_qname_t *) AXIS2_ARRAY_LIST_GET(config_impl->
                engaged_modules, env, i);
            if(AXIS2_TRUE == AXIS2_QNAME_EQUALS(module_ref, env, qname))
            {
                to_be_engaged = AXIS2_FALSE;
                /* Instead of throwing the error, we can just log this problem */
                /*log.info("Attempt to engage an already engaged module "+ qName);*/
            }
        }
    } 
    else 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MODULE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if (to_be_engaged) 
    {
        axis2_phase_resolver_t *phase_resolver = NULL;
        phase_resolver = axis2_phase_resolver_create_with_config(env, conf);
        AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_GLOBALLY(phase_resolver, env, 
            module_desc);
        AXIS2_ARRAY_LIST_ADD(config_impl->engaged_modules, env, module_ref);
    }
    if (is_new_module) 
    {
        axis2_conf_add_module(conf, env, module_desc);
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_conf_get_repos(axis2_conf_t *conf,
                        axis2_env_t **env)
{
    axis2_conf_impl_t *conf_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    conf_impl = AXIS2_INTF_TO_IMPL(conf);

    return conf_impl->axis2_repos;

}

axis2_status_t AXIS2_CALL
axis2_conf_set_repos(axis2_conf_t *conf,
               axis2_env_t **env,
               axis2_char_t *repos_path)
{
    axis2_conf_impl_t *conf_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    conf_impl = AXIS2_INTF_TO_IMPL(conf);

    if(conf_impl->axis2_repos)
    {
        AXIS2_FREE((*env)->allocator, conf_impl->axis2_repos);
        conf_impl->axis2_repos = NULL;
    }
    conf_impl->axis2_repos = AXIS2_STRDUP(repos_path, env);
    return AXIS2_SUCCESS;
}
