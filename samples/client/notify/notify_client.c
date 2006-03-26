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

#include <axis2_call.h>
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
#include <platforms/axis2_platform_auto_sense.h>

axis2_om_node_t *
build_om_programatically(axis2_env_t **env);

int main(int argc, char** argv)
{
    axis2_om_node_t *node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_char_t *address = NULL;
    axis2_char_t *wsa_action = NULL;
    axis2_char_t *client_home = NULL;
    axis2_om_node_t *ret_node = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    axis2_call_t *call = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_mep_client_t *mep_client = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_conf_t *conf = NULL;
    axis2_msg_ctx_t *response_ctx = NULL;
    
    /* set up the envioronment with allocator and log*/
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "notify.log");
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_TRACE;
    axis2_error_init();

    /* Set up deploy folder. It is from the deploy folder, the configuration is picked up 
     * using the axis2.xml file.
     * In this sample client_home points to the Axis2/C default deploy folder. The client_home can 
     * be different from this folder on your system. For example, you may have a different folder 
     *(say, my_client_folder) with its own axis2.xml file. my_client_folder/modules will have the 
     * modules that the client uses
     */
    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";
    
    /* Set end point reference of notify service */
    address = "http://localhost:9090/axis2/services/notify";
    wsa_action = "http://example.org/action/notify";
    if (argc > 1 )
        address = argv[1];
    if (AXIS2_STRCMP(address, "-h") == 0)
    {
        printf("Usage : %s [endpoint_url]\n", argv[0]);
        printf("use -h for help\n");
        return 0;
    }

    printf ("Using endpoint : %s\n", address);

    /* build the SOAP request message content using OM API.*/
    node = build_om_programatically(&env);

    /* create call struct */
    call = axis2_call_create(&env, NULL, client_home);
    mep_client = AXIS2_CALL_GET_BASE(call, &env);

    /* Prepare the SOAP envelope, using the SOAP message content to be sent.
     * Get a reference to the message context */
    msg_ctx = AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(mep_client, &env, node);
    if (!msg_ctx)
    {
        printf("ERROR: Could not prepare message context. ");
        printf("May be you havent set the repository corretly.\n");
        return -1;
    }

    /* Get the reference to message info headers structure from the message context. 
       This can be used to manipulate SOAP header content when using WS-Addressing. */
    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, &env);

    /* create an axis2_endpoint_ref_t struct with ERP assigned */
    endpoint_ref = axis2_endpoint_ref_create(&env, address);

    /* Set header parameters, required for WS-Addressing. 
     * Required only if you need to make use of WS-Addressing.
     */
    AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, &env, endpoint_ref);
    AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, &env, wsa_action); 
    
    AXIS2_CALL_SET_TO(call, &env, endpoint_ref);

    /* Get the configuration context */
    conf = AXIS2_CONF_CTX_GET_CONF(
                            AXIS2_SVC_CTX_GET_CONF_CTX(
                                AXIS2_MEP_CLIENT_GET_SVC_CTX(mep_client, &env), 
                                &env), 
                                &env);

    /* Get the notify service context if it is already loaded to service context*/
    svc = AXIS2_CONF_GET_SVC(conf, &env, "notify");
    if (svc)
    {
        op = AXIS2_SVC_GET_OP_WITH_NAME(svc, &env, "notify");
        if (op)
        {
            AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, &env, AXIS2_MEP_URI_OUT_IN);
        }
    }
    else
    {
       /* notify service is not in the configuration context. We need to create the 
        * operation and add it to service context. Then add service context into 
        * configuration context.
        */
        axis2_qname_t *op_qname = NULL;
        axis2_qname_t *svc_qname = axis2_qname_create(&env, "notify", NULL, NULL);
        svc = axis2_svc_create_with_qname(&env, svc_qname);
        op_qname = axis2_qname_create(&env, "notify", NULL, NULL);
        op = axis2_op_create_with_qname(&env, op_qname);
        AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, &env, AXIS2_MEP_URI_OUT_IN);
        AXIS2_SVC_ADD_OP(svc, &env, op);
        AXIS2_CONF_ADD_SVC(conf, &env, svc);
    }

    if (!op)
    {
        printf("ERROR: operation not present in service\n");
        return -1;
    }

   /* Invoke the operation. Client blocks until the response message comes. 
    * Response message gets set in the response message context.
    */
    response_ctx = AXIS2_CALL_INVOKE_BLOCKING(call, &env, op, msg_ctx);

    if (response_ctx)
    {
        printf("\nnotify stub invoke SUCCESSFUL!\n");
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke failed: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("notify stub invoke failed!\n");
    }
    
    if (call)
    {
        AXIS2_CALL_FREE(call, &env);
    }
    return status;
}

/* build SOAP request message content using OM */
axis2_om_node_t *
build_om_programatically(axis2_env_t **env)
{
    axis2_om_node_t *notify_om_node = NULL;
    axis2_om_element_t* notify_om_ele = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    

    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    ns1 = axis2_om_namespace_create (env, "http://example.org/notify", "m");

    notify_om_ele = axis2_om_element_create(env, NULL, "notify", ns1, &notify_om_node);
    
    AXIS2_OM_ELEMENT_SET_TEXT(notify_om_ele, env, "notify5", notify_om_node);
    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(notify_om_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("\nSending OM node in XML : %s \n",  buffer); 

    return notify_om_node;
}

