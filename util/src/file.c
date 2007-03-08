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

#include <axis2_file.h>
#include <axis2_string.h>

struct axis2_file
{
    axis2_char_t *name;
    axis2_char_t *path;
    AXIS2_TIME_T timestamp;
};

AXIS2_EXTERN axis2_file_t *AXIS2_CALL
axis2_file_create(const axis2_env_t *env)
{
    axis2_file_t *file = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    file = (axis2_file_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_file_t));

    if (NULL == file)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    file->name = NULL;
    file->path = NULL;
    file->timestamp = 0;

    return file;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_file_free(axis2_file_t *file,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (file->name)
    {
        AXIS2_FREE(env->allocator, file->name);
    }

    if (file->path)
    {
        AXIS2_FREE(env->allocator, file->path);
    }

    if (file)
    {
        AXIS2_FREE(env->allocator, file);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_file_set_name(axis2_file_t *file,
        const axis2_env_t *env,
        axis2_char_t *name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    if (file->name)
    {
        AXIS2_FREE(env->allocator, file->name);
    }
    file->name = AXIS2_STRDUP(name, env);
    if (!file->name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_file_get_name(axis2_file_t *file,
        const axis2_env_t *env)
{
    if (!file->name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_FILE_NAME_NOT_SET,
                AXIS2_FAILURE);
        return NULL;
    }
    return (file->name);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_file_set_path(axis2_file_t *file,
        const axis2_env_t *env,
        axis2_char_t *path)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (NULL == path)
    {
        /* path is optional */
        return AXIS2_SUCCESS;
    }

    if (file->path)
    {
        AXIS2_FREE(env->allocator, file->path);
    }
    file->path = AXIS2_STRDUP(path, env);
    if (!(file->path))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_file_get_path(axis2_file_t *file,
        const axis2_env_t *env)
{
    if (!(file->path))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_FILE_NAME_NOT_SET,
                AXIS2_FAILURE);
        return NULL;
    }

    return (file->path);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_file_set_timestamp(axis2_file_t *file,
        const axis2_env_t *env,
        AXIS2_TIME_T timestamp)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    file->timestamp = timestamp;
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN AXIS2_TIME_T AXIS2_CALL
axis2_file_get_timestamp(axis2_file_t *file,
        const axis2_env_t *env)
{
    return file->timestamp;
}

AXIS2_EXTERN axis2_file_t *AXIS2_CALL
axis2_file_clone(axis2_file_t *file,
        const axis2_env_t *env)
{
    axis2_file_t *new_file = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    AXIS2_ENV_CHECK(env, NULL);
    new_file = axis2_file_create(env);
    status =  axis2_file_set_name(new_file, env, file->name);
    if (AXIS2_SUCCESS != status)
    {
        return NULL;
    }
    status =  axis2_file_set_path(new_file, env, file->path);
    if (AXIS2_SUCCESS != status)
    {
        return NULL;
    }
     axis2_file_set_timestamp(new_file, env, file->timestamp);
    return new_file;
}


