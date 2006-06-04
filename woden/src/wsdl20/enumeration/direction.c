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

#include <woden/wsdl20/enumeration/axis2_woden_direction.h>

typedef struct axis2_woden_direction_impl axis2_woden_direction_impl_t;

/** 
 * @brief Direction Struct Impl
 *   Axis2 Direction  
 */ 
struct axis2_woden_direction_impl
{
    axis2_woden_direction_t direction;
    axis2_char_t *f_value;
};

#define INTF_TO_IMPL(direction) ((axis2_woden_direction_impl_t *) direction)

static axis2_woden_direction_t *WODEN_DIRECTION_IN = NULL;
static axis2_woden_direction_t *WODEN_DIRECTION_OUT = NULL;

axis2_status_t AXIS2_CALL 
axis2_woden_direction_free(
        void *direction,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_direction_get_direction_in(
        void *direction,
        const axis2_env_t *env);

void *AXIS2_CALL
axis2_woden_direction_get_direction_out(
        void *direction,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_woden_direction_to_string(
        void *direction,
        const axis2_env_t *env);

static axis2_woden_direction_t *
create(
        const axis2_env_t *env)
{
    axis2_woden_direction_impl_t *direction_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    direction_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_direction_impl_t));

    direction_impl->f_value = NULL;
   
    direction_impl->direction.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_direction_ops_t));
    
    direction_impl->direction.ops->free = axis2_woden_direction_free;

    direction_impl->direction.ops->get_direction_in = 
        axis2_woden_direction_get_direction_in;
    direction_impl->direction.ops->get_direction_out = 
        axis2_woden_direction_get_direction_out;
    direction_impl->direction.ops->to_string = axis2_woden_direction_to_string;
    

    return &(direction_impl->direction);
}

AXIS2_EXTERN axis2_woden_direction_t * AXIS2_CALL
axis2_woden_direction_create(
        const axis2_env_t *env,
        axis2_char_t *value)
{
    axis2_woden_direction_impl_t *direction_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    direction_impl = (axis2_woden_direction_impl_t *) create(env);

    direction_impl->f_value = AXIS2_STRDUP(value, env);

    return &(direction_impl->direction);
}

axis2_status_t AXIS2_CALL
axis2_woden_direction_free(
        void *direction,
        const axis2_env_t *env)
{
    axis2_woden_direction_impl_t *direction_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    direction_impl = INTF_TO_IMPL(direction);

    if(direction_impl->f_value)
    {
        AXIS2_FREE(env->allocator, direction_impl->f_value);
        direction_impl->f_value = NULL;
    }

    if((&(direction_impl->direction))->ops)
    {
        AXIS2_FREE(env->allocator, (&(direction_impl->direction))->ops);
        (&(direction_impl->direction))->ops = NULL;
    }

    if(direction_impl)
    {
        AXIS2_FREE(env->allocator, direction_impl);
        direction_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_woden_direction_get_direction_in(
        void *direction,
        const axis2_env_t *env)
{
    axis2_woden_direction_impl_t *direction_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    direction_impl = INTF_TO_IMPL(direction);
    
    if(!WODEN_DIRECTION_IN)
        WODEN_DIRECTION_IN = axis2_woden_direction_create(env, "in");

    return WODEN_DIRECTION_IN;
}

void *AXIS2_CALL
axis2_woden_direction_get_direction_out(
        void *direction,
        const axis2_env_t *env)
{
    axis2_woden_direction_impl_t *direction_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    direction_impl = INTF_TO_IMPL(direction);

    if(!WODEN_DIRECTION_OUT)
        WODEN_DIRECTION_OUT = axis2_woden_direction_create(env, "out");
    return WODEN_DIRECTION_OUT;
}

axis2_char_t *AXIS2_CALL
axis2_woden_direction_to_string(
        void *direction,
        const axis2_env_t *env)
{
    axis2_woden_direction_impl_t *direction_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    direction_impl = INTF_TO_IMPL(direction);

    return direction_impl->f_value;
}

