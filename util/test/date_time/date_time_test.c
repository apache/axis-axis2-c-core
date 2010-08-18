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

#include <axutil_date_time.h>
#include <time.h>
#include <axutil_date_time_util.h>
#include <stdio.h>
#include <axutil_env.h>
#include "../util/create_env.h"
#include <cut_defs.h>

/** @brief test_rand 
 *  *   deserialize and serialize the time 
 *   */

void test_date_time(axutil_env_t *env)
{
    axutil_date_time_t *date_time = NULL;
    axutil_date_time_t *ref = NULL;
    axutil_date_time_t *date_time_offset = NULL;
    axis2_char_t *time_str = "22:45:12";
    axis2_char_t *date_str = "2000-12-12";
    axis2_char_t *date_time_str = "2000-11-11T12:30:24";
    axis2_status_t status = AXIS2_FAILURE;
    axutil_date_time_comp_result_t compare_res = AXIS2_DATE_TIME_COMP_RES_FAILURE;
    axis2_char_t *t_str = NULL, *d_str = NULL, *dt_str = NULL;
    int year , month , date , hour , min , sec , msec;
    
    date_time_offset = axutil_date_time_create_with_offset(env, 100);
    CUT_ASSERT(date_time_offset != NULL);
    if (date_time_offset != NULL) axutil_date_time_free(date_time_offset, env);
    date_time = axutil_date_time_create(env);
    CUT_ASSERT(date_time != NULL);
    if(date_time)
    {
        status = axutil_date_time_deserialize_time(date_time, env, time_str);
        CUT_ASSERT(status == AXIS2_SUCCESS);
        status = axutil_date_time_deserialize_date(date_time, env, date_str);
        CUT_ASSERT(status == AXIS2_SUCCESS);
        status = axutil_date_time_deserialize_date_time(date_time, env, date_time_str);
        CUT_ASSERT(status == AXIS2_SUCCESS);

        ref = axutil_date_time_create(env);
        CUT_ASSERT(ref != NULL);
        if(ref)
        {
            compare_res = axutil_date_time_compare(date_time, env, ref);
            CUT_ASSERT(compare_res != AXIS2_DATE_TIME_COMP_RES_FAILURE);

            status = axutil_date_time_deserialize_date_time(ref, env, date_time_str);
            CUT_ASSERT(status == AXIS2_SUCCESS);
            compare_res = axutil_date_time_compare(date_time, env, ref);
            CUT_ASSERT(compare_res == AXIS2_DATE_TIME_COMP_RES_EQUAL);
            axutil_date_time_free(ref, env);
        }
        status = axutil_date_time_set_date_time(date_time, env, 2008, 1, 8, 12, 18, 57, 799);
        CUT_ASSERT(status == AXIS2_SUCCESS);

        t_str = axutil_date_time_serialize_time(date_time, env);
        CUT_ASSERT(t_str != NULL);
        d_str = axutil_date_time_serialize_date(date_time, env);
        CUT_ASSERT(d_str != NULL);
        dt_str = axutil_date_time_serialize_date_time(date_time, env);
        CUT_ASSERT(d_str != NULL);
        year = axutil_date_time_get_year(date_time,env);
        month=axutil_date_time_get_month(date_time,env);
        date = axutil_date_time_get_date(date_time,env);
        hour = axutil_date_time_get_hour(date_time,env);
        min  = axutil_date_time_get_minute(date_time,env);
        sec  = axutil_date_time_get_second(date_time,env);
        msec = axutil_date_time_get_msec(date_time,env);
        CUT_ASSERT(year == 2008);
        CUT_ASSERT(month == 1);
        CUT_ASSERT(date == 8);
        CUT_ASSERT(hour == 12);
        CUT_ASSERT(min == 18);
        CUT_ASSERT(sec == 57);
        CUT_ASSERT(msec == 799); 
        
        /* To avoid warning of not using cut_ptr_equal */
        CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
        /* To avoid warning of not using cut_int_equal */
        CUT_ASSERT_INT_EQUAL(0, 0, 0);
        /* To avoid warning of not using cut_str_equal */
        CUT_ASSERT_STR_EQUAL("", "", 0);

        axutil_date_time_free(date_time,env);
   }
}

int main()
{
    axutil_env_t *env = NULL;
    env = cut_setup_env("Date Time");
    CUT_ASSERT(env != NULL);
    if (env) 
    {
        test_date_time(env);
        axutil_env_free(env);
    }
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}

