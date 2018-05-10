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

#include <string.h>
#include "../util/create_env.h"
#include <axutil_string_util.h>
#include <axutil_array_list.h>

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


/** @brief test string
 *  tokenize a string
 */

TEST_F(TestString, test_string)
{
    int delim = ' ';
    void *token = NULL;
    void *last_token_string = NULL;
    void *first_token_string = NULL;
    axutil_array_list_t * first_token, * last_token;
    axis2_char_t * in =  "this is a test string";

    axutil_array_list_t * tokenize = axutil_tokenize(m_env, in, delim);
    ASSERT_NE(tokenize, nullptr);
    token  = axutil_array_list_get(tokenize,m_env,4);
    ASSERT_NE(token, nullptr);
    ASSERT_STREQ((const char *)token, "string");

    first_token = axutil_first_token(m_env,in,delim);
    ASSERT_NE(first_token, nullptr);
    if(first_token)
    {
        first_token_string = axutil_array_list_get(first_token,m_env,1);
        ASSERT_NE(first_token_string, nullptr);
        ASSERT_STREQ((const char *)first_token_string, "is a test string");
        while(axutil_array_list_size(first_token, m_env)) {
            token = axutil_array_list_remove(first_token, m_env, 0);
            AXIS2_FREE(m_allocator, token);
        }
        axutil_array_list_free(first_token, m_env);
    }

    last_token = axutil_last_token(m_env,in,delim);
    ASSERT_NE(last_token, nullptr);
    if(last_token)
    {
        last_token_string = axutil_array_list_get(last_token,m_env,1);
        ASSERT_NE(last_token_string, nullptr);
        ASSERT_STREQ((const char *)last_token_string, "string");
        while(axutil_array_list_size(last_token, m_env)) {
            token = axutil_array_list_remove(last_token, m_env, 0);
            AXIS2_FREE(m_allocator, token);
        }
        axutil_array_list_free(last_token, m_env);
    }

    while(axutil_array_list_size(tokenize, m_env)) {
        token = axutil_array_list_remove(tokenize, m_env, 0);
        AXIS2_FREE(m_allocator, token);
    }

    axutil_array_list_free(tokenize, m_env);


}
