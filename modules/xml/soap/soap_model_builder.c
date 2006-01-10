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
    
    int soap_version;
    
}axis2_soap_model_builder_impl_t;

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
                                 
static void identify_soap_version(axis2_char_t* soap_version_uri_from_transport);


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
    
    builder_impl->soap_builder.ops = (axis2_soap_model_builder_ops_t*)
            AXIS2_MALLOC((*env)->allocator, sizeof(axis2_soap_model_builder_ops_t));
    
    
    
    
    
    
    
    
    
    
    
}                                