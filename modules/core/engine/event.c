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

#include "axis2_event.h"
#include <axis2_const.h>

struct axis2_event
{
    /** service the event registered to */
    axis2_svc_t *svc;
    /** event type */
    int event_type;
};

AXIS2_EXTERN axis2_event_t *AXIS2_CALL
axis2_event_create(
    const axis2_env_t *env,
    axis2_svc_t *svc,
    const int event_type)
{
    axis2_event_t *event = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    event = AXIS2_MALLOC(env->allocator, sizeof(axis2_event_t));
    if (!event)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    event->svc = NULL;
    event->event_type = event_type;

    if (svc)
    {
        event->svc = svc;
    }

    return event;
}

AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
axis2_event_get_svc(
    const axis2_event_t *event,
    const axis2_env_t *env)
{
    return event->svc;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_event_get_event_type(
    const axis2_event_t *event,
    const axis2_env_t *env)
{
    return event->event_type;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_event_free(
    axis2_event_t *event,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    AXIS2_FREE(env->allocator, event);

    return AXIS2_SUCCESS;
}

