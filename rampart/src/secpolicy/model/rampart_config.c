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


#include <rp_rampart_config.h>

struct rp_rampart_config_t
{
    axis2_char_t *user;
    axis2_char_t *encryption_user;
    axis2_char_t *password_callback_class;
    axis2_char_t *authenticate_module;
    rp_signature_crypto_t *signature_crypto;
    rp_encryption_crypto_t *encryption_crypto;
};

AXIS2_EXTERN rp_rampart_config_t *AXIS2_CALL 
rp_rampart_config_create(const axis2_env_t *env)
{
    rp_rampart_config_t *rampart_config = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    rampart_config =  (rp_rampart_config_t *) AXIS2_MALLOC (env->allocator,
    sizeof (rp_rampart_config_t));

    if(rampart_config == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    rampart_config->user = NULL;
    rampart_config->encryption_user = NULL;
    rampart_config->password_callback_class = NULL;
    rampart_config->signature_crypto = NULL;
    rampart_config->encryption_crypto = NULL;
    rampart_config->authenticate_module = NULL;

    return rampart_config;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_free(rp_rampart_config_t *rampart_config,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(rampart_config)
    {
        if(rampart_config->encryption_crypto)
        {
            rp_encryption_crypto_free(rampart_config->encryption_crypto,env);
            rampart_config->encryption_crypto = NULL;
        }
        if(rampart_config->signature_crypto)
        {
            rp_signature_crypto_free(rampart_config->signature_crypto,env);
            rampart_config->signature_crypto = NULL;
        }
        AXIS2_FREE(env->allocator,rampart_config);
        rampart_config = NULL;
    }
    return AXIS2_SUCCESS;
}


/* Implementations */

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_user(
    rp_rampart_config_t *rampart_config,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return rampart_config->user;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_user(rp_rampart_config_t *rampart_config,
            const axis2_env_t *env,
            axis2_char_t *user)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,user,AXIS2_FAILURE);

    rampart_config->user = user;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_encryption_user(
    rp_rampart_config_t *rampart_config,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->encryption_user;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_encryption_user(rp_rampart_config_t *rampart_config,
            const axis2_env_t *env,
            axis2_char_t *encryption_user)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,encryption_user,AXIS2_FAILURE);
        
    rampart_config->encryption_user = encryption_user;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_password_callback_class(
    rp_rampart_config_t *rampart_config,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->password_callback_class;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_password_callback_class(rp_rampart_config_t *rampart_config,
            const axis2_env_t *env,
            axis2_char_t *password_callback_class)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,password_callback_class,AXIS2_FAILURE);   
        
    rampart_config->password_callback_class = password_callback_class;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_char_t *AXIS2_CALL
rp_rampart_config_get_authenticate_module(
    rp_rampart_config_t *rampart_config,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->authenticate_module;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_authenticate_module(rp_rampart_config_t *rampart_config,
            const axis2_env_t *env,
            axis2_char_t *authenticate_module)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,authenticate_module,AXIS2_FAILURE);   
        
    rampart_config->authenticate_module = authenticate_module;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN rp_signature_crypto_t *AXIS2_CALL
rp_rampart_config_get_signature_crypto(
    rp_rampart_config_t *rampart_config,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->signature_crypto;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_signature_crypto(rp_rampart_config_t *rampart_config,
            const axis2_env_t *env,
            rp_signature_crypto_t *signature_crypto)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,signature_crypto,AXIS2_FAILURE);
        
    rampart_config->signature_crypto = signature_crypto;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN rp_encryption_crypto_t *AXIS2_CALL
rp_rampart_config_get_encryption_crypto(
    rp_rampart_config_t *rampart_config,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return rampart_config->encryption_crypto;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_set_encryption_crypto(rp_rampart_config_t *rampart_config,
            const axis2_env_t *env,
            rp_encryption_crypto_t *encryption_crypto)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error,encryption_crypto,AXIS2_FAILURE);   
        
    rampart_config->encryption_crypto = encryption_crypto;
    return AXIS2_SUCCESS;
}
