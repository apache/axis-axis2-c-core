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
 
#include <axis2_wsdl_soap_operation.h>

/** 
 * @brief Wsdl soap operation struct impl
 *	Wsdl soap element
 */ 
typedef struct axis2_wsdl_soap_operation_impl
{
	axis2_wsdl_soap_operation_t soap_operation;
    
    axis2_char_t *soap_action;

    axis2_char_t *style;
    
} axis2_wsdl_soap_operation_impl_t;

#define AXIS2_INTF_TO_IMPL(soap_operation) \
		((axis2_wsdl_soap_operation_impl_t *)soap_operation)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_soap_operation_free (
                axis2_wsdl_soap_operation_t *soap_operation,
				axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_soap_operation_get_style(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env);
    
axis2_status_t AXIS2_CALL
axis2_wsdl_soap_operation_set_style(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env,
                                    axis2_char_t *style);

axis2_char_t * AXIS2_CALL
axis2_wsdl_soap_operation_get_soap_action(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_soap_operation_set_soap_action(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env,
                                    axis2_char_t *soap_action);

                        
/************************** End of function prototypes ************************/

axis2_wsdl_soap_operation_t * AXIS2_CALL 
axis2_wsdl_soap_operation_create (axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, NULL);
	
	axis2_wsdl_soap_operation_impl_t *soap_operation_impl = 
		(axis2_wsdl_soap_operation_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_wsdl_soap_operation_impl_t));
	
	
	if(NULL == soap_operation_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
    
	soap_operation_impl->soap_operation.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_soap_operation_ops_t));
	if(NULL == soap_operation_impl->soap_operation.ops)
    {
        axis2_wsdl_soap_operation_free(&(soap_operation_impl->
            soap_operation), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    soap_operation_impl->soap_operation.ops->free =  
        axis2_wsdl_soap_operation_free;
    
	soap_operation_impl->soap_operation.ops->get_style =  
        axis2_wsdl_soap_operation_get_style;
	soap_operation_impl->soap_operation.ops->set_style =  
        axis2_wsdl_soap_operation_set_style;
	soap_operation_impl->soap_operation.ops->get_soap_action =  
        axis2_wsdl_soap_operation_get_soap_action;
	soap_operation_impl->soap_operation.ops->set_soap_action = 
        axis2_wsdl_soap_operation_set_soap_action;
	
    soap_operation_impl->style = NULL;
    soap_operation_impl->soap_action = NULL;
    
    axis2_qname_t *type_l = axis2_qname_create(env, "operation", 
        SOAP_11_OPERATION, NULL);
    
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(soap_operation_impl->soap_operation.
        extensible_element, env, type_l);
    
	return &(soap_operation_impl->soap_operation);
}

axis2_wsdl_soap_operation_t * AXIS2_CALL 
axis2_wsdl_soap_operation_create_with_type (axis2_env_t **env, axis2_qname_t *type)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, type, NULL);
    axis2_wsdl_soap_operation_t *soap_operation =
        axis2_wsdl_soap_operation_create(env);
    
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(soap_operation->extensible_element, env, type);
    
    return soap_operation;
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_soap_operation_free (
                        axis2_wsdl_soap_operation_t *soap_operation, 
                        axis2_env_t **env)
{
    axis2_wsdl_soap_operation_impl_t *soap_operation_impl = 
        AXIS2_INTF_TO_IMPL(soap_operation);
    AXIS2_FUNC_PARAM_CHECK(soap_operation, env, AXIS2_FAILURE);
	if(NULL != soap_operation->ops)
        AXIS2_FREE((*env)->allocator, soap_operation->ops);
    
    if(NULL != soap_operation_impl->style)
    {
        AXIS2_FREE((*env)->allocator, soap_operation_impl->style);
    }
    
    if(NULL != soap_operation_impl->soap_action)
    {
        AXIS2_FREE((*env)->allocator, soap_operation_impl->soap_action);
    }
    
    AXIS2_FREE((*env)->allocator, soap_operation_impl);
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_soap_operation_get_style(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(soap_operation, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(soap_operation)->style;
}
    
axis2_status_t AXIS2_CALL
axis2_wsdl_soap_operation_set_style(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env,
                                    axis2_char_t *style) 
{
    axis2_wsdl_soap_operation_impl_t *soap_operation_impl = AXIS2_INTF_TO_IMPL(
        soap_operation);
    
    AXIS2_FUNC_PARAM_CHECK(soap_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    if(soap_operation_impl->style)
        AXIS2_FREE((*env)->allocator, style);
    
    soap_operation_impl->style = style;
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_soap_operation_get_soap_action(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(soap_operation, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(soap_operation)->soap_action;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_soap_operation_set_soap_action(axis2_wsdl_soap_operation_t *soap_operation,
                                    axis2_env_t **env,
                                    axis2_char_t *soap_action) 
{   
    axis2_wsdl_soap_operation_impl_t *soap_operation_impl = AXIS2_INTF_TO_IMPL(
        soap_operation);
    
    AXIS2_FUNC_PARAM_CHECK(soap_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_action, AXIS2_FAILURE);
    
    if(soap_operation_impl->soap_action)
        AXIS2_FREE((*env)->allocator, soap_action);
    
    soap_operation_impl->soap_action = soap_action;
    return AXIS2_SUCCESS;
}
