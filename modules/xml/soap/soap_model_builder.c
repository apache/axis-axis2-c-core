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
 
 #include <axis2_soap_model_builder.h>
 #include <axis2_soap_message.h>
 #include <axis2_soap_envelope.h>
 #include <axis2_soap_header.h>
 #include <axis2_soap11_builder_helper.h>
 #include <axis2_soap12_builder_helper.h>
 #include <axis2_soap.h>
 #include <axis2_soap_body.h>
 #include <axis2_soap_header_block.h>
 
 /*********************** impl struct *****************************************/
 
 typedef struct axis2_soap_model_builder_impl_t
 {
    axis2_soap_model_builder_t soap_builder;
    
    axis2_om_stax_builder_t *om_builder;
    
    axis2_soap_message_t *soap_message;
    
    axis2_soap_envelope_t *soap_envelope;
    
    axis2_bool_t header_present;
    
    axis2_bool_t body_present;
    
    int element_level;
    
    axis2_bool_t processing_fault;
    
    axis2_bool_t processing_detail_elements;
    
    axis2_char_t* sender_fault_code;
    
    axis2_char_t* receiver_fault_code;
    
    axis2_bool_t processing_mandatory_fault_elements;
    
    void *builder_helper;
    
    axis2_om_namespace_t *envelope_ns;
    
    int soap_version;
    
    int last_node_status;
    
    
}axis2_soap_model_builder_impl_t;

typedef enum axis2_builder_last_node_states
{
    AXIS2_BUILDER_LAST_NODE_NULL = 0,
    AXIS2_BUILDER_LAST_NODE_DONE_TRUE,
    AXIS2_BUILDER_LAST_NODE_DONE_FALSE
}axis2_builder_last_node_states;

/***************** Macro ******************************************************/

 #define AXIS2_INTF_TO_IMPL(builder) ((axis2_soap_model_builder_impl_t*)builder)
 
 /**************** function prototypes ****************************************/
 
axis2_status_t AXIS2_CALL 
axis2_soap_model_builder_free(axis2_soap_model_builder_t *builder,
                              axis2_env_t **env);

axis2_soap_envelope_t* AXIS2_CALL 
axis2_soap_model_builder_get_soap_envelope
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env);
                                
axis2_om_document_t* AXIS2_CALL 
axis2_soap_model_builder_get_document
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env);
                                
axis2_om_node_t * AXIS2_CALL
axis2_soap_model_builder_next(axis2_soap_model_builder_t *builder,
                              axis2_env_t **env);
                                    
axis2_om_node_t* AXIS2_CALL
axis2_soap_model_builder_get_document_element
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env); 
                                 
axis2_om_node_t *axis2_soap_model_builder_create_om_element
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *current_node);
                                 
axis2_status_t 
axis2_soap_model_builder_process_namespace_data
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *om_node,
                                 int is_soap_element);

static axis2_status_t
axis2_soap_model_builder_end_element(axis2_soap_model_builder_t *builder,
                                     axis2_env_t **env);
                                                                      
                                 
static void 
identify_soap_version(axis2_soap_model_builder_t *builder,
                      axis2_env_t **env,
                      axis2_char_t* soap_version_uri_from_transport);
                      
static axis2_om_node_t* 
axis2_soap_model_builder_construct_node(axis2_soap_model_builder_t *builder,
                                axis2_env_t **env,
                                axis2_om_node_t *parent,
                                axis2_om_node_t *element_node,
                                axis2_bool_t is_soap_envelope);                      


static void parse_headers();

/***************** function implementations ***********************************/

AXIS2_DECLARE(axis2_soap_model_builder_t *)
axis2_soap_model_builder_create(axis2_env_t **env,
                                axis2_om_stax_builder_t *builder)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, builder, NULL);
    
    builder_impl = (axis2_soap_model_builder_impl_t*)AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_soap_model_builder_impl_t));
    if(builder_impl == NULL)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_impl->body_present = AXIS2_FALSE;
    builder_impl->builder_helper = NULL;
    builder_impl->element_level= 0;
    builder_impl->header_present = AXIS2_FALSE;
    builder_impl->om_builder = NULL;
    builder_impl->processing_detail_elements = AXIS2_FALSE;
    builder_impl->processing_fault = AXIS2_FALSE;
    builder_impl->processing_mandatory_fault_elements = AXIS2_FALSE;
    builder_impl->receiver_fault_code = NULL;
    builder_impl->sender_fault_code = NULL;
    builder_impl->soap_builder.ops = NULL;
    builder_impl->last_node_status = -1;
    builder_impl->envelope_ns = NULL;
    
    builder_impl->soap_builder.ops = (axis2_soap_model_builder_ops_t*)
            AXIS2_MALLOC((*env)->allocator, sizeof(axis2_soap_model_builder_ops_t));
    if(!(builder_impl->soap_builder.ops))
    {
        AXIS2_FREE((*env)->allocator, builder_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_impl->om_builder = builder;
    builder_impl->soap_builder.ops->free = 
            axis2_soap_model_builder_free;
    builder_impl->soap_builder.ops->next =
            axis2_soap_model_builder_next;
    builder_impl->soap_builder.ops->get_soap_envelope =
            axis2_soap_model_builder_get_soap_envelope;
    builder_impl->soap_builder.ops->get_document =
            axis2_soap_model_builder_get_document;                                   

    return &(builder_impl->soap_builder);    
} 

axis2_status_t AXIS2_CALL 
axis2_soap_model_builder_free(axis2_soap_model_builder_t *builder,
                              axis2_env_t **env)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder->ops)
    {
        AXIS2_FREE((*env)->allocator, builder->ops);
    }
    AXIS2_FREE((*env)->allocator, builder_impl);
    return AXIS2_SUCCESS;
}
                              

axis2_soap_envelope_t* AXIS2_CALL 
axis2_soap_model_builder_get_soap_envelope
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    while(!(builder_impl->soap_envelope) || 
        !AXIS2_OM_STAX_BUILDER_IS_COMPLETE(builder_impl->om_builder, env))
    {
        axis2_soap_model_builder_next(builder, env);
    }        
    return builder_impl->soap_envelope;
}
                                
axis2_om_document_t* AXIS2_CALL 
axis2_soap_model_builder_get_document
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env)
 {
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(builder_impl->om_builder, env);
 }
                                
axis2_om_node_t * AXIS2_CALL
axis2_soap_model_builder_next(axis2_soap_model_builder_t *builder,
                              axis2_env_t **env)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    axis2_om_node_t *lastnode = NULL;
    axis2_om_node_t *current_node =  NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    lastnode = AXIS2_OM_STAX_BUILDER_GET_LAST_NODE(builder_impl->om_builder, env);
    if(!lastnode)
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_NULL;
    }else if(AXIS2_OM_NODE_GET_BUILD_STATUS(lastnode, env))
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_DONE_TRUE;
    }else
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_NULL;
    }
    
    current_node = AXIS2_OM_STAX_BUILDER_NEXT(builder_impl->om_builder, env);
    if(AXIS2_OM_STAX_BUILDER_GET_CURRENT_EVENT(builder_impl->om_builder, env) == 
            AXIS2_XML_READER_EMPTY_ELEMENT || AXIS2_XML_READER_START_ELEMENT)
    {
        axis2_soap_model_builder_create_om_element(builder, env, current_node);
        axis2_soap_model_builder_end_element(builder, env);
    
    }            
    return current_node;
}
                                    
axis2_om_node_t* AXIS2_CALL
axis2_soap_model_builder_get_document_element
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    axis2_om_document_t *om_doc = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(builder_impl->soap_envelope, env);
} 
                                       
static axis2_status_t
axis2_soap_model_builder_end_element(axis2_soap_model_builder_t *builder,
                                     axis2_env_t **env)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->element_level--;
    return AXIS2_SUCCESS;
}                                                          
                                     
axis2_om_node_t *
axis2_soap_model_builder_create_om_element
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *current_node)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK( builder, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, current_node, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->element_level++;
    if(builder_impl->last_node_status == AXIS2_BUILDER_LAST_NODE_NULL)
    {
        axis2_soap_model_builder_construct_node(builder, env, NULL, current_node, AXIS2_TRUE);
         
    }else if(builder_impl->last_node_status == AXIS2_BUILDER_LAST_NODE_DONE_TRUE)
    {
        axis2_soap_model_builder_construct_node(builder, env, 
            AXIS2_OM_NODE_GET_PARENT(current_node, env), current_node, AXIS2_FALSE);
    
    }
    else
    {
       axis2_soap_model_builder_construct_node(builder, env, 
            AXIS2_OM_NODE_GET_PARENT(current_node, env), current_node, AXIS2_FALSE);
    }
    return current_node;  
}   

static axis2_om_node_t* 
axis2_soap_model_builder_construct_node(axis2_soap_model_builder_t *builder,
                                axis2_env_t **env,
                                axis2_om_node_t *parent,
                                axis2_om_node_t *om_element_node,
                                axis2_bool_t is_soap_envelope)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    axis2_om_element_t *parent_ele  = NULL;
    axis2_char_t *parent_localname = NULL;
    axis2_char_t *element_name = NULL;
    axis2_om_element_t *om_element = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, om_element_node, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    om_element = (axis2_om_element_t *)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(om_element_node, env);
    
    if(parent)
    {
        parent_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(parent, env);
        parent_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(parent_ele, env);
    }
    if(!parent)
    {
        if(AXIS2_STRCASECMP(element_name, AXIS2_SOAP_ENVELOPE_LOCAL_NAME) == 0)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME, AXIS2_FAILURE);
            return NULL;   
        }
        builder_impl->soap_envelope = axis2_soap_envelope_create_null(env);
        AXIS2_SOAP_ENVELOPE_SET_BASE_NODE(builder_impl->soap_envelope, env, om_element_node);
        AXIS2_SOAP_ENVELOPE_SET_SOAP_VERSION(builder_impl->soap_envelope, env, builder_impl->soap_version);
        axis2_soap_model_builder_process_namespace_data(builder, env, om_element_node, AXIS2_TRUE);
    }
    else if(builder_impl->element_level == 2)
    {
        axis2_soap_header_t *soap_header = NULL;
        if(AXIS2_STRCMP(element_name, AXIS2_SOAP_HEADER_LOCAL_NAME) == 0)
        {
            if(builder_impl->header_present)
            {
                AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED, AXIS2_FAILURE);
                    return NULL;
            }
            if(builder_impl->body_present)
            {
                AXIS2_ERROR_SET((*env)->error,
                    AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER, AXIS2_FAILURE);
                return  NULL;                                              
            }
            builder_impl->header_present = AXIS2_TRUE;
            soap_header = axis2_soap_header_create(env);
            AXIS2_SOAP_HEADER_SET_BASE_NODE(soap_header, env, om_element_node);
            AXIS2_SOAP_HEADER_SET_SOAP_VERSION(soap_header, env, builder_impl->soap_version);
            AXIS2_SOAP_ENVELOPE_SET_HEADER(builder_impl->soap_envelope, env, soap_header);
            axis2_soap_model_builder_process_namespace_data(builder, env, om_element_node, AXIS2_TRUE);
        }
        else if(AXIS2_STRCMP(element_name, AXIS2_SOAP_BODY_LOCAL_NAME))
        {
            axis2_soap_body_t *soap_body = NULL;
            if(builder_impl->body_present)
            {
                AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                return NULL;                   
            
            }
            builder_impl->body_present = AXIS2_TRUE;                
            soap_body = axis2_soap_body_create(env);
            AXIS2_SOAP_BODY_SET_BASE_NODE(soap_body, env, om_element_node);
            AXIS2_SOAP_BODY_SET_SOAP_VERSION(soap_body, env, builder_impl->soap_version);
            AXIS2_SOAP_ENVELOPE_SET_BODY(builder_impl->soap_envelope, env, soap_body);
            axis2_soap_model_builder_process_namespace_data(builder, env, om_element_node, AXIS2_TRUE);
                    
        }else
        {
            AXIS2_ERROR_SET((*env)->error,
                    AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY, AXIS2_FAILURE);
            return NULL;                                            
        }
    }
    else if((builder_impl->element_level == 3) && AXIS2_STRCASECMP(parent_localname, 
            AXIS2_SOAP_HEADER_LOCAL_NAME) == 0)
    {
            axis2_soap_header_block_t *header_block = NULL;
            axis2_soap_header_t *soap_header = NULL;
            header_block = axis2_soap_header_block_create(env);
            AXIS2_SOAP_HEADER_BLOCK_SET_BASE_NODE(header_block, env, om_element_node);
            AXIS2_SOAP_HEADER_BLOCK_SET_SOAP_VERSION(header_block, env, builder_impl->soap_version);
            soap_header = AXIS2_SOAP_ENVELOPE_GET_HEADER(builder_impl->soap_envelope, env);
            AXIS2_SOAP_HEADER_SET_HEADER_BLOCK(soap_header, env, header_block);

    }
    else if((builder_impl->element_level == 3) && 
            AXIS2_STRCASECMP(parent_localname, AXIS2_SOAP_BODY_LOCAL_NAME) == 0 &&
                AXIS2_STRCASECMP(element_name, AXIS2_SOAP_BODY_FAULT_LOCAL_NAME) == 0)
    {
            axis2_soap_body_t *soap_body = NULL;
            axis2_soap_fault_t *soap_fault = NULL;
            soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(builder_impl->soap_envelope, env);
            soap_fault = axis2_soap_fault_create(env);
            AXIS2_SOAP_FAULT_SET_BASE_NODE(soap_fault, env, om_element_node);
            AXIS2_SOAP_FAULT_SET_SOAP_VERSION(soap_fault, env, builder_impl->soap_version);
            AXIS2_SOAP_BODY_ADD_FAULT(soap_body, env, soap_fault);                    
                                
            builder_impl->processing_fault = AXIS2_TRUE;
            builder_impl->processing_mandatory_fault_elements = AXIS2_TRUE; 
            
            if(AXIS2_STRCMP(AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, 
                AXIS2_OM_NAMESPACE_GET_URI(builder_impl->envelope_ns, env)) == 0)
            {
                builder_impl->builder_helper = axis2_soap12_builder_helper_create(env, builder); 
             
            }
            else if(AXIS2_STRCMP(AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, 
                AXIS2_OM_NAMESPACE_GET_URI(builder_impl->envelope_ns, env)) == 0)
            {
                builder_impl->builder_helper = axis2_soap12_builder_helper_create(env, builder);            
            }                
    }else if(builder_impl->element_level > 3 && builder_impl->processing_fault)
    {
        if(builder_impl->soap_version == AXIS2_SOAP11)
        {
             AXIS2_SOAP11_BUILDER_HELPER_HANDLE_EVENT(((axis2_soap11_builder_helper_t*)(builder_impl->builder_helper)), 
                env, builder, om_element_node , builder_impl->element_level);
        
        }
        if(builder_impl->soap_version == AXIS2_SOAP12)
        {
            AXIS2_SOAP12_BUILDER_HELPER_HANDLE_EVENT(((axis2_soap12_builder_helper_t *)(builder_impl->builder_helper)), 
                env, builder, om_element_node , builder_impl->element_level);
        }
    }
    return om_element_node;
}

axis2_status_t 
axis2_soap_model_builder_process_namespace_data
                                (axis2_soap_model_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *om_node,
                                 int is_soap_element)
{
    axis2_soap_model_builder_impl_t *builder_impl = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_om_namespace_t *om_ns = NULL;
    axis2_char_t *ns_uri = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_node, AXIS2_FAILURE);
    if(!is_soap_element)
        return AXIS2_SUCCESS;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(AXIS2_OM_NODE_GET_NODE_TYPE(om_node, env) == AXIS2_OM_ELEMENT)
    {
        om_ele = (axis2_om_element_t *) AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
        om_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env);
        ns_uri = AXIS2_OM_NAMESPACE_GET_URI(om_ns, env);
        if(om_ns && 
                AXIS2_STRCMP(ns_uri, AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) != 0 ||
                AXIS2_STRCMP(ns_uri, AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI) != 0)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI, AXIS2_FAILURE);                
            return AXIS2_FAILURE;
        }
    }    
    return AXIS2_SUCCESS;
}                                                                                                                  