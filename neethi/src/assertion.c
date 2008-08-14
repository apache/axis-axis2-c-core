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

#include <neethi_assertion.h>
#include <rp_x509_token.h>
#include <rp_property.h>
#include <rp_layout.h>
#include <rp_algorithmsuite.h>
#include <rp_wss10.h>
#include <rp_wss11.h>
#include <rp_trust10.h>
#include <rp_supporting_tokens.h>
#include <rp_username_token.h>
#include <rp_asymmetric_binding.h>
#include <rp_rampart_config.h>
#include <rp_signed_encrypted_parts.h>
#include <rp_symmetric_binding.h>
#include <rp_transport_binding.h>
#include <rp_saml_token.h>
#include <rp_issued_token.h>

struct neethi_assertion_t
{
    void *value;
    neethi_assertion_type_t type;
    axutil_array_list_t *policy_components;
    axiom_element_t *element;
    axiom_node_t *node;
    axis2_bool_t is_optional;
    AXIS2_FREE_VOID_ARG free_func;
};

AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
neethi_assertion_create(
    const axutil_env_t *env)
{
    neethi_assertion_t *neethi_assertion = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    neethi_assertion = (neethi_assertion_t *) AXIS2_MALLOC(env->allocator,
                                                           sizeof
                                                           (neethi_assertion_t));

    if (neethi_assertion == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_assertion->policy_components = NULL;

    neethi_assertion->policy_components = axutil_array_list_create(env, 0);
    if (!(neethi_assertion->policy_components))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }
    neethi_assertion->value = NULL;
    neethi_assertion->type = ASSERTION_TYPE_UNKNOWN;
    neethi_assertion->element = NULL;
    neethi_assertion->is_optional = AXIS2_FALSE;
    neethi_assertion->node = NULL;
    neethi_assertion->free_func = 0;

    return neethi_assertion;
}

neethi_assertion_t *AXIS2_CALL
neethi_assertion_create_with_args(
    const axutil_env_t *env,
    AXIS2_FREE_VOID_ARG free_func,
    void *value,
    neethi_assertion_type_t type)
{
    neethi_assertion_t *neethi_assertion = NULL;
    neethi_assertion = (neethi_assertion_t *) AXIS2_MALLOC(
        env->allocator, sizeof(neethi_assertion_t));

    if (!neethi_assertion)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Neethi assertion creation failed. Out of memory");
        return NULL;
    }

    neethi_assertion->policy_components = NULL;
    neethi_assertion->policy_components = axutil_array_list_create(env, 0);
    if (!(neethi_assertion->policy_components))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Neethi assertion policy components creation failed.");
        return NULL;
    }

    /* This ref count is for asertions which are represented from a struct.
     * These assertion structs are more probably referenced from some other
     * struct. So we need to increment the ref count in order to prevent
     * unnecessary memory freeing */

    if (type == ASSERTION_TYPE_X509_TOKEN)
    {
        rp_x509_token_increment_ref((rp_x509_token_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_SECURITY_CONTEXT_TOKEN)
    {
        rp_security_context_token_increment_ref((rp_security_context_token_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_INITIATOR_TOKEN)
    {
        rp_property_increment_ref((rp_property_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_RECIPIENT_TOKEN)
    {
        rp_property_increment_ref((rp_property_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_PROTECTION_TOKEN)
    {
        rp_property_increment_ref((rp_property_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_ENCRYPTION_TOKEN)
    {
        rp_property_increment_ref((rp_property_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_TRANSPORT_TOKEN)
    {
        rp_property_increment_ref((rp_property_t *) value, env);
    }    
    else if (type == ASSERTION_TYPE_SIGNATURE_TOKEN)
    {
        rp_property_increment_ref((rp_property_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_LAYOUT)
    {
        rp_layout_increment_ref((rp_layout_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_ALGORITHM_SUITE)
    {
        rp_algorithmsuite_increment_ref((rp_algorithmsuite_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_WSS10)
    {
        rp_wss10_increment_ref((rp_wss10_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_WSS11)
    {
        rp_wss11_increment_ref((rp_wss11_t *) value, env); 
    }
    else if (type == ASSERTION_TYPE_TRUST10)
    {
        rp_trust10_increment_ref((rp_trust10_t *) value, env); 
    }
    else if (type == ASSERTION_TYPE_SUPPORTING_TOKENS)
    {
        rp_supporting_tokens_increment_ref((rp_supporting_tokens_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_USERNAME_TOKEN)
    {
        rp_username_token_increment_ref((rp_username_token_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_ASSYMMETRIC_BINDING)
    {
        rp_asymmetric_binding_increment_ref((rp_asymmetric_binding_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_SYMMETRIC_BINDING)
    {
        rp_symmetric_binding_increment_ref((rp_symmetric_binding_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_TRANSPORT_BINDING)
    {
        rp_transport_binding_increment_ref((rp_transport_binding_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_SIGNED_ENCRYPTED_PARTS)
    {
        rp_signed_encrypted_parts_increment_ref((rp_signed_encrypted_parts_t *)value, env);
    }
    else if (type == ASSERTION_TYPE_RAMPART_CONFIG)
    {
        rp_rampart_config_increment_ref((rp_rampart_config_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_ISSUED_TOKEN)
    {
        rp_issued_token_increment_ref((rp_issued_token_t *) value, env);
    }
    else if (type == ASSERTION_TYPE_SAML_TOKEN)
    {
        rp_saml_token_increment_ref((rp_saml_token_t *) value, env);
    }
    
    neethi_assertion->value = value;
    neethi_assertion->type = type;
    neethi_assertion->element = NULL;
    neethi_assertion->is_optional = AXIS2_FALSE;
    neethi_assertion->node = NULL;
    neethi_assertion->free_func = free_func;

    return neethi_assertion;
}

AXIS2_EXTERN void AXIS2_CALL
neethi_assertion_free(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    if (neethi_assertion)
    {
        if (neethi_assertion->policy_components)
        {
            int i = 0;
            for (i = 0;
                 i < axutil_array_list_size(neethi_assertion->policy_components,
                                            env); i++)
            {
                neethi_operator_t *operator = NULL;
                operator =(neethi_operator_t *)
                    axutil_array_list_get(neethi_assertion->policy_components,
                                          env, i);
                if (operator)
                    neethi_operator_free(operator, env);

                operator = NULL;
            }
            axutil_array_list_free(neethi_assertion->policy_components, env);
            neethi_assertion->policy_components = NULL;
        }
        if (neethi_assertion->value)
        {
            if (neethi_assertion->free_func)
            {
                neethi_assertion->free_func(neethi_assertion->value, env);
            }
        }
        AXIS2_FREE(env->allocator, neethi_assertion);
        neethi_assertion = NULL;
    }
    return;
}

/* Implementations */

AXIS2_EXTERN neethi_assertion_type_t AXIS2_CALL
neethi_assertion_get_type(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    return neethi_assertion->type;
}

AXIS2_EXTERN void *AXIS2_CALL
neethi_assertion_get_value(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    return neethi_assertion->value;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_assertion_set_value(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env,
    void *value,
    neethi_assertion_type_t type)
{
    neethi_assertion->type = type;
    if (type == ASSERTION_TYPE_X509_TOKEN)
    {
        rp_x509_token_increment_ref((rp_x509_token_t *) value, env);
    }
    neethi_assertion->value = (void *) value;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_element_t *AXIS2_CALL
neethi_assertion_get_element(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return neethi_assertion->element;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_assertion_set_element(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env,
    axiom_element_t *element)
{
    neethi_assertion->element = element;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
neethi_assertion_get_node(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    return neethi_assertion->node;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_assertion_set_node(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env,
    axiom_node_t * node)
{
    neethi_assertion->node = node;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
neethi_assertion_get_is_optional(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    return neethi_assertion->is_optional;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_assertion_set_is_optional(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env,
    axis2_bool_t is_optional)
{
    neethi_assertion->is_optional = is_optional;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
neethi_assertion_get_policy_components(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    return neethi_assertion->policy_components;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_assertion_add_policy_components(
    neethi_assertion_t *neethi_assertion,
    axutil_array_list_t *arraylist,
    const axutil_env_t *env)
{

    int size = axutil_array_list_size(arraylist, env);
    int i = 0;

    if (axutil_array_list_ensure_capacity
        (neethi_assertion->policy_components, env, size + 1) != AXIS2_SUCCESS)
        return AXIS2_FAILURE;

    for (i = 0; i < size; i++)
    {
        void *value = NULL;
        value = axutil_array_list_get(arraylist, env, i);
        neethi_operator_increment_ref((neethi_operator_t *) value, env);
        axutil_array_list_add(neethi_assertion->policy_components, env, value);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_assertion_add_operator(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env,
    neethi_operator_t *operator)
{
    neethi_operator_increment_ref(operator, env);
    axutil_array_list_add(neethi_assertion->policy_components, env, operator);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
neethi_assertion_is_empty(
    neethi_assertion_t *neethi_assertion,
    const axutil_env_t *env)
{
    return axutil_array_list_is_empty(neethi_assertion->policy_components, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
neethi_assertion_serialize(
    neethi_assertion_t *assertion,
    axiom_node_t *parent,
    const axutil_env_t *env)
{
    axiom_namespace_t *namespace = NULL;
    axiom_element_t *element = NULL;
    axiom_node_t *node = NULL;
    axis2_char_t *localname = NULL;

    namespace =
        axiom_element_get_namespace(assertion->element, env, assertion->node);
    localname = axiom_element_get_localname(assertion->element, env);

    element = axiom_element_create(env, parent, localname, namespace, &node);

    if (!node)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
