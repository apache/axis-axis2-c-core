
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

#include <rp_token.h>

struct rp_token_t
{
    axis2_char_t *issuer;
    axis2_char_t *inclusion;
    axiom_node_t *claim;
    axis2_bool_t is_issuer_name; /* shows whether 'issuer' points to issuer name or end point */
    derive_key_type_t derive_key;
    derive_key_version_t derive_key_version;
    int ref;
};

AXIS2_EXTERN rp_token_t *AXIS2_CALL
rp_token_create(
    const axutil_env_t * env)
{
    rp_token_t *token = NULL;
    token = (rp_token_t *) AXIS2_MALLOC(env->allocator, sizeof (rp_token_t));

    if (!token)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] Token creation failed. Insufficient memory");
        return NULL;
    }
    token->issuer = NULL;
    token->is_issuer_name = AXIS2_FALSE;
    token->claim = NULL;
    token->derive_key = DERIVEKEY_NONE;
    token->derive_key_version = DERIVEKEY_VERSION_SC13;
    token->inclusion = RP_INCLUDE_ALWAYS_SP12;
    token->ref = 0;

    return token;
}

AXIS2_EXTERN void AXIS2_CALL
rp_token_free(
    rp_token_t * token,
    const axutil_env_t * env)
{
    if (token)
    {
        if (--(token->ref) > 0)
        {
            return;
        }

        AXIS2_FREE(env->allocator, token);
        token = NULL;
    }
}

/* Implementations */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_token_get_issuer(
    rp_token_t * token,
    const axutil_env_t * env)
{
    return token->issuer;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_token_set_issuer(
    rp_token_t * token,
    const axutil_env_t * env,
    axis2_char_t * issuer)
{
    token->issuer = issuer;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_token_set_derive_key_version(
    rp_token_t *token, 
    const axutil_env_t *env, 
    derive_key_version_t version)
{
    token->derive_key_version = version;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN derive_key_version_t AXIS2_CALL
rp_token_get_derive_key_version(
    rp_token_t *token, 
    const axutil_env_t *env)
{
    return token->derive_key_version;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_token_set_inclusion(
    rp_token_t *token, 
    const axutil_env_t *env, 
    axis2_char_t *inclusion)
{
    token->inclusion = inclusion;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_token_get_inclusion(
    rp_token_t *token, 
    const axutil_env_t *env)
{
    return token->inclusion;
}

AXIS2_EXTERN derive_key_type_t AXIS2_CALL
rp_token_get_derivedkey_type(
    rp_token_t * token,
    const axutil_env_t * env)
{
    return token->derive_key;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_token_set_derivedkey_type(
    rp_token_t * token,
    const axutil_env_t * env,
    derive_key_type_t derivedkey)
{
    token->derive_key = derivedkey;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_token_get_is_issuer_name(
    rp_token_t * token,
    const axutil_env_t * env)
{
    return token->is_issuer_name;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_token_set_is_issuer_name(
    rp_token_t * token,
    const axutil_env_t * env,
    axis2_bool_t is_issuer_name)
{
    token->is_issuer_name = is_issuer_name;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
rp_token_get_claim(
    rp_token_t * token,
    const axutil_env_t * env)
{
    return token->claim;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_token_set_claim(
    rp_token_t * token,
    const axutil_env_t * env,
    axiom_node_t *claim)
{
    token->claim = claim;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_token_increment_ref(
    rp_token_t * token,
    const axutil_env_t * env)
{
    token->ref++;
    return AXIS2_SUCCESS;
}
