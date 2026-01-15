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
#include <cstring>

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
 * AXIS2C-1222: Test that the close callback is invoked with valid context.
 *
 * The bug was that axis2_libxml2_reader_wrapper_free() freed the ctx pointer
 * BEFORE calling xmlTextReaderClose(), which triggers the close callback.
 * This caused use-after-free when the close callback tried to access ctx.
 *
 * This test verifies the fix: the close callback should be called while
 * the context is still valid, and should be able to access context data.
 *
 * NOTE: This test is only meaningful with libxml2 parser. When using guththila
 * parser, the close callback mechanism is different (guththila owns and frees
 * the context directly).
 */

#ifdef AXIS2_LIBXML2_ENABLED

/* Global flag to track if close callback was invoked (since ctx may be freed) */
static int g_axis2c_1222_close_called = 0;
static int g_axis2c_1222_ctx_valid = 0;

/* Test context structure for AXIS2C-1222 */
struct axis2c_1222_test_ctx {
    const char *xml_data;
    int read_pos;
    int magic;  /* Magic number to verify context validity */
};

#define AXIS2C_1222_MAGIC 0xDEADBEEF

/* Read callback for AXIS2C-1222 test */
static int AXIS2_CALL axis2c_1222_read_callback(char *buffer, int size, void *ctx)
{
    struct axis2c_1222_test_ctx *test_ctx = (struct axis2c_1222_test_ctx *)ctx;

    /* Verify context is valid */
    if (test_ctx->magic != AXIS2C_1222_MAGIC) {
        return -1;  /* Context corrupted */
    }

    int remaining = strlen(test_ctx->xml_data) - test_ctx->read_pos;
    if (remaining <= 0) {
        return 0;  /* EOF */
    }

    int to_read = (remaining < size - 1) ? remaining : size - 1;
    memcpy(buffer, test_ctx->xml_data + test_ctx->read_pos, to_read);
    buffer[to_read] = '\0';
    test_ctx->read_pos += to_read;

    return to_read;
}

/* Close callback for AXIS2C-1222 test */
static int AXIS2_CALL axis2c_1222_close_callback(void *ctx)
{
    struct axis2c_1222_test_ctx *test_ctx = (struct axis2c_1222_test_ctx *)ctx;

    g_axis2c_1222_close_called = 1;

    /*
     * AXIS2C-1222: This is the critical test - the close callback must be
     * able to access the context. Before the fix, ctx would already be freed
     * at this point, causing use-after-free (detected by ASAN/valgrind).
     */
    if (test_ctx->magic == AXIS2C_1222_MAGIC) {
        g_axis2c_1222_ctx_valid = 1;
    }

    return 0;
}

TEST_F(TestParser, test_axis2c_1222_close_callback_ctx_valid) {
    printf("\n\n _______ TEST AXIS2C-1222: Close callback context validity _______ \n\n");

    /* Reset global flags */
    g_axis2c_1222_close_called = 0;
    g_axis2c_1222_ctx_valid = 0;

    /* Set up test context - reader will take ownership and free it */
    struct axis2c_1222_test_ctx *test_ctx =
        (struct axis2c_1222_test_ctx *)AXIS2_MALLOC(m_env->allocator,
            sizeof(struct axis2c_1222_test_ctx));
    ASSERT_NE(test_ctx, nullptr);

    test_ctx->xml_data = "<root><child>test data</child></root>";
    test_ctx->read_pos = 0;
    test_ctx->magic = AXIS2C_1222_MAGIC;

    /* Create XML reader with IO callbacks */
    axiom_xml_reader_t *reader = axiom_xml_reader_create_for_io(
        m_env,
        axis2c_1222_read_callback,
        axis2c_1222_close_callback,
        test_ctx,
        "UTF-8");
    ASSERT_NE(reader, nullptr) << "Failed to create XML reader for IO";

    /* Read through the XML to exercise the reader */
    int event;
    while ((event = axiom_xml_reader_next(reader, m_env)) > 0) {
        /* Just consume the events until EOF or error */
    }

    /*
     * Free the reader - this is where AXIS2C-1222 bug would manifest.
     * Before the fix: ctx freed first, then close callback called (use-after-free)
     * After the fix: close callback called first, then ctx freed
     *
     * NOTE: After this call, test_ctx is freed by the reader. Do not access it!
     */
    axiom_xml_reader_free(reader, m_env);

    /*
     * Check the global flags to verify callback behavior.
     * The critical test is that g_axis2c_1222_ctx_valid is true, meaning
     * the close callback was able to access the context without use-after-free.
     */
    EXPECT_EQ(g_axis2c_1222_close_called, 1)
        << "AXIS2C-1222: Close callback should have been called";
    EXPECT_EQ(g_axis2c_1222_ctx_valid, 1)
        << "AXIS2C-1222: Context should have been valid when close callback was invoked";

    printf("  Close callback was called: %s\n",
           g_axis2c_1222_close_called ? "YES" : "NO");
    printf("  Context was valid in callback: %s\n",
           g_axis2c_1222_ctx_valid ? "YES (no use-after-free)" : "NO (BUG!)");

    printf("\n _______ END TEST AXIS2C-1222 _______ \n\n");
}

#else /* !AXIS2_LIBXML2_ENABLED */

TEST_F(TestParser, test_axis2c_1222_close_callback_ctx_valid) {
    printf("\n\n _______ TEST AXIS2C-1222: SKIPPED (libxml2 not enabled) _______ \n\n");
    /* This test is only relevant for libxml2 parser.
     * With guththila, the close callback mechanism works differently. */
    GTEST_SKIP() << "AXIS2C-1222 test requires libxml2 parser";
}

#endif /* AXIS2_LIBXML2_ENABLED */

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
