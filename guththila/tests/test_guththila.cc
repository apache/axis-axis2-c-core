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
#include <guththila_defines.h>

#define BUF_SIZE 256

class TestGuththila: public ::testing::Test
{

    protected:
        void SetUp()
        {
            m_allocator = axutil_allocator_init(NULL);
            ASSERT_NE(m_allocator, nullptr);
            m_env = axutil_env_create(m_allocator);
            ASSERT_NE(m_env, nullptr);

            m_parser = (guththila_t*) AXIS2_MALLOC(m_allocator, sizeof(guththila_t));
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

        axutil_allocator_t *m_allocator;
        axutil_env_t *m_env;
        guththila_reader_t *m_reader;
        guththila_t *m_parser;
        char m_buffer[BUF_SIZE];

};


TEST_F(TestGuththila, test_start_element) {

    int c = 0;
    char *p;

    m_reader = guththila_reader_create_for_file("resources/om/axis.xml", m_env);
    ASSERT_NE(m_reader, nullptr);
    guththila_init(m_parser, m_reader, m_env);

    guththila_reader_read(m_parser->reader, m_buffer, 0, BUF_SIZE,  m_env);
    c = guththila_next(m_parser, m_env);

    while((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);
    ASSERT_EQ(c, GUTHTHILA_START_ELEMENT);
    ASSERT_STREQ(p, "root");
    AXIS2_FREE(m_allocator, p);
    c = 0;

    while((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);
    ASSERT_EQ(c, GUTHTHILA_START_ELEMENT);
    ASSERT_STREQ(p, "a");
    AXIS2_FREE(m_allocator, p);

    c = 0;
    while((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);
    ASSERT_STREQ(p, "b");
    AXIS2_FREE(m_allocator, p);
}

TEST_F(TestGuththila, test_empty_element) {

    int c = 0;
    char *p;

    m_reader = guththila_reader_create_for_file("resources/om/axis.xml", m_env);
    ASSERT_NE(m_reader, nullptr);
    guththila_init(m_parser, m_reader, m_env);

    guththila_reader_read(m_parser->reader, m_buffer, 0, BUF_SIZE,  m_env);
    c = guththila_next(m_parser, m_env);

    while((c != GUTHTHILA_EMPTY_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);

    ASSERT_EQ(c, GUTHTHILA_EMPTY_ELEMENT);
    ASSERT_STREQ(p, "a.1");
    AXIS2_FREE(m_allocator, p);

    c = 0;

    while((c != GUTHTHILA_EMPTY_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);

    ASSERT_EQ(c, GUTHTHILA_EMPTY_ELEMENT);
    ASSERT_STREQ(p, "a.2");
    AXIS2_FREE(m_allocator, p);

    c = 0;
    while((c != GUTHTHILA_START_ELEMENT))
        c = guththila_next(m_parser, m_env);

    c = 0;
    while((c != GUTHTHILA_EMPTY_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);

    ASSERT_EQ(c, GUTHTHILA_EMPTY_ELEMENT);
    ASSERT_STREQ(p, "b.1");
    AXIS2_FREE(m_allocator, p);

}


TEST_F(TestGuththila, test_end_element) {

    int c = 0;
    char *p;

    m_reader = guththila_reader_create_for_file("resources/om/axis.xml", m_env);
    ASSERT_NE(m_reader, nullptr);
    guththila_init(m_parser, m_reader, m_env);

    guththila_reader_read(m_parser->reader, m_buffer, 0, BUF_SIZE,  m_env);
    c = guththila_next(m_parser, m_env);

    while((c != GUTHTHILA_END_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);

    ASSERT_EQ(c, GUTHTHILA_END_ELEMENT);
    ASSERT_STREQ(p, "a");
    AXIS2_FREE(m_allocator, p);

    c = 0;
    while((c != GUTHTHILA_END_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);

    ASSERT_EQ(c, GUTHTHILA_END_ELEMENT);
    ASSERT_STREQ(p, "b");
    AXIS2_FREE(m_allocator, p);

    c = 0;
    while((c != GUTHTHILA_END_ELEMENT))
        c = guththila_next(m_parser, m_env);

    p = guththila_get_name(m_parser, m_env);
    ASSERT_NE(p, nullptr);

    ASSERT_EQ(c, GUTHTHILA_END_ELEMENT);
    ASSERT_STREQ(p, "root");
    AXIS2_FREE(m_allocator, p);
}

TEST_F(TestGuththila, test_character) {

    int c = 0;
    int i = 0;
    char *p;

    m_reader = guththila_reader_create_for_file("resources/om/numbers.xml", m_env);
    ASSERT_NE(m_reader, nullptr);
    guththila_init(m_parser, m_reader, m_env);

    guththila_reader_read(m_parser->reader, m_buffer, 0, BUF_SIZE,  m_env);
    c = guththila_next(m_parser, m_env);
    while(i < 3)
    {
        if(c == GUTHTHILA_START_ELEMENT)
            i++;
        c = guththila_next(m_parser, m_env);
    }

    if(c == GUTHTHILA_CHARACTER)
        p = guththila_get_value(m_parser, m_env);
    ASSERT_NE(p, nullptr);
    ASSERT_STREQ(p, "3");
    AXIS2_FREE(m_allocator, p);

    c = 0;
    while(c != GUTHTHILA_CHARACTER)
        c = guththila_next(m_parser, m_env);
    p = guththila_get_value(m_parser, m_env);
    ASSERT_NE(p, nullptr);
    ASSERT_STREQ(p, "24");
    AXIS2_FREE(m_allocator, p);
}
