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

#include <axis2_qname.h>
#include <axiom_namespace.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_token_encrypted_data.h>
#include <oxs_token_encrypted_key.h>
#include <oxs_token_encryption_method.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_enc_engine.h>
#include <oxs_cipher.h>
#include <openssl_cipher_ctx.h>
#include <openssl_crypt.h>
#include <openssl_constants.h>
#include <openssl_rsa.h>

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_get_encrypted_key(const axis2_env_t *env,
                            axiom_node_t *enc_key_node,
                            oxs_key_t *session_key)
{
    axis2_char_t *key_enc_algo = NULL, *encrypted_key_value = NULL;
    axiom_node_t *enc_method_node = NULL, *cd_node = NULL, *cv_node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    oxs_buffer_ptr encrypted_key_buf = NULL, decrypted_key_buf = NULL;
    /*Verify*/
    if(!enc_key_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
            "Passed encrypted key is NULL");
        return AXIS2_FAILURE;
    }

    enc_method_node = oxs_axiom_get_first_child_node_by_name(env, enc_key_node, OXS_NodeEncryptionMethod, NULL, NULL);
    if(!enc_method_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
            "Cannot find EncryptionMethodElement");
        return AXIS2_FAILURE;
    }

    key_enc_algo =  oxs_token_get_encryption_method(env, enc_method_node);
    if(!key_enc_algo){
        /*If not found use default*/
        key_enc_algo = OXS_DEFAULT_KT_ALGO_HREF;
    }

    cd_node = oxs_axiom_get_first_child_node_by_name(env, enc_key_node, OXS_NodeCipherData, NULL, NULL);
    if(!cd_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
            "Cannot find CipherData element");
        return AXIS2_FAILURE;
    }

    cv_node = oxs_axiom_get_first_child_node_by_name(env, cd_node, OXS_NodeCipherValue, NULL, NULL);
    if(!cv_node){
        oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
            "Cannot find CipherValue element");
        return AXIS2_FAILURE;
    }
    /*Encrypted key*/
    encrypted_key_value = (axis2_char_t*)oxs_token_get_cipher_value(env, cv_node);

    /*Create buffers for decryption*/
    encrypted_key_buf = oxs_create_buffer(env, AXIS2_STRLEN(encrypted_key_value));
    encrypted_key_buf->data = (unsigned char *)encrypted_key_value;
    decrypted_key_buf = oxs_create_buffer(env, OXS_BUFFER_INITIAL_SIZE);

    /*Decrypt the encrypted key*/
    status  = oxs_prvkey_decrypt_data(env, encrypted_key_buf, decrypted_key_buf, OXS_KEY_GET_NAME(session_key, env));  
    if(status == AXIS2_FAILURE){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_prvkey_decrypt_data failed");
        return AXIS2_FAILURE;
    }
    /*Create the session key*/
    /*Trim data to the key size*/
    OXS_KEY_SET_DATA(session_key, env,(axis2_char_t*) decrypted_key_buf->data);
    OXS_KEY_SET_SIZE(session_key, env, decrypted_key_buf->size);
    OXS_KEY_SET_USAGE(session_key, env, OXS_KEY_USAGE_DECRYPT);
    /*
    session_key->data = AXIS2_STRMEMDUP(decrypted_key_buf->data, decrypted_key_buf->size, env);    
    session_key->size = decrypted_key_buf->size;
    session_key->usage = OXS_KEY_USAGE_DECRYPT;
    */
    /*printf("\n>>>>>>>>decrypted session_key %s\n", session_key->data);*/
    return AXIS2_SUCCESS;
}

/*Decrypt data using the private key*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_prvkey_decrypt_data(const axis2_env_t *env, oxs_buffer_ptr input, oxs_buffer_ptr result, axis2_char_t *filename)
{
    evp_pkey_ptr prvk = NULL;
    axis2_char_t  *decoded_encrypted_str = NULL;    
    unsigned char *decrypted  =  NULL;
    int ret, declen;

    /*First do base64 decode*/
    decoded_encrypted_str = AXIS2_MALLOC(env->allocator, axis2_base64_decode_len( (char*)(input->data)));
    ret = axis2_base64_decode(decoded_encrypted_str, (char*)(input->data));
    

    /*Load the private _key*/
    prvk = evp_pkey_load(env, filename, "");
    if(!prvk){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "cannot load the private key from the file %s", filename);
         return AXIS2_FAILURE;

    }
    
    /*Now we support only rsa*/
    declen = openssl_rsa_prv_decrypt(env, prvk, (unsigned char *)decoded_encrypted_str, &decrypted);
    if(declen < 0 ){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "decryption failed");
         return AXIS2_FAILURE;

    }

    result->data = decrypted;
    result->size = declen;
    
    return AXIS2_SUCCESS;
}


/*TODO better to have pk_ctx instead of individual parameters like filename, algorithm*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_pubkey_encrypt_data(const axis2_env_t *env, oxs_buffer_ptr input, oxs_buffer_ptr result, axis2_char_t *filename )
{
    evp_pkey_ptr pubk = NULL; 
    axis2_char_t *encoded_str = NULL;
    unsigned char *encrypted  =  NULL;
    int ret, enclen, encodedlen ;

    /*Load the public key. Note: evp_pkey_load first try to load the private key. So need to provide the correct key*/
    pubk = evp_pkey_load(env, filename, "");   
    if(!pubk){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "cannot load the public key from the file %s", filename);
         return AXIS2_FAILURE;
        
    }
    /*Now we support only rsa*/
    ret = openssl_rsa_pub_encrypt(env, pubk, input->data, &encrypted);
    if(ret < 0 ){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_rsa_pub_encrypt");
         return AXIS2_FAILURE;

    }
    enclen = ret; /*return value is the length of the encrypted data*/
    /*base64 encode*/
    encodedlen = axis2_base64_encode_len(enclen);
    encoded_str = AXIS2_MALLOC(env->allocator, encodedlen );

    ret = axis2_base64_encode(encoded_str, (const char *)encrypted, enclen);
    if(ret < 0){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "axis2_base64_encode");
            return AXIS2_FAILURE;
    }

    /*Attach the result to the result buf*/
    result->size = encodedlen;
    result->data = (unsigned char*)AXIS2_STRDUP(encoded_str, env);

    return AXIS2_SUCCESS;
}


/*Encrypt or decrypt an input depending on what is set in the enc_ctx*/
/*TODO Default IV*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_crypt(const axis2_env_t *env, 
                enc_ctx_ptr enc_ctx,
                oxs_buffer_ptr input,
                oxs_buffer_ptr result)
{
    unsigned char *out_main_buf = NULL;
    openssl_evp_block_cipher_ctx_ptr bc_ctx = NULL;
    axis2_char_t *iv = NULL;   
    axis2_char_t *cipher_name =  NULL;   
    axis2_char_t *encoded_str=NULL;
    axis2_char_t *in_data = NULL;
    int ret, enclen, encodedlen, decoded_len;
    axis2_char_t *temp = NULL;
    openssl_cipher_property_t *cprop = NULL;

    /*Initializations*/
    ret = AXIS2_FAILURE;
    enclen = -1;
    encodedlen = -1;   

 
    /*Create the context*/
    bc_ctx = openssl_evp_block_cipher_ctx_create(env);
    if(!bc_ctx){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_evp_block_cipher_ctx_create failed");
         return AXIS2_FAILURE;
    }

    /*Get cipher property*/    
    cprop =  oxs_get_cipher_property_for_url(env, enc_ctx->encmtd_algorithm);    

    /*Set the IV*/   
    /*iv = OPENSSL_DEFAULT_IV16;*/ /*oxs_iv_generate_for_algo(env,  enc_ctx->encmtd_algorithm); */
    iv =(axis2_char_t*)oxs_iv_generate_for_algo(env,  enc_ctx->encmtd_algorithm); 

    /*Set the key*/
    temp =  AXIS2_STRNDUP(OXS_KEY_GET_DATA(enc_ctx->key, env),  OPENSSL_CIPHER_PROPERTY_GET_KEY_SIZE(cprop, env), env);
    bc_ctx->key = AXIS2_STRNDUP(OXS_KEY_GET_DATA(enc_ctx->key, env),OPENSSL_CIPHER_PROPERTY_GET_KEY_SIZE(cprop, env),  env);
    bc_ctx->key_initialized = 1;

    /*Set the IV*/
    bc_ctx->iv = AXIS2_STRNDUP(iv, OPENSSL_CIPHER_PROPERTY_GET_IV_SIZE(cprop, env), env);

    /*TODO: Get the cipher name */
    cipher_name = (axis2_char_t*)OPENSSL_CIPHER_PROPERTY_GET_NAME(cprop, env);
    if(!cipher_name){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_get_cipher failed");
        
        return AXIS2_FAILURE;
    } 

    /*Initialize block cipher ctx*/
    if(enc_ctx->operation == oxs_operation_encrypt){
        ret =  openssl_evp_block_cipher_ctx_init(env, bc_ctx,
                            OPENSSL_ENCRYPT, (const unsigned char*)cipher_name);
        if(ret == AXIS2_FAILURE ){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_evp_block_cipher_ctx_init failed");
        }
    }else if(enc_ctx->operation == oxs_operation_decrypt){
        ret =  openssl_evp_block_cipher_ctx_init(env, bc_ctx,
                            OPENSSL_DECRYPT, (const unsigned char*)cipher_name);
        if(ret == AXIS2_FAILURE ){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_evp_block_cipher_ctx_init failed");
        }
    }else{
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "Invalid operation type %d", enc_ctx->operation);
        return AXIS2_FAILURE;
    }

    /****************Encryption or decryption happens here ************/

    /*If this is to encrypt we simply pass the data to crypto function*/
    if(enc_ctx->operation == oxs_operation_encrypt){
        enclen = openssl_block_cipher_crypt(env, bc_ctx,
                                         input->data,  &out_main_buf, OPENSSL_ENCRYPT);
    
    /*If this is to decrypt, then we need to base64decode first*/
    }else if(enc_ctx->operation == oxs_operation_decrypt){
        in_data = AXIS2_MALLOC(env->allocator, axis2_base64_decode_len( (char*)(enc_ctx->inputdata)));
        /*axis2_base64_decode(in_data, (char*)(enc_ctx->inputdata)); */
        decoded_len = axis2_base64_decode_binary((unsigned char*)in_data, (char*)(enc_ctx->inputdata));
        if(decoded_len < 0){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "base64 decoding failed");
        }
        enclen = openssl_block_cipher_crypt(env, bc_ctx,
                                         (unsigned char*)in_data,  &out_main_buf, OPENSSL_DECRYPT);
    }else{
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "Invalid operation type %d", enc_ctx->operation);
        return AXIS2_FAILURE;

    }

    if(enclen < 0){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_block_cipher_crypt failed");
         return AXIS2_FAILURE;
    }
   
#if 0 
    FILE *outf;
    outf = fopen("outbuf", "wb");
    fwrite(out_main_buf, 1, enclen, outf);
#endif
    
    
    /*If the operation is to encrypt we will encode the encrypted data*/
    if(enc_ctx->operation == oxs_operation_encrypt){
        encodedlen = axis2_base64_encode_len(enclen);
        encoded_str = AXIS2_MALLOC(env->allocator, encodedlen );
      
        /*out_main_buf[enclen] ="\0";*/ /*Null terminate ??? Prob???*/
        ret = axis2_base64_encode(encoded_str, (const char *)out_main_buf, enclen);
        if(ret < 0){
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "axis2_base64_encode");
            return AXIS2_FAILURE;
        }
    
        /*Attach the result to the result buf*/    
        result->size = encodedlen;
        result->data = (unsigned char*)AXIS2_STRDUP(encoded_str, env);
    }else if(enc_ctx->operation == oxs_operation_decrypt){
        result->size = enclen;
        result->data = AXIS2_STRMEMDUP(out_main_buf, enclen, env);
    
    }else{
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "Invalid operation type %d", enc_ctx->operation);
        return AXIS2_FAILURE;

    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_populate_cipher_value(const axis2_env_t *env,
                             axiom_node_t* template_node,
                             oxs_buffer_ptr databuf)
{
    axis2_status_t ret = AXIS2_FAILURE;
    axiom_element_t *template_ele = NULL, *cv_ele = NULL, *cd_ele = NULL;
    axiom_node_t *cv_node = NULL, *cd_node = NULL;
    axis2_qname_t *qname = NULL;

    template_ele = AXIOM_NODE_GET_DATA_ELEMENT(template_node, env);
    
    if(!template_ele){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "Cannot retriew  element from the template given");
        return AXIS2_FAILURE;
    }

    /*Get CipherData*/
    qname = axis2_qname_create(env, OXS_NodeCipherData, NULL, NULL);
    cd_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(template_ele, env, qname, template_node, &cd_node);
    
    if(!cd_ele){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "Cannot retriew CipherData element from %s", AXIOM_ELEMENT_GET_LOCALNAME(template_ele, env));
        return AXIS2_FAILURE;
    }

    AXIS2_QNAME_FREE(qname, env);
    qname= NULL;

    /*Get CipherValue*/
    qname = axis2_qname_create(env, OXS_NodeCipherValue, NULL, NULL);
    cv_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(cd_ele, env, qname, cd_node, &cv_node);
   
    if(!cv_ele){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "Cannot retriew CipherValue element from %s", AXIOM_ELEMENT_GET_LOCALNAME(cd_ele, env));
        return AXIS2_FAILURE;

    }
    

    ret =  AXIOM_ELEMENT_SET_TEXT(cv_ele, env, (axis2_char_t *)databuf->data , cv_node); 
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "Cannot set data to the CipherValue element");
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}
/*We expect user to provide a template as below*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_decrypt_template(const axis2_env_t *env,
                        axiom_node_t* template_node,
                        axis2_char_t** decrypted_data,
                        enc_ctx_ptr enc_ctx
                        )
{
    axis2_status_t  ret =  AXIS2_FAILURE;
    oxs_buffer_ptr input = NULL;
    oxs_buffer_ptr result = NULL;

    /*Populate enc_ctx*/
    enc_ctx->operation = oxs_operation_decrypt;
    enc_ctx->mode = enc_ctx_mode_encrypted_data;

    ret = oxs_enc_encryption_data_node_read(env, enc_ctx, template_node);
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "reading encrypted data failed");
        return ret;
    }

    /*Now look for data to be decrypted*/
    input = oxs_create_buffer(env, OXS_BUFFER_INITIAL_SIZE);
    input->data = (unsigned char *)enc_ctx->inputdata;
    input->size = AXIS2_STRLEN(enc_ctx->inputdata);

    /*Initialize the result buffer*/
    result = oxs_create_buffer(env, OXS_BUFFER_INITIAL_SIZE);

    ret = oxs_enc_crypt(env, enc_ctx, input,  result ); 
    if(ret != AXIS2_SUCCESS){
           oxs_error(ERROR_LOCATION, OXS_ERROR_DECRYPT_FAILED,
                     "oxs_enc_decrypt failed");
        return ret;
    }

    *decrypted_data =   AXIS2_MALLOC(env->allocator,result->size); 
    *decrypted_data =  (axis2_char_t*)result->data;
    
    return ret;
}

/*We expect user to provide a template as below*/
/**/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encrypt_template(const axis2_env_t *env,
                        axiom_node_t* template_node, 
                        axis2_char_t* data,
                        enc_ctx_ptr enc_ctx
                        )
{
    axis2_status_t  ret =  AXIS2_FAILURE;
    oxs_buffer_ptr input = NULL;
    oxs_buffer_ptr result = NULL;
   
       
    /*Populate enc_ctx*/
    enc_ctx->operation = oxs_operation_encrypt;
    enc_ctx->mode = enc_ctx_mode_encrypted_data;
     
    ret = oxs_enc_encryption_data_node_read(env, enc_ctx, template_node);
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_block_cipher_crypt failed");    
        return ret;
    }
    
    /*We've populated the context*/
    
    /*Create the input buffer*/
    input = oxs_string_to_buffer(env, data);
    result = oxs_create_buffer(env, OXS_BUFFER_INITIAL_SIZE);
     
    ret = oxs_enc_crypt(env, enc_ctx, input, result ); 
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_enc_encrypt failed");    
        return ret;
    }
     
    ret = oxs_enc_populate_cipher_value (env, template_node, result);
   
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_enc_populate_cipher_value failed");
        return ret;
    } 
    
    return AXIS2_SUCCESS;
    
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_cipher_data_node_read(const axis2_env_t *env, 
                                enc_ctx_ptr enc_ctx, axiom_node_t* node_cipher_data)
{
    axiom_node_t* cur = NULL;
    axiom_element_t *ele = NULL;
    axis2_char_t *data = NULL;
    /*We've a cipher data node here.
     The child element is either a CipherReference or a CipherValue element*/

    cur = AXIOM_NODE_GET_FIRST_CHILD(node_cipher_data, env); 
    
    if(!cur){        
        
        return AXIS2_FAILURE;
    }

    if( oxs_axiom_check_node_name(env, cur, OXS_NodeCipherValue, OXS_EncNs )){
        /*Got a cipher value*/
        if(enc_ctx->operation == oxs_operation_decrypt)
        {  
            enc_ctx->cipher_value_node = cur;
            ele = AXIOM_NODE_GET_DATA_ELEMENT(cur, env);
            /*Read data from the CipherValue node and set it to the context*/
            data = AXIOM_ELEMENT_GET_TEXT(ele, env, cur);
            enc_ctx->inputdata = AXIS2_STRDUP(data, env);
        } 
    }else if(oxs_axiom_check_node_name(env, cur, OXS_NodeCipherReference, OXS_EncNs )){
        /*Got a cipher reference*/
        /*TODO : Support Cipher references*/
        /*Check cipher reference and set it to enc_ctx->cipher_value_node*/
    }

    return AXIS2_SUCCESS;    
}
/**
* We have EncryptedData of EncryptedKey node by now. 
* So process it and populate the ctx.
*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encryption_data_node_read(const axis2_env_t *env,
            enc_ctx_ptr enc_ctx, axiom_node_t* node)
{
    axiom_node_t* cur = NULL;
    axiom_element_t *ele = NULL;
    int ret;
    
    /* Operation is either encrypt or decrypt */
    if(!((enc_ctx->operation == oxs_operation_encrypt) || (enc_ctx->operation == oxs_operation_decrypt))) return (-1);
    
    /*Check which mode we are in.
      1. Encrypted Data
      2. Encrypted Key
    */
    
    switch(enc_ctx->mode) {
    case enc_ctx_mode_encrypted_data:
        if(!(oxs_axiom_check_node_name(env, node, OXS_NodeEncryptedData, OXS_EncNs )))
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "No %s node found",OXS_NodeEncryptedData );
            return AXIS2_FAILURE;
        }
        break;
    case enc_ctx_mode_encrypted_key:
        if(!(oxs_axiom_check_node_name(env, node, OXS_NodeEncryptedKey, OXS_EncNs)))
        {
            oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "No %s node found", OXS_NodeEncryptedKey);
             return AXIS2_FAILURE;
        }
        break;
    }
    
    enc_ctx->enc_data_node = node;
    
    /*Element is either Encrypted Data or Encrypted Key*/ 
    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    if(!ele){
         oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "No element found");
         return AXIS2_FAILURE;
    }
    
    /*Populate ctx with id, type, mime and encoding types if found*/
    enc_ctx->id = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrId);
    enc_ctx->type = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrType);
    enc_ctx->mime_type = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrMimeType);
    enc_ctx->encoding = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrEncoding);
    if(enc_ctx->mode == enc_ctx_mode_encrypted_key) {
        enc_ctx->recipient = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrRecipient);
    }
    
    cur = AXIOM_NODE_GET_FIRST_CHILD(node, env);

    /* first node is optional EncryptionMethod, we'll read it later */
    if((cur != NULL) && (oxs_axiom_check_node_name(env, cur, OXS_NodeEncryptionMethod, OXS_EncNs))) {
        enc_ctx->enc_method_node = cur;
        cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);
    }

    /* next node is optional KeyInfo, we'll process it later */
    if((cur != NULL) && (  oxs_axiom_check_node_name(env, cur, OXS_NodeKeyInfo, OXS_DSigNs))) {
        enc_ctx->key_info_node = cur;
        cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);
    }

    /* next is required CipherData node */
    if((cur == NULL) || (!oxs_axiom_check_node_name(env, cur, OXS_NodeCipherData, OXS_EncNs))) {
        return AXIS2_FAILURE;
    }

    ret = oxs_enc_cipher_data_node_read(env, enc_ctx, cur);
    if(ret < 0) {
        return AXIS2_FAILURE;
    }
    
       
    /* now read the encryption method node */
    if(enc_ctx->enc_method_node != NULL) {
        ele = AXIOM_NODE_GET_DATA_ELEMENT(enc_ctx->enc_method_node, env);
        enc_ctx->encmtd_algorithm = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrAlgorithm);
    }
      
    /*If you need to populate the context more this is the place*/   

    return AXIS2_SUCCESS;

}





