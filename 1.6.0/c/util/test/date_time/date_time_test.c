#include <axutil_date_time.h>
#include <time.h>
#include <axutil_date_time_util.h>
#include <stdio.h>
#include <axutil_env.h>
#include "../util/create_env.h"

/** @brief test_rand 
 *  *   deserialize and serialize the time 
 *   */

axis2_status_t test_date_time(axutil_env_t *env)
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
    if(!date_time_offset)
    {
        printf("axutil_date_time_t creation failed.\n");
        return AXIS2_FAILURE;
    }
    date_time = axutil_date_time_create(env);
    if(!date_time)
    {
        printf("axutil_date_time_t creation failed.\n");
        return AXIS2_FAILURE;
    }
    status = axutil_date_time_deserialize_time(date_time, env, time_str);
    if(status)
        printf("axutil_date_time_t time string deserialization success.\n");
    status = axutil_date_time_deserialize_date(date_time, env, date_str);
    if(status)
        printf("axutil_date_time_t date string deserialization success.\n");
    status = axutil_date_time_deserialize_date_time(date_time, env, date_time_str);
    if(status)
        printf("axutil_date_time_t date time string deserialization success.\n");
    
    ref = axutil_date_time_create(env);
    if(!ref)
    {
        printf("axutil_date_time_t creation failed.\n");
        return AXIS2_FAILURE;
    }
    compare_res = axutil_date_time_compare(date_time, env, ref);
    if(compare_res == AXIS2_DATE_TIME_COMP_RES_FAILURE)
    {
        printf("axutil_date_time comparison failed.\n");
    }
    
    status = axutil_date_time_deserialize_date_time(ref, env, date_time_str);
    if(status)
        printf("axutil_date_time_t date time string deserialization success.\n");
    compare_res = axutil_date_time_compare(date_time, env, ref);
    if(compare_res == AXIS2_DATE_TIME_COMP_RES_EQUAL)
    {
        printf("axutil_date_time_t comparison success.");
    }
    status = axutil_date_time_set_date_time(date_time, env, 2008, 1, 8, 12, 18, 57, 799);
    if(status)
    {
        printf("axutil_date_time_t set date time success.\n");
    }
    
    t_str = axutil_date_time_serialize_time(date_time, env);
    if(!t_str)
    {
        printf("axutil_date_time_t time serialization failed.\n");
    }
    else
    {
        printf("axutil_date_time_t Time: %s\n", t_str);
    }
    d_str = axutil_date_time_serialize_date(date_time, env);
    if(!d_str)
    {
        printf("axutil_date_time_t date serialization failed.\n");
    }
    else
    {
        printf("axutil_date_time_t Date: %s\n", d_str);
    }
    dt_str = axutil_date_time_serialize_date_time(date_time, env);
    if(!dt_str)
    {
        printf("axutil_date_time_t date time serialization failed.\n");
    }
    else
    {
        printf("axutil_date_time_t Date Time: %s\n", dt_str);
    }
    year = axutil_date_time_get_year(date_time,env);
    month=axutil_date_time_get_month(date_time,env);
    date = axutil_date_time_get_date(date_time,env);
    hour = axutil_date_time_get_hour(date_time,env);
    min  = axutil_date_time_get_minute(date_time,env);
    sec  = axutil_date_time_get_second(date_time,env);
    msec = axutil_date_time_get_msec(date_time,env);
    printf("axutil_date_time_t year: %d \n",year);
    printf("axutil_date_time_t month: %d \n",month);
    printf("axutil_date_time_t date: %d \n",date);
    printf("axutil_date_time_t hour: %d \n",hour);
    printf("axutil_date_time_t min: %d \n",min);
    printf("axutil_date_time_t sec: %d \n",sec);
    printf("axutil_date_time_t msec: %d \n",msec);
    
    axutil_date_time_free(date_time,env);
    axutil_date_time_free(ref, env);
    axutil_date_time_free(date_time_offset, env);
    return AXIS2_SUCCESS;
}

int main()
{
    axutil_env_t *env = NULL;
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_date_time(env);
    if(status != AXIS2_SUCCESS)
    {
        printf("axutil_date_time_t test failed");
    }
    else
    {
        printf("axutil_date_time_t test successful");
    }
    axutil_env_free(env);
    return 0;
}

