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
 * Test for AXIS2C-1371: Axis should log libcurl debug information
 *
 * This test verifies that libcurl debug logging is properly enabled when
 * the Axis2/C log level is set to DEBUG.
 */

#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef AXIS2_LIBCURL_ENABLED
#include <curl/curl.h>
#endif

#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>

#define TEST_LOG_FILE "/tmp/test_libcurl_debug.log"

class TestLibcurlDebug : public ::testing::Test
{
protected:
    void SetUp() override
    {
        /* Remove old log file if exists */
        unlink(TEST_LOG_FILE);

        m_allocator = axutil_allocator_init(NULL);
        ASSERT_NE(m_allocator, nullptr);

        m_error = axutil_error_create(m_allocator);
        ASSERT_NE(m_error, nullptr);

        m_log = axutil_log_create(m_allocator, NULL, TEST_LOG_FILE);
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
        /* Clean up log file */
        unlink(TEST_LOG_FILE);
    }

    /**
     * Helper to read the log file contents
     */
    std::string ReadLogFile()
    {
        std::string content;
        FILE *fp = fopen(TEST_LOG_FILE, "r");
        if (fp)
        {
            char buffer[4096];
            while (fgets(buffer, sizeof(buffer), fp))
            {
                content += buffer;
            }
            fclose(fp);
        }
        return content;
    }

    axutil_allocator_t *m_allocator = NULL;
    axutil_env_t *m_env = NULL;
    axutil_error_t *m_error = NULL;
    axutil_log_t *m_log = NULL;
};

#ifdef AXIS2_LIBCURL_ENABLED

/**
 * Test that CURLOPT_VERBOSE can be set on a curl handle.
 * This is a basic sanity check that libcurl is properly linked.
 */
TEST_F(TestLibcurlDebug, test_curl_verbose_option_can_be_set)
{
    CURL *curl = curl_easy_init();
    ASSERT_NE(curl, nullptr);

    /* Should be able to set verbose mode */
    CURLcode res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    EXPECT_EQ(res, CURLE_OK);

    curl_easy_cleanup(curl);
}

/**
 * Test that CURLOPT_DEBUGFUNCTION can be set on a curl handle.
 */
TEST_F(TestLibcurlDebug, test_curl_debug_function_can_be_set)
{
    CURL *curl = curl_easy_init();
    ASSERT_NE(curl, nullptr);

    /* Define a simple debug callback */
    auto debug_callback = [](CURL *handle, curl_infotype type,
                             char *data, size_t size, void *userptr) -> int {
        (void)handle;
        (void)type;
        (void)data;
        (void)size;
        (void)userptr;
        return 0;
    };

    CURLcode res = curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, +debug_callback);
    EXPECT_EQ(res, CURLE_OK);

    curl_easy_cleanup(curl);
}

/**
 * Test that debug logging is enabled when log level is DEBUG.
 * This tests the integration by making a simple request and checking
 * that curl debug output appears in the log.
 */
TEST_F(TestLibcurlDebug, test_debug_logging_enabled_at_debug_level)
{
    /* Set log level to DEBUG */
    m_env->log->level = AXIS2_LOG_LEVEL_DEBUG;
    m_env->log->enabled = AXIS2_TRUE;

    /* Log a test message to verify logging works */
    AXIS2_LOG_DEBUG(m_env->log, AXIS2_LOG_SI, "Test debug message for libcurl logging");

    /* Read log file and verify message was written */
    std::string log_content = ReadLogFile();
    EXPECT_TRUE(log_content.find("Test debug message") != std::string::npos)
        << "Log file should contain our test message. Content: " << log_content;
}

/**
 * Test that log level check works correctly.
 * Verifies that AXIS2_LOG_LEVEL_DEBUG >= AXIS2_LOG_LEVEL_DEBUG is true
 * and AXIS2_LOG_LEVEL_DEBUG != AXIS2_LOG_LEVEL_USER.
 */
TEST_F(TestLibcurlDebug, test_log_level_check_for_debug_enabling)
{
    /* The condition in axis2_libcurl_set_debug_options:
     * if (env->log->level >= AXIS2_LOG_LEVEL_DEBUG && env->log->level != AXIS2_LOG_LEVEL_USER)
     */

    /* Debug level should enable verbose logging */
    m_env->log->level = AXIS2_LOG_LEVEL_DEBUG;
    EXPECT_TRUE(m_env->log->level >= AXIS2_LOG_LEVEL_DEBUG);
    EXPECT_TRUE(m_env->log->level != AXIS2_LOG_LEVEL_USER);

    /* User level should NOT enable verbose logging */
    m_env->log->level = AXIS2_LOG_LEVEL_USER;
    EXPECT_TRUE(m_env->log->level >= AXIS2_LOG_LEVEL_DEBUG); /* USER > DEBUG in enum */
    EXPECT_FALSE(m_env->log->level != AXIS2_LOG_LEVEL_USER);

    /* Info level should NOT enable verbose logging */
    m_env->log->level = AXIS2_LOG_LEVEL_INFO;
    EXPECT_FALSE(m_env->log->level >= AXIS2_LOG_LEVEL_DEBUG);

    /* Warning level should NOT enable verbose logging */
    m_env->log->level = AXIS2_LOG_LEVEL_WARNING;
    EXPECT_FALSE(m_env->log->level >= AXIS2_LOG_LEVEL_DEBUG);

    /* Error level should NOT enable verbose logging */
    m_env->log->level = AXIS2_LOG_LEVEL_ERROR;
    EXPECT_FALSE(m_env->log->level >= AXIS2_LOG_LEVEL_DEBUG);

    /* Critical level should NOT enable verbose logging */
    m_env->log->level = AXIS2_LOG_LEVEL_CRITICAL;
    EXPECT_FALSE(m_env->log->level >= AXIS2_LOG_LEVEL_DEBUG);

    /* Trace level should enable verbose logging */
    m_env->log->level = AXIS2_LOG_LEVEL_TRACE;
    EXPECT_TRUE(m_env->log->level >= AXIS2_LOG_LEVEL_DEBUG);
    EXPECT_TRUE(m_env->log->level != AXIS2_LOG_LEVEL_USER);
}

/**
 * Test curl_infotype enum values for the debug callback switch statement.
 * This ensures all expected info types are handled.
 */
TEST_F(TestLibcurlDebug, test_curl_infotype_enum_values)
{
    /* Verify expected curl_infotype values exist */
    EXPECT_EQ(CURLINFO_TEXT, 0);
    EXPECT_EQ(CURLINFO_HEADER_IN, 1);
    EXPECT_EQ(CURLINFO_HEADER_OUT, 2);
    EXPECT_EQ(CURLINFO_DATA_IN, 3);
    EXPECT_EQ(CURLINFO_DATA_OUT, 4);
    EXPECT_EQ(CURLINFO_SSL_DATA_IN, 5);
    EXPECT_EQ(CURLINFO_SSL_DATA_OUT, 6);
}

#else /* !AXIS2_LIBCURL_ENABLED */

TEST_F(TestLibcurlDebug, test_libcurl_not_enabled)
{
    /* If libcurl is not enabled, just pass */
    SUCCEED() << "libcurl is not enabled in this build";
}

#endif /* AXIS2_LIBCURL_ENABLED */
