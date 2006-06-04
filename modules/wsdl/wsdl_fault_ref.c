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
 
#include <axis2_wsdl_fault_ref.h>

/** 
 * @brief Wsdl Binding Fault Reference struct impl
 * Referes to the MEP the Message relates to.  
 */ 
typedef struct axis2_wsdl_fault_ref_impl
{
    axis2_wsdl_fault_ref_t fault_ref;
    
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
    
    /**
     * Field ref
     */
    axis2_qname_t *ref;
    
} axis2_wsdl_fault_ref_impl_t;

#define AXIS2_INTF_TO_IMPL(fault_ref) \
      ((axis2_wsdl_fault_ref_impl_t *)fault_ref)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
   axis2_wsdl_fault_ref_free (axis2_wsdl_fault_ref_t *fault_ref,
                           const axis2_env_t *env);

axis2_char_t * AXIS2_CALL
axis2_wsdl_fault_ref_get_direction(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_wsdl_fault_ref_set_direction(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *direction);

axis2_char_t * AXIS2_CALL
axis2_wsdl_fault_ref_get_msg_label(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_fault_ref_set_msg_label(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *msg_label);
                                            
axis2_qname_t * AXIS2_CALL
axis2_wsdl_fault_ref_get_ref(axis2_wsdl_fault_ref_t *fault_ref,
                                const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wdsl_fault_ref_set_ref(axis2_wsdl_fault_ref_t *fault_ref,
                                const axis2_env_t *env,
                                axis2_qname_t *ref);                                           

/************************** End of function prototypes ************************/

axis2_wsdl_fault_ref_t * AXIS2_CALL 
axis2_wsdl_fault_ref_create (const axis2_env_t *env)
{
    axis2_wsdl_fault_ref_impl_t *fault_ref_impl = NULL;
    
   AXIS2_ENV_CHECK(env, NULL);
   
   fault_ref_impl = (axis2_wsdl_fault_ref_impl_t *) AXIS2_MALLOC(env->
        allocator, sizeof(axis2_wsdl_fault_ref_impl_t));
   
   if(NULL == fault_ref_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    fault_ref_impl->msg_label = NULL;
    fault_ref_impl->direction = NULL;
    fault_ref_impl->ref = NULL;
    fault_ref_impl->fault_ref.wsdl_component = NULL;
    fault_ref_impl->fault_ref.ops = NULL;
       
    fault_ref_impl->fault_ref.wsdl_component = axis2_wsdl_component_create(env);
 
    if(NULL == fault_ref_impl->fault_ref.wsdl_component)
    {
        AXIS2_FREE(env->allocator, fault_ref_impl);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   fault_ref_impl->fault_ref.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_fault_ref_ops_t));
   if(NULL == fault_ref_impl->fault_ref.ops)
    {
        AXIS2_WSDL_COMPONENT_FREE(fault_ref_impl->fault_ref.wsdl_component, env);
        AXIS2_FREE(env->allocator, fault_ref_impl);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   fault_ref_impl->fault_ref.ops->free =  axis2_wsdl_fault_ref_free;
    
   fault_ref_impl->fault_ref.ops->get_direction =  
        axis2_wsdl_fault_ref_get_direction;
    
   fault_ref_impl->fault_ref.ops->set_direction =  
        axis2_wsdl_fault_ref_set_direction;
    
    fault_ref_impl->fault_ref.ops->get_msg_label =  
        axis2_wsdl_fault_ref_get_msg_label;
    
   fault_ref_impl->fault_ref.ops->set_msg_label =  
        axis2_wsdl_fault_ref_set_msg_label;
    
    fault_ref_impl->fault_ref.ops->get_ref =  axis2_wsdl_fault_ref_get_ref;
   fault_ref_impl->fault_ref.ops->set_ref =  axis2_wdsl_fault_ref_set_ref;
    
   return &(fault_ref_impl->fault_ref);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_fault_ref_free (axis2_wsdl_fault_ref_t *fault_ref, 
                            const axis2_env_t *env)
{
    axis2_wsdl_fault_ref_impl_t *fault_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    fault_ref_impl = AXIS2_INTF_TO_IMPL(fault_ref);
    
   if(NULL != fault_ref->ops)
    {
        AXIS2_FREE(env->allocator, fault_ref->ops);
        fault_ref->ops = NULL;
    }
    
    if(NULL != fault_ref_impl->msg_label)
    {
        AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(fault_ref)->msg_label);
        fault_ref_impl->msg_label = NULL;
    }
    
    if(NULL != fault_ref_impl->direction)
    {
        AXIS2_FREE(env->allocator, fault_ref_impl->direction);
        fault_ref_impl->direction = NULL;   
    }
    
    if(NULL != fault_ref_impl->ref)
    {
        AXIS2_QNAME_FREE(fault_ref_impl->ref, env);
        fault_ref_impl->ref = NULL;
    }
    
    if(NULL != fault_ref->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(fault_ref->wsdl_component, env);
        fault_ref->wsdl_component = NULL;
    }
    
    if(fault_ref_impl)
        AXIS2_FREE(env->allocator, fault_ref_impl);
    fault_ref_impl = NULL;
    
   return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_fault_ref_get_direction(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(fault_ref)->direction;
}

axis2_status_t AXIS2_CALL 
axis2_wsdl_fault_ref_set_direction(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *direction) 
{
    axis2_wsdl_fault_ref_impl_t *fault_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, direction, AXIS2_FAILURE);
    
    fault_ref_impl = AXIS2_INTF_TO_IMPL(fault_ref);
    if(fault_ref_impl->direction)
    {
        AXIS2_FREE(env->allocator, fault_ref_impl->direction);
        fault_ref_impl->direction = NULL;
    }
    fault_ref_impl->direction = AXIS2_STRDUP(direction, env);
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL
axis2_wsdl_fault_ref_get_msg_label(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(fault_ref)->msg_label;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_fault_ref_set_msg_label(axis2_wsdl_fault_ref_t *fault_ref,
                                            const axis2_env_t *env,
                                            axis2_char_t *msg_label) 
{
    axis2_wsdl_fault_ref_impl_t *fault_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_label, AXIS2_FAILURE);
    
    fault_ref_impl = AXIS2_INTF_TO_IMPL(fault_ref);
    if(fault_ref_impl->msg_label)
    {
        AXIS2_FREE(env->allocator, fault_ref_impl->msg_label);
        fault_ref_impl->msg_label = NULL;
    }
    fault_ref_impl->msg_label = AXIS2_STRDUP(msg_label, env);
    return AXIS2_SUCCESS;
}

axis2_qname_t * AXIS2_CALL
axis2_wsdl_fault_ref_get_ref(axis2_wsdl_fault_ref_t *fault_ref,
                                const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(fault_ref)->ref;
}

axis2_status_t AXIS2_CALL
axis2_wdsl_fault_ref_set_ref(axis2_wsdl_fault_ref_t *fault_ref,
                                const axis2_env_t *env,
                                axis2_qname_t *ref) 
{
    axis2_wsdl_fault_ref_impl_t *fault_ref_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, ref, AXIS2_FAILURE);
    
    fault_ref_impl = AXIS2_INTF_TO_IMPL(fault_ref);
    if(fault_ref_impl->ref)
    {
        AXIS2_QNAME_FREE(fault_ref_impl->ref, env);
        fault_ref_impl->ref = NULL;
    }
    fault_ref_impl->ref = AXIS2_QNAME_CLONE(ref, env);
    return AXIS2_SUCCESS;
}
