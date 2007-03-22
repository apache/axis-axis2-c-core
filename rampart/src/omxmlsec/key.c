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

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_key.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_error.h>
#include <openssl_cipher_property.h>
#include <openssl_util.h>

struct oxs_key_t
{
    oxs_buffer_t *buf;
    axis2_char_t *name;
    int           usage;
};

/******************** end of function headers *****************/

unsigned char *AXIS2_CALL
oxs_key_get_data(
    const oxs_key_t *key,
    const axis2_env_t *env)
{    
    AXIS2_ENV_CHECK(env, NULL);    

    return OXS_BUFFER_GET_DATA(key->buf, env);
}

axis2_char_t *AXIS2_CALL
oxs_key_get_name(
    const oxs_key_t *key,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);    

    return key->name;

}

int AXIS2_CALL
oxs_key_get_size(
    const oxs_key_t *key,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    return OXS_BUFFER_GET_SIZE(key->buf, env);
}

int AXIS2_CALL
oxs_key_get_usage(
    const oxs_key_t *key,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return key->usage;
}



axis2_status_t AXIS2_CALL
oxs_key_set_name(
    oxs_key_t *key,
    const axis2_env_t *env,
    axis2_char_t *name)
{

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    if (key->name)
    {
        AXIS2_FREE(env->allocator, key->name);
        key->name = NULL;
    }
    key->name = axis2_strdup(name, env);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
oxs_key_set_usage(
    oxs_key_t *key,
    const axis2_env_t *env,
    int usage)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    key->usage = usage;
    return AXIS2_SUCCESS;

}



AXIS2_EXTERN oxs_key_t *AXIS2_CALL
oxs_key_create(const axis2_env_t *env)
{
	oxs_key_t *key = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (!key)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    key->buf = NULL;
    key->name = NULL;
    key->usage = -1;
    
    /*additionally we need to create a buffer to keep data*/
    key->buf = oxs_buffer_create(env);

    return key;
}


axis2_status_t AXIS2_CALL
oxs_key_free(oxs_key_t *key,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    OXS_BUFFER_FREE(key->buf, env);
    key->buf = NULL;
    AXIS2_FREE(env->allocator,  key->name);
    key->name = NULL;

    AXIS2_FREE(env->allocator,  key);
    key = NULL;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_key_populate(oxs_key_t *key,
        const axis2_env_t *env,
        unsigned char *data,
        axis2_char_t *name,
        int size,
        int usage)
{
    int ret;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ret = OXS_KEY_SET_NAME(key, env, name);
    ret = OXS_KEY_SET_USAGE(key, env, usage);

    ret = OXS_BUFFER_POPULATE(key->buf, env, data, size);

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
oxs_key_read_from_file(oxs_key_t *key,
        const axis2_env_t *env,
        axis2_char_t *file_name)
{
    oxs_buffer_t *buf = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    buf = oxs_buffer_create(env);
    status = OXS_BUFFER_READ_FILE(buf, env, file_name);

    status = OXS_KEY_POPULATE(key, env,
            OXS_BUFFER_GET_DATA(buf, env), file_name,
            OXS_BUFFER_GET_SIZE(buf, env), OXS_KEY_USAGE_NONE);

    return status;

}

axis2_status_t AXIS2_CALL
oxs_key_for_algo(oxs_key_t *key,
        const axis2_env_t *env,
        axis2_char_t *key_algo)
{
    oxs_buffer_t *key_buf = NULL;
    openssl_cipher_property_t * cprop = NULL;
    axis2_status_t ret = AXIS2_FAILURE;
    int size;
    unsigned char *temp_str = NULL;
    int temp_int = 0;

    cprop = (openssl_cipher_property_t *)oxs_get_cipher_property_for_url(env, key_algo);
    if (!cprop)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                "openssl_get_cipher_property failed");
        return AXIS2_FAILURE;
    }

    size = OPENSSL_CIPHER_PROPERTY_GET_KEY_SIZE(cprop, env);

    key_buf = oxs_buffer_create(env);
    /*The actual key generation happens here*/
    ret = generate_random_data(env, key_buf, size);
    if (ret == AXIS2_FAILURE)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                "generate_random_data failed");
        return AXIS2_FAILURE;
    }

    temp_int = OXS_BUFFER_GET_SIZE(key_buf, env);
    temp_str = OXS_BUFFER_GET_DATA(key_buf, env);

    ret = OXS_KEY_POPULATE(key, env,
            OXS_BUFFER_GET_DATA(key_buf, env), NULL,
            OXS_BUFFER_GET_SIZE(key_buf, env), OXS_KEY_USAGE_NONE);

    /* Duplicate key data and free key_buf*/
    return ret;
}
