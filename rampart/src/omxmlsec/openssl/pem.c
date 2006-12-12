/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
#include <openssl_pkcs12.h>
#include <openssl_pem.h>


AXIS2_EXTERN axis2_status_t AXIS2_CALL
openssl_pem_read_pkey(const axis2_env_t *env,
    axis2_char_t *filename,
    axis2_char_t *password,
    openssl_pem_pkey_type_t type,
    EVP_PKEY **pkey)
{
    BIO *bio;
    int ret; 

    bio = BIO_new_file(filename, "rb");

    if(type == OPENSSL_PEM_PKEY_TYPE_PRIVATE_KEY){
        *pkey = PEM_read_bio_PrivateKey(bio, NULL, 0 , password);
    }else if(type == OPENSSL_PEM_PKEY_TYPE_PUBLIC_KEY){ 
        *pkey = PEM_read_bio_PUBKEY(bio, NULL, 0 , password);
    }else{/*Type unknown*/
        /*Try to read the prv key first*/
        *pkey = PEM_read_bio_PrivateKey(bio, NULL, 0 , password);
        if (!*pkey)
        {
            /*If prv key is not found then read the public key*/
            ret  = BIO_reset(bio);
            *pkey = PEM_read_bio_PUBKEY(bio, NULL, 0 , password);
        }
    }

    ret = BIO_reset(bio);
    ret = BIO_free(bio);
    bio = NULL;
    if(!*pkey){
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}


