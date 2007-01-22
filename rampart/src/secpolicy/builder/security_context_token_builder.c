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


#include <rp_security_context_token_builder.h>

AXIS2_EXTERN rp_security_context_token_t *AXIS2_CALL
rp_security_context_token_builder_build(
            const axis2_env_t *env,
            axiom_node_t *security_context,
            axiom_element_t *security_context_ele)
{
    rp_security_context_token_t *security_context_token = NULL;
    axiom_node_t *policy = NULL;
    axiom_element_t *pol_ele = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env,NULL);

    security_context_token = rp_security_context_token_create(env);
    if(!security_context_token)
        return NULL;

    status = rp_security_context_token_builder_set_inclusion_value(env,security_context_token,security_context_ele);
    if(status != AXIS2_SUCCESS )
    {
        rp_security_context_token_free(security_context_token,env);
        security_context_token = NULL;
        return NULL;
    }
    policy = AXIOM_NODE_GET_FIRST_CHILD(security_context,env);
    if(AXIOM_NODE_GET_NODE_TYPE(policy, env) == AXIOM_ELEMENT)
    {
        pol_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(policy, env);
        if(pol_ele)
        {
            axiom_children_iterator_t *children_iter = NULL;
            children_iter = AXIOM_ELEMENT_GET_CHILDREN(pol_ele, env, policy);
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
                                    status = rp_security_context_token_builder_set_properties(node,ele,local_name,security_context_token,env);
                                    if(status != AXIS2_SUCCESS)
                                    {
                                        rp_security_context_token_free(security_context_token,env);
                                        security_context_token = NULL;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return security_context_token;
}            

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
rp_security_context_token_builder_set_inclusion_value(
        const axis2_env_t *env,
        rp_security_context_token_t *token,
        axiom_element_t *element)
{
    
	axis2_char_t *inclusion_value = NULL;
    axis2_qname_t *qname = NULL;
	
	AXIS2_ENV_CHECK(env,AXIS2_FAILURE);

    qname = axis2_qname_create(env,RP_INCLUDE_TOKEN,RP_SP_NS,RP_SP_PREFIX);
    if(!qname)
        return AXIS2_FAILURE;
    
    inclusion_value = AXIOM_ELEMENT_GET_ATTRIBUTE_VALUE(element,env,qname);
            
    AXIS2_QNAME_FREE(qname,env);
    qname = NULL;         

    return rp_security_context_token_set_inclusion(token,env,inclusion_value);    
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_security_context_token_builder_set_properties(
        axiom_node_t *node,
        axiom_element_t *element,
        axis2_char_t *local_name,
        rp_security_context_token_t *security_context_token,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);

    if(strcmp(local_name,RP_REQUIRE_EXTERNAL_URI_REFERENCE)==0)
    {
        if(rp_match_secpolicy_qname(env,RP_REQUIRE_EXTERNAL_URI_REFERENCE,node,element))
        {
            rp_security_context_token_set_require_external_uri_ref(security_context_token,env,AXIS2_TRUE);
            printf("%s\n",local_name);
            return AXIS2_SUCCESS;
        }
        else
            return AXIS2_FAILURE;
    }
    else if(strcmp(local_name,RP_SC10_SECURITY_CONTEXT_TOKEN)==0)
    {
        if(rp_match_secpolicy_qname(env,RP_SC10_SECURITY_CONTEXT_TOKEN,node,element))
        {
            rp_security_context_token_set_sc10_security_context_token(security_context_token,env,AXIS2_TRUE);
            printf("%s\n",local_name);
            return AXIS2_SUCCESS;
        }
        else
            return AXIS2_FAILURE;
    }
    else if(strcmp(local_name,RP_REQUIRE_DERIVED_KEYS)==0)
    {
        if(rp_match_secpolicy_qname(env,RP_REQUIRE_DERIVED_KEYS,node,element))
        {
            rp_security_context_token_set_derivedkeys(security_context_token,env,AXIS2_TRUE);
            printf("%s\n",local_name);
            return AXIS2_SUCCESS;
        }
        else
            return AXIS2_FAILURE;
    }
    else
        return AXIS2_FAILURE;
}
