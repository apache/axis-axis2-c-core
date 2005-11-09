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
typedef struct axis2_wsdl_svc_impl_s axis2_wsdl_svc_impl_t;  
  
struct axis2_wsdl_svc_impl_s
{
	axis2_wsdl_svc_t wsdl_svc;
    axis2_hash_t * component_properties;
    axis2_qname_t *qname;
	
};

#define AXIS2_INTF_TO_IMPL(wsdl_svc) ((axis2_wsdl_svc_impl_t *)(wsdl_svc))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_svc_free(axis2_wsdl_svc_t *wsdl_svc,
                    axis2_env_t **env);

axis2_qname_t * AXIS2_CALL 
axis2_svc_get_name(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_svc_set_name(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env, 
                    axis2_qname_t *qname);

axis2_hash_t * AXIS2_CALL
axis2_wsdl_svc_get_component_properties(axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_set_component_properties(axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env,
                                        axis2_hash_t *properties);

axis2_wsdl_component_t * AXIS2_CALL
axis2_wsdl_svc_get_component_property(axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env,
                                        const axis2_char_t *key);

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_set_component_property (axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env,
                                        const void *key,
                                        void *value);

/***************************** End of function headers ************************/

axis2_wsdl_svc_t * AXIS2_CALL 
axis2_wsdl_svc_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
	axis2_wsdl_svc_impl_t *wsdl_svc_impl = (axis2_wsdl_svc_impl_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_svc_t));
	if(NULL == wsdl_svc_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    wsdl_svc_impl->component_properties = axis2_hash_make(env);
    if(NULL == wsdl_svc_impl->component_properties)
	{
        AXIS2_FREE((*env)->allocator, wsdl_svc_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
	wsdl_svc_impl->wsdl_svc.ops = 
        (axis2_wsdl_svc_ops_t *) AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_wsdl_svc_ops_t));
    
	if(NULL == wsdl_svc_impl->wsdl_svc.ops)
	{
        axis2_hash_free(wsdl_svc_impl->component_properties, env);
		AXIS2_FREE((*env)->allocator, wsdl_svc_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
	wsdl_svc_impl->wsdl_svc.ops->free = axis2_wsdl_svc_free;
    wsdl_svc_impl->wsdl_svc.ops->get_name = axis2_svc_get_name;
    wsdl_svc_impl->wsdl_svc.ops->set_name = axis2_svc_set_name;
	
	return &(wsdl_svc_impl->wsdl_svc);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_free (axis2_wsdl_svc_t *wsdl_svc,
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(wsdl_svc, env, AXIS2_FAILURE);
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_svc)->component_properties)
        axis2_hash_free(AXIS2_INTF_TO_IMPL(wsdl_svc)->component_properties, env);
    
    if(NULL != wsdl_svc->ops)
        AXIS2_FREE((*env)->allocator, wsdl_svc->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_svc));
    
	return AXIS2_SUCCESS;
}

axis2_qname_t * AXIS2_CALL 
axis2_svc_get_name(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_svc, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(wsdl_svc)->qname; 
}

axis2_status_t AXIS2_CALL
axis2_svc_set_name(axis2_wsdl_svc_t *wsdl_svc, 
                    axis2_env_t **env, 
                    axis2_qname_t *qname) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_svc)->qname = qname;
    
    return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_wsdl_svc_get_component_properties(axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_svc, env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_svc)->component_properties;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_set_component_properties(axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env,
                                        axis2_hash_t *properties)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_svc, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_svc)->component_properties = properties;
    return AXIS2_SUCCESS;
}

axis2_wsdl_component_t * AXIS2_CALL
axis2_wsdl_svc_get_component_property(axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env,
                                        const axis2_char_t *key)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_svc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, key, NULL);
    return axis2_hash_get(AXIS2_INTF_TO_IMPL(wsdl_svc)->component_properties,
        (axis2_char_t *) key, AXIS2_HASH_KEY_STRING); 
}

axis2_status_t AXIS2_CALL
axis2_wsdl_svc_set_component_property (axis2_wsdl_svc_t *wsdl_svc,
                                        axis2_env_t **env,
                                        const void *key,
                                        void *value)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_svc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    axis2_hash_set (AXIS2_INTF_TO_IMPL(wsdl_svc)->component_properties,
        (axis2_char_t *) key, AXIS2_HASH_KEY_STRING, value);
    
    return AXIS2_SUCCESS;
}
