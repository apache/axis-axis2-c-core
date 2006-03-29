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
#include <axis2_om_node.h>
#include <axis2_om_element.h>
#include <axis2_om_text.h>
#include <axis2_wsdl4c_parser.h>
#include <axis2_stream.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axis2_di_client.h>
#include <stdlib.h>
#include <stdio.h>

axis2_om_node_t *
build_om_programatically(axis2_env_t **env);

int main(int argc, char** argv)
{
	axis2_status_t status = AXIS2_FAILURE;
	void *wp = NULL;
	axis2_om_node_t *node = NULL;
	axis2_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axis2_allocator_t *allocator = NULL;
    char *wsdl_file_name = NULL;
    axis2_di_client_t *diclient = NULL;

    /* set up the envioronment with allocator and log*/
    allocator = axis2_allocator_init (NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "addr_echo.log");
    env = axis2_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_TRACE;
    axis2_error_init();

	if (argc <= 1)
    {
      printf("Usage: print <wsdl_File_Name>\n");
      return 1;
    }
    /* build the SOAP request message content using OM API.*/
    node = build_om_programatically(&env);
    wsdl_file_name = argv[1];
    diclient = axis2_di_client_create(&env);
	status = AXIS2_DI_CLIENT_INVOKE(diclient, &env, wsdl_file_name, node);
	if(AXIS2_SUCCESS == status)
		printf("status:%s\n", "Success");
	else
		printf("status:%s\n", "Failure");
	return 0;
}

/* build SOAP request message content using OM */
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

    ns1 = axis2_om_namespace_create (env, "http://example.org/echo", "tns1");

    echo_om_ele = axis2_om_element_create(env, NULL, "echoIn", ns1, &echo_om_node);
    
    AXIS2_OM_ELEMENT_SET_TEXT(echo_om_ele, env, "Hello World", echo_om_node);
    
    
    xml_writer = axis2_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE, AXIS2_FALSE);
    om_output = axis2_om_output_create( env, xml_writer);
    
    AXIS2_OM_NODE_SERIALIZE(echo_om_node, env, om_output);
    buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);         
    printf("\nSending OM node in XML : %s \n",  buffer); 
    AXIS2_FREE((*env)->allocator, buffer);
    AXIS2_OM_OUTPUT_FREE(om_output, env);

    return echo_om_node;
}
