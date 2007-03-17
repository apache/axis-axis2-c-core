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
#include <openssl_cipher_property.h>
#include <openssl_constants.h>
#include <oxs_error.h>
#include <openssl/evp.h>
#include <openssl/rand.h>


typedef struct openssl_cipher_property_impl
{
    openssl_cipher_property_t   cprop;

    EVP_CIPHER                  *cipher;
    axis2_char_t                *name;
    axis2_char_t                *url;
    int                         key_size;
    int                         block_size;
    int                         iv_size;
}
openssl_cipher_property_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(openssl_cipher_property) ((openssl_cipher_property_impl_t *)openssl_cipher_property)

/*private functions*/
static void
openssl_cipher_property_init_ops(
    openssl_cipher_property_t *cprop);

/*public functions*/
EVP_CIPHER * AXIS2_CALL
openssl_cipher_property_get_cipher(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env);

axis2_char_t * AXIS2_CALL
openssl_cipher_property_get_name(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env);

axis2_char_t * AXIS2_CALL
openssl_cipher_property_get_url(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env);

int AXIS2_CALL
openssl_cipher_property_get_key_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env);

int AXIS2_CALL
openssl_cipher_property_get_block_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env);

int AXIS2_CALL
openssl_cipher_property_get_iv_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_cipher(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    EVP_CIPHER *cipher);

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_name(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    axis2_char_t *name);

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_url(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    axis2_char_t *name);

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_key_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    int   key_size);

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_block_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    int  block_size);

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_iv_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    int   iv_size);

axis2_status_t AXIS2_CALL
openssl_cipher_property_free(openssl_cipher_property_t * cprop, const axis2_env_t *env);

/********* end of function headers ******************/

EVP_CIPHER * AXIS2_CALL
openssl_cipher_property_get_cipher(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    return cprop_impl->cipher;
}

axis2_char_t * AXIS2_CALL
openssl_cipher_property_get_name(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    return cprop_impl->name;
}

axis2_char_t * AXIS2_CALL
openssl_cipher_property_get_url(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    return cprop_impl->url;
}


int AXIS2_CALL
openssl_cipher_property_get_key_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    return cprop_impl->key_size;
}

int AXIS2_CALL
openssl_cipher_property_get_block_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    return cprop_impl->block_size;
}

int AXIS2_CALL
openssl_cipher_property_get_iv_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    return cprop_impl->iv_size;
}

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_cipher(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    EVP_CIPHER *cipher)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    if (cprop_impl->cipher)
    {
        AXIS2_FREE(env->allocator, cprop_impl->cipher);
        cprop_impl->cipher = NULL;
    }
    cprop_impl->cipher = cipher;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_name(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    axis2_char_t *name)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    if (cprop_impl->name)
    {
        AXIS2_FREE(env->allocator, cprop_impl->name);
        cprop_impl->name = NULL;
    }
    cprop_impl->name = axis2_strdup(name, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_url(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    axis2_char_t *url)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, url, AXIS2_FAILURE);

    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    if (cprop_impl->url)
    {
        AXIS2_FREE(env->allocator, cprop_impl->url);
        cprop_impl->url = NULL;
    }
    cprop_impl->url = axis2_strdup(url, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_cipher_property_set_key_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    int   key_size)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    cprop_impl->key_size = key_size;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
openssl_cipher_property_set_block_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    int  block_size)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    cprop_impl->block_size = block_size;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
openssl_cipher_property_set_iv_size(
    const openssl_cipher_property_t *cprop,
    const axis2_env_t *env,
    int   iv_size)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    cprop_impl->iv_size = iv_size;
    return AXIS2_SUCCESS;
}



static void
openssl_cipher_property_init_ops(
    openssl_cipher_property_t *cprop)
{
    cprop->ops->get_cipher = openssl_cipher_property_get_cipher ;
    cprop->ops->get_name = openssl_cipher_property_get_name;
    cprop->ops->get_url = openssl_cipher_property_get_url;
    cprop->ops->get_key_size = openssl_cipher_property_get_key_size;
    cprop->ops->get_block_size = openssl_cipher_property_get_block_size;
    cprop->ops->get_iv_size = openssl_cipher_property_get_iv_size;
    cprop->ops->set_cipher = openssl_cipher_property_set_cipher;
    cprop->ops->set_name = openssl_cipher_property_set_name;
    cprop->ops->set_url = openssl_cipher_property_set_url;
    cprop->ops->set_key_size = openssl_cipher_property_set_key_size;
    cprop->ops->set_block_size = openssl_cipher_property_set_block_size;
    cprop->ops->set_iv_size = openssl_cipher_property_set_iv_size;
    cprop->ops->free = openssl_cipher_property_free;

}

AXIS2_EXTERN openssl_cipher_property_t *AXIS2_CALL
openssl_cipher_property_create(const axis2_env_t *env)
{

    openssl_cipher_property_impl_t *cprop_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    cprop_impl = AXIS2_MALLOC(env->allocator, sizeof(openssl_cipher_property_impl_t));
    if (!cprop_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    cprop_impl->cipher = NULL;
    cprop_impl->name = NULL;
    cprop_impl->url = NULL;
    cprop_impl->key_size = -1;
    cprop_impl->block_size = -1;
    cprop_impl->iv_size = -1;

    cprop_impl->cprop.ops =  AXIS2_MALLOC(env->allocator, sizeof(openssl_cipher_property_ops_t));
    if (!cprop_impl->cprop.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        openssl_cipher_property_free(&(cprop_impl->cprop), env);
        return NULL;
    }

    openssl_cipher_property_init_ops(&(cprop_impl->cprop));

    return &(cprop_impl->cprop);

}


axis2_status_t AXIS2_CALL
openssl_cipher_property_free(openssl_cipher_property_t * cprop, const axis2_env_t *env)
{
    openssl_cipher_property_impl_t *cprop_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    cprop_impl = AXIS2_INTF_TO_IMPL(cprop);

    if (cprop_impl->cipher)
    {
        AXIS2_FREE(env->allocator, cprop_impl->cipher);
        cprop_impl->cipher = NULL;
    }

    if (cprop_impl->name)
    {
        AXIS2_FREE(env->allocator, cprop_impl->name);
        cprop_impl->name = NULL;
    }

    if (cprop_impl->url)
    {
        AXIS2_FREE(env->allocator, cprop_impl->url);
        cprop_impl->url = NULL;
    }

    AXIS2_FREE(env->allocator,  cprop_impl);
    cprop_impl = NULL;

    return AXIS2_SUCCESS;
}

