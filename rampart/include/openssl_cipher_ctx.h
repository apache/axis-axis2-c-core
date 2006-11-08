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
#include <axis2_defines.h>
#include <axis2_env.h>
#include <openssl/evp.h>
#include <oxs_key.h>
/**
  * @file 
  * @brief 
  */
#ifndef OPENSSL_CIPHER_CTX_H
#define OPENSSL_CIPHER_CTX_H

#ifdef __cplusplus
extern "C" {
#endif


    /** Type name for struct openssl_cipher_ctx_ops */
    typedef struct openssl_cipher_ctx_ops openssl_cipher_ctx_ops_t;
    /** Type name for struct openssl_cipher_ctx */
    typedef struct openssl_cipher_ctx openssl_cipher_ctx_t;


    struct openssl_cipher_ctx_ops
    {

        axis2_status_t (AXIS2_CALL *
        free)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env
            );
        
        const EVP_CIPHER* (AXIS2_CALL *
        get_cipher)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env
            );
        
        oxs_key_t *(AXIS2_CALL *
        get_key)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env
            );
        
        axis2_char_t *(AXIS2_CALL *
        get_iv)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env
            );
        
        axis2_char_t *(AXIS2_CALL *
        get_pad)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env
            );
    
        axis2_status_t (AXIS2_CALL *
        set_cipher)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env,
            const EVP_CIPHER* cipher
            );
        
        axis2_status_t (AXIS2_CALL *
        set_key_value)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env,
            oxs_key_t *key
            );

        axis2_status_t (AXIS2_CALL *
        set_iv)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env,
            axis2_char_t *iv 
            );

        axis2_status_t (AXIS2_CALL *
        set_pad)(openssl_cipher_ctx_t *ctx,
            const axis2_env_t *env,
            axis2_char_t *pad 
            );

    };

    struct openssl_cipher_ctx
    {
        /** operations of openssl_cipher_ctx */
        openssl_cipher_ctx_ops_t *ops;
    };

    /*Create function*/
    AXIS2_EXTERN openssl_cipher_ctx_t *AXIS2_CALL
    openssl_cipher_ctx_create(const axis2_env_t *env);

/**********************Macros******************************************/
#define OPENSSL_CIPHER_CTX_FREE(ctx, env)\
        ((ctx)->ops->free(ctx, env))

#define OPENSSL_CIPHER_CTX_GET_CIPHER(ctx, env)\
        ((ctx)->ops->get_cipher(ctx, env)) 

#define OPENSSL_CIPHER_CTX_GET_KEY(ctx, env)\
        ((ctx)->ops->get_key(ctx, env))

#define OPENSSL_CIPHER_CTX_GET_IV(ctx, env)\
        ((ctx)->ops->get_iv(ctx, env)) 

#define OPENSSL_CIPHER_CTX_GET_PAD(ctx, env)\
        ((ctx)->ops->get_pad(ctx, env)) 

#define OPENSSL_CIPHER_CTX_SET_CIPHER(ctx, env, cipher)\
        ((ctx)->ops->set_cipher(ctx, env, cipher)) 

#define OPENSSL_CIPHER_CTX_SET_IV(ctx, env, iv)\
        ((ctx)->ops->set_iv(ctx, env, iv)) 

#define OPENSSL_CIPHER_CTX_SET_KEY(ctx, env, key)\
        ((ctx)->ops->set_key_value(ctx, env, key)) 

#define OPENSSL_CIPHER_CTX_SET_PAD(ctx, env, pad)\
        ((ctx)->ops->set_pad(ctx, env, pad)) 




/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_CIPHER_CTX_H */
