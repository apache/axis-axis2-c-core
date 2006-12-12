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

#include "axis2_stub_interop_test_port_type_doc_service.h"

axiom_node_t* generate_request_xml(const  axis2_env_t* env,
        axis2_char_t *operation,
        axis2_char_t *value);
void handle_respone_xml(const  axis2_env_t* env, axiom_node_t* res);

int main(int argc, char** argv)
{
    axis2_env_t* env = NULL;
    axis2_char_t* client_home = NULL;
    axis2_char_t* endpoint_uri = NULL;
    axis2_svc_client_t *svc_client = NULL;
    axis2_options_t *options = NULL;

    axis2_stub_t* stub = NULL;

    axiom_node_t* req = NULL;
    axiom_node_t* res = NULL;

    endpoint_uri = "http://localhost:9090/axis2/services/InteropTestPortTypeDocService";

    env = axis2_env_create_all("w2c_test.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set up deploy folder.*/
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../../deploy";

    stub = axis2_stub_interop_test_port_type_doc_service_create(env, client_home , endpoint_uri);
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    /*AXIS2_SVC_CLIENT_DISENGAGE_MODULE(svc_client, env, AXIS2_MODULE_ADDRESSING);*/
    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    AXIS2_OPTIONS_SET_ACTION(options, env, "http://soapinterop.org");

    /** testing echoString */
    printf("echoing string..\n");
    req =  generate_request_xml(env, "echoString", "test string");
    res = axis2_stub_interop_test_port_type_doc_service_echo_string(stub, env, req);
    handle_respone_xml(env , res);
    printf("\n");

    stub = axis2_stub_interop_test_port_type_doc_service_create(env, client_home , endpoint_uri);
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    /*AXIS2_SVC_CLIENT_DISENGAGE_MODULE(svc_client, env, AXIS2_MODULE_ADDRESSING);*/
    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    AXIS2_OPTIONS_SET_ACTION(options, env, "http://soapinterop.org");

    /** testing echoInteger*/
    printf("echoing integer..\n");
    req =  generate_request_xml(env, "echoInteger", "1343");
    res = axis2_stub_interop_test_port_type_doc_service_echo_integer(stub, env, req);
    handle_respone_xml(env , res);
    printf("\n");

    stub = axis2_stub_interop_test_port_type_doc_service_create(env, client_home , endpoint_uri);
    svc_client = AXIS2_STUB_GET_SVC_CLIENT(stub, env);
    /*AXIS2_SVC_CLIENT_DISENGAGE_MODULE(svc_client, env, AXIS2_MODULE_ADDRESSING);*/
    options = AXIS2_STUB_GET_OPTIONS(stub, env);
    AXIS2_OPTIONS_SET_ACTION(options, env, "http://soapinterop.org");

    /** testing echoString */
    printf("echoing float..\n");
    req =  generate_request_xml(env, "echoFloat", "400.23");
    res = axis2_stub_interop_test_port_type_doc_service_echo_float(stub, env, req);
    handle_respone_xml(env , res);
    printf("\n");

    return 0;
}

axiom_node_t* generate_request_xml(const  axis2_env_t* env,
        axis2_char_t *operation,
        axis2_char_t *value)
{
    axiom_node_t *op_node = NULL;
    axiom_element_t* op_ele = NULL;
    axiom_node_t* value_node = NULL;
    axiom_element_t * value_ele = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;


    ns1 = axiom_namespace_create(env, "http://soapinterop.org", "ns1");
    op_ele = axiom_element_create(env, NULL, operation, ns1, &op_node);

    /** here according to wsdl schema definite name should be provided anyway
     * we used "arg" for all the tests */
    value_ele = axiom_element_create(env, op_node, "arg", NULL, &value_node);
    AXIOM_ELEMENT_SET_TEXT(value_ele, env, value, value_node);


    printf("requesting %s\n" , value);
    om_str = AXIOM_NODE_TO_STRING(op_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return op_node;

}

void handle_respone_xml(const  axis2_env_t* env, axiom_node_t* res)
{
    axiom_node_t* node = NULL;
    axiom_element_t* ele = NULL;
    axis2_char_t* text =  NULL;
    axis2_char_t *om_str = NULL;

    if (NULL == res)
    {
        printf("response null\n");
        return;
    }
    om_str = AXIOM_NODE_TO_STRING(res, env);
    if (om_str)
        printf("\nRetrieving OM : %s\n", om_str);
    node = AXIOM_NODE_GET_FIRST_CHILD(res, env);
    if (node)
    {
        ele = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
        text = AXIOM_ELEMENT_GET_TEXT(ele, env, node);

        printf("response= %s\n", text);
    }

}
