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

#include <rampart_signature.h>
#include <stdio.h>
#include <axis2_utils.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_utility.h>
#include <rampart_constants.h>
#include <oxs_tokens.h>
#include <axis2_array_list.h>
#include <oxs_axiom.h>
#include <axis2_key_type.h>
#include <oxs_key.h>
#include <oxs_key_mgr.h>
#include <openssl_pkey.h>
#include <oxs_error.h>
#include <oxs_transform.h>
#include <oxs_transforms_factory.h>
#include <oxs_sign_ctx.h>
#include <oxs_sign_part.h>
#include <oxs_xml_signature.h>
#include <axis2_key_type.h>
#include <rampart_token_builder.h>
/*Public functions*/

oxs_x509_cert_t *AXIS2_CALL 
rampart_sig_get_cert(const axis2_env_t *env,
        rampart_context_t *rampart_context)
{
    void *key_buf = NULL;
    axis2_key_type_t type = 0;
    oxs_x509_cert_t *cert = NULL;    
    axis2_char_t *certificate_file = NULL;

    key_buf = rampart_context_get_certificate(rampart_context,env);
    if(key_buf)
    {
        type = rampart_context_get_certificate_type(rampart_context,env);
        if(type == AXIS2_KEY_TYPE_PEM)
        {
            cert = oxs_key_mgr_load_x509_cert_from_string(env,(axis2_char_t *)key_buf);
            if(!cert)
            {
                AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Certificate cannot be loaded from the buffer.");
                return NULL;
            } 
            else return cert;              
        }
        else
        {
            AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Key file type unknown.");
            return NULL;
        }
    }
    else
    {
        certificate_file = rampart_context_get_certificate_file(rampart_context,env);
        if(certificate_file)
        {
            cert = oxs_key_mgr_load_x509_cert_from_pem_file(env,certificate_file);
            if(!cert)
            {
                AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Certificate cannot be loaded from the file.");
                return NULL;
            }
            else return cert;
        }
        else
        {
            AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Public key certificate file is not specified.");
            return NULL;
        }
    }

}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_sig_sign_message(const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    rampart_context_t *rampart_context,
    axiom_soap_envelope_t *soap_envelope,
    axiom_node_t *sec_node)
{
    axis2_array_list_t *nodes_to_sign = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *asym_sig_algo = NULL;
    axis2_char_t *digest_method = NULL;
    oxs_sign_ctx_t *sign_ctx = NULL;
    axis2_array_list_t *sign_parts = NULL;
    axis2_array_list_t *tr_list = NULL;
    axis2_char_t *prv_key_file = NULL;
    axis2_char_t *password = NULL;
    axis2_bool_t server_side = AXIS2_FALSE;
    int token_type = 0;   
    rp_property_t *token = NULL;
    axis2_char_t *enc_user = NULL;
    openssl_pkey_t *prvkey = NULL;
    rampart_callback_t *password_callback = NULL;
    password_callback_fn password_function = NULL;
    axiom_node_t *sig_node = NULL;
    axis2_char_t *eki = NULL;
    axis2_bool_t is_direct_reference = AXIS2_TRUE;
    void *param = NULL;
    void *key_buf = NULL;
    int i = 0;
    oxs_x509_cert_t *cert = NULL;
    axiom_node_t *key_info_node = NULL;
    axiom_node_t *bst_node = NULL;
    axis2_char_t *cert_id = NULL;

    /*Get nodes to be signed*/
    server_side = axis2_msg_ctx_get_server_side(msg_ctx,env);
    nodes_to_sign = axis2_array_list_create(env,0);

    status = rampart_context_get_nodes_to_sign(rampart_context,env,soap_envelope,nodes_to_sign);

    if((status!=AXIS2_SUCCESS)||(axis2_array_list_size(nodes_to_sign,env)==0))
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] No parts specified or specified parts can't be found for Signature.");
        return AXIS2_SUCCESS;
    }                            
    /*Now we have to check whether a token is specified.*/
    token = rampart_context_get_token(rampart_context,env,AXIS2_FALSE,server_side);
    if(!token)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Signature Token is not specified");
        return AXIS2_FAILURE;
    }   
    token_type = rp_property_get_type(token,env);
    if(token_type != RP_TOKEN_X509)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] We only support X509 tokens");
        return AXIS2_FAILURE;
    }
    if(rampart_context_check_is_derived_keys(env,token))
    {
        AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] We still do not support derived keys");
        return AXIS2_FAILURE;
    }        
    eki = rampart_context_get_enc_key_identifier(rampart_context,token,server_side,env);
    if(!eki)
    {
        AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Key Identifier cannot be found.");
        return AXIS2_FAILURE;
    }        

    /*If the type is direct reference we first build bst element*/
    if(axis2_strcmp(eki,RAMPART_STR_DIRECT_REFERENCE)==0)
    {
        axis2_char_t *bst_data = NULL;

        cert = rampart_sig_get_cert(env,rampart_context);
        if(!cert)
        {
            return AXIS2_FAILURE;    
        }
        /*This flag will be useful when creating key Info element.*/            
        is_direct_reference = AXIS2_TRUE;

        cert_id = oxs_util_generate_id(env,(axis2_char_t*)OXS_CERT_ID);
        bst_data = oxs_x509_cert_get_data(cert, env); 
        if(!bst_data)
        {
            AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Certificate data cannot be loaded from the cert.");
            return AXIS2_FAILURE;
        }            
        
        bst_node = oxs_token_build_binary_security_token_element(env,sec_node, 
                cert_id , OXS_VALUE_X509V3, OXS_ENCODING_BASE64BINARY, bst_data); 
        if(!bst_node)
        {
            AXIS2_LOG_INFO(env->log,"[rampart][rampart_signature] Binary Security Token creation failed.");
            return AXIS2_FAILURE;
        }            
        
    }        
    else
        is_direct_reference = AXIS2_FALSE;

    /*Get the asymmetric signature algorithm*/
    asym_sig_algo = rampart_context_get_asym_sig_algo(rampart_context,env);
    digest_method = rampart_context_get_digest_mtd(rampart_context,env);
    
    sign_parts = axis2_array_list_create(env,0);   
    tr_list = axis2_array_list_create(env,0);

    /*Now we should create sign part for each node in the arraylist.*/
    
    for(i=0 ; i < axis2_array_list_size(nodes_to_sign, env); i++)
    {
        axiom_node_t *node_to_sign = NULL;
        axis2_char_t *id = NULL;
        oxs_sign_part_t *sign_part = NULL;                
        oxs_transform_t *tr = NULL;

        node_to_sign = (axiom_node_t *)axis2_array_list_get(nodes_to_sign,env,i);
        if(node_to_sign)
        {
            sign_part = oxs_sign_part_create(env);
            id = oxs_util_generate_id(env,(axis2_char_t*)OXS_SIG_ID);
            tr = oxs_transforms_factory_produce_transform(env, OXS_HREF_TRANSFORM_XML_EXC_C14N);
            axis2_array_list_add(tr_list, env, tr);
            oxs_sign_part_set_transforms(sign_part, env, tr_list);                                    
            oxs_axiom_add_attribute(env, node_to_sign, OXS_WSU, OXS_WSSE_XMLNS,OXS_ATTR_ID,id);
            oxs_sign_part_set_node(sign_part, env,node_to_sign);
            oxs_sign_part_set_digest_mtd(sign_part, env, digest_method);
            axis2_array_list_add(sign_parts, env, sign_part);
        }    
    }
/*            
    eki = rampart_context_get_enc_key_identifier(rampart_context,token,server_side,env);
    if(!eki)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_encryption] The token is not needed for inclusion.");
        return AXIS2_SUCCESS;
    }
*/
    sign_ctx = oxs_sign_ctx_create(env);
    
    /*First check whether the private key is set*/
    key_buf = rampart_context_get_prv_key(rampart_context,env);
    if(key_buf)
    {
        axis2_key_type_t type = 0;
        type = rampart_context_get_prv_key_type(rampart_context,env);
        if(type == AXIS2_KEY_TYPE_PEM)
        {
            prvkey = oxs_key_mgr_load_private_key_from_string(env,(axis2_char_t *)key_buf,NULL);
            if(!prvkey)
            {
                AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Can't load the key from buffer");
                return AXIS2_FAILURE;
            }
        }            
    }        
    
    /*Buffer is null load from the file*/
    else
    {
        prv_key_file = rampart_context_get_private_key_file(rampart_context,env);
        if(!prv_key_file)
        {
            AXIS2_LOG_INFO(env->log, "Private Key file is not specified.");
            return AXIS2_FAILURE;
        }

        /*Get the password to retrieve the key from key store*/
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
                        AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Password call back module is not loaded.");
                        return AXIS2_FAILURE;
                    }
                    password = rampart_callback_password(env, password_callback, enc_user);
                }
            }
        }
        prvkey = oxs_key_mgr_load_private_key_from_file(env, prv_key_file,password);
        if(!prvkey)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Cannot load the private key from file.");
            return AXIS2_FAILURE;
        }
    }
    /*These properties will set for creating signed info element*/
    oxs_sign_ctx_set_private_key(sign_ctx, env, prvkey);
    oxs_sign_ctx_set_sign_mtd_algo(sign_ctx, env, asym_sig_algo);                                                
    oxs_sign_ctx_set_c14n_mtd(sign_ctx, env, OXS_HREF_XML_EXC_C14N);
    oxs_sign_ctx_set_sign_parts(sign_ctx, env, sign_parts);
    oxs_sign_ctx_set_operation(sign_ctx, env, OXS_SIGN_OPERATION_SIGN);
   
    /*All the things are ready for signing.
    So lets try signing*/

    status = oxs_xml_sig_sign(env, sign_ctx,sec_node, &sig_node);
    if(status!=AXIS2_SUCCESS)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Message signing failed.");
        return AXIS2_FAILURE;
    } 
    /*Now we must build the Key Info element*/
    
    key_info_node = oxs_token_build_key_info_element(env,sig_node);
    if(!key_info_node)
    {
        AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Key info element build failed.");
        return AXIS2_FAILURE;
    }
    if(is_direct_reference)
    {
        axiom_node_t *str_node = NULL;
        axiom_node_t *reference_node = NULL;
        axis2_char_t *cert_id_ref = NULL;
        str_node = oxs_token_build_security_token_reference_element(env,key_info_node);
        if(!str_node)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Security Token element creation failed in Direct reference.");
            return AXIS2_FAILURE;
        }    
        cert_id_ref = axis2_stracat("#",cert_id,env);
        reference_node = oxs_token_build_reference_element(env,str_node,cert_id_ref,OXS_VALUE_X509V3); 
        if(!reference_node)
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Security Token element creation failed in Direct reference.");
            return AXIS2_FAILURE;
        }
    }    
    else
    {
        cert = rampart_sig_get_cert(env,rampart_context);
        if(!cert)
        {
            return AXIS2_FAILURE;   
        }
        if(axis2_strcmp(eki,RAMPART_STR_EMBEDDED)==0)
        {
            status = rampart_token_build_security_token_reference(env,key_info_node,cert,RTBP_EMBEDDED);    
        }
        else if(axis2_strcmp(eki,RAMPART_STR_ISSUER_SERIAL)==0)
        {
            status = rampart_token_build_security_token_reference(env,key_info_node,cert,RTBP_X509DATA_ISSUER_SERIAL);
        }
        else if(axis2_strcmp(eki,RAMPART_STR_KEY_IDENTIFIER)==0)
        {
            status = rampart_token_build_security_token_reference(env,key_info_node,cert,RTBP_KEY_IDENTIFIER);
        }
        else
        {
            AXIS2_LOG_INFO(env->log, "[rampart][rampart_signature] Unknown key Identifier type.Token attaching failed");
            status = AXIS2_FAILURE;    
        }
    }
    return status;
}




