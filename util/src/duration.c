/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <axutil_error.h>
#include <axutil_utils.h>

struct axutil_duration
{
    axis2_bool_t is_negative;
    int years;
    int months;
    int days;
    int hours;
    int mins;
    double secs;
};

AXIS2_EXTERN axutil_duration_t *AXIS2_CALL
axutil_duration_create(
    axutil_env_t *env)
{
    return axutil_duration_create_from_values(env, 0, 0, 0, 0, 0, 0, 0.0);
}

AXIS2_EXTERN axutil_duration_t *AXIS2_CALL
axutil_duration_create_from_values(
    const axutil_env_t *env,
    axis2_bool_t negative,
    int years,
    int months,
    int days,
    int hours,
    int minutes,
    double seconds)
{
    axutil_duration_t *duration = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    duration = (axutil_duration_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axutil_duration_t));
    if (!duration)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }

    duration->is_negative = negative;
    if (years > -1)
    {
        duration->years = years;
    }
    else
    {
        duration->years = 0;
    }
    if (months > -1)
    {
        duration->months = months;
    }
    else
    {
        duration->months = 0;
    }
    if (days > -1)
    {
        duration->days = days;
    }
    else
    {
        duration->days = 0;
    }
    if (hours > -1)
    {
        duration->hours = hours;
    }
    else
    {
        duration->hours = 0;
    }
    if (minutes > -1)
    {
        duration->mins = minutes;
    }
    else
    {
        duration->mins = 0;
    }
    if (seconds >= 0)
    {
        duration->secs = seconds;
    }
    else
    {
        duration->secs = 0.0;
    }
    return duration;
}

AXIS2_EXTERN axutil_duration_t *AXIS2_CALL
axutil_duration_create_from_string(
    const axutil_env_t *env,
    const axis2_char_t *duration_str)
{
    axutil_duration_t *duration = NULL;

    duration = (axutil_duration_t *) AXIS2_MALLOC(env->allocator,
        sizeof(axutil_duration_t));
    if (!duration)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Out of memory");
        return NULL;
    }

    axutil_duration_deserialize_duration(duration, env, duration_str);
    return duration;
}

/***************************Function implementation****************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_free(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (duration)
    {
        AXIS2_FREE(env->allocator, duration);
        duration = NULL;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_deserialize_duration(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    const axis2_char_t *duration_str)
{
    const axis2_char_t *cur = duration_str;
    double num;
    int num_type = 0;
    unsigned int seq = 0;
    const char desig[] = { 'Y', 'M', 'D', 'H', 'M', 'S' };

    AXIS2_PARAM_CHECK(env->error, duration, AXIS2_FAILURE);

    duration->is_negative = AXIS2_FALSE;
    duration->years = duration->months = duration->days = duration->hours = duration->mins = 0;
    duration->secs = 0;

    if (!duration_str)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    if (*cur == '-')
    {
        duration->is_negative = 1;
        cur++;
    }

    if (*cur++ != 'P')
    {
        duration->is_negative = AXIS2_FALSE;
        duration->years = duration->months = duration->days = duration->hours = duration->mins = 0;
        duration->secs = 0;
        
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NONE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    if (!*cur)
    {
        duration->is_negative = AXIS2_FALSE;
        duration->years = duration->months = duration->days = duration->hours = duration->mins = 0;
        duration->secs = 0;

        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NONE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    while (*cur)
    {
        if (seq >= sizeof(desig))
        {
            duration->is_negative = AXIS2_FALSE;
            duration->years = duration->months = duration->days = duration->hours = duration->mins = 0;
            duration->secs = 0;

            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NONE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }

        if (*cur == 'T')
        {
            if (!(seq > 3))
            {
                seq = 3;
                cur++;
            }
            else
            {
                duration->is_negative = AXIS2_FALSE;
                duration->years = duration->months = duration->days = duration->hours = duration->mins = 0;
                duration->secs = 0;

                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NONE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
        }
        num = 0;

        if ((*cur < '0') || (*cur > '9'))
            num_type = -1;
        else
        {
            num_type = 0;
            while ((*cur >= '0') && (*cur <= '9'))
            {
                num = num * 10 + (*cur - '0');
                cur++;
            }
        }

        if (!num_type && (*cur == '.'))
        {
            double mult = 1;
            cur++;
            if ((*cur < '0') || (*cur > '9'))
                num_type = -1;
            else
                num_type = 1;
            while ((*cur >= '0') && (*cur <= '9'))
            {
                mult /= 10;
                num += (*cur - '0') * mult;
                cur++;
            }
        }

        if ((num_type == -1) || (*cur == 0))
        {
            duration->is_negative = AXIS2_FALSE;
            duration->years = duration->months = duration->days = duration->hours = duration->mins = 0;
            duration->secs = 0;

            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NONE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }

        while (seq < sizeof(desig))
        {
            if (*cur == desig[seq])
            {
                num_type = 0;
                /*if (seq < (sizeof(desig) - 1))
                {
                    duration->is_negative = AXIS2_FALSE;
                    duration->years = duration->months = duration->days = duration->hours = duration->mins = 0;
                    duration->secs = 0;

                    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NONE,
                                    AXIS2_FAILURE);
                    return AXIS2_FAILURE;
                }*/

                switch (seq)
                {
                case 0:
                    duration->years = (int) num;
                    seq++;
                    break;
                case 1:
                    duration->months = (int) num;
                    seq++;
                    break;
                case 2:
                    duration->days = (int) num;
                    seq++;
                    break;
                case 3:
                    duration->hours = (int) num;
                    seq++;
                    break;
                case 4:
                    duration->mins = (int) num;
                    seq++;
                    break;
                case 5:
                    duration->secs = num;
                    seq++;
                    break;
                }
                break;
            }
            if ((++seq == 3) || (seq == 6))
                return AXIS2_SUCCESS;
        }
        cur++;
    }
    return AXIS2_SUCCESS;

}

AXIS2_EXTERN char *AXIS2_CALL
axutil_duration_serialize_duration(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    axis2_char_t *duration_str = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    duration_str = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                       sizeof(axis2_char_t) * 64);

    if (duration->is_negative == 0)
        sprintf(duration_str, "P%dY%dM%dDT%dH%dM%fS", duration->years,
            duration->months, duration->days, duration->hours,
            duration->mins, duration->secs);
    else
        sprintf(duration_str, "-P%dY%dM%dDT%dH%dM%fS", duration->years,
            duration->months, duration->days, duration->hours,
            duration->mins, duration->secs);

    return duration_str;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_duration(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    axis2_bool_t negative,
    int years,
    int months,
    int days,
    int hours,
    int mins,
    double seconds)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (years > -1)
        duration->years = years;
    if (months > -1)
        duration->months = months;
    if (days > -1)
        duration->days = days;
    if (hours > -1)
        duration->hours = hours;
    if (mins > -1)
        duration->mins = mins;
    if (seconds >= 0)
        duration->secs = seconds;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_duration_get_years(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return duration->years;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_years(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    int years)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (years > -1)
    {
        duration->years = years;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_duration_get_months(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return duration->months;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_months(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    int months)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (months > -1)
    {
        duration->months = months;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_duration_get_days(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return duration->days;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_days(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    int days)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (days > -1)
    {
        duration->days = days;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_duration_get_hours(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return duration->hours;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_hours(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    int hours)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (hours > -1)
    {
        duration->hours = hours;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axutil_duration_get_mins(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return duration->mins;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_mins(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    int mins)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (mins > -1)
    {
        duration->mins = mins;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN double AXIS2_CALL
axutil_duration_get_seconds(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return duration->secs;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_seconds(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    double seconds)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (seconds >= 0)
    {
        duration->secs = seconds;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axutil_duration_get_is_negative(
    axutil_duration_t *duration,
    const axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return duration->is_negative;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axutil_duration_set_is_negative(
    axutil_duration_t *duration,
    const axutil_env_t *env,
    axis2_bool_t is_negative)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    duration->is_negative = is_negative;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axutil_duration_compare(
    axutil_duration_t *duration_one,
    axutil_duration_t *duration_two,
    axutil_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    if (!duration_one || !duration_two)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
            AXIS2_FAILURE);
        return AXIS2_FALSE;
    }

    if (duration_one->is_negative != duration_two->is_negative)
        return AXIS2_FALSE;
    if (duration_one->years != duration_two->years)
        return AXIS2_FALSE;
    if (duration_one->months != duration_two->months)
        return AXIS2_FALSE;
    if (duration_one->days != duration_two->days)
        return AXIS2_FALSE;
    if (duration_one->hours != duration_two->hours)
        return AXIS2_FALSE;
    if (duration_one->mins != duration_two->mins)
        return AXIS2_FALSE;
    if (duration_one->secs != duration_two->secs)
        return AXIS2_FALSE;
    return AXIS2_SUCCESS;
}

