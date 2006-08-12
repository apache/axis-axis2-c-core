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

#ifndef AXIS2_HANDLER_DESC_H
#define AXIS2_HANDLER_DESC_H

/**
 * @defgroup axis2_handler_desc handler description
 * @ingroup axis2_desc
 * Description
 * @{
 */

/**
 * @file axis2_handler_desc.h
 * @brief Axis2 handler_desc interface
 */

#include <axis2_utils_defines.h>
#include <axis2_qname.h>
#include <axis2_param.h>
#include <axis2_param_container.h>
#include <axis2_phase_rule.h>
#include <axis2_handler.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_handler_desc */
    typedef struct axis2_handler_desc axis2_handler_desc_t;
    /** Type name for struct axis2_handler_desc_ops  */
    typedef struct axis2_handler_desc_ops axis2_handler_desc_ops_t;

    /**
     * Handler Description ops struct
     */
    struct axis2_handler_desc_ops
    {
        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    const axis2_handler_desc_t *handler_desc,
                    const axis2_env_t *env);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param qname pointer to qname
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         */
        axis2_phase_rule_t *(AXIS2_CALL *
                get_rules)(
                    const axis2_handler_desc_t *handler_desc,
                    const axis2_env_t *env);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param phase_rule pointer to phase rule
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_rules)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env,
                    axis2_phase_rule_t *phase_rule);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param name pointer to name
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_handler_desc_t *handler_desc,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param param pointer to param
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_params)(
                    const axis2_handler_desc_t *handler_desc,
                    const axis2_env_t *env);

        /**
         * checks whether the paramter is locked at any levle 
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param param_name pointer to param name
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env, 
                    axis2_char_t *param_name);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         */
        axis2_handler_t *(AXIS2_CALL *
                get_handler)(
                    const axis2_handler_desc_t *handler_desc,
                    const axis2_env_t *env);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param handler pointer to handler
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_handler)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env,
                    axis2_handler_t *handler);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_class_name)(
                    const axis2_handler_desc_t *handler_desc,
                    const axis2_env_t *env);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param class_name pointer to class name
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_class_name)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env,
                    axis2_char_t *class_name);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         */
        axis2_param_container_t *(AXIS2_CALL *
                get_parent)(
                    const axis2_handler_desc_t *handler_desc,
                    const axis2_env_t *env);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @param parent pointer to parent
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env,
                    axis2_param_container_t *parent);

        /**
         * @param axis2_handler_desc_t pointer to handler description
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_handler_desc *handler_desc,
                    const axis2_env_t *env);

    };

    /**
     * Handler Description struct
     */
    struct axis2_handler_desc
    {
        /** Handler Description related ops */
        axis2_handler_desc_ops_t *ops;

        /** param container */
        axis2_param_container_t *param_container;

    };


    /**
     * creates handler_desc struct
     * @param env pointer to env pointer to environment struct
     * @param qname pointer to qname, can be NULL
     */
    AXIS2_EXTERN axis2_handler_desc_t *AXIS2_CALL
    axis2_handler_desc_create_with_qname(
        const axis2_env_t *env, 
        axis2_qname_t *qname);

/** Get handler.
    @sa axis2_handler_desc_ops#get_handler */
#define AXIS2_HANDLER_DESC_GET_HANDLER(handler_desc, env) \
        ((handler_desc)->ops->get_handler(handler_desc, env))

/** Set handler.
    @sa axis2_handler_desc_ops#set_handler */
#define AXIS2_HANDLER_DESC_SET_HANDLER(handler_desc, env, handler) \
        ((handler_desc)->ops->set_handler(handler_desc, env, handler))

/** Get rules.
    @sa axis2_handler_desc_ops#get_rules */
#define AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env) \
        ((handler_desc)->ops->get_rules(handler_desc, env))

/** Set rules.
    @sa axis2_handler_desc_ops#set_rules */
#define AXIS2_HANDLER_DESC_SET_RULES(handler_desc, env, rules) \
        ((handler_desc)->ops->set_rules(handler_desc, env, rules))

/** Get qname.
    @sa axis2_handler_desc_ops#get_qname */
#define AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env) \
        ((handler_desc)->ops->get_qname(handler_desc, env))

/** Set qname.
    @sa axis2_handler_desc_ops#set_qname */
#define AXIS2_HANDLER_DESC_SET_QNAME(handler_desc, env, name) \
        ((handler_desc)->ops->set_qname(handler_desc, env, name))

/** Get param.
    @sa axis2_handler_desc_ops#get_param */
#define AXIS2_HANDLER_DESC_GET_PARAM(handler_desc, env, name) \
        ((handler_desc)->ops->get_param(handler_desc, env, name))

/** Add param.
    @sa axis2_handler_desc_ops#add_param */
#define AXIS2_HANDLER_DESC_ADD_PARAM(handler_desc, env, param) \
        ((handler_desc)->ops->add_param(handler_desc, env, param))

/** Is phase first.
    @sa axis2_handler_desc_ops#is_phase_first */
#define AXIS2_HANDLER_DESC_IS_PHASE_FIRST(handler_desc, env) \
        ((handler_desc)->ops->is_phase_first((handler_desc, env))

/** Get pahse first. 
    @sa axis2_handler_desc_ops#get_phase_first */
#define AXIS2_HANDLER_DESC_GET_PHASE_FIRST(handler_desc, env, phase_first) \
        ((handler_desc)->ops->set_phase_first(handler_desc, env, phase_first))

/** Is phase last.
    @sa axis2_handler_desc_ops#is_past_last */
#define AXIS2_HANDLER_DESC_IS_PHASE_LAST(handler_desc, env) \
        ((handler_desc)->ops->is_phase_last(handler_desc, env))

/** Get phase last.
    @sa axis2_handler_desc_ops#get_phase_last */
#define AXIS2_HANDLER_DESC_GET_PHASE_LAST(handler_desc, env, phase_last) \
        ((handler_desc)->ops->set_phase_last(handler_desc, env, phase_last))

/** Get class name.
    @sa axis2_handler_desc_ops#get_class_name */
#define AXIS2_HANDLER_DESC_GET_CLASS_NAME(handler_desc, env) \
        ((handler_desc)->ops->get_class_name(handler_desc, env))

/** Set class name.
    @sa axis2_handler_desc_ops#set_class_name */
#define AXIS2_HANDLER_DESC_SET_CLASS_NAME(handler_desc, env, class_name) \
        ((handler_desc)->ops->set_class_name(handler_desc, env, class_name))

/** Get parent.
    @sa axis2_handler_desc_ops#get_parent */
#define AXIS2_HANDLER_DESC_GET_PARENT(handler_desc, env) \
        ((handler_desc)->ops->get_parent(handler_desc, env))

/** Set parent.
    @sa axis2_handler_desc_ops#set_parent */
#define AXIS2_HANDLER_DESC_SET_PARENT(handler_desc, env, parent) \
        ((handler_desc)->ops->set_parent(handler_desc, env, parent))

/** Frees the handler description.
    @sa axis2_handler_desc_ops#free */
#define AXIS2_HANDLER_DESC_FREE(handler_desc, env) \
        ((handler_desc)->ops->free(handler_desc, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_HANDLER_DESC_H */
