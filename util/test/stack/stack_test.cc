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
#include <axutil_stack.h>

class TestStack: public ::testing::Test
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

TEST_F(TestStack, test_om_build)
{
    axutil_stack_t * stack = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    void * get_value = NULL;
    char value[10] = "test\n";

    stack = axutil_stack_create(m_env);
    ASSERT_NE(stack, nullptr);
    status = axutil_stack_push(stack,m_env,(void *)value);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    ASSERT_EQ(axutil_stack_size(stack,m_env), 1);
    ASSERT_EQ(value, (char *)axutil_stack_get(stack,m_env));
    get_value = axutil_stack_get_at(stack,m_env,0);
    ASSERT_STREQ(value,(const char *)get_value);
    ASSERT_STREQ(value,(char *)axutil_stack_pop(stack,m_env));
    ASSERT_EQ(axutil_stack_size(stack,m_env), 0);

    axutil_stack_free(stack,m_env);
}

