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
 
 #include "axiom_soap11_builder_helper.h"
 #include <axiom_stax_builder.h>
 #include <axiom_soap_builder.h>
 #include <axiom_soap_const.h>
 #include "_axiom_soap_fault_code.h"
 #include "_axiom_soap_fault_value.h"
 #include "_axiom_soap_fault_reason.h"
 #include "_axiom_soap_fault.h"
 #include "_axiom_soap_body.h"
 #include "_axiom_soap_fault_detail.h"
 #include <axiom_soap_fault_text.h>
 #include "_axiom_soap_fault_role.h"
 #include <axiom_stax_builder_internal.h>
 #include <axiom_node_internal.h>
 
typedef struct axiom_soap11_builder_helper_impl_t
{
    axiom_soap11_builder_helper_t builder_helper;
    
    axiom_soap_builder_t *soap_builder;
    
    axis2_bool_t fault_code_present;
    
    axis2_bool_t fault_string_present;
    
    axiom_stax_builder_t *om_builder;
    
    axiom_node_t *last_processed_node;
    
}axiom_soap11_builder_helper_impl_t;
 
/********************** Macro *************************************************/

#define AXIS2_INTF_TO_IMPL(builder_helper) \
        ((axiom_soap11_builder_helper_impl_t*)builder_helper)

/********************* function implementations *******************************/

axis2_status_t AXIS2_CALL 
axiom_soap11_builder_helper_free(axiom_soap11_builder_helper_t *builder_helper,
                                 const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axiom_soap11_builder_helper_handle_event (axiom_soap11_builder_helper_t *builder_helper,
                             const axis2_env_t *env,
                             axiom_node_t *om_element_node,
                             int element_level);
                             
static axis2_status_t 
axiom_soap11_builder_helper_process_text(axiom_soap11_builder_helper_t *builder_helper,
                                    const axis2_env_t *env);
                             
                             

AXIS2_EXTERN axiom_soap11_builder_helper_t* AXIS2_CALL
axiom_soap11_builder_helper_create(const axis2_env_t *env, 
                                   axiom_soap_builder_t *soap_builder,
                                   axiom_stax_builder_t *om_builder)
{
    axiom_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, soap_builder, NULL);
    AXIS2_PARAM_CHECK(env->error, om_builder, NULL);
    
    builder_helper_impl = (axiom_soap11_builder_helper_impl_t*)AXIS2_MALLOC(
                            env->allocator, sizeof(axiom_soap11_builder_helper_impl_t));
    if(!builder_helper_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_helper_impl->fault_code_present = AXIS2_FALSE;
    builder_helper_impl->fault_string_present = AXIS2_FALSE;
    builder_helper_impl->last_processed_node = NULL;
    builder_helper_impl->om_builder = NULL;
    builder_helper_impl->builder_helper.ops = NULL;   
    
    builder_helper_impl->builder_helper.ops = (axiom_soap11_builder_helper_ops_t*) AXIS2_MALLOC(
                                                env->allocator, sizeof(axiom_soap11_builder_helper_ops_t));
                                                
    if(!(builder_helper_impl->builder_helper.ops))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, builder_helper_impl->builder_helper.ops);
        return NULL;
    }
    builder_helper_impl->soap_builder = soap_builder;
    builder_helper_impl->om_builder = om_builder;
    
    builder_helper_impl->builder_helper.ops->free_fn = 
            axiom_soap11_builder_helper_free;
    
    builder_helper_impl->builder_helper.ops->handle_event =
            axiom_soap11_builder_helper_handle_event;
            
    return &(builder_helper_impl->builder_helper);    
} 

axis2_status_t AXIS2_CALL 
axiom_soap11_builder_helper_free(axiom_soap11_builder_helper_t *builder_helper,
                                 const axis2_env_t *env)
{
    axiom_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);

   
    if(builder_helper_impl->builder_helper.ops)
    {
        AXIS2_FREE(env->allocator, builder_helper_impl->builder_helper.ops);
        builder_helper_impl->builder_helper.ops = NULL;
    }

    AXIS2_FREE(env->allocator, builder_helper_impl);
    builder_helper_impl = NULL;    
    return AXIS2_SUCCESS;
}                                 

axis2_status_t AXIS2_CALL 
axiom_soap11_builder_helper_handle_event (axiom_soap11_builder_helper_t *builder_helper,
                             const axis2_env_t *env,
                             axiom_node_t *om_element_node,
                             int element_level)
{
    axiom_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *ele_localname = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_element_node, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);
      
    om_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(om_element_node, env);
    
    ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_ele, env);
    if(!ele_localname)
    {
        return AXIS2_FAILURE;
    }
   
    soap_envelope = AXIOM_SOAP_BUILDER_GET_SOAP_ENVELOPE(builder_helper_impl->soap_builder, env);
    
    if(!soap_envelope)
    {
        return AXIS2_FAILURE; 
    }

    soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    if(!soap_body)
    {
        return AXIS2_FAILURE;
    }

    soap_fault = AXIOM_SOAP_BODY_GET_FAULT(soap_body, env);
    if(!soap_fault)
    {
        return AXIS2_FAILURE;
    }
    
                
    if(element_level == 4)
    {
        axiom_soap_fault_code_t *fault_code = NULL;
        axiom_soap_fault_value_t *fault_value = NULL;
        axiom_node_t *fault_value_node = NULL;
        axiom_element_t *fault_value_ele  = NULL;
        
        if(AXIS2_STRCMP(ele_localname, AXIOM_SOAP11_SOAP_FAULT_CODE_LOCAL_NAME) == 0)
        {   
            axis2_status_t status = AXIS2_SUCCESS;
            
            if(builder_helper_impl->fault_string_present)
            {
                AXIOM_SOAP_BUILDER_SET_BOOL_PROCESSING_MANDATORY_FAULT_ELEMENTS(
                    builder_helper_impl->soap_builder, env, AXIS2_FALSE);
            }
            
            fault_code = axiom_soap_fault_code_create(env);
            if(!fault_code)
                return AXIS2_FAILURE;
            
            axiom_soap_fault_code_set_base_node (fault_code, env, om_element_node);
            
            axiom_soap_fault_set_code (soap_fault, env, fault_code);
            
            axiom_soap_fault_code_set_builder (fault_code, env, builder_helper_impl->soap_builder);
            
            AXIOM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIOM_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME);
                        
            fault_value = axiom_soap_fault_value_create_with_code(env, fault_code);
            if(!fault_value)
                return AXIS2_FAILURE;
            
            fault_value_node = AXIOM_SOAP_FAULT_VALUE_GET_BASE_NODE(fault_value, env);
            if(!fault_value_node)
                return AXIS2_FAILURE;
                
            fault_value_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                                fault_value_node, env);

            axiom_stax_builder_set_lastnode(builder_helper_impl->om_builder,
                                                env, fault_value_node); 
            
            status = axiom_soap11_builder_helper_process_text(builder_helper, env);
            if(status  == AXIS2_FAILURE)
            {
                return AXIS2_FAILURE;
            }
            axiom_stax_builder_set_lastnode(builder_helper_impl->om_builder,
                                                env, om_element_node); 
            axiom_node_set_complete(om_element_node, env, AXIS2_TRUE); 
            
            axiom_stax_builder_set_element_level(builder_helper_impl->om_builder, 
                    env, (element_level-1));
            builder_helper_impl->fault_code_present = AXIS2_TRUE;                        
        }
        else if(AXIS2_STRCMP(AXIOM_SOAP11_SOAP_FAULT_STRING_LOCAL_NAME, ele_localname) == 0)
        {
            
            axiom_soap_fault_reason_t *fault_reason = NULL;
            axiom_soap_fault_text_t *fault_text = NULL;
            axiom_node_t *fault_text_node = NULL;
            int status = AXIS2_SUCCESS;
            if(builder_helper_impl->fault_code_present)
            {
                AXIOM_SOAP_BUILDER_SET_BOOL_PROCESSING_MANDATORY_FAULT_ELEMENTS(
                    builder_helper_impl->soap_builder, env, AXIS2_FALSE);
            }
            
            AXIOM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIOM_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME);
            
            fault_reason = axiom_soap_fault_reason_create(env);
            if(!fault_reason)
                return AXIS2_FAILURE;
                
            axiom_soap_fault_reason_set_base_node (fault_reason, env, om_element_node);
            
            axiom_soap_fault_set_reason (soap_fault, env, fault_reason); 
            
            fault_text = axiom_soap_fault_text_create_with_parent(env, fault_reason);
            if(!fault_text)
                return AXIS2_FAILURE;
            
            fault_text_node = AXIOM_SOAP_FAULT_TEXT_GET_BASE_NODE(fault_text, env);
            if(!fault_text_node)
                return AXIS2_FAILURE;
                
            /* TODO process namespace data */
            axiom_stax_builder_set_lastnode(builder_helper_impl->om_builder, env, fault_text_node);
            
            status = axiom_soap11_builder_helper_process_text(builder_helper, env);
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
            
            axiom_stax_builder_set_lastnode(builder_helper_impl->om_builder, env, om_element_node);
            
            axiom_node_set_complete(om_element_node, env, AXIS2_TRUE);
            
            axiom_stax_builder_set_element_level(builder_helper_impl->om_builder, 
                env , (element_level -1));
         
            builder_helper_impl->fault_string_present = AXIS2_TRUE;   
            
        }else if(AXIS2_STRCMP(AXIOM_SOAP11_SOAP_FAULT_ACTOR_LOCAL_NAME, ele_localname) == 0)
        {
            axiom_soap_fault_role_t *fault_role = NULL;
            fault_role = axiom_soap_fault_role_create(env);
            if(!fault_role)
                return AXIS2_FAILURE;
            
            AXIOM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIOM_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME);

            axiom_soap_fault_role_set_base_node (fault_role, env, om_element_node);
            
            axiom_soap_fault_set_role (soap_fault, env, fault_role);
            /*
            Role element may not have a namespace associated, hence commented, else it segfaults here - Samisa
            status = AXIOM_SOAP_BUILDER_PROCESS_NAMESPACE_DATA(
                builder_helper_impl->soap_builder, env, om_element_node, AXIS2_TRUE);
            if(status == AXIS2_FAILURE)
                    return AXIS2_FAILURE;*/
        }
        else if(AXIS2_STRCMP(AXIOM_SOAP11_SOAP_FAULT_DETAIL_LOCAL_NAME, ele_localname) == 0)
        {
            axiom_soap_fault_detail_t *fault_detail = NULL;
            fault_detail = axiom_soap_fault_detail_create(env);
            if(!fault_detail)
                return AXIS2_FAILURE;
                
            AXIOM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIOM_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME);
            
            axiom_soap_fault_detail_set_base_node(fault_detail, env, om_element_node);
            
            axiom_soap_fault_set_detail (soap_fault, env, fault_detail);
        }
        else
        {
           return AXIS2_SUCCESS;
        }
    }
    else if(element_level == 5)
    {
        axiom_node_t *parent_node = NULL;
        axiom_element_t *parent_element = NULL;
        axis2_char_t *parent_localname = NULL;

        parent_node = AXIOM_NODE_GET_PARENT(om_element_node, env);
        if(!parent_node)
            return AXIS2_FAILURE;                
        parent_element = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(
                            om_element_node, env);
        parent_localname = AXIOM_ELEMENT_GET_LOCALNAME(parent_element, env);
        
        if(!parent_localname)
            return AXIS2_FAILURE;
        if(AXIS2_STRCMP(parent_localname, AXIOM_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME) == 0)
        {
            AXIS2_ERROR_SET(env->error, 
                AXIS2_ERROR_SOAP11_FAULT_ACTOR_SHOULD_NOT_HAVE_CHILD_ELEMENTS, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}                                                                                                                    
                                         
static axis2_status_t  
axiom_soap11_builder_helper_process_text(axiom_soap11_builder_helper_t *builder_helper,
                                    const axis2_env_t *env)
{
    axiom_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    int token = 0;

    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);

    token = AXIOM_STAX_BUILDER_NEXT_WITH_TOKEN(builder_helper_impl->om_builder, env);

    if(token == -1)
        return AXIS2_FAILURE;
   
    while(token != AXIS2_XML_READER_END_ELEMENT )
    {
        if(token != AXIS2_XML_READER_CHARACTER)
        {
            AXIS2_ERROR_SET(env->error, 
                AXIS2_ERROR_ONLY_CHARACTERS_ARE_ALLOWED_HERE, AXIS2_FAILURE);
            return AXIS2_FAILURE;                
        
        }
        token = AXIOM_STAX_BUILDER_NEXT_WITH_TOKEN(builder_helper_impl->om_builder, env);    
        if(token == -1)
            return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;                
}
