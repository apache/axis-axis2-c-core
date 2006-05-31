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
 
#include <axis2_wsdl_ext_soap_address.h>
#include <axis2_wsdl.h>

/** 
 * @brief Wsdl Extension Soap Address struct impl
 *	 Wsdl Extention Soap Address  
 */ 
typedef struct axis2_wsdl_ext_soap_address_impl
{
	axis2_wsdl_ext_soap_address_t ext_soap_address;
    axis2_char_t *location_uri;
    
} axis2_wsdl_ext_soap_address_impl_t;

#define AXIS2_INTF_TO_IMPL(ext_soap_address) \
		((axis2_wsdl_ext_soap_address_impl_t *)ext_soap_address)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_ext_soap_address_free (axis2_wsdl_ext_soap_address_t *ext_soap_address,
									const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_address_get_location_uri(axis2_wsdl_ext_soap_address_t *ext_soap_address,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_address_set_location_uri(axis2_wsdl_ext_soap_address_t *ext_soap_address,
                                const axis2_env_t *env,
                                axis2_char_t *location_uri);

/************************** End of function prototypes ************************/

axis2_wsdl_ext_soap_address_t * AXIS2_CALL 
axis2_wsdl_ext_soap_address_create (const axis2_env_t *env,
                                 axis2_qname_t *qtype)
{
    axis2_wsdl_ext_soap_address_impl_t *ext_soap_address_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	ext_soap_address_impl = (axis2_wsdl_ext_soap_address_impl_t *) AXIS2_MALLOC(env->allocator,
			sizeof(axis2_wsdl_ext_soap_address_impl_t));
	
	
	if(NULL == ext_soap_address_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    ext_soap_address_impl->location_uri = NULL;
    ext_soap_address_impl->ext_soap_address.ops = NULL;
    ext_soap_address_impl->ext_soap_address.ext_element = NULL;
	

    ext_soap_address_impl->ext_soap_address.ext_element = 
        axis2_wsdl_extensible_element_create(env);
    if(NULL == ext_soap_address_impl->ext_soap_address.ext_element)
    {
        axis2_wsdl_ext_soap_address_free(&(ext_soap_address_impl->ext_soap_address), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }    
    
	ext_soap_address_impl->ext_soap_address.ops = 
		AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_ext_soap_address_ops_t));
	if(NULL == ext_soap_address_impl->ext_soap_address.ops)
    {
        axis2_wsdl_ext_soap_address_free(&(ext_soap_address_impl->ext_soap_address), env);
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    if(!qtype)
    {
        qtype = axis2_qname_create(env, "address", AXIS2_SOAP_11_ADDRESS, NULL);
        if(!qtype) return NULL;
    }    
    AXIS2_WSDL_EXTENSIBLE_ELEMENT_SET_TYPE(
        (&(ext_soap_address_impl->ext_soap_address))->ext_element, env, qtype);

	ext_soap_address_impl->ext_soap_address.ops->free =  axis2_wsdl_ext_soap_address_free;
	ext_soap_address_impl->ext_soap_address.ops->get_location_uri = 
        axis2_wsdl_ext_soap_address_get_location_uri;
    ext_soap_address_impl->ext_soap_address.ops->set_location_uri = 
        axis2_wsdl_ext_soap_address_set_location_uri;
	
	return &(ext_soap_address_impl->ext_soap_address);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_ext_soap_address_free (axis2_wsdl_ext_soap_address_t *ext_soap_address, 
                            const axis2_env_t *env)
{
    axis2_wsdl_ext_soap_address_impl_t *ext_soap_address_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    ext_soap_address_impl = AXIS2_INTF_TO_IMPL(ext_soap_address);
    
	if(NULL != ext_soap_address->ops)
        AXIS2_FREE(env->allocator, ext_soap_address->ops);
    
    if(NULL != ext_soap_address_impl->location_uri)
    {
        AXIS2_FREE(env->allocator, ext_soap_address_impl->location_uri);
        ext_soap_address_impl->location_uri = NULL;
    }
    
    AXIS2_FREE(env->allocator, ext_soap_address_impl);
    ext_soap_address_impl = NULL;
    
	return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_ext_soap_address_get_location_uri(axis2_wsdl_ext_soap_address_t *ext_soap_address,
                                const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(ext_soap_address)->location_uri;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_ext_soap_address_set_location_uri(axis2_wsdl_ext_soap_address_t *ext_soap_address,
                                const axis2_env_t *env,
                                axis2_char_t *location_uri) 
{
    axis2_wsdl_ext_soap_address_impl_t *exp_soap_address_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, location_uri, AXIS2_FAILURE);
    exp_soap_address_impl = AXIS2_INTF_TO_IMPL(ext_soap_address); 

    if(exp_soap_address_impl->location_uri)
    {
        AXIS2_FREE(env->allocator, exp_soap_address_impl->location_uri);
        exp_soap_address_impl->location_uri = NULL;
    }
    exp_soap_address_impl->location_uri = AXIS2_STRDUP(location_uri, env);
    if(!exp_soap_address_impl->location_uri)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

