
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

#include <rp_trust10.h>

struct rp_trust10_t
{
    axis2_bool_t must_support_client_challenge;
    axis2_bool_t must_support_server_challenge;
    axis2_bool_t require_client_entropy;
    axis2_bool_t require_server_entropy;
    axis2_bool_t must_support_issued_token;
    int ref;
};

AXIS2_EXTERN rp_trust10_t *AXIS2_CALL
rp_trust10_create(
    const axutil_env_t * env)
{
    rp_trust10_t *trust10 = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    trust10 = (rp_trust10_t *) AXIS2_MALLOC(env->allocator, sizeof(rp_trust10_t));

    if (trust10 == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    trust10->must_support_client_challenge = AXIS2_FALSE;
    trust10->must_support_server_challenge = AXIS2_FALSE;
    trust10->require_client_entropy = AXIS2_FALSE;
    trust10->require_server_entropy = AXIS2_FALSE;
    trust10->must_support_issued_token = AXIS2_FALSE;
    trust10->ref = 0;

    return trust10;

}

AXIS2_EXTERN void AXIS2_CALL
rp_trust10_free(
    rp_trust10_t * trust10,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (trust10)
    {
        if (--(trust10->ref) > 0)
        {
            return;
        }

        AXIS2_FREE(env->allocator, trust10);
        trust10 = NULL;
    }
    return;
}

/* Implementations */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_trust10_get_must_support_client_challenge(
    rp_trust10_t * trust10,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    return trust10->must_support_client_challenge;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_trust10_set_must_support_client_challenge(
    rp_trust10_t * trust10,
    const axutil_env_t * env,
    axis2_bool_t must_support_client_challenge)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, must_support_client_challenge,
                      AXIS2_FAILURE);
    trust10->must_support_client_challenge = must_support_client_challenge;

    return AXIS2_SUCCESS;

}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_trust10_get_must_support_server_challenge(
    rp_trust10_t * trust10,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    return trust10->must_support_server_challenge;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_trust10_set_must_support_server_challenge(
    rp_trust10_t * trust10,
    const axutil_env_t * env,
    axis2_bool_t must_support_server_challenge)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, must_support_server_challenge,
                      AXIS2_FAILURE);
    trust10->must_support_server_challenge = must_support_server_challenge;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_trust10_get_require_client_entropy(
    rp_trust10_t * trust10,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    return trust10->require_client_entropy;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_trust10_set_require_client_entropy(
    rp_trust10_t * trust10,
    const axutil_env_t * env,
    axis2_bool_t require_client_entropy)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, require_client_entropy, AXIS2_FAILURE);
    trust10->require_client_entropy = require_client_entropy;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_trust10_get_require_server_entropy(
    rp_trust10_t * trust10,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    return trust10->require_server_entropy;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_trust10_set_require_server_entropy(
    rp_trust10_t * trust10,
    const axutil_env_t * env,
    axis2_bool_t require_server_entropy)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, require_server_entropy, AXIS2_FAILURE);
    trust10->require_server_entropy = require_server_entropy;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_trust10_get_must_support_issued_token(
    rp_trust10_t * trust10,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    return trust10->must_support_issued_token;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_trust10_set_must_support_issued_token(
    rp_trust10_t * trust10,
    const axutil_env_t * env,
    axis2_bool_t must_support_issued_token)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, must_support_issued_token, AXIS2_FAILURE);
    trust10->must_support_issued_token = must_support_issued_token;

    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_trust10_increment_ref(
    rp_trust10_t * trust10,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    trust10->ref++;
    return AXIS2_SUCCESS;
}
