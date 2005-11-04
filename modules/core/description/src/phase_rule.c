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

#include <axis2.h>
#include <axis2_phase_rule.h>
#include <axis2_string.h>

typedef struct axis2_phase_rule_impl
{
	/** Phase Rule related operations */
	axis2_phase_rule_t phase_rule;
	/** name of phase or handler before */
	axis2_char_t *before;
	/** name of phase or handler after */
	axis2_char_t *after;
	/** phase name */
	axis2_char_t *name;
	/** Is this first phase? */
	axis2_bool_t phase_first;
	/** Is this last phase? */
	axis2_bool_t phase_last;
	
} axis2_phase_rule_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(phase_rule) ((axis2_phase_rule_impl_t *)phase_rule)

axis2_char_t* AXIS2_CALL axis2_phase_rule_get_before (struct axis2_phase_rule *phase_rule, 
                                       axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_phase_rule_set_before (struct axis2_phase_rule *phase_rule, 
                                       axis2_env_t **env, axis2_char_t *before);
axis2_char_t* AXIS2_CALL axis2_phase_rule_get_after (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_phase_rule_set_after (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_char_t *after);
axis2_char_t* AXIS2_CALL axis2_phase_rule_get_name (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_phase_rule_set_name (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_char_t *name);
axis2_bool_t AXIS2_CALL axis2_phase_rule_is_phase_first (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_phase_rule_set_phase_first (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_bool_t phae_first);
axis2_bool_t AXIS2_CALL axis2_phase_rule_is_phase_last (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env);
axis2_status_t AXIS2_CALL axis2_phase_rule_set_phase_last (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_bool_t phae_last);
axis2_status_t AXIS2_CALL axis2_phase_rule_free (struct axis2_phase_rule *phase_rule, 
                                   axis2_env_t **env);

axis2_phase_rule_t* AXIS2_CALL axis2_phase_rule_create(axis2_env_t **env, axis2_char_t *name)
{
    axis2_phase_rule_impl_t *phase_rule_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    phase_rule_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_phase_rule_impl_t) );
    if (!phase_rule_impl)
    { 
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        return NULL;        
    }

    phase_rule_impl->before = NULL;
	phase_rule_impl->after = NULL;
	phase_rule_impl->name = NULL;
	phase_rule_impl->phase_first = AXIS2_FALSE;
	phase_rule_impl->phase_last = AXIS2_FALSE;
	
	if (name)
	{
		phase_rule_impl->name = axis2_strdup(name);
	}
    
    /* initialize operations */
    phase_rule_impl->phase_rule.ops = NULL;
    phase_rule_impl->phase_rule.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_phase_rule_ops_t) );
    if (!phase_rule_impl->phase_rule.ops)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
        AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
        axis2_phase_rule_free(&(phase_rule_impl->phase_rule), env);
        return NULL;        
    }

    phase_rule_impl->phase_rule.ops->get_before = axis2_phase_rule_get_before;
    phase_rule_impl->phase_rule.ops->set_before = axis2_phase_rule_set_before;
    phase_rule_impl->phase_rule.ops->get_after = axis2_phase_rule_get_after;
    phase_rule_impl->phase_rule.ops->set_after = axis2_phase_rule_set_after;
    phase_rule_impl->phase_rule.ops->get_name = axis2_phase_rule_get_name;
    phase_rule_impl->phase_rule.ops->set_name = axis2_phase_rule_set_name;
    phase_rule_impl->phase_rule.ops->is_phase_first = axis2_phase_rule_is_phase_first;
    phase_rule_impl->phase_rule.ops->set_phase_first = axis2_phase_rule_set_phase_first;
    phase_rule_impl->phase_rule.ops->is_phase_last = axis2_phase_rule_is_phase_last;
    phase_rule_impl->phase_rule.ops->set_phase_last = axis2_phase_rule_set_phase_last;
    phase_rule_impl->phase_rule.ops->free = axis2_phase_rule_free;

    return &(phase_rule_impl->phase_rule);
}

axis2_char_t* AXIS2_CALL axis2_phase_rule_get_before (struct axis2_phase_rule *phase_rule, 
                                       axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, NULL);
    return AXIS2_INTF_TO_IMPL(phase_rule)->before;
}

axis2_status_t AXIS2_CALL axis2_phase_rule_set_before (struct axis2_phase_rule *phase_rule, 
                                       axis2_env_t **env, axis2_char_t *before)
{
    axis2_phase_rule_impl_t *phase_rule_impl = NULL;
    
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    
    phase_rule_impl = AXIS2_INTF_TO_IMPL(phase_rule);
    if (phase_rule_impl->before)
    {
        AXIS2_FREE((*env)->allocator, phase_rule_impl->before);
        phase_rule_impl->before = NULL;
    }
    
    if (before)
    {
        phase_rule_impl->before  = axis2_strdup(before);
        if (!phase_rule_impl->before)
        {
            AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
            AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL axis2_phase_rule_get_after (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, NULL);
    return AXIS2_INTF_TO_IMPL(phase_rule)->after;
}

axis2_status_t AXIS2_CALL axis2_phase_rule_set_after (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_char_t *after)
{
    axis2_phase_rule_impl_t *phase_rule_impl = NULL;
    
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    
    phase_rule_impl = AXIS2_INTF_TO_IMPL(phase_rule);
    if (phase_rule_impl->after)
    {
        AXIS2_FREE((*env)->allocator, phase_rule_impl->after);
        phase_rule_impl->after = NULL;
    }
    
    if (after)
    {
        phase_rule_impl->after  = axis2_strdup(after);
        if (!phase_rule_impl->after)
        {
            AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
            AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL axis2_phase_rule_get_name (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, NULL);
    return AXIS2_INTF_TO_IMPL(phase_rule)->name;
}

axis2_status_t AXIS2_CALL axis2_phase_rule_set_name (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_char_t *name)
{
    axis2_phase_rule_impl_t *phase_rule_impl = NULL;
    
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    
    phase_rule_impl = AXIS2_INTF_TO_IMPL(phase_rule);
    if (phase_rule_impl->name)
    {
        AXIS2_FREE((*env)->allocator, phase_rule_impl->name);
        phase_rule_impl->name = NULL;
    }
    
    if (name)
    {
        phase_rule_impl->name  = axis2_strdup(name);
        if (!phase_rule_impl->name)
        {
            AXIS2_ERROR_SET_ERROR_NUMBER((*env)->error, AXIS2_ERROR_NO_MEMORY);
            AXIS2_ERROR_SET_STATUS_CODE((*env)->error, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;

}

axis2_bool_t AXIS2_CALL axis2_phase_rule_is_phase_first (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(phase_rule)->phase_first;
}

axis2_status_t AXIS2_CALL axis2_phase_rule_set_phase_first (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_bool_t phase_first)
{
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(phase_rule)->phase_first = phase_first;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL axis2_phase_rule_is_phase_last (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env)
{
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(phase_rule)->phase_last;
}

axis2_status_t AXIS2_CALL axis2_phase_rule_set_phase_last (struct axis2_phase_rule * phase_rule, 
                                       axis2_env_t **env, axis2_bool_t phase_last)
{
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(phase_rule)->phase_last = phase_last;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL axis2_phase_rule_free (struct axis2_phase_rule *phase_rule, 
                                   axis2_env_t **env)
{
    axis2_phase_rule_impl_t *phase_rule_impl = NULL;
    
    AXIS2_FUNC_PARAMETER_CHECK(phase_rule, env, AXIS2_FAILURE);
    
    phase_rule_impl = AXIS2_INTF_TO_IMPL(phase_rule);
    
    if (phase_rule_impl->phase_rule.ops)
    {
        AXIS2_FREE((*env)->allocator, phase_rule_impl->phase_rule.ops);
        phase_rule_impl->phase_rule.ops = NULL;
    }
    
    if (phase_rule_impl->before)
    {
        AXIS2_FREE((*env)->allocator, phase_rule_impl->before);
        phase_rule_impl->before = NULL;
    }    
    
	if (phase_rule_impl->after)
    {
        AXIS2_FREE((*env)->allocator, phase_rule_impl->after);
        phase_rule_impl->after = NULL;
    }
	
    if (phase_rule_impl->name)
    {
        AXIS2_FREE((*env)->allocator, phase_rule_impl->name);
        phase_rule_impl->name = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, phase_rule_impl);
    phase_rule_impl = NULL;
    
    return AXIS2_SUCCESS;
}
