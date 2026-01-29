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
 * @file test_axis2c_1527.cc
 * @brief Unit test for AXIS2C-1527: Local to UTC time calculation wrong
 *
 * This test verifies that axutil_date_time_local_to_utc correctly converts
 * local time with timezone offset to UTC time.
 *
 * Prior to the fix, the function was double-converting year and month values
 * because it read values already in tm format (years since 1900, 0-indexed
 * months) but treated them as actual calendar values.
 */

#include <gtest/gtest.h>

extern "C" {
#include <axutil_env.h>
#include <axutil_allocator.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_date_time.h>
#include <string.h>
}

class DateTimeLocalToUtcTest : public ::testing::Test {
protected:
    axutil_env_t *env;
    axutil_allocator_t *allocator;
    axutil_error_t *error;
    axutil_log_t *log;

    void SetUp() override {
        allocator = axutil_allocator_init(NULL);
        ASSERT_NE(allocator, nullptr) << "Failed to create allocator";

        error = axutil_error_create(allocator);
        log = axutil_log_create(allocator, NULL, NULL);
        env = axutil_env_create_with_error_log(allocator, error, log);
        ASSERT_NE(env, nullptr) << "Failed to create environment";
    }

    void TearDown() override {
        if (env) {
            axutil_env_free(env);
        }
    }
};

/**
 * Test the original bug case from AXIS2C-1527.
 * Input: 2010-01-01T00:00:00+02:00 (midnight Jan 1 2010 in UTC+2)
 * Expected: 2009-12-31T22:00:00Z (10pm Dec 31 2009 in UTC)
 *
 * The bug caused output like "110-11-30T22:00:00Z" due to double-conversion
 * of tm-format year/month values.
 */
TEST_F(DateTimeLocalToUtcTest, OriginalBugCase) {
    axutil_date_time_t *dt = NULL;
    axutil_date_time_t *utc = NULL;
    axis2_char_t *result_str = NULL;
    axis2_status_t status;

    /* Create date_time and parse input with timezone */
    dt = axutil_date_time_create(env);
    ASSERT_NE(dt, nullptr) << "Failed to create date_time";

    status = axutil_date_time_deserialize_date_time_with_time_zone(
        dt, env, "2010-01-01T00:00:00+02:00");
    ASSERT_EQ(status, AXIS2_SUCCESS) << "Failed to parse date_time string";

    /* Convert to UTC */
    utc = axutil_date_time_local_to_utc(dt, env);
    ASSERT_NE(utc, nullptr) << "local_to_utc returned NULL";

    /* Serialize result */
    result_str = axutil_date_time_serialize_date_time(utc, env);
    ASSERT_NE(result_str, nullptr) << "Failed to serialize result";

    /* Verify year is correct (2009, not 110 or some garbage) */
    EXPECT_TRUE(strncmp(result_str, "2009-", 5) == 0)
        << "AXIS2C-1527: Year should be 2009, got: " << result_str;

    /* Verify month is December (12), not November (11) */
    EXPECT_TRUE(strncmp(result_str + 5, "12-", 3) == 0)
        << "Month should be 12 (December), got: " << result_str;

    /* Verify day is 31 */
    EXPECT_TRUE(strncmp(result_str + 8, "31", 2) == 0)
        << "Day should be 31, got: " << result_str;

    /* Verify time is 22:00:00 */
    EXPECT_TRUE(strstr(result_str, "T22:00:") != NULL)
        << "Time should be 22:00, got: " << result_str;

    /* Cleanup */
    AXIS2_FREE(env->allocator, result_str);
    axutil_date_time_free(utc, env);
    axutil_date_time_free(dt, env);
}

/**
 * Test conversion with negative timezone offset.
 * Input: 2010-01-01T00:00:00-05:00 (midnight Jan 1 2010 in UTC-5, e.g. EST)
 * Expected: 2010-01-01T05:00:00Z (5am Jan 1 2010 in UTC)
 */
TEST_F(DateTimeLocalToUtcTest, NegativeTimezoneOffset) {
    axutil_date_time_t *dt = NULL;
    axutil_date_time_t *utc = NULL;
    axis2_char_t *result_str = NULL;
    axis2_status_t status;

    dt = axutil_date_time_create(env);
    ASSERT_NE(dt, nullptr);

    status = axutil_date_time_deserialize_date_time_with_time_zone(
        dt, env, "2010-01-01T00:00:00-05:00");
    ASSERT_EQ(status, AXIS2_SUCCESS);

    utc = axutil_date_time_local_to_utc(dt, env);
    ASSERT_NE(utc, nullptr);

    result_str = axutil_date_time_serialize_date_time(utc, env);
    ASSERT_NE(result_str, nullptr);

    /* Should be 2010-01-01T05:00:00Z */
    EXPECT_TRUE(strncmp(result_str, "2010-01-01", 10) == 0)
        << "Date should be 2010-01-01, got: " << result_str;

    EXPECT_TRUE(strstr(result_str, "T05:00:") != NULL)
        << "Time should be 05:00, got: " << result_str;

    AXIS2_FREE(env->allocator, result_str);
    axutil_date_time_free(utc, env);
    axutil_date_time_free(dt, env);
}

/**
 * Test conversion that crosses year boundary going forward.
 * Input: 2009-12-31T23:00:00-02:00 (11pm Dec 31 2009 in UTC-2)
 * Expected: 2010-01-01T01:00:00Z (1am Jan 1 2010 in UTC)
 */
TEST_F(DateTimeLocalToUtcTest, YearBoundaryForward) {
    axutil_date_time_t *dt = NULL;
    axutil_date_time_t *utc = NULL;
    axis2_char_t *result_str = NULL;
    axis2_status_t status;

    dt = axutil_date_time_create(env);
    ASSERT_NE(dt, nullptr);

    status = axutil_date_time_deserialize_date_time_with_time_zone(
        dt, env, "2009-12-31T23:00:00-02:00");
    ASSERT_EQ(status, AXIS2_SUCCESS);

    utc = axutil_date_time_local_to_utc(dt, env);
    ASSERT_NE(utc, nullptr);

    result_str = axutil_date_time_serialize_date_time(utc, env);
    ASSERT_NE(result_str, nullptr);

    /* Should be 2010-01-01T01:00:00Z */
    EXPECT_TRUE(strncmp(result_str, "2010-01-01", 10) == 0)
        << "Date should be 2010-01-01, got: " << result_str;

    EXPECT_TRUE(strstr(result_str, "T01:00:") != NULL)
        << "Time should be 01:00, got: " << result_str;

    AXIS2_FREE(env->allocator, result_str);
    axutil_date_time_free(utc, env);
    axutil_date_time_free(dt, env);
}

/**
 * Test with a more recent year to ensure no Y2K-like issues.
 * Input: 2024-06-15T12:30:00+05:30 (12:30pm June 15 2024 in IST)
 * Expected: 2024-06-15T07:00:00Z (7am June 15 2024 in UTC)
 */
TEST_F(DateTimeLocalToUtcTest, RecentYear) {
    axutil_date_time_t *dt = NULL;
    axutil_date_time_t *utc = NULL;
    axis2_char_t *result_str = NULL;
    axis2_status_t status;

    dt = axutil_date_time_create(env);
    ASSERT_NE(dt, nullptr);

    status = axutil_date_time_deserialize_date_time_with_time_zone(
        dt, env, "2024-06-15T12:30:00+05:30");
    ASSERT_EQ(status, AXIS2_SUCCESS);

    utc = axutil_date_time_local_to_utc(dt, env);
    ASSERT_NE(utc, nullptr);

    result_str = axutil_date_time_serialize_date_time(utc, env);
    ASSERT_NE(result_str, nullptr);

    /* Should be 2024-06-15T07:00:00Z */
    EXPECT_TRUE(strncmp(result_str, "2024-06-15", 10) == 0)
        << "Date should be 2024-06-15, got: " << result_str;

    EXPECT_TRUE(strstr(result_str, "T07:00:") != NULL)
        << "Time should be 07:00, got: " << result_str;

    AXIS2_FREE(env->allocator, result_str);
    axutil_date_time_free(utc, env);
    axutil_date_time_free(dt, env);
}
