#include <axutil_duration.h>
#include "../util/create_env.h"

axis2_bool_t compere;
axutil_duration_t * duration;
axutil_duration_t * duration_one;
axis2_char_t * duration_str = "2003:12:23-11-45-45";
axutil_env_t *env = NULL;
int year,get_year,get_month,get_day,get_hour,get_minute,get_second,month,day,hour,minute;
double second;

/** @brief test duration
 *  create duration from values and retrieve values
 */
test_duration()
{ 
    env = create_environment();
    axutil_duration_create(env);
    duration = axutil_duration_create_from_values(env,0,2003,12,23,11,45,45.00);
    duration_one = axutil_duration_create_from_values(env,0,2007,11,2,23,11,50.00);
    axutil_duration_create_from_string(env,duration_str);
    year = axutil_duration_get_years(duration,env);
    month = axutil_duration_get_months(duration,env);
    day = axutil_duration_get_days(duration,env);
    hour = axutil_duration_get_hours(duration,env);
    minute = axutil_duration_get_mins(duration,env);
    second = axutil_duration_get_seconds(duration,env);
    axutil_duration_deserialize_duration(duration,env,duration_str);
    axutil_duration_serialize_duration(duration,env);
    axutil_duration_set_duration(duration,env,0,2003,12,23,11,45,56.00);
    axutil_duration_free(duration,env);
    printf("The time is  %d:%d:%d-%d-%d-%f\n ",year,month,day,hour,minute,second);
    printf("The test is success\n");
    return AXIS2_SUCCESS;
}

/** @brief set values
 *  set values for the duration and get the values 
 */
set_values()
{
    axutil_duration_set_years(duration,env,2005);
    get_year = axutil_duration_get_years(duration,env);
    axutil_duration_set_months(duration,env,9);
    get_month = axutil_duration_get_months(duration,env);
    axutil_duration_set_days(duration,env,29);
    get_day = axutil_duration_get_days(duration,env);
    axutil_duration_set_hours(duration,env,59);
    get_hour = axutil_duration_get_hours(duration,env);
    axutil_duration_set_mins(duration,env,21);
    get_minute = axutil_duration_get_mins(duration,env);
    axutil_duration_set_seconds(duration,env,49);
    get_second = axutil_duration_get_seconds(duration,env);
    printf("The time is now %d:%d:%d-%d-%d-%f\n ",get_year,get_month,get_day,get_hour,get_minute,get_second);
    compere = axutil_duration_compare(duration_one,duration,env);
    printf("The test is success\n");
    return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;

    axutil_env_t *env = NULL;
    axutil_allocator_t *allocator = NULL;
    env = create_environment();

    status = test_duration();
    status = set_values();

    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}








