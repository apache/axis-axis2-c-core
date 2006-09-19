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

#include <axis2_wsdl_include.h>

/**
 * @brief Wsdl Include struct impl
 *   Wsdl Includes
 */
typedef struct axis2_wsdl_include_impl
{
    axis2_wsdl_include_t wsdl_include;
    axis2_char_t *location;

}
axis2_wsdl_include_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_include) \
      ((axis2_wsdl_include_impl_t *)wsdl_include)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_include_free(axis2_wsdl_include_t *wsdl_include,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_wsdl_include_get_location(axis2_wsdl_include_t *wsdl_include,
        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_include_set_location(axis2_wsdl_include_t *wsdl_include,
        const axis2_env_t *env,
        axis2_char_t *location);

/************************** End of function prototypes ************************/

axis2_wsdl_include_t * AXIS2_CALL
axis2_wsdl_include_create(const axis2_env_t *env)
{
    axis2_wsdl_include_impl_t *wsdl_include_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    wsdl_include_impl = (axis2_wsdl_include_impl_t *) AXIS2_MALLOC(env->allocator,
            sizeof(axis2_wsdl_include_impl_t));


    if (NULL == wsdl_include_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_include_impl->location = NULL;
    wsdl_include_impl->wsdl_include.ops = NULL;
    wsdl_include_impl->wsdl_include.wsdl_component = NULL;

    wsdl_include_impl->wsdl_include.wsdl_component = axis2_wsdl_component_create(env);
    if (NULL == wsdl_include_impl->wsdl_include.wsdl_component)
    {
        axis2_wsdl_include_free(&(wsdl_include_impl->wsdl_include), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_include_impl->wsdl_include.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_wsdl_include_ops_t));
    if (NULL == wsdl_include_impl->wsdl_include.ops)
    {
        axis2_wsdl_include_free(&(wsdl_include_impl->wsdl_include), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    wsdl_include_impl->wsdl_include.ops->free =  axis2_wsdl_include_free;
    wsdl_include_impl->wsdl_include.ops->get_location =
        axis2_wsdl_include_get_location;
    wsdl_include_impl->wsdl_include.ops->set_location =
        axis2_wsdl_include_set_location;

    return &(wsdl_include_impl->wsdl_include);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_include_free(axis2_wsdl_include_t *wsdl_include,
        const axis2_env_t *env)
{
    axis2_wsdl_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    include_impl = AXIS2_INTF_TO_IMPL(wsdl_include);

    if (wsdl_include->ops)
        AXIS2_FREE(env->allocator, wsdl_include->ops);

    if (include_impl->location)
    {
        AXIS2_FREE(env->allocator, include_impl->location);
        include_impl->location = NULL;
    }

    if (wsdl_include->wsdl_component)
    {
        AXIS2_WSDL_COMPONENT_FREE(wsdl_include->wsdl_component, env);
        wsdl_include->wsdl_component = NULL;
    }

    AXIS2_FREE(env->allocator, include_impl);
    include_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_wsdl_include_get_location(axis2_wsdl_include_t *wsdl_include,
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_include)->location;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_include_set_location(axis2_wsdl_include_t *wsdl_include,
        const axis2_env_t *env,
        axis2_char_t *location)
{
    axis2_wsdl_include_impl_t *include_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, location, AXIS2_FAILURE);

    include_impl = AXIS2_INTF_TO_IMPL(wsdl_include);

    if (include_impl->location)
    {
        AXIS2_FREE(env->allocator, include_impl->location);
        include_impl->location = NULL;
    }
    include_impl->location = AXIS2_STRDUP(location, env);
    if (!include_impl->location)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
