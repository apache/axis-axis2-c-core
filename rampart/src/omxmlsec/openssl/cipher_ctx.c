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
#include <openssl_constants.h>
#include <oxs_error.h>
#include <openssl/evp.h>
#include <openssl/rand.h>


AXIS2_EXTERN openssl_evp_block_cipher_ctx_ptr AXIS2_CALL
openssl_evp_block_cipher_ctx_create(const axis2_env_t *env)
{
    openssl_evp_block_cipher_ctx_ptr bc_ctx = NULL;
    bc_ctx = (openssl_evp_block_cipher_ctx_ptr)AXIS2_MALLOC(env->allocator,sizeof(openssl_evp_block_cipher_ctx));
    
    return bc_ctx;
}

AXIS2_EXTERN int  AXIS2_CALL  
openssl_evp_block_cipher_ctx_init(const axis2_env_t *env,
                             openssl_evp_block_cipher_ctx_ptr bc_ctx,
                             int encrypt,
                             const unsigned char* cipher_name)
{
    
    int iv_len;
    int ret;

    
    /*If bc_ctx is NULL create a new one*/
    if(!bc_ctx){
        bc_ctx = openssl_evp_block_cipher_ctx_create(env);
        if(!bc_ctx) return (-1); 
    }    
    printf("\nCreating cipher ctx for %s", cipher_name);
    /*Set the cipher. TODO Support more ciphers later*/
    if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_des_ede3_cbc ))
    {
        bc_ctx->cipher = EVP_des_ede3_cbc();         
    }else if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_aes_128_cbc ))
    {
        bc_ctx->cipher = EVP_aes_128_cbc(); 
    }else if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_aes_128_cbc ))
    {
        bc_ctx->cipher = EVP_aes_192_cbc();
    }else if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_aes_128_cbc ))
    {
        bc_ctx->cipher = EVP_aes_256_cbc();
    }else{
        return (-1);
    }

    /*Sets the IV if not set. Well..How we convey this IV to decrypt*/
    if(!(bc_ctx->iv)){
        iv_len = EVP_CIPHER_iv_length(bc_ctx->cipher);
        ret = RAND_bytes(bc_ctx->iv, iv_len);
        if(ret != 1) {
                return(-1);
        }
    }else{
        iv_len =  AXIS2_STRLEN(bc_ctx->iv);
    }

    /*Key supposed to be set before this */
    if(!bc_ctx->key) return (-1);
       
    /*Check if key and IV sizes are not applicable for the cipher*/
    if(EVP_CIPHER_key_length(bc_ctx->cipher) != strlen(bc_ctx->cipher) ){
        printf("Key size is not applicable for the cipher\n");
        return (-1);
    }  

    if(EVP_CIPHER_iv_length(bc_ctx->cipher) != strlen(bc_ctx->iv) ){
        printf("IV size is not applicable for the cipher\n");
        return (-1);
    }  

    /*Init ctx*/    
    EVP_CIPHER_CTX_init(&(bc_ctx->cipher_ctx));
    
    EVP_CipherInit_ex(&(bc_ctx->cipher_ctx), bc_ctx->cipher, NULL, NULL, NULL, encrypt);
    EVP_CIPHER_CTX_set_key_length(&(bc_ctx->cipher_ctx), strlen(bc_ctx->key));
    /* We finished modifying parameters so now we can set key and IV */
    ret  = EVP_CipherInit_ex(&(bc_ctx->cipher_ctx), NULL, NULL, bc_ctx->key, bc_ctx->iv, encrypt);
    
    if(ret < 0 ) return (-1);
    
    bc_ctx->ctx_initialized = 1;
      
    return 0;
}

#if 0

AXIS2_EXTERN openssl_evp_block_cipher_ctx_ptr AXIS2_CALL  
openssl_evp_block_cipher_ctx_init(const axis2_env_t *env,
                             openssl_evp_block_cipher_ctx_ptr bc_ctx,
                             oxs_buffer_ptr in,
                             oxs_buffer_ptr out,
                             int encrypt,
                             const unsigned char* cipher_name)
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
#endif
