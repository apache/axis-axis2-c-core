/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIOM_SOAP_MESSAGE_H
#define AXIOM_SOAP_MESSAGE_H

/**
* @file axiom_soap_message.h
* @brief axiom_soap_message struct
*        corresponds to root element of soap message
*/
#include <axis2_env.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_builder.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axiom_soap_message        axiom_soap_message_t;

    /**
     * @defgroup axiom_soap_message soap message
     * @ingroup axiom_soap
     * @{
     */

    /**
      * creates a soap message struct 
      * @param env Environment. MUST NOT be NULL
      */
    AXIS2_EXTERN axiom_soap_message_t * AXIS2_CALL
    axiom_soap_message_create(const axis2_env_t *env,
            axiom_soap_builder_t *soap_builder,
            axiom_document_t *om_doc);

    /**
      * Free an axiom_soap_message
      * @param  body pointer to soap_message struct
      * @param  env Environment. MUST NOT be NULL
      * @return satus of the op. AXIS2_SUCCESS on success 
      *         else AXIS2_FAILURE
      */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_message_free(axiom_soap_message_t *message,
            const axis2_env_t *env);

    AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
    axiom_soap_message_get_soap_envelope(axiom_soap_message_t *message,
            const axis2_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axiom_soap_message_serialize(axiom_soap_message_t *message,
            const axis2_env_t *env,
            axiom_output_t *om_output);

#define AXIOM_SOAP_MESSAGE_FREE(message , env) \
        ((message)->ops->free(message, env)

#define AXIOM_SOAP_MESSAGE_GET_SOAP_ENVELOPE(message, env) \
        ((message)->ops->get_envelope(message, env)

#define AXIOM_SOAP_MESSAGE_SERIALIZE(message, env, om_output) \
        ((message)->ops->serialize(message, env, om_output)
    /** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIOM_SOAP_MESSAGE_H */

