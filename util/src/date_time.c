/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_date_time.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <axis2_error.h>
#include <axis2_utils.h>

/** 
 * @brief
 */
typedef struct axis2_date_time_impl
{
    axis2_date_time_t date_time;
    
    struct tm* utcTime;

} axis2_date_time_impl_t;

#define AXIS2_INTF_TO_IMPL(date_time) \
    ((axis2_date_time_impl_t *) date_time)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_date_time_free (axis2_date_time_t *date_time, 
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* time_str);

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* date_str);
    
axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* date_time_str);
    
axis2_status_t AXIS2_CALL
axis2_date_time_set_date_time (axis2_date_time_t* date_time,
                        const axis2_env_t *env,
                        int year, int month, int date,
                        int hour, int min, int second );
 
char* AXIS2_CALL
axis2_date_time_serialize_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env );
  
char* AXIS2_CALL
axis2_date_time_serialize_date (axis2_date_time_t *date_time,
                        const axis2_env_t *env );
   
char* AXIS2_CALL
axis2_date_time_serialize_date_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_date_time_get_year(axis2_date_time_t *date_time,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_date_time_get_month(axis2_date_time_t *date_time,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_date_time_get_date(axis2_date_time_t *date_time,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_date_time_get_hour(axis2_date_time_t *date_time,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_date_time_get_minute(axis2_date_time_t *date_time,
                        const axis2_env_t *env );

int AXIS2_CALL
axis2_date_time_get_second(axis2_date_time_t *date_time,
                        const axis2_env_t *env );

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL 
axis2_date_time_create (const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    time_t now;
   
    AXIS2_ENV_CHECK(env, NULL);

    date_time_impl = (axis2_date_time_impl_t *) AXIS2_MALLOC(env->
        allocator, sizeof(axis2_date_time_impl_t));

    if(NULL == date_time_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    now = time (NULL );
    date_time_impl-> utcTime = gmtime ( &now);

    date_time_impl->date_time.ops = 
        AXIS2_MALLOC (env->allocator, sizeof(axis2_date_time_ops_t));
    if(NULL == date_time_impl->date_time.ops)
    {
        axis2_date_time_free(&(date_time_impl->date_time), env);
    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    date_time_impl->date_time.ops->free = axis2_date_time_free;
    date_time_impl->date_time.ops->deserialize_time = axis2_date_time_deserialize_time;
    date_time_impl->date_time.ops->deserialize_date = axis2_date_time_deserialize_date;
    date_time_impl->date_time.ops->deserialize_date_time = axis2_date_time_deserialize_date_time;
    date_time_impl->date_time.ops->set_date_time = axis2_date_time_set_date_time;
    date_time_impl->date_time.ops->serialize_time = axis2_date_time_serialize_time;
    date_time_impl->date_time.ops->serialize_date = axis2_date_time_serialize_date;
    date_time_impl->date_time.ops->serialize_date_time = axis2_date_time_serialize_date_time;
    date_time_impl->date_time.ops->get_year = axis2_date_time_get_year;
    date_time_impl->date_time.ops->get_month = axis2_date_time_get_month;
    date_time_impl->date_time.ops->get_date = axis2_date_time_get_date;
    date_time_impl->date_time.ops->get_hour = axis2_date_time_get_hour;
    date_time_impl->date_time.ops->get_minute = axis2_date_time_get_minute;
    date_time_impl->date_time.ops->get_second = axis2_date_time_get_second;

    return &(date_time_impl->date_time);
}


/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_date_time_free (axis2_date_time_t *date_time, 
                            const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    
    if(date_time->ops)
    {
        AXIS2_FREE(env->allocator, date_time->ops);
        date_time->ops = NULL;
    }

    if(date_time_impl->utcTime)
    {
        free ( date_time_impl->utcTime);
        date_time_impl->utcTime = NULL;
    }
    
    if(date_time_impl)
    {
        AXIS2_FREE(env->allocator, date_time_impl);
        date_time_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* time_str)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    sscanf (time_str, "%d:%d:%dZ" , &time-> tm_hour, &time-> tm_min,
            &time-> tm_sec );
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* date_str)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    sscanf (date_str, "%d-%d-%d" , &time-> tm_year, &time -> tm_mon,
            &time-> tm_mday );
    time-> tm_year -= 1900;
    return AXIS2_SUCCESS;
}
    
axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env,
                        const char* date_time_str)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    sscanf (date_time_str, "%d-%d-%dT%d:%d:%dZ", &time-> tm_year, &time -> tm_mon,
            &time-> tm_mday, &time-> tm_hour, &time-> tm_min,
            &time-> tm_sec );
    time-> tm_year -= 1900;
    return AXIS2_SUCCESS;
}
    
axis2_status_t AXIS2_CALL
axis2_date_time_set_date_time (axis2_date_time_t* date_time,
                        const axis2_env_t *env,
                        int year, int month, int day,
                        int hour, int min, int second )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    if ( year > -1 )time-> tm_year = year - 1900;
    if ( month > -1 )time-> tm_mon = month;
    if ( day > -1 )time-> tm_mday = day;
    if ( hour > -1 )time -> tm_hour = hour;
    if ( min > -1 )time -> tm_min = min;
    if ( second > -1 )time -> tm_sec = second;
    return AXIS2_SUCCESS;
}
 
char* AXIS2_CALL
axis2_date_time_serialize_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    char* time_str = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    time_str = malloc ( sizeof (char) * 32 );
        
    sprintf (time_str, "%d:%d:%dZ" , time-> tm_hour, time-> tm_min, time-> tm_sec );
    return time_str;
}
char* AXIS2_CALL
axis2_date_time_serialize_date (axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time= NULL;
    char* date_str = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    date_str = malloc ( sizeof (char) * 32 );
        
    sprintf (date_str, "%d-%d-%d" , time-> tm_year + 1900,
            time -> tm_mon,
            time-> tm_mday );
    return date_str;  
}
char* AXIS2_CALL
axis2_date_time_serialize_date_time (axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    char* date_time_str = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    date_time_str = malloc ( sizeof (char) * 32 );
        
    sprintf (date_time_str, "%d-%d-%dT%d:%d:%dZ" , time-> tm_year+1900,
            time -> tm_mon, time-> tm_mday, time-> tm_hour, time-> tm_min,
            time-> tm_sec );
    return date_time_str;
}

int AXIS2_CALL 
axis2_date_time_get_year(axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    return (time-> tm_year+1900);
}

int AXIS2_CALL 
axis2_date_time_get_month(axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    return (time-> tm_mon);
}

int AXIS2_CALL 
axis2_date_time_get_date(axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    return (time-> tm_mday);
}

int AXIS2_CALL 
axis2_date_time_get_hour(axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    return (time-> tm_hour);
}

int AXIS2_CALL 
axis2_date_time_get_minute(axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    return (time-> tm_min);
}

int AXIS2_CALL 
axis2_date_time_get_second(axis2_date_time_t *date_time,
                        const axis2_env_t *env )
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    struct tm* time = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);
    time = date_time_impl->utcTime;

    return (time-> tm_sec);
}

