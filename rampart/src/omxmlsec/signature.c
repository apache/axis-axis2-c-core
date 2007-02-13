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
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_sign_ctx.h>
#include <oxs_signature.h>
#include <oxs_buffer.h>
#include <openssl_rsa.h>
#include <openssl_digest.h>
/*Private functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_sig_sign_rsa_sha1(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *output)
{
    axis2_char_t *to_be_signed_content = NULL;
    axis2_char_t *encoded_str = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    oxs_buffer_t *digested_buf = NULL;
    oxs_buffer_t *signed_result_buf = NULL;
    openssl_pkey_t *prv_key = NULL;
    openssl_rsa_t *rsa = NULL;
    int signedlen = -1, encodedlen = -1, ret = -1;

    /*Make sha-1 digest. Do we really need to do this OR is it part of OpenSSL operation????*/
    to_be_signed_content = openssl_sha1(env, (axis2_char_t*)OXS_BUFFER_GET_DATA(input, env),  OXS_BUFFER_GET_SIZE(input, env));
  
    /*Create and populate the new input buffer*/
    digested_buf = oxs_buffer_create(env);
    OXS_BUFFER_POPULATE(digested_buf, env, (unsigned char*)to_be_signed_content, axis2_strlen(to_be_signed_content));


    /*Get the key*/
    prv_key = oxs_sign_ctx_get_private_key(sign_ctx, env);

    /*Create output buffer to store signed data*/
    signed_result_buf = oxs_buffer_create(env);

    /*Sign */
    rsa = openssl_rsa_create(env);
    signedlen = OPENSSL_RSA_PRV_ENCRYPT(rsa, env, prv_key, OPENSSL_RSA_PKCS1_PADDING, digested_buf, signed_result_buf);
    if(signedlen < 0){
        /*Error*/
    }
    
    /*Base64 encode*/
    encodedlen = axis2_base64_encode_len(signedlen);
    encoded_str = AXIS2_MALLOC(env->allocator, encodedlen);
    ret = axis2_base64_encode(encoded_str, (const char *)OXS_BUFFER_GET_DATA(signed_result_buf, env), signedlen);
    status = OXS_BUFFER_POPULATE(output, env, (unsigned char*)AXIS2_STRDUP(encoded_str, env), encodedlen);

    /*Free digested_buf*/
    /*Free signed_result_buf*/
    /*Free rsa*/

    return AXIS2_SUCCESS;
}


/*Public functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_sig_sign(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *output)
{
    axis2_char_t *sign_algo = NULL;


    /*Get algo*/
    sign_algo = oxs_sign_ctx_get_sign_mtd_algo(sign_ctx, env);

    /*Prepare content and sign*/
    if(0==(axis2_strcmp(sign_algo, OXS_HREF_RSA_SHA1))){
        oxs_sig_sign_rsa_sha1(env, sign_ctx, input, output);    
    }else if(0==(axis2_strcmp(sign_algo, OXS_HREF_DSA_SHA1))){
        /*Error we do not support*/
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                        "Cannot support cipher %s", sign_algo);
        return AXIS2_FAILURE;
    }else{
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                        "Cannot support cipher %s", sign_algo);
        return AXIS2_FAILURE;
    }


    return AXIS2_SUCCESS;
}

