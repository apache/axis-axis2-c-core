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
#include <platforms/axis2_platform_auto_sense.h>

/**
 * @brief
 */
typedef struct axis2_date_time_impl
{
    axis2_date_time_t date_time;

    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;

}
axis2_date_time_impl_t;

#define AXIS2_INTF_TO_IMPL(date_time) \
    ((axis2_date_time_impl_t *) date_time)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_date_time_free(axis2_date_time_t *date_time,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const char* time_str);

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const char* date_str);

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const char* date_time_str);

axis2_status_t AXIS2_CALL
axis2_date_time_set_date_time(axis2_date_time_t* date_time,
        const axis2_env_t *env,
        int year, int month, int date,
        int hour, int min, int second);

char* AXIS2_CALL
axis2_date_time_serialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env);

char* AXIS2_CALL
axis2_date_time_serialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env);

char* AXIS2_CALL
axis2_date_time_serialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env);

int AXIS2_CALL
axis2_date_time_get_year(axis2_date_time_t *date_time,
        const axis2_env_t *env);

int AXIS2_CALL
axis2_date_time_get_month(axis2_date_time_t *date_time,
        const axis2_env_t *env);

int AXIS2_CALL
axis2_date_time_get_date(axis2_date_time_t *date_time,
        const axis2_env_t *env);

int AXIS2_CALL
axis2_date_time_get_hour(axis2_date_time_t *date_time,
        const axis2_env_t *env);

int AXIS2_CALL
axis2_date_time_get_minute(axis2_date_time_t *date_time,
        const axis2_env_t *env);

int AXIS2_CALL
axis2_date_time_get_second(axis2_date_time_t *date_time,
        const axis2_env_t *env);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL
axis2_date_time_create_with_offset(const axis2_env_t *env, int offset)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    time_t t;
    struct tm* utc_time = NULL;
    /*struct tm* utc_time_ret = NULL;*/

    AXIS2_ENV_CHECK(env, NULL);

    date_time_impl = (axis2_date_time_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_date_time_impl_t));

    if (NULL == date_time_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    t = time(NULL) + offset;
    /*Temporary fixed the segmentation fault. JIRA AXIS2C-302 */
    /*utc_time_ret = (struct tm *)axis2_gmtime_r(&t, utc_time);*/
    utc_time = gmtime(&t);

    date_time_impl-> year = utc_time-> tm_year;
    date_time_impl-> mon = utc_time-> tm_mon;
    date_time_impl-> day = utc_time-> tm_mday;
    date_time_impl-> hour = utc_time-> tm_hour;
    date_time_impl-> min = utc_time-> tm_min;
    date_time_impl-> sec = utc_time-> tm_sec;

    date_time_impl->date_time.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_date_time_ops_t));
    if (NULL == date_time_impl->date_time.ops)
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


AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL
axis2_date_time_create(const axis2_env_t *env)
{
    return axis2_date_time_create_with_offset(env, 0);
}


/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_date_time_free(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    if (date_time->ops)
    {
        AXIS2_FREE(env->allocator, date_time->ops);
        date_time->ops = NULL;
    }

    if (date_time_impl)
    {
        AXIS2_FREE(env->allocator, date_time_impl);
        date_time_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const char* time_str)
{
    axis2_date_time_impl_t *date_time_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    sscanf(time_str, "%d:%d:%dZ" , &date_time_impl-> hour, &date_time_impl-> min,
            &date_time_impl-> sec);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const char* date_str)
{
    axis2_date_time_impl_t *date_time_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    sscanf(date_str, "%d-%d-%d" , &date_time_impl-> year, &date_time_impl-> mon,
            &date_time_impl-> day);
    date_time_impl-> year -= 1900;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_date_time_deserialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const char* date_time_str)
{
    axis2_date_time_impl_t *date_time_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    sscanf(date_time_str, "%d-%d-%dT%d:%d:%dZ", &date_time_impl-> year, &date_time_impl-> mon,
            &date_time_impl-> day, &date_time_impl-> hour, &date_time_impl-> min,
            &date_time_impl-> sec);
    date_time_impl-> year -= 1900;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_date_time_set_date_time(axis2_date_time_t* date_time,
        const axis2_env_t *env,
        int year, int month, int day,
        int hour, int min, int second)
{
    axis2_date_time_impl_t *date_time_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    if (year > -1)date_time_impl-> year = year - 1900;
    if (month > -1)date_time_impl-> mon = month;
    if (day > -1)date_time_impl-> day = day;
    if (hour > -1)date_time_impl-> hour = hour;
    if (min > -1)date_time_impl-> min = min;
    if (second > -1)date_time_impl-> sec = second;
    return AXIS2_SUCCESS;
}

char* AXIS2_CALL
axis2_date_time_serialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    char* time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    time_str = (char*)AXIS2_MALLOC(env->allocator, sizeof(char) * 32);

    sprintf(time_str, "%d:%d:%dZ" , date_time_impl-> hour, date_time_impl-> min, date_time_impl-> sec);
    return time_str;
}

char* AXIS2_CALL
axis2_date_time_serialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    char* date_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    date_str = (char*)AXIS2_MALLOC(env-> allocator, sizeof(char) * 32);

    sprintf(date_str, "%d-%d-%d" , date_time_impl-> year + 1900,
            date_time_impl-> mon,
            date_time_impl-> day);
    return date_str;
}
char* AXIS2_CALL
axis2_date_time_serialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    char* date_time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    date_time_str = AXIS2_MALLOC(env-> allocator, sizeof(char) * 32);
    sprintf(date_time_str, "%d-%02d-%02dT%02d:%02d:%02dZ" , date_time_impl-> year + 1900,
            date_time_impl-> mon + 1, date_time_impl-> day, date_time_impl-> hour, date_time_impl-> min,
            date_time_impl-> sec);
    return date_time_str;
}

int AXIS2_CALL
axis2_date_time_get_year(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    return (date_time_impl-> year + 1900);
}

int AXIS2_CALL
axis2_date_time_get_month(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    return (date_time_impl-> mon);
}

int AXIS2_CALL
axis2_date_time_get_date(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    return (date_time_impl-> day);
}

int AXIS2_CALL
axis2_date_time_get_hour(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    return (date_time_impl-> hour);
}

int AXIS2_CALL
axis2_date_time_get_minute(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    return (date_time_impl-> min);
}

int AXIS2_CALL
axis2_date_time_get_second(axis2_date_time_t *date_time,
        const axis2_env_t *env)
{
    axis2_date_time_impl_t *date_time_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    date_time_impl = AXIS2_INTF_TO_IMPL(date_time);

    return (date_time_impl-> sec);
}

