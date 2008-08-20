
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

#ifndef RP_SECURITY_CONTEXT_TOKEN_H
#define RP_SECURITY_CONTEXT_TOKEN_H

/** @defgroup rp_security_context_token
 * @ingroup rp_security_context_token
 * @{
 */

#include <rp_includes.h>
#include <neethi_policy.h>
#include <rp_token.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct rp_security_context_token_t rp_security_context_token_t;

    AXIS2_EXTERN rp_security_context_token_t *AXIS2_CALL
    rp_security_context_token_create(
        const axutil_env_t * env);

    AXIS2_EXTERN void AXIS2_CALL
    rp_security_context_token_free(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rp_security_context_token_get_inclusion(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_inclusion(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env,
        axis2_char_t * inclusion);

    AXIS2_EXTERN derive_key_type_t AXIS2_CALL
    rp_security_context_token_get_derivedkey(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_derivedkey(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env,
        derive_key_type_t derivedkey);

    AXIS2_EXTERN derive_key_version_t AXIS2_CALL
    rp_security_context_token_get_derivedkey_version(
        rp_security_context_token_t *security_context_token,
        const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_derivedkey_version(
        rp_security_context_token_t *security_context_token,
        const axutil_env_t *env,
        derive_key_version_t version);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_security_context_token_get_require_external_uri_ref(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_require_external_uri_ref(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env,
        axis2_bool_t require_external_uri_ref);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_security_context_token_get_sc10_security_context_token(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_sc10_security_context_token(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env,
        axis2_bool_t sc10_security_context_token);

    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    rp_security_context_token_get_issuer(
         rp_security_context_token_t *security_context_token, 
         const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_issuer(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env,
        axis2_char_t *issuer);

    AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
    rp_security_context_token_get_bootstrap_policy(
         rp_security_context_token_t *security_context_token, 
         const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_bootstrap_policy(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env,
        neethi_policy_t *bootstrap_policy);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    rp_security_context_token_get_is_secure_conversation_token(
         rp_security_context_token_t *security_context_token, 
         const axutil_env_t *env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_set_is_secure_conversation_token(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env,
        axis2_bool_t is_secure_conversation_token);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    rp_security_context_token_increment_ref(
        rp_security_context_token_t * security_context_token,
        const axutil_env_t * env);

#ifdef __cplusplus
}
#endif
#endif
