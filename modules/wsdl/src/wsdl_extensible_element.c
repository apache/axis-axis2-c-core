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
 
#include <axis2_wsdl_extensible_element.h>

/** 
 * @brief Wsdl extensible element struct impl
 *	Wsdl extensible element
 */ 
typedef struct axis2_wsdl_extensible_element_impl
{
	axis2_wsdl_extensible_element_t extensible_element;
    
    axis2_qname_t *type;

    axis2_bool_t required;
    
} axis2_wsdl_extensible_element_impl_t;

#define AXIS2_INTF_TO_IMPL(extensible_element) \
		((axis2_wsdl_extensible_element_impl_t *)extensible_element)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_extensible_element_free (
                axis2_wsdl_extensible_element_t *extensible_element,
				axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_wsdl_extensible_element_is_required(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_element_set_required(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env,
                            axis2_bool_t required);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_extensible_element_get_type(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_element_set_type(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env,
                            axis2_qname_t *type);
                        
/************************** End of function prototypes ************************/

axis2_wsdl_extensible_element_t * AXIS2_CALL 
axis2_wsdl_extensible_element_create (axis2_env_t **env)
{
    axis2_wsdl_extensible_element_impl_t *extensible_element_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	extensible_element_impl = (axis2_wsdl_extensible_element_impl_t *) 
        AXIS2_MALLOC((*env)->allocator, sizeof(axis2_wsdl_extensible_element_impl_t));
	
	
	if(NULL == extensible_element_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;        
    }
    
    extensible_element_impl->type = NULL;
    extensible_element_impl->extensible_element.ops = NULL;
    
	extensible_element_impl->extensible_element.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_extensible_element_ops_t));
	if(NULL == extensible_element_impl->extensible_element.ops)
    {
        axis2_wsdl_extensible_element_free(&(extensible_element_impl->
            extensible_element), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
    extensible_element_impl->extensible_element.ops->free =  
        axis2_wsdl_extensible_element_free;
    
	extensible_element_impl->extensible_element.ops->is_required =  
        axis2_wsdl_extensible_element_is_required;
	extensible_element_impl->extensible_element.ops->set_required =  
        axis2_wsdl_extensible_element_set_required;
	extensible_element_impl->extensible_element.ops->get_type =  
        axis2_wsdl_extensible_element_get_type;
	extensible_element_impl->extensible_element.ops->set_type = 
        axis2_wsdl_extensible_element_set_type;
	
    
	return &(extensible_element_impl->extensible_element);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_extensible_element_free (
                        axis2_wsdl_extensible_element_t *extensible_element, 
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(extensible_element, env, AXIS2_FAILURE);
	if(NULL != extensible_element->ops)
        AXIS2_FREE((*env)->allocator, extensible_element->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(extensible_element)->type)
    {
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(extensible_element)->
            type, env);
        AXIS2_INTF_TO_IMPL(extensible_element)->type = NULL;
        
    }
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(extensible_element));
    
	return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_wsdl_extensible_element_is_required(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(extensible_element, env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(extensible_element)->required;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_element_set_required(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env,
                            axis2_bool_t required) 
{
    AXIS2_FUNC_PARAM_CHECK(extensible_element, env, AXIS2_FALSE);
    
    AXIS2_INTF_TO_IMPL(extensible_element)->required = required;
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_extensible_element_get_type(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(extensible_element, env, NULL);
    return AXIS2_INTF_TO_IMPL(extensible_element)->type;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_element_set_type(
                            axis2_wsdl_extensible_element_t *extensible_element,
                            axis2_env_t **env,
                            axis2_qname_t *type) 
{
    axis2_wsdl_extensible_element_impl_t *extensible_element_impl = 
        AXIS2_INTF_TO_IMPL(extensible_element);
    
    AXIS2_FUNC_PARAM_CHECK(extensible_element, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, type, AXIS2_FAILURE);
    
    if(extensible_element_impl->type)
    {
        AXIS2_QNAME_FREE(extensible_element_impl->type, env);
        extensible_element_impl->type = NULL;
    }
    
    extensible_element_impl->type = type;
    return AXIS2_SUCCESS;
}
