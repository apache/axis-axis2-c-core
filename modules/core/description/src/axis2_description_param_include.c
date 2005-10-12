#include <axis2_description_param_include.h>
#include <stdlib.h>

static apr_pool_t *om_pool;

void axis2_description_param_include_add_parameter
    (axis2_description_param_include_t *param_include,
     axis2_parameter_t * param);
	 
axis2_parameter_t *axis2_description_param_include_get_parameter
    (axis2_description_param_include_t *param_include, char *name);

/* To get all the parameters in a given description */
apr_hash_t *axis2_description_param_include_get_parameters
    (axis2_description_param_include_t *param_include);

/* To check whether the paramter is locked at any level */
int axis2_description_param_include_is_parameter_locked
    (axis2_description_param_include_t *param_include, char *param_name);
	 
axis2_description_param_include_t *create_axis2_description_param_include ()
{
	axis2_description_param_include_t *param_include = 
		(axis2_description_param_include_t *)
		malloc(sizeof(axis2_description_param_include_t));
	if(param_include)
	{
		axis2_description_param_include_ops_t *ops = 
			malloc(sizeof(axis2_description_param_include_ops_t));
		if(ops)
		{
			param_include->ops->add_parameter = 
				(axis2_description_param_include_add_parameter_t) 
						axis2_description_param_include_add_parameter;
			param_include->ops->get_parameter = 
				(axis2_description_param_include_get_parameter_t) 
						axis2_description_param_include_get_parameter;
			param_include->ops->get_parameters = 
				(axis2_description_param_include_get_parameters_t) 
						axis2_description_param_include_get_parameters;
			param_include->ops->is_parameter_locked = 
				(axis2_description_param_include_is_parameter_locked_t) 
						axis2_description_param_include_is_parameter_locked;
			
			apr_pool_create (&om_pool, NULL);
			param_include->parameters = 
				(apr_hash_t *) axis2_apr_hash_make (om_pool);
			if(param_include->parameters)
			{
				
				param_include->is_locked = FALSE;
				return param_include;
			}
			else
				fprintf(stderr, "*** error_code:%d, %s\n",
					AXIS2_ERROR_OM_MEMORY_ALLOCATION,
					"AXIS2_ERROR_OM_MEMORY_ALLOCATION");
		}else
		{
			fprintf(stderr, "*** error_code:%d, %s\n", 
					AXIS2_ERROR_OM_MEMORY_ALLOCATION,
			 		"AXIS2_ERROR_OM_MEMORY_ALLOCATION");
		}
		
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", 
			AXIS2_ERROR_OM_MEMORY_ALLOCATION
			, "AXIS2_ERROR_OM_MEMORY_ALLOCATION");
	
	
	return NULL;
}

void free_axis2_description_param_include (axis2_description_param_include_t
    *param_include)
{
	if(param_include)
		free(param_include);
}

void axis2_description_param_include_add_parameter
    (axis2_description_param_include_t *param_include,
     axis2_parameter_t * param)
{
	if(param_include)
	{
		if (!(param_include->parameters))
		{        
            apr_pool_create (&om_pool, NULL);
			param_include->parameters = 
				(apr_hash_t *) axis2_apr_hash_make (om_pool);
		}
		if(param)
		{
			apr_hash_set (param_include->parameters, param->name,
				APR_HASH_KEY_STRING, param);
		}
	}
	fprintf(stderr, "*** error_code:%d, %s\n", AXIS2_ERROR_NULL_MEMORY_ACCESS
		, "AXIS2_ERROR_NULL_MEMORY_ACCESS");

}

axis2_parameter_t *axis2_description_param_include_get_parameter
    (axis2_description_param_include_t * param_include, char *name)
{
	if(param_include)
		return (axis2_parameter_t *)(apr_hash_get 
		(param_include->parameters, strdup(name), APR_HASH_KEY_STRING));
	return NULL;
}

/* To get all the parameters in a given description */
apr_hash_t *axis2_descriptin_param_include_get_parameters
    (axis2_description_param_include_t * param_include)
{
	if(param_include)
		return param_include->parameters;
	return NULL;
}

/* To check whether the paramter is locked at any level */
int axis2_description_param_include_is_parameter_locked
    (axis2_description_param_include_t * param_include, char *param_name)
{
	return ((axis2_parameter_t *)(apr_hash_get 
		(param_include->parameters, strdup(param_name), 
			APR_HASH_KEY_STRING)))->is_locked;
	return FALSE;
}
