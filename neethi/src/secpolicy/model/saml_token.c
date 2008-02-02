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

#include <rp_saml_token.h>

struct rp_saml_token
{
    axis2_char_t *inclusion;
    axis2_bool_t derivedkeys;
    axis2_bool_t require_key_identifier_reference;
    axis2_char_t *token_version_and_type;
    int ref;
};

AXIS2_EXTERN rp_saml_token_t * AXIS2_CALL
rp_saml_token_create(
    const axutil_env_t *env)
{
    rp_saml_token_t * saml_token;
    
    saml_token = (rp_saml_token_t*)AXIS2_MALLOC(env->allocator, sizeof(rp_saml_token_t));
    
    if (saml_token == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    saml_token->inclusion = NULL;
    saml_token->derivedkeys = AXIS2_FALSE;
    saml_token->require_key_identifier_reference = AXIS2_FALSE;
    saml_token->token_version_and_type = NULL;
    saml_token->ref = 0;
    
    return saml_token;    
}

AXIS2_EXTERN void AXIS2_CALL
rp_saml_token_free(
    rp_saml_token_t *saml_token,
    const axutil_env_t *env)
{
    if(saml_token)
    {
        if(--(saml_token->ref) > 0)
        {
            return;
        }
        
        AXIS2_FREE(env->allocator, saml_token);
        saml_token = NULL;
    }
    
    return;
}

AXIS2_EXTERN axis2_char_t * AXIS2_CALL
rp_saml_token_get_inclusion(
        rp_saml_token_t *saml_token,
        const axutil_env_t *env)
{
    return saml_token->inclusion;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_saml_token_set_inclusion(
        rp_saml_token_t *saml_token,
        const axutil_env_t *env,
        axis2_char_t * inclusion)
{
    AXIS2_PARAM_CHECK(env->error, inclusion, AXIS2_FAILURE);
    saml_token->inclusion = inclusion;
    
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_saml_token_get_derivedkeys(
		rp_saml_token_t *saml_token,
		const axutil_env_t *env)
{
	return saml_token->derivedkeys;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_saml_token_set_derivedkeys(
		rp_saml_token_t *saml_token,
		const axutil_env_t *env,
		axis2_bool_t derivedkeys)
{
	AXIS2_PARAM_CHECK(env->error, derivedkeys, AXIS2_FAILURE);
	saml_token->derivedkeys = derivedkeys;
	
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_saml_token_get_require_key_identifier_reference(
    rp_saml_token_t * saml_token,
    const axutil_env_t * env)
{
    return saml_token->require_key_identifier_reference;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_saml_token_set_require_key_identifier_reference(
    rp_saml_token_t * saml_token,
    const axutil_env_t * env,
    axis2_bool_t require_key_identifier_reference)
{
    AXIS2_PARAM_CHECK(env->error, require_key_identifier_reference,
                      AXIS2_FAILURE);
    saml_token->require_key_identifier_reference =
        require_key_identifier_reference;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_saml_token_get_token_version_and_type(
    rp_saml_token_t * saml_token,
    const axutil_env_t * env)
{
    return saml_token->token_version_and_type;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_saml_token_set_token_version_and_type(
    rp_saml_token_t * saml_token,
    const axutil_env_t * env,
    axis2_char_t * token_version_and_type)
{
    AXIS2_PARAM_CHECK(env->error, token_version_and_type, AXIS2_FAILURE);

    saml_token->token_version_and_type = token_version_and_type;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_saml_token_increment_ref(
    rp_saml_token_t * saml_token,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    saml_token->ref++;
    return AXIS2_SUCCESS;
}


