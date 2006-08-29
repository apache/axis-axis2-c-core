/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef OXS_ENC_ENGINE_H
#define OXS_ENC_ENGINE_H


/**
  * @file oxs_enc.h
  * @brief 
  */

#include <axis2_defines.h>
#include <oxs_ctx.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct oxs_enc_engine_ops */
    typedef struct oxs_enc_engine_ops oxs_enc_engine_ops_t;
    /** Type name for struct oxs_enc_engine */
    typedef struct oxs_enc_engine oxs_enc_engine_t;
    /**
     * Test ops struct
     * Encapsulator struct for ops of oxs_enc_engine
     */
    struct oxs_enc_engine_ops
    {
        /**
         * Deallocate memory
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL * 
        free)(
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env);
        /**
         * 
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        prvkey_decrypt_data) (
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env,
                oxs_buffer_ptr input,
                oxs_buffer_ptr result,
                axis2_char_t *filename
                );

        /**
         * 
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        pubkey_encrypt_data)(
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env,
                oxs_buffer_ptr input,
                oxs_buffer_ptr result,
                axis2_char_t *filename
                );

        /**
         * Extracts data from the encryption key node and decrypt to get the  session key. 
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @param enc_key_node EncryptionKey node
         * @param prv_key Private key to be used to decrypt the sesison key
         * @param session_key Decrypted session key
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        get_encrypted_key)(
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env,
                axiom_node_t *enc_key_node,
                oxs_key_t *prv_key,
                oxs_key_t *session_key);

        /**
         * Common method to either en/decrypt data in the input buffer.
         * The operation is determined by the parameter set in the context.
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @param enc_ctx the encryption context
         * @param input The input buffer containing data to be en/decrypted
         * @param result The resulting buffer to carry data after en/decrypting
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        crypt)(
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env,
                oxs_ctx_t * enc_ctx,
                oxs_buffer_ptr input,
                oxs_buffer_ptr result);

        /**
         * Populate the cipher value node. 
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @param template_node the root node of the template
         * @param databuf data buffer containing the cipher value
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        populate_cipher_value)(
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env,
                axiom_node_t* template_node,
                oxs_buffer_ptr databuf);
    
        /**
         * Decrypt an xml document template. Expects a template similar to the above. 
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @param template_node the root node of the template to be decrypted
         * @param data The address reference of the decrypted data
         * @param enc_ctx the encryption context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        decrypt_template)(
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env,
                axiom_node_t* template_node,
                axis2_char_t** decrypted_data,
                oxs_ctx_t * enc_ctx);
    
        /**
         * Encrypt an xml document template. Expects a template similar to the above. 
         * @param enc_engine pointer to enc_engine
         * @param env pointer to environment struct
         * @param template_node the root node of the template to be encrypted
         * @param data The address reference of the encrypted data
         * @param enc_ctx the encryption context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        encrypt_template)(
                oxs_enc_engine_t *enc_engine,
                const axis2_env_t *env,
                axiom_node_t* template_node,
                axis2_char_t* data,
                oxs_ctx_t * enc_ctx);

    };

    /**
     * Engryption Engine struct
     */
    struct oxs_enc_engine
    {
        /** Operations of enc_engine */
        oxs_enc_engine_ops_t *ops;
    };

    /**
     * Creates enc_engine struct
     * @param env pointer to environment struct
     * @return pointer to newly created enc_engine
     */
    AXIS2_EXTERN oxs_enc_engine_t *AXIS2_CALL
    oxs_enc_engine_create (
        const axis2_env_t *env);

/*************************** Function macros **********************************/
#define OXS_ENC_ENGINE_FREE(enc_engine, env) \
        ((enc_engine)->ops->free(enc_engine, env))

#define OXS_ENC_ENGINE_PRV_KEY_DECRYPT_DATA(enc_engine, env, input, result, filename) \
        ((enc_engine)->ops->prvkey_decrypt_data(enc_engine, env, input, result, filename))

#define OXS_ENC_ENGINE_PUB_KEY_ENCRYPT_DATA(enc_engine, env, input, result, filename) \
        ((enc_engine)->ops->pubkey_encrypt_data(enc_engine, env, input, result, filename))

#define OXS_ENC_ENGINE_GET_ENCRYPTED_KEY(enc_engine, env, enc_key_node, prv_key, session_key) \
        ((enc_engine)->ops->get_encrypted_key(enc_engine, env, enc_key_node, prv_key, session_key))

#define OXS_ENC_ENGINE_CRYPT(enc_engine, env, enc_ctx, input, result) \
        ((enc_engine)->ops->crypt(enc_engine, env, enc_ctx, input, result))

#define OXS_ENC_ENGINE_POPULATE_CIPHER_VALUE(enc_engine, env, template_node, databuf) \
        ((enc_engine)->ops->populate_cipher_value(enc_engine, env, template_node, databuf))

#define OXS_ENC_ENGINE_DECRYPT_TEMPLATE(enc_engine, env, template_node, decrypted_data, enc_ctx) \
        ((enc_engine)->ops->decrypt_template(enc_engine, env, template_node, decrypted_data, enc_ctx))

#define OXS_ENC_ENGINE_ENCRYPT_TEMPLATE(enc_engine, env, template_node, data, enc_ctx) \
        ((enc_engine)->ops->encrypt_template(enc_engine, env, template_node, data, enc_ctx))


/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_ENC_ENGINE_H */
