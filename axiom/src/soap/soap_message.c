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

#include <axiom_soap_message.h>
#include <axiom_document.h>
#include <axiom_output.h>
#include "_axiom_soap_envelope.h"

struct axiom_soap_message
{
    axiom_document_t *om_doc;

    axiom_soap_envelope_t *soap_envelope;
};

AXIS2_EXTERN axiom_soap_message_t* AXIS2_CALL
axiom_soap_message_create(const axis2_env_t *env,
        axiom_soap_builder_t *builder,
        axiom_document_t *om_doc)
{
    axiom_soap_message_t *soap_message = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, om_doc, NULL);
    AXIS2_PARAM_CHECK(env->error, builder, NULL);

    soap_message = (axiom_soap_message_t *)AXIS2_MALLOC(env->allocator,
            sizeof(axiom_soap_message_t));
    if (!soap_message)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    soap_message->om_doc = NULL;
    soap_message->soap_envelope = NULL;

    return soap_message;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_message_free(axiom_soap_message_t *soap_message,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (soap_message->om_doc)
    {
        AXIOM_DOCUMENT_FREE(soap_message->om_doc, env);
        soap_message->om_doc = NULL;
    }
    if (soap_message->soap_envelope)
    {
        AXIOM_SOAP_ENVELOPE_FREE(soap_message->soap_envelope, env);
        soap_message->soap_envelope = NULL;
    }
    AXIS2_FREE(env->allocator, soap_message);
    soap_message = NULL;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
axiom_soap_message_get_soap_envelope(axiom_soap_message_t *soap_message,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    if (soap_message->soap_envelope)
    {
        return soap_message->soap_envelope;
    }
    else
    {
        soap_message->soap_envelope = axiom_soap_envelope_create_null(env);

        return soap_message->soap_envelope;
    }
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axiom_soap_message_serialize(axiom_soap_message_t *soap_message,
        const axis2_env_t *env,
        axiom_output_t *om_output)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIOM_DOCUMENT_SERIALIZE(soap_message->om_doc, env, om_output);
}

