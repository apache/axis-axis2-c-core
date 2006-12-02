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

#include <axis2_properties.h>
#include <axis2_string.h>
#include <axis2_utils.h>

typedef struct axis2_properties_impl
{
    axis2_properties_t properties;
    axis2_hash_t *prop_hash;
}
axis2_properties_impl_t;

#define AXIS2_INTF_TO_IMPL(properties) \
    ((axis2_properties_impl_t *) properties)

axis2_status_t AXIS2_CALL
axis2_properties_free(axis2_properties_t *properties,
        const axis2_env_t *env);

axis2_char_t* AXIS2_CALL
axis2_properties_get_property(axis2_properties_t *properties,
        const axis2_env_t *env,
        axis2_char_t *key);

axis2_hash_t* AXIS2_CALL
axis2_properties_get_all(axis2_properties_t *properties,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_properties_set_property(axis2_properties_t *properties,
        const axis2_env_t *env,
        axis2_char_t *key,
        axis2_char_t *value);

axis2_status_t AXIS2_CALL
axis2_properties_load(axis2_properties_t *properties,
        const axis2_env_t *env,
        FILE *input);

axis2_status_t AXIS2_CALL
axis2_properties_store(axis2_properties_t *properties,
        const axis2_env_t *env,
        FILE *output);

axis2_char_t*
axis2_properties_read_next(axis2_char_t* cur);

axis2_char_t*
axis2_properties_trunk_and_dup(axis2_char_t* start, axis2_char_t* end,
        const axis2_env_t* env);

axis2_char_t*
axis2_properties_read(FILE* input,
        const axis2_env_t* env);

/************************** End of function prototypes ************************/

axis2_properties_t *AXIS2_CALL
axis2_properties_create(const axis2_env_t *env)
{
    axis2_properties_impl_t *properties_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    properties_impl = (axis2_properties_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_properties_impl_t));

    if (NULL == properties_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    properties_impl->prop_hash = axis2_hash_make(env);

    properties_impl->properties.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_properties_ops_t));
    if (NULL == properties_impl->properties.ops)
    {
        axis2_properties_free(&(properties_impl->properties), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    properties_impl->properties.ops->free =  axis2_properties_free;
    properties_impl->properties.ops->get_property = axis2_properties_get_property;
    properties_impl->properties.ops->set_property = axis2_properties_set_property;
    properties_impl->properties.ops->load = axis2_properties_load;
    properties_impl->properties.ops->store = axis2_properties_store;
    properties_impl->properties.ops->get_all = axis2_properties_get_all;
    return &(properties_impl->properties);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_properties_free(axis2_properties_t *properties,
        const axis2_env_t *env)
{
    axis2_properties_impl_t *properties_impl = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;
    axis2_hash_index_t *hi = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    properties_impl = AXIS2_INTF_TO_IMPL(properties);

    if (properties_impl->prop_hash)
    {
        for (hi = axis2_hash_first(properties_impl->prop_hash, env);
                hi; hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, (void*)&key, NULL, (void*)&value);
            if (key)
                AXIS2_FREE(env-> allocator, key);
            if (value)
                AXIS2_FREE(env-> allocator, value);
        }
        axis2_hash_free(properties_impl->prop_hash, env);
    }

    if (properties_impl->properties.ops)
    {
        AXIS2_FREE(env->allocator, properties_impl->properties.ops);
        properties_impl->properties.ops = NULL;
    }

    if (properties_impl)
    {
        AXIS2_FREE(env->allocator, properties_impl);
        properties_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL
axis2_properties_get_property(axis2_properties_t *properties,
        const axis2_env_t *env,
        axis2_char_t *key)
{
    axis2_properties_impl_t *properties_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env-> error, key, NULL);

    properties_impl = AXIS2_INTF_TO_IMPL(properties);

    return axis2_hash_get(properties_impl-> prop_hash,
            key, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_properties_set_property(axis2_properties_t *properties,
        const axis2_env_t *env,
        axis2_char_t *key,
        axis2_char_t *value)
{
    axis2_properties_impl_t *properties_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env-> error, key, AXIS2_FAILURE);

    properties_impl = AXIS2_INTF_TO_IMPL(properties);

    axis2_hash_set(properties_impl-> prop_hash, key,
            AXIS2_HASH_KEY_STRING, value);
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL
axis2_properties_get_all(axis2_properties_t *properties,
        const axis2_env_t *env)
{
    axis2_properties_impl_t *properties_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    properties_impl = AXIS2_INTF_TO_IMPL(properties);

    return properties_impl-> prop_hash;
}

axis2_status_t AXIS2_CALL
axis2_properties_store(axis2_properties_t *properties,
        const axis2_env_t *env,
        FILE *output)
{
    axis2_properties_impl_t *properties_impl = NULL;
    axis2_hash_index_t *hi = NULL;
    axis2_char_t *key = NULL;
    axis2_char_t *value = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env-> error, output, AXIS2_FAILURE);
    properties_impl = AXIS2_INTF_TO_IMPL(properties);


    if (properties_impl->prop_hash)
    {
        for (hi = axis2_hash_first(properties_impl->prop_hash, env);
                hi; hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, (void*)&key, NULL, (void*)&value);
            if (key)
            {
                if (NULL == value)
                {
                    value = AXIS2_STRDUP("", env);
                }
                fprintf(output, "%s=%s\n", key, value);
            }
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_properties_load(axis2_properties_t *properties,
        const axis2_env_t *env,
        FILE *input)
{
    axis2_properties_impl_t *properties_impl = NULL;
    axis2_char_t *cur = NULL;
    axis2_char_t *tag = NULL;
    const int LINE_STARTED = -1;
    const int LINE_MIDWAY = 0;
    const int EQUAL_FOUND = 1;
    const int LINE_HALFWAY = 2;
    int status = LINE_STARTED;

    axis2_char_t *key = NULL;
    axis2_hash_t *prop_hash = NULL;
    axis2_char_t *buffer = NULL;
    axis2_char_t loginfo[1024];

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env-> error, input, AXIS2_FAILURE);

    properties_impl = AXIS2_INTF_TO_IMPL(properties);
    prop_hash = properties_impl-> prop_hash;

    buffer = axis2_properties_read(input, env);

    if (buffer == NULL)
    {
        sprintf(loginfo, "error in reading file\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                loginfo);
        AXIS2_FREE(env-> allocator, buffer);
        return AXIS2_FAILURE;
    }



    for (cur = axis2_properties_read_next(buffer); *cur ;
            cur = axis2_properties_read_next(++cur))
    {
        if (*cur == '\r')
        {
            *cur = '\0';
        }
        else if (*cur != '\0' && *cur != '\n' && status == LINE_STARTED)
        {
            tag = cur;
            status = LINE_MIDWAY;
        }
        /* equal found just create a property */
        else if (*cur == '=' &&  status == LINE_MIDWAY)
        {
            *cur = '\0';
            if (status != LINE_MIDWAY)
            {
                sprintf(loginfo, "equal apear in wrong place around %s\n", tag);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        loginfo);
                AXIS2_FREE(env-> allocator, buffer);
                return AXIS2_FAILURE;
            }
            status = EQUAL_FOUND;
            key =  axis2_properties_trunk_and_dup(tag, cur, env);
        }
        /* right next to the equal found */
        else if (status == EQUAL_FOUND)
        {
            tag = cur;
            status = LINE_HALFWAY;
        }

        else if (*cur == '\n')
        {
            *cur = '\0';
            if (status == LINE_HALFWAY)
            {
                tag =  axis2_properties_trunk_and_dup(tag, cur, env);
                axis2_hash_set(prop_hash,
                        key, AXIS2_HASH_KEY_STRING, tag);
            }
            status = LINE_STARTED;
        }
    }
    if (status == LINE_HALFWAY)
    {
        *cur = '\0';
        tag =  axis2_properties_trunk_and_dup(tag, cur, env);
        axis2_hash_set(prop_hash,
                key, AXIS2_HASH_KEY_STRING, tag);
        status = LINE_STARTED;
    }
    if (status != LINE_STARTED)
    {
        sprintf(loginfo, "error parsing properties\n");
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                loginfo);
        AXIS2_FREE(env-> allocator, buffer);
        return AXIS2_FAILURE;
    }
    AXIS2_FREE(env-> allocator, buffer);
    return AXIS2_SUCCESS;
}


axis2_char_t*
axis2_properties_read_next(axis2_char_t* cur)
{
    /* ignore comment */
    if (*cur == '#')
    {
        for (;*cur != '\n' && *cur != '\0'; cur ++);
    }
    /* check '\\''\n' case */
    if (*cur == '\\' && *(cur + 1) == '\n')
    {
        /* ignore two axis2_char_ts */
        *(cur++) = ' ';
        *(cur++) = ' ';
    }
    return cur;
}

axis2_char_t*
axis2_properties_trunk_and_dup(axis2_char_t* start, axis2_char_t* end,
        const axis2_env_t* env)
{
    for (; *start == ' '; start ++); /* remove front spaces */
    for (end --; *end == ' '; end --); /* remove rear spaces */
    *(++end) = '\0';
    start = (axis2_char_t*)AXIS2_STRDUP(start, env);
    return start;
}

#define MAX_SIZE 1024
#define MAX_ALLOC (MAX_SIZE * 64)

axis2_char_t*
axis2_properties_read(FILE* input,
        const axis2_env_t* env)
{
    int nread = 0;
    axis2_char_t* out_stream = NULL;
    int ncount = 0;
    size_t curr_alloc = MAX_SIZE * 2;
    size_t total_alloc = curr_alloc;

    out_stream = (axis2_char_t*) AXIS2_MALLOC(env-> allocator,
            sizeof(axis2_char_t) * curr_alloc);
    if (out_stream == NULL)
    {
        return NULL;
    }

    do
    {
        nread = fread(out_stream + ncount, sizeof(axis2_char_t), MAX_SIZE,
                      input);
        ncount += nread;

        if (ncount + MAX_SIZE > total_alloc)
        {
            axis2_char_t *new_stream = NULL;
            if (curr_alloc < MAX_ALLOC)
            {
                curr_alloc *= 2;
            }

            total_alloc += curr_alloc;
            new_stream = AXIS2_MALLOC(env->allocator,
                                           sizeof(axis2_char_t) * total_alloc);
            if (new_stream == NULL)
            {
                return NULL;
            }

            memcpy(new_stream, out_stream, sizeof(axis2_char_t) * ncount);
            out_stream = new_stream;
        }
    }
    while (nread == MAX_SIZE);

    out_stream[ncount] = '\0';
    return out_stream;
}

