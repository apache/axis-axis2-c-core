
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

#ifndef TCPMON_UTIL_H
#define TCPMON_UTIL_H

#include <axutil_env.h>
#include <axutil_string.h>
#include <axutil_stream.h>

/**
 * @file tcpmon_util.h
 * @brief hold util functions of tcpmon
 */

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @defgroup hold util functions of tcpmon
     * @ingroup tcpmon
     * @{
     */

    /**
     * format the data as xml
     * @param env pointer to environment struct. MUST NOT be NULL.
     * @param data to be formatted
     */
    axis2_char_t *tcpmon_util_format_as_xml(
        const axutil_env_t * env,
        axis2_char_t * data,
        int format);

	char *str_replace(
		char *str,
		const char *search,
		const char *replace);

    axis2_char_t * tcpmon_util_strcat(
        axis2_char_t * dest,
        axis2_char_t * source,
        int *buff_size,
        const axutil_env_t * env);

    axis2_char_t *
    tcpmon_util_read_current_stream(
        const axutil_env_t * env,
        axutil_stream_t * stream,
        int *stream_size,
        axis2_char_t ** header,
        axis2_char_t ** data);

    char *
    tcpmon_util_str_replace(
        const axutil_env_t *env,    
        char *str,
        const char *search,
        const char *replace);

    int
    tcpmon_util_write_to_file(
        char *filename,
        char *buffer);



    /** @} */

#ifdef __cplusplus
}
#endif

#endif                          /* TCPMON_UTIL_H */
