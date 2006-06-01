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
 
 #include <axis2_soap_message.h>
 #include <axis2_om_document.h>
 #include <axis2_om_output.h>
 #include "_axis2_soap_envelope.h"

/*************************** impl struct **************************************/

typedef struct axis2_soap_message_impl_t
{
    axis2_soap_message_t soap_message;
    
    axis2_om_document_t *om_doc;
    
    axis2_soap_envelope_t *soap_envelope;
    
}axis2_soap_message_impl_t;

/*************************** Macro ********************************************/

#define AXIS2_INTF_TO_IMPL(message) ((axis2_soap_message_impl_t*)message) 

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_soap_message_free(axis2_soap_message_t *message,
                        const axis2_env_t *env);
                                        
axis2_soap_envelope_t* AXIS2_CALL
axis2_soap_message_get_soap_envelope(axis2_soap_message_t *message,
                                     const axis2_env_t *env);
                                        
                                        
axis2_status_t AXIS2_CALL
axis2_soap_message_serialize(axis2_soap_message_t *message,
                             const axis2_env_t *env,
                             axis2_om_output_t *om_output);
                             
/************************** function implementations **************************/

AXIS2_EXTERN axis2_soap_message_t* AXIS2_CALL
axis2_soap_message_create(const axis2_env_t *env,
                          axis2_soap_builder_t *builder,
                          axis2_om_document_t *om_doc)
{
    axis2_soap_message_impl_t *soap_message_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, om_doc, NULL);
    AXIS2_PARAM_CHECK(env->error, builder, NULL);
    
    soap_message_impl = (axis2_soap_message_impl_t *)AXIS2_MALLOC(env->allocator, 
                            sizeof(axis2_soap_message_impl_t));
    if(!soap_message_impl)                            
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    soap_message_impl->soap_message.ops = NULL;
    soap_message_impl->om_doc = NULL;
    soap_message_impl->soap_envelope = NULL;
    
    soap_message_impl->soap_message.ops = (axis2_soap_message_ops_t*)AXIS2_MALLOC(env->allocator,
                                            sizeof(axis2_soap_message_ops_t));
                                            
    if(!(soap_message_impl->soap_message.ops))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, soap_message_impl);
        soap_message_impl = NULL;
        return NULL;
    }  
    
    soap_message_impl->om_doc = om_doc;
    
    soap_message_impl->soap_message.ops->free_fn =
            axis2_soap_message_free;
    soap_message_impl->soap_message.ops->serialize =
            axis2_soap_message_serialize;
    soap_message_impl->soap_message.ops->get_soap_envelope =
            axis2_soap_message_get_soap_envelope;
                        
            
    return &(soap_message_impl->soap_message);
}

axis2_status_t AXIS2_CALL
axis2_soap_message_free(axis2_soap_message_t *message,
                        const axis2_env_t *env)
{
    axis2_soap_message_impl_t *soap_message_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_message_impl = AXIS2_INTF_TO_IMPL(message);
    if(soap_message_impl->om_doc)
    {
        AXIS2_OM_DOCUMENT_FREE(soap_message_impl->om_doc, env);
        soap_message_impl->om_doc = NULL;
    }
    if(soap_message_impl->soap_envelope)
    {
        AXIS2_SOAP_ENVELOPE_FREE(soap_message_impl->soap_envelope, env);
        soap_message_impl->soap_envelope = NULL;
    }   
    if(soap_message_impl->soap_message.ops)
    {
        AXIS2_FREE(env->allocator, soap_message_impl->soap_message.ops);
        soap_message_impl->soap_message.ops = NULL;
    }
    AXIS2_FREE(env->allocator, soap_message_impl);
    soap_message_impl = NULL;
    return AXIS2_SUCCESS;
}

axis2_soap_envelope_t* AXIS2_CALL
axis2_soap_message_get_soap_envelope(axis2_soap_message_t *message,
                                     const axis2_env_t *env)
{
    axis2_soap_message_impl_t* soap_message_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    soap_message_impl = AXIS2_INTF_TO_IMPL(message);
    if(soap_message_impl->soap_envelope)
    {
        return soap_message_impl->soap_envelope;
    }    
    else 
    {
        soap_message_impl->soap_envelope = axis2_soap_envelope_create_null(env);
        
        return soap_message_impl->soap_envelope;        
    }
}    

axis2_status_t AXIS2_CALL
axis2_soap_message_serialize(axis2_soap_message_t *message,
                             const axis2_env_t *env,
                             axis2_om_output_t *om_output)
{
    axis2_soap_message_impl_t *soap_message_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    soap_message_impl = AXIS2_INTF_TO_IMPL(message);
    return AXIS2_OM_DOCUMENT_SERIALIZE(soap_message_impl->om_doc, env, om_output);
}                             
                                     
