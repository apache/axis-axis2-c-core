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

typedef struct axis2_svc_grp_impl_s axis2_svc_grp_impl_t;

/** 
 * @brief Service group struct impl
 * Axis2 Service group impl  
 */  
struct axis2_svc_grp_impl_s
{
	axis2_svc_grp_t svc_grp;
	axis2_param_container_t *param_container;
	axis2_char_t *svc_grp_name;
	axis2_hash_t *svcs;
	
};

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
		                const axis2_qname_t* svc_name);
		
axis2_status_t AXIS2_CALL 
axis2_svc_grp_remove_svc (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    const axis2_qname_t* svc_name);
		
axis2_status_t AXIS2_CALL 
axis2_svc_grp_add_param (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    axis2_param_t *param);
		

axis2_param_t * AXIS2_CALL 
axis2_svc_grp_get_param(axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                const axis2_char_t *name);
		
axis2_hash_t * AXIS2_CALL 
axis2_svc_grp_get_params (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env);
		
axis2_bool_t AXIS2_CALL 
axis2_svc_grp_is_param_locked(axis2_svc_grp_t *svc_grp, 
                                axis2_env_t **env,
		                        const axis2_char_t *param_name);

/***************************** End of function headers ************************/

axis2_svc_grp_t * AXIS2_CALL 
axis2_svc_grp_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
	axis2_svc_grp_impl_t *svc_grp_impl = (axis2_svc_grp_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_svc_grp_impl_t));
    
	if(NULL == svc_grp_impl)
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	
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
	svc_grp_impl->svc_grp.ops->remove_svc = axis2_svc_grp_remove_svc;
	svc_grp_impl->svc_grp.ops->add_param = axis2_svc_grp_add_param;
	svc_grp_impl->svc_grp.ops->get_param = axis2_svc_grp_get_param;
	svc_grp_impl->svc_grp.ops->get_params = axis2_svc_grp_get_params;
	svc_grp_impl->svc_grp.ops->is_param_locked = axis2_svc_grp_is_param_locked;
	
	
	
	axis2_param_container_t *param_container = (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == param_container)
	{
        AXIS2_FREE((*env)->allocator, svc_grp_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	svc_grp_impl->param_container = param_container;
	
	/*svc_grp_impl->parent = NULL;*/
	
	svc_grp_impl->svcs = axis2_hash_make (env);				
	if(NULL == svc_grp_impl->svcs)
    {
        AXIS2_FREE((*env)->allocator, svc_grp_impl);
        AXIS2_FREE((*env)->allocator, svc_grp_impl->param_container);
	    AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
	svc_grp_impl->svc_grp_name = NULL;

	return &(svc_grp_impl->svc_grp);	
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_svc_grp_free (axis2_svc_grp_t *svc_grp, 
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    
    if(NULL != svc_grp->ops)
		AXIS2_FREE((*env)->allocator, svc_grp->ops);
    
	if(NULL != AXIS2_INTF_TO_IMPL(svc_grp)->param_container)
        AXIS2_PARAM_CONTAINER_FREE(AXIS2_INTF_TO_IMPL(svc_grp)->param_container, 
            env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc_grp)->svc_grp_name)
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svc_grp)->svc_grp_name);
    
    axis2_hash_free(AXIS2_INTF_TO_IMPL(svc_grp)->svcs, env);
    
	AXIS2_FREE((*env)->allocator, svc_grp);
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_set_name (axis2_svc_grp_t *svc_grp, 
                        axis2_env_t **env,
		                axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
	
	axis2_char_t *tempname = axis2_strdup(name);
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
		                const axis2_qname_t* svc_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_name, NULL);
	
	return (axis2_svc_t *) (axis2_hash_get (AXIS2_INTF_TO_IMPL(svc_grp)->svcs, 
        AXIS2_QNAME_GET_LOCALPART(svc_name, env), AXIS2_HASH_KEY_STRING));
}

axis2_status_t AXIS2_CALL 
axis2_svc_grp_remove_svc (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    const axis2_qname_t* svc_name)
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
    
    if(NULL == AXIS2_INTF_TO_IMPL(svc_grp)->param_container)
	    AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FAILURE);
    
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FAILURE);
	
	axis2_hash_set (AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(
        svc_grp)->param_container, env), AXIS2_PARAM_GET_NAME(param, env), 
        AXIS2_HASH_KEY_STRING, param);
	
	return AXIS2_SUCCESS;
}

axis2_param_t * AXIS2_CALL 
axis2_svc_grp_get_param (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env,
		                    const axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FAILURE);
    
	if(NULL == AXIS2_INTF_TO_IMPL(svc_grp)->param_container)
	    AXIS2_ERROR_SET((*env)->error
        , AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, NULL);
    
	axis2_char_t *tempname = axis2_strdup(name);
    
	if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FALSE);
		
	return (axis2_param_t *)(axis2_hash_get (AXIS2_PARAM_CONTAINER_GET_PARAMS(
        AXIS2_INTF_TO_IMPL(svc_grp)->param_container, env), tempname, 
        AXIS2_HASH_KEY_STRING));
	
}

axis2_hash_t * AXIS2_CALL 
axis2_svc_grp_get_params (axis2_svc_grp_t *svc_grp, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, NULL);
	
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(AXIS2_INTF_TO_IMPL(svc_grp)->
        param_container, env);
	
}

axis2_bool_t AXIS2_CALL 
axis2_svc_grp_is_param_locked (axis2_svc_grp_t *svc_grp, 
                                axis2_env_t **env,
		                        const axis2_char_t *param_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc_grp, env, AXIS2_FALSE);
	
	if(NULL == AXIS2_INTF_TO_IMPL(svc_grp)->param_container)
	    AXIS2_ERROR_SET((*env)->error
        , AXIS2_ERROR_INVALID_STATE_PARAM_CONTAINER, AXIS2_FALSE);
	
	axis2_char_t *tempname = axis2_strdup(param_name);
	if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FALSE);
	
	return AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED
		(AXIS2_INTF_TO_IMPL(svc_grp)->param_container, env, param_name); 
	
}
