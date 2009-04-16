
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

#include <axis2_rm_assertion.h>
#include <neethi_assertion.h>
#include <neethi_all.h>

struct axis2_rm_assertion_t
{
    /* RM Policy 1.1 assertions */
    axis2_bool_t is_sequence_str;
    axis2_bool_t is_sequence_transport_security;
    axis2_bool_t is_exactly_once;
    axis2_bool_t is_atleast_once;
    axis2_bool_t is_atmost_once;
    axis2_bool_t is_inorder;

    /* RM Policy 1.0 assertions */
    axis2_char_t *inactivity_timeout;
    axis2_char_t *retrans_interval;
    axis2_char_t *ack_interval;
    axis2_bool_t is_exp_backoff;

    /* Sandesha2 specific assrtions */
    axis2_char_t *storage_mgr;
    axis2_char_t *message_types_to_drop;
    axis2_char_t *max_retrans_count;
    axis2_char_t *sender_sleep_time;    
    axis2_char_t *invoker_sleep_time;
    axis2_char_t *polling_wait_time;
    axis2_char_t *terminate_delay;
    axis2_char_t *sandesha2_db;
};

AXIS2_EXTERN axis2_rm_assertion_t *AXIS2_CALL
axis2_rm_assertion_create(
    const axutil_env_t * env)
{
    axis2_rm_assertion_t *rm_assertion;

    rm_assertion = (axis2_rm_assertion_t *) AXIS2_MALLOC(env->allocator,
                                                sizeof(axis2_rm_assertion_t));

    if (rm_assertion == NULL)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    rm_assertion->is_sequence_str = AXIS2_FALSE;
    rm_assertion->is_sequence_transport_security = AXIS2_FALSE;
    rm_assertion->is_exactly_once = AXIS2_TRUE;
    rm_assertion->is_atleast_once = AXIS2_FALSE;
    rm_assertion->is_atmost_once = AXIS2_FALSE;
    rm_assertion->is_inorder = AXIS2_FALSE;

    /* RM Policy 1.0 assertions */
    rm_assertion->inactivity_timeout = NULL;
    rm_assertion->retrans_interval = NULL;
    rm_assertion->ack_interval = NULL;
    rm_assertion->is_exp_backoff = AXIS2_FALSE;

    /* Sandesha2 specific assrtions */
    rm_assertion->storage_mgr = NULL;
    rm_assertion->message_types_to_drop = NULL;
    rm_assertion->max_retrans_count = NULL;
    rm_assertion->sender_sleep_time = NULL;
    rm_assertion->invoker_sleep_time = NULL;
    rm_assertion->polling_wait_time = NULL;
    rm_assertion->terminate_delay = NULL;
    rm_assertion->sandesha2_db = NULL;

    return rm_assertion;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_rm_assertion_free(
    axis2_rm_assertion_t * rm_assertion,
    const axutil_env_t * env)
{
    if(rm_assertion)
    {
        AXIS2_FREE(env->allocator, rm_assertion);
        rm_assertion = NULL;
    }

    return;
}

/* Implementations.
 * Following functions are getters and setters 
 * for rm_assertion struct */

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_rm_assertion_get_is_sequence_str(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->is_sequence_str;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_is_sequence_str(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_bool_t is_sequence_str)
{
    rm_assertion->is_sequence_str = is_sequence_str;
 
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_rm_assertion_get_is_sequence_transport_security(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->is_sequence_transport_security;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_is_sequence_transport_security(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_bool_t is_sequence_transport_security)
{
    rm_assertion->is_sequence_transport_security = 
        is_sequence_transport_security;
 
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_rm_assertion_get_is_exactly_once(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->is_exactly_once;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_is_exactly_once(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_bool_t is_exactly_once)
{
    rm_assertion->is_exactly_once = is_exactly_once;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_rm_assertion_get_is_atleast_once(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->is_atleast_once;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_is_atleast_once(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_bool_t is_atleast_once)
{
    rm_assertion->is_atleast_once = is_atleast_once;
 
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_rm_assertion_get_is_atmost_once(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->is_atmost_once;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_is_atmost_once(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_bool_t is_atmost_once)
{
    rm_assertion->is_atmost_once = is_atmost_once;
 
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_rm_assertion_get_is_inorder(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->is_inorder;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_is_inorder(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_bool_t is_inorder)
{
    rm_assertion->is_inorder = is_inorder;
 
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_inactivity_timeout(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->inactivity_timeout;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_inactivity_timeout(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* inactivity_timeout)
{
    rm_assertion->inactivity_timeout = inactivity_timeout;
 
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_retrans_interval(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->retrans_interval;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_retrans_interval(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* retrans_interval)
{
    rm_assertion->retrans_interval = retrans_interval;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_ack_interval(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->ack_interval;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_ack_interval(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* ack_interval)
{
    rm_assertion->ack_interval = ack_interval;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_rm_assertion_get_is_exp_backoff(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->is_exp_backoff;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_is_exp_backoff(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_bool_t is_exp_backoff)
{
    rm_assertion->is_exp_backoff = is_exp_backoff;
 
    return AXIS2_SUCCESS;
}



AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_storage_mgr(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->storage_mgr;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_storage_mgr(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* storage_mgr)
{
    rm_assertion->storage_mgr = storage_mgr;
 
    return AXIS2_SUCCESS;
}



AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_message_types_to_drop(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->message_types_to_drop;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_message_types_to_drop(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* message_types_to_drop)
{
    rm_assertion->message_types_to_drop = message_types_to_drop;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_max_retrans_count(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->max_retrans_count;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_max_retrans_count(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* max_retrans_count)
{
    rm_assertion->max_retrans_count = max_retrans_count;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_sender_sleep_time(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->sender_sleep_time;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_sender_sleep_time(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* sender_sleep_time)
{
    rm_assertion->sender_sleep_time = sender_sleep_time;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_invoker_sleep_time(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->invoker_sleep_time;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_invoker_sleep_time(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* invoker_sleep_time)
{
    rm_assertion->invoker_sleep_time = invoker_sleep_time;
 
    return AXIS2_SUCCESS;
}



AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_polling_wait_time(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->polling_wait_time;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_polling_wait_time(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* polling_wait_time)
{
    rm_assertion->polling_wait_time = polling_wait_time;
 
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_terminate_delay(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->terminate_delay;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_terminate_delay(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* terminate_delay)
{
    rm_assertion->terminate_delay = terminate_delay;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_rm_assertion_get_sandesha2_db(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env)
{
    return rm_assertion->sandesha2_db;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_rm_assertion_set_sandesha2_db(
    axis2_rm_assertion_t *rm_assertion,
    const axutil_env_t * env,
    axis2_char_t* sandesha2_db)
{
    rm_assertion->sandesha2_db = sandesha2_db;
 
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_rm_assertion_t* AXIS2_CALL
axis2_rm_assertion_get_from_policy(
    const axutil_env_t *env,
    neethi_policy_t *policy)
{
    axutil_array_list_t *alternatives = NULL;
    neethi_operator_t *component = NULL;
    neethi_all_t *all = NULL;
    axutil_array_list_t *arraylist = NULL;
    neethi_operator_t *operator = NULL;
    neethi_assertion_t *assertion = NULL;
    neethi_assertion_type_t type;
    void *value = NULL;
    int i = 0;

    alternatives = neethi_policy_get_alternatives(policy, env);

    component =
        (neethi_operator_t *) axutil_array_list_get(alternatives, env, 0);
    all = (neethi_all_t *) neethi_operator_get_value(component, env);

    arraylist = neethi_all_get_policy_components(all, env);

    for (i = 0; i < axutil_array_list_size(arraylist, env); i++)
    {
        operator =(neethi_operator_t *) axutil_array_list_get(arraylist, env,
                                                              i);
        assertion =
            (neethi_assertion_t *) neethi_operator_get_value(operator, env);
        value = neethi_assertion_get_value(assertion, env);
        type = neethi_assertion_get_type(assertion, env);

        if (value)
        {
            if (type == ASSERTION_TYPE_RM_ASSERTION)
            {
                return (axis2_rm_assertion_t *)value;
            }
        }
    }
    
    return NULL;
}

