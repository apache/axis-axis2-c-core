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

#include <axutil_duration.h>
#include <cut_defs.h>
#include "../util/create_env.h"

/** @brief test duration
 *  create duration from values and retrieve values
 */
void test_duration(axutil_env_t *env)
{ 
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

    duration = axutil_duration_create_from_values(env,AXIS2_TRUE,3,12,23,11,45,45.00);
	CUT_ASSERT(duration != NULL);
    duration_one = axutil_duration_create_from_values(env,AXIS2_FALSE,7,11,2,23,11,50.00);
	CUT_ASSERT(duration_one != NULL);
    duration_two = axutil_duration_create_from_string(env,duration_str);
	CUT_ASSERT(duration_two != NULL);
    duration_three = axutil_duration_create(env);
	CUT_ASSERT(duration_three != NULL);
    duration_four  = axutil_duration_create(env);
	CUT_ASSERT(duration_four != NULL);

    year = axutil_duration_get_years(duration,env);
    month = axutil_duration_get_months(duration,env);
    day = axutil_duration_get_days(duration,env);
    hour = axutil_duration_get_hours(duration,env);
    minute = axutil_duration_get_mins(duration,env);
    second = axutil_duration_get_seconds(duration,env);
    is_negative = axutil_duration_get_is_negative(duration,env);

    status = axutil_duration_deserialize_duration(duration_three,env,duration_str);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    status = axutil_duration_deserialize_duration(duration_four,env,duration_str1);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    serialize = axutil_duration_serialize_duration(duration_three,env);
    CUT_ASSERT(serialize != NULL);
	CUT_ASSERT(strcmp(duration_str, serialize) == 0);
    serialize1 = axutil_duration_serialize_duration(duration_four,env);
    CUT_ASSERT(serialize1 != NULL);
	CUT_ASSERT(strcmp(duration_str1, serialize1) == 0);
    status  = axutil_duration_set_duration(duration,env,AXIS2_TRUE,3,12,23,11,45,56.00);
    CUT_ASSERT(status == AXIS2_SUCCESS);
        
    /* To avoid warning of not using cut_ptr_equal */
    CUT_ASSERT_PTR_EQUAL(NULL, NULL, 0);
    /* To avoid warning of not using cut_int_equal */
    CUT_ASSERT_INT_EQUAL(0, 0, 0);
    /* To avoid warning of not using cut_str_equal */
    CUT_ASSERT_STR_EQUAL("", "", 0);

    axutil_duration_free(duration,env);
    axutil_duration_free(duration_one,env);
    axutil_duration_free(duration_two,env);
    axutil_duration_free(duration_three,env);
    axutil_duration_free(duration_four,env);
}

/** @brief set values
 *  set values for the duration and get the values 
 */
void set_values(axutil_env_t *env)
{
    axis2_status_t status;
    axutil_duration_t * duration;
    axutil_duration_t * duration_one;
    int get_year,get_month,get_day,get_hour,get_minute;
    axis2_bool_t is_negative;
    double get_second;

    duration  = axutil_duration_create(env);
	CUT_ASSERT(duration != NULL);
	if ( duration == NULL ) return;
    duration_one = axutil_duration_create_from_values(env,AXIS2_TRUE,5,9,29,59,21,49);
	CUT_ASSERT(duration_one != NULL);

    status = axutil_duration_set_is_negative(duration,env,AXIS2_TRUE);
	CUT_ASSERT(status == AXIS2_SUCCESS);
    if (status != AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return;
    }
    is_negative = axutil_duration_get_is_negative(duration,env);
	CUT_ASSERT(is_negative == AXIS2_TRUE);
    if (!is_negative)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return;
    }

    status = axutil_duration_set_years(duration,env,5);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    get_year = axutil_duration_get_years(duration,env);
	CUT_ASSERT(get_year == 5);
    status = axutil_duration_set_months(duration,env,9);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    get_month = axutil_duration_get_months(duration,env);
	CUT_ASSERT(get_month == 9);
    status = axutil_duration_set_days(duration,env,29);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    get_day = axutil_duration_get_days(duration,env);
	CUT_ASSERT(get_day == 29);
    status = axutil_duration_set_hours(duration,env,59);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    get_hour = axutil_duration_get_hours(duration,env);
 	CUT_ASSERT(get_hour == 59);
    status = axutil_duration_set_mins(duration,env,21);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    get_minute = axutil_duration_get_mins(duration,env);
  	CUT_ASSERT(get_minute == 21);
    status = axutil_duration_set_seconds(duration,env,49);
    CUT_ASSERT(status == AXIS2_SUCCESS);
    get_second = axutil_duration_get_seconds(duration,env);
  	CUT_ASSERT(get_second == 49);
    CUT_ASSERT(axutil_duration_compare(duration_one,duration,env));
    axutil_duration_free(duration,env);
    axutil_duration_free(duration_one,env);
}
int main()
{
    axutil_env_t *env = cut_setup_env("Duration");
    CUT_ASSERT(env != NULL);
	if (env) {
       test_duration(env);
       set_values(env);
       axutil_env_free(env);
	}
    CUT_RETURN_ON_FAILURE(-1);
    return 0;
}
