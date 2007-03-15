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

#ifndef OXS_CTX_H
#define OXS_CTX_H


/**
  * @file oxs_ctx.h
  * @brief Keeps configurations for the OMXMLSecurity 
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axiom_node.h>
#include <oxs_buffer.h>
#include <oxs_key.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup oxs_ctx Message Context 
 * @ingroup oxs_context
 * @{
 */

/*TODO We need to change the name oxs_ctx -> oxs_sym_ctx.
 * The name is sounds too generic as there is another oxs_asym_ctx.
 * */

    typedef enum  {
        OXS_CTX_OPERATION_NONE = 0,
        OXS_CTX_OPERATION_ENCRYPT,
        OXS_CTX_OPERATION_DECRYPT
    } oxs_ctx_operation_t;

    typedef enum {
        OXS_CTX_MODE_ENCRYPTED_DATA = 0,
        OXS_CTX_MODE_ENCRYPTED_KEY
    } oxs_ctx_mode_t;

    /** Type name for struct oxs_ctx_ops */
    typedef struct oxs_ctx_ops oxs_ctx_ops_t;

    /** Type name for struct oxs_ctx */
    typedef struct oxs_ctx oxs_ctx_t;


    struct oxs_ctx_ops
    {
        /**
         * Free function of the context
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        free)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * Returns the mode of the context
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return mode of the context 
         */
        oxs_ctx_mode_t (AXIS2_CALL *
        get_mode)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return operation of the context 
         */
        oxs_ctx_operation_t (AXIS2_CALL *
        get_operation)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        oxs_key_t *(AXIS2_CALL *
        get_key)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_id)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );
        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_mime_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_encoding)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_recipient)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_carried_key_name)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_enc_mtd_algorithm)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axis2_char_t *(AXIS2_CALL *
        get_input_data)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axiom_node_t *(AXIS2_CALL *
        get_enc_data_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axiom_node_t *(AXIS2_CALL *
        get_enc_method_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axiom_node_t *(AXIS2_CALL *
        get_key_info_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @return of the context 
         */
        axiom_node_t *(AXIS2_CALL *
        get_cipher_value_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param mode the mode of operation, EncryptedData/EncryptedKey
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_mode)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_ctx_mode_t mode
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param operation the operation Encrypt/Decrypt/Sign/Verify
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_operation)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_ctx_operation_t operation
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param key the key used
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_key)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_key_t *key
                    );
        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param id the id of the context 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_id)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *id
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param type ???Depricated?
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *type
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param mime_type the mime type used
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_mime_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *mime_type
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param encoding the encoding used
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_encoding)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *encoding
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param recipient name of recipient
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_recipient)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *recipient
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param carried_key_name the key name 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_carried_key_name)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *carried_key_name
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param enc_mtd_algorithm the encryption method algorithm
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_enc_mtd_algorithm)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *enc_mtd_algorithm
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param input_data the input data
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_input_data)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *input_data
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param enc_data_node  reference to the EncryptionData node 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_enc_data_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axiom_node_t *enc_data_node
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param enc_method_node  reference to the EncryptionMethod node 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_enc_method_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axiom_node_t *enc_method_node
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param key_info_node  reference to the KeyInfo node  
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_key_info_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axiom_node_t *key_info_node
                    );

        /**
         * 
         * @param ctx The OMXMLSecurity context 
         * @param env pointer to environment struct
         * @param cipher_value_node reference to the cipherValue node 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
        set_cipher_value_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axiom_node_t *cipher_value_node
                    );

    };
    
    struct oxs_ctx
    {
        /** operations of oxs_ctx */
        oxs_ctx_ops_t *ops;
    };

    /*Create function*/
    AXIS2_EXTERN oxs_ctx_t *AXIS2_CALL
    oxs_ctx_create(const axis2_env_t *env);


/**********************Macros******************************************/
#define OXS_CTX_FREE(ctx, env) \
        ((ctx)->ops->free(ctx, env) )
    
#define OXS_CTX_GET_MODE(ctx, env) \
        ((ctx)->ops->get_mode(ctx, env) )
    
#define OXS_CTX_GET_OPERATION(ctx, env) \
        ((ctx)->ops->get_operation(ctx, env) )
    
#define OXS_CTX_GET_KEY(ctx, env) \
        ((ctx)->ops->get_key(ctx, env) )
    
#define OXS_CTX_GET_ID(ctx, env) \
        ((ctx)->ops->get_id(ctx, env) )
    
#define OXS_CTX_GET_TYPE(ctx, env) \
        ((ctx)->ops->get_type(ctx, env) )
    
#define OXS_CTX_GET_MIME_TYPE(ctx, env) \
        ((ctx)->ops->get_mime_type(ctx, env) )
    
#define OXS_CTX_GET_ENCODING(ctx, env) \
        ((ctx)->ops->get_encoding(ctx, env) )
    
#define OXS_CTX_GET_RECIPIENT(ctx, env) \
        ((ctx)->ops->get_recipient(ctx, env) )
    
#define OXS_CTX_GET_CARRIED_KEY_NAME(ctx, env) \
        ((ctx)->ops->get_carried_key_name(ctx, env) )
    
#define OXS_CTX_GET_ENC_MTD_ALGORITHM(ctx, env) \
        ((ctx)->ops->get_enc_mtd_algorithm(ctx, env) )
    
#define OXS_CTX_GET_INPUT_DATA(ctx, env) \
        ((ctx)->ops->get_input_data(ctx, env) )
    
#define OXS_CTX_GET_ENC_DATA_NODE(ctx, env) \
        ((ctx)->ops->get_enc_data_node(ctx, env) )
    
#define OXS_CTX_GET_ENC_METHOD_NODE(ctx, env) \
        ((ctx)->ops->get_enc_method_node(ctx, env) )
    
#define OXS_CTX_GET_KEY_INFO_NODE(ctx, env) \
        ((ctx)->ops->get_key_info_node(ctx, env) )
    
#define OXS_CTX_GET_CIPHER_VALUE_NODE(ctx, env) \
        ((ctx)->ops->get_cipher_value_node(ctx, env) )
    
#define OXS_CTX_SET_MODE(ctx, env, mode) \
        ((ctx)->ops->set_mode(ctx, env, mode) )
    
#define OXS_CTX_SET_OPERATION(ctx, env, operation) \
        ((ctx)->ops->set_operation(ctx, env, operation) )
    
#define OXS_CTX_SET_KEY(ctx, env, key) \
        ((ctx)->ops->set_key(ctx, env, key) )
    
#define OXS_CTX_SET_ID(ctx, env, id) \
        ((ctx)->ops->set_id(ctx, env, id) )
    
#define OXS_CTX_SET_TYPE(ctx, env, type) \
        ((ctx)->ops->set_type(ctx, env, type) )
    
#define OXS_CTX_SET_MIME_TYPE(ctx, env, mime_type) \
        ((ctx)->ops->set_mime_type(ctx, env, mime_type) )
    
#define OXS_CTX_SET_ENCODING(ctx, env, encoding) \
        ((ctx)->ops->set_encoding(ctx, env, encoding) )
    
#define OXS_CTX_SET_RECIPIENT(ctx, env, recipient) \
        ((ctx)->ops->set_recipient(ctx, env, recipient) )
    
#define OXS_CTX_SET_CARRIED_KEY_NAME(ctx, env, key_name) \
        ((ctx)->ops->set_carried_key_name(ctx, env, key_name) )
    
#define OXS_CTX_SET_ENC_MTD_ALGORITHM(ctx, env, enc_mtd_algorithm) \
        ((ctx)->ops->set_enc_mtd_algorithm(ctx, env, enc_mtd_algorithm) )
    
#define OXS_CTX_SET_INPUT_DATA(ctx, env, input_data) \
        ((ctx)->ops->set_input_data(ctx, env, input_data) )
    
#define OXS_CTX_SET_ENC_DATA_NODE(ctx, env, enc_data_node) \
        ((ctx)->ops->set_enc_data_node(ctx, env, enc_data_node) )
    
#define OXS_CTX_SET_ENC_METHOD_NODE(ctx, env, enc_method_node) \
        ((ctx)->ops->set_enc_method_node(ctx, env, enc_method_node) )
    
#define OXS_CTX_SET_KEY_INFO_NODE(ctx, env, key_info_node) \
        ((ctx)->ops->set_key_info_node(ctx, env, key_info_node) )
    
#define OXS_CTX_SET_CIPHER_VALUE_NODE(ctx, env, cipher_value_node) \
        ((ctx)->ops->set_cipher_value_node(ctx, env, cipher_value_node) )
    



/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_CTX_H */
