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

    /** Type name for struct openssl_rsa_ops */
    typedef struct openssl_rsa_ops      openssl_rsa_ops_t;
    /** Type name for struct openssl_rsa */
    typedef struct openssl_rsa          openssl_rsa_t;


    struct openssl_rsa_ops
    {
        axis2_status_t (AXIS2_CALL *
        free)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env);

        int (AXIS2_CALL *
        prv_decrypt)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env,
            const openssl_pkey_t *pkey,
            unsigned char *in,
            unsigned char **out );

        int (AXIS2_CALL*
        pub_encrypt)(
            openssl_rsa_t *rsa,
            const axis2_env_t *env,
            const openssl_pkey_t *pkey,
            unsigned char *in,
            unsigned char **out );
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

#define OPENSSL_RSA_PRV_DECRYPT(rsa, env, pkey, in, out) \
        ((rsa)->ops->prv_decrypt(rsa, env, pkey, in, out) )

#define OPENSSL_RSA_PUB_ENCRYPT(rsa, env, pkey, in, out) \
        ((rsa)->ops->pub_encrypt(rsa, env, pkey, in, out) )


/** @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_RSA_H */
