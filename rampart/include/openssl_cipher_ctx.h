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

#include<openssl/evp.h>
#include<oxs_buffer.h>

/**
  * @file 
  * @brief 
  */
#ifndef OPENSSL_CIPHER_CTX_H
#define OPENSSL_CIPHER_CTX_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _openssl_evp_block_cipher_ctx openssl_evp_block_cipher_ctx, *openssl_evp_block_cipher_ctx_ptr;

struct _openssl_evp_block_cipher_ctx {
    const EVP_CIPHER*   cipher;
    EVP_CIPHER_CTX  cipher_ctx;
    int         key_initialized;
    int         ctx_initialized;
    unsigned char  *key;
    unsigned char  *iv;
    unsigned char  *pad;
};


/**
* Create a fresh block cipher context
*@return openssl_evp_block_cipher_ctx_ptr the created block cipher context
*/
AXIS2_EXTERN openssl_evp_block_cipher_ctx_ptr AXIS2_CALL
openssl_evp_block_cipher_ctx_create(const axis2_env_t *env);

/**
* Initialize the block cipher context for a given cipher
*
*@param openssl_evp_block_cipher_ctx_ptr block cipher context
*@param in input buffer
*@param out output buffer
*@param encrypt 1 to encrypt 0 to decrypt
*@param cipher_name Name of the cipher Passing NULL will use the default
*@return openssl_evp_block_cipher_ctx_ptr initialized block cipher context
*/

AXIS2_EXTERN int AXIS2_CALL  
openssl_evp_block_cipher_ctx_init(const axis2_env_t *env,
                             openssl_evp_block_cipher_ctx_ptr bc_ctx,
                             int encrypt,
                             const unsigned char* cipher_name
                             );

AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_evp_block_cipher_ctx_reset(const axis2_env_t *env, openssl_evp_block_cipher_ctx_ptr bc_ctx);
/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_evp_block_cipher_ctx_free(const axis2_env_t *env, openssl_evp_block_cipher_ctx_ptr bc_ctx);
*/

/*TODO write the free function*/


/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_CIPHER_CTX_H */
