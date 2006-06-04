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

#ifndef AXIS2_EVENT_H
#define AXIS2_EVENT_H


/**
  * @file axis2_event.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_svc.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_event Event
 * @ingroup axis2_core_engine
 * @{
 */
    
#define AXIS2_SERVICE_DEPLOY 1
#define AXIS2_SERVICE_REMOVE 0
    
typedef struct axis2_event_ops axis2_event_ops_t;
typedef struct axis2_event axis2_event_t; 

    
/** 
 * @brief An Axis event is sent to registered listeners whenever anything significant 
 * happens to <code>AxisConfiguration</code>.
 */
struct axis2_event_ops
{

    axis2_svc_t* (AXIS2_CALL *
    get_svc)(struct axis2_event *event, 
            const axis2_env_t *env);

            
    int (AXIS2_CALL *
    get_event_type)(struct axis2_event *event, 
                    const axis2_env_t *env);

                    
    axis2_status_t (AXIS2_CALL *
    free)(struct axis2_event *event,
          const axis2_env_t *env);
};

/** 
 * @brief Message Context struct
  *   Axis2 Message Context
 */
struct axis2_event
{
    axis2_event_ops_t *ops;    
};

AXIS2_EXTERN axis2_event_t* AXIS2_CALL 
axis2_event_create(const axis2_env_t *env, 
                   axis2_svc_t *svc, 
                   int event_type);
    
/************************** Start of function macros **************************/

#define AXIS2_EVENT_GET_SVC(event, env) \
        ((event)->ops->get_svc(event, env))
        
#define AXIS2_EVENT_GET_EVENT_TYPE(event, env) \
        ((event)->ops->get_event_type(event, env))
        
#define AXIS2_EVENT_FREE(event, env) \
        ((event)->ops->free (event, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_EVENT_H */
