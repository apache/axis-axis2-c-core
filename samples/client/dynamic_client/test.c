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
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_text.h>
#include <axis2_stream.h>
#include <axis2_log_default.h>
#include <axis2_error_default.h>
#include <axis2_svc_client.h>
#include <stdlib.h>
#include <stdio.h>

axiom_node_t *
build_om_programatically(axutil_env_t *env);

int main(int argc, char** argv)
{
    axutil_env_t *env = NULL;
    axis2_error_t *error = NULL;
    axis2_log_t *log = NULL;
    axutil_allocator_t *allocator = NULL;

    /* set up the envioronment with allocator and log*/
    allocator = axutil_allocator_init(NULL);
    error = axis2_error_create(allocator);
    log = axis2_log_create(allocator, NULL, "addr_echo.log");
    env = axutil_env_create_with_error_log(allocator, error, log);
    env->log->level = AXIS2_LOG_LEVEL_TRACE;
    axis2_error_init();


    build_om_programatically(env);
    return 0;
}

/* build SOAP request message content using OM */
axiom_node_t *
build_om_programatically(axutil_env_t *env)
{
    axiom_node_t *add_om_node = NULL;
    axiom_element_t* add_om_ele = NULL;
    axiom_node_t *arg0_om_node = NULL;
    axiom_element_t* arg0_om_ele = NULL;
    axiom_node_t *arg1_om_node = NULL;
    axiom_element_t* arg1_om_ele = NULL;
    axiom_node_t *seq_om_node = NULL;
    axiom_element_t* seq_om_ele = NULL;
    axiom_node_t *cmplx_type_om_node = NULL;
    axiom_element_t* cmplx_type_om_ele = NULL;
    axiom_namespace_t *ns1 = NULL;
    axiom_attribute_t *attr = NULL;
    axiom_attribute_t *attr_name_arg0 = NULL;
    axiom_attribute_t *attr_name_arg1 = NULL;


    axiom_xml_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;
    axis2_char_t *ns = NULL;

    ns = axis2_strdup(env, "http://localhost/axis/Calculator");
    ns1 = axiom_namespace_create(env, ns, "tns0");

    add_om_ele = axiom_element_create(env, NULL, "add", ns1, &add_om_node);
    cmplx_type_om_ele = axiom_element_create(env, add_om_node, "complexType", ns1, &cmplx_type_om_node);
    seq_om_ele = axiom_element_create(env, cmplx_type_om_node, "sequence", ns1, &seq_om_node);
    attr = axiom_attribute_create(env, "type", "xsd:int", ns1);
    attr_name_arg0 = axiom_attribute_create(env, "name", "arg_0_0", ns1);
    attr_name_arg1 = axiom_attribute_create(env, "name", "arg_1_0", ns1);
    arg0_om_ele = axiom_element_create(env, seq_om_node, "arg_0_0", ns1, &arg0_om_node);
    axiom_element_set_text(arg0_om_ele, env, "5", arg0_om_node);
    axiom_element_add_attribute(arg0_om_ele, env, attr, arg0_om_node);
    axiom_element_add_attribute(arg0_om_ele, env, attr_name_arg0, arg0_om_node);
    arg1_om_ele = axiom_element_create(env, seq_om_node, "arg_1_0", ns1, &arg1_om_node);
    axiom_element_set_text(arg1_om_ele, env, "10", arg1_om_node);
    axiom_element_add_attribute(arg1_om_ele, env, attr_name_arg1, arg1_om_node);
    axiom_element_add_attribute(arg1_om_ele, env, attr, arg1_om_node);

    xml_writer = axiom_xml_writer_create_for_memory(env, NULL, AXIS2_FALSE,
            AXIS2_FALSE, AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create(env, xml_writer);

    AXIOM_NODE_SERIALIZE(add_om_node, env, om_output);
    buffer = AXIOM_XML_WRITER_GET_XML(xml_writer, env);
    printf("\nOM node in XML : %s \n",  buffer);
    AXIS2_FREE(env->allocator, buffer);
    axiom_output_free(om_output, env);

    return add_om_node;
}
