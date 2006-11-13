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
    
    /*Remove the node from the parent*/
    if(AXIS2_SUCCESS == ret){
        AXIOM_NODE_DETACH(node, env);
    }
    /*Free*/
    OXS_BUFFER_FREE(serialized_buf, env); 

    /*Return success*/
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
    OXS_CTX_SET_OPERATION(enc_ctx, env, OXS_CTX_OPERATION_ENCRYPT);

    /*Create an empty buffer for encrypted data*/
    result_buf = oxs_buffer_create(env);
    /*Call encryption. Result should be base64 encoded*/
    ret = oxs_encryption_symmetric_crypt(env, enc_ctx, content_buf, result_buf);

    /*Create EncryptionMethod, CipherData element and populate*/
    enc_mtd_node = oxs_token_build_encryption_method_element(env, *enc_type_node, sym_algo);     
    cd_node = oxs_token_build_cipher_data_element(env, *enc_type_node);
    cv_node = oxs_token_build_cipher_value_element(env, cd_node, (axis2_char_t*)OXS_BUFFER_GET_DATA(result_buf, env));

    /*Free buffers*/
    OXS_BUFFER_FREE(result_buf, env); 

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_xml_enc_decrypt_node(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *enc_type_node,
                            axiom_node_t **decrypted_node)
{
    axiom_node_t *deserialized_node = NULL;
    axiom_node_t *parent_of_enc_node = NULL;
    oxs_buffer_t *result_buf = NULL;
    axis2_char_t *decrypted_data = NULL;/*Can be either am XML-Element or XML-Content*/

    /*Create an empty buffer for results*/
    result_buf = oxs_buffer_create(env);

    /*Decrypt*/
    oxs_xml_enc_decrypt_data(env, enc_ctx, enc_type_node, result_buf);
    decrypted_data = (axis2_char_t *)OXS_BUFFER_GET_DATA(result_buf, env);
    printf("\nDecrypted_Data = %s\n", decrypted_data); 
    /*De-serialize the decrypted content to build the node*/
    deserialized_node = (axiom_node_t*)oxs_axiom_deserialize_node(env, decrypted_data);
    if(!deserialized_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,"Cannot deserialize a node from the content.");
        return AXIS2_FAILURE;
    }
    /*Assign deserialized_node to the reference passed*/
    *decrypted_node = deserialized_node;

    /*Replace the encrypted node with the de-serialized node*/
    parent_of_enc_node = AXIOM_NODE_GET_PARENT(enc_type_node, env);
    AXIOM_NODE_ADD_CHILD(parent_of_enc_node, env, deserialized_node);
    AXIOM_NODE_DETACH(enc_type_node, env);    

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
oxs_xml_enc_decrypt_data(const axis2_env_t *env,
                            oxs_ctx_t * enc_ctx,
                            axiom_node_t *enc_type_node,
                            oxs_buffer_t *result_buf)
{
    axiom_node_t *enc_mtd_node = NULL;
    axiom_node_t *cd_node = NULL;
    axiom_node_t *cv_node = NULL;
    axis2_char_t *cipher_val = NULL;
    axis2_char_t *sym_algo = NULL;
    axis2_char_t *type = NULL;
    axis2_char_t *id = NULL;
    oxs_buffer_t *input_buf = NULL;

    /*Get the symmetric encryption algorithm*/
    enc_mtd_node = oxs_axiom_get_first_child_node_by_name(env, enc_type_node, OXS_NodeEncryptionMethod, NULL, NULL);
    sym_algo = oxs_token_get_encryption_method(env, enc_mtd_node);

    /*Get ID, Type, MimeType attributes from the EncryptedDataNode*/
    id = oxs_axiom_get_attribute_value_of_node_by_name(env, enc_type_node, OXS_AttrId);
    type = oxs_axiom_get_attribute_value_of_node_by_name(env, enc_type_node, OXS_AttrType);

    /*Populate the context for future use*/
    OXS_CTX_SET_ENC_MTD_ALGORITHM(enc_ctx, env, sym_algo);
    OXS_CTX_SET_ID(enc_ctx, env, id);
    OXS_CTX_SET_TYPE(enc_ctx, env, type);
    
    /*Get the cipher value*/
    cd_node = oxs_axiom_get_first_child_node_by_name(env, enc_type_node, OXS_NodeCipherData, NULL, NULL);
    cv_node = oxs_axiom_get_first_child_node_by_name(env, cd_node, OXS_NodeCipherValue, NULL, NULL);
    cipher_val = oxs_token_get_cipher_value(env, cv_node); 
    
    /*Create input buffer with cipher data obtained*/
    input_buf = oxs_buffer_create(env);
    OXS_BUFFER_POPULATE(input_buf, env, (unsigned char*)cipher_val, AXIS2_STRLEN(cipher_val) );

    /*Decrypt*/
    OXS_CTX_SET_OPERATION(enc_ctx, env, OXS_CTX_OPERATION_DECRYPT);
    return oxs_encryption_symmetric_crypt(env, enc_ctx, input_buf, result_buf);
}

axis2_status_t AXIS2_CALL
oxs_xml_enc_encrypt_key(const axis2_env_t *env,
                            oxs_asym_ctx_t * asym_ctx,
                            axiom_node_t *parent,
                            oxs_key_t *sym_key)
{
    axis2_char_t *algorithm = NULL;
    axis2_char_t *encrypted_key_data = NULL;
    oxs_buffer_t *input = NULL;
    oxs_buffer_t *result = NULL;
    axiom_node_t *encrypted_key_node = NULL;
    axiom_node_t *enc_mtd_node = NULL;
    /*axiom_node_t *key_info_node = NULL;*/
    axiom_node_t *cd_node = NULL;
    axiom_node_t *cv_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    /*Create input buffer*/
    input = oxs_buffer_create(env);
    OXS_BUFFER_POPULATE(input, env, OXS_KEY_GET_DATA(sym_key, env), OXS_KEY_GET_SIZE(sym_key, env));

    /*Create an empty buffer to collect results*/
    result = oxs_buffer_create(env);
    
    /*Call encryption*/
    status = oxs_encryption_asymmetric_crypt(env, asym_ctx, input, result);

    /*Get the encrypted key*/
    encrypted_key_data = (axis2_char_t *)OXS_BUFFER_GET_DATA(result, env);

    /*Build nodes*/
    encrypted_key_node = oxs_token_build_encrypted_key_element(env, parent);
    algorithm = oxs_asym_ctx_get_algorithm(asym_ctx, env);
    enc_mtd_node = oxs_token_build_encryption_method_element(env, encrypted_key_node, algorithm);
    /*key_info_node = oxs_token_build_key_info_element(env, encrypted_key_node);*/
    /*TODO SecurityTokenReference*/
    cd_node = oxs_token_build_cipher_data_element(env, encrypted_key_node);
    cv_node = oxs_token_build_cipher_value_element(env, cd_node,  encrypted_key_data);

    return AXIS2_SUCCESS; 
}


