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
 
#include <axis2_wsdl_ext_soap_body.h>
#include <axis2_wsdl.h>

/** 
 * @brief Wsdl Extension Soap Body struct impl
 *	 Wsdl Extention Soap Body  
 */ 
typedef struct axis2_wsdl_ext_soap_body_impl
{
	axis2_wsdl_ext_soap_body_t ext_soap_body;
    axis2_char_t *use;
    axis2_char_t *namespc_uri;
    
} axis2_wsdl_ext_soap_body_impl_t;

#define AXIS2_INTF_TO_IMPL(ext_soap_body) \
		((axis2_wsdl_ext_soap_body_impl_t *)ext_soap_body)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_ext_soap_body_free (axis2_wsdl_ext_soap_body_t *ext_soap_body,
									axis2_env_t **env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_body_get_use(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_body_set_use(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env,
                                axis2_char_t *use);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_body_get_namespc_uri(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_body_set_namespc_uri(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env,
                                axis2_char_t *namespc_uri);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_ext_soap_body_t * AXIS2_CALL 
axis2_wsdl_ext_soap_body_create (axis2_env_t **env,
                                 axis2_qname_t *qtype)
{
    axis2_wsdl_ext_soap_body_impl_t *ext_soap_body_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	ext_soap_body_impl = (axis2_wsdl_ext_soap_body_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_wsdl_ext_soap_body_impl_t));
	
	
	if(NULL == ext_soap_body_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    ext_soap_body_impl->use = NULL;
    ext_soap_body_impl->namespc_uri = NULL;
    ext_soap_body_impl->ext_soap_body.ops = NULL;
    ext_soap_body_impl->ext_soap_body.ext_element = NULL;
	
    ext_soap_body_impl->ext_soap_body.ext_element = 
        axis2_wsdl_extensible_element_create(env);
    if(NULL == ext_soap_body_impl->ext_soap_body.ext_element)
    {
        axis2_wsdl_ext_soap_body_free(&(ext_soap_body_impl->ext_soap_body), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }    
    
	ext_soap_body_impl->ext_soap_body.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_ext_soap_body_ops_t));
	if(NULL == ext_soap_body_impl->ext_soap_body.ops)
    {
        axis2_wsdl_ext_soap_body_free(&(ext_soap_body_impl->ext_soap_body), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if(!qtype)
    {
        qtype = axis2_qname_create(env, "body", AXIS2_SOAP_11_BODY, NULL);
        if(!qtype) return NULL;
    }    
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(
        (&(ext_soap_body_impl->ext_soap_body))->ext_element, env, qtype);

	ext_soap_body_impl->ext_soap_body.ops->free =  axis2_wsdl_ext_soap_body_free;
	ext_soap_body_impl->ext_soap_body.ops->get_use = 
        axis2_wsdl_ext_soap_body_get_use;
    ext_soap_body_impl->ext_soap_body.ops->set_use = 
        axis2_wsdl_ext_soap_body_set_use;
	ext_soap_body_impl->ext_soap_body.ops->get_namespc_uri = 
        axis2_wsdl_ext_soap_body_get_namespc_uri;
    ext_soap_body_impl->ext_soap_body.ops->set_namespc_uri = 
        axis2_wsdl_ext_soap_body_set_namespc_uri;
	
	return &(ext_soap_body_impl->ext_soap_body);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_ext_soap_body_free (axis2_wsdl_ext_soap_body_t *ext_soap_body, 
                            axis2_env_t **env)
{
    axis2_wsdl_ext_soap_body_impl_t *exp_soap_body_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    exp_soap_body_impl = AXIS2_INTF_TO_IMPL(ext_soap_body);
    
	if(NULL != ext_soap_body->ops)
        AXIS2_FREE((*env)->allocator, ext_soap_body->ops);
    
    if(NULL != exp_soap_body_impl->use)
    {
        AXIS2_FREE((*env)->allocator, exp_soap_body_impl->use);
        exp_soap_body_impl->use = NULL;
    }
    
    if(NULL != exp_soap_body_impl->namespc_uri)
    {
        AXIS2_FREE((*env)->allocator, exp_soap_body_impl->namespc_uri);
        exp_soap_body_impl->namespc_uri = NULL;
    }
    
    if(NULL != ext_soap_body->ext_element)
    {
        AXIS2_WSDL_EXTENSIBLE_ELEMENT_FREE(ext_soap_body->ext_element, env);
        ext_soap_body->ext_element = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, exp_soap_body_impl);
    exp_soap_body_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_body_get_use(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(ext_soap_body)->use;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_body_set_use(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env,
                                axis2_char_t *use) 
{
    axis2_wsdl_ext_soap_body_impl_t *exp_soap_body_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, use, AXIS2_FAILURE);
    
    exp_soap_body_impl = AXIS2_INTF_TO_IMPL(ext_soap_body);
    
    if(exp_soap_body_impl->use)
    {
        AXIS2_FREE((*env)->allocator, exp_soap_body_impl->use);
        exp_soap_body_impl->use = NULL;
    }
    exp_soap_body_impl->use = AXIS2_STRDUP(use, env);
    if(!exp_soap_body_impl->use)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_body_get_namespc_uri(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(ext_soap_body)->namespc_uri;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_body_set_namespc_uri(axis2_wsdl_ext_soap_body_t *ext_soap_body,
                                axis2_env_t **env,
                                axis2_char_t *namespc_uri) 
{
    axis2_wsdl_ext_soap_body_impl_t *exp_soap_body_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, namespc_uri, AXIS2_FAILURE);
    exp_soap_body_impl = AXIS2_INTF_TO_IMPL(ext_soap_body); 

    if(exp_soap_body_impl->namespc_uri)
    {
        AXIS2_FREE((*env)->allocator, exp_soap_body_impl->namespc_uri);
        exp_soap_body_impl->namespc_uri = NULL;
    }
    exp_soap_body_impl->namespc_uri = AXIS2_STRDUP(namespc_uri, env);
    if(!exp_soap_body_impl->namespc_uri)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

