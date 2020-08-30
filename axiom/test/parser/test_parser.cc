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

#include <axiom.h>


class TestParser: public ::testing::Test
{

    protected:
        void SetUp()
        {
            m_allocator = axutil_allocator_init(NULL);
            ASSERT_NE(m_allocator, nullptr);
            m_env = axutil_env_create(m_allocator);
            ASSERT_NE(m_env, nullptr);
        }

        void TearDown()
        {
            axutil_env_free(m_env);
        }

        axutil_allocator_t *m_allocator;
        axutil_env_t *m_env;
};


TEST_F(TestParser, test_axisc_1453) {

    void* buffer[256];
    axis2_char_t input1[] = "<root><![CDATA[abc&#123;def]]></root>";
    axis2_char_t expected1[] = "<root>abc&amp;#123;def</root>";

    axiom_node_t* node_input1 = axiom_node_create_from_buffer(m_env, input1);
    ASSERT_NE(node_input1, nullptr);
    axis2_char_t* output1 = axiom_node_to_string(node_input1, m_env);
    ASSERT_NE(output1, nullptr);
    ASSERT_STREQ(output1, expected1);

    axiom_node_free_tree(node_input1, m_env);
    AXIS2_FREE(m_env->allocator, output1);

    axis2_char_t input2[] = "<root><![CDATA[abc</root>def]]></root>";
    axis2_char_t expected2[] = "<root>abc&lt;/root&gt;def</root>";

    axiom_node_t* node_input2 = axiom_node_create_from_buffer(m_env, input2);
    ASSERT_NE(node_input2, nullptr);
    axis2_char_t* output2 = axiom_node_to_string(node_input2, m_env);
    ASSERT_NE(output2, nullptr);
    ASSERT_STREQ(output2, expected2);

    axiom_node_free_tree(node_input2, m_env);
    AXIS2_FREE(m_env->allocator, output2);

    axis2_char_t input3[] = "<root><![CDATA[]]></root>";
    axis2_char_t expected3[] = "<root></root>";

    axiom_node_t* node_input3 = axiom_node_create_from_buffer(m_env, input3);
    ASSERT_NE(node_input3, nullptr);
    axis2_char_t* output3 = axiom_node_to_string(node_input3, m_env);
    ASSERT_NE(output3, nullptr);
    ASSERT_STREQ(output3, expected3);

    axiom_node_free_tree(node_input3, m_env);
    AXIS2_FREE(m_env->allocator, output3);
}
