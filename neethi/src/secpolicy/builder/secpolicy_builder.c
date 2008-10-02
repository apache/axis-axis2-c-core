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

#include <rp_secpolicy_builder.h>
#include <neethi_operator.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>
#include <neethi_assertion.h>

/*private functions*/

axis2_status_t AXIS2_CALL secpolicy_process_alternatives(
    const axutil_env_t *env,
    neethi_all_t *all,
    rp_secpolicy_t *secpolicy);

/***********************************/

AXIS2_EXTERN rp_secpolicy_t *AXIS2_CALL
rp_secpolicy_builder_build(
    const axutil_env_t *env,
    neethi_policy_t *policy)
{
    axutil_array_list_t *alternatives = NULL;
    neethi_operator_t *component = NULL;
    neethi_all_t *all = NULL;
    rp_secpolicy_t *secpolicy = NULL;

    secpolicy = rp_secpolicy_create(env);

    alternatives = neethi_policy_get_alternatives(policy, env);

    component =
        (neethi_operator_t *) axutil_array_list_get(alternatives, env, 0);
    all = (neethi_all_t *) neethi_operator_get_value(component, env);

    if (secpolicy_process_alternatives(env, all, secpolicy) == AXIS2_FAILURE)
    {
        return NULL;
    }
    return secpolicy;
}

axis2_status_t AXIS2_CALL
secpolicy_process_alternatives(
    const axutil_env_t *env,
    neethi_all_t *all,
    rp_secpolicy_t *secpolicy)
{

    neethi_operator_t *operator = NULL;
    axutil_array_list_t *arraylist = NULL;
    neethi_assertion_t *assertion = NULL;
    neethi_assertion_type_t type;
    void *value = NULL;

    int i = 0;

    arraylist = neethi_all_get_policy_components(all, env);

    for (i = 0; i < axutil_array_list_size(arraylist, env); i++)
    {
        operator =(neethi_operator_t *) axutil_array_list_get(arraylist, env,
                                                              i);
        assertion =
            (neethi_assertion_t *) neethi_operator_get_value(operator, env);
        value = neethi_assertion_get_value(assertion, env);
        type = neethi_assertion_get_type(assertion, env);

        if (value)
        {
            if (type == ASSERTION_TYPE_TRANSPORT_BINDING)
            {
                rp_property_t *binding = NULL;
                rp_transport_binding_t *transport_binding = NULL;

                transport_binding =
                    (rp_transport_binding_t *)
                    neethi_assertion_get_value(assertion, env);
                if (!transport_binding)
                {
                    return AXIS2_FAILURE;
                }
                binding = rp_property_create(env);
                rp_property_set_value(binding, env, transport_binding,
                                      RP_PROPERTY_TRANSPORT_BINDING);
                rp_secpolicy_set_binding(secpolicy, env, binding);
            }
            else if (type == ASSERTION_TYPE_ASSYMMETRIC_BINDING)
            {
                rp_property_t *binding = NULL;
                rp_asymmetric_binding_t *asymmetric_binding = NULL;

                asymmetric_binding =
                    (rp_asymmetric_binding_t *)
                    neethi_assertion_get_value(assertion, env);
                if (!asymmetric_binding)
                {
                    return AXIS2_FAILURE;
                }
                binding = rp_property_create(env);
                rp_property_set_value(binding, env, asymmetric_binding,
                                      RP_PROPERTY_ASYMMETRIC_BINDING);
                rp_secpolicy_set_binding(secpolicy, env, binding);
            }
            else if (type == ASSERTION_TYPE_SYMMETRIC_BINDING)
            {
                rp_property_t *binding = NULL;
                rp_symmetric_binding_t *symmetric_binding = NULL;

                symmetric_binding =
                    (rp_symmetric_binding_t *)
                    neethi_assertion_get_value(assertion, env);
                if (!symmetric_binding)
                {
                    return AXIS2_FAILURE;
                }
                binding = rp_property_create(env);
                rp_property_set_value(binding, env, symmetric_binding,
                                      RP_PROPERTY_SYMMETRIC_BINDING);
                rp_secpolicy_set_binding(secpolicy, env, binding);
            }
            else if (type == ASSERTION_TYPE_SUPPORTING_TOKENS)
            {
                rp_supporting_tokens_t *supporting_tokens = NULL;
                supporting_tokens =
                    (rp_supporting_tokens_t *)
                    neethi_assertion_get_value(assertion, env);
                if (supporting_tokens)
                {
                    rp_property_type_t type;
                    type =
                        rp_supporting_tokens_get_type(supporting_tokens, env);
                    if (type == RP_PROPERTY_SIGNED_SUPPORTING_TOKEN)
                    {
                        rp_secpolicy_set_signed_supporting_tokens(secpolicy,
                                                                  env,
                                                                  supporting_tokens);
                    }
                    else if (type ==
                             RP_PROPERTY_SIGNED_ENDORSING_SUPPORTING_TOKEN)
                    {
                        rp_secpolicy_set_signed_endorsing_supporting_tokens(secpolicy,
                                                                     env,
                                                                     supporting_tokens);
                    }
                    else if (type == RP_PROPERTY_SUPPORTING_SUPPORTING_TOKEN)
                    {
                        rp_secpolicy_set_supporting_tokens(secpolicy,
                                                                  env,
                                                                  supporting_tokens);
                    }
                    else if (type ==
                             RP_PROPERTY_ENDORSING_SUPPORTING_TOKEN)
                    {
                        rp_secpolicy_set_endorsing_supporting_tokens(secpolicy,
                                                                     env,
                                                                     supporting_tokens);
                    }
                    else
                        return AXIS2_FAILURE;
                }
                else
                    return AXIS2_FAILURE;
            }
            else if (type == ASSERTION_TYPE_WSS10)
            {
                rp_wss10_t *wss10 = NULL;
                rp_property_t *wss = NULL;
                wss10 =
                    (rp_wss10_t *) neethi_assertion_get_value(assertion, env);
                if (!wss10)
                {
                    return AXIS2_FAILURE;
                }
                wss = rp_property_create(env);
                rp_property_set_value(wss, env, wss10, RP_PROPERTY_WSS10);
                rp_secpolicy_set_wss(secpolicy, env, wss);
            }
            else if (type == ASSERTION_TYPE_WSS11)
            {
                rp_wss11_t *wss11 = NULL;
                rp_property_t *wss = NULL;
                wss11 =
                    (rp_wss11_t *) neethi_assertion_get_value(assertion, env);
                if (!wss11)
                {
                    return AXIS2_FAILURE;
                }
                wss = rp_property_create(env);
                rp_property_set_value(wss, env, wss11, RP_PROPERTY_WSS11);
                rp_secpolicy_set_wss(secpolicy, env, wss);
            }
            else if (type == ASSERTION_TYPE_TRUST10)
            {
                rp_trust10_t *trust10 = NULL;
                trust10 =
                    (rp_trust10_t *) neethi_assertion_get_value(assertion, env);
                if (!trust10)
                {
                    return AXIS2_FAILURE;
                }
                rp_secpolicy_set_trust10(secpolicy, env, trust10);
            }            
            else if (type == ASSERTION_TYPE_SIGNED_ENCRYPTED_PARTS)
            {
                rp_signed_encrypted_parts_t *signed_encrypted_parts = NULL;
                signed_encrypted_parts =
                    (rp_signed_encrypted_parts_t *)
                    neethi_assertion_get_value(assertion, env);
                if (signed_encrypted_parts)
                {
                    if (rp_signed_encrypted_parts_get_signedparts
                        (signed_encrypted_parts, env))
                    {
                        rp_secpolicy_set_signed_parts(secpolicy, env,
                                                      signed_encrypted_parts);
                    }
                    else
                    {
                        rp_secpolicy_set_encrypted_parts(secpolicy, env,
                                                         signed_encrypted_parts);
                    }
                }
                else
                    return AXIS2_FAILURE;

            }
            else if (type == ASSERTION_TYPE_RAMPART_CONFIG)
            {
                rp_rampart_config_t *rampart_config = NULL;
                rampart_config =
                    (rp_rampart_config_t *)
                    neethi_assertion_get_value(assertion, env);
                if (!rampart_config)
                {
                    return AXIS2_FAILURE;
                }
                rp_secpolicy_set_rampart_config(secpolicy, env, rampart_config);
            }
            else
            {
                continue;
            }
        }
        else
            return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
