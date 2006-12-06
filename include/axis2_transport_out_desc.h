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

#ifndef AXIS2_TRANSPORT_OUT_DESC_H
#define AXIS2_TRANSPORT_OUT_DESC_H

/** 
 * @defgroup axis2_transport_out_desc transport out description
 * @ingroup axis2_desc
 * transport out description represents a transport sender configured in 
 * Axis2 configuration. There can be multiple transport senders configured 
 * in axis2.xml file and each of them will be represented with a transport 
 * out description instance. deployment engine takes care of creating and 
 * instantiating transport out descriptions.
 * transport out description encapsulates flows related to the transport out
 * and also holds a reference to related transport sender.
 * @{
 */

/**
 * @file axis2_transport_out_desc.h
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_array_list.h>
#include <axis2_phase_meta.h>
#include <axis2_phase.h>
#include <axis2_flow.h>
#include <axis2_transport_sender.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_transport_out_desc */
    typedef struct axis2_transport_out_desc axis2_transport_out_desc_t;
    /** Type name for struct axis2_transport_out_desc_ops  */
    typedef struct axis2_transport_out_desc_ops axis2_transport_out_desc_ops_t;

    struct axis2_phase;
    struct axis2_transport_sender;

    /**
     * transport out description ops struct.
     * Encapsulator struct for ops of axis2_transport_out_desc.
     */
    struct axis2_transport_out_desc_ops
    {
        /**
         * Frees transport out description.
         * @param transport_out_dec pointer to transport out description
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env);

        /**
         * Frees transport out description given as a void pointer.
         * @param transport_out_dec pointer to transport out description as a 
         * void pointer 
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free_void_arg)(
                    void *transport_out,
                    const axis2_env_t *env);

        /**
         * Gets QName.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @return pointer to QName, returns a reference, not a cloned copy
         */
        const axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    const axis2_transport_out_desc_t *transport_out,
                    const axis2_env_t *env);

        /**
         * Sets QName.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @param qname pointer to QName, this method creates a clone of QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

        /**
         * Gets out flow. Out flow represents the list of phases invoked 
         * along the sender path.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @return pointer to flow representing out flow, returns a reference,
         * not a cloned copy
         */
        struct axis2_flow *(AXIS2_CALL *
                get_out_flow)(
                    const axis2_transport_out_desc_t *transport_out,
                    const axis2_env_t *env);

        /**
         * Sets out flow. Out flow represents the list of phases invoked 
         * along the sender path.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @param out_flow pointer to out flow, transport out description 
         * assumes ownership of flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_flow)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_flow *out_flow);

        /**
         * Gets fault out flow. Fault out flow represents the list of phases 
         * invoked along the sender path if a fault happens.         
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @return pointer to flow representing fault out flow
         */
        struct axis2_flow *(AXIS2_CALL *
                get_fault_out_flow)(
                    const axis2_transport_out_desc_t *transport_out,
                    const axis2_env_t *env);

        /**
         * Sets fault out flow. Fault out flow represents the list of phases 
         * invoked along the sender path if a fault happens.         
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @param fault_out_flow pointer to fault_out_flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_out_flow)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_flow *fault_out_flow);

        /**
         * Gets transport sender.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @return pointer to transport sender associated wit the transport out
         * description, returns a reference, not a cloned copy
         */
        axis2_transport_sender_t *(AXIS2_CALL *
                get_sender)(
                    const axis2_transport_out_desc_t *transport_out,
                    const axis2_env_t *env);

        /**
         * Sets transport sender.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @param sender pointer to transport sender, transport description
         * assumes ownership of sender 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_sender)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    axis2_transport_sender_t *sender);

        /**
         * Gets transport out phase.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @return pointer to phase representing transport out phase, returns a 
         * reference, not a cloned copy
         */
        struct axis2_phase * (AXIS2_CALL *
                get_out_phase)(
                    const axis2_transport_out_desc_t *transport_out,
                    const axis2_env_t *env);

        /**
         * Sets transport out phase.
         * @param transport_out pointer to transport_out 
         * @param env pointer to environment struct
         * @param out_phase pointer to phase representing transport out phase,
         * transport out description assumes ownership of phase
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_phase)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_phase *out_phase);

        /**
         * Gets fault phase.
         * @param transport_out  pointer to transport_out
         * @param env pointer to environment struct
         * @return pointer to phase representing fault phase, returns a 
         * reference, not a cloned copy
         */
        struct axis2_phase *(AXIS2_CALL *
                get_fault_phase)(
                    const axis2_transport_out_desc_t *transport_out,
                    const axis2_env_t *env);

        /**
         * Sets fault phase.
         * @param transport_out pointer to transport_out
         * @param env pointer to environment struct
         * @param fault_phase pointer to phase representing fault phase, 
         * transport description assumes ownership of phase
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_phase)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_phase *fault_phase);

        /**
         * Adds parameter.
         * @param transport_out_desc pointer to tarn sport out description
         * @param env pointer to environment struct
         * @param param pointer to parameter, transport out description assumes
         * ownership of parameter
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets named parameter. 
         * @param transport_out_desc pointer to transport out description
         * @param env pointer to environment struct
         * @param param_name parameter name string
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Checks if the named parameter is locked.
         * @param transport_out_desc pointer to transport out description
         * @param env pointer to environment struct
         * @param param_name pointer to parameter name
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

    };

    /**
     * transport out description struct.
     */
    struct axis2_transport_out_desc
    {
        /** operations of transport out description struct */
        axis2_transport_out_desc_ops_t *ops;
        /** parameter container that holds parameter */
        axis2_param_container_t *param_container;
    };

    /**
     * Creates transport out description with given QName.
     * @param env pointer to environment struct
     * @param qname pointer to QName, newly created transport out description
     * creates a clone of this
     * @return pointer to newly created transport out
     */
    AXIS2_EXTERN axis2_transport_out_desc_t * AXIS2_CALL
    axis2_transport_out_desc_create_with_qname (
        const axis2_env_t *env,
        const axis2_qname_t *qname);

    /**
     * Frees transport out description given as a void pointer.
     * @param transport_out_dec pointer to transport out description as a 
     * void pointer 
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    axis2_status_t AXIS2_CALL
    axis2_transport_out_desc_free_void_arg(
        void *transport_out,
        const axis2_env_t *env);

/** Frees transport out description.
    @sa axis2_transport_out_desc_ops#free */
#define AXIS2_TRANSPORT_OUT_DESC_FREE(transport_out_desc, env) \
      ((transport_out_desc)->ops->free (transport_out_desc, env))

/** Gets QName.
    @sa axis2_transport_out_desc_ops#get_qname */
#define AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_qname (transport_out_desc, env))

/** Sets QName.
    @sa axis2_transport_out_desc_ops#set_qname */
#define AXIS2_TRANSPORT_OUT_DESC_SET_QNAME(transport_out_desc, env, qname) \
      ((transport_out_desc)->ops->set_qname (transport_out_desc, env, qname))

/** Gets out flow.
    @sa axis2_transport_out_desc_ops#get_out_flow */
#define AXIS2_TRANSPORT_OUT_DESC_GET_OUT_FLOW(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_out_flow (transport_out_desc, env))

/** Sets out flow.
    @sa axis2_transport_out_desc_ops#set_out_flow */
#define AXIS2_TRANSPORT_OUT_DESC_SET_OUT_FLOW(transport_out_desc, env, out_flow) \
      ((transport_out_desc)->ops->set_out_flow (transport_out_desc, env, out_flow))

/** Gets fault out flow.
    @sa axis2_transport_out_desc_ops#get_fault_out_flow */
#define AXIS2_TRANSPORT_OUT_DESC_GET_FAULT_OUT_FLOW(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_fault_out_flow (transport_out_desc, env))

/** Sets fault out flow.
    @sa axis2_transport_out_desc_ops#set_fault_out_flow */
#define AXIS2_TRANSPORT_OUT_DESC_SET_FAULT_OUT_FLOW(transport_out_desc, env, fault_out_flow) \
      ((transport_out_desc)->ops->set_fault_out_flow (transport_out_desc, env, fault_out_flow))

/** Gets transport sender.
    @sa axis2_transport_out_desc_ops#get_sender */
#define AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_sender (transport_out_desc, env))

/** Sets transport sender.
    @sa axis2_transport_out_desc_ops#set_sender */
#define AXIS2_TRANSPORT_OUT_DESC_SET_SENDER(transport_out_desc, env, sender) \
      ((transport_out_desc)->ops->set_sender (transport_out_desc, env, sender))

/** Gets out phase.
    @sa axis2_transport_out_desc_ops#get_out_phase */
#define AXIS2_TRANSPORT_OUT_DESC_GET_OUT_PHASE(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_out_phase (transport_out_desc, env))

/** Sets out phase.
    @sa axis2_transport_out_desc_ops#set_out_phase */
#define AXIS2_TRANSPORT_OUT_DESC_SET_OUT_PHASE(transport_out_desc, env, out_phase) \
      ((transport_out_desc)->ops->set_out_phase (transport_out_desc, env, out_phase))

/** Gets fault phase.
    @sa axis2_transport_out_desc_ops#get_fault_phase */
#define AXIS2_TRANSPORT_OUT_DESC_GET_FAULT_PHASE(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_fault_phase (transport_out_desc, env))

/** Sets fault phase.
    @sa axis2_transport_out_desc_ops#set_fault_phase */
#define AXIS2_TRANSPORT_OUT_DESC_SET_FAULT_PHASE(transport_out_desc, env, fault_phase) \
      ((transport_out_desc)->ops->set_fault_phase (transport_out_desc, env, fault_phase))

/** Adds given parameter.
    @sa axis2_transport_out_desc_ops#add_param */
#define AXIS2_TRANSPORT_OUT_DESC_ADD_PARAM(transport_out_desc, env, param) \
      ((transport_out_desc)->ops->add_param (transport_out_desc, env, param))

/** Gets named parameter.
    @sa axis2_transport_out_desc_ops#get_param */
#define AXIS2_TRANSPORT_OUT_DESC_GET_PARAM(transport_out_desc, env, param_name) \
      ((transport_out_desc)->ops->get_param (transport_out_desc, env, param_name))

/** Checks if the named parameter locked.
    @sa axis2_transport_out_desc_ops#is_param_locked */
#define AXIS2_TRANSPORT_OUT_DESC_IS_PARAM_LOCKED(transport_out_desc, env, param_name) \
      ((transport_out_desc)->ops->is_param_locked (transport_out_desc, env, param_name))

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_OUT_DESC_H */
