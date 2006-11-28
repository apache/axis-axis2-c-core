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
#include <oxs_constants.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_encryption.h>
#include <oxs_cipher.h>
#include <oxs_iv.h>
#include <openssl_cipher_ctx.h>
#include <openssl_crypt.h>
#include <openssl_constants.h>
#include <openssl_rsa.h>
#include <openssl_util.h>
#include <oxs_key_mgr.h>


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_encryption_symmetric_crypt(const axis2_env_t *env,
    oxs_ctx_t * enc_ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *result)
{
    openssl_cipher_ctx_t *oc_ctx = NULL;
    openssl_cipher_property_t *cprop = NULL;
    axis2_char_t *iv = NULL;
    axis2_char_t *cipher_name = NULL;
    axis2_status_t ret = AXIS2_FAILURE;
    
    /*Get cipher property*/
    cprop =  oxs_get_cipher_property_for_url(env, OXS_CTX_GET_ENC_MTD_ALGORITHM(enc_ctx, env));
    
    /*Get the IV*/
    iv = AXIS2_STRNDUP((axis2_char_t*)oxs_iv_generate_for_algo(
                env,
                OXS_CTX_GET_ENC_MTD_ALGORITHM(enc_ctx, env)),
            OPENSSL_CIPHER_PROPERTY_GET_IV_SIZE(cprop, env),
            env);

    /*Create the openssl context*/
    oc_ctx = openssl_cipher_ctx_create(env);
    if (!oc_ctx)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "openssl_cipher_ctx_create failed");
        return AXIS2_FAILURE;
    }

    /*Set IV*/
    ret = OPENSSL_CIPHER_CTX_SET_IV(oc_ctx, env, iv);

    /*Set key*/
    ret = OPENSSL_CIPHER_CTX_SET_KEY(oc_ctx, env, OXS_CTX_GET_KEY(enc_ctx, env));

    /*Set the cipher*/
    cipher_name = (axis2_char_t*)OPENSSL_CIPHER_PROPERTY_GET_NAME(cprop, env);
    if (!cipher_name)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "oxs_get_cipher failed");

        return AXIS2_FAILURE;
    }
    ret = OPENSSL_CIPHER_CTX_SET_CIPHER(oc_ctx,
            env,
            (EVP_CIPHER*)openssl_get_evp_cipher_by_name(
                env, (axis2_char_t*)cipher_name)
            );
    
    /*Now everything is ready for the en/decryption*/ 
    /*ENCRYPTION*/
    if (OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_ENCRYPT)
    {
        axis2_char_t *encoded_str = NULL;
        unsigned char *out_main_buf = NULL;
        int enclen = -1;
        int encodedlen = -1;

        enclen = openssl_block_cipher_crypt(env, oc_ctx,
                OXS_BUFFER_GET_DATA(input, env),
                OXS_BUFFER_GET_SIZE(input, env),
                &out_main_buf, OPENSSL_ENCRYPT);
        if(enclen < 0){
            oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                    "openssl_block_cipher_crypt");
            return AXIS2_FAILURE;
        }

        encodedlen = axis2_base64_encode_len(enclen);
        encoded_str = AXIS2_MALLOC(env->allocator, encodedlen);
        ret = axis2_base64_encode_binary(encoded_str, out_main_buf, enclen);
        if (ret < 0)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "axis2_base64_encode_binary failed");
            return AXIS2_FAILURE;
        }

        /*Attach the result to the result buf*/
        ret = OXS_BUFFER_POPULATE(result, env, (unsigned char*)AXIS2_STRDUP(encoded_str, env), encodedlen);
        
        /*Free*/
        AXIS2_FREE(env->allocator, encoded_str);
        encoded_str = NULL;

    /*DECRYPTION*/
    }else if(OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_DECRYPT){
        unsigned char *decoded_data = NULL;/*Can be binary*/
        unsigned char *out_main_buf = NULL;
        int decoded_len = -1;
        int enclen = -1;

        /*First we need to base64 decode*/
        decoded_data = AXIS2_MALLOC(env->allocator, axis2_base64_decode_len((char*)OXS_BUFFER_GET_DATA(input, env)));
        decoded_len = axis2_base64_decode_binary(decoded_data, (char*)OXS_BUFFER_GET_DATA(input, env) );
        if (decoded_len < 0)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                    "axis2_base64_decode_binary failed");
        }
        /*Then we decrypt*/
        enclen = openssl_block_cipher_crypt(env, oc_ctx,
                decoded_data, decoded_len,  &out_main_buf, OPENSSL_DECRYPT);

        ret = OXS_BUFFER_POPULATE(result, env, AXIS2_STRMEMDUP(out_main_buf, enclen, env), enclen);
        /*Free*/
        AXIS2_FREE(env->allocator, decoded_data);
        decoded_data = NULL;

    }else{
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Invalid operation type %d", OXS_CTX_GET_OPERATION(enc_ctx, env));
        return AXIS2_FAILURE;
    }
  
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN  axis2_status_t AXIS2_CALL
oxs_encryption_asymmetric_crypt(const axis2_env_t *env,
    oxs_asym_ctx_t *ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *result)
{
    openssl_pkey_t *pkey = NULL;
    openssl_rsa_t *rsa = NULL;
    oxs_asym_ctx_operation_t operation = -1;
    axis2_status_t status = AXIS2_FAILURE;
    
    /*TODO We support RSA encryption only. If any other algorithm is specified, reject*/


    /*Load the key using key manager*/
    status = oxs_key_mgr_load_key(env, ctx);

        
#if 0
    /*1. Try to get the pkey from the asy_ctx*/
    axis2_char_t *file_name = NULL;
    /*2. If not try to load the key from the dec_prop_file*/
    file_name = oxs_asym_ctx_get_file_name(ctx, env);
    pkey =  openssl_pkey_create(env);
    status = OPENSSL_PKEY_LOAD(pkey, env, file_name, "");/*TODO password*/
#endif

    /*Check for the operation and call appropriate method*/
    operation = oxs_asym_ctx_get_operation(ctx, env);
    rsa = openssl_rsa_create(env);
    if(   OXS_ASYM_CTX_OPERATION_PUB_ENCRYPT == operation ){
        axis2_char_t *encoded_str = NULL;
        oxs_x509_cert_t *x509_cert = NULL;
        unsigned char *encrypted = NULL;
        int enclen = -1;
        int encodedlen = -1;
        int ret = -1;
       
        /*Operation is PUB ENCRYPT; Get the public key from the context*/
        x509_cert = oxs_asym_ctx_get_certificate(ctx, env);
        pkey = oxs_x509_cert_get_public_key(x509_cert, env);

        /*Encrypt using the public key. Then base64 encode and populate the buffer */
        enclen = OPENSSL_RSA_PUB_ENCRYPT(rsa, env, pkey, OXS_BUFFER_GET_DATA(input, env), &encrypted);
        encodedlen = axis2_base64_encode_len(enclen);
        encoded_str = AXIS2_MALLOC(env->allocator, encodedlen);
        ret = axis2_base64_encode(encoded_str, (const char *)encrypted, enclen); 
        status = OXS_BUFFER_POPULATE(result, env, (unsigned char*)AXIS2_STRDUP(encoded_str, env), encodedlen);

    }else if(OXS_ASYM_CTX_OPERATION_PRV_DECRYPT == operation ){
        unsigned char  *decoded_encrypted_str = NULL;
        unsigned char *decrypted  =  NULL;
        int ret = -1;
        int  declen = -1;

        /*Operation id PRV DECRYPT; Get the private key from the context*/
        pkey = oxs_asym_ctx_get_private_key(ctx, env);
        /*Base64 decode first. Then do the decryption and populate the buffer*/
        decoded_encrypted_str = AXIS2_MALLOC(env->allocator, axis2_base64_decode_len((char*)OXS_BUFFER_GET_DATA(input, env)));
        ret = axis2_base64_decode((char*)decoded_encrypted_str, (char*)OXS_BUFFER_GET_DATA(input, env));
        declen = OPENSSL_RSA_PRV_DECRYPT(rsa, env, pkey, decoded_encrypted_str, &decrypted);
        status = OXS_BUFFER_POPULATE(result, env, decrypted, declen);
    
    }else if(OXS_ASYM_CTX_OPERATION_PRV_ENCRYPT == operation ){
        /**/
    }else if(OXS_ASYM_CTX_OPERATION_PRV_ENCRYPT == operation ){
        /**/
    }else{

    }

    /*TODO Set certificate information taken from the PEM file */
    
    return AXIS2_SUCCESS;
}
