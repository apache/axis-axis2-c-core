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
 #include <axiom_soap_fault_value.h>
 #include "_axiom_soap_fault_sub_code.h"
 #include "_axiom_soap_fault_code.h"
 #include <axiom_element.h>
 #include <axiom_text.h>
 
/****************************** impl struct  *********************************/

typedef struct axiom_soap_fault_value_impl_t
{
    axiom_soap_fault_value_t fault_value;
    
    /** store om element node */
    axiom_node_t *om_ele_node;
    
    /** pointer to parent */
}axiom_soap_fault_value_impl_t;

/*********************** macro ************************************************/

#define AXIS2_INTF_TO_IMPL(fault_val) \
        ((axiom_soap_fault_value_impl_t*)fault_val)

/******************** function prototypes *************************************/

axis2_status_t AXIS2_CALL 
axiom_soap_fault_value_free(axiom_soap_fault_value_t *fault_value,
                            const axis2_env_t *env);

axiom_node_t* AXIS2_CALL
axiom_soap_fault_value_get_base_node(axiom_soap_fault_value_t *fault_value,
                                     const axis2_env_t *env);
                                  
axis2_char_t* AXIS2_CALL
axiom_soap_fault_value_get_text(axiom_soap_fault_value_t *fault_value,
                           const axis2_env_t *env);
                           
axis2_status_t AXIS2_CALL
axiom_soap_fault_value_set_text(axiom_soap_fault_value_t *fault_value,
                        const axis2_env_t *env,
                        axis2_char_t *text);                           
                           
/*************************** function implementations *************************/

AXIS2_EXTERN axiom_soap_fault_value_t * AXIS2_CALL
axiom_soap_fault_value_create(const axis2_env_t *env)
{
    axiom_soap_fault_value_impl_t *fault_val_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    fault_val_impl = (axiom_soap_fault_value_impl_t *)AXIS2_MALLOC(
                            env->allocator,
                            sizeof(axiom_soap_fault_value_impl_t));
    if(!fault_val_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_val_impl->om_ele_node = NULL;
    
    fault_val_impl->fault_value.ops = NULL;
    fault_val_impl->fault_value.ops = (axiom_soap_fault_value_ops_t *)AXIS2_MALLOC(
                                        env->allocator, sizeof(axiom_soap_fault_value_ops_t));
    if(!(fault_val_impl->fault_value.ops))
    {
        AXIS2_FREE(env->allocator, fault_val_impl);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_val_impl->fault_value.ops->free_fn =
        axiom_soap_fault_value_free;
        
    fault_val_impl->fault_value.ops->get_base_node =
        axiom_soap_fault_value_get_base_node;

   fault_val_impl->fault_value.ops->get_text =
      axiom_soap_fault_value_get_text;
      
    fault_val_impl->fault_value.ops->set_text =
        axiom_soap_fault_value_set_text;             
        
    return &(fault_val_impl->fault_value);
}


AXIS2_EXTERN axiom_soap_fault_value_t * AXIS2_CALL
axiom_soap_fault_value_create_with_subcode(const axis2_env_t *env,
                            axiom_soap_fault_sub_code_t *parent)
{
    axiom_soap_fault_value_impl_t *fault_val_impl = NULL;
    axiom_soap_fault_value_t *fault_value = NULL;
    
    axiom_element_t *this_ele = NULL;
    axiom_node_t *this_node = NULL;
    axiom_namespace_t *parent_ns = NULL;
    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele = NULL;
    int soap_version = -1;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, parent, NULL);
    
    fault_value = axiom_soap_fault_value_create(env);
    
    if(!fault_value)
    {
        return NULL;
    }
    
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    
    
    parent_node = AXIOM_SOAP_FAULT_SUB_CODE_GET_BASE_NODE(parent, env);
    
    if(!parent_node)
    {
        AXIOM_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    
    parent_ele  = (axiom_element_t*)
                    AXIOM_NODE_GET_DATA_ELEMENT(parent_node, env);
    
    if(!parent_ele)
    {
        AXIOM_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    soap_version = 
        axiom_soap_fault_sub_code_get_soap_version(parent, env);
    if(soap_version == AXIOM_SOAP12)
    {
        parent_ns = AXIOM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    }
    
    this_ele = axiom_element_create(env, 
                    parent_node,
                    AXIOM_SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME, 
                    parent_ns,
                    &this_node);
    if(!this_ele)
    {
        AXIOM_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    
    fault_val_impl->om_ele_node = this_node;
    
    axiom_soap_fault_sub_code_set_value (parent, env, fault_value);
    
    return &(fault_val_impl->fault_value);
}                            

AXIS2_EXTERN axiom_soap_fault_value_t * AXIS2_CALL
axiom_soap_fault_value_create_with_code(const axis2_env_t *env,
                            axiom_soap_fault_code_t *parent)
{
    axiom_soap_fault_value_impl_t *fault_val_impl = NULL;
    axiom_soap_fault_value_t *fault_value = NULL;
    int soap_version = -1;
    axiom_element_t *this_ele = NULL;
    axiom_node_t *this_node = NULL;
    axiom_namespace_t *parent_ns = NULL;
    axiom_node_t *parent_node = NULL;
    axiom_element_t *parent_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, parent, NULL);
    
    fault_value = axiom_soap_fault_value_create(env);
    
    if(!fault_value)
    {
        return NULL;
    }
    
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    
    parent_node = AXIOM_SOAP_FAULT_CODE_GET_BASE_NODE(parent, env);
    if(!parent_node)
    {
        AXIOM_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    parent_ele  = (axiom_element_t*)
                    AXIOM_NODE_GET_DATA_ELEMENT(parent_node, env);
    if(!parent_ele)
    {
        AXIOM_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    soap_version = axiom_soap_fault_code_get_soap_version(parent, env);
    if(soap_version == AXIOM_SOAP12)
    {
        parent_ns = AXIOM_ELEMENT_GET_NAMESPACE(parent_ele, env, parent_node);
    }
    this_ele = axiom_element_create(env, 
                parent_node, AXIOM_SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME, 
                parent_ns, &this_node);
                
    if(!this_ele)
    {
        AXIOM_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    fault_val_impl->om_ele_node = this_node;
    
    axiom_soap_fault_code_set_value (parent, env, fault_value);
    
    return &(fault_val_impl->fault_value);
}                                                        


axis2_status_t AXIS2_CALL 
axiom_soap_fault_value_free(axiom_soap_fault_value_t *fault_value,
                              const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if(NULL != fault_value->ops)
    {
        AXIS2_FREE(env->allocator, fault_value->ops);
        fault_value->ops = NULL;
    }
    AXIS2_FREE(env->allocator, AXIS2_INTF_TO_IMPL(fault_value));
    fault_value = NULL;
    return AXIS2_SUCCESS;
}                              

axis2_status_t AXIS2_CALL
axiom_soap_fault_value_set_base_node(axiom_soap_fault_value_t *fault_value,
                                  const axis2_env_t *env,
                                  axiom_node_t *node)
{
    axiom_soap_fault_value_impl_t *fault_val_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if(AXIOM_NODE_GET_NODE_TYPE(node, env) != AXIOM_ELEMENT)
    {
        AXIS2_ERROR_SET(env->error, 
                AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    fault_val_impl->om_ele_node = node;
    return AXIS2_SUCCESS;
}                                  
    
axiom_node_t* AXIS2_CALL
axiom_soap_fault_value_get_base_node(axiom_soap_fault_value_t *fault_value,
                                  const axis2_env_t *env)
{
    axiom_soap_fault_value_impl_t *fault_val_impl = NULL;
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    return fault_val_impl->om_ele_node;
}

axis2_char_t* AXIS2_CALL
axiom_soap_fault_value_get_text(axiom_soap_fault_value_t *fault_value,
                        const axis2_env_t *env)
{
   axiom_node_t *value_node = NULL;
   axiom_element_t *value_element = NULL;
   
   value_node = axiom_soap_fault_value_get_base_node(fault_value, env);
   
   if (!value_node)
      return NULL;

   value_element = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(
      value_node, env);
   
   if (!value_element)
      return NULL;
   
   return AXIOM_ELEMENT_GET_TEXT(value_element, env, value_node);
}

axis2_status_t AXIS2_CALL
axiom_soap_fault_value_set_text(axiom_soap_fault_value_t *fault_value,
                        const axis2_env_t *env,
                        axis2_char_t *text)
{
    axiom_soap_fault_value_impl_t *fault_value_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, text, AXIS2_FAILURE);
    fault_value_impl = AXIS2_INTF_TO_IMPL(fault_value);
    if(NULL != fault_value_impl->om_ele_node && 
        AXIOM_NODE_GET_NODE_TYPE(fault_value_impl->om_ele_node, env) == AXIOM_ELEMENT)
    {
        axiom_element_t *om_ele = NULL;
        om_ele = (axiom_element_t *)
            AXIOM_NODE_GET_DATA_ELEMENT(fault_value_impl->om_ele_node, env);
        return AXIOM_ELEMENT_SET_TEXT(om_ele, env, text, fault_value_impl->om_ele_node);
    }
    return AXIS2_FAILURE;
}                        
