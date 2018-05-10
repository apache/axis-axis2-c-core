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

#include "../util/create_env.h"
#include <axutil_utils.h>

class TestUtils: public ::testing::Test
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


/** @brief test utils
 *  test quote string
 */

TEST_F(TestUtils, test_utils)
{

    axis2_char_t * request = "This is a request";
    axis2_char_t * s = "<root>This is a & in xml string</root>";
    axis2_char_t c = 'c';
    axis2_char_t **op, *quote_string;
    int hexit;
    op = axutil_parse_request_url_for_svc_and_op(m_env,request);
    ASSERT_NE(op, nullptr);
    AXIS2_FREE(m_allocator, op);
    quote_string = axutil_xml_quote_string(m_env,s,1);
    ASSERT_STREQ(quote_string, "&lt;root&gt;This is a &amp; in xml string&lt;/root&gt;");
    AXIS2_FREE(m_allocator, quote_string);
    hexit = axutil_hexit(c);
    ASSERT_EQ(hexit, 12);
}

