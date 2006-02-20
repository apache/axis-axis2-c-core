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
#include <axis2_om_node.h>
#include <axis2_msg_ctx.h>
#include <axis2_call.h>
#include <axis2_platform_auto_sense.h>

axis2_om_node_t *
build_soap_body_content(axis2_env_t **env, axis2_char_t *operation, axis2_char_t *google_key, 
                        axis2_char_t *word_to_spell, axis2_om_node_t *body_node);

void print_invalid_om(axis2_env_t **env, axis2_om_node_t *ret_node);

int main(int argc, char** argv)
{
    axis2_om_node_t *node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_char_t *address = NULL;
    axis2_char_t *client_home = NULL;
    axis2_om_node_t *ret_node = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    axis2_call_t *call = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_mep_client_t *mep_client = NULL;
    axis2_endpoint_ref_t* endpoint_ref = NULL;
    axis2_conf_t *conf = NULL;
    axis2_msg_ctx_t *response_ctx = NULL;
    axis2_soap_envelope_t *soap_envelope = NULL;
    axis2_soap_body_t *soap_body = NULL;
    axis2_om_namespace_t *env_ns = NULL;
    axis2_om_node_t *body_node = NULL;
    axis2_char_t *operation = NULL;
    axis2_char_t *google_key = NULL;
    axis2_char_t *word_to_spell = NULL;
    allocator = axis2_allocator_init (NULL);

    operation = "doSpellingSuggestion";
    google_key = "00000000000000000000000000000000";
    word_to_spell = "salvasion";
    
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "google_client.log");
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_INFO;
    axis2_error_init();

    client_home = AXIS2_GETENV("AXIS2C_HOME");
    if (!client_home)
        client_home = "../../deploy";
    
    address = "http://api.google.com/search/beta2";
    
    if ( (argc > 1) && (AXIS2_STRCMP("-h", argv[1]) == 0) )
    {
        printf("\nUsage : %s [google_key] [word_to_spell] \n", argv[0]);
        printf("\tgoogle_key Your Google license key. Default value won't work. You must use your key here.\n");
        printf("\tword_to_spell Word to be spelled by Google service. Default is %s\n", word_to_spell);
        printf("NOTE: command line arguments must appear in given order, with trailing ones being optional\n");
        printf("\tUse -h for help\n");
        return 0;
    }

    if (argc > 1 )
        google_key = argv[1];
    if (argc > 2 )
        word_to_spell = argv[2];

    printf ("Using endpoint : %s\n", address);
    printf ("\nInvoking operation %s with params %s and %s\n", operation, google_key, word_to_spell);


    /* create call without passing svc_ctx_t struct */
    call = axis2_call_create(&env, NULL, client_home);
    mep_client = AXIS2_CALL_GET_BASE(call, &env);
    /* prepare SOAP envelope */
    env_ns = axis2_om_namespace_create(&env, AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI, "SOAP-ENV"); 
    soap_envelope = axis2_soap_envelope_create(&env, env_ns);
    soap_body = axis2_soap_body_create_with_parent(&env, soap_envelope);
    body_node = AXIS2_SOAP_BODY_GET_BASE_NODE(soap_body, &env);
    build_soap_body_content(&env, operation, google_key, word_to_spell, body_node);

    msg_ctx = AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(mep_client, &env, node);
    AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, &env, soap_envelope);
    endpoint_ref = axis2_endpoint_ref_create(&env, address);
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
        axis2_qname_t *svc_qname = axis2_qname_create(&env, "google", NULL, NULL);
        svc = axis2_svc_create_with_qname(&env, svc_qname);
        op_qname = axis2_qname_create(&env, "doSpellingSuggestion", NULL, NULL);
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
    response_ctx = AXIS2_CALL_INVOKE_BLOCKING(call, &env, op, msg_ctx);

    if (response_ctx)
    {
        axis2_soap_envelope_t *soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(response_ctx, &env);
        ret_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, &env);
    }

    if(ret_node)
    {        
        if (AXIS2_OM_NODE_GET_NODE_TYPE(ret_node, &env) == AXIS2_OM_ELEMENT)
        {
            axis2_char_t *result = NULL;
            axis2_om_element_t *result_ele = NULL;
            axis2_om_node_t *ret_node1 = NULL;
            ret_node1 = AXIS2_OM_NODE_GET_FIRST_CHILD(ret_node, &env); /*Body*/
            if (!ret_node1)
            {
                print_invalid_om(&env, ret_node);
                return AXIS2_FAILURE;
            }
            ret_node = AXIS2_OM_NODE_GET_FIRST_CHILD(ret_node1, &env); /*doSpellingSuggestionResponse*/
            if (!ret_node)
            {
                print_invalid_om(&env, ret_node1);
                return AXIS2_FAILURE;
            }
            /*see if we have got a fault*/
            result_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(ret_node, &env);
            if (AXIS2_STRCMP(AXIS2_OM_ELEMENT_GET_LOCALNAME(result_ele, &env), "doSpellingSuggestionResponse") != 0 )
            {
                print_invalid_om(&env, ret_node1);
                return AXIS2_FAILURE;
            }
            
            ret_node1 = AXIS2_OM_NODE_GET_FIRST_CHILD(ret_node, &env); /*return*/
            if (!ret_node1)
            {
                print_invalid_om(&env, ret_node);
                return AXIS2_FAILURE;
            }
            result_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(ret_node1, &env);
            printf(AXIS2_OM_ELEMENT_GET_LOCALNAME(result_ele, &env)); 
            result = AXIS2_OM_ELEMENT_GET_TEXT(result_ele, &env, ret_node1);
            printf( "\nResult = %s\n", result);
        }
        else
        {
            print_invalid_om(&env, ret_node);
            return AXIS2_FAILURE;
        }
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Stub invoke failed: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("google stub invoke failed!\n");
    }
    
    /*if (node)
    {
        AXIS2_OM_NODE_FREE_TREE(node, &env);
    }*/
    return status;
}

axis2_om_node_t *
build_soap_body_content(axis2_env_t **env, axis2_char_t *operation, axis2_char_t *google_key, 
                        axis2_char_t *word_to_spell, axis2_om_node_t *body_node)
{
    axis2_om_node_t* envelope_node = NULL;
    axis2_om_element_t* envelope_element = NULL;
    axis2_om_node_t *google_om_node = NULL;
    axis2_om_element_t* google_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    axis2_om_namespace_t *ns0 = NULL, *ns1 = NULL, *ns2 = NULL, *ns3 = NULL;
    
    axis2_om_attribute_t* attri1 = NULL;

    axis2_xml_writer_t *xml_writer = NULL;
    axis2_xml_writer_t *writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    ns1 = axis2_om_namespace_create (env, "urn:GoogleSearch", "ns1");
    ns2 = axis2_om_namespace_create (env, "http://www.w3.org/1999/XMLSchema-instance", "xsi");
    ns3 = axis2_om_namespace_create (env, "http://www.w3.org/1999/XMLSchema", "xsd");

    envelope_node = AXIS2_OM_NODE_GET_PARENT(body_node, env);
    envelope_element = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(envelope_node, env);
    AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(envelope_element, env, envelope_node, ns2);
    AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(envelope_element, env, envelope_node, ns3);
    ns0 = AXIS2_OM_ELEMENT_GET_NAMESPACE(envelope_element, env);
    attri1 = axis2_om_attribute_create (env, "encodingStyle", "http://schemas.xmlsoap.org/soap/encoding/", ns0);
    
    google_om_ele = axis2_om_element_create(env, body_node, operation, ns1, &google_om_node);
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(google_om_ele, env, attri1, google_om_node);
    
    text_om_ele = axis2_om_element_create(env, google_om_node, "key", NULL, &text_om_node);
    attri1 = axis2_om_attribute_create (env, "type", "xsd:string", ns2);
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(text_om_ele, env, attri1, text_om_node);

    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, google_key, text_om_node);


    text_om_ele = axis2_om_element_create(env, google_om_node, "phrase", NULL, &text_om_node);
    /*attri1 = axis2_om_attribute_create (env, "type", "xsd:string", ns2);*/
    AXIS2_OM_ELEMENT_ADD_ATTRIBUTE(text_om_ele, env,attri1, text_om_node);

    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, word_to_spell, text_om_node);
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(google_om_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "\nSending OM node in XML : %s \n",  buffer); 

        /*axis2_om_output_t *om_output = NULL;*/
	/*axis2_char_t *buffer = NULL;*/
	writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0);
	om_output = axis2_om_output_create (env, writer);
	AXIS2_OM_NODE_SERIALIZE (google_om_node, env, om_output);
	buffer = AXIS2_XML_WRITER_GET_XML(writer, env);
	printf("%s\n", buffer);
    return google_om_node;
}

void print_invalid_om(axis2_env_t **env, axis2_om_node_t *ret_node)
{

    axis2_xml_writer_t *writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;
    writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_TRUE, 0);
    om_output = axis2_om_output_create (env, writer);

    AXIS2_OM_NODE_SERIALIZE (ret_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(writer, env);
    printf ("\nReceived OM as result : %s\n", buffer);
}
/***************************Function implementation****************************/
/*axis2_om_node_t *AXIS2_CALL
axis2_google_stub_call(axis2_stub_t *stub,
			axis2_env_t **env,
			axis2_om_node_t *node)
{
	axis2_call_t *call = NULL;
	axis2_om_node_t *ret_node = NULL;

	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	call = AXIS2_STUB_GET_CALL_OBJ(stub, env);
	ret_node = AXIS2_CALL_INVOKE_BLOCKING_WITH_OM(call, env, "doSpellingSuggestionResponse", node);

	return ret_node;
}*/


