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

#ifndef AXIS2_PHASE_H
#define AXIS2_PHASE_H

/**
 * @file axis2_phase.h
 * @brief Axis2 phase interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_handler.h>
#include <axis2_handler_desc.h>
#include <axis2_array_list.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /** Both Before and After */
    static const int AXIS2_BOTH_BEFORE_AFTER = 0;    
    /** Before */
    static const int AXIS2_BEFORE = 1;    
    /** After */
    static const int AXIS2_AFTER = 2;    
    /** Anywhere */
    static const int AXIS2_ANYWHERE = 3;
    
    struct axis2_phase_ops;
    struct axis2_phase;
    struct axis2_msg_ctx;

/**
 * @defgroup axis2_phase Phases
 * @ingroup axis2_core_phases
 * @{
 */

/**
 *   \brief Phase ops struct
 */
 AXIS2_DECLARE_DATA   typedef struct axis2_phase_ops
    {
        /**
         * adds given handler to the specified position in the phase array lisst
         *
         * @param hander
         * @param index
         */
         axis2_status_t (AXIS2_CALL *add_handler_at)(struct axis2_phase *phase, 
                                      axis2_env_t **env,
                                      int index, 
                                      axis2_handler_t *hander);
        /**
        * add to next empty phase
        *
        * @param phase
        */
        axis2_status_t (AXIS2_CALL *add_handler)(struct axis2_phase *phase, 
                                     axis2_env_t **env,
                                     axis2_handler_t *handler);
        /**
         * If need to see how this works look at the stack!
         *
         * @param msg_ctx
         * @throws org.apache.axis2.AxisFault
         */
        axis2_status_t (AXIS2_CALL *invoke)(struct axis2_phase *phase, 
                                          axis2_env_t **env,
                                          struct axis2_msg_ctx *msg_ctx);
        /**
         * phase name accessor
         * @return returns the phase name.
         */
        axis2_char_t* (AXIS2_CALL *get_name)(struct axis2_phase *phase, 
                                      axis2_env_t **env);
         
        int (AXIS2_CALL *get_handler_count)(struct axis2_phase *phase, 
                                      axis2_env_t **env);
        
        /**
         * Method setPhaseFirst
         *
         * @param first_handler
         * @throws PhaseException
         */
        axis2_status_t (AXIS2_CALL *set_first_handler)(struct axis2_phase *phase, 
                                      axis2_env_t **env, axis2_handler_t * handler);
        /**
         * Method setPhaseLast
         *
         * @param last_handler
         * @throws PhaseException
         */
        axis2_status_t (AXIS2_CALL *set_last_handler)(struct axis2_phase *phase, 
                                      axis2_env_t **env, axis2_handler_t * handler);
        /**
         * Method add_handler
         *
         * @param handler
         * @throws PhaseException
         */
        axis2_status_t (AXIS2_CALL *add_handler_desc)(struct axis2_phase *phase, 
                                      axis2_env_t **env, axis2_handler_desc_t * handler_desc);
        /**
         * Method axis2_phase_insert_before
         *
         * @param handler
         */
        axis2_status_t (AXIS2_CALL *insert_before)(struct axis2_phase *phase, axis2_env_t **env, axis2_handler_t * handler);
        
        /**
         * Method axis2_phase_insert_after
         *
         * @param handler
         */
        axis2_status_t (AXIS2_CALL *insert_after)(struct axis2_phase *phase, axis2_env_t **env, axis2_handler_t * handler);
        
        /**
         * This method assume that both the before and after cant be a same hander
         * That condition is not checked by this function. It should be checked befor calling this function
         *
         * @param handler
         * @throws PhaseException
         */
        axis2_status_t (AXIS2_CALL *insert_before_and_after)(struct axis2_phase *phase, axis2_env_t **env, axis2_handler_t * handler);
        
        axis2_status_t (AXIS2_CALL *insert_handler_desc)(struct axis2_phase *phase, axis2_env_t **env, axis2_handler_desc_t * handler_desc);
        
        /**
         * To get the all the handlers in the phase
         *
         * @return
         */
        axis2_array_list_t* (AXIS2_CALL *get_handlers)(struct axis2_phase *phase, axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *invoke_start_from_handler)(struct axis2_phase *phase, axis2_env_t **env, axis2_qname_t *qname, struct axis2_msg_ctx *msg_ctx);
        axis2_status_t (AXIS2_CALL *free)(struct axis2_phase *phase, 
                                                       axis2_env_t **env);
    } axis2_phase_ops_t;
	
   /** 
    * \brief Phase struct
    */
    typedef struct axis2_phase
    {
        /** Phase related ops */
        axis2_phase_ops_t *ops;
    } axis2_phase_t;


/**
 * creates phase struct
 */
AXIS2_DECLARE(axis2_phase_t*) axis2_phase_create(axis2_env_t **env, const axis2_char_t *phase_name);

#define AXIS2_PHASE_ADD_HANDLER_AT(phase, env, handler) ((phase)->ops->add_handler_at(phase, env, handler))
#define AXIS2_PHASE_ADD_HANDLER(phase, env, handler) ((phase)->ops->add_handler(phase, env, handler))
#define AXIS2_PHASE_ADD_HANDLER_DESC(phase, env, handler_desc) ((phase)->ops->add_handler_desc(phase, env, handler_desc))
#define AXIS2_PHASE_INVOKE(phase, env, msg_ctx) ((phase)->ops->invoke(phase, env, msg_ctx))
#define AXIS2_PHASE_GET_NAME(phase, env) ((phase)->ops->get_name(phase, env))
#define AXIS2_PHASE_GET_HANDLER_COUNT(phase, env) ((phase)->ops->get_handler_count(phase, env))
#define AXIS2_PHASE_SET_FIRST_HANDLER(phase, env, handler) ((phase)->ops->set_first_handler(phase, env, handler))
#define AXIS2_PHASE_SET_LAST_HANDLER(phase, env, handler) ((phase)->ops->set_last_handler(phase, env, handler))
#define AXIS2_PHASE_INSERT_BEFORE(phase, env, handler) ((phase)->ops->insert_before(phase, env, handler))
#define AXIS2_PHASE_INSERT_AFTER(phase, env, handler) ((phase)->ops->insert_after(phase, env, handler))
#define AXIS2_PHASE_INSERT_BEFORE_AND_AFTER(phase, env, handler) ((phase)->ops->insert_before_and_after(phase, env, handler))
#define AXIS2_PHASE_INSERT_HANDLER_DESC(phase, env, handler_desc) ((phase)->ops->insert_handler_desc(phase, env, handler_desc))
#define AXIS2_PHASE_GET_HANDLERS(phase, env) ((phase)->ops->get_handlers(phase, env))
#define AXIS2_PHASE_INVOKE_START_FROM_HANDLER(phase, env, qname, msg_ctx) ((phase)->ops->invoke_start_from_handler(phase, env, qname, msg_ctx))
#define AXIS2_PHASE_FREE(phase, env) ((phase)->ops->free(phase, env))
       
/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PHASE_H */
