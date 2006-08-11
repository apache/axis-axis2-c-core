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
#ifndef OPENSSL_CIPHER_PROPERTY_H
#define OPENSSL_CIPHER_PROPERTY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _openssl_evp_block_cipher_property cipher_prop, *cipher_prop_ptr;

struct _openssl_evp_block_cipher_property {
    EVP_CIPHER*   cipher;
    axis2_char_t *name;
    int         key_size;
    int         block_size;
    int         iv_size;
};


/**
* Create a fresh block cipher property
*@return cipher_prop_ptr
*/
AXIS2_EXTERN cipher_prop_ptr AXIS2_CALL
openssl_cipher_property_create(const axis2_env_t *env);


AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_cipher_property_reset(const axis2_env_t *env, cipher_prop_ptr cprop);

/*TODO write the free function*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL  
openssl_cipher_property_free(const axis2_env_t *env );



/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_CIPHER_PROPERTY_H */
