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
  
typedef struct axis2_wsdl_impl_service_s axis2_wsdl_impl_service_t;  
  
struct axis2_wsdl_impl_service_s
{
	axis2_wsdl_service_t wsdl_srv;
	
};

/***************************** Function headers *******************************/

axis2_status_t axis2_wsdl_service_free(axis2_env_t *env
		, axis2_wsdl_service_t *wsdl_srv);


/***************************** End of function headers ************************/

axis2_status_t axis2_wsdl_service_create
		(axis2_env_t *env, axis2_wsdl_service_t **wsdl_srv)
{
	axis2_wsdl_impl_service_t *wsdl_impl_srv 
		= (axis2_wsdl_impl_service_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_service_t));
	if(NULL == wsdl_impl_srv)
	{
		return AXIS2_ERROR_NO_MEMORY;
	}
	axis2_wsdl_service_t *wsdl_srv_l
		= (axis2_wsdl_service_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_service_t));
	if(NULL == wsdl_srv_l)
	{
		axis2_free(env->allocator, wsdl_impl_srv);
		return AXIS2_ERROR_NO_MEMORY;
	}
	wsdl_srv_l->ops 
		= (axis2_wsdl_service_ops_t *)
		axis2_malloc (env->allocator, sizeof(axis2_wsdl_service_ops_t));
	if(NULL == wsdl_srv_l->ops)
	{
		axis2_free(env->allocator, wsdl_impl_srv);
		axis2_free(env->allocator, wsdl_srv_l);
		return AXIS2_ERROR_NO_MEMORY;	
	}
	(wsdl_srv_l->ops)->free = axis2_wsdl_service_free;
	wsdl_impl_srv->wsdl_srv = *wsdl_srv_l;
	*(wsdl_srv) = &(wsdl_impl_srv->wsdl_srv);
	
	return AXIS2_SUCCESS;	
}

/******************************************************************************/

axis2_status_t axis2_wsdl_service_free(axis2_env_t *env
		, axis2_wsdl_service_t *wsdl_srv)
{
	if(!env || !wsdl_srv)
		return AXIS2_ERROR_INVALID_NULL_PARAM;
	axis2_free(env->allocator, wsdl_srv);
	return AXIS2_SUCCESS;
}
