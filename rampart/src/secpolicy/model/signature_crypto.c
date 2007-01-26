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



#include <rp_signature_crypto.h>

struct rp_signature_crypto_t
{
    axis2_char_t *signature_prop_file;
};

AXIS2_EXTERN rp_signature_crypto_t *AXIS2_CALL 
rp_signature_crypto_create(const axis2_env_t *env)
{
    rp_signature_crypto_t *signature_crypto = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    signature_crypto =  (rp_signature_crypto_t *) AXIS2_MALLOC (env->allocator,
    sizeof (rp_signature_crypto_t));

    if(signature_crypto == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    signature_crypto->signature_prop_file = NULL;
    
    return signature_crypto;

}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rp_signature_crypto_free(
    rp_signature_crypto_t *signature_crypto,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    if(signature_crypto)
    {
        AXIS2_FREE(env->allocator, signature_crypto);
        signature_crypto = NULL;
    }
    return AXIS2_SUCCESS;
}


/* Implementations */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL 
rp_signature_crypto_get_signature_prop_file(
    rp_signature_crypto_t *signature_crypto,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return signature_crypto->signature_prop_file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rp_signature_crypto_set_signature_prop_file(
    rp_signature_crypto_t *signature_crypto,
    const axis2_env_t *env,
    axis2_char_t *signature_prop_file)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,signature_prop_file,AXIS2_FAILURE);
    
    signature_crypto->signature_prop_file = signature_prop_file;
    return AXIS2_SUCCESS;
}