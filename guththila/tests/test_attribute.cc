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

            guththila_reader_free(m_reader, m_env);
            m_reader = nullptr;
            guththila_un_init(m_parser, m_env);
            m_parser = nullptr;
            axutil_env_free(m_env);

        }

        guththila_reader_t *m_reader;
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

}
