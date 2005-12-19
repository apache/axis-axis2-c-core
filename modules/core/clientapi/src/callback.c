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

#include <axis2_callback.h>
#include <axis2.h>
#include <axis2_hash.h>

typedef struct axis2_callback_impl
{
    /** context base struct */
    axis2_callback_t callback;
    /** callback complete? */
    axis2_bool_t complete;
} axis2_callback_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(callback) ((axis2_callback_impl_t *)callback)

axis2_status_t AXIS2_CALL axis2_callback_invoke_on_complete(struct axis2_callback *callback, axis2_env_t **env, axis2_async_result_t *result);
axis2_status_t AXIS2_CALL axis2_callback_report_error(struct axis2_callback *callback, axis2_env_t **env, int exception);
axis2_bool_t AXIS2_CALL axis2_callback_get_complete(struct axis2_callback *callback, axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_callback_set_complete(struct axis2_callback *callback, axis2_env_t **env, axis2_bool_t complete) ;
axis2_status_t AXIS2_CALL axis2_callback_free (struct axis2_callback *callback, 
                                   axis2_env_t **env);

axis2_callback_t* AXIS2_CALL axis2_callback_create(axis2_env_t **env)
{
    axis2_callback_impl_t *callback_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    callback_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_callback_impl_t) );
    if (!callback_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    callback_impl->callback.ops = NULL;
    callback_impl->complete = AXIS2_FALSE;
    
    /* initialize ops */    
    callback_impl->callback.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_callback_ops_t) );
    if (!callback_impl->callback.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_callback_free(&(callback_impl->callback), env);
        return NULL;        
    }

    callback_impl->callback.ops->invoke_on_complete = axis2_callback_invoke_on_complete;
    callback_impl->callback.ops->report_error = axis2_callback_report_error;
    callback_impl->callback.ops->get_complete = axis2_callback_get_complete;
    callback_impl->callback.ops->set_complete = axis2_callback_set_complete;    
    callback_impl->callback.ops->free = axis2_callback_free;

    return &(callback_impl->callback);
}

/**
 * This Method is called by Axis2 once the Async Operation is sucessfully completed and the result returns
 *
 * @param result
 */
axis2_status_t AXIS2_CALL axis2_callback_invoke_on_complete(struct axis2_callback *callback, axis2_env_t **env, axis2_async_result_t *result)
{
    return AXIS2_SUCCESS;
}

/**
 * This Method is called by Axis2 once the Async Operation fails and the result returns
 *
 * @param e
 */
axis2_status_t AXIS2_CALL axis2_callback_report_error(struct axis2_callback *callback, axis2_env_t **env, int exception)
{
    return AXIS2_SUCCESS;
}

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
axis2_bool_t AXIS2_CALL axis2_callback_get_complete(struct axis2_callback *callback, axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(callback, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(callback)->complete;
}

/**
 * Method setComplete
 *
 * @param complete
 */
axis2_status_t AXIS2_CALL axis2_callback_set_complete(struct axis2_callback *callback, axis2_env_t **env, axis2_bool_t complete) 
{
    AXIS2_FUNC_PARAM_CHECK(callback, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(callback)->complete = complete;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_callback_free (struct axis2_callback *callback, 
                                   axis2_env_t **env)
{
    axis2_callback_impl_t *callback_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(callback, env, AXIS2_FAILURE);
    
    callback_impl = AXIS2_INTF_TO_IMPL(callback);
    
    if (callback_impl->callback.ops)
    {
        AXIS2_FREE((*env)->allocator, callback_impl->callback.ops);
        callback_impl->callback.ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, callback_impl);
    callback_impl = NULL;
    
    return AXIS2_SUCCESS;
}
