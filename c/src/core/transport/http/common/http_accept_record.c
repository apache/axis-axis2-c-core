
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

#include <axis2_http_accept_record.h>
#include <axutil_string.h>
#include <axutil_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <axis2_http_transport.h>

struct axis2_http_accept_record
{
    axis2_char_t *name;
    float quality;
    int level;
    axis2_char_t *record;
};

AXIS2_EXTERN axis2_http_accept_record_t *AXIS2_CALL
axis2_http_accept_record_create(
    const axutil_env_t * env,
    const axis2_char_t * str)
{
    axis2_char_t *tmp_accept_record = NULL;
    axis2_char_t *tmp = NULL;
    axis2_http_accept_record_t *accept_record = NULL;
    float quality = 1.0;
    int level = -1;
    axis2_char_t *name = NULL;

    AXIS2_PARAM_CHECK(env->error, str, NULL);

    tmp_accept_record = (axis2_char_t *) axutil_strdup(env, str);
    if (!tmp_accept_record)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, 
                         "unable to strdup string %s", str);
        return NULL;
    }

    accept_record = (axis2_http_accept_record_t *) AXIS2_MALLOC
        (env->allocator, sizeof(axis2_http_accept_record_t));

    if (!accept_record)
    {
        AXIS2_HANDLE_ERROR (env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    memset ((void *)accept_record, 0, sizeof (axis2_http_accept_record_t));
    accept_record->record = axutil_strtrim(env, tmp_accept_record, AXIS2_SPACE_COMMA);

    tmp = strchr(tmp_accept_record, AXIS2_Q);
    if (tmp)
    {
        *tmp = AXIS2_ESC_NULL;
        tmp++;
        tmp = axutil_strtrim(env, tmp, AXIS2_EQ_N_SEMICOLON);
        if (tmp)
        {
            sscanf(tmp, "%f", &quality);
            AXIS2_FREE(env->allocator, tmp);
        }
    }

    tmp = strstr(tmp_accept_record, AXIS2_LEVEL);
    if (tmp)
    {
        *tmp = AXIS2_ESC_NULL;
        tmp++;
        tmp = axutil_strtrim(env, tmp, AXIS2_EQ_N_SEMICOLON);
        if (tmp)
        {
            sscanf(tmp, "%d", &level);
            AXIS2_FREE(env->allocator, tmp);
        }
    }

    tmp = axutil_strtrim(env, tmp_accept_record, AXIS2_SPACE_SEMICOLON);
    if (tmp)
    {
        name = tmp;
    }

    if (!name || quality > 1.0 || quality < 0.0)
    {
        axis2_http_accept_record_free(accept_record, env);
        return NULL;
    }

    accept_record->name = name;
    accept_record->quality = quality;
    accept_record->level = level;

    AXIS2_FREE(env->allocator, tmp_accept_record);
    return accept_record;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_accept_record_free(
    axis2_http_accept_record_t * accept_record,
    const axutil_env_t * env)
{

    if (!accept_record)
    {
        return;
    }

    if (accept_record->name)
    {
        AXIS2_FREE(env->allocator, accept_record->name);
    }
    if (accept_record->record)
    {
        AXIS2_FREE(env->allocator, accept_record->record);
    }
    AXIS2_FREE(env->allocator, accept_record);
    return;
}

AXIS2_EXTERN float AXIS2_CALL
axis2_http_accept_record_get_quality_factor(
    const axis2_http_accept_record_t * accept_record,
    const axutil_env_t * env)
{
    return accept_record->quality;
}


AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_accept_record_get_name(
    const axis2_http_accept_record_t * accept_record,
    const axutil_env_t * env)
{
    return accept_record->name;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_http_accept_record_get_level(
    const axis2_http_accept_record_t * accept_record,
    const axutil_env_t * env)
{
    return accept_record->level;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_accept_record_to_string(
    axis2_http_accept_record_t * accept_record,
    const axutil_env_t * env)
{
    return accept_record->record;
}
