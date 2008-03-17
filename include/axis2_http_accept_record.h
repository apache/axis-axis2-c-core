
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

#ifndef AXIS2_HTTP_ACCEPT_RECORD_H
#define AXIS2_HTTP_ACCEPT_RECORD_H

/**
 * @defgroup axis2_http_accept_record HTTP Accept record
 * @ingroup axis2_core_trans_http
 * @{
 */

/**
 * @file axis2_http_accept_record.h
 * @brief Axis2 HTTP Accept record
 */

#include <axis2_const.h>
#include <axis2_defines.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_http_accept_record */
    typedef struct axis2_http_accept_record axis2_http_accept_record_t;

    /**
     * Gets quality factor of accept record
     * @param accept_record pointer to accept record
     * @param env pointer to environment struct
     * @return quality factor between 0 and 1 (1 meaning maximum
     * and 0 meaning minimum)
     */
    AXIS2_EXTERN float AXIS2_CALL
    axis2_http_accept_record_get_quality_factor(
        const axis2_http_accept_record_t * accept_record,
        const axutil_env_t * env);

    /**
     * Gets name of accept record
     * @param accept_record pointer to accept record
     * @param env pointer to environment struct
     * @return name of accept record
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_accept_record_get_name(
        const axis2_http_accept_record_t * accept_record,
        const axutil_env_t * env);

    /**
     * Gets level of accept record
     * @param accept_record pointer to accept record
     * @param env pointer to environment struct
     * @return level of accept record (1 meaning highest).
     * A return value of -1 indicates that no level is 
     * associated.
     */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_http_accept_record_get_level(
        const axis2_http_accept_record_t * accept_record,
        const axutil_env_t * env);

    /**
     * Gets string representation of accept record
     * @param accept_record pointer to accept record
     * @param env pointer to environment struct
     * @return created accept record string
     */
    AXIS2_EXTERN axis2_char_t *AXIS2_CALL
    axis2_http_accept_record_to_string(
        axis2_http_accept_record_t * accept_record,
        const axutil_env_t * env);

    /**
     * Frees accept record
     * @param accept_record pointer to accept record
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN void AXIS2_CALL
    axis2_http_accept_record_free(
        axis2_http_accept_record_t * accept_record,
        const axutil_env_t * env);

    /**
     * Creates accept record
     * @param env pointer to environment struct
     * @param str pointer to str
     * @return created accept record
     */
    AXIS2_EXTERN axis2_http_accept_record_t *AXIS2_CALL
    axis2_http_accept_record_create(
        const axutil_env_t * env,
        const axis2_char_t * str);

    /** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_HTTP_ACCEPT_RECORD_H */
