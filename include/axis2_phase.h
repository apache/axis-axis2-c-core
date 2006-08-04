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
 * @defgroup axis2_phase phases
 * @ingroup axis2_engine
 * @{
 */

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

/** Both Before and After */
#define AXIS2_PHASE_BOTH_BEFORE_AFTER  0

/** Before */
#define AXIS2_PHASE_BEFORE  1

/** After */
#define AXIS2_PHASE_AFTER  2

/** Anywhere */
#define AXIS2_PHASE_ANYWHERE  3

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_phase_ops axis2_phase_ops_t;
    typedef struct axis2_phase axis2_phase_t;
    struct axis2_msg_ctx;

    /**
     * phase ops struct
     * Encapsulator struct for operations of axis2_phase
     */
    struct axis2_phase_ops
    {
        /**
         * Adds given handler to the specified position in the phase array list.
         * @param phase pointer to phase struct
         * @param env pointer to environment struct
         * @param index index
         * @param handler pointer to hanlder
         */
        axis2_status_t (AXIS2_CALL *
                add_handler_at)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    int index,
                    axis2_handler_t *handler);
        /**
         * add to next empty phase
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param handler pointer to handler 
         */
        axis2_status_t (AXIS2_CALL *
                add_handler)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_t *handler);
         /**
          * If need to see how this works look at the stack!
          * @param phase pointer to phase
          * @param env pointer to environment struct
          * @param msg_ctx pointer to message context
          */
        axis2_status_t (AXIS2_CALL *
                invoke)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);
        /**
         * phase name accessor
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @return returns the phase name.
         */
        axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_phase_t *phase,
                    const axis2_env_t *env);

        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         */
        int (AXIS2_CALL *
                get_handler_count)( 
                    const axis2_phase_t *phase,
                    const axis2_env_t *env);

        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param handler pointer to handler
         */
        axis2_status_t (AXIS2_CALL *
                set_first_handler)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_t *handler);
        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param handler pointer to handler
         */
        axis2_status_t (AXIS2_CALL *
                set_last_handler)(  
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_t *handler);
        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param handler_desc pointer to handler description
         */
        axis2_status_t (AXIS2_CALL *
                add_handler_desc)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_desc_t *handler_desc);
        /**
         * @param phase pointer to phaes
         * @param env pointer to environment struct
         * @param handler pointer to handler
         */
        axis2_status_t (AXIS2_CALL *
                insert_before)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_t *handler);

        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param handler pointer to handler
         */
        axis2_status_t (AXIS2_CALL *
                insert_after)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_t *handler);

        /**
         * This method assume that both the before and after can't be a same 
         * handler . That condition is not checked by this function. 
         * It should be checked befor calling this function
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param handler pointer to handler
         */
        axis2_status_t (AXIS2_CALL *
                insert_before_and_after)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_t *handler);

        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param handler_desc pointer to handler description
         */
        axis2_status_t (AXIS2_CALL *
                insert_handler_desc)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_handler_desc_t *handler_desc);

        /**
         * To get the all the handlers in the phase
         * @param phase pointer to phase
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_handlers)(
                    const axis2_phase_t *phase,
                    const axis2_env_t *env);

        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         * @param qname pointer to qname
         * @param msg_ctx pointer to message context
         */
        axis2_status_t (AXIS2_CALL *
                invoke_start_from_handler)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env,
                    axis2_qname_t *qname,
                    struct axis2_msg_ctx *msg_ctx);


        /**
         * @param phase pointer to phase
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_phase *phase,
                    const axis2_env_t *env);

    };

    /**
     * phase struct
     */
    struct axis2_phase
    {
        /** operations of phase */
        axis2_phase_ops_t *ops;
    };


    /**
     * creates the phase struct
     * @param env pointer to environment struct
     * @param phase_name pointer to phase name
     */
    AXIS2_EXTERN axis2_phase_t *AXIS2_CALL
    axis2_phase_create(
            const axis2_env_t *env,
            const axis2_char_t *phase_name);

/** Add handler at.
    @sa axis2_phase_ops#add_handler_at */
#define AXIS2_PHASE_ADD_HANDLER_AT(phase, env, index, handler) \
      ((phase)->ops->add_handler_at(phase, env, index, handler))

/** Adds the handler.
    @sa axis2_phase_ops#add_handler */
#define AXIS2_PHASE_ADD_HANDLER(phase, env, handler) \
      ((phase)->ops->add_handler(phase, env, handler))

/** Adds the handler description.
    @sa axis2_phase_ops#add_handler_desc */
#define AXIS2_PHASE_ADD_HANDLER_DESC(phase, env, handler_desc) \
      ((phase)->ops->add_handler_desc(phase, env, handler_desc))

/** Invoke.
    @sa axis2_phase_ops#invoke */
#define AXIS2_PHASE_INVOKE(phase, env, msg_ctx) \
       ((phase)->ops->invoke(phase, env, msg_ctx))

/** Gets the name.
    @sa axis2_phase_ops#set_name */
#define AXIS2_PHASE_GET_NAME(phase, env) \
      ((phase)->ops->get_name(phase, env))

/** Gets the handler count.
    @sa axis2_phase_ops#get_handler_count */
#define AXIS2_PHASE_GET_HANDLER_COUNT(phase, env) \
      ((phase)->ops->get_handler_count(phase, env))

/** Sets the first handler.
    @sa axis2_phase_ops#set_first_handler */
#define AXIS2_PHASE_SET_FIRST_HANDLER(phase, env, handler) \
      ((phase)->ops->set_first_handler(phase, env, handler))

/** Sets the last handler.
    @sa axis2_phase_ops#set_last_handler */
#define AXIS2_PHASE_SET_LAST_HANDLER(phase, env, handler) \
      ((phase)->ops->set_last_handler(phase, env, handler))

/** Insert before.
    @sa axis2_phase_ops#insert_before */
#define AXIS2_PHASE_INSERT_BEFORE(phase, env, handler) \
      ((phase)->ops->insert_before(phase, env, handler))

/** Insert after.
    @sa axis2_phase_ops#insert_after */
#define AXIS2_PHASE_INSERT_AFTER(phase, env, handler) \
      ((phase)->ops->insert_after(phase, env, handler))

/** Insert before and after.
    @sa axis2_phase_ops#insert_before_and_after */
#define AXIS2_PHASE_INSERT_BEFORE_AND_AFTER(phase, env, handler) \
      ((phase)->ops->insert_before_and_after(phase, env, handler))

/** Inserts the handler description.
    @sa axis2_phase_ops#insert_handler_desc */
#define AXIS2_PHASE_INSERT_HANDLER_DESC(phase, env, handler_desc) \
      ((phase)->ops->insert_handler_desc(phase, env, handler_desc))

/** Gets the handlers.
    @sa axis2_phase_ops#get_handlers */
#define AXIS2_PHASE_GET_HANDLERS(phase, env) \
      ((phase)->ops->get_handlers(phase, env))

/** Invoke start from handler.
    @sa axis2_phase_ops#invoke_start_from_handler */
#define AXIS2_PHASE_INVOKE_START_FROM_HANDLER(phase, env, qname, msg_ctx) \
      ((phase)->ops->invoke_start_from_handler(phase, env, qname, msg_ctx))

/** Frees the phase.
    @sa axis2_phase_ops#free */
#define AXIS2_PHASE_FREE(phase, env) \
      ((phase)->ops->free(phase, env))

    /** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PHASE_H */
