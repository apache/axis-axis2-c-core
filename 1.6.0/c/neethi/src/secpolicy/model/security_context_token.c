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

#include <rp_security_context_token.h>

struct rp_security_context_token_t
{
    rp_token_t *token;
    axis2_bool_t require_external_uri_ref;
    axis2_bool_t sc10_security_context_token;
    neethi_policy_t *bootstrap_policy;
    axis2_bool_t is_secure_conversation_token;
    int ref;
};

AXIS2_EXTERN rp_security_context_token_t *AXIS2_CALL
rp_security_context_token_create(
    const axutil_env_t * env)
{
    rp_security_context_token_t *security_context_token = NULL;

    security_context_token = (rp_security_context_token_t *) 
        AXIS2_MALLOC(env->allocator, sizeof(rp_security_context_token_t));

    if(!security_context_token)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Security context token assertion creation failed. Insufficient memory");
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    security_context_token->ref = 0;
    security_context_token->require_external_uri_ref = AXIS2_FALSE;
    security_context_token->sc10_security_context_token = AXIS2_FALSE;
    security_context_token->bootstrap_policy = NULL;
    security_context_token->is_secure_conversation_token = AXIS2_FALSE;

    security_context_token->token = rp_token_create(env);
    if(!security_context_token->token)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Security context token assertion creation failed.");
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        rp_security_context_token_free(security_context_token, env);
        return NULL;
    }

    return security_context_token;
}

AXIS2_EXTERN void AXIS2_CALL
rp_security_context_token_free(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env)
{
    if (security_context_token)
    {
        if (--(security_context_token->ref) > 0)
        {
            return;
        }

        if(security_context_token->bootstrap_policy)
        {
            neethi_policy_free(security_context_token->bootstrap_policy, env);
        }

        rp_token_free(security_context_token->token, env);
        AXIS2_FREE(env->allocator, security_context_token);
        security_context_token = NULL;
    }
}

/* Implementations */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_security_context_token_get_inclusion(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env)
{
    return rp_token_get_inclusion(security_context_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_inclusion(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env,
    axis2_char_t * inclusion)
{
    return rp_token_set_inclusion(security_context_token->token, env, inclusion);
}

AXIS2_EXTERN derive_key_type_t AXIS2_CALL
rp_security_context_token_get_derivedkey(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env)
{
    return rp_token_get_derivedkey_type(security_context_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_derivedkey(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env,
    derive_key_type_t derivedkey)
{
    return rp_token_set_derivedkey_type(security_context_token->token, env, derivedkey);
}

AXIS2_EXTERN derive_key_version_t AXIS2_CALL
rp_security_context_token_get_derivedkey_version(
    rp_security_context_token_t *security_context_token,
    const axutil_env_t *env)
{
    return rp_token_get_derive_key_version(security_context_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_derivedkey_version(
    rp_security_context_token_t *security_context_token,
    const axutil_env_t *env,
    derive_key_version_t version)
{
    return rp_token_set_derive_key_version(security_context_token->token, env, version);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_security_context_token_get_require_external_uri_ref(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env)
{
    return security_context_token->require_external_uri_ref;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_require_external_uri_ref(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env,
    axis2_bool_t require_external_uri_ref)
{
    security_context_token->require_external_uri_ref = require_external_uri_ref;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_security_context_token_get_sc10_security_context_token(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env)
{
    return security_context_token->sc10_security_context_token;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_sc10_security_context_token(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env,
    axis2_bool_t sc10_security_context_token)
{
    security_context_token->sc10_security_context_token = sc10_security_context_token;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_security_context_token_get_issuer(
     rp_security_context_token_t *security_context_token, 
     const axutil_env_t *env)
{
    return rp_token_get_issuer(security_context_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_issuer(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env,
    axis2_char_t *issuer)
{
    return rp_token_set_issuer(security_context_token->token, env, issuer);
}

AXIS2_EXTERN neethi_policy_t *AXIS2_CALL
rp_security_context_token_get_bootstrap_policy(
     rp_security_context_token_t *security_context_token, 
     const axutil_env_t *env)
{
    return security_context_token->bootstrap_policy;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_bootstrap_policy(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env,
    neethi_policy_t *bootstrap_policy)
{
    security_context_token->bootstrap_policy = bootstrap_policy;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_security_context_token_get_is_secure_conversation_token(
     rp_security_context_token_t *security_context_token, 
     const axutil_env_t *env)
{
    return security_context_token->is_secure_conversation_token;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_set_is_secure_conversation_token(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env,
    axis2_bool_t is_secure_conversation_token)
{
    security_context_token->is_secure_conversation_token = is_secure_conversation_token;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_increment_ref(
    rp_security_context_token_t * security_context_token,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    security_context_token->ref++;
    return AXIS2_SUCCESS;
}
