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
 
#include <axis2_wsdl_endpoint.h>

/** 
 * @brief Wsdl endpoint struct impl
 *	Wsdl endpoints  
 */ 
typedef struct axis2_wsdl_endpoint_impl
{
	axis2_wsdl_endpoint_t wsdl_endpoint;
    
    /**
     * Field name
     */
    axis2_qname_t *qname;

    /**
     * Field binding
     */
    struct axis2_wsdl_binding *wsdl_binding;	
    
} axis2_wsdl_endpoint_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_endpoint) \
		((axis2_wsdl_endpoint_impl_t *)wsdl_endpoint)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_endpoint_free (axis2_wsdl_endpoint_t *wsdl_endpoint,
									axis2_env_t **env);

axis2_qname_t * AXIS2_CALL
axis2_wsdl_endpoint_get_name(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_endpoint_set_name(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env,
                                axis2_qname_t *qname);

struct axis2_wsdl_binding * AXIS2_CALL
axis2_wsdl_endpoint_get_binding(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_endpoint_set_binding(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env,
                                struct axis2_wsdl_binding *wsdl_binding);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_endpoint_t * AXIS2_CALL 
axis2_wsdl_endpoint_create (axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, NULL);
	
	axis2_wsdl_endpoint_impl_t *wsdl_endpoint_impl = 
		(axis2_wsdl_endpoint_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_wsdl_endpoint_impl_t));
	
	
	if(NULL == wsdl_endpoint_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
	
    wsdl_endpoint_impl->wsdl_endpoint.wsdl_component = axis2_wsdl_component_create(env);
    if(NULL == wsdl_endpoint_impl->wsdl_endpoint.wsdl_component)
    {
        AXIS2_FREE((*env)->allocator, wsdl_endpoint_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }    
    
	wsdl_endpoint_impl->wsdl_endpoint.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_endpoint_ops_t));
	if(NULL == wsdl_endpoint_impl->wsdl_endpoint.ops)
    {
        AXIS2_WSDL_FEATURE_FREE(wsdl_endpoint_impl->wsdl_endpoint.wsdl_component, env);
        AXIS2_FREE((*env)->allocator, wsdl_endpoint_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
	wsdl_endpoint_impl->wsdl_endpoint.ops->free =  axis2_wsdl_endpoint_free;
	wsdl_endpoint_impl->wsdl_endpoint.ops->get_name = 
        axis2_wsdl_endpoint_get_name;
    wsdl_endpoint_impl->wsdl_endpoint.ops->set_name = 
        axis2_wsdl_endpoint_set_name;
	wsdl_endpoint_impl->wsdl_endpoint.ops->get_binding = 
        axis2_wsdl_endpoint_get_binding;
    wsdl_endpoint_impl->wsdl_endpoint.ops->set_binding = 
        axis2_wsdl_endpoint_set_binding;
	
	wsdl_endpoint_impl->qname = NULL;
    wsdl_endpoint_impl->wsdl_binding = NULL;
	
	return &(wsdl_endpoint_impl->wsdl_endpoint);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_endpoint_free (axis2_wsdl_endpoint_t *wsdl_endpoint, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_endpoint, env, AXIS2_FAILURE);
	if(NULL != wsdl_endpoint->ops)
        AXIS2_FREE((*env)->allocator, wsdl_endpoint->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_endpoint)->qname)
    {
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_endpoint)->qname);
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_endpoint)->wsdl_binding)
    {
        AXIS2_WSDL_BINDING_FREE(AXIS2_INTF_TO_IMPL(wsdl_endpoint)->wsdl_binding, env);
    }
    
    if(NULL != wsdl_endpoint->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_endpoint->wsdl_component, env);
    }
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_endpoint));
    
	return AXIS2_SUCCESS;
}

/**
 * Method getName
 *
 * @return
 */
axis2_qname_t *AXIS2_CALL
axis2_wsdl_endpoint_get_name(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_endpoint, env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_endpoint)->qname;
}

/**
 * Method setName
 *
 * @param name
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_endpoint_set_name(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env,
                                axis2_qname_t *qname) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_endpoint, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_endpoint)->qname = qname;
    return AXIS2_SUCCESS;
}

/**
 * Method getBinding
 *
 * @return
 */
struct axis2_wsdl_binding * AXIS2_CALL
axis2_wsdl_endpoint_get_binding(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_endpoint, env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(wsdl_endpoint)->wsdl_binding;
}

/**
 * Method setBinding
 *
 * @param binding
 */
axis2_status_t AXIS2_CALL
axis2_wsdl_endpoint_set_binding(axis2_wsdl_endpoint_t *wsdl_endpoint,
                                axis2_env_t **env,
                                struct axis2_wsdl_binding *wsdl_binding) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_endpoint, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_endpoint)->wsdl_binding = wsdl_binding;
    return AXIS2_SUCCESS;
}
