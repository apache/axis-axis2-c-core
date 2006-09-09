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

#include "echo_util.h"
#include <axis2_util.h>
#include <axiom_soap.h>
#include <axis2_client.h>

#define MAX_COUNT  3000000

/* to check whether the callback is completed */
int is_complete = 0;

int main(int argc, char** argv)
{
    const axis2_env_t *env = NULL;
    const axis2_char_t *address = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_endpoint_ref_t* reply_to = NULL;
    axis2_options_t *options = NULL;
    const axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *payload2 = NULL;
    int count = 0;
    axis2_property_t *property = NULL;
    /*axis2_qname_t *op_qname = NULL;*/
   
    /* Set up the environment */
    env = axis2_env_create_all("echo_non_blocking_dual.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of echo service */
    address = "http://localhost:9090/axis2/services/rm_echo";
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
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    /* Setup options */
    options = axis2_options_create(env);
    AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);
    AXIS2_OPTIONS_SET_USE_SEPARATE_LISTENER(options, env, AXIS2_TRUE);
    
    /* Seperate listner needs addressing, hence addressing stuff in options */
    AXIS2_OPTIONS_SET_ACTION(options, env,
        "http://ws.apache.org/axis2/c/samples/echoString");
    reply_to = axis2_endpoint_ref_create(env, "http://localhost:6060/axis2/services/__ANONYMOUS_SERVICE__/__OPERATION_OUT_IN__");

    AXIS2_OPTIONS_SET_REPLY_TO(options, env, reply_to);
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_APPLICATION);
    AXIS2_PROPERTY_SET_VALUE(property, env, AXIS2_VALUE_TRUE);

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
    svc_client = axis2_svc_client_create(env, client_home);
    if (!svc_client)
    {
        printf("Error creating service client\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
                  " %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        return -1;
    }

    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options);    
    
    AXIS2_SVC_CLIENT_ENGAGE_MODULE(svc_client, env, AXIS2_MODULE_ADDRESSING);  
    AXIS2_SVC_CLIENT_ENGAGE_MODULE(svc_client, env, "sandesha2");
    
    /* Build the SOAP request message payload using OM API.*/
    payload = build_om_payload_for_echo_svc(env, "echo1");
    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload);
    AXIS2_SLEEP(2);

    /* Build the SOAP request message payload using OM API.*/
    payload2 = build_om_payload_for_echo_svc(env, "echo2");
    
    AXIS2_OPTIONS_SET_PROPERTY(options, env, "Sandesha2LastMessage", 
            property);
    /* Send request */
    AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload2);

    /** Wait till callback is complete. Simply keep the parent thread running
       until our on_complete or on_error is invoked */
   while(count < MAX_COUNT )
   {
      if (is_complete)
      {
         /* We are done with the callback */
         break;
      }
        AXIS2_SLEEP(1);
        count++;
   }
    
    if (!(count < MAX_COUNT))
    {
        printf("\nrm_echo client invoke FAILED. Counter timed out.\n");
    }
    
    if (svc_client)
    {
        AXIS2_SVC_CLIENT_FREE(svc_client, env);
        svc_client = NULL;
    }
    
    return 0;
}

