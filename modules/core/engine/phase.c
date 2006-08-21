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

#include <axis2_string.h>
#include <axis2_phase.h>
#include <axis2_array_list.h>
#include <axis2_msg_ctx.h>
#include <axis2_const.h>


typedef struct axis2_phase_impl 
{
    /** phase */
   axis2_phase_t phase;
    /** phase name */
    axis2_char_t *name;    
    /** array list of phases */
    axis2_array_list_t* handlers;    
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
} axis2_phase_impl_t;
 

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(phase) ((axis2_phase_impl_t *)phase)


axis2_status_t AXIS2_CALL 
axis2_phase_add_handler_at(
    axis2_phase_t *phase, 
    const axis2_env_t *env,
    int index, 
    axis2_handler_t *handler);

axis2_status_t AXIS2_CALL 
axis2_phase_add_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env,
    axis2_handler_t *handler);

axis2_status_t AXIS2_CALL 
axis2_phase_invoke(
    axis2_phase_t *phase, 
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

const axis2_char_t *AXIS2_CALL 
axis2_phase_get_name(
    const axis2_phase_t *phase, 
    const axis2_env_t *env);

int AXIS2_CALL 
axis2_phase_get_handler_count(
    const axis2_phase_t *phase, 
    const axis2_env_t *env); 

int AXIS2_CALL 
_axis2_phase_get_before_after(
    axis2_handler_t *handler, 
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_phase_set_first_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler);

axis2_status_t AXIS2_CALL 
axis2_phase_set_last_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler);

axis2_status_t AXIS2_CALL 
axis2_phase_add_handler_desc(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_desc_t *handler_desc);

axis2_bool_t AXIS2_CALL 
_axis2_phase_is_valid_before(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler);

axis2_bool_t AXIS2_CALL 
_axis2_phase_is_valid_after(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler);

axis2_status_t AXIS2_CALL 
axis2_phase_insert_before(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler);

axis2_status_t AXIS2_CALL 
axis2_phase_insert_after(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler) ;

axis2_status_t AXIS2_CALL 
axis2_phase_insert_before_and_after(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler);

axis2_status_t AXIS2_CALL 
axis2_phase_insert_handler_desc(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_desc_t *handler_desc);

axis2_array_list_t *AXIS2_CALL 
axis2_phase_get_all_handlers(
    const axis2_phase_t *phase, 
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL 
axis2_phase_invoke_start_from_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    int paused_handler_index, 
    axis2_msg_ctx_t *msg_ctx) ;
                                      
axis2_status_t AXIS2_CALL 
axis2_phase_free (
    axis2_phase_t *phase, 
    const axis2_env_t *env);

static axis2_status_t 
add_unique(
    const axis2_env_t *env,
    axis2_array_list_t *list,
    axis2_handler_t *handler);

axis2_phase_t *AXIS2_CALL 
axis2_phase_create(
    const axis2_env_t *env, 
    const axis2_char_t *phase_name) 
{
    axis2_phase_impl_t *phase_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    phase_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_phase_impl_t) );
    if (!phase_impl)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    phase_impl->name = NULL;
    phase_impl->handlers = NULL;
    phase_impl->first_handler = NULL;    
    phase_impl->first_handler_set = AXIS2_FALSE;
    phase_impl->last_handler = NULL;    
    phase_impl->last_handler_set = AXIS2_FALSE;    
    phase_impl->is_one_handler = AXIS2_FALSE;
    
   phase_impl->handlers = axis2_array_list_create(env, 10);
    if (!(phase_impl->handlers))
    {
        /** error is already set by last method array list container create */
        axis2_phase_free(&(phase_impl->phase), env);
        return NULL;        
    }    
    
    if (phase_name)
    {
        phase_impl->name = AXIS2_STRDUP(phase_name, env);
        if (!(phase_impl->name))
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            axis2_phase_free(&(phase_impl->phase), env);
            return NULL;        
        }
    }

    /* initialize ops */
    phase_impl->phase.ops = NULL;
    phase_impl->phase.ops  = AXIS2_MALLOC( env->allocator, sizeof(axis2_phase_ops_t) );
    if (!phase_impl->phase.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_phase_free(&(phase_impl->phase), env);
        return NULL;        
    }

    phase_impl->phase.ops->add_handler_at = 
        axis2_phase_add_handler_at;
        
    phase_impl->phase.ops->add_handler = 
        axis2_phase_add_handler;
        
    phase_impl->phase.ops->invoke = 
        axis2_phase_invoke;
        
    phase_impl->phase.ops->get_name = 
        axis2_phase_get_name;
        
    phase_impl->phase.ops->get_handler_count = 
        axis2_phase_get_handler_count;
        
    phase_impl->phase.ops->set_first_handler = 
        axis2_phase_set_first_handler;
        
    phase_impl->phase.ops->set_last_handler = 
        axis2_phase_set_last_handler;
        
    phase_impl->phase.ops->add_handler_desc = 
        axis2_phase_add_handler_desc;
        
    phase_impl->phase.ops->insert_before = 
        axis2_phase_insert_before;
        
    phase_impl->phase.ops->insert_after = 
        axis2_phase_insert_after;
        
    phase_impl->phase.ops->insert_before_and_after = 
        axis2_phase_insert_before_and_after;
        
    phase_impl->phase.ops->insert_handler_desc = 
        axis2_phase_insert_handler_desc;
        
    phase_impl->phase.ops->get_all_handlers = 
        axis2_phase_get_all_handlers;
        
    phase_impl->phase.ops->invoke_start_from_handler = 
        axis2_phase_invoke_start_from_handler;
        
    phase_impl->phase.ops->free = 
        axis2_phase_free;

    return &(phase_impl->phase);
}

axis2_status_t AXIS2_CALL 
axis2_phase_add_handler_at(
    axis2_phase_t *phase, 
    const axis2_env_t *env,
    int index, 
    axis2_handler_t *handler) 
{
    axis2_phase_impl_t *phase_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase);
    
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
            "axis2_handler_t *%s added to the index %d of the phase %s",
            AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_GET_QNAME(handler, env), env),
            index,
            phase_impl->name);
    
    return AXIS2_ARRAY_LIST_ADD_AT(phase_impl->handlers, env, index, handler);
}

axis2_status_t AXIS2_CALL 
axis2_phase_add_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env,
    axis2_handler_t *handler) 
{
    axis2_phase_impl_t *phase_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase);
    
    AXIS2_LOG_INFO(env->log, "Handler %s added to phase %s",
                     AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_GET_QNAME(handler, env), env), 
                     phase_impl->name);
    
    /*return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
    return add_unique(env, phase_impl->handlers, handler);
}

axis2_status_t AXIS2_CALL 
axis2_phase_invoke(
    axis2_phase_t *phase, 
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_phase_impl_t *phase_impl = NULL;
    int index = 0, size = 0;
    axis2_status_t status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase);
    
    AXIS2_MSG_CTX_SET_PAUSED_PHASE_NAME(msg_ctx, env, phase_impl->name);
    if (phase_impl->first_handler)
    {
        if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
        {
            return AXIS2_SUCCESS;
        } 
        else 
        {
            AXIS2_LOG_INFO(env->log, 
                     "Invoke the first handler %s within the phase %s",
                     AXIS2_QNAME_GET_LOCALPART(
                            AXIS2_HANDLER_GET_QNAME(phase_impl->first_handler, env), env), 
                     phase_impl->name);
            
            status = AXIS2_HANDLER_INVOKE(phase_impl->first_handler, env, msg_ctx);
            if (status != AXIS2_SUCCESS)
                return status;
        }
    }
    /* Invoking the rest of handlers except first_handler and last_handler */
    size = AXIS2_ARRAY_LIST_SIZE(phase_impl->handlers, env);
    
    while (index < size ) 
    {
        if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)) 
        {
            break;
        } 
        else 
        {
            axis2_handler_t *handler = (axis2_handler_t*) 
                        AXIS2_ARRAY_LIST_GET(phase_impl->handlers, env, index);
            if (handler) 
            {
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                        "Invoke the handler %s within the phase %s",
                         AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_GET_QNAME(handler, env), env), 
                         phase_impl->name);
                status = AXIS2_HANDLER_INVOKE(handler, env, msg_ctx);
                if (status != AXIS2_SUCCESS)
                    return status;
                /* index increment should be done after the invoke function. If the invocation 
                   failed this handler is taken care of and no need to revoke again */
                index++;
                AXIS2_MSG_CTX_SET_CURRENT_HANDLER_INDEX(msg_ctx, env, index);
            }
        }
    }
    
    /* If phase last handler is there invoke it here*/
    if (phase_impl->last_handler) 
    {
        if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
        {
            return AXIS2_SUCCESS;
        } 
        else 
        {
            AXIS2_LOG_INFO(env->log, "Invoke the last handler %s within the phase %s",
                     AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_GET_QNAME(phase_impl->last_handler, env), env), 
                     phase_impl->name);
            status = AXIS2_HANDLER_INVOKE(phase_impl->last_handler, env, msg_ctx);
            if (status != AXIS2_SUCCESS)
                return status;
        }
    }
    
    return AXIS2_SUCCESS;
}


const axis2_char_t *AXIS2_CALL 
axis2_phase_get_name(
    const axis2_phase_t *phase, 
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);    
    return AXIS2_INTF_TO_IMPL(phase)->name;
}

int AXIS2_CALL 
axis2_phase_get_handler_count(
    const axis2_phase_t *phase, 
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);    
    return AXIS2_ARRAY_LIST_SIZE(AXIS2_INTF_TO_IMPL(phase)->handlers, env );
}


int AXIS2_CALL 
_axis2_phase_get_before_after(
    axis2_handler_t *handler, 
    const axis2_env_t *env)
{
    axis2_char_t *before = NULL, *after = NULL;
    axis2_handler_desc_t *handler_desc = NULL;
    axis2_phase_rule_t *rules = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    rules = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    before = AXIS2_PHASE_RULE_GET_BEFORE(rules, env);
    after = AXIS2_PHASE_RULE_GET_AFTER(rules, env);
    
    if (before && after)
    {
        if (AXIS2_STRCMP(before, after) == 0 )
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_BEFORE_AFTER_HANDLERS_SAME, AXIS2_FAILURE);
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

axis2_status_t AXIS2_CALL 
axis2_phase_set_first_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler)
{
    axis2_phase_impl_t *phase_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase);
    
    if (phase_impl->first_handler_set)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_PHASE_FIRST_HANDLER_ALREADY_SET, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        if (_axis2_phase_get_before_after(handler, env) != AXIS2_PHASE_ANYWHERE)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_PHASE_FIRST_HANDLER, AXIS2_FAILURE);
            return AXIS2_FAILURE;            
        }
        phase_impl->first_handler = handler;
        phase_impl->first_handler_set = AXIS2_TRUE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_phase_set_last_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler)
{
    axis2_phase_impl_t *phase_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase);

    if (phase_impl->last_handler_set)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_PHASE_LAST_HANDLER_ALREADY_SET, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        if (_axis2_phase_get_before_after(handler, env) != AXIS2_PHASE_ANYWHERE)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_PHASE_LAST_HANDLER, AXIS2_FAILURE);
            return AXIS2_FAILURE;            
        }
        phase_impl->last_handler = handler;
        phase_impl->last_handler_set = AXIS2_TRUE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_phase_add_handler_desc(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_desc_t *handler_desc)
{
    axis2_phase_impl_t *phase_impl = NULL;
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_t *handler = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_bool_t first = AXIS2_FALSE, last = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 

    if (phase_impl->is_one_handler) 
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_PHASE_ADD_HANDLER_INVALID, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    } 
    else 
    {
        rules = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
        if (!rules)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        first = AXIS2_PHASE_RULE_IS_PHASE_FIRST(rules, env);
        last = AXIS2_PHASE_RULE_IS_PHASE_LAST(rules, env);
        
        if (first && last)
        {
            if (AXIS2_ARRAY_LIST_SIZE(phase_impl->handlers, env) > 0 )
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_RULES, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            handler = AXIS2_HANDLER_DESC_GET_HANDLER(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }    
            
            /*status = AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
            status = add_unique(env, phase_impl->handlers, handler);
            if (status == AXIS2_SUCCESS)
                phase_impl->is_one_handler = AXIS2_TRUE;
            return status;            
        }
        else if (first)
        {
            handler = AXIS2_HANDLER_DESC_GET_HANDLER(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }    
            return axis2_phase_set_first_handler(phase, env, handler);
        }
        else if (last)
        {
            handler = AXIS2_HANDLER_DESC_GET_HANDLER(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }    
            return axis2_phase_set_last_handler(phase, env, handler);
        }
        else
        {
            handler = AXIS2_HANDLER_DESC_GET_HANDLER(handler_desc, env);
            if (!handler)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            } 
                        
            return axis2_phase_insert_handler_desc(phase, env, handler_desc);
        }
    }
}

axis2_bool_t AXIS2_CALL 
_axis2_phase_is_valid_before(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler)
{
    axis2_phase_impl_t *phase_impl = NULL;
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;    
    axis2_char_t *first_handler_name = NULL, *before = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 
    
    if (phase_impl->first_handler)
    {
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(phase_impl->first_handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;
        
        first_handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
        
        if (!first_handler_name)
            return AXIS2_TRUE;
        
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;
        
        rules = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
        if (!rules)
            return AXIS2_TRUE;
        
        before = AXIS2_PHASE_RULE_GET_BEFORE(rules, env);
        if (!before)
            return AXIS2_TRUE;
        
        if (AXIS2_STRCMP(first_handler_name, before) == 0)
            return AXIS2_FALSE;
        else
            return AXIS2_TRUE;
            
    }
    return AXIS2_TRUE;
}

axis2_bool_t AXIS2_CALL 
_axis2_phase_is_valid_after(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler)
{
    axis2_phase_impl_t *phase_impl = NULL;
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;    
    axis2_char_t *last_handler_name = NULL, *after = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 
    
    if (phase_impl->last_handler)
    {
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(phase_impl->last_handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;
        
        last_handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
        
        if (!last_handler_name)
            return AXIS2_TRUE;
        
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
        if (!handler_desc)
            return AXIS2_TRUE;
        
        rules = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
        if (!rules)
            return AXIS2_TRUE;
        
        after = AXIS2_PHASE_RULE_GET_AFTER(rules, env);
        if (!after)
            return AXIS2_TRUE;
        
        if (AXIS2_STRCMP(last_handler_name, after) == 0)
            return AXIS2_FALSE;
        else
            return AXIS2_TRUE;
            
    }
    return AXIS2_TRUE;    
}

axis2_status_t AXIS2_CALL 
axis2_phase_insert_before(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler) 
{
    axis2_phase_impl_t *phase_impl = NULL;
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;    
    axis2_char_t *handler_name = NULL, *before = NULL;
    int i = 0;
    int size = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 
    
    handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    
    rules = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
   
    before = AXIS2_PHASE_RULE_GET_BEFORE(rules, env);
    if (!before)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    if (phase_impl->last_handler)
    {
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(phase_impl->last_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
        if (!handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        if (AXIS2_STRCMP(before, handler_name) == 0 )
        {
            /*return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
            return add_unique(env, phase_impl->handlers, handler);
        }
    }

    size = AXIS2_ARRAY_LIST_SIZE(phase_impl->handlers, env);
    
    for (i = 0; i < size; i++) 
    {
        axis2_handler_t *temp_handler = (axis2_handler_t *) AXIS2_ARRAY_LIST_GET(phase_impl->handlers, env, i);
        if (temp_handler)
        {
            handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(temp_handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
            if (!handler_name)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            if (AXIS2_STRCMP(before, handler_name) == 0 )
            {
                return AXIS2_ARRAY_LIST_ADD_AT(phase_impl->handlers, env, i, handler);
            }
        }     
    }
    /* add as the last handler */
    /* return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler); */
    return add_unique(env, phase_impl->handlers, handler);
}

axis2_status_t AXIS2_CALL 
axis2_phase_insert_after(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler) 
{
    axis2_phase_impl_t *phase_impl = NULL;
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;    
    axis2_char_t *handler_name = NULL, *after = NULL;
    int i = 0;
    int size = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 
    
    handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    
    rules = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
   
    after = AXIS2_PHASE_RULE_GET_AFTER(rules, env);
    if (!after)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    if (phase_impl->first_handler)
    {
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(phase_impl->first_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
        if (!handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        if (AXIS2_STRCMP(after, handler_name) == 0 )
        {
            return AXIS2_ARRAY_LIST_ADD_AT(phase_impl->handlers, env, 0, handler);
        }
    }

    size = AXIS2_ARRAY_LIST_SIZE(phase_impl->handlers, env);
    
    for (i = 0; i < size; i++) 
    {
        axis2_handler_t *temp_handler = (axis2_handler_t *) AXIS2_ARRAY_LIST_GET(phase_impl->handlers, env, i);
        if (temp_handler)
        {
            handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(temp_handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
            if (!handler_name)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            if (AXIS2_STRCMP(after, handler_name) == 0 )
            {
                if (i == (size - 1))
                {
                    /*return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
                    return add_unique(env, phase_impl->handlers, handler);
                }
                else
                    return AXIS2_ARRAY_LIST_ADD_AT(phase_impl->handlers, env, i + 1, handler);
            }
        }
    }

    if (size > 0)
        return AXIS2_ARRAY_LIST_ADD_AT(phase_impl->handlers, env, 0, handler);
    else
    {
        /*return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
        return add_unique(env, phase_impl->handlers, handler);
    }
}

axis2_status_t AXIS2_CALL 
axis2_phase_insert_before_and_after(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_t *handler)
{
    int before = -1;
    int after = -1;
    axis2_phase_impl_t *phase_impl = NULL;
    axis2_phase_rule_t *rules = NULL;
    axis2_handler_desc_t *handler_desc = NULL;    
    axis2_char_t *before_handler_name = NULL, *after_handler_name = NULL, 
                 *after_name = NULL, *before_name = NULL, *handler_name = NULL;
    int i = 0;
    int size = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 
    
    handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
    if (!handler_desc)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    
    rules = AXIS2_HANDLER_DESC_GET_RULES(handler_desc, env);
    if (!rules)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
   
    before_name = AXIS2_PHASE_RULE_GET_BEFORE(rules, env);
    if (!before_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    after_name = AXIS2_PHASE_RULE_GET_AFTER(rules, env);
    if (!after_name)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    if (phase_impl->first_handler)
    {
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(phase_impl->first_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        before_handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
        if (!handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    if (phase_impl->last_handler)
    {
        handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(phase_impl->last_handler, env);
        if (!handler_desc)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        after_handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
        if (!handler_name)
        {
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    if (before_handler_name && after_handler_name)
    {
        if (AXIS2_STRCMP(before_handler_name, before_name) == 0 &&
            AXIS2_STRCMP(after_handler_name, after_name) == 0 )
        {
            /*return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
            return add_unique(env, phase_impl->handlers, handler);
        }
    }
    
    if (after_handler_name)
    {
        if (AXIS2_STRCMP(after_handler_name, after_name) == 0 )
            after = 0;
    }
    
    size = AXIS2_ARRAY_LIST_SIZE(phase_impl->handlers, env);
    
    if (after_handler_name)
    {
        if (AXIS2_STRCMP(before_handler_name, before_name) == 0 )
            before = size;
    }
    
    for (i = 0; i < size; i++)
    {
        axis2_handler_t *temp_handler = (axis2_handler_t *) AXIS2_ARRAY_LIST_GET(phase_impl->handlers, env, i);
        if (temp_handler)
        {
            handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(temp_handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            handler_name = AXIS2_QNAME_GET_LOCALPART(AXIS2_HANDLER_DESC_GET_QNAME(handler_desc, env), env);
            if (!handler_name)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            if (AXIS2_STRCMP(handler_name, after_name) == 0 )
                after = i;
            if (AXIS2_STRCMP(handler_name, before_name) == 0 )
                before = i;            
        }

        if ((after >= 0) && (before >= 0))
        {
            /*both the before and after indexes have been found */
            if (after > before) 
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            } 
            else 
            {
                if (after + 1 < size) 
                {
                    return AXIS2_ARRAY_LIST_ADD_AT(phase_impl->handlers, env, after + 1, handler);
                } 
                else 
                {
                    /*return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
                    return add_unique(env, phase_impl->handlers, handler);
                }
            }
        }
    }
    /*return AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
    return add_unique(env, phase_impl->handlers, handler);
}

axis2_status_t AXIS2_CALL 
axis2_phase_insert_handler_desc(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    axis2_handler_desc_t *handler_desc)
{
    axis2_phase_impl_t *phase_impl = NULL;
    int type = 0;
    axis2_handler_t *handler = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 
    
    handler = AXIS2_HANDLER_DESC_GET_HANDLER(handler_desc, env);

    if (!handler)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    
    if (!_axis2_phase_is_valid_after(phase, env, handler))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    
    if (!_axis2_phase_is_valid_before(phase, env, handler))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
        return AXIS2_FAILURE;        
    }
    
    type = _axis2_phase_get_before_after(handler, env);
    
    switch (type) 
    {
        case 0: /*AXIS2_BOTH_BEFORE_AFTER:*/
                status = axis2_phase_insert_before_and_after(phase, env, handler);
                break;
        case 1: /*AXIS2_BEFORE:*/
                status = axis2_phase_insert_before(phase, env, handler);
                break;
        case 2: /*AXIS2_AFTER:*/
                status = axis2_phase_insert_after(phase, env, handler);
                break;
        case 3: /*AXIS2_ANYWHERE:*/
                /*status = AXIS2_ARRAY_LIST_ADD(phase_impl->handlers, env, handler);*/
                status = add_unique(env, phase_impl->handlers, handler);
                break;
        default:
            status = AXIS2_FAILURE;
            break;
    }
    return status;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phase_get_all_handlers(
    const axis2_phase_t *phase, 
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);    
    return AXIS2_INTF_TO_IMPL(phase)->handlers;
}

axis2_status_t AXIS2_CALL 
axis2_phase_invoke_start_from_handler(
    axis2_phase_t *phase, 
    const axis2_env_t *env, 
    int paused_handler_index,
    axis2_msg_ctx_t *msg_ctx) 
{
    axis2_phase_impl_t *phase_impl = NULL;
    int i = 0, size = 0;
    axis2_status_t status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase); 
    
    AXIS2_MSG_CTX_SET_PAUSED_PHASE_NAME(msg_ctx, env, phase_impl->name);    
    
    size = AXIS2_ARRAY_LIST_SIZE(phase_impl->handlers, env);
    for(i = paused_handler_index; i < size; i++)
    { 
        axis2_handler_t *handler = (axis2_handler_t *) AXIS2_ARRAY_LIST_GET(
            phase_impl->handlers, env, i);
        if (handler)
        {
            int index = -1;
            
            axis2_handler_desc_t *handler_desc = AXIS2_HANDLER_GET_HANDLER_DESC(handler, env);
            if (!handler_desc)
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_HANDLER_STATE, AXIS2_FAILURE);
                return AXIS2_FAILURE;
            }
            
            AXIS2_HANDLER_INVOKE(handler, env, msg_ctx);
            index = AXIS2_MSG_CTX_GET_CURRENT_HANDLER_INDEX(msg_ctx, env);
            AXIS2_MSG_CTX_SET_CURRENT_HANDLER_INDEX(msg_ctx, env, (index+1));
        }
    }
    return status;
}

axis2_status_t AXIS2_CALL 
axis2_phase_free(
    axis2_phase_t *phase, 
    const axis2_env_t *env)
{
    axis2_phase_impl_t *phase_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    phase_impl = AXIS2_INTF_TO_IMPL(phase);
    
    if (phase_impl->phase.ops)
    {
        AXIS2_FREE(env->allocator, phase_impl->phase.ops);
        phase_impl->phase.ops = NULL;
    }
    
    if (phase_impl->name)
    {
        AXIS2_FREE(env->allocator, phase_impl->name);
        phase_impl->name = NULL;
    }    
    
    if (phase_impl->handlers)
    {
        AXIS2_ARRAY_LIST_FREE(phase_impl->handlers, env);
        phase_impl->handlers = NULL;
    }    

    AXIS2_FREE(env->allocator, phase_impl);
    phase_impl = NULL;
    
    return AXIS2_SUCCESS;
}

static axis2_status_t 
add_unique(
    const axis2_env_t *env,
    axis2_array_list_t *list,
    axis2_handler_t *handler)
{
    int i = 0, size = 0;
    axis2_bool_t add_handler = AXIS2_TRUE;

    size = AXIS2_ARRAY_LIST_SIZE(list, env);
    for(i = 0; i < size; i++)
    {
        axis2_handler_t *obj = NULL;

        obj = AXIS2_ARRAY_LIST_GET(list, env, i);
        if(obj == handler)
        {
            add_handler = AXIS2_FALSE;
            break;
        }
    }
    if(AXIS2_TRUE == add_handler)
        AXIS2_ARRAY_LIST_ADD(list, env, handler);
    return AXIS2_SUCCESS;
}


