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
 * Test for AXIS2C-1532: Last POST parameter is skipped when it has no value
 *
 * This test verifies that URL query string parameters with empty values
 * are correctly parsed and included in the parameter hash.
 *
 * The bug was that a query string like "param1=value1&param2=" would
 * skip param2 because the code required the value to be non-empty.
 */

#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_hash.h>
#include <axis2_http_transport_utils.h>

class TestAxis2C1532 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_allocator = axutil_allocator_init(NULL);
        ASSERT_NE(m_allocator, nullptr);

        m_error = axutil_error_create(m_allocator);
        ASSERT_NE(m_error, nullptr);

        m_log = axutil_log_create(m_allocator, NULL, NULL);
        ASSERT_NE(m_log, nullptr);

        m_env = axutil_env_create_with_error_log(m_allocator, m_error, m_log);
        ASSERT_NE(m_env, nullptr);
    }

    void TearDown() override
    {
        if (m_env)
        {
            axutil_env_free(m_env);
            m_env = NULL;
        }
    }

    /**
     * Helper to get a value from the param hash and compare
     */
    void AssertParamValue(axutil_hash_t *params, const char *key, const char *expected_value)
    {
        ASSERT_NE(params, nullptr) << "params hash is NULL";
        void *value = axutil_hash_get(params, key, AXIS2_HASH_KEY_STRING);
        if (expected_value == NULL)
        {
            EXPECT_EQ(value, nullptr) << "Expected param '" << key << "' to be absent";
        }
        else
        {
            ASSERT_NE(value, nullptr) << "Param '" << key << "' not found in hash";
            EXPECT_STREQ((char *)value, expected_value)
                << "Param '" << key << "' has wrong value";
        }
    }

    /**
     * Helper to free param hash and its contents
     */
    void FreeParamHash(axutil_hash_t *params)
    {
        if (params)
        {
            axutil_hash_index_t *hi;
            for (hi = axutil_hash_first(params, m_env); hi; hi = axutil_hash_next(m_env, hi))
            {
                const void *key;
                void *value;
                axutil_hash_this(hi, &key, NULL, &value);
                if (value)
                {
                    AXIS2_FREE(m_env->allocator, value);
                }
            }
            axutil_hash_free(params, m_env);
        }
    }

    axutil_allocator_t *m_allocator = NULL;
    axutil_env_t *m_env = NULL;
    axutil_error_t *m_error = NULL;
    axutil_log_t *m_log = NULL;
};

/**
 * Test normal parameters with values - baseline test
 */
TEST_F(TestAxis2C1532, test_normal_params_with_values)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method?param1=value1&param2=value2");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    ASSERT_NE(params, nullptr);
    AssertParamValue(params, "param1", "value1");
    AssertParamValue(params, "param2", "value2");

    FreeParamHash(params);
    AXIS2_FREE(m_env->allocator, uri);
}

/**
 * Test single parameter with empty value
 */
TEST_F(TestAxis2C1532, test_single_param_empty_value)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method?param1=");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    ASSERT_NE(params, nullptr) << "Should return hash even for empty value";
    AssertParamValue(params, "param1", "");

    FreeParamHash(params);
    AXIS2_FREE(m_env->allocator, uri);
}

/**
 * AXIS2C-1532: Last parameter with empty value should NOT be skipped
 * This is the main regression test for the bug.
 */
TEST_F(TestAxis2C1532, test_last_param_empty_value)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method?param1=value1&param2=");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    ASSERT_NE(params, nullptr);
    AssertParamValue(params, "param1", "value1");
    /* AXIS2C-1532: This used to fail - param2 was skipped */
    AssertParamValue(params, "param2", "");

    FreeParamHash(params);
    AXIS2_FREE(m_env->allocator, uri);
}

/**
 * Test multiple parameters where middle one has empty value
 */
TEST_F(TestAxis2C1532, test_middle_param_empty_value)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method?param1=value1&param2=&param3=value3");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    ASSERT_NE(params, nullptr);
    AssertParamValue(params, "param1", "value1");
    AssertParamValue(params, "param2", "");
    AssertParamValue(params, "param3", "value3");

    FreeParamHash(params);
    AXIS2_FREE(m_env->allocator, uri);
}

/**
 * Test all parameters with empty values
 */
TEST_F(TestAxis2C1532, test_all_params_empty_values)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method?param1=&param2=&param3=");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    ASSERT_NE(params, nullptr);
    AssertParamValue(params, "param1", "");
    AssertParamValue(params, "param2", "");
    AssertParamValue(params, "param3", "");

    FreeParamHash(params);
    AXIS2_FREE(m_env->allocator, uri);
}

/**
 * Test no query string - should return NULL
 */
TEST_F(TestAxis2C1532, test_no_query_string)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    EXPECT_EQ(params, nullptr) << "Should return NULL when no query string";

    AXIS2_FREE(m_env->allocator, uri);
}

/**
 * Test empty query string (just ?) - should return NULL
 */
TEST_F(TestAxis2C1532, test_empty_query_string)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method?");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    EXPECT_EQ(params, nullptr) << "Should return NULL for empty query string";

    AXIS2_FREE(m_env->allocator, uri);
}

/**
 * Test URL-encoded values
 */
TEST_F(TestAxis2C1532, test_url_encoded_values)
{
    char *uri = (char *)axutil_strdup(m_env,
        "/service/method?param1=hello%20world&param2=foo%26bar");

    axutil_hash_t *params = axis2_http_transport_utils_get_request_params(m_env, uri);

    ASSERT_NE(params, nullptr);
    AssertParamValue(params, "param1", "hello world");
    AssertParamValue(params, "param2", "foo&bar");

    FreeParamHash(params);
    AXIS2_FREE(m_env->allocator, uri);
}
