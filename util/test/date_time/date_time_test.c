#include <axutil_date_time.h>
#include <time.h>
#include <axutil_date_time_util.h>
#include <stdio.h>
#include <axutil_env.h>
#include "../util/create_env.h"

axutil_env_t *env = NULL;
axutil_date_time_t * date_time;
axis2_char_t * time_str = "22:45:12";
axis2_char_t * date_str = "2000-12-12";
axis2_char_t * date_time_str = "2000-11-11 12:30:24";
axutil_date_time_t * ref;
int year , month , date , hour , min , sec , msec;

/** @brief test_rand 
 *   deserialize and serialize the time 
 */

axis2_status_t test_date_time()
{
    env = create_environment();
    axutil_date_time_create_with_offset(env,100);
    date_time = axutil_date_time_create(env);
    ref = axutil_date_time_create(env);
    axutil_date_time_deserialize_time(date_time,env,time_str);
    axutil_date_time_deserialize_date(date_time,env,date_str);
    axutil_date_time_deserialize_date_time(date_time,env,date_time_str);
    axutil_date_time_compare(date_time,env,ref);
    axutil_date_time_set_date_time(date_time,env,2008,1,8,12,18,57,799);
    axutil_date_time_serialize_time(date_time,env);
    axutil_date_time_serialize_date(date_time,env);
    axutil_date_time_serialize_date_time(date_time,env);
    year = axutil_date_time_get_year(date_time,env);
    month=axutil_date_time_get_month(date_time,env);
    date = axutil_date_time_get_date(date_time,env);
    hour = axutil_date_time_get_hour(date_time,env);
    min  = axutil_date_time_get_minute(date_time,env);
    sec  = axutil_date_time_get_second(date_time,env);
    msec = axutil_date_time_get_msec(date_time,env);
    printf("The year is  %d \n",year);
    printf("The month is  %d \n",month);
    printf("The date is %d \n",date);
    printf("The hour is %d \n",hour);
    printf("The min is %d \n",min);
    printf("The sec is %d \n",sec);
    printf("The msec is %d \n",msec);
    if(date_time)
    {
    printf("The test is SUCCESS\n");
    }
    if(!date_time)
    {
    printf("The test is FAIL");
    }
    axutil_date_time_free(date_time,env);
    return AXIS2_SUCCESS;
}

int main()
{
    int status = AXIS2_SUCCESS;
    env = create_environment();
    status = test_date_time();
    if(status == AXIS2_FAILURE)
    {
        printf(" test  failed");
    }
    axutil_env_free(env);
    return 0;
}

