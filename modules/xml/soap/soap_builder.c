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
 
 #include <axis2_soap_builder.h>
 #include <axis2_soap_message.h>
 #include <axis2_soap_envelope.h>
 #include <axis2_soap_header.h>
 #include <axis2_soap11_builder_helper.h>
 #include <axis2_soap12_builder_helper.h>
 #include <axis2_soap.h>
 #include <axis2_soap_body.h>
 #include <axis2_soap_header_block.h>
 
 /*********************** impl struct *****************************************/
 
 typedef struct axis2_soap_builder_impl_t
 {
    axis2_soap_builder_t soap_builder;
    
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
    
    axis2_bool_t  done;
}axis2_soap_builder_impl_t;

typedef enum axis2_builder_last_node_states
{
    AXIS2_BUILDER_LAST_NODE_NULL = 0,
    AXIS2_BUILDER_LAST_NODE_DONE_TRUE,
    AXIS2_BUILDER_LAST_NODE_DONE_FALSE
}axis2_builder_last_node_states;

#define AXIS2_MAX_EVENT 100


/***************** Macro ******************************************************/

 #define AXIS2_INTF_TO_IMPL(builder) ((axis2_soap_builder_impl_t*)builder)
 
 /**************** function prototypes ****************************************/
 
axis2_status_t AXIS2_CALL 
axis2_soap_builder_free(axis2_soap_builder_t *builder,
                              axis2_env_t **env);

axis2_soap_envelope_t* AXIS2_CALL 
axis2_soap_builder_get_soap_envelope
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env);
                                
axis2_om_document_t* AXIS2_CALL 
axis2_soap_builder_get_document
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env);
                                
axis2_status_t AXIS2_CALL
axis2_soap_builder_next(axis2_soap_builder_t *builder,
                              axis2_env_t **env);
                                    
axis2_om_node_t* AXIS2_CALL
axis2_soap_builder_get_document_element
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env); 
                                 
axis2_status_t 
axis2_soap_builder_create_om_element
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *current_node);
                                 
axis2_status_t 
axis2_soap_builder_process_namespace_data
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *om_node,
                                 int is_soap_element);

static axis2_status_t
identify_soap_version(axis2_soap_builder_t *builder,
                      axis2_env_t **env,
                      axis2_char_t* soap_version_uri_from_transport);
                      
static axis2_status_t
axis2_soap_builder_construct_node(axis2_soap_builder_t *builder,
                                axis2_env_t **env,
                                axis2_om_node_t *parent,
                                axis2_om_node_t *element_node,
                                axis2_bool_t is_soap_envelope);                      

static axis2_status_t 
axis2_soap_builder_parse_headers(axis2_soap_builder_t *builder,
                                 axis2_env_t **env); 
              

axis2_status_t AXIS2_CALL 
axis2_soap_builder_set_bool_processing_mandatory_fault_elements
                                    (axis2_soap_builder_t *builder,
                                     axis2_env_t **env,
                                     axis2_bool_t value);
                                     
axis2_status_t AXIS2_CALL 
axis2_soap_builder_set_processing_detail_elements
                                    (axis2_soap_builder_t *builder,
                                     axis2_env_t **env,
                                     axis2_bool_t value); 
                                     
axis2_bool_t AXIS2_CALL 
axis2_soap_builder_is_processing_detail_elements
                            (axis2_soap_builder_t *builder, 
                             axis2_env_t **env); 
                             
int AXIS2_CALL
axis2_soap_builder_get_soap_version (axis2_soap_builder_t *builder, 
                             axis2_env_t **env);                                                                                                                   

/***************** function implementations ***********************************/

AXIS2_DECLARE(axis2_soap_builder_t *)
axis2_soap_builder_create(axis2_env_t **env,
                                axis2_om_stax_builder_t *builder,
                                axis2_char_t *soap_version)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, builder, NULL);
    
    builder_impl = (axis2_soap_builder_impl_t*)AXIS2_MALLOC((*env)->allocator, 
            sizeof(axis2_soap_builder_impl_t));
    if(builder_impl == NULL)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_impl->done = AXIS2_FALSE;
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
    builder_impl->soap_version = AXIS2_SOAP12;
    builder_impl->soap_builder.ops = NULL;
    builder_impl->last_node_status = -1;
    builder_impl->envelope_ns = NULL;
    builder_impl->soap_envelope = NULL;
    builder_impl->soap_message = NULL;
    builder_impl->soap_builder.ops = (axis2_soap_builder_ops_t*)
            AXIS2_MALLOC((*env)->allocator, sizeof(axis2_soap_builder_ops_t));
    if(!(builder_impl->soap_builder.ops))
    {
        AXIS2_FREE((*env)->allocator, builder_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_impl->om_builder = builder;
    builder_impl->soap_builder.ops->free = 
            axis2_soap_builder_free;
    builder_impl->soap_builder.ops->next =
            axis2_soap_builder_next;
    builder_impl->soap_builder.ops->get_soap_envelope =
            axis2_soap_builder_get_soap_envelope;
    builder_impl->soap_builder.ops->get_document =
            axis2_soap_builder_get_document;
    builder_impl->soap_builder.ops->set_bool_processing_mandatory_fault_elements =
            axis2_soap_builder_set_bool_processing_mandatory_fault_elements;                
    builder_impl->soap_builder.ops->set_processing_detail_elements =
            axis2_soap_builder_set_processing_detail_elements;
    builder_impl->soap_builder.ops->is_processing_detail_elements = 
            axis2_soap_builder_is_processing_detail_elements;            
    builder_impl->soap_builder.ops->get_soap_version =
            axis2_soap_builder_get_soap_version;
    status = identify_soap_version(&(builder_impl->soap_builder), env, soap_version);
    printf(" \nbuilder soap version %d \n", builder_impl->soap_version);
    if(status == AXIS2_FAILURE)
    {
        axis2_soap_builder_free(&(builder_impl->soap_builder), env);
        return NULL;
    }
    return &(builder_impl->soap_builder);    
} 

axis2_status_t AXIS2_CALL 
axis2_soap_builder_free(axis2_soap_builder_t *builder,
                              axis2_env_t **env)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder_impl->builder_helper)
    {
        if(builder_impl->soap_version == AXIS2_SOAP11 && builder_impl->builder_helper)
        {
            AXIS2_SOAP11_BUILDER_HELPER_FREE((axis2_soap11_builder_helper_t *)(builder_impl->builder_helper), env);
        }
        else if(builder_impl->soap_version == AXIS2_SOAP12 && builder_impl->builder_helper)
        {
            AXIS2_SOAP12_BUILDER_HELPER_FREE((axis2_soap12_builder_helper_t *)(builder_impl->builder_helper), env);
        }
    }
    if(builder->ops)
    {
        AXIS2_FREE((*env)->allocator, builder->ops);
    }
    AXIS2_FREE((*env)->allocator, builder_impl);
    return AXIS2_SUCCESS;
}
                              

axis2_soap_envelope_t* AXIS2_CALL 
axis2_soap_builder_get_soap_envelope
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    if(!(builder_impl->om_builder))
        return NULL;
    
    while(!(builder_impl->soap_envelope) && 
        !AXIS2_OM_STAX_BUILDER_IS_COMPLETE(builder_impl->om_builder, env))
    {
       status = axis2_soap_builder_next(builder, env); 
    }        
    
    return builder_impl->soap_envelope;
}
                                
axis2_om_document_t* AXIS2_CALL 
axis2_soap_builder_get_document
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env)
 {
    axis2_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return AXIS2_OM_STAX_BUILDER_GET_DOCUMENT(builder_impl->om_builder, env);
 }
                                
axis2_status_t AXIS2_CALL
axis2_soap_builder_next(axis2_soap_builder_t *builder,
                              axis2_env_t **env)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    axis2_om_node_t *lastnode = NULL;
    int current_event = AXIS2_MAX_EVENT;
    axis2_om_node_t *current_node =  NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder_impl->done)
    {
        return AXIS2_FAILURE;
    }
    
    lastnode = AXIS2_OM_STAX_BUILDER_GET_LAST_NODE(builder_impl->om_builder, env);
    
    if(!lastnode)
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_NULL;
    }/*else if(AXIS2_OM_NODE_GET_BUILD_STATUS(lastnode, env))
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_DONE_TRUE;
    }*/
    else
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_DONE_FALSE;
    }
    /*
    current_node = AXIS2_OM_STAX_BUILDER_NEXT(builder_impl->om_builder, env);
    */
    current_event = AXIS2_OM_STAX_BUILDER_NEXT_WITH_TOKEN(builder_impl->om_builder, env);
    /*
    current_event = AXIS2_OM_STAX_BUILDER_GET_CURRENT_EVENT(builder_impl->om_builder, env);
    */
    if(current_event == -1)
    {
        builder_impl->done = AXIS2_TRUE;
        return AXIS2_FAILURE;
    }
    if(current_event == AXIS2_XML_READER_EMPTY_ELEMENT ||
        current_event ==  AXIS2_XML_READER_START_ELEMENT)
    {
      current_node = AXIS2_OM_STAX_BUILDER_GET_LAST_NODE(builder_impl->om_builder, env);
    
      status = axis2_soap_builder_create_om_element(builder, env, current_node);
    }
   
    return status;
}
                                    
axis2_om_node_t* AXIS2_CALL
axis2_soap_builder_get_document_element
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(builder_impl->soap_envelope, env);
} 
                                       
                                                          
                                     
axis2_status_t
axis2_soap_builder_create_om_element
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *current_node)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    int ret_val = AXIS2_SUCCESS;
    AXIS2_FUNC_PARAM_CHECK( builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, current_node, AXIS2_FAILURE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder_impl->last_node_status == AXIS2_BUILDER_LAST_NODE_NULL)
    {
       
       ret_val =  axis2_soap_builder_construct_node(builder, env, NULL, current_node, AXIS2_TRUE);
         
    }/*else if(builder_impl->last_node_status == AXIS2_BUILDER_LAST_NODE_DONE_TRUE)
    {
      ret_val =  axis2_soap_builder_construct_node(builder, env, 
            AXIS2_OM_NODE_GET_PARENT(current_node, env), current_node, AXIS2_FALSE);
    
    }*/
    else
    {
       ret_val = axis2_soap_builder_construct_node(builder, env, 
            AXIS2_OM_NODE_GET_PARENT(current_node, env), current_node, AXIS2_FALSE);
    }
    return ret_val;
}   

static axis2_status_t
axis2_soap_builder_construct_node(axis2_soap_builder_t *builder,
                                axis2_env_t **env,
                                axis2_om_node_t *parent,
                                axis2_om_node_t *om_element_node,
                                axis2_bool_t is_soap_envelope)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    
    axis2_om_element_t *parent_ele  = NULL;
    axis2_char_t *parent_localname = NULL;
    
    axis2_om_element_t *om_element = NULL;
    axis2_char_t *ele_localname = NULL;
    
    int element_level = 0;
    int status = AXIS2_SUCCESS;
    
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_element_node, AXIS2_FAILURE);
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    
    element_level = AXIS2_OM_STAX_BUILDER_GET_ELEMENT_LEVEL(
                        builder_impl->om_builder, env);
    
    om_element = (axis2_om_element_t *)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(om_element_node, env);
    
    ele_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_element, env);
    
    
    if(parent)
    {
        parent_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(parent, env);
        parent_localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(parent_ele, env);
    }
    if(!parent && is_soap_envelope)
    {   
        int status = AXIS2_SUCCESS;
        
        if(AXIS2_STRCASECMP(ele_localname, AXIS2_SOAP_ENVELOPE_LOCAL_NAME) != 0)
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME, AXIS2_FAILURE);
            return AXIS2_FAILURE;   
        }
        builder_impl->soap_envelope = axis2_soap_envelope_create_null(env);
       
        status = AXIS2_SOAP_ENVELOPE_SET_BASE_NODE(builder_impl->soap_envelope, 
                    env, om_element_node);
        
        AXIS2_SOAP_ENVELOPE_SET_BUILDER(builder_impl->soap_envelope, env, builder);
        
        status = axis2_soap_builder_process_namespace_data(builder, env, om_element_node, AXIS2_TRUE);
    }
    else if(element_level == 2)
    {
        if(AXIS2_STRCMP(ele_localname, AXIS2_SOAP_HEADER_LOCAL_NAME) == 0)
        {   
            axis2_soap_header_t *soap_header = NULL;
            if(builder_impl->header_present)
            {
                printf("AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED");
                AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
            }
            if(builder_impl->body_present)
            {
                printf("AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER");
                AXIS2_ERROR_SET((*env)->error,
                    AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER, AXIS2_FAILURE);
                return  AXIS2_FAILURE;                                              
            }
            builder_impl->header_present = AXIS2_TRUE;
            soap_header = axis2_soap_header_create(env);
            AXIS2_SOAP_HEADER_SET_BASE_NODE(soap_header, env, om_element_node);
            AXIS2_SOAP_HEADER_SET_SOAP_VERSION(soap_header, env, builder_impl->soap_version);
            AXIS2_SOAP_ENVELOPE_SET_HEADER(builder_impl->soap_envelope, env, soap_header);
            AXIS2_SOAP_HEADER_SET_BUILDER(soap_header, env, builder);
            status = axis2_soap_builder_process_namespace_data(builder, env, 
                                om_element_node, AXIS2_TRUE);
            
        }
        else if(AXIS2_STRCMP(ele_localname, AXIS2_SOAP_BODY_LOCAL_NAME) == 0)
        {
           
            axis2_soap_body_t *soap_body = NULL;
            if(builder_impl->body_present)
            {
                printf("AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED");
                AXIS2_ERROR_SET((*env)->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                return AXIS2_FAILURE;                   
            
            }
            builder_impl->body_present = AXIS2_TRUE;                
            soap_body = axis2_soap_body_create(env);
            AXIS2_SOAP_BODY_SET_BASE_NODE(soap_body, env, om_element_node);
            AXIS2_SOAP_BODY_SET_SOAP_VERSION(soap_body, env, builder_impl->soap_version);
            AXIS2_SOAP_ENVELOPE_SET_BODY(builder_impl->soap_envelope, env, soap_body);
            status = axis2_soap_builder_process_namespace_data(builder, env, 
                        om_element_node, AXIS2_TRUE);
        }
        else
        {
            printf("AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY");
            AXIS2_ERROR_SET((*env)->error,
                    AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY, AXIS2_FAILURE);
            return AXIS2_FAILURE;                                            
        }
    }
    else if((element_level == 3) && AXIS2_STRCASECMP(parent_localname, 
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
    else if((element_level == 3) && 
            AXIS2_STRCASECMP(parent_localname, AXIS2_SOAP_BODY_LOCAL_NAME) == 0 &&
                AXIS2_STRCASECMP(ele_localname, AXIS2_SOAP_BODY_FAULT_LOCAL_NAME) == 0)
    {
            axis2_soap_body_t *soap_body = NULL;
            axis2_soap_fault_t *soap_fault = NULL;
            axis2_om_node_t *envelope_node = NULL;
            axis2_om_element_t *envelope_ele = NULL;
            axis2_om_namespace_t *env_ns = NULL;
            envelope_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(
                builder_impl->soap_envelope, env);
            envelope_ele = (axis2_om_element_t *)
                AXIS2_OM_NODE_GET_DATA_ELEMENT(envelope_node, env);
            env_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(envelope_ele, env);
                       
            soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(builder_impl->soap_envelope, env);
            soap_fault = axis2_soap_fault_create(env);
            
            AXIS2_SOAP_FAULT_SET_BASE_NODE(soap_fault, env, om_element_node);
            
            AXIS2_SOAP_FAULT_SET_SOAP_VERSION(soap_fault, env, builder_impl->soap_version);
            
            AXIS2_SOAP_BODY_ADD_FAULT(soap_body, env, soap_fault);                    
                                
            builder_impl->processing_fault = AXIS2_TRUE;
            builder_impl->processing_mandatory_fault_elements = AXIS2_TRUE; 
            if(AXIS2_STRCMP(AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, 
                AXIS2_OM_NAMESPACE_GET_URI(env_ns , env)) == 0)
            {
                builder_impl->builder_helper = axis2_soap12_builder_helper_create(env, builder); 
             
            }
            else if(AXIS2_STRCMP(AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, 
                AXIS2_OM_NAMESPACE_GET_URI(env_ns , env)) == 0)
            {   
                builder_impl->builder_helper = axis2_soap11_builder_helper_create(env, builder, builder_impl->om_builder);            
            }
             
    }
    else if(element_level > 3 && builder_impl->processing_fault)
    {
        if(builder_impl->soap_version == AXIS2_SOAP11)
        {   
             status = AXIS2_SOAP11_BUILDER_HELPER_HANDLE_EVENT(((axis2_soap11_builder_helper_t*)(builder_impl->builder_helper)), 
                env,  om_element_node , element_level);
        
        }
        if(builder_impl->soap_version == AXIS2_SOAP12)
        {
            status = AXIS2_SOAP12_BUILDER_HELPER_HANDLE_EVENT(((axis2_soap12_builder_helper_t *)
            (builder_impl->builder_helper)), env,  om_element_node , element_level);
        }
        
    }
    return status;
}

axis2_status_t 
axis2_soap_builder_process_namespace_data
                                (axis2_soap_builder_t *builder,
                                 axis2_env_t **env,
                                 axis2_om_node_t *om_node,
                                 int is_soap_element)
{
    axis2_om_element_t *om_ele = NULL;
    axis2_om_namespace_t *om_ns = NULL;
    axis2_char_t *ns_uri = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_node, AXIS2_FAILURE);
    if(!is_soap_element)
        return AXIS2_SUCCESS;
    if(AXIS2_OM_NODE_GET_NODE_TYPE(om_node, env) == AXIS2_OM_ELEMENT)
    {
        om_ele = (axis2_om_element_t *) AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
        om_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env);
        ns_uri = AXIS2_OM_NAMESPACE_GET_URI(om_ns, env);
        if(om_ns && 
                (AXIS2_STRCMP(ns_uri, AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) != 0) &&
                (AXIS2_STRCMP(ns_uri, AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI) != 0))
        {
            printf("AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI");
            
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI, AXIS2_FAILURE);                
            return AXIS2_FAILURE;
        }
    }    
    return AXIS2_SUCCESS;
}

static axis2_status_t 
identify_soap_version(axis2_soap_builder_t *builder,
                      axis2_env_t **env,
                      axis2_char_t* soap_version_uri_from_transport)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    axis2_om_namespace_t *om_ns = NULL;
    axis2_om_node_t *envelope_node = NULL;
    axis2_om_element_t *om_ele = NULL;
    axis2_char_t *ns_uri = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FAILURE);
    
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->soap_envelope = 
        axis2_soap_builder_get_soap_envelope(builder, env);
    if(builder_impl->soap_envelope == NULL)
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    envelope_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(builder_impl->soap_envelope, env);
    om_ele = (axis2_om_element_t *) AXIS2_OM_NODE_GET_DATA_ELEMENT(envelope_node, env);
    
    om_ns = AXIS2_OM_ELEMENT_GET_NAMESPACE(om_ele, env);
    
    ns_uri = AXIS2_OM_NAMESPACE_GET_URI(om_ns, env);
        
    if(soap_version_uri_from_transport && AXIS2_STRCMP(soap_version_uri_from_transport, ns_uri) != 0)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }            
    if(AXIS2_STRCMP(AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, ns_uri) == 0)
        builder_impl->soap_version = AXIS2_SOAP11;        
    else if(AXIS2_STRCMP(AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, ns_uri) == 0)
        builder_impl->soap_version = AXIS2_SOAP12;          
        
        
    AXIS2_SOAP_ENVELOPE_SET_SOAP_VERSION(builder_impl->soap_envelope, env, builder_impl->soap_version);        
    return AXIS2_SUCCESS;
} 

static axis2_status_t 
axis2_soap_builder_parse_headers(axis2_soap_builder_t *builder,
              axis2_env_t **env)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    axis2_om_node_t *om_node = NULL;
    axis2_soap_header_t *soap_header = NULL;
    int status = AXIS2_SUCCESS;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    soap_header = AXIS2_SOAP_ENVELOPE_GET_HEADER(builder_impl->soap_envelope, env);
    om_node = AXIS2_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
    
    if(soap_header)
    {
        while(!AXIS2_OM_NODE_GET_BUILD_STATUS(om_node, env))
        {
            status = axis2_soap_builder_next(builder, env);
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
            
        }
    }
    return AXIS2_SUCCESS;
} 

axis2_status_t AXIS2_CALL 
axis2_soap_builder_set_bool_processing_mandatory_fault_elements
                                    (axis2_soap_builder_t *builder,
                                     axis2_env_t **env,
                                     axis2_bool_t value)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->processing_mandatory_fault_elements = value;
    return AXIS2_SUCCESS;
}                                                                                                                                                      

axis2_status_t AXIS2_CALL 
axis2_soap_builder_set_processing_detail_elements
                                    (axis2_soap_builder_t *builder,
                                     axis2_env_t **env,
                                     axis2_bool_t value)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->processing_detail_elements = value;
    return AXIS2_SUCCESS;
}    

axis2_bool_t AXIS2_CALL 
axis2_soap_builder_is_processing_detail_elements
                            (axis2_soap_builder_t *builder, 
                             axis2_env_t **env)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->processing_detail_elements ;
}     

int AXIS2_CALL
axis2_soap_builder_get_soap_version (axis2_soap_builder_t *builder, 
                             axis2_env_t **env)
{
    axis2_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(builder, env, AXIS2_FALSE);
     builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->soap_version ;
}
