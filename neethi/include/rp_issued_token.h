
/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RP_ISSUED_TOKEN_H
#define RP_ISSUED_TOKEN_H

/** @defgroup trust10
 * @ingroup trust10
 * @{
 */

#include <rp_includes.h>
#include <axutil_utils.h>
#include <neethi_operator.h>
#include <neethi_policy.h>
#include <neethi_exactlyone.h>
#include <neethi_all.h>
#include <neethi_engine.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
	typedef struct rp_issued_token rp_issued_token_t;
	
	AXIS2_EXTERN rp_issued_token_t * AXIS2_CALL
	rp_issued_token_create(
			const axutil_env_t *env);
	
	AXIS2_EXTERN void AXIS2_CALL
	rp_issued_token_free(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
	AXIS2_EXTERN axis2_char_t * AXIS2_CALL
	rp_issued_token_get_inclusion(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	rp_issued_token_set_inclusion(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env,
			axis2_char_t *inclusion);
	
	AXIS2_EXTERN axiom_node_t * AXIS2_CALL
	rp_issued_token_get_issuer_epr(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	rp_issued_token_set_issuer_epr(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env,
			axiom_node_t *issuer_epr);
	
	AXIS2_EXTERN axiom_node_t * AXIS2_CALL
	rp_issued_token_get_requested_sec_token_template(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	rp_issued_token_set_requested_sec_token_template(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env,
			axiom_node_t *req_sec_token_template);
	
	AXIS2_EXTERN axis2_bool_t AXIS2_CALL
	rp_issued_token_get_derivedkeys(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	rp_issued_token_set_derivedkeys(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env,
			axis2_bool_t derivedkeys);
	
	AXIS2_EXTERN axis2_bool_t AXIS2_CALL
	rp_issued_token_get_require_external_reference(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	rp_issued_token_set_require_exernal_reference(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env,
			axis2_bool_t require_external_reference);
	
	AXIS2_EXTERN axis2_bool_t AXIS2_CALL
	rp_issued_token_get_require_internal_reference(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	rp_issued_token_set_require_internal_reference(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env,
			axis2_bool_t require_internal_reference);
	
	AXIS2_EXTERN axis2_status_t AXIS2_CALL
	rp_issued_token_increment_ref(
			rp_issued_token_t *issued_token,
			const axutil_env_t *env);
	
#ifdef __cplusplus
}
#endif
#endif
