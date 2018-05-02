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

#include <guththila.h>
#include <axiom.h>


#define BUF_SIZE 256

class TestAttribute: public ::testing::Test
{

    protected:
        void SetUp()
        {
            m_allocator = axutil_allocator_init(NULL);
            ASSERT_NE(m_allocator, nullptr);
            m_env = axutil_env_create(m_allocator);
            ASSERT_NE(m_env, nullptr);

            m_parser = (guththila_t*) calloc(1, sizeof(guththila_t));
            ASSERT_NE(m_parser, nullptr);

            memset(m_buffer, 0, BUF_SIZE);

        }

        void TearDown()
        {

            axutil_env_free(m_env);

        }

        guththila_reader_t *m_reader = nullptr;
        axutil_allocator_t *m_allocator;
        axutil_env_t *m_env;
        guththila_t *m_parser;
        char m_buffer[BUF_SIZE];

};

TEST_F(TestAttribute, test_attribute) {

    int count = 0;
    int c = 0;
    guththila_attr_t *att;

    m_reader = guththila_reader_create_for_file("resources/om/evaluate.xml", m_env);
    ASSERT_NE(m_reader, nullptr);
    guththila_init(m_parser, m_reader, m_env);

    guththila_reader_read(m_parser->reader, m_buffer, 0, BUF_SIZE,  m_env);
    c = guththila_next(m_parser, m_env);

    while (!count)
    {
        /* FIXME:  This is potentially an infinite loop */
        c = guththila_next(m_parser, m_env);
        count = guththila_get_attribute_count(m_parser, m_env);
        if (count)
            att = guththila_get_attribute(m_parser, m_env);
    }

    ASSERT_NE(count, 0);
    ASSERT_STREQ(guththila_get_attribute_name(m_parser, att, m_env), "color");
    ASSERT_STREQ(guththila_get_attribute_value(m_parser, att, m_env), "brown");

    guththila_reader_free(m_reader, m_env);
    m_reader = nullptr;
    guththila_un_init(m_parser, m_env);
    m_parser = nullptr;

}


TEST_F(TestAttribute, test_attribute_prefix) {

    int count = 0;
    int c = 0;
    guththila_attr_t *att;

    m_reader = guththila_reader_create_for_file("resources/soap/soapmessage.xml", m_env);
    ASSERT_NE(m_reader, nullptr);
    guththila_init(m_parser, m_reader, m_env);

    guththila_reader_read(m_parser->reader, m_buffer, 0, BUF_SIZE,  m_env);
    c = guththila_next(m_parser, m_env);

    while(!count)
    {
        /* FIXME:  This is potentially an infinite loop */
        c = guththila_next(m_parser, m_env);
        count = guththila_get_attribute_count(m_parser, m_env);
        if (count)
            att = guththila_get_attribute(m_parser, m_env);
    }

    ASSERT_NE(count, 0);
    ASSERT_STREQ(guththila_get_attribute_prefix(m_parser, att, m_env), "soapenv");
    ASSERT_STREQ(guththila_get_attribute_name(m_parser, att, m_env), "mustUnderstand");
    ASSERT_STREQ(guththila_get_attribute_value(m_parser, att, m_env), "0");

    count = 0;
    while(!count)
    {
        /* FIXME:  This is potentially an infinite loop */
        c = guththila_next(m_parser, m_env);
        count = guththila_get_attribute_count(m_parser, m_env);
        if (count)
            att = guththila_get_attribute(m_parser, m_env);
    }
    ASSERT_NE(count, 0);
    ASSERT_STREQ(guththila_get_attribute_prefix(m_parser, att, m_env), "soapenv");
    ASSERT_STREQ(guththila_get_attribute_name(m_parser, att, m_env), "mustUnderstand");
    ASSERT_STREQ(guththila_get_attribute_value(m_parser, att, m_env), "0");

    guththila_reader_free(m_reader, m_env);
    m_reader = nullptr;
    guththila_un_init(m_parser, m_env);
    m_parser = nullptr;

}

/* AXIS2C-1627 */
TEST_F(TestAttribute, test_special_chars)
{
     axiom_namespace_t * ns = axiom_namespace_create(m_env, "namespace", "ns");

     char * attribute;
     axiom_node_t * node;
     axiom_node_t * deserialized_node;
     axiom_element_t * element = axiom_element_create(m_env, NULL, "el", ns, &node);

     axiom_element_set_text(element, m_env, "T1 & T2", node);
     axiom_element_add_attribute(element, m_env, axiom_attribute_create(m_env, "name", "A1 & A2", NULL), node);

     axis2_char_t * xml = axiom_node_to_string(node, m_env);

     ASSERT_STREQ(xml, "<ns:el xmlns:ns=\"namespace\" name=\"A1 &amp; A2\">T1 &amp; T2</ns:el>");

     deserialized_node = axiom_node_create_from_buffer(m_env, xml);

     axiom_element_t * deserialized_element = (axiom_element_t*)axiom_node_get_data_element(deserialized_node, m_env);

     attribute = axiom_element_get_attribute_value_by_name(deserialized_element, m_env,"name");
     char * text = axiom_element_get_text(deserialized_element, m_env, deserialized_node);

     ASSERT_STREQ(attribute, "A1 & A2");
     ASSERT_STREQ(text, "T1 & T2");

}
