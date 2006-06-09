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

#include <woden/wsdl20/enumeration/axis2_woden_msg_label.h>

typedef struct axis2_woden_msg_label_impl axis2_woden_msg_label_impl_t;

/** 
 * @brief Message Label Struct Impl
 *   Axis2 Message Label  
 */ 
struct axis2_woden_msg_label_impl
{
    axis2_woden_msg_label_t msg_label;
    axis2_char_t *f_value;
    axis2_bool_t f_valid;
};

#define INTF_TO_IMPL(msg_label) ((axis2_woden_msg_label_impl_t *) msg_label)

static axis2_woden_msg_label_t *WODEN_MSG_LABEL_IN = NULL;
static axis2_woden_msg_label_t *WODEN_MSG_LABEL_OUT = NULL;

axis2_status_t AXIS2_CALL 
axis2_woden_msg_label_free(
        void *msg_label,
        const axis2_env_t *env);

axis2_char_t *AXIS2_CALL
axis2_woden_msg_label_to_string(
        void *msg_label,
        const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_woden_msg_label_is_valid(
        void *msg_label,
        const axis2_env_t *env);

axis2_bool_t AXIS2_CALL
axis2_woden_msg_label_equals(
        void *msg_label,
        const axis2_env_t *env,
        axis2_woden_msg_label_t *other);

static axis2_woden_msg_label_t *
create(
        const axis2_env_t *env)
{
    axis2_woden_msg_label_impl_t *msg_label_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_label_impl = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_msg_label_impl_t));

    msg_label_impl->f_value = NULL;
    msg_label_impl->f_valid = AXIS2_TRUE;
   
    msg_label_impl->msg_label.ops = AXIS2_MALLOC(env->allocator, 
                    sizeof(axis2_woden_msg_label_ops_t));
    
    msg_label_impl->msg_label.ops->free = axis2_woden_msg_label_free;

    msg_label_impl->msg_label.ops->to_string = axis2_woden_msg_label_to_string;
    msg_label_impl->msg_label.ops->is_valid = axis2_woden_msg_label_is_valid;
    msg_label_impl->msg_label.ops->equals = axis2_woden_msg_label_equals;
    

    return &(msg_label_impl->msg_label);
}

AXIS2_EXTERN axis2_woden_msg_label_t * AXIS2_CALL
axis2_woden_msg_label_create(
        const axis2_env_t *env,
        axis2_char_t *value,
        axis2_bool_t valid)
{
    axis2_woden_msg_label_impl_t *msg_label_impl = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_label_impl = (axis2_woden_msg_label_impl_t *) create(env);

    msg_label_impl->f_value = AXIS2_STRDUP(value, env);
    msg_label_impl->f_valid = valid;

    return &(msg_label_impl->msg_label);
}

AXIS2_EXTERN axis2_woden_msg_label_t *AXIS2_CALL
axis2_woden_msg_label_get_msg_label_in(
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!WODEN_MSG_LABEL_IN)
        WODEN_MSG_LABEL_IN = axis2_woden_msg_label_create(env, "In", 
                AXIS2_TRUE);

    return WODEN_MSG_LABEL_IN;
}

AXIS2_EXTERN axis2_woden_msg_label_t *AXIS2_CALL
axis2_woden_msg_label_get_msg_label_out(
        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if(!WODEN_MSG_LABEL_OUT)
        WODEN_MSG_LABEL_OUT = axis2_woden_msg_label_create(env, "Out", 
                AXIS2_TRUE);

    return WODEN_MSG_LABEL_OUT;
}

AXIS2_EXTERN axis2_woden_msg_label_t *AXIS2_CALL
axis2_woden_msg_label_get_invalid_value(
        const axis2_env_t *env,
        const axis2_char_t *value)
{
    axis2_woden_msg_label_t *msg_label = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    msg_label = axis2_woden_msg_label_create(env, value, AXIS2_FALSE);

    return msg_label;
}

axis2_status_t AXIS2_CALL
axis2_woden_msg_label_free(
        void *msg_label,
        const axis2_env_t *env)
{
    axis2_woden_msg_label_impl_t *msg_label_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_label_impl = INTF_TO_IMPL(msg_label);

    if(msg_label_impl->f_value)
    {
        AXIS2_FREE(env->allocator, msg_label_impl->f_value);
        msg_label_impl->f_value = NULL;
    }

    if((&(msg_label_impl->msg_label))->ops)
    {
        AXIS2_FREE(env->allocator, (&(msg_label_impl->msg_label))->ops);
        (&(msg_label_impl->msg_label))->ops = NULL;
    }

    if(msg_label_impl)
    {
        AXIS2_FREE(env->allocator, msg_label_impl);
        msg_label_impl = NULL;
    }
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
axis2_woden_msg_label_to_string(
        void *msg_label,
        const axis2_env_t *env)
{
    axis2_woden_msg_label_impl_t *msg_label_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_label_impl = INTF_TO_IMPL(msg_label);

    return msg_label_impl->f_value;
}

axis2_bool_t AXIS2_CALL
axis2_woden_msg_label_is_valid(
        void *msg_label,
        const axis2_env_t *env)
{
    axis2_woden_msg_label_impl_t *msg_label_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    msg_label_impl = INTF_TO_IMPL(msg_label);

    return msg_label_impl->f_valid;
}

axis2_bool_t AXIS2_CALL
axis2_woden_msg_label_equals(
        void *msg_label,
        const axis2_env_t *env,
        axis2_woden_msg_label_t *other)
{
    axis2_woden_msg_label_impl_t *msg_label_impl = NULL;
    axis2_char_t *value = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, other, AXIS2_FAILURE);
    msg_label_impl = INTF_TO_IMPL(msg_label);

    value = AXIS2_WODEN_MSG_LABEL_TO_STRING(other, env);
    if(0 == AXIS2_STRCMP(msg_label_impl->f_value, value))
    {
        return AXIS2_TRUE;
    }

    return AXIS2_FALSE;
}

