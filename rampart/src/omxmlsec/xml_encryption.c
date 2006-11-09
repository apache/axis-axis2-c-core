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
#include <axis2_qname.h>
#include <axiom_namespace.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axis2_util.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_token_cipher_value.h>
#include <oxs_token_cipher_data.h>
#include <oxs_constants.h>
#include <oxs_axiom.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <oxs_iv.h>
#include <oxs_encryption.h>
#include <oxs_xml_encryption.h>

/*private functions*/


/*public functions*/
axis2_status_t AXIS2_CALL
oxs_xml_enc_encrypt_node(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *node,
                            axiom_node_t **enc_type_node)
{
    axis2_char_t *serialized_data = NULL;
    oxs_buffer_t *serialized_buf= NULL;
    axis2_status_t ret = AXIS2_FAILURE;
    
    /*Serialize node*/
    serialized_data = AXIOM_NODE_TO_STRING(node, env);
    serialized_buf = oxs_buffer_create(env);
    ret =  OXS_BUFFER_POPULATE(serialized_buf, env, (unsigned char *)serialized_data, AXIS2_STRLEN(serialized_data));
   
    /*We call encrypt_data*/
    ret = oxs_xml_enc_encrypt_data(env, enc_ctx, serialized_buf, enc_type_node); 
    
    /*Free*/
    OXS_BUFFER_FREE(serialized_buf, env); 

    /*Return success*/
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_xml_enc_decrypt_node(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *enc_type_node,
                            axiom_node_t **decrypted_node)
{
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_xml_enc_encrypt_data(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            oxs_buffer_t *content_buf,
                            axiom_node_t **enc_type_node)
{
    oxs_buffer_t *result_buf= NULL;
    oxs_key_t *sym_key = NULL;
    axis2_char_t *sym_algo = NULL;
    axiom_node_t *enc_mtd_node = NULL;
    axiom_node_t *cd_node = NULL;
    axiom_node_t *cv_node = NULL;
    axis2_status_t ret = AXIS2_FAILURE;

    /*Determine the algorithm to be used*/
    sym_algo = OXS_CTX_GET_ENC_MTD_ALGORITHM(enc_ctx, env);
    
    /*Determine the key to be used*/
    sym_key = OXS_CTX_GET_KEY(enc_ctx, env);

    /*Set the operation to encrypt*/
    OXS_CTX_SET_OPERATION(enc_ctx, env, OXS_CTX_OPERATION_DECRYPT);

    /*Create an empty buffer for encrypted data*/
    result_buf = oxs_buffer_create(env);
    /*Call encryption. Result should be base64 encoded*/
    ret = oxs_encryption_symmetric_crypt(env, enc_ctx, content_buf, result_buf);

    /*Create EncryptionMethod, CipherData element and populate*/
    enc_mtd_node = oxs_token_build_encryption_method_element(env, *enc_type_node, sym_algo);     
    cd_node = oxs_token_build_cipher_data_element(env, *enc_type_node);
    cv_node = oxs_token_build_cipher_value_element(env, cd_node, (axis2_char_t*)OXS_BUFFER_GET_DATA(result_buf, env));

    /*Replace the node with enc_type_node*/
    /*Free buffers*/
    OXS_BUFFER_FREE(result_buf, env); 

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_xml_enc_decrypt_data(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *enc_type_node,
                            oxs_buffer_t *result_buf)
{
    return AXIS2_SUCCESS;
}

