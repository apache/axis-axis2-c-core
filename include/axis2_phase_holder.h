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

/**
 * @file axis2_phase_holder.h
 * @brief Axis2 Wsdl soap op interface
 */

#include <axis2.h>
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
#include <axis2_wsdl.h>

#ifdef __cplusplus
extern "C"
{
#endif

struct axis2_phase;
struct axis2_handler_desc;
struct axis2_handler;
struct axis2_phase_rule;
typedef struct axis2_phase_holder axis2_phase_holder_t;
typedef struct axis2_phase_holder_ops axis2_phase_holder_ops_t;

/** @defgroup axis2_phase_holder Phase Holder
 * @ingroup axis2_phase
 * @{
 */

/** 
 * @brief Wsdl Phase Holder ops struct
 * Encapsulator struct for ops of axis2_phase_holder
 */
AXIS2_DECLARE_DATA struct axis2_phase_holder_ops
{
	/** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *free)(
                    axis2_phase_holder_t *phase_holder,
			        axis2_env_t **env);
    
    /**
     * Method isPhaseExist
     *
     * @param phaseName
     * @return
     */
    axis2_bool_t (AXIS2_CALL *
    is_phase_exist) (axis2_phase_holder_t *phase_holder, 
                                axis2_env_t **env,
                                axis2_char_t *phase_name);
    
    /**
     * Method addHandler
     *
     * @param handler
     * @throws PhaseException
     */
    axis2_status_t (AXIS2_CALL *
    add_handler) (axis2_phase_holder_t *phase_holder, 
                                    axis2_env_t **env,
                                    struct axis2_handler_desc *handler);
    
    /**
     * this method is used to get the actual phase object given in the phase array list
     *
     * @param phaseName
     * @return
     */
    struct axis2_phase * (AXIS2_CALL *
    get_phase) (axis2_phase_holder_t *phase_holder, 
                                    axis2_env_t **env,
                                    axis2_char_t *phase_name);
    
    /**
     * This method is to build the transport phase , here load the corresponding 
     * handlers and added them
     * in to correct phase
     *
     * @param phase
     * @param handlers
     * @throws PhaseException
     */
    axis2_status_t (AXIS2_CALL *
    build_transport_handler_chain) (axis2_phase_holder_t *phase_holder, 
                                                        axis2_env_t **env,
                                                        struct axis2_phase *phase,
                                                        axis2_array_list_t *handlers);



};

/** 
 * @brief Phase Holder struct  
 */  
AXIS2_DECLARE_DATA struct axis2_phase_holder
{
	axis2_phase_holder_ops_t *ops;
};

/**
 * Creates phase holder struct
 * @return pointer to newly created phase holder
 */
AXIS2_DECLARE(axis2_phase_holder_t *) 
axis2_phase_holder_create (axis2_env_t **env);

/**
 * Creates phase holder struct
 * @param phases
 * @return pointer to newly created phase holder
 */
AXIS2_DECLARE(axis2_phase_holder_t *) 
axis2_phase_holder_create_with_phases (axis2_env_t **env, axis2_array_list_t *phases);

/*************************** Function macros **********************************/

#define AXIS2_PHASE_HOLDER_FREE(phase_holder, env) \
		((phase_holder->ops)->free (phase_holder, env))

#define AXIS2_PHASE_HOLDER_IS_PHASE_EXIST(phase_holder, env, phase_name) \
		((phase_holder->ops)->is_phase_exist (phase_holder, env, phase_name))

#define AXIS2_PHASE_HOLDER_ADD_HANDLER(phase_holder, env, handler) \
		((phase_holder->ops)->add_handler (phase_holder, env, handler))

#define AXIS2_PHASE_HOLDER_GET_PHASE(phase_holder, env, phase_name) \
		((phase_holder->ops)->get_phase (phase_holder, env, phase_name))

#define AXIS2_PHASE_HOLDER_BUILD_TRANSPORT_HANDLER_CHAIN(phase_holder, env, phase, handlers) \
		((phase_holder->ops)->build_transport_handler_chain (phase_holder, env, phase, handlers))       

/*************************** End of function macros ***************************/

/** @} */

#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_PHASE_HOLDER_H */
