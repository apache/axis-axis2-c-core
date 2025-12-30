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
#include <pthread.h>

/* Revolutionary Service Provider Registry - thread-safe singleton */
static axis2_http_service_provider_t* g_service_provider = NULL;
static pthread_mutex_t g_provider_mutex = PTHREAD_MUTEX_INITIALIZER;

/* No initialization needed - using static PTHREAD_MUTEX_INITIALIZER */

AXIS2_EXTERN void AXIS2_CALL
axis2_http_service_provider_set_impl(
    const axutil_env_t* env,
    axis2_http_service_provider_t* provider)
{
    /* Thread-safe using statically initialized pthread mutex */
    pthread_mutex_lock(&g_provider_mutex);
    g_service_provider = provider;
    pthread_mutex_unlock(&g_provider_mutex);
}

AXIS2_EXTERN axis2_http_service_provider_t* AXIS2_CALL
axis2_http_service_provider_get_impl(
    const axutil_env_t* env)
{
    axis2_http_service_provider_t* provider = NULL;

    /* Thread-safe using statically initialized pthread mutex */
    pthread_mutex_lock(&g_provider_mutex);
    provider = g_service_provider;
    pthread_mutex_unlock(&g_provider_mutex);

    return provider;
}