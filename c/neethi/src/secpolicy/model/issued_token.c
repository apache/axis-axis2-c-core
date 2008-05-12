
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

#include <rp_issued_token.h>

struct rp_issued_token {
	axis2_char_t *inclusion;
	axiom_node_t *issuer_epr;
	axiom_node_t *requested_sec_token_template;
	axis2_bool_t derivedkeys;
	axis2_bool_t require_external_reference;
	axis2_bool_t require_internal_reference;
	int ref;
};

AXIS2_EXTERN rp_issued_token_t * AXIS2_CALL
rp_issued_token_create(
		const axutil_env_t *env)
{
	rp_issued_token_t *issued_token = NULL;
	
	issued_token = (rp_issued_token_t*)AXIS2_MALLOC(env->allocator, sizeof(rp_issued_token_t));
	
	if (issued_token == NULL)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
	    return NULL;
	}
	
	issued_token->inclusion = NULL;
	issued_token->issuer_epr = NULL;
	issued_token->requested_sec_token_template = NULL;
	issued_token->derivedkeys = AXIS2_FALSE;
	issued_token->require_external_reference = AXIS2_FALSE;
	issued_token->require_internal_reference = AXIS2_FALSE;
	issued_token->ref = 0;
	
	return issued_token;	
}

AXIS2_EXTERN void AXIS2_CALL
rp_issued_token_free(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	if(issued_token)
	{
		if(--(issued_token->ref) > 0)
		{
			return;
		}
		
		AXIS2_FREE(env->allocator, issued_token);
		issued_token = NULL;
	}
	return;
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
rp_issued_token_get_inclusion(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	return issued_token->inclusion;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_issued_token_set_inclusion(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env,
		axis2_char_t *inclusion)
{
	if(inclusion)
	{
		issued_token->inclusion = inclusion;
		return AXIS2_SUCCESS;		
	}
	
	return AXIS2_FAILURE;
}

AXIS2_EXTERN axiom_node_t * AXIS2_CALL
rp_issued_token_get_issuer_epr(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	return issued_token->issuer_epr;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_issued_token_set_issuer_epr(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env,
		axiom_node_t *issuer_epr)
{
	if(issuer_epr)
	{
		issued_token->issuer_epr = issuer_epr;
		return AXIS2_SUCCESS;
	}
	
	return AXIS2_FAILURE;
}

AXIS2_EXTERN axiom_node_t * AXIS2_CALL
rp_issued_token_get_requested_sec_token_template(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	return issued_token->requested_sec_token_template;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_issued_token_set_requested_sec_token_template(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env,
		axiom_node_t *req_sec_token_template)
{
	if(req_sec_token_template)
	{
		issued_token->requested_sec_token_template = req_sec_token_template;
		return AXIS2_SUCCESS;
	}
	
	return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_issued_token_get_derivedkeys(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	return issued_token->derivedkeys;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_issued_token_set_derivedkeys(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env,
		axis2_bool_t derivedkeys)
{
	AXIS2_PARAM_CHECK(env->error, derivedkeys, AXIS2_FAILURE);
	issued_token->derivedkeys = derivedkeys;
	
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_issued_token_get_require_external_reference(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	return issued_token->require_external_reference;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_issued_token_set_require_exernal_reference(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env,
		axis2_bool_t require_external_reference)
{
	AXIS2_PARAM_CHECK(env->error, require_external_reference, AXIS2_FAILURE);
	issued_token->require_external_reference = require_external_reference;
	
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_issued_token_get_require_internal_reference(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	return issued_token->require_internal_reference;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_issued_token_set_require_internal_reference(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env,
		axis2_bool_t require_internal_reference)
{
	AXIS2_PARAM_CHECK(env->error, require_internal_reference, AXIS2_FAILURE);
	issued_token->require_internal_reference = require_internal_reference;
	
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_issued_token_increment_ref(
		rp_issued_token_t *issued_token,
		const axutil_env_t *env)
{
	issued_token->ref++;
	return AXIS2_SUCCESS;
}

