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
 
#include <axis2_wsdl_feature.h>

/** 
 * @brief Wsdl feature struct impl
 *	Wsdl features  
 */ 
typedef struct axis2_wsdl_feature_impl_s
{
	axis2_wsdl_feature_t wsdl_feature;
    axis2_char_t *name;
    axis2_bool_t required;	
} axis2_wsdl_feature_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_feature) \
		((axis2_wsdl_feature_impl_t *)wsdl_feature)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_feature_free (axis2_wsdl_feature_t *wsdl_feature,
									axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_feature_get_name(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_name(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env,
                                axis2_char_t *name);

axis2_bool_t AXIS2_CALL
axis2_wsdl_feature_is_required(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_required(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env,
                                axis2_bool_t required);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_feature_t * AXIS2_CALL 
axis2_wsdl_feature_create (axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, NULL);
	
	axis2_wsdl_feature_impl_t *wsdl_feature_impl = 
		(axis2_wsdl_feature_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_wsdl_feature_impl_t));
	
	
	if(NULL == wsdl_feature_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
	
	wsdl_feature_impl->wsdl_feature.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_feature_ops_t));
	if(NULL == wsdl_feature_impl->wsdl_feature.ops)
    {
        AXIS2_FREE((*env)->allocator, wsdl_feature_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
	wsdl_feature_impl->wsdl_feature.ops->free =  axis2_wsdl_feature_free;
	wsdl_feature_impl->wsdl_feature.ops->get_name = 
        axis2_wsdl_feature_get_name;
    wsdl_feature_impl->wsdl_feature.ops->set_name = 
        axis2_wsdl_feature_set_name;
	wsdl_feature_impl->wsdl_feature.ops->is_required = 
        axis2_wsdl_feature_is_required;
    wsdl_feature_impl->wsdl_feature.ops->set_required = 
        axis2_wsdl_feature_set_required;
	
	wsdl_feature_impl->name = NULL;
    wsdl_feature_impl->required = AXIS2_FALSE;
	
	return &(wsdl_feature_impl->wsdl_feature);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_feature_free (axis2_wsdl_feature_t *wsdl_feature, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_feature, env, AXIS2_FAILURE);
	if(NULL != wsdl_feature->ops)
        AXIS2_FREE((*env)->allocator, wsdl_feature->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(wsdl_feature)->name)
    {
        AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_feature)->name);
    }
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(wsdl_feature));
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_feature_get_name(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_feature, env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_feature)->name;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_name(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env,
                                axis2_char_t *name) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_feature, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, name, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_feature)->name = name;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_wsdl_feature_is_required(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_feature, env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(wsdl_feature)->required;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_required(axis2_wsdl_feature_t *wsdl_feature,
                                axis2_env_t **env,
                                axis2_bool_t required) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_feature, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_feature)->required = required;
    return AXIS2_SUCCESS;
}
