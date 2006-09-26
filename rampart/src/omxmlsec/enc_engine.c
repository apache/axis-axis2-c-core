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
#include <oxs_axiom.h>
#include <axis2_qname.h>
#include <axiom_namespace.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_enc_engine.h>
#include <oxs_cipher.h>
#include <openssl_cipher_ctx.h>
#include <openssl_crypt.h>
#include <openssl_constants.h>
#include <openssl_rsa.h>
#include <oxs_token_cipher_value.h>
#include <oxs_iv.h>
#include <openssl_util.h>

typedef struct oxs_enc_engine_impl
{
    oxs_enc_engine_t enc_engine;
}
oxs_enc_engine_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(enc_engine) ((oxs_enc_engine_impl_t *)enc_engine)

/** private functions */
static void
oxs_enc_engine_init_ops(
    oxs_enc_engine_t *enc_engine);

static axis2_status_t
oxs_enc_engine_cipher_data_node_read(
    const axis2_env_t *env,
    oxs_ctx_t * enc_ctx,
    axiom_node_t* node);

static axis2_status_t
oxs_enc_engine_encrypted_data_node_read(
    const axis2_env_t *env,
    oxs_ctx_t * enc_ctx,
    axiom_node_t* node);

/** public function */
axis2_status_t AXIS2_CALL
oxs_enc_engine_free(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
oxs_enc_engine_prvkey_decrypt_data(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    oxs_buffer_t * input,
    oxs_buffer_t * result,
    axis2_char_t *filename);

axis2_status_t AXIS2_CALL
oxs_enc_engine_pubkey_encrypt_data(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    oxs_buffer_t * input,
    oxs_buffer_t * result,
    axis2_char_t *filename);

axis2_status_t AXIS2_CALL
oxs_enc_engine_get_encrypted_key(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t *enc_key_node,
    oxs_key_t *prv_key,
    oxs_key_t *session_key);

axis2_status_t AXIS2_CALL
oxs_enc_engine_crypt(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    oxs_ctx_t * enc_ctx,
    oxs_buffer_t * input,
    oxs_buffer_t * result);

axis2_status_t AXIS2_CALL
oxs_enc_engine_populate_cipher_value(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t* template_node,
    oxs_buffer_t * databuf);

axis2_status_t AXIS2_CALL
oxs_enc_engine_decrypt_template(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t* template_node,
    axis2_char_t** decrypted_data,
    oxs_ctx_t * enc_ctx);

axis2_status_t AXIS2_CALL
oxs_enc_engine_encrypt_template(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t* template_node,
    axis2_char_t* data,
    oxs_ctx_t * enc_ctx);

/******* end of function headers ****************/

axis2_status_t AXIS2_CALL
oxs_enc_engine_free(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env)
{
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);

    if (enc_engine->ops)
    {
        AXIS2_FREE(env->allocator, enc_engine->ops);
        enc_engine->ops = NULL;
    }
    if (enc_engine_impl)
    {
        AXIS2_FREE(env->allocator, enc_engine_impl);
        enc_engine_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

oxs_enc_engine_t *AXIS2_CALL
oxs_enc_engine_create(
    const axis2_env_t *env)
{
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    enc_engine_impl = (oxs_enc_engine_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(oxs_enc_engine_impl_t));

    if (NULL == enc_engine_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    enc_engine_impl->enc_engine.ops = AXIS2_MALLOC(env->allocator,
            sizeof(oxs_enc_engine_ops_t));
    if (NULL == enc_engine_impl->enc_engine.ops)
    {
        oxs_enc_engine_free(&(enc_engine_impl->enc_engine), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    oxs_enc_engine_init_ops(&(enc_engine_impl->enc_engine));

    return &(enc_engine_impl->enc_engine);

}

static void
oxs_enc_engine_init_ops(
    oxs_enc_engine_t *enc_engine)
{
    enc_engine->ops->free = oxs_enc_engine_free;
    enc_engine->ops->prvkey_decrypt_data = oxs_enc_engine_prvkey_decrypt_data;
    enc_engine->ops->pubkey_encrypt_data = oxs_enc_engine_pubkey_encrypt_data;
    enc_engine->ops->get_encrypted_key = oxs_enc_engine_get_encrypted_key;
    enc_engine->ops->crypt = oxs_enc_engine_crypt;
    enc_engine->ops->populate_cipher_value = oxs_enc_engine_populate_cipher_value;
    enc_engine->ops->decrypt_template = oxs_enc_engine_decrypt_template;
    enc_engine->ops->encrypt_template = oxs_enc_engine_encrypt_template;

}

axis2_status_t AXIS2_CALL
oxs_enc_engine_get_encrypted_key(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t *enc_key_node,
    oxs_key_t *prv_key,
    oxs_key_t *session_key)
{
    axis2_char_t *key_enc_algo = NULL;
    axis2_char_t *encrypted_key_value = NULL;
    axiom_node_t *enc_method_node = NULL;
    axiom_node_t *cd_node = NULL;
    axiom_node_t *cv_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    oxs_buffer_t *encrypted_key_buf = NULL;
    oxs_buffer_t *decrypted_key_buf = NULL;
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);


    /*Verify*/
    if (!enc_key_node)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                "Passed encrypted key is NULL");
        return AXIS2_FAILURE;
    }

    enc_method_node = (axiom_node_t*)oxs_axiom_get_first_child_node_by_name(env, enc_key_node, OXS_NodeEncryptionMethod, NULL, NULL);
    if (!enc_method_node)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                "Cannot find EncryptionMethodElement");
        return AXIS2_FAILURE;
    }

    key_enc_algo =  oxs_token_get_encryption_method(env, enc_method_node);
    if (!key_enc_algo)
    {
        /*If not found use default*/
        key_enc_algo = OXS_DEFAULT_KT_ALGO_HREF;
    }

    cd_node = (axiom_node_t*)oxs_axiom_get_first_child_node_by_name(env, enc_key_node, OXS_NodeCipherData, NULL, NULL);
    if (!cd_node)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                "Cannot find CipherData element");
        return AXIS2_FAILURE;
    }

    cv_node = (axiom_node_t*)oxs_axiom_get_first_child_node_by_name(env, cd_node, OXS_NodeCipherValue, NULL, NULL);
    if (!cv_node)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                "Cannot find CipherValue element");
        return AXIS2_FAILURE;
    }
    /*Encrypted key*/
    encrypted_key_value = (axis2_char_t*)oxs_token_get_cipher_value(env, cv_node);

    /*Create buffers for decryption*/
    encrypted_key_buf = oxs_buffer_create(env);
    status = OXS_BUFFER_POPULATE(encrypted_key_buf, env,
            (unsigned char *)encrypted_key_value,
            AXIS2_STRLEN(encrypted_key_value));
    decrypted_key_buf = oxs_buffer_create(env);

    /*Decrypt the encrypted key*/
    status  = oxs_enc_engine_prvkey_decrypt_data(enc_engine, env, encrypted_key_buf, decrypted_key_buf, OXS_KEY_GET_NAME(prv_key, env));
    if (status == AXIS2_FAILURE)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "oxs_prvkey_decrypt_data failed");
        return AXIS2_FAILURE;
    }
    /*Trim data to the key size*/
    OXS_KEY_SET_DATA(session_key, env, OXS_BUFFER_GET_DATA(decrypted_key_buf, env));
    OXS_KEY_SET_SIZE(session_key, env, OXS_BUFFER_GET_SIZE(decrypted_key_buf, env));
    OXS_KEY_SET_USAGE(session_key, env, OXS_KEY_USAGE_DECRYPT);

    return AXIS2_SUCCESS;
}


/*Decrypt data using the private key*/
axis2_status_t AXIS2_CALL
oxs_enc_engine_prvkey_decrypt_data(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    oxs_buffer_t *input,
    oxs_buffer_t *result,
    axis2_char_t *filename)
{
    openssl_pkey_t *pkey = NULL;
    axis2_char_t  *decoded_encrypted_str = NULL;
    unsigned char *decrypted  =  NULL;
    int ret, declen;
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    openssl_rsa_t *rsa = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);

    /*First do base64 decode*/
    decoded_encrypted_str = AXIS2_MALLOC(env->allocator, axis2_base64_decode_len((char*)OXS_BUFFER_GET_DATA(input, env)));
    ret = axis2_base64_decode(decoded_encrypted_str, (char*)OXS_BUFFER_GET_DATA(input, env));

    printf("oxs_enc_engine_prvkey_decrypt_data\n");
    /*Create a private key*/
    pkey =  openssl_pkey_create(env);
    /*Load the private _key*/
    status = OPENSSL_PKEY_LOAD(pkey, env, filename, "");/*TODO password*/
    if (status == AXIS2_FAILURE)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "cannot load the private key from the file %s", filename);
        return AXIS2_FAILURE;

    }

    /*Now we support only rsa*/
    rsa = openssl_rsa_create(env);
    declen = OPENSSL_RSA_PRV_DECRYPT(rsa, env, pkey, (unsigned char *)decoded_encrypted_str, &decrypted);
    if (declen < 0)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "decryption failed");
        return AXIS2_FAILURE;
    }

    status = OXS_BUFFER_POPULATE(result, env, (unsigned char*)decrypted, declen);

    return AXIS2_SUCCESS;
}


/*TODO better to have pk_ctx instead of individual parameters like filename, algorithm*/
axis2_status_t AXIS2_CALL
oxs_enc_engine_pubkey_encrypt_data(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    oxs_buffer_t *input,
    oxs_buffer_t *result,
    axis2_char_t *filename)
{
    openssl_pkey_t *pkey = NULL;
    axis2_char_t *encoded_str = NULL;
    unsigned char *encrypted  =  NULL;
    int ret, enclen, encodedlen ;
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;
    openssl_rsa_t *rsa = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);

    /*Create a key*/
    pkey =  openssl_pkey_create(env);

    /*Load the public key. Note: evp_pkey_load first try to load the private key. So need to provide the correct key*/
    status = OPENSSL_PKEY_LOAD(pkey, env, filename, "");/*TODO password*/
    if (status == AXIS2_FAILURE)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "cannot load the public key from the file %s", filename);
        return AXIS2_FAILURE;

    }
    /*Now we support only rsa*/
    rsa = openssl_rsa_create(env);
    ret = OPENSSL_RSA_PUB_ENCRYPT(rsa, env, pkey, OXS_BUFFER_GET_DATA(input, env), &encrypted);
    if (ret < 0)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "openssl_rsa_pub_encrypt");
        return AXIS2_FAILURE;

    }
    enclen = ret; /*return value is the length of the encrypted data*/
    /*base64 encode*/
    encodedlen = axis2_base64_encode_len(enclen);
    encoded_str = AXIS2_MALLOC(env->allocator, encodedlen);

    ret = axis2_base64_encode(encoded_str, (const char *)encrypted, enclen);
    if (ret < 0)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "axis2_base64_encode");
        return AXIS2_FAILURE;
    }

    /*Attach the result to the result buf*/
    status = OXS_BUFFER_POPULATE(result, env, (unsigned char*)AXIS2_STRDUP(encoded_str, env), encodedlen);

    return AXIS2_SUCCESS;
}


/*Encrypt or decrypt an input depending on what is set in the enc_ctx*/
/*TODO Default IV*/
axis2_status_t AXIS2_CALL
oxs_enc_engine_crypt(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    oxs_ctx_t * enc_ctx,
    oxs_buffer_t *input,
    oxs_buffer_t *result)
{
    unsigned char *out_main_buf = NULL;
    openssl_cipher_ctx_t *oc_ctx = NULL;
    axis2_char_t *iv = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *cipher_name =  NULL;
    axis2_char_t *encoded_str = NULL;
    axis2_char_t *in_data = NULL;
    int ret, enclen, encodedlen, decoded_len;
    openssl_cipher_property_t *cprop = NULL;
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);

    /*Initializations*/
    ret = AXIS2_FAILURE;
    enclen = -1;
    encodedlen = -1;


    /*Create the context*/
    oc_ctx = openssl_cipher_ctx_create(env);
    if (!oc_ctx)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "openssl_cipher_ctx_create failed");
        return AXIS2_FAILURE;
    }

    /*Get cipher property*/
    cprop =  oxs_get_cipher_property_for_url(env, OXS_CTX_GET_ENC_MTD_ALGORITHM(enc_ctx, env));


    /*Set the IV*/
    iv = AXIS2_STRNDUP((axis2_char_t*)oxs_iv_generate_for_algo(env,  OXS_CTX_GET_ENC_MTD_ALGORITHM(enc_ctx, env)),
            OPENSSL_CIPHER_PROPERTY_GET_IV_SIZE(cprop, env),
            env);
    ret = OPENSSL_CIPHER_CTX_SET_IV(oc_ctx, env, iv);

    /*Set the key*/
    key =  AXIS2_STRNDUP((axis2_char_t*)OXS_KEY_GET_DATA(OXS_CTX_GET_KEY(enc_ctx,
            env),
            env),
            OPENSSL_CIPHER_PROPERTY_GET_KEY_SIZE(cprop, env),
            env);

    ret = OPENSSL_CIPHER_CTX_SET_KEY(oc_ctx, env, key);

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
            (EVP_CIPHER*)openssl_get_evp_cipher_by_name(env, (axis2_char_t*)cipher_name));


    /****************Encryption or decryption happens here ************/

    /*If this is to encrypt we simply pass the data to crypto function*/
    if (OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_ENCRYPT)
    {
        enclen = openssl_block_cipher_crypt(env, oc_ctx,
                OXS_BUFFER_GET_DATA(input, env),
                OXS_BUFFER_GET_SIZE(input, env),
                &out_main_buf, OPENSSL_ENCRYPT);

        /*If this is to decrypt, then we need to base64decode first*/
    }
    else if (OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_DECRYPT)
    {
        in_data = AXIS2_MALLOC(env->allocator, axis2_base64_decode_len((char*)(OXS_CTX_GET_INPUT_DATA(enc_ctx, env))));
        /*axis2_base64_decode(in_data, (char*)(enc_ctx->inputdata)); */
        decoded_len = axis2_base64_decode_binary((unsigned char*)in_data, (char*)(OXS_CTX_GET_INPUT_DATA(enc_ctx, env)));
        if (decoded_len < 0)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "base64 decoding failed");
        }
        enclen = openssl_block_cipher_crypt(env, oc_ctx,
                (unsigned char*)in_data, decoded_len,  &out_main_buf, OPENSSL_DECRYPT);
    }
    else
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Invalid operation type %d", OXS_CTX_GET_OPERATION(enc_ctx, env));
        return AXIS2_FAILURE;

    }

    if (enclen < 0)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "openssl_block_cipher_crypt failed");
        return AXIS2_FAILURE;
    }

#if 0
    FILE *outf;
    outf = fopen("outbuf", "wb");
    fwrite(out_main_buf, 1, enclen, outf);
#endif


    /*If the operation is to encrypt we will encode the encrypted data*/
    if (OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_ENCRYPT)
    {
        encodedlen = axis2_base64_encode_len(enclen);
        encoded_str = AXIS2_MALLOC(env->allocator, encodedlen);

        ret = axis2_base64_encode(encoded_str, (const char *)out_main_buf, enclen);
        if (ret < 0)
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                    "axis2_base64_encode");
            return AXIS2_FAILURE;
        }

        /*Attach the result to the result buf*/
        ret = OXS_BUFFER_POPULATE(result, env, (unsigned char*)AXIS2_STRDUP(encoded_str, env), encodedlen);

    }
    else if (OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_DECRYPT)
    {
        ret = OXS_BUFFER_POPULATE(result, env, AXIS2_STRMEMDUP(out_main_buf, enclen, env), enclen);

    }
    else
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Invalid operation type %d", OXS_CTX_GET_OPERATION(enc_ctx, env));
        return AXIS2_FAILURE;

    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_enc_engine_populate_cipher_value(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t* template_node,
    oxs_buffer_t *databuf)
{
    axis2_status_t ret = AXIS2_FAILURE;
    axiom_element_t *template_ele = NULL, *cv_ele = NULL, *cd_ele = NULL;
    axiom_node_t *cv_node = NULL, *cd_node = NULL;
    axis2_qname_t *qname = NULL;
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);

    template_ele = AXIOM_NODE_GET_DATA_ELEMENT(template_node, env);

    if (!template_ele)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Cannot retriew  element from the template given");
        return AXIS2_FAILURE;
    }

    /*Get CipherData*/
    qname = axis2_qname_create(env, OXS_NodeCipherData, NULL, NULL);
    cd_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(template_ele, env, qname, template_node, &cd_node);

    if (!cd_ele)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Cannot retriew CipherData element from %s", AXIOM_ELEMENT_GET_LOCALNAME(template_ele, env));
        return AXIS2_FAILURE;
    }

    AXIS2_QNAME_FREE(qname, env);
    qname = NULL;

    /*Get CipherValue*/
    qname = axis2_qname_create(env, OXS_NodeCipherValue, NULL, NULL);
    cv_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(cd_ele, env, qname, cd_node, &cv_node);

    if (!cv_ele)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Cannot retriew CipherValue element from %s", AXIOM_ELEMENT_GET_LOCALNAME(cd_ele, env));
        return AXIS2_FAILURE;

    }


    ret =  AXIOM_ELEMENT_SET_TEXT(cv_ele, env, (axis2_char_t *)OXS_BUFFER_GET_DATA(databuf, env) , cv_node);
    if (ret != AXIS2_SUCCESS)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Cannot set data to the CipherValue element");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}
/*We expect user to provide a template as below*/
axis2_status_t AXIS2_CALL
oxs_enc_engine_decrypt_template(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t* template_node,
    axis2_char_t** decrypted_data,
    oxs_ctx_t * enc_ctx
)
{
    axis2_status_t  ret =  AXIS2_FAILURE;
    oxs_buffer_t *input = NULL;
    oxs_buffer_t *result = NULL;
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);

    /*Populate enc_ctx*/
    OXS_CTX_SET_OPERATION(enc_ctx, env, OXS_CTX_OPERATION_DECRYPT);
    OXS_CTX_SET_MODE(enc_ctx, env, OXS_CTX_MODE_ENCRYPTED_DATA);

    ret = oxs_enc_engine_encrypted_data_node_read(env, enc_ctx, template_node);
    if (ret != AXIS2_SUCCESS)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "reading encrypted data failed");
        return ret;
    }

    /*Now look for data to be decrypted*/
    input = oxs_buffer_create(env);
    ret  = OXS_BUFFER_POPULATE(input, env, (unsigned char *)OXS_CTX_GET_INPUT_DATA(enc_ctx, env),
            AXIS2_STRLEN(OXS_CTX_GET_INPUT_DATA(enc_ctx, env)));

    /*Initialize the result buffer*/
    result = oxs_buffer_create(env);

    ret = oxs_enc_engine_crypt(enc_engine, env, enc_ctx, input,  result);
    if (ret != AXIS2_SUCCESS)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                "oxs_enc_decrypt failed");
        return ret;
    }

    *decrypted_data =   AXIS2_MALLOC(env->allocator, OXS_BUFFER_GET_SIZE(result, env));
    *decrypted_data = (axis2_char_t*)OXS_BUFFER_GET_DATA(result, env);

    return ret;
}

/*We expect user to provide a template as below*/
/**/
axis2_status_t AXIS2_CALL
oxs_enc_engine_encrypt_template(
    oxs_enc_engine_t *enc_engine,
    const axis2_env_t *env,
    axiom_node_t* template_node,
    axis2_char_t* data,
    oxs_ctx_t * enc_ctx
)
{
    axis2_status_t  ret =  AXIS2_FAILURE;
    oxs_buffer_t *input = NULL;
    oxs_buffer_t *result = NULL;
    oxs_enc_engine_impl_t *enc_engine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    enc_engine_impl = AXIS2_INTF_TO_IMPL(enc_engine);


    /*Populate enc_ctx*/
    OXS_CTX_SET_OPERATION(enc_ctx, env, OXS_CTX_OPERATION_ENCRYPT);
    OXS_CTX_SET_MODE(enc_ctx, env, OXS_CTX_MODE_ENCRYPTED_DATA);

    ret = oxs_enc_engine_encrypted_data_node_read(env, enc_ctx, template_node);
    if (ret != AXIS2_SUCCESS)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "openssl_block_cipher_crypt failed");
        return ret;
    }

    /*We've populated the context*/

    /*Create the input buffer*/
    input = oxs_buffer_create(env);
    ret =  OXS_BUFFER_POPULATE(input, env, (unsigned char *)data, AXIS2_STRLEN(data));

    result = oxs_buffer_create(env);

    ret = oxs_enc_engine_crypt(enc_engine, env, enc_ctx, input, result);
    if (ret != AXIS2_SUCCESS)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "oxs_enc_encrypt failed");
        return ret;
    }

    ret = oxs_enc_engine_populate_cipher_value(enc_engine, env, template_node, result);

    if (ret != AXIS2_SUCCESS)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "oxs_enc_engine_populate_cipher_value failed");
        return ret;
    }

    return AXIS2_SUCCESS;

}

static axis2_status_t
oxs_enc_engine_cipher_data_node_read(const axis2_env_t *env,
        oxs_ctx_t * enc_ctx, axiom_node_t* node_cipher_data)
{
    axiom_node_t* cur = NULL;
    axiom_element_t *ele = NULL;
    axis2_char_t *data = NULL;
    /*We've a cipher data node here.
     The child element is either a CipherReference or a CipherValue element*/

    cur = AXIOM_NODE_GET_FIRST_CHILD(node_cipher_data, env);

    if (!cur)
    {

        return AXIS2_FAILURE;
    }

    if (oxs_axiom_check_node_name(env, cur, OXS_NodeCipherValue, OXS_EncNs))
    {
        /*Got a cipher value*/
        if (OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_DECRYPT)
        {
            OXS_CTX_SET_CIPHER_VALUE_NODE(enc_ctx, env, cur);
            ele = AXIOM_NODE_GET_DATA_ELEMENT(cur, env);
            /*Read data from the CipherValue node and set it to the context*/
            data = AXIOM_ELEMENT_GET_TEXT(ele, env, cur);
            OXS_CTX_SET_INPUT_DATA(enc_ctx, env, AXIS2_STRDUP(data, env));
        }
    }
    else if (oxs_axiom_check_node_name(env, cur, OXS_NodeCipherReference, OXS_EncNs))
    {
        /*Got a cipher reference*/
        /*TODO : Support Cipher references*/
        /*Check cipher reference and set it to ctx->cipher_value_node*/
    }

    return AXIS2_SUCCESS;
}
/**
* We have EncryptedData of EncryptedKey node by now.
* So process it and populate the ctx.
*/
static axis2_status_t
oxs_enc_engine_encrypted_data_node_read(const axis2_env_t *env,
        oxs_ctx_t * enc_ctx, axiom_node_t* node)
{
    axiom_node_t* cur = NULL;
    axiom_element_t *ele = NULL;
    int ret;

    /* Operation is either encrypt or decrypt */
    if (!((OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_ENCRYPT) || (OXS_CTX_GET_OPERATION(enc_ctx, env) == OXS_CTX_OPERATION_DECRYPT)))
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "Operation is NOT either encrypt or decrypt");
        return AXIS2_FAILURE;
    }

    /*Check which mode we are in.
      1. Encrypted Data
      2. Encrypted Key
    */

    switch (OXS_CTX_GET_MODE(enc_ctx, env))
    {
        case OXS_CTX_MODE_ENCRYPTED_DATA:
            if (!(oxs_axiom_check_node_name(env, node, OXS_NodeEncryptedData, OXS_EncNs)))
            {
                oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                        "No %s node found", OXS_NodeEncryptedData);
                return AXIS2_FAILURE;
            }
            break;
        case OXS_CTX_MODE_ENCRYPTED_KEY:
            if (!(oxs_axiom_check_node_name(env, node, OXS_NodeEncryptedKey, OXS_EncNs)))
            {
                oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                        "No %s node found", OXS_NodeEncryptedKey);
                return AXIS2_FAILURE;
            }
            break;
    }

    OXS_CTX_SET_ENC_DATA_NODE(enc_ctx, env,  node);

    /*Element is either Encrypted Data or Encrypted Key*/
    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    if (!ele)
    {
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                "No element found");
        return AXIS2_FAILURE;
    }

    /*Populate ctx with id, type, mime and encoding types if found*/
    OXS_CTX_SET_ID(enc_ctx, env, AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrId));
    OXS_CTX_SET_TYPE(enc_ctx, env, AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrType));
    OXS_CTX_SET_MIME_TYPE(enc_ctx, env, AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrMimeType));
    OXS_CTX_SET_ENCODING(enc_ctx, env, AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrEncoding));

    if (OXS_CTX_GET_MODE(enc_ctx, env) == OXS_CTX_MODE_ENCRYPTED_KEY)
    {
        OXS_CTX_SET_RECIPIENT(enc_ctx, env, AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrRecipient));
    }

    cur = AXIOM_NODE_GET_FIRST_CHILD(node, env);

    /* first node is optional EncryptionMethod, we'll read it later */
    if ((cur) && (oxs_axiom_check_node_name(env, cur, OXS_NodeEncryptionMethod, OXS_EncNs)))
    {
        OXS_CTX_SET_ENC_METHOD_NODE(enc_ctx, env, cur);
        cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);
    }

    /* next node is optional KeyInfo, we'll process it later */
    if ((cur) && (oxs_axiom_check_node_name(env, cur, OXS_NodeKeyInfo, OXS_DSigNs)))
    {
        OXS_CTX_SET_KEY_INFO_NODE(enc_ctx, env, cur);
        cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);
    }

    /* next is required CipherData node */
    if ((cur == NULL) || (!oxs_axiom_check_node_name(env, cur, OXS_NodeCipherData, OXS_EncNs)))
    {
        return AXIS2_FAILURE;
    }

    ret = oxs_enc_engine_cipher_data_node_read(env, enc_ctx, cur);
    if (ret < 0)
    {
        return AXIS2_FAILURE;
    }


    /* now read the encryption method node */
    if (OXS_CTX_GET_ENC_METHOD_NODE(enc_ctx, env))
    {
        ele = AXIOM_NODE_GET_DATA_ELEMENT(OXS_CTX_GET_ENC_METHOD_NODE(enc_ctx, env), env);
        OXS_CTX_SET_ENC_MTD_ALGORITHM(enc_ctx, env, AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrAlgorithm));
    }

    /*If you need to populate the context more this is the place*/

    return AXIS2_SUCCESS;

}





