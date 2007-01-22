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

#include <rp_binding_commons_builder.h>
#include <rp_transport_binding_builder.h>
#include <rp_qname_matcher.h>
#include <rp_token_builder.h>

AXIS2_EXTERN rp_transport_binding_t *AXIS2_CALL 
rp_transport_binding_builder_build(const axis2_env_t *env, axiom_node_t *transport)
{
    rp_transport_binding_t *transport_binding = NULL;
    rp_binding_commons_t *commons = NULL;        
    axiom_element_t *pol_ele = NULL;
    axiom_node_t *policy = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if(transport)
    {
        policy = AXIOM_NODE_GET_FIRST_CHILD(transport,env);

        if(AXIOM_NODE_GET_NODE_TYPE(policy, env) == AXIOM_ELEMENT)
        {
            pol_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(policy, env);

            transport_binding = rp_transport_binding_create(env);
            if(!transport_binding)
                return NULL;

            commons = rp_binding_commons_create(env);
            if(!commons)
                return NULL;
                        
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
                                        if(rp_transport_binding_builder_istoken(env,local_name))
                                            rp_transport_binding_builder_set_token_type(env,transport_binding,node,ele,local_name); 
                                        else
                                            rp_binding_commons_builder_build(env,node,ele,local_name,commons);
                                    }
                                }
                            }
                        }
                    }
                }
                rp_transport_binding_set_binding_commons(transport_binding,env,commons);               
            }
        }
    }
    return transport_binding;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_transport_binding_builder_set_token_type(
            const axis2_env_t *env,
            rp_transport_binding_t *transport_binding,
            axiom_node_t *node,
            axiom_element_t *element,
            axis2_char_t *local_name)    
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    
    if(AXIS2_STRCMP(local_name,RP_TRANSPORT_TOKEN)==0)
    {
        if(rp_match_secpolicy_qname(env,RP_TRANSPORT_TOKEN,node,element))
        {
            rp_property_t *transport_token = NULL;
            transport_token = rp_token_builder_build(env,node);
            if(!transport_token)
                return AXIS2_FAILURE;

            printf("%s\n",local_name);
            return rp_transport_binding_set_transport_token(transport_binding,env,transport_token);
        }
        else
            return AXIS2_FAILURE;
    }
    else
        return AXIS2_FAILURE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
rp_transport_binding_builder_istoken(
        const axis2_env_t *env,
        axis2_char_t *local_name)
{
    AXIS2_ENV_CHECK(env,AXIS2_FALSE);

    if(AXIS2_STRCMP(local_name,RP_TRANSPORT_TOKEN)==0)
        return AXIS2_TRUE;
    
    else
        return AXIS2_FALSE;
}
