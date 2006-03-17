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
#include <_axis2_soap_fault.h>
#include <axis2_om_element.h>
#include <axis2_soap_fault_text.h>
#include <axis2_soap_builder.h>
#include <axis2_soap_builder.h>

/*************************** impl struct **************************************/

typedef struct axis2_soap_fault_reason_impl_t
{
    axis2_soap_fault_reason_t fault_reason;
    /* corresponding om element node */    
    axis2_om_node_t *om_ele_node;
    /* sub element soap fault text */    
    axis2_soap_fault_text_t *text;
    /* pointer to soap builder */
    axis2_soap_builder_t *soap_builder;
    
}axis2_soap_fault_reason_impl_t;

/*************************** Macro ********************************************/

#define AXIS2_INTF_TO_IMPL(reason) ((axis2_soap_fault_reason_impl_t*)reason)

/*************************** function prototypes ******************************/

axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_free(axis2_soap_fault_reason_t *fault_reason,
                                  axis2_env_t **env);

axis2_soap_fault_text_t* AXIS2_CALL 
axis2_soap_fault_reason_get_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                                
axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_reason_get_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env);
                                 

                                    

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
    fault_reason_impl->om_ele_node = NULL;
    fault_reason_impl->text = NULL;
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
    
    fault_reason_impl->fault_reason.ops->get_soap_fault_text =
        axis2_soap_fault_reason_get_soap_fault_text;        
        
    fault_reason_impl->fault_reason.ops->get_base_node =
        axis2_soap_fault_reason_get_base_node;
        
  return  &(fault_reason_impl->fault_reason);  
}

AXIS2_DECLARE(axis2_soap_fault_reason_t *)
axis2_soap_fault_reason_create_with_parent(axis2_env_t **env,
                            axis2_soap_fault_t *fault)
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
    parent_node = AXIS2_SOAP_FAULT_GET_BASE_NODE(fault, env);
    if(!parent_node)
    {
        return NULL;
    }
    parent_ele  = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                        parent_node, env);
    if(!parent_ele)
    {
        return NULL;
    }
    
    parent_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(parent_ele, env);
    
    this_ele = axis2_om_element_create(env, 
                                       parent_node,                             
                                       AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME,
                                       parent_ns,
                                       &this_node);
    if(!this_ele)
    {
        return NULL;
    }
    
    fault_reason_impl->om_ele_node = this_node; 
    
    axis2_soap_fault_set_reason (fault, env, fault_reason);                                      
    
    return  &(fault_reason_impl->fault_reason);            
}                            
                            
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_free(axis2_soap_fault_reason_t *fault_reason,
                                  axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(NULL != fault_reason_impl->text)
    {
        AXIS2_SOAP_FAULT_TEXT_FREE(fault_reason_impl->text, env);
        fault_reason_impl->text = NULL;
    }
    if(NULL != fault_reason->ops)
    {
        AXIS2_FREE((*env)->allocator, fault_reason->ops);
        fault_reason->ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, fault_reason_impl);
    
    fault_reason_impl = NULL;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env,
                                 axis2_soap_fault_text_t *soap_text)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_text, AXIS2_FAILURE);
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    if(!(fault_reason_impl->text))
    {
        fault_reason_impl->text = soap_text;
        return AXIS2_SUCCESS;
    }
    else
    {
        AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, 
            " error trying to set soap fault text twice");
        
    }
    return AXIS2_FAILURE;
}
                                
axis2_soap_fault_text_t* AXIS2_CALL 
axis2_soap_fault_reason_get_soap_fault_text
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    int status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, fault_reason, NULL);
    
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    
    if(fault_reason_impl->text)
    {
        return fault_reason_impl->text;
    }    
    if(fault_reason_impl->soap_builder && !(fault_reason_impl->text))
    {
        while( !(fault_reason_impl->text) && 
            !(AXIS2_OM_NODE_GET_BUILD_STATUS(fault_reason_impl->om_ele_node, env)))
        {
           status = AXIS2_SOAP_BUILDER_NEXT(fault_reason_impl->soap_builder, env);
           if(status == AXIS2_FAILURE)
                return NULL;
        }
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
   
   AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
   AXIS2_PARAM_CHECK((*env)->error, node, AXIS2_FAILURE);
   
   fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
   
   if(AXIS2_OM_NODE_GET_NODE_TYPE(node, env) != AXIS2_OM_ELEMENT)
   {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_INVALID_BASE_TYPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
   }
   
   fault_reason_impl->om_ele_node = node;
   
   return AXIS2_SUCCESS;
}

axis2_om_node_t* AXIS2_CALL 
axis2_soap_fault_reason_get_base_node
                                (axis2_soap_fault_reason_t *fault_reason,
                                 axis2_env_t **env)
{
     AXIS2_ENV_CHECK(env, NULL);
     return AXIS2_INTF_TO_IMPL(fault_reason)->om_ele_node;
}
                            
axis2_status_t AXIS2_CALL 
axis2_soap_fault_reason_set_builder(axis2_soap_fault_reason_t *fault_reason,
                                    axis2_env_t **env,
                                    axis2_soap_builder_t *builder)
{
    axis2_soap_fault_reason_impl_t *fault_reason_impl = NULL;
    AXIS2_PARAM_CHECK((*env)->error, builder, AXIS2_FAILURE);
    
    fault_reason_impl = AXIS2_INTF_TO_IMPL(fault_reason);
    fault_reason_impl->soap_builder = builder;
    
    return AXIS2_SUCCESS;
}
