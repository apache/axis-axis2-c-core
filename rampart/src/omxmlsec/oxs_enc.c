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
#include <stdio.h>
#include <axis2_util.h>
#include <omxmlsec/oxs_constants.h>
#include <omxmlsec/oxs_ctx.h>
#include <omxmlsec/oxs_enc.h>

/************** Function Headers ****************************/
/*
AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_xml_encrypt(const axis2_env_t *env,
                        enc_ctx_t* ctx,
                        axiom_node_t* tmpl,
                        axiom_node_t* node
                    );

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_binary_encrypt(const axis2_env_t *env,
                        enc_ctx_t* ctx,
                        axiom_node_t* tmpl,
                        axis2_char_t* data
                    );


AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_populate_ctx(const axis2_env_t *env,
                    enc_ctx_t* ctx,
                    axiom_node_t* tmpl
                    );

*/

/************* End of function headers **********************/



AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_binary_encrypt(const axis2_env_t *env,
                        enc_ctx_t* ctx,
                        axiom_node_t* tmpl,
                        axis2_char_t* data)
{
    axis2_status_t temp_status = AXIS2_FAILURE;
    if(!ctx) return AXIS2_FAILURE;
    if(!tmpl) return AXIS2_FAILURE;
    
    ctx->operation = enc_ctx_transform_operation_encrypt;

    temp_status = oxs_enc_populate_ctx(env, ctx, tmpl);    

    if(temp_status){
        printf("oxs_enc_populate_ctx SUCCESS");
        return AXIS2_SUCCESS;
    }else{
        printf("oxs_enc_populate_ctx FAILURE");
        return AXIS2_FAILURE;
    }
    
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_xml_encrypt(const axis2_env_t *env,
                        enc_ctx_t* ctx,
                        axiom_node_t* tmpl,
                        axiom_node_t* node)
{

    return AXIS2_SUCCESS;

}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
oxs_enc_populate_ctx(const axis2_env_t *env,
                    enc_ctx_t* ctx,
                    axiom_node_t* tmpl_node)
{
    axiom_element_t *enc_data_ele = NULL, *enc_method_ele = NULL, *key_info_ele = NULL ;
    axiom_element_t *cipher_data_ele = NULL, *cipher_val_ele = NULL ;
    axiom_node_t *enc_data_node = NULL, *enc_method_node = NULL, *key_info_node = NULL ;
    axiom_node_t *cipher_data_node = NULL, *cipher_val_node = NULL ;
    axis2_qname_t *qname = NULL;
 
    if(!ctx) return AXIS2_FAILURE;    
    if(!tmpl_node) return AXIS2_FAILURE;    
        
    enc_data_node = tmpl_node;
    
    /*Traverse the template and populate the context*/    
    enc_data_ele = AXIOM_NODE_GET_DATA_ELEMENT(enc_data_node , env);
   
    if(!enc_data_ele){
        return AXIS2_FAILURE; 
    }
    /*Populate EncryptionData element attributes */
    ctx->encdata_id = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(enc_data_ele, env, OXS_AttrId);
    ctx->encdata_type = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(enc_data_ele, env, OXS_AttrType);
    ctx->encdata_mime_type = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(enc_data_ele, env, OXS_AttrMimeType);
    ctx->encdata_encoding = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(enc_data_ele, env, OXS_AttrEncoding);
    
    ctx->enc_data_node = enc_data_node;
     
    qname = axis2_qname_create(env, OXS_NodeEncryptionMethod, NULL, NULL);/*EncryptionMethod*/

    enc_method_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(enc_data_ele, env, qname, enc_data_node, enc_method_node);
    if(!enc_method_ele){ printf("\nNo EncryptionMethod element"); return AXIS2_FAILURE; }
   
    ctx->encmtd_algorithm = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(enc_method_ele, env, OXS_AttrAlgorithm);
    ctx->enc_method_node = enc_method_node;

    AXIS2_QNAME_FREE(qname, env);
    qname= NULL;
    qname = axis2_qname_create(env, OXS_NodeKeyInfo, NULL, NULL);/*KeyInfo*/

    key_info_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(enc_data_ele, env, qname, enc_data_node, key_info_node);    
    
    if(key_info_ele)
    {
        ctx->key_info_node = key_info_node;
    }
    
    AXIS2_QNAME_FREE(qname, env);
    qname= NULL;
    qname = axis2_qname_create(env, OXS_NodeCipherData, NULL, NULL);/*CipherData*/
    cipher_data_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(enc_data_ele, env, qname,  enc_data_node, cipher_data_node);

    if(cipher_data_ele)
    {
        AXIS2_QNAME_FREE(qname, env);
        qname= NULL;
        qname = axis2_qname_create(env, OXS_NodeCipherValue, NULL, NULL);/*CipherValue*/
        cipher_val_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(cipher_data_ele, env, qname, cipher_data_node, cipher_val_node);
        if(cipher_val_ele){
            printf("\nCipherValue Element found");
            ctx->cipher_value_node = cipher_val_node;
        }
    }

 
    return AXIS2_SUCCESS;

}



