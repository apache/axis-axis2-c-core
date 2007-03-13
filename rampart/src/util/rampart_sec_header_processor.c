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
#include <oxs_key.h>
#include <oxs_axiom.h>
#include <oxs_asym_ctx.h>
#include <oxs_tokens.h>
#include <axis2_utils.h>
#include <axis2_array_list.h>
#include <axis2_key_type.h>
#include <rampart_token_processor.h>
#include <oxs_sign_ctx.h>
#include <oxs_xml_signature.h>

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
        qname = axis2_qname_create(env,local_name,OXS_DSIG_NS,OXS_DS);

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
    axis2_char_t *prv_key_file = NULL;
    axis2_char_t *password = NULL;
    axis2_char_t *enc_user = NULL;
    rampart_callback_t *password_callback = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    oxs_asym_ctx_t *asym_ctx = NULL;
    oxs_key_t *decrypted_sym_key = NULL;
    axis2_char_t *enc_asym_algo_in_pol = NULL; 
    axis2_char_t *enc_sym_algo_in_pol = NULL;
    password_callback_fn password_function = NULL;
    void *param = NULL;
    int i = 0;
    void *key_buf = NULL;
    
    /*Get EncryptedData references */
    ref_list_node = oxs_axiom_get_first_child_node_by_name(env, encrypted_key_node, OXS_NODE_REFERENCE_LIST, NULL, NULL);
    reference_list = oxs_token_get_reference_list_data(env, ref_list_node);
    /*If there are no references. Nothing to do. Return success*/
    if((!reference_list) || (0 == axis2_array_list_size(reference_list, env))){
        AXIS2_LOG_INFO(env->log, "[rampart][shp] Reference List is empty");
        return AXIS2_SUCCESS;
    }
    AXIS2_LOG_INFO(env->log, "[rampart][shp] Reference List has %d node reference(s)", axis2_array_list_size(reference_list, env));

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

    oxs_asym_ctx_set_algorithm(asym_ctx, env, enc_asym_algo);

    key_buf = rampart_context_get_prv_key(rampart_context,env);
    if(key_buf)
    {
        axis2_key_type_t type = 0;
        type = rampart_context_get_prv_key_type(rampart_context,env);
        if(type == AXIS2_KEY_TYPE_PEM)
        {
            oxs_asym_ctx_set_pem_buf(asym_ctx, env, (axis2_char_t *)key_buf);
            oxs_asym_ctx_set_format(asym_ctx, env,OXS_ASYM_CTX_FORMAT_PEM);
        }            
    }        
    else
    {        
        prv_key_file = rampart_context_get_private_key_file(rampart_context,env);
        if(!prv_key_file)
        {
            AXIS2_LOG_INFO(env->log, "Private Key is not specified.");
            return AXIS2_FAILURE;
        }
        oxs_asym_ctx_set_file_name(asym_ctx, env, prv_key_file);
        oxs_asym_ctx_set_format(asym_ctx, env, oxs_util_get_format_by_file_extension(env, prv_key_file));

        /*Get the password to retrieve the key from key store*/
        /*  password = rampart_callback_encuser_password(env, actions, msg_ctx);*/

        password = rampart_context_get_prv_key_password(rampart_context,env);

        if(!password)
        {
            enc_user = rampart_context_get_encryption_user(rampart_context,env);

            if(!enc_user)
                enc_user = rampart_context_get_user(rampart_context,env);

            if(enc_user)
            {                
                password_function = rampart_context_get_pwcb_function(rampart_context,env);
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
            }   
        }
        oxs_asym_ctx_set_password(asym_ctx, env, password);
    }        
    oxs_asym_ctx_set_operation(asym_ctx, env, OXS_ASYM_CTX_OPERATION_PRV_DECRYPT);
    
    /*oxs_asym_ctx_set_format(asym_ctx, env, OXS_ASYM_CTX_FORMAT_PKCS12);*/

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

    for(i=0 ; i < axis2_array_list_size(reference_list, env); i++ )
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
        id = (axis2_char_t*)axis2_array_list_get(reference_list, env, i);
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


static axis2_status_t 
rampart_shp_process_signature(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node,
    axiom_node_t *sig_node)
{

    oxs_sign_ctx_t *sign_ctx = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *digest_mtd_pol = NULL;
    axis2_char_t *sig_mtd_pol = NULL;
    axiom_node_t *sign_info_node = NULL;
    axiom_node_t *cur_node = NULL;    
    rp_property_t *token = NULL;
    axis2_bool_t server_side = AXIS2_FALSE;
    axis2_char_t *eki = NULL;
    int token_type = 0;
    axiom_node_t *key_info_node = NULL;    
    axiom_node_t *str_node = NULL;
    axiom_node_t *str_child_node = NULL;        
    axis2_char_t *str_child_name = NULL;
    oxs_x509_cert_t *cert = NULL;
    axiom_node_t *key_info_child_node = NULL;
    axiom_node_t *envelope_node = NULL;
    server_side = axis2_msg_ctx_get_server_side(msg_ctx,env);
    sig_mtd_pol = rampart_context_get_asym_sig_algo(rampart_context,env);
    digest_mtd_pol = rampart_context_get_digest_mtd(rampart_context,env);

    if(!sig_mtd_pol || !digest_mtd_pol)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][shp] Error in policy. Specifying signature algorithms.");
        return AXIS2_FAILURE;  
    }    
    
    sign_info_node = oxs_axiom_get_first_child_node_by_name(env, sig_node,
                            OXS_NODE_SIGNEDINFO, OXS_DSIG_NS, OXS_DS );

    if(!sign_info_node)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][shp] Sign info cannot be found.Verifycation failed");
        return AXIS2_FAILURE;
    }

    cur_node = axiom_node_get_first_element(sign_info_node, env);
    while(cur_node)
    {
        axis2_char_t *localname =  NULL;
        localname  = axiom_util_get_localname(cur_node, env);
        if(axis2_strcmp(localname, OXS_NODE_SIGNATURE_METHOD)==0)
        {
            /*Verify the signature method with policy*/
            axis2_char_t *sig_mtd = NULL;
            sig_mtd = oxs_token_get_signature_method(env, cur_node);
            if(sig_mtd)
            {
                if(axis2_strcmp(sig_mtd_pol,sig_mtd)!=0)
                {
                    AXIS2_LOG_INFO(env->log,"[rampart][shp] Signature method in the message mismatch with policy.");
                    return AXIS2_FAILURE;
                }
            }
            else return AXIS2_FAILURE;
        }
        else if(axis2_strcmp(localname, OXS_NODE_REFERENCE)==0)
        {
            /*Verify each digest method with policy*/    
            axiom_node_t *digest_mtd_node = NULL;
            axis2_char_t *digest_mtd = NULL;
            digest_mtd_node  = oxs_axiom_get_first_child_node_by_name(env,cur_node,
                           OXS_NODE_DIGEST_METHOD, OXS_DSIG_NS, OXS_DS);
            if(digest_mtd_node)
            {
                digest_mtd = oxs_token_get_digest_method(env, digest_mtd_node);
                if(digest_mtd)
                {
                    if(axis2_strcmp(digest_mtd_pol,digest_mtd)!=0)
                    {
                        AXIS2_LOG_INFO(env->log,"[rampart][shp]Digest method is mismatch with policy.");
                        return AXIS2_FAILURE;                        
                    }
                }
                else return AXIS2_FAILURE;
            }
            else return AXIS2_FAILURE;                        
        }
        else
        {
            /*we do not need to process at this moment*/
        }
        cur_node = AXIOM_NODE_GET_NEXT_SIBLING(cur_node, env);
    }

    /*Get the key identifiers and build the certificate*/
    /*First we should verify with policy*/

    token = rampart_context_get_token(rampart_context,env,AXIS2_FALSE,server_side);

    if(!token)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][shp] Signature Token is not specified");
        return AXIS2_SUCCESS;
    }
    token_type = rp_property_get_type(token,env);
    if(token_type != RP_TOKEN_X509)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][shp] We still only support X509 tokens");
        return AXIS2_SUCCESS;
    }
    if(rampart_context_check_is_derived_keys(env,token))
    {
        AXIS2_LOG_INFO(env->log,"[rampart][shp] We still do not support derived keys");
        return AXIS2_FAILURE;
    }
    eki = rampart_context_get_enc_key_identifier(rampart_context,token,server_side,env);
    if(!eki)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp] No way of gettting the token.");
        return AXIS2_FAILURE;
    }
    key_info_node = oxs_axiom_get_first_child_node_by_name(env, sig_node,
                            OXS_NODE_KEY_INFO,OXS_DSIG_NS, OXS_DS );
    if(!key_info_node)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][shp]Verify failed. Key Info node is not in the message.");
        return AXIS2_FAILURE;
    }
    str_node = oxs_axiom_get_first_child_node_by_name(env,key_info_node,
                            OXS_NODE_SECURITY_TOKEN_REFRENCE,OXS_WSSE_XMLNS,OXS_WSSE);

    if(str_node)
    {
        str_child_node = axiom_node_get_first_element(str_node,env);
        if(str_child_node)
        {
            str_child_name = axiom_util_get_localname(str_child_node, env);
            if(str_child_name)
            {
                if(0 == axis2_strcmp(str_child_name,OXS_NODE_EMBEDDED))
                {
                    if(axis2_strcmp(eki,RAMPART_STR_EMBEDDED)!=0)
                    {
                        AXIS2_LOG_INFO(env->log,"[Rampart][shp]Key Reference Info is mismatch with policy");
                        return AXIS2_FAILURE;
                    }
                }
                else if(0 == axis2_strcmp(str_child_name,OXS_NODE_KEY_IDENTIFIER))
                {
                    if(axis2_strcmp(eki,RAMPART_STR_KEY_IDENTIFIER)!=0)
                    {
                        AXIS2_LOG_INFO(env->log,"[Rampart][shp]Key Reference Info is mismatch with policy");
                        return AXIS2_FAILURE;
                    }
                }
                else if(0 == axis2_strcmp(str_child_name,OXS_NODE_X509_DATA))
                {
                    if(axis2_strcmp(eki,RAMPART_STR_ISSUER_SERIAL)!=0)
                    {
                        AXIS2_LOG_INFO(env->log,"[Rampart][shp]Key Reference Info is mismatch with policy");
                        return AXIS2_FAILURE;
                    }
                }
                else if(0 != axis2_strcmp(str_child_name,OXS_NODE_REFERENCE))
                {
                    AXIS2_LOG_INFO(env->log,"[Rampart][shp]Unknown key reference element inside Security Token Reference");
                    return AXIS2_FAILURE;
                }
                cert = oxs_x509_cert_create(env);
                status = rampart_token_process_security_token_reference(env,str_node,sec_node,cert);
                if(status!=AXIS2_SUCCESS || !cert)
                {
                    AXIS2_LOG_INFO(env->log,"[Rampart][shp]Cannot load the message to verify the message.");
                    return AXIS2_FAILURE;
                }
            }
            else
            {
                AXIS2_LOG_INFO(env->log,"[Rampart][shp]Cannot get the key Reference Type from the message.");
                return AXIS2_FAILURE;
            }
        }
        else
        {
            AXIS2_LOG_INFO(env->log,"[Rampart][shp]No Child node in the Security Token Reference Element .");
            return AXIS2_FAILURE;
        }
    }
    /*So there may be scenarios where there is no Security Token Reference Element.*/
    else
    {
        /*In such case policy support only Isssuer Serial scenario.*/
        if(axis2_strcmp(eki,RAMPART_STR_ISSUER_SERIAL)==0)
        {
            key_info_child_node = axiom_node_get_first_element(key_info_node,env);
            if(key_info_child_node)
            {
                axis2_char_t *key_info_child_name = NULL;
                key_info_child_name = axiom_util_get_localname(key_info_child_node, env);
                if(key_info_child_name)
                {
                    if(0 == axis2_strcmp(key_info_child_name,OXS_NODE_X509_DATA))
                    {
                        status = rampart_token_process_x509_data(env,key_info_child_node,cert);
                        if(status!=AXIS2_SUCCESS || !cert)
                        {
                            AXIS2_LOG_INFO(env->log,"[Rampart][shp]Cannot load the message to verify the message.");
                            return AXIS2_FAILURE;
                        }
                    }
                    else
                    {
                        AXIS2_LOG_INFO(env->log,"[Rampart][shp]Cannot get the key Reference Type from the message.");
                        return AXIS2_FAILURE;
                    }
                }
                else
                {
                    AXIS2_LOG_INFO(env->log,"[Rampart][shp]Cannot get the key Reference Type from the message.");
                    return AXIS2_FAILURE;
                }                        
            }
            else
            {
                AXIS2_LOG_INFO(env->log,"[Rampart][shp]Cannot get the key Reference Type from the message.");
                return AXIS2_FAILURE;
            }                        
        }
        else
        {
            AXIS2_LOG_INFO(env->log,"[Rampart][shp]Can't be used as a direct child of Key Info");
            return AXIS2_FAILURE;
        }
    }
    sign_ctx = oxs_sign_ctx_create(env);
    if(!sign_ctx)
    {
        AXIS2_LOG_INFO(env->log,"[Rampart][shp]Sign context creation failed. Out of Memeory.");
        return AXIS2_FAILURE;
    }    
    /*Set the required values in sig_ctx*/
    oxs_sign_ctx_set_operation(sign_ctx, env, OXS_SIGN_OPERATION_VERIFY);
    oxs_sign_ctx_set_certificate(sign_ctx, env, cert);
       
    envelope_node = axiom_soap_envelope_get_base_node(soap_envelope,env);
    if(!envelope_node)
    {
        AXIS2_LOG_INFO(env->log,"[Rampart][shp]Cannot get the node from envelope.");
        return AXIS2_FAILURE;
    }
 
    /*Verify the signature*/
    status = oxs_xml_sig_verify(env, sign_ctx, sig_node,envelope_node);
    if(status!=AXIS2_SUCCESS)
    {
        AXIS2_LOG_INFO(env->log,"[Rampart][shp]Signature Verification failed.");
        return AXIS2_FAILURE;
    }

    return status;
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
    size = axis2_array_list_size(items_list, env);

  
    for (i = 0; i < size; i++)
    {
        axis2_char_t *item = NULL;
        item = axis2_array_list_get(items_list, env, i);    
        
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
    size = axis2_array_list_size(items_list, env);


    for (i = 0; i < size; i++)
    {
        axis2_char_t *item = NULL;
        axis2_char_t *result = NULL;
        item = axis2_array_list_get(items_list, env, i);

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
            /*First we should verify signature.*/
            if(rampart_context_check_whether_to_sign(rampart_context,env))
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_SIGNATURE);
                if(!cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] No Signature element");
                    return AXIS2_FAILURE;
                }
                if(!rampart_shp_validate_qnames(env,cur_node))             
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] Error in the security header");
                    return AXIS2_FAILURE;
                }                   
                AXIS2_LOG_INFO(env->log, "[rampart][shp] Processing Signature element.");
                status = rampart_shp_process_signature(env,msg_ctx,rampart_context,soap_envelope,sec_node,cur_node);
                if(status!=AXIS2_SUCCESS)
                    return status;
            }
            else
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_SIGNATURE);
                if(cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] policy does not specify signature.");
                    return AXIS2_FAILURE;
                }
                else
                    status = AXIS2_SUCCESS;
            }

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
            if(rampart_context_check_whether_to_sign(rampart_context,env))
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_SIGNATURE);
                if(!cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] No Signature element");
                    return AXIS2_FAILURE;
                }
                if(!rampart_shp_validate_qnames(env,cur_node))
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] Error in the security header");
                    return AXIS2_FAILURE;
                }
                AXIS2_LOG_INFO(env->log, "[rampart][shp] Processing Signature element.");
                status = rampart_shp_process_signature(env,msg_ctx,rampart_context,soap_envelope,sec_node,cur_node);
                if(status!=AXIS2_SUCCESS)
                    return status;
            }
            else
            {
                cur_node = oxs_axiom_get_node_by_local_name(env,sec_node,OXS_NODE_SIGNATURE);
                if(cur_node)
                {
                    AXIS2_LOG_INFO(env->log, "[rampart][shp] policy does not specify signature.");
                    return AXIS2_FAILURE;
                }
                else
                    status = AXIS2_SUCCESS;
            }
        }
        /*Now we can process timestamp*/   
        status = rampart_shp_process_timestamptoken(env,msg_ctx,rampart_context,sec_node);
        if(status!=AXIS2_SUCCESS)
            return status; 

        if( axis2_msg_ctx_get_server_side(msg_ctx, env))
        {            
            status = rampart_shp_process_usernametoken(env,msg_ctx,rampart_context,sec_node);      
            if(status!=AXIS2_SUCCESS)
                return status;
        }
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



