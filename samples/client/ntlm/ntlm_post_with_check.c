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
#include <axiom.h>
#include <axis2_util.h>
#include <axiom_soap.h>
#include <axis2_client.h>
#include <axis2_http_transport.h>
#include "ntlm_util.h"

int
main(
    int argc,
    char **argv)
{
    const axutil_env_t *env = NULL;
    axis2_char_t *address = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    axis2_char_t *client_home = NULL;
    axis2_svc_client_t *svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *ret_node = NULL;
    axis2_char_t *un = NULL;
    axis2_char_t *pw = NULL;
    axis2_char_t *flags = NULL;
    int fg = 0;
    axis2_char_t *domain = NULL;
    axis2_char_t *workstation = NULL;
    axis2_bool_t http_auth_required = AXIS2_FALSE;
    axis2_bool_t proxy_auth_required = AXIS2_FALSE;
    axutil_string_t *soap_action = NULL;
	int i = 0;

    /* Set up the environment */
    env = axutil_env_create_all("ntlm_post_with_check.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of ntlm service */
    address = "http://172.16.176.132:80/myservice/Service1.asmx";
    if (argc > 1)
    {
        if (axutil_strcmp(argv[1], "-h") == 0)
        {
            printf("Usage : %s [endpoint_url] (-n [username] [password] [flags] [domain] [workstation])\n",
                   argv[0]);
            printf("use -n option for NTLM HTTP Authentication\n");
            printf("use -h for help\n");
            return 0;
        }
        else if (axutil_strcmp(argv[1], "-n") == 0)
        {
            if (argc > 3)
            {
                un = argv[2];
                pw = argv[3];
                flags = argv[4];
                domain = argv[5];
                workstation = argv[6];
            }
        }
        else
        {
            address = argv[1];
        }

        if (argc > 4)
        {
            if (axutil_strcmp(argv[2], "-n") == 0)
            {
                un = argv[3];
                pw = argv[4];
                flags = argv[5];
                domain = argv[6];
                workstation = argv[7];
            }
        }
    }
    printf("Using endpoint : %s\n", address);

    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    /* Setup options */
    options = axis2_options_create(env);
    axis2_options_set_to(options, env, endpoint_ref);
    soap_action =
        axutil_string_create(env, "http://tempuri.org/HelloWorld");
    axis2_options_set_soap_action(options, env, soap_action);
    axutil_string_free(soap_action, env);

    /* Set up deploy folder. It is from the deploy folder, the configuration is picked up
     * using the axis2.xml file.
     * In this sample client_home points to the Axis2/C default deploy folder. The client_home can 
     * be different from this folder on your system. For example, you may have a different folder 
     * (say, my_client_folder) with its own axis2.xml file. my_client_folder/modules will have the 
     * modules that the client uses
     */
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home || !strcmp(client_home, ""))
        client_home = "../..";

    /* Create service client */
    svc_client = axis2_svc_client_create(env, client_home);
    if (!svc_client)
    {
        printf
            ("Error creating service client, Please check AXIS2C_HOME again\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Stub invoke FAILED: Error code:" " %d :: %s",
                        env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        return -1;
    }

    /* Enabling REST for HTTP HEAD Request */
    axis2_options_set_enable_rest(options, env, AXIS2_TRUE);

    /* Setting Request as HTTP HEAD Request */
    axis2_options_set_http_method(options, env, AXIS2_HTTP_HEAD);

    /* Sending dummy authentication info */
    if (un && pw)
    {
        axis2_options_set_ntlm_proxy_auth_info(options, env, "", "", 0, NULL, NULL, NULL);
    }
    if(flags)
    {
        fg = atoi(flags);
    }
    else
    {
        fg = 0;
    }

    /* Force authentication tests */
    axis2_options_set_test_http_auth(options, env, AXIS2_TRUE);
    axis2_options_set_test_proxy_auth(options, env, AXIS2_TRUE);

    /* Set service client options */
    axis2_svc_client_set_options(svc_client, env, options);

    /* un-comment line below to setup proxy from code*/
    /*axis2_svc_client_set_proxy_with_auth(svc_client, env, "127.0.0.1", "3128", NULL, NULL);*/

    /* Sending robust authentication test message */
    axis2_svc_client_send_robust(svc_client, env, NULL);

    /* Checking whether authentication is required */
    if (axis2_svc_client_get_proxy_auth_required(svc_client, env))
    {
        proxy_auth_required = AXIS2_TRUE;

        /* Set proxy-auth information */
        if (un && pw)
        {
            axis2_options_set_ntlm_proxy_auth_info(options, env, un, pw, fg, domain, workstation,
                                              axis2_svc_client_get_auth_type(svc_client, env));
        }

        /* Sending robust authentication test message */
        axis2_svc_client_send_robust(svc_client, env, NULL);
    }
    if (axis2_svc_client_get_http_auth_required(svc_client, env))
    {
        http_auth_required = AXIS2_TRUE;
        /* Set http-auth information */
        if (un && pw)
        {
            axis2_options_set_ntlm_http_auth_info(options, env, un, pw, fg, domain, workstation,
                                             axis2_svc_client_get_auth_type(svc_client, env));
        }
    }

    /* Cancel authentication tests */
    axis2_options_set_test_http_auth(options, env, AXIS2_FALSE);
    axis2_options_set_test_proxy_auth(options, env, AXIS2_FALSE);

    /* Print whether authentication was required */
    if (http_auth_required)
    {
        printf("\nHTTP Authentication info required.\n");
    }
    if (proxy_auth_required)
    {
        printf("\nProxy Authentication info required.\n");
    }

    /* Disabling REST for SOAP Request */
    axis2_options_set_enable_rest(options, env, AXIS2_FALSE);

    /* Setting Request as HTTP POST Request */
    axis2_options_set_http_method(options, env, AXIS2_HTTP_POST);


    for(i = 0; i < 1; i++)
    {
        /* Build the SOAP request message payload using OM API. */
        payload = build_om_payload_for_ntlm_auth_service(env);

        /*axis2_svc_client_send_robust(svc_client, env, payload);*/
        /* Send request */
        ret_node = axis2_svc_client_send_receive(svc_client, env, payload);

        if (ret_node)
        {
            axis2_char_t *om_str = NULL;
            om_str = axiom_node_to_string(ret_node, env);
            if (om_str)
                printf("\nReceived OM : %s\n", om_str);
            printf("\nntlm client invoke SUCCESSFUL!\n");

            AXIS2_FREE(env->allocator, om_str);
            ret_node = NULL;
        }
        else
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Stub invoke FAILED: Error code:" " %d :: %s",
                            env->error->error_number,
                            AXIS2_ERROR_GET_MESSAGE(env->error));
            printf("ntlm client invoke FAILED!\n");
        }
    }
    if (svc_client)
    {
        axis2_svc_client_free(svc_client, env);
        svc_client = NULL;
    }

    if (env)
    {
        axutil_env_free((axutil_env_t *) env);
        env = NULL;
    }

    return 0;
}

