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

#include <axis2_callback.h>
#include <axis2_const.h>
#include <axis2_hash.h>

typedef struct axis2_callback_impl
{
    /** context base struct */
    axis2_callback_t callback;
    /** callback complete? */
    axis2_bool_t complete;
    /** envelope corresponding to the result */
    axiom_soap_envelope_t *envelope;
    /** error code */
    int error;
    /** to store callback specific data */
    void *data;
    axis2_thread_mutex_t *mutex;
}
axis2_callback_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(callback) ((axis2_callback_impl_t *)callback)


axis2_status_t AXIS2_CALL
axis2_callback_invoke_on_complete(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    axis2_async_result_t *result);

axis2_status_t AXIS2_CALL
axis2_callback_on_complete(
    axis2_callback_t *callback,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_callback_report_error(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    int exception);

axis2_status_t AXIS2_CALL
axis2_callback_on_error(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    int exception);

axis2_bool_t AXIS2_CALL
axis2_callback_get_complete(
    const axis2_callback_t *callback,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_callback_set_complete(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    axis2_bool_t complete);

axiom_soap_envelope_t *AXIS2_CALL
axis2_callback_get_envelope(
    const axis2_callback_t *callback,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_callback_set_envelope(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    axiom_soap_envelope_t *envelope);

int AXIS2_CALL
axis2_callback_get_error(
    const axis2_callback_t *callback,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_callback_set_error(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    int error);

axis2_status_t AXIS2_CALL
axis2_callback_free(
    axis2_callback_t *callback,
    const axis2_env_t *env);

void AXIS2_CALL
axis2_callback_set_on_complete(
    axis2_callback_t *callback,
    axis2_on_complete_func_ptr f);

void AXIS2_CALL
axis2_callback_set_on_error(
    axis2_callback_t *callback,
    axis2_on_error_func_ptr f);

axis2_status_t AXIS2_CALL
axis2_callback_set_data(
    axis2_callback_t *callback,
    void *data);

void *AXIS2_CALL
axis2_callback_get_data(
    const axis2_callback_t *callback);

axis2_callback_t *AXIS2_CALL
axis2_callback_create(
    const axis2_env_t *env)
{
    axis2_callback_impl_t *callback_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    callback_impl = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_impl_t));
    if (!callback_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    callback_impl->callback.ops = NULL;
    callback_impl->complete = AXIS2_FALSE;
    callback_impl->envelope = NULL;
    callback_impl->error = AXIS2_ERROR_NONE;
    callback_impl->data = NULL;
    callback_impl->mutex = NULL;

    /* initialize ops */
    callback_impl->callback.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_ops_t));

    if (!callback_impl->callback.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_callback_free(&(callback_impl->callback), env);
        return NULL;
    }
    callback_impl->mutex = axis2_thread_mutex_create(env->allocator,
                                 AXIS2_THREAD_MUTEX_DEFAULT);
    callback_impl->callback.ops->invoke_on_complete =
        axis2_callback_invoke_on_complete;
    callback_impl->callback.ops->on_complete =
        axis2_callback_on_complete;
    callback_impl->callback.ops->report_error =
        axis2_callback_report_error;
    callback_impl->callback.ops->on_error =
        axis2_callback_on_error;
    callback_impl->callback.ops->get_complete =
        axis2_callback_get_complete;
    callback_impl->callback.ops->set_complete =
        axis2_callback_set_complete;
    callback_impl->callback.ops->get_envelope =
        axis2_callback_get_envelope;
    callback_impl->callback.ops->set_envelope =
        axis2_callback_set_envelope;
    callback_impl->callback.ops->get_error =
        axis2_callback_get_error;
    callback_impl->callback.ops->set_error =
        axis2_callback_set_error;
    callback_impl->callback.ops->set_data =
        axis2_callback_set_data;
    callback_impl->callback.ops->get_data =
        axis2_callback_get_data;
    callback_impl->callback.ops->set_on_complete =
        axis2_callback_set_on_complete;
    callback_impl->callback.ops->set_on_error =
        axis2_callback_set_on_error;
    callback_impl->callback.ops->free =
        axis2_callback_free;

    return &(callback_impl->callback);
}

axis2_status_t AXIS2_CALL
axis2_callback_invoke_on_complete(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    axis2_async_result_t *result)
{
    axis2_callback_impl_t *callback_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    callback_impl = AXIS2_INTF_TO_IMPL(callback);
    axis2_callback_set_envelope(callback, env,
            AXIS2_ASYNC_RESULT_GET_ENVELOPE(result, env));
    status = AXIS2_CALLBACK_ON_COMPLETE(callback, env);

    return status;
}

axis2_status_t AXIS2_CALL
axis2_callback_report_error(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    int exception)
{
    axis2_callback_set_error(callback, env, exception);
    return AXIS2_CALLBACK_ON_ERROR(callback, env, exception);
}

axis2_bool_t AXIS2_CALL
axis2_callback_get_complete(
    const axis2_callback_t *callback,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(callback)->complete;
}

axis2_status_t AXIS2_CALL
axis2_callback_set_complete(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    axis2_bool_t complete)
{
    axis2_callback_impl_t *callback_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    callback_impl = AXIS2_INTF_TO_IMPL(callback);
    AXIS2_INTF_TO_IMPL(callback)->complete = complete;
    return AXIS2_SUCCESS;
}

axiom_soap_envelope_t *AXIS2_CALL
axis2_callback_get_envelope(
    const axis2_callback_t *callback,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(callback)->envelope;
}

axis2_status_t AXIS2_CALL
axis2_callback_set_envelope(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    axiom_soap_envelope_t *envelope)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(callback)->envelope = envelope;
    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_callback_get_error(
    const axis2_callback_t *callback,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(callback)->error;
}
axis2_status_t AXIS2_CALL
axis2_callback_set_error(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    int error)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(callback)->error = error;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_callback_free(
    axis2_callback_t *callback,
    const axis2_env_t *env)
{
    axis2_callback_impl_t *callback_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    callback_impl = AXIS2_INTF_TO_IMPL(callback);

    if(NULL != callback_impl->mutex)
    {
        axis2_thread_mutex_destroy(callback_impl->mutex);
        callback_impl->mutex = NULL;
    }
    if (callback_impl->callback.ops)
    {
        AXIS2_FREE(env->allocator, callback_impl->callback.ops);
        callback_impl->callback.ops = NULL;
    }

    AXIS2_FREE(env->allocator, callback_impl);
    callback_impl = NULL;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_callback_set_data(
    axis2_callback_t *callback,
    void *data)
{
    axis2_callback_impl_t *callback_impl = NULL;

    callback_impl = AXIS2_INTF_TO_IMPL(callback);

    callback_impl->data = (void*)data;

    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_callback_get_data(
    const axis2_callback_t *callback)
{
    axis2_callback_impl_t *callback_impl = NULL;

    callback_impl = AXIS2_INTF_TO_IMPL(callback);

    return callback_impl->data;
}

void AXIS2_CALL
axis2_callback_set_on_complete(
    axis2_callback_t *callback,
    axis2_on_complete_func_ptr func)
{
    callback->ops->on_complete = func;
}

void AXIS2_CALL
axis2_callback_set_on_error(
    axis2_callback_t *callback,
    axis2_on_error_func_ptr func)
{
    callback->ops->on_error = func;
}

axis2_status_t AXIS2_CALL
axis2_callback_on_complete(
    axis2_callback_t *callback,
    const axis2_env_t *env)
{
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_callback_on_error(
    axis2_callback_t *callback,
    const axis2_env_t *env,
    int exception)
{
    return AXIS2_SUCCESS;
}
