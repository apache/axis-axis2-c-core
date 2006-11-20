/*
 *   Copyright 2003-2004 The Apache Software Foundation.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include <stdio.h>
#include <axis2_util.h>
#include <oxs_key.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_error.h>
#include <openssl_cipher_property.h>
#include <openssl_util.h>

typedef struct oxs_key_impl
{
    oxs_key_t key;


    oxs_buffer_t *buf;
    axis2_char_t *name;
    int            usage;
    /*int            size;
    unsigned char *data;*/
}
oxs_key_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(oxs_key) ((oxs_key_impl_t *)oxs_key)

/******** function headers ***************/

/*private functions*/
static void
oxs_key_init_ops(
    oxs_key_t *key);

/*Public functions*/
unsigned char *AXIS2_CALL
oxs_key_get_data(
    const oxs_key_t *key,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
oxs_key_get_name(
    const oxs_key_t *key,
    const axis2_env_t *env);

int AXIS2_CALL
oxs_key_get_size(
    const oxs_key_t *key,
    const axis2_env_t *env);

int AXIS2_CALL
oxs_key_get_usage(
    const oxs_key_t *key,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
oxs_key_set_data(
    oxs_key_t *key,
    const axis2_env_t *env,
    unsigned char *data);

axis2_status_t AXIS2_CALL
oxs_key_set_name(
    oxs_key_t *key,
    const axis2_env_t *env,
    axis2_char_t *name);

axis2_status_t AXIS2_CALL
oxs_key_set_size(
    oxs_key_t *key,
    const axis2_env_t *env,
    int size);

axis2_status_t AXIS2_CALL
oxs_key_set_usage(
    oxs_key_t *key,
    const axis2_env_t *env,
    int usage);

axis2_status_t AXIS2_CALL
oxs_key_free(
    oxs_key_t *key,
    const axis2_env_t *env
);

axis2_status_t AXIS2_CALL
oxs_key_populate(
    oxs_key_t *key,
    const axis2_env_t *env,
    unsigned char *data,
    axis2_char_t *name,
    int size,
    int usage);

axis2_status_t AXIS2_CALL
oxs_key_read_from_file(
    oxs_key_t *key,
    const axis2_env_t *env,
    axis2_char_t *file_name);

axis2_status_t AXIS2_CALL
oxs_key_for_algo(oxs_key_t *key,
        const axis2_env_t *env,
        axis2_char_t *key_algo);

/******************** end of function headers *****************/

unsigned char *AXIS2_CALL
oxs_key_get_data(
    const oxs_key_t *key,
    const axis2_env_t *env)
{
    oxs_key_impl_t *key_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    key_impl = AXIS2_INTF_TO_IMPL(key);

    return OXS_BUFFER_GET_DATA(key_impl->buf, env);

}

axis2_char_t *AXIS2_CALL
oxs_key_get_name(
    const oxs_key_t *key,
    const axis2_env_t *env)
{
    oxs_key_impl_t *key_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    key_impl = AXIS2_INTF_TO_IMPL(key);

    return key_impl->name;

}

int AXIS2_CALL
oxs_key_get_size(
    const oxs_key_t *key,
    const axis2_env_t *env)
{
    oxs_key_impl_t *key_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    key_impl = AXIS2_INTF_TO_IMPL(key);
    
    return OXS_BUFFER_GET_SIZE(key_impl->buf, env);

}

int AXIS2_CALL
oxs_key_get_usage(
    const oxs_key_t *key,
    const axis2_env_t *env)
{
    oxs_key_impl_t *key_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    key_impl = AXIS2_INTF_TO_IMPL(key);

    return key_impl->usage;

}



axis2_status_t AXIS2_CALL
oxs_key_set_name(
    oxs_key_t *key,
    const axis2_env_t *env,
    axis2_char_t *name)
{
    oxs_key_impl_t *oxs_key_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);
    oxs_key_impl = AXIS2_INTF_TO_IMPL(key);

    if (oxs_key_impl->name)
    {
        AXIS2_FREE(env->allocator, oxs_key_impl->name);
        oxs_key_impl->name = NULL;
    }
    oxs_key_impl->name = AXIS2_STRDUP(name, env);
    return AXIS2_SUCCESS;

}


axis2_status_t AXIS2_CALL
oxs_key_set_usage(
    oxs_key_t *key,
    const axis2_env_t *env,
    int usage)
{
    oxs_key_impl_t *oxs_key_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    oxs_key_impl = AXIS2_INTF_TO_IMPL(key);

    oxs_key_impl->usage = usage;
    return AXIS2_SUCCESS;

}



static void
oxs_key_init_ops(
    oxs_key_t *key)
{
    key->ops->get_data  = oxs_key_get_data ;
    key->ops->get_name  = oxs_key_get_name;
    key->ops->get_size  = oxs_key_get_size;
    key->ops->get_usage = oxs_key_get_usage;
    key->ops->set_name  = oxs_key_set_name;
    key->ops->set_usage = oxs_key_set_usage;
    key->ops->free      = oxs_key_free;
    key->ops->populate  = oxs_key_populate;
    key->ops->read_from_file = oxs_key_read_from_file;
    key->ops->for_algo  = oxs_key_for_algo;
}


AXIS2_EXTERN oxs_key_t *AXIS2_CALL
oxs_key_create(const axis2_env_t *env)
{
    oxs_key_impl_t *key_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    key_impl = AXIS2_MALLOC(env->allocator, sizeof(oxs_key_impl_t));
    if (!key_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    key_impl->buf = NULL;
    key_impl->name = NULL;
    key_impl->usage = -1;
    
    /*additionally we need to create a buffer to keep data*/
    key_impl->buf = oxs_buffer_create(env);

    key_impl->key.ops =  AXIS2_MALLOC(env->allocator, sizeof(oxs_key_ops_t));
    if (!key_impl->key.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        oxs_key_free(&(key_impl->key), env);
        return NULL;
    }

    oxs_key_init_ops(&(key_impl->key));
    return &(key_impl->key);

}


axis2_status_t AXIS2_CALL
oxs_key_free(oxs_key_t *key,
        const axis2_env_t *env)
{
    oxs_key_impl_t *key_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    key_impl = AXIS2_INTF_TO_IMPL(key);

    OXS_BUFFER_FREE(key_impl->buf, env);
    key_impl->buf = NULL;
    AXIS2_FREE(env->allocator,  key_impl->name);
    key_impl->name = NULL;

    AXIS2_FREE(env->allocator,  key_impl);
    key_impl = NULL;

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
    oxs_key_impl_t *key_impl = NULL;
    int ret;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    key_impl = AXIS2_INTF_TO_IMPL(key);

    ret = OXS_KEY_SET_NAME(key, env, name);
    ret = OXS_KEY_SET_USAGE(key, env, usage);

    ret = OXS_BUFFER_POPULATE(key_impl->buf, env, data, size);

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
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                "openssl_get_cipher_property failed");
        return AXIS2_FAILURE;
    }

    size = OPENSSL_CIPHER_PROPERTY_GET_KEY_SIZE(cprop, env);

    key_buf = oxs_buffer_create(env);
    ret = generate_random_data(env, key_buf, size);
    if (ret == AXIS2_FAILURE)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
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
