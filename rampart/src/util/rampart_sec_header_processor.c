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
#include <rampart_encryption.h>
#include <rampart_action.h>
#include <rampart_constants.h>
#include <rampart_sec_header_processor.h>
#include <rampart_username_token.h>
#include <rampart_timestamp_token.h>
#include <rampart_util.h>
#include <rampart_sec_processed_result.h>
#include <rampart_handler_util.h>
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
#include <oxs_axiom.h>
#include <oxs_asym_ctx.h>
#include <oxs_token_reference_list.h>
#include <axis2_utils.h>
#include <axis2_array_list.h>

/*Private functions*/
static axis2_bool_t 
rampart_shp_validate_qnames(const axis2_env_t *env,
    axiom_node_t *node)
    
{
    axiom_element_t *node_ele = NULL;
    axis2_qname_t *qname = NULL;
    axis2_qname_t *node_qname = NULL;
    axis2_char_t *local_name = NULL;

    AXIS2_ENV_CHECK(env,AXIS2_FALSE);

    node_ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    if(!node_ele)
        return AXIS2_FALSE;

    local_name = AXIOM_ELEMENT_GET_LOCALNAME(node_ele,env);
    if(!local_name)
        return AXIS2_FALSE;

    if(AXIS2_STRCMP(local_name,RAMPART_SECURITY_TIMESTAMP)==0)
        qname = axis2_qname_create(env,local_name,RAMPART_WSU_XMLNS,RAMPART_WSU);

    else if(AXIS2_STRCMP(local_name,RAMPART_SECURITY_USERNAMETOKEN)==0)
        qname = axis2_qname_create(env,local_name,RAMPART_WSSE_XMLNS,RAMPART_WSSE);    

    else if(AXIS2_STRCMP(local_name,OXS_NODE_ENCRYPTED_KEY)==0)
        qname = axis2_qname_create(env,local_name,OXS_ENC_NS,OXS_XENC);

    else if(AXIS2_STRCMP(local_name,OXS_NODE_ENCRYPTED_DATA)==0)
        qname = axis2_qname_create(env,local_name,OXS_ENC_NS,OXS_XENC);

    else if(AXIS2_STRCMP(local_name,OXS_NODE_SIGNATURE)==0)
        return AXIS2_FALSE;

    else if(AXIS2_STRCMP(local_name,OXS_NODE_BINARY_SECURITY_TOKEN)==0)
        return AXIS2_FALSE;
    
    else if(AXIS2_STRCMP(local_name,OXS_NODE_REFERENCE_LIST)==0)
        return AXIS2_FALSE;
    
    else return AXIS2_FALSE;

    if(!qname)
        return AXIS2_FALSE;

    node_qname = AXIOM_ELEMENT_GET_QNAME(node_ele,env,node);

    if(!node_qname)
    {
        AXIS2_QNAME_FREE(qname,env);
        qname = NULL;
        return AXIS2_FALSE;
    }

    if(AXIS2_QNAME_EQUALS(qname,env,node_qname))
    {
        AXIS2_QNAME_FREE(qname,env);
        qname = NULL;
        return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}



static axis2_status_t 
rampart_shp_process_timestamptoken(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_node_t *sec_node)
{
    rampart_timestamp_token_t *timestamp_token = NULL;
    axis2_status_t valid_ts = AXIS2_FAILURE;
    axiom_node_t *ts_node = NULL;

    ts_node = oxs_axiom_get_node_by_local_name(env,sec_node,RAMPART_SECURITY_TIMESTAMP);
    if(!ts_node)
    {
        if(rampart_context_is_include_timestamp(rampart_context,env))
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Timestamp is not in the message"); 
            return AXIS2_FAILURE;
        }
        else
            return AXIS2_SUCCESS;
    }            
    else if(!rampart_context_is_include_timestamp(rampart_context,env))
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Timestamp should not be in the message.");
        return AXIS2_FAILURE;
    }
    else      
    {
        if(!rampart_shp_validate_qnames(env,ts_node))
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Error in the security header");
            return AXIS2_FAILURE;
        }
        
        timestamp_token = rampart_timestamp_token_create(env);
        valid_ts = RAMPART_TIMESTAMP_TOKEN_VALIDATE(timestamp_token, env, msg_ctx, ts_node);

        if (valid_ts)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][scp] Validating Timestamp is SUCCESS ");
            return AXIS2_SUCCESS;
        }
        else
        {   
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[rampart][scp] Timestamp is not valid");
            rampart_create_fault_envelope(env, RAMPART_FAULT_FAILED_AUTHENTICATION, "Timestamp is not valid", RAMPART_FAULT_IN_TIMESTAMP, msg_ctx);
        return AXIS2_FAILURE;
        }
    }
}

static axis2_status_t 
rampart_shp_process_usernametoken(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_node_t *sec_node)
{
    rampart_username_token_t *username_token = NULL;
    axis2_status_t valid_user = AXIS2_FAILURE;
    axiom_node_t *ut_node = NULL;

    ut_node = oxs_axiom_get_node_by_local_name(env,sec_node,RAMPART_SECURITY_USERNAMETOKEN);
    if(!ut_node)
    {
        if(rampart_context_is_include_username_token(rampart_context,env))
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Username token is not in the message");
            return AXIS2_FAILURE;
        }
        else
            return AXIS2_SUCCESS;
    }
    else if(!rampart_context_is_include_username_token(rampart_context,env))
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Username token should not be in the message.");
        return AXIS2_FAILURE;
    }
    else
    {
        if(!rampart_shp_validate_qnames(env,ut_node))
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Error in the security header");
            return AXIS2_FAILURE;
        }

        username_token = rampart_username_token_create(env);
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Validating UsernameToken");
        valid_user = RAMPART_USERNAME_TOKEN_VALIDATE(username_token, env,
                            msg_ctx, ut_node, rampart_context);    
    }
    if (valid_user)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Validating UsernameToken SUCCESS");
        return AXIS2_SUCCESS;
    }else{
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Validating UsernameToken FAILED");
        rampart_create_fault_envelope(env, RAMPART_FAULT_FAILED_AUTHENTICATION, "UsernameToken is not valid", RAMPART_FAULT_IN_USERNAMETOKEN, msg_ctx);
        return AXIS2_FAILURE;
    }
}

static axis2_status_t 
rampart_shp_process_encrypted_key(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node,
    axiom_node_t *encrypted_key_node)
{
    axiom_node_t *ref_list_node = NULL;
    axiom_node_t *enc_mtd_node = NULL;
    axis2_array_list_t *reference_list = NULL;
    axis2_char_t *enc_asym_algo = NULL;
    axis2_char_t *dec_key_file = NULL;
    axis2_char_t *password = NULL;
    axis2_char_t *enc_user = NULL;
    rampart_callback_t *password_callback = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    oxs_asym_ctx_t *asym_ctx = NULL;
    oxs_key_t *decrypted_sym_key = NULL;
    axis2_char_t *enc_asym_algo_in_pol = NULL; 
    axis2_char_t *enc_sym_algo_in_pol = NULL;
    pfunc password_function = NULL;
    void *param = NULL;
    int i = 0;
    
    /*Get EncryptedData references */
    ref_list_node = oxs_axiom_get_first_child_node_by_name(env, encrypted_key_node, OXS_NODE_REFERENCE_LIST, NULL, NULL);
    reference_list = oxs_token_get_reference_list_data(env, ref_list_node);
    /*If there are no references. Nothing to do. Return success*/
    if((!reference_list) || (0 == AXIS2_ARRAY_LIST_SIZE(reference_list, env))){
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Reference List is empty");
        return AXIS2_SUCCESS;
    }
    AXIS2_LOG_INFO(env->log, "[rampart][shp] Reference List has %d node reference(s)", AXIS2_ARRAY_LIST_SIZE(reference_list, env));

    /*Get the algorithm to decrypt the sesison key*/
    enc_mtd_node = oxs_axiom_get_first_child_node_by_name(env, encrypted_key_node, OXS_NODE_ENCRYPTION_METHOD, NULL, NULL);
    enc_asym_algo = oxs_token_get_encryption_method(env, enc_mtd_node);
    /*If the reference list > 0 then We have nodes to decrypt. Next step is to get the encrypted key*/
    /*Obtain the session key which is encrypted*/
    /*Create an asym_ctx*/
    /*We should verify the algorithm with policy*/
    enc_asym_algo_in_pol = rampart_context_get_enc_asym_algo(rampart_context,env);
    if(!enc_asym_algo_in_pol)
        return AXIS2_FAILURE;

    if(AXIS2_STRCMP(enc_asym_algo_in_pol,enc_asym_algo)!=0)
    {
        AXIS2_LOG_INFO(env->log, "The key is encrypted with the wrong algorithm");
        return AXIS2_FAILURE;
    }
                    
    asym_ctx = oxs_asym_ctx_create(env);
    dec_key_file = rampart_context_get_decryption_prop_file(rampart_context,env);
    if(!dec_key_file)
    {
        AXIS2_LOG_INFO(env->log, "Decryption Profile is not specified.");
        return AXIS2_FAILURE;
    }
    /*Get the password to retrieve the key from key store*/
/*  password = rampart_callback_encuser_password(env, actions, msg_ctx);*/

    enc_user = rampart_context_get_encryption_user(rampart_context,env);

    if(!enc_user)
        enc_user = rampart_context_get_user(rampart_context,env);

    if(!enc_user)
        return AXIS2_FAILURE;

    password_function = rampart_context_get_password_function(rampart_context,env);
    if(password_function)
        password = (*password_function)(env,enc_user,param);

    else
    {
        password_callback = rampart_context_get_password_callback(rampart_context,env);
        if(!password_callback)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] Password call back module is not specified.");
            return AXIS2_FAILURE;
        }
        password = rampart_callback_password(env, password_callback, enc_user);
    }
    
    oxs_asym_ctx_set_algorithm(asym_ctx, env, enc_asym_algo);
    oxs_asym_ctx_set_file_name(asym_ctx, env, dec_key_file);
    
/*  oxs_asym_ctx_set_pem_buf(asym_ctx, env, RAMPART_ACTIONS_GET_KEY_BUF(actions, env));*/
    oxs_asym_ctx_set_operation(asym_ctx, env, OXS_ASYM_CTX_OPERATION_PRV_DECRYPT);
    oxs_asym_ctx_set_password(asym_ctx, env, password);
    
    /*oxs_asym_ctx_set_format(asym_ctx, env, OXS_ASYM_CTX_FORMAT_PKCS12);*/
    oxs_asym_ctx_set_format(asym_ctx, env, oxs_util_get_format_by_file_extension(env, dec_key_file));

    /*Create an empty key*/
    decrypted_sym_key = oxs_key_create(env);

    /*Call decrypt for the EncryptedKey*/
    status = oxs_xml_enc_decrypt_key(env, asym_ctx, sec_node, encrypted_key_node,  decrypted_sym_key); 
    if(AXIS2_FAILURE == status){
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "[rampart][shp] Cannot decrypt the EncryptedKey");     
        rampart_create_fault_envelope(env, RAMPART_FAULT_FAILED_CHECK,
                                "Key decryption failed", RAMPART_FAULT_IN_ENCRYPTED_KEY, msg_ctx);
        return AXIS2_FAILURE;
    }
    /*Alright now we have the key used to encrypt the elements in the reference_list*/
    /*Go thru each and every node in the list and decrypt them*/

    /*Before decrypt we should get the symmetric algo from policy. 
      So for each encrypted element we can compare the algo. */
    
    enc_sym_algo_in_pol = rampart_context_get_enc_sym_algo(rampart_context,env);
    if(!enc_sym_algo_in_pol)
        return AXIS2_FAILURE;

    for(i=0 ; i < AXIS2_ARRAY_LIST_SIZE(reference_list, env); i++ )
    {
        axis2_char_t *id = NULL;
        axis2_char_t *id2 = NULL;
        axiom_node_t *enc_data_node = NULL;
        axiom_node_t *envelope_node = NULL;
        oxs_ctx_t *ctx = NULL;
        axiom_node_t *decrypted_node = NULL; 
        axiom_node_t *mtd_node = NULL;
        axis2_char_t *sym_algo = NULL;
        axiom_soap_body_t *soap_body = NULL;

        /*This need to be done in order to build the soap body.Do not remove.*/

        soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);

        /*Get the i-th element and decrypt it */
        id = (axis2_char_t*)AXIS2_ARRAY_LIST_GET(reference_list, env, i);
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Decrypting node, ID=%s", id);

        /*Need to remove # sign from the ID*/
        id2 = axis2_string_substring_starting_at(id, 1);
        envelope_node = AXIOM_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, env);

        /*Search for the node by its ID*/
        enc_data_node = oxs_axiom_get_node_by_id(env, envelope_node, OXS_ATTR_ID, id2);
        if(!enc_data_node){
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Node with ID=%s cannot be found", id);
            /*continue;*/
            rampart_create_fault_envelope(env, RAMPART_FAULT_FAILED_CHECK,
                                "Cannot find EncryptedData element", RAMPART_FAULT_IN_ENCRYPTED_DATA, msg_ctx);
            return AXIS2_FAILURE;
        }
        /*Create an enc_ctx*/    
        mtd_node = oxs_axiom_get_first_child_node_by_name(env, enc_data_node, OXS_NODE_ENCRYPTION_METHOD, NULL, NULL);
        if(!mtd_node)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Node with ID=%s cannot be found", id);
            /*continue;*/
            rampart_create_fault_envelope(env, RAMPART_FAULT_FAILED_CHECK,
                                "Cannot find EncryptionMethod Element", RAMPART_FAULT_IN_ENCRYPTED_DATA, msg_ctx);
            return AXIS2_FAILURE;
        }
        sym_algo = oxs_token_get_encryption_method(env, mtd_node);
        if(!sym_algo)
            return AXIS2_FAILURE;        

        if(AXIS2_STRCMP(sym_algo, enc_sym_algo_in_pol)!=0)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][shp] Sym algorithm is mismathced with policy.");
            return AXIS2_FAILURE;
        }        

        ctx = oxs_ctx_create(env);
        OXS_CTX_SET_KEY(ctx, env, decrypted_sym_key);
        
        status = oxs_xml_enc_decrypt_node(env, ctx, enc_data_node, &decrypted_node);
        if(AXIS2_FAILURE == status){
            rampart_create_fault_envelope(env, RAMPART_FAULT_FAILED_CHECK,
                                "Data decryption failed", RAMPART_FAULT_IN_ENCRYPTED_DATA, msg_ctx);
            return AXIS2_FAILURE;
        }
        /*Free*/
        OXS_CTX_FREE(ctx, env);
        ctx = NULL;

        AXIS2_LOG_INFO(env->log, "[rampart][shp] Node ID=%s decrypted successfuly", id);
    }
    
    
    /*Set the security processed result*/
    rampart_set_security_processed_result(env, msg_ctx,RAMPART_SPR_ENC_CHECKED, RAMPART_YES);

    /*Free*/
    oxs_asym_ctx_free(asym_ctx, env);
    asym_ctx = NULL;
    OXS_KEY_FREE(decrypted_sym_key, env);
    decrypted_sym_key = NULL;

    return AXIS2_SUCCESS;    
}
/*
#ifdef PRE_CHECK    
static axis2_status_t 
rampart_shp_pre_security_check(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_actions_t *actions,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node)
{
    axis2_char_t *items = NULL;
    axis2_array_list_t *items_list = NULL;
    int i = 0, size = 0;

    AXIS2_LOG_INFO(env->log, "[rampart][shp] Enforcing Security");
    items = RAMPART_ACTIONS_GET_ITEMS(actions, env);    
    if (!items)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] No items defined. So nothing to do.");
        return AXIS2_SUCCESS;
    }

  
    items_list = axis2_tokenize(env, items, ' ');
    size = AXIS2_ARRAY_LIST_SIZE(items_list, env);

  
    for (i = 0; i < size; i++)
    {
        axis2_char_t *item = NULL;
        item = AXIS2_ARRAY_LIST_GET(items_list, env, i);    
        
        if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_USERNAMETOKEN, AXIS2_STRTRIM(env, item, NULL))){
  
            int num_of_ut = 0;
            num_of_ut = oxs_axiom_get_number_of_children_with_qname(env, sec_node, 
                        RAMPART_SECURITY_TIMESTAMP, RAMPART_WSU_XMLNS, RAMPART_WSSE); 
            if(1 != num_of_ut){
                AXIS2_LOG_INFO(env->log, "[rampart][shp] UsernameToken is required. But not available");
                rampart_create_fault_envelope(env, RAMPART_FAULT_SECURITY_TOKEN_UNAVAILABLE, 
                        "UsernameToken is not available", RAMPART_FAULT_IN_USERNAMETOKEN, msg_ctx);
                return AXIS2_FAILURE;
            }
        }else if(0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_TIMESTAMP, AXIS2_STRTRIM(env, item, NULL))){
        
            int num_of_ts = 0;
            num_of_ts = oxs_axiom_get_number_of_children_with_qname(env, sec_node,
                        RAMPART_SECURITY_TIMESTAMP, RAMPART_WSU_XMLNS, RAMPART_WSSE);
            if(1 != num_of_ts){
                AXIS2_LOG_INFO(env->log, "[rampart][shp] Timestamp is required. But not available");
                rampart_create_fault_envelope(env, RAMPART_FAULT_SECURITY_TOKEN_UNAVAILABLE, 
                        "Timestamp is not available", RAMPART_FAULT_IN_TIMESTAMP, msg_ctx);
                return AXIS2_FAILURE;
            }

        }

    } 
    return AXIS2_SUCCESS;
}
#endif
*/
/*Compare security checked results with action items*/
/*
static axis2_status_t
rampart_shp_post_security_check(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_actions_t *actions)
{
    axis2_char_t *items = NULL;
    axis2_array_list_t *items_list = NULL;
    int i = 0, size = 0;

    AXIS2_LOG_INFO(env->log, "[rampart][shp] Enforcing Security");
    items = RAMPART_ACTIONS_GET_ITEMS(actions, env);
    if (!items)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] No items defined. So nothing to do.");
        return AXIS2_SUCCESS;
    }


    items_list = axis2_tokenize(env, items, ' ');
    size = AXIS2_ARRAY_LIST_SIZE(items_list, env);


    for (i = 0; i < size; i++)
    {
        axis2_char_t *item = NULL;
        axis2_char_t *result = NULL;
        item = AXIS2_ARRAY_LIST_GET(items_list, env, i);

        if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_USERNAMETOKEN, AXIS2_STRTRIM(env, item, NULL))){

            result = (axis2_char_t*)rampart_get_security_processed_result(env, msg_ctx, RAMPART_SPR_UT_CHECKED);   
            if(!result || (0 != AXIS2_STRCMP(result, RAMPART_YES)) ){
                AXIS2_LOG_INFO(env->log, "[rampart][shp] UsernameToken is required. But not available");
                rampart_create_fault_envelope(env, RAMPART_FAULT_SECURITY_TOKEN_UNAVAILABLE,
                        "UsernameToken is not available", RAMPART_FAULT_IN_USERNAMETOKEN, msg_ctx);
                return AXIS2_FAILURE;
            }
            result = NULL;
        }else if(0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_TIMESTAMP, AXIS2_STRTRIM(env, item, NULL))){

            result = (axis2_char_t*)rampart_get_security_processed_result(env, msg_ctx, RAMPART_SPR_TS_CHECKED);
            if(!result || (0 != AXIS2_STRCMP(result, RAMPART_YES)) ){
                AXIS2_LOG_INFO(env->log, "[rampart][shp] Timestamp is required. But not available");
                rampart_create_fault_envelope(env, RAMPART_FAULT_SECURITY_TOKEN_UNAVAILABLE,
                        "Timestamp is not available", RAMPART_FAULT_IN_TIMESTAMP, msg_ctx);
                return AXIS2_FAILURE;
            }
            result = NULL;
        }else if(0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_ENCRYPT, AXIS2_STRTRIM(env, item, NULL))){

            result = (axis2_char_t*)rampart_get_security_processed_result(env, msg_ctx, RAMPART_SPR_ENC_CHECKED);
            if(!result || (0 != AXIS2_STRCMP(result, RAMPART_YES)) ){
                AXIS2_LOG_INFO(env->log, "[rampart][shp] Encryption is required. But not available");
                rampart_create_fault_envelope(env, RAMPART_FAULT_SECURITY_TOKEN_UNAVAILABLE,
                        "Data are not encrypted", RAMPART_FAULT_IN_ENCRYPTED_KEY, msg_ctx);
                return AXIS2_FAILURE;
            }
            result = NULL;
        }else if (0 == AXIS2_STRCMP(RAMPART_ACTION_ITEMS_SIGNATURE, AXIS2_STRTRIM(env, item, NULL))){

        }

    }
    return AXIS2_SUCCESS;
}
*/

/*Public functions*/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_shp_process_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node)
{
    axiom_node_t *cur_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    /*If certian security elements are expected by the reciever, rampart should check for those */
    /*This should be removed once header encryption is introduced. But this pre-check avoids further processing of headers.*/
/*    
#ifdef PRE_CHECK    
    status =  rampart_shp_pre_security_check(env, msg_ctx, actions,  soap_envelope, sec_node);
    if(AXIS2_FAILURE == status){
        return AXIS2_FAILURE;
    }
#endif
*/
    /*TO DO*/
    /*There should be method to verify the security header before processing*/

    AXIS2_LOG_INFO(env->log, "[rampart][shp] Process security header");

    if((rampart_context_get_binding_type(rampart_context,env)) == RP_BINDING_ASYMMETRIC)
    {
        if(rampart_context_is_encrypt_before_sign(rampart_context,env))
        {
            /*First we should verify signature, When  signature is supported.*/

            /*This verification is a quick hack.This should be cganged in the future
              with a proper verification method before message processing */
            if(rampart_context_check_whether_to_encrypt(rampart_context,env))
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_ENCRYPTED_KEY);
                if(!cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] No Encrypted Key element");
                    return AXIS2_FAILURE;
                }
                if(!rampart_shp_validate_qnames(env,cur_node))             
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] Error in the security header");
                    return AXIS2_FAILURE;
                }                   
   
                AXIS2_LOG_INFO(env->log, "[rampart][shp] Process EncryptedKey");
                status = rampart_shp_process_encrypted_key(env,msg_ctx, rampart_context, soap_envelope, sec_node, cur_node);
                if(status!=AXIS2_SUCCESS)
                    return status;
            }
            else
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_ENCRYPTED_KEY);
                if(cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] policy does not specify encryption.");
                    return AXIS2_FAILURE;
                }
                else
                    status = AXIS2_SUCCESS;
            }
            
        }
        else 
        {
           /*We should decrypt the message first*/
            if(rampart_context_check_whether_to_encrypt(rampart_context,env))
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_ENCRYPTED_KEY);
                if(!cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] No Encrypted Key element");
                    return AXIS2_FAILURE;
                }
                if(!rampart_shp_validate_qnames(env,cur_node))
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] Error in the security header");
                    return AXIS2_FAILURE;
                }

                AXIS2_LOG_INFO(env->log, "[rampart][shp] Process EncryptedKey");
                status = rampart_shp_process_encrypted_key(env,msg_ctx, rampart_context, soap_envelope, sec_node, cur_node);
                if(status!=AXIS2_SUCCESS)
                    return status;
            }
            else
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_ENCRYPTED_KEY);
                if(cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] policy does not specify encryption.");
                    return AXIS2_FAILURE;
                }
                else
                    status = AXIS2_SUCCESS;;
            }
            /*After decrypting we may verify signature stuff.*/
        }
        /*Now we can process timestamp*/   
        status = rampart_shp_process_timestamptoken(env,msg_ctx,rampart_context,sec_node);
        if(status!=AXIS2_SUCCESS)
            return status; 

        status = rampart_shp_process_usernametoken(env,msg_ctx,rampart_context,sec_node);      
        if(status!=AXIS2_SUCCESS)
            return status;

        AXIS2_LOG_INFO(env->log, "[rampart][shp] Security header element processing, DONE ");
        /*Do the action accordingly*/
        return AXIS2_SUCCESS;

         
    }            
    else if((rampart_context_get_binding_type(rampart_context,env)) == RP_BINDING_SYMMETRIC)   
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] We still not support Symmetric binding.");
        return AXIS2_FAILURE;
    }        
    else if((rampart_context_get_binding_type(rampart_context,env)) == RP_BINDING_TRANSPORT)        
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] We still not support Transport binding.");
        return AXIS2_FAILURE;
    }
    else
    {
       AXIS2_LOG_INFO(env->log, "[rampart][shp] Invalid binding type.");
       return AXIS2_FAILURE;
    }        
    /*
    status = rampart_shp_post_security_check(env, msg_ctx, actions); 
    if(AXIS2_FAILURE == status){
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Security header doesn't confirms reciever's policy");
        return AXIS2_FAILURE;
    }*/

}



