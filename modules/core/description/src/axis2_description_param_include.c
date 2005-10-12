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
 
#include <axis2_description_param_include.h>

static apr_pool_t *om_pool;

int axis2_description_param_include_add_parameter
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
	 
axis2_description_param_include_t *axis2_description_param_include_create ()
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
					AXIS2_ERROR_NO_MEMORY,
					"AXIS2_ERROR_NO_MEMORY");
		}else
		{
			fprintf(stderr, "*** error_code:%d, %s\n", 
					AXIS2_ERROR_NO_MEMORY,
			 		"AXIS2_ERROR_NO_MEMORY");
		}
		
	}
	else
		fprintf(stderr, "*** error_code:%d, %s\n", 
			AXIS2_ERROR_NO_MEMORY
			, "AXIS2_ERROR_NO_MEMORY");
	
	
	return NULL;
}

int axis2_description_param_include_free (axis2_description_param_include_t
    *param_include)
{
	if(param_include)
		free(param_include);
}

int axis2_description_param_include_add_parameter
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
	fprintf(stderr, "*** error_code:%d, %s\n"
		, AXIS2_ERROR_INVALID_NULL_PARAMETER
		, "AXIS2_ERROR_INVALID_NULL_PARAMETER");

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
