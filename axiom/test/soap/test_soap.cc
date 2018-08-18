
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

#include <gtest/gtest.h>

#include <axiom_stax_builder.h>
#include <axiom_document.h>
#include <axiom_node.h>
#include <axiom_element.h>
#include <axiom_text.h>
#include <axutil_stream.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_xml_reader.h>
#include <stdio.h>
#include <axiom_xml_writer.h>
#include <axiom_soap_builder.h>
#include <axiom_soap_const.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>
#include <axiom_soap_header.h>
#include <axiom_soap_header_block.h>
#include <axiom_soap_fault.h>
#include <axiom_soap_fault_code.h>
#include <axiom_soap_fault_role.h>
#include <axiom_soap_fault_value.h>
#include <axiom_soap_fault_detail.h>
#include <axiom_soap_fault_role.h>
#include <axiom_soap_fault_node.h>

FILE *f = NULL;
int
read_soap(
    char *buffer,
    int size,
    void *ctx)
{
    int len = 0;
    char *pos = NULL;
    len = fread(buffer, sizeof(char), size, f);
    if (buffer)
        pos = strstr(buffer, "---");
    if (pos)
    {
        len = pos - buffer;
        *pos = '\0';
    }
    printf("buffer = %s\n", buffer);
    return len;
}


/*int read_soap(char *buffer, int size, void *ctx)
{
    return fread(buffer, sizeof(char), size, f);
}*/

int
close_soap(
    void *ctx)
{
    fclose(f);
    return AXIS2_TRUE;
}

int
printnode(
    axiom_node_t * om_node,
    const axutil_env_t * env)
{
    axiom_element_t *om_ele = NULL;
    axis2_char_t *localname = NULL;
    axiom_namespace_t *om_ns = NULL;
    axis2_char_t *uri = NULL;
    axis2_char_t *prefix = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (!om_node)
        return AXIS2_FAILURE;

    if (axiom_node_get_node_type(om_node, env) == AXIOM_ELEMENT)
    {
        om_ele = (axiom_element_t *) axiom_node_get_data_element(om_node, env);
        if (!om_ele)
            return AXIS2_FAILURE;

        localname = axiom_element_get_localname(om_ele, env);
        if (localname)
            printf("\n %s \n", localname);
        om_ns = axiom_element_get_namespace(om_ele, env, om_node);

        if (om_ns)
        {
            prefix = axiom_namespace_get_prefix(om_ns, env);
            uri = axiom_namespace_get_uri(om_ns, env);

            printf("\n uri %s \n prefix %s \n", uri, prefix);
        }
    }
    return 0;
}


/* FIXME 
 * These tests exercise code, but don't actually check that the output is
 * correct.  They didn't when they were in the old test format, either.
 */

class TestSOAP: public ::testing::Test
{
    protected:
        void SetUp()
        {
            m_allocator = axutil_allocator_init(NULL);
            m_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);
            m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_log);
        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }

        axutil_env_t *m_env = NULL;
        axutil_allocator_t *m_allocator = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_log = NULL;

};


TEST_F(TestSOAP, test_build_soap) {

    const axis2_char_t *uri = AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    const char *filename = "../resources/xml/soap/test.xml";

    axiom_stax_builder_t *om_builder = NULL;

    axiom_xml_reader_t *xml_reader = NULL;

    axiom_soap_builder_t *soap_builder = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;

    axiom_node_t *om_node = NULL;
    axis2_char_t *buffer = NULL;

    axiom_xml_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;

    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_children_qname_iterator_t *children_iter = NULL;

    int status = AXIS2_SUCCESS;

    f = fopen(filename, "r");
    ASSERT_NE(f, nullptr);

    printf(" \n\n _________ Test SOAP BUILD ___________ \n\n ");

    xml_reader =
        axiom_xml_reader_create_for_io(m_env, read_soap, close_soap, NULL, NULL);

    ASSERT_NE(xml_reader, nullptr);

    om_builder = axiom_stax_builder_create(m_env, xml_reader);
    ASSERT_NE(om_builder, nullptr);

    soap_builder = axiom_soap_builder_create(m_env, om_builder, uri);
    ASSERT_NE(soap_builder, nullptr);

    soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, m_env);
    ASSERT_NE(soap_envelope, nullptr);

    om_node = axiom_soap_envelope_get_base_node(soap_envelope, m_env);
    ASSERT_NE(om_node, nullptr);
    printnode(om_node, m_env);

    soap_header = axiom_soap_envelope_get_header(soap_envelope, m_env);
    ASSERT_NE(soap_header, nullptr);
    om_node = axiom_soap_header_get_base_node(soap_header, m_env);
    ASSERT_NE(om_node, nullptr);
    printnode(om_node, m_env);

    /* FIXME JIRA: AXIS2C-1695 */
    /*
    children_iter =
        axiom_soap_header_examine_all_header_blocks(soap_header, m_env);
    if (children_iter)
    {
        while (axiom_children_iterator_has_next(children_iter, env))
            {
            om_node = axiom_children_iterator_next(children_iter, env);
            if (om_node)
            printnode(om_node, env);
            }
    }
    */

    soap_body = axiom_soap_envelope_get_body(soap_envelope, m_env);
    ASSERT_NE(soap_body, nullptr);
    if (axiom_soap_body_has_fault(soap_body, m_env))
        printf("axiom_soap_body_has_fault\n");
    om_node = axiom_soap_body_get_base_node(soap_body, m_env);
    ASSERT_NE(om_node, nullptr);
    printnode(om_node, m_env);

    if (axiom_soap_body_has_fault(soap_body, m_env))
    {
        printf("\n\nsoap body  has a fault element\n\n\n ");
    }

    om_node = axiom_soap_body_get_base_node(soap_body, m_env);
    ASSERT_NE(om_node, nullptr);
    while (!(axiom_node_is_complete(om_node, m_env)))
    {
        status = axiom_soap_builder_next(soap_builder, m_env);
        ASSERT_NE(status, AXIS2_FAILURE);
    }

    xml_writer =
        axiom_xml_writer_create_for_memory(m_env, NULL, AXIS2_FALSE, AXIS2_FALSE,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);
    ASSERT_NE(xml_writer, nullptr);

    om_output = axiom_output_create(m_env, xml_writer);
    ASSERT_NE(om_output, nullptr);

    axiom_soap_envelope_serialize(soap_envelope, m_env, om_output, AXIS2_FALSE);

    buffer = (axis2_char_t *) axiom_xml_writer_get_xml(xml_writer, m_env);

    printf("\n\nThe serialized xml is >>>>>>>>>>>>>\n\n\n%s \n\n\n", buffer);

    axiom_output_free(om_output, m_env);
    axiom_soap_envelope_free(soap_envelope, m_env);

    printf(" \n __________ END TEST SOAP BUILD ____________ \n");
}


TEST_F(TestSOAP, test_build_programatically) {
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_soap_fault_code_t *fault_code = NULL;
    axiom_soap_header_block_t *hb1 = NULL;

    axiom_namespace_t *env_ns = NULL;
    axiom_namespace_t *test_ns = NULL;
    axiom_namespace_t *role_ns = NULL;

    axiom_xml_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;

    axis2_char_t *buffer = NULL;
    axiom_node_t *hb_node = NULL;
    axiom_element_t *hb_ele = NULL;

    printf(" \n ____________ BUILD SOAP PROGRAMATICALLY _______________ \n");

    env_ns =
        axiom_namespace_create(m_env, "http://www.w3.org/2003/05/soap-envelope",
                               "env");
    if (!env_ns)
    ASSERT_NE(env_ns, nullptr);

    soap_envelope = axiom_soap_envelope_create(m_env, env_ns);
    ASSERT_NE(soap_envelope, nullptr);

    soap_header = axiom_soap_header_create_with_parent(m_env, soap_envelope);
    ASSERT_NE(soap_header, nullptr);

    test_ns =
        axiom_namespace_create(m_env, "http://example.org/ts-tests", "test");

    role_ns =
        axiom_namespace_create(m_env,
                               "http://www.w3.org/2003/05/soap-envelope/role/next",
                               "role");

    hb1 =
        axiom_soap_header_block_create_with_parent(m_env, "echoOk", role_ns,
                                                   soap_header);

    hb_node = axiom_soap_header_block_get_base_node(hb1, m_env);

    hb_ele = (axiom_element_t*) axiom_node_get_data_element(hb_node, m_env);

    axiom_element_set_namespace(hb_ele, m_env, test_ns, hb_node);

    soap_body = axiom_soap_body_create_with_parent(m_env, soap_envelope);

    soap_fault = axiom_soap_fault_create_with_parent(m_env, soap_body);

    fault_code = axiom_soap_fault_code_create_with_parent(m_env, soap_fault);

    xml_writer =
        axiom_xml_writer_create_for_memory(m_env, NULL, AXIS2_FALSE, AXIS2_FALSE,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);

    om_output = axiom_output_create(m_env, xml_writer);

    axiom_soap_envelope_serialize(soap_envelope, m_env, om_output, AXIS2_FALSE);

    buffer = (axis2_char_t *) axiom_xml_writer_get_xml(xml_writer, m_env);

    printf("%s \n", buffer);

    axiom_soap_envelope_free(soap_envelope, m_env);

    axiom_namespace_free(env_ns, m_env);
    axiom_namespace_free(test_ns, m_env);
    /* multiple references requires multiple frees */
    axiom_namespace_free(role_ns, m_env);
    axiom_namespace_free(role_ns, m_env);

    axiom_output_free(om_output, m_env);

    printf("\n __________ END BUILD SOAP PROGRAMATICALLY ____________\n");
}


TEST_F(TestSOAP, test_create_soap_fault) {
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;
    axis2_char_t *exception_text = NULL;
    axis2_status_t status = 0;

    soap_envelope =
        axiom_soap_envelope_create_default_soap_fault_envelope(m_env,
                                                               "Fault Code",
                                                               "Fault Reason",
                                                               AXIOM_SOAP11,
                                                               NULL, NULL);
    soap_body = axiom_soap_envelope_get_body(soap_envelope, m_env);
    soap_fault = axiom_soap_body_get_fault(soap_body, m_env);

    axiom_soap_fault_detail_create_with_parent(m_env, soap_fault);
    axiom_soap_fault_role_create_with_parent(m_env, soap_fault);
    xml_writer =
        axiom_xml_writer_create_for_memory(m_env, NULL, AXIS2_FALSE, AXIS2_FALSE,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create(m_env, xml_writer);
    axiom_soap_envelope_serialize(soap_envelope, m_env, om_output, AXIS2_FALSE);
    buffer = (axis2_char_t *) axiom_xml_writer_get_xml(xml_writer, m_env);
    printf("\n Testing Create soap fault \n %s \n", buffer);

    status = axiom_soap_fault_set_exception(soap_fault, m_env, "MyNewException");
    exception_text = axiom_soap_fault_get_exception(soap_fault, m_env);

    printf(" Testing soap fault set exception \n");
    printf("Actual = %s Expected = %s |", exception_text, "MyNewException");
    ASSERT_EQ(status, AXIS2_SUCCESS);
    ASSERT_STREQ(exception_text, "MyNewException");

    axiom_soap_envelope_free(soap_envelope, m_env);
    axiom_output_free(om_output, m_env);
}

TEST_F(TestSOAP, test_create_soap_fault_with_exception) {
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_soap_fault_detail_t *fault_detail = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    axis2_char_t *exception_text = NULL;

    soap_envelope =
        axiom_soap_envelope_create_default_soap_fault_envelope(m_env,
                                                               "Fault Code",
                                                               "Fault Reason",
                                                               AXIOM_SOAP11,
                                                               NULL, NULL);
    soap_body = axiom_soap_envelope_get_body(soap_envelope, m_env);
    soap_fault =
        axiom_soap_fault_create_with_exception(m_env, soap_body, "MyException");

    fault_detail = axiom_soap_fault_detail_create_with_parent(m_env, soap_fault);
    ASSERT_NE(fault_detail, nullptr);
    axiom_soap_fault_role_create_with_parent(m_env, soap_fault);
    xml_writer =
        axiom_xml_writer_create_for_memory(m_env, NULL, AXIS2_FALSE, AXIS2_FALSE,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create(m_env, xml_writer);
    axiom_soap_envelope_serialize(soap_envelope, m_env, om_output, AXIS2_FALSE);

    exception_text = axiom_soap_fault_get_exception(soap_fault, m_env);
    printf(" Testing Create soap fault with exception \n");
    printf("Actual = %s Expected = %s |", exception_text, "MyException");
    ASSERT_STREQ(exception_text, "MyException");

    axiom_soap_fault_free(soap_fault, m_env);
    axiom_soap_fault_detail_free(fault_detail, m_env);
    axiom_soap_envelope_free(soap_envelope, m_env);
    axiom_output_free(om_output, m_env);
}

TEST_F(TestSOAP, test_soap_fault_node) {
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axis2_char_t *node_text = NULL;
    axiom_soap_fault_node_t *fault_node = NULL;
    axis2_status_t status = 0;

    printf("Testing soap fault node \n");
    soap_envelope =
        axiom_soap_envelope_create_default_soap_fault_envelope(m_env,
                                                               "env:Receiver",
                                                               "Something went wrong!",
                                                               AXIOM_SOAP12,
                                                               NULL, NULL);
    soap_body = axiom_soap_envelope_get_body(soap_envelope, m_env);
    soap_fault = axiom_soap_body_get_fault(soap_body, m_env);

    fault_node = axiom_soap_fault_node_create_with_parent(m_env, soap_fault);
    status = axiom_soap_fault_node_set_value(fault_node, m_env, "MyFaultNode");
    node_text = axiom_soap_fault_node_get_value(fault_node, m_env);

    ASSERT_STREQ(node_text, "MyFaultNode");

    axiom_soap_envelope_free(soap_envelope, m_env);
}


TEST_F(TestSOAP, test_soap_fault_value) {
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_soap_fault_t *soap_fault = NULL;
    axiom_soap_fault_code_t *soap_code = NULL;
    axiom_soap_fault_value_t *value = NULL;
    axis2_char_t *value_text = NULL;

    printf("TEST SOAP FAULT VALUE\n");
    soap_envelope =
        axiom_soap_envelope_create_default_soap_fault_envelope(m_env,
                                                               "env:Receiver",
                                                               "Something went wrong!",
                                                               AXIOM_SOAP12,
                                                               NULL, NULL);
    soap_body = axiom_soap_envelope_get_body(soap_envelope, m_env);
    soap_fault = axiom_soap_body_get_fault(soap_body, m_env);
    soap_code = axiom_soap_fault_get_code(soap_fault, m_env);
    value = axiom_soap_fault_code_get_value(soap_code, m_env);
    value_text = axiom_soap_fault_value_get_text(value, m_env);

    printf("Actual = %s Expected = %s |", value_text, "env:Receiver");
    ASSERT_STREQ(value_text, "env:Receiver");

    axiom_soap_envelope_free(soap_envelope, m_env);
}

