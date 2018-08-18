
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
#include <axiom_data_source.h>
#include <axutil_stream.h>
#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_xml_reader.h>
#include <stdio.h>
#include <axiom_xml_writer.h>
#include <axutil_env.h>

/* FIXME 
 * These tests exercise code, but don't actually check that the output is
 * correct.  They didn't when they were in the old test format, either.
 */

class TestOM: public ::testing::Test
{

    protected:
        void SetUp()
        {

            m_allocator = axutil_allocator_init(NULL);
            m_axis_log = axutil_log_create(m_allocator, NULL, NULL);
            m_error = axutil_error_create(m_allocator);

            m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_axis_log);

        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }


        axutil_allocator_t *m_allocator = NULL;
        axutil_env_t *m_env = NULL;
        axutil_error_t *m_error = NULL;
        axutil_log_t *m_axis_log = NULL;

};

TEST_F(TestOM, test_om_build) {

    char *filename = "../resources/xml/om/test.xml";

    FILE *f = NULL;
    axiom_element_t *ele1 = NULL;
    axiom_element_t *ele2 = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_text_t *text = NULL;
    axiom_document_t *document = NULL;
    axiom_node_t *node1 = NULL;
    axiom_node_t *node2 = NULL;
    axiom_node_t *node3 = NULL;
    axiom_output_t *om_output = NULL;
    axiom_namespace_t *ns = NULL;
    axiom_xml_reader_t *reader = NULL;
    axiom_xml_writer_t *writer = NULL;

    char *buffer = NULL;
    axutil_hash_t* hash = NULL;

    /** create pull parser */
    reader =
        axiom_xml_reader_create_for_file(m_env, filename, NULL);
    ASSERT_NE(reader, nullptr);

     /** create axiom_stax_builder by parsing pull_parser struct */
    builder = axiom_stax_builder_create(m_env, reader);
    ASSERT_NE(builder, nullptr);

    /**
        create an om document
        document is the container of om model created using builder
    */
    document = axiom_stax_builder_get_document(builder, m_env);
    ASSERT_NE(document, nullptr);

    node1 = axiom_document_get_root_element(document, m_env);
    ASSERT_NE(node1, nullptr);
    if (node1)
    {
        /** print root node information */
        ele1 = (axiom_element_t*)axiom_node_get_data_element(node1, m_env);
        if (ele1)
        {
            printf("root localname %s\n",
                   axiom_element_get_localname(ele1, m_env));

            hash = axiom_element_get_all_attributes(ele1,m_env);
            if(hash)
            {
                axutil_hash_index_t *hi;
                const void *key= NULL;
                void *val = NULL;
                for (hi = axutil_hash_first(hash,m_env); hi; hi = axutil_hash_next(m_env, hi))
                {
                    axutil_hash_this(hi, &key, NULL,&val);
                    if(val)
                    {
                        printf(" Attribute name: %s",
                            axiom_attribute_get_localname((axiom_attribute_t *)val,m_env));
                        printf("   value: %s\n",
                            axiom_attribute_get_value((axiom_attribute_t *)val,m_env));
                    }
                }
            }
        }

        ns = axiom_element_get_namespace(ele1, m_env, node1);

        if (ns)
        {
            printf("root ns prefix %s\n",
                   axiom_namespace_get_prefix(ns, m_env));
            printf("root ns uri %s\n",
                   axiom_namespace_get_uri(ns, m_env));
                        printf("=============================================");

        }
        else
            printf("=============================================");

    }

    /** build the document continuously until all the xml file is built in to a om model */

    node2 = axiom_document_build_next(document, m_env);
    do
    {

        if (!node2)
            break;

        switch (axiom_node_get_node_type(node2, m_env))
        {
        case AXIOM_ELEMENT:
            ele2 =
                (axiom_element_t *) axiom_node_get_data_element(node2,
                                                                m_env);
                    printf("=============================================");
            if (ele2 && axiom_element_get_localname(ele2, m_env))
            {
                printf("\n localname %s\n",
                           axiom_element_get_localname(ele2, m_env));
                hash = axiom_element_get_all_attributes(ele2,m_env);
                if(hash)
                {
                    axutil_hash_index_t *hi;
                    const void *key= NULL;
                    void *val = NULL;
                    for (hi = axutil_hash_first(hash,m_env); hi; hi = axutil_hash_next(m_env, hi))
                    {
                        axutil_hash_this(hi, &key, NULL,&val);
                        if(val)
                        {
                            printf(" Attribute name: %s",
                                axiom_attribute_get_localname((axiom_attribute_t *)val,m_env));
                            printf("   value: %s\n",
                                axiom_attribute_get_value((axiom_attribute_t *)val,m_env));
                        }
                    }
                }

            }



            if (!node3)
                node3 = node2;

            break;
        case AXIOM_TEXT:

            text =
                (axiom_text_t *) axiom_node_get_data_element(node2,
                                                             m_env);
            if (text && axiom_text_get_value(text, m_env))
                printf("\n text value  %s \n",
                       axiom_text_get_value(text, m_env));
            break;

        default:
            break;
        }

        node2 = axiom_document_build_next(document, m_env);
    }
    while (node2);
    printf("END: pull document\n");

    printf("Serialize pulled document\n");

    writer =
        axiom_xml_writer_create_for_memory(m_env, NULL, AXIS2_TRUE, 0,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create(m_env, writer);

    axiom_node_serialize_sub_tree(node3, m_env, om_output);

    buffer = (axis2_char_t *) axiom_xml_writer_get_xml(writer, m_env);

    if (buffer)
        printf("Sub Tree = %s\n", buffer);

    axiom_output_free(om_output, m_env);

    axiom_stax_builder_free(builder, m_env);

    printf("\nend test_om_build\n");
}


TEST_F(TestOM, test_om_serialize) {

    /*
       <book xmlns:axiomc="http://ws.apache.org/axis2/c/om" xmlns:isbn="urn:ISBN:0-395-74341-6">
       <title>Axis2/C OM HOWTO</title>
       <isbn:number>1748491379</isbn:number>
       <author title="Mr" name="Axitoc Oman" />
       <notes>
       <p xmlns="urn:w3-org-ns:HTML">
       This is <i>vey good</i> book on OM!
       </p>
       </notes>
       </book>
     */
    int status;
    axiom_element_t *ele1 = NULL,
        *ele2 = NULL,
        *ele3 = NULL,
        *ele4 = NULL;
    axiom_node_t *node1 = NULL,
        *node2 = NULL,
        *node3 = NULL,
        *node4 = NULL,
        *node5 = NULL,
        *node6 = NULL;
    axiom_data_source_t *data_source = NULL;
    axutil_stream_t *stream = NULL;
    axiom_attribute_t *attr1 = NULL,
        *attr2 = NULL;
    axiom_namespace_t *ns1 = NULL,
        *ns2 = NULL;
    axiom_text_t *text1 = NULL;
    axiom_output_t *om_output = NULL;
    axiom_xml_writer_t *writer = NULL;
    axis2_char_t *output_buffer = NULL;

    printf("\nstart test_om_serialize\n");

    ns1 =
        axiom_namespace_create(m_env,
                               "http://ws.apache.org/axis2/c/om", "axiom");
    ns2 = axiom_namespace_create(m_env, "urn:ISBN:0-395-74341-6", "isbn");
    ele1 = axiom_element_create(m_env, NULL, "book", ns1, &node1);
    axiom_element_declare_namespace(ele1, m_env, node1, ns2);

    ele2 = axiom_element_create(m_env, node1, "title", ns1, &node2);
    attr1 = axiom_attribute_create(m_env, "title22", NULL, NULL);

    axiom_element_add_attribute(ele2, m_env, attr1, node2);

    text1 = axiom_text_create(m_env, node2, "Axis2/C OM HOWTO", &node3);

    ele3 = axiom_element_create(m_env, node1, "number", ns2, &node4);

    text1 = axiom_text_create(m_env, node4, "1748491379", &node5);

    ele4 = axiom_element_create(m_env, node1, "author", ns1, &node6);

    attr1 = axiom_attribute_create(m_env, "title", "Mr", ns1);

    axiom_element_add_attribute(ele4, m_env, attr1, node6);

    attr2 = axiom_attribute_create(m_env, "name", "Axitoc Oman", ns1);

    axiom_element_add_attribute(ele4, m_env, attr2, node6);

    data_source = axiom_data_source_create(m_env, node1, &node6);
    stream = axiom_data_source_get_stream(data_source, m_env);
    if (stream)
    {
        axutil_stream_write(stream, m_env,
                            "<this xmlns:axiom=\"http://ws.apache.org/axis2/c/om\">is a test</this>",
                            axutil_strlen
                            ("<this xmlns:axiom=\"http://ws.apache.org/axis2/c/om\">is a test</this>"));
    }

    /* serializing stuff */
    writer =
        axiom_xml_writer_create_for_memory(m_env, NULL, AXIS2_TRUE, 0,
                                           AXIS2_XML_PARSER_TYPE_BUFFER);
    om_output = axiom_output_create(m_env, writer);

    printf("Serialize built document\n");
    status = axiom_node_serialize(node1, m_env, om_output);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    printf("\naxiom_node_serialize success\n");
    /* end serializing stuff */

    axiom_node_free_tree(node1, m_env);
    output_buffer =
        (axis2_char_t *) axiom_xml_writer_get_xml(writer, m_env);

    axiom_output_free(om_output, m_env);
    /*    if (output_buffer) */
    /*     { */
    /*         printf("%s", output_buffer); */
    /*         AXIS2_FREE(m_env->allocator, output_buffer); */
    /*     } */

    printf("\nend test_om_serialize\n");

    axiom_namespace_free(ns1, m_env);
    axiom_namespace_free(ns2, m_env);
}


TEST_F(TestOM, test_om_buffer) {

    /*AXIS2C-1628 buffer modified by axiom_node_create_from_buffer */
    axis2_char_t * output;

    char * xml = strdup("<foo>T1 &amp; T2</foo>");
    char * xml_unaltered= strdup("<foo>T1 &amp; T2</foo>");

    printf("\nstart test_om_bufer\n");

    axiom_node_t * om_node = axiom_node_create_from_buffer(m_env, xml);

    output = axiom_node_to_string(om_node,m_env);

    ASSERT_EQ(0, axutil_strcmp(xml,xml_unaltered));

    ASSERT_EQ(0, axutil_strcmp(output,xml_unaltered));

    axiom_node_free_tree(om_node,m_env);
    AXIS2_FREE(m_env->allocator, output);
    AXIS2_FREE(m_env->allocator, xml);
    AXIS2_FREE(m_env->allocator, xml_unaltered);

    printf("\nend test_om_bufer\n");

    return;
}

/* AXIS2C-1627 */
TEST_F(TestOM, test_attr_special_chars)
{
     axiom_namespace_t * ns = axiom_namespace_create(m_env, "namespace", "ns");

     char * attribute;
     axiom_node_t * node;
     axiom_node_t * deserialized_node;
     axiom_attribute_t * attr;
     axiom_element_t * element = axiom_element_create(m_env, NULL, "el", ns, &node);

     axiom_element_set_text(element, m_env, "T1 & T2", node);
     attr = axiom_attribute_create(m_env, "name", "A1 & A2", NULL);
     axiom_element_add_attribute(element, m_env, attr, node);

     axis2_char_t * xml = axiom_node_to_string(node, m_env);

     //TODO with libxml2, the order of the attrs is different, so this test fails.
     ASSERT_STREQ(xml, "<ns:el xmlns:ns=\"namespace\" name=\"A1 &amp; A2\">T1 &amp; T2</ns:el>");

     deserialized_node = axiom_node_create_from_buffer(m_env, xml);

     axiom_element_t * deserialized_element = (axiom_element_t*)axiom_node_get_data_element(deserialized_node, m_env);

     attribute = axiom_element_get_attribute_value_by_name(deserialized_element, m_env,"name");
     char * text = axiom_element_get_text(deserialized_element, m_env, deserialized_node);

     ASSERT_STREQ(attribute, "A1 & A2");
     ASSERT_STREQ(text, "T1 & T2");

     axiom_node_free_tree(deserialized_node, m_env);
     axiom_node_free_tree(node, m_env);
     axiom_namespace_free(ns, m_env);
     AXIS2_FREE(m_allocator, xml);
}

TEST_F(TestOM, test_attribute)
{
    axiom_attribute_t *attr;
    axiom_namespace_t * ns1;
    axiom_namespace_t * ns2;

    ns1 = axiom_namespace_create(m_env, "urn:123456", "abc");
    ASSERT_NE(ns1, nullptr);

    attr = axiom_attribute_create(m_env, "foo", "bar", ns1);
    ASSERT_NE(attr, nullptr);

    ASSERT_STREQ("foo", axiom_attribute_get_localname(attr, m_env));
    ASSERT_STREQ("bar", axiom_attribute_get_value(attr, m_env));
    ns2 = axiom_attribute_get_namespace(attr, m_env);
    ASSERT_STREQ("abc", axiom_namespace_get_prefix(ns2, m_env));
    ASSERT_STREQ("urn:123456", axiom_namespace_get_uri(ns2, m_env));



    //TODO set/check name, value, namespace
    //TODO set/check string functions?
    //TODO test clone

    //TODO free?  Also trace down other invalid free(s) in test (per valgrind)
    axiom_attribute_free(attr, m_env);
    axiom_namespace_free(ns1, m_env);


}
