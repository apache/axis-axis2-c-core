/*
 * Copyright 2004,2005 The Apache Software Foundation.
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

#include "axis2_math_stub.h"
#include <axis2_om_stax_builder.h>
#include <axis2_om_document.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_stream.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axis2_xml_reader.h>
#include <stdio.h>
#include <axis2_xml_writer.h>
#include <axis2_soap_builder.h>
#include <axis2_soap.h>
#include <axis2_soap_envelope.h>
#include <axis2_soap_body.h>
#include <axis2_soap_header.h>
#include <axis2_soap_message.h>
#include <axis2_soap_header_block.h>
#include <axis2_soap_fault.h>
#include <axis2_soap_fault_code.h>
#include <axis2_soap_fault_role.h>
#include <axis2_platform_auto_sense.h>

axis2_om_node_t *
build_om_programatically(axis2_env_t **env, axis2_char_t *operation, axis2_char_t *param1, axis2_char_t *param2);

int main(int argc, char** argv)
{
    axis2_stub_t *stub = NULL;
    axis2_om_node_t *node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_char_t *address = NULL;
    axis2_char_t *client_home = NULL;
    axis2_om_node_t *ret_node = NULL;

    axis2_char_t *operation = "add";
    axis2_char_t *param1 = "40";
    axis2_char_t *param2 = "8";
    
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "math_client.log");
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_INFO;
    axis2_error_init();

    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";
    
    address = "http://localhost:9090/axis2/services/math";
    if (argc > 1 )
        operation = argv[1];
    if (AXIS2_STRCMP(operation, "-h") == 0)
    {
        printf("Usage : %s [operation] [param1] [param2] [endpoint_url]\n", argv[0]);
        printf("use -h for help\n");
        printf("default operation add\n");
        printf("default param1 %s\n", param1);
        printf("default param2 %s\n", param2);
        printf("default endpoint_url %s\n", address);
        printf("NOTE: command line arguments must appear in given order, with trailing ones being optional\n");
        return 0;
    }
    if (argc > 2 )
        param1 = argv[2];
    if (argc > 3 )
        param2 = argv[3];
    if (argc > 4 )
        address = argv[4];

    printf ("Using endpoint : %s\n", address);
    printf ("\nInvoking operation %s with params %s and %s\n", operation, param1, param2);

    node = build_om_programatically(&env, operation, param1, param2);
    stub = 
        axis2_stub_create_with_endpoint_uri_and_client_home(&env, address,
            client_home);
    AXIS2_STUB_SET_TRANSPORT_INFO(stub, &env, AXIS2_TRANSPORT_HTTP, 
        AXIS2_TRANSPORT_HTTP, AXIS2_FALSE);
    /* create node and invoke math */
    ret_node = axis2_math_stub_add(stub, &env, node);
    if(ret_node)
    {
        if (AXIS2_OM_NODE_GET_NODE_TYPE(ret_node, &env) == AXIS2_OM_ELEMENT)
        {
            axis2_char_t *result = NULL;
            axis2_om_element_t *result_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(ret_node, &env);
            
            result = AXIS2_OM_ELEMENT_GET_TEXT(result_ele, &env, ret_node);
            printf( "\nResult = %s\n", result);
        }
        else
        {
            axis2_xml_writer_t *writer = NULL;
            axis2_om_output_t *om_output = NULL;
            axis2_char_t *buffer = NULL;
            writer = axis2_xml_writer_create_for_memory(&env, NULL, AXIS2_TRUE, 0);
            om_output = axis2_om_output_create (&env, writer);

            AXIS2_OM_NODE_SERIALIZE (ret_node, &env, om_output);
            buffer = AXIS2_XML_WRITER_GET_XML(writer, &env);
            printf ("\nReceived invalid OM as result : %s\n", buffer);
        }
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke failed: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("math stub invoke failed!\n");
    }
    
    if (node)
    {
        AXIS2_OM_NODE_FREE_TREE(node, &env);
    }

    if (stub)
    {
        AXIS2_STUB_FREE(stub, &env);
    }
    return status;
}

axis2_om_node_t *
build_om_programatically(axis2_env_t **env, axis2_char_t *operation, axis2_char_t *param1, axis2_char_t *param2)
{
    axis2_om_node_t *math_om_node = NULL;
    axis2_om_element_t* math_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    

    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    ns1 = axis2_om_namespace_create (env, "http://localhost:9090/axis2/services/math", "ns1");

    math_om_ele = axis2_om_element_create(env, NULL, operation, ns1, &math_om_node);
    
    text_om_ele = axis2_om_element_create(env, math_om_node, "param1", NULL, &text_om_node);
    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, param1, text_om_node);
    
    text_om_ele = axis2_om_element_create(env, math_om_node, "param2", NULL, &text_om_node);
    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, param2, text_om_node);
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(math_om_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "\nSending OM node in XML : %s \n",  buffer); 

    return math_om_node;
}

