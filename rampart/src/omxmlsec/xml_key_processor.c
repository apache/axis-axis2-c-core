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
#include <oxs_token_x509_issuer_name.h>
#include <oxs_token_x509_serial_number.h>
#include <oxs_xml_key_processor.h>
/*Private functions*/

/*Public functions*/
AXIS2_EXTERN oxs_x509_cert_t *AXIS2_CALL
oxs_xml_key_process_X509IssuerSerial(const axis2_env_t *env,
    axiom_node_t *X509_issuer_serial_node)
{
    oxs_x509_cert_t *cert = NULL;
    axiom_node_t *issuer_name_node = NULL;
    axiom_node_t *serial_num_node = NULL;
    axis2_char_t *node_name = NULL;
    axis2_char_t *issuer_name = NULL;
    axis2_char_t *serial_num_str = NULL;

    node_name = axiom_util_get_localname(X509_issuer_serial_node, env);
    if(0 != axis2_strcmp(node_name, OXS_NODE_X509_ISSUER_SERIAL)){
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_INVALID_DATA,"Invalid node. Expected %s. Found", OXS_NODE_X509_ISSUER_SERIAL, node_name);
        return NULL;
    }

    
    issuer_name_node =  AXIOM_NODE_GET_FIRST_CHILD(X509_issuer_serial_node, env);
    if(issuer_name_node){
        issuer_name = oxs_token_get_issuer_name(env, issuer_name_node);
    }
    
    serial_num_node = AXIOM_NODE_GET_NEXT_SIBLING(issuer_name_node, env);
    if(serial_num_node){
        serial_num_str = oxs_token_get_serial_number(env, serial_num_node);
    }
    
    /*we set the key issuername and the serial number*/
    cert = oxs_x509_cert_create(env); 
    oxs_x509_cert_set_issuer(cert, env, issuer_name);
    oxs_x509_cert_set_serial_number(cert, env, atoi(serial_num_str));

    return cert;
}

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
    }else if(0 == axis2_strcmp(child_name, OXS_NODE_X509_ISSUER_SERIAL)){
         cert = oxs_xml_key_process_X509IssuerSerial(env, child_node);
    }else if(0 == axis2_strcmp(child_name,OXS_NODE_X509_SUBJECT_NAME )){
        /*TODO*/ 
    }else{
        /*We do not support*/

    }
    
    return cert;
}

