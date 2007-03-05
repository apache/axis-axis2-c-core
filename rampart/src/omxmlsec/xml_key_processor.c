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
#include <oxs_c14n.h>
#include <oxs_axiom.h>
#include <oxs_utility.h>
#include <openssl_rsa.h>
#include <openssl_digest.h>
#include <oxs_key_mgr.h>
#include <oxs_token_x509_data.h>
#include <oxs_token_x509_certificate.h>
#include <oxs_token_key_info.h>
#include <oxs_xml_key_processor.h>
/*Private functions*/

/*Public functions*/

AXIS2_EXTERN oxs_x509_cert_t *AXIS2_CALL
oxs_xml_key_process_X509Certificate(const axis2_env_t *env,
    axiom_node_t *X509_cert_node)
{
    axis2_char_t *data = NULL;
    axis2_char_t *node_name = NULL;
    oxs_x509_cert_t *cert = NULL;
    
    node_name = axiom_util_get_localname(X509_cert_node, env);
    if(0 != axis2_strcmp(node_name, OXS_NODE_X509_CERTIFICATE)){
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,"Invalid node. Expected %s. Found", OXS_NODE_X509_CERTIFICATE, node_name);
        return NULL;
    }
    /*Get contents*/
    data = oxs_token_get_x509_certificate(env, X509_cert_node);
    cert = oxs_key_mgr_load_x509_cert_from_string(env, data);
    
    return cert;
}


AXIS2_EXTERN oxs_x509_cert_t *AXIS2_CALL
oxs_xml_key_process_X509Data(const axis2_env_t *env,
    axiom_node_t *X509_data_node)
{
    oxs_x509_cert_t *cert = NULL;
    axiom_node_t *child_node = NULL;
    axis2_char_t *child_name = NULL;
    axis2_char_t *node_name = NULL;

    node_name = axiom_util_get_localname(X509_data_node, env);
    if(0 != axis2_strcmp(node_name, OXS_NODE_X509_DATA)){
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,"Invalid node. Expected %s. Found", OXS_NODE_X509_DATA, node_name);
        return NULL;
    }
    child_node = AXIOM_NODE_GET_FIRST_CHILD( X509_data_node, env);
    child_name = axiom_util_get_localname(child_node, env);
    
    /*Check wht is inside the <ds:X509Data>*/
    if(0 == axis2_strcmp(child_name, OXS_NODE_X509_CERTIFICATE)){
        cert = oxs_xml_key_process_X509Certificate(env, child_node);
    }else if(0 == axis2_strcmp(child_name,OXS_NODE_X509_SUBJECT_NAME )){
        /*TODO*/ 
    }else{
        /*We do not support*/

    }
    
    return cert;
}

