#include <axis2_description_operation.h>

/**
  * @struct axis2_description_operation_s
  * @brief DESCRIPTION operation
  * This holds the information about operation.
  */
struct axis2_description_operation_s
{
	axis2_description_param_include_t *param_include;
	axis2_service_description_t *parent;
	axis2_qname_t *name;	

};

axis2_description_operation_t *axis2_description_operation_create 
		(axis2_environment_t *env)
{
	axis2_description_param_include_t * param_include = NULL;
	axis2_description_operation_t *operation_desc 
		= (axis2_description_operation_t *)
			malloc (sizeof (axis2_description_operation_t));
	if(operation_desc)
	{
		
		param_include = (axis2_description_param_include_t *)
			create_axis2_description_param_include();		
		if(param_include)
		{
			operation_desc->param_include = param_include;
			operation_desc->parent = NULL;			
						
			return operation_desc;
		}
		else
			fprintf(stderr, "*** error_code:%d, %s\n"
			, AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
			
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
	return NULL;
}

axis2_description_operation_t 
		*axis2_description_operation_create_with_param_include 
		(axis2_environment_t *env
		, axis2_description_param_include_t* param_include)
{
	axis2_description_operation_t *operation_desc = 
		(axis2_description_operation_t *)
		malloc (sizeof (axis2_description_operation_t));
	if(operation_desc)
	{
		if(!param_include)
			param_include = (axis2_description_param_include_t *)
				create_axis2_description_param_include();
		if(param_include)
		{
			operation_desc->param_include = param_include;
			operation_desc->parent = NULL;
			return operation_desc;
		}
		else
			fprintf(stderr, "*** error_code:%d, %s\n"
			, AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
			
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
	return NULL;
}

axis2_description_operation_t *create_axis2_description_operation_with_name 
		(axis2_environment_t *env
		, axis2_description_param_include_t *param_include, axis2_qname_t *name)
{
	axis2_description_operation_t *operation_desc = 
		(axis2_description_operation_t *)
		malloc (sizeof (axis2_description_operation_t));
	if(operation_desc)
	{
		if(!param_include)
			param_include = (axis2_description_param_include_t *)
				create_axis2_description_param_include();
		if(param_include)
		{
			operation_desc->param_include = param_include;
			if(name)
				operation_desc->name = name;
			operation_desc->parent = NULL;
			return operation_desc;
		}
		else
			fprintf(stderr, "*** error_code:%d, %s\n"
			, AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
			
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
	return NULL;
}

axis2_status_t axis2_description_operation_free (axis2_environment_t *env
		, axis2_description_operation_t *operation_desc)
{
	if(operation_desc){
		free(operation_desc);
		return AXIS2_SUCCESS;
	}
	return AXIS2_FAILURE;
}	
	
axis2_status_t axis2_description_operation_add_parameter
		(axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, axis2_parameter_t *param)
{
	apr_hash_set (operation_desc->param_include->parameters, param->name
		, APR_HASH_KEY_STRING, param);	
	return AXIS2_SUCCESS;
	
}

axis2_description_param_t *axis2_description_operation_get_parameter(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc
		, const axis2_char_t *name)
{
	if(operation_desc && operation_desc->param_include)
		return (axis2_parameter_t *)(apr_hash_get 
		(operation_desc->param_include->parameters, strdup(name)
		, APR_HASH_KEY_STRING));
	return NULL;
}

axis2_description_param_t *axis2_description_operation_get_parameters(
		axis2_environment_t *env, axis2_description_operation_t *operation_desc)
{
	if(operation_desc && operation_desc->param_include)
		return (axis2_parameter_t *) operation_desc->param_include->parameters;
	return NULL;
}	

axis2_status_t axis2_description_operation_set_parent(axis2_environment_t *env
		,axis2_description_operation_t *operation_desc
		, axis2_description_service_t *service_desc)
{
	if(operation_desc && service_desc){
		operation_desc->parent = service_desc;
		return AXIS2_SUCCESS;
	}
	return AXIS2_FAILURE;
}

axis2_description_service_t *axis2_description_operation_get_parent
		(axis2_environment_t *env
		, axis2_description_operation_t *operation_desc)
{
	if(operation_desc)
		return operation_desc->parent;
	else return NULL;
}
