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
 
#include <axis2_wsdl_soap_op.h>

/** 
 * @brief Wsdl soap op struct impl
 *	Wsdl soap element
 */ 
typedef struct axis2_wsdl_soap_op_impl
{
	axis2_wsdl_soap_op_t soap_op;
    
    axis2_char_t *soap_action;

    axis2_char_t *style;
    
} axis2_wsdl_soap_op_impl_t;

#define AXIS2_INTF_TO_IMPL(soap_op) \
		((axis2_wsdl_soap_op_impl_t *)soap_op)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_soap_op_free (
                axis2_wsdl_soap_op_t *soap_op,
				axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_soap_op_get_style(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env);
    
axis2_status_t AXIS2_CALL
axis2_wsdl_soap_op_set_style(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env,
                                    axis2_char_t *style);

axis2_char_t * AXIS2_CALL
axis2_wsdl_soap_op_get_soap_action(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_soap_op_set_soap_action(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env,
                                    axis2_char_t *soap_action);

                        
/************************** End of function prototypes ************************/

axis2_wsdl_soap_op_t * AXIS2_CALL 
axis2_wsdl_soap_op_create (axis2_env_t **env)
{
    axis2_wsdl_soap_op_impl_t *soap_op_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	soap_op_impl = (axis2_wsdl_soap_op_impl_t *) 
        AXIS2_MALLOC((*env)->allocator, sizeof(axis2_wsdl_soap_op_impl_t));
	
	if(NULL == soap_op_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    soap_op_impl->style = NULL;
    soap_op_impl->soap_action = NULL;
    soap_op_impl->soap_op.ops = NULL;
    axis2_qname_t *type_l = NULL;
    
    type_l = axis2_qname_create(env, "op", SOAP_11_OP, NULL);
    
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(soap_op_impl->soap_op.
        extensible_element, env, type_l);
    
	soap_op_impl->soap_op.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_soap_op_ops_t));
	if(NULL == soap_op_impl->soap_op.ops)
    {
        axis2_wsdl_soap_op_free(&(soap_op_impl->
            soap_op), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    soap_op_impl->soap_op.ops->free =  
        axis2_wsdl_soap_op_free;
    
	soap_op_impl->soap_op.ops->get_style =  
        axis2_wsdl_soap_op_get_style;
	soap_op_impl->soap_op.ops->set_style =  
        axis2_wsdl_soap_op_set_style;
	soap_op_impl->soap_op.ops->get_soap_action =  
        axis2_wsdl_soap_op_get_soap_action;
	soap_op_impl->soap_op.ops->set_soap_action = 
        axis2_wsdl_soap_op_set_soap_action;
    
	return &(soap_op_impl->soap_op);
}

axis2_wsdl_soap_op_t * AXIS2_CALL 
axis2_wsdl_soap_op_create_with_type (axis2_env_t **env, axis2_qname_t *type)
{
    axis2_wsdl_soap_op_t *soap_op = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, type, NULL);
    
    soap_op = axis2_wsdl_soap_op_create(env);
    
    if(AXIS2_FAILURE == AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(soap_op->
        extensible_element, env, type))
    {
        axis2_wsdl_soap_op_free(soap_op, env);
        soap_op = NULL;
    }
    return soap_op;
    
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_soap_op_free (
                        axis2_wsdl_soap_op_t *soap_op, 
                        axis2_env_t **env)
{
    axis2_wsdl_soap_op_impl_t *soap_op_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(soap_op, env, AXIS2_FAILURE);
    
    soap_op_impl = AXIS2_INTF_TO_IMPL(soap_op);
    
	if(NULL != soap_op->ops)
    {
        AXIS2_FREE((*env)->allocator, soap_op->ops);
        soap_op->ops = NULL;
    }
    
    if(NULL != soap_op_impl->style)
    {
        AXIS2_FREE((*env)->allocator, soap_op_impl->style);
        soap_op_impl->style = NULL;
    }
    
    if(NULL != soap_op_impl->soap_action)
    {
        AXIS2_FREE((*env)->allocator, soap_op_impl->soap_action);
        soap_op_impl->soap_action = NULL;
    }
    
    if(soap_op_impl)
        AXIS2_FREE((*env)->allocator, soap_op_impl);
    soap_op_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_soap_op_get_style(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(soap_op, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(soap_op)->style;
}
    
axis2_status_t AXIS2_CALL
axis2_wsdl_soap_op_set_style(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env,
                                    axis2_char_t *style) 
{
    axis2_wsdl_soap_op_impl_t *soap_op_impl = AXIS2_INTF_TO_IMPL(
        soap_op);
    
    AXIS2_FUNC_PARAM_CHECK(soap_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    if(soap_op_impl->style)
    {
        AXIS2_FREE((*env)->allocator, style);
        soap_op_impl->style = NULL;
    }
    
    soap_op_impl->style = style;
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_soap_op_get_soap_action(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(soap_op, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(soap_op)->soap_action;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_soap_op_set_soap_action(axis2_wsdl_soap_op_t *soap_op,
                                    axis2_env_t **env,
                                    axis2_char_t *soap_action) 
{   
    axis2_wsdl_soap_op_impl_t *soap_op_impl = AXIS2_INTF_TO_IMPL(
        soap_op);
    
    AXIS2_FUNC_PARAM_CHECK(soap_op, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_action, AXIS2_FAILURE);
    
    if(soap_op_impl->soap_action)
    {
        AXIS2_FREE((*env)->allocator, soap_action);
        soap_op_impl->soap_action = NULL;
    }
    
    soap_op_impl->soap_action = soap_action;
    return AXIS2_SUCCESS;
}
