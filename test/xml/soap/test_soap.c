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

FILE *f = NULL;

int read_soap(char *buffer, int size, void *ctx)
{
    return fread(buffer, sizeof(char), size, f); 
}

int printnode(axis2_om_node_t *om_node, axis2_env_t **env)
{
    axis2_om_element_t *om_ele = NULL;
    axis2_char_t *localname = NULL;
    AXIS2_FUNC_PARAM_CHECK(om_node, env, AXIS2_FAILURE);
    if(AXIS2_OM_NODE_GET_NODE_TYPE(om_node, env) == AXIS2_OM_ELEMENT)
    {
        om_ele = (axis2_om_element_t *)AXIS2_OM_NODE_GET_DATA_ELEMENT(om_node, env);
        localname = AXIS2_OM_ELEMENT_GET_LOCALNAME(om_ele, env);
        printf("\n %s \n", localname);
    }
    return 0;
}

int build_soap(axis2_env_t **env, char *filename)
{
    axis2_om_document_t *om_doc = NULL;
    axis2_om_stax_builder_t *om_builder = NULL;
    axis2_xml_reader_t *xml_reader = NULL;
    axis2_soap_builder_t *soap_builder = NULL;
    axis2_soap_envelope_t *soap_envelope = NULL;
    axis2_om_node_t *om_node = NULL;
    axis2_char_t *buffer = NULL;
    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_soap_body_t *soap_body = NULL;
    axis2_soap_header_t *soap_header = NULL;
    
    
    
    if(!filename)
        return -1;
    f = fopen(filename, "r");
    if(!f)
        return -1;
    
    xml_reader = axis2_xml_reader_create_for_memory(env, read_soap, NULL, NULL);
    
    om_builder = axis2_om_stax_builder_create(env, xml_reader);
    
    om_doc = axis2_om_document_create(env, NULL, om_builder);
    
    soap_builder = axis2_soap_builder_create(env, om_builder, AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
    if(soap_builder)    
        printf("soap version %d", AXIS2_SOAP_BUILDER_GET_SOAP_VERSION(soap_builder, env) );
    soap_envelope = AXIS2_SOAP_BUILDER_GET_SOAP_ENVELOPE(soap_builder, env);
          
    om_node = AXIS2_SOAP_ENVELOPE_GET_BASE_NODE(soap_envelope, env);
    
    printnode(om_node, env);
    
    soap_header = AXIS2_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
    om_node = AXIS2_SOAP_HEADER_GET_BASE_NODE(soap_header, env);
    printnode(om_node, env);
    
    soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
    if (soap_body)
    {
        om_node = AXIS2_SOAP_HEADER_GET_BASE_NODE(soap_body, env);
        if(!om_node)
            printf("om_node null");
        printnode(om_node, env);
    }
    else
    {
        printf ("\n\n ERROR soap_body NULL.\n\n");
        return AXIS2_FAILURE;
    }
    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);  
    AXIS2_SOAP_ENVELOPE_SERIALIZE(soap_envelope, env, om_output, AXIS2_FALSE);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
/*    printf("%s \n",  buffer); */
    return AXIS2_SUCCESS;
}


int build_soap_programatically(axis2_env_t **env)
{
    axis2_soap_envelope_t *soap_envelope = NULL;
    axis2_soap_body_t *soap_body = NULL;
    axis2_soap_header_t *soap_header = NULL;
    axis2_soap_fault_t *soap_fault = NULL;
    axis2_om_namespace_t *env_ns = NULL;
    axis2_xml_writer_t *xml_writer = NULL;
    axis2_om_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;

    env_ns = axis2_om_namespace_create(env, "http://www.w3.org/2003/05/soap-envelope", "env");
    soap_envelope = axis2_soap_envelope_create(env, env_ns);
    
    soap_header = axis2_soap12_header_create_with_parent(env, soap_envelope);
    soap_body = axis2_soap_body_create_with_parent(env, soap_envelope);

    soap_fault = axis2_soap_fault_create_with_parent(env, soap_body);
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_SOAP_ENVELOPE_SERIALIZE(soap_envelope, env, om_output, AXIS2_FALSE);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("%s \n",  buffer);    
    return AXIS2_SUCCESS;
}


int main(int argc, char *argv[])
{
    axis2_env_t *env = NULL;
    axis2_allocator_t *allocator = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    char *filename = "test.xml";
    if(argc > 1)
        filename = argv[1];
    allocator = axis2_allocator_init (NULL);
    log = axis2_log_create(allocator, NULL);
    error = axis2_error_create(allocator);
    
    env = axis2_env_create_with_error_log(allocator, error,  log);
    build_soap_programatically(&env);
    build_soap(&env, filename); 
    return 0;        
}






