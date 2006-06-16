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

#include <axis2_const.h>
#include <axis2_env.h>
#include <axis2_engine.h>
#include <axis2_allocator.h>
#include <axis2_svc_client.h>
#include <axis2_options.h>
#include <platforms/axis2_platform_auto_sense.h>

axiom_node_t *
build_om_payload_for_echo_svc(const axis2_env_t *env,
		              axis2_char_t* echo_str);

void axis2_test_svc_client_blocking()
{
   axis2_env_t* env = NULL;
   axis2_options_t *options = NULL;
   axis2_char_t *client_home = NULL;
   axis2_svc_client_t* svc_client = NULL;
   axiom_node_t *payload = NULL;
   axiom_node_t *ret_node = NULL;
   axis2_char_t *address = NULL;
   axis2_endpoint_ref_t* endpoint_ref = NULL;

   axis2_allocator_t *allocator = axis2_allocator_init (NULL);
   env = axis2_env_create (allocator);
   axiom_element_t *result_ele = NULL;
   axis2_char_t* echo_text = "echo_text";
   axis2_char_t* result = NULL;
    
   address = "http://localhost:9090/axis2/services/echo/echo";
   endpoint_ref = axis2_endpoint_ref_create(env, address);
   client_home = AXIS2_GETENV("AXIS2C_HOME");
   if (!client_home)
       client_home = "../../deploy";

   svc_client = axis2_svc_client_create(env, client_home);

   options = axis2_options_create(env);
   AXIS2_OPTIONS_SET_TO(options, env, endpoint_ref);
   AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options);
   
   payload = build_om_payload_for_echo_svc(env, echo_text );
   ret_node = AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload);
   if ( ret_node )
   {
      if (AXIOM_NODE_GET_NODE_TYPE(ret_node, env) == AXIOM_ELEMENT )
      {
        ret_node = AXIOM_NODE_GET_FIRST_CHILD ( ret_node, env);
	result_ele = (axiom_element_t*)AXIOM_NODE_GET_DATA_ELEMENT(ret_node, env);
	result = AXIOM_ELEMENT_GET_TEXT(result_ele, env, ret_node);
        if (!strcmp( result, echo_text) )
            printf("axis2_test SVC_CLIENT_SEND_RECEIVE SUCCESS\n");
        else
            printf("axis2_test SVC_CLIENT_SEND_RECEIVE FAILURE\n");
      }
   }
   AXIS2_SVC_CLIENT_FREE(svc_client, env);    
}

/* build SOAP request message content using OM */
axiom_node_t *
build_om_payload_for_echo_svc(const axis2_env_t *env,
		              axis2_char_t* echo_text)
{
    axiom_node_t *echo_om_node = NULL;
    axiom_element_t* echo_om_ele = NULL;
    axiom_node_t* text_om_node = NULL;
    axiom_element_t * text_om_ele = NULL;
    axiom_namespace_t *ns1 = NULL;

    ns1 = axiom_namespace_create (env, "http://ws.apache.org/axis2/c/samples", "ns1");
    echo_om_ele = axiom_element_create(env, NULL, "echoString", ns1, &echo_om_node);
    text_om_ele = axiom_element_create(env, echo_om_node, "text", NULL, &text_om_node);
    AXIOM_ELEMENT_SET_TEXT(text_om_ele, env, echo_text, text_om_node);

    return echo_om_node;
}

int main()
{
   axis2_test_svc_client_blocking();
   return 0;
}
