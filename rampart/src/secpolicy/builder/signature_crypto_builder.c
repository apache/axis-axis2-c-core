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


#include <rp_signature_crypto_builder.h>

AXIS2_EXTERN rp_signature_crypto_t *AXIS2_CALL 
rp_signature_crypto_builder_build(const axis2_env_t *env, axiom_node_t *signature)
  
{
    rp_signature_crypto_t *signature_crypto = NULL;
    axiom_element_t *signature_ele = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, NULL);

    if(signature)
    {
        if(AXIOM_NODE_GET_NODE_TYPE(signature,env) == AXIOM_ELEMENT)
        {
            signature_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(signature, env);

            signature_crypto = rp_signature_crypto_create(env);
            if(!signature_crypto)
                return NULL;

            if(signature_ele)
            {
                axiom_children_iterator_t *children_iter = NULL;
                children_iter = AXIOM_ELEMENT_GET_CHILDREN(signature_ele, env, signature);
                if(children_iter )
                {
                    while(AXIOM_CHILDREN_ITERATOR_HAS_NEXT(children_iter, env))
                    {
                        axiom_node_t *node = NULL;
                        axiom_element_t *ele = NULL;
                        axis2_char_t *local_name = NULL;
                        node = AXIOM_CHILDREN_ITERATOR_NEXT(children_iter, env);
                        if(node)
                        {
                            if(AXIOM_NODE_GET_NODE_TYPE(node, env) == AXIOM_ELEMENT)
                            {
                                ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(node, env);
                                if(ele)
                                {
                                    local_name = AXIOM_ELEMENT_GET_LOCALNAME(ele,env);
                                    if(local_name)
                                    {
                                        {
                                            status = rp_signature_crypto_builder_populate(env,signature_crypto,node,ele,local_name);
                                            if(status!=AXIS2_SUCCESS)
                                                return NULL;
                                        }
                                            
                                    }
                                    
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return signature_crypto;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_signature_crypto_builder_populate(
            const axis2_env_t *env,
            rp_signature_crypto_t *signature_crypto,
            axiom_node_t *node,
            axiom_element_t *element,
            axis2_char_t *local_name)    
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    
    if(AXIS2_STRCMP(local_name,RP_SIGNATURE_PROP_FILE)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_SIGNATURE_PROP_FILE,node,element))
        {
            axis2_char_t *signature_prop_file = NULL;
            signature_prop_file = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            printf("%s\n",local_name);
            printf("%s\n",signature_prop_file);

            return rp_signature_crypto_set_signature_prop_file(signature_crypto,env,signature_prop_file);
        }
        else
            return AXIS2_FAILURE;
    }
    else return AXIS2_FAILURE;
}
