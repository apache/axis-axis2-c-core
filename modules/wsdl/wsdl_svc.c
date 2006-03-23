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
 
#include <axis2_wsdl_svc.h>

/** 
 * @brief Wsdl Service struct impl
 *	Axis2 Wsdl Service impl  
 */
typedef struct axis2_wsdl_svc_impl axis2_wsdl_svc_impl_t;  
  
struct axis2_wsdl_svc_impl
{
	axis2_wsdl_svc_t wsdl_svc;
    
    /**
     * The QName that identifies the Service. This namespace of the QName
     * should be the target namespace defined in the Definitions component.
     */
    axis2_qname_t *qname;
	/**
     * The Interface that this Service is an instance of.
     */
    axis2_wsdl_interface_t *svc_interface;
    /**
     *
     */
    axis2_hash_t *endpoints;
};

#define AXIS2_INTF_TO_IMPL(wsdl_svc) ((axis2_wsdl_svc_impl_t *)(wsdl_svc))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_free(axis2_wsdl_svc_t *wsdl_svc,
                    axis2_env_t **env);

axis2_qname_t * AXIS2_CALL 
axis2_wsdl_svc_get_qname(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_set_qname(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env, 
                    axis2_qname_t *qname);

axis2_hash_t *AXIS2_CALL
axis2_wsdl_svc_get_endpoints(axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_set_endpoints(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env,
                                axis2_hash_t *endpoints);

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_set_endpoint(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env,
                                axis2_wsdl_endpoint_t *endpoint);

axis2_wsdl_endpoint_t * AXIS2_CALL
axis2_wsdl_svc_get_endpoint(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env,
                                axis2_qname_t *qname);

axis2_char_t *AXIS2_CALL
axis2_wsdl_svc_get_namespace(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env);

axis2_wsdl_interface_t * AXIS2_CALL
axis2_wsdl_svc_get_svc_interface(axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_set_svc_interface(axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env,
                                    axis2_wsdl_interface_t *svc_interface);

/***************************** End of function headers ************************/

axis2_wsdl_svc_t *AXIS2_CALL 
axis2_wsdl_svc_create (axis2_env_t **env)
{
    axis2_wsdl_svc_impl_t *wsdl_svc_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
	wsdl_svc_impl = (axis2_wsdl_svc_impl_t *) AXIS2_MALLOC ((*env)->allocator, 
            sizeof(axis2_wsdl_svc_impl_t));
	if(NULL == wsdl_svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    wsdl_svc_impl->svc_interface = NULL;
    wsdl_svc_impl->wsdl_svc.wsdl_component = NULL;
    wsdl_svc_impl->endpoints = NULL;
    wsdl_svc_impl->wsdl_svc.ops = NULL;
    wsdl_svc_impl->qname = NULL;
    
    wsdl_svc_impl->wsdl_svc.wsdl_component = axis2_wsdl_component_create(env);
    if(NULL == wsdl_svc_impl->wsdl_svc.wsdl_component)
	{
        axis2_wsdl_svc_free(&(wsdl_svc_impl->wsdl_svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY,AXIS2_FAILURE);
        return NULL;
    }
    
    wsdl_svc_impl->endpoints = axis2_hash_make(env);
    if(NULL == wsdl_svc_impl->endpoints)
    {
        axis2_wsdl_svc_free(&(wsdl_svc_impl->wsdl_svc), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	wsdl_svc_impl->wsdl_svc.ops = (axis2_wsdl_svc_ops_t *) AXIS2_MALLOC ((*env)->
        allocator, sizeof(axis2_wsdl_svc_ops_t));
    
	if(NULL == wsdl_svc_impl->wsdl_svc.ops)
	{
        axis2_wsdl_svc_free(&(wsdl_svc_impl->wsdl_svc), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
	}
    
	wsdl_svc_impl->wsdl_svc.ops->free = axis2_wsdl_svc_free;
    wsdl_svc_impl->wsdl_svc.ops->get_qname = axis2_wsdl_svc_get_qname;
    wsdl_svc_impl->wsdl_svc.ops->set_qname = axis2_wsdl_svc_set_qname;
    wsdl_svc_impl->wsdl_svc.ops->get_endpoints = axis2_wsdl_svc_get_endpoints; 
    wsdl_svc_impl->wsdl_svc.ops->set_endpoints = axis2_wsdl_svc_set_endpoints;
    wsdl_svc_impl->wsdl_svc.ops->set_endpoint = axis2_wsdl_svc_set_endpoint;
    wsdl_svc_impl->wsdl_svc.ops->get_endpoint = axis2_wsdl_svc_get_endpoint;
    wsdl_svc_impl->wsdl_svc.ops->get_namespace = axis2_wsdl_svc_get_namespace;
    wsdl_svc_impl->wsdl_svc.ops->get_svc_interface = axis2_wsdl_svc_get_svc_interface;
    wsdl_svc_impl->wsdl_svc.ops->set_svc_interface = axis2_wsdl_svc_set_svc_interface;
    
	return &(wsdl_svc_impl->wsdl_svc);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_free (axis2_wsdl_svc_t *wsdl_svc,
                        axis2_env_t **env)
{
    axis2_wsdl_svc_impl_t *wsdl_svc_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    wsdl_svc_impl = AXIS2_INTF_TO_IMPL(wsdl_svc);
    
    if(wsdl_svc->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_svc->wsdl_component, env);
        wsdl_svc->wsdl_component = NULL;
    }
    
    if(wsdl_svc_impl->endpoints)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (wsdl_svc_impl->endpoints, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_wsdl_endpoint_t *endpoint = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            endpoint = (axis2_wsdl_endpoint_t *) val;
            if (endpoint)
                AXIS2_WSDL_ENDPOINT_FREE (endpoint, env);
            
            val = NULL;
            endpoint = NULL;
               
        }
        axis2_hash_free(wsdl_svc_impl->endpoints, env);
        wsdl_svc_impl->endpoints = NULL;
    }
    
    if(wsdl_svc_impl->svc_interface)
    {
        AXIS2_WSDL_INTERFACE_FREE(wsdl_svc_impl->svc_interface, env);
        wsdl_svc_impl->svc_interface = NULL;
    }

    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_svc)->qname)
    {
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(wsdl_svc)->qname, env);
        AXIS2_INTF_TO_IMPL(wsdl_svc)->qname = NULL;
    }
    
    if(wsdl_svc->ops)
    {
        AXIS2_FREE((*env)->allocator, wsdl_svc->ops);
        wsdl_svc->ops = NULL;
    }
    
    if(wsdl_svc_impl)
	    AXIS2_FREE((*env)->allocator, wsdl_svc_impl);
    wsdl_svc_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_qname_t * AXIS2_CALL 
axis2_wsdl_svc_get_qname(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_svc)->qname; 
}

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_set_qname(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env, 
                    axis2_qname_t *qname) 
{
    axis2_qname_t *svc_name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
    svc_name = AXIS2_QNAME_CLONE(qname, env);
    if(NULL == svc_name)
    {
        return AXIS2_FAILURE;
    }
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_svc)->qname)
    {
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(wsdl_svc)->qname, env);
        AXIS2_INTF_TO_IMPL(wsdl_svc)->qname = NULL;
    }
    
    AXIS2_INTF_TO_IMPL(wsdl_svc)->qname = svc_name;
    
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_wsdl_svc_get_endpoints(axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_svc)->endpoints;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_set_endpoints(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env,
                                axis2_hash_t *endpoints) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, endpoints, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_svc)->endpoints = endpoints;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_set_endpoint(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env,
                                axis2_wsdl_endpoint_t *endpoint) 
{
    axis2_wsdl_svc_impl_t *wsdl_svc_impl = NULL;
    axis2_qname_t *endpoint_qname = NULL;
    axis2_char_t *endpoint_name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, endpoint, AXIS2_FAILURE);
    wsdl_svc_impl = AXIS2_INTF_TO_IMPL(wsdl_svc);
   
    endpoint_qname = AXIS2_WSDL_ENDPOINT_GET_NAME(endpoint, env);
    endpoint_name = AXIS2_QNAME_TO_STRING(endpoint_qname, env);
    axis2_hash_set(wsdl_svc_impl->endpoints, endpoint_name , 
        AXIS2_HASH_KEY_STRING, endpoint);
    
    return AXIS2_SUCCESS;
}

axis2_wsdl_endpoint_t * AXIS2_CALL
axis2_wsdl_svc_get_endpoint(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env,
                                axis2_qname_t *qname) 
{
    axis2_wsdl_svc_impl_t *wsdl_svc_impl = NULL;
    axis2_char_t *name = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
    wsdl_svc_impl = AXIS2_INTF_TO_IMPL(wsdl_svc);

    name = AXIS2_QNAME_TO_STRING(qname, env);
    return (axis2_wsdl_endpoint_t *) axis2_hash_get(wsdl_svc_impl->endpoints, 
        name, AXIS2_HASH_KEY_STRING);
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_svc_get_namespace(axis2_wsdl_svc_t *wsdl_svc,
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    if (NULL == AXIS2_INTF_TO_IMPL(wsdl_svc)->qname) 
    {
        /* Target Namespace not set and the Service Name is null */
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_WSDL_SVC, 
            AXIS2_FAILURE);
        return NULL;
    }
    
    return AXIS2_QNAME_GET_URI(AXIS2_INTF_TO_IMPL(wsdl_svc)->qname, env);
}

axis2_wsdl_interface_t * AXIS2_CALL
axis2_wsdl_svc_get_svc_interface(axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_svc)->svc_interface;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_set_svc_interface(axis2_wsdl_svc_t *wsdl_svc,
                                    axis2_env_t **env,
                                    axis2_wsdl_interface_t *svc_interface) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc_interface, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_svc)->svc_interface = svc_interface;
    return AXIS2_SUCCESS;
}
