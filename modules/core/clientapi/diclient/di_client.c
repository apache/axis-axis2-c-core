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
#include <axis2_di_client.h>
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
#include <axis2_soap_const.h>
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
#include <axis2_wsdl4c_soap.h>

#include <axis2_wsdl_ext_soap_address.h>
#include <axis2_wsdl_ext_soap_op.h>

/** 
 * @brief Dynamic Invocation Client struct impl
 *	Dynamic Invocation Client 
 */ 
typedef struct axis2_diclient_impl
{
    axis2_diclient_t diclient;
    void *parser;
    axis2_wsdl_desc_t *wsdl_desc;
    axis2_hash_t *op_map;
    axis2_hash_t *endpoint_map;
    axis2_hash_t *svc_map;
    axis2_hash_t *op_param_map;
    axis2_char_t *wsa_action;
    axis2_char_t *address;
    axis2_char_t *param_namespc;
    axis2_char_t *param_name;
    
} axis2_diclient_impl_t;

#define AXIS2_INTF_TO_IMPL(diclient) \
		((axis2_diclient_impl_t *)diclient)


/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_diclient_free (axis2_diclient_t *diclient,
									const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_diclient_invoke(axis2_diclient_t *diclient,
                        const axis2_env_t *env,
				       	axis2_om_node_t *node,
                        axis2_op_t *op);
                                
axis2_status_t AXIS2_CALL
axis2_diclient_init(axis2_diclient_t *diclient,
                        const axis2_env_t *env,
						axis2_char_t *wsdl_file_name);

axis2_status_t *AXIS2_CALL
axis2_diclient_set_address_and_action_for_op(axis2_diclient_t *diclient,
                                        const axis2_env_t *env,
                                        axis2_qname_t *op_qname);


axis2_hash_t *AXIS2_CALL
axis2_diclient_get_endpoints(axis2_diclient_t *diclient,
                                const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
axis2_diclient_get_services(axis2_diclient_t *diclient,
                                const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
axis2_diclient_get_operations(axis2_diclient_t *diclient,
                                const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_diclient_get_param_localname(axis2_diclient_t *diclient,
                            const axis2_env_t *env,
                            axis2_qname_t *op_qname);

axis2_char_t *AXIS2_CALL
axis2_diclient_get_param_namespace(axis2_diclient_t *diclient,
                            const axis2_env_t *env,
                            axis2_qname_t *op_qname);

static axis2_status_t
axis2_diclient_process_param(axis2_diclient_t *diclient,
                                const axis2_env_t *env,
                                axis2_qname_t *op_qname);

axis2_hash_t *AXIS2_CALL
axis2_diclient_get_params(axis2_diclient_t *diclient,
                            const axis2_env_t *env,
                            axis2_qname_t *op_qname);

/************************** End of function prototypes ************************/

axis2_diclient_t * AXIS2_CALL 
axis2_diclient_create (const axis2_env_t *env)
{
    axis2_diclient_impl_t *diclient_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	diclient_impl = (axis2_diclient_impl_t *) AXIS2_MALLOC(env->allocator,
			sizeof(axis2_diclient_impl_t));
	
	
	if(NULL == diclient_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    diclient_impl->parser = NULL;
    diclient_impl->wsdl_desc = NULL;
    diclient_impl->param_name = NULL;
    diclient_impl->param_namespc = NULL;
    diclient_impl->op_map = NULL;
    diclient_impl->op_param_map = NULL;
    diclient_impl->endpoint_map = NULL;
    diclient_impl->svc_map = NULL;
    diclient_impl->wsa_action = NULL;
    diclient_impl->address= NULL;
    diclient_impl->diclient.ops = NULL;
	
	diclient_impl->diclient.ops = 
		AXIS2_MALLOC (env->allocator, sizeof(axis2_diclient_ops_t));
	if(NULL == diclient_impl->diclient.ops)
    {
        axis2_diclient_free(&(diclient_impl->diclient), env);
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	diclient_impl->diclient.ops->free =  axis2_diclient_free;
	diclient_impl->diclient.ops->init = axis2_diclient_init;
	diclient_impl->diclient.ops->invoke = axis2_diclient_invoke;
    diclient_impl->diclient.ops->set_address_and_action_for_op =  
            axis2_diclient_set_address_and_action_for_op;
	diclient_impl->diclient.ops->get_endpoints = axis2_diclient_get_endpoints;
	diclient_impl->diclient.ops->get_services = axis2_diclient_get_services;
	diclient_impl->diclient.ops->get_operations = axis2_diclient_get_operations;
    diclient_impl->diclient.ops->get_param_localname = 
            axis2_diclient_get_param_localname;
    diclient_impl->diclient.ops->get_param_namespace = 
            axis2_diclient_get_param_namespace;
	
	return &(diclient_impl->diclient);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_diclient_free (axis2_diclient_t *diclient, 
                            const axis2_env_t *env)
{
    axis2_diclient_impl_t *diclient_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);
    
    if(diclient_impl->parser)
    {
        axis2_wsdl4c_parser_destroy(diclient_impl->parser);
        diclient_impl->parser = NULL;
    }

    if(diclient_impl->wsdl_desc)
    {
        AXIS2_WSDL_DESC_FREE(diclient_impl->wsdl_desc, env);
        diclient_impl->wsdl_desc = NULL;
    }
    
    if(diclient_impl->op_map)
    {
        axis2_hash_free(diclient_impl->op_map, env);
        diclient_impl->op_map = NULL;
    }
    
    if(diclient_impl->endpoint_map)
    {
        axis2_hash_free(diclient_impl->endpoint_map, env);
        diclient_impl->endpoint_map = NULL;
    }
    
    if(diclient_impl->wsa_action)
    {
        AXIS2_FREE(env->allocator, diclient_impl->wsa_action);
        diclient_impl->wsa_action = NULL;
    }
    
    if(diclient_impl->address)
    {
        AXIS2_FREE(env->allocator, diclient_impl->address);
        diclient_impl->address = NULL;
    }
    
	if(NULL != diclient->ops)
        AXIS2_FREE(env->allocator, diclient->ops);
    
    if(diclient_impl)
    {
        AXIS2_FREE(env->allocator, diclient_impl);
        diclient_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_diclient_init(axis2_diclient_t *diclient,
                        const axis2_env_t *env,
						axis2_char_t *wsdl_file_name)
{
    axis2_diclient_impl_t *diclient_impl = NULL;
	axis2_wsdl_pump_t *wsdl_pump = NULL;
	axis2_hash_t *endpoints = NULL;
	axis2_hash_index_t *index1 = NULL;
	axis2_hash_t *ops = NULL;
	axis2_wsdl_svc_t *wsdl_svc = NULL;
	axis2_wsdl_endpoint_t *wsdl_endpoint = NULL;
	axis2_wsdl_binding_t *wsdl_binding = NULL;
	axis2_wsdl_interface_t *wsdl_interface = NULL;
	axis2_wsdl_op_t *wsdl_op = NULL;
    axis2_svc_t *svc = NULL;
	axis2_op_t *op = NULL;
	axis2_qname_t *op_qname = NULL;
	axis2_char_t *op_name = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, wsdl_file_name, AXIS2_FAILURE);
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);
    
    diclient_impl->parser = axis2_wsdl4c_parser_create(wsdl_file_name, "");
	diclient_impl->wsdl_desc = axis2_wsdl_desc_create(env);
    if(!diclient_impl->op_map)
    {
        diclient_impl->op_map = axis2_hash_make(env);
    }
    if(!diclient_impl->endpoint_map)
    {
        diclient_impl->endpoint_map = axis2_hash_make(env);
    }
    wsdl_pump = axis2_wsdl_pump_create(env, diclient_impl->wsdl_desc, diclient_impl->parser);
	if(!wsdl_pump)
	{
		return AXIS2_FAILURE;
	}
	AXIS2_WSDL_PUMP_PUMP(wsdl_pump, env);
    
    diclient_impl->svc_map = AXIS2_WSDL_DESC_GET_SVCS(diclient_impl->wsdl_desc, env);
    for (index1 = axis2_hash_first (diclient_impl->svc_map, env); index1; 
                    index1 = axis2_hash_next (env, index1))
    {
        void *value = NULL;
	    axis2_hash_index_t *index2 = NULL;
        
	    axis2_hash_this (index1, NULL, NULL, &value);
	    wsdl_svc = (axis2_wsdl_svc_t *) value;
	    if(!wsdl_svc)
	    {
		    return AXIS2_FAILURE;
	    }
	    svc = axis2_svc_create_with_wsdl_svc(env, wsdl_svc);
        if(!svc) return AXIS2_FAILURE;
	    endpoints = AXIS2_WSDL_SVC_GET_ENDPOINTS(wsdl_svc, env);
        for (index2 = axis2_hash_first (endpoints, env); index2; 
                    index2 = axis2_hash_next (env, index2))
        {
	        axis2_hash_index_t *index3 = NULL;
            void *value = NULL;
            axis2_qname_t *endpoint_qname = NULL;
            axis2_char_t *endpoint_qname_str = NULL;
            
	        axis2_hash_this(index2, NULL, NULL, &value);
	        wsdl_endpoint = (axis2_wsdl_endpoint_t *) value;
	        if(!wsdl_endpoint)
	        {
		        return AXIS2_FAILURE;
	        }
            endpoint_qname = AXIS2_WSDL_ENDPOINT_GET_NAME(wsdl_endpoint, env);
            endpoint_qname_str = AXIS2_QNAME_TO_STRING(endpoint_qname, env);
            axis2_hash_set(diclient_impl->endpoint_map, endpoint_qname_str, 
                    AXIS2_HASH_KEY_STRING, wsdl_endpoint); 
            AXIS2_WSDL_SVC_SET_ENDPOINT(wsdl_svc, env, wsdl_endpoint);
            wsdl_binding = AXIS2_WSDL_ENDPOINT_GET_BINDING(wsdl_endpoint, env);
	        if(!wsdl_binding)
	        {
		        return AXIS2_FAILURE;
	        }
	        wsdl_interface = AXIS2_WSDL_BINDING_GET_BOUND_INTERFACE(wsdl_binding, env);
	        if(!wsdl_interface)
	        {
		        return AXIS2_FAILURE;
	        }
            AXIS2_WSDL_SVC_SET_SVC_INTERFACE(wsdl_svc, env, wsdl_interface);
	        ops = AXIS2_WSDL_INTERFACE_GET_OPS(wsdl_interface, env);
            for (index3 = axis2_hash_first (ops, env); index3; 
                    index3 = axis2_hash_next (env, index3))
            {
                void *value = NULL;

	            axis2_hash_this(index3, NULL, NULL, &value);
	            wsdl_op = (axis2_wsdl_op_t *) value;
	            if(!wsdl_op)
	            {
		            return AXIS2_FAILURE;
	            }
	            op = axis2_op_create_with_wsdl_op(env, wsdl_op);
                if(!op)
	            {
		            return AXIS2_FAILURE;
	            }
                AXIS2_OP_SET_PARENT(op, env, svc);
	            op_qname = AXIS2_OP_GET_QNAME(op, env);
	            op_name = AXIS2_QNAME_GET_LOCALPART(op_qname, env);
                axis2_hash_set(diclient_impl->op_map, op_name, 
                        AXIS2_HASH_KEY_STRING, op);
            }
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_diclient_invoke(axis2_diclient_t *diclient,
                        const axis2_env_t *env,
				       	axis2_om_node_t *node,
                        axis2_op_t *op)
{    
    axis2_diclient_impl_t *diclient_impl = NULL;
    axis2_char_t *client_home = NULL;
    axis2_om_node_t *ret_node = NULL;
    axis2_call_t *call = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_mep_client_t *mep_client = NULL;
    axis2_msg_info_headers_t *msg_info_headers = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_conf_t *conf = NULL;
    axis2_msg_ctx_t *response_ctx = NULL;
    axis2_svc_t *svc = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, node, AXIS2_FAILURE);
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);

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

    /* create call struct */
    call = axis2_call_create(env, NULL, client_home);
    mep_client = AXIS2_CALL_GET_BASE(call, env);

    /* Prepare the SOAP envelope, using the SOAP message content to be sent.
     * Get a reference to the message context */
    msg_ctx = AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(mep_client, env, node);
    if (!msg_ctx)
    {
        printf("ERROR: Could not prepare message context. ");
        printf("May be you havent set the repository corretly.\n");
        return AXIS2_FAILURE;
    }

    /* Get the reference to message info headers structure from the message context. 
       This can be used to manipulate SOAP header content when using WS-Addressing. */
    msg_info_headers = AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, env);

    /* create an axis2_endpoint_ref_t struct with ERP assigned */
    endpoint_ref = axis2_endpoint_ref_create(env, diclient_impl->address);

    /* Set header parameters, required for WS-Addressing. 
     * Required only if you need to make use of WS-Addressing.
     */
    AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, endpoint_ref);
    AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, diclient_impl->wsa_action); 
    
    AXIS2_CALL_SET_TO(call, env, endpoint_ref);

    /* Get the configuration context */
    conf = AXIS2_CONF_CTX_GET_CONF(
				AXIS2_SVC_CTX_GET_CONF_CTX(
                		AXIS2_MEP_CLIENT_GET_SVC_CTX(mep_client, env), 
                        env), 
                env);

	/* svc = axis2_svc_create_with_wsdl_svc(env, wsdl_svc);*/
    svc = AXIS2_OP_GET_PARENT(op, env);
	/* echo service is not in the configuration context. We need to create the 
     * operation and add it to service context. Then add service context into 
     * configuration context.
     */
    AXIS2_SVC_ADD_OP(svc, env, op);
    AXIS2_CONF_ADD_SVC(conf, env, svc);

   /* Invoke the operation. Client blocks until the response message comes. 
    * Response message gets set in the response message context.
    */
    response_ctx = AXIS2_CALL_INVOKE_BLOCKING(call, env, op, msg_ctx);

    if (response_ctx)
    {
        /* Get the response SOAP message from response message context */
        axis2_soap_envelope_t *soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(response_ctx, env);
        ret_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, env);
    }
                                                        
    if(ret_node)
    {
        /* Get the response value from the SOAP message */
        axis2_xml_writer_t *writer = NULL;
        axis2_om_output_t *om_output = NULL;
        axis2_char_t *buffer = NULL;
        
        printf("\necho stub invoke SUCCESSFUL!\n");
        writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0,
					AXIS2_XML_PARSER_TYPE_BUFFER);
        om_output = axis2_om_output_create (env, writer);

        AXIS2_OM_NODE_SERIALIZE (ret_node, env, om_output);
        buffer = (axis2_char_t*)AXIS2_XML_WRITER_GET_XML(writer, env);
        printf ("\nReceived OM node in XML : %s\n", buffer);
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke failed: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("echo stub invoke failed!\n");
        return AXIS2_FAILURE;
    }
    
    if (call)
    {
        AXIS2_CALL_FREE(call, env);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t *AXIS2_CALL
axis2_diclient_set_address_and_action_for_op(axis2_diclient_t *diclient,
                                        const axis2_env_t *env,
                                        axis2_qname_t *op_qname)
{
    axis2_diclient_impl_t *diclient_impl = NULL;
    axis2_op_t *op = NULL;
    axis2_svc_t *svc = NULL;
    axis2_hash_t *wsdl_endpoints = NULL;
    axis2_wsdl_binding_t *wsdl_binding = NULL;
    axis2_char_t *op_name = NULL;
    axis2_linked_list_t *ext_elements = NULL;
    axis2_wsdl_ext_soap_address_t *soap_address = NULL;
    axis2_wsdl_ext_soap_op_t *soap_op = NULL;
    axis2_char_t *address = NULL;
    axis2_hash_t *binding_ops = NULL;
    axis2_hash_index_t *index = NULL;
    axis2_wsdl_binding_op_t *wsdl_binding_op = NULL;
    axis2_char_t *wsa_action = NULL;
    int i = 0, size = 0;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_qname, NULL);
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);

    op_name = AXIS2_QNAME_GET_LOCALPART(op_qname, env);
    op = axis2_hash_get(diclient_impl->op_map, op_name, AXIS2_HASH_KEY_STRING);
    svc = AXIS2_OP_GET_PARENT(op, env);
    wsdl_endpoints = AXIS2_SVC_GET_ENDPOINTS(svc, env);
    for (index = axis2_hash_first (wsdl_endpoints, env); index; 
            index = axis2_hash_next (env, index))
    {
        void *value = NULL;
        axis2_wsdl_endpoint_t *wsdl_endpoint = NULL;

        axis2_hash_this(index, NULL, NULL, &value);
        wsdl_endpoint = (axis2_wsdl_endpoint_t *) value;
        ext_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
                wsdl_endpoint->wsdl_component, env);
        size = AXIS2_LINKED_LIST_SIZE(ext_elements, env);
        for(i = 0; i < size; i++)
        {
            axis2_wsdl_ext_t *wsdl_ext = NULL;
            axis2_char_t *namespc = NULL;
            
            wsdl_ext = AXIS2_LINKED_LIST_GET(ext_elements, env, i);
            if(!wsdl_ext) return AXIS2_FAILURE;
            if(0 == AXIS2_STRCMP(AXIS2_WSDL4C_SOAP_BINDING_URI, wsdl_ext->namespc))
            {
                soap_address = (axis2_wsdl_ext_soap_address_t *) wsdl_ext;
                address = AXIS2_WSDL_EXT_SOAP_ADDRESS_GET_LOCATION_URI(
                    soap_address, env);
                diclient_impl->address = AXIS2_STRDUP(address, env);
                if(!diclient_impl->address) return AXIS2_FAILURE;
                printf("address:%s\n", address);
                printf("mep:%s\n", AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env));
            }
        }
        wsdl_binding = AXIS2_WSDL_ENDPOINT_GET_BINDING(wsdl_endpoint, env);
        binding_ops = AXIS2_WSDL_BINDING_GET_BINDING_OPS(wsdl_binding, env);
        for (index = axis2_hash_first (binding_ops, env); index; 
                index = axis2_hash_next (env, index))
        {
            axis2_wsdl_op_t *op_x = NULL;
            axis2_qname_t *op_qname_x = NULL;
            axis2_char_t *op_name_x = NULL;
            void *value = NULL;

            axis2_hash_this(index, NULL, NULL, &value);
            wsdl_binding_op = (axis2_wsdl_binding_op_t *) value;
            op_x = AXIS2_WSDL_BINDING_OP_GET_OP(wsdl_binding_op, env);
            op_qname_x = AXIS2_WSDL_OP_GET_QNAME(op_x, env);
            op_name_x = AXIS2_QNAME_GET_LOCALPART(op_qname_x, env);
            if(0 == AXIS2_STRCMP(op_name, op_name_x))
                break;
        }
        ext_elements = AXIS2_WSDL_COMPONENT_GET_EXTENSIBILITY_ELEMENTS(
            wsdl_binding_op->extensible_component->wsdl_component, env);
        size = AXIS2_LINKED_LIST_SIZE(ext_elements, env);
        for(i = 0; i < size; i++)
        {
            axis2_wsdl_ext_t *wsdl_ext = NULL;
            axis2_char_t *namespc = NULL;
            
            wsdl_ext = AXIS2_LINKED_LIST_GET(ext_elements, env, i);
            if(!wsdl_ext) return AXIS2_FAILURE;
            if(0 == AXIS2_STRCMP(AXIS2_WSDL4C_SOAP_BINDING_URI, wsdl_ext->namespc))
            {
                soap_op = (axis2_wsdl_ext_soap_op_t *) wsdl_ext;
                wsa_action = AXIS2_WSDL_EXT_SOAP_OP_GET_SOAP_ACTION(soap_op, env);
                diclient_impl->wsa_action = AXIS2_STRDUP(wsa_action, env);
                if(!diclient_impl->wsa_action) return AXIS2_FAILURE;
                printf("wsa_action:%s\n", wsa_action);
            }
        }
    }
    return AXIS2_SUCCESS;
}

axis2_hash_t *AXIS2_CALL
axis2_diclient_get_endpoints(axis2_diclient_t *diclient,
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(diclient)->endpoint_map;
}

axis2_hash_t *AXIS2_CALL
axis2_diclient_get_services(axis2_diclient_t *diclient,
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(diclient)->svc_map;
}

axis2_hash_t *AXIS2_CALL
axis2_diclient_get_operations(axis2_diclient_t *diclient,
                                const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(diclient)->op_map;
}

axis2_char_t *AXIS2_CALL
axis2_diclient_get_param_localname(axis2_diclient_t *diclient,
                            const axis2_env_t *env,
                            axis2_qname_t *op_qname)
{
    axis2_diclient_impl_t *diclient_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);
    
    if(!diclient_impl->param_name)
    {
        axis2_diclient_process_param(diclient, env, op_qname);
    }
    return AXIS2_INTF_TO_IMPL(diclient)->param_name;
}

axis2_char_t *AXIS2_CALL
axis2_diclient_get_param_namespace(axis2_diclient_t *diclient,
                            const axis2_env_t *env,
                            axis2_qname_t *op_qname)
{
    axis2_diclient_impl_t *diclient_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);

    if(!diclient_impl->param_namespc)
    {
        axis2_diclient_process_param(diclient, env, op_qname);
    }
    return AXIS2_INTF_TO_IMPL(diclient)->param_namespc;
}

static axis2_status_t
axis2_diclient_process_param(axis2_diclient_t *diclient,
                                const axis2_env_t *env,
                                axis2_qname_t *op_qname)
{    
    axis2_diclient_impl_t *diclient_impl = NULL;
    axis2_op_t *op = NULL;
    axis2_char_t *op_name = NULL;
    axis2_wsdl_msg_ref_t *input_msg = NULL;
    axis2_qname_t *element_qname = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, op_qname, NULL);
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);

    op_name = AXIS2_QNAME_GET_LOCALPART(op_qname, env);
    op = axis2_hash_get(diclient_impl->op_map, op_name, AXIS2_HASH_KEY_STRING);
    input_msg = AXIS2_OP_GET_INPUT_MSG(op, env);
    element_qname = AXIS2_WSDL_MSG_REF_GET_ELEMENT(input_msg, env);
    diclient_impl->param_namespc = AXIS2_QNAME_GET_URI(element_qname, env);
    diclient_impl->param_name = AXIS2_QNAME_GET_LOCALPART(element_qname, env);
    printf("element_name:%s\n", diclient_impl->param_name, env);
    printf("element_namespc:%s\n", diclient_impl->param_namespc, env);
    return AXIS2_SUCCESS;
}
                                
axis2_hash_t *AXIS2_CALL
axis2_diclient_get_params(axis2_diclient_t *diclient,
                            const axis2_env_t *env,
                            axis2_qname_t *op_qname)
{
    axis2_diclient_impl_t *diclient_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    diclient_impl = AXIS2_INTF_TO_IMPL(diclient);

    /* TODO if not wsdl2 handle multiple params */
    return NULL;
}
