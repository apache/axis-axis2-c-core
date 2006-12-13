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

#include <axis2_property.h>

typedef struct axis2_property_impl
{
    axis2_property_t property;
    axis2_scope_t scope;
    AXIS2_FREE_VOID_ARG free_func;
    void *value;

}
axis2_property_impl_t;

#define AXIS2_INTF_TO_IMPL(property) \
    ((axis2_property_impl_t *) property)

axis2_status_t AXIS2_CALL
axis2_property_free(axis2_property_t *property,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_property_set_scope(axis2_property_t *property,
        const axis2_env_t *env,
        axis2_scope_t scope);

axis2_status_t AXIS2_CALL
axis2_property_set_free_func(axis2_property_t *property,
        const axis2_env_t *env,
        AXIS2_FREE_VOID_ARG free_func);

axis2_status_t AXIS2_CALL
axis2_property_set_value(axis2_property_t *property,
        const axis2_env_t *env,
        void *value);

void *AXIS2_CALL
axis2_property_get_value(axis2_property_t *property,
        const axis2_env_t *env);

/************************** End of function prototypes ************************/

axis2_property_t *AXIS2_CALL
axis2_property_create(const axis2_env_t *env)
{
    axis2_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property_impl = (axis2_property_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_property_impl_t));

    if (NULL == property_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    property_impl->value = NULL;
    property_impl->scope = AXIS2_SCOPE_REQUEST;
    property_impl->free_func = 0;

    property_impl->property.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_property_ops_t));
    if (NULL == property_impl->property.ops)
    {
        axis2_property_free(&(property_impl->property), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    property_impl->property.ops->free =  axis2_property_free;
    property_impl->property.ops->set_free_func = axis2_property_set_free_func;
    property_impl->property.ops->set_scope = axis2_property_set_scope;
    property_impl->property.ops->set_value = axis2_property_set_value;
    property_impl->property.ops->get_value = axis2_property_get_value;
    return &(property_impl->property);
}
/*****************************************************************************/
AXIS2_EXTERN axis2_property_t *AXIS2_CALL
axis2_property_create_with_args(
    const axis2_env_t *env,
    axis2_scope_t scope,
    AXIS2_FREE_VOID_ARG free_func,
    void *value)
{
    axis2_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    property_impl = (axis2_property_impl_t *) axis2_property_create(env);

    if (NULL == property_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    property_impl->value = value;
    property_impl->scope = scope;
    property_impl->free_func = free_func;

    return &(property_impl->property);
}
/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_property_free(axis2_property_t *property,
        const axis2_env_t *env)
{
    axis2_property_impl_t *property_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = AXIS2_INTF_TO_IMPL(property);

    if (property_impl->value)
    {
        if (property_impl->scope != AXIS2_SCOPE_APPLICATION)
        {
            if (property_impl->free_func)
            {
                property_impl->free_func(property_impl->value, env);
            }
            else
            {
                AXIS2_FREE(env->allocator, property_impl->value);
            }
            property_impl->value = NULL;
        }
    }

    if (property_impl->property.ops)
    {
        AXIS2_FREE(env->allocator, property_impl->property.ops);
        property_impl->property.ops = NULL;
    }

    if (property_impl)
    {
        AXIS2_FREE(env->allocator, property_impl);
        property_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_property_set_scope(axis2_property_t *property,
        const axis2_env_t *env,
        axis2_scope_t scope)
{
    axis2_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = AXIS2_INTF_TO_IMPL(property);

    property_impl->scope = scope;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_property_set_free_func(axis2_property_t *property,
        const axis2_env_t *env,
        AXIS2_FREE_VOID_ARG free_func)
{
    axis2_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = AXIS2_INTF_TO_IMPL(property);

    property_impl->free_func = free_func;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_property_set_value(axis2_property_t *property,
        const axis2_env_t *env,
        void *value)
{
    axis2_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    property_impl = AXIS2_INTF_TO_IMPL(property);

    if (property_impl->value)
    {
        if (property_impl->scope != AXIS2_SCOPE_APPLICATION)
        {
            if (property_impl->free_func)
            {
                property_impl->free_func(property_impl->value, env);
            }
            else
            {
                AXIS2_FREE(env->allocator, property_impl->value);
            }
            property_impl->value = NULL;
        }
    }

    property_impl->value = value;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_property_get_value(axis2_property_t *property,
        const axis2_env_t *env)
{
    axis2_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    property_impl = AXIS2_INTF_TO_IMPL(property);

    return property_impl->value;
}
