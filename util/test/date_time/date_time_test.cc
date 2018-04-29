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

#include <axutil_date_time.h>
#include <time.h>
#include <axutil_date_time_util.h>
#include <stdio.h>
#include <axutil_env.h>
#include "../util/create_env.h"

class TestDateTime: public ::testing::Test
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


/** @brief test_rand 
 *  *   deserialize and serialize the time 
 *   */

TEST_F(TestDateTime, test_date_time) {

    axutil_date_time_t *date_time = NULL;
    axutil_date_time_t *ref = NULL;
    axutil_date_time_t *date_time_offset = NULL;
    axis2_char_t *time_str = "22:45:12";
    axis2_char_t *date_str = "2000-12-12";
    axis2_char_t *date_time_str = "2000-11-11T12:30:24";
    axis2_status_t status = AXIS2_FAILURE;
    axutil_date_time_comp_result_t compare_res = AXIS2_DATE_TIME_COMP_RES_FAILURE;
    axis2_char_t *t_str = NULL, *d_str = NULL, *dt_str = NULL;
    int year , month , day, hour , min , sec , msec;

    date_time_offset = axutil_date_time_create_with_offset(m_env, 100);
    ASSERT_NE(date_time_offset, nullptr);
    if (date_time_offset != nullptr) axutil_date_time_free(date_time_offset, m_env);
    date_time = axutil_date_time_create(m_env);
    ASSERT_NE(date_time, nullptr);
    if(date_time)
    {
        status = axutil_date_time_deserialize_time(date_time, m_env, time_str);
        ASSERT_EQ(status, AXIS2_SUCCESS);
        status = axutil_date_time_deserialize_date(date_time, m_env, date_str);
        ASSERT_EQ(status, AXIS2_SUCCESS);
        status = axutil_date_time_deserialize_date_time(date_time, m_env, date_time_str);
        ASSERT_EQ(status, AXIS2_SUCCESS);

        ref = axutil_date_time_create(m_env);
        ASSERT_NE(ref, nullptr);
        if(ref)
        {
            compare_res = axutil_date_time_compare(date_time, m_env, ref);
            ASSERT_NE(compare_res, AXIS2_DATE_TIME_COMP_RES_FAILURE);

            status = axutil_date_time_deserialize_date_time(ref, m_env, date_time_str);
            ASSERT_EQ(status, AXIS2_SUCCESS);
            compare_res = axutil_date_time_compare(date_time, m_env, ref);
            ASSERT_EQ(compare_res, AXIS2_DATE_TIME_COMP_RES_EQUAL);
            axutil_date_time_free(ref, m_env);
        }
        status = axutil_date_time_set_date_time(date_time, m_env, 2008, 1, 8, 12, 18, 57, 799);
        ASSERT_EQ(status, AXIS2_SUCCESS);

        t_str = axutil_date_time_serialize_time(date_time, m_env);
        ASSERT_NE(t_str, nullptr);
        d_str = axutil_date_time_serialize_date(date_time, m_env);
        ASSERT_NE(d_str, nullptr);
        dt_str = axutil_date_time_serialize_date_time(date_time, m_env);
        ASSERT_NE(dt_str, nullptr);
        year = axutil_date_time_get_year(date_time, m_env);
        month=axutil_date_time_get_month(date_time, m_env);
        day = axutil_date_time_get_day(date_time, m_env);
        hour = axutil_date_time_get_hour(date_time, m_env);
        min  = axutil_date_time_get_minute(date_time, m_env);
        sec  = axutil_date_time_get_second(date_time, m_env);
        msec = axutil_date_time_get_msec(date_time, m_env);
        ASSERT_EQ(year, 2008);
        ASSERT_EQ(month, 1);
        ASSERT_EQ(day, 8);
        ASSERT_EQ(hour, 12);
        ASSERT_EQ(min, 18);
        ASSERT_EQ(sec, 57);
        ASSERT_EQ(msec, 799);

        axutil_date_time_free(date_time,m_env);
   }
}

