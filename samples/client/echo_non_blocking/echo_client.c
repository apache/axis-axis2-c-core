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
#include <axis2_async_result.h>

/* my on_complete callback function */
axis2_status_t AXIS2_CALL
my_on_complete(struct axis2_callback *callback,
                                  axis2_env_t **env);

/* my on_error callback function */
axis2_status_t AXIS2_CALL
my_on_error(struct axis2_callback *callback,
                            axis2_env_t **env,
                            int exception);

axis2_om_node_t *
build_om_programatically(axis2_env_t **env);

/* to check whether the callback is completed */
int isComplete = 0;

int main(int argc, char** argv)
{
    axis2_om_node_t *node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_thread_pool_t *thread_pool = NULL;
    axis2_char_t *address = NULL;
    axis2_char_t *wsa_action = NULL;
    axis2_char_t *client_home = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    axis2_call_t *call = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_mep_client_t *mep_client = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_conf_t *conf = NULL;
    axis2_callback_t* callback = NULL;
    
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "addr_echo.log");
    thread_pool = axis2_thread_pool_init(allocator);
    env = axis2_env_create_with_error_log_thread_pool(allocator, error, log, thread_pool);
    env->log->level = AXIS2_LOG_LEVEL_TRACE;
    axis2_error_init();

    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";
    
    address = "http://localhost:9090/axis2/services/echo";
    wsa_action = "http://ws.apache.org/axis2/c/samples/echo/echoString";
    if (argc > 1 )
        address = argv[1];
    if (AXIS2_STRCMP(address, "-h") == 0)
    {
        printf("Usage : %s [endpoint_url]\n", argv[0]);
        printf("use -h for help\n");
        return 0;
    }

    printf ("Using endpoint : %s\n", address);

    node = build_om_programatically(&env);

    /* create call without passing svc_ctx_t struct */
    call = axis2_call_create(&env, NULL, client_home);
    mep_client = AXIS2_CALL_GET_BASE(call, &env);
    msg_ctx = AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(mep_client, &env, node);
    if (!msg_ctx)
    {
        printf("ERROR: Could not prepare message context. ");
        printf("May be you havent set the repository corretly.\n");
        return -1;
    }
    
    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, &env);
    endpoint_ref = axis2_endpoint_ref_create(&env, address);
    AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, &env, endpoint_ref);
    AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, &env, wsa_action); 
    AXIS2_CALL_SET_TO(call, &env, endpoint_ref);
    conf = AXIS2_CONF_CTX_GET_CONF(
                            AXIS2_SVC_CTX_GET_CONF_CTX(
                                AXIS2_MEP_CLIENT_GET_SVC_CTX(mep_client, &env), 
                                &env), 
                                &env);
    svc = AXIS2_CONF_GET_SVC(conf, &env, "echo");
    if (svc)
    {
        op = AXIS2_SVC_GET_OP_WITH_NAME(svc, &env, "echoString");
        if (op)
        {
            AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op, &env, AXIS2_MEP_URI_OUT_IN);
        }
    }
    else
    {
        axis2_qname_t *op_qname = NULL;
        axis2_qname_t *svc_qname = axis2_qname_create(&env, "echo", NULL, NULL);
        svc = axis2_svc_create_with_qname(&env, svc_qname);
        op_qname = axis2_qname_create(&env, "echoString", NULL, NULL);
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

	/* create the callback object with default
	on_complete and on_error callback functions*/
    callback = axis2_callback_create(&env);
	
	/* set our on_complete fucntion pointer to the callback object */
	AXIS2_CALLBACK_SET_ON_COMPLETE(callback, my_on_complete);

	/* set our on_error function pointer to the callback object */
	AXIS2_CALLBACK_SET_ON_ERROR(callback, my_on_error);
	
	/* invoke the operation and get the control back to the main 
	   program immediately (without blocking)*/
    AXIS2_CALL_INVOKE_NON_BLOCKING(call, &env, op, msg_ctx, callback);

    printf("Non blocking call invoked - Control back to Main Program\n");

	/** this is simply to keep the parent thread running
	  until our on_complete or on_error is invoked 
	  */
	while(1)
	{
		if (isComplete)
		{
			/* we are done with the callback */
			break;
		}
	}
  
    if (call)
    {
        AXIS2_CALL_FREE(call, &env);
    }
	if (callback)
	{
		AXIS2_CALLBACK_FREE(callback, &env);
	}
    return status;
}

axis2_om_node_t *
build_om_programatically(axis2_env_t **env)
{
    axis2_om_node_t *echo_om_node = NULL;
    axis2_om_element_t* echo_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    axis2_om_namespace_t *ns1 = NULL;
    

    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    ns1 = axis2_om_namespace_create (env, "http://ws.apache.org/axis2/c/samples", "ns1");

    echo_om_ele = axis2_om_element_create(env, NULL, "echoString", ns1, &echo_om_node);
    
    text_om_ele = axis2_om_element_create(env, echo_om_node, "text", NULL, &text_om_node);

    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, "echo5", text_om_node);
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(echo_om_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("\nSending OM node in XML : %s \n",  buffer); 
    AXIS2_OM_OUTPUT_FREE(om_output, env);
    AXIS2_FREE((*env)->allocator, buffer);

    return echo_om_node;
}

axis2_status_t AXIS2_CALL
my_on_complete(struct axis2_callback *callback,
                                  axis2_env_t **env)
{
	/** SOAP response has arrived here; get the soap envelope 
	  from the callback object and do whatever you want to do with it */
	
	axis2_soap_envelope_t *soap_envelope = NULL;
	axis2_om_node_t *ret_node = NULL;
	
	axis2_xml_writer_t *writer = NULL;
	axis2_om_output_t *om_output = NULL;
	axis2_char_t *buffer = NULL;
	
	printf("inside on_complete_callback function\n");
	
	soap_envelope = AXIS2_CALLBACK_GET_ENVELOPE(callback, env);
	
	if (!soap_envelope)
	{
		 AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
                     " %d :: %s", (*env)->error->error_number,
                     AXIS2_ERROR_GET_MESSAGE((*env)->error));
		printf("echo stub invoke FAILED!\n");
		return AXIS2_FAILURE;
	}
	
	ret_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, env);

	if(!ret_node)
	{
		AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, "Stub invoke FAILED: Error code:"
	                  " %d :: %s", (*env)->error->error_number,
	                  AXIS2_ERROR_GET_MESSAGE((*env)->error));
		printf("echo stub invoke FAILED!\n");
		return AXIS2_FAILURE;
	}

	/*we just serialize the SOAP message and output to stdout */
	printf("\necho stub invoke SUCCESSFUL!\n");
	writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0);
	om_output = axis2_om_output_create (env, writer);

	AXIS2_OM_NODE_SERIALIZE (ret_node, env, om_output);
	buffer = AXIS2_XML_WRITER_GET_XML(writer, env);
	printf ("\nReceived OM node in XML : %s\n", buffer);
	AXIS2_OM_OUTPUT_FREE(om_output, env);
	AXIS2_FREE((*env)->allocator, buffer);

	isComplete = 1;
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
my_on_error(struct axis2_callback *callback,
                            axis2_env_t **env,
                            int exception)
{
	/** take necessary action on error */

	printf("my on_error error code:%d ::%s", exception, 
			AXIS2_ERROR_GET_MESSAGE((*env)->error));
	isComplete = 1;
	return AXIS2_SUCCESS;
}

