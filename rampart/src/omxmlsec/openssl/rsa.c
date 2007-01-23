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
#include <openssl_rsa.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <oxs_buffer.h>
#include <oxs_error.h>

/**
struct _evp_pkey{
    EVP_PKEY *key;
    unsigned char *name;
    int size;
    int type;
}
*/

typedef struct openssl_rsa_impl
{
    openssl_rsa_t rsa;
}
openssl_rsa_impl_t;

#define AXIS2_INTF_TO_IMPL(rsa) ((openssl_rsa_impl_t *)rsa)

/*******************Function Headers ****************************/
static void
openssl_rsa_init_ops(
    openssl_rsa_t *rsa);

/*Public function*/
axis2_status_t AXIS2_CALL
openssl_rsa_free(
    openssl_rsa_t *rsa,
    const axis2_env_t *env);

int AXIS2_CALL
openssl_rsa_prv_decrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out);

int AXIS2_CALL
openssl_rsa_prv_encrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out);

int AXIS2_CALL
openssl_rsa_pub_encrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out);

int AXIS2_CALL
openssl_rsa_pub_decrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out);

/*****************End of function headers ****************************/
static void
openssl_rsa_init_ops(
    openssl_rsa_t *rsa)
{
    rsa->ops->free = openssl_rsa_free;
    rsa->ops->prv_decrypt = openssl_rsa_prv_decrypt;
    rsa->ops->pub_encrypt = openssl_rsa_pub_encrypt;
    rsa->ops->prv_encrypt = openssl_rsa_prv_encrypt;
    rsa->ops->pub_decrypt = openssl_rsa_pub_decrypt;
}

openssl_rsa_t *AXIS2_CALL
openssl_rsa_create(
    const axis2_env_t *env)
{
    openssl_rsa_impl_t *rsa_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    rsa_impl = (openssl_rsa_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(openssl_rsa_impl_t));

    if (NULL == rsa_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    rsa_impl->rsa.ops = AXIS2_MALLOC(env->allocator,
            sizeof(openssl_rsa_ops_t));

    if (NULL == rsa_impl->rsa.ops)
    {
        openssl_rsa_free(&(rsa_impl->rsa), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** initialize ops */
    openssl_rsa_init_ops(&(rsa_impl->rsa));

    return &(rsa_impl->rsa);

}

axis2_status_t AXIS2_CALL
openssl_rsa_free(openssl_rsa_t * rsa,
        const axis2_env_t *env)
{
    openssl_rsa_impl_t * rsa_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    rsa_impl = AXIS2_INTF_TO_IMPL(rsa);
    AXIS2_FREE(env->allocator, rsa_impl);
    rsa_impl = NULL;

    return AXIS2_SUCCESS;

}


int AXIS2_CALL
openssl_rsa_pub_encrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out)
{
    unsigned char *encrypted = NULL;
    openssl_rsa_impl_t *rsa_impl = NULL;
    int ret;
    EVP_PKEY *key = NULL;
    int pad = RSA_PKCS1_PADDING;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    rsa_impl = AXIS2_INTF_TO_IMPL(rsa);

    key = (EVP_PKEY *)OPENSSL_PKEY_GET_KEY(pkey, env);
    if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_OAEP_PADDING  ) ){
        pad = RSA_PKCS1_OAEP_PADDING;
    }else if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_PADDING  ) ){
        pad = RSA_PKCS1_PADDING;
    }

    encrypted = AXIS2_MALLOC(env->allocator, RSA_size(key->pkey.rsa));
    ret = RSA_public_encrypt(OXS_BUFFER_GET_SIZE(in, env),
            OXS_BUFFER_GET_DATA(in, env),
            encrypted,
            key->pkey.rsa ,
            pad);
    
    if (ret < 0)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_OPENSSL_FUNC_FAILED,
                            "RSA encryption failed");
        return (-1);
    }
    OXS_BUFFER_POPULATE(out, env, encrypted, ret);
    return ret;
}

int AXIS2_CALL
openssl_rsa_pub_decrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out)
{
    unsigned char *decrypted = NULL;
    openssl_rsa_impl_t *rsa_impl = NULL;
    int ret;
    EVP_PKEY *key = NULL;
    int pad = RSA_PKCS1_PADDING;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    rsa_impl = AXIS2_INTF_TO_IMPL(rsa);

    key = (EVP_PKEY *)OPENSSL_PKEY_GET_KEY(pkey, env);
    if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_OAEP_PADDING  ) ){
        pad = RSA_PKCS1_OAEP_PADDING;
    }else if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_PADDING  ) ){
        pad = RSA_PKCS1_PADDING;
    }

    decrypted = AXIS2_MALLOC(env->allocator, RSA_size(key->pkey.rsa));
    ret = RSA_public_decrypt(OXS_BUFFER_GET_SIZE(in, env),
            OXS_BUFFER_GET_DATA(in, env),
            decrypted,
            key->pkey.rsa ,
            pad);
    
    if (ret < 0)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_OPENSSL_FUNC_FAILED,
                            "PUBKEY decrypt (signature verification) failed");
        return (-1);
    }
    OXS_BUFFER_POPULATE(out, env, decrypted, ret);
    return ret;
 }


int AXIS2_CALL
openssl_rsa_prv_decrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out)
{
    unsigned char *decrypted = NULL;
    openssl_rsa_impl_t *rsa_impl = NULL;
    int ret;
    EVP_PKEY *key = NULL;
    int pad = RSA_PKCS1_PADDING;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    rsa_impl = AXIS2_INTF_TO_IMPL(rsa);

    key = (EVP_PKEY *)OPENSSL_PKEY_GET_KEY(pkey, env);

    /*Set padding. This is the only diff btwn RSA-v1.5 and RSA-OAEP*/
    if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_OAEP_PADDING  ) ){
        pad = RSA_PKCS1_OAEP_PADDING;
    }else if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_PADDING  ) ){
        pad = RSA_PKCS1_PADDING;
    }

    decrypted =  AXIS2_MALLOC(env->allocator, RSA_size(key->pkey.rsa));
    ret = RSA_private_decrypt(RSA_size(key->pkey.rsa),
            OXS_BUFFER_GET_DATA(in, env),
            decrypted,
            key->pkey.rsa,
            pad);
    
    if (ret < 0)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_OPENSSL_FUNC_FAILED,
                            "RSA decryption failed");
        return (-1);
    }
    OXS_BUFFER_POPULATE(out, env, decrypted, ret);
    return ret;
}

int AXIS2_CALL
openssl_rsa_prv_encrypt(
    openssl_rsa_t *rsa,
    const axis2_env_t *env,
    const openssl_pkey_t *pkey,
    const axis2_char_t *padding,
    oxs_buffer_t *in,
    oxs_buffer_t *out)
{
    unsigned char *encrypted = NULL;
    openssl_rsa_impl_t *rsa_impl = NULL;
    int ret;
    EVP_PKEY *key = NULL;
    int pad = RSA_PKCS1_PADDING;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    rsa_impl = AXIS2_INTF_TO_IMPL(rsa);

    /*Get the private key*/
    key = (EVP_PKEY *)OPENSSL_PKEY_GET_KEY(pkey, env);

    /*Set padding. This is the only diff btwn RSA-v1.5 and RSA-OAEP*/
    if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_OAEP_PADDING  ) ){
        pad = RSA_PKCS1_OAEP_PADDING;
    }else if(0 == AXIS2_STRCMP(padding, OPENSSL_RSA_PKCS1_PADDING  ) ){
        pad = RSA_PKCS1_PADDING;
    }

    encrypted =  AXIS2_MALLOC(env->allocator, RSA_size(key->pkey.rsa));
    ret = RSA_private_encrypt(RSA_size(key->pkey.rsa),
            OXS_BUFFER_GET_DATA(in, env),
            encrypted,
            key->pkey.rsa,
            pad);

    if (ret < 0)
    {
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_OPENSSL_FUNC_FAILED,
                            "RSA private encryption(Signing) failed");
        return (-1);
    }
    OXS_BUFFER_POPULATE(out, env, encrypted, ret);
    return ret;
}

