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
  * @brief 
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


    typedef enum  {
        OXS_CTX_OPERATION_NONE = 0,
        OXS_CTX_OPERATION_SIGN,
        OXS_CTX_OPERATION_VERIFY,
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

        axis2_status_t (AXIS2_CALL *
        free)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        oxs_ctx_mode_t (AXIS2_CALL *
        get_mode)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        oxs_ctx_operation_t (AXIS2_CALL *
        get_operation)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        oxs_key_t *(AXIS2_CALL *
        get_key)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_id)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );
        axis2_char_t *(AXIS2_CALL *
        get_mime_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_encoding)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_recipient)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_carried_key_name)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_enc_mtd_algorithm)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_char_t *(AXIS2_CALL *
        get_input_data)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axiom_node_t *(AXIS2_CALL *
        get_enc_data_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axiom_node_t *(AXIS2_CALL *
        get_enc_method_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axiom_node_t *(AXIS2_CALL *
        get_key_info_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axiom_node_t *(AXIS2_CALL *
        get_cipher_value_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env
                    );

        axis2_status_t (AXIS2_CALL *
        set_mode)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_ctx_mode_t mode
                    );

        axis2_status_t (AXIS2_CALL *
        set_operation)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_ctx_operation_t operation
                    );

        axis2_status_t (AXIS2_CALL *
        set_key)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    oxs_key_t *key
                    );
        axis2_status_t (AXIS2_CALL *
        set_id)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *id
                    );

        axis2_status_t (AXIS2_CALL *
        set_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *type
                    );

        axis2_status_t (AXIS2_CALL *
        set_mime_type)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *mime_type
                    );

        axis2_status_t (AXIS2_CALL *
        set_encoding)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *encoding
                    );

        axis2_status_t (AXIS2_CALL *
        set_recipient)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *recipient
                    );

        axis2_status_t (AXIS2_CALL *
        set_carried_key_name)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *carried_key_name
                    );

        axis2_status_t (AXIS2_CALL *
        set_enc_mtd_algorithm)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *enc_mtd_algorithm
                    );

        axis2_status_t (AXIS2_CALL *
        set_input_data)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axis2_char_t *input_data
                    );

        axis2_status_t (AXIS2_CALL *
        set_enc_data_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axiom_node_t *enc_data_node
                    );

        axis2_status_t (AXIS2_CALL *
        set_enc_method_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axiom_node_t *enc_method_node
                    );

        axis2_status_t (AXIS2_CALL *
        set_key_info_node)(
                    oxs_ctx_t *ctx,
                    const axis2_env_t *env,
                    axiom_node_t *key_info_node
                    );

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
    

/**
  <complexType name='EncryptedType' abstract='true'>
    <sequence>
      <element name='EncryptionMethod' type='xenc:EncryptionMethodType' 
               minOccurs='0'/>
      <element ref='ds:KeyInfo' minOccurs='0'/>
      <element ref='xenc:CipherData'/>
      <element ref='xenc:EncryptionProperties' minOccurs='0'/>
    </sequence>
    <attribute name='Id' type='ID' use='optional'/>
    <attribute name='Type' type='anyURI' use='optional'/>
    <attribute name='MimeType' type='string' use='optional'/>
    <attribute name='Encoding' type='anyURI' use='optional'/> 
   </complexType>

  <complexType name='EncryptionMethodType' mixed='true'>
    <sequence>
      <element name='KeySize' minOccurs='0' type='xenc:KeySizeType'/>
      <element name='OAEPparams' minOccurs='0' type='base64Binary'/>
      <any namespace='##other' minOccurs='0' maxOccurs='unbounded'/>
    </sequence>
    <attribute name='Algorithm' type='anyURI' use='required'/>
  </complexType>


*/



/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* OXS_CTX_H */
