
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

#include <neethi_assertion_builder.h>

AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
neethi_assertion_builder_build(
    const axutil_env_t *env,
    axiom_node_t *node,
    axiom_element_t *element)
{
    axis2_char_t *localname = NULL;
    axis2_char_t *ns = NULL;
    axutil_qname_t *node_qname = NULL;

    localname = axiom_element_get_localname(element, env);
    if(!localname)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[neethi] Cannot get localname from element.");
        return NULL;
    }

    node_qname = axiom_element_get_qname(element, env, node);
    if(!node_qname)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot get qname from element %s.", localname);
        return NULL;
    }

    ns = axutil_qname_get_uri(node_qname, env);
    if(!ns)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Cannot get namespace from element %s.", localname);
        return NULL;
    }

    if(!(axutil_strcmp(ns, RP_SP_NS_11) && axutil_strcmp(ns, RP_SP_NS_12)))
    {
        /* if namespace is WS-SecurityPolicy Namespace */
        if(!axutil_strcmp(localname, RP_TRANSPORT_BINDING))
        {
            return rp_transport_binding_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_ASYMMETRIC_BINDING))
        {
            return rp_asymmetric_binding_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_SYMMETRIC_BINDING))
        {
            return rp_symmetric_binding_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_TRANSPORT_TOKEN))
        {
            return rp_transport_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_RECIPIENT_TOKEN))
        {
            return rp_recipient_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_INITIATOR_TOKEN))
        {
            return rp_initiator_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_PROTECTION_TOKEN))
        {
            return rp_protection_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_ENCRYPTION_TOKEN))
        {
            return rp_encryption_token_builder_build(env, node, element);
        }

        else if(!axutil_strcmp(localname, RP_SIGNATURE_TOKEN))
        {
            return rp_signature_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_X509_TOKEN))
        {
            return rp_x509_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_SECURITY_CONTEXT_TOKEN))
        {
            return rp_security_context_token_builder_build(env, node, element, ns, AXIS2_FALSE);
        }
        else if(!axutil_strcmp(localname, RP_SECURE_CONVERSATION_TOKEN))
        {
            return rp_security_context_token_builder_build(env, node, element, ns, AXIS2_TRUE);
        }
        else if(!axutil_strcmp(localname, RP_ENCRYPT_BEFORE_SIGNING))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_ENCRYPT_BEFORE_SIGNING);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_SIGN_BEFORE_ENCRYPTING))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_SIGN_BEFORE_ENCRYPTING);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_ENCRYPT_SIGNATURE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_ENCRYPT_SIGNATURE);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_PROTECT_TOKENS))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_PROTECT_TOKENS);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_ONLY_SIGN_ENTIRE_HEADERS_AND_BODY))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_ONLY_SIGN_ENTIRE_HEADERS_AND_BODY);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_KEY_IDENTIFIRE_REFERENCE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_REQUIRE_KEY_IDENTIFIRE_REFERENCE);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_ISSUER_SERIAL_REFERENCE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_REQUIRE_ISSUER_SERIAL_REFERENCE);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_EMBEDDED_TOKEN_REFERENCE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_REQUIRE_EMBEDDED_TOKEN_REFERENCE);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_THUMBPRINT_REFERENCE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_REQUIRE_THUMBPRINT_REFERENCE);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_WSS_X509_V1_TOKEN_10))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_X509_V1_TOKEN_10);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_WSS_X509_V3_TOKEN_10))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_X509_V3_TOKEN_10);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_ALGORITHM_SUITE))
        {
            return rp_algorithmsuite_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_LAYOUT))
        {
            return rp_layout_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_USERNAME_TOKEN))
        {
            return rp_username_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_SIGNED_SUPPORTING_TOKENS))
        {
            return rp_supporting_tokens_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_SUPPORTING_TOKENS))
        {
            return rp_supporting_tokens_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_ENDORSING_SUPPORTING_TOKENS))
        {
            return rp_supporting_tokens_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_SIGNED_ENDORSING_SUPPORTING_TOKENS))
        {
            return rp_supporting_tokens_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_WSS10))
        {
            return rp_wss10_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_WSS11))
        {
            return rp_wss11_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_TRUST10))
        {
            return rp_trust10_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_TRUST13))
        {
            /* we can still use rp_trust10 structures */
            return rp_trust10_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_INCLUDE_TIMESTAMP))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_INCLUDE_TIMESTAMP);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_HTTPS_TOKEN))
        {
            return rp_https_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_WSS_USERNAME_TOKEN_10))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_USERNAME_TOKEN_10);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_WSS_USERNAME_TOKEN_11))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_USERNAME_TOKEN_11);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_REF_KEY_IDENTIFIER))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_MUST_SUPPORT_REF_KEY_IDENTIFIER);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_REF_ISSUER_SERIAL))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_MUST_SUPPORT_REF_ISSUER_SERIAL);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_REF_EXTERNAL_URI))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_MUST_SUPPORT_REF_EXTERNAL_URI);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_REF_EMBEDDED_TOKEN))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_MUST_SUPPORT_REF_EMBEDDED_TOKEN);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_SIGNED_PARTS))
        {
            return rp_signed_encrypted_parts_builder_build(env, node, element, AXIS2_TRUE);
        }
        else if(!axutil_strcmp(localname, RP_ENCRYPTED_PARTS))
        {
            return rp_signed_encrypted_parts_builder_build(env, node, element, AXIS2_FALSE);
        }
        else if(!axutil_strcmp(localname, RP_BOOTSTRAP_POLICY))
        {
            return rp_bootstrap_policy_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_REF_THUMBPRINT))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_MUST_SUPPORT_REF_THUMBPRINT);
            return assertion;
        }    
        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_REF_ENCRYPTED_KEY))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_MUST_SUPPORT_REF_ENCRYPTED_KEY);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_SIGNATURE_CONFIRMATION))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_REQUIRE_SIGNATURE_CONFIRMATION);
            return assertion;
        }

        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_CLIENT_CHALLENGE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_MUST_SUPPORT_CLIENT_CHALLENGE);
            return assertion;
        }

        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_SERVER_CHALLENGE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_MUST_SUPPORT_SERVER_CHALLENGE);
            return assertion;
        }        

        else if(!axutil_strcmp(localname, RP_REQUIRE_CLIENT_ENTROPY))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_REQUIRE_CLIENT_ENTROPY);
            return assertion;
        }

        else if(!axutil_strcmp(localname, RP_REQUIRE_SERVER_ENTROPHY))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_REQUIRE_SERVER_ENTROPHY);
            return assertion;
        }

        else if(!axutil_strcmp(localname, RP_MUST_SUPPORT_ISSUED_TOKENS))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_MUST_SUPPORT_ISSUED_TOKENS);
            return assertion;
        }

        else if(!axutil_strcmp(localname, RP_REQUIRE_DERIVED_KEYS))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            if(!axutil_strcmp(ns, RP_SP_NS_11))
            {
                /* derived key should be as defined in WS-SecConversation 1.0 */
                neethi_assertion_set_value(
                    assertion, env, NULL, ASSERTION_TYPE_REQUIRE_DERIVED_KEYS_SC10);
            }
            else
            {
                /* derived key should be as defined in WS-SecConversation 1.3 */
                neethi_assertion_set_value(
                    assertion, env, NULL, ASSERTION_TYPE_REQUIRE_DERIVED_KEYS_SC13);
            }
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_EXTERNAL_URI_REFERENCE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_REQUIRE_EXTERNAL_URI);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_SC10_SECURITY_CONTEXT_TOKEN))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_SC10_SECURITY_CONTEXT_TOKEN);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_SC13_SECURITY_CONTEXT_TOKEN))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_SC13_SECURITY_CONTEXT_TOKEN);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_ISSUER))
        {
            neethi_assertion_t *assertion = NULL;
            axis2_char_t *issuer = NULL;

            issuer = axiom_element_get_text(element, env, node);
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, issuer, ASSERTION_TYPE_ISSUER);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_EXTERNAL_REFERENCE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, 
                ASSERTION_TYPE_REQUIRE_EXTERNAL_REFERENCE);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_REQUIRE_INTERNAL_REFERENCE))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                ASSERTION_TYPE_REQUIRE_INTERNAL_REFERENCE);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_ISSUED_TOKEN))
        {
            return rp_issued_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_SAML_TOKEN))
        {
            return rp_saml_token_builder_build(env, node, element);
        }
        else if(!axutil_strcmp(localname, RP_WSS_SAML_V10_TOKEN_V10))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_SAML_V10_TOKEN_V10);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_WSS_SAML_V10_TOKEN_V11))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_SAML_V10_TOKEN_V11);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_WSS_SAML_V11_TOKEN_V10))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_SAML_V11_TOKEN_V10);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_WSS_SAML_V11_TOKEN_V11))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_SAML_V11_TOKEN_V11);
            return assertion;
        }
        else if(!axutil_strcmp(localname, RP_WSS_SAML_V20_TOKEN_V11))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_WSS_SAML_V20_TOKEN_V11);
            return assertion;
        }
    }
    else if(!axutil_strcmp(ns, RP_RAMPART_NS))
    {
        /* if namespace is Rampart Namespace */
        if(!axutil_strcmp(localname, RP_RAMPART_CONFIG))
        {
            return rp_rampart_config_builder_build(env, node, element);
        }
    }
    else if(!axutil_strcmp(ns, AXIS2_MTOM_POLICY_NS))
    {
        if(!axutil_strcmp(localname, AXIS2_OPTIMIZED_MIME_SERIALIZATION))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);  
            neethi_assertion_set_value(assertion, env, NULL, ASSERTION_TYPE_OPTIMIZED_MIME_SERIALIZATION) ;
            return assertion;     
        }
    }
    else if((!axutil_strcmp(ns, AXIS2_RM_POLICY_10_NS))||
        (!axutil_strcmp(ns, AXIS2_RM_POLICY_11_NS)))
    {
        if(!axutil_strcmp(localname, AXIS2_RM_RMASSERTION))
        {
            return axis2_rm_assertion_builder_build(env, node, element);
        }
    }

    /* This assertion cannot be processed */
    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NEETHI_UNKNOWN_ASSERTION, AXIS2_FAILURE);
    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
        "[neethi] Unknown Assertion %s with namespace %s", localname, ns);
    return NULL;
}
