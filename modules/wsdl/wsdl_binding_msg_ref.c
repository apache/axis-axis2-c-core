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
 
#include <axis2_wsdl_binding_msg_ref.h>

/** 
 * @brief Wsdl Binding Message Reference struct impl
 * Referes to the MEP the Message relates to.  
 */ 
typedef struct axis2_wsdl_binding_msg_ref_impl
{
    axis2_wsdl_binding_msg_ref_t binding_msg_ref;
    
    /**
     * Field messageLabel
     */
    axis2_char_t *msg_label;
    
    /**
     * Field Direction
     * Can be "in" or "out" depending on the element name being "input" or
     * "output" respectively;
     */
    axis2_char_t *direction;
    
} axis2_wsdl_binding_msg_ref_impl_t;

#define AXIS2_INTF_TO_IMPL(binding_msg_ref) \
      ((axis2_wsdl_binding_msg_ref_impl_t *)binding_msg_ref)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
   axis2_wsdl_binding_msg_ref_free (axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                           const axis2_env_t *env);

axis2_char_t * AXIS2_CALL
axis2_wsdl_binding_msg_ref_get_direction(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_msg_ref_set_direction(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *direction);

axis2_char_t * AXIS2_CALL
axis2_wsdl_binding_msg_ref_get_msg_label(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_msg_ref_set_msg_label(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *msg_label);

/************************** End of function prototypes ************************/

axis2_wsdl_binding_msg_ref_t * AXIS2_CALL 
axis2_wsdl_binding_msg_ref_create (const axis2_env_t *env)
{
    axis2_wsdl_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    
   AXIS2_ENV_CHECK(env, NULL);
   
   binding_msg_ref_impl = (axis2_wsdl_binding_msg_ref_impl_t *) 
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_binding_msg_ref_impl_t));
   
   
   if(NULL == binding_msg_ref_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    binding_msg_ref_impl->msg_label = NULL;
    binding_msg_ref_impl->direction = NULL;
    binding_msg_ref_impl->binding_msg_ref.extensible_component = NULL;   
    binding_msg_ref_impl->binding_msg_ref.ops = NULL;
    
    binding_msg_ref_impl->binding_msg_ref.extensible_component = 
        axis2_wsdl_extensible_component_create(env);
 
    if(NULL == binding_msg_ref_impl->binding_msg_ref.extensible_component)
    {
        axis2_wsdl_binding_msg_ref_free(&(binding_msg_ref_impl->binding_msg_ref),
            env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   binding_msg_ref_impl->binding_msg_ref.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_binding_msg_ref_ops_t));
   if(NULL == binding_msg_ref_impl->binding_msg_ref.ops)
    {
        axis2_wsdl_binding_msg_ref_free(&(binding_msg_ref_impl->binding_msg_ref),
            env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   binding_msg_ref_impl->binding_msg_ref.ops->free =  axis2_wsdl_binding_msg_ref_free;
    
   binding_msg_ref_impl->binding_msg_ref.ops->get_direction =  
        axis2_wsdl_binding_msg_ref_get_direction;
    
   binding_msg_ref_impl->binding_msg_ref.ops->set_direction =  
        axis2_wsdl_binding_msg_ref_set_direction;
    
    binding_msg_ref_impl->binding_msg_ref.ops->get_msg_label =  
        axis2_wsdl_binding_msg_ref_get_msg_label;
    
   binding_msg_ref_impl->binding_msg_ref.ops->set_msg_label =  
        axis2_wsdl_binding_msg_ref_set_msg_label;
    
   return &(binding_msg_ref_impl->binding_msg_ref);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_msg_ref_free (axis2_wsdl_binding_msg_ref_t *binding_msg_ref, 
                            const axis2_env_t *env)
{
    axis2_wsdl_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    binding_msg_ref_impl = AXIS2_INTF_TO_IMPL(binding_msg_ref);
    
   if(NULL != binding_msg_ref->ops)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref->ops);
        binding_msg_ref->ops = NULL;
    }
    
    if(NULL != binding_msg_ref_impl->msg_label)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->msg_label);
        binding_msg_ref_impl->msg_label = NULL;
    }
    
    if(NULL != binding_msg_ref_impl->direction)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->direction);
        binding_msg_ref_impl->direction = NULL;
    }
    
    if(NULL != binding_msg_ref->extensible_component)
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(binding_msg_ref->
            extensible_component, env);
    
    if(binding_msg_ref_impl)
        AXIS2_FREE(env->allocator, binding_msg_ref_impl);
    binding_msg_ref_impl = NULL;
   return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_binding_msg_ref_get_direction(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_msg_ref)->direction;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_msg_ref_set_direction(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *direction) 
{
    axis2_wsdl_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, direction, AXIS2_FAILURE);
    
    binding_msg_ref_impl = AXIS2_INTF_TO_IMPL(binding_msg_ref);
    
    if(binding_msg_ref_impl->direction)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->direction);
        binding_msg_ref_impl->direction = NULL;
    }
    binding_msg_ref_impl->direction = direction;
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_binding_msg_ref_get_msg_label(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_msg_ref)->msg_label;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_msg_ref_set_msg_label(axis2_wsdl_binding_msg_ref_t *binding_msg_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *msg_label) 
{
    axis2_wsdl_binding_msg_ref_impl_t *binding_msg_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_label, AXIS2_FAILURE);
    
    binding_msg_ref_impl = AXIS2_INTF_TO_IMPL(binding_msg_ref);
    
    if(binding_msg_ref_impl->msg_label)
    {
        AXIS2_FREE(env->allocator, binding_msg_ref_impl->msg_label);
        binding_msg_ref_impl->direction = NULL;
    }
    AXIS2_INTF_TO_IMPL(binding_msg_ref)->msg_label = msg_label;
    return AXIS2_SUCCESS;
}
