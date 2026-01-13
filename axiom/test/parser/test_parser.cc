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

/*
 * AXIS2C-1520: Test that UTF-8 diacritic marks are correctly parsed and preserved.
 * Characters like æøåÆØÅ (Nordic) should work in text content.
 */
TEST_F(TestParser, test_axis2c_1520_utf8_diacritics) {

    printf("\n\n _______ TEST AXIS2C-1520: UTF-8 Diacritic Marks _______ \n\n");

    /* Test 1: Nordic diacritics (æøåÆØÅ) */
    axis2_char_t input1[] = "<text>æøåÆØÅ</text>";

    axiom_node_t* node1 = axiom_node_create_from_buffer(m_env, input1);
    ASSERT_NE(node1, nullptr) << "AXIS2C-1520: Failed to parse XML with Nordic diacritics";

    /* Get the text content */
    axiom_element_t* elem1 = (axiom_element_t*)axiom_node_get_data_element(node1, m_env);
    ASSERT_NE(elem1, nullptr);

    const axis2_char_t* text1 = axiom_element_get_text(elem1, m_env, node1);
    ASSERT_NE(text1, nullptr) << "AXIS2C-1520: Failed to get text with diacritics";
    EXPECT_STREQ(text1, "æøåÆØÅ") << "AXIS2C-1520: Diacritic text not preserved correctly";

    /* Verify roundtrip serialization */
    axis2_char_t* output1 = axiom_node_to_string(node1, m_env);
    ASSERT_NE(output1, nullptr);
    EXPECT_STREQ(output1, "<text>æøåÆØÅ</text>") << "AXIS2C-1520: Serialization changed diacritics";

    axiom_node_free_tree(node1, m_env);
    AXIS2_FREE(m_env->allocator, output1);

    /* Test 2: German umlauts (äöüÄÖÜß) */
    axis2_char_t input2[] = "<text>äöüÄÖÜß</text>";

    axiom_node_t* node2 = axiom_node_create_from_buffer(m_env, input2);
    ASSERT_NE(node2, nullptr) << "AXIS2C-1520: Failed to parse XML with German umlauts";

    axiom_element_t* elem2 = (axiom_element_t*)axiom_node_get_data_element(node2, m_env);
    ASSERT_NE(elem2, nullptr);

    const axis2_char_t* text2 = axiom_element_get_text(elem2, m_env, node2);
    ASSERT_NE(text2, nullptr) << "AXIS2C-1520: Failed to get text with umlauts";
    EXPECT_STREQ(text2, "äöüÄÖÜß") << "AXIS2C-1520: Umlaut text not preserved correctly";

    axiom_node_free_tree(node2, m_env);

    /* Test 3: French accents (éèêëàâùûôîç) */
    axis2_char_t input3[] = "<text>éèêëàâùûôîç</text>";

    axiom_node_t* node3 = axiom_node_create_from_buffer(m_env, input3);
    ASSERT_NE(node3, nullptr) << "AXIS2C-1520: Failed to parse XML with French accents";

    axiom_element_t* elem3 = (axiom_element_t*)axiom_node_get_data_element(node3, m_env);
    ASSERT_NE(elem3, nullptr);

    const axis2_char_t* text3 = axiom_element_get_text(elem3, m_env, node3);
    ASSERT_NE(text3, nullptr) << "AXIS2C-1520: Failed to get text with French accents";
    EXPECT_STREQ(text3, "éèêëàâùûôîç") << "AXIS2C-1520: French accent text not preserved";

    axiom_node_free_tree(node3, m_env);

    /* Test 4: Mixed ASCII and UTF-8 */
    axis2_char_t input4[] = "<text>Hello æøå World</text>";

    axiom_node_t* node4 = axiom_node_create_from_buffer(m_env, input4);
    ASSERT_NE(node4, nullptr) << "AXIS2C-1520: Failed to parse XML with mixed ASCII/UTF-8";

    axiom_element_t* elem4 = (axiom_element_t*)axiom_node_get_data_element(node4, m_env);
    ASSERT_NE(elem4, nullptr);

    const axis2_char_t* text4 = axiom_element_get_text(elem4, m_env, node4);
    ASSERT_NE(text4, nullptr) << "AXIS2C-1520: Failed to get mixed text";
    EXPECT_STREQ(text4, "Hello æøå World") << "AXIS2C-1520: Mixed text not preserved";

    axiom_node_free_tree(node4, m_env);

    printf("\n _______ END TEST AXIS2C-1520 _______ \n\n");
}
