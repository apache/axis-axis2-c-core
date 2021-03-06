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

#include <rp_property.h>
#include <rp_symmetric_binding.h>
#include <rp_asymmetric_binding.h>
#include <rp_transport_binding.h>
#include <rp_supporting_tokens.h>
#include <rp_wss10.h>
#include <rp_wss11.h>

struct rp_property_t
{
    rp_property_type_t type;
    void *value;
    int ref;
};

AXIS2_EXTERN rp_property_t *AXIS2_CALL
rp_property_create(
    const axutil_env_t * env)
{
    rp_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property = (rp_property_t *)AXIS2_MALLOC(env->allocator, sizeof(rp_property_t));

    if(property == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    property->type = RP_PROPERTY_UNKNOWN;
    property->value = NULL;
    property->ref = 0;

    return property;
}

AXIS2_EXTERN void AXIS2_CALL
rp_property_free(
    rp_property_t * property,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(property)
    {
        if(--(property->ref) > 0)
        {
            return;
        }

        if(property->value)
        {
            switch(property->type)
            {
                case RP_PROPERTY_USERNAME_TOKEN:
                    rp_username_token_free((rp_username_token_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_X509_TOKEN:
                    rp_x509_token_free((rp_x509_token_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_ISSUED_TOKEN:
                    rp_issued_token_free((rp_issued_token_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_SAML_TOKEN:
                    rp_saml_token_free((rp_saml_token_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_SECURITY_CONTEXT_TOKEN:
                    rp_security_context_token_free((rp_security_context_token_t *)property->value,
                        env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_HTTPS_TOKEN:
                    rp_https_token_free((rp_https_token_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_SYMMETRIC_BINDING:
                    rp_symmetric_binding_free((rp_symmetric_binding_t *)property-> value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_ASYMMETRIC_BINDING:
                    rp_asymmetric_binding_free((rp_asymmetric_binding_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_TRANSPORT_BINDING:
                    rp_transport_binding_free((rp_transport_binding_t *)property-> value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_SIGNED_SUPPORTING_TOKEN:
                    rp_supporting_tokens_free((rp_supporting_tokens_t *)property-> value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_SIGNED_ENDORSING_SUPPORTING_TOKEN:
                    rp_supporting_tokens_free((rp_supporting_tokens_t *)property-> value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_SUPPORTING_SUPPORTING_TOKEN:
                    rp_supporting_tokens_free((rp_supporting_tokens_t *)property-> value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_ENDORSING_SUPPORTING_TOKEN:
                    rp_supporting_tokens_free((rp_supporting_tokens_t *)property-> value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_SUPPORTING_TOKEN:
                    rp_supporting_tokens_free((rp_supporting_tokens_t *)property-> value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_WSS10:
                    rp_wss10_free((rp_wss10_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_WSS11:
                    rp_wss11_free((rp_wss11_t *)property->value, env);
                    property->value = NULL;
                    break;

                case RP_PROPERTY_UNKNOWN:
                    break;
            }
        }
        AXIS2_FREE(env->allocator, property);
    }

    return;
}

/* Implementations */
AXIS2_EXTERN void *AXIS2_CALL
rp_property_get_value(
    rp_property_t * property,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return property->value;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_property_set_value(
    rp_property_t * property,
    const axutil_env_t * env,
    void *value,
    rp_property_type_t type)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);

    property->type = type;

    if(type == RP_PROPERTY_X509_TOKEN)
    {
        rp_x509_token_increment_ref((rp_x509_token_t *)value, env);
    }
    else if(type == RP_PROPERTY_SECURITY_CONTEXT_TOKEN)
    {
        rp_security_context_token_increment_ref((rp_security_context_token_t *)value, env);
    }
    else if(type == RP_PROPERTY_WSS10)
    {
        rp_wss10_increment_ref((rp_wss10_t *)value, env);
    }
    else if(type == RP_PROPERTY_WSS11)
    {
        rp_wss11_increment_ref((rp_wss11_t *)value, env);
    }
    else if(type == RP_PROPERTY_USERNAME_TOKEN)
    {
        rp_username_token_increment_ref((rp_username_token_t *)value, env);
    }

    else if(type == RP_PROPERTY_HTTPS_TOKEN)
    {
        rp_https_token_increment_ref((rp_https_token_t *)value, env);
    }

    else if(type == RP_PROPERTY_SIGNED_SUPPORTING_TOKEN)
    {
        rp_supporting_tokens_increment_ref((rp_supporting_tokens_t *)value, env);
    }
    else if(type == RP_PROPERTY_SUPPORTING_SUPPORTING_TOKEN)
    {
        rp_supporting_tokens_increment_ref((rp_supporting_tokens_t *)value, env);
    }
    else if(type == RP_PROPERTY_SIGNED_ENDORSING_SUPPORTING_TOKEN)
    {
        rp_supporting_tokens_increment_ref((rp_supporting_tokens_t *)value, env);
    }
    else if(type == RP_PROPERTY_ENDORSING_SUPPORTING_TOKEN)
    {
        rp_supporting_tokens_increment_ref((rp_supporting_tokens_t *)value, env);
    }
    else if(type == RP_PROPERTY_ASYMMETRIC_BINDING)
    {
        rp_asymmetric_binding_increment_ref((rp_asymmetric_binding_t *)value, env);
    }
    else if(type == RP_PROPERTY_TRANSPORT_BINDING)
    {
        rp_transport_binding_increment_ref((rp_transport_binding_t *)value, env);
    }
    else if(type == RP_PROPERTY_SYMMETRIC_BINDING)
    {
        rp_symmetric_binding_increment_ref((rp_symmetric_binding_t *)value, env);
    }
    property->value = (void *)value;

    return AXIS2_SUCCESS;

}

AXIS2_EXTERN rp_property_type_t AXIS2_CALL
rp_property_get_type(
    rp_property_t * property,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return property->type;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_property_increment_ref(
    rp_property_t * property,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property->ref++;
    return AXIS2_SUCCESS;
}
