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

axis2_om_node_t *
build_om_programatically(axis2_env_t **env);

int main(void)
{
    axis2_stub_t *stub = NULL;
    axis2_om_node_t *node = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_char_t *address = NULL;
    axis2_char_t *client_home = NULL;
    axis2_om_node_t *ret_node = NULL;
    
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL);
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_INFO;
    axis2_error_init();

    client_home = AXIS2_GETENV("AXIS2C_HOME");

    node = build_om_programatically(&env);
    address = "http://localhost:9090/axis2/services/echo/";
    stub = 
        axis2_stub_create_with_endpoint_uri_and_client_home(&env, address,
            client_home);
    AXIS2_STUB_SET_TRANSPORT_INFO(stub, &env, AXIS2_TRANSPORT_HTTP, 
        AXIS2_TRANSPORT_HTTP, AXIS2_FALSE);
    /* create node and invoke echo */
    ret_node = axis2_echo_stub_echo(stub, &env, node);
    if(ret_node)
    {
        printf("echo stub invoke successful!\n");
    }
    else
    {
		AXIS2_LOG_ERROR(env->log, LOG_SI, "Server start failed: Error code:"
						" %d :: %s", env->error->error_number,
                        AXIS2_ERROR_GET_MESSAGE(env->error));
        printf("echo stub invoke failed!\n");
    }
    
    if (node)
    {
        AXIS2_OM_NODE_FREE_TREE(node, &env);
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
    

    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    echo_om_ele = axis2_om_element_create(env, NULL, "echoString", NULL, &echo_om_node);
    
    text_om_ele = axis2_om_element_create(env, echo_om_node, "text", NULL, &text_om_node);

    AXIS2_OM_ELEMENT_SET_TEXT(text_om_ele, env, "echo5", text_om_node);
    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(echo_om_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("%s \n",  buffer); 

    return echo_om_node;
}

