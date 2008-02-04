#include <axutil_duration.h>
#include "../util/create_env.h"

axis2_bool_t compare;
axutil_duration_t * duration;
axutil_duration_t * duration_one;
axutil_duration_t * duration_two;
axutil_duration_t * duration_three;
axutil_duration_t * duration_four;
axis2_status_t status = AXIS2_FAILURE;
axis2_char_t * duration_str = "P2003Y12M23D11H45M45.000000S";
axis2_char_t * duration_str1 = "-P2003Y12M23D11H45M45.000000S"; 
axutil_env_t *env = NULL;
int get_year,get_month,get_day,get_hour,get_minute,year,month,day,hour,minute;
double second, get_second;

/** @brief test duration
 *  create duration from values and retrieve values
 */
axis2_status_t test_duration()
{ 
    env = create_environment();
    duration_three = axutil_duration_create(env);
    duration_four  = axutil_duration_create(env); 
    duration = axutil_duration_create_from_values(env,0,2003,12,23,11,45,45.00);
    duration_one = axutil_duration_create_from_values(env,0,2007,11,2,23,11,50.00);
    duration_two = axutil_duration_create_from_string(env,duration_str);
    year = axutil_duration_get_years(duration,env);
    month = axutil_duration_get_months(duration,env);
    day = axutil_duration_get_days(duration,env);
    hour = axutil_duration_get_hours(duration,env);
    minute = axutil_duration_get_mins(duration,env);
    second = axutil_duration_get_seconds(duration,env);
    status = axutil_duration_deserialize_duration(duration_three,env,duration_str);
    if (status == AXIS2_SUCCESS)
        printf("The test 1 is successful\n");
    else
        printf("The test 1 failed\n");
    status = axutil_duration_deserialize_duration(duration_four,env,duration_str1);
    if (status == AXIS2_SUCCESS)
        printf("The test 2 is successful\n");
    else
        printf("The test 2 failed\n");
    printf("The time is %s\n", axutil_duration_serialize_duration(duration,env));
    printf("The test 3 is completed\n");
    axutil_duration_set_duration(duration,env,0,2003,12,23,11,45,56.00);
    axutil_duration_free(duration,env);
    axutil_duration_free(duration_one,env);
    axutil_duration_free(duration_two,env);
    axutil_duration_free(duration_three,env);
    axutil_duration_free(duration_four,env);
    printf("The time is %d:%d:%d-%d-%d-%f\n ",year,month,day,hour,minute,second);
    printf("The test 4 is completed\n");
    return AXIS2_SUCCESS;
}

/** @brief set values
 *  set values for the duration and get the values 
 */
axis2_status_t set_values()
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
    printf("The time is %d:%d:%d-%d-%d-%f\n ",get_year,get_month,get_day,get_hour,get_minute,get_second);
    compare = axutil_duration_compare(duration_one,duration,env);
    printf("The test 5 is completed\n");
    return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;

    axutil_env_t *env = NULL;
    env = create_environment();

    status = test_duration();
    status = set_values();

    if(status == AXIS2_FAILURE)
    {
        printf("The test 5 failed");
    }
    axutil_env_free(env);
    return 0;
}








