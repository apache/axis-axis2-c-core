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
 
 #include <axis2_soap12_builder_helper.h>
 #include <axis2_array_list.h>
 #include <axis2_soap_body.h>
 #include <axis2_soap_fault.h>
 #include <axis2_soap_envelope.h>
 #include <axis2_soap_message.h>
 #include <axis2_soap_fault_code.h>
 #include <axis2_soap_fault_node.h>
 #include <axis2_soap_fault_detail.h>
 #include <axis2_Soap_fault_reason.h>

/********************* impl struct ********************************************/
 
typedef struct axis2_soap12_builder_helper_impl_t
{
    axis2_soap12_builder_helper_t builder_helper;
    
    axis2_soap_builder_t *soap_builder;
    
    axis2_bool_t code_present;
    
    axis2_bool_t reason_present;
    
    axis2_bool_t node_present;
    
    axis2_bool_t role_present;
    
    axis2_bool_t detail_present;
    
    axis2_bool_t subcode_value_present;
    
    axis2_bool_t value_present;
    
    axis2_bool_t sub_code_present;
    
    axis2_bool_t code_processing;
    
    axis2_bool_t sub_code_processing;
    
    axis2_bool_t reason_processing;
    
    axis2_array_list_t *detail_element_names;
    

}axis2_soap12_builder_helper_impl_t;

/******************** Macro ***************************************************/

#define AXIS2_INTF_TO_IMPL(builder_helper) ((axis2_soap12_builder_helper_impl_t*)builder_helper)

/******************** function prototypes *************************************/

axis2_status_t AXIS2_CALL 
axis2_soap12_builder_helper_free(axis2_soap12_builder_helper_t *builder_helper,
                                 axis2_env_t **env);

axis2_om_node_t* AXIS2_CALL 
axis2_soap12_builder_helper_handle_event
                            (axis2_soap12_builder_helper_t *builder_helper,
                             axis2_env_t **env,
                             axis2_soap_builder_t *soap_builder,
                             axis2_om_node_t *om_element_node,
                             int element_level);
                             
/******************** function implementations ********************************/

 AXIS2_DECLARE(axis2_soap12_builder_helper_t*)
axis2_soap12_builder_helper_create(axis2_env_t **env, 
                                   axis2_soap_builder_t *soap_builder)
{
    axis2_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, soap_builder, NULL);
    
    builder_helper_impl = (axis2_soap12_builder_helper_impl_t*)AXIS2_MALLOC(
                            (*env)->allocator, sizeof(axis2_soap12_builder_helper_impl_t));
    if(!builder_helper_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_helper_impl->code_present = AXIS2_FALSE;
    builder_helper_impl->detail_present = AXIS2_FALSE;
    builder_helper_impl->reason_present = AXIS2_FALSE;
    builder_helper_impl->role_present = AXIS2_FALSE;
    builder_helper_impl->sub_code_present = AXIS2_FALSE;
    builder_helper_impl->reason_processing = AXIS2_FALSE;
    builder_helper_impl->code_processing = AXIS2_FALSE;
    builder_helper_impl->sub_code_processing = AXIS2_FALSE;
    builder_helper_impl->detail_element_names = NULL;
    builder_helper_impl->builder_helper.ops = NULL; 
    builder_helper_impl->node_present = AXIS2_FALSE;
    builder_helper_impl->soap_builder = soap_builder;  
    
    builder_helper_impl->builder_helper.ops = (axis2_soap12_builder_helper_ops_t*) AXIS2_MALLOC(
                                                (*env)->allocator, sizeof(axis2_soap12_builder_helper_ops_t));
    if(!(builder_helper_impl->builder_helper.ops))
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE((*env)->allocator, builder_helper_impl->builder_helper.ops);
        return NULL;
    }
    
    builder_helper_impl->builder_helper.ops->free_fn = 
            axis2_soap12_builder_helper_free;
    builder_helper_impl->builder_helper.ops->handle_event =
            axis2_soap12_builder_helper_handle_event;
            
    return &(builder_helper_impl->builder_helper);    
} 

axis2_status_t AXIS2_CALL 
axis2_soap12_builder_helper_free(axis2_soap12_builder_helper_t *builder_helper,
                                 axis2_env_t **env)
{
    axis2_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder_helper, env, AXIS2_FAILURE);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);

    if(builder_helper_impl->detail_element_names)
    {
        AXIS2_ARRAY_LIST_FREE(builder_helper_impl->detail_element_names, env);
        builder_helper_impl->detail_element_names = NULL;
    }
    if(builder_helper_impl->builder_helper.ops)
    {
        AXIS2_FREE((*env)->allocator, builder_helper_impl->builder_helper.ops);
        builder_helper_impl->builder_helper.ops = NULL;
    }

    AXIS2_FREE((*env)->allocator, builder_helper_impl);
    builder_helper_impl = NULL;    
    return AXIS2_SUCCESS;
}                                 

axis2_om_node_t* AXIS2_CALL 
axis2_soap12_builder_helper_handle_event (axis2_soap12_builder_helper_t *builder_helper,
                             axis2_env_t **env,
                             axis2_soap_builder_t *soap_builder,
                             axis2_om_node_t *om_ele_node,
                             int element_level)
{
    axis2_soap12_builder_helper_impl_t *builder_helper_impl = NULL;
    axis2_char_t* ele_localname = NULL;
    axis2_om_node_t *om_node = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_soap_body_t *soap_body = NULL;
    axis2_soap_fault_t *soap_fault = NULL;
    axis2_soap_envelope_t *soap_envelope = NULL;    
    
    
    AXIS2_FUNC_PARAM_CHECK(builder_helper, env, NULL);
    builder_helper_impl = AXIS2_INTF_TO_IMPL(builder_helper);
    om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(om_ele_node, env);
    ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
    
    soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(builder_helper_impl->soap_builder, env);
    soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    soap_fault = AXIS2_SOAP_BODY_GET_FAULT(soap_body, env);   
    
    if(element_level == 4)
    {
        if(AXIS2_STRCMP(AXIS2_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME, ele_localname) == 0)
        {
            if(builder_helper_impl->code_present)
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_MULTIPLE_CODE_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                return NULL;
            }else
            {
                axis2_soap_fault_code_t* soap_fault_code = NULL;
                soap_fault_code = axis2_soap_fault_code_create(env);
                AXIS2_SOAP_FAULT_CODE_SET_BASE_NODE(soap_fault, env, om_ele_node);            
                AXIS2_SOAP_FAULT_CODE_SET_SOAP_VERSION(soap_fault_code, env, AXIS2_SOAP12);
                AXIS2_SOAP_FAULT_SET_CODE(soap_fault, env, soap_fault_code);
                builder_helper_impl->code_present = AXIS2_TRUE;
                builder_helper_impl->code_processing = AXIS2_TRUE;
            }
        }
        else if(AXIS2_STRCMP(AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME, ele_localname) == 0)
        {
            if(!(builder_helper_impl->code_processing) && !(builder_helper_impl->sub_code_processing))
            {
                if(builder_helper_impl->code_present)
                {
                    if(builder_helper_impl->reason_present)
                    {
                        AXIS2_ERROR_SET((*env)->error, 
                            AXIS2_ERROR_MULTIPLE_REASON_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                        return NULL;
                    }
                    else
                    {
                        axis2_soap_fault_reason_t *fault_reason = NULL;
                        fault_reason = axis2_soap_fault_reason_create(env);
                        AXIS2_SOAP_FAULT_REASON_SET_BASE_NODE(fault_reason, env, om_ele_node);
                        AXIS2_SOAP_FAULT_SET_SOAP_VERSION(fault_reason, env, AXIS2_SOAP12);
                        AXIS2_SOAP_FAULT_SET_REASON(soap_fault, env, fault_reason);
                        builder_helper_impl->reason_present = AXIS2_TRUE;
                        builder_helper_impl->reason_processing = AXIS2_TRUE;
                    }                
                }
                else
                {
                    AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED, AXIS2_FAILURE);
                    return NULL;                        
                }
            }
            else 
            {
                if(builder_helper_impl->code_processing)
                {
                    AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE, AXIS2_FAILURE);
                    return NULL;                            
                }
                else
                {
                    AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_SOAP_FAULT_CODE_DOES_NOT_HAVE_A_VALUE, AXIS2_FAILURE);
                    return NULL;                
                }
            }
        }
        else if(AXIS2_STRCMP(ele_localname, AXIS2_SOAP12_SOAP_FAULT_NODE_LOCAL_NAME) == 0)
        {
            if(!(builder_helper_impl->reason_processing))
            {
                if(builder_helper_impl->reason_present && 
                    !(builder_helper_impl->role_present) && 
                    !(builder_helper_impl->detail_present))
                {
                    if(builder_helper_impl->node_present)
                    {
                        AXIS2_ERROR_SET((*env)->error, 
                            AXIS2_ERROR_MULTIPLE_NODE_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                        return NULL;
                    }
                    else
                    {
                        axis2_soap_fault_node_t *soap_fault_node = NULL;
                        soap_fault_node = axis2_soap_fault_node_create(env);
                        AXIS2_SOAP_FAULT_NODE_SET_BASE_NODE(soap_fault_node, env, om_ele_node);
                        AXIS2_SOAP_FAULT_NODE_SET_SOAP_VERSION(soap_fault_node, env, AXIS2_SOAP12);
                        AXIS2_SOAP_FAULT_SET_NODE(soap_fault, env, soap_fault_node);                    
                                        
                        builder_helper_impl->node_present = AXIS2_TRUE;                    
                    }
                }else
                {
                    AXIS2_ERROR_SET((*env)->error, 
                        AXIS2_ERROR_WRONG_ELEMENT_ORDER_ENCOUNTERED, AXIS2_FALSE);
                    return NULL;                
                }
                
            }
        }        
    }
    return NULL;
}                                                                                                                    
