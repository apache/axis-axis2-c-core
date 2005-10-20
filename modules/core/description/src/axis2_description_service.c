#include <axis2_description_service.h>

/**
  * @struct axis2_description_service
  * @brief DESCRIPTION service struct
  * This holds the information about service
  */
struct axis2_description_service_s
{
	axis2_description_service_ops_t *ops;
	axis2_description_param_include_t *param_include;
	axis2_description_servicegroup_t *parent;
	axis2_hash_t *wasaction_opeartionmap;
	axis2_qname_t *qname;    

};

/*************************** Function headers ********************************/

axis2_status_t axis2_description_service_ops_free (axis2_environment_t *env
		, axis2_description_service_t *service_desc);

axis2_status_t axis2_description_service_ops_add_param(axis2_environment_t *env
		, axis2_description_service_t *operation_desc
		, axis2_description_param_t *param);

axis2_description_param_t *axis2_description_service_ops_get_param
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t *name);

axis2_hash_t *axis2_description_service_ops_get_params
		(axis2_environment_t *env, axis2_description_service_t *service_desc);

axis2_status_t axis2_description_service_ops_add_operation
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_operation_t *operation_desc);

axis2_description_operation_t *
		axis2_description_service_ops_get_operation_with_qname(
		axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_qname_t *operation_name);
		
axis2_description_operation_t *
		axis2_description_service_ops_get_operation_with_name
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t* operation_name);

axis2_hash_t *axis2_description_service_ops_get_operations(
		axis2_environment_t *env, axis2_description_service_t *service_desc);
		
axis2_status_t axis2_description_service_ops_set_parent(axis2_environment_t *env
		, axis2_description_service_t *service_desc
		,axis2_description_servicegroup_t *servicegroup_desc);

axis2_description_servicegroup_t *axis2_description_service_ops_get_parent
		(axis2_environment_t *env, axis2_description_service_t *service_desc);
		
/************************* End of function headers ***************************/

axis2_description_service_ops_t * axis2_description_service_get_ops
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	if(!service_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return service_desc->ops;
}

axis2_description_service_t *axis2_description_service_create
		(axis2_environment_t *env)
{
	axis2_description_service_t *service_desc = (axis2_description_service_t *)
		axis2_malloc (env->allocator, sizeof(axis2_description_service_t));
	if(!service_desc)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_description_service_ops_t *ops = (axis2_description_service_ops_t *)
		axis2_malloc (env->allocator, sizeof(axis2_description_service_ops_t));
	if(!ops)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;	
	}
	ops->free = axis2_description_service_ops_free;
	ops->add_operation = axis2_description_service_ops_add_operation;
	ops->add_param = axis2_description_service_ops_add_param;
	ops->get_param = axis2_description_service_ops_get_param;
	ops->get_params = axis2_description_service_ops_get_params;
	ops->get_operation_with_qname = 
		axis2_description_service_ops_get_operation_with_qname;
	ops->get_operation_with_name = 
		axis2_description_service_ops_get_operation_with_name;
	ops->get_operations = axis2_description_service_ops_get_operations;
	ops->set_parent = axis2_description_service_ops_set_parent;
	ops->get_parent = axis2_description_service_ops_get_parent;
	
	service_desc->ops = ops;
	
	axis2_description_param_include_t *param_include 
		= (axis2_description_param_include_t *)
		axis2_description_param_include_create(env);		
	if(!param_include)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}

	service_desc->param_include = param_include;
	
	service_desc->parent = NULL;
	
	service_desc->wasaction_opeartionmap = axis2_hash_make (env);				
	if(!service_desc->wasaction_opeartionmap)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;			
	}
	service_desc->qname = NULL;

	return service_desc;	
}

axis2_description_service_t *axis2_description_service_create_with_qname
		(axis2_environment_t *env, axis2_qname_t *qname)
{
	axis2_description_service_t *service_desc = 
		axis2_description_service_create(env);
	if(!service_desc)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	service_desc->qname = axis2_strdup(env->string, qname);
	
	return service_desc;
}

/********************** Start of function implementations ********************/

axis2_status_t axis2_description_service_ops_free
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	if(service_desc)
		axis2_free(env->allocator, service_desc);
	return AXIS2_SUCCESS;
}

axis2_status_t axis2_description_service_ops_add_param(axis2_environment_t *env
		, axis2_description_service_t *service_desc
		, axis2_description_param_t *param)
{
	if(!service_desc || !service_desc->param_include || !param)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	axis2_hash_set (axis2_description_param_include_get_params(env
		, service_desc->param_include), axis2_description_param_get_name(env
		, param)
		, AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
}

axis2_description_param_t *axis2_description_service_ops_get_param(
		axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t *name)
{
	if(!service_desc || !service_desc->param_include)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	axis2_char_t *tempname = axis2_strdup(env->string, name);
	if(!tempname)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
		
	return (axis2_description_param_t *)(axis2_hash_get 
		(axis2_description_param_include_get_params(env
		, service_desc->param_include), tempname, AXIS2_HASH_KEY_STRING));
	
}

axis2_hash_t *axis2_description_service_ops_get_params
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	if(!service_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;		
	}
	return axis2_description_param_include_get_params(env
		, service_desc->param_include);
	
}

axis2_status_t axis2_description_service_ops_add_operation
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_description_operation_t *operation_desc)
{
	if(!service_desc || !operation_desc)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	}
	if (!(service_desc->wasaction_opeartionmap))
	{                    
		service_desc->wasaction_opeartionmap = axis2_hash_make (env);
	}
	axis2_description_operation_ops_t *tempopt = 
		(axis2_description_operation_get_ops(env, operation_desc));
	
	if(!tempopt) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
		
	axis2_qname_t *tempqname = tempopt->get_name(env, operation_desc);
	
	if(!tempqname) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
		
	axis2_char_t *tempname = tempqname->localpart;
	
	if(!tempname) return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	
	axis2_hash_set (service_desc->wasaction_opeartionmap		
		, tempname
		, AXIS2_HASH_KEY_STRING, operation_desc);
	
	return AXIS2_SUCCESS;
}

axis2_description_operation_t 
		*axis2_description_service_ops_get_operation_with_qname
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, axis2_qname_t *operation_name)
{
	if(!service_desc || !service_desc->wasaction_opeartionmap)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;		
	}
	return (axis2_description_operation_t *) (axis2_hash_get 
		(service_desc->wasaction_opeartionmap, operation_name->localpart
		, AXIS2_HASH_KEY_STRING));
	
}	

axis2_description_operation_t 
		*axis2_description_service_ops_get_operation_with_name
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		, const axis2_char_t* name)
{
	if(!service_desc || !service_desc->wasaction_opeartionmap)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;		
	}
	return (axis2_description_operation_t *) (axis2_hash_get 
		(service_desc->wasaction_opeartionmap
		, axis2_strdup(env->string, name), AXIS2_HASH_KEY_STRING));
}

axis2_hash_t *axis2_description_service_ops_get_operations
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	if(!service_desc)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;		
	}
	return service_desc->wasaction_opeartionmap;
}
	
axis2_status_t axis2_description_service_ops_set_parent
		(axis2_environment_t *env, axis2_description_service_t *service_desc
		,axis2_description_servicegroup_t *servicegroup_desc)
{
	if(!service_desc || !servicegroup_desc)
	{
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;	
	}
	service_desc->parent = servicegroup_desc;
	return AXIS2_SUCCESS;
}

axis2_description_servicegroup_t *axis2_description_service_ops_get_parent
		(axis2_environment_t *env, axis2_description_service_t *service_desc)
{
	if(!service_desc || !service_desc->parent)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;	
	}
	return service_desc->parent;
}
