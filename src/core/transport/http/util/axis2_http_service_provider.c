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

/**
 * @file axis2_http_service_provider.c
 * @brief Revolutionary HTTP Service Provider Registry Implementation
 *
 * Following axis2_http2_msg_recv.c Java interface pattern to solve
 * circular dependency between HTTP transport and engine.
 */

#include <axis2_http_service_provider.h>
#include <axutil_thread.h>
#include <axis2_thread_mutex.h>

/* Revolutionary Service Provider Registry - thread-safe singleton */
static axis2_http_service_provider_t* g_service_provider = NULL;
static axutil_thread_mutex_t* g_provider_mutex = NULL;
static axis2_bool_t g_registry_initialized = AXIS2_FALSE;

/* Initialize registry thread-safety */
static void
axis2_http_service_provider_init_registry(const axutil_env_t* env)
{
    if (!g_registry_initialized) {
        g_provider_mutex = axutil_thread_mutex_create(env->allocator, AXIS2_THREAD_MUTEX_DEFAULT);
        g_registry_initialized = AXIS2_TRUE;
    }
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_service_provider_set_impl(
    const axutil_env_t* env,
    axis2_http_service_provider_t* provider)
{
    axis2_http_service_provider_init_registry(env);

    if (g_provider_mutex) {
        axutil_thread_mutex_lock(g_provider_mutex);
        g_service_provider = provider;
        axutil_thread_mutex_unlock(g_provider_mutex);
    } else {
        g_service_provider = provider;
    }
}

AXIS2_EXTERN axis2_http_service_provider_t* AXIS2_CALL
axis2_http_service_provider_get_impl(
    const axutil_env_t* env)
{
    axis2_http_service_provider_t* provider = NULL;

    axis2_http_service_provider_init_registry(env);

    if (g_provider_mutex) {
        axutil_thread_mutex_lock(g_provider_mutex);
        provider = g_service_provider;
        axutil_thread_mutex_unlock(g_provider_mutex);
    } else {
        provider = g_service_provider;
    }

    return provider;
}