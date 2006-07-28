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

#ifndef AXIS2_ASYNC_RESULT_H
#define AXIS2_ASYNC_RESULT_H

/** @defgroup axis2_async_result async result
 * @ingroup axis2_client_api
 * @{
 */

/**
 * @file axis2_async_result.h
 * @brief axis2 async result interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_msg_ctx.h>
#include <axiom_soap_envelope.h>

#ifdef __cplusplus
extern "C"
{
#endif


    /** Type name for struct axis2_async_result_ops */
    typedef struct axis2_async_result_ops axis2_async_result_ops_t;
    /** Type name for struct axis2_async_result */
    typedef struct axis2_async_result axis2_async_result_t;


    /**
     * @brief async result ops struct
     * Encapsulator struct for ops of axis2_async_result
     */
    struct axis2_async_result_ops
    {
	/**
	 * @param async_result pointer to async result struct
	 * @param env pointer to environment struct
	 */
        axiom_soap_envelope_t* (AXIS2_CALL *
                get_envelope)(
		    struct axis2_async_result *async_result,
                    const axis2_env_t *env);


	/**
	 * @param async_result pointer to async result struct
	 * @param env pointer to environment struct
	 */
        axis2_msg_ctx_t* (AXIS2_CALL *
                get_result)(
	            struct axis2_async_result *async_result,
                    const axis2_env_t *env);

	/**
	 * @param async_result pointer to async result struct
	 * @param env pointer to environment struct
	 */
        axis2_status_t (AXIS2_CALL *
                free)(
		    struct axis2_async_result *async_result,
                    const axis2_env_t *env);
    };

    /**
     *  async result struct
     */
    struct axis2_async_result
    {
	/** operations of async result */
        axis2_async_result_ops_t *ops;
    };

    /** Creates a async result struct 
     * @param env pointer to environment struct
     * @param result pointer to result
     */
    AXIS2_EXTERN axis2_async_result_t* AXIS2_CALL
    axis2_async_result_create(const axis2_env_t *env,
            axis2_msg_ctx_t *result);

/************************** Start of function macros **************************/

/** Gets the envelope.
    @sa axis2_async_result_ops#get_envelope*/
#define AXIS2_ASYNC_RESULT_GET_ENVELOPE(async_result, env) \
      ((async_result)->ops->get_envelope(async_result, env))

/** Gets the result.
    @sa axis2_async_result_ops#get_result*/
#define AXIS2_ASYNC_RESULT_GET_RESULT(async_result, env) \
      ((async_result)->ops->get_result(async_result, env))

/** Free.
    @sa axis2_async_result_ops#free*/
#define AXIS2_ASYNC_RESULT_FREE(async_result, env) \
      ((async_result)->ops->free (async_result, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_ASYNC_RESULT_H */
