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

#ifndef AXIS2_DATE_TIME_H
#define AXIS2_DATE_TIME_H

#include <axis2_utils_defines.h>
#include <axis2_env.h>

/**
 * @file axis2_date_time.h
 * @brief axis2-util
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup axis2_date_time
     * @ingroup axis2_util
     * @{
     */

    typedef struct axis2_date_time axis2_date_time_t;

    /**
     * Creates axis2_date_time struct with current date time
     * @param env double pointer to environment struct. MUST NOT be NULL
     * @return pointer to newly created axis2_date_time struct
     */
    AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL
    axis2_date_time_create(const axis2_env_t *env);

    /*
     * Creates axis2_date_time struct with an additional offset value
     * If the offset is a positive value then the time will be in the future
     *        offset is 0, then the time will be the current time
     *        offset is a negative value then the time is in the past.
     * @param env double pointer to environment struct. MUST NOT be NULL
     * @param offset the offset from the current time in seconds
     * @return pointer to newly created axis2_date_time struct
     **/
    AXIS2_EXTERN axis2_date_time_t * AXIS2_CALL
    axis2_date_time_create_with_offset(const axis2_env_t *env, int offset);

    /**
     * free the axis2_date_time.
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_date_time_free(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * store the time value from plain text.
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @param time time as a string format HH:MM:TTZ
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_date_time_deserialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const axis2_char_t* time_str);

    /**
     * store the date value from plain text.
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @param date date as a string format YYYY-MM-DD
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_date_time_deserialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const axis2_char_t* date_str);

    /**
     * store the date value from plain text.
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @param date_time string format YYYY-MM-DDTHH:MM:SSZ
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL 
    axis2_date_time_deserialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env,
        const axis2_char_t* date_time_str);

    /**
     * store the date value from set of values
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @param year Integer -1 can be used to ignore
     * @param month Integer -1 can be used to ignore
     * @param date Integer -1 can be used to ignore
     * @param hour Integer -1 can be used to ignore
     * @param min Integer -1 can be used to ignore
     * @param second Integer -1 can be used to ignore
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_date_time_set_date_time(axis2_date_time_t* date_time,
        const axis2_env_t *env,
        int year, int month, int date,
        int hour, int min, int second, int milliseconds);

    /**
     * retrive the stored time  as a string
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return time as a string format HH:MM:SSZ
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axis2_date_time_serialize_time(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrive the stored date as a string
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return date as a string  format YYYY-MM-DD
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axis2_date_time_serialize_date(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrive the stored date time  as a string
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return time as a string format YYYY-MM-DDTHH:MM:SSZ
     */
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
    axis2_date_time_serialize_date_time(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrieve the year of the date time
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return year as an integer 
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_date_time_get_year(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrieve the month of the date time
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return month as an integer 
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_date_time_get_month(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrieve the date of the date time
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return date as an integer 
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_date_time_get_date(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrieve the hour of the date time
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return hour as an integer 
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_date_time_get_hour(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrieve the minute of the date time
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return minute as an integer 
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_date_time_get_minute(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    /**
     * retrieve the second of the date time
     * @param date_time represet the type object
     * @param env pointer to environment struct. MUST NOT be NULL
     * @return second as an integer 
     */
    AXIS2_EXTERN int AXIS2_CALL 
    axis2_date_time_get_second(axis2_date_time_t *date_time,
        const axis2_env_t *env);

    AXIS2_EXTERN int AXIS2_CALL 
    axis2_date_time_get_msec(axis2_date_time_t *date_time,
        const axis2_env_t *env);


#define AXIS2_DATE_TIME_FREE(date_time, env) \
        axis2_date_time_free (date_time, env)

#define AXIS2_DATE_TIME_DESERIALIZE_TIME(date_time, env, time) \
        axis2_date_time_deserialize_time (date_time, env, time)

#define AXIS2_DATE_TIME_DESERIALIZE_DATE(date_time, env, date) \
        axis2_date_time_deserialize_date (date_time, env, date_str)

#define AXIS2_DATE_TIME_DESERIALIZE_DATE_TIME(date_time, env, date_time_str) \
        axis2_date_time_deserialize_date_time(date_time, env, date_time_str)

#define AXIS2_DATE_TIME_SET_DATE_TIME(date_time, env, \
                           year, month, date, hour, min, second, milliseconds) \
        axis2_date_time_set_date_time (date_time, env, \
                           year, month, date, hour, min, second, milliseconds)

#define AXIS2_DATE_TIME_SERIALIZE_TIME(date_time, env) \
        axis2_date_time_serialize_time (date_time, env)

#define AXIS2_DATE_TIME_SERIALIZE_DATE(date_time, env) \
        axis2_date_time_serialize_date (date_time, env)

#define AXIS2_DATE_TIME_SERIALIZE_DATE_TIME(date_time, env) \
        axis2_date_time_serialize_date_time (date_time, env)

#define AXIS2_DATE_TIME_GET_YEAR(date_time, env) \
        axis2_date_time_get_year(date_time, env)

#define AXIS2_DATE_TIME_GET_MONTH(date_time, env) \
        axis2_date_time_get_month(date_time, env)

#define AXIS2_DATE_TIME_GET_DATE(date_time, env) \
        axis2_date_time_get_date(date_time, env)

#define AXIS2_DATE_TIME_GET_HOUR(date_time, env) \
        axis2_date_time_get_hour(date_time, env)

#define AXIS2_DATE_TIME_GET_MINUTE(date_time, env) \
        axis2_date_time_get_minute(date_time, env)

#define AXIS2_DATE_TIME_GET_SECOND(date_time, env) \
        axis2_date_time_get_second(date_time, env)

#define AXIS2_DATE_TIME_GET_MSEC(date_time, env) \
        axis2_date_time_get_msec(date_time, env)

    /** @} */

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_DATE_TIME_H */
