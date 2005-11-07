#include <axis2_svc.h>

typedef struct axis2_svc_impl_s axis2_svc_impl_t;
	
/** 
 * @brief Service struct impl
 *	Axis2 Service impl  
 */ 
struct axis2_svc_impl_s
{
	axis2_svc_t svc;
	axis2_param_container_t *param_container;
	axis2_svc_grp_t *parent;
	axis2_hash_t *wasaction_opeartionmap;
	axis2_qname_t *name;    

};

#define AXIS2_INTF_TO_IMPL(svc) ((axis2_operation_impl_t *)svc)

/*************************** Function headers ********************************/

axis2_status_t AXIS2_CALL 
axis2_svc_free (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc, axis2_env_t **env
		,                   axis2_operation_t *operation);

axis2_operation_t * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc, axis2_env_t **env,
		                            axis2_qname_t *operation_name);
		
axis2_operation_t * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, axis2_env_t **env,
		                            const axis2_char_t* operation_name);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, axis2_env_t **env, 
                        axis2_svc_grp_t *svc_grp);

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, axis2_env_t **env);
		
axis2_qname_t * AXIS2_CALL
axis2_svc_get_name (const axis2_svc_t *svc, axis2_env_t **env);	

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, axis2_env_t **env, axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, axis2_env_t **env,
		                const axis2_char_t *name);

axis2_hash_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, axis2_env_t **env,
		                    const axis2_char_t *param_name);
		
/************************* End of function headers ***************************/

axis2_svc_t * AXIS2_CALL
axis2_svc_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
	axis2_svc_impl_t *svc_impl = (axis2_svc_impl_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_svc_impl_t));
    if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	svc_impl->ops->free = axis2_svc_free;
	svc_impl->ops->add_operation = axis2_svc_add_operation;
	svc_impl->ops->get_operation_with_qname = 
		axis2_svc_get_operation_with_qname;
	svc_impl->ops->get_operation_with_name = 
		axis2_svc_get_operation_with_name;
	svc_impl->ops->get_operations = axis2_svc_get_operations;
	svc_impl->ops->set_parent = axis2_svc_set_parent;
	svc_impl->ops->get_parent = axis2_svc_get_parent;
	svc_impl->ops->get_name = axis2_svc_get_name;
	svc_impl->ops->add_param = axis2_svc_add_param;
	svc_impl->ops->get_param = axis2_svc_get_param;
	svc_impl->ops->get_params = axis2_svc_get_params;
	
	axis2_param_container_t *param_container 
		= (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == param_container)
	{
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	svc_impl->param_container = param_container;
	
	svc_impl->parent = NULL;
	
	svc_impl->wasaction_opeartionmap = axis2_hash_make (env);				
	if(NULL == svc_impl->wasaction_opeartionmap)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}
	svc_impl->name = NULL;

	return &(svc_impl->svc);	
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname)
{
	axis2_svc_impl_t *svc_impl = axis2_svc_create(env);
	if(NULL == svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
	svc_impl->name = qname;
	
	return &(svc_impl->svc);
}

/********************** Start of function implementations ********************/

axis2_status_t AXIS2_CALL
axis2_svc_free (axis2_svc_t *svc, 
                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
	if(NULL != svc->ops)
		AXIS2_FREE((*env)->allocator, svc->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->param_container)
	    AXIS2_PARAM_CONTAINER_FREE(AXIS2_INTF_TO_IMPL(svc)->param_container
        , env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->parent)
	    AXIS2_SVC_GRP_FREE(AXIS2_INTF_TO_IMPL(svc)->parent, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap)
		axis2_hash_free(AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(svc)->qname)
	    AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(svc)->qname, env);  
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(svc));
    
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc,
                            axis2_env_t **env,
		                    axis2_operation_t *operation)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, operation, AXIS2_FALSE);
	
	if (NULL == (AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap))
	{                    
		AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap = axis2_hash_make (env);
	}
    
    axis2_qname_t *tempqname = AXIS2_OPERATION_GET_NAME(operation, env);
	
	if(!tempqname) return AXIS2_ERROR_INVALID_NULL_PARAM;
		
	axis2_char_t *tempname = tempqname->localpart;
	
    if(NULL == tempname)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	
	axis2_hash_set (AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap		
		, tempname
		, AXIS2_HASH_KEY_STRING, operation);
	
	return AXIS2_SUCCESS;
}

axis2_operation_t * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc,
                                        axis2_env_t **env,
		                                axis2_qname_t *operation_name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, operation_name, NULL);
	if(NULL == AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_NULL_PARAM, NULL);	
	}
    
	return (axis2_operation_t *) (axis2_hash_get (AXIS2_INTF_TO_IMPL(svc)->
        wasaction_opeartionmap, operation_name->localpart, AXIS2_HASH_KEY_STRING));
	
}	

axis2_operation_t * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, 
                                    axis2_env_t **env,
		                            const axis2_char_t* name)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
	axis2_char_t temp_name = axis2_strdup(name);
    AXIS2_PARAM_CHECK((*env)->error, temp_name, NULL); 
    
	return (axis2_operation_t *) (axis2_hash_get 
		(AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap
		, temp_name, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, NULL);
	
	return AXIS2_INTF_TO_IMPL(svc)->wasaction_opeartionmap;
}
	
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_svc_grp_t *svc_grp)
{
    AXIS2_FUNC_PARAM_CHECK(svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(
	if(!svc || !svc_grp)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAM;	
	}
	svc->parent = svc_grp;
	return AXIS2_SUCCESS;
}

axis2_svc_grp_t * AXIS2_CALL
axis2_svc_get_parent (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
	if(!svc || !svc->parent)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;	
	}
	return svc->parent;
}

axis2_qname_t * AXIS2_CALL
axis2_svc_get_name (const axis2_svc_t *svc, 
                    axis2_env_t **env)
{
	if(!env || !svc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;	
	}
	return svc->name;
}

axis2_status_t AXIS2_CALL
axis2_svc_add_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_param_t *param)
{
	if(!env || !svc || !svc->param_container || !param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAM;
	}
	axis2_hash_set (axis2_param_container_get_params
		(svc->param_container, env), axis2_param_get_name(param
		, env)
		, AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
}

axis2_param_t * AXIS2_CALL
axis2_svc_get_param (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                const axis2_char_t *name)
{
	if(!env || !svc || !svc->param_container)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;
	}
	axis2_char_t *tempname = axis2_strdup(name);
	if(!tempname)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
		
	return (axis2_param_t *)(axis2_hash_get 
		(axis2_param_container_get_params(svc->param_container, env)
		, tempname, AXIS2_HASH_KEY_STRING));
	
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_params (axis2_svc_t *svc, 
                        axis2_env_t **env)
{
	if(!env || !svc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;		
	}
	return axis2_param_container_get_params(svc->param_container, env);
	
}

axis2_bool_t AXIS2_CALL
axis2_svc_is_param_locked (axis2_svc_t *svc, 
                            axis2_env_t **env,
		                    const axis2_char_t *param_name)
{
	if(!env || !svc || !svc->param_container)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return AXIS2_FALSE;
	}
	axis2_char_t *tempname = axis2_strdup(param_name);
	if(!tempname)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return AXIS2_FALSE;
	}
		
	return axis2_param_container_is_param_locked
		(svc->param_container, env, param_name); 
	
}
