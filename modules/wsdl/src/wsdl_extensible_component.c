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
 
#include <axis2_wsdl_extensible_component.h>

/** 
 * @brief Wsdl extensible component struct impl
 *	Wsdl extensible component
 */ 
typedef struct axis2_wsdl_extensible_component_impl_s
{
	axis2_wsdl_extensible_component_t extensible_component;
    /**
     * Field features
     */
    axis2_linked_list_t *features;

    /**
     * Field properties
     */
    axis2_linked_list_t *properties;
    
} axis2_wsdl_extensible_component_impl_t;

#define AXIS2_INTF_TO_IMPL(extensible_component) \
		((axis2_wsdl_extensible_component_impl_t *)extensible_component)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_extensible_component_free (
                axis2_wsdl_extensible_component_t *extensible_component,
				axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_feature(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env,
                        axis2_wsdl_feature_t *wsdl_feature);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_features(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env);
                        
axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_property(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env,
                        axis2_wsdl_property_t *wsdl_property);

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_properties(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env);
                        
/************************** End of function prototypes ************************/

axis2_wsdl_extensible_component_t * AXIS2_CALL 
axis2_wsdl_extensible_component_create (axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, NULL);
	
	axis2_wsdl_extensible_component_impl_t *extensible_component_impl = 
		(axis2_wsdl_extensible_component_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_wsdl_extensible_component_impl_t));
	
	
	if(NULL == extensible_component_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
	
	extensible_component_impl->extensible_component.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_extensible_component_ops_t));
	if(NULL == extensible_component_impl->extensible_component.ops)
    {
        AXIS2_FREE((*env)->allocator, extensible_component_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
	extensible_component_impl->extensible_component.ops->free =  
        axis2_wsdl_extensible_component_free;
	extensible_component_impl->extensible_component.ops->add_feature =  
        axis2_wsdl_extensible_component_add_feature;
	extensible_component_impl->extensible_component.ops->get_features =  
        axis2_wsdl_extensible_component_get_features;
	extensible_component_impl->extensible_component.ops->add_property = 
        axis2_wsdl_extensible_component_add_property;
    extensible_component_impl->extensible_component.ops->get_properties = 
        axis2_wsdl_extensible_component_get_properties;
	
	extensible_component_impl->features = axis2_linked_list_create(env);
    if(NULL == extensible_component_impl->features)
    {
        AXIS2_FREE((*env)->allocator, extensible_component_impl->extensible_component.ops);
        AXIS2_FREE((*env)->allocator, extensible_component_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    extensible_component_impl->properties = axis2_linked_list_create(env);
    if(NULL == extensible_component_impl->properties)
    {
        AXIS2_LINKED_LIST_FREE(extensible_component_impl->features, env);
        AXIS2_FREE((*env)->allocator, extensible_component_impl->
            extensible_component.ops);
        AXIS2_FREE((*env)->allocator, extensible_component_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
	
	return &(extensible_component_impl->extensible_component);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_extensible_component_free (
                        axis2_wsdl_extensible_component_t *extensible_component, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(extensible_component, env, AXIS2_FAILURE);
	if(NULL != extensible_component->ops)
        AXIS2_FREE((*env)->allocator, extensible_component->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(extensible_component)->features)
    {
        AXIS2_LINKED_LIST_FREE(AXIS2_INTF_TO_IMPL(extensible_component)->
            features, env);
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(extensible_component)->properties)
    {
        AXIS2_LINKED_LIST_FREE(AXIS2_INTF_TO_IMPL(extensible_component)->
            properties, env);
    }
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(extensible_component));
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_feature(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env,
                        axis2_wsdl_feature_t *wsdl_feature) 
{
    AXIS2_FUNC_PARAM_CHECK(extensible_component, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_feature, AXIS2_FAILURE);
    
    if (NULL == AXIS2_INTF_TO_IMPL(extensible_component)->features) 
    {
        AXIS2_INTF_TO_IMPL(extensible_component)->features = 
            axis2_linked_list_create(env);
    }
    
    return AXIS2_LINKED_LIST_ADD(AXIS2_INTF_TO_IMPL(extensible_component)->features,
        env, wsdl_feature);
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_features(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(extensible_component, env, NULL);
    
    if (NULL == AXIS2_INTF_TO_IMPL(extensible_component)->features) 
    {
        return axis2_linked_list_create(env);
    }
    return AXIS2_INTF_TO_IMPL(extensible_component)->features;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_component_add_property(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env,
                        axis2_wsdl_property_t *wsdl_property) 
{
    AXIS2_FUNC_PARAM_CHECK(extensible_component, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_property, AXIS2_FAILURE);
    if (NULL == AXIS2_INTF_TO_IMPL(extensible_component)->properties) 
    {
        AXIS2_INTF_TO_IMPL(extensible_component)->properties =
            axis2_linked_list_create(env);
    }
    
    return  AXIS2_LINKED_LIST_ADD(AXIS2_INTF_TO_IMPL(extensible_component)->
        features, env, wsdl_property);
}

axis2_linked_list_t *AXIS2_CALL
axis2_wsdl_extensible_component_get_properties(
                        axis2_wsdl_extensible_component_t *extensible_component,
                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(extensible_component, env, NULL);
    
    if (NULL == AXIS2_INTF_TO_IMPL(extensible_component)->properties) 
    {
        return axis2_linked_list_create(env);
    }
    return AXIS2_INTF_TO_IMPL(extensible_component)->properties;
}
