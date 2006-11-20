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
#include <axis2_utils.h>
#include <rampart_encryption.h>
#include <rampart_action.h>
#include <rampart_constants.h>
#include <rampart_sec_header_processor.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_utility.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_token_data_reference.h>
#include <oxs_token_cipher_data.h>
#include <oxs_token_cipher_value.h>
#include <oxs_token_key_info.h>
#include <oxs_token_key_name.h>
#include <oxs_key.h>
#include <oxs_token_reference_list.h>
#include <axis2_array_list.h>
#include <oxs_axiom.h>
#include <oxs_asym_ctx.h>

/*Private functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_shp_process_encrypted_key(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_actions_t *actions,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *encrypted_key_node)
{
    axiom_node_t *ref_list_node = NULL;
    axis2_array_list_t *reference_list = NULL;
    axis2_char_t *enc_asym_algo = NULL;
    axis2_char_t *certificate_file = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    oxs_asym_ctx_t *asym_ctx = NULL;
    oxs_key_t *decrypted_sym_key = NULL;
    int i = 0;

    /*Get EncryptedData references */
    ref_list_node = oxs_axiom_get_first_child_node_by_name(env, encrypted_key_node, OXS_NODE_REFERENCE_LIST, NULL, NULL);
    reference_list = oxs_token_get_reference_list_data(env, ref_list_node);
    /*If there are no references. Nothing to do. Return success*/
    if(0 == AXIS2_ARRAY_LIST_SIZE(reference_list, env)){
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Reference List is empty");
        return AXIS2_SUCCESS;
    }
    AXIS2_LOG_INFO(env->log, "[rampart][shp] Reference List has %d node references", AXIS2_ARRAY_LIST_SIZE(reference_list, env));

    /*If the reference list > 0 then We have nodes to decrypt. Next step is to get the encrypted key*/
    /*Obtain the session key which is encrypted*/
    /*Create an asym_ctx*/
    asym_ctx = oxs_asym_ctx_create(env);
    /*Set default values. Might be useful if there are no data available to identify those*/
    enc_asym_algo = RAMPART_ACTIONS_GET_ENC_KT_ALGO(actions, env);
    certificate_file = RAMPART_ACTIONS_GET_DEC_KEY_FILE(actions, env);
    oxs_asym_ctx_set_algorithm(asym_ctx, env, enc_asym_algo);
    oxs_asym_ctx_set_file_name(asym_ctx, env, certificate_file);
    oxs_asym_ctx_set_operation(asym_ctx, env, OXS_ASYM_CTX_OPERATION_PRV_DECRYPT);

    /*Create an empty key*/
    decrypted_sym_key = oxs_key_create(env);

    /*Call decrypt for the EncryptedKey*/
    status = oxs_xml_enc_decrypt_key(env, asym_ctx, encrypted_key_node,  decrypted_sym_key); 
    
    /*Alright now we have the key used to encrypt the elements in the reference_list*/
    /*Go thru each and every node in the list and decrypt them*/
    for(i=0 ; i < AXIS2_ARRAY_LIST_SIZE(reference_list, env); i++ ){
        axis2_char_t *id = NULL;
        axiom_node_t *enc_data_node = NULL;
        oxs_ctx_t *ctx = NULL;
        axiom_node_t *decrypted_node = NULL; 
        axiom_soap_body_t *body = NULL;
        axiom_node_t *body_node = NULL;

        /*Get the i-th element and decrypt it */
        id = (axis2_char_t*)AXIS2_ARRAY_LIST_GET(reference_list, env, i);
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Decrypting node, ID=%s", id);
        /*TODO Write a proper function to get the node bu its ID*/

        /*This is only for test*/
        body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
        body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
        enc_data_node = AXIOM_NODE_GET_FIRST_CHILD(body_node, env);    
        
        /*Create an enc_ctx*/    
        ctx = oxs_ctx_create(env);
        OXS_CTX_SET_KEY(ctx, env, decrypted_sym_key);
    
        status = oxs_xml_enc_decrypt_node(env, ctx, enc_data_node, &decrypted_node);
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Node ID=%s decrypted successfuly", id);
    }

    
    return AXIS2_SUCCESS;    
}


/*Public functions*/


AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_shp_process_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_actions_t *actions,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node)
{
    axiom_node_t *cur_node = NULL;
    axiom_element_t *cur_ele = NULL;
    axis2_char_t *cur_node_name = NULL;
    axis2_qname_t *cur_qname = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_LOG_INFO(env->log, "[rampart][shp] Process security header");
    /*Get the first token of the security header element*/
    cur_node = AXIOM_NODE_GET_FIRST_CHILD(sec_node, env);
    
    while(cur_node){
        cur_ele = AXIOM_NODE_GET_DATA_ELEMENT(cur_node, env);
        cur_qname = AXIOM_ELEMENT_GET_QNAME(cur_ele, env, cur_node);
        cur_node_name  = AXIS2_QNAME_GET_LOCALPART(cur_qname, env);
        
        /*Check For following types, Username token, EncryptedKey, EncryptedData, ReferenceList, Signature*/
        if(0 == AXIS2_STRCMP(cur_node_name , RAMPART_SECURITY_USERNAMETOKEN) ){
            /*Process UT*/
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Process Usernametoken");

        }else if(0 == AXIS2_STRCMP(cur_node_name , RAMPART_SECURITY_TIMESTAMP)){
            /*Verify TS*/
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Process Timestamptoken");

        }else if(0 == AXIS2_STRCMP(cur_node_name ,OXS_NODE_ENCRYPTED_KEY)){
            /*Process EncryptedKey*/
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Process EncryptedKey");
            status = rampart_shp_process_encrypted_key(env,msg_ctx, actions, soap_envelope, cur_node);
        }else if(0 == AXIS2_STRCMP(cur_node_name ,OXS_NODE_ENCRYPTED_DATA)){
            /*Process Encrypteddata*/
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Process EncryptedData");

        }else if(0 == AXIS2_STRCMP(cur_node_name ,OXS_NODE_REFERENCE_LIST)){
            /*List is placed Out side of the EncryptedKey*/
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Process ReferenceList");
        }
        cur_node = AXIOM_NODE_GET_NEXT_SIBLING(cur_node, env);
    }/*End of while*/
    
    
    AXIS2_LOG_INFO(env->log, "[rampart][shp] Security header element processing, DONE ");
    /*Do the action accordingly*/
    return AXIS2_SUCCESS;
}

