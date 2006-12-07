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
#include <oxs_constants.h>
#include <oxs_error.h>
#include <oxs_token_encryption_method.h>
#include <axiom_attribute.h>
#include <axiom_element.h>


AXIS2_EXTERN axiom_node_t* AXIS2_CALL
oxs_token_build_encryption_method_element(const axis2_env_t *env,
        axiom_node_t *parent,
        axis2_char_t* algorithm
                                         )
{
    axiom_node_t *encryption_method_node = NULL;
    axiom_element_t *encryption_method_ele = NULL;
    axiom_attribute_t *algo_attr = NULL;
    int ret;
    axiom_namespace_t *ns_obj = NULL;

    ns_obj = axiom_namespace_create(env, OXS_ENC_NS,
            OXS_XENC);

    encryption_method_ele = axiom_element_create(env, parent, OXS_NODE_ENCRYPTION_METHOD, ns_obj, &encryption_method_node);
    if (!encryption_method_ele)
    {
        oxs_error(ERROR_LOCATION,
                OXS_ERROR_ELEMENT_FAILED, "Error creating encryption method element");
        return NULL;
    }
    /*Fixed bug. If kt algorithm is NULL then use the default*/
    if (!algorithm)
    {
        algorithm = (axis2_char_t*)OXS_DEFAULT_KT_ALGO_HREF;
    }

    algo_attr =  axiom_attribute_create(env, OXS_ATTR_ALGORITHM, algorithm, NULL);

    ret = AXIOM_ELEMENT_ADD_ATTRIBUTE(encryption_method_ele, env, algo_attr, encryption_method_node);

    return encryption_method_node;

}



AXIS2_EXTERN axis2_char_t *AXIS2_CALL
oxs_token_get_encryption_method(const axis2_env_t *env, axiom_node_t *enc_mtd_node)
{
    axis2_char_t *enc_mtd = NULL;
    axiom_element_t *enc_mtd_ele = NULL;

    if(!enc_mtd_node){
        return NULL;
    }

    enc_mtd_ele = AXIOM_NODE_GET_DATA_ELEMENT(enc_mtd_node, env);
    if (!enc_mtd_ele)
    {
        oxs_error(ERROR_LOCATION,
                OXS_ERROR_ELEMENT_FAILED, "Error retrieving encryption method element");
        return NULL;
    }

    enc_mtd = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE_BY_NAME(enc_mtd_ele, env, OXS_ATTR_ALGORITHM);
    if((!enc_mtd) ||(0 == AXIS2_STRCMP("", enc_mtd))){
        return NULL;
    }
    return enc_mtd;

}

