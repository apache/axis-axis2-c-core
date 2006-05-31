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
typedef struct axis2_wsdl_feature_impl
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
									const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_feature_get_name(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_name(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env,
                                axis2_char_t *name);

axis2_bool_t AXIS2_CALL
axis2_wsdl_feature_is_required(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_required(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env,
                                axis2_bool_t required);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_feature_t * AXIS2_CALL 
axis2_wsdl_feature_create (const axis2_env_t *env)
{
    axis2_wsdl_feature_impl_t *wsdl_feature_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	wsdl_feature_impl = (axis2_wsdl_feature_impl_t *) AXIS2_MALLOC(env->allocator,
			sizeof(axis2_wsdl_feature_impl_t));
	
	
	if(NULL == wsdl_feature_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    wsdl_feature_impl->name = NULL;
    wsdl_feature_impl->required = AXIS2_FALSE;
    wsdl_feature_impl->wsdl_feature.ops = NULL;
    wsdl_feature_impl->wsdl_feature.wsdl_component = NULL;
	
    wsdl_feature_impl->wsdl_feature.wsdl_component = axis2_wsdl_component_create(env);
    if(NULL == wsdl_feature_impl->wsdl_feature.wsdl_component)
    {
        axis2_wsdl_feature_free(&(wsdl_feature_impl->wsdl_feature), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }    
    
	wsdl_feature_impl->wsdl_feature.ops = 
		AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_feature_ops_t));
	if(NULL == wsdl_feature_impl->wsdl_feature.ops)
    {
        axis2_wsdl_feature_free(&(wsdl_feature_impl->wsdl_feature), env);
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
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
	
	return &(wsdl_feature_impl->wsdl_feature);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_feature_free (axis2_wsdl_feature_t *wsdl_feature, 
                            const axis2_env_t *env)
{
    axis2_wsdl_feature_impl_t *feature_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    feature_impl = AXIS2_INTF_TO_IMPL(wsdl_feature);
    
	if(NULL != wsdl_feature->ops)
        AXIS2_FREE(env->allocator, wsdl_feature->ops);
    
    if(NULL != feature_impl->name)
    {
        AXIS2_FREE(env->allocator, feature_impl->name);
        feature_impl->name = NULL;
    }
    
    if(NULL != wsdl_feature->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_feature->wsdl_component, env);
        wsdl_feature->wsdl_component = NULL;
    }
    
    AXIS2_FREE(env->allocator, feature_impl);
    feature_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_feature_get_name(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_feature)->name;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_name(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env,
                                axis2_char_t *name) 
{
    axis2_wsdl_feature_impl_t *feature_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    
    feature_impl = AXIS2_INTF_TO_IMPL(wsdl_feature);
    
    if(feature_impl->name)
    {
        AXIS2_FREE(env->allocator, feature_impl->name);
        feature_impl->name = NULL;
    }
    feature_impl->name = AXIS2_STRDUP(name, env);
    if(!feature_impl->name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_wsdl_feature_is_required(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(wsdl_feature)->required;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_feature_set_required(axis2_wsdl_feature_t *wsdl_feature,
                                const axis2_env_t *env,
                                axis2_bool_t required) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(wsdl_feature)->required = required;
    return AXIS2_SUCCESS;
}
