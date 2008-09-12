
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


#ifndef AXIS2_RM_ASSERTION_H
#define AXIS2_RM_ASSERTION_H

/** @defgroup axis2_rm_assertion
 * @ingroup axis2_rm_assertion
 * @{
 */

#include <neethi_includes.h>
#include <neethi_policy.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct axis2_rm_assertion_t axis2_rm_assertion_t;


    AXIS2_EXTERN axis2_rm_assertion_t *AXIS2_CALL
    axis2_rm_assertion_create(
        const axutil_env_t * env);

    AXIS2_EXTERN void AXIS2_CALL
    axis2_rm_assertion_free(
        axis2_rm_assertion_t * rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_rm_assertion_get_is_sequence_str(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_is_sequence_str(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_bool_t is_sequence_str);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_rm_assertion_get_is_sequence_transport_security(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_is_sequence_transport_security(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_bool_t is_sequence_transport_security);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_rm_assertion_get_is_exactly_once(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_is_exactly_once(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_bool_t is_exactly_once);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_rm_assertion_get_is_atleast_once(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_is_atleast_once(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_bool_t is_atleast_once);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_rm_assertion_get_is_atmost_once(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_is_atmost_once(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_bool_t is_atmost_once);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_rm_assertion_get_is_inorder(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_is_inorder(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_bool_t is_inorder);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_inactivity_timeout(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_inactivity_timeout(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* inactivity_timeout);
       
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_retrans_interval(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_retrans_interval(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* retrans_interval);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_ack_interval(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_ack_interval(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* ack_interval);

    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_rm_assertion_get_is_exp_backoff(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_is_exp_backoff(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_bool_t is_exp_backoff);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_storage_mgr(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_storage_mgr(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* storage_mgr);
       
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_message_types_to_drop(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_message_types_to_drop(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* message_types_to_drop);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_max_retrans_count(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_max_retrans_count(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* max_retrans_count);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_sender_sleep_time(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_sender_sleep_time(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* sender_sleep_time);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_invoker_sleep_time(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_invoker_sleep_time(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* invoker_sleep_time);
       
    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_polling_wait_time(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_polling_wait_time(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* polling_wait_time);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_terminate_delay(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_terminate_delay(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* terminate_delay);

    AXIS2_EXTERN axis2_char_t* AXIS2_CALL
    axis2_rm_assertion_get_sandesha2_db(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_rm_assertion_set_sandesha2_db(
        axis2_rm_assertion_t *rm_assertion,
        const axutil_env_t * env,
        axis2_char_t* sandesha2_db);

    AXIS2_EXTERN axis2_rm_assertion_t* AXIS2_CALL
        axis2_rm_assertion_get_from_policy(
        const axutil_env_t *env,
        neethi_policy_t *policy);



#ifdef __cplusplus
}
#endif
#endif
