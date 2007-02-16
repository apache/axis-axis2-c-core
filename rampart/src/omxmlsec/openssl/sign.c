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
#include <oxs_buffer.h>
#include <oxs_key.h>
#include <oxs_error.h>
#include <openssl_cipher_ctx.h>
#include <openssl_sign.h>
#include <openssl_pkey.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>

#define BUFSIZE 64
AXIS2_EXTERN int AXIS2_CALL
openssl_sign(const axis2_env_t *env,
        oxs_sign_ctx_t *sign_ctx,
        oxs_buffer_t *input_buf,
        oxs_buffer_t *output_buf)
{
    openssl_pkey_t *open_pkey = NULL;
    unsigned char sig_buf[4096]; /*Allocate enough memory dynamically*/
    unsigned int sig_len;
    const EVP_MD*   digest;
    EVP_MD_CTX      md_ctx;
    EVP_PKEY*       pkey;
    int err, ret;
    /*Get the key*/
    open_pkey = oxs_sign_ctx_get_private_key(sign_ctx, env);
    pkey = OPENSSL_PKEY_GET_KEY(open_pkey, env);

    /*Set the digest according to the signature method*/
    digest = EVP_sha1();
    /*Sign init*/
    
    ret = EVP_SignInit(&md_ctx, digest);
    
    EVP_SignUpdate (&md_ctx, OXS_BUFFER_GET_DATA(input_buf, env), OXS_BUFFER_GET_SIZE(input_buf, env));
    sig_len = sizeof(sig_buf);
    err = EVP_SignFinal (&md_ctx,
               sig_buf,
               &sig_len,
               pkey);
    if (err != 1) {  
        ERR_print_errors_fp (stderr);     
    }
    /*Fill the output buffer*/
    OXS_BUFFER_POPULATE(output_buf, env, sig_buf, sig_len);

    return sig_len;
}

