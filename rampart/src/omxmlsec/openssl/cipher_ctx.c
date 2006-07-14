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
#include <openssl_cipher_ctx.h>
#include <openssl/evp.h>
#include <openssl/rand.h>


AXIS2_EXTERN openssl_evp_block_cipher_ctx_ptr AXIS2_CALL  openssl_evp_block_cipher_ctx_create(const axis2_env_t *env){
    openssl_evp_block_cipher_ctx_ptr bc_ctx = NULL;
    bc_ctx = (openssl_evp_block_cipher_ctx_ptr)AXIS2_MALLOC(env->allocator,sizeof(openssl_evp_block_cipher_ctx));
    
    return bc_ctx;
}

AXIS2_EXTERN openssl_evp_block_cipher_ctx_ptr AXIS2_CALL  openssl_evp_block_cipher_ctx_init(const axis2_env_t *env,
                             openssl_evp_block_cipher_ctx_ptr bc_ctx,
                             oxs_buffer_ptr in,
                             oxs_buffer_ptr out,
                             int encrypt,
                             const unsigned char* cipher_name
                             )
{
    int ivLen;
    int ret;
        
    bc_ctx->cipher  =  EVP_des_ede3_cbc(); /* Right now we support only this*/

    EVP_CIPHER_CTX_init(&(bc_ctx->cipher_ctx));

    ivLen = EVP_CIPHER_iv_length(bc_ctx->cipher);

    if(encrypt) {
        /* generate random iv */
        ret = RAND_bytes(bc_ctx->iv, ivLen);
        if(ret != 1) {
            return(-1);
        }

        /*Write IV to the output */
        ret = oxs_buffer_append(env, out, bc_ctx->iv, ivLen); 

    }else{
        /* if we don't have enough data, exit and hope that
         * we'll have iv next time */
        
        /*TODO Decrypt*/
       return 0;
    }

    /* set iv */
    ret = EVP_CipherInit(&(bc_ctx->cipher_ctx), bc_ctx->cipher, bc_ctx->key, bc_ctx->iv, encrypt);
    if(ret != 1) {
        return (-1);
    }
   
    bc_ctx->ctxInitialized = 1;
    
    return 0;
}
#if 0
AXIS2_EXTERN int AXIS2_CALL  openssl_evp_block_cipher_ctx_update  (const axis2_env_t *env,
                             openssl_evp_block_cipher_ctx_ptr ctx,
                             oxs_buffer_ptr in,
                             oxs_buffer_ptr out,
                             const unsigned char* cipherName
                             )
{
    int block_len, fixLength = 0, outLen = 0;
    unsigned int inSize, outSize;
    unsigned char* outBuf;
    int ret;

    block_len = EVP_CIPHER_block_size(ctx->cipher);

    inSize = in->size;
    outSize = out->size;

    if(inSize == 0) {
        /* wait for more data */
        return(0);
    }

    outBuf = out->data + outSize;    


    /* encrypt/decrypt */
    ret = EVP_CipherUpdate(&(ctx->cipherCtx), outBuf, &outLen, in->data, inSize);
    if(ret != 1) {
        return (-1);
    }


    out->size = outSize + outLen;

     /* remove the processed block from input */
    ret = oxs_remove_head(in, inSize);
    if(ret < 0) return (-1);
    
    return (0); 

}

AXIS2_EXTERN int  AXIS2_CALL openssl_evp_block_cipher_ctx_final (const axis2_env_t *env,
                             openssl_evp_block_cipher_ctx_ptr ctx,
                             oxs_buffer_ptr out,
                             const unsigned char* cipherName
                             )
{
    int block_len, outLen = 0, outLen2 = 0;
    unsigned int outSize;
    unsigned char* outBuf;
    int ret;

    block_len = EVP_CIPHER_block_size(ctx->cipher);
    if(block_len > 0) return (-1);

    outSize = out->size;
    ret = oxs_buffer_set_max_size(env, out, outSize + 2 * block_len);
    
    if(ret < 0) return (-1);
    
    outBuf = out->data + outSize;


    /* finalize transform */
    ret = EVP_CipherFinal(&(ctx->cipherCtx), outBuf, &outLen2);
    if(ret != 1) {
        return (-1);
    }

    /* set correct output buffer size */
    ret = oxs_buffer_set_size(env, out, outSize + outLen + outLen2);
    if(ret < 0) {
        return(-1);
    }

    return(0);
}
#endif
    
