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


class TestStack: public ::testing::Test
{

    protected:
        void SetUp()
        {
            m_allocator = axutil_allocator_init(NULL);
            ASSERT_NE(m_allocator, nullptr);
            m_env = axutil_env_create(m_allocator);
            ASSERT_NE(m_env, nullptr);
            m_stack = (guththila_stack_t*) AXIS2_MALLOC(m_allocator, sizeof(guththila_stack_t));
            ASSERT_NE(m_stack, nullptr);
        }

        void TearDown()
        {
            guththila_stack_free(m_stack, m_env);
            axutil_env_free(m_env);
        }

        axutil_allocator_t *m_allocator;
        axutil_env_t *m_env;
        guththila_stack_t *m_stack;
};


TEST_F(TestStack, test_stack) {


    int status;

    status = guththila_stack_init(m_stack, m_env);
    ASSERT_EQ(status, GUTHTHILA_SUCCESS);

    ASSERT_TRUE(guththila_stack_is_empty(m_stack, m_env));

    /* default max is 16 elements, so this will force a reallocation */
    const char * abc = "abcdefghijklmnopqrstuvwxyz";
    const char * p;
    for (p = abc; *p != 0; ++p)
    {
        status = guththila_stack_push(m_stack, (void*)p, m_env);
        /* push returns the index of the top element */
        ASSERT_EQ(status, p-abc);
    }

    ASSERT_FALSE(guththila_stack_is_empty(m_stack, m_env));
    ASSERT_EQ((char*)guththila_stack_peek(m_stack, m_env), (p-1));

    status = guththila_stack_del_top(m_stack, m_env);
    ASSERT_EQ(status, GUTHTHILA_SUCCESS);
    ASSERT_EQ((char*)guththila_stack_peek(m_stack, m_env), (p-2));

    status = guththila_stack_del_top(m_stack, m_env);
    ASSERT_EQ(status, GUTHTHILA_SUCCESS);
    ASSERT_EQ((char*)guththila_stack_peek(m_stack, m_env), (p-3));

    ASSERT_EQ((char*)guththila_stack_get_by_index(m_stack, 0, m_env), abc);
    ASSERT_EQ((char*)guththila_stack_get_by_index(m_stack, 10, m_env), abc+10);
    ASSERT_EQ((char*)guththila_stack_get_by_index(m_stack, 23, m_env), p-3);


    for (p = p-3; p >= abc; --p)
    {
        ASSERT_EQ((char*)guththila_stack_pop(m_stack, m_env), p);
    }

    ASSERT_TRUE(guththila_stack_is_empty(m_stack, m_env));


    /* Test that un_init/re_init */
    guththila_stack_un_init(m_stack, m_env);
    status = guththila_stack_init(m_stack, m_env);
    ASSERT_EQ(status, GUTHTHILA_SUCCESS);

}
