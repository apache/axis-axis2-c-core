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

/*************************** Function headers ********************************/

axis2_status_t AXIS2_CALL 
axis2_svc_free (axis2_svc_t *svc, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc, axis2_env_t **env
		,                   axis2_operation_t *operation_desc);

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
	axis2_svc_t *svc = (axis2_svc_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_svc_t));
	if(!svc)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_svc_ops_t *ops = (axis2_svc_ops_t *)
		AXIS2_MALLOC (env->allocator, sizeof(axis2_svc_ops_t));
	if(!ops)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;	
	}
	ops->free = axis2_svc_free;
	ops->add_operation = axis2_svc_add_operation;
	ops->get_operation_with_qname = 
		axis2_svc_get_operation_with_qname;
	ops->get_operation_with_name = 
		axis2_svc_get_operation_with_name;
	ops->get_operations = axis2_svc_get_operations;
	ops->set_parent = axis2_svc_set_parent;
	ops->get_parent = axis2_svc_get_parent;
	ops->get_name = axis2_svc_get_name;
	ops->add_param = axis2_svc_add_param;
	ops->get_param = axis2_svc_get_param;
	ops->get_params = axis2_svc_get_params;
	
	svc->ops = ops;
	
	axis2_param_container_t *param_container 
		= (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(!param_container)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}

	svc->param_container = param_container;
	
	svc->parent = NULL;
	
	svc->wasaction_opeartionmap = axis2_hash_make (env);				
	if(!svc->wasaction_opeartionmap)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;			
	}
	svc->name = NULL;

	return svc;	
}

axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname)
{
	axis2_svc_t *svc = 
		axis2_svc_create(env);
	if(!svc)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	svc->name = axis2_strdup(qname);
	
	return svc;
}

/********************** Start of function implementations ********************/

axis2_status_t AXIS2_CALL
axis2_svc_free (axis2_svc_t *svc, 
                axis2_env_t **env)
{
	if(!env)
		return AXIS2_ERROR_INVALID_NULL_PARAM;
	if(svc)
	{
		AXIS2_FREE(env->allocator, svc);
		return AXIS2_SUCCESS;
	}
	return AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED;
}


axis2_status_t AXIS2_CALL
axis2_svc_add_operation (axis2_svc_t *svc,
                            axis2_env_t **env,
		                    axis2_operation_t *operation_desc)
{
	if(!svc || !operation_desc)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAM;
	}
	if (!(svc->wasaction_opeartionmap))
	{                    
		svc->wasaction_opeartionmap = axis2_hash_make (env);
	}
	axis2_operation_ops_t *tempopt = 
		(axis2_operation_get_ops(operation_desc, env));
	
	if(!tempopt) return AXIS2_ERROR_INVALID_NULL_PARAM;
		
	axis2_qname_t *tempqname = tempopt->get_name(operation_desc, env);
	
	if(!tempqname) return AXIS2_ERROR_INVALID_NULL_PARAM;
		
	axis2_char_t *tempname = tempqname->localpart;
	
	if(!tempname) return AXIS2_ERROR_INVALID_NULL_PARAM;
	
	axis2_hash_set (svc->wasaction_opeartionmap		
		, tempname
		, AXIS2_HASH_KEY_STRING, operation_desc);
	
	return AXIS2_SUCCESS;
}

axis2_operation_t * AXIS2_CALL
axis2_svc_get_operation_with_qname (axis2_svc_t *svc,
                                        axis2_env_t **env,
		                                axis2_qname_t *operation_name)
{
	if(!svc || !svc->wasaction_opeartionmap)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;		
	}
	return (axis2_operation_t *) (axis2_hash_get 
		(svc->wasaction_opeartionmap, operation_name->localpart
		, AXIS2_HASH_KEY_STRING));
	
}	

axis2_operation_t * AXIS2_CALL
axis2_svc_get_operation_with_name (axis2_svc_t *svc, 
                                    axis2_env_t **env,
		                            const axis2_char_t* name)
{
	if(!svc || !svc->wasaction_opeartionmap)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;		
	}
	return (axis2_operation_t *) (axis2_hash_get 
		(svc->wasaction_opeartionmap
		, axis2_strdup(name), AXIS2_HASH_KEY_STRING));
}

axis2_hash_t * AXIS2_CALL
axis2_svc_get_operations (axis2_svc_t *svc, 
                            axis2_env_t **env)
{
	if(!svc)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;		
	}
	return svc->wasaction_opeartionmap;
}
	
axis2_status_t AXIS2_CALL
axis2_svc_set_parent (axis2_svc_t *svc, 
                        axis2_env_t **env,
		                axis2_svc_grp_t *svc_grp)
{
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
