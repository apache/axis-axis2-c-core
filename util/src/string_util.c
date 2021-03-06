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

#include <axutil_string_util.h>
#include <string.h>
#include <axutil_utils.h>

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axutil_tokenize(
    const axutil_env_t *env,
    axis2_char_t *in,
    int delim)
{
    axutil_array_list_t *list = NULL;
    axis2_char_t *rest = NULL;
    axis2_char_t *str = NULL;
    axis2_char_t *temp = NULL;
    axis2_bool_t loop_state = AXIS2_TRUE;

    axis2_char_t *index = NULL;

    if(!in || !*in)
    {
        return NULL;
    }
    list = axutil_array_list_create(env, 10);
    if(!list)
    {
        return NULL;
    }

    str = axutil_strdup(env, in);
    temp = str;

    do
    {
        index = strchr(str, delim);
        if((!index) && str && *str)
        {
            axutil_array_list_add(list, env, axutil_strdup(env, str));
            break;
        }

        rest = index + 1;
        str[index - str] = '\0';
        if(str && *str)
        {
            axutil_array_list_add(list, env, axutil_strdup(env, str));
        }

        if(!rest || !*rest)
        {
            break;
        }
        str = rest;
        rest = NULL;
        index = NULL;

    }
    while(loop_state);
    if(temp)
    {
        AXIS2_FREE(env->allocator, temp);
    }
    return list;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axutil_first_token(
    const axutil_env_t *env,
    axis2_char_t *in,
    int delim)
{
    axutil_array_list_t *list = NULL;
    axis2_char_t *str = NULL;
    axis2_char_t *rest = NULL;
    axis2_char_t *index = NULL;

    if(!in && !*in)
    {
        return NULL;
    }

    list = axutil_array_list_create(env, 2);
    if(!list)
    {
        return NULL;
    }
    str = axutil_strdup(env, in);

    index = strchr(str, delim);
    if(!index)
    {
        axutil_array_list_add(list, env, str);
        axutil_array_list_add(list, env, axutil_strdup(env, ""));
        return list;
    }

    rest = index + 1;
    str[index - str] = '\0';

    axutil_array_list_add(list, env, str);
    axutil_array_list_add(list, env, axutil_strdup(env, rest));
    return list;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axutil_last_token(
    const axutil_env_t *env,
    axis2_char_t *in,
    int delim)
{
    axutil_array_list_t *list = NULL;
    axis2_char_t *str = NULL;
    axis2_char_t *rest = NULL;
    axis2_char_t *index = NULL;

    if(!in && !*in)
    {
        return NULL;
    }

    list = axutil_array_list_create(env, 2);
    if(!list)
    {
        return NULL;
    }

    str = axutil_strdup(env, in);
    index = axutil_rindex(str, (axis2_char_t)delim);
    /* We are sure that the conversion is safe */
    if(!index)
    {
        axutil_array_list_add(list, env, axutil_strdup(env, ""));
        axutil_array_list_add(list, env, str);
        return list;
    }

    rest = index + 1;
    str[index - str] = '\0';

    axutil_array_list_add(list, env, str);
    axutil_array_list_add(list, env, axutil_strdup(env, rest));

    return list;
}
