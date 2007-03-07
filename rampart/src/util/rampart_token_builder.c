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

/* 
 *
 */
#include <stdio.h>
#include <rampart_util.h>
#include <axis2_util.h>
#include <axis2_base64.h>
#include <oxs_buffer.h>
#include <rampart_constants.h>
#include <rampart_token_builder.h>
#include <oxs_constants.h>
#include <oxs_key_mgr.h>
#include <oxs_axiom.h>
#include <oxs_x509_cert.h>
#include <oxs_token_reference.h>
#include <oxs_token_binary_security_token.h>
#include <oxs_xml_key_processor.h>
#include <oxs_token_security_token_reference.h>
#include <oxs_token_binary_security_token.h>
#include <oxs_token_embedded.h>

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_security_token_reference(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert,
    rampart_token_build_pattern_t pattern)
{
    axis2_status_t status = AXIS2_FAILURE;
    axiom_node_t *stref_node = NULL;

    stref_node = oxs_token_build_security_token_reference_element(env, parent);

    if(RTBP_EMBEDDED == pattern){
        status = rampart_token_build_embedded(env, stref_node, cert);
    }else if(RTBP_KEY_IDENTIFIER == pattern){
        status = rampart_token_build_key_identifier(env, stref_node, cert);
    }else if(RTBP_X509DATA_ISSUER_SERIAL == pattern){
        status = rampart_token_build_x509_data_issuer_serial(env, stref_node, cert);
    }else if(RTBP_X509DATA_X509CERTIFICATE == pattern){
        status = rampart_token_process_x509_data_x509_certificate(env, stref_node, cert);
    }else{
        oxs_error(env, ERROR_LOCATION, OXS_ERROR_ELEMENT_FAILED, "Unsupported pattern %d to build wsse:SecurityTokenReference ", pattern);
        /*We do not support*/
        return AXIS2_FAILURE;
    }

    return status;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_embedded(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert)
{
    axis2_char_t *data  = NULL;
    axis2_char_t *bst_id  = NULL;
    axiom_node_t *embedded_node = NULL;
    axiom_node_t *bst_node = NULL;

    /*Get data from the certificate*/
    data = oxs_x509_cert_get_data(cert, env);
    if(!data){
        return AXIS2_FAILURE;
    }
    embedded_node = oxs_token_build_embedded_element(env, parent, "ID");
    bst_id = "bst-id";/*TODO*/
    bst_node =  oxs_token_build_binary_security_token_element(env, embedded_node, bst_id , OXS_VALUE_X509V3, OXS_ENCODING_BASE64BINARY, data);   
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_key_identifier(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert)
{
  
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
rampart_token_build_x509_data(const axis2_env_t *env,
    axiom_node_t *parent,
    oxs_x509_cert_t *cert)
{
    return AXIS2_SUCCESS;
}
