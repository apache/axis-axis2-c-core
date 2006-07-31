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
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_enc_engine.h>
#include <oxs_cipher.h>
#include <openssl_cipher_ctx.h>
#include <openssl_crypt.h>
#include <openssl_constants.h>



/*TODO Default IV*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_crypt(const axis2_env_t *env, 
                enc_ctx_ptr enc_ctx,
                oxs_buffer_ptr input,
                axis2_char_t* key,
                oxs_buffer_ptr result)
{
    unsigned char *out_main_buf = NULL;
    openssl_evp_block_cipher_ctx_ptr bc_ctx = NULL;
    axis2_char_t *iv =  OPENSSL_DEFAULT_IV8;   
    axis2_char_t *cipher_name =  NULL;   
    axis2_char_t *encoded_str=NULL;
    axis2_char_t *in_data = NULL;
    int ret, enclen, encodedlen;

    /*Safety initializations*/
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
    
    /*Set the key*/
    bc_ctx->key = AXIS2_STRDUP(key, env);
    bc_ctx->key_initialized = 1;
    /*Set the IV*/
    bc_ctx->iv =  AXIS2_STRDUP(iv, env);

    /*TODO: Get the cipher by giving the algoritm attribute */
    cipher_name = oxs_get_cipher(env, (unsigned char*)enc_ctx->encmtd_algorithm);
    if(!cipher_name){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_get_cipher failed");
        
        return AXIS2_FAILURE;
    } 

    ret =  openssl_evp_block_cipher_ctx_init(env, bc_ctx,
                            OPENSSL_ENCRYPT, cipher_name);
   
    if(ret < 0){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_evp_block_cipher_ctx_init failed");
        return AXIS2_FAILURE;
    }
    /*If this is to encrypt we simply pass the data to crypto function*/
    if(enc_ctx->operation == oxs_operation_encrypt){
        enclen = openssl_block_cipher_crypt(env, bc_ctx,
                                         input->data,  &out_main_buf, OPENSSL_ENCRYPT);
    
    /*If this is to decrypt, then we need to base64decode first*/
    }else if(enc_ctx->operation == oxs_operation_decrypt){
        in_data = AXIS2_MALLOC(env->allocator, axis2_base64_decode_len( (char*)(enc_ctx->inputdata)));
        axis2_base64_decode(in_data, (char*)(enc_ctx->inputdata)); 
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
        encoded_str = AXIS2_MALLOC(env->allocator, encodedlen);
        ret = axis2_base64_encode(encoded_str, out_main_buf, enclen);
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
        result->data = (unsigned char*)AXIS2_STRDUP(out_main_buf, env);
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
    

    ret =  AXIOM_ELEMENT_SET_TEXT(cv_ele, env, databuf->data , cv_node); 
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
    axis2_char_t *key = NULL;

    /*Populate enc_ctx*/
    enc_ctx->operation = oxs_operation_decrypt;
    enc_ctx->mode = enc_ctx_mode_encrypted_data;

    ret = oxs_enc_encryption_data_node_read(env, enc_ctx, template_node);
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "openssl_block_cipher_crypt failed");
        return ret;
    }

    /*Now look for data to be decrypted*/
    input = enc_ctx->inputdata;
    result = oxs_create_buffer(env, OXS_BUFFER_INITIAL_SIZE);
     
    key = enc_ctx->key->data;

    ret = oxs_enc_crypt(env, enc_ctx, input, key, result ); 
    if(ret != AXIS2_SUCCESS){
           oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_enc_encrypt failed");
        return ret;
    }

    *decrypted_data =  result->data;
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
    axis2_char_t *key = NULL;
   
       
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
     
    key = enc_ctx->key->data;

    ret = oxs_enc_crypt(env, enc_ctx, input, key, result ); 
    if(ret != AXIS2_SUCCESS){
        oxs_error(ERROR_LOCATION, OXS_ERROR_INVALID_DATA,
                     "oxs_enc_encrypt failed");    
        return ret;
    }
     
    oxs_enc_populate_cipher_value (env, template_node, result);
   
     
    return AXIS2_SUCCESS;
    
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_cipher_data_node_read(const axis2_env_t *env, 
                                enc_ctx_ptr enc_ctx, axiom_node_t* node_cipher_data)
{
    axiom_node_t* cur = NULL;
    axiom_element_t *ele = NULL;
    axis2_char_t *data = NULL;
    int ret;
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





