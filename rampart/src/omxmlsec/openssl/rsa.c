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
#include <openssl_rsa.h>
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

AXIS2_EXTERN int AXIS2_CALL
generate_random_key(const axis2_env_t *env, oxs_buffer_ptr buf, int size)
{
    int ret;
    ret = oxs_buffer_set_size(env, buf, size);
    if(ret < 0){
        oxs_error(ERROR_LOCATION,
                OXS_ERROR_DEFAULT, "oxs_buffer_set_size failed %d",size );
        return (-1);
    }
    ret = RAND_bytes(buf->data, size);
    if(ret < 0){
        oxs_error(ERROR_LOCATION,
                OXS_ERROR_DEFAULT, "RAND_bytes failed %d",size );
        return (-1);
    }
    return (0);
}


AXIS2_EXTERN evp_pkey_ptr AXIS2_CALL
evp_pkey_create(const axis2_env_t *env)
{
    evp_pkey_ptr pkey = NULL;
    pkey = (evp_pkey_ptr) AXIS2_MALLOC(env->allocator, sizeof(evp_pkey));
    return pkey;    
}

AXIS2_EXTERN evp_pkey_ptr AXIS2_CALL
evp_pkey_init(const axis2_env_t *env, evp_pkey_ptr pkey, EVP_PKEY *key, axis2_char_t *name, int type)
{
    if(!pkey){
        pkey = evp_pkey_create(env);   
    }
 
    pkey->key = key;
    pkey->name = name;
    pkey->size = sizeof(key);
    pkey->type = type;

    return pkey;
}

AXIS2_EXTERN evp_pkey_ptr AXIS2_CALL
evp_pkey_load(const axis2_env_t *env, axis2_char_t *filename, axis2_char_t *password)
{
    EVP_PKEY *pk = NULL;
    BIO *bio;
    evp_pkey_ptr pkey = NULL; 
    int type = OPENSSL_EVP_KEY_TYPE_UNKNOWN;

    bio = BIO_new_file(filename, "rb");
    /*Try to read the prv key first*/
    pk = PEM_read_bio_PrivateKey(bio, NULL, 0 , password);
    if(!pk){
        /*If prv key is not found then read the public key*/     
        BIO_reset(bio);
        pk = PEM_read_bio_PUBKEY(bio, NULL, 0 , password);
        if(!pk){
            /*If there is no key by now its an error*/
            return NULL;
        }
        type = OPENSSL_EVP_KEY_TYPE_PUBLIC_KEY;
    }else{
        type = OPENSSL_EVP_KEY_TYPE_PRIVATE_KEY;
    }
     
    pkey = evp_pkey_create(env);
    pkey = evp_pkey_init(env, pkey, pk, filename, type) ;
   
    return pkey;  
    
}

AXIS2_EXTERN int  AXIS2_CALL
openssl_rsa_pub_encrypt(const axis2_env_t *env, evp_pkey_ptr pubkey, unsigned char *in, unsigned char **out )
{
    unsigned char *encrypted = NULL;
    int ret;
    encrypted = malloc(RSA_size(pubkey->key->pkey.rsa));
    ret = RSA_public_encrypt(strlen((char*)in), in, encrypted, pubkey->key->pkey.rsa, RSA_PKCS1_PADDING);
    if(ret < 0) {
        printf("Encryption failed \n");
    }
    *out = encrypted;
    return ret;
}

AXIS2_EXTERN int  AXIS2_CALL
openssl_rsa_prv_decrypt(const axis2_env_t *env, evp_pkey_ptr prvkey, unsigned char *in, unsigned char **out )
{
    unsigned char *decrypted = NULL;
    int ret;
    decrypted = malloc(RSA_size(prvkey->key->pkey.rsa));
    ret = RSA_private_decrypt(RSA_size(prvkey->key->pkey.rsa), in, decrypted, prvkey->key->pkey.rsa, RSA_PKCS1_PADDING);
    if(ret < 0) {
        printf("Encryption failed \n");
    }
    *out = decrypted;
    return ret;
}



