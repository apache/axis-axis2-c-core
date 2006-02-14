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
 
 #include <axis2_soap11_builder_helper.h>
 #include <axis2_om_stax_builder.h>
 #include <axis2_soap_builder.h>
 #include <axis2_soap.h>
 #include <axis2_soap_fault_code.h>
 #include <axis2_soap_fault_value.h>
 #include <axis2_soap_fault_reason.h>
 #include <axis2_soap_body.h>
 #include <axis2_soap_fault_detail.h>
 #include <axis2_soap_fault_text.h>
 #include <axis2_soap_fault_role.h>
 
 


 typedef struct axis2_soap11_builder_helper_impl_t
 {
    axis2_soap11_builder_helper_t builder_helper;
    
    axis2_soap_builder_t *soap_builder;
    
    axis2_bool_t fault_code_present;
    
    axis2_bool_t fault_string_present;
    
    axis2_om_stax_builder_t *om_builder;
    
    axis2_om_node_t *last_processed_node;
    
    
}axis2_soap11_builder_helper_impl_t;
 
 /********************* Macro *************************************************/

#define AXIS2_INTF_TO_IMPL(builder_helper) \
        ((axis2_soap11_builder_helper_impl_t*)builder_helper)

/********************* function implementations *******************************/

axis2_status_t AXIS2_CALL 
axis2_soap11_builder_helper_free(axis2_soap11_builder_helper_t *builder_helper,
                                 axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_soap11_builder_helper_handle_event (axis2_soap11_builder_helper_t *builder_helper,
                             axis2_env_t **env,
                             axis2_om_node_t *om_element_node,
                             int element_level);
                             
static axis2_status_t 
axis2_soap11_builder_helper_process_text(axis2_soap11_builder_helper_t *builder_helper,
                                    axis2_env_t **env);
                             
                             

AXIS2_DECLARE(axis2_soap11_builder_helper_t*)
axis2_soap11_builder_helper_create(axis2_env_t **env, 
                                   axis2_soap_builder_t *soap_builder,
                                   axis2_om_stax_builder_t *om_builder)
{
    axis2_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, soap_builder, NULL);
    AXIS2_PARAM_CHECK((*env)->error, om_builder, NULL);
    
    builder_helper_impl = (axis2_soap11_builder_helper_impl_t*)AXIS2_MALLOC(
                            (*env)->allocator, sizeof(axis2_soap11_builder_helper_impl_t));
    if(!builder_helper_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_helper_impl->fault_code_present = AXIS2_FALSE;
    builder_helper_impl->fault_string_present = AXIS2_FALSE;
    builder_helper_impl->last_processed_node = NULL;
    builder_helper_impl->om_builder = NULL;
    builder_helper_impl->builder_helper.ops = NULL;   
    
    builder_helper_impl->builder_helper.ops = (axis2_soap11_builder_helper_ops_t*) AXIS2_MALLOC(
                                                (*env)->allocator, sizeof(axis2_soap11_builder_helper_ops_t));
    if(!(builder_helper_impl->builder_helper.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, builder_helper_impl->builder_helper.ops);
        return NULL;
    }
    builder_helper_impl->soap_builder = soap_builder;
    builder_helper_impl->om_builder = om_builder;
    
    builder_helper_impl->builder_helper.ops->free_fn = 
            axis2_soap11_builder_helper_free;
    builder_helper_impl->builder_helper.ops->handle_event =
            axis2_soap11_builder_helper_handle_event;
            
    return &(builder_helper_impl->builder_helper);    
} 

axis2_status_t AXIS2_CALL 
axis2_soap11_builder_helper_free(axis2_soap11_builder_helper_t *builder_helper,
                                 axis2_env_t **env)
{
    axis2_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);

   
    if(builder_helper_impl->builder_helper.ops)
    {
        AXIS2_FREE((*env)->allocator, builder_helper_impl->builder_helper.ops);
        builder_helper_impl->builder_helper.ops = NULL;
    }

    AXIS2_FREE((*env)->allocator, builder_helper_impl);
    builder_helper_impl = NULL;    
    return AXIS2_SUCCESS;
}                                 

axis2_status_t AXIS2_CALL 
axis2_soap11_builder_helper_handle_event (axis2_soap11_builder_helper_t *builder_helper,
                             axis2_env_t **env,
                             axis2_om_node_t *om_element_node,
                             int element_level)
{
    axis2_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_char_t *ele_localname = NULL;
    axis2_soap_envelope_t *soap_envelope = NULL;
    axis2_soap_body_t *soap_body = NULL;
    axis2_soap_fault_t *soap_fault = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);
      
    om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(om_element_node, env);
    ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
    
    soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(builder_helper_impl->soap_builder, env);
    if(soap_envelope)
    {
        soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
        soap_fault = AXIS2_SOAP_BODY_GET_FAULT(soap_body, env);
    }
    
                
    if(element_level == 4)
    {
        axis2_soap_fault_code_t *fault_code = NULL;
        axis2_soap_fault_value_t *fault_value = NULL;
        axis2_om_node_t *fault_value_node = NULL;
        axis2_om_element_t *fault_value_ele  = NULL;
        


        if(AXIS2_STRCMP(ele_localname, AXIS2_SOAP11_SOAP_FAULT_CODE_LOCAL_NAME) == 0)
        {   
            if(builder_helper_impl->fault_string_present)
            {
                AXIS2_SOAP_BUILDER_SET_BOOL_PROCESSING_MANDATORY_FAULT_ELEMENTS(
                    builder_helper_impl->soap_builder, env, AXIS2_FALSE);
            }
            fault_code = axis2_soap_fault_code_create(env);
            AXIS2_SOAP_FAULT_CODE_SET_BASE_NODE(fault_code, env, om_element_node);
            AXIS2_SOAP_FAULT_CODE_SET_SOAP_VERSION(fault_code, env, AXIS2_SOAP11);
            AXIS2_SOAP_FAULT_SET_CODE(soap_fault, env, fault_code);
            
            AXIS2_SOAP_FAULT_CODE_SET_BUILDER(fault_code, env, builder_helper_impl->soap_builder);
            
            AXIS2_OM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIS2_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME);
                        
            fault_value = axis2_soap_fault_value_create_with_code(env, fault_code);
            
            fault_value_node = AXIS2_SOAP_FAULT_VALUE_GET_BASE_NODE(fault_value, env);
            fault_value_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                                fault_value_node, env);

            AXIS2_OM_STAX_BUILDER_SET_LAST_NODE(builder_helper_impl->om_builder,
                                                env, fault_value_node); 
            
            axis2_soap11_builder_helper_process_text(builder_helper, env);
            AXIS2_OM_STAX_BUILDER_SET_LAST_NODE(builder_helper_impl->om_builder,
                                                env, om_element_node); 
            AXIS2_OM_NODE_SET_BUILD_STATUS(om_element_node, env, AXIS2_TRUE); 
            
            AXIS2_OM_STAX_BUILDER_SET_ELEMENT_LEVEL(builder_helper_impl->om_builder, 
                    env, (element_level-1));
            builder_helper_impl->fault_code_present = AXIS2_TRUE;                        
        }
        else if(AXIS2_STRCMP(AXIS2_SOAP11_SOAP_FAULT_STRING_LOCAL_NAME, ele_localname) == 0)
        {
            
            axis2_soap_fault_reason_t *fault_reason = NULL;
            axis2_soap_fault_text_t *fault_text = NULL;
            axis2_om_node_t *fault_text_node = NULL;
            if(builder_helper_impl->fault_code_present)
            {
                AXIS2_SOAP_BUILDER_SET_BOOL_PROCESSING_MANDATORY_FAULT_ELEMENTS(
                    builder_helper_impl->soap_builder, env, AXIS2_FALSE);
            }
            AXIS2_OM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME);
            fault_reason = axis2_soap_fault_reason_create(env);
            
            AXIS2_SOAP_FAULT_REASON_SET_BASE_NODE(fault_reason, env, om_element_node);
            
            AXIS2_SOAP_FAULT_SET_REASON(soap_fault, env, fault_reason); 
            
            fault_text = axis2_soap_fault_text_create_with_parent(env, fault_reason);
            
            
            fault_text_node = AXIS2_SOAP_FAULT_TEXT_GET_BASE_NODE(fault_text, env);
            /* TODO process namespace data */
            AXIS2_OM_STAX_BUILDER_SET_LAST_NODE(builder_helper_impl->om_builder, env, fault_text_node);
            
            axis2_soap11_builder_helper_process_text(builder_helper, env);
            AXIS2_OM_STAX_BUILDER_SET_LAST_NODE(builder_helper_impl->om_builder, env, om_element_node);
            
            AXIS2_OM_NODE_SET_BUILD_STATUS(om_element_node, env, AXIS2_TRUE);
            
            AXIS2_OM_STAX_BUILDER_SET_ELEMENT_LEVEL(builder_helper_impl->om_builder, 
                env , (element_level -1));
         
            builder_helper_impl->fault_string_present = AXIS2_TRUE;   
        }else if(AXIS2_STRCMP(AXIS2_SOAP11_SOAP_FAULT_ACTOR_LOCAL_NAME, ele_localname) == 0)
        {
            axis2_soap_fault_role_t *fault_role = NULL;
            int status = AXIS2_SUCCESS;
            fault_role = axis2_soap_fault_role_create(env);
            AXIS2_OM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIS2_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME);
            AXIS2_SOAP_FAULT_ROLE_SET_BASE_NODE(fault_role, env, om_element_node);
            AXIS2_SOAP_FAULT_ROLE_SET_SOAP_VRESION(fault_role, env, AXIS2_SOAP11);
            AXIS2_SOAP_FAULT_SET_ROLE(soap_fault, env, fault_role);
            /*
            Role element may not have a namespace associated, hence commented, else it segfaults here - Samisa
            status = AXIS2_SOAP_BUILDER_PROCESS_NAMESPACE_DATA(
                builder_helper_impl->soap_builder, env, om_element_node, AXIS2_TRUE);
            if(status == AXIS2_FAILURE)
                    return AXIS2_FAILURE;*/
        }
        else if(AXIS2_STRCMP(AXIS2_SOAP11_SOAP_FAULT_DETAIL_LOCAL_NAME, ele_localname) == 0)
        {
            axis2_soap_fault_detail_t *fault_detail = NULL;
            fault_detail = axis2_soap_fault_detail_create(env);
            AXIS2_OM_ELEMENT_SET_LOCALNAME(om_ele, env, AXIS2_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME);
            AXIS2_SOAP_FAULT_DETAIL_SET_BASE_NODE(fault_detail, env, om_element_node);
            AXIS2_SOAP_FAULT_SET_DETAIL(soap_fault, env, fault_detail);
            AXIS2_SOAP_FAULT_SET_SOAP_VERSION(soap_fault, env, AXIS2_SOAP11);
        }
        else
        {
           return AXIS2_SUCCESS;
        }
    }
    else if(element_level == 5)
    {
        axis2_om_node_t *parent_node = NULL;
        axis2_om_element_t *parent_element = NULL;
        axis2_char_t *parent_localname = NULL;

        parent_node = AXIS2_OM_NODE_GET_PARENT(om_element_node, env);
        if(!parent_node)
            return AXIS2_FAILURE;                
        parent_element = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(
                            om_element_node, env);
        parent_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(parent_element, env);
        
        if(!parent_localname)
            return AXIS2_FAILURE;
        if(AXIS2_STRCMP(parent_localname, AXIS2_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME) == 0)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_SOAP11_FAULT_ACTOR_SHOULD_NOT_HAVE_CHILD_ELEMENTS, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}                                                                                                                    
                                         
static axis2_status_t  
axis2_soap11_builder_helper_process_text(axis2_soap11_builder_helper_t *builder_helper,
                                    axis2_env_t **env)
{
    axis2_soap11_builder_helper_impl_t *builder_helper_impl = NULL;
    int token = 0;
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);
    token = AXIS2_OM_STAX_BUILDER_NEXT_WITH_TOKEN(builder_helper_impl->om_builder, env);
   
   /* while(!AXIS2_OM_NODE_GET_BUILD_STATUS(last_node, env))
    {
        
        token = AXIS2_OM_STAX_BUILDER_NEXT_WITH_TOKEN(builder_helper_impl->om_builder, env);
        if(token
        if(token != AXIS2_XML_READER_CHARACTER)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_ONLY_CHARACTERS_ARE_ALLOWED_HERE, AXIS2_FAILURE);
            return AXIS2_FAILURE;                
        }
    }
    */
    while(token != AXIS2_XML_READER_END_ELEMENT)
    {
        if(token != AXIS2_XML_READER_CHARACTER)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_ONLY_CHARACTERS_ARE_ALLOWED_HERE, AXIS2_FAILURE);
            return AXIS2_FAILURE;                
        
        }
        token = AXIS2_OM_STAX_BUILDER_NEXT_WITH_TOKEN(builder_helper_impl->om_builder, env);    
    }
    return AXIS2_SUCCESS;                
}
