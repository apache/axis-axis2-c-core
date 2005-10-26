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
 
#include <axis2_wsdl_service.h>

/** @struct axis2_wsdl_service_s
  * @brief WSDL service
  * This holds operations of wsdl service
  */
struct axis2_wsdl_service_s
{
	axis2_wsdl_service_ops_t *ops;
	
};

/***************************** Function headers *******************************/

axis2_status_t axis2_wsdl_service_ops_free(axis2_environment_t *env
		, axis2_wsdl_service_t *wsdl_srv);


/***************************** End of function headers ************************/

axis2_wsdl_service_ops_t *axis2_wsdl_service_get_ops
		(axis2_environment_t *env
		, axis2_wsdl_service_t *wsdl_srv)
{
	if(!env || !wsdl_srv)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;
	}
	return wsdl_srv->ops;
}

axis2_wsdl_service_t *axis2_wsdl_service_create
		(axis2_environment_t *env)
{
	axis2_wsdl_service_t *wsdl_srv 
		= (axis2_wsdl_service_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_service_t));
	if(!wsdl_srv)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	axis2_wsdl_service_ops_t *ops 
		= (axis2_wsdl_service_ops_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_service_ops_t));
	if(!ops)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;	
	}
	ops->free = axis2_wsdl_service_ops_free;
	
	wsdl_srv->ops = ops;
	
	return wsdl_srv;	
}

/******************************************************************************/

axis2_status_t axis2_wsdl_service_ops_free(axis2_environment_t *env
		, axis2_wsdl_service_t *wsdl_srv)
{
	if(!env || !wsdl_srv)
		return AXIS2_ERROR_INVALID_NULL_PARAMETER;
	axis2_free(env->allocator, wsdl_srv);
	return AXIS2_SUCCESS;
}
