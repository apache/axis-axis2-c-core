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
#include <openssl_cipher_ctx.h>
#include <openssl_constants.h>
#include <openssl_cipher_property.h>
#include <oxs_error.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl_util.h>


typedef struct openssl_cipher_ctx_impl
{
    openssl_cipher_ctx_t ctx;

    const EVP_CIPHER*   cipher;
    oxs_key_t *key;
    axis2_char_t  *iv;
    axis2_char_t  *pad;
}
openssl_cipher_ctx_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(openssl_cipher_ctx) ((openssl_cipher_ctx_impl_t *)openssl_cipher_ctx)

/******************* function headers ******************************/
/* private functions */
static void
openssl_cipher_ctx_init_ops(
    openssl_cipher_ctx_t *ctx);

/*public functions*/

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_free(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                       );

const EVP_CIPHER* AXIS2_CALL
openssl_cipher_ctx_get_cipher(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                             );

oxs_key_t *AXIS2_CALL
openssl_cipher_ctx_get_key(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                          );

axis2_char_t *AXIS2_CALL
openssl_cipher_ctx_get_iv(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                         );

axis2_char_t *AXIS2_CALL
openssl_cipher_ctx_get_pad(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                          );

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_cipher(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        const EVP_CIPHER*
                             );

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_key(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        oxs_key_t *key
                          );

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_iv(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        axis2_char_t *iv
                         );

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_pad(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        axis2_char_t *pad
                          );



/******************* end of function headers ******************************/
AXIS2_EXTERN openssl_cipher_ctx_t *AXIS2_CALL
openssl_cipher_ctx_create(const axis2_env_t *env)
{
    openssl_cipher_ctx_impl_t *ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    ctx_impl = AXIS2_MALLOC(env->allocator, sizeof(openssl_cipher_ctx_impl_t));
    if (!ctx_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ctx_impl->cipher  = NULL;
    ctx_impl->key  = NULL;
    ctx_impl->iv  = NULL;
    ctx_impl->pad  = NULL;



    ctx_impl->ctx.ops =  AXIS2_MALLOC(env->allocator, sizeof(openssl_cipher_ctx_ops_t));
    if (!ctx_impl->ctx.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        openssl_cipher_ctx_free(&(ctx_impl->ctx), env);
        return NULL;
    }

    openssl_cipher_ctx_init_ops(&(ctx_impl->ctx));

    return &(ctx_impl->ctx);
}

/* private functions */
static void
openssl_cipher_ctx_init_ops(
    openssl_cipher_ctx_t *ctx)
{
    ctx->ops->free = openssl_cipher_ctx_free ;
    ctx->ops->get_cipher = openssl_cipher_ctx_get_cipher ;
    ctx->ops->get_key = openssl_cipher_ctx_get_key ;
    ctx->ops->get_iv = openssl_cipher_ctx_get_iv ;
    ctx->ops->get_pad = openssl_cipher_ctx_get_pad ;
    ctx->ops->set_cipher = openssl_cipher_ctx_set_cipher ;
    ctx->ops->set_key_value = openssl_cipher_ctx_set_key ;
    ctx->ops->set_iv = openssl_cipher_ctx_set_iv ;
    ctx->ops->set_pad = openssl_cipher_ctx_set_pad ;
}

/* public functions*/
axis2_status_t AXIS2_CALL
openssl_cipher_ctx_free(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                       )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    if (ctx_impl->iv)
    {
        AXIS2_FREE(env->allocator, ctx_impl->iv);
        ctx_impl->iv = NULL;
    }

    if (ctx_impl->pad)
    {
        AXIS2_FREE(env->allocator, ctx_impl->pad);
        ctx_impl->pad = NULL;
    }

    AXIS2_FREE(env->allocator,  ctx_impl);
    ctx_impl = NULL;

    return AXIS2_SUCCESS;
}

const EVP_CIPHER* AXIS2_CALL
openssl_cipher_ctx_get_cipher(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env)
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->cipher ;
}

oxs_key_t *AXIS2_CALL
openssl_cipher_ctx_get_key(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                          )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->key ;
}

axis2_char_t *AXIS2_CALL
openssl_cipher_ctx_get_iv(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                         )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->iv ;
}

axis2_char_t *AXIS2_CALL
openssl_cipher_ctx_get_pad(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env
                          )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);

    return ctx_impl->pad;
}

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_cipher(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        const EVP_CIPHER *cipher
                             )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);


    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    /*if (ctx_impl->cipher){
        AXIS2_FREE(env->allocator, ctx_impl->cipher);
        ctx_impl->cipher = NULL;
    }
    */
    ctx_impl->cipher = cipher;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_key(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        oxs_key_t *key
                          )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, key, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->key)
    {
        OXS_KEY_FREE(ctx_impl->key, env);
        ctx_impl->key = NULL;
    }
    ctx_impl->key = key ;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_iv(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        axis2_char_t *iv
                         )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, iv, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->iv)
    {
        AXIS2_FREE(env->allocator, ctx_impl->iv);
        ctx_impl->iv = NULL;
    }
    ctx_impl->iv = AXIS2_STRDUP(iv, env);

    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
openssl_cipher_ctx_set_pad(openssl_cipher_ctx_t *ctx,
        const axis2_env_t *env,
        axis2_char_t *pad
                          )
{
    openssl_cipher_ctx_impl_t * ctx_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, pad, AXIS2_FAILURE);

    ctx_impl = AXIS2_INTF_TO_IMPL(ctx);
    if (ctx_impl->pad)
    {
        AXIS2_FREE(env->allocator, ctx_impl->pad);
        ctx_impl->pad = NULL;
    }
    ctx_impl->pad = AXIS2_STRDUP(pad, env);

    return AXIS2_SUCCESS;
}















