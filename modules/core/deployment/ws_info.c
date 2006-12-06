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

#include "axis2_ws_info.h"
#include <string.h>

typedef struct axis2_ws_info_impl
{
    axis2_ws_info_t ws_info;

    axis2_char_t *file_name;
    long last_modified_date;
    /**
     * To check whether the file is a module or a servise
     */
    int type;


}
axis2_ws_info_impl_t;

#define AXIS2_INTF_TO_IMPL(ws_info) \
    ((axis2_ws_info_impl_t *) ws_info)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_ws_info_free(
    axis2_ws_info_t *ws_info,
    const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_ws_info_get_file_name(
    const axis2_ws_info_t *ws_info,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_ws_info_set_file_name(
    axis2_ws_info_t *ws_info,
    const axis2_env_t *env,
    axis2_char_t *file_name);

long AXIS2_CALL
axis2_ws_info_get_last_modified_date(
    const axis2_ws_info_t *ws_info,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_ws_info_set_last_modified_date(
    axis2_ws_info_t *ws_info,
    const axis2_env_t *env,
    long last_modified_date);

int AXIS2_CALL
axis2_ws_info_get_type(
    const axis2_ws_info_t *ws_info,
    const axis2_env_t *env);

/************************** End of function prototypes ************************/

axis2_ws_info_t *AXIS2_CALL
axis2_ws_info_create_with_file_name_and_last_modified_date(
    const axis2_env_t *env,
    axis2_char_t *file_name,
    long last_modified_date)
{
    axis2_ws_info_impl_t *ws_info_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, file_name, NULL);

    ws_info_impl = (axis2_ws_info_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_ws_info_impl_t));


    if (NULL == ws_info_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ws_info_impl->file_name = NULL;
    ws_info_impl->last_modified_date = 0;
    ws_info_impl->type = 0;
    ws_info_impl->ws_info.ops = NULL;

    ws_info_impl->file_name = AXIS2_STRDUP(file_name, env);
    if (!ws_info_impl->file_name)
    {
        axis2_ws_info_free(&(ws_info_impl->ws_info), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ws_info_impl->last_modified_date = last_modified_date;

    ws_info_impl->ws_info.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_ws_info_ops_t));
    if (NULL == ws_info_impl->ws_info.ops)
    {
        axis2_ws_info_free(&(ws_info_impl->ws_info), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    ws_info_impl->ws_info.ops->free =  axis2_ws_info_free;
    ws_info_impl->ws_info.ops->get_file_name = axis2_ws_info_get_file_name;
    ws_info_impl->ws_info.ops->set_file_name = axis2_ws_info_set_file_name;
    ws_info_impl->ws_info.ops->get_last_modified_date =
        axis2_ws_info_get_last_modified_date;
    ws_info_impl->ws_info.ops->set_last_modified_date =
        axis2_ws_info_set_last_modified_date;
    ws_info_impl->ws_info.ops->get_type = axis2_ws_info_get_type;

    return &(ws_info_impl->ws_info);
}

axis2_ws_info_t *AXIS2_CALL
axis2_ws_info_create_with_file_name_and_last_modified_date_and_type(
    const axis2_env_t *env,
    axis2_char_t *file_name,
    long last_modified_date,
    int type)
{
    axis2_ws_info_impl_t *ws_info_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, file_name, NULL);

    ws_info_impl = (axis2_ws_info_impl_t *)
            axis2_ws_info_create_with_file_name_and_last_modified_date(env,
                    file_name, last_modified_date);
    if (!ws_info_impl)
    {
        axis2_ws_info_free(&(ws_info_impl->ws_info), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    ws_info_impl->type = type;
    return &(ws_info_impl->ws_info);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_ws_info_free(
    axis2_ws_info_t *ws_info,
    const axis2_env_t *env)
{
    axis2_ws_info_impl_t *ws_info_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    ws_info_impl = AXIS2_INTF_TO_IMPL(ws_info);

    if (ws_info->ops)
    {
        AXIS2_FREE(env->allocator, ws_info->ops);
        ws_info->ops = NULL;
    }

    if (ws_info_impl->file_name)
    {
        AXIS2_FREE(env->allocator, ws_info_impl->file_name);
        ws_info_impl->file_name = NULL;
    }

    if (ws_info_impl)
    {
        AXIS2_FREE(env->allocator, ws_info_impl);
        ws_info_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_ws_info_get_file_name(
    const axis2_ws_info_t *ws_info,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(ws_info)->file_name;
}

axis2_status_t AXIS2_CALL
axis2_ws_info_set_file_name(
    axis2_ws_info_t *ws_info,
    const axis2_env_t *env,
    axis2_char_t *file_name)
{
    axis2_ws_info_impl_t *ws_info_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, file_name, AXIS2_FAILURE);

    ws_info_impl = AXIS2_INTF_TO_IMPL(ws_info);
    if (ws_info_impl->file_name)
    {
        AXIS2_FREE(env->allocator, ws_info_impl->file_name);
        ws_info_impl->file_name = NULL;
    }
    ws_info_impl->file_name = file_name;
    return AXIS2_SUCCESS;
}

long AXIS2_CALL
axis2_ws_info_get_last_modified_date(
    const axis2_ws_info_t *ws_info,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    return AXIS2_INTF_TO_IMPL(ws_info)->last_modified_date;
}

axis2_status_t AXIS2_CALL
axis2_ws_info_set_last_modified_date(
    axis2_ws_info_t *ws_info,
    const axis2_env_t *env,
    long last_modified_date)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(ws_info)->last_modified_date = last_modified_date;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_ws_info_get_type(
    const axis2_ws_info_t *ws_info,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(ws_info)->type;
}
