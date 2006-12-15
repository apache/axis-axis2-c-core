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

#ifndef AXIS2_PHASES_INFO_H
#define AXIS2_PHASES_INFO_H

/** @defgroup axis2_phases_info phases information
 * @ingroup axis2_engine
 * Description
 * @{
 */

/**
 * @file axis2_phases_info.h
 * @brief axis2 phases info interface. 
 */
#include <axis2_const.h>
#include <axis2_error.h>
#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_allocator.h>
#include <axis2_string.h>
#include <axis2_array_list.h>
#include <axis2_op.h>
#include <axis2_phase.h>

#ifdef __cplusplus
extern "C"
{
#endif


    /** Type name for struct axis2_phases_info_ops */
    typedef struct axis2_phases_info_ops axis2_phases_info_ops_t;
    /** Type name for struct axis2_phases_info */
    typedef struct axis2_phases_info axis2_phases_info_t;

    /**
     * Phases Info ops struct
     * Encapsulator struct for ops of axis2_phases_info
     */
    struct axis2_phases_info_ops
    {
        /** Deallocate memory
         * @param pahses_info pointer to phases info
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         * @param in_phases inter to in phases
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_in_phases)(
                    axis2_phases_info_t *phases_info,
                    const axis2_env_t *env,
                    axis2_array_list_t *in_phases);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         * @param out_phases pointer to out phases
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_phases)(
                    axis2_phases_info_t *phases_info,
                    const axis2_env_t *env,
                    axis2_array_list_t *out_phases);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         * @param in_faultphases pionter to in fault phases
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_in_faultphases)(
                    axis2_phases_info_t *phases_info,
                    const axis2_env_t *env,
                    axis2_array_list_t *in_faultphases);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to env
         * @param out_faultphases pointer to out fault phases
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_faultphases)(
                    axis2_phases_info_t *phases_info,
                    const axis2_env_t *env,
                    axis2_array_list_t *out_faultphases);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_in_phases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_out_phases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @parma env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_in_faultphases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_out_faultphases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_op_in_phases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_op_out_phases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info 
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_op_in_faultphases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_op_out_faultphases)(
                    const axis2_phases_info_t *phases_info,
                    const axis2_env_t *env);

        /**
         * @param phases_info pointer to phases info
         * @param env pointer to environment struct
         * @param axis2_opt pointer to axis2 opt
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_op_phases)(
                    axis2_phases_info_t *phases_info,
                    const axis2_env_t *env,
                    struct axis2_op *axis2_opt);

    };

    /**
     * phases info struct
     */
    struct axis2_phases_info
    {
        /** operations of phases info struct */
        axis2_phases_info_ops_t *ops;
    };

    /** create Phases Info struct
     * @param env pointer to environment struct
     * @return pointer to newly created phases info
     */
    AXIS2_EXTERN axis2_phases_info_t *AXIS2_CALL
    axis2_phases_info_create (
        const axis2_env_t *env);

    axis2_array_list_t *AXIS2_CALL
    axis2_phases_info_copy_flow(
        const axis2_env_t *env,
        const axis2_array_list_t *flow_to_copy);
/**************************** Start of function macros ************************/

/** Frees the phases info.
    @sa axis2_phases_info_ops#free */
#define AXIS2_PHASES_INFO_FREE(phases_info, env) \
      ((phases_info)->ops->free (phases_info, env))

/** Sets the in phases.
    @sa axis2_phases_info_ops#set_in_phases */
#define AXIS2_PHASES_INFO_SET_IN_PHASES(phases_info, env, in_phases) \
      ((phases_info)->ops->set_in_phases(phases_info, env, in_phases))

/** Sets the out phases.
    @sa axis2_phases_info_ops#set_out_phases */
#define AXIS2_PHASES_INFO_SET_OUT_PHASES(phases_info, env, out_phases) \
      ((phases_info)->ops->set_out_phases(phases_info, env, out_phases))

/** Sets the in fault phases.
    @sa axis2_phases_info_ops#set_in_faultphases */
#define AXIS2_PHASES_INFO_SET_IN_FAULTPHASES(phases_info, env, in_faultphases) \
      ((phases_info)->ops->set_in_faultphases(phases_info, env, in_faultphases))

/** Sets the out fault phases.
    @sa axis2_phases_info_ops#set_out_faultphases */
#define AXIS2_PHASES_INFO_SET_OUT_FAULTPHASES(phases_info, env, out_faultphases) \
      ((phases_info)->ops->set_out_faultphases(phases_info, env, out_faultphases))

/** Gets the in phases.
    @sa axis2_phases_info_ops#get_in_phases */
#define AXIS2_PHASES_INFO_GET_IN_PHASES(phases_info, env) \
      ((phases_info)->ops->get_in_phases(phases_info, env))

/** Gets the out phases.
    @sa axis2_phases_info_ops#get_out_phases */
#define AXIS2_PHASES_INFO_GET_OUT_PHASES(phases_info, env) \
      ((phases_info)->ops->get_out_phases(phases_info, env))

/** Gets the in fault phases.
    @sa axis2_phases_info_ops#get_in_faultphases */
#define AXIS2_PHASES_INFO_GET_IN_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_in_faultphases(phases_info, env))

/** Gets the out fault phases.
    @sa axis2_phases_info_ops#get_out_faultphases */
#define AXIS2_PHASES_INFO_GET_OUT_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_out_faultphases(phases_info, env))

/** Gets the op in phases.
    @sa axis2_phases_info_ops#get_op_in_phases */
#define AXIS2_PHASES_INFO_GET_OP_IN_PHASES(phases_info, env) \
      ((phases_info)->ops->get_op_in_phases(phases_info, env))

/** Gets the op out phases.
    @sa axis2_phases_info_ops#get_op_out_phases */
#define AXIS2_PHASES_INFO_GET_OP_OUT_PHASES(phases_info, env) \
      ((phases_info)->ops->get_op_out_phases(phases_info, env))

/** Gets the op in fault phases.
    @sa axis2_phases_info_ops#get_op_in_faultphases */
#define AXIS2_PHASES_INFO_GET_OP_IN_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_op_in_faultphases(phases_info, env))

/** Gets the op out fault phases.
    @sa axis2_phases_info_ops#get_op_out_faultphases */
#define AXIS2_PHASES_INFO_GET_OP_OUT_FAULTPHASES(phases_info, env) \
      ((phases_info)->ops->get_op_out_faultphases(phases_info, env))

/** Sets the op phases.
    @sa axis2_phases_info_ops#set_op_phases */
#define AXIS2_PHASES_INFO_SET_OP_PHASES(phases_info, env, axis2_opt) \
      ((phases_info)->ops->set_op_phases(phases_info, env, axis2_opt))

/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif /*AXIS2_PHASES_INFO_H*/
