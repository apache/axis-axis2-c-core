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

#ifndef AXIS2_TRANSPORT_IN_DESC_H
#define AXIS2_TRANSPORT_IN_DESC_H

/** 
 * @defgroup axis2_transport_in_desc transport in description
 * @ingroup axis2_desc
 * Description.
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
#include <axis2_qname.h>
#include <axis2_array_list.h> 
/*#include <axis2_transport_receiver.h>*/
#include <axis2_phase_meta.h>
#include <axis2_phase.h>
#include <axis2_flow.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_phase;
    struct axis2_transport_receiver;
    /** Type name for struct axis2_transport_in_desc */
    typedef struct axis2_transport_in_desc axis2_transport_in_desc_t;
    /** Type name for struct axis2_transport_in_desc_ops  */
    typedef struct axis2_transport_in_desc_ops axis2_transport_in_desc_ops_t;

    /**
     * Description Transport In ops struct
     * Encapsulator struct for ops of axis2_transport_in_desc
     */
    struct axis2_transport_in_desc_ops
    {
        /** 
         * De-allocate memory
         * @param transport_in_desc pointer to transport in description
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         * @param qname pointer to qname struct
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         */
        struct axis2_flow *(AXIS2_CALL *
                get_inflow)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * @param transport_in pointer to transport in 
         * @param env pointer to environment struct
         * @param inflow pointer to in flow
         */
        axis2_status_t (AXIS2_CALL *
                set_inflow)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_flow *inflow);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         */
        struct axis2_flow *(AXIS2_CALL *
                get_faultflow)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         * @param faultflow pointer to fault flow
         */
        axis2_status_t (AXIS2_CALL *
                set_faultflow)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_flow *faultflow);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         */
        struct axis2_transport_receiver *(AXIS2_CALL *
                get_recv)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         * @param recv pointer to transport receiver
         */
        axis2_status_t (AXIS2_CALL *
                set_recv)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_transport_receiver *recv);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         */
        struct axis2_phase *(AXIS2_CALL *
                get_in_phase)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         * @param in_phase pointer to in phase
         */
        axis2_status_t (AXIS2_CALL *
                set_in_phase)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_phase *in_phase);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         */
        struct axis2_phase *(AXIS2_CALL *
                get_faultphase)(
                    const axis2_transport_in_desc_t *transport_in,
                    const axis2_env_t *env);

        /**
         * @param transport_in pointer to transport in
         * @param env pointer to environment struct
         * @param faultphase pointer to fault phase
         */
        axis2_status_t (AXIS2_CALL *
                set_faultphase)(
                    struct axis2_transport_in_desc *transport_in,
                    const axis2_env_t *env,
                    struct axis2_phase *faultphase);

        /**
         * @param transport_in_desc pointer to transport in description
         * @param env pointer to environment struct
         * @param param pointer to param
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * @param transport_in_desc pointer to transport in description
         * @param env pointer to environment struct
         * @param param_name pointer to parameter name
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env,
                    axis2_char_t *param_name);

        /**
         * @param transport_in_desc pointer to transport in description
         * @param env pointer to environment struct
         * @param param_name pointer to parameter name
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_transport_in_desc_t *transport_in_desc,
                    const axis2_env_t *env,
                    axis2_char_t *param_name);
    };

    /**
     * transport in description struct
     */
    struct axis2_transport_in_desc
    {
        /** transport in description struct ops */
        axis2_transport_in_desc_ops_t *ops;
        axis2_param_container_t *param_container;
    };

    /**
     * Creates phase holder struct
     * @param env pointer to environment struct
     * @param qname pointer to qname
     * @return pointer to newly created phase holder
     */
    AXIS2_EXTERN axis2_transport_in_desc_t *AXIS2_CALL
    axis2_transport_in_desc_create_with_qname (
            const axis2_env_t *env,
            axis2_qname_t *qname);

/*************************** Function macros **********************************/

/** Frees the transport in description.
    @sa axis2_transport_in_desc_ops#free */
#define AXIS2_TRANSPORT_IN_DESC_FREE(transport_in_desc, env) \
      ((transport_in_desc)->ops->free (transport_in_desc, env))

/** Gets the qualified name.
    @sa axis2_transport_in_desc_ops#get_qname */
#define AXIS2_TRANSPORT_IN_DESC_GET_QNAME(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_qname (transport_in_desc, env))

/** Sets the qualified name.
    @sa axis2_transport_in_desc_ops#set_qname */
#define AXIS2_TRANSPORT_IN_DESC_SET_QNAME(transport_in_desc, env, qname) \
      ((transport_in_desc)->ops->set_qname (transport_in_desc, env, qname))

/** Gets the in flow.
    @sa axis2_transport_in_desc_ops#get_inflow */
#define AXIS2_TRANSPORT_IN_DESC_GET_INFLOW(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_inflow (transport_in_desc, env))

/** Sets the in flow.
    @sa axis2_transport_in_desc_ops#set_inflow */
#define AXIS2_TRANSPORT_IN_DESC_SET_INFLOW(transport_in_desc, env, inflow) \
      ((transport_in_desc)->ops->set_inflow (transport_in_desc, env, inflow))

/** Gets the fault flow.
    @sa axis2_transport_in_desc_ops#get_faultflow */
#define AXIS2_TRANSPORT_IN_DESC_GET_FAULTFLOW(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_faultflow (transport_in_desc, env))

/** Sets the fault flow.
    @sa axis2_transport_in_desc_ops#set_faultflow */
#define AXIS2_TRANSPORT_IN_DESC_SET_FAULTFLOW(transport_in_desc, env, faultflow) \
      ((transport_in_desc)->ops->set_faultflow (transport_in_desc, env, faultflow))

/** Gets the receiver.
    @sa axis2_transport_in_desc_ops#get_recv */
#define AXIS2_TRANSPORT_IN_DESC_GET_RECV(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_recv (transport_in_desc, env))

/** Sets the receiver.
    @sa axis2_transport_in_desc_ops#set_recv */
#define AXIS2_TRANSPORT_IN_DESC_SET_RECV(transport_in_desc, env, recv) \
      ((transport_in_desc)->ops->set_recv (transport_in_desc, env, recv))

/** Gets the in phase.
    @sa axis2_transport_in_desc_ops#get_in_phase */
#define AXIS2_TRANSPORT_IN_DESC_GET_IN_PHASE(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_in_phase (transport_in_desc, env))

/** Sets the in phase.
    @sa axis2_transport_in_desc_ops#set_in_phase */
#define AXIS2_TRANSPORT_IN_DESC_SET_IN_PHASE(transport_in_desc, env, in_phase) \
      ((transport_in_desc)->ops->set_in_phase (transport_in_desc, env, in_phase))

/** Gets the fault phase.
    @sa axis2_transport_in_desc_ops#get_faultphase */
#define AXIS2_TRANSPORT_IN_DESC_GET_FAULTPHASE(transport_in_desc, env) \
      ((transport_in_desc)->ops->get_faultphase (transport_in_desc, env))

/** Sets the fault phase.
    @sa axis2_transport_in_desc_ops#set_faultphase */
#define AXIS2_TRANSPORT_IN_DESC_SET_FAULTPHASE(transport_in_desc, env, faultphase) \
      ((transport_in_desc)->ops->set_faultphase (transport_in_desc, env, faultphase))

/** Adds the parameter.
    @sa axis2_transport_in_desc_ops#add_param */
#define AXIS2_TRANSPORT_IN_DESC_ADD_PARAM(transport_in_desc, env, param) \
      ((transport_in_desc)->ops->add_param (transport_in_desc, env, param))

/** Gets the parameter.
    @sa axis2_transport_in_desc_ops#get_param */
#define AXIS2_TRANSPORT_IN_DESC_GET_PARAM(transport_in_desc, env, param_name) \
      ((transport_in_desc)->ops->get_param (transport_in_desc, env, param_name))

/** Is param locked.
    @sa axis2_transport_in_desc_ops#is_param_locked */
#define AXIS2_TRANSPORT_IN_DESC_IS_PARAM_LOCKED(transport_in_desc, env, param_name) \
      ((transport_in_desc)->ops->is_param_locked (transport_in_desc, env, param_name))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_IN_DESC_H */
