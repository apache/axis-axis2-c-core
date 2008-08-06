
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

#include <axutil_string.h>
#include <axis2_phase.h>
#include <axutil_array_list.h>
#include <axis2_msg_ctx.h>
#include <axis2_const.h>

static axis2_status_t axis2_phase_add_unique(
    const axutil_env_t * env,
    axutil_array_list_t * list,
    axis2_handler_t * handler);

static axis2_status_t axis2_phase_remove_unique(
    const axutil_env_t * env,
    axutil_array_list_t * list,
    axis2_handler_t * handler);

struct axis2_phase
{

    /** phase name */
    axis2_char_t *name;

    /** array list of handlers */
    axutil_array_list_t *handlers;

    /** first handler of phase */
    axis2_handler_t *first_handler;

    /** first handler of phase set? */
    axis2_bool_t first_handler_set;

    /** last handler of phase */
    axis2_handler_t *last_handler;

    /** last handler of phase set? */
    axis2_bool_t last_handler_set;

    /**
     * handler_first and handler_last are the same handler
     * that is for this phase there is only one handler
     */
    axis2_bool_t is_one_handler;

    int ref;
};

AXIS2_EXTERN axis2_phase_t *AXIS2_CALL
axis2_phase_create(
    const axutil_env_t * env,
    const axis2_char_t * phase_name)
{
    axis2_phase_t *phase = NULL;

    phase = AXIS2_MALLOC(env->allocator, sizeof(axis2_phase_t));
    if (!phase)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "No memory");
        return NULL;
    }

    phase->name = NULL;
    phase->handlers = NULL;
    phase->first_handler = NULL;
    phase->first_handler_set = AXIS2_FALSE;
    phase->last_handler = NULL;
    phase->last_handler_set = AXIS2_FALSE;
    phase->is_one_handler = AXIS2_FALSE;
    phase->ref = 1;

    phase->handlers = axutil_array_list_create(env, 10);
    if (!(phase->handlers))
    {

        /** error is already set by last method array list container create */
        axis2_phase_free(phase, env);
        return NULL;
    }

    if (phase_name)
    {
        phase->name = axutil_strdup(env, phase_name);
        if (!(phase->name))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "No memory");
            axis2_phase_free(phase, env);
            return NULL;
        }
    }

    return phase;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_add_handler_at(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    const int index,
    axis2_handler_t * handler)
{
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "axis2_handler_t *%s added to the index %d of the phase %s",
                    axutil_string_get_buffer(axis2_handler_get_name
                                             (handler, env), env), index,
                    phase->name);

    return axutil_array_list_add_at(phase->handlers, env, index, handler);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_add_handler(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    AXIS2_LOG_INFO(env->log, "Handler %s added to phase %s",
                   axutil_string_get_buffer(axis2_handler_get_name
                                            (handler, env), env), phase->name);

    return axis2_phase_add_unique(env, phase->handlers, handler);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_remove_handler(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    AXIS2_LOG_INFO(env->log, "Handler %s romoved from phase %s",
                   axutil_string_get_buffer(axis2_handler_get_name
                                            (handler, env), env), phase->name);

    return axis2_phase_remove_unique(env, phase->handlers, handler);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_invoke(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx)
{
    int index = 0,
        size = 0;
    axis2_status_t status = AXIS2_SUCCESS;
    
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Entry:axis2_phase_invoke");
    axis2_msg_ctx_set_paused_phase_name(msg_ctx, env, phase->name);
    if (phase->first_handler)
    {
        if (axis2_msg_ctx_is_paused(msg_ctx, env))
        {
            AXIS2_LOG_INFO(env->log, "Message context is paused in the phase %s", 
                phase->name);
            return AXIS2_SUCCESS;
        }
        else
        {
            const axis2_char_t *handler_name = axutil_string_get_buffer(
                axis2_handler_get_name(phase->first_handler, env), env);
            AXIS2_LOG_INFO(env->log,
                "Invoke the first handler %s within the phase %s", handler_name, 
                    phase->name);

            status = axis2_handler_invoke(phase->first_handler, env, msg_ctx);
            if (!status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Handler %s invoke failed within phase %s", handler_name, 
                        phase->name);
                return status;
            }
        }
    }
    /* Invoking the rest of handlers except first_handler and last_handler */
    size = axutil_array_list_size(phase->handlers, env);
    while (index < size)
    {
        if (axis2_msg_ctx_is_paused(msg_ctx, env))
        {
            break;
        }
        else
        {
            axis2_handler_t *handler = (axis2_handler_t *)
                axutil_array_list_get(phase->handlers, env, index);
            if (handler)
            {
                const axis2_char_t *handler_name = axutil_string_get_buffer(
                    axis2_handler_get_name(handler, env), env);
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "Invoke the handler %s within the phase %s", handler_name,
                        phase->name);

                /* Test code. This is used when valgrind is used to find leaks in Axis2/C modules.*/
                /*if(!axutil_strcmp(handler_name, "SandeshaGlobalInHandler") || !axutil_strcmp(
                            handler_name, "SandeshaInHandler"))
                {
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                        "dam_handler_name %s. dam_phase_name %s", handler_name, phase->name);
                    if(!axutil_strcmp(handler_name, "SandeshaGlobalInHandler"))
                    {
                        status = sandesha2_global_in_handler_invoke(phase->first_handler, env, msg_ctx);
                    }
                    if(!axutil_strcmp(handler_name, "SandeshaInHandler"))
                    {
                        status = sandesha2_in_handler_invoke(phase->first_handler, env, msg_ctx);
                    }
                }
                else*/
                    status = axis2_handler_invoke(handler, env, msg_ctx);
                if (!status)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                        "Handler %s invoke failed within phase %s", 
                            handler_name, phase->name);
                    return status;
                }
                /* index increment should be done after the invoke function. If the invocation
                   failed this handler is taken care of and no need to revoke again */
                index++;
                axis2_msg_ctx_set_current_handler_index(msg_ctx, env, index);
            }
        }
    }

    /* If phase last handler is there invoke it here */
    if (phase->last_handler)
    {
        if (axis2_msg_ctx_is_paused(msg_ctx, env))
        {
            AXIS2_LOG_INFO(env->log, "Message context is paused in the phase %s", 
                phase->name);
            return AXIS2_SUCCESS;
        }
        else
        {
            const axis2_char_t *handler_name = axutil_string_get_buffer(
                axis2_handler_get_name(phase->last_handler, env), env);
            AXIS2_LOG_INFO(env->log,
                "Invoke the last handler %s within the phase %s", handler_name, 
                phase->name);
            
            status = axis2_handler_invoke(phase->last_handler, env, msg_ctx);
            if (!status)
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Handler %s invoke failed within  phase %s", handler_name, 
                        phase->name);
                return status;
            }
        }
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "Exit:axis2_phase_invoke");
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
axis2_phase_get_name(
    const axis2_phase_t * phase,
    const axutil_env_t * env)
{
    return phase->name;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_phase_get_handler_count(
    const axis2_phase_t * phase,
    const axutil_env_t * env)
{
    return axutil_array_list_size(phase->handlers, env);
}

AXIS2_EXTERN int AXIS2_CALL
_axis2_phase_get_before_after(
    axis2_handler_t * handler,
    const axutil_env_t * env)
{
    const axis2_char_t *before = NULL,
        *after = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    axis2_phase_rule_t *rules = NULL;
    const axis2_char_t *name = 
        axutil_string_get_buffer(axis2_handler_get_name(handler, env), env);

    handler_desc = axis2_handler_get_handler_desc(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler description is not set for the Handler %s", name);
        return AXIS2_FAILURE;
    }

    rules = axis2_handler_desc_get_rules(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler rules are not set for the Handler Description %s", name);
        return AXIS2_FAILURE;
    }

    before = axis2_phase_rule_get_before(rules, env);
    after = axis2_phase_rule_get_after(rules, env);

    if (before && after)
    {
        if (!axutil_strcmp(before, after))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BEFORE_AFTER_HANDLERS_SAME,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Both before and after handlers cannot be the same");
            return AXIS2_FAILURE;
        }
        return AXIS2_PHASE_BOTH_BEFORE_AFTER;
    }
    else if (before)
    {
        return AXIS2_PHASE_BEFORE;
    }
    else if (after)
    {
        return AXIS2_PHASE_AFTER;
    }
    else
    {
        return AXIS2_PHASE_ANYWHERE;
    }
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_set_first_handler(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    const axis2_char_t *handler_name = 
        axutil_string_get_buffer(axis2_handler_get_name(handler, env), env);
    const axis2_char_t *phase_name = axis2_phase_get_name(phase, env);
    if (phase->first_handler_set)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_PHASE_FIRST_HANDLER_ALREADY_SET,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "First handler of phase already set, so cannot set handler %s "\
            "in to the phase %s as first handler", handler_name, phase_name);
        return AXIS2_FAILURE;
    }
    else
    {
        if (_axis2_phase_get_before_after(handler, env) != AXIS2_PHASE_ANYWHERE)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_PHASE_FIRST_HANDLER,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Invalid first handler %s set for the Phase %s", handler_name, 
                phase_name);
            return AXIS2_FAILURE;
        }
        phase->first_handler = handler;
        phase->first_handler_set = AXIS2_TRUE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_set_last_handler(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    const axis2_char_t *handler_name = 
        axutil_string_get_buffer(axis2_handler_get_name(handler, env), env);
    const axis2_char_t *phase_name = axis2_phase_get_name(phase, env);
    if (phase->last_handler_set)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_PHASE_LAST_HANDLER_ALREADY_SET,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Last handler of phase already set, so cannot set handler %s "\
            "in to the phase %s as last handler", handler_name, phase_name);
        return AXIS2_FAILURE;
    }
    else
    {
        if (_axis2_phase_get_before_after(handler, env) != AXIS2_PHASE_ANYWHERE)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_PHASE_LAST_HANDLER,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                "Invalid last handler %s set for the Phase %s", handler_name, 
                phase_name);
            return AXIS2_FAILURE;
        }
        phase->last_handler = handler;
        phase->last_handler_set = AXIS2_TRUE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_add_handler_desc(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_desc_t * handler_desc)
{
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_t *handler = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_bool_t first = AXIS2_FALSE,
        last = AXIS2_FALSE;
    const axis2_char_t *handler_desc_name = 
        axutil_string_get_buffer(axis2_handler_desc_get_name(handler_desc, env), 
            env);
    if (phase->is_one_handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_PHASE_ADD_HANDLER_INVALID,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Only one handler allowed for phase %s, adding handler %s is not "\
            "allowed", phase->name, handler_desc_name);
        return AXIS2_FAILURE;
    }
    else
    {
        rules = axis2_handler_desc_get_rules(handler_desc, env);
        if (!rules)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler rules are not set for the Hanlder Description %s "\
                    "within phase %s", handler_desc_name, phase->name);
            return AXIS2_FAILURE;
        }

        first = axis2_phase_rule_is_first(rules, env);
        last = axis2_phase_rule_is_last(rules, env);

        if (first && last)
        {
            if (axutil_array_list_size(phase->handlers, env) > 0)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_RULES,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Invalid handler rules, so unable to add handler %s to "\
                        "phase %s", handler_desc_name, phase->name);
                return AXIS2_FAILURE;
            }

            handler = axis2_handler_desc_get_handler(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler is not set for the Handler Description %s "\
                        "within phase %s", handler_desc_name, phase->name);
                return AXIS2_FAILURE;
            }

            /*status = axutil_array_list_add(phase->handlers, env, handler); */
            status = axis2_phase_add_unique(env, phase->handlers, handler);
            if (status)
                phase->is_one_handler = AXIS2_TRUE;
            return status;
        }
        else if (first)
        {
            handler = axis2_handler_desc_get_handler(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler is not set for the Handler Description %s "\
                        "within phase %s", handler_desc_name, phase->name);
                return AXIS2_FAILURE;
            }
            return axis2_phase_set_first_handler(phase, env, handler);
        }
        else if (last)
        {
            handler = axis2_handler_desc_get_handler(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler is not set for the Handler Description %s "\
                        "within phase %s", handler_desc_name, phase->name);
                return AXIS2_FAILURE;
            }
            return axis2_phase_set_last_handler(phase, env, handler);
        }
        else
        {
            handler = axis2_handler_desc_get_handler(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler is not set for the Handler Description %s "\
                        "within phase %s", handler_desc_name, phase->name);
                return AXIS2_FAILURE;
            }

            return axis2_phase_insert_handler_desc(phase, env, handler_desc);
        }
    }
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
_axis2_phase_is_valid_before(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    const axis2_char_t *first_handler_name = NULL,
        *before = NULL;

    if (phase->first_handler)
    {
        handler_desc =
            axis2_handler_get_handler_desc(phase->first_handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;

        first_handler_name =
            axutil_string_get_buffer(axis2_handler_desc_get_name
                                     (handler_desc, env), env);

        if (!first_handler_name)
            return AXIS2_TRUE;

        handler_desc = axis2_handler_get_handler_desc(handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;

        rules = axis2_handler_desc_get_rules(handler_desc, env);
        if (!rules)
            return AXIS2_TRUE;

        before = axis2_phase_rule_get_before(rules, env);
        if (!before)
            return AXIS2_TRUE;

        if (!axutil_strcmp(first_handler_name, before))
            return AXIS2_FALSE;
        else
            return AXIS2_TRUE;

    }
    return AXIS2_TRUE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
_axis2_phase_is_valid_after(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    const axis2_char_t *last_handler_name = NULL,
        *after = NULL;

    if (phase->last_handler)
    {
        handler_desc = axis2_handler_get_handler_desc(phase->last_handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;

        last_handler_name =
            axutil_string_get_buffer(axis2_handler_desc_get_name
                                     (handler_desc, env), env);

        if (!last_handler_name)
            return AXIS2_TRUE;

        handler_desc = axis2_handler_get_handler_desc(handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;

        rules = axis2_handler_desc_get_rules(handler_desc, env);
        if (!rules)
            return AXIS2_TRUE;

        after = axis2_phase_rule_get_after(rules, env);
        if (!after)
            return AXIS2_TRUE;

        if (!axutil_strcmp(last_handler_name, after))
            return AXIS2_FALSE;
        else
            return AXIS2_TRUE;

    }
    return AXIS2_TRUE;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_insert_before(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    const axis2_char_t *handler_name = NULL,
        *before = NULL;
    int i = 0;
    int size = 0;
    const axis2_char_t *name = 
        axutil_string_get_buffer(axis2_handler_get_name(handler, env), env);
    const axis2_char_t *handler_desc_name = NULL;

    handler_desc = axis2_handler_get_handler_desc(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler description is not set in the handler %s within phase %s", 
                name, phase->name);
        return AXIS2_FAILURE;
    }
    handler_desc_name = axutil_string_get_buffer(axis2_handler_desc_get_name(
        handler_desc, env), env);

    rules = axis2_handler_desc_get_rules(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Handler rules are not set in the handler description %s within "\
                "phase %s", handler_desc_name, phase->name);
        return AXIS2_FAILURE;
    }

    before = axis2_phase_rule_get_before(rules, env);
    if (!before)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Rule `before` is not set in the handler rules for handler %s "\
                "within phase %s", name, phase->name);
        return AXIS2_FAILURE;
    }

    if (phase->last_handler)
    {
        const axis2_char_t *last_handler_name = 
            axutil_string_get_buffer(axis2_handler_get_name(phase->last_handler, 
                env), env);
        handler_desc = axis2_handler_get_handler_desc(phase->last_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler description is not set in the last handler %s of "\
                    "phase %s", last_handler_name, phase->name);
            return AXIS2_FAILURE;
        }

        handler_name =
            axutil_string_get_buffer(axis2_handler_desc_get_name
                                     (handler_desc, env), env);
        if (!handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler name is not set in the handler description for "\
                    "last handler %s within phase %s", last_handler_name, 
                        phase->name);
            return AXIS2_FAILURE;
        }

        if (!axutil_strcmp(before, handler_name))
        {
            /*return axutil_array_list_add(phase->handlers, env, handler); */
            return axis2_phase_add_unique(env, phase->handlers, handler);
        }
    }

    size = axutil_array_list_size(phase->handlers, env);

    for (i = 0; i < size; i++)
    {
        axis2_handler_t *temp_handler =
            (axis2_handler_t *) axutil_array_list_get(phase->handlers, env, i);
        if (temp_handler)
        {
            const axis2_char_t *temp_handler_name = 
                axutil_string_get_buffer(axis2_handler_get_name(temp_handler, 
                    env), env);
            handler_desc = axis2_handler_get_handler_desc(temp_handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler Description is not set for the Handler %s "\
                        "within phase %s", temp_handler_name, phase->name);
                return AXIS2_FAILURE;
            }

            handler_name =
                axutil_string_get_buffer(axis2_handler_desc_get_name
                                         (handler_desc, env), env);
            if (!handler_name)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler name is not set in the handler description for "\
                        "handler %s within phase %s", temp_handler_name, 
                            phase->name);
                return AXIS2_FAILURE;
            }

            if (!axutil_strcmp(before, handler_name))
            {
                return axutil_array_list_add_at(phase->handlers, env, i,
                                                handler);
            }
        }
    }
    /* add as the last handler */
    /* return axutil_array_list_add(phase->handlers, env, handler); */
    return axis2_phase_add_unique(env, phase->handlers, handler);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_insert_after(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    const axis2_char_t *handler_name = NULL;
    const axis2_char_t *after = NULL;
    int i = 0;
    int size = 0;
    const axis2_char_t *name = axutil_string_get_buffer(
        axis2_handler_get_name(handler, env), env);
    const axis2_char_t *handler_desc_name = NULL;

    handler_desc = axis2_handler_get_handler_desc(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler description is not set in the handler %s within phase %s",
                name, phase->name);
        return AXIS2_FAILURE;
    }
    handler_desc_name = axutil_string_get_buffer(
        axis2_handler_desc_get_name(handler_desc, env), env);
    rules = axis2_handler_desc_get_rules(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Handler rules are not set in the handler description %s within "\
                "phase %s", handler_desc_name, phase->name);
        return AXIS2_FAILURE;
    }

    after = axis2_phase_rule_get_after(rules, env);
    if (!after)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Rule `after` is not set in the handler rules for handler desc "\
                "%s within phase %s", handler_desc_name, phase->name);
        return AXIS2_FAILURE;
    }

    if (phase->first_handler)
    {
        const axis2_char_t *first_handler_name = axutil_string_get_buffer(
            axis2_handler_get_name(phase->first_handler, env), env);
        handler_desc =
            axis2_handler_get_handler_desc(phase->first_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler description is not set in the first handler %s "\
                    "within phase %s", first_handler_name, phase->name);
            return AXIS2_FAILURE;
        }

        handler_name =
            axutil_string_get_buffer(axis2_handler_desc_get_name
                                     (handler_desc, env), env);
        if (!handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler name is not set in the handler description for "\
                    "handler %s within phase %s", name, phase->name);
            return AXIS2_FAILURE;
        }

        if (!axutil_strcmp(after, handler_name))
        {
            return axutil_array_list_add_at(phase->handlers, env, 0, handler);
        }
    }

    size = axutil_array_list_size(phase->handlers, env);

    for (i = 0; i < size; i++)
    {
        axis2_handler_t *temp_handler =
            (axis2_handler_t *) axutil_array_list_get(phase->handlers, env, i);
        if (temp_handler)
        {
            const axis2_char_t *temp_handler_name = axutil_string_get_buffer(
                axis2_handler_get_name(temp_handler, env), env);
            handler_desc = axis2_handler_get_handler_desc(temp_handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler description is not set in the handler %s within"\
                        " phase %s", temp_handler_name, phase->name);
                return AXIS2_FAILURE;
            }

            handler_name =
                axutil_string_get_buffer(axis2_handler_desc_get_name
                                         (handler_desc, env), env);
            if (!handler_name)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler name is not set in the handler description %s "\
                        "within phase %s", temp_handler_name, phase->name);
                return AXIS2_FAILURE;
            }

            if (!axutil_strcmp(after, handler_name))
            {
                if (i == (size - 1))
                {
                    return axis2_phase_add_unique(env, phase->handlers,
                                                  handler);
                }
                else
                    return axutil_array_list_add_at(phase->handlers, env, i + 1,
                                                    handler);
            }
        }
    }

    if (size > 0)
        return axutil_array_list_add_at(phase->handlers, env, 0, handler);
    else
    {
        return axis2_phase_add_unique(env, phase->handlers, handler);
    }
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_insert_before_and_after(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_t * handler)
{
    int before = -1;
    int after = -1;
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    const axis2_char_t *before_handler_name = NULL,
        *after_handler_name = NULL,
        *after_name = NULL,
        *before_name = NULL,
        *handler_name = NULL;
    int i = 0;
    int size = 0;
    const axis2_char_t *name = axutil_string_get_buffer(
        axis2_handler_get_name(handler, env), env);
    const axis2_char_t *handler_desc_name = NULL;

    handler_desc = axis2_handler_get_handler_desc(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler description is not set in the handler %s within phase %s", 
                name, phase->name);
        return AXIS2_FAILURE;
    }
    handler_desc_name = axutil_string_get_buffer(
        axis2_handler_desc_get_name(handler_desc, env), env);

    rules = axis2_handler_desc_get_rules(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Handler rules are not set in the handler description %s within "\
                "phase %s", handler_desc_name, phase->name);
        return AXIS2_FAILURE;
    }

    before_name = axis2_phase_rule_get_before(rules, env);
    if (!before_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Rule `before` is not set in the handler rules for handler desc "\
            "   %s within phase %s", handler_desc_name, phase->name);
        return AXIS2_FAILURE;
    }

    after_name = axis2_phase_rule_get_after(rules, env);
    if (!after_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "Rule `after` is not set in the handler rules for handler desc "\
                "%s within phase %s", handler_desc_name, phase->name);
        return AXIS2_FAILURE;
    }

    if (phase->first_handler)
    {
        const axis2_char_t *first_handler_name = axutil_string_get_buffer(
            axis2_handler_get_name(phase->first_handler, env), env);
        handler_desc =
            axis2_handler_get_handler_desc(phase->first_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler description is not set in the first handler %s "\
                    "within phase %s", first_handler_name, phase->name);
            return AXIS2_FAILURE;
        }

        before_handler_name =
            axutil_string_get_buffer(axis2_handler_desc_get_name
                                     (handler_desc, env), env);
        if (!before_handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler name is not set in the handler description for the "\
                    "first handler %s within phase %s", first_handler_name, 
                        phase->name);
            return AXIS2_FAILURE;
        }
    }

    if (phase->last_handler)
    {
        const axis2_char_t *last_handler_name = axutil_string_get_buffer(
            axis2_handler_get_name(phase->last_handler, env), env);
        handler_desc = axis2_handler_get_handler_desc(phase->last_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler description is not set in the last handler %s "\
                    "within phase %s", last_handler_name, phase->name);
            return AXIS2_FAILURE;
        }

        after_handler_name =
            axutil_string_get_buffer(axis2_handler_desc_get_name
                                     (handler_desc, env), env);
        if (!after_handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                            AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Handler name is not set in the handler description for the "\
                    "last handler %s within phase %s", last_handler_name, 
                        phase->name);
            return AXIS2_FAILURE;
        }
    }

    if (before_handler_name && after_handler_name)
    {
        if (!axutil_strcmp(before_handler_name, before_name) &&
            !axutil_strcmp(after_handler_name, after_name))
        {
            return axis2_phase_add_unique(env, phase->handlers, handler);
        }
    }

    if (after_handler_name)
    {
        if (!axutil_strcmp(after_handler_name, after_name))
            after = 0;
    }

    size = axutil_array_list_size(phase->handlers, env);

    if (after_handler_name)
    {
        if (!axutil_strcmp(before_handler_name, before_name))
            before = size;
    }

    for (i = 0; i < size; i++)
    {
        axis2_handler_t *temp_handler =
            (axis2_handler_t *) axutil_array_list_get(phase->handlers, env, i);
        if (temp_handler)
        {
            const axis2_char_t *temp_handler_name = axutil_string_get_buffer(
                axis2_handler_get_name(temp_handler, env), env);
            handler_desc = axis2_handler_get_handler_desc(temp_handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler Description is not set for the Handler %s "\
                        "within phase %s", temp_handler_name, phase->name);
                return AXIS2_FAILURE;
            }

            handler_name =
                axutil_string_get_buffer(axis2_handler_desc_get_name
                                         (handler_desc, env), env);
            if (!handler_name)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Handler name is not set in the handler Description for "\
                        "handler %s within phase %s", temp_handler_name, 
                            phase->name);
                return AXIS2_FAILURE;
            }

            if (!axutil_strcmp(handler_name, after_name))
                after = i;
            if (!axutil_strcmp(handler_name, before_name))
                before = i;
        }

        if ((after >= 0) && (before >= 0))
        {
            /*both the before and after indexes have been found */
            if (after > before)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                    "Both the before and after indexes have been found and "\
                    "`after` comes before `before` which is wrong within "\
                    "phase %s", phase->name);
                return AXIS2_FAILURE;
            }
            else
            {
                if (after + 1 < size)
                {
                    return axutil_array_list_add_at(phase->handlers, env,
                                                    after + 1, handler);
                }
                else
                {
                    return axis2_phase_add_unique(env, phase->handlers,
                                                  handler);
                }
            }
        }
    }
    return axis2_phase_add_unique(env, phase->handlers, handler);
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_insert_handler_desc(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_desc_t * handler_desc)
{
    int type = 0;
    axis2_handler_t *handler = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    const axis2_char_t *handler_desc_name = axutil_string_get_buffer(
        axis2_handler_desc_get_name(handler_desc, env), env);
    const axis2_char_t *handler_name = NULL;
    handler = axis2_handler_desc_get_handler(handler_desc, env);

    if (!handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler is not set in the handler description %s", 
            handler_desc_name);
        return AXIS2_FAILURE;
    }
    handler_name = axutil_string_get_buffer(
        axis2_handler_get_name(handler, env), env);

    if (!_axis2_phase_is_valid_after(phase, env, handler))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Invalid Handler State for the handler %s within the phase %s", 
                handler_name, phase->name);
        return AXIS2_FAILURE;
    }

    if (!_axis2_phase_is_valid_before(phase, env, handler))
    {
        AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Invalid Handler State for the handler %s within the phase %s", 
                handler_name, phase->name);
        return AXIS2_FAILURE;
    }

    type = _axis2_phase_get_before_after(handler, env);

    switch (type)
    {
    case 0:                    /*AXIS2_BOTH_BEFORE_AFTER: */
        status = axis2_phase_insert_before_and_after(phase, env, handler);
        break;
    case 1:                    /*AXIS2_BEFORE: */
        status = axis2_phase_insert_before(phase, env, handler);
        break;
    case 2:                    /*AXIS2_AFTER: */
        status = axis2_phase_insert_after(phase, env, handler);
        break;
    case 3:                    /*AXIS2_ANYWHERE: */
        status = axis2_phase_add_unique(env, phase->handlers, handler);
        break;
    default:
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler description %s insertion failed within the phase %s", 
                handler_desc_name, phase->name);
        status = AXIS2_FAILURE;
        break;
    }
    return status;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_phase_get_all_handlers(
    const axis2_phase_t * phase,
    const axutil_env_t * env)
{
    return phase->handlers;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_invoke_start_from_handler(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    const int paused_handler_index,
    axis2_msg_ctx_t * msg_ctx)
{
    int i = 0,
        size = 0;
    axis2_status_t status = AXIS2_SUCCESS;

    axis2_msg_ctx_set_paused_phase_name(msg_ctx, env, phase->name);

    size = axutil_array_list_size(phase->handlers, env);
    for (i = paused_handler_index; i < size; i++)
    {
        axis2_handler_t *handler =
            (axis2_handler_t *) axutil_array_list_get(phase->handlers, env, i);
        if (handler)
        {
            const axis2_char_t *handler_name = axutil_string_get_buffer(
                axis2_handler_get_name(handler, env), env);
            int index = -1;

            axis2_handler_desc_t *handler_desc =
                axis2_handler_get_handler_desc(handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE,
                                AXIS2_FAILURE);
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
                    "Invalid Handler State for the handler %s within phase %s", 
                        handler_name, phase->name);
                return AXIS2_FAILURE;
            }

            axis2_handler_invoke(handler, env, msg_ctx);
            index = axis2_msg_ctx_get_current_handler_index(msg_ctx, env);
            axis2_msg_ctx_set_current_handler_index(msg_ctx, env, (index + 1));
        }
    }
    return status;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_phase_free(
    axis2_phase_t * phase,
    const axutil_env_t * env)
{
    if (--(phase->ref) > 0)
    {
        return;
    }

    if (phase->name)
    {
        AXIS2_FREE(env->allocator, phase->name);
    }

    if (phase->handlers)
    {
        axutil_array_list_free(phase->handlers, env);
    }
    AXIS2_FREE(env->allocator, phase);
    return;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_remove_handler_desc(
    axis2_phase_t * phase,
    const axutil_env_t * env,
    axis2_handler_desc_t * handler_desc)
{
    axis2_handler_t *handler;
    const axis2_char_t *handler_desc_name = axutil_string_get_buffer(
        axis2_handler_desc_get_name(handler_desc, env), env);
    handler = axis2_handler_desc_get_handler(handler_desc, env);
    if (!handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM,
                        AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, 
            "Handler is not set in the Handler Description %s within phase %s", 
                handler_desc_name, phase->name);
        return AXIS2_FAILURE;
    }
    return axis2_phase_remove_unique(env, phase->handlers, handler);
}

static axis2_status_t
axis2_phase_add_unique(
    const axutil_env_t * env,
    axutil_array_list_t * list,
    axis2_handler_t * handler)
{
    int i = 0,
        size = 0;
    axis2_bool_t add_handler = AXIS2_TRUE;
    const axutil_string_t *handler_name = NULL;

    handler_name = axis2_handler_get_name(handler, env);
    size = axutil_array_list_size(list, env);
    for (i = 0; i < size; i++)
    {
        axis2_handler_t *obj = NULL;
        const axutil_string_t *obj_name = NULL;

        obj = (axis2_handler_t *) axutil_array_list_get(list, env, i);
        obj_name = axis2_handler_get_name(obj, env);
        if (obj == handler)
        {
            add_handler = AXIS2_FALSE;
            break;
        }
        else if (!axutil_strcmp(axutil_string_get_buffer(handler_name, env),
                                    axutil_string_get_buffer(obj_name, env)))
        {
            add_handler = AXIS2_FALSE;
            break;
        }
    }
    if (add_handler)
        axutil_array_list_add(list, env, handler);
    return AXIS2_SUCCESS;
}

static axis2_status_t
axis2_phase_remove_unique(
    const axutil_env_t * env,
    axutil_array_list_t * list,
    axis2_handler_t * handler)
{
    int i = 0,
        size = 0;
    axis2_bool_t remove_handler = AXIS2_FALSE;
    const axutil_string_t *handler_name = NULL;

    handler_name = axis2_handler_get_name(handler, env);
    size = axutil_array_list_size(list, env);
    for (i = 0; i < size; i++)
    {
        axis2_handler_t *obj = NULL;
        const axutil_string_t *obj_name = NULL;

        obj = (axis2_handler_t *) axutil_array_list_get(list, env, i);
        obj_name = axis2_handler_get_name(obj, env);
        if (obj == handler)
        {
            remove_handler = AXIS2_TRUE;
            break;
        }
        else if (!axutil_strcmp(axutil_string_get_buffer(handler_name, env),
                                axutil_string_get_buffer(obj_name, env)))
        {
            remove_handler = AXIS2_TRUE;
            break;
        }
    }
    if (remove_handler)
        axutil_array_list_remove(list, env, i);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_phase_increment_ref(
    axis2_phase_t * phase,
    const axutil_env_t * env)
{
    phase->ref++;
    return AXIS2_SUCCESS;
}

