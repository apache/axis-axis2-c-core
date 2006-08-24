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

#ifndef AXIS2_PHASE_RULE_H
#define AXIS2_PHASE_RULE_H

/**
 * @defgroup axis2_phase_rule phase rule
 * @ingroup axis2_desc
 * phase rule encapsulates data and operations related to phase rules for a 
 * given handler. phase rule lives within a handler. 
 * phase rules of a handler specify the relative location of the 
 * handler within the phase to which it belongs, with respect to other handlers 
 * in the phase or first and last positions of the handler chain of the phase.
 * @{
 */

/**
 * @file axis2_phase_rule.h
 */

#include <axis2_defines.h>
#include <axis2_qname.h>
#include <axis2_param.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_phase_rule */
    typedef struct axis2_phase_rule axis2_phase_rule_t;
    /** Type name for struct axis2_phase_rule_ops */
    typedef struct axis2_phase_rule_ops axis2_phase_rule_ops_t;


    /**
     * phase rule ops struct.
     * Encapsulator struct for ops of axis2_phase_rule.
     */
    struct axis2_phase_rule_ops
    {
        /**
         * Gets the name of the handler before which the handler associated with 
         * this rule should be placed.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @return name of handler before which the handler should be placed
         */
        const axis2_char_t *(AXIS2_CALL *
                get_before)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * Sets the name of the handler before which the handler associated with 
         * this rule should be placed.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param before name of handler before which the handler should be placed
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_before)(
                    axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env,
                    const axis2_char_t *before);

        /**
         * Gets the name of the handler after which the handler associated with 
         * this rule should be placed.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @return name of handler after which the handler should be placed
         */
        const axis2_char_t *(AXIS2_CALL *
                get_after)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * Sets the name of the handler after which the handler associated with 
         * this rule should be placed.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param after name of handler after which the handler should be placed
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_after)(
                    axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env,
                    const axis2_char_t *after);

        /**
         * Gets name.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @return name string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * Sets name.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param name name string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_name)(
                    axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * Checks if the handler is the first in phase. 
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if the handler associated with this rule is the 
         * first in phase, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_first)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * Sets handler to be the first in phase.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param first AXIS2_TRUE if the handler associated with this rule is the 
         * first in phase, else AXIS2_FALSE
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_first)(
                    axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env,
                    axis2_bool_t first);

        /**
         * Checks if the handler is the last in phase. 
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if the handler associated with this rule is the 
         * last in phase, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_last)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * Sets handler to be the last in phase.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param last AXIS2_TRUE if the handler associated with this rule is the 
         * last in phase, else AXIS2_FALSE
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_last)(
                    axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env,
                    axis2_bool_t last);

        /**
         * Frees phase rule.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * Clones phase rule.
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @return pointer to newly cloned phase rule
         */
        axis2_phase_rule_t *(AXIS2_CALL *
                clone)(
                    axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

    };

    /**
     * phase rule struct.
     */
    struct axis2_phase_rule
    {
        /** phase rule related ops */
        axis2_phase_rule_ops_t *ops;
    };


    /**
     * Creates a phase rule struct instance.
     * @param env pointer to environment struct
     * @param phase_name name of the phase rule 
     * @return pointer to newly created phase rule
     */
    AXIS2_EXTERN axis2_phase_rule_t *AXIS2_CALL
    axis2_phase_rule_create(
        const axis2_env_t *env,
        const axis2_char_t *name);

/** Gets name of the handler before which the handler associated with this rule 
    is to be placed.
    @sa axis2_phase_rule_ops#get_before */
#define AXIS2_PHASE_RULE_GET_BEFORE(phase_rule, env) \
      ((phase_rule)->ops->get_before(phase_rule, env))

/** Sets name of the handler before which the handler associated with this rule 
    is to be placed.
    @sa axis2_phase_rule_ops#set_before */
#define AXIS2_PHASE_RULE_SET_BEFORE(phase_rule, env, before) \
      ((phase_rule)->ops->set_before(phase_rule, env, before))

/** Gets name of the handler after which the handler associated with this rule 
    is to be placed.
    @sa axis2_phase_rule_ops#get_after */
#define AXIS2_PHASE_RULE_GET_AFTER(phase_rule, env) \
      ((phase_rule)->ops->get_after(phase_rule, env))

/** Sets name of the handler after which the handler associated with this rule 
    is to be placed.
    @sa axis2_phase_rule_ops#set_after */
#define AXIS2_PHASE_RULE_SET_AFTER(phase_rule, env, after) \
      ((phase_rule)->ops->set_after(phase_rule, env, after))

/** Gets name.
    @sa axis2_phase_rule_ops#get_name */
#define AXIS2_PHASE_RULE_GET_NAME(phase_rule, env) \
      ((phase_rule)->ops->get_name(phase_rule, env))

/** Sets name.
    @sa axis2_phase_rule_ops#set_name */
#define AXIS2_PHASE_RULE_SET_NAME(phase_rule, env, name)\
      ((phase_rule)->ops->set_name(phase_rule, env, name))

/** Checks if the associated handler is the first in phase.
    @sa axis2_phase_rule_ops#is_first */
#define AXIS2_PHASE_RULE_IS_FIRST(phase_rule, env) \
      ((phase_rule)->ops->is_first(phase_rule, env))

/** Sets the associated handler to be the first in phase.
    @sa axis2_phase_rule_ops#set_first */
#define AXIS2_PHASE_RULE_SET_FIRST(phase_rule, env, first) \
      ((phase_rule)->ops->set_first(phase_rule, env, first))

/** Checks if the associated handler is the last in phase.
    @sa axis2_phase_rule_ops#is_last */
#define AXIS2_PHASE_RULE_IS_LAST(phase_rule, env) \
      ((phase_rule)->ops->is_last(phase_rule, env))

/** Sets the associated handler to be the last in phase.
    @sa axis2_phase_rule_ops#set_last */
#define AXIS2_PHASE_RULE_SET_LAST(phase_rule, env, last) \
      ((phase_rule)->ops->set_last(phase_rule, env, last))

/** Frees phase rule.
    @sa axis2_phase_rule_ops#free */
#define AXIS2_PHASE_RULE_FREE(phase_rule, env) \
      ((phase_rule)->ops->free(phase_rule, env))

/** Clones given phase rule.
    @sa axis2_phase_rule_ops#clone */
#define AXIS2_PHASE_RULE_CLONE(phase_rule, env) \
      ((phase_rule)->ops->clone(phase_rule, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PHASE_RULE_H */
