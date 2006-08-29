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
#include <openssl_pkey.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <oxs_buffer.h>
#include <oxs_error.h>

/**
struct _evp_pkey{
    EVP_PKEY *key;
    unsigned char *name;
    int size;
    int type;
}
*/
typedef struct openssl_pkey_impl{
    openssl_pkey_t pkey;
    
    EVP_PKEY *key;
    axis2_char_t *name;
    int type;
}openssl_pkey_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(openssl_pkey) ((openssl_pkey_impl_t *)openssl_pkey)

/******************* function headers ******************************/
/* private functions */
static void
openssl_pkey_init_ops(
                    openssl_pkey_t *pkey);

/* public functions */
EVP_PKEY *AXIS2_CALL
openssl_pkey_get_key (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );
axis2_char_t *AXIS2_CALL
openssl_pkey_get_name (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );
int AXIS2_CALL
openssl_pkey_get_size (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );
int AXIS2_CALL
openssl_pkey_get_type (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    );


axis2_status_t AXIS2_CALL
openssl_pkey_set_key (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    EVP_PKEY *key
                    );

axis2_status_t AXIS2_CALL
openssl_pkey_set_name (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    axis2_char_t *name
                    );
axis2_status_t AXIS2_CALL
openssl_pkey_set_type (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    int type
                    );

axis2_status_t AXIS2_CALL
openssl_pkey_load (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    axis2_char_t *filename,
                    axis2_char_t *password
                    );

axis2_status_t AXIS2_CALL
openssl_pkey_populate(
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    EVP_PKEY *key, 
                    axis2_char_t *name, 
                    int type
                    );

axis2_status_t AXIS2_CALL
openssl_pkey_free (
                   openssl_pkey_t *pkey,
                   const axis2_env_t *env
                   );

/********************** end of function headers **************/

AXIS2_EXTERN openssl_pkey_t *AXIS2_CALL
openssl_pkey_create(const axis2_env_t *env)
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, NULL);
    pkey_impl = AXIS2_MALLOC(env->allocator,sizeof(openssl_pkey_impl_t));
    if (!pkey_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    pkey_impl->key   = NULL;
    pkey_impl->name = NULL ;
    pkey_impl->type = OPENSSL_PKEY_TYPE_UNKNOWN;

    pkey_impl->pkey.ops =  AXIS2_MALLOC(env->allocator,sizeof(openssl_pkey_ops_t));
    if (!pkey_impl->pkey.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        openssl_pkey_free(&(pkey_impl->pkey), env);
        return NULL;
    }

    openssl_pkey_init_ops(&(pkey_impl->pkey));

    return &(pkey_impl->pkey);

}

static void
openssl_pkey_init_ops(
                    openssl_pkey_t *pkey)
{
    pkey->ops->get_key  = openssl_pkey_get_key;
    pkey->ops->get_name = openssl_pkey_get_name;
    pkey->ops->get_size = openssl_pkey_get_size;
    pkey->ops->get_type = openssl_pkey_get_type;
    pkey->ops->set_key  = openssl_pkey_set_key;
    pkey->ops->set_name = openssl_pkey_set_name;
    pkey->ops->set_type = openssl_pkey_set_type;
    pkey->ops->populate = openssl_pkey_populate;
    pkey->ops->load     = openssl_pkey_load;
    pkey->ops->free     = openssl_pkey_free;

}

EVP_PKEY *AXIS2_CALL
openssl_pkey_get_key (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, NULL);
    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    return pkey_impl->key ;
}

axis2_char_t *AXIS2_CALL
openssl_pkey_get_name (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, NULL);
    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    return pkey_impl->name ;

}
int AXIS2_CALL
openssl_pkey_get_size (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    return sizeof(pkey_impl->key) ;
}
int AXIS2_CALL
openssl_pkey_get_type (
                    const openssl_pkey_t *pkey,
                    const axis2_env_t *env
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    return pkey_impl->type ;
}

axis2_status_t AXIS2_CALL
openssl_pkey_set_key (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    EVP_PKEY *key
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);
    if (pkey_impl->key){
        AXIS2_FREE(env->allocator, pkey_impl->key);
        pkey_impl->key = NULL;
    }

    pkey_impl->key = key;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_pkey_set_name (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    axis2_char_t *name
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);
    if (pkey_impl->name){
        AXIS2_FREE(env->allocator, pkey_impl->name);
        pkey_impl->name = NULL;
    }

    pkey_impl->name = name;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_pkey_set_type (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    int type
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    pkey_impl->type = type;

    return AXIS2_SUCCESS;

}

axis2_status_t AXIS2_CALL
openssl_pkey_load (
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    axis2_char_t *filename,
                    axis2_char_t *password
                    )
{
    EVP_PKEY *pk = NULL;
    BIO *bio;
    int type = OPENSSL_PKEY_TYPE_UNKNOWN;
    int ret ;
    openssl_pkey_impl_t * pkey_impl= NULL;
    axis2_status_t status = AXIS2_FAILURE;
 
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    bio = BIO_new_file(filename, "rb");
    /*Try to read the prv key first*/
    pk = PEM_read_bio_PrivateKey(bio, NULL, 0 , password);
    if(!pk){
        /*If prv key is not found then read the public key*/
        ret  = BIO_reset(bio);
        pk = PEM_read_bio_PUBKEY(bio, NULL, 0 , password);
        if(!pk){
            /*If there is no key by now its an error*/
            oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "Cannot load key from %s", filename);

            return AXIS2_FAILURE;
        }
        type = OPENSSL_PKEY_TYPE_PUBLIC_KEY;
    }else{
        type = OPENSSL_PKEY_TYPE_PRIVATE_KEY;
    }

    status = openssl_pkey_populate(pkey, env,  pk, filename, type) ;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_pkey_populate(
                    openssl_pkey_t *pkey,
                    const axis2_env_t *env,
                    EVP_PKEY *key,
                    axis2_char_t *name,
                    int type
                    )
{
    openssl_pkey_impl_t * pkey_impl= NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    status = openssl_pkey_set_key(pkey, env, key);
    status = openssl_pkey_set_name(pkey, env, name);
    status = openssl_pkey_set_type(pkey, env, type);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
openssl_pkey_free (
                   openssl_pkey_t *pkey,
                   const axis2_env_t *env
                   )
{
    openssl_pkey_impl_t * pkey_impl= NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);

    if (pkey_impl->key){
        AXIS2_FREE(env->allocator, pkey_impl->key);
        pkey_impl->key = NULL;
    }
    if (pkey_impl->name){
        AXIS2_FREE(env->allocator, pkey_impl->name);
        pkey_impl->name = NULL;
    }

    pkey_impl = AXIS2_INTF_TO_IMPL(pkey);
    AXIS2_FREE(env->allocator, pkey_impl);
    pkey_impl = NULL;

    return AXIS2_SUCCESS;
}




