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

#ifndef AXIS2_TRANSPORT_IN_DESC_H
#define AXIS2_TRANSPORT_IN_DESC_H

/** 
 * @defgroup axis2_transport_in_desc transport in description
 * @ingroup axis2_desc
 * transport in description represents a transport receiver configured in 
 * Axis2 configuration. There can be multiple transport receivers configured 
 * in axis2.xml file and each of them will be represented with a transport 
 * in description instance. deployment engine takes care of creating and 
 * instantiating transport in descriptions.
 * transport in description encapsulates flows related to the transport in 
 * and also holds a reference to related transport receiver.
 * @{
 */

/**
 * @file axis2_transport_in_desc.h
 * @brief Axis2 description transport in interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_array_list.h> 
/*#include <axis2_transport_receiver.h>*/
#include <axis2_phase_meta.h>
#include <axis2_phase.h>
#include <axis2_flow.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_transport_in_desc */
    typedef struct axis2_transport_in_desc axis2_transport_in_desc_t;
    /** Type name for struct axis2_transport_in_desc_ops  */
    typedef struct axis2_transport_in_desc_ops axis2_transport_in_desc_ops_t;

    struct axis2_phase;
    struct axis2_transport_receiver;

    /**
     * transport in description ops struct.
     * Encapsulator struct for ops of axis2_transport_in_desc.
     */
    struct axis2_transport_in_desc_ops
    {
        /** 
         * Frees transport in description. 
         * @param transport_in_desc pointer to transport in description struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env);

        /**
         * Frees transport in description given as a void parameter.
         * @param transport_in pointer to transport in description as a void 
         * pointer
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
            free_void_arg) (
                    void *transport_in,
                    const axis2_env_t *env);

        /**
         * Gets transport enum.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @return transport enum 
         */
        AXIS2_TRANSPORT_ENUMS (AXIS2_CALL *
                get_enum)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * Sets transport enum.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @param trans_enum transport enum
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_enum)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    const AXIS2_TRANSPORT_ENUMS trans_enum);

        /**
         * Gets in flow. In flow represents the list of phases invoked 
         * along the receive path.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @return pointer to flow representing in flow, returns a reference,
         * not a cloned copy
         */
        struct axis2_flow *(AXIS2_CALL *
                get_in_flow)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * Sets in flow. In flow represents the list of phases invoked 
         * along the receive path.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @param in_flow pointer to in flow representing in flow, transport in 
         * description assumes ownership of the flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_in_flow)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_flow *in_flow);

        /**
         * Gets fault in flow. Fault in flow represents the list of phases 
         * invoked along the receive path if a fault happens.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @return pointer to flow representing fault in flow, returns a 
         * reference, not a cloned copy 
         */
        struct axis2_flow *(AXIS2_CALL *
                get_fault_in_flow)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * Sets fault in flow. Fault in flow represents the list of phases 
         * invoked along the receive path if a fault happens.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @param fault_in_flow pointer to flow representing fault in flow, 
         * transport in description assumes the ownership of the flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_in_flow)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_flow *fault_in_flow);

        /**
         * Gets transport receiver associated with the transport in description.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @return pointer to transport receiver, returns a reference, not 
         * a cloned copy
         */
        struct axis2_transport_receiver *(AXIS2_CALL *
                get_recv)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * Sets transport receiver associated with the transport in description.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @param recv pointer to transport receiver, transport in 
         * description assumes ownership of the receiver
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_recv)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_transport_receiver *recv);

        /**
         * Gets the transport in phase associated with transport in description.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @return transport in phase, returns a reference, not a cloned copy
         */
        struct axis2_phase *(AXIS2_CALL *
                get_in_phase)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * Sets the transport in phase associated with transport in description.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @param in_phase pointer to phase representing transport in phase, 
         * transport in description assumes ownership of phase
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_in_phase)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_phase *in_phase);

        /**
         * Gets the transport fault phase associated with transport in description.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @return pointer to phase representing fault phase
         */
        struct axis2_phase *(AXIS2_CALL *
                get_fault_phase)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * Sets the transport fault phase associated with transport in description.
         * @param transport_in pointer to transport in description struct
         * @param env pointer to environment struct
         * @param fault_phase pointer to fault phase
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_phase)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_phase *fault_phase);

        /**
         * Adds given parameter.
         * @param transport_in_desc pointer to transport in description struct
         * @param env pointer to environment struct
         * @param param pointer to parameter, transport in description assumes
         * ownership of parameter
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets named parameter.
         * @param transport_in_desc pointer to transport in description struct
         * @param env pointer to environment struct
         * @param param_name parameter name string
         * @return pointer to named parameter if it exists, else NULL. Returns
         * a reference, not a cloned copy
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Checks if the named parameter is locked.
         * @param transport_in_desc pointer to transport in description struct
         * @param env pointer to environment struct
         * @param param_name parameter name string
         * @return AXIS2_TRUE if named parameter is locked, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);
    };

    /**
     * transport in description struct.
     */
    struct axis2_transport_in_desc
    {
        /** transport in description struct ops */
        axis2_transport_in_desc_ops_t *ops;
        /** parameter container to hold transport in related parameters */
        axis2_param_container_t *param_container;
    };

    /**
     * Creates transport in description with given transport enum.
     * @param env pointer to environment struct
     * @param trans_enum transport enum 
     * @return pointer to newly created phase holder
     */
    AXIS2_EXTERN axis2_transport_in_desc_t *AXIS2_CALL
    axis2_transport_in_desc_create(
            const axis2_env_t *env,
            const AXIS2_TRANSPORT_ENUMS trans_enum);

    /**
     * Frees transport in description given as a void parameter.
     * @param transport_in pointer to transport in description as a void 
     * pointer
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
     */
     axis2_status_t AXIS2_CALL
     axis2_transport_in_desc_free_void_arg(
            void *transport_in,
            const axis2_env_t *env);

/** Frees transport in description.
    @sa axis2_transport_in_desc_ops#free */
#define AXIS2_TRANSPORT_IN_DESC_FREE(transport_in_desc, env) \
      ((transport_in_desc)->ops->free (transport_in_desc, env))

/** Gets transport enum.
    @sa axis2_transport_in_desc_ops#get_enum */
#define AXIS2_TRANSPORT_IN_DESC_GET_ENUM(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_enum (transport_in_desc, env))

/** Sets transport enum.
    @sa axis2_transport_in_desc_ops#set_enum */
#define AXIS2_TRANSPORT_IN_DESC_SET_ENUM(transport_in_desc, env, trans_enum) \
      ((transport_in_desc)->ops->set_enum (transport_in_desc, env, trans_enum))

/** Gets in flow.
    @sa axis2_transport_in_desc_ops#get_in_flow */
#define AXIS2_TRANSPORT_IN_DESC_GET_IN_FLOW(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_in_flow (transport_in_desc, env))

/** Sets in flow.
    @sa axis2_transport_in_desc_ops#set_in_flow */
#define AXIS2_TRANSPORT_IN_DESC_SET_IN_FLOW(transport_in_desc, env, in_flow) \
      ((transport_in_desc)->ops->set_in_flow (transport_in_desc, env, in_flow))

/** Gets fault flow.
    @sa axis2_transport_in_desc_ops#get_fault_in_flow */
#define AXIS2_TRANSPORT_IN_DESC_GET_FAULT_IN_FLOW(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_fault_in_flow (transport_in_desc, env))

/** Sets fault flow.
    @sa axis2_transport_in_desc_ops#set_fault_in_flow */
#define AXIS2_TRANSPORT_IN_DESC_SET_FAULT_IN_FLOW(transport_in_desc, env, fault_in_flow) \
      ((transport_in_desc)->ops->set_fault_in_flow (transport_in_desc, env, fault_in_flow))

/** Gets transport receiver.
    @sa axis2_transport_in_desc_ops#get_recv */
#define AXIS2_TRANSPORT_IN_DESC_GET_RECV(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_recv (transport_in_desc, env))

/** Sets transport receiver.
    @sa axis2_transport_in_desc_ops#set_recv */
#define AXIS2_TRANSPORT_IN_DESC_SET_RECV(transport_in_desc, env, recv) \
      ((transport_in_desc)->ops->set_recv (transport_in_desc, env, recv))

/** Gets in phase.
    @sa axis2_transport_in_desc_ops#get_in_phase */
#define AXIS2_TRANSPORT_IN_DESC_GET_IN_PHASE(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_in_phase (transport_in_desc, env))

/** Sets in phase.
    @sa axis2_transport_in_desc_ops#set_in_phase */
#define AXIS2_TRANSPORT_IN_DESC_SET_IN_PHASE(transport_in_desc, env, in_phase) \
      ((transport_in_desc)->ops->set_in_phase (transport_in_desc, env, in_phase))

/** Gets fault phase.
    @sa axis2_transport_in_desc_ops#get_fault_phase */
#define AXIS2_TRANSPORT_IN_DESC_GET_FAULT_PHASE(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_fault_phase (transport_in_desc, env))

/** Sets fault phase.
    @sa axis2_transport_in_desc_ops#set_fault_phase */
#define AXIS2_TRANSPORT_IN_DESC_SET_FAULT_PHASE(transport_in_desc, env, fault_phase) \
      ((transport_in_desc)->ops->set_fault_phase (transport_in_desc, env, fault_phase))

/** Adds parameter.
    @sa axis2_transport_in_desc_ops#add_param */
#define AXIS2_TRANSPORT_IN_DESC_ADD_PARAM(transport_in_desc, env, param) \
      ((transport_in_desc)->ops->add_param (transport_in_desc, env, param))

/** Gets named parameter.
    @sa axis2_transport_in_desc_ops#get_param */
#define AXIS2_TRANSPORT_IN_DESC_GET_PARAM(transport_in_desc, env, param_name) \
      ((transport_in_desc)->ops->get_param (transport_in_desc, env, param_name))

/** Checks if the named parameter is locked.
    @sa axis2_transport_in_desc_ops#is_param_locked */
#define AXIS2_TRANSPORT_IN_DESC_IS_PARAM_LOCKED(transport_in_desc, env, param_name) \
      ((transport_in_desc)->ops->is_param_locked (transport_in_desc, env, param_name))

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_IN_DESC_H */
