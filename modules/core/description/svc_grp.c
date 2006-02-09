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
 
#include <axis2_svc_grp.h>

/** 
 * @brief Service group struct impl
 * Axis2 Service group impl  
 */  
typedef struct axis2_svc_grp_impl
{
	axis2_svc_grp_t svc_grp;
	axis2_char_t *svc_grp_name;
	axis2_hash_t *svcs;
    /** to store service Group modules name */
    axis2_array_list_t *modules;
    /** to store module ref at deploy time parsing */
    axis2_conf_t *parent; 
    axis2_array_list_t *module_list;
    
	
} axis2_svc_grp_impl_t;

#define AXIS2_INTF_TO_IMPL(svc_grp) ((axis2_svc_grp_impl_t *)svc_grp)

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_svc_grp_free (axis2_svc_grp_t *svc_grp, 
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_svc_grp_set_svc_grp_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_char_t *name);

axis2_char_t * AXIS2_CALL 
axis2_svc_grp_get_svc_grp_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_svc_grp_add_svc (axis2_svc_grp_t* svc_grp, 
                        axis2_env_t **env,
		                axis2_svc_t *svc);

axis2_svc_t * AXIS2_CALL 
axis2_svc_grp_get_svc (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_qname_t* svc_name);

axis2_hash_t *AXIS2_CALL
axis2_svc_grp_get_svcs(axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env);
                        
axis2_status_t AXIS2_CALL 
axis2_svc_grp_remove_svc (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    axis2_qname_t* svc_name);
		
axis2_status_t AXIS2_CALL 
axis2_svc_grp_add_param (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    axis2_param_t *param);
		

axis2_param_t * AXIS2_CALL 
axis2_svc_grp_get_param(axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                const axis2_char_t *name);
		
axis2_array_list_t * AXIS2_CALL 
axis2_svc_grp_get_params (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env);
		
axis2_bool_t AXIS2_CALL 
axis2_svc_grp_is_param_locked(axis2_svc_grp_t *svc_grp, 
                                axis2_env_t **env,
		                        axis2_char_t *param_name);
                                
axis2_status_t AXIS2_CALL
axis2_svc_grp_add_module(axis2_svc_grp_t *svc_grp, 
                                axis2_env_t **env,
                                axis2_qname_t *module_qname);                                

axis2_conf_t * AXIS2_CALL
axis2_svc_grp_get_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_grp_set_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            axis2_conf_t *parent);

axis2_status_t AXIS2_CALL
axis2_svc_grp_engage_module_to_grp(axis2_svc_grp_t *svc_grp,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_name);

axis2_array_list_t *AXIS2_CALL
axis2_svc_grp_get_svc_grp_modules(axis2_svc_grp_t *svc_grp,
                                    axis2_env_t **env);

axis2_conf_t *AXIS2_CALL
axis2_svc_grp_get_axis_desc(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_grp_set_axis_desc(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            axis2_conf_t *axis2_desc);

axis2_status_t AXIS2_CALL
axis2_svc_grp_add_moduleref(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            axis2_qname_t *moduleref);

axis2_array_list_t *AXIS2_CALL
axis2_svc_grp_get_modules(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env);
                            
axis2_svc_grp_ctx_t *AXIS2_CALL
axis2_svc_grp_get_svc_grp_ctx(axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env,
                                axis2_conf_ctx_t *parent);                            
                          
/***************************** End of function headers ************************/

AXIS2_DECLARE(axis2_svc_grp_t *) 
axis2_svc_grp_create (axis2_env_t **env)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    svc_grp_impl = (axis2_svc_grp_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_grp_impl_t));
    
	if(NULL == svc_grp_impl)
    {
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
	
    svc_grp_impl->svc_grp.param_container = NULL;
    svc_grp_impl->modules = NULL;
    svc_grp_impl->svcs = NULL;
    svc_grp_impl->svc_grp.ops = NULL;
    svc_grp_impl->parent = NULL;
    svc_grp_impl->svc_grp_name = NULL;
    svc_grp_impl->module_list = NULL;
    
    svc_grp_impl->svc_grp.param_container =  axis2_param_container_create(env);
    if(NULL == svc_grp_impl->svc_grp.param_container)
    {
        axis2_svc_grp_free(&(svc_grp_impl->svc_grp), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    svc_grp_impl->modules = axis2_array_list_create(env, 20);
    if(NULL == svc_grp_impl->modules)
    {
        axis2_svc_grp_free(&(svc_grp_impl->svc_grp), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    svc_grp_impl->module_list = axis2_array_list_create(env, 20);
    if(NULL == svc_grp_impl->module_list)
    {
        axis2_svc_grp_free(&(svc_grp_impl->svc_grp), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    svc_grp_impl->svcs = axis2_hash_make (env);				
	if(NULL == svc_grp_impl->svcs)
    {
        axis2_svc_grp_free(&(svc_grp_impl->svc_grp), env);
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    svc_grp_impl->svc_grp.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_svc_grp_ops_t));
	if(NULL == svc_grp_impl->svc_grp.ops)
	{
        AXIS2_FREE((*env)->allocator, svc_grp_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
	svc_grp_impl->svc_grp.ops->free = axis2_svc_grp_free;
	svc_grp_impl->svc_grp.ops->set_svc_grp_name = axis2_svc_grp_set_svc_grp_name;
	svc_grp_impl->svc_grp.ops->get_svc_grp_name = axis2_svc_grp_get_svc_grp_name;
	svc_grp_impl->svc_grp.ops->add_svc = axis2_svc_grp_add_svc;
	svc_grp_impl->svc_grp.ops->get_svc = axis2_svc_grp_get_svc;
    svc_grp_impl->svc_grp.ops->get_svcs = axis2_svc_grp_get_svcs;
	svc_grp_impl->svc_grp.ops->remove_svc = axis2_svc_grp_remove_svc;
	svc_grp_impl->svc_grp.ops->add_param = axis2_svc_grp_add_param;
	svc_grp_impl->svc_grp.ops->get_param = axis2_svc_grp_get_param;
	svc_grp_impl->svc_grp.ops->get_params = axis2_svc_grp_get_params;
	svc_grp_impl->svc_grp.ops->is_param_locked = axis2_svc_grp_is_param_locked;
    svc_grp_impl->svc_grp.ops->add_module = axis2_svc_grp_add_module;
    svc_grp_impl->svc_grp.ops->get_parent = axis2_svc_grp_get_parent;
    svc_grp_impl->svc_grp.ops->set_parent = axis2_svc_grp_set_parent;
    svc_grp_impl->svc_grp.ops->engage_module_to_grp = axis2_svc_grp_engage_module_to_grp;                            
    svc_grp_impl->svc_grp.ops->get_svc_grp_modules = axis2_svc_grp_get_svc_grp_modules;                            
    svc_grp_impl->svc_grp.ops->get_axis_desc = axis2_svc_grp_get_axis_desc;                            
    svc_grp_impl->svc_grp.ops->set_axis_desc = axis2_svc_grp_set_axis_desc;                            
    svc_grp_impl->svc_grp.ops->add_moduleref = axis2_svc_grp_add_moduleref;                            
    svc_grp_impl->svc_grp.ops->get_modules = axis2_svc_grp_get_modules;
    svc_grp_impl->svc_grp.ops->get_svc_grp_ctx = axis2_svc_grp_get_svc_grp_ctx;  	

	return &(svc_grp_impl->svc_grp);	
}

AXIS2_DECLARE(axis2_svc_grp_t *) 
axis2_svc_grp_create_with_conf (axis2_env_t **env,
                                         axis2_conf_t *conf)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, conf, NULL);
    
    svc_grp_impl = (axis2_svc_grp_impl_t *) axis2_svc_grp_create(env);
    if(NULL != svc_grp_impl)
        svc_grp_impl->parent = conf;
    
    return &(svc_grp_impl->svc_grp);
        
}
/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_svc_grp_free (axis2_svc_grp_t *svc_grp, 
                    axis2_env_t **env)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    
    if(NULL != svc_grp->ops)
    {
		AXIS2_FREE((*env)->allocator, svc_grp->ops);
        svc_grp->ops = NULL;
    }
    
	if(NULL != svc_grp->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(svc_grp->param_container, 
            env);
        svc_grp->param_container = NULL;
    }
    
    if(NULL != svc_grp_impl->svc_grp_name)
    {
        AXIS2_FREE((*env)->allocator, svc_grp_impl->svc_grp_name);
        svc_grp_impl->svc_grp_name = NULL;
    }
    
    if(NULL != svc_grp_impl->svcs)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (svc_grp_impl->svcs, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_svc_t *svc = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            svc = (axis2_svc_t *) val;
            if (svc)
               AXIS2_SVC_FREE (svc, env);
            
            val = NULL;
            svc = NULL;
               
        }
        axis2_hash_free(svc_grp_impl->svcs, env);
        svc_grp_impl->svcs = NULL;
    }
    
    if(NULL != svc_grp_impl->modules)
    {
        AXIS2_ARRAY_LIST_FREE(svc_grp_impl->modules, env);
        svc_grp_impl->modules = NULL;
    }
    
    if(NULL != svc_grp_impl->module_list)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(svc_grp_impl->module_list, env); i++)
        {
            axis2_module_desc_t *module_desc = NULL;
            module_desc = AXIS2_ARRAY_LIST_GET(svc_grp_impl->module_list, env, i);
            
            module_desc = (axis2_module_desc_t *) val;
            if (module_desc)
               AXIS2_MODULE_DESC_FREE (module_desc, env);
            
            val = NULL;
            module_desc = NULL;
               
        }
        AXIS2_ARRAY_LIST_FREE(svc_grp_impl->module_list, env);
        svc_grp_impl->module_list = NULL;
    }
   
    /* Samisa: parenet should not be freed here. 
    if(NULL != svc_grp_impl->parent)
    {
        AXIS2_CONF_FREE(svc_grp_impl->parent, env);
        svc_grp_impl->parent = NULL;
    }*/
    
    if(NULL != svc_grp_impl) 
    {
	    AXIS2_FREE((*env)->allocator, svc_grp_impl);
        svc_grp_impl = NULL;
    }
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_set_svc_grp_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_char_t *name)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    axis2_char_t *svc_grp_name_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    
	svc_grp_name_l = AXIS2_STRDUP(name, env);
    if(NULL == svc_grp_name_l)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
	
    if(svc_grp_impl->svc_grp_name)
        AXIS2_FREE((*env)->allocator, svc_grp_impl->svc_grp_name);
	svc_grp_impl->svc_grp_name = name;
    
	return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL 
axis2_svc_grp_get_svc_grp_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	
	return AXIS2_INTF_TO_IMPL(svc_grp)->svc_grp_name;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_add_svc (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_svc_t *svc)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    axis2_phase_resolver_t *handler_resolver = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_qname_t *svc_qname = NULL;
    axis2_char_t *svc_name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
	
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    
    if(!svc_grp_impl->svcs)
    {
        svc_grp_impl->svcs = axis2_hash_make(env);
        if(!svc_grp_impl->svcs)
            return AXIS2_FAILURE;
    }
    svc_qname = AXIS2_SVC_GET_QNAME(svc, env);
    svc_name = AXIS2_QNAME_TO_STRING(svc_qname, env);
	axis2_hash_set (svc_grp_impl->svcs, svc_name, AXIS2_HASH_KEY_STRING, svc);
    
    handler_resolver = axis2_phase_resolver_create_with_config_and_svc(env,
        svc_grp_impl->parent, svc);
    
    if(NULL == handler_resolver)
    {
        /* Remove the previously added service */
        axis2_hash_set(svc_grp_impl->svcs, svc_name, AXIS2_HASH_KEY_STRING, 
            NULL);
        
        return AXIS2_FAILURE;
    }

    status = AXIS2_PHASE_RESOLVER_BUILD_CHAINS(handler_resolver, env);
    if(AXIS2_SUCCESS != status)
    {
        /* Remove the previously added service */
        axis2_hash_set(svc_grp_impl->svcs, svc_name, AXIS2_HASH_KEY_STRING, NULL);
        AXIS2_PHASE_RESOLVER_FREE(handler_resolver, env);
        return status;
    }
    
    status = AXIS2_SVC_SET_LAST_UPDATE(svc, env);
    if(AXIS2_SUCCESS != status)
    {
        /* Remove the previously added service */
        axis2_hash_set(svc_grp_impl->svcs, svc_name, AXIS2_HASH_KEY_STRING, NULL);
        AXIS2_PHASE_RESOLVER_FREE(handler_resolver, env);
        return status;
    }
    
    status = AXIS2_SVC_SET_PARENT(svc, env, svc_grp);
    if(AXIS2_SUCCESS != status)
    {
        /* Remove the previously added service */
        axis2_hash_set(svc_grp_impl->svcs, svc_name, AXIS2_HASH_KEY_STRING, NULL);
        AXIS2_PHASE_RESOLVER_FREE(handler_resolver, env);
        return status;
    }   
    
	AXIS2_PHASE_RESOLVER_FREE(handler_resolver, env);
    return status;
}

axis2_svc_t *AXIS2_CALL
axis2_svc_grp_get_svc(axis2_svc_grp_t *svc_grp,
                        axis2_env_t **env,
                        axis2_qname_t *qname)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    axis2_char_t *name = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);

    name = AXIS2_QNAME_TO_STRING(qname, env);
    return (axis2_svc_t *) axis2_hash_get(svc_grp_impl->svcs, name, 
        AXIS2_HASH_KEY_STRING);
}

axis2_hash_t *AXIS2_CALL
axis2_svc_grp_get_svcs(axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_grp)->svcs;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_remove_svc (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    axis2_qname_t* svc_qname)
{
    axis2_svc_t *svc = NULL;
    axis2_char_t *svc_name = NULL;
        
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, svc_name, AXIS2_FAILURE);
    
    svc = axis2_svc_grp_get_svc(svc_grp, env, svc_qname);
    if (NULL != svc) 
    {
        /*this.parent.notifyObservers(AxisEvent.SERVICE_DEPLOY , service);*/
    }
    svc_name = AXIS2_QNAME_TO_STRING(svc_qname, env);
	axis2_hash_set (AXIS2_INTF_TO_IMPL(svc_grp)->svcs, svc_name, 
        AXIS2_HASH_KEY_STRING, NULL);
	
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_add_param (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env, 
                            axis2_param_t *param)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
    if(NULL == svc_grp->param_container)
    {
        svc_grp->param_container = axis2_param_container_create(env);
        if(!svc_grp->param_container)
        {
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_PARAM_CONTAINER_ADD_PARAM(svc_grp->param_container, env, param);
}

axis2_param_t * AXIS2_CALL 
axis2_svc_grp_get_param (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    const axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
	if(NULL == svc_grp->param_container)
    {
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER,
            AXIS2_FAILURE);
        return NULL;
    }
	
    return AXIS2_PARAM_CONTAINER_GET_PARAM(svc_grp->param_container, env, name);
}

axis2_array_list_t * AXIS2_CALL 
axis2_svc_grp_get_params (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(svc_grp->param_container, env);
	
}

axis2_bool_t AXIS2_CALL
axis2_svc_grp_is_param_locked(axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env,
                                axis2_char_t *param_name) 
{
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_conf_t *parent = NULL;
    axis2_param_t *param = NULL;
    axis2_bool_t ret = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);

    parent = axis2_svc_grp_get_parent(svc_grp, env);
    /* checking the locked value of parent */
    if (NULL != parent) 
    {
        locked =  AXIS2_CONF_IS_PARAM_LOCKED(parent, env, param_name);
    }
    if(locked)
    {
        ret = AXIS2_TRUE;
    } else 
    {
        param = axis2_svc_grp_get_param(svc_grp, env, param_name);
        if(NULL != param && AXIS2_PARAM_IS_LOCKED(param, env))
        {
            ret = AXIS2_TRUE;
        } else 
        {
            ret = AXIS2_FALSE;
        }
    }
    return ret;
}
    
axis2_status_t AXIS2_CALL
axis2_svc_grp_add_module(axis2_svc_grp_t *svc_grp, 
                                axis2_env_t **env,
                                axis2_qname_t *module_qname)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return AXIS2_ARRAY_LIST_ADD(AXIS2_INTF_TO_IMPL(svc_grp)->modules, env, 
        module_qname);
}

axis2_conf_t * AXIS2_CALL
axis2_svc_grp_get_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(svc_grp)->parent;
}

axis2_status_t AXIS2_CALL
axis2_svc_grp_set_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            axis2_conf_t *parent) 
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, parent, AXIS2_FAILURE);
    
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    if(svc_grp_impl->parent)
        AXIS2_CONF_FREE(svc_grp_impl->parent, env);
    svc_grp_impl->parent = parent;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_grp_engage_module_to_grp(axis2_svc_grp_t *svc_grp,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_name) 
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_qname_t *modu = NULL;
    axis2_char_t *modu_local = NULL;
    axis2_char_t *module_name_local = NULL;
    axis2_hash_t *svc = NULL;
    axis2_phase_resolver_t *phase_resolver = NULL;
    axis2_module_desc_t *module = NULL;
    
    axis2_svc_t *axis_svc = NULL;
    int size = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FAILURE);
    
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    size = AXIS2_ARRAY_LIST_SIZE(svc_grp_impl->modules, env);
    if(AXIS2_TRUE != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_FAILURE;
    }
    for(i = 0; size; i++)
    {
        modu = AXIS2_ARRAY_LIST_GET(svc_grp_impl->modules, env, i);
        modu_local = AXIS2_QNAME_GET_LOCALPART(modu, env);
        module_name_local = AXIS2_QNAME_GET_LOCALPART(module_name, env);
        if(0 == AXIS2_STRCMP(modu_local, module_name_local))
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_SVC_GRP, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    svc = axis2_svc_grp_get_svcs(svc_grp, env);
    if(NULL == svc) return AXIS2_FAILURE;
        
    phase_resolver = axis2_phase_resolver_create_with_config(env, svc_grp_impl->
        parent);
    
    if(NULL == phase_resolver) return AXIS2_FAILURE;
        
    module = AXIS2_CONF_GET_MODULE(svc_grp_impl->parent, env, module_name);
    if(NULL != module)
    {
        axis2_hash_index_t *index = NULL;
        index = axis2_hash_first (svc, env);
        while(NULL != index);  
        {
            void *v = NULL;
            /* engage in per each service */
            axis2_hash_this (index, NULL, NULL, &v);
            axis_svc = (axis2_svc_t *) v;
            status = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_SVC(phase_resolver, 
                env, axis_svc, module);
            if(AXIS2_FAILURE == status)
            {
                if(phase_resolver)
                    AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
                return status;
            }
            index = axis2_hash_next (env, index);
        }          
    }
    if(phase_resolver)
        AXIS2_PHASE_RESOLVER_FREE(phase_resolver, env);
    
    return axis2_svc_grp_add_module(svc_grp, env, module_name);
}

axis2_array_list_t *AXIS2_CALL
axis2_svc_grp_get_svc_grp_modules(axis2_svc_grp_t *svc_grp,
                                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(svc_grp)->modules;
}

axis2_conf_t *AXIS2_CALL
axis2_svc_grp_get_axis_desc(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(svc_grp)->parent;
}

axis2_status_t AXIS2_CALL
axis2_svc_grp_set_axis_desc(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            axis2_conf_t *axis2_desc) 
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, axis2_desc, AXIS2_FAILURE);
    
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    
    if(NULL != svc_grp_impl->parent)
        AXIS2_CONF_FREE(svc_grp_impl->parent, env);
    svc_grp_impl->parent = axis2_desc;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_svc_grp_add_moduleref(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            axis2_qname_t *moduleref)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, moduleref, AXIS2_FAILURE);
    
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    if(!svc_grp_impl->module_list)
    {
        svc_grp_impl->module_list = axis2_array_list_create(env, 20);
        if(!svc_grp_impl->module_list)
            return AXIS2_FAILURE;
    }
    return AXIS2_ARRAY_LIST_ADD(svc_grp_impl->module_list, env, moduleref);
}

axis2_array_list_t *AXIS2_CALL
axis2_svc_grp_get_modules(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(svc_grp)->module_list;
}

axis2_svc_grp_ctx_t *AXIS2_CALL
axis2_svc_grp_get_svc_grp_ctx(axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env,
                                axis2_conf_ctx_t *parent)
{
    axis2_svc_grp_ctx_t *svc_grp_ctx = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, parent, NULL);
    
    svc_grp_ctx = axis2_svc_grp_ctx_create(env, svc_grp, parent);
    if(!svc_grp_ctx)
    {
        return NULL;
    }
    return svc_grp_ctx;
}
