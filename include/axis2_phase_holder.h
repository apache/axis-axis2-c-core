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

#ifndef AXIS2_PHASE_HOLDER_H
#define AXIS2_PHASE_HOLDER_H

/** @defgroup axis2_phase_holder phase holder
 * @ingroup axis2_phase_resolver
 * Description
 * @{
 */

/**
 * @file axis2_phase_holder.h
 * @brief Axis2 Wsdl soap op interface
 */

#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_qname.h>
#include <axis2_array_list.h>
#include <axis2_handler_desc.h>
#include <axis2_phase.h>
#include <axis2_phase_rule.h>
#include <axis2_handler.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_phase;
    struct axis2_handler_desc;
    struct axis2_handler;
    struct axis2_phase_rule;
    /** Type name for struct axis2_phase_holder */
    typedef struct axis2_phase_holder axis2_phase_holder_t;
    /** Type name for struct axis2_phase_holder_ops  */
    typedef struct axis2_phase_holder_ops axis2_phase_holder_ops_t;

    /**
     * Wsdl Phase Holder ops struct
     * Encapsulator struct for ops of axis2_phase_holder
     */
    AXIS2_DECLARE_DATA struct axis2_phase_holder_ops
    {
        /** De-allocate memory
          * @param phase_holder pointer to phase holder
	  * @param env pointer to environment struct
          */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_phase_holder_t *phase_holder,
                    const axis2_env_t *env);

        /**
         * Method isPhaseExist
         * @param phase_holder pointer to phase holder
	 * @param env pointer to environment struct
	 * @param phase_name pointer to phase name
         * @return
         */
        axis2_bool_t (AXIS2_CALL *
                is_phase_exist)(
                    axis2_phase_holder_t *phase_holder,
                    const axis2_env_t *env,
                    axis2_char_t *phase_name);

        /**
         * @param phase_holder pointer to phase holder
	 * @param env pointer to environment struct
	 * @para handler pointer to handler
         */
        axis2_status_t (AXIS2_CALL *
                add_handler)(
                    axis2_phase_holder_t *phase_holder,
                    const axis2_env_t *env,
                    struct axis2_handler_desc *handler);

        /**
         * this method is used to get the actual phase object given in the phase
         * array list
	 * @param phase_holder pointer to phase holder
	 * @param env pointer to environment struct
	 * @param phase_name pointer to phase name
         */
        struct axis2_phase * (AXIS2_CALL *
                get_phase)(
                    axis2_phase_holder_t *phase_holder,
                    const axis2_env_t *env,
                    axis2_char_t *phase_name);

        /**
         * This method is to build the transport phase , here load the corresponding 
         * handlers and added them
         * in to correct phase
	 * @param phase_holder pointer to phase holder
	 * @param env pointer to environment struct
	 * @param phase pointer to phase
	 * @param handlers pointer to handlers
         */
        axis2_status_t (AXIS2_CALL *
                build_transport_handler_chain)(
                    axis2_phase_holder_t *phase_holder,
                    const axis2_env_t *env,
                    struct axis2_phase *phase,
                    axis2_array_list_t *handlers);



    };

    /**
     * phase holder struct  
     */
    AXIS2_DECLARE_DATA struct axis2_phase_holder
    {
        /** operations of phase holder struct */
        axis2_phase_holder_ops_t *ops;
    };

    /**
     * Creates phase holder struct
     * @param env pointer to environment struct
     * @return pointer to newly created phase holder
     */
    AXIS2_EXTERN axis2_phase_holder_t * AXIS2_CALL
            axis2_phase_holder_create (
                const axis2_env_t *env);

    /**
     * Creates phase holder struct
     * @param env pointer to environment struct
     * @param phases pointer to phases
     * @return pointer to newly created phase holder
     */
    AXIS2_EXTERN axis2_phase_holder_t * AXIS2_CALL
            axis2_phase_holder_create_with_phases (
                const axis2_env_t *env,
                axis2_array_list_t *phases);

/*************************** Function macros **********************************/

/** Frees the phase holder.
    @sa axis2_phase_holder_ops#free */
#define AXIS2_PHASE_HOLDER_FREE(phase_holder, env) \
      ((phase_holder)->ops->free (phase_holder, env))

/** Is phase exist.
    @sa axis2_phase_holder_ops#is_phase_exist */
#define AXIS2_PHASE_HOLDER_IS_PHASE_EXIST(phase_holder, env, phase_name) \
      ((phase_holder)->ops->is_phase_exist (phase_holder, env, phase_name))

/** Add handler.
    @sa axis2_phase_holder_ops#add_handler */
#define AXIS2_PHASE_HOLDER_ADD_HANDLER(phase_holder, env, handler) \
      ((phase_holder)->ops->add_handler (phase_holder, env, handler))

/** Gets the phase.
    @sa axis2_phase_holder_ops#get_phase */
#define AXIS2_PHASE_HOLDER_GET_PHASE(phase_holder, env, phase_name) \
      ((phase_holder)->ops->get_phase (phase_holder, env, phase_name))

/** Build transport handler chain.
    @sa axis2_phase_holder_ops#build_transport_handler_chain */
#define AXIS2_PHASE_HOLDER_BUILD_TRANSPORT_HANDLER_CHAIN(phase_holder, env, phase, handlers) \
      ((phase_holder)->ops->build_transport_handler_chain (phase_holder, env, phase, handlers))

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_PHASE_HOLDER_H */
