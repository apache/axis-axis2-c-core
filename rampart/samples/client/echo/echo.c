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
#include <axiom.h>
#include <axis2_util.h>
#include <axiom_soap.h>
#include <axis2_client.h>
#include <rampart_constants.h>

/*Enable following line to setup dynamic values*/
#define DYN_SETTINGS 1

axiom_node_t *
build_om_payload_for_echo_svc(const axis2_env_t *env);


int main(int argc, char** argv)
{
    const axis2_env_t *env = NULL;
    const axis2_char_t *address = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    const axis2_char_t *client_home = NULL;
    axis2_svc_client_t* svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *ret_node = NULL;
#ifdef DYN_SETTINGS
    axis2_property_t *un_property = NULL;
    axis2_property_t *pw_property = NULL;
    axis2_property_t *pw_type_property = NULL;
    axis2_property_t *items_property = NULL;
    axis2_property_t *pw_cb_property = NULL;
    axis2_property_t *time_to_live_property = NULL;
#endif
    /* Set up the environment */
    env = axis2_env_create_all("echo.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of echo service */
    address = "http://localhost:9090/axis2/services/echo";
    if (argc > 2 )
    {
        address = argv[1];
        client_home = argv[2];
        printf ("Using endpoint : %s\n", address);
        printf ("Using client_home : %s\n", client_home);
    } 
    
    if (AXIS2_STRCMP(address, "-h") == 0)
    {
        printf("Usage : %s [endpoint_url] [client_home]\n", argv[0]);
        printf("use -h for help\n");
        return 0;
    }

    
    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    /* Setup options */
    options = axis2_options_create(env);
    AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);
    AXIS2_OPTIONS_SET_ACTION(options, env,
        "http://ws.apache.org/axis2/c/samples/echoString");



   /*
    * Set security params. If you need to enable dynamic settings uncomment
    * following code section.
    */

#ifdef DYN_SETTINGS    
   un_property = axis2_property_create(env);
   AXIS2_PROPERTY_SET_VALUE(un_property, env, "Raigama");
   AXIS2_OPTIONS_SET_PROPERTY(options, env, RAMPART_ACTION_USER, un_property);

   pw_property = axis2_property_create(env);
   AXIS2_PROPERTY_SET_VALUE(pw_property, env, "RaigamaPW");
   AXIS2_OPTIONS_SET_PROPERTY(options, env, RAMPART_ACTION_PASSWORD,  pw_property);

   pw_type_property = axis2_property_create(env);
   AXIS2_PROPERTY_SET_VALUE(pw_type_property, env, RAMPART_PASSWORD_DIGEST);
   AXIS2_OPTIONS_SET_PROPERTY(options, env, RAMPART_ACTION_PASSWORD_TYPE, pw_type_property);

   /*Make sure these action itmes are validated by the server side. Otherwise error*/
   items_property = axis2_property_create(env);
   AXIS2_PROPERTY_SET_VALUE(items_property, env, "UsernameToken Timestamp" );
   AXIS2_OPTIONS_SET_PROPERTY(options, env, RAMPART_ACTION_ITEMS, items_property);

   /*Make sure this is the correct path for the callback module*/
   pw_cb_property = axis2_property_create(env);
   AXIS2_PROPERTY_SET_VALUE(pw_cb_property, env, "/home/kau/axis2/c/deploy/rampart/samples/callback/libpwcb.so");
   AXIS2_OPTIONS_SET_PROPERTY(options, env, RAMPART_ACTION_PW_CALLBACK_CLASS , pw_cb_property);

   time_to_live_property = axis2_property_create(env);
   AXIS2_PROPERTY_SET_VALUE(time_to_live_property, env, "420");
   AXIS2_OPTIONS_SET_PROPERTY(options, env, RAMPART_ACTION_TIME_TO_LIVE, time_to_live_property);
#endif    
               
    if(!client_home)
    {
       client_home = AXIS2_GETENV("AXIS2C_HOME");
        printf("\nNo client_home specified. Using default %s", client_home);
    }
             
    
    /* Set up deploy folder. It is from the deploy folder, the configuration is picked up 
     * using the axis2.xml file.
     * In this sample client_home points to the Axis2/C default deploy folder. The client_home can 
     * be different from this folder on your system. For example, you may have a different folder 
     * (say, my_client_folder) with its own axis2.xml file. my_client_folder/modules will have the 
     * modules that the client uses
     */
/*    if (!client_home)
        client_home = "../../deploy";
*/

    /* Create service client */
    printf("client_home= %s", client_home);
    svc_client = axis2_svc_client_create(env, client_home);
    if (!svc_client)
    {
        printf("Error creating service client\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
                  " %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
    }

    /* Set service client options */
    AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options);    
    
    /* Engage addressing module */
    AXIS2_SVC_CLIENT_ENGAGE_MODULE(svc_client, env, AXIS2_MODULE_ADDRESSING);
    
    /* Build the SOAP request message payload using OM API.*/
    payload = build_om_payload_for_echo_svc(env);
    
    /* Send request */
    ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload);
    
    if(ret_node)
    {
        axis2_char_t *om_str = NULL;
        om_str = AXIOM_NODE_TO_STRING(ret_node, env);
        if (om_str)
            printf("\nReceived OM : %s\n", om_str);
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
        AXIS2_SVC_CLIENT_FREE(svc_client, env);
        svc_client = NULL;
    }
    if (endpoint_ref)
    {
        AXIS2_ENDPOINT_REF_FREE(endpoint_ref, env);
        endpoint_ref = NULL;
    }
    return 0;
}

/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_echo_svc(const axis2_env_t *env)
{
    axiom_node_t *echo_om_node = NULL;
    axiom_element_t* echo_om_ele = NULL;
    axiom_node_t* text_om_node = NULL;
    axiom_element_t * text_om_ele = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;
    
    ns1 = axiom_namespace_create (env, "http://ws.apache.org/axis2/c/samples", "ns1");
    echo_om_ele = axiom_element_create(env, NULL, "echoString", ns1, &echo_om_node);
    text_om_ele = axiom_element_create(env, echo_om_node, "text", NULL, &text_om_node);
    AXIOM_ELEMENT_SET_TEXT(text_om_ele, env, "echo5", text_om_node);
    
    om_str = AXIOM_NODE_TO_STRING(echo_om_node, env);
    if (om_str)
        printf("\nSending OM : %s\n", om_str);

    return echo_om_node;
}
