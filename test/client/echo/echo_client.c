#include "axis2_echo_stub.h"
#include <axis2_om_stax_builder.h>
#include <axis2_om_document.h>
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_stream_default.h>
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

axis2_soap_envelope_t *
build_soap_programatically(axis2_env_t **env);

int main(void)
{
    axis2_stub_t *stub = NULL;
    axis2_soap_envelope_t *envelope = NULL;
    axis2_om_node_t *node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_endpoint_ref_t *epr = NULL;
    axis2_char_t *address = NULL;
    
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL);
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_INFO;

    envelope = build_soap_programatically(&env);
    node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(envelope, &env);
    address = "http://localhost:9090/echo";
    epr = axis2_endpoint_ref_create(&env, address);
    stub = axis2_echo_stub_create_with_endpoint_ref(&env, epr);
    AXIS2_STUB_SET_TRANSPORT_INFO(stub, &env, AXIS2_TRANSPORT_HTTP, 
        AXIS2_TRANSPORT_HTTP, AXIS2_FALSE);
    /* create node and invoke echo */
    status = axis2_echo_stub_echo(stub, &env, node);
    if(AXIS2_SUCCESS != status)
    {
        printf("echo stub invoke failed!\n");
    }
    else
    {
        printf("echo stub invoke successful!\n");
    }
    AXIS2_SOAP_ENVELOPE_FREE(envelope, &env);
    return status;
}

axis2_soap_envelope_t *
build_soap_programatically(axis2_env_t **env)
{
    axis2_soap_envelope_t *soap_envelope = NULL;
    axis2_soap_body_t *soap_body = NULL;
    axis2_om_node_t *envelope_base_node = NULL;
    axis2_om_element_t *envelope_om_ele = NULL;
    axis2_om_node_t *echo_om_node = NULL;
    axis2_om_element_t* echo_om_ele = NULL;
    axis2_om_node_t* text_om_node = NULL;
    axis2_om_element_t * text_om_ele = NULL;
    

    axis2_om_node_t *body_om_node = NULL;
    axis2_om_namespace_t *env_ns = NULL;
    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;
    axis2_om_namespace_t *wsa_ns = NULL;

    env_ns = axis2_om_namespace_create(env, "http://www.w3.org/2003/05/soap-envelope", "soapenv");
    soap_envelope = axis2_soap_envelope_create(env, env_ns);
    
    
    wsa_ns = axis2_om_namespace_create(env, "http://schemas.xmlsoap.org/ws/2004/08/addressing", "wsa");

    envelope_base_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, env);
    envelope_om_ele = (axis2_om_element_t*)AXIS2_OM_NODE_GET_DATA_ELEMENT(envelope_base_node, env);
    AXIS2_OM_ELEMENT_DECLARE_NAMESPACE(envelope_om_ele, env, envelope_base_node, 
        wsa_ns);
    
    soap_body = axis2_soap_body_create_with_parent(env, soap_envelope);
    
    body_om_node = AXIS2_SOAP_BODY_GET_BASE_NODE(soap_body, env);
    
    echo_om_ele = axis2_om_element_create(env, body_om_node, "echoString", NULL, &echo_om_node);
    
    text_om_ele = axis2_om_element_create(env, echo_om_node, "text", NULL, &text_om_node);

    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, "echo5", text_om_node);
    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_SOAP_ENVELOPE_SERIALIZE(soap_envelope, env, om_output, AXIS2_FALSE);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("%s \n",  buffer); 

    return soap_envelope;
}
