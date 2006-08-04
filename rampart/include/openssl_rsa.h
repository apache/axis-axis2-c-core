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

#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl_constants.h>
#include <axis2_util.h>
#include <oxs_buffer.h>

/**
  * @file 
  * @brief 
  */
#ifndef OPENSSL_RSA_H
#define OPENSSL_RSA_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup
 * @ingroup
 * @{
 */

#define OPENSSL_EVP_KEY_TYPE_UNKNOWN        0
#define OPENSSL_EVP_KEY_TYPE_PUBLIC_KEY     1
#define OPENSSL_EVP_KEY_TYPE_PRIVATE_KEY    2


typedef struct _evp_pkey evp_pkey, *evp_pkey_ptr;

struct _evp_pkey{
    EVP_PKEY *key;
    unsigned char *name;
    int size;
    int type;
};

AXIS2_EXTERN axis2_status_t AXIS2_CALL
generate_random_key(const axis2_env_t *env, oxs_buffer_ptr buf, int size);

AXIS2_EXTERN evp_pkey_ptr AXIS2_CALL
evp_pkey_create(const axis2_env_t *env);

AXIS2_EXTERN evp_pkey_ptr AXIS2_CALL
evp_pkey_init(const axis2_env_t *env, evp_pkey_ptr pkey, EVP_PKEY *key, axis2_char_t *name, int type);

AXIS2_EXTERN evp_pkey_ptr AXIS2_CALL
evp_pkey_load(const axis2_env_t *env, axis2_char_t *filename,  axis2_char_t *password);

AXIS2_EXTERN int  AXIS2_CALL
openssl_rsa_pub_encrypt(const axis2_env_t *env, evp_pkey_ptr pubkey, unsigned char *in, unsigned char **out );

AXIS2_EXTERN int  AXIS2_CALL
openssl_rsa_prv_decrypt(const axis2_env_t *env, evp_pkey_ptr prvkey, unsigned char *in, unsigned char **out );

/** @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_RSA_H */
