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
#include <openssl_util.h>
#include <openssl_constants.h>
#include <openssl_cipher_property.h>
#include <openssl_util.h>
#include <oxs_error.h>
#include <openssl/evp.h>
#include <openssl/rand.h>


AXIS2_EXTERN cipher_prop_ptr  AXIS2_CALL
openssl_get_cipher_property(const axis2_env_t *env, axis2_char_t *cipher_name)
{
    int ret, key_length;
    EVP_CIPHER* cipher;
    EVP_CIPHER_CTX ctx;
    cipher_prop_ptr cprop = NULL;
    
   
    cipher = openssl_get_evp_cipher_by_name(env, cipher_name); 
    if(!cipher){
         oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "openssl_get_evp_cipher_by_name failed");

        return NULL;
    }
    /*Initialize a cipher ctx*/
    EVP_CIPHER_CTX_init(&ctx);
    EVP_CipherInit_ex(&ctx, cipher, NULL, NULL, NULL, -1);
    
    /*Create a cipher property and populate it*/
    cprop = openssl_cipher_property_create(env);
    cprop->cipher = cipher;
    cprop->name = AXIS2_STRDUP(cipher_name, env);
    cprop->key_size = EVP_CIPHER_CTX_key_length(&ctx);
    cprop->block_size = EVP_CIPHER_CTX_block_size(&ctx);
    cprop->iv_size = EVP_CIPHER_CTX_iv_length(&ctx);
    return cprop;
}

AXIS2_EXTERN EVP_CIPHER*  AXIS2_CALL
openssl_get_evp_cipher_by_name(const axis2_env_t *env, axis2_char_t *cipher_name)
{
    if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_des_ede3_cbc ))
    {
        return EVP_des_ede3_cbc();
    }else if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_aes_128_cbc ))
    {
        return EVP_aes_128_cbc();
    }else if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_aes_128_cbc ))
    {
        return EVP_aes_192_cbc();
    }else if(AXIS2_STRCMP((char*)cipher_name, (char*)OPENSSL_EVP_aes_128_cbc ))
    {
        return EVP_aes_256_cbc();
    }else{
        return NULL;
    }
}


