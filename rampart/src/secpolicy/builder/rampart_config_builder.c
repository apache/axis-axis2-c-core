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


#include <rp_rampart_config_builder.h>

AXIS2_EXTERN rp_rampart_config_t *AXIS2_CALL 
rp_rampart_config_builder_build(const axis2_env_t *env, axiom_node_t *config)
  
{
    rp_rampart_config_t *rampart_config = NULL;
    axiom_element_t *config_ele = NULL;
    axis2_status_t status = AXIS2_SUCCESS;

    AXIS2_ENV_CHECK(env, NULL);

    if(config)
    {
        if(AXIOM_NODE_GET_NODE_TYPE(config,env) == AXIOM_ELEMENT)
        {
            config_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(config, env);

            rampart_config = rp_rampart_config_create(env);
            if(!rampart_config)
                return NULL;

            if(config_ele)
            {
                axiom_children_iterator_t *children_iter = NULL;
                children_iter = AXIOM_ELEMENT_GET_CHILDREN(config_ele, env, config);
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
                                            status = rp_rampart_config_builder_populate(env,rampart_config,node,ele,local_name);
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
    return rampart_config;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rp_rampart_config_builder_populate(
            const axis2_env_t *env,
            rp_rampart_config_t *rampart_config,
            axiom_node_t *node,
            axiom_element_t *element,
            axis2_char_t *local_name)    
{
    AXIS2_ENV_CHECK(env,AXIS2_FAILURE);
    
    if(AXIS2_STRCMP(local_name,RP_USER)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_USER,node,element))
        {
            axis2_char_t *user = NULL;
            user = AXIOM_ELEMENT_GET_TEXT(element,env,node);

            return rp_rampart_config_set_user(rampart_config,env,user);
        }
        else
            return AXIS2_FAILURE;
    }
    else if(AXIS2_STRCMP(local_name,RP_ENCRYPTION_USER)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_ENCRYPTION_USER,node,element))
        {
            axis2_char_t *encryption_user = NULL;
            encryption_user = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            return rp_rampart_config_set_encryption_user(rampart_config,env,encryption_user);
        }
        else
            return AXIS2_FAILURE;
    }    
    else if(AXIS2_STRCMP(local_name,RP_PASSWORD_CALLBACK_CLASS)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_PASSWORD_CALLBACK_CLASS,node,element))
        {
            axis2_char_t *password_callback_class = NULL;
            password_callback_class = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            return rp_rampart_config_set_password_callback_class(rampart_config,env,password_callback_class);
        }
        else
            return AXIS2_FAILURE;
    }
    else if(AXIS2_STRCMP(local_name,RP_AUTHN_MODULE_NAME)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_AUTHN_MODULE_NAME,node,element))
        {
            axis2_char_t *authenticate_module = NULL;
            authenticate_module = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            return rp_rampart_config_set_authenticate_module(rampart_config,env,authenticate_module);
        }
        else
            return AXIS2_FAILURE;
    }
    else if(AXIS2_STRCMP(local_name,RP_PASSWORD_TYPE)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_PASSWORD_TYPE,node,element))
        {
            axis2_char_t *password_type = NULL;
            password_type = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            return rp_rampart_config_set_password_type(rampart_config,env,password_type);
        }
        else
            return AXIS2_FAILURE;
    }

    else if(AXIS2_STRCMP(local_name,RP_PUBLIC_KEY)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_PUBLIC_KEY,node,element))
        {
            axis2_char_t *public_key_file = NULL;
            public_key_file = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            return rp_rampart_config_set_public_key_file(rampart_config,env,public_key_file);
        }
        else
            return AXIS2_FAILURE;
    }
    else if(AXIS2_STRCMP(local_name,RP_PRIVATE_KEY)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_PRIVATE_KEY,node,element))
        {
            axis2_char_t *private_key_file = NULL;
            private_key_file = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            return rp_rampart_config_set_private_key_file(rampart_config,env,private_key_file);
        }
        else
            return AXIS2_FAILURE;
    }
    else if(AXIS2_STRCMP(local_name,RP_TIME_TO_LIVE)==0)
    {
        if(rp_match_rampart_config_qname(env,RP_TIME_TO_LIVE,node,element))
        {
            axis2_char_t *time_to_live = NULL;
            time_to_live = AXIOM_ELEMENT_GET_TEXT(element,env,node);
            return rp_rampart_config_set_time_to_live(rampart_config,env,time_to_live);
        }
        else
            return AXIS2_FAILURE;
    }
    else return AXIS2_FAILURE;
}
