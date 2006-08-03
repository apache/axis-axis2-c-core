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

#ifndef AXIS2_DISP_CHECKER_H
#define AXIS2_DISP_CHECKER_H

/**
 * @defgroup axis2_disp_checker dispatcher checker
 * @ingroup axis2_engine
 * Description.
 * @{
 */

/**
 * @file axis2_disp_checker.h
 * @brief Axis2 Dispatcher interface
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_handler.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_disp_checker_ops  */
    typedef struct axis2_disp_checker_ops axis2_disp_checker_ops_t;
    /** Type name for struct axis2_disp_checker  */
    typedef struct axis2_disp_checker axis2_disp_checker_t;

    /**
     * dispatcher checker ops struct
     * Encapsulator struct for operations of axis2_disp_checker
     */
    AXIS2_DECLARE_DATA struct axis2_disp_checker_ops
    {
        /**
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         */
        axis2_handler_t* (AXIS2_CALL *
                get_base)(
                    const axis2_disp_checker_t *disp_checker,
                    const axis2_env_t *env);


        /**
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         */
        axis2_qname_t* (AXIS2_CALL *
                get_qname)(
	                const axis2_disp_checker_t *disp_checker,
                    const axis2_env_t *env);

        /**
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         * @param qname pointer to qname
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    struct axis2_disp_checker *disp_checker,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_disp_checker *disp_checker,
                    const axis2_env_t *env);

    };

    /**
     * dispatcher checker struct
     */
    struct axis2_disp_checker
    {
        /** operations of dispatcher checker */
        axis2_disp_checker_ops_t *ops;
    };


    /**
     * Creates a dispatcher checker struct
     * @param env pointer to environment struct
     * @param qname pointer to qname, it can be NULL
     */
    AXIS2_EXTERN axis2_disp_checker_t* AXIS2_CALL
    axis2_disp_checker_create(
        const axis2_env_t *env,
        axis2_qname_t *qname);

/** Gets the base.
    @sa axis2_disp_checker_ops#get_base */
#define AXIS2_DISP_CHECKER_GET_BASE(disp_checker, env) \
      ((disp_checker)->ops->get_base(disp_checker, env))

/** Gets the qname.
    @sa axis2_disp_checker_ops#get_qname */
#define AXIS2_DISP_CHECKER_GET_QNAME(disp_checker, env) \
      ((disp_checker)->ops->get_qname(disp_checker, env))

/** Sets the qname.
    @sa axis2_disp_checker_ops#set_qname */
#define AXIS2_DISP_CHECKER_SET_QNAME(disp_checker, env, name) \
      ((disp_checker)->ops->set_qname(disp_checker, env, name))

/** Frees the dispatcher checker.
    @sa axis2_disp_checker_ops#free */
#define AXIS2_DISP_CHECKER_FREE(disp_checker, env) \
      ((disp_checker)->ops->free(disp_checker, env))


/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_DISP_CHECKER_H */
