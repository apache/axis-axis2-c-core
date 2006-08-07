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
#include<openssl_cipher_property.h>

/**
  * @file 
  * @brief 
  */
#ifndef OPENSSL_UTIL_H
#define OPENSSL_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif
/*Get the cipher property for a given cipher name
  @see openssl_cipher_property.h*/
AXIS2_EXTERN cipher_prop_ptr  AXIS2_CALL
openssl_get_cipher_property(const axis2_env_t *env, axis2_char_t *cipher_name);

/*Get a cipher for a given name*/
AXIS2_EXTERN EVP_CIPHER*  AXIS2_CALL
openssl_get_evp_cipher_by_name(const axis2_env_t *env, axis2_char_t *cipher_name);


/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_UTIL_H */
