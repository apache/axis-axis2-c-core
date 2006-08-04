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

#ifndef AXIS2_FLOW_H
#define AXIS2_FLOW_H

/** @defgroup axis2_flow flow
 * @ingroup axis2_desc
 * Description.
 * @{
 */

/**
 * @file axis2_flow.h
 * @brief axis2 flow interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_handler_desc.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_flow_ops */
    typedef struct axis2_flow_ops axis2_flow_ops_t;
    /** Type name for struct axis2_flow */
    typedef struct axis2_flow axis2_flow_t;


    /**
     * Flow ops struct
     * Encapsulator struct for ops of axis2_flow
     */
    struct axis2_flow_ops
    {
        /** Deallocate memory
    * @param flow pointer to flow
    * @param env pointer to environment struct
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_flow_t *flow,
                    const axis2_env_t *env);

        /**
         * Add handler description
    * @param flow pointer to flow
    * @param env pointer to environment struct
         * @param handler handler description
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_handler)(
                    axis2_flow_t *flow,
                    const axis2_env_t *env,
                    axis2_handler_desc_t *handler);

        /**
         * Get handler
    * @param flow pointer to flow
    * @param env pointer to environment struct
         * @param index index of the handler
         * @return handler description
         */
        axis2_handler_desc_t * (AXIS2_CALL *
                get_handler)(
                    const axis2_flow_t *flow,
                    const axis2_env_t *env,
                    int index);

        /**
         * Get handler count
    * @param flow pointer to flow
    * @param env pointer to environment struct
         * @return handler count
         */
        int (AXIS2_CALL *
                get_handler_count)(
                    const axis2_flow_t *flow,
                    const axis2_env_t *env);
    };

    /**
     * Flow struct
     */
    struct axis2_flow
    {
        /** Operations of flow */
        axis2_flow_ops_t *ops;
    };

    /**
     * Creates flow struct
     * @param env pointer to environment struct
     * @return pointer to newly created flow
     */
    AXIS2_EXTERN axis2_flow_t * AXIS2_CALL
    axis2_flow_create (
        const axis2_env_t *env);

    /**
     * Free flow passed as void pointer. This will be
     * cast into appropriate type and then pass the cast object
     * into the flow structure's free method
     * @param flow pointer to flow
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_flow_free_void_arg (
        void *flow,
        const axis2_env_t *env);

/*************************** Function macros **********************************/

/** Frees the flow.
    @sa axis2_flow_ops#free */
#define AXIS2_FLOW_FREE(flow, env) ((flow)->ops->free (flow, env))

/** Adds the handler.
    @sa axis2_flow_ops#add_handler */
#define AXIS2_FLOW_ADD_HANDLER(flow, env, handler) \
      ((flow)->ops->add_handler (flow, env, handler))

/** Gets the handler.
    @sa axis2_flow_ops#get_handler */
#define AXIS2_FLOW_GET_HANDLER(flow, env, index) \
      ((flow)->ops->get_handler (flow, env, index))

/** Gets the handler count.
    @sa axis2_flow_ops#get_handler_count */
#define AXIS2_FLOW_GET_HANDLER_COUNT(flow, env) \
      ((flow)->ops->get_handler_count (flow, env))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_FLOW_H */
