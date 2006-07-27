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
#include <stdio.h>
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_ctx.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_enc_engine.h>
#include <openssl_cipher_ctx.h>
#include <openssl_crypt.h>
#include <openssl_constants.h>


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encrypt(const axis2_env_t *env, 
                enc_ctx_ptr enc_ctx,
                oxs_buffer_ptr input,
                axis2_char_t* key,
                oxs_buffer_ptr result)
{
    unsigned char *out_main_buf;
    openssl_evp_block_cipher_ctx_ptr bc_ctx = NULL;
    axis2_char_t* iv =  "12345678";   
    int ret;
 
    bc_ctx = openssl_evp_block_cipher_ctx_create(env);
    if(!bc_ctx) return (-1);
    
    /*Set the key*/
    bc_ctx->key = AXIS2_STRDUP(key, env);
    bc_ctx->key_initialized = 1;
    /*Set the IV*/
    bc_ctx->iv =  AXIS2_STRDUP(iv, env);

    /*TODO: Get the cipher by giving the algoritm attribute */
    ret =  openssl_evp_block_cipher_ctx_init(env, bc_ctx,
                            OPENSSL_ENCRYPT, (unsigned char*)OPENSSL_EVP_aes_128_cbc);
    
    if(ret < 0) return AXIS2_FAILURE;
    ret = openssl_block_cipher_crypt(env, bc_ctx,
                                         input->data,  &out_main_buf, OPENSSL_ENCRYPT);
    if(ret < 0) return AXIS2_FAILURE;
   
#if 0 
    FILE *outf;
    outf = fopen("outbuf", "wb");
    fwrite(out_main_buf, 1, ret, outf);
#endif
    
    oxs_buffer_set_size(env, result, ret);
    result->data = out_main_buf;
    
    return AXIS2_SUCCESS;
}

/*We expect user to provide a template as below*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encrypt_template(const axis2_env_t *env,
                        axiom_node_t* template, 
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
    
    ret = oxs_enc_cipher_data_node_read(env, enc_ctx, template);
    if(ret != AXIS2_SUCCESS){
        return ret;
    }
    
    /*We've populated the context*/
    
    /*Create the input buffer*/
    input = oxs_string_to_buffer(env, data);
    
    result = oxs_create_buffer(env, ret);
    
    /*TODO: Obtain the key*/
    key = "0123456789012345";
    key = enc_ctx->key->data;

    oxs_enc_encrypt(env, enc_ctx, input, key, result ); 
    
    printf("Encrypted Result = %s\n", oxs_buffer_to_string(env, result));
     
    return ret;
    
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_cipher_data_node_read(const axis2_env_t *env, 
                                enc_ctx_ptr enc_ctx, axiom_node_t* node)
{
    axiom_node_t* cur = NULL;
    axiom_element_t *ele = NULL;
    int ret;
    /*We've a cipher data node here.
     The child element is either a CipherReference or a CipherValue element*/

    cur = AXIOM_NODE_GET_FIRST_CHILD(node, env); 
    if(!cur){        
        return AXIS2_FAILURE;
    }
    if( oxs_axiom_check_node_name(env, cur, OXS_NodeCipherValue, OXS_EncNs )){
        /*Got a cipher value*/
        if(enc_ctx->operation == oxs_operation_decrypt)
        {   
            enc_ctx->cipher_value_node = cur;
        } 
    }else if(oxs_axiom_check_node_name(env, cur, OXS_NodeCipherReference, OXS_EncNs )){
        /*Got a cipher reference*/
        /*TODO : Support Cipher references*/
    }

    return AXIS2_SUCCESS;    
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_encryption_data_node_read(const axis2_env_t *env,
            enc_ctx_ptr enc_ctx, axiom_node_t* node)
{
    axiom_node_t* cur = NULL;
    axiom_element_t *ele = NULL;
    int ret;

    if(!(enc_ctx->operation == oxs_operation_encrypt) || (enc_ctx->operation == oxs_operation_decrypt)) return (-1);

    switch(enc_ctx->mode) {
    case enc_ctx_mode_encrypted_data:
        if(!(oxs_axiom_check_node_name(env, node, OXS_NodeEncryptedData, OXS_EncNs )))
        {
            return AXIS2_FAILURE;
        }
        break;
    case enc_ctx_mode_encrypted_key:
        if(!(oxs_axiom_check_node_name(env, node, OXS_NodeEncryptedKey, OXS_EncNs)))
        {
             return AXIS2_FAILURE;
        }
        break;
    }

    
    ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
    if(!ele) return AXIS2_FAILURE;

    enc_ctx->id = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrId);
    enc_ctx->type = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrType);
    enc_ctx->mime_type = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrMimeType);
    enc_ctx->encoding = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrEncoding);
    if(enc_ctx->mode == enc_ctx_mode_encrypted_key) {
        enc_ctx->recipient = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(ele, env, OXS_AttrRecipient);
    }
    
    cur = AXIOM_NODE_GET_NEXT_SIBLING(node, env);

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

    /*TODO ret = xmlSecEncCtxCipherDataNodeRead(enc_ctx, cur);*/
    ret = oxs_enc_cipher_data_node_read(env, enc_ctx, cur);
    if(ret < 0) {
        return AXIS2_FAILURE;
    }
    cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);

    /* next is optional EncryptionProperties node (we simply ignore it) */
    if((cur != NULL) && (oxs_axiom_check_node_name(env, cur, OXS_NodeEncryptionProperties, OXS_EncNs))) {
        cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);
    }

    /* there are more possible nodes for the <EncryptedKey> node */
    if(enc_ctx->mode == enc_ctx_mode_encrypted_key) {
    /* next is optional ReferenceList node (we simply ignore it) */
        if((cur != NULL) && (oxs_axiom_check_node_name(env, cur, OXS_NodeReferenceList, OXS_EncNs))) {
        cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);
    }

    /* next is optional CarriedKeyName node (we simply ignore it) */
    if((cur != NULL) && (oxs_axiom_check_node_name(env, cur, OXS_NodeCarriedKeyName, OXS_EncNs))) {
        enc_ctx->carriedKeyName = xmlNodeGetContent(cur);
        if(enc_ctx->carriedKeyName == NULL) {
            return AXIS2_FAILURE;
        }
        /* TODO: decode the name? */
        cur = AXIOM_NODE_GET_NEXT_SIBLING(cur, env);
    }
    }

    /* if there is something left than it's an error */
    if( cur != NULL) return AXIS2_FAILURE;

    
    /* now read the encryption method node */
    /*if(enc_ctx->enc_method_node != NULL) {*/
        /*TODO xmlSecTransformCtxNodeRead*/
        /*enc_ctx->encMethod = oxs_transform_ctx_node_read(&(enc_ctx->transformCtx), enc_ctx->encMethodNode,
                        xmlSecTransformUsageEncryptionMethod);
        */
    /*if(enc_ctx->encMethod == NULL) {
        return(-1);
    }
    } else if(enc_ctx->defEncMethodId != xmlSecTransformIdUnknown) {
    enc_ctx->encMethod = xmlSecTransformCtxCreateAndAppend(&(enc_ctx->transformCtx),
                                  enc_ctx->defEncMethodId);
    if(enc_ctx->encMethod == NULL) {
            xmlSecError(XMLSEC_ERRORS_HERE,
            NULL,
            "xmlSecTransformCtxAppend",
            XMLSEC_ERRORS_R_XMLSEC_FAILED,
            XMLSEC_ERRORS_NO_MESSAGE);
        return(-1);
    }
    } else {
    xmlSecError(XMLSEC_ERRORS_HERE,
            NULL,
            NULL,
            XMLSEC_ERRORS_R_INVALID_DATA,
            "encryption method not specified");
    return(-1);
    }
    enc_ctx->encMethod->operation = enc_ctx->operation;

*/

return AXIS2_SUCCESS;

}





