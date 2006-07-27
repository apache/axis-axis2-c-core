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

#include <openssl/evp.h>
#include <openssl_cipher_ctx.h>
#include <openssl_constants.h>
#include <axis2_util.h>

/**
  * @file 
  * @brief 
  */
#ifndef OPENSSL_CRYPT_H
#define OPENSSL_CRYPT_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup
 * @ingroup
 * @{
 */

/**
*   
*@param bc_ctx block cipher context
*@param in_buf buffer to en/decrypt
*@param out_buf buffer to place the en/decrypted data
*@param do_encrypt 1 to encrypt 0 to decrypt
*@return -1 if failed
*/
AXIS2_EXTERN int AXIS2_CALL  openssl_block_cipher_crypt(const axis2_env_t *env,     openssl_evp_block_cipher_ctx_ptr bc_ctx,
    unsigned char *in_main_buf, 
    unsigned char **out_main_buf,
    int do_encrypt);



/** @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_CRYPT_H */
