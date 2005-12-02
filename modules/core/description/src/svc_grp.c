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
    struct axis2_engine_config *parent;
	
} axis2_svc_grp_impl_t;

#define AXIS2_INTF_TO_IMPL(svc_grp) ((axis2_svc_grp_impl_t *)svc_grp)

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_svc_grp_free (axis2_svc_grp_t *svc_grp, 
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_svc_grp_set_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_char_t *name);

axis2_char_t * AXIS2_CALL 
axis2_svc_grp_get_name (axis2_svc_grp_t *svc_grp, 
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

struct axis2_engine_config * AXIS2_CALL
axis2_svc_grp_get_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_grp_set_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            struct axis2_engine_config *parent);

/***************************** End of function headers ************************/

axis2_svc_grp_t * AXIS2_CALL 
axis2_svc_grp_create (axis2_env_t **env)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    svc_grp_impl = (axis2_svc_grp_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_grp_impl_t));
    
	if(NULL == svc_grp_impl)
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	
    svc_grp_impl->svc_grp.param_container = NULL;
    svc_grp_impl->modules = NULL;
    svc_grp_impl->svcs = NULL;
    svc_grp_impl->svc_grp.ops = NULL;
    svc_grp_impl->parent = NULL;
    svc_grp_impl->svc_grp_name = NULL;
    
    svc_grp_impl->svc_grp.param_container =  axis2_param_container_create(env);
    if(NULL == svc_grp_impl->svc_grp.param_container)
    {
        axis2_svc_grp_free(&(svc_grp_impl->svc_grp), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    svc_grp_impl->modules = axis2_array_list_create(env, 0);
    if(NULL == svc_grp_impl->modules)
    {
        axis2_svc_grp_free(&(svc_grp_impl->svc_grp), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    svc_grp_impl->svcs = axis2_hash_make (env);				
	if(NULL == svc_grp_impl->svcs)
    {
        axis2_svc_grp_free(&(svc_grp_impl->svc_grp), env);
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    svc_grp_impl->svc_grp.ops = AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_svc_grp_ops_t));
	if(NULL == svc_grp_impl->svc_grp.ops)
	{
        AXIS2_FREE((*env)->allocator, svc_grp_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
	svc_grp_impl->svc_grp.ops->free = axis2_svc_grp_free;
	svc_grp_impl->svc_grp.ops->set_name = axis2_svc_grp_set_name;
	svc_grp_impl->svc_grp.ops->get_name = axis2_svc_grp_get_name;
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

	return &(svc_grp_impl->svc_grp);	
}

axis2_svc_grp_t * AXIS2_CALL 
axis2_svc_grp_create_with_engine_config (axis2_env_t **env,
                                         struct axis2_engine_config *engine_config)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, engine_config, NULL);
    
    svc_grp_impl = (axis2_svc_grp_impl_t *) axis2_svc_grp_create(env);
    if(NULL != svc_grp_impl)
        svc_grp_impl->parent = engine_config;
    
    return &(svc_grp_impl->svc_grp);
        
}
/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_svc_grp_free (axis2_svc_grp_t *svc_grp, 
                    axis2_env_t **env)
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    
    if(NULL != svc_grp->ops)
		AXIS2_FREE((*env)->allocator, svc_grp->ops);
    
	if(NULL != svc_grp->param_container)
        AXIS2_PARAM_CONTAINER_FREE(svc_grp->param_container, 
            env);
    
    if(NULL != svc_grp_impl->svc_grp_name)
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svc_grp)->svc_grp_name);
    
    if(NULL != svc_grp_impl->svcs)
        axis2_hash_free(svc_grp_impl->svcs, env);
    
    if(NULL != svc_grp_impl->modules)
        AXIS2_ARRAY_LIST_FREE(svc_grp_impl->modules, env);
    
    /*
    if(NULL != svc_grp_impl->parent)
    {
        AXIS2_ENGINE_CONFIG_FREE(svc_grp_impl->parent, env);
    }
    */
    
    if(NULL != svc_grp_impl) 
    {
	    AXIS2_FREE((*env)->allocator, svc_grp_impl);
    }
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_set_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
	
	axis2_char_t *tempname = AXIS2_STRDUP(name, env);
    if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	
	AXIS2_INTF_TO_IMPL(svc_grp)->svc_grp_name = name;
    
	return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL 
axis2_svc_grp_get_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
	
	return AXIS2_INTF_TO_IMPL(svc_grp)->svc_grp_name;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_add_svc (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_svc_t *svc)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
	
	axis2_qname_t *tempname = AXIS2_SVC_GET_NAME(svc, env);
    if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_SVC, 
        AXIS2_FAILURE);
    
	axis2_hash_set (AXIS2_INTF_TO_IMPL(svc_grp)->svcs, 
        AXIS2_QNAME_GET_LOCALPART(tempname, env), AXIS2_HASH_KEY_STRING, svc);
    
	return AXIS2_SUCCESS;
}

axis2_svc_t * AXIS2_CALL 
axis2_svc_grp_get_svc (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_qname_t* svc_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, NULL);
	
	return (axis2_svc_t *) (axis2_hash_get (AXIS2_INTF_TO_IMPL(svc_grp)->svcs, 
        AXIS2_QNAME_GET_LOCALPART(svc_name, env), AXIS2_HASH_KEY_STRING));
}

axis2_hash_t *AXIS2_CALL
axis2_svc_grp_get_svcs(axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
    return AXIS2_INTF_TO_IMPL(svc_grp)->svcs;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_remove_svc (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    axis2_qname_t* svc_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    
	AXIS2_PARAM_CHECK((*env)->error, svc_name, AXIS2_FAILURE);
    
	axis2_hash_set (AXIS2_INTF_TO_IMPL(svc_grp)->svcs, 
        AXIS2_QNAME_GET_LOCALPART(svc_name, env), AXIS2_HASH_KEY_STRING, NULL);
	
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_add_param (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env, 
                            axis2_param_t *param)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
    
    if(NULL == svc_grp->param_container)
    {
	    AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    return AXIS2_PARAM_CONTAINER_ADD_PARAM(svc_grp->param_container, env, param);
}

axis2_param_t * AXIS2_CALL 
axis2_svc_grp_get_param (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    const axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
	if(NULL == svc_grp->param_container)
    {
	    AXIS2_ERROR_SET((*env)->error
        , AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, NULL);
        return NULL;
    }
	
    return AXIS2_PARAM_CONTAINER_GET_PARAM(svc_grp->param_container, env, name);
}

axis2_array_list_t * AXIS2_CALL 
axis2_svc_grp_get_params (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
	
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(svc_grp->param_container, env);
	
}

axis2_bool_t AXIS2_CALL
axis2_svc_grp_is_param_locked(axis2_svc_grp_t *svc_grp,
                                axis2_env_t **env,
                                axis2_char_t *param_name) 
{
    axis2_bool_t locked = AXIS2_FALSE;
    struct axis2_engine_config *parent = NULL;
    axis2_param_t *param = NULL;
    axis2_bool_t ret = AXIS2_FALSE;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);

    parent = axis2_svc_grp_get_parent(svc_grp, env);
    /* checking the locked value of parent */
    if (NULL != parent) 
    {
        locked =  AXIS2_ENGINE_CONFIG_IS_PARAM_LOCKED(parent, env, param_name);
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
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    return AXIS2_ARRAY_LIST_ADD(AXIS2_INTF_TO_IMPL(svc_grp)->modules, env, 
        module_qname);
}

struct axis2_engine_config * AXIS2_CALL
axis2_svc_grp_get_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(svc_grp)->parent;
}

axis2_status_t AXIS2_CALL
axis2_svc_grp_set_parent(axis2_svc_grp_t *svc_grp,
                            axis2_env_t **env,
                            struct axis2_engine_config *parent) 
{
    axis2_svc_grp_impl_t *svc_grp_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, parent, AXIS2_FAILURE);
    
    svc_grp_impl = AXIS2_INTF_TO_IMPL(svc_grp);
    if(svc_grp_impl->parent)
        AXIS2_ENGINE_CONFIG_FREE(svc_grp_impl->parent, env);
    svc_grp_impl->parent = parent;
    return AXIS2_SUCCESS;
}
