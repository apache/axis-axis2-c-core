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
#include <openssl_pkey.h>
#include <axis2_util.h>
#include <oxs_buffer.h>

/**
  * @file openssl_rsa.h 
  * @brief For RSA encryption. 
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

    /** Type name for struct openssl_rsa_ops */
    typedef struct openssl_rsa_ops      openssl_rsa_ops_t;
    /** Type name for struct openssl_rsa */
    typedef struct openssl_rsa          openssl_rsa_t;


    struct openssl_rsa_ops
    {
    
      /**
        * Free function
        * @rsa pointer to openssl_rsa struct
        * @env pointer to environment struct
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        **/

        axis2_status_t (AXIS2_CALL *
        free)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env);

      /**
        * Decrypts data using a private key specified in @pkey
        * @rsa pointer to openssl_rsa struct
        * @env pointer to environment struct
        * @pkey private key for decryption
        * @in input data
        * @out output data
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        **/
        int (AXIS2_CALL *
        prv_decrypt)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env,
            const openssl_pkey_t *pkey,
            const axis2_char_t *padding,
            oxs_buffer_t *in,
            oxs_buffer_t *out );

      /**
        * Encrypts data using a public key specified in @pkey
        * @rsa pointer to openssl_rsa struct
        * @env pointer to environment struct
        * @pkey public key for encryption
        * @in input data
        * @out output data
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        **/
        int (AXIS2_CALL*
        pub_encrypt)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env,
            const openssl_pkey_t *pkey,
            const axis2_char_t *padding,
            oxs_buffer_t *in,
            oxs_buffer_t *out );
      
      /**
        * Sign data using a private key specified in @pkey
        * @rsa pointer to openssl_rsa struct
        * @env pointer to environment struct
        * @pkey private key for decryption
        * @in input data
        * @out output data
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        **/
        int (AXIS2_CALL *
        prv_encrypt)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env,
            const openssl_pkey_t *pkey,
            const axis2_char_t *padding,
            oxs_buffer_t *in,
            oxs_buffer_t *out );
      
      /**
        * Verifies data using a public key specified in @pkey
        * @rsa pointer to openssl_rsa struct
        * @env pointer to environment struct
        * @pkey public key for encryption
        * @in input data
        * @out output data
        * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        **/
        int (AXIS2_CALL*
        pub_decrypt)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env,
            const openssl_pkey_t *pkey,
            const axis2_char_t *padding,
            oxs_buffer_t *in,
            oxs_buffer_t *out );
    };

    struct openssl_rsa
    {
        /** operations of openssl_rsa */
        openssl_rsa_ops_t *ops;
    };

    /*Create function*/
    AXIS2_EXTERN openssl_rsa_t *AXIS2_CALL
    openssl_rsa_create(const axis2_env_t *env);

/**********************Macros******************************************/
#define OPENSSL_RSA_FREE(rsa, env) \
        ((rsa)->ops->free(rsa, env) )

#define OPENSSL_RSA_PRV_DECRYPT(rsa, env, pkey, padding, in, out) \
        ((rsa)->ops->prv_decrypt(rsa, env, pkey, padding, in, out) )

#define OPENSSL_RSA_PUB_ENCRYPT(rsa, env, pkey, padding, in, out) \
        ((rsa)->ops->pub_encrypt(rsa, env, pkey, padding, in, out) )

#define OPENSSL_RSA_PRV_ENCRYPT(rsa, env, pkey, padding, in, out) \
        ((rsa)->ops->prv_encrypt(rsa, env, pkey, padding, in, out) )

#define OPENSSL_RSA_PUB_DECRYPT(rsa, env, pkey, padding, in, out) \
        ((rsa)->ops->pub_decrypt(rsa, env, pkey, padding, in, out) )


/** @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_RSA_H */
