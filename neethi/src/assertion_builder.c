
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
    const axutil_env_t * env,
    axiom_node_t * node,
    axiom_element_t * element)
{

    axis2_char_t *localname = NULL;

    localname = axiom_element_get_localname(element, env);
    if (!localname)
    {
        return NULL;
    }

    if (axutil_strcmp(localname, RP_TRANSPORT_BINDING) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_TRANSPORT_BINDING, node, element))
        {
            return rp_transport_binding_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_ASYMMETRIC_BINDING) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_ASYMMETRIC_BINDING, node, element))
        {
            return rp_asymmetric_binding_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_SYMMETRIC_BINDING) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_SYMMETRIC_BINDING, node, element))
        {
            return rp_symmetric_binding_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_TRANSPORT_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_TRANSPORT_TOKEN, node, element))
        {
            return rp_transport_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_RECIPIENT_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_RECIPIENT_TOKEN, node, element))
        {
            return rp_recipient_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    
    else if (axutil_strcmp(localname, RP_INITIATOR_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_INITIATOR_TOKEN, node, element))
        {
            return rp_initiator_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_PROTECTION_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_PROTECTION_TOKEN, node, element))
        {
            return rp_protection_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_ENCRYPTION_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_ENCRYPTION_TOKEN, node, element))
        {
            return rp_encryption_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_SIGNATURE_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_SIGNATURE_TOKEN, node, element))
        {
            return rp_signature_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_X509_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_X509_TOKEN, node, element))
        {
            return rp_x509_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SECURITY_CONTEXT_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_SECURITY_CONTEXT_TOKEN, node, element))
        {
            return rp_security_context_token_builder_build(env, node, element, AXIS2_FALSE);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SECURE_CONVERSATION_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_SECURE_CONVERSATION_TOKEN, node, element))
        {
            return rp_security_context_token_builder_build(env, node, element, AXIS2_TRUE);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_ENCRYPT_BEFORE_SIGNING) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_ENCRYPT_BEFORE_SIGNING, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_ENCRYPT_BEFORE_SIGNING);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SIGN_BEFORE_ENCRYPTING) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_SIGN_BEFORE_ENCRYPTING, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_SIGN_BEFORE_ENCRYPTING);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_ENCRYPT_SIGNATURE) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_ENCRYPT_SIGNATURE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_ENCRYPT_SIGNATURE);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_PROTECT_TOKENS) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_PROTECT_TOKENS, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_PROTECT_TOKENS);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_ONLY_SIGN_ENTIRE_HEADERS_AND_BODY) ==
             0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_ONLY_SIGN_ENTIRE_HEADERS_AND_BODY, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_ONLY_SIGN_ENTIRE_HEADERS_AND_BODY);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_KEY_IDENTIFIRE_REFERENCE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_KEY_IDENTIFIRE_REFERENCE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_KEY_IDENTIFIRE_REFERENCE);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_ISSUER_SERIAL_REFERENCE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_ISSUER_SERIAL_REFERENCE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_ISSUER_SERIAL_REFERENCE);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_EMBEDDED_TOKEN_REFERENCE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_EMBEDDED_TOKEN_REFERENCE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_EMBEDDED_TOKEN_REFERENCE);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_THUMBPRINT_REFERENCE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_THUMBPRINT_REFERENCE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_THUMBPRINT_REFERENCE);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_X509_V1_TOKEN_10) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_X509_V1_TOKEN_10, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_X509_V1_TOKEN_10);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_X509_V3_TOKEN_10) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_X509_V3_TOKEN_10, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_X509_V3_TOKEN_10);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_ALGORITHM_SUITE) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_ALGORITHM_SUITE, node, element))
        {
            return rp_algorithmsuite_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_LAYOUT) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_LAYOUT, node, element))
        {
            return rp_layout_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_USERNAME_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_USERNAME_TOKEN, node, element))
        {
            return rp_username_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SIGNED_SUPPORTING_TOKENS) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_SIGNED_SUPPORTING_TOKENS, node, element))
        {
            return rp_supporting_tokens_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SIGNED_ENDORSING_SUPPORTING_TOKENS) ==
             0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_SIGNED_ENDORSING_SUPPORTING_TOKENS, node, element))
        {
            return rp_supporting_tokens_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS10) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_WSS10, node, element))
        {
            return rp_wss10_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS11) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_WSS11, node, element))
        {
            return rp_wss11_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_TRUST10) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_TRUST10, node, element))
        {
            return rp_trust10_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    
    else if (axutil_strcmp(localname, RP_INCLUDE_TIMESTAMP) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_INCLUDE_TIMESTAMP, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_INCLUDE_TIMESTAMP);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_HTTPS_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_HTTPS_TOKEN, node, element))
        {
            return rp_https_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_USERNAME_TOKEN_10) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_USERNAME_TOKEN_10, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_USERNAME_TOKEN_10);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_USERNAME_TOKEN_11) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_USERNAME_TOKEN_11, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_USERNAME_TOKEN_11);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_REF_KEY_IDENTIFIER) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_REF_KEY_IDENTIFIER, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_MUST_SUPPORT_REF_KEY_IDENTIFIER);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_REF_ISSUER_SERIAL) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_REF_ISSUER_SERIAL, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_MUST_SUPPORT_REF_ISSUER_SERIAL);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_REF_EXTERNAL_URI) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_REF_EXTERNAL_URI, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_MUST_SUPPORT_REF_EXTERNAL_URI);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_REF_EMBEDDED_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_REF_EMBEDDED_TOKEN, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_MUST_SUPPORT_REF_EMBEDDED_TOKEN);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SIGNED_PARTS) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_SIGNED_PARTS, node, element))
        {
            return rp_signed_encrypted_parts_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_ENCRYPTED_PARTS) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_ENCRYPTED_PARTS, node, element))
        {
            return rp_signed_encrypted_parts_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_RAMPART_CONFIG) == 0)
    {
        if (rp_match_rampart_config_qname
            (env, RP_RAMPART_CONFIG, node, element))
        {
            return rp_rampart_config_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if(axutil_strcmp(localname, RP_BOOTSTRAP_POLICY) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_BOOTSTRAP_POLICY, node, element))
        {
            return rp_bootstrap_policy_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_REF_THUMBPRINT) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_REF_THUMBPRINT, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_MUST_SUPPORT_REF_THUMBPRINT);
            return assertion;
        }
        else
            return NULL;
    }    
    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_REF_ENCRYPTED_KEY) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_REF_ENCRYPTED_KEY, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_MUST_SUPPORT_REF_ENCRYPTED_KEY);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_SIGNATURE_CONFIRMATION) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_SIGNATURE_CONFIRMATION, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                      ASSERTION_TYPE_REQUIRE_SIGNATURE_CONFIRMATION);
            return assertion;
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_CLIENT_CHALLENGE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_CLIENT_CHALLENGE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                      ASSERTION_TYPE_MUST_SUPPORT_CLIENT_CHALLENGE);
            return assertion;
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_SERVER_CHALLENGE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_SERVER_CHALLENGE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                      ASSERTION_TYPE_MUST_SUPPORT_SERVER_CHALLENGE);
            return assertion;
        }
        else
            return NULL;
    }        

    else if (axutil_strcmp(localname, RP_REQUIRE_CLIENT_ENTROPY) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_CLIENT_ENTROPY, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                      ASSERTION_TYPE_REQUIRE_CLIENT_ENTROPY);
            return assertion;
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_REQUIRE_SERVER_ENTROPHY) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_SERVER_ENTROPHY, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                      ASSERTION_TYPE_REQUIRE_SERVER_ENTROPHY);
            return assertion;
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_MUST_SUPPORT_ISSUED_TOKENS) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_MUST_SUPPORT_ISSUED_TOKENS, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                      ASSERTION_TYPE_MUST_SUPPORT_ISSUED_TOKENS);
            return assertion;
        }
        else
            return NULL;
    }

    else if (axutil_strcmp(localname, RP_REQUIRE_DERIVED_KEYS) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_DERIVED_KEYS, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_DERIVED_KEYS);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_EXTERNAL_URI_REFERENCE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_EXTERNAL_URI_REFERENCE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_EXTERNAL_URI);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SC10_SECURITY_CONTEXT_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_SC10_SECURITY_CONTEXT_TOKEN, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_SC10_SECURITY_CONTEXT_TOKEN);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_ISSUER) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_ISSUER, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            axis2_char_t *issuer = NULL;

            issuer = axiom_element_get_text(element, env, node);
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, issuer, ASSERTION_TYPE_ISSUER);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_EXTERNAL_REFERENCE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_EXTERNAL_REFERENCE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_EXTERNAL_REFERENCE);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_REQUIRE_INTERNAL_REFERENCE) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_REQUIRE_INTERNAL_REFERENCE, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_REQUIRE_INTERNAL_REFERENCE);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_ISSUED_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_ISSUED_TOKEN, node, element))
        {
            return rp_issued_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_SAML_TOKEN) == 0)
    {
        if (rp_match_secpolicy_qname(env, RP_SAML_TOKEN, node, element))
        {
            return rp_saml_token_builder_build(env, node, element);
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_SAML_V10_TOKEN_V10) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_SAML_V10_TOKEN_V10, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_SAML_V10_TOKEN_V10);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_SAML_V10_TOKEN_V11) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_SAML_V10_TOKEN_V11, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_SAML_V10_TOKEN_V11);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_SAML_V11_TOKEN_V10) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_SAML_V11_TOKEN_V10, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_SAML_V11_TOKEN_V10);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_SAML_V11_TOKEN_V11) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_SAML_V11_TOKEN_V11, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_SAML_V11_TOKEN_V11);
            return assertion;
        }
        else
            return NULL;
    }
    else if (axutil_strcmp(localname, RP_WSS_SAML_V20_TOKEN_V11) == 0)
    {
        if (rp_match_secpolicy_qname
            (env, RP_WSS_SAML_V20_TOKEN_V11, node, element))
        {
            neethi_assertion_t *assertion = NULL;
            assertion = neethi_assertion_create(env);
            neethi_assertion_set_value(assertion, env, NULL,
                                       ASSERTION_TYPE_WSS_SAML_V20_TOKEN_V11);
            return assertion;
        }
        else
            return NULL;
    }
    else
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_NEETHI_UNKNOWN_ASSERTION,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[neethi] Unknown Assertion %s",
                        localname);
        return NULL;
        
        /*assertion = neethi_assertion_create(env);
        neethi_assertion_set_value(assertion, env, NULL,
                                   ASSERTION_TYPE_UNKNOWN);
        neethi_assertion_set_element(assertion, env, element);
        neethi_assertion_set_node(assertion, env, node);
        return assertion;
        */
    }
}
