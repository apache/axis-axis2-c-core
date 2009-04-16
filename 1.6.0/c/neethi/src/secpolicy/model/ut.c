
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

#include <rp_username_token.h>
#include <rp_token.h>

struct rp_username_token_t
{
    axis2_char_t *inclusion;
    password_type_t password_type;
    axis2_bool_t useUTprofile10;
    axis2_bool_t useUTprofile11;
    rp_token_t *token;
    int ref;
};

AXIS2_EXTERN rp_username_token_t *AXIS2_CALL
rp_username_token_create(
    const axutil_env_t * env)
{
    rp_username_token_t *username_token = NULL;
    username_token = (rp_username_token_t *) AXIS2_MALLOC(
        env->allocator, sizeof (rp_username_token_t));

    if (!username_token)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] User name token creation failed. Insufficient memory");
        return NULL;
    }

    username_token->token = rp_token_create(env);
    if(!username_token->token)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "[neethi] User name token creation failed.");
        rp_username_token_free(username_token, env);
        return NULL;
    }

    username_token->inclusion = RP_INCLUDE_ALWAYS;
    username_token->password_type = PASSWORD_PLAIN;
    username_token->useUTprofile10 = AXIS2_TRUE;
    username_token->useUTprofile11 = AXIS2_FALSE;
    username_token->ref = 0;

    return username_token;
}

AXIS2_EXTERN void AXIS2_CALL
rp_username_token_free(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    if(username_token)
    {
        if (--(username_token->ref) > 0)
        {
            return;
        }

        rp_token_free(username_token->token, env);
        AXIS2_FREE(env->allocator, username_token);
        username_token = NULL;
    }
}

/* Implementations */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_username_token_get_inclusion(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    return username_token->inclusion;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_set_inclusion(
    rp_username_token_t * username_token,
    const axutil_env_t * env,
    axis2_char_t * inclusion)
{
    AXIS2_PARAM_CHECK(env->error, inclusion, AXIS2_FAILURE);
    username_token->inclusion = inclusion;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_username_token_get_useUTprofile10(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    return username_token->useUTprofile10;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_set_useUTprofile10(
    rp_username_token_t * username_token,
    const axutil_env_t * env,
    axis2_bool_t useUTprofile10)
{
    username_token->useUTprofile10 = useUTprofile10;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_username_token_get_useUTprofile11(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    return username_token->useUTprofile10;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_set_useUTprofile11(
    rp_username_token_t * username_token,
    const axutil_env_t * env,
    axis2_bool_t useUTprofile11)
{
    username_token->useUTprofile11 = useUTprofile11;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_increment_ref(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    username_token->ref++;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_username_token_get_issuer(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    return rp_token_get_issuer(username_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_set_issuer(
    rp_username_token_t * username_token,
    const axutil_env_t * env,
    axis2_char_t * issuer)
{
    return rp_token_set_issuer(username_token->token, env, issuer);
}

AXIS2_EXTERN derive_key_type_t AXIS2_CALL
rp_username_token_get_derivedkey_type(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    return rp_token_get_derivedkey_type(username_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_set_derivedkey_type(
    rp_username_token_t * username_token,
    const axutil_env_t * env,
    derive_key_type_t derivedkey)
{
    return rp_token_set_derivedkey_type(username_token->token, env, derivedkey);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_username_token_get_is_issuer_name(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    return rp_token_get_is_issuer_name(username_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_set_is_issuer_name(
    rp_username_token_t * username_token,
    const axutil_env_t * env,
    axis2_bool_t is_issuer_name)
{
    return rp_token_set_is_issuer_name(username_token->token, env, is_issuer_name);
}

AXIS2_EXTERN axiom_node_t *AXIS2_CALL
rp_username_token_get_claim(
    rp_username_token_t * username_token,
    const axutil_env_t * env)
{
    return rp_token_get_claim(username_token->token, env);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_username_token_set_claim(
    rp_username_token_t * username_token,
    const axutil_env_t * env,
    axiom_node_t *claim)
{
    return rp_token_set_claim(username_token->token, env, claim);
}
