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

#ifndef AXIS2_FLOW_CONTAINER_H
#define AXIS2_FLOW_CONTAINER_H

/** 
 * @defgroup axis2_flow_container flow container
 * @ingroup axis2_desc
 * Flow container is the encapsulating struct for all the four flows. The four flows 
 * possible are in flow, out flow, in fault flow and out fault flow.
 * @{
 */

/**
 * @file axis2_flow_container.h
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_flow.h>


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_flow_container_ops */
    typedef struct axis2_flow_container_ops axis2_flow_container_ops_t;
    /** Type name for struct axis2_flow_container */
    typedef struct axis2_flow_container axis2_flow_container_t;


    /**
     * flow container ops struct.
     * Encapsulator struct for ops of axis2_flow_container.
     */
    struct axis2_flow_container_ops
    {
        /** 
         * Frees flow container.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_flow_container_t *flow_container,
                    const axis2_env_t *env);

        /**
         * Gets in flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @return pointer to in flow, returns a reference, not a cloned copy
         */
        axis2_flow_t *(AXIS2_CALL *
                get_in_flow)(
                    const axis2_flow_container_t *flow_container,
                    const axis2_env_t *env);

        /**
         * Sets in flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @param in_flow pointer to in flow struct, flow container assumes 
         * ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_in_flow)(
                    axis2_flow_container_t *flow_container,
                    const axis2_env_t *env,
                    axis2_flow_t *in_flow);

        /**
         * Gets out flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @return out flow, returns a reference, not a cloned copy
         */
        axis2_flow_t *(AXIS2_CALL *
                get_out_flow)(
                    const axis2_flow_container_t *flow_container,
                    const axis2_env_t *env);

        /**
         * Sets out flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @param out_flow pointer to out flow, flow container assumes 
         * ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_flow)(
                    axis2_flow_container_t *flow_container,
                    const axis2_env_t *env,
                    axis2_flow_t *out_flow);

        /**
         * Gets fault in flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @return fault in flow, returns a reference, not a cloned copy
         */
        axis2_flow_t *(AXIS2_CALL *
                get_fault_in_flow)(
                    const axis2_flow_container_t *flow_container,
                    const axis2_env_t *env);

        /**
         * Sets fault in flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @param falut_in_flow pointer to falut in flow, flow container assumes 
         * ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_in_flow)(
                    axis2_flow_container_t *flow_container,
                    const axis2_env_t *env,
                    axis2_flow_t *falut_in_flow);
        /**
         * Gets fault out flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @return fault out flow, returns a reference, not a cloned copy
         */
        axis2_flow_t *(AXIS2_CALL *
                get_fault_out_flow)(
                    const axis2_flow_container_t *flow_container,
                    const axis2_env_t *env);

        /**
         * Sets fault out flow.
         * @param flow_container pointer to flow container
         * @param env pointer to environment struct
         * @param fault_out_flow pointer to fault out flow, flow container assumes 
         * ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_out_flow)(
                    axis2_flow_container_t *flow_container,
                    const axis2_env_t *env,
                    axis2_flow_t *fault_out_flow);
    };

    /**
     * flow container struct.
     */
    struct axis2_flow_container
    {
        /** Operations of flow container struct */
        axis2_flow_container_ops_t *ops;
    };

    /**
     * Creates flow container struct.
     * @param env pointer to environment struct
     * @return pointer to newly created flow container
     */
    AXIS2_EXTERN axis2_flow_container_t *AXIS2_CALL
    axis2_flow_container_create (
        const axis2_env_t *env);


/** Frees flow container.
    @sa axis2_flow_container_ops#free */
#define AXIS2_FLOW_CONTAINER_FREE(flow_container, env) \
        ((flow_container)->ops->free (flow_container, env))

/** Gets in flow.
    @sa axis2_flow_container_ops#get_in_flow */
#define AXIS2_FLOW_CONTAINER_GET_IN_FLOW(flow_container, env) \
        ((flow_container)->ops->get_in_flow (flow_container, env))

/** Sets in flow.
    @sa axis2_flow_container_ops#set_in_flow */
#define AXIS2_FLOW_CONTAINER_SET_IN_FLOW(flow_container, env, in_flow) \
        ((flow_container)->ops->set_in_flow (flow_container, env, in_flow))

/** Gets out flow.
    @sa axis2_flow_container_ops#get_out_flow */
#define AXIS2_FLOW_CONTAINER_GET_OUT_FLOW(flow_container, env) \
        ((flow_container)->ops->get_out_flow (flow_container, env))

/** Sets out flow.
    @sa axis2_flow_container_ops#set_out_flow */
#define AXIS2_FLOW_CONTAINER_SET_OUT_FLOW(flow_container, env, out_flow) \
        ((flow_container)->ops->set_out_flow (flow_container, env, out_flow))

/** Gets fault in flow.
    @sa axis2_flow_container_ops#get_fault_in_flow */
#define AXIS2_FLOW_CONTAINER_GET_FAULT_IN_FLOW(flow_container, env) \
        ((flow_container)->ops->get_fault_in_flow (flow_container, env))

/** Sets fault in flow.
    @sa axis2_flow_container_ops#set_fault_in_flow */
#define AXIS2_FLOW_CONTAINER_SET_FAULT_IN_FLOW(flow_container, env, fault_in_flow) \
        ((flow_container)->ops->set_fault_in_flow (flow_container, env, fault_in_flow))

/** Gets fault out flow.
    @sa axis2_flow_container_ops#get_fault_out_flow */
#define AXIS2_FLOW_CONTAINER_GET_FAULT_OUT_FLOW(flow_container, env) \
        ((flow_container)->ops->get_fault_out_flow (flow_container, env))

/** Sets fault out flow.
    @sa axis2_flow_container_ops#set_fault_out_flow */
#define AXIS2_FLOW_CONTAINER_SET_FAULT_OUT_FLOW(flow_container, env, fault_out_flow) \
        ((flow_container)->ops->set_fault_out_flow (flow_container, env, fault_out_flow))

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_FLOW_CONTAINER_H */
