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

/**
 * @file test_buffer_safety.cc
 * @brief Unit tests for buffer overflow prevention and memory safety
 *
 * Tests verify that Axis2/C utility functions properly handle edge cases
 * that could lead to buffer overflows or memory corruption.
 */

#include <gtest/gtest.h>

extern "C" {
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_string.h>
#include <axutil_utils.h>
}

class BufferSafetyTest : public ::testing::Test {
protected:
    axutil_env_t *env;

    void SetUp() override {
        axutil_allocator_t *allocator = axutil_allocator_init(NULL);
        axutil_error_t *error = axutil_error_create(allocator);
        axutil_log_t *log = axutil_log_create(allocator, NULL, NULL);
        env = axutil_env_create_with_error_log(allocator, error, log);
        ASSERT_NE(env, nullptr);
    }

    void TearDown() override {
        if (env) {
            axutil_env_free(env);
            env = NULL;
        }
    }
};

/**
 * Test axutil_strdup with NULL input
 */
TEST_F(BufferSafetyTest, StrdupNullSafe) {
    axis2_char_t *result = axutil_strdup(env, NULL);
    EXPECT_EQ(result, nullptr) << "strdup(NULL) should return NULL safely";
}

/**
 * Test axutil_strdup with empty string
 */
TEST_F(BufferSafetyTest, StrdupEmptyString) {
    axis2_char_t *result = axutil_strdup(env, "");
    ASSERT_NE(result, nullptr) << "strdup(\"\") should succeed";
    EXPECT_STREQ(result, "") << "Result should be empty string";
    AXIS2_FREE(env->allocator, result);
}

/**
 * Test axutil_strdup with normal string
 */
TEST_F(BufferSafetyTest, StrdupNormalString) {
    const char *test_str = "Hello, World!";
    axis2_char_t *result = axutil_strdup(env, test_str);
    ASSERT_NE(result, nullptr) << "strdup should succeed";
    EXPECT_STREQ(result, test_str) << "Result should match input";
    AXIS2_FREE(env->allocator, result);
}

/**
 * Test axutil_stracat (string concatenation) with NULL inputs
 */
TEST_F(BufferSafetyTest, StracatNullSafe) {
    /* Both NULL */
    axis2_char_t *result1 = axutil_stracat(env, NULL, NULL);
    /* Result behavior may vary - just ensure no crash */

    /* First NULL */
    axis2_char_t *result2 = axutil_stracat(env, NULL, "test");

    /* Second NULL */
    axis2_char_t *result3 = axutil_stracat(env, "test", NULL);

    /* Clean up any allocated results */
    if (result1) AXIS2_FREE(env->allocator, result1);
    if (result2) AXIS2_FREE(env->allocator, result2);
    if (result3) AXIS2_FREE(env->allocator, result3);

    /* Test passes if we reach here without crashing */
    SUCCEED() << "NULL handling completed without crash";
}

/**
 * Test axutil_stracat with normal strings
 */
TEST_F(BufferSafetyTest, StracatNormalStrings) {
    axis2_char_t *result = axutil_stracat(env, "Hello, ", "World!");
    ASSERT_NE(result, nullptr) << "stracat should succeed";
    EXPECT_STREQ(result, "Hello, World!") << "Concatenation should work";
    AXIS2_FREE(env->allocator, result);
}

/**
 * Test string functions with very long input (potential overflow)
 */
TEST_F(BufferSafetyTest, LongStringHandling) {
    /* Create a moderately long string (not so long it causes test timeout) */
    const size_t len = 10000;
    char *long_str = (char*)malloc(len + 1);
    ASSERT_NE(long_str, nullptr);
    memset(long_str, 'A', len);
    long_str[len] = '\0';

    axis2_char_t *result = axutil_strdup(env, long_str);
    ASSERT_NE(result, nullptr) << "Long string strdup should succeed";
    EXPECT_EQ(strlen(result), len) << "Length should be preserved";

    AXIS2_FREE(env->allocator, result);
    free(long_str);
}

/**
 * Test that environment NULL checks work
 */
TEST_F(BufferSafetyTest, NullEnvHandling) {
    /* These should not crash even with NULL env */
    axis2_char_t *result = axutil_strdup(NULL, "test");
    /* Result is undefined but should not crash */
    if (result) {
        /* Can't free without env, but test passed if we got here */
    }
    SUCCEED() << "NULL env handling completed without crash";
}
