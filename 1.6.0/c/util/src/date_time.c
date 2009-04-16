
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
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <axutil_error.h>
#include <axutil_utils.h>
#include <axutil_date_time_util.h>

struct axutil_date_time
{
    int year;
    int mon;
    int day;
    int hour;
    int min;
    float sec;
    axis2_bool_t tz_pos;
    int tz_hour;
    int tz_min;
};

AXIS2_EXTERN axutil_date_time_t *AXIS2_CALL
axutil_date_time_create_with_offset(
    const axutil_env_t *env,
    int offset)
{
    axutil_date_time_t *date_time = NULL;
    time_t t;
    struct tm *utc_time = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    date_time = (axutil_date_time_t *) AXIS2_MALLOC(env->allocator,
                    sizeof(axutil_date_time_t));

    if (!date_time)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }

    t = time(NULL) + offset;

    utc_time = gmtime(&t);

    date_time->year = utc_time->tm_year;
    date_time->mon = utc_time->tm_mon;
    date_time->day = utc_time->tm_mday;
    date_time->hour = utc_time->tm_hour;
    date_time->min = utc_time->tm_min;
    date_time->sec = (float)utc_time->tm_sec;
    date_time->sec += (float)axutil_get_milliseconds(env) / 1000;
    date_time->tz_hour = 0;
    date_time->tz_min = 0;
    date_time->tz_pos = AXIS2_TRUE;

    return date_time;
}

AXIS2_EXTERN axutil_date_time_t *AXIS2_CALL
axutil_date_time_create(
    const axutil_env_t *env)
{
    return axutil_date_time_create_with_offset(env, 0);
}

AXIS2_EXTERN void AXIS2_CALL
axutil_date_time_free(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (date_time)
    {
        AXIS2_FREE(env->allocator, date_time);
    }
    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_date_time_deserialize_time(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    const axis2_char_t *time_str)
{
    int hour;
    int min;
    float sec;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    sscanf(time_str, "%d:%d:%fZ", &hour, &min, &sec);
    if (hour < 0 || hour > 23)
    {
        return AXIS2_FAILURE;
    }
    if (min < 0 || min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (sec < 0 || sec >= 60)
    {
        return AXIS2_FAILURE;
    }
    date_time->hour = hour;
    date_time->min = min;
    date_time->sec = sec;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_date_time_deserialize_time_with_time_zone(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    const axis2_char_t *time_str)
{
    int hour;
    int min;
    float sec;
    int tz_hour;
    int tz_min;

    axis2_bool_t tz_pos = AXIS2_TRUE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (strchr(time_str, 'Z'))
    {
        return AXIS2_FAILURE;
    }
    else if (!strchr(time_str, '+'))
    {
        tz_pos = AXIS2_FALSE;
    }

    if (tz_pos)
    {
        sscanf(time_str, "%d:%d:%f+%d:%d", &hour, &min,
            &sec, &tz_hour, &tz_min);
    }
    else
    {
        sscanf(time_str, "%d:%d:%f-%d:%d", &hour, &min,
            &sec, &tz_hour, &tz_min);
    }
    if (hour < 0 || hour > 23)
    {
        return AXIS2_FAILURE;
    }
    if (min < 0 || min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (sec < 0 || sec >= 60)
    {
        return AXIS2_FAILURE;
    }
    if (tz_hour < 0 || tz_hour > 14)
    {
        return AXIS2_FAILURE;
    }
    if (tz_min < 0 || tz_min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (tz_hour == 14 && tz_min != 0)
    {
        return AXIS2_FAILURE;
    }
    date_time->hour = hour;
    date_time->min = min;
    date_time->sec = sec;
    date_time->tz_pos = tz_pos;
    date_time->tz_hour = tz_hour;
    date_time->tz_min = tz_min;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_date_time_deserialize_date(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    const axis2_char_t *date_str)
{
    int year;
    int mon;
    int day;
    int is_year_neg = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!date_str || *date_str == '+')
    {
        return AXIS2_FAILURE;
    }
    if (*date_str == '-')
    {
        is_year_neg++;
    }
    sscanf(date_str + is_year_neg, "%d-%d-%d", &year, &mon,
           &day);
    if (is_year_neg)
    {
        year *= -1;
    }
    if (mon < 1 || mon > 12)
    {
        return AXIS2_FAILURE;
    }
    if (day < 1 || day > 31)
    {
        return AXIS2_FAILURE;
    }
    if (day == 31 && (mon == 2 || mon == 4 ||
        mon == 6 || mon == 9 || mon == 11))
    {
        return AXIS2_FAILURE;
    }
    if (day == 30 && mon == 2)
    {
        return AXIS2_FAILURE;
    }
    if (day == 29 && mon == 2)
    {
        if (year % 4 != 0 || year % 400 == 0)
        {
            return AXIS2_FAILURE;
        }
    }
    date_time->year = year - 1900;
    date_time->mon = mon - 1;
    date_time->day = day;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_date_time_deserialize_date_time(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    const axis2_char_t *date_time_str)
{
    int year;
    int mon;
    int day;
    int hour;
    int min;
    float sec;
    int is_year_neg = 0;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!date_time_str || *date_time_str == '+')
    {
        return AXIS2_FAILURE;
    }
    if (*date_time_str == '-')
    {
        is_year_neg++;
    }
    sscanf(date_time_str + is_year_neg, "%d-%d-%dT%d:%d:%fZ", &year,
        &mon, &day, &hour, &min, &sec);

    if (is_year_neg)
    {
        year *= -1;
    }
    if (mon < 1 || mon > 12)
    {
        return AXIS2_FAILURE;
    }
    if (day < 1 || day > 31)
    {
        return AXIS2_FAILURE;
    }
    if (day == 31 && (mon == 2 || mon == 4 ||
        mon == 6 || mon == 9 || mon == 11))
    {
        return AXIS2_FAILURE;
    }
    if (day == 30 && mon == 2)
    {
        return AXIS2_FAILURE;
    }
    if (day == 29 && mon == 2)
    {
        if (year % 4 != 0 || year % 400 == 0)
        {
            return AXIS2_FAILURE;
        }
    }
    if (hour < 0 || hour > 23)
    {
        return AXIS2_FAILURE;
    }
    if (min < 0 || min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (sec < 0 || sec >= 60)
    {
        return AXIS2_FAILURE;
    }
    date_time->year = year - 1900;
    date_time->mon = mon - 1;
    date_time->day = day;
    date_time->hour = hour;
    date_time->min = min;
    date_time->sec = sec;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_date_time_deserialize_date_time_with_time_zone(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    const axis2_char_t *date_time_str)
{
    int year;
    int mon;
    int day;
    int hour;
    int min;
    float sec;
    int tz_hour;
    int tz_min;
    int is_year_neg = 0;
    axis2_bool_t tz_pos = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (!date_time_str || *date_time_str == '+')
    {
        return AXIS2_FAILURE;
    }
    if (*date_time_str == '-')
    {
        is_year_neg++;
    }

    if (strchr(date_time_str, 'Z'))
    {
        return AXIS2_FAILURE;
    }
    else if (strchr(date_time_str, '+'))
    {
        tz_pos = AXIS2_TRUE;
    }

    if (tz_pos)
    {
        sscanf(date_time_str + is_year_neg, "%d-%d-%dT%d:%d:%f+%d:%d", &year,
            &mon, &day, &hour, &min, &sec, &tz_hour, &tz_min);
    }
    else
    {
        sscanf(date_time_str + is_year_neg, "%d-%d-%dT%d:%d:%f-%d:%d", &year,
            &mon, &day, &hour, &min, &sec, &tz_hour, &tz_min);
    }

    if (is_year_neg)
    {
        year *= -1;
    }
    if (mon < 1 || mon > 12)
    {
        return AXIS2_FAILURE;
    }
    if (day < 1 || day > 31)
    {
        return AXIS2_FAILURE;
    }
    if (day == 31 && (mon == 2 || mon == 4 ||
        mon == 6 || mon == 9 || mon == 11))
    {
        return AXIS2_FAILURE;
    }
    if (day == 30 && mon == 2)
    {
        return AXIS2_FAILURE;
    }
    if (day == 29 && mon == 2)
    {
        if (year % 4 != 0 || year % 400 == 0)
        {
            return AXIS2_FAILURE;
        }
    }
    if (hour < 0 || hour > 23)
    {
        return AXIS2_FAILURE;
    }
    if (min < 0 || min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (sec < 0 || sec >= 60)
    {
        return AXIS2_FAILURE;
    }
    if (tz_hour < 0 || tz_hour > 14)
    {
        return AXIS2_FAILURE;
    }
    if (tz_min < 0 || tz_min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (tz_hour == 14 && tz_min != 0)
    {
        return AXIS2_FAILURE;
    }
    date_time->year = year - 1900;
    date_time->mon = mon - 1;
    date_time->day = day;
    date_time->hour = hour;
    date_time->min = min;
    date_time->sec = sec;
    date_time->tz_pos = tz_pos;
    date_time->tz_hour = tz_hour;
    date_time->tz_min = tz_min;
    return AXIS2_SUCCESS;
}

/*Check if the @data_time is not expired, compared to @ref*/
AXIS2_EXTERN axutil_date_time_comp_result_t AXIS2_CALL
axutil_date_time_compare(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    axutil_date_time_t *ref)
{
    int dt_min;
    int ref_min;
    int dt_hour;
    int ref_hour;
    AXIS2_ENV_CHECK(env, AXIS2_DATE_TIME_COMP_RES_FAILURE);

    if (date_time->year < ref->year)
    {
        return AXIS2_DATE_TIME_COMP_RES_NOT_EXPIRED;
    }
    else if (date_time->year > ref->year)
    {
        return AXIS2_DATE_TIME_COMP_RES_EXPIRED;
    }

    if (date_time->mon < ref->mon)
    {
        return AXIS2_DATE_TIME_COMP_RES_NOT_EXPIRED;
    }
    else if (date_time->mon > ref->mon)
    {
        return AXIS2_DATE_TIME_COMP_RES_EXPIRED;
    }

    if (date_time->day < ref->day)
    {
        return AXIS2_DATE_TIME_COMP_RES_NOT_EXPIRED;
    }
    else if (date_time->day > ref->day)
    {
        return AXIS2_DATE_TIME_COMP_RES_EXPIRED;
    }

    dt_min = date_time->tz_min;
    dt_hour = date_time->tz_hour;
    ref_min = ref->tz_min;
    ref_hour = ref->tz_hour;
    if (date_time->tz_pos)
    {
        dt_min *= -1;
        dt_hour *= -1;
    }
    if (ref->tz_pos)
    {
        ref_min *= -1;
        ref_hour *= -1;
    }

    dt_min += date_time->min;
    dt_hour += date_time->hour;
    ref_min += ref->min;
    ref_hour += ref->hour;

    if (dt_hour < ref_hour)
    {
        return AXIS2_DATE_TIME_COMP_RES_NOT_EXPIRED;
    }
    else if (dt_hour > ref_hour)
    {
        return AXIS2_DATE_TIME_COMP_RES_EXPIRED;
    }

    if (dt_min < ref_min)
    {
        return AXIS2_DATE_TIME_COMP_RES_NOT_EXPIRED;
    }
    else if (dt_min > ref_min)
    {
        return AXIS2_DATE_TIME_COMP_RES_EXPIRED;
    }

    if (date_time->sec < ref->sec)
    {
        return AXIS2_DATE_TIME_COMP_RES_NOT_EXPIRED;
    }
    else if (date_time->sec > ref->sec)
    {
        return AXIS2_DATE_TIME_COMP_RES_EXPIRED;
    }

    return AXIS2_DATE_TIME_COMP_RES_EQUAL;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_date_time_set_time_zone(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    axis2_bool_t is_positive,
    int hour,
    int min)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (hour < 0 || hour > 14)
    {
        return AXIS2_FAILURE;
    }
    if (min < 0 || min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (hour == 14 && min != 0)
    {
        return AXIS2_FAILURE;
    }
    date_time->tz_pos = is_positive;
    date_time->tz_hour = hour;
    date_time->tz_min = min;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_date_time_set_date_time(
    axutil_date_time_t *date_time,
    const axutil_env_t *env,
    int year,
    int mon,
    int day,
    int hour,
    int min,
    int sec,
    int msec)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (mon < 1 || mon > 12)
    {
        return AXIS2_FAILURE;
    }
    if (day < 1 || day > 31)
    {
        return AXIS2_FAILURE;
    }
    if (day == 31 && (mon == 2 || mon == 4 ||
        mon == 6 || mon == 9 || mon == 11))
    {
        return AXIS2_FAILURE;
    }
    if (day == 30 && mon == 2)
    {
        return AXIS2_FAILURE;
    }
    if (day == 29 && mon == 2)
    {
        if (year % 4 != 0 || year % 400 == 0)
        {
            return AXIS2_FAILURE;
        }
    }
    if (hour < 0 || hour > 23)
    {
        return AXIS2_FAILURE;
    }
    if (min < 0 || min > 59)
    {
        return AXIS2_FAILURE;
    }
    if (sec < 0 || sec > 59)
    {
        return AXIS2_FAILURE;
    }
    if (msec < 0 || msec > 999)
    {
        return AXIS2_FAILURE;
    }

    date_time->year = year - 1900;
    date_time->mon = mon - 1;
    date_time->day = day;
    date_time->hour = hour;
    date_time->min = min;
    date_time->sec = (float)sec;
    date_time->sec += (float)msec / 1000;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_date_time_serialize_time(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    axis2_char_t *time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    time_str = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                   sizeof(axis2_char_t) * 32);
    sprintf(time_str, "%02d:%02d:%06.3fZ", date_time->hour, date_time->min,
        date_time->sec);
    return time_str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_date_time_serialize_time_with_time_zone(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    axis2_char_t *time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!date_time->tz_hour && !date_time->tz_min)
    {
        return axutil_date_time_serialize_time(date_time, env);
    }

    time_str =
        (axis2_char_t *) AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 37);
    sprintf(time_str, "%02d:%02d:%06.3f%c%02d:%02d", date_time->hour, date_time->min,
        date_time->sec, date_time->tz_pos ? '+': '-',
        date_time->tz_hour, date_time->tz_min);
    return time_str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_date_time_serialize_date(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    axis2_char_t *date_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_str = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axis2_char_t) * 32);

    sprintf(date_str, "%d-%02d-%02d", date_time->year + 1900, date_time->mon + 1,
        date_time->day);
    return date_str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_date_time_serialize_date_time(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    axis2_char_t *date_time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_time_str = AXIS2_MALLOC(env->allocator, sizeof(char) * 32);
    sprintf(date_time_str, "%d-%02d-%02dT%02d:%02d:%06.3fZ",
        date_time->year + 1900, date_time->mon + 1, date_time->day,
        date_time->hour, date_time->min, date_time->sec);
    return date_time_str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_date_time_serialize_date_time_without_millisecond(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    axis2_char_t *date_time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    date_time_str = AXIS2_MALLOC(env->allocator, sizeof(char) * 32);
    sprintf(date_time_str, "%d-%02d-%02dT%02d:%02d:%02.0fZ",
        date_time->year + 1900, date_time->mon + 1, date_time->day,
        date_time->hour, date_time->min, date_time->sec);
    return date_time_str;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axutil_date_time_serialize_date_time_with_time_zone(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    axis2_char_t *date_time_str = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    if (!date_time->tz_hour && !date_time->tz_min)
    {
        return axutil_date_time_serialize_date_time(date_time, env);
    }

    date_time_str = AXIS2_MALLOC(env->allocator, sizeof(char) * 37);
    sprintf(date_time_str, "%d-%02d-%02dT%02d:%02d:%06.3f%c%02d:%02d",
        date_time->year + 1900, date_time->mon + 1, date_time->day,
        date_time->hour, date_time->min, date_time->sec, 
        date_time->tz_pos ? '+': '-', date_time->tz_hour, date_time->tz_min);
    return date_time_str;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_year(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return (date_time->year + 1900);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_month(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (date_time->mon + 1);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_date(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (date_time->day);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_hour(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (date_time->hour);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_minute(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (date_time->min);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_second(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (int)(date_time->sec);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_msec(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    /* Precision is 1/100 of a millisecond */
    float ret = (float)((date_time->sec - (float)((int)date_time->sec)) * 1000.0);
    return (int)((ret * 100.0 + 0.5) / 100.0);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_time_zone_hour(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (date_time->tz_hour);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_get_time_zone_minute(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (date_time->tz_min);
}

AXIS2_EXTERN int AXIS2_CALL
axutil_date_time_is_time_zone_positive(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    return (date_time->tz_pos);
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axutil_date_time_is_utc(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    axis2_bool_t is_utc = AXIS2_TRUE;
    if (date_time->tz_hour || date_time->tz_min)
    {
        is_utc = AXIS2_FALSE;
    }
    return is_utc;
}

AXIS2_EXTERN axutil_date_time_t *AXIS2_CALL
axutil_date_time_local_to_utc(
    axutil_date_time_t *date_time,
    const axutil_env_t *env)
{
    int year;
    int mon;
    int day;
    int hour;
    int min;
    float sec;
    int tz_hour;
    int tz_min;
    axis2_bool_t tz_pos = AXIS2_FALSE;
    
    axutil_date_time_t *ret = NULL;

    year = date_time->year;
    mon = date_time->mon;
    day = date_time->day;
    hour = date_time->hour;
    min = date_time->min;
    sec = date_time->sec;
    tz_pos = date_time->tz_pos;
    tz_hour = date_time->tz_hour;
    tz_min = date_time->tz_min;

    if (tz_pos)
    {
        tz_hour *= -1;
        tz_min *= -1;
    }
    hour += tz_hour;
    min += tz_min;

    if (min > 59)
    {
        hour += min / 60;
        min %= 60;
    }
    while (min < 0)
    {
        hour--;
        min += 60;
    }

    if (hour > 23)
    {
        day += hour / 24;
        hour %= 24;
    }
    while (hour < 0)
    {
        day--;
        hour += 24;
    }

    mon--;
    while (mon < 0)
    {
        mon += 12;
        year--;
    }
    while (mon > 11)
    {
        mon -= 12;
        year++;
    }
    mon++;

    day--;
    while (day > 27)
    {
        if (mon == 2)
        {
            if (year % 4 != 0 || year % 400 == 0)
            {
                day -= 28;
                mon++;
            }
            else if (day > 28)
            {
                day -= 29;
                mon++;
            }
            else
            {
                break;
            }
        }
        else if (day > 29)
        {
            if (mon == 4 || mon == 6 ||
                mon == 9 || mon == 11)
            {
                day -= 30;
            }
            else if (day > 30)
            {
                day -= 31;
            }
            else
            {
                break;
            }
            mon++;
        }
        else
        {
            break;
        }
        if (mon > 12)
        {
            mon = 1;
            year++;
        }
    }
    while (day < 0)
    {
        if (mon == 3)
        {
            day += 28;
            if (year % 4 == 0 || year % 400 != 0)
            {
                day++;
            }
        }
        if (mon == 5 || mon == 7 ||
            mon == 10 || mon == 12)
        {
            day += 30;
        }
        else
        {
            day += 31;
        }
        mon--;
        if (mon < 1)
        {
            mon = 12;
            year--;
        }
    }
    day++;

    if (mon < 1 || mon > 12)
    {
        return NULL;
    }
    if (day < 1 || day > 31)
    {
        return NULL;
    }
    if (day == 31 && (mon == 2 || mon == 4 ||
        mon == 6 || mon == 9 || mon == 11))
    {
        return NULL;
    }
    if (day == 30 && mon == 2)
    {
        return NULL;
    }
    if (day == 29 && mon == 2)
    {
        if (year % 4 != 0 || year % 400 == 0)
        {
            return NULL;
        }
    }
    if (hour < 0 || hour > 23)
    {
        return NULL;
    }
    if (min < 0 || min > 59)
    {
        return NULL;
    }
    if (sec < 0 || sec >= 60)
    {
        return NULL;
    }

    ret = axutil_date_time_create(env);
    ret->year = year - 1900;
    ret->mon = mon - 1;
    ret->day = day;
    ret->hour = hour;
    ret->min = min;
    ret->sec = sec;
    return ret;
}

AXIS2_EXTERN axutil_date_time_t *AXIS2_CALL
axutil_date_time_utc_to_local(
    axutil_date_time_t *date_time_in,
    const axutil_env_t *env,
    axis2_bool_t is_positive,
    int hour,
    int min)
{
    axutil_date_time_t *date_time = NULL;
    axutil_date_time_t *ret = NULL;
    if (date_time_in->tz_hour && date_time_in->tz_min)
    {
        return NULL;
    }
    date_time->year = date_time_in->year;
    date_time->mon = date_time_in->mon;
    date_time->day = date_time_in->day;
    date_time->hour = date_time_in->hour;
    date_time->min = date_time_in->min;
    date_time->sec = date_time_in->sec;
    date_time->tz_hour = hour;
    date_time->tz_min = min;
    
    date_time->tz_pos = is_positive ? AXIS2_FALSE : AXIS2_TRUE;
    ret = axutil_date_time_local_to_utc(date_time, env);
    ret->tz_hour = hour;
    ret->tz_min = min;
    
    ret->tz_pos = is_positive;

    axutil_date_time_free(date_time, env);
    return ret;
}
