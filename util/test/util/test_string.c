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

#include <stdio.h>
#include <axis2_error_default.h>
#include <axis2_log.h>
#include <axis2_string.h>

void test_strltrim(const axis2_env_t *env)
{
    axis2_char_t *s = AXIS2_STRDUP("    abcd efgh    ", env);
    axis2_char_t *trimmed = NULL;
    trimmed = AXIS2_STRLTRIM(env, s, " \t\r\n");
    if (0 == AXIS2_STRCMP(trimmed, "abcd efgh    "))
        printf("AXIS2_STRLTRIM successful\n");
    else
        printf("AXIS2_STRLTRIM failed [%s]\n", trimmed);
    if (trimmed)
        AXIS2_FREE(env->allocator, trimmed);
    if (s)
        AXIS2_FREE(env->allocator, s);
}

void test_strrtrim(const axis2_env_t *env)
{
    axis2_char_t *s = AXIS2_STRDUP("abcd efgh    ", env);
    axis2_char_t *trimmed = NULL;
    trimmed = AXIS2_STRRTRIM(env, s, " \t\r\n");
    if (0 == AXIS2_STRCMP(trimmed, "    abcd efgh"))
        printf("AXIS2_STRRTRIM successful\n");
    else
        printf("AXIS2_STRRTRIM failed [%s]\n", trimmed);
    if (trimmed)
        AXIS2_FREE(env->allocator, trimmed);
    if (s)
        AXIS2_FREE(env->allocator, s);
}

void test_strtrim(const axis2_env_t *env)
{
    axis2_char_t *s = AXIS2_STRDUP("    abcd efgh    ", env);
    axis2_char_t *trimmed = NULL;
    trimmed = AXIS2_STRTRIM(env, s, " \t\r\n");
    if (0 == AXIS2_STRCMP(trimmed, "abcd efgh"))
        printf("AXIS2_STRTRIM successful\n");
    else
        printf("AXIS2_STRTRIM failed [%s]\n", trimmed);
    if (trimmed)
        AXIS2_FREE(env->allocator, trimmed);
    if (s)
        AXIS2_FREE(env->allocator, s);
}

void run_test_string(axis2_env_t *env)
{
    if (!env)
        return;
    test_strltrim(env);
    test_strrtrim(env);
    test_strtrim(env);
}

