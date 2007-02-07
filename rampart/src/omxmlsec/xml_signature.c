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
#include <oxs_sign_ctx.h>
#include <oxs_sign_part.h>
#include <oxs_token_ds_reference.h>
#include <oxs_token_digest_method.h>
#include <oxs_token_digest_value.h>
#include <oxs_token_transforms.h>
#include <oxs_token_transform.h>
/*Private functions*/

/*parent is ds:SignedInfo*/
static axis2_status_t
rampart_xml_sig_build_reference(const axis2_env_t *env,
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
    if(0 < AXIS2_ARRAY_LIST_SIZE(transforms, env)){
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

    /*Make digest.*/
    digest_mtd = oxs_sign_part_get_digest_mtd(sign_part, env);
    digest = "j6lwx3rvEPO0vKtMup4NbeVu8nk=";/*TODO : Hard coded*/ 

    /*Construct nodes*/
    digest_mtd_node = oxs_token_build_digest_method_element(env, reference_node, digest_mtd);
    digest_value_node = oxs_token_build_digest_value_element(env, reference_node, digest);

    return AXIS2_SUCCESS; 
}

/*Public functions*/
AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_xml_sig_sign(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx)
{
    axiom_node_t *signed_info_node = NULL;
    axis2_array_list_t *sign_parts = NULL;
    int i=0;

    /*Get the signature context*/

    /*Look for signature parts*/
    sign_parts = oxs_sign_ctx_get_sign_parts(sign_ctx , env);

    /*For each and every signature part in sig ctx,*/
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(sign_parts, env); i++){
        oxs_sign_part_t *sign_part =  NULL;
        /*TODO Get ith sign_part*/
        
        /*Create <ds:Reference> elements */
        rampart_xml_sig_build_reference(env, signed_info_node, sign_part);

    }

    /*Then construct the <SignatureMethod> , <CanonicalizationMethod> */

    /*Finalize <SignedInfo>*/

    /*Then serialize <SignedInfo>*/

    /*Sign the data using the private key*/

    /*Construct the <Signature> element*/
    
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
oxs_xml_sig_verify(const axis2_env_t *env,
    oxs_sign_ctx_t *sign_ctx)
{
    return AXIS2_SUCCESS;
}
