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

#ifndef AXIS2_EVENT_H
#define AXIS2_EVENT_H

/** @defgroup axis2_event event
 * @ingroup axis2_core_engine
 * @{
 * An Axis event is sent to registered listeners whenever anything significant 
 * happens to Axis configuration.
 */

/**
 * @file axis2_event.h
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_svc.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define AXIS2_SERVICE_DEPLOY 1
#define AXIS2_SERVICE_REMOVE 0

    /** Type name for struct axis2_event */
    typedef struct axis2_event axis2_event_t;

    /**
     * Gets service associated with the event.        
     * @param even pointer to event
     * @param env pointer to environment struct 
     * @return pointer to service
     */
    AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
    axis2_event_get_svc(const axis2_event_t *event,
        const axis2_env_t *env);

    /** 
     * Gets event type.
     * @param even pointer to event
     * @param env pointer to environment struct 
     * @return event type as an integer
     */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_event_get_event_type(const axis2_event_t *event,
        const axis2_env_t *env);

    /** 
     * Frees the event struct.
     * @param even pointer to event
     * @param env pointer to environment struct 
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_event_free(axis2_event_t *event,
        const axis2_env_t *env);

    /**
     * Created an event struct instance.
     * @param env pointer to environment struct
     * @param svc pointer to service, event does not assume the ownership of service
     * @param event type event type
     * @return pointer to newly created event struct
     */
    AXIS2_EXTERN axis2_event_t *AXIS2_CALL
    axis2_event_create(
        const axis2_env_t *env,
        axis2_svc_t *svc,
        const int event_type);

/** Gets service associated with the event. */
#define AXIS2_EVENT_GET_SVC(event, env) \
        axis2_event_get_svc(event, env)

/** Gets event type as an integer. */
#define AXIS2_EVENT_GET_EVENT_TYPE(event, env) \
        axis2_event_get_event_type(event, env)

/** Frees event struct. */
#define AXIS2_EVENT_FREE(event, env) \
        axis2_event_free (event, env)

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_EVENT_H */
