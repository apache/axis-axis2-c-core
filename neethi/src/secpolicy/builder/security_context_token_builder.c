
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

#include <rp_security_context_token_builder.h>
#include <neethi_operator.h>
#include <neethi_policy.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>

/*private functions*/

axis2_status_t AXIS2_CALL security_context_token_process_alternatives(
    const axutil_env_t * env,
    neethi_all_t * all,
    rp_security_context_token_t * security_context_token);

/***********************************/

AXIS2_EXTERN neethi_assertion_t *AXIS2_CALL
rp_security_context_token_builder_build(
    const axutil_env_t * env,
    axiom_node_t * node,
    axiom_element_t * element,
    axis2_bool_t is_secure_conversation_token)
{
    rp_security_context_token_t *security_context_token = NULL;
    neethi_policy_t *policy = NULL;
    axiom_node_t *child_node = NULL;
    axiom_element_t *child_element = NULL;
    axutil_array_list_t *alternatives = NULL;
    neethi_operator_t *component = NULL;
    neethi_all_t *all = NULL;
    axis2_char_t *inclusion_value = NULL;
    axutil_qname_t *qname = NULL;
    neethi_assertion_t *assertion = NULL;
    neethi_policy_t *normalized_policy = NULL;
    /*axutil_array_list_t *temp = NULL; */

    security_context_token = rp_security_context_token_create(env);
    qname = axutil_qname_create(env, RP_INCLUDE_TOKEN, RP_SP_NS, RP_SP_PREFIX);

    inclusion_value = axiom_element_get_attribute_value(element, env, qname);

    axutil_qname_free(qname, env);
    qname = NULL;

    rp_security_context_token_set_inclusion(security_context_token, env, inclusion_value);
    rp_security_context_token_set_is_secure_conversation_token(security_context_token, env, is_secure_conversation_token);

    child_node = axiom_node_get_first_element(node, env);
    if (!child_node)
    {
        return NULL;
    }

    if (axiom_node_get_node_type(child_node, env) == AXIOM_ELEMENT)
    {
        child_element =
            (axiom_element_t *) axiom_node_get_data_element(child_node, env);
        if (child_element)
        {
            policy = neethi_engine_get_policy(env, child_node, child_element);
            if (!policy)
            {
                return NULL;
            }
            normalized_policy =
                neethi_engine_get_normalize(env, AXIS2_FALSE, policy);
            neethi_policy_free(policy, env);
            policy = NULL;
            alternatives =
                neethi_policy_get_alternatives(normalized_policy, env);
            component =
                (neethi_operator_t *) axutil_array_list_get(alternatives, env,
                                                            0);
            all = (neethi_all_t *) neethi_operator_get_value(component, env);
            security_context_token_process_alternatives(env, all, security_context_token);

            assertion =
                neethi_assertion_create_with_args(env,
                                                  (void *) rp_security_context_token_free,
                                                  security_context_token,
                                                  ASSERTION_TYPE_SECURITY_CONTEXT_TOKEN);

            neethi_policy_free(normalized_policy, env);
            normalized_policy = NULL;

            return assertion;
        }
        else
            return NULL;
    }
    else
        return NULL;
}

axis2_status_t AXIS2_CALL
security_context_token_process_alternatives(
    const axutil_env_t * env,
    neethi_all_t * all,
    rp_security_context_token_t * security_context_token)
{
    neethi_operator_t *operator = NULL;
    axutil_array_list_t *arraylist = NULL;
    neethi_assertion_t *assertion = NULL;
    neethi_assertion_type_t type;

    int i = 0;

    arraylist = neethi_all_get_policy_components(all, env);

    for (i = 0; i < axutil_array_list_size(arraylist, env); i++)
    {
        operator =(neethi_operator_t *) axutil_array_list_get(arraylist, env,
                                                              i);
        assertion =
            (neethi_assertion_t *) neethi_operator_get_value(operator, env);
        type = neethi_assertion_get_type(assertion, env);

        if(type == ASSERTION_TYPE_REQUIRE_DERIVED_KEYS)
        {
            rp_security_context_token_set_derivedkeys(security_context_token, env, 
                                         AXIS2_TRUE);
        }    
        else if(type == ASSERTION_TYPE_REQUIRE_EXTERNAL_URI)
        {
            rp_security_context_token_set_require_external_uri_ref(security_context_token, env,
                                                     AXIS2_TRUE);
        }
        else if(type == ASSERTION_TYPE_SC10_SECURITY_CONTEXT_TOKEN)
        {
            rp_security_context_token_set_sc10_security_context_token(security_context_token, env,
                                                     AXIS2_TRUE);
        }
        else if(type == ASSERTION_TYPE_ISSUER)
        {
            axis2_char_t* issuer = NULL;
            issuer = (axis2_char_t *)neethi_assertion_get_value(assertion, env);
            rp_security_context_token_set_issuer(security_context_token, env, issuer);
        }
        else if(type == ASSERTION_TYPE_BOOTSTRAP_POLICY)
        {
            neethi_policy_t *bootstrap_policy = NULL;
            bootstrap_policy = (neethi_policy_t *)neethi_assertion_get_value(assertion, env);
            rp_security_context_token_set_bootstrap_policy(security_context_token, env, bootstrap_policy);
        }
        else
            return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}