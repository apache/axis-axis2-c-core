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

#include <axis2_event.h>
#include <axis2.h>

typedef struct axis2_event_impl
{
    /** context base struct */
    axis2_event_t event;
    /** service the event registered to */
    axis2_svc_t *svc;
    /** event type */
    int event_type;
} axis2_event_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(event) ((axis2_event_impl_t *)event)

axis2_svc_t* AXIS2_CALL 
axis2_event_get_svc(struct axis2_event *event, 
                    axis2_env_t **env);

                    
int AXIS2_CALL 
axis2_event_get_event_type(struct axis2_event *event, 
                           axis2_env_t **env);
                           
axis2_status_t AXIS2_CALL 
axis2_event_free(struct axis2_event *event, 
                 axis2_env_t **env);
                 

axis2_event_t* AXIS2_CALL 
axis2_event_create(axis2_env_t **env, 
                   axis2_svc_t *svc, 
                   int event_type)
{
    axis2_event_impl_t *event_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    event_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_event_impl_t) );
    if (!event_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    event_impl->event.ops = NULL;
    event_impl->svc = NULL;
	event_impl->event_type = event_type;
    
    if (svc)
    {
        event_impl->svc = svc;
    }
	
    /* initialize ops */
    event_impl->event.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_event_ops_t) );
    if (!event_impl->event.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_event_free(&(event_impl->event), env);
        return NULL;        
    }

    event_impl->event.ops->get_svc = axis2_event_get_svc;
    event_impl->event.ops->get_event_type = axis2_event_get_event_type;
    event_impl->event.ops->free = axis2_event_free;

    return &(event_impl->event);
}

axis2_svc_t* AXIS2_CALL 
axis2_event_get_svc(struct axis2_event *event, 
                    axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(event)->svc;
}

int AXIS2_CALL 
axis2_event_get_event_type(struct axis2_event *event, 
                           axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(event)->event_type;
}

axis2_status_t AXIS2_CALL 
axis2_event_free (struct axis2_event *event, 
                  axis2_env_t **env)
{
    axis2_event_impl_t *event_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    event_impl = AXIS2_INTF_TO_IMPL(event);
    
    if (event_impl->event.ops)
    {
        AXIS2_FREE((*env)->allocator, event_impl->event.ops);
        event_impl->event.ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, event_impl);
    event_impl = NULL;
    
    return AXIS2_SUCCESS;
}
