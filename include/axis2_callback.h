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
 * @defgroup axis2_callback callback
 * @ingroup axis2_client_api
 * callback represents the callback mechanisms to be used in case of asynchronous
 * invocations. It holds the complete status of the invocation, the resulting 
 * SOAP envelope and also callback specific data. One can define a function
 * to be called on complete of the callback as well as a function to be called 
 * on error. 
 * @{
 */

/**
 * @file axis2_callback.h
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
    /** Type name for function pointer to be called on complete of callback  */
    typedef axis2_status_t (AXIS2_CALL *
            axis2_on_complete_func_ptr)(axis2_callback_t *,
                    const axis2_env_t *);
    /** Type name for function pointer to be called on error of callback  */
    typedef axis2_status_t (AXIS2_CALL *
            axis2_on_error_func_ptr)(axis2_callback_t *,
                    const axis2_env_t *, int);

    /**
     * callback ops struct.
     * Encapsulator struct for ops of axis2_callback.
     */
    struct axis2_callback_ops
    {
        /**
         * This function is called once the asynchronous operation is successfully 
         * completed and the result is available.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @param result pointer to async result
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                invoke_on_complete)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env,
                    axis2_async_result_t *result);

        /**
         * This function is called by invoke_on_complete.
         * Users could provide this method so that they can define what to be 
         * done when the callback returns on completion.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                on_complete)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env);

        /**
         * This function is called once the asynchronous operation fails and 
         * the failure code returns. 
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @param exception error code representing the error
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                report_error)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env,
                    const int exception);

        /**
         * This function is called by report_error.
         * Users could provide this method so that they can define what to be done
         * when the callback returns an error.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @param exception error code representing the error
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                on_error)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env,
                    const int exception);

        /**
         * Gets the complete status for the callback. This method is useful 
         * for polling (busy waiting).
         * e.g.
         * <code>
         *      <pre>
         *          while(!AXIS2_CALLBACK_GET_COMPLETE(callback, env))
         *          {
         *             sleep(10);
         *          }
         *          // do whatever you need here 
         *      </pre>
         * </code>
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if callback is complete, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                get_complete)(
                    const axis2_callback_t *callback,
                    const axis2_env_t *env);

        /**
         * Sets the complete status.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @param complete bool value representing the status
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_complete)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env,
                    const axis2_bool_t complete);

        /**
         * Gets the resulting SOAP envelope.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @return result SOAP envelope if present, else NULL
         */
        axiom_soap_envelope_t *(AXIS2_CALL *
                get_envelope)(
                    const axis2_callback_t *callback,
                    const axis2_env_t *env);

        /**
         * Sets the SOAP envelope.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @param envelope pointer to SOAP envelope
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_envelope)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env,
                    axiom_soap_envelope_t *envelope);

        /**
         * Gets error code representing the error.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @return error code representing the error
         */
        int (AXIS2_CALL *
                get_error)(
                    const axis2_callback_t *callback,
                    const axis2_env_t *env);

        /**
         * Sets the error code.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @param error error code representing the error
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_error)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env,
                    const int error);

        /**
         * Sets the callback data.
         * @param callback pointer to callback struct
         * @param data pointer to data
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_data)(
                    axis2_callback_t *callback,
                    void *data);

        /**
         * Gets the callback data.
         * @param callback pointer to callback struct
         * @return pointer to callback data
         */
        void *(AXIS2_CALL *
                get_data)(
                    const axis2_callback_t *callback);

        /**
         * Sets the on complete callback function.
         * @param callback pointer to callback struct
         * @param f on complete callback function pointer
         */
        void (AXIS2_CALL *
                set_on_complete)(
                    axis2_callback_t *callback,
                    axis2_on_complete_func_ptr f);

        /**
         * Sets the on error callback function.
         * @param callback pointer to callback struct
         * @param f on error callback function pointer
         */
        void (AXIS2_CALL *
                set_on_error)(
                    axis2_callback_t *callback,
                    axis2_on_error_func_ptr f);

        /**
         * Frees callback struct.
         * @param callback pointer to callback struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_callback_t *callback,
                    const axis2_env_t *env);
    };

    /**
     * callback struct.
     */
    struct axis2_callback
    {
        /** operations of callback. */
        axis2_callback_ops_t *ops;
    };

    /**
     * Creates a callback struct.
     * @param env pointer to environment struct
     * @return pointer to newly created callback struct
     */
    AXIS2_EXTERN axis2_callback_t *AXIS2_CALL 
    axis2_callback_create(
        const axis2_env_t *env);


/** Invokes on complete functions.
    @sa axis2_callback_ops#invoke_on_complete */
#define AXIS2_CALLBACK_INVOKE_ON_COMPLETE(callback, env, result) \
      ((callback)->ops->invoke_on_complete(callback, env, result))

/** Function to be called on complete of callback.
    @sa axis2_callback_ops#on_complete */
#define AXIS2_CALLBACK_ON_COMPLETE(callback, env) \
      ((callback)->ops->on_complete(callback, env))

/** Reports error.
    @sa axis2_callback_ops#report_error */
#define AXIS2_CALLBACK_REPORT_ERROR(callback, env, error) \
      ((callback)->ops->report_error(callback, env, error))

/** Function to be called on error of callback.
    @sa axis2_callback_ops#on_error */
#define AXIS2_CALLBACK_ON_ERROR(callback, env, error) \
      ((callback)->ops->on_error(callback, env, error))

/** Gets complete status.
    @sa axis2_callback_ops#get_complete */
#define AXIS2_CALLBACK_GET_COMPLETE(callback, env)\
      ((callback)->ops->get_complete(callback, env))

/** Sets complete status.
    @sa axis2_callback_ops#set_complete */
#define AXIS2_CALLBACK_SET_COMPLETE(callback, env, complete) \
      ((callback)->ops->set_complete(callback, env, complete))

/** Gets result SOAP envelope.
    @sa axis2_callback_ops#get_envelope */
#define AXIS2_CALLBACK_GET_ENVELOPE(callback, env) \
      ((callback)->ops->get_envelope(callback, env))

/** Sets SOAP envelope.
    @sa axis2_callback_ops#set_envelope */
#define AXIS2_CALLBACK_SET_ENVELOPE(callback, env, envelope) \
      ((callback)->ops->set_envelope(callback, env, envelope))

/** Gets error code.
    @sa axis2_callback_ops#get_error */
#define AXIS2_CALLBACK_GET_ERROR(callback, env) \
      ((callback)->ops->get_error(callback, env))

/** Sets error code.
    @sa axis2_callback_ops#set_error */
#define AXIS2_CALLBACK_SET_ERROR(callback, env, error) \
      ((callback)->ops->set_error(callback, env, error))

/** Gets data.
    @sa axis2_callback_ops#get_data */
#define AXIS2_CALLBACK_GET_DATA(callback) \
      ((callback)->ops->get_data(callback))

/** Sets data.
    @sa axis2_callback_ops#set_data */
#define AXIS2_CALLBACK_SET_DATA(callback, data) \
      ((callback)->ops->set_data(callback, data))

/** Sets on complete callback function.
    @sa axis2_callback_ops#set_on_complete */
#define AXIS2_CALLBACK_SET_ON_COMPLETE(callback, func) \
      ((callback)->ops->set_on_complete(callback, func))

/** Set on error callback function.
    @sa axis2_callback_ops#set_on_error */
#define AXIS2_CALLBACK_SET_ON_ERROR(callback, func) \
      ((callback)->ops->set_on_error(callback, func))

/** Frees callback struct.
    @sa axis2_callback_ops#free */
#define AXIS2_CALLBACK_FREE(callback, env) \
      ((callback)->ops->free (callback, env))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CALLBACK_H */
