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

axiom_node_t *build_om_payload_for_echo_svc(
    const axutil_env_t * env);

int
main(
    int argc,
    char **argv)
{
    const axutil_env_t *env = NULL;
    const axis2_char_t *address = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    axis2_options_t *options = NULL;
    const axis2_char_t *client_home = NULL;
    axis2_svc_client_t *svc_client = NULL;
    axiom_node_t *payload = NULL;
    axiom_node_t *ret_node = NULL;
    const axis2_char_t *un = NULL;
    const axis2_char_t *pw = NULL;
    const axis2_char_t *unp = NULL;
    const axis2_char_t *pwp = NULL;
    axis2_bool_t http_auth_required = AXIS2_FALSE;
    axis2_bool_t proxy_auth_required = AXIS2_FALSE;

    /* Set up the environment */
    env = axutil_env_create_all("echo_blocking_auth.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of echo service */
    address = "http://localhost:9090/axis2/services/echo";
    if (argc > 1)
    {
        if (axutil_strcmp(argv[1], "-h") == 0)
        {
            printf("Usage : %s [endpoint_url] (-a [username] [password]) (-p [username] [password])\n",
                   argv[0]);
            printf("use -a option for HTTP Authentication\n");
            printf("use -p option for Proxy Authentication\n");
            printf("use -h for help\n");
            return 0;
        }
        else if (axutil_strcmp(argv[1], "-a") == 0)
        {
            if (argc > 3)
            {
                un = argv[2];
                pw = argv[3];
            }
        }
        else if (axutil_strcmp(argv[1], "-p") == 0)
        {
            if (argc > 3)
            {
                unp = argv[2];
                pwp = argv[3];
            }
        }
        else
        {
            address = argv[1];
        }

        if (argc > 4)
        {
            if (axutil_strcmp(argv[2], "-a") == 0)
            {
                un = argv[3];
                pw = argv[4];
            }
            else if (axutil_strcmp(argv[2], "-p") == 0)
            {
                unp = argv[3];
                pwp = argv[4];
            }
        }
        if (argc > 6)
        {
            if (axutil_strcmp(argv[4], "-a") == 0)
            {
                un = argv[5];
                pw = argv[6];
                if (argc > 7)
                {
                    address = argv[7];
                }
            }
            else if (axutil_strcmp(argv[4], "-p") == 0)
            {
                unp = argv[5];
                pwp = argv[6];
                if (argc > 7)
                {
                    address = argv[7];
                }
            }
        }
        if (argc > 7)
        {
            if (axutil_strcmp(argv[5], "-a") == 0)
            {
                un = argv[6];
                pw = argv[7];
                if (!address)
                {
                    address = argv[4];
                }
            }
            else if (axutil_strcmp(argv[5], "-p") == 0)
            {
                unp = argv[6];
                pwp = argv[7];
                if (!address)
                {
                    address = argv[4];
                }
            }
        }
    }
    printf("Using endpoint : %s\n", address);

    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    /* Setup options */
    options = axis2_options_create(env);
    axis2_options_set_to(options, env, endpoint_ref);
    axis2_options_set_action(options, env,
                             "http://ws.apache.org/axis2/c/samples/echoString");

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
        axis2_options_set_http_auth_info(options, env, "", "", NULL); 
    }
    if (unp && pwp)
    {
        axis2_options_set_proxy_auth_info(options, env, "", "", NULL); 
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
        if (unp && pwp)
        {
            axis2_options_set_proxy_auth_info(options, env, unp, pwp,
                                              axis2_svc_client_get_auth_type(svc_client, env));
            /* un-comment line below to setup proxy from code*/
            /*axis2_svc_client_set_proxy_with_auth(svc_client, env, "127.0.0.1", "3128", unp, pwp);*/
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
            axis2_options_set_http_auth_info(options, env, un, pw,
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

    /* Engage addressing module */
    axis2_svc_client_engage_module(svc_client, env, AXIS2_MODULE_ADDRESSING);

    /* Build the SOAP request message payload using OM API. */
    payload = build_om_payload_for_echo_svc(env);

    /* Send request */
    ret_node = axis2_svc_client_send_receive(svc_client, env, payload);

    if (ret_node)
    {
        axis2_char_t *om_str = NULL;
        om_str = axiom_node_to_string(ret_node, env);
        if (om_str)
            printf("\nReceived OM : %s\n", om_str);
        printf("\necho client invoke SUCCESSFUL!\n");

        AXIS2_FREE(env->allocator, om_str);
        ret_node = NULL;
    }
    else
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Stub invoke FAILED: Error code:" " %d :: %s",
                        env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("echo client invoke FAILED!\n");
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
