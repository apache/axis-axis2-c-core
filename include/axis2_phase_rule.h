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
 * @defgroup axis2_phase_rule phase rules
 * @ingroup axis2_desc
 * @{
 */

/**
 * @file axis2_phase_rule.h
 * @brief Axis2 phase_rule interface
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
     * Phase Rule ops struct
     */
    struct axis2_phase_rule_ops
    {
        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_before)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param before before 
         */
        axis2_status_t (AXIS2_CALL *
                set_before)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env,
                    const axis2_char_t *before);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_after)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param after after
         */
        axis2_status_t (AXIS2_CALL *
                set_after)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env,
                    const axis2_char_t *after);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_phase_rule_t *phase_rule,
                    const axis2_env_t *env);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param name pointer to name
         */
        axis2_status_t (AXIS2_CALL *
                set_name)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_phase_first)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param phase_first phase first 
         */
        axis2_status_t (AXIS2_CALL *
                set_phase_first)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env,
                    axis2_bool_t phase_first);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_phase_last)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         * @param phase_last phsae last
         */
        axis2_status_t (AXIS2_CALL *
                set_phase_last)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env,
                    axis2_bool_t phase_last);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env);

        /**
         * @param phase_rule pointer to phase rule
         * @param env pointer to environment struct
         */
        struct axis2_phase_rule *(AXIS2_CALL *
                clone)(
                    struct axis2_phase_rule *phase_rule,
                    const axis2_env_t *env);

    };

    /**
     * Phase Rule struct
     */
    struct axis2_phase_rule
    {
        /** Phase Rule related ops */
        axis2_phase_rule_ops_t *ops;
    };


    /**
     * creates phase_rule struct
     * @param env pointer to environment struct
     * @param phase_name pointer to phase name
     */
    AXIS2_EXTERN axis2_phase_rule_t *AXIS2_CALL
    axis2_phase_rule_create(
        const axis2_env_t *env,
        const axis2_char_t *phase_name);

/** Get before.
    @sa axis2_phase_rule_ops#get_before */
#define AXIS2_PHASE_RULE_GET_BEFORE(phase_rule, env) \
      ((phase_rule)->ops->get_before(phase_rule, env))

/** Set before.
    @sa axis2_phase_rule_ops#set_before */
#define AXIS2_PHASE_RULE_SET_BEFORE(phase_rule, env, before) \
      ((phase_rule)->ops->set_before(phase_rule, env, before))

/** Get after.
    @sa axis2_phase_rule_ops#get_after */
#define AXIS2_PHASE_RULE_GET_AFTER(phase_rule, env) \
      ((phase_rule)->ops->get_after(phase_rule, env))

/** Set after.
    @sa axis2_phase_rule_ops#set_after */
#define AXIS2_PHASE_RULE_SET_AFTER(phase_rule, env, after) \
      ((phase_rule)->ops->set_after(phase_rule, env, after))

/** Get name.
    @sa axis2_phase_rule_ops#get_name */
#define AXIS2_PHASE_RULE_GET_NAME(phase_rule, env) \
      ((phase_rule)->ops->get_name(phase_rule, env))

/** Set name.
    @sa axis2_phase_rule_ops#set_name */
#define AXIS2_PHASE_RULE_SET_NAME(phase_rule, env, name)\
      ((phase_rule)->ops->set_name(phase_rule, env, name))

/** Is phase first.
    @sa axis2_phase_rule_ops#is_phase_first */
#define AXIS2_PHASE_RULE_IS_PHASE_FIRST(phase_rule, env) \
      ((phase_rule)->ops->is_phase_first(phase_rule, env))

/** Set phase first.
    @sa axis2_phase_rule_ops#set_phase_first */
#define AXIS2_PHASE_RULE_SET_PHASE_FIRST(phase_rule, env, phase_first) \
      ((phase_rule)->ops->set_phase_first(phase_rule, env, phase_first))

/** Is phase last.
    @sa axis2_phase_rule_ops#is_phase_last */
#define AXIS2_PHASE_RULE_IS_PHASE_LAST(phase_rule, env) \
      ((phase_rule)->ops->is_phase_last(phase_rule, env))

/** Set phase last.
    @sa axis2_phase_rule_ops#set_phase_last */
#define AXIS2_PHASE_RULE_SET_PHASE_LAST(phase_rule, env, phase_last) \
      ((phase_rule)->ops->set_phase_last(phase_rule, env, phase_last))

/** Frees the phase rule.
    @sa axis2_phase_rule_ops#free */
#define AXIS2_PHASE_RULE_FREE(phase_rule, env) \
      ((phase_rule)->ops->free(phase_rule, env))

/** Clone.
    @sa axis2_phase_rule_ops#clone */
#define AXIS2_PHASE_RULE_CLONE(phase_rule, env) \
      ((phase_rule)->ops->clone(phase_rule, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_PHASE_RULE_H */
