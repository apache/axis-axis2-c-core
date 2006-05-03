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

#include <stdio.h>
#include <axis2_util.h>
#include <axis2_om.h>
#include <axis2_soap.h>
#include <axis2_client.h>

axis2_om_node_t *
build_om_payload(axis2_env_t **env);

void 
print_om(axis2_env_t **env, axis2_om_node_t *node);

int main(int argc, char** argv)
{
    axis2_env_t *env = NULL;
    axis2_char_t *address = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axis2_om_node_t *payload = NULL;
    axis2_om_node_t *ret_node = NULL;
   
    /* Set up the envioronment */
    env = axis2_env_create_all("echo_blocking.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of echo service */
    address = "http://localhost:9090/axis2/services/echo";
    if (argc > 1 )
        address = argv[1];
    if (AXIS2_STRCMP(address, "-h") == 0)
    {
        printf("Usage : %s [endpoint_url]\n", argv[0]);
        printf("use -h for help\n");
        return 0;
    }
    printf ("Using endpoint : %s\n", address);
    
    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(&env, address);

    /* Setup options */
    options = axis2_options_create(&env);
    AXIS2_OPTIONS_SET_TO(options, &env, endpoint_ref);

    /* Set up deploy folder. It is from the deploy folder, the configuration is picked up 
     * using the axis2.xml file.
     * In this sample client_home points to the Axis2/C default deploy folder. The client_home can 
     * be different from this folder on your system. For example, you may have a different folder 
     * (say, my_client_folder) with its own axis2.xml file. my_client_folder/modules will have the 
     * modules that the client uses
     */
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";

    /* Create service client */
    svc_client = axis2_svc_client_create(&env, client_home);
    if (!svc_client)
    {
        printf("Error creating service client\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
    }

    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, &env, options);    

    /* Build the SOAP request message payload using OM API.*/
    payload = build_om_payload(&env);
    
    /* Send request */
    ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, &env, payload);
    
    if(ret_node)
    {
        printf("\nReceived OM : ");
        print_om(&env, ret_node);
        printf("\necho client invoke SUCCESSFUL!\n");
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("echo client invoke FAILED!\n");
    }
    
    if (svc_client)
    {
        AXIS2_SVC_CLIENT_FREE(svc_client, &env);
        svc_client = NULL;
    }
    if (endpoint_ref)
    {
        AXIS2_ENDPOINT_REF_FREE(endpoint_ref, &env);
        endpoint_ref = NULL;
    }
    return 0;
}

/* build SOAP request message content using OM */
axis2_om_node_t *
build_om_payload(axis2_env_t **env)
{
    axis2_om_node_t *echo_om_node = NULL;
    axis2_om_element_t* echo_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    
    ns1 = axis2_om_namespace_create (env, "http://ws.apache.org/axis2/c/samples", "ns1");
    echo_om_ele = axis2_om_element_create(env, NULL, "echoString", ns1, &echo_om_node);
    text_om_ele = axis2_om_element_create(env, echo_om_node, "text", NULL, &text_om_node);
    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, "echo5", text_om_node);
    
    printf("\nSending OM : ");
    print_om(env, echo_om_node);

    return echo_om_node;
}

void print_om(axis2_env_t **env, axis2_om_node_t *node)
{
    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("%s\n",  buffer); 
    AXIS2_FREE((*env)->allocator, buffer);
    AXIS2_OM_OUTPUT_FREE(om_output, env);
    return;
}
