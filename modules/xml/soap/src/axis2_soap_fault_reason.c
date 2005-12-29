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
 
#include <axis2_soap_fault_reason.h>
#include <axis2_soap_fault.h>
#include <axis2_om_element.h>
#include <axis2_soap_fault_text.h>
#include <axis2_soap_utils.h>

/*************************** impl struct **************************************/

typedef struct axis2_soap_fault_reason_impl_t
{
    axis2_soap_fault_reason_t fault_reason;
    
    axis2_om_element_t *om_ele;
    
    axis2_om_node_t *om_ele_node;
    
    axis2_soap_fault_t *parent;
    
    axis2_soap_fault_text_t *text;
    
    int soap_version;
    
}axis2_soap_fault_reason_impl_t;

/*************************** Macro ********************************************/

#define AXIS2_INTF_TO_IMPL(reason) ((axis2_soap_fault_reason_impl_t*)reason)

/*************************** function prototypes ******************************/

axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_free(axis2_soap_fault_reason_t *fault_reason,
                                  axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_soap_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_soap_fault_text_t *soap_text);
                                
axis2_soap_fault_text_t* AXIS2_CALL 
axis2_soap_fault_reason_get_soap_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                                
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_om_node_t *node);

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_reason_get_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                            
int AXIS2_CALL 
axis2_soap_fault_reason_get_soap_version
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_soap_version
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 int soap_version);

/************************* function implementations ***************************/

AXIS2_DECLARE(axis2_soap_fault_reason_t *)
axis2_soap_fault_reason_create(axis2_env_t **env)
{
 axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    fault_reason_impl = (axis2_soap_fault_reason_impl_t*) AXIS2_MALLOC(
                            (*env)->allocator,
                            sizeof(axis2_soap_fault_reason_impl_t));
    if(!fault_reason_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    fault_reason_impl->fault_reason.ops = NULL;
    fault_reason_impl->om_ele = NULL;
    fault_reason_impl->om_ele_node = NULL;
    fault_reason_impl->text = NULL;
    
    fault_reason_impl->soap_version = AXIS2_SOAP_VERSION_NOT_SET;
    fault_reason_impl->parent = NULL;
    
    fault_reason_impl->fault_reason.ops = 
            (axis2_soap_fault_reason_ops_t*)AXIS2_MALLOC((*env)->allocator,
                sizeof(axis2_soap_fault_reason_ops_t));
                
    if(!(fault_reason_impl->fault_reason.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, fault_reason_impl);
        return NULL;
    }                                                                  
    
    fault_reason_impl->fault_reason.ops->free_fn = 
        axis2_soap_fault_reason_free;
    
    fault_reason_impl->fault_reason.ops->set_soap_text =
        axis2_soap_fault_reason_set_soap_text;        
        
    fault_reason_impl->fault_reason.ops->get_soap_text =
        axis2_soap_fault_reason_get_soap_text;        
        
    fault_reason_impl->fault_reason.ops->set_base_node =
        axis2_soap_fault_reason_set_base_node;
        
    fault_reason_impl->fault_reason.ops->get_base_node =
        axis2_soap_fault_reason_get_base_node;
        
    fault_reason_impl->fault_reason.ops->get_soap_version =
        axis2_soap_fault_reason_get_soap_version; 
        
    fault_reason_impl->fault_reason.ops->set_soap_version =
        axis2_soap_fault_reason_set_soap_version;                    
                                
  return  &(fault_reason_impl->fault_reason);  
}

AXIS2_DECLARE(axis2_soap_fault_reason_t *)
axis2_soap_fault_reason_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault,
                            axis2_bool_t extract_ns_from_parent)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    axis2_soap_fault_reason_t *fault_reason = NULL;
    axis2_om_element_t *this_ele = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_om_node_t *parent_node = NULL;
    axis2_om_element_t *parent_ele = NULL;
    axis2_om_namespace_t *parent_ns = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault, NULL);
    
    fault_reason = axis2_soap_fault_reason_create(env);
    if(!fault_reason)
        return NULL;
        
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    fault_reason_impl->parent = fault;
    
    parent_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env);
    
    parent_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
    
    if(extract_ns_from_parent)
    {
        parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    }
    
    this_ele = axis2_om_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    fault_reason_impl->om_ele = this_ele;
    fault_reason_impl->om_ele_node = this_node; 
    
    AXIS2_SOAP_FAULT_SET_REASON(fault, env, fault_reason);                                      
    return  &(fault_reason_impl->fault_reason);            
}                            
                            
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_free(axis2_soap_fault_reason_t *fault_reason,
                                  axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(fault_reason, env, AXIS2_FAILURE);
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(fault_reason_impl->text)
    {
        AXIS2_SOAP_FAULT_TEXT_FREE(fault_reason_impl->text, env);
        fault_reason_impl->text = NULL;
    }
    if(fault_reason->ops)
    {
        AXIS2_FREE((*env)->allocator, fault_reason->ops);
        fault_reason->ops = NULL;
    }
    AXIS2_FREE((*env)->allocator, fault_reason_impl);
    fault_reason_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_soap_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_soap_fault_text_t *soap_text)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    axis2_om_node_t *text_node = NULL;
    axis2_om_node_t *my_node = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault_reason, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_text, AXIS2_FAILURE);
    
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(fault_reason_impl->text)
    {
        my_node = AXIS2_SOAP_FAULT_TEXT_GET_BASE_NODE(
                            fault_reason_impl->text, env);
        AXIS2_SOAP_FAULT_TEXT_FREE(fault_reason_impl->text, env);
        fault_reason_impl->text = NULL;
    }
    
    text_node = AXIS2_SOAP_FAULT_TEXT_GET_BASE_NODE(
                            soap_text, env);
    
    axis2_soap_utils_set_new_node(env, 
                    fault_reason_impl->om_ele_node,
                    &my_node, 
                    text_node);
                    
    fault_reason_impl->text = soap_text;
    return AXIS2_SUCCESS;
}
                                
axis2_soap_fault_text_t* AXIS2_CALL 
axis2_soap_fault_reason_get_soap_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    axis2_om_node_t *text_node = NULL;
    axis2_om_node_t *this_node = NULL;
    axis2_soap_fault_text_t* text = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(fault_reason, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault_reason, NULL);
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    this_node = fault_reason_impl->om_ele_node ;
    
    
    if(!(fault_reason_impl->text))
    {
        text_node = axis2_soap_utils_get_child_with_name(env, 
                                            this_node, 
                                            AXIS2_SOAP12_SOAP_FAULT_TEXT_LOCAL_NAME);
                                                    
        text = axis2_soap_fault_text_create(env);
        AXIS2_SOAP_FAULT_TEXT_SET_BASE_NODE(text , env, text_node);
        fault_reason_impl->text = text;
    }
    return fault_reason_impl->text;

}
                                
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_om_node_t *node)
{
   axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
   AXIS2_FUNC_PARAM_CHECK(fault_reason, env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   fault_reason_impl->om_ele_node = node;
   fault_reason_impl->om_ele = (axis2_om_element_t *)
            AXIS2_OM_NODE_GET_DATA_ELEMENT(node, env);
   return AXIS2_SUCCESS;

}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_reason_get_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
     AXIS2_FUNC_PARAM_CHECK(fault_reason, env, NULL);
     return AXIS2_INTF_TO_IMPL(fault_reason)->om_ele_node;



}
                            
int AXIS2_CALL 
axis2_soap_fault_reason_get_soap_version
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(fault_reason, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(fault_reason)->soap_version;
}
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_soap_version
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 int soap_version)
{
    AXIS2_FUNC_PARAM_CHECK(fault_reason, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_version, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(fault_reason)->soap_version = soap_version;
    return AXIS2_SUCCESS;
}                            

/************************** soap11 ********************************************/
AXIS2_DECLARE(axis2_soap_fault_reason_t *)
axis2_soap11_fault_reason_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault, NULL);
    return axis2_soap_fault_reason_create_with_parent(env, fault, AXIS2_FALSE);
}                            

AXIS2_DECLARE(axis2_soap_fault_reason_t *)
axis2_soap12_fault_reason_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault, NULL);
    return axis2_soap_fault_reason_create_with_parent(env, fault, AXIS2_TRUE);

}
