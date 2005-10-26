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
 
#include <axis2_wsdl_operation.h>

/** @struct axis2_wsdl_operation_s
  * @brief WSDL operation
  * This holds operations of wsdl operation
  */
struct axis2_wsdl_operation_s
{
	axis2_wsdl_operation_ops_t *ops;
	
};

/***************************** Function headers *******************************/

axis2_status_t axis2_wsdl_operation_ops_free(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt);


/***************************** End of function headers ************************/

axis2_wsdl_operation_ops_t *axis2_wsdl_operation_get_ops
		(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt)
{
	if(!env || !wsdl_opt)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return wsdl_opt->ops;
}

axis2_wsdl_operation_t *axis2_wsdl_operation_create
		(axis2_environment_t *env)
{
	axis2_wsdl_operation_t *wsdl_opt 
		= (axis2_wsdl_operation_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_operation_t));
	if(!wsdl_opt)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_wsdl_operation_ops_t *ops 
		= (axis2_wsdl_operation_ops_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_operation_ops_t));
	if(!ops)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;	
	}
	ops->free = axis2_wsdl_operation_ops_free;
	
	
	wsdl_opt->ops = ops;

	return wsdl_opt;	
}

/******************************************************************************/

axis2_status_t axis2_wsdl_operation_ops_free(axis2_environment_t *env
		, axis2_wsdl_operation_t *wsdl_opt)
{
	if(!env || !wsdl_opt)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	axis2_free(env->allocator, wsdl_opt);
	return AXIS2_SUCCESS;
}
