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
#include <axis2_wsdl4c_parser.h>
#include <axis2_wsdl_pump.h>
#include <axis2_wsdl_desc.h>

#include <axis2_wsdl_ext_soap_address.h>
#include <axis2_wsdl_ext_soap_op.h>

axis2_om_node_t *
build_om_programatically(axis2_env_t **env, axis2_char_t *operation, 
				axis2_char_t *param1, axis2_char_t *param2);

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
    axis2_call_t *call = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_mep_client_t *mep_client = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_conf_t *conf = NULL;
    axis2_msg_ctx_t *response_ctx = NULL;
    axis2_char_t *param1 = "40";
    axis2_char_t *param2 = "8";
    void *wp = NULL;
	axis2_wsdl_desc_t *wsdl_desc = NULL;
	axis2_wsdl_pump_t *wsdl_pump = NULL;
	axis2_hash_index_t *index = NULL;
	axis2_hash_t *svcs = NULL;
	axis2_hash_t *endpoints = NULL;
	axis2_hash_t *ops = NULL;
	axis2_hash_t *binding_ops = NULL;
	void *value = NULL;
	axis2_wsdl_svc_t *wsdl_svc = NULL;
	axis2_wsdl_endpoint_t *wsdl_endpoint = NULL;
	axis2_wsdl_binding_t *wsdl_binding = NULL;
	axis2_wsdl_binding_op_t *wsdl_binding_op = NULL;
	axis2_wsdl_interface_t *wsdl_interface = NULL;
	axis2_wsdl_op_t *wsdl_op = NULL;
	axis2_svc_t *svc = NULL;
	axis2_op_t *op = NULL;
	axis2_qname_t *op_qname = NULL;
	axis2_char_t *op_name = NULL;
	axis2_linked_list_t *ext_elements = NULL;
	axis2_wsdl_ext_soap_address_t *soap_address = NULL;
	axis2_wsdl_ext_soap_op_t *soap_op = NULL;

    /* set up the envioronment with allocator and log*/
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "addr_echo.log");
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
    
	if (argc <= 1)
    {
      printf("Usage: print <wsdl_File_Name>\n");
      return 1;
    }
    wp = axis2_wsdl4c_parser_create(argv[1], "");
	wsdl_desc = axis2_wsdl_desc_create(&env);
    wsdl_pump = axis2_wsdl_pump_create(&env, wsdl_desc, wp);
	if(!wsdl_pump)
	{
		return -1;
	}
	status = AXIS2_WSDL_PUMP_PUMP(wsdl_pump, &env);
	if(AXIS2_SUCCESS != status)
	{
		return -1;
	}
	svcs = AXIS2_WSDL_DESC_GET_SVCS(wsdl_desc, &env);
    index = axis2_hash_first (svcs, &env);	
	axis2_hash_this (index, NULL, NULL, &value);
	wsdl_svc = (axis2_wsdl_svc_t *) value;
	if(!wsdl_svc)
	{
		return -1;
	}
	endpoints = AXIS2_WSDL_SVC_GET_ENDPOINTS(wsdl_svc, &env);
	index = axis2_hash_first (endpoints, &env);
	axis2_hash_this(index, NULL, NULL, &value);
	wsdl_endpoint = (axis2_wsdl_endpoint_t *) value;
	if(!wsdl_endpoint)
	{
		return -1;
	}
    wsdl_binding = AXIS2_WSDL_ENDPOINT_GET_BINDING(wsdl_endpoint, &env);
	if(!wsdl_binding)
	{
		return -1;
	}
	wsdl_interface = AXIS2_WSDL_BINDING_GET_BOUND_INTERFACE(wsdl_binding, &env);
	if(!wsdl_interface)
	{
		return -1;
	}
	ops = AXIS2_WSDL_INTERFACE_GET_OPS(wsdl_interface, &env);
	index = axis2_hash_first (ops, &env);
	axis2_hash_this(index, NULL, NULL, &value);
	wsdl_op = (axis2_wsdl_op_t *) value;
	if(!wsdl_op)
	{
		return -1;
	}
	op = axis2_op_create_with_wsdl_op(&env, wsdl_op);
	{
		return -1;
	}
	op_qname = AXIS2_OP_GET_QNAME(op, &env);
	op_name = AXIS2_QNAME_GET_LOCALPART(op_qname, &env);

	ext_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
				wsdl_endpoint->wsdl_component, &env);
	soap_address = AXIS2_LINKED_LIST_GET(ext_elements, &env, 0);
	if(!soap_address) return -1;
	address = AXIS2_WSDL_EXT_SOAP_ADDRESS_GET_LOCATION_URI(soap_address, &env);
	binding_ops = AXIS2_WSDL_BINDING_GET_BINDING_OPS(wsdl_binding, &env);
	index = axis2_hash_first (binding_ops, &env);
	axis2_hash_this(index, NULL, NULL, &value);
	wsdl_binding_op = (axis2_wsdl_binding_op_t *) value;
	ext_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
				wsdl_binding_op->extensible_component->wsdl_component, &env);
	soap_op = AXIS2_LINKED_LIST_GET(ext_elements, &env, 0);
	wsa_action = AXIS2_WSDL_EXT_SOAP_OP_GET_SOAP_ACTION(soap_op, &env);
	if (argc > 2 )
        param1 = argv[2];
    if (argc > 3 )
        param2 = argv[3];

    /* build the SOAP request message content using OM API.*/
    node =  build_om_programatically(&env, op_name, param1, param2);

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

	svc = axis2_svc_create_with_wsdl_svc(&env, wsdl_svc);
	/* echo service is not in the configuration context. We need to create the 
     * operation and add it to service context. Then add service context into 
     * configuration context.
     */
    AXIS2_SVC_ADD_OP(svc, &env, op);
    AXIS2_CONF_ADD_SVC(conf, &env, svc);

   /* Invoke the operation. Client blocks until the response message comes. 
    * Response message gets set in the response message context.
    */
    response_ctx = AXIS2_CALL_INVOKE_BLOCKING(call, &env, op, msg_ctx);

    if (response_ctx)
    {
        /* Get the response SOAP message from response message context */
        axis2_soap_envelope_t *soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(response_ctx, &env);
        ret_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, &env);
    }
                                                        
    if(ret_node)
    {
        /* Get the response value from the SOAP message */
        axis2_xml_writer_t *writer = NULL;
        axis2_om_output_t *om_output = NULL;
        axis2_char_t *buffer = NULL;
        
        printf("\necho stub invoke SUCCESSFUL!\n");
        writer = axis2_xml_writer_create_for_memory(&env, NULL, AXIS2_TRUE, 0);
        om_output = axis2_om_output_create (&env, writer);

        AXIS2_OM_NODE_SERIALIZE (ret_node, &env, om_output);
        buffer = AXIS2_XML_WRITER_GET_XML(writer, &env);
        printf ("\nReceived OM node in XML : %s\n", buffer);
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke failed: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("echo stub invoke failed!\n");
    }
    
    if (call)
    {
        AXIS2_CALL_FREE(call, &env);
    }
    return status;
}

axis2_om_node_t *
build_om_programatically(axis2_env_t **env, axis2_char_t *operation, 
				axis2_char_t *param1, axis2_char_t *param2)
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

