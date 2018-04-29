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

#include <axutil_log_default.h>
#include <axutil_error_default.h>
#include <axiom_node.h>
#include <stdio.h>
#include "../util/create_env.h"

unsigned char buffer[1024];

class TestAllocator: public ::testing::Test
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


TEST_F(TestAllocator, test_base64)
{

    axis2_status_t status = AXIS2_FAILURE;
    axutil_base64_binary_t *base64_binary;
    axutil_base64_binary_t *plain_base64_binary;
    const char *encoded_binary;
    unsigned char * get_binary = NULL;
    int plain_binary_len, b_len;
    unsigned char * plain_binary = NULL;
	FILE *in = fopen("test","rb");
    ASSERT_NE(in, nullptr);

    plain_binary_len = fread(buffer,1,sizeof(buffer),in);
    fclose(in);
    plain_binary = buffer;
    ASSERT_NE(plain_binary_len, 0);

    base64_binary = axutil_base64_binary_create(m_env);
    ASSERT_NE(base64_binary, nullptr);

    plain_base64_binary = axutil_base64_binary_create_with_plain_binary(m_env,
                                                                        plain_binary,
                                                                        plain_binary_len); 
    ASSERT_NE(plain_base64_binary, nullptr);

    encoded_binary = axutil_base64_binary_get_encoded_binary(plain_base64_binary, m_env);
    ASSERT_NE(encoded_binary, nullptr);

    status = axutil_base64_binary_set_plain_binary(base64_binary,m_env,plain_binary,
                                                   plain_binary_len);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    plain_binary = axutil_base64_binary_get_plain_binary(base64_binary,m_env,&b_len);
    ASSERT_NE(plain_binary, nullptr);
    status = axutil_base64_binary_set_encoded_binary(base64_binary,m_env,encoded_binary);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_binary = (unsigned char *) axutil_base64_binary_get_encoded_binary(base64_binary,m_env);
    ASSERT_NE(get_binary, nullptr);
    b_len = axutil_base64_binary_get_encoded_binary_len(base64_binary,m_env);
    ASSERT_NE(b_len, 0);

    axutil_base64_binary_free(base64_binary, m_env);
	base64_binary = axutil_base64_binary_create_with_encoded_binary(m_env, encoded_binary);
    ASSERT_NE(base64_binary, nullptr);
    if (base64_binary != nullptr)
	{
		int binary_len;
		get_binary = axutil_base64_binary_get_plain_binary(base64_binary,m_env, &binary_len);
		ASSERT_EQ(binary_len, plain_binary_len);
        ASSERT_EQ(memcmp(get_binary, plain_binary, plain_binary_len), 0);
        axutil_base64_binary_free(base64_binary, m_env);
	}

	axutil_base64_binary_free(plain_base64_binary, m_env);
}

