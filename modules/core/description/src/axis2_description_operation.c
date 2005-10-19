#include <axis2_description_operation.h>

/**
  * @struct axis2_description_operation_s
  * @brief DESCRIPTION operation
  * This holds the information about operation.
  */
struct axis2_description_operation_s
{
	axis2_description_operation_ops_t *ops;
	axis2_description_param_include_t *param_include;
	axis2_description_service_t *parent;
	axis2_qname_t *name;	

};

	
/*************************** Function headers *********************************/

axis2_status_t axis2_description_operation_ops_free (axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);	

axis2_status_t axis2_description_operation_ops_add_param
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_description_param_t *param);

axis2_description_param_t *axis2_description_operation_ops_get_param
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *name);

axis2_hash_t *axis2_description_operation_ops_get_params
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);

axis2_status_t axis2_description_operation_ops_set_parent
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_description_service_t *service_desc);

axis2_description_service_t *axis2_description_operation_ops_get_parent
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc);
		
/************************* End of function headers ****************************/	

axis2_description_operation_t *axis2_description_operation_get_ops
		(axis2_environment_t *env)
{
	return NULL;
}

axis2_description_operation_t *axis2_description_operation_create_local 
		(axis2_environment_t *env)
{
	axis2_description_operation_ops_t *ops = NULL;
	axis2_description_operation_t *operation_desc = 
		(axis2_description_operation_t *) axis2_malloc (env->allocator
		, sizeof (axis2_description_operation_t));
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	ops = (axis2_description_operation_ops_t *) axis2_malloc(env->allocator,
		sizeof(axis2_description_operation_ops_t));
	if(!ops)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}		

	ops->free = axis2_description_operation_ops_free;
	ops->add_param = axis2_description_operation_ops_add_param;
	ops->get_param = axis2_description_operation_ops_get_param;
	ops->get_params = axis2_description_operation_ops_get_params;
	/*ops->set_parent = axis2_description_operation_ops_set_parent;
	ops->get_parent = axis2_description_ops_operation_ops_get_parent;*/
	
	operation_desc->ops = ops;
	operation_desc->param_include = NULL;
	operation_desc->name = NULL;
	/*operation_desc->parent = NULL;*/
						
	return operation_desc;
}

axis2_description_operation_t *axis2_description_operation_create 
		(axis2_environment_t *env)
{
	axis2_description_param_include_t * param_include = NULL;
	axis2_description_operation_t *operation_desc 
		= axis2_description_operation_create_local(env);
	
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
			
	param_include = (axis2_description_param_include_t *)
		axis2_description_param_include_create(env);		
	if(!param_include)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}

	operation_desc->param_include = param_include;
						
	return operation_desc;
}

axis2_description_operation_t *axis2_description_operation_create_with_name 
		(axis2_environment_t *env, axis2_qname_t *name)
{
	axis2_description_operation_t *operation_desc 
		= axis2_description_operation_create_local(env);
	
	if(!operation_desc)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	if(!name)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}		
						
	return operation_desc;	
}

/******************************************************************************/

axis2_status_t axis2_description_operation_ops_free (axis2_environment_t *env
		, axis2_description_operation_t *operation_desc)
{
	if(operation_desc){
		free(operation_desc);
		return AXIS2_SUCCESS;
	}
	return AXIS2_FAILURE;
}	
	
axis2_status_t axis2_description_operation_ops_add_param
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_description_param_t *param)
{
	axis2_hash_set (axis2_description_param_include_get_params(env
		, operation_desc->param_include), axis2_description_param_get_name(env
		, param)
		, AXIS2_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
	
}

axis2_description_param_t *axis2_description_operation_ops_get_param(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *name)
{
	if(!operation_desc || !operation_desc->param_include)
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
		, operation_desc->param_include), tempname, AXIS2_HASH_KEY_STRING));
}

axis2_hash_t *axis2_description_operation_ops_get_params(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc)
{
	if(!operation_desc || !operation_desc->param_include)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	
	return axis2_description_param_include_get_params(env
		, operation_desc->param_include);
}	

axis2_status_t axis2_description_operation_ops_set_parent(axis2_environment_t *env
		,axis2_description_operation_t *operation_desc
		, axis2_description_service_t *service_desc)
{
	if(operation_desc && service_desc){
		operation_desc->parent = service_desc;
		return AXIS2_SUCCESS;
	}
	return AXIS2_ERROR_INVALID_NULL_PARAMETER;
}

axis2_description_service_t *axis2_description_ops_operation_ops_get_parent
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc)
{
	if(operation_desc)
		return operation_desc->parent;
	else return NULL;
}
