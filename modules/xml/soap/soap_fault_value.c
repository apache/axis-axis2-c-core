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
 #include <axis2_soap_fault_value.h>
 #include <axis2_soap_fault_sub_code.h>
 #include <axis2_soap_fault_code.h>
 
 
/****************************** impl struct  *********************************/

typedef struct axis2_soap_fault_value_impl_t
{
    axis2_soap_fault_value_t fault_value;
    
    /** store om element node */
    axis2_om_node_t *om_ele_node;
    
    /** pointer to parent */
}axis2_soap_fault_value_impl_t;

/*********************** macro ************************************************/

#define AXIS2_INTF_TO_IMPL(fault_val) \
        ((axis2_soap_fault_value_impl_t*)fault_val)

/******************** function prototypes *************************************/

axis2_status_t AXIS2_CALL 
axis2_soap_fault_value_free(axis2_soap_fault_value_t *fault_value,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_soap_fault_value_set_base_node(axis2_soap_fault_value_t *fault_value,
                                  axis2_env_t **env,
                                  axis2_om_node_t *node);
    
axis2_om_node_t* AXIS2_CALL
axis2_soap_fault_value_get_base_node(axis2_soap_fault_value_t *fault_value,
                                     axis2_env_t **env);
                                  

/*************************** function implementations *************************/

AXIS2_DECLARE(axis2_soap_fault_value_t *)
axis2_soap_fault_value_create(axis2_env_t **env)
{
    axis2_soap_fault_value_impl_t *fault_val_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    
    fault_val_impl = (axis2_soap_fault_value_impl_t *)AXIS2_MALLOC(
                            (*env)->allocator,
                            sizeof(axis2_soap_fault_value_impl_t));
    if(!fault_val_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_val_impl->om_ele_node = NULL;
    
    fault_val_impl->fault_value.ops = NULL;
    fault_val_impl->fault_value.ops = (axis2_soap_fault_value_ops_t *)AXIS2_MALLOC(
                                        (*env)->allocator, sizeof(axis2_soap_fault_value_ops_t));
    if(!(fault_val_impl->fault_value.ops))
    {
        AXIS2_FREE((*env)->allocator, fault_val_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_val_impl->fault_value.ops->free_fn =
        axis2_soap_fault_value_free;
        
    fault_val_impl->fault_value.ops->set_base_node =
        axis2_soap_fault_value_set_base_node;
    
    fault_val_impl->fault_value.ops->get_base_node =
        axis2_soap_fault_value_get_base_node;
        
    return &(fault_val_impl->fault_value);
}


AXIS2_DECLARE(axis2_soap_fault_value_t *)
axis2_soap_fault_value_create_with_subcode(axis2_env_t **env,
                            axis2_soap_fault_sub_code_t *parent)
{
    axis2_soap_fault_value_impl_t *fault_val_impl = NULL;
    axis2_soap_fault_value_t *fault_value = NULL;
    
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, parent, NULL);
    
    fault_value = axis2_soap_fault_value_create(env);
    
    if(!fault_value)
    {
        return NULL;
    }
    
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    
    
    parent_node = AXIS2_SOAP_FAULT_SUB_CODE_GET_BASE_NODE(parent, env);
    
    if(!parent_node)
    {
        AXIS2_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    
    parent_ele  = (axis2_om_element_t*)
                    AXIS2_OM_NODE_GET_DATA_ELEMENT(parent_node, env);
    
    if(!parent_ele)
    {
        AXIS2_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    this_ele = axis2_om_element_create(env, 
                    parent_node,
                    AXIS2_SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME, 
                    AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env),
                    &this_node);
    if(!this_ele)
    {
        AXIS2_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    
    fault_val_impl->om_ele_node = this_node;
    
    AXIS2_SOAP_FAULT_SUB_CODE_SET_VALUE(parent, env, fault_value);
    
    return &(fault_val_impl->fault_value);
}                            

AXIS2_DECLARE(axis2_soap_fault_value_t *)
axis2_soap_fault_value_create_with_code(axis2_env_t **env,
                            axis2_soap_fault_code_t *parent)
{
    axis2_soap_fault_value_impl_t *fault_val_impl = NULL;
    axis2_soap_fault_value_t *fault_value = NULL;
    
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, parent, NULL);
    
    fault_value = axis2_soap_fault_value_create(env);
    
    if(!fault_value)
    {
        return NULL;
    }
    
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    
    parent_node = AXIS2_SOAP_FAULT_CODE_GET_BASE_NODE(parent, env);
    if(!parent_node)
    {
        AXIS2_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    parent_ele  = (axis2_om_element_t*)
                    AXIS2_OM_NODE_GET_DATA_ELEMENT(parent_node, env);
    if(!parent_ele)
    {
        AXIS2_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    
    this_ele = axis2_om_element_create(env, 
                    parent_node,
                    AXIS2_SOAP12_SOAP_FAULT_VALUE_LOCAL_NAME, 
                    AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env),
                    &this_node);
    if(!this_ele)
    {
        AXIS2_SOAP_FAULT_VALUE_FREE(fault_value, env);
        return NULL;
    }
    fault_val_impl->om_ele_node = this_node;
    
    AXIS2_SOAP_FAULT_CODE_SET_VALUE(parent, env, fault_value);
    
    return &(fault_val_impl->fault_value);
}                                                        


axis2_status_t AXIS2_CALL 
axis2_soap_fault_value_free(axis2_soap_fault_value_t *fault_value,
                              axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if(NULL != fault_value->ops)
    {
        AXIS2_FREE((*env)->allocator, fault_value->ops);
        fault_value->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(fault_value));
    fault_value = NULL;
    return AXIS2_SUCCESS;
}                              

axis2_status_t AXIS2_CALL
axis2_soap_fault_value_set_base_node(axis2_soap_fault_value_t *fault_value,
                                  axis2_env_t **env,
                                  axis2_om_node_t *node)
{
    axis2_soap_fault_value_impl_t *fault_val_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
    {
        AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    fault_val_impl->om_ele_node = node;
    return AXIS2_SUCCESS;
}                                  
    
axis2_om_node_t* AXIS2_CALL
axis2_soap_fault_value_get_base_node(axis2_soap_fault_value_t *fault_value,
                                  axis2_env_t **env)
{
    axis2_soap_fault_value_impl_t *fault_val_impl = NULL;
    fault_val_impl = AXIS2_INTF_TO_IMPL(fault_value);
    return fault_val_impl->om_ele_node;
}                                  

