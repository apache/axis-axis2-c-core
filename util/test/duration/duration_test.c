#include <axutil_duration.h>
#include "../util/create_env.h"

/** @brief test duration
 *  create duration from values and retrieve values
 */
axis2_status_t test_duration(axutil_env_t *env)
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
    axis2_char_t * neg_str = "";

    duration = axutil_duration_create_from_values(env,AXIS2_TRUE,3,12,23,11,45,45.00);
    duration_one = axutil_duration_create_from_values(env,AXIS2_FALSE,7,11,2,23,11,50.00);
    duration_two = axutil_duration_create_from_string(env,duration_str);
    duration_three = axutil_duration_create(env);
    duration_four  = axutil_duration_create(env);

    year = axutil_duration_get_years(duration,env);
    month = axutil_duration_get_months(duration,env);
    day = axutil_duration_get_days(duration,env);
    hour = axutil_duration_get_hours(duration,env);
    minute = axutil_duration_get_mins(duration,env);
    second = axutil_duration_get_seconds(duration,env);
    is_negative = axutil_duration_get_is_negative(duration,env);

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
    printf("Duration for test 3: %s\n", axutil_duration_serialize_duration(duration,env));
    printf("The test 3 is completed\n");
    status  = axutil_duration_set_duration(duration,env,AXIS2_TRUE,3,12,23,11,45,56.00);
    if (status == AXIS2_SUCCESS)
    {
        printf("The test 4 is successful\n");
    }
    else
    {
        printf("The test 4 failed\n");
    }
    axutil_duration_free(duration,env);
    axutil_duration_free(duration_one,env);
    axutil_duration_free(duration_two,env);
    axutil_duration_free(duration_three,env);
    axutil_duration_free(duration_four,env);
    if (is_negative)
        neg_str = "(-) ";
    printf("Duration for test 5: %s%d-%d-%d %d:%d:%.0f\n",neg_str,year,month,day,hour,minute,second);
    printf("The test 5 is completed\n");
    return AXIS2_SUCCESS;
}

/** @brief set values
 *  set values for the duration and get the values 
 */
axis2_status_t set_values(axutil_env_t *env)
{
    axutil_duration_t * duration;
    axutil_duration_t * duration_one;
    int get_year,get_month,get_day,get_hour,get_minute;
    axis2_bool_t is_negative;
    double get_second;

    duration  = axutil_duration_create(env);
    duration_one = axutil_duration_create_from_values(env,AXIS2_TRUE,5,9,29,59,21,49);

    if (axutil_duration_set_is_negative(duration,env,AXIS2_TRUE) != AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    is_negative = axutil_duration_get_is_negative(duration,env);
    if (!is_negative)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }

    if (axutil_duration_set_years(duration,env,5) != AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    get_year = axutil_duration_get_years(duration,env);
    if (axutil_duration_set_months(duration,env,9) != AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    get_month = axutil_duration_get_months(duration,env);
    if (axutil_duration_set_days(duration,env,29) != AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    get_day = axutil_duration_get_days(duration,env);
    if (axutil_duration_set_hours(duration,env,59) != AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    get_hour = axutil_duration_get_hours(duration,env);
    if (axutil_duration_set_mins(duration,env,21) !=  AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    get_minute = axutil_duration_get_mins(duration,env);
    if (axutil_duration_set_seconds(duration,env,49) != AXIS2_SUCCESS)
    {
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    get_second = axutil_duration_get_seconds(duration,env);
    printf("Duration for test 6: %d-%d-%d %d:%d:%.0f\n",get_year,get_month,get_day,get_hour,get_minute,get_second);
    printf("The test 6 is completed\n");
    if (!axutil_duration_compare(duration_one,duration,env))
    {
        printf("The test 7 failed\n");
        axutil_duration_free(duration,env);
        axutil_duration_free(duration_one,env);
        return AXIS2_FAILURE;
    }
    printf("The test 7 is successful\n");
    axutil_duration_free(duration,env);
    axutil_duration_free(duration_one,env);
    return AXIS2_SUCCESS;
}
int main()
{
    int status = AXIS2_SUCCESS;

    axutil_env_t *env = NULL;
    env = create_environment();

    status = test_duration(env);
    if(status == AXIS2_FAILURE)
    {
        printf("The test test_duration failed\n");
    }
    status = set_values(env);
    if(status == AXIS2_FAILURE)
    {
        printf("The test set_values failed\n");
    }
    axutil_env_free(env);
    return 0;
}
