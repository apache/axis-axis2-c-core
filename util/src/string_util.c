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

#include <axis2_string_util.h>
#include <string.h>
#include <axis2_utils.h>

AXIS2_EXTERN axutil_array_list_t * AXIS2_CALL
axis2_tokenize(const axutil_env_t *env,
    axis2_char_t *in,
    int delim)
{
    axutil_array_list_t *list = NULL;
    axis2_char_t *rest = NULL;
    axis2_char_t *str  = NULL;

    axis2_char_t *index = NULL;

    if (!in || axis2_strcmp(in, "") == 0)
    {
        return NULL;
    }
    list = axutil_array_list_create(env, 10);
    if (!list)
    {
        return NULL;
    }

    str = axis2_strdup(env, in);

    do
    {
        index = strchr(str, delim);
        if ((!index) && (str) && axis2_strcmp(str, "") != 0)
        {
            axutil_array_list_add(list, env, str);
            break;
        }

        rest = index + 1;
        str[index - str] = '\0';
        if ((list) && (str) && axis2_strcmp(str, "") != 0)
        {

            axutil_array_list_add(list, env, str);
        }

        if (!rest || axis2_strcmp(rest, "") == 0)
	{
            break;
	}
        str = rest;
        rest = NULL;
        index = NULL;

    }
    while (1);
    return list;
}

AXIS2_EXTERN axutil_array_list_t * AXIS2_CALL
axis2_first_token(const axutil_env_t *env,
    axis2_char_t *in,
    int delim)
{
    axutil_array_list_t *list    = NULL;
    axis2_char_t *str           = NULL;
    axis2_char_t *rest          = NULL;
    axis2_char_t *index         = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (!in && (axis2_strcmp(in, "") == 0))
    {
        return NULL;
    }

    list = axutil_array_list_create(env, 2);
    if (!list)
    {
        return NULL;
    }
    str = axis2_strdup(env, in);

    index = strchr(str, delim);
    if (!index)
    {
        axutil_array_list_add(list, env, str);
        axutil_array_list_add(list, env, axis2_strdup(env, ""));
        return list;
    }

    rest = index + 1;
    str[index - str] = '\0';

    axutil_array_list_add(list, env, str);
    axutil_array_list_add(list, env, rest);
    return list;
}

AXIS2_EXTERN axutil_array_list_t * AXIS2_CALL
axis2_last_token(const axutil_env_t *env,
    axis2_char_t *in,
    int delim)
{
    axutil_array_list_t *list    = NULL;
    axis2_char_t *str           = NULL;
    axis2_char_t *rest          = NULL;
    axis2_char_t *index         = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (!in && (axis2_strcmp(in, "") == 0))
    {
        return NULL;
    }

    list = axutil_array_list_create(env, 2);
    if (!list)
    {
        return NULL;
    }

    str = axis2_strdup(env, in);
    index = axis2_rindex(str, delim);

    if (!index)
    {
        axutil_array_list_add(list, env, axis2_strdup(env, ""));
        axutil_array_list_add(list, env, str);
        return list;
    }

    rest = index + 1;
    str[index - str] = '\0';

    axutil_array_list_add(list, env, str);
    axutil_array_list_add(list, env, rest);

    return list;
}

