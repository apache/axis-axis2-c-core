#include <axis2_operation_description.h>

axis2_operation_description_t 
	*create_axis2_operation_description ()
{
	axis2_description_param_include_t * param_include = NULL;
	axis2_operation_description_t *operation_desc = (axis2_operation_description_t *)
			malloc (sizeof (axis2_operation_description_t));
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
			fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
			
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
	return NULL;
}

axis2_operation_description_t 
	*create_axis2_operation_description_with_param_include (
	axis2_description_param_include_t* param_include)
{
	axis2_operation_description_t *operation_desc = (axis2_operation_description_t *)
			malloc (sizeof (axis2_operation_description_t));
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
			fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
			
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
	return NULL;
}

axis2_operation_description_t *create_axis2_operation_description_with_name 
	(axis2_description_param_include_t *param_include, axis2_qname_t *name)
{
	axis2_operation_description_t *operation_desc = (axis2_operation_description_t *)
			malloc (sizeof (axis2_operation_description_t));
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
			fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
			
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_MEMORY_ALLOCATION
			, "AXIS2_ERROR_MEMORY_ALLOCATION");
	return NULL;
}

void free_axis2_operation_description (axis2_operation_description_t
    *operation_desc)
{
	if(operation_desc)
		free(operation_desc);
}	
	
void axis2_operation_description_add_parameter(axis2_operation_description_t
	*operation_desc, axis2_parameter_t *param)
{
	
}

axis2_parameter_t *axis2_operation_description_get_parameter(
	axis2_operation_description_t *operation_desc, char *name)
{
}

axis2_parameter_t *axis2_operation_description_get_parameters(
	axis2_operation_description_t * operation_desc)
{
}	

void setParent(axis2_operation_description_t *operation_description
	, axis2_service_description_t *service_description)
{
}

axis2_service_description_t *getParent(axis2_operation_description_t 
	*operation_desc)
{
}
