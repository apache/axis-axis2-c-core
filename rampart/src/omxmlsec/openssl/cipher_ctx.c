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
#include <openssl_cipher_property.h>
#include <oxs_error.h>
#include <openssl/evp.h>
#include <openssl/rand.h>




AXIS2_EXTERN openssl_evp_block_cipher_ctx_ptr AXIS2_CALL
openssl_evp_block_cipher_ctx_create(const axis2_env_t *env)
{
    openssl_evp_block_cipher_ctx_ptr   bc_ctx = NULL;
    bc_ctx = (openssl_evp_block_cipher_ctx_ptr)AXIS2_MALLOC(env->allocator,sizeof(openssl_evp_block_cipher_ctx));
    openssl_evp_block_cipher_ctx_reset(env, bc_ctx); 
    return bc_ctx;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_evp_block_cipher_ctx_reset(const axis2_env_t *env, openssl_evp_block_cipher_ctx_ptr bc_ctx)
{
    bc_ctx->cipher = NULL;
    bc_ctx->key_initialized = -1;
    bc_ctx->ctx_initialized = -1;
    bc_ctx->key = NULL;
    bc_ctx->iv = NULL;
    bc_ctx->pad = NULL;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t  AXIS2_CALL  
openssl_evp_block_cipher_ctx_init(const axis2_env_t *env,
                             openssl_evp_block_cipher_ctx_ptr bc_ctx,
                             int encrypt,
                             const unsigned char* cipher_name)
{
    
    /*int iv_len;*/
    axis2_status_t ret =  AXIS2_FAILURE;

    
    /*If bc_ctx is NULL create a new one*/
    if(!bc_ctx){
        printf("Given bc_ctx is NULL. Creating a new one\n");
        bc_ctx = openssl_evp_block_cipher_ctx_create(env);
        if(!bc_ctx){
             return AXIS2_FAILURE;
        } 
    }    
    printf("\nCreating cipher ctx for %s", cipher_name);

    bc_ctx->cipher =  (EVP_CIPHER*)openssl_get_evp_cipher_by_name(env, (axis2_char_t*)cipher_name);

    /*Key supposed to be set before this */
    if(!bc_ctx->key){
         return AXIS2_FAILURE;
    };
       
    /*Check if key and IV sizes are not applicable for the cipher*/
#if 1
    if(EVP_CIPHER_key_length(bc_ctx->cipher) != strlen((char*)bc_ctx->key) ){
        printf("WARNING : Key size is not applicable for the cipher %d = %d\n", EVP_CIPHER_key_length(bc_ctx->cipher),  strlen((char*)bc_ctx->key)  );
        
    }  

    if(EVP_CIPHER_iv_length(bc_ctx->cipher) != strlen((char*)bc_ctx->iv) ){
        printf("WARNING : IV size is not applicable for the cipher %d = %d\n", EVP_CIPHER_iv_length(bc_ctx->cipher) , strlen((char*)bc_ctx->iv) );
        
    }  
#endif

    /*Init ctx*/    
    EVP_CIPHER_CTX_init(&(bc_ctx->cipher_ctx));
    
    EVP_CipherInit_ex(&(bc_ctx->cipher_ctx), bc_ctx->cipher, NULL, NULL, NULL, encrypt);
    EVP_CIPHER_CTX_set_key_length(&(bc_ctx->cipher_ctx), strlen((axis2_char_t*)bc_ctx->key));
    /* We finished modifying parameters so now we can set key and IV */
    ret  = EVP_CipherInit_ex(&(bc_ctx->cipher_ctx), NULL, NULL, bc_ctx->key, bc_ctx->iv, encrypt);
    
    if(ret < 0 ){
         return AXIS2_FAILURE;
    }
    
    bc_ctx->ctx_initialized = 1;
      
    return AXIS2_SUCCESS;
}
/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_evp_block_cipher_ctx_free(const axis2_env_t *env, openssl_evp_block_cipher_ctx_ptr bc_ctx)
{
    

}*/
