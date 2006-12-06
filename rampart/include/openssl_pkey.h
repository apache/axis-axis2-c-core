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

/**
  * @file openssl_pkey 
  * @brief holds either a public key or a private key. 
  * The type is determined by the type attribute
  */
#ifndef OPENSSL_PKEY_H
#define OPENSSL_PKEY_H

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup
 * @ingroup
 * @{
 */

#define OPENSSL_PKEY_TYPE_UNKNOWN        0
#define OPENSSL_PKEY_TYPE_PUBLIC_KEY     1
#define OPENSSL_PKEY_TYPE_PRIVATE_KEY    2

    /** Type name for struct openssl_pkey_ops */
    typedef struct openssl_pkey_ops openssl_pkey_ops_t;
    /** Type name for struct openssl_pkey */
    typedef struct openssl_pkey openssl_pkey_t;


    struct openssl_pkey_ops
    {

        EVP_PKEY *(AXIS2_CALL *
        get_key)(
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );
        axis2_char_t *(AXIS2_CALL *
        get_name) (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );
        int (AXIS2_CALL *
        get_size)(
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );
        int (AXIS2_CALL *
        get_type) (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );

        axis2_status_t (AXIS2_CALL *
        set_key )(
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    EVP_PKEY *key
                    );

        axis2_status_t (AXIS2_CALL*
        set_name )(
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    axis2_char_t *name
                    );
        axis2_status_t (AXIS2_CALL*
        set_type )(
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    int type
                    );

        axis2_status_t (AXIS2_CALL*
        load )(
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    axis2_char_t *filename,
                    axis2_char_t *password
                    );

        axis2_status_t (AXIS2_CALL*
        populate)(
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    EVP_PKEY *key,
                    axis2_char_t *name,
                    int type
                    );

        axis2_status_t (AXIS2_CALL*
        free )(
                   openssl_pkey_t *pkey,
                   const axis2_env_t *env
                   );
        
    };

    struct openssl_pkey
    {
        /** operations of openssl_pkey */
        openssl_pkey_ops_t *ops;
    };

    /*Create function*/
    AXIS2_EXTERN openssl_pkey_t *AXIS2_CALL
    openssl_pkey_create(const axis2_env_t *env);


/**********************Macros******************************************/
#define OPENSSL_PKEY_GET_KEY(pkey, env) \
        ((pkey)->ops->get_key(pkey, env))

#define OPENSSL_PKEY_GET_NAME(pkey, env) \
        ((pkey)->ops->get_name(pkey, env))

#define OPENSSL_PKEY_GET_SIZE(pkey, env) \
        ((pkey)->ops->get_size(pkey, env))

#define OPENSSL_PKEY_GET_TYPE(pkey, env) \
        ((pkey)->ops->get_type(pkey, env))

#define OPENSSL_PKEY_SET_KEY(pkey, env, key) \
        ((pkey)->ops->set_key(pkey, env, key))

#define OPENSSL_PKEY_SET_NAME(pkey, env, name) \
        ((pkey)->ops->set_name(pkey, env, name))

#define OPENSSL_PKEY_SET_TYPE(pkey, env, type) \
        ((pkey)->ops->set_type(pkey, env, type))

#define OPENSSL_PKEY_LOAD(pkey, env, filename, password) \
        ((pkey)->ops->load(pkey, env, filename, password))

#define OPENSSL_PKEY_POPULATE(pkey, env, key, name, type) \
        ((pkey)->ops->populate(pkey, env, key, name, type))

#define OPENSSL_PKEY_FREE(pkey, env) \
        ((pkey)->ops->free(pkey, env))


/** @} */
#ifdef __cplusplus
}
#endif

#endif    /* OPENSSL_PKEY_H */
