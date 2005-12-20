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

#ifndef AXIS2_CALLBACK_H
#define AXIS2_CALLBACK_H


/**
  * @file axis2_callback.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_async_result.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_callback Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_callback_ops axis2_callback_ops_t;
typedef struct axis2_callback axis2_callback_t; 

    
/** 
 * @brief Message Context ops struct
 * Encapsulator struct for ops of axis2_callback
 */  
struct axis2_callback_ops
{
    /**
     * This Method is called by Axis2 once the Async Operation is sucessfully completed and the result returns
     *
     * @param result
     */
    axis2_status_t (AXIS2_CALL *invoke_on_complete)(struct axis2_callback *callback, 
        axis2_env_t **env, 
        axis2_async_result_t *result);
    /**
     * This Method is called by Axis2 once the Async Operation fails and the result returns
     *
     * @param e
     */
    axis2_status_t (AXIS2_CALL *report_error)(struct axis2_callback *callback, 
        axis2_env_t **env, 
        int exception);
    /**
     * This says has the Async Operation is completed or not. this could be useful for poleing 
     * with a special callback written for poleing (checking repeatedly time to time).
     * e.g.
     * <code>
     *      <pre>
     *          while(!callback.isComplete()){
     *             Thread.sleep(1000);
     *          }
     *          do whatever u need to do
     *      </pre>
     * </code>
     *
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_complete)(struct axis2_callback *callback, 
        axis2_env_t **env);
    /**
     * Method setComplete
     *
     * @param complete
     */
    axis2_status_t (AXIS2_CALL *set_complete)(struct axis2_callback *callback, 
        axis2_env_t **env, 
        axis2_bool_t complete);
    /* TODO axis2_soap_envelope_t* (AXIS2_CALL *get_envelope)(struct axis2_callback *callback, 
        axis2_env_t **env);
    axis2_status_t (AXIS2_CALL *set_envelope)(struct axis2_callback *callback, 
        axis2_env_t **env, axis2_soap_envelope_t *envelope);*/
    int (AXIS2_CALL *get_error)(struct axis2_callback *callback, 
        axis2_env_t **env);
    axis2_status_t (AXIS2_CALL *set_error)(struct axis2_callback *callback, 
        axis2_env_t **env, 
        int error);
    axis2_status_t (AXIS2_CALL *free)(struct axis2_callback *callback, 
                                       axis2_env_t **env);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_callback
{
    axis2_callback_ops_t *ops;    
};

AXIS2_DECLARE(axis2_callback_t*) axis2_callback_create(axis2_env_t **env);
    
/************************** Start of function macros **************************/

#define AXIS2_CALLBACK_INVOKE_ON_COMPLETE(callback, env, result) ((callback)->ops->invoke_on_complete(callback, env, result))
#define AXIS2_CALLBACK_REPORT_ERROR(callback, env, error) ((callback)->ops->report_error(callback, env, error))
#define AXIS2_CALLBACK_GET_COMPLETE(callback, env) ((callback)->ops->get_complete(callback, env))
#define AXIS2_CALLBACK_SET_COMPLETE(callback, env, complete) ((callback)->ops->set_complete(callback, env, complete))
#define AXIS2_CALLBACK_GET_ENVELOPE(callback, env) ((callback)->ops->get_envelope(callback, env))
#define AXIS2_CALLBACK_SET_ENVELOPE(callback, env, envelope) ((callback)->ops->set_envelope(callback, env, envelope))
#define AXIS2_CALLBACK_GET_ERROR(callback, env) ((callback)->ops->get_error(callback, env))
#define AXIS2_CALLBACK_SET_ERROR(callback, env, error) ((callback)->ops->set_error(callback, env, error))
#define AXIS2_CALLBACK_FREE(callback, env) ((callback)->ops->free (callback, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CALLBACK_H */
