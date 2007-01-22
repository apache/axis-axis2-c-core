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

#include <rp_encryption_crypto.h>

struct rp_encryption_crypto_t
{
    axis2_char_t *encryption_prop_file;
    axis2_char_t *decryption_prop_file;
};

AXIS2_EXTERN rp_encryption_crypto_t *AXIS2_CALL 
rp_encryption_crypto_create(const axis2_env_t *env)
{
    rp_encryption_crypto_t *encryption_crypto = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    encryption_crypto =  (rp_encryption_crypto_t *) AXIS2_MALLOC (env->allocator,
    sizeof (rp_encryption_crypto_t));

    if(encryption_crypto == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    encryption_crypto->encryption_prop_file = NULL;
    encryption_crypto->decryption_prop_file = NULL;

    return encryption_crypto;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rp_encryption_crypto_free(
    rp_encryption_crypto_t *encryption_crypto,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if(encryption_crypto)
    {
        AXIS2_FREE(env->allocator, encryption_crypto);
        encryption_crypto = NULL;
    }
    return AXIS2_SUCCESS;
}


/* Implementations */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL 
rp_encryption_crypto_get_encryption_prop_file(
    rp_encryption_crypto_t *encryption_crypto,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
        
    return encryption_crypto->encryption_prop_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rp_encryption_crypto_set_encryption_prop_file(
    rp_encryption_crypto_t *encryption_crypto,
    const axis2_env_t *env,
    axis2_char_t *encryption_prop_file)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,encryption_prop_file,AXIS2_FAILURE);
    
    encryption_crypto->encryption_prop_file = encryption_prop_file;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL 
rp_encryption_crypto_get_decryption_prop_file(
    rp_encryption_crypto_t *encryption_crypto,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
        
    return encryption_crypto->decryption_prop_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rp_encryption_crypto_set_decryption_prop_file(
    rp_encryption_crypto_t *encryption_crypto,
    const axis2_env_t *env,
    axis2_char_t *decryption_prop_file)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,decryption_prop_file,AXIS2_FAILURE);

    encryption_crypto->decryption_prop_file = decryption_prop_file;
    return AXIS2_SUCCESS;
}
