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
  * @file openssl_cipher_property.h 
  * @brief Bean class for cipher properties
  */
#ifndef OPENSSL_CIPHER_PROPERTY_H
#define OPENSSL_CIPHER_PROPERTY_H

#ifdef __cplusplus
extern "C" {
#endif


    /** Type name for struct  openssl_cipher_property_ops */
    typedef struct openssl_cipher_property_ops openssl_cipher_property_ops_t;

    /** Type name for struct  openssl_cipher_property */
    typedef struct openssl_cipher_property openssl_cipher_property_t;

    struct openssl_cipher_property_ops
    {
        EVP_CIPHER *(AXIS2_CALL *
                get_cipher)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env);

        axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env);

        axis2_char_t *(AXIS2_CALL *
                get_url)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env);

        int (AXIS2_CALL *
                get_key_size)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env);

        int (AXIS2_CALL *
                get_block_size)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env);

        int (AXIS2_CALL *
                get_iv_size)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env);
    
        axis2_status_t (AXIS2_CALL *
                set_cipher)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env,
                    EVP_CIPHER *cipher);

        axis2_status_t (AXIS2_CALL *
                set_name)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env,
                    axis2_char_t *name);

        axis2_status_t (AXIS2_CALL *
                set_url)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env,
                    axis2_char_t *url);

        axis2_status_t (AXIS2_CALL *
                set_key_size)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env,
                    int key_size);

        axis2_status_t (AXIS2_CALL *
                set_block_size)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env,
                    int block_size);

        axis2_status_t (AXIS2_CALL *
                set_iv_size)(
                    const openssl_cipher_property_t *cprop,
                    const axis2_env_t *env,
                    int iv_size);
 
        axis2_status_t (AXIS2_CALL *
                free)(
                    openssl_cipher_property_t *cprop,
                    const axis2_env_t *env);

    };

    struct openssl_cipher_property
    {
        openssl_cipher_property_ops_t *ops;    
    };

/**
* Create a fresh block cipher property
*@return cipher_prop_ptr
*/
AXIS2_EXTERN openssl_cipher_property_t *AXIS2_CALL
openssl_cipher_property_create(const axis2_env_t *env);

/******** Macros ***************/

#define OPENSSL_CIPHER_PROPERTY_GET_CIPHER(cprop, env)\
    ((cprop)->ops->get_cipher(cprop, env))

#define OPENSSL_CIPHER_PROPERTY_GET_NAME(cprop, env)\
    ((cprop)->ops->get_name(cprop, env))

#define OPENSSL_CIPHER_PROPERTY_GET_URL(cprop, env)\
    ((cprop)->ops->get_url(cprop, env))

#define OPENSSL_CIPHER_PROPERTY_GET_KEY_SIZE(cprop, env)\
    ((cprop)->ops->get_key_size(cprop, env))

#define OPENSSL_CIPHER_PROPERTY_GET_BLOCK_SIZE(cprop, env)\
    ((cprop)->ops->get_block_size(cprop, env))

#define OPENSSL_CIPHER_PROPERTY_GET_IV_SIZE(cprop, env)\
    ((cprop)->ops->get_iv_size(cprop, env))

#define OPENSSL_CIPHER_PROPERTY_SET_CIPHER(cprop, env, cipher)\
    ((cprop)->ops->set_cipher(cprop, env, cipher))

#define OPENSSL_CIPHER_PROPERTY_SET_NAME(cprop, env, name)\
    ((cprop)->ops->set_name(cprop, env, name))

#define OPENSSL_CIPHER_PROPERTY_SET_URL(cprop, env, url)\
    ((cprop)->ops->set_url(cprop, env, url))

#define OPENSSL_CIPHER_PROPERTY_SET_KEY_SIZE(cprop, env, key_size)\
    ((cprop)->ops->set_key_size(cprop, env, key_size))

#define OPENSSL_CIPHER_PROPERTY_SET_BLOCK_SIZE(cprop, env, block_size)\
    ((cprop)->ops->set_block_size(cprop, env, block_size))

#define OPENSSL_CIPHER_PROPERTY_SET_IV_SIZE(cprop, env, iv_size)\
    ((cprop)->ops->set_iv_size(cprop, env, iv_size))

#define OPENSSL_CIPHER_PROPERTY_FREE(cprop, env)\
    ((cprop)->ops->free(cprop, env))

/* @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_CIPHER_PROPERTY_H */
