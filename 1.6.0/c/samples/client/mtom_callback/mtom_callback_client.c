
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
#include <axutil_uuid_gen.h>

axiom_node_t *build_om_programatically(
    const axutil_env_t * env,
    const axis2_char_t * image_name);

int 
process_response_node(
    const axutil_env_t * env,
    axiom_node_t *node);


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
    const axis2_char_t *image_name = "resources/axis2.jpg";

    /* Set up the environment */
    env = axutil_env_create_all("mtom_callback.log", AXIS2_LOG_LEVEL_TRACE);

    /* Set end point reference of mtom service */
    address = "http://localhost:9090/axis2/services/mtom_callback";
    if (argc > 1)
        address = argv[1];
    if (axutil_strcmp(address, "-h") == 0)
    {
        printf
            ("Usage : %s [endpoint_url] [image_name] \n",
             argv[0]);
        printf("use -h for help\n");
        return 0;
    }
    if (argc > 2)
        image_name = argv[2];

    printf("Using endpoint : %s\n", address);

    /* Create EPR with given address */
    endpoint_ref = axis2_endpoint_ref_create(env, address);

    /* Setup options */
    options = axis2_options_create(env);
    axis2_options_set_to(options, env, endpoint_ref);
    axis2_options_set_action(options, env,
                             "http://ws.apache.org/axis2/c/samples/mtomCallbackSample");

    axis2_options_set_soap_version(options, env, AXIOM_SOAP11);

    axis2_options_set_enable_mtom(options, env, AXIS2_TRUE);

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

    /* Set service client options */
    axis2_svc_client_set_options(svc_client, env, options);

    /* Engage addressing module */
    axis2_svc_client_engage_module(svc_client, env, AXIS2_MODULE_ADDRESSING);

    /* Build the SOAP request message payload using OM API. */
    payload =
        build_om_programatically(env, image_name);

    /* Send request */
    ret_node = axis2_svc_client_send_receive(svc_client, env, payload);
    if (ret_node)
    {
        axis2_char_t *om_str = NULL;
        om_str = axiom_node_to_string(ret_node, env);
        if (om_str)
        {
            if (axis2_svc_client_get_last_response_has_fault(svc_client, env) == AXIS2_TRUE)
            {
                printf("\nRecieved Fault : %s\n", om_str);
                AXIS2_FREE(env->allocator, om_str);
            }
            else
            {
                printf("\nReceived OM : %s\n", om_str);
                AXIS2_FREE(env->allocator, om_str);
                printf("\nmtom client invoke SUCCESSFUL!\n");
                process_response_node(env, ret_node);
            }
        }
    }
    else
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Stub invoke FAILED: Error code:" " %d :: %s",
                        env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("\nmtom client invoke FAILED!\n");
        printf("\nSending callback may not be set. Check the log for more details.\n");
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

/* build SOAP request message content using OM */
axiom_node_t *
build_om_programatically(
    const axutil_env_t * env,
    const axis2_char_t * attachment_name)
{
    axiom_node_t *mtom_om_node = NULL;
    axiom_element_t *mtom_om_ele = NULL;
    axiom_node_t *attachment_om_node = NULL;
    axiom_element_t *attachment_om_ele = NULL;
    axiom_node_t *data_om_node = NULL;
    axiom_text_t *data_text = NULL;
    axiom_namespace_t *ns1 = NULL;
    axis2_char_t *om_str = NULL;

    axiom_data_handler_t *data_handler = NULL;

    ns1 =
        axiom_namespace_create(env, "http://ws.apache.org/axis2/c/samples/mtom",
                               "ns1");
    mtom_om_ele =
        axiom_element_create(env, NULL, "mtomSample", ns1, &mtom_om_node);

    attachment_om_ele =
        axiom_element_create(env, mtom_om_node, "attachment", ns1, &attachment_om_node);

    /* The attachment is loaded using the callback. The callback should be 
     * specified in the axis2.xml */

    data_handler = axiom_data_handler_create(env, NULL, "image/jpeg");
    axiom_data_handler_set_data_handler_type(data_handler, env, AXIOM_DATA_HANDLER_TYPE_CALLBACK); 
    axiom_data_handler_set_user_param(data_handler, env, (void *)attachment_name);

    data_text =
        axiom_text_create_with_data_handler(env, attachment_om_node, data_handler,
                                            &data_om_node);

    axiom_text_set_optimize(data_text, env, AXIS2_TRUE);
    om_str = axiom_node_to_string(mtom_om_node, env);
    if (om_str)
    {
        printf("%s", om_str);
        AXIS2_FREE(env->allocator, om_str);
    }
    return mtom_om_node;
}


int 
process_response_node(
    const axutil_env_t * env,
    axiom_node_t *node)
{
    axiom_node_t *res_om_node = NULL;
    res_om_node = axiom_node_get_first_child(node, env);

    if(axiom_node_get_node_type(res_om_node, env) == AXIOM_TEXT)
    {/** received mtom atttachment */
        axiom_data_handler_t *data_handler = NULL;
        axiom_text_t *axiom_text = (axiom_text_t*)axiom_node_get_data_element(res_om_node, env);
        data_handler = axiom_text_get_data_handler(axiom_text, env);

        if(axiom_data_handler_get_cached(data_handler, env)) 
        {
            axis2_char_t *mime_id = NULL;
            
            printf("Attachment is cached.\n");

            mime_id = axiom_data_handler_get_mime_id(data_handler, env);
            if(mime_id)
            {
                /* The client implementer should know what to do with 
                 * the attachment in the response. Becasue the attachment 
                 * was stored using the callback given by the client */

                /*axis2_char_t command[1000];

                sprintf(command, "rm -f /opt/tmp/%s", mime_id);
                system(command);*/
            }
        }   
        else
        {
            axiom_data_handler_set_file_name(data_handler, env, "test");
            axiom_data_handler_write_to(data_handler, env);
        }
    }
    else
    {
        printf("No attachemnt in the response\n");
    }

    return 0;
}


