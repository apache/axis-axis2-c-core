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
 
 #include <axiom_soap_builder.h>
 #include <axiom_soap_message.h>
 #include "_axiom_soap_envelope.h"
 #include "_axiom_soap_header.h"
 #include "axiom_soap11_builder_helper.h"
 #include "axiom_soap12_builder_helper.h"
 #include <axiom_soap_const.h>
 #include "_axiom_soap_body.h"
 #include "_axiom_soap_header_block.h"
 #include <axiom_stax_builder_internal.h>
 #include "_axiom_soap_fault.h"
 /*********************** impl struct *****************************************/
 
 typedef struct axiom_soap_builder_impl_t
 {
    axiom_soap_builder_t soap_builder;
    
    axiom_stax_builder_t *om_builder;
    
    axiom_soap_message_t *soap_message;
    
    axiom_soap_envelope_t *soap_envelope;
    
    axis2_bool_t header_present;
    
    axis2_bool_t body_present;
    
    int element_level;
    
    axis2_bool_t processing_fault;
    
    axis2_bool_t processing_detail_elements;
    
    axis2_char_t* sender_fault_code;
    
    axis2_char_t* receiver_fault_code;
    
    axis2_bool_t processing_mandatory_fault_elements;
    
    void *builder_helper;
    
    axiom_namespace_t *envelope_ns;
    
    int soap_version;
    
    int last_node_status;
    
    axis2_bool_t  done;

    axis2_hash_t *mime_body_parts;
    
}axiom_soap_builder_impl_t;

typedef enum axis2_builder_last_node_states
{
    AXIS2_BUILDER_LAST_NODE_NULL = 0,
    AXIS2_BUILDER_LAST_NODE_NOT_NULL
}axis2_builder_last_node_states;

#define AXIS2_MAX_EVENT 100


/***************** Macro ******************************************************/

 #define AXIS2_INTF_TO_IMPL(builder) ((axiom_soap_builder_impl_t*)builder)
 
 /**************** function prototypes ****************************************/
 
axis2_status_t AXIS2_CALL 
axiom_soap_builder_free(axiom_soap_builder_t *builder,
                              const axis2_env_t *env);

axiom_soap_envelope_t* AXIS2_CALL 
axiom_soap_builder_get_soap_envelope
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env);
                                
axiom_document_t* AXIS2_CALL 
axiom_soap_builder_get_document
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env);
                                
axis2_status_t AXIS2_CALL
axiom_soap_builder_next(axiom_soap_builder_t *builder,
                              const axis2_env_t *env);
                                    
axiom_node_t* AXIS2_CALL
axiom_soap_builder_get_document_element
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env); 
                                 
axis2_status_t 
axiom_soap_builder_create_om_element
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env,
                                 axiom_node_t *current_node,
                                 int current_event);
                                 
axis2_status_t AXIS2_CALL
axiom_soap_builder_process_namespace_data
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env,
                                 axiom_node_t *om_node,
                                 int is_soap_element);

static axis2_status_t
axiom_soap_builder_identify_soap_version(axiom_soap_builder_t *builder,
                      const axis2_env_t *env,
                      const axis2_char_t* soap_version_uri_from_transport);
                      
static axis2_status_t
axiom_soap_builder_construct_node(axiom_soap_builder_t *builder,
                                const axis2_env_t *env,
                                axiom_node_t *parent,
                                axiom_node_t *element_node,
                                axis2_bool_t is_soap_envelope);

static axis2_status_t
axiom_soap_builder_construct_node_for_empty_element
                               (axiom_soap_builder_t *builder,
                                const axis2_env_t *env,
                                axiom_node_t *parent,
                                axiom_node_t *element_node);

static axis2_status_t 
axiom_soap_builder_parse_headers(axiom_soap_builder_t *builder,
                                 const axis2_env_t *env); 
              

axis2_status_t AXIS2_CALL 
axiom_soap_builder_set_bool_processing_mandatory_fault_elements
                                    (axiom_soap_builder_t *builder,
                                     const axis2_env_t *env,
                                     axis2_bool_t value);
                                     
axis2_status_t AXIS2_CALL 
axiom_soap_builder_set_processing_detail_elements
                                    (axiom_soap_builder_t *builder,
                                     const axis2_env_t *env,
                                     axis2_bool_t value); 
                                     
axis2_bool_t AXIS2_CALL 
axiom_soap_builder_is_processing_detail_elements
                            (axiom_soap_builder_t *builder, 
                             const axis2_env_t *env); 
                             
int AXIS2_CALL
axiom_soap_builder_get_soap_version (axiom_soap_builder_t *builder, 
                             const axis2_env_t *env);                                                                                                                   
axis2_status_t AXIS2_CALL 
axiom_soap_builder_set_mime_body_parts
                            (axiom_soap_builder_t *builder, 
                             const axis2_env_t *env,
                             axis2_hash_t *map); 
/***************** function implementations ***********************************/

AXIS2_EXTERN axiom_soap_builder_t * AXIS2_CALL
axiom_soap_builder_create(const axis2_env_t *env,
                                axiom_stax_builder_t *builder,
                                const axis2_char_t *soap_version)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, builder, NULL);
    
    builder_impl = (axiom_soap_builder_impl_t*)AXIS2_MALLOC(env->allocator, 
            sizeof(axiom_soap_builder_impl_t));
    if(builder_impl == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
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
    builder_impl->soap_version = AXIOM_SOAP12;
    builder_impl->soap_builder.ops = NULL;
    builder_impl->last_node_status = -1;
    builder_impl->envelope_ns = NULL;
    builder_impl->soap_envelope = NULL;
    builder_impl->soap_message = NULL;
    builder_impl->mime_body_parts = NULL;

    builder_impl->soap_builder.ops = (axiom_soap_builder_ops_t*)
            AXIS2_MALLOC(env->allocator, sizeof(axiom_soap_builder_ops_t));
    
    if(!(builder_impl->soap_builder.ops))
    {
        AXIS2_FREE(env->allocator, builder_impl);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    builder_impl->om_builder = builder;
    builder_impl->soap_builder.ops->free = 
            axiom_soap_builder_free;
    builder_impl->soap_builder.ops->next =
            axiom_soap_builder_next;
    builder_impl->soap_builder.ops->get_soap_envelope =
            axiom_soap_builder_get_soap_envelope;
    builder_impl->soap_builder.ops->get_document =
            axiom_soap_builder_get_document;
    builder_impl->soap_builder.ops->set_bool_processing_mandatory_fault_elements =
            axiom_soap_builder_set_bool_processing_mandatory_fault_elements;                
    builder_impl->soap_builder.ops->set_processing_detail_elements =
            axiom_soap_builder_set_processing_detail_elements;
    builder_impl->soap_builder.ops->is_processing_detail_elements = 
            axiom_soap_builder_is_processing_detail_elements;            
    builder_impl->soap_builder.ops->get_soap_version =
            axiom_soap_builder_get_soap_version;
    builder_impl->soap_builder.ops->process_namespace_data =
            axiom_soap_builder_process_namespace_data;
    builder_impl->soap_builder.ops->set_mime_body_parts =
            axiom_soap_builder_set_mime_body_parts;
    
    status = axiom_soap_builder_identify_soap_version(&(builder_impl->soap_builder), env, soap_version);
    if(status == AXIS2_FAILURE)
    {
        axiom_soap_builder_free(&(builder_impl->soap_builder), env);
        return NULL;
    }
    status = axiom_soap_builder_parse_headers(&(builder_impl->soap_builder), env);
    if(status == AXIS2_FAILURE)
    {
        axiom_soap_builder_free(&(builder_impl->soap_builder), env);
        return NULL;
    }
    return &(builder_impl->soap_builder);    
} 

axis2_status_t AXIS2_CALL 
axiom_soap_builder_free(axiom_soap_builder_t *builder,
                              const axis2_env_t *env)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!builder)
        return AXIS2_FAILURE;
        
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(NULL != builder_impl->builder_helper)
    {
        if(builder_impl->soap_version == AXIOM_SOAP11 && builder_impl->builder_helper)
        {
            AXIOM_SOAP11_BUILDER_HELPER_FREE((axiom_soap11_builder_helper_t *)(builder_impl->builder_helper), env);
            builder_impl->builder_helper = NULL;
        }
        else if(builder_impl->soap_version == AXIOM_SOAP12 && builder_impl->builder_helper)
        {
            AXIOM_SOAP12_BUILDER_HELPER_FREE((axiom_soap12_builder_helper_t *)(builder_impl->builder_helper), env);
            builder_impl->builder_helper = NULL;
        }
    }
    if(NULL != builder_impl->om_builder)
    {
        AXIOM_STAX_BUILDER_FREE(builder_impl->om_builder, env);
        builder_impl->om_builder = NULL;
    }
    if(NULL != builder->ops)
    {
        AXIS2_FREE(env->allocator, builder->ops);
        builder->ops = NULL;
    }
    AXIS2_FREE(env->allocator, builder_impl);
    return AXIS2_SUCCESS;
}
                              

axiom_soap_envelope_t* AXIS2_CALL 
axiom_soap_builder_get_soap_envelope
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    int status = AXIS2_SUCCESS ;
    
    AXIS2_ENV_CHECK(env, NULL);
    if(!builder)
        return NULL;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    if(!(builder_impl->om_builder))
        return NULL;
    
    while(!(builder_impl->soap_envelope) && 
        !AXIOM_STAX_BUILDER_IS_COMPLETE(builder_impl->om_builder, env))
    {
       status = axiom_soap_builder_next(builder, env); 
            if(status == AXIS2_FAILURE)
                  break;
    }        
    
    return builder_impl->soap_envelope;
}
                                
axiom_document_t* AXIS2_CALL 
axiom_soap_builder_get_document
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env)
 {
    axiom_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    if(!builder)
        return NULL;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder_impl->om_builder)
    {
        return AXIOM_STAX_BUILDER_GET_DOCUMENT(builder_impl->om_builder, env);
    }
    else
        return NULL;
 }
                                
axis2_status_t AXIS2_CALL
axiom_soap_builder_next(axiom_soap_builder_t *builder,
                              const axis2_env_t *env)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    axiom_node_t *lastnode = NULL;
    int current_event = AXIS2_MAX_EVENT;
    axiom_node_t *current_node =  NULL;
    int status = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if(!builder)
        return AXIS2_FAILURE;
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder_impl->done)
    {
        return AXIS2_FAILURE;   
    }
    if(!(builder_impl->om_builder))
        return AXIS2_FAILURE;
   
    lastnode = axiom_stax_builder_get_lastnode(builder_impl->om_builder, env);
    
    if(!lastnode)
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_NULL;
    }
    else
    {
        builder_impl->last_node_status = AXIS2_BUILDER_LAST_NODE_NOT_NULL;
    }
    current_event = AXIOM_STAX_BUILDER_NEXT_WITH_TOKEN(builder_impl->om_builder, env);
    if(current_event == -1)
    {
        builder_impl->done = AXIS2_TRUE;
        return AXIS2_FAILURE;
    }
    if(current_event == AXIOM_READER_EMPTY_ELEMENT ||
        current_event ==  AXIOM_READER_START_ELEMENT)
    {
      current_node = axiom_stax_builder_get_lastnode(builder_impl->om_builder, env);
      if(current_node) 
          status = axiom_soap_builder_create_om_element(builder, env, current_node, current_event);
      else
        return AXIS2_FAILURE;
    }
   
    return status;
}
                                    
axiom_node_t* AXIS2_CALL
axiom_soap_builder_get_document_element
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder_impl->soap_envelope)
    {
        return AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(builder_impl->soap_envelope, env);
    }
    else
        return NULL;
} 
                                       
                                                          
                                     
axis2_status_t
axiom_soap_builder_create_om_element
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env,
                                 axiom_node_t *current_node,
                                 int current_event)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    int ret_val = AXIS2_SUCCESS;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, current_node, AXIS2_FAILURE);
    if(!builder)
        return AXIS2_FAILURE;
        
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(builder_impl->last_node_status == AXIS2_BUILDER_LAST_NODE_NULL)
    {
       ret_val =  axiom_soap_builder_construct_node(builder, env, NULL, current_node, AXIS2_TRUE);
    }
    else
    {
       int element_level = 0;
       axiom_node_t *parent_node = NULL;
       parent_node = AXIOM_NODE_GET_PARENT(current_node, env);
       if(!builder_impl->om_builder)
           return  AXIS2_FAILURE;
       
       element_level = axiom_stax_builder_get_element_level(
                        builder_impl->om_builder, env);
       if(parent_node && element_level == 1 && current_event == AXIOM_READER_EMPTY_ELEMENT)
       {
           ret_val = axiom_soap_builder_construct_node_for_empty_element(builder, env, parent_node , current_node);
       }
       else if(parent_node)
       {
           ret_val = axiom_soap_builder_construct_node(builder, env, parent_node , current_node, AXIS2_FALSE);
       }
       else
            return AXIS2_FAILURE;
    }
    return ret_val;
}   

static axis2_status_t
axiom_soap_builder_construct_node(axiom_soap_builder_t *builder,
                                const axis2_env_t *env,
                                axiom_node_t *parent,
                                axiom_node_t *om_element_node,
                                axis2_bool_t is_soap_envelope)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    
    axiom_element_t *parent_ele  = NULL;
    axis2_char_t *parent_localname = NULL;
    
    axiom_element_t *om_element = NULL;
    const axis2_char_t *ele_localname = NULL;
    
    int element_level = 0;
    int status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_element_node, AXIS2_FAILURE);
    if(!builder)
        return AXIS2_FAILURE;
   
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(!builder_impl->om_builder)
        return AXIS2_FAILURE;
    /** get element level of this om element */
    element_level = axiom_stax_builder_get_element_level(
                        builder_impl->om_builder, env);
    /* get om element struct from node */
    om_element = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(om_element_node, env);
    if(!om_element)
        return AXIS2_FAILURE;
    /* get element localname */    
    ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_element, env);
    if(!ele_localname)
        return AXIS2_FAILURE;
   
    /* start: handle MTOM stuff */
    if (AXIS2_STRCMP(ele_localname, AXIS2_XOP_INCLUDE) == 0)
    {
        axiom_namespace_t *ns = NULL;
        ns = AXIOM_ELEMENT_GET_NAMESPACE(om_element, env, om_element_node);
        if (ns)
        {
            axis2_char_t *uri = AXIOM_NAMESPACE_GET_URI(ns, env);
            if (uri)
            {
                if (AXIS2_STRCMP(uri, AXIS2_XOP_NAMESPACE_URI) == 0)
                {
                    axis2_qname_t *qname = NULL;
                    qname = axis2_qname_create(env, "href", NULL, NULL);
                    if (qname)
                    {
                        axis2_char_t *id = NULL;
                        id = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE(om_element, env, qname);                        
                        if (id)
                        {
                            axis2_char_t *pos = NULL;
                            pos = AXIS2_STRSTR(id, "cid:");
                            if (pos)
                            {
                                axiom_data_handler_t *data_handler = NULL;
                                id += 4;
                                if (builder_impl->mime_body_parts)
                                {
                                    data_handler = (axiom_data_handler_t *)axis2_hash_get(
                                        builder_impl->mime_body_parts, 
                                        (void*)id, AXIS2_HASH_KEY_STRING);
                                    if (data_handler)
                                    {
                                        axiom_text_t *data_text = NULL;
                                        axiom_node_t *data_om_node = NULL;
                                        data_text = axiom_text_create_with_data_handler(
                                            env, om_element_node, data_handler, &data_om_node);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    /* end: handle MTOM stuff */
   
    if(parent)
    {
        /** a parent node exist , so not soap envelope element */
        parent_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(parent, env);
        if(parent_ele)
            parent_localname = AXIOM_ELEMENT_GET_LOCALNAME(parent_ele, env);
    }
    if(!parent && is_soap_envelope)
    {   
        /** this is the soap envelope element */
        if(AXIS2_STRCASECMP(ele_localname, AXIOM_SOAP_ENVELOPE_LOCAL_NAME) != 0)
        {
            AXIS2_ERROR_SET(env->error, 
                AXIS2_ERROR_SOAP_MESSAGE_FIRST_ELEMENT_MUST_CONTAIN_LOCAL_NAME, AXIS2_FAILURE);
            return AXIS2_FAILURE;   
        }
        
        /** create a null soap envelope struct */
        builder_impl->soap_envelope = axiom_soap_envelope_create_null(env);
        if(!builder_impl->soap_envelope)
            return AXIS2_FAILURE;
        /** wrap this om node in it */
        status = axiom_soap_envelope_set_base_node(builder_impl->soap_envelope, 
                    env, om_element_node);
        
        axiom_soap_envelope_set_builder(builder_impl->soap_envelope, env, builder);
        status = axiom_soap_builder_process_namespace_data(builder, env, om_element_node, AXIS2_TRUE);
        if(status == AXIS2_FAILURE)
            return AXIS2_FAILURE;
    }
    else if(element_level == 2)
    {
        if(AXIS2_STRCMP(ele_localname, AXIOM_SOAP_HEADER_LOCAL_NAME) == 0)
        {  
            /** this is the soap header element */
            axiom_soap_header_t *soap_header = NULL;
            if(builder_impl->header_present)
            {
                AXIS2_ERROR_SET(env->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
            }
            if(builder_impl->body_present)
            {
                AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER, AXIS2_FAILURE);
                return  AXIS2_FAILURE;                                              
            }
            
            builder_impl->header_present = AXIS2_TRUE;
            
            soap_header = axiom_soap_header_create(env);
            if(!soap_header)
                return AXIS2_FAILURE;
                
            axiom_soap_header_set_base_node (soap_header, env, om_element_node);

            axiom_soap_envelope_set_header(builder_impl->soap_envelope, env, soap_header);
            
            axiom_soap_header_set_builder (soap_header, env, builder);
           
            axiom_soap_header_set_soap_version (soap_header, env, builder_impl->soap_version);
           
            status = axiom_soap_builder_process_namespace_data(builder, env, 
                                om_element_node, AXIS2_TRUE);
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
            
        }
        else if(AXIS2_STRCMP(ele_localname, AXIOM_SOAP_BODY_LOCAL_NAME) == 0)
        {
           
            axiom_soap_body_t *soap_body = NULL;
            if(builder_impl->body_present)
            {
                AXIS2_ERROR_SET(env->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                return AXIS2_FAILURE;                   
            
            }
            builder_impl->body_present = AXIS2_TRUE;                
            
            soap_body = axiom_soap_body_create(env);
            if(!soap_body)
                return AXIS2_FAILURE;
                
            axiom_soap_body_set_base_node (soap_body, env, om_element_node);
            
            axiom_soap_body_set_builder (soap_body, env, builder);
            
            axiom_soap_envelope_set_body(builder_impl->soap_envelope, env, soap_body);
            
            status = axiom_soap_builder_process_namespace_data(builder, env, 
                        om_element_node, AXIS2_TRUE);
            
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY, AXIS2_FAILURE);
            return AXIS2_FAILURE;                                            
        }
    }
    else if((element_level == 3) &&  parent_localname &&  
                AXIS2_STRCASECMP(parent_localname, AXIOM_SOAP_HEADER_LOCAL_NAME) == 0)
    {
            axiom_soap_header_block_t *header_block = NULL;
            axiom_soap_header_t *soap_header = NULL;
            
            soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(builder_impl->soap_envelope, env);
            if(!soap_header)
                return AXIS2_FAILURE;
                
            header_block = axiom_soap_header_block_create(env);
            if(!header_block)
                return AXIS2_FAILURE;
                
            axiom_soap_header_block_set_base_node (header_block, env, om_element_node);
           
            axiom_soap_header_set_header_block (soap_header, env, header_block);

            axiom_soap_header_block_set_soap_version (header_block, env, builder_impl->soap_version);
            

    }
    else if((element_level == 3) && parent_localname &&
            AXIS2_STRCASECMP(parent_localname, AXIOM_SOAP_BODY_LOCAL_NAME) == 0 &&
                AXIS2_STRCASECMP(ele_localname, AXIOM_SOAP_BODY_FAULT_LOCAL_NAME) == 0)
    {
            axiom_soap_body_t *soap_body = NULL;
            axiom_soap_fault_t *soap_fault = NULL;
            axiom_namespace_t *env_ns = NULL;
            
            env_ns = AXIOM_SOAP_ENVELOPE_GET_NAMESPACE(builder_impl->soap_envelope, env);
            if(!env_ns)
                return AXIS2_FAILURE;
           
            soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(builder_impl->soap_envelope, env);
            
            if(!soap_body )
                return AXIS2_FAILURE;
            
            soap_fault = axiom_soap_fault_create(env);
            if(!soap_fault) 
                return AXIS2_FAILURE;
            
            axiom_soap_fault_set_base_node (soap_fault, env, om_element_node);
            
            axiom_soap_body_set_fault (soap_body, env, soap_fault);                    
                                
            builder_impl->processing_fault = AXIS2_TRUE;
            
            builder_impl->processing_mandatory_fault_elements = AXIS2_TRUE; 
            
            if(AXIS2_STRCMP(AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, 
                AXIOM_NAMESPACE_GET_URI(env_ns , env)) == 0)
            {
                builder_impl->builder_helper = axiom_soap12_builder_helper_create(env, builder);
                if(!(builder_impl->builder_helper))
                    return AXIS2_FAILURE;
            }
            else if(AXIS2_STRCMP(AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, 
                AXIOM_NAMESPACE_GET_URI(env_ns , env)) == 0)
            {   
                builder_impl->builder_helper = axiom_soap11_builder_helper_create(env, builder, builder_impl->om_builder);                 if(!(builder_impl->builder_helper))
                    return AXIS2_FAILURE;
            }
             
    }
    else if(element_level > 3 && builder_impl->processing_fault)
    {
        if(builder_impl->soap_version == AXIOM_SOAP11)
        {   
            status = AXIOM_SOAP11_BUILDER_HELPER_HANDLE_EVENT(
                ((axiom_soap11_builder_helper_t*)(builder_impl->builder_helper)),
                env,  om_element_node , element_level);
        
        }
        else if(builder_impl->soap_version == AXIOM_SOAP12)
        {
            status = AXIOM_SOAP12_BUILDER_HELPER_HANDLE_EVENT(
                ((axiom_soap12_builder_helper_t *)(builder_impl->builder_helper)), 
                env,  om_element_node , element_level);
        }
        
    }
    return status;
}

axis2_status_t AXIS2_CALL 
axiom_soap_builder_process_namespace_data
                                (axiom_soap_builder_t *builder,
                                 const axis2_env_t *env,
                                 axiom_node_t *om_node,
                                 int is_soap_element)
{
    axiom_element_t *om_ele = NULL;
    axiom_namespace_t *om_ns = NULL;
    axis2_char_t *ns_uri = NULL;
  
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if(!om_node)
        return AXIS2_FAILURE;
        
    if(!is_soap_element)
        return AXIS2_SUCCESS;
    
    if(AXIOM_NODE_GET_NODE_TYPE(om_node, env) == AXIOM_ELEMENT)
    {
        om_ele = (axiom_element_t *) AXIOM_NODE_GET_DATA_ELEMENT(om_node, env);
        if(om_ele)
        {
            om_ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, om_node);
            if(om_ns)
            {
                ns_uri = AXIOM_NAMESPACE_GET_URI(om_ns, env);
                if(ns_uri && 
                    (AXIS2_STRCMP(ns_uri, AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI) != 0) &&
                    (AXIS2_STRCMP(ns_uri, AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI) != 0))
                {
                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI, AXIS2_FAILURE);                
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "AXIS2_ERROR_INVALID_SOAP_NAMESPACE_URI");
                    return AXIS2_FAILURE;
                }
                
            }
        }
    }        
    return AXIS2_SUCCESS;
}

static axis2_status_t 
axiom_soap_builder_identify_soap_version(axiom_soap_builder_t *builder,
                      const axis2_env_t *env,
                      const axis2_char_t* soap_version_uri_from_transport)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    axiom_namespace_t *om_ns = NULL;
    axiom_node_t *envelope_node = NULL;
    axiom_element_t *om_ele = NULL;
    axis2_char_t *ns_uri = NULL;
    
    AXIS2_PARAM_CHECK(env->error, soap_version_uri_from_transport, AXIS2_FAILURE);    
    if(!builder)
        return AXIS2_FAILURE;
    
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    builder_impl->soap_envelope = axiom_soap_builder_get_soap_envelope(builder, env);
    
    if(builder_impl->soap_envelope == NULL)
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_SOAP_MESSAGE_DOES_NOT_CONTAIN_AN_ENVELOPE, AXIS2_FAILURE);
        AXIS2_LOG_CRITICAL(env->log, AXIS2_LOG_SI,
            "SOAP message does not have a SOAP envelope element ");        
        return AXIS2_FAILURE;
    }
    
    envelope_node = AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(builder_impl->soap_envelope, env);
    
    if(!envelope_node)
        return AXIS2_FAILURE;
        
    om_ele = (axiom_element_t *) AXIOM_NODE_GET_DATA_ELEMENT(envelope_node, env);
    if(!om_ele)
        return AXIS2_FAILURE;
        
    om_ns = AXIOM_ELEMENT_GET_NAMESPACE(om_ele, env, envelope_node);
    if(!om_ns)
        return AXIS2_FAILURE;
        
    ns_uri = AXIOM_NAMESPACE_GET_URI(om_ns, env);
    
    if(ns_uri != NULL)
    {        
       if(soap_version_uri_from_transport && AXIS2_STRCMP(soap_version_uri_from_transport, ns_uri) != 0)
        {
            AXIS2_ERROR_SET(env->error, 
                AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP, AXIS2_FAILURE);
            
            AXIS2_LOG_ERROR(env->log , AXIS2_LOG_SI, 
                "AXIS2_ERROR_TRANSPORT_LEVEL_INFORMATION_DOES_NOT_MATCH_WITH_SOAP");            
            return AXIS2_FAILURE;
        }            
        if(AXIS2_STRCMP(AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, ns_uri) == 0)
        {
            builder_impl->soap_version = AXIOM_SOAP11;        
      
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Identified soap version is soap11");

            axiom_soap_envelope_set_soap_version_internal(builder_impl->soap_envelope, 
                env, builder_impl->soap_version);        

            return AXIS2_SUCCESS;
        }
        else if(AXIS2_STRCMP(AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, ns_uri) == 0)
        {
            builder_impl->soap_version = AXIOM_SOAP12;          
        
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "identified soap version is soap12");
            
            axiom_soap_envelope_set_soap_version_internal(builder_impl->soap_envelope, 
                env, builder_impl->soap_version);        

            return AXIS2_SUCCESS;                
        }
    }
    return AXIS2_FAILURE;
} 

static axis2_status_t 
axiom_soap_builder_parse_headers(axiom_soap_builder_t *builder,
              const axis2_env_t *env)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    axiom_node_t *om_node = NULL;
    axiom_soap_header_t *soap_header = NULL;
    int status = AXIS2_SUCCESS;
    if(!builder)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    
    if(!builder_impl->soap_envelope)
        return AXIS2_FAILURE;
        
    soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(builder_impl->soap_envelope, env);
    
    if(soap_header != NULL)
    {
        om_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
        if(om_node)
        {        while(!AXIOM_NODE_IS_COMPLETE(om_node, env))
                {
                    status = axiom_soap_builder_next(builder, env);
                    if(status == AXIS2_FAILURE)
                        return AXIS2_FAILURE;
                }
                /*HACK: to fix AXIS2C-129 - Samisa*/
                /*
                axiom_stax_builder_set_element_level(
                        builder_impl->om_builder, env, 1);
                */                        
        }                
    }
    return AXIS2_SUCCESS;
} 

axis2_status_t AXIS2_CALL 
axiom_soap_builder_set_bool_processing_mandatory_fault_elements
                                    (axiom_soap_builder_t *builder,
                                     const axis2_env_t *env,
                                     axis2_bool_t value)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->processing_mandatory_fault_elements = value;
    return AXIS2_SUCCESS;
}                                                                                                                                                      

axis2_status_t AXIS2_CALL 
axiom_soap_builder_set_processing_detail_elements
                                    (axiom_soap_builder_t *builder,
                                     const axis2_env_t *env,
                                     axis2_bool_t value)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    builder_impl->processing_detail_elements = value;
    return AXIS2_SUCCESS;
}    

axis2_bool_t AXIS2_CALL 
axiom_soap_builder_is_processing_detail_elements
                            (axiom_soap_builder_t *builder, 
                             const axis2_env_t *env)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->processing_detail_elements ;
}     

int AXIS2_CALL
axiom_soap_builder_get_soap_version (axiom_soap_builder_t *builder, 
                             const axis2_env_t *env)
{
    axiom_soap_builder_impl_t *builder_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
     builder_impl = AXIS2_INTF_TO_IMPL(builder);
    return builder_impl->soap_version ;
}

axis2_status_t AXIS2_CALL 
axiom_soap_builder_set_mime_body_parts
                            (axiom_soap_builder_t *builder, 
                             const axis2_env_t *env,
                             axis2_hash_t *map)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(builder)->mime_body_parts = map;
    return AXIS2_SUCCESS;
}

static axis2_status_t
axiom_soap_builder_construct_node_for_empty_element
                               (axiom_soap_builder_t *builder,
                                const axis2_env_t *env,
                                axiom_node_t *parent,
                                axiom_node_t *om_element_node)
{
    
    axiom_soap_builder_impl_t *builder_impl = NULL;
    
    axiom_element_t *parent_ele  = NULL;
    axis2_char_t *parent_localname = NULL;
    
    axiom_element_t *om_element = NULL;
    axis2_char_t *ele_localname = NULL;
    
    int element_level = 0;
    int status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_element_node, AXIS2_FAILURE);
    if(!builder)
        return AXIS2_FAILURE;
   
    builder_impl = AXIS2_INTF_TO_IMPL(builder);
    if(!builder_impl->om_builder)
        return AXIS2_FAILURE;

    element_level = axiom_stax_builder_get_element_level(
                        builder_impl->om_builder, env);

    om_element = (axiom_element_t *)
                AXIOM_NODE_GET_DATA_ELEMENT(om_element_node, env);
    if(!om_element)
        return AXIS2_FAILURE;

    ele_localname = AXIOM_ELEMENT_GET_LOCALNAME(om_element, env);
    if(!ele_localname)
        return AXIS2_FAILURE;
    
    if(!parent)
        return AXIS2_FAILURE;
        

    parent_ele = (axiom_element_t *)AXIOM_NODE_GET_DATA_ELEMENT(parent, env);
    if(!parent_ele)
        return AXIS2_FAILURE;
    
    parent_localname = AXIOM_ELEMENT_GET_LOCALNAME(parent_ele, env);
    if(!parent_localname)
        return AXIS2_FAILURE;
    
    if(element_level == 1)
    {
        if(AXIS2_STRCMP(ele_localname, AXIOM_SOAP_HEADER_LOCAL_NAME) == 0)
        {  
            /** this is the soap header element */
            axiom_soap_header_t *soap_header = NULL;
            if(builder_impl->header_present)
            {
                AXIS2_ERROR_SET(env->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_HEADERS_ENCOUNTERED, AXIS2_FAILURE);
                    return AXIS2_FAILURE;
            }
            if(builder_impl->body_present)
            {
                AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_SOAP_BUILDER_HEADER_BODY_WRONG_ORDER, AXIS2_FAILURE);
                return  AXIS2_FAILURE;                                              
            }
            
            builder_impl->header_present = AXIS2_TRUE;
            
            soap_header = axiom_soap_header_create(env);
            if(!soap_header)
                return AXIS2_FAILURE;
                
            axiom_soap_header_set_base_node (soap_header, env, om_element_node);

            axiom_soap_envelope_set_header(builder_impl->soap_envelope, env, soap_header);
            
            axiom_soap_header_set_builder (soap_header, env, builder);
           
            axiom_soap_header_set_soap_version(soap_header, env, builder_impl->soap_version);
           
            status = axiom_soap_builder_process_namespace_data(builder, env, 
                                om_element_node, AXIS2_TRUE);
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
            
        } else if(AXIS2_STRCMP(ele_localname, AXIOM_SOAP_BODY_LOCAL_NAME) == 0)
        {
           
            axiom_soap_body_t *soap_body = NULL;
            if(builder_impl->body_present)
            {
                AXIS2_ERROR_SET(env->error, 
                    AXIS2_ERROR_SOAP_BUILDER_MULTIPLE_BODY_ELEMENTS_ENCOUNTERED, AXIS2_FAILURE);
                return AXIS2_FAILURE;                   
            
            }
            builder_impl->body_present = AXIS2_TRUE;                
            
            soap_body = axiom_soap_body_create(env);
            if(!soap_body)
                return AXIS2_FAILURE;
                
            axiom_soap_body_set_base_node (soap_body, env, om_element_node);
            
            axiom_soap_body_set_builder (soap_body, env, builder);
            
            axiom_soap_envelope_set_body(builder_impl->soap_envelope, env, soap_body);
            
            status = axiom_soap_builder_process_namespace_data(builder, env, 
                        om_element_node, AXIS2_TRUE);
            
            if(status == AXIS2_FAILURE)
                return AXIS2_FAILURE;
        }
        else
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_SOAP_BUILDER_ENVELOPE_CAN_HAVE_ONLY_HEADER_AND_BODY, AXIS2_FAILURE);
            return AXIS2_FAILURE;                                            
        }
        }
    return status; 
}
