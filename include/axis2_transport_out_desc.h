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

#ifndef AXIS2_TRANSPORT_OUT_DESC_H
#define AXIS2_TRANSPORT_OUT_DESC_H

/** @defgroup axis2_transport_out_desc transport out description
 * @ingroup axis2_desc
 * Description.
 * @{
 */

/**
 * @file axis2_transport_out_desc.h
 * @brief Axis2 description transport out interface
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

    struct axis2_phase;
    struct axis2_transport_sender;
    /** Type name for struct axis2_transport_out_desc */
    typedef struct axis2_transport_out_desc axis2_transport_out_desc_t;
    /** Type name for struct axis2_transport_out_desc_ops  */
    typedef struct axis2_transport_out_desc_ops axis2_transport_out_desc_ops_t;

    /**
     * @brief Description Transport Out ops struct
     * Encapsulator struct for ops of axis2_transport_out_desc
     */
    AXIS2_DECLARE_DATA struct axis2_transport_out_desc_ops
    {
        /** 
    * De-allocate memory
    * @param transport_out_dec pointer to transport out description
    * @param env pointer to environment struct
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
        */
        axis2_status_t (AXIS2_CALL *
           free)(
                    axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env);

    /**
     * De-allocate memory
     * @param void transport_out to be freed
     * @return status code
     */
        axis2_status_t (AXIS2_CALL *
            free_void_arg) (
                    void *transport_out,
                    const axis2_env_t *env);

        /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);

        /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    * @param qname pointer to qname
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

   /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    */
        struct axis2_flow *(AXIS2_CALL *
                get_outflow)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);

   /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    * @param outflow pointer to out flow
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
        axis2_status_t (AXIS2_CALL *
                set_outflow)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_flow *outflow);

   /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    */
        struct axis2_flow *(AXIS2_CALL *
                get_faultflow)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);

   /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    * @param faultflow pointer to faultflow
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
        axis2_status_t (AXIS2_CALL *
                set_faultflow)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_flow *faultflow);

        /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
         */
        axis2_transport_sender_t *(AXIS2_CALL *
                get_sender)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);

        /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    * @param sender pointer to sender
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_sender)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    axis2_transport_sender_t *sender);

        /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    */
        struct axis2_phase * (AXIS2_CALL *
                get_out_phase)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);

   /**
    * @param transport_out pointer to transport_out 
    * @param env pointer to environment struct
    * @param out_phase pointer to out phase
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
        axis2_status_t (AXIS2_CALL *
                set_out_phase)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_phase *out_phase);

   /**
    * @param transport_out  pointer to transport_out
    * @param env pointer to environment struct
    */
        struct axis2_phase *(AXIS2_CALL *
                get_faultphase)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env);

   /**
    * @param transport_out pointer to transport_out
    * @param env pointer to environment struct
    * @param faultphase pointer to fault phase
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
        axis2_status_t (AXIS2_CALL *
                set_faultphase)(
                    struct axis2_transport_out_desc *transport_out,
                    const axis2_env_t *env,
                    struct axis2_phase *faultphase);

   /**
    * @param transport_out_desc pointer to tarnsport out description
    * @param env pointer to environment struct
    * @param param pointer to param
    * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
    */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

   /**
    * @param transport_out_desc pointer to transport out description
    * @param env pointer to environment struct
    * @param param_name pointer to parameter name
    */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env,
                    axis2_char_t *param_name);

   /**
    * @param transport_out_desc pointer to transport out description
    * @param env pointer to environment struct
    * @param param_name pointer to parameter name
    */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_transport_out_desc_t *transport_out_desc,
                    const axis2_env_t *env,
                    axis2_char_t *param_name);

    };

    /**
     * transport out description struct  
     */
    AXIS2_DECLARE_DATA struct axis2_transport_out_desc
    {
        /** operatoins of transport out description struct  */
        axis2_transport_out_desc_ops_t *ops;
        axis2_param_container_t *param_container;
    };

    /**
     * Creates transport out struct
     * @param env pointer to environment struct
     * @param qname pointer to qname
     * @return pointer to newly created transport out
     */
    AXIS2_EXTERN axis2_transport_out_desc_t * AXIS2_CALL
    axis2_transport_out_desc_create_with_qname (
            const axis2_env_t *env, 
            axis2_qname_t *qname);


    axis2_status_t AXIS2_CALL
    axis2_transport_out_desc_free_void_arg(
            void *transport_out,
            const axis2_env_t *env);
/*************************** Function macros **********************************/

/** Frees the transport out description.
    @sa axis2_transport_out_desc_ops#free */
#define AXIS2_TRANSPORT_OUT_DESC_FREE(transport_out_desc, env) \
      ((transport_out_desc)->ops->free (transport_out_desc, env))

/** Gets the qname.
    @sa axis2_transport_out_desc_ops#get_qname */
#define AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_qname (transport_out_desc, env))

/** Sets the qname.
    @sa axis2_transport_out_desc_ops#set_qname */
#define AXIS2_TRANSPORT_OUT_DESC_SET_QNAME(transport_out_desc, env, qname) \
      ((transport_out_desc)->ops->set_qname (transport_out_desc, env, qname))

/** Gets the out flow.
    @sa axis2_transport_out_desc_ops#get_outflow */
#define AXIS2_TRANSPORT_OUT_DESC_GET_OUTFLOW(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_outflow (transport_out_desc, env))

/** Sets the out flow.
    @sa axis2_transport_out_desc_ops#set_outflow */
#define AXIS2_TRANSPORT_OUT_DESC_SET_OUTFLOW(transport_out_desc, env, outflow) \
      ((transport_out_desc)->ops->set_outflow (transport_out_desc, env, outflow))

/** Gets the fault flow.
    @sa axis2_transport_out_desc_ops#get_faultflow */
#define AXIS2_TRANSPORT_OUT_DESC_GET_FAULTFLOW(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_faultflow (transport_out_desc, env))

/** Sets the fault flow.
    @sa axis2_transport_out_desc_ops#set_faultflow */
#define AXIS2_TRANSPORT_OUT_DESC_SET_FAULTFLOW(transport_out_desc, env, faultflow) \
      ((transport_out_desc)->ops->set_faultflow (transport_out_desc, env, faultflow))

/** Gets the sender.
    @sa axis2_transport_out_desc_ops#get_sender */
#define AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_sender (transport_out_desc, env))

/** Sets the sender.
    @sa axis2_transport_out_desc_ops#set_sender */
#define AXIS2_TRANSPORT_OUT_DESC_SET_SENDER(transport_out_desc, env, sender) \
      ((transport_out_desc)->ops->set_sender (transport_out_desc, env, sender))

/** Gets the out phase.
    @sa axis2_transport_out_desc_ops#get_out_phase */
#define AXIS2_TRANSPORT_OUT_DESC_GET_OUT_PHASE(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_out_phase (transport_out_desc, env))

/** Sets the out phase.
    @sa axis2_transport_out_desc_ops#set_out_phase */
#define AXIS2_TRANSPORT_OUT_DESC_SET_OUT_PHASE(transport_out_desc, env, out_phase) \
      ((transport_out_desc)->ops->set_out_phase (transport_out_desc, env, out_phase))

/** Gets the fault phsae.
    @sa axis2_transport_out_desc_ops#get_faultphase */
#define AXIS2_TRANSPORT_OUT_DESC_GET_FAULTPHASE(transport_out_desc, env) \
      ((transport_out_desc)->ops->get_faultphase (transport_out_desc, env))

/** Sets the fault phase.
    @sa axis2_transport_out_desc_ops#set_faultphase */
#define AXIS2_TRANSPORT_OUT_DESC_SET_FAULTPHASE(transport_out_desc, env, faultphase) \
      ((transport_out_desc)->ops->set_faultphase (transport_out_desc, env, faultphase))

/** Adds the parameter.
    @sa axis2_transport_out_desc_ops#add_param */
#define AXIS2_TRANSPORT_OUT_DESC_ADD_PARAM(transport_out_desc, env, param) \
      ((transport_out_desc)->ops->add_param (transport_out_desc, env, param))

/** Gets the parameter.
    @sa axis2_transport_out_desc_ops#get_param */
#define AXIS2_TRANSPORT_OUT_DESC_GET_PARAM(transport_out_desc, env, param_name) \
      ((transport_out_desc)->ops->get_param (transport_out_desc, env, param_name))

/** Is param locked.
    @sa axis2_transport_out_desc_ops#is_param_locked */
#define AXIS2_TRANSPORT_OUT_DESC_IS_PARAM_LOCKED(transport_out_desc, env, param_name) \
      ((transport_out_desc)->ops->is_param_locked (transport_out_desc, env, param_name))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_TRANSPORT_OUT_DESC_H */
