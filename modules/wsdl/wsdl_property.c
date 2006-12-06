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

#include <axis2_wsdl_property.h>

/**
 * @brief Wsdl property struct impl
 *   Wsdl properties
 */
typedef struct axis2_wsdl_property_impl
{
    axis2_wsdl_property_t wsdl_property;
    axis2_char_t *name;
    void *constraint;
    void *value;

}
axis2_wsdl_property_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_property) \
      ((axis2_wsdl_property_impl_t *)wsdl_property)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_property_free(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env);

void * AXIS2_CALL
axis2_wsdl_property_get_constraint(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env) ;

axis2_status_t AXIS2_CALL
axis2_wsdl_property_set_constraint(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env,
        void *constraint);

axis2_char_t *AXIS2_CALL
axis2_wsdl_property_get_name(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_property_set_name(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env,
        axis2_char_t *name);

void *AXIS2_CALL
axis2_wsdl_property_get_value(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_property_set_value(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env,
        void *value);

/************************** End of function prototypes ************************/

axis2_wsdl_property_t * AXIS2_CALL
axis2_wsdl_property_create(const axis2_env_t *env)
{
    axis2_wsdl_property_impl_t *wsdl_property_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    wsdl_property_impl = (axis2_wsdl_property_impl_t *) AXIS2_MALLOC(env->
            allocator, sizeof(axis2_wsdl_property_impl_t));


    if (NULL == wsdl_property_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_property_impl->name = NULL;
    wsdl_property_impl->constraint = NULL;
    wsdl_property_impl->value = NULL;
    wsdl_property_impl->wsdl_property.wsdl_component = NULL;

    wsdl_property_impl->wsdl_property.wsdl_component = axis2_wsdl_component_create(env);
    if (NULL == wsdl_property_impl->wsdl_property.wsdl_component)
    {
        axis2_wsdl_property_free(&(wsdl_property_impl->wsdl_property), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_property_impl->wsdl_property.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_wsdl_property_ops_t));
    if (NULL == wsdl_property_impl->wsdl_property.ops)
    {
        axis2_wsdl_property_free(&(wsdl_property_impl->wsdl_property), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_property_impl->wsdl_property.ops->free =  axis2_wsdl_property_free;
    wsdl_property_impl->wsdl_property.ops->get_constraint =
        axis2_wsdl_property_get_constraint;
    wsdl_property_impl->wsdl_property.ops->set_constraint =
        axis2_wsdl_property_set_constraint;
    wsdl_property_impl->wsdl_property.ops->get_name =
        axis2_wsdl_property_get_name;
    wsdl_property_impl->wsdl_property.ops->set_name =
        axis2_wsdl_property_set_name;
    wsdl_property_impl->wsdl_property.ops->get_value =
        axis2_wsdl_property_get_value;
    wsdl_property_impl->wsdl_property.ops->set_value =
        axis2_wsdl_property_set_value;

    return &(wsdl_property_impl->wsdl_property);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_property_free(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env)
{
    axis2_wsdl_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    if (wsdl_property->ops)
        AXIS2_FREE(env->allocator, wsdl_property->ops);

    property_impl = AXIS2_INTF_TO_IMPL(wsdl_property);

    if (property_impl->name)
    {
        AXIS2_FREE(env->allocator, property_impl->name);
        property_impl->name = NULL;
    }

    property_impl->constraint = NULL;

    property_impl->value = NULL;

    if (wsdl_property->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_property->wsdl_component, env);
        wsdl_property->wsdl_component = NULL;
    }

    AXIS2_FREE(env->allocator, property_impl);

    return AXIS2_SUCCESS;
}

void * AXIS2_CALL
axis2_wsdl_property_get_constraint(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_property)->constraint;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_property_set_constraint(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env,
        void *constraint)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, constraint, AXIS2_FAILURE);

    AXIS2_INTF_TO_IMPL(wsdl_property)->constraint = constraint;
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_property_get_name(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_property)->name;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_property_set_name(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env,
        axis2_char_t *name)
{
    axis2_wsdl_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, name, AXIS2_FAILURE);

    property_impl = AXIS2_INTF_TO_IMPL(wsdl_property);

    if (property_impl->name)
    {
        AXIS2_FREE(env->allocator, property_impl->name);
        property_impl->name = NULL;
    }
    property_impl->name = name;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_wsdl_property_get_value(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_property)->value;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_property_set_value(axis2_wsdl_property_t *wsdl_property,
        const axis2_env_t *env,
        void *value)
{
    axis2_wsdl_property_impl_t *property_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, value, AXIS2_FAILURE);

    property_impl = AXIS2_INTF_TO_IMPL(wsdl_property);

    property_impl->value = value;

    return AXIS2_SUCCESS;
}
