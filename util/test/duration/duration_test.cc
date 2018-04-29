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

#include <axutil_duration.h>
#include "../util/create_env.h"


class TestDuration: public ::testing::Test
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


/** @brief test duration
 *  create duration from values and retrieve values
 */

TEST_F(TestDuration, test_duration) {

    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t * duration_str = "P3Y12M23DT11H45M45.000000S";
    axis2_char_t * duration_str1 = "-P3Y12M23DT11H45M45.000000S";
    int year,month,day,hour,minute;
    double second;
    axutil_duration_t * duration;
    axutil_duration_t * duration_one;
    axutil_duration_t * duration_two;
    axutil_duration_t * duration_three;
    axutil_duration_t * duration_four;
    axis2_bool_t is_negative = AXIS2_FALSE;
    axis2_char_t *serialize, *serialize1;

    duration = axutil_duration_create_from_values(m_env,AXIS2_TRUE,3,12,23,11,45,45.00);
	ASSERT_NE(duration, nullptr);
    duration_one = axutil_duration_create_from_values(m_env,AXIS2_FALSE,7,11,2,23,11,50.00);
	ASSERT_NE(duration_one, nullptr);
    duration_two = axutil_duration_create_from_string(m_env,duration_str);
	ASSERT_NE(duration_two, nullptr);
    duration_three = axutil_duration_create(m_env);
	ASSERT_NE(duration_three, nullptr);
    duration_four  = axutil_duration_create(m_env);
	ASSERT_NE(duration_four, nullptr);

    year = axutil_duration_get_years(duration, m_env);
    month = axutil_duration_get_months(duration, m_env);
    day = axutil_duration_get_days(duration, m_env);
    hour = axutil_duration_get_hours(duration, m_env);
    minute = axutil_duration_get_mins(duration, m_env);
    second = axutil_duration_get_seconds(duration, m_env);
    is_negative = axutil_duration_get_is_negative(duration, m_env);

    status = axutil_duration_deserialize_duration(duration_three, m_env,duration_str);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    status = axutil_duration_deserialize_duration(duration_four, m_env,duration_str1);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    serialize = axutil_duration_serialize_duration(duration_three, m_env);
    ASSERT_NE(serialize, nullptr);
	ASSERT_STREQ(duration_str, serialize);
    serialize1 = axutil_duration_serialize_duration(duration_four, m_env);
    ASSERT_NE(serialize1, nullptr);
	ASSERT_STREQ(duration_str1, serialize1);
    status  = axutil_duration_set_duration(duration,m_env,AXIS2_TRUE,3,12,23,11,45,56.00);
    ASSERT_EQ(status, AXIS2_SUCCESS);


    axutil_duration_free(duration, m_env);
    axutil_duration_free(duration_one, m_env);
    axutil_duration_free(duration_two, m_env);
    axutil_duration_free(duration_three, m_env);
    axutil_duration_free(duration_four, m_env);
}

/** @brief set values
 *  set values for the duration and get the values
 */
TEST_F(TestDuration, test_set_values) {

    axis2_status_t status;
    axutil_duration_t * duration;
    axutil_duration_t * duration_one;
    int get_year,get_month,get_day,get_hour,get_minute;
    axis2_bool_t is_negative;
    double get_second;

    duration  = axutil_duration_create(m_env);
    ASSERT_NE(duration, nullptr);
    duration_one = axutil_duration_create_from_values(m_env,AXIS2_TRUE,5,9,29,59,21,49);
    ASSERT_NE(duration_one, nullptr);

    status = axutil_duration_set_is_negative(duration,m_env,AXIS2_TRUE);
    ASSERT_EQ(status, AXIS2_SUCCESS);

    is_negative = axutil_duration_get_is_negative(duration, m_env);
    ASSERT_EQ(is_negative, AXIS2_TRUE);

    status = axutil_duration_set_years(duration, m_env,5);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_year = axutil_duration_get_years(duration, m_env);
    ASSERT_EQ(get_year, 5);
    status = axutil_duration_set_months(duration, m_env,9);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_month = axutil_duration_get_months(duration, m_env);
    ASSERT_EQ(get_month, 9);
    status = axutil_duration_set_days(duration, m_env,29);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_day = axutil_duration_get_days(duration, m_env);
    ASSERT_EQ(get_day, 29);
    status = axutil_duration_set_hours(duration, m_env,59);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_hour = axutil_duration_get_hours(duration, m_env);
    ASSERT_EQ(get_hour, 59);
    status = axutil_duration_set_mins(duration, m_env,21);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_minute = axutil_duration_get_mins(duration, m_env);
    ASSERT_EQ(get_minute, 21);
    status = axutil_duration_set_seconds(duration, m_env,49);
    ASSERT_EQ(status, AXIS2_SUCCESS);
    get_second = axutil_duration_get_seconds(duration, m_env);
    ASSERT_EQ(get_second, 49);
    ASSERT_TRUE(axutil_duration_compare(duration_one,duration, m_env));
    axutil_duration_free(duration, m_env);
    axutil_duration_free(duration_one, m_env);
}
