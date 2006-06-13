/*
 *Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <axis2_qname.h>
#include <axis2_svc.h>
#include <axiom_soap_header.h>
#include <axiom_soap_body.h>
#include <axiom_soap_header_block.h>
#include <axis2_endpoint_ref.h>
#include <axis2_property.h>
#include <rampart/rampart_constants.h>
#include <rampart/rampart_crypto_util.h>
#include <rampart/rampart_util.h>

/*************************** Function headers *********************************/

AXIS2_EXTERN axiom_node_t* AXIS2_CALL
rampart_build_timestamp_token(const axis2_env_t *env,
                                const axis2_ctx_t *ctx,
                                axiom_node_t *sec_node,
                                const  axiom_namespace_t *sec_ns_obj,
                                int ttl
                                );
                                
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_validate_timestamp(const axis2_env_t *env,
                                                        axiom_node_t *ts_node);

/************************* End of function headers ****************************/
AXIS2_EXTERN axiom_node_t* AXIS2_CALL
rampart_build_timestamp_token(const axis2_env_t *env,
                                const axis2_ctx_t *ctx,
                                axiom_node_t *sec_node,
                                const  axiom_namespace_t *sec_ns_obj,
                                int ttl
                                )
{

    axiom_node_t  *ts_node, *created_node, *expires_node = NULL;    
    axiom_element_t  *ts_ele, *created_ele, *expires_ele = NULL;
    axis2_char_t *created_val, *expires_val = NULL;
    axiom_namespace_t *wsu_ns_obj = NULL;

    wsu_ns_obj = axiom_namespace_create (env, RAMPART_WSU_XMLNS,
                                              RAMPART_WSU);
    
    ts_ele = axiom_element_create (env, sec_node, 
                                        RAMPART_SECURITY_TIMESTAMP, 
                                        wsu_ns_obj,
                                        &ts_node);
    if(NULL != ts_ele)
    {
         
         created_ele = axiom_element_create (env, ts_node, RAMPART_SECURITY_TIMESTAMP_CREATED, wsu_ns_obj,
                                             &created_node);
        
        if(NULL != created_ele)
        {
             created_val = rampart_generate_time(env, 0);   /*Current time*/
             AXIOM_ELEMENT_SET_TEXT (created_ele, env, created_val, created_node);                     
        }    
        
        
         expires_ele = axiom_element_create (env, ts_node, RAMPART_SECURITY_TIMESTAMP_EXPIRES, wsu_ns_obj,
                                             &expires_node);
        
        if(NULL != expires_ele)
        {
              expires_val = rampart_generate_time(env, ttl);
              AXIOM_ELEMENT_SET_TEXT (expires_ele, env, expires_val, expires_node);           
            
        }       
    }
    
    return sec_node;
}/*rampart_build_timestamp_token*/

                                
AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_validate_timestamp(const axis2_env_t *env,
                                                        axiom_node_t *ts_node)
{
    axis2_status_t validity = AXIS2_FAILURE;
    axis2_qname_t *created_qname, *expires_qname = NULL;
    axiom_element_t *created_ele, *expires_ele , *ts_ele= NULL;
    axiom_node_t *created_node, *expires_node = NULL;
    axis2_char_t *created_val, *expires_val = NULL;    
    
    ts_ele = AXIOM_NODE_GET_DATA_ELEMENT(ts_node, env);
    
    if(!ts_ele)
    {
        AXIS2_LOG_INFO(env->log," Cannot find timestamp ... :(");
        return AXIS2_FAILURE;
    }
    
    created_qname = axis2_qname_create(env,
                                 RAMPART_SECURITY_TIMESTAMP_CREATED,
                                 RAMPART_WSU_XMLNS,
                                 RAMPART_WSU);
    if(created_qname)
    {
        created_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(ts_ele, env, created_qname, ts_node, &created_node);
        if(!created_ele)
        {
            AXIS2_LOG_INFO(env->log,"Cannot find created  in timestamp element...");
            return AXIS2_FAILURE;
        }
    }

     expires_qname = axis2_qname_create(env,
                                 RAMPART_SECURITY_TIMESTAMP_EXPIRES,
                                 RAMPART_WSU_XMLNS,
                                 RAMPART_WSU);
    if(expires_qname)
    {
        expires_ele = AXIOM_ELEMENT_GET_FIRST_CHILD_WITH_QNAME(ts_ele, env, expires_qname, ts_node, &expires_node);
        if(!expires_ele)
        {
            AXIS2_LOG_INFO(env->log,"Cannot find expires  in timestamp element...");
            return AXIS2_FAILURE;
        }
    }
    
    created_val = AXIOM_ELEMENT_GET_TEXT(created_ele, env, created_node);
    expires_val = AXIOM_ELEMENT_GET_TEXT(expires_ele, env, expires_node);
    
    /*TODO check weather time has expired or not*/
    
    validity = AXIS2_SUCCESS;
    /*free memory for qnames*/
    return validity;
}
