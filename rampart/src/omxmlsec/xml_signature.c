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
#include <axis2_util.h>
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_buffer.h>
#include <oxs_cipher.h>
#include <openssl_rsa.h>
#include <openssl_digest.h>
#include <oxs_sign_ctx.h>
#include <oxs_sign_part.h>
#include <oxs_xml_signature.h>
#include <oxs_signature.h>
#include <oxs_token_ds_reference.h>
#include <oxs_token_digest_method.h>
#include <oxs_token_digest_value.h>
#include <oxs_token_transforms.h>
#include <oxs_token_transform.h>
#include <oxs_token_c14n_method.h>
#include <oxs_token_signature.h>
#include <oxs_token_signature_method.h>
#include <oxs_token_signature_value.h>
#include <oxs_token_signed_info.h>
/*Private functions*/


/*parent is ds:SignedInfo*/
static axis2_status_t
oxs_xml_sig_build_reference(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_sign_part_t *sign_part)
{
    axis2_char_t *uri = NULL; 
    axis2_char_t *serialized_node = NULL; 
    axis2_char_t *digest = NULL; 
    axis2_char_t *digest_mtd = NULL; 
    axis2_array_list_t *transforms = NULL;
    axiom_node_t *node = NULL;
    axiom_node_t *reference_node = NULL;
    axiom_node_t *digest_value_node = NULL;
    axiom_node_t *digest_mtd_node = NULL;
    int i=0;

    /*Get the node to digest*/
    node = oxs_sign_part_get_node(sign_part, env);

    reference_node = oxs_token_build_ds_reference_element(env, parent ,NULL, uri, NULL);

    /*Get transforms if any*/
    transforms = oxs_sign_part_get_transforms(sign_part, env);
    
    if((transforms) && (0 < AXIS2_ARRAY_LIST_SIZE(transforms, env))){
        axiom_node_t *transforms_node = NULL;
        /*Add ds:Transforms element*/
        transforms_node = oxs_token_build_transforms_element(env, reference_node);
        /*LOOP: Apply transforms. This usually C14N*/
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(transforms, env); i++){
            /*Apply transform*/

            /*Add to ds:Transforms*/
        }
    }
    /*Serialize node*/
    serialized_node = AXIOM_NODE_TO_STRING(node, env);
    printf("serialized_node %s\n", serialized_node);
    /*Make digest.*/
    digest_mtd = oxs_sign_part_get_digest_mtd(sign_part, env);
    digest = openssl_sha1(env, serialized_node, axis2_strlen(serialized_node)); 

    /*Construct nodes*/
    digest_mtd_node = oxs_token_build_digest_method_element(env, reference_node, digest_mtd);
    digest_value_node = oxs_token_build_digest_value_element(env, reference_node, digest);
    
    return AXIS2_SUCCESS; 
}
/**
 *  C14N -> Serialize -> Sign the <SignedInfo> element
 */
static axis2_status_t
oxs_xml_sig_sign_signed_info(const axis2_env_t *env,
    axiom_node_t *signature_node,
    axiom_node_t *signed_info_node,
    oxs_sign_ctx_t *sign_ctx)
{
    axis2_char_t *signature_val = "FAKE_SIG_VAL(734dwe93721fd8y2==";
    axis2_char_t *serialized_signed_info = NULL;
    axis2_char_t *c14n_algo = NULL;
    axiom_node_t *signature_val_node = NULL;
    oxs_buffer_t *input_buf = NULL;
    oxs_buffer_t *output_buf = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    /*TODO : Cannonicalize <SignedInfo>*/
    c14n_algo = oxs_sign_ctx_get_c14n_mtd(sign_ctx, env);

    /*Then serialize <SignedInfo>*/
    serialized_signed_info = AXIOM_NODE_TO_STRING(signed_info_node, env);
    printf("serialized_signed_info %s\n",serialized_signed_info); 

    /*Make the input and out put buffers*/
    input_buf = oxs_buffer_create(env);
    output_buf = oxs_buffer_create(env);

    OXS_BUFFER_POPULATE(input_buf, env, (unsigned char *)serialized_signed_info, axis2_strlen(serialized_signed_info));
    /*Then sign... NOTE: The signature process includes making the digest. e.g. rsa-sha1 => RSA(SHA-1(contents))*/ 
    status = oxs_sig_sign(env, sign_ctx, input_buf, output_buf);

    /*Sign the data using the private key*/
    signature_val = "MC0CFFrVLtRlk=";
    
    /*Construct <SignatureValue>*/
    signature_val_node = oxs_token_build_signature_value_element(env, signature_node, signature_val);

    return AXIS2_SUCCESS;
}

/*Public functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_xml_sig_sign(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    axiom_node_t *parent)
{
    axiom_node_t *signed_info_node = NULL;
    axiom_node_t *signature_node = NULL;
    axiom_node_t *signature_mtd_node = NULL;
    axiom_node_t *c14n_mtd_node = NULL;
    axis2_char_t *sign_algo = NULL;
    axis2_char_t *c14n_algo = NULL;
    axis2_array_list_t *sign_parts = NULL;
    int i=0;

    /*Construct the <Signature> element*/
    signature_node = oxs_token_build_signature_element(env, parent, "Sign-ID");

    /*Construct the <SignedInfo>  */
    signed_info_node = oxs_token_build_signed_info_element(env, signature_node);

    /*Construct the <SignatureMethod>  */
    sign_algo = oxs_sign_ctx_get_sign_mtd_algo(sign_ctx, env);
    signature_mtd_node = oxs_token_build_signature_method_element(env, signed_info_node, sign_algo);

    /*Construct the <CanonicalizationMethod> */
    c14n_algo = oxs_sign_ctx_get_c14n_mtd(sign_ctx, env);
    c14n_mtd_node = oxs_token_build_c14n_method_element(env, signed_info_node, c14n_algo);

    /*Look for signature parts*/
    sign_parts = oxs_sign_ctx_get_sign_parts(sign_ctx , env);

    /*For each and every signature part in sig ctx,*/
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(sign_parts, env); i++){
        oxs_sign_part_t *sign_part =  NULL;

        /*TODO Get ith sign_part*/
        sign_part = (oxs_sign_part_t*)axis2_array_list_get(sign_parts, env, i);
        /*Create <ds:Reference> elements */
        oxs_xml_sig_build_reference(env, signed_info_node, sign_part);

    }
    /*At this point we have a complete <SignedInfo> node. Now we need to sign it*/
    oxs_xml_sig_sign_signed_info(env, signature_node, signed_info_node, sign_ctx); 

    
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_xml_sig_verify(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx,
    axiom_node_t *signature_node)
{
    return AXIS2_SUCCESS;
}
