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

#include <rp_issued_token_builder.h>

AXIS2_EXTERN neethi_assertion_t * AXIS2_CALL
rp_issued_token_builder_build(const axutil_env_t *env,
		axiom_node_t *node, 
		axiom_element_t *element) 
{
	rp_issued_token_t *issued_token= NULL;
	neethi_policy_t *policy= NULL;
	neethi_policy_t *normalized_policy= NULL;
	neethi_all_t *all= NULL;
	axutil_array_list_t *alternatives= NULL;
	neethi_operator_t *component= NULL;
	axis2_char_t *inclusion_value= NULL;
	axutil_qname_t *qname= NULL;
	axiom_node_t *issuer_node= NULL;
	axiom_element_t *issuer_ele= NULL;
	axiom_element_t *issuer_first_child_ele= NULL;
	axiom_node_t *issuer_first_child_node= NULL;
	axiom_node_t *req_sec_tok_template_node= NULL;
	axiom_element_t *req_sec_tok_template_ele= NULL;
	axiom_node_t *policy_node= NULL;
	axiom_element_t *policy_ele= NULL;
	neethi_assertion_t *assertion= NULL;

	issued_token = rp_issued_token_create(env);
	qname = axutil_qname_create(env, RP_INCLUDE_TOKEN, RP_SP_NS_11, RP_SP_PREFIX);
	inclusion_value = axiom_element_get_attribute_value(element, env, qname);
	axutil_qname_free(qname, env);
	qname = NULL;

    if(!inclusion_value)
    {
        /* we can try whether WS-SP1.2 specific inclusion value */
        qname = axutil_qname_create(env, RP_INCLUDE_TOKEN, RP_SP_NS_12, RP_SP_PREFIX);
        inclusion_value = axiom_element_get_attribute_value(element, env, qname);
        axutil_qname_free(qname, env);
        qname = NULL;
    }

	if (inclusion_value) 
	{
		rp_issued_token_set_inclusion(issued_token, env, inclusion_value);
	}

	qname = axutil_qname_create(env, RP_ISSUER, RP_SP_NS_11, RP_SP_PREFIX);
	issuer_ele = axiom_element_get_first_child_with_qname(element, env, qname,
			node, &issuer_node);
	if (issuer_ele) 
	{
		issuer_first_child_ele = axiom_element_get_first_element(issuer_ele,
				env, issuer_node, &issuer_first_child_node);
		if (issuer_first_child_ele) 
		{
			rp_issued_token_set_issuer_epr(issued_token, env,
					issuer_first_child_node);
		}
	}
	axutil_qname_free(qname, env);
	qname = NULL;

	qname = axutil_qname_create(env, RP_REQUEST_SEC_TOKEN_TEMPLATE, RP_SP_NS_11,
			RP_SP_PREFIX);
	req_sec_tok_template_ele = axiom_element_get_first_child_with_qname(
			element, env, qname, node, &req_sec_tok_template_node);
	if (req_sec_tok_template_ele) 
	{
		rp_issued_token_set_requested_sec_token_template(issued_token, env,
				req_sec_tok_template_node);
	} 
	else 
	{
		return NULL;
	}

	axutil_qname_free(qname, env);
	qname = NULL;

	qname = axutil_qname_create(env, RP_POLICY, RP_POLICY_NS, RP_POLICY_PREFIX);
	policy_ele = axiom_element_get_first_child_with_qname(element, env, qname,
			node, &policy_node);
	if (policy_ele) 
	{
		policy = neethi_engine_get_policy(env, policy_node, policy_ele);
		if (!policy)
		{
			return NULL;
		}
		normalized_policy = neethi_engine_get_normalize(env, AXIS2_FALSE,
				policy);
		neethi_policy_free(policy, env);
		policy = NULL;
		alternatives = neethi_policy_get_alternatives(normalized_policy, env);
		component = (neethi_operator_t *) axutil_array_list_get(alternatives,
				env, 0);
		all = (neethi_all_t *) neethi_operator_get_value(component, env);
		if(AXIS2_FAILURE == rp_issued_token_builder_process_alternatives(env, all, issued_token))
			return NULL;
		assertion = neethi_assertion_create_with_args(env,
				(AXIS2_FREE_VOID_ARG)rp_issued_token_free, issued_token,
				ASSERTION_TYPE_ISSUED_TOKEN);

		neethi_policy_free(normalized_policy, env);
		normalized_policy = NULL;

		return assertion;
	} 

	assertion = neethi_assertion_create(env);
   	neethi_assertion_set_value(
				   	assertion,
				   	env, 
					issued_token, 
					ASSERTION_TYPE_ISSUED_TOKEN);


	return assertion;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL rp_issued_token_builder_process_alternatives(
		const axutil_env_t *env, neethi_all_t *all,
		rp_issued_token_t *issued_token) {
	neethi_operator_t *operator= NULL;
	axutil_array_list_t *arraylist= NULL;
	neethi_assertion_t *assertion= NULL;
	neethi_assertion_type_t type;

	int i = 0;

	arraylist = neethi_all_get_policy_components(all, env);

	for (i = 0; i < axutil_array_list_size(arraylist, env); i++) 
	{
		operator
				=(neethi_operator_t *) axutil_array_list_get(arraylist, env, i);
		assertion = (neethi_assertion_t *) neethi_operator_get_value(operator,
				env);
		type = neethi_assertion_get_type(assertion, env);

		if (type == ASSERTION_TYPE_REQUIRE_DERIVED_KEYS_SC10) 
		{
			rp_issued_token_set_derivedkeys(issued_token, env, AXIS2_TRUE);
		} 
		else if (type == ASSERTION_TYPE_REQUIRE_EXTERNAL_REFERENCE)
		{
			rp_issued_token_set_require_exernal_reference(issued_token, env, AXIS2_TRUE);
		}
		else if (type == ASSERTION_TYPE_REQUIRE_INTERNAL_REFERENCE)
		{
			rp_issued_token_set_require_internal_reference(issued_token, env, AXIS2_TRUE);			
		}
		else
			return AXIS2_FAILURE;
	}
	return AXIS2_SUCCESS;
}

