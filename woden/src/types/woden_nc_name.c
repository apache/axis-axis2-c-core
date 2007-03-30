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

#include <woden_nc_name.h>
#include <woden_xml_char.h>

typedef struct woden_nc_name_impl woden_nc_name_impl_t;

/**
 * @brief NC Name Struct Impl
 *   Axis2 NC Name
 */
struct woden_nc_name_impl
{
    woden_nc_name_t nc_name;
    axis2_char_t *f_value;
};

#define INTF_TO_IMPL(nc_name) ((woden_nc_name_impl_t *) nc_name)

axis2_status_t AXIS2_CALL
woden_nc_name_free(
    void *nc_name,
    const axutil_env_t *envv);

axis2_bool_t AXIS2_CALL
woden_nc_name_is_valid(
    void *nc_name,
    const axutil_env_t *env,
    axis2_char_t *st_value);

axis2_status_t AXIS2_CALL
woden_nc_name_set_value(
    void *nc_name,
    const axutil_env_t *env,
    axis2_char_t *value);

axis2_char_t *AXIS2_CALL
woden_nc_name_to_string(
    void *nc_name,
    const axutil_env_t *env);

AXIS2_EXTERN woden_nc_name_t * AXIS2_CALL
woden_nc_name_create(
    const axutil_env_t *env,
    axis2_char_t *value)
{
    woden_nc_name_impl_t *nc_name_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    nc_name_impl = AXIS2_MALLOC(env->allocator,
            sizeof(woden_nc_name_impl_t));

    nc_name_impl->f_value = NULL;

    nc_name_impl->nc_name.ops = AXIS2_MALLOC(env->allocator,
            sizeof(woden_nc_name_ops_t));

    nc_name_impl->nc_name.ops->free = woden_nc_name_free;
    nc_name_impl->nc_name.ops->is_valid = woden_nc_name_is_valid;
    nc_name_impl->nc_name.ops->set_value = woden_nc_name_set_value;
    nc_name_impl->nc_name.ops->to_string = woden_nc_name_to_string;

    woden_nc_name_set_value(&(nc_name_impl->nc_name), env, value);

    return &(nc_name_impl->nc_name);
}

axis2_status_t AXIS2_CALL
woden_nc_name_free(
    void *nc_name,
    const axutil_env_t *env)
{
    woden_nc_name_impl_t *nc_name_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    nc_name_impl = INTF_TO_IMPL(nc_name);

    if (nc_name_impl->f_value)
    {
        AXIS2_FREE(env->allocator, nc_name_impl->f_value);
        nc_name_impl->f_value = NULL;
    }

    if ((&(nc_name_impl->nc_name))->ops)
    {
        AXIS2_FREE(env->allocator, (&(nc_name_impl->nc_name))->ops);
        (&(nc_name_impl->nc_name))->ops = NULL;
    }

    if (nc_name_impl)
    {
        AXIS2_FREE(env->allocator, nc_name_impl);
        nc_name_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
woden_nc_name_is_valid(
    void *nc_name,
    const axutil_env_t *env,
    axis2_char_t *st_value)
{
    woden_nc_name_impl_t *nc_name_impl = NULL;
    int scan = 0;
    axis2_bool_t b_valid = AXIS2_TRUE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    nc_name_impl = INTF_TO_IMPL(nc_name);

    for (scan = 0; scan < axis2_strlen(st_value); scan++)
    {
        if (scan == 0)
            b_valid = woden_xml_char_is_nc_name_start(st_value[scan]);
        else
            b_valid = woden_xml_char_is_nc_name(st_value[scan]);
        if (b_valid == AXIS2_FALSE)
            break;
    }
    return b_valid;
}

axis2_status_t AXIS2_CALL
woden_nc_name_set_value(
    void *nc_name,
    const axutil_env_t *env,
    axis2_char_t *value)
{
    woden_nc_name_impl_t *nc_name_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    nc_name_impl = INTF_TO_IMPL(nc_name);

    if (AXIS2_TRUE != woden_nc_name_is_valid(nc_name, env, value))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_STRING_DOES_NOT_REPRESENT_A_VALID_NC_NAME,
                AXIS2_FAILURE);
    }
    if (nc_name_impl->f_value)
    {
        AXIS2_FREE(env->allocator, nc_name_impl->f_value);
        nc_name_impl->f_value = NULL;
    }
    nc_name_impl->f_value = axis2_strdup(env, value);

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
woden_nc_name_to_string(
    void *nc_name,
    const axutil_env_t *env)
{
    woden_nc_name_impl_t *nc_name_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    nc_name_impl = INTF_TO_IMPL(nc_name);

    return nc_name_impl->f_value;
}

