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

/* Rampart_crypto_util will contain crypto functionalities of the rampart model
 * 
 */
#include <stdio.h>
#include <axis2_utils.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_enc_engine.h>
#include <rampart_crypto_engine.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_token_cipher_data.h>
#include <oxs_token_cipher_value.h>
#include <oxs_token_key_info.h>
#include <oxs_token_key_name.h>
#include <oxs_key.h>
#include <rampart_action.h>
#include <rampart_constants.h>
#include <oxs_token_reference_list.h>
#include <axis2_array_list.h>


AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_crypto_decrypt_message(const axis2_env_t *env,
                      axis2_msg_ctx_t *msg_ctx,
                      axis2_param_t *param_action,
                      axiom_soap_envelope_t *soap_envelope ,
                      axiom_node_t *sec_node)
{
    axis2_status_t ret = AXIS2_FAILURE;
    axiom_node_t *enc_data_node = NULL;
    axiom_node_t *body_node = NULL, *header_node = NULL;
    axiom_node_t *ref_list_node = NULL;
    axiom_soap_body_t *body = NULL;
    axiom_soap_header_t *header = NULL;
    axis2_char_t *decrypted_data = NULL;
    enc_ctx_ptr enc_ctx = NULL;
    axiom_node_t *enc_key_node = NULL; 
    oxs_key_ptr session_key = NULL;
    axis2_array_list_t *uuid_list = NULL;
    axis2_array_list_t *enc_data_node_list = NULL;

    ret = AXIS2_SUCCESS;/*TODO Remove*/
    /*TODO get the key using callbacks*/

    body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);

    body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
    header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(header, env);
    
    /*TODO Get the Encrypted key*/
    enc_key_node =  oxs_axiom_get_first_child_node_by_name(env, sec_node, OXS_NodeEncryptedKey, NULL, NULL);

    /*We support only one Encrypted Key element at the moment*/   
    session_key = oxs_key_create_key(env, "keys/rsakey.pem", NULL, 0, OXS_KEY_USAGE_DECRYPT);
    ret = oxs_get_encrypted_key(env, enc_key_node, session_key); 
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                     "Cannot get the encrypted key");
        return ret;

    }
    /*Ohh yeah... now we got the seesion key, which is used encrypt data referred by the reference list*/    
    ref_list_node = oxs_axiom_get_first_child_node_by_name(env, enc_key_node, OXS_NodeReferenceList, NULL, NULL);
    if(!ref_list_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                     "Cannot get the ReferenceList node");
        return ret;

    }
    uuid_list = oxs_token_get_reference_list_data(env, ref_list_node);    
    
    /*TODO Get the encrypted node(s). Right now we support only one. To support more than one EncryptedData element use the uuid_list*/
    
    enc_data_node = oxs_axiom_get_first_child_node_by_name(env, body_node, OXS_NodeEncryptedData, NULL, NULL);
    

    /*TODO We assume that the very first element of bpody is encrypted data.
    This might be different if a sub element is encrypted*/
    /*Build the encryption ctx*/
    enc_ctx = oxs_ctx_create_ctx(env);

    /*Set the key*/
    enc_ctx->key = session_key ; /*oxs_key_create_key(env, "noname", "012345670123456701234567", 24, OXS_KEY_USAGE_DECRYPT);*/

    ret = oxs_enc_decrypt_template(env, enc_data_node, &decrypted_data, enc_ctx);
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "oxs_enc_decrypt_template failed");
        return ret;
    }else{
        printf("\nDecrypted data is \n %s\n\n", decrypted_data);
    }

    /*Create a stream reader and then build the node using decrypted text*/

    return ret;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_crypto_encrypt_message(const axis2_env_t *env,
                      axis2_msg_ctx_t *msg_ctx,
                      axis2_param_t *param_action,
                      axiom_soap_envelope_t *soap_envelope ,
                      axiom_node_t *sec_node)
{
    /*axis2_char_t *key = NULL, *key_name = NULL;*/
    axis2_status_t ret = AXIS2_FAILURE;
    axiom_node_t *node_to_enc = NULL, *body_node = NULL, *header_node = NULL;
    /*EncryptedData*/
    axiom_node_t *enc_data_node = NULL, *enc_mtd_node = NULL, *key_info_node = NULL, *key_name_node = NULL;
    axiom_node_t *cv_node = NULL, *cd_node = NULL, *enc_key_ref_list_node = NULL, *enc_key_data_ref_node = NULL;
    /*EncryptedKey*/
    axiom_node_t *enc_key_node = NULL, *enc_key_enc_mtd_node = NULL, *enc_key_key_info_node = NULL, *enc_key_key_name_node = NULL;
    axiom_node_t *enc_key_cv_node = NULL, *enc_key_cd_node = NULL;
    axiom_soap_body_t *body = NULL;
    axiom_soap_header_t *header = NULL;
    axis2_char_t *str_to_enc = NULL;
    enc_ctx_ptr enc_ctx = NULL;
    actions_ptr actions= NULL;
    oxs_key_ptr sessionkey = NULL;
    oxs_buffer_ptr session_key_buf_plain = NULL, session_key_buf_encrypted = NULL;    
    axis2_char_t* uuid = NULL;

    /*Populate actions*/ 
    actions = oxs_ctx_create_actions_ptr(env);
    ret = oxs_ctx_populate_actions_ptr(env, actions, param_action);   
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "Actions failure");
        return AXIS2_FAILURE;
    }

    /*Generate the session key*/
    sessionkey = oxs_key_generate_for_algo(env, actions->encryption_sym_algorithm);
    if(!sessionkey){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "Session key generate failed");
        return AXIS2_FAILURE;
    }
    sessionkey->name = "sessionkey";
    /*printf("\nsession_key for encryption = %s\n", sessionkey->data);*/

    body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
       
    body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
    header_node = AXIOM_SOAP_HEADER_GET_BASE_NODE(header, env);

   
    /*TODO Get the node to be encrypted. As per encryptionParts in the OutflowSecurity*/
    
    /*TODO Generate uuid for the EncryptedDataNode*/   
    uuid = "EncDataId-34526";

    /*If non is specified we encrypt the first element of the Body element*/
    if(!node_to_enc){
        node_to_enc = AXIOM_NODE_GET_FIRST_CHILD(body_node, env);
    }

    str_to_enc = AXIOM_NODE_TO_STRING(node_to_enc, env);
    
    /*Build the template*/
    
    enc_data_node =  oxs_token_build_encrypted_data_element(env, 
                        AXIOM_NODE_GET_PARENT(node_to_enc, env),
                        OXS_TypeEncElement,
                        uuid );
    enc_mtd_node = oxs_token_build_encryption_method_element(env, enc_data_node, OXS_HrefDes3Cbc);
    key_info_node = oxs_token_build_key_info_element(env, enc_data_node);
    key_name_node = oxs_token_build_key_name_element(env, key_info_node, sessionkey->name );
    cd_node = oxs_token_build_cipher_data_element(env, enc_data_node);
    cv_node = oxs_token_build_cipher_value_element(env,  cd_node, NULL); /*We pass NULL here OMXMLSEC will populate this*/

    /*Build the encryption ctx*/
    enc_ctx = oxs_ctx_create_ctx(env); 
    
    /*Set the key*/
    enc_ctx->key = oxs_key_create_key(env, sessionkey->name, (unsigned char*)sessionkey->data, sessionkey->size, OXS_KEY_USAGE_ENCRYPT);

    /*Hand the template over to OMXMLSEC*/
    ret = oxs_enc_encrypt_template(env, enc_data_node, str_to_enc, enc_ctx);
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "oxs_enc_encrypt_template failed");
        return ret;
    }else{
        /*printf("Encryption template is \n %s", AXIOM_NODE_TO_STRING(enc_data_node, env));*/
    }

    

    /*Encrypt the session key using the public key TODO*/
    /*Here u have the key file name or the key store name. Right now we support only the key file name*/
    session_key_buf_plain = oxs_string_to_buffer(env, (axis2_char_t*)sessionkey->data);
    session_key_buf_encrypted = oxs_create_buffer(env, (int)OXS_BUFFER_INITIAL_SIZE);

    ret = oxs_pubkey_encrypt_data(env, session_key_buf_plain, session_key_buf_encrypted, "keys/rsapub.pem");
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "oxs_pubkey_encrypt_data failed");
        return ret;
    
    }

    /*Create the key info*/
    enc_key_node = oxs_token_build_encrypted_key_element(env,sec_node );
    enc_key_enc_mtd_node = oxs_token_build_encryption_method_element(env, enc_key_node, actions->encryption_key_transport_algorithm);
    enc_key_key_info_node = oxs_token_build_key_info_element(env, enc_key_node );
    enc_key_key_name_node = oxs_token_build_key_name_element(env, enc_key_key_info_node,"hard-coded-key-name" );
    enc_key_cd_node = oxs_token_build_cipher_data_element(env, enc_key_node);
    enc_key_cv_node = oxs_token_build_cipher_value_element(env, enc_key_cd_node, (axis2_char_t*)session_key_buf_encrypted->data);
    enc_key_ref_list_node = oxs_token_build_reference_list_element(env, enc_key_node);
    /*TODO If there are multiple elements encrypted by the same session key, enqueue those here*/
    enc_key_data_ref_node = oxs_token_build_data_reference_element(env, enc_key_ref_list_node, uuid);
    
    /*Remove the encrypted node*/
    /*temp = AXIOM_NODE_DETACH(node_to_enc, env);
    if(!temp){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "Detaching encrypyted node failed");
        return AXIS2_FAILURE;
    }
*/
    /*Now arrange this encrypted nodes in a suitable manner to the envelope*/ 
   
    /*FREE*/
    oxs_ctx_free_ctx(enc_ctx); 
    return ret;
}

