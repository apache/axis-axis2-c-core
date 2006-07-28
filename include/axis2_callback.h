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

/** @defgroup axis2_callback callback
 * @ingroup axis2_client_api
 * @{
 */

/**
 * @file axis2_callback.h
 * @brief axis2 callback interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_async_result.h>
#include <axiom_soap_envelope.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for axis2_callback_ops */
    typedef struct axis2_callback_ops axis2_callback_ops_t;
    /** Type name for axis2_callback */
    typedef struct axis2_callback axis2_callback_t;

    typedef axis2_status_t (AXIS2_CALL *
            on_complete_func_ptr)(struct axis2_callback *,
                    const axis2_env_t *);

    typedef axis2_status_t (AXIS2_CALL *
            on_error_func_ptr)(struct axis2_callback *,
                    const axis2_env_t *, int);

    /**
     * @brief callback ops struct
     * Encapsulator struct for ops of axis2_callback
     */
    struct axis2_callback_ops
    {
        /**
         * This Method is called by Axis2 once the Async Operation is sucessfully completed and the result returns
         * @param callback pointer to callback struct
	 * @param env pointer to environment struct
         * @param result pointer to result
         */
        axis2_status_t (AXIS2_CALL *
                invoke_on_complete)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env,
                    axis2_async_result_t *result);
        /**
         * This Method is called by invoke_on_complete.
         * Users could provide this method so that they can define what to be done
         * when the callback returns on completion 
	 * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                on_complete)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env);
        /**
         * This Method is called by Axis2 once the Async Operation fails and the result returns
         * @param callback pointer to callback struct
	 * @param env pointer to environment struct
         * @param execption
         */
        axis2_status_t (AXIS2_CALL *
                report_error)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env,
                    int exception);
        /**
         * This Method is called by report_error.
         * Users could provide this method so that they can define what to be done
         * when the callback returns an error
	 * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 * @param execption
         */
        axis2_status_t (AXIS2_CALL *
                on_error)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env,
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
         * @param callback pointer to callback struct
	 * @param env pointer to environment struct
         * @return
         */
        axis2_bool_t (AXIS2_CALL *
                get_complete)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env);
        /**
         * Method setComplete
         * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 * @param complete
         */
        axis2_status_t (AXIS2_CALL *
                set_complete)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env,
                    axis2_bool_t complete);

        /**
	 * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 */
        axiom_soap_envelope_t* (AXIS2_CALL *
                get_envelope)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env);

	/**
	 * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 * @param envelope pointer to envelope
	 */
        axis2_status_t (AXIS2_CALL *
                set_envelope)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env,
                    axiom_soap_envelope_t *envelope);

        /**
	 * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 */
        int (AXIS2_CALL *
                get_error)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env);

        /**
	 * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 * @param error
	 */
        axis2_status_t (AXIS2_CALL *
                set_error)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env,
                    int error);

	/**
	 * @param callback pointer to callback struct
	 * @param data pointer to data
	 */
        axis2_status_t (AXIS2_CALL *
                set_data)(
                    struct axis2_callback *callback,
                    void *data);

	/**
	 * @param callback pointer to callback struct
	 */
        void * (AXIS2_CALL *
                get_data)(
                    struct axis2_callback *callback);

	/**
	 * @param callback pointer to clalback struct
	 * @param f
	 */
        void (AXIS2_CALL *
                set_on_complete)(
                    struct axis2_callback *callback,
                    on_complete_func_ptr f);

	/**
	 * @param callback pointer to callback struct
	 * @param f
	 */
        void (AXIS2_CALL *
                set_on_error)(
                    struct axis2_callback *callback,
                    on_error_func_ptr f);

	/**
	 * @param callback pointer to callback struct
	 * @param env pointer to environment struct
	 */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_callback *callback,
                    const axis2_env_t *env);
    };

    /**
     * callback struct
     */
    struct axis2_callback
    {
	    /** operations of callback */
        axis2_callback_ops_t *ops;
    };

    /**
     * Creates a callback struct
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_callback_t *AXIS2_CALL axis2_callback_create(const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Invoke on complete.
    @sa axis2_callback_ops#invoke_on_complete */
#define AXIS2_CALLBACK_INVOKE_ON_COMPLETE(callback, env, result) \
      ((callback)->ops->invoke_on_complete(callback, env, result))

/** On complete.
    @sa axis2_callback_ops#on_complete */
#define AXIS2_CALLBACK_ON_COMPLETE(callback, env) \
      ((callback)->ops->on_complete(callback, env))

/** Report error.
    @sa axis2_callback_ops#report_error */
#define AXIS2_CALLBACK_REPORT_ERROR(callback, env, error) \
      ((callback)->ops->report_error(callback, env, error))

/** On error.
    @sa axis2_callback_ops#on_error */
#define AXIS2_CALLBACK_ON_ERROR(callback, env, error) \
      ((callback)->ops->on_error(callback, env, error))

/** Get complete.
    @sa axis2_callback_ops#get_complete */
#define AXIS2_CALLBACK_GET_COMPLETE(callback, env)\
      ((callback)->ops->get_complete(callback, env))

/** Set complete.
    @sa axis2_callback_ops#set_complete */
#define AXIS2_CALLBACK_SET_COMPLETE(callback, env, complete) \
      ((callback)->ops->set_complete(callback, env, complete))

/** Get envelope.
    @sa axis2_callback_ops#get_envelope */
#define AXIS2_CALLBACK_GET_ENVELOPE(callback, env) \
      ((callback)->ops->get_envelope(callback, env))

/** Set envelope.
    @sa axis2_callback_ops#set_envelope */
#define AXIS2_CALLBACK_SET_ENVELOPE(callback, env, envelope) \
      ((callback)->ops->set_envelope(callback, env, envelope))

/** Get error.
    @sa axis2_callback_ops#get_error */
#define AXIS2_CALLBACK_GET_ERROR(callback, env) \
      ((callback)->ops->get_error(callback, env))

/** Set error.
    @sa axis2_callback_ops#set_error */
#define AXIS2_CALLBACK_SET_ERROR(callback, env, error) \
      ((callback)->ops->set_error(callback, env, error))

/** Get data.
    @sa axis2_callback_ops#get_data */
#define AXIS2_CALLBACK_GET_DATA(callback) \
      ((callback)->ops->get_data(callback))

/** Set data.
    @sa axis2_callback_ops#set_data */
#define AXIS2_CALLBACK_SET_DATA(callback, data) \
      ((callback)->ops->set_data(callback, data))

/** Set on complete.
    @sa axis2_callback_ops#set_on_complete */
#define AXIS2_CALLBACK_SET_ON_COMPLETE(callback, func) \
      ((callback)->ops->set_on_complete(callback, func))

/** Set on error.
    @sa axis2_callback_ops#set_on_error */
#define AXIS2_CALLBACK_SET_ON_ERROR(callback, func) \
      ((callback)->ops->set_on_error(callback, func))

/** Free.
    @sa axis2_callback_ops#free */
#define AXIS2_CALLBACK_FREE(callback, env) \
      ((callback)->ops->free (callback, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CALLBACK_H */
