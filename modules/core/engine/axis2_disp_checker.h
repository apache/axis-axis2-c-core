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

#ifndef AXIS2_DISP_CHECKER_H
#define AXIS2_DISP_CHECKER_H

/**
 * @defgroup axis2_disp_checker dispatcher checker
 * @ingroup axis2_engine
 * dispatcher checker is responsible of checking the status of the dispatchers.
 * @{
 */

/**
 * @file axis2_disp_checker.h
 */

#include <axis2_defines.h>
#include <axis2_string.h>
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
     * dispatcher checker ops struct.
     * Encapsulator struct for operations of axis2_disp_checker.
     */
     struct axis2_disp_checker_ops
    {
        /**
         * Gets the base handler.
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         * @return pointer to base handler, returns a reference not a cloned copy
         */
        axis2_handler_t *(AXIS2_CALL *
                get_base)(
                    const axis2_disp_checker_t *disp_checker,
                    const axis2_env_t *env);


        /**
         * Gets QName.
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         * @return returns a pointer to the QName, returns a reference not a 
         * cloned copy
         */
        axis2_string_t *(AXIS2_CALL *
                get_name)(
                    const axis2_disp_checker_t *disp_checker,
                    const axis2_env_t *env);

        /**
         * Sets QName.
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         * @param name pointer to QName. A clone would be created within the method
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_name)(
                    axis2_disp_checker_t *disp_checker,
                    const axis2_env_t *env,
                    const axis2_string_t *name);

        /**
         * Frees dispatcher checker.
         * @param disp_checker pointer to dispatcher checker
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_disp_checker_t *disp_checker,
                    const axis2_env_t *env);

    };

    /**
     * dispatcher checker struct.
     */
    struct axis2_disp_checker
    {
        /** operations of dispatcher checker */
        axis2_disp_checker_ops_t *ops;
    };


    /**
     * Creates a dispatcher checker struct instance.
     * @param env pointer to environment struct
     * @return pointer to newly created dispatcher checker struct
     */
    AXIS2_EXTERN axis2_disp_checker_t *AXIS2_CALL
    axis2_disp_checker_create(
        const axis2_env_t *env);
    

/** Gets base handler.
    @sa axis2_disp_checker_ops#get_base */
#define AXIS2_DISP_CHECKER_GET_BASE(disp_checker, env) \
      ((disp_checker)->ops->get_base(disp_checker, env))

/** Gets QName.
    @sa axis2_disp_checker_ops#get_name */
#define AXIS2_DISP_CHECKER_GET_NAME(disp_checker, env) \
      ((disp_checker)->ops->get_name(disp_checker, env))

/** Sets the name.
    @sa axis2_disp_checker_ops#set_name */
#define AXIS2_DISP_CHECKER_SET_NAME(disp_checker, env, name) \
      ((disp_checker)->ops->set_name(disp_checker, env, name))

/** Frees dispatcher checker.
    @sa axis2_disp_checker_ops#free */
#define AXIS2_DISP_CHECKER_FREE(disp_checker, env) \
      ((disp_checker)->ops->free(disp_checker, env))


/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_DISP_CHECKER_H */
