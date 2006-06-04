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
 
#include <axis2_wsdl_extensible_attribute.h>

/** 
 * @brief Wsdl extensible attribute struct impl
 *   Wsdl extensible attribute
 */ 
typedef struct axis2_wsdl_extensible_attribute_impl
{
   axis2_wsdl_extensible_attribute_t extensible_attribute;
    
    axis2_qname_t *key;
    axis2_qname_t *value;

    
} axis2_wsdl_extensible_attribute_impl_t;

#define AXIS2_INTF_TO_IMPL(extensible_attribute) \
      ((axis2_wsdl_extensible_attribute_impl_t *)extensible_attribute)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
   axis2_wsdl_extensible_attribute_free (
                axis2_wsdl_extensible_attribute_t *extensible_attribute,
            const axis2_env_t *env);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_extensible_attribute_get_key(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_attribute_set_key(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env,
                            axis2_qname_t *key);
                        
axis2_qname_t *AXIS2_CALL
axis2_wsdl_extensible_attribute_get_value(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_attribute_set_value(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env,
                            axis2_qname_t *value);
                        
/************************** End of function prototypes ************************/

axis2_wsdl_extensible_attribute_t * AXIS2_CALL 
axis2_wsdl_extensible_attribute_create (const axis2_env_t *env)
{
    axis2_wsdl_extensible_attribute_impl_t *extensible_attribute_impl = NULL;
    
   AXIS2_ENV_CHECK(env, NULL);
   
   extensible_attribute_impl = (axis2_wsdl_extensible_attribute_impl_t *) 
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_extensible_attribute_impl_t));
   
   
   if(NULL == extensible_attribute_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    extensible_attribute_impl->key = NULL;
    extensible_attribute_impl->value = NULL;
    extensible_attribute_impl->extensible_attribute.ops = NULL;
    
   extensible_attribute_impl->extensible_attribute.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_extensible_attribute_ops_t));
   if(NULL == extensible_attribute_impl->extensible_attribute.ops)
    {
        axis2_wsdl_extensible_attribute_free(&(extensible_attribute_impl->
            extensible_attribute), env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    extensible_attribute_impl->extensible_attribute.ops->free =  
        axis2_wsdl_extensible_attribute_free;
    
   extensible_attribute_impl->extensible_attribute.ops->get_key =  
        axis2_wsdl_extensible_attribute_get_key;
   extensible_attribute_impl->extensible_attribute.ops->set_key = 
        axis2_wsdl_extensible_attribute_set_key;
   extensible_attribute_impl->extensible_attribute.ops->get_value =  
        axis2_wsdl_extensible_attribute_get_value;
   extensible_attribute_impl->extensible_attribute.ops->set_value = 
        axis2_wsdl_extensible_attribute_set_value;
   
    
   return &(extensible_attribute_impl->extensible_attribute);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_extensible_attribute_free (
                        axis2_wsdl_extensible_attribute_t *extensible_attribute, 
                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   if(NULL != extensible_attribute->ops)
        AXIS2_FREE(env->allocator, extensible_attribute->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(extensible_attribute)->key)
    {
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(extensible_attribute)->
            key, env);
        AXIS2_INTF_TO_IMPL(extensible_attribute)->key = NULL;    
    }
    
    if(NULL != AXIS2_INTF_TO_IMPL(extensible_attribute)->value)
    {
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(extensible_attribute)->
            value, env);
        AXIS2_INTF_TO_IMPL(extensible_attribute)->value = NULL;
    }
    
    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(extensible_attribute));
    
   return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_extensible_attribute_get_key(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(extensible_attribute)->key;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_attribute_set_key(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env,
                            axis2_qname_t *key) 
{
    axis2_wsdl_extensible_attribute_impl_t *extensible_attribute_impl = 
        AXIS2_INTF_TO_IMPL(extensible_attribute);
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, key, AXIS2_FAILURE);
    
    if(extensible_attribute_impl->key)
    {
        AXIS2_QNAME_FREE(extensible_attribute_impl->key, env);
        extensible_attribute_impl->key = NULL;
    }
    
    extensible_attribute_impl->key = AXIS2_QNAME_CLONE(key, env);
    if(!extensible_attribute_impl->key)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_extensible_attribute_get_value(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(extensible_attribute)->value;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_extensible_attribute_set_value(
                            axis2_wsdl_extensible_attribute_t *extensible_attribute,
                            const axis2_env_t *env,
                            axis2_qname_t *value) 
{
    axis2_wsdl_extensible_attribute_impl_t *extensible_attribute_impl = 
        AXIS2_INTF_TO_IMPL(extensible_attribute);
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);
    
    if(extensible_attribute_impl->value)
    {
        AXIS2_QNAME_FREE(extensible_attribute_impl->value, env);
        extensible_attribute_impl->value = NULL;
    }
    
    extensible_attribute_impl->value = AXIS2_QNAME_CLONE(value, env);
    if(!extensible_attribute_impl->value)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
