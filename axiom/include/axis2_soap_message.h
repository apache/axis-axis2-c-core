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
 
 #ifndef AXIS2_SOAP_MESSAGE_H
 #define AXIS2_SOAP_MESSAGE_H
 
 /**
 * @file axis2_soap_message.h
 * @brief axis2_soap_message struct
 *        corresponds to root element of soap message
 */
#include <axis2_env.h>
#include <axis2_soap_envelope.h>
#include <axis2_soap_builder.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_soap_message        axis2_soap_message_t;
    typedef struct axis2_soap_message_ops    axis2_soap_message_ops_t;
    
    
/**
 * @defgroup axis2_soap_message
 * @ingroup axis2_soap
 * @{
 */

/**
 *   \brief soap_message operations struct
 *   ops Encapsulator struct of axis2_soap body
 */
 AXIS2_DECLARE_DATA   struct axis2_soap_message_ops
    {
      /**
        * Free an axis2_soap_message
        * @param  body pointer to soap_message struct
        * @param  env Environment. MUST NOT be NULL
        * @return satus of the op. AXIS2_SUCCESS on success 
        *         else AXIS2_FAILURE
        */

        axis2_status_t (AXIS2_CALL *
		free_fn)(axis2_soap_message_t *message,
                 const axis2_env_t *env);
                                             
        axis2_soap_envelope_t* (AXIS2_CALL *
		get_soap_envelope)(axis2_soap_message_t *message,
                           const axis2_env_t *env);
                                             
        axis2_status_t (AXIS2_CALL *
		serialize)(axis2_soap_message_t *message,
                   const axis2_env_t *env,
                   axis2_om_output_t *om_output);
                                                       
                                                                                            
    };

  /**
    * \brief soap_message struct
    * represent a soap_message
    */
    struct axis2_soap_message
    {
        /** operation of axis2_soap_message struct */
        axis2_soap_message_ops_t *ops;
       
    };

  /**
    * creates a soap message struct 
    * @param env Environment. MUST NOT be NULL
    */
AXIS2_DECLARE(axis2_soap_message_t *)
axis2_soap_message_create(const axis2_env_t *env,
                          axis2_soap_builder_t *soap_builder,
                          axis2_om_document_t *om_doc);

/******************** Macros **************************************************/
    
    
/** free soap_message */
#define AXIS2_SOAP_MESSAGE_FREE(message , env) \
        ((message)->ops->free_fn(message, env))
        
#define AXIS2_SOAP_MESSAGE_GET_SOAP_ENVELOPE(message, env) \
        ((message)->ops->get_envelope(message, env))
        
#define AXIS2_SOAP_MESSAGE_SERIALIZE(message, env, om_output) \
        ((message)->ops->serialize(message, env, om_output))
/** @} */

#ifdef __cplusplus
}
#endif
 
 
 
#endif /* AXIS2_SOAP_MESSAGE_H */
