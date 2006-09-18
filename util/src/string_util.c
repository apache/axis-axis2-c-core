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

#include <axis2_string_util.h>
#include <string.h>
#include <axis2_utils.h>

AXIS2_EXTERN axis2_array_list_t * AXIS2_CALL
axis2_tokenize(const axis2_env_t *env,
        axis2_char_t *in,
        int delim)
{
    axis2_array_list_t *list = NULL;
    axis2_char_t *rest = NULL;
    axis2_char_t *str  = NULL;

    axis2_char_t *index = NULL;

    if (!in || AXIS2_STRCMP(in, "") == 0)
        return NULL;
    list = axis2_array_list_create(env, 10);
    if (!list)
        return NULL;

    str = AXIS2_STRDUP(in, env);

    do
    {
        index = strchr(str, delim);
        if ((index == NULL) && (NULL != str) && AXIS2_STRCMP(str, "") != 0)
        {
            AXIS2_ARRAY_LIST_ADD(list, env, str);
            break;
        }

        rest = index + 1;
        str[index - str] = '\0';
        if ((NULL != list) && (NULL != str) && AXIS2_STRCMP(str, "") != 0)
        {

            AXIS2_ARRAY_LIST_ADD(list, env, str);
        }

        if (!rest || AXIS2_STRCMP(rest, "") == 0)
            break;
        str = rest;
        rest = NULL;
        index = NULL;

    }
    while (1);
    return list;
}

AXIS2_EXTERN axis2_array_list_t * AXIS2_CALL
axis2_first_token(const axis2_env_t *env,
        axis2_char_t *in,
        int delim)
{
    axis2_array_list_t *list    = NULL;
    axis2_char_t *str           = NULL;
    axis2_char_t *rest          = NULL;
    axis2_char_t *index         = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (!in && (AXIS2_STRCMP(in, "") == 0))
    {
        return NULL;
    }

    list = axis2_array_list_create(env, 2);
    if (NULL == list)
        return NULL;
    str = AXIS2_STRDUP(in, env);

    index = strchr(str, delim);
    if (index == NULL)
    {
        AXIS2_ARRAY_LIST_ADD(list, env, str);
        AXIS2_ARRAY_LIST_ADD(list, env, AXIS2_STRDUP("", env));
        return list;
    }

    rest = index + 1;
    str[index - str] = '\0';

    AXIS2_ARRAY_LIST_ADD(list, env, str);
    AXIS2_ARRAY_LIST_ADD(list, env, rest);
    return list;
}

AXIS2_EXTERN axis2_array_list_t * AXIS2_CALL
axis2_last_token(const axis2_env_t *env,
        axis2_char_t *in,
        int delim)
{
    axis2_array_list_t *list    = NULL;
    axis2_char_t *str           = NULL;
    axis2_char_t *rest          = NULL;
    axis2_char_t *index         = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    if (!in && (AXIS2_STRCMP(in, "") == 0))
    {
        return NULL;
    }

    list = axis2_array_list_create(env, 2);
    if (NULL == list)
        return NULL;

    str = AXIS2_STRDUP(in, env);
    index = AXIS2_RINDEX(str, delim);

    if (index == NULL)
    {
        AXIS2_ARRAY_LIST_ADD(list, env, AXIS2_STRDUP("", env));
        AXIS2_ARRAY_LIST_ADD(list, env, str);
        return list;
    }

    rest = index + 1;
    str[index - str] = '\0';

    AXIS2_ARRAY_LIST_ADD(list, env, str);
    AXIS2_ARRAY_LIST_ADD(list, env, rest);

    return list;
}

