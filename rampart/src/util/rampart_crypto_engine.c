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
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_enc_engine.h>
#include <rampart_crypto_engine.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_token_data_reference.h>
#include <oxs_token_cipher_data.h>
#include <oxs_token_cipher_value.h>
#include <oxs_token_key_info.h>
#include <oxs_token_key_name.h>
#include <oxs_key.h>
#include <rampart_action.h>
#include <rampart_constants.h>
#include <oxs_token_reference_list.h>
#include <axis2_array_list.h>

typedef struct rampart_crypto_engine_impl
{
    rampart_crypto_engine_t engine;
}
rampart_crypto_engine_impl_t;

#define AXIS2_INTF_TO_IMPL(engine) ((rampart_crypto_engine_impl_t *)engine)

/*******************Function Headers ****************************/
static void
rampart_crypto_engine_init_ops(
    rampart_crypto_engine_t *engine);

/*Public function*/
axis2_status_t AXIS2_CALL
rampart_crypto_engine_free(
    rampart_crypto_engine_t *engine,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
rampart_crypto_engine_encrypt_message(
                      rampart_crypto_engine_t *engine,
                      const axis2_env_t *env,
                      axis2_msg_ctx_t *msg_ctx,
                      rampart_actions_t *actions,
                      axiom_soap_envelope_t *soap_envelope ,
                      axiom_node_t *sec_node);

axis2_status_t AXIS2_CALL
rampart_crypto_engine_decrypt_message(
                      rampart_crypto_engine_t *engine,
                      const axis2_env_t *env,
                      axis2_msg_ctx_t *msg_ctx,
                      rampart_actions_t *actions,
                      axiom_soap_envelope_t *soap_envelope ,
                      axiom_node_t *sec_node);

/*****************End of function headers ****************************/

rampart_crypto_engine_t *AXIS2_CALL
rampart_crypto_engine_create(
        const axis2_env_t *env)
{
    rampart_crypto_engine_impl_t *engine_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    engine_impl =  (rampart_crypto_engine_impl_t *) AXIS2_MALLOC (env->allocator,
                        sizeof (rampart_crypto_engine_impl_t));

    if(NULL == engine_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    engine_impl->engine.ops = AXIS2_MALLOC (env->allocator,
        sizeof(rampart_crypto_engine_ops_t));
    
    if(NULL == engine_impl->engine.ops)
    {
        rampart_crypto_engine_free(&(engine_impl->engine), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** initialize ops */
    rampart_crypto_engine_init_ops(&(engine_impl->engine));

    return &(engine_impl->engine);

}

static void
rampart_crypto_engine_init_ops(
    rampart_crypto_engine_t *engine)
{
    engine->ops->decrypt_message = rampart_crypto_engine_decrypt_message;
    engine->ops->encrypt_message = rampart_crypto_engine_encrypt_message;
    engine->ops->free = rampart_crypto_engine_free;
}


axis2_status_t AXIS2_CALL
rampart_crypto_engine_free(
    rampart_crypto_engine_t *engine,
    const axis2_env_t *env)
{
    rampart_crypto_engine_impl_t *engine_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(engine);

    if(engine->ops)
    {
        AXIS2_FREE(env->allocator, engine->ops);
        engine->ops = NULL;
    }
    if(engine_impl)
    {
        AXIS2_FREE(env->allocator, engine_impl);
        engine_impl = NULL;
    }
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
rampart_crypto_engine_encrypt_message(
                      rampart_crypto_engine_t *engine,
                      const axis2_env_t *env,
                      axis2_msg_ctx_t *msg_ctx,
                      rampart_actions_t *actions,
                      axiom_soap_envelope_t *soap_envelope ,
                      axiom_node_t *sec_node)
{
    
    axis2_status_t ret = AXIS2_FAILURE;
    axiom_node_t *node_to_enc = NULL, *body_node = NULL;
    /*EncryptedData variables*/
    axiom_node_t *enc_data_node = NULL, *enc_mtd_node = NULL, *key_info_node = NULL, *key_name_node = NULL;
    axiom_node_t *cv_node = NULL, *cd_node = NULL, *enc_key_ref_list_node = NULL, *enc_key_data_ref_node = NULL;
    /*EncryptedKey varaibles*/
    axiom_node_t *enc_key_node = NULL, *enc_key_enc_mtd_node = NULL, *enc_key_key_info_node = NULL, *enc_key_key_name_node = NULL;
    axiom_node_t *enc_key_cv_node = NULL, *enc_key_cd_node = NULL;
    axiom_soap_body_t *body = NULL;
    axis2_char_t *str_to_enc = NULL;
    oxs_ctx_t * enc_ctx = NULL;
    oxs_key_t *sessionkey = NULL;
    oxs_buffer_t *session_key_buf_plain = NULL;
    oxs_buffer_t *session_key_buf_encrypted = NULL;    
    axis2_char_t* uuid = NULL;
    oxs_enc_engine_t *enc_engine = NULL;
    rampart_crypto_engine_impl_t *engine_impl = NULL;
    axiom_node_t *removed_node = NULL;
    axis2_char_t* tmp_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(engine);

    /*Generate the session key*/
    sessionkey = oxs_key_create_key(env);
    if(!sessionkey){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "Session key generate failed");
        return AXIS2_FAILURE;
    }
    /*Generate a random key for the encryption session*/
    ret = OXS_KEY_FOR_ALGO(sessionkey, env,  RAMPART_ACTIONS_GET_ENC_SYM_ALGO(actions, env));
    ret = OXS_KEY_SET_NAME(sessionkey, env, "sessionkey");
    ret = OXS_KEY_SET_USAGE(sessionkey, env, OXS_KEY_USAGE_ENCRYPT);

    /*printf("\nSession Key is %s", OXS_KEY_GET_DATA(sessionkey,env));    */

    body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
   
    /*TODO Get the node to be encrypted. As per encryptionParts in the OutflowSecurity*/
    
    /*TODO Generate uuid for the EncryptedDataNode*/   
    uuid = "EncDataId-34526";

    /*If non is specified we encrypt the first element of the Body element*/
    if(!node_to_enc){
        node_to_enc = AXIOM_NODE_GET_FIRST_CHILD(body_node, env);
    }


    str_to_enc = AXIOM_NODE_TO_STRING(node_to_enc, env);
    
    /*Build the template*/
    tmp_str = AXIOM_NODE_TO_STRING(AXIOM_NODE_GET_PARENT(node_to_enc, env), env) ;

    enc_data_node =  oxs_token_build_encrypted_data_element(env,
                        AXIOM_NODE_GET_PARENT(node_to_enc, env), 
                        OXS_TypeEncElement,
                        uuid );
    enc_mtd_node = oxs_token_build_encryption_method_element(env, enc_data_node, RAMPART_ACTIONS_GET_ENC_SYM_ALGO(actions, env));
    key_info_node = oxs_token_build_key_info_element(env, enc_data_node);
    key_name_node = oxs_token_build_key_name_element(env, key_info_node, OXS_KEY_GET_NAME(sessionkey, env));
    cd_node = oxs_token_build_cipher_data_element(env, enc_data_node);
    cv_node = oxs_token_build_cipher_value_element(env,  cd_node, NULL); /*We pass NULL here OMXMLSEC will populate this*/

    /*Build the encryption ctx*/
    enc_ctx = oxs_ctx_create(env); 
    
    /*Set the key*/
    OXS_CTX_SET_KEY(enc_ctx, env, sessionkey);


    /*printf("\nSession_key for encryption = %s\n", OXS_KEY_GET_DATA(sessionkey, env));*/
    printf("\nString for encryption = %s\n", str_to_enc);

    /*Hand the template over to OMXMLSEC*/
    enc_engine = oxs_enc_engine_create(env);
    ret = OXS_ENC_ENGINE_ENCRYPT_TEMPLATE(enc_engine, env, enc_data_node, str_to_enc, enc_ctx);
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "Template encryption failed");
        return AXIS2_FAILURE;
    }else{
        /*printf("Encryption template is \n %s", AXIOM_NODE_TO_STRING(enc_data_node, env));*/
    }

    /*Here u have the public key file name or the key store name. Right now we support only the key file name. 
     The meaning is totally wrong but for the moment we have to live with this*/
    session_key_buf_plain = oxs_buffer_create(env);
    ret = OXS_BUFFER_POPULATE(session_key_buf_plain, env,
                                OXS_KEY_GET_DATA(sessionkey, env),
                                 OXS_KEY_GET_SIZE(sessionkey, env));

    session_key_buf_encrypted = oxs_buffer_create(env);
    ret = OXS_ENC_ENGINE_PUB_KEY_ENCRYPT_DATA(enc_engine, env, session_key_buf_plain,
                         session_key_buf_encrypted, 
                            RAMPART_ACTIONS_GET_ENC_PROP_FILE(actions, env));
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
                     "oxs_pubkey_encrypt_data failed");
        return ret;
    }
    

    /*Create the key info*/
    enc_key_node = oxs_token_build_encrypted_key_element(env, sec_node );
    /*return AXIS2_SUCCESS;*/ /*TODO remove: This is here to test the SOAP HEADER problem while attaching enc_mtd_node*/
    tmp_str = AXIOM_NODE_TO_STRING(AXIOM_NODE_GET_PARENT(sec_node, env), env) ;
   
    tmp_str = RAMPART_ACTIONS_GET_ENC_KT_ALGO(actions, env);
    enc_key_enc_mtd_node = oxs_token_build_encryption_method_element(env, enc_key_node, RAMPART_ACTIONS_GET_ENC_KT_ALGO(actions, env));
    
    enc_key_key_info_node = oxs_token_build_key_info_element(env, enc_key_node );
    enc_key_key_name_node = oxs_token_build_key_name_element(env, enc_key_key_info_node,"hard-coded-key-name" );
    enc_key_cd_node = oxs_token_build_cipher_data_element(env, enc_key_node);
    enc_key_cv_node = oxs_token_build_cipher_value_element(env, enc_key_cd_node, 
                        (axis2_char_t*)OXS_BUFFER_GET_DATA(session_key_buf_encrypted, env));
    enc_key_ref_list_node = oxs_token_build_reference_list_element(env, enc_key_node);
    /*TODO If there are multiple elements encrypted by the same session key, enqueue those here*/
    enc_key_data_ref_node = (axiom_node_t*)oxs_token_build_data_reference_element(env, enc_key_ref_list_node, uuid);
    
    /*Remove the encrypted node*/
    removed_node = AXIOM_NODE_DETACH(node_to_enc, env);
    if(!removed_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_ENCRYPT_FAILED,
            "Detaching encrypyted node failed");
        return AXIS2_FAILURE;
    }
    
    /*Now arrange this encrypted nodes in a suitable manner to the envelope*/ 
   
    /*FREE*/
    /*oxs_ctx_free_ctx(enc_ctx); */
    return ret;
}

axis2_status_t AXIS2_CALL
rampart_crypto_engine_decrypt_message(
                      rampart_crypto_engine_t *engine,
                      const axis2_env_t *env,
                      axis2_msg_ctx_t *msg_ctx,
                      rampart_actions_t *actions,
                      axiom_soap_envelope_t *soap_envelope ,
                      axiom_node_t *sec_node)
{
    axiom_node_t *enc_data_node = NULL, *parent_of_enc_node = NULL;
    axiom_node_t *body_node = NULL , *decrypted_node = NULL;
    axiom_node_t *ref_list_node = NULL;
    axiom_soap_body_t *body = NULL;
    axis2_char_t *decrypted_data = NULL;
    oxs_ctx_t * enc_ctx = NULL;
    axiom_node_t *enc_key_node = NULL; 
    oxs_key_t *session_key = NULL;
    oxs_key_t *prv_key = NULL;
    axis2_array_list_t *uuid_list = NULL;
    oxs_enc_engine_t *enc_engine = NULL;
    rampart_crypto_engine_impl_t *engine_impl = NULL;
    axis2_status_t ret = AXIS2_FAILURE;
    axis2_char_t *temp_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    engine_impl = AXIS2_INTF_TO_IMPL(engine);


    body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(body, env);
    
    /*TODO Get the Encrypted key*/
    enc_key_node =  (axiom_node_t*)oxs_axiom_get_first_child_node_by_name(env, sec_node, OXS_NodeEncryptedKey, NULL, NULL);

    /*Create a private key and use to extract the session key*/
    temp_str = RAMPART_ACTIONS_GET_DEC_PROP_FILE(actions, env) ;
    prv_key = oxs_key_create_key(env);
    ret = OXS_KEY_POPULATE(prv_key, env, NULL, 
                            RAMPART_ACTIONS_GET_DEC_PROP_FILE(actions, env) ,
                             0, OXS_KEY_USAGE_DECRYPT);
    
    /*We support only one Encrypted Key element at the moment*/   
    session_key = oxs_key_create_key(env);
    enc_engine = oxs_enc_engine_create(env);
    ret = OXS_ENC_ENGINE_GET_ENCRYPTED_KEY(enc_engine, env, enc_key_node, prv_key , session_key); 
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                     "Cannot get the encrypted key");
        return ret;

    }
    /*Ohh yeah... now we got the seesion key, which is used encrypt data referred by the reference list*/    
    ref_list_node = (axiom_node_t*)oxs_axiom_get_first_child_node_by_name(env, enc_key_node, OXS_NodeReferenceList, NULL, NULL);
    if(!ref_list_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                     "Cannot get the ReferenceList node");
        return ret;

    }
    uuid_list = oxs_token_get_reference_list_data(env, ref_list_node);    
    
    /*TODO Get the encrypted node(s). Right now we support only one. To support more than one EncryptedData element use the uuid_list*/
    
    enc_data_node = (axiom_node_t*)oxs_axiom_get_first_child_node_by_name(env, body_node, OXS_NodeEncryptedData, NULL, NULL);
    

    /*TODO We assume that the very first element of bpody is encrypted data.
    This might be different if a sub element is encrypted*/
    /*Build the encryption ctx*/
    enc_ctx = oxs_ctx_create(env);

    /*Set the key*/
    OXS_CTX_SET_KEY(enc_ctx, env, session_key);
    
    ret = OXS_ENC_ENGINE_DECRYPT_TEMPLATE(enc_engine, env, enc_data_node, &decrypted_data, enc_ctx);
    if(ret == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                     "oxs_enc_decrypt_template failed");
        return ret;
    }else{
        printf("\nDecrypted data is \n %s\n\n", decrypted_data);
    }
    
    /*Now build the node using decrypted data*/
    if(!decrypted_data){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                 "No decrypted data");
        return AXIS2_FAILURE;
    }
    parent_of_enc_node = AXIOM_NODE_GET_PARENT(enc_data_node, env);
    
    #if 1
    decrypted_node = (axiom_node_t*)oxs_axiom_deserialize_node(env, decrypted_data);
   
    /*Remove enc_node*/  
    AXIOM_NODE_DETACH(enc_data_node, env);     

    /*Replace with decrypted node*/
    ret = AXIOM_NODE_ADD_CHILD(parent_of_enc_node, env, decrypted_node);
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                 "Attaching decrypted node failed");

        return AXIS2_FAILURE;
    }
    #endif
    return ret;
}
