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
 
 #include <axis2_soap_fault_role.h>
 #include <axis2_soap_fault.h>
 #include <axis2_soap_builder.h>
 #include <axis2_om_element.h>
 
 /***************************** impl struct **********************************/
 
 typedef struct axis2_soap_fault_role_impl_t
 {
    axis2_soap_fault_role_t fault_role;
    
    axis2_om_node_t *om_ele_node;
    
 }axis2_soap_fault_role_impl_t;

/**************************** Macro *******************************************/

#define AXIS2_INTF_TO_IMPL(role) ((axis2_soap_fault_role_impl_t*)role)

/**************************** function prototypes *****************************/

axis2_status_t AXIS2_CALL 
axis2_soap_fault_role_free(axis2_soap_fault_role_t *fault_role,
                              axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_soap_fault_role_set_role_value
                            (axis2_soap_fault_role_t *fault_role,
                             axis2_env_t **env,
                             axis2_char_t* uri);
                                
axis2_char_t* AXIS2_CALL 
axis2_soap_fault_role_get_role_value
                            (axis2_soap_fault_role_t *fault_role,
                             axis2_env_t **env);
                                
axis2_status_t AXIS2_CALL 
axis2_soap_fault_role_set_base_node
                            (axis2_soap_fault_role_t *fault_role,
                             axis2_env_t **env,
                             axis2_om_node_t *node);

axis2_om_node_t* AXIS2_CALL
axis2_soap_fault_role_get_base_node
                        (axis2_soap_fault_role_t *fault_role,
                         axis2_env_t **env);
                            
/*********************** end function prototypes ******************************/


AXIS2_DECLARE(axis2_soap_fault_role_t *)
axis2_soap_fault_role_create(axis2_env_t **env)
{
    axis2_soap_fault_role_impl_t *fault_role_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    fault_role_impl = (axis2_soap_fault_role_impl_t*) AXIS2_MALLOC(
                            (*env)->allocator,
                            sizeof(axis2_soap_fault_role_impl_t));
    if(!fault_role_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_role_impl->fault_role.ops = NULL;
    fault_role_impl->om_ele_node = NULL;
    
    fault_role_impl->fault_role.ops = 
            (axis2_soap_fault_role_ops_t*)AXIS2_MALLOC((*env)->allocator,
                sizeof(axis2_soap_fault_role_ops_t));
                
    if(!(fault_role_impl->fault_role.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, fault_role_impl);
        return NULL;
    }                                                                  
    
    fault_role_impl->fault_role.ops->free_fn = 
        axis2_soap_fault_role_free;
        
    fault_role_impl->fault_role.ops->get_role_value =
        axis2_soap_fault_role_get_role_value;
        
    fault_role_impl->fault_role.ops->set_role_value =
        axis2_soap_fault_role_set_role_value;
        
    fault_role_impl->fault_role.ops->set_base_node =
        axis2_soap_fault_role_set_base_node;
        
    fault_role_impl->fault_role.ops->get_base_node =
        axis2_soap_fault_role_get_base_node;
        
  return  &(fault_role_impl->fault_role);  
}

AXIS2_DECLARE(axis2_soap_fault_role_t *)
axis2_soap_fault_role_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault)
{
    axis2_soap_fault_role_impl_t *fault_role_impl = NULL;
    axis2_soap_fault_role_t *fault_role = NULL;
    
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    
    axis2_om_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault, NULL);
    
    fault_role = axis2_soap_fault_role_create(env);
    if(!fault_role)
        return NULL;
        
    fault_role_impl = AXIS2_INTF_TO_IMPL(fault_role);
    
    parent_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env);
    if(!parent_node)
    {
        AXIS2_SOAP_FAULT_ROLE_FREE(fault_role, env);
        return NULL;
    }
    parent_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
   
    if(!parent_ele)
    {
        AXIS2_SOAP_FAULT_ROLE_FREE(fault_role, env);
        return NULL;
    }
    
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
       
    this_ele = axis2_om_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    if(!this_ele)
    {
        AXIS2_SOAP_FAULT_ROLE_FREE(fault_role, env);
        return NULL;
    }
                                       
    fault_role_impl->om_ele_node = this_node;  
    AXIS2_SOAP_FAULT_SET_ROLE(fault, env, fault_role);                                     
    return  &(fault_role_impl->fault_role);            
}


axis2_status_t AXIS2_CALL 
axis2_soap_fault_role_free(axis2_soap_fault_role_t *fault_role,
                              axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(NULL != fault_role->ops)
    {
        AXIS2_FREE((*env)->allocator, fault_role->ops);
        fault_role->ops = NULL;
    }    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(fault_role));        
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_role_set_role_value
                            (axis2_soap_fault_role_t *fault_role,
                             axis2_env_t **env,
                             axis2_char_t* uri)
{
    axis2_soap_fault_role_impl_t *fault_role_impl = NULL;
    axis2_om_element_t *role_ele = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, uri, AXIS2_FAILURE);
    fault_role_impl = AXIS2_INTF_TO_IMPL(fault_role);
    
    if(!fault_role_impl->om_ele_node)
        return AXIS2_FAILURE;
        
    role_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT
                    (fault_role_impl->om_ele_node, env);
    
    if(NULL != role_ele)
    {
        return AXIS2_OM_ELEMENT_SET_TEXT(role_ele,  env, uri,
                                fault_role_impl->om_ele_node);
    }
    return AXIS2_FAILURE;    
}
                                
axis2_char_t* AXIS2_CALL 
axis2_soap_fault_role_get_role_value
                            (axis2_soap_fault_role_t *fault_role,
                             axis2_env_t **env)
{
    axis2_soap_fault_role_impl_t *fault_role_impl = NULL;
    axis2_om_element_t *role_ele = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    fault_role_impl = AXIS2_INTF_TO_IMPL(fault_role);
    if(!fault_role_impl->om_ele_node)
        return NULL;
    
    role_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT
                    (fault_role_impl->om_ele_node, env);
                    
    if(NULL != role_ele)   
    {
        return AXIS2_OM_ELEMENT_GET_TEXT(role_ele,  env,
                                         fault_role_impl->om_ele_node); 
    }
    return NULL;
}
                                
axis2_status_t AXIS2_CALL 
axis2_soap_fault_role_set_base_node
                            (axis2_soap_fault_role_t *fault_role,
                             axis2_env_t **env,
                             axis2_om_node_t *node)
{
   axis2_soap_fault_role_impl_t *fault_role_impl = NULL;
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   fault_role_impl = AXIS2_INTF_TO_IMPL(fault_role);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   fault_role_impl->om_ele_node = node;
   return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL
axis2_soap_fault_role_get_base_node
                        (axis2_soap_fault_role_t *fault_role,
                         axis2_env_t **env)
{
     AXIS2_ENV_CHECK(env, NULL);
     return AXIS2_INTF_TO_IMPL(fault_role)->om_ele_node;
}
