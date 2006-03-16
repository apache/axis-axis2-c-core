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
 
 #include <axis2_soap_fault_node.h>
 #include <axis2_om_node.h>
 #include <axis2_om_element.h>
 
 /********************* impl struct *******************************************/
 
 typedef struct axis2_soap_fault_node_impl_t
 {
    axis2_soap_fault_node_t fault_node;
    
    axis2_om_node_t *om_ele_node;
    
 }axis2_soap_fault_node_impl_t;
 
 /******************* Macro ***************************************************/
 
 #define AXIS2_INTF_TO_IMPL(node) ((axis2_soap_fault_node_impl_t*)node) 
 
 /****************** function prototypes **************************************/
 
axis2_status_t AXIS2_CALL 
axis2_soap_fault_node_free(axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_soap_fault_node_set_value
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env,
                           axis2_char_t *fault_val);
                            
axis2_char_t* AXIS2_CALL 
axis2_soap_fault_node_get_value
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env);
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_node_set_base_node
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env,
                           axis2_om_node_t *node);

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_node_get_base_node
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env);
                        
 
 /*********************** function implementations ****************************/

AXIS2_DECLARE(axis2_soap_fault_node_t *)
axis2_soap_fault_node_create(axis2_env_t **env)
{
    axis2_soap_fault_node_impl_t *fault_node_impl = NULL;
    
    fault_node_impl = (axis2_soap_fault_node_impl_t*)AXIS2_MALLOC(
                            (*env)->allocator,
                            sizeof(axis2_soap_fault_node_impl_t));
    if(!fault_node_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                            
    fault_node_impl->fault_node.ops = NULL;
    fault_node_impl->om_ele_node = NULL;
    
    fault_node_impl->fault_node.ops = (axis2_soap_fault_node_ops_t*)AXIS2_MALLOC(
                    (*env)->allocator, sizeof(axis2_soap_fault_node_ops_t));
                    
    if(!(fault_node_impl->fault_node.ops))
    {
        AXIS2_FREE((*env)->allocator, fault_node_impl);
        fault_node_impl = NULL;
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }                    
                                                                        
    fault_node_impl->fault_node.ops->set_base_node =
        axis2_soap_fault_node_set_base_node;
        
    fault_node_impl->fault_node.ops->get_base_node =
        axis2_soap_fault_node_get_base_node;
        
    fault_node_impl->fault_node.ops->set_value =
        axis2_soap_fault_node_set_value;
        
    fault_node_impl->fault_node.ops->get_value =
        axis2_soap_fault_node_get_value;
    fault_node_impl->fault_node.ops->free_fn =
        axis2_soap_fault_node_free;
                                        
    return &(fault_node_impl->fault_node);
}
 
AXIS2_DECLARE(axis2_soap_fault_node_t *)
axis2_soap_fault_node_create_with_parent(axis2_env_t **env,
                                         axis2_soap_fault_t *fault)
{
    axis2_soap_fault_node_impl_t *fault_node_impl = NULL;
    axis2_soap_fault_node_t *fault_node = NULL;
    
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    
    axis2_om_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault, NULL);
    
    fault_node = axis2_soap_fault_node_create(env);
    if(!fault_node)
        return NULL;
        
    fault_node_impl = AXIS2_INTF_TO_IMPL(fault_node);
    
    parent_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env);
    if(!parent_node)
    {
        AXIS2_SOAP_FAULT_NODE_FREE(fault_node, env);
        return NULL;
    }
    
    parent_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
    if(!parent_ele)
    {
        AXIS2_SOAP_FAULT_NODE_FREE(fault_node, env);
        return NULL;
    }
    
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    
    this_ele = axis2_om_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP12_SOAP_FAULT_NODE_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    if(!this_ele)
    {
        AXIS2_SOAP_FAULT_NODE_FREE(fault_node, env);
        return NULL;
    }
                         
    fault_node_impl->om_ele_node = this_node;       
    
    AXIS2_SOAP_FAULT_SET_NODE(fault, env, fault_node);
                                    
    return  &(fault_node_impl->fault_node);  
}

                                    
                                         
axis2_status_t AXIS2_CALL 
axis2_soap_fault_node_free(axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(NULL != fault_node->ops)
    {
        AXIS2_FREE((*env)->allocator, fault_node->ops);
        fault_node->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(fault_node));
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_node_set_value
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env,
                           axis2_char_t *uri)
{
    axis2_soap_fault_node_impl_t *fault_node_impl = NULL;
    axis2_om_element_t *om_ele = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    
    fault_node_impl = AXIS2_INTF_TO_IMPL(fault_node);
    if(NULL != fault_node_impl->om_ele_node)
    {
        om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                    fault_node_impl->om_ele_node, env);

        if(NULL != om_ele)
        {
            return AXIS2_OM_ELEMENT_SET_TEXT(om_ele, env, uri, 
                    fault_node_impl->om_ele_node);
        }
    }

    return AXIS2_FAILURE;
}
                            
axis2_char_t* AXIS2_CALL 
axis2_soap_fault_node_get_value
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env)
{
    axis2_soap_fault_node_impl_t *fault_node_impl = NULL;
    axis2_om_element_t *om_ele =  NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    fault_node_impl = AXIS2_INTF_TO_IMPL(fault_node);
    if(NULL != fault_node_impl->om_ele_node)
    {
        om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
            fault_node_impl->om_ele_node, env);
        if(NULL != om_ele)
        {
            return AXIS2_OM_ELEMENT_GET_TEXT(om_ele,
                            env, fault_node_impl->om_ele_node);
        }
    
    }
    return NULL;
}
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_node_set_base_node
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env,
                           axis2_om_node_t *node)
{
    axis2_soap_fault_node_impl_t* fault_node_impl = NULL;
    fault_node_impl = AXIS2_INTF_TO_IMPL(fault_node); 
    
    AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
    if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
    {
        AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    fault_node_impl->om_ele_node = node;  
    
    return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_node_get_base_node
                          (axis2_soap_fault_node_t *fault_node,
                           axis2_env_t **env)
{
    axis2_soap_fault_node_impl_t* fault_node_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_node_impl = AXIS2_INTF_TO_IMPL(fault_node); 
    return fault_node_impl->om_ele_node;
}
                        
