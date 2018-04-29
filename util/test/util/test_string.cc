
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

#include <stdio.h>
#include <axutil_error_default.h>
#include <axutil_log.h>
#include <axutil_log_default.h>
#include <axutil_string.h>

class TestString: public ::testing::Test
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


TEST_F(TestString, test_strltrim)
{

    axis2_char_t *s = (axis2_char_t*) axutil_strdup(m_env, "    abcd efgh    ");
    axis2_char_t *trimmed = NULL;
    trimmed = axutil_strltrim(m_env, s, " \t\r\n");
    ASSERT_STREQ(trimmed, "abcd efgh    ");
    if (trimmed)
        AXIS2_FREE(m_env->allocator, trimmed);
    if (s)
        AXIS2_FREE(m_env->allocator, s);
}

TEST_F(TestString, test_strrtrim)
{
    axis2_char_t *s = (axis2_char_t*) axutil_strdup(m_env, "    abcd efgh    ");
    axis2_char_t *trimmed = NULL;
    trimmed = axutil_strrtrim(m_env, s, " \t\r\n");
    ASSERT_STREQ(trimmed, "    abcd efgh");
    if (trimmed)
        AXIS2_FREE(m_env->allocator, trimmed);
    if (s)
        AXIS2_FREE(m_env->allocator, s);
}

TEST_F(TestString, test_strtrim)
{
    axis2_char_t *s = (axis2_char_t*) axutil_strdup(m_env, "    abcd efgh    ");
    axis2_char_t *trimmed = NULL;
    trimmed = axutil_strtrim(m_env, s, " \t\r\n");
    ASSERT_STREQ(trimmed, "abcd efgh");
    if (trimmed)
        AXIS2_FREE(m_env->allocator, trimmed);
    if (s)
        AXIS2_FREE(m_env->allocator, s);
}
