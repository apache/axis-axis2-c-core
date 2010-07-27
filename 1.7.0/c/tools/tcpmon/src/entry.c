
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

#include <stdio.h>
#include <axutil_utils.h>
#include <axutil_error.h>
#include <axutil_string.h>
#include <axutil_network_handler.h>
#include <axutil_stream.h>
#include <time.h>

#include "tcpmon_entry_local.h"
#include "tcpmon_session_local.h"
#include "tcpmon_util.h"

#define AXIS2_TCPMON

/**
 * @brief
 */
typedef struct tcpmon_entry_impl
{
    tcpmon_entry_t entry;

    axis2_char_t *arrived_time;
    axis2_char_t *sent_time;
    axis2_char_t *sent_data;
    axis2_char_t *arrived_data;
    axis2_char_t *sent_headers;
    axis2_char_t *arrived_headers;
    axis2_bool_t is_success;
    axis2_char_t *time_diff;
    axis2_char_t *test_file_name;
    int format_bit;
    int sent_data_length;
    int arrived_data_length;
}
tcpmon_entry_impl_t;

#define AXIS2_INTF_TO_IMPL(entry) \
    ((tcpmon_entry_impl_t *) entry)

/************************* Function prototypes ********************************/


axis2_status_t AXIS2_CALL tcpmon_entry_free(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL tcpmon_entry_arrived_time(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL tcpmon_entry_sent_time(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL tcpmon_entry_time_diff(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL tcpmon_entry_sent_data(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL tcpmon_entry_sent_headers(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL tcpmon_entry_arrived_data(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *AXIS2_CALL tcpmon_entry_arrived_headers(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_bool_t AXIS2_CALL tcpmon_entry_is_success(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_char_t *get_current_stream_to_buffer(
    const axutil_env_t * env,
    axutil_stream_t * stream,
    int *stream_size);


int AXIS2_CALL tcpmon_entry_get_format_bit(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

int AXIS2_CALL tcpmon_entry_get_sent_data_length(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

int AXIS2_CALL tcpmon_entry_get_arrived_data_length(
    tcpmon_entry_t * entry,
    const axutil_env_t * env);

axis2_status_t AXIS2_CALL tcpmon_entry_set_format_bit(
    tcpmon_entry_t * entry,
    const axutil_env_t * env,
    int format_bit);

/************************** End of function prototypes ************************/

tcpmon_entry_t *AXIS2_CALL
tcpmon_entry_create(
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = (tcpmon_entry_impl_t *) AXIS2_MALLOC(env->
                                                      allocator,
                                                      sizeof
                                                      (tcpmon_entry_impl_t));

    if (!entry_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    entry_impl->arrived_time = AXIS2_MALLOC(env->allocator, 32);
    entry_impl->sent_time = AXIS2_MALLOC(env->allocator, 32);
    entry_impl->time_diff = AXIS2_MALLOC(env->allocator, 32);
    entry_impl->arrived_data = NULL;
    entry_impl->sent_data = NULL;
    entry_impl->arrived_headers = NULL;
    entry_impl->sent_headers = NULL;
    entry_impl->is_success = AXIS2_FALSE;
    entry_impl->format_bit = 0;
    entry_impl->sent_data_length = 0;
    entry_impl->arrived_data_length = 0;

    entry_impl->entry.ops =
        AXIS2_MALLOC(env->allocator, sizeof(tcpmon_entry_ops_t));
    if (!entry_impl->entry.ops)
    {
        tcpmon_entry_free(&(entry_impl->entry), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    entry_impl->entry.ops->free = tcpmon_entry_free;
    entry_impl->entry.ops->arrived_time = tcpmon_entry_arrived_time;
    entry_impl->entry.ops->sent_time = tcpmon_entry_sent_time;
    entry_impl->entry.ops->time_diff = tcpmon_entry_time_diff;
    entry_impl->entry.ops->sent_data = tcpmon_entry_sent_data;
    entry_impl->entry.ops->sent_headers = tcpmon_entry_sent_headers;
    entry_impl->entry.ops->arrived_data = tcpmon_entry_arrived_data;
    entry_impl->entry.ops->arrived_headers = tcpmon_entry_arrived_headers;
    entry_impl->entry.ops->is_success = tcpmon_entry_is_success;
    entry_impl->entry.ops->set_format_bit = tcpmon_entry_set_format_bit;
    entry_impl->entry.ops->get_format_bit = tcpmon_entry_get_format_bit;
    entry_impl->entry.ops->get_sent_data_length = tcpmon_entry_get_sent_data_length;
    entry_impl->entry.ops->get_arrived_data_length = tcpmon_entry_get_arrived_data_length;

    return &(entry_impl->entry);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
tcpmon_entry_free(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    if (entry->ops)
    {
        AXIS2_FREE(env->allocator, entry->ops);
        entry->ops = NULL;
    }

    if (entry_impl->arrived_time)
    {
        AXIS2_FREE(env->allocator, entry_impl->arrived_time);
        entry_impl->arrived_time = NULL;
    }
    if (entry_impl->sent_time)
    {
        AXIS2_FREE(env->allocator, entry_impl->sent_time);
        entry_impl->sent_time = NULL;
    }
    if (entry_impl->time_diff)
    {
        AXIS2_FREE(env->allocator, entry_impl->time_diff);
        entry_impl->time_diff = NULL;
    }
    if (entry_impl->arrived_data)
    {
        AXIS2_FREE(env->allocator, entry_impl->arrived_data);
        entry_impl->arrived_data = NULL;
    }
    if (entry_impl->sent_data)
    {
        AXIS2_FREE(env->allocator, entry_impl->sent_data);
        entry_impl->sent_data = NULL;
    }
    if (entry_impl->arrived_headers)
    {
        AXIS2_FREE(env->allocator, entry_impl->arrived_headers);
        entry_impl->arrived_headers = NULL;
    }
    if (entry_impl->sent_headers)
    {
        AXIS2_FREE(env->allocator, entry_impl->sent_headers);
        entry_impl->sent_headers = NULL;
    }

    if (entry_impl)
    {
        AXIS2_FREE(env->allocator, entry_impl);
        entry_impl = NULL;
    }

    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL
tcpmon_entry_arrived_time(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->arrived_time;
}

axis2_char_t *AXIS2_CALL
tcpmon_entry_sent_time(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->sent_time;
}

axis2_char_t *AXIS2_CALL
tcpmon_entry_time_diff(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->time_diff;
}

axis2_char_t *AXIS2_CALL
tcpmon_entry_sent_data(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->sent_data;
}

axis2_char_t *AXIS2_CALL
tcpmon_entry_sent_headers(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->sent_headers;
}

axis2_char_t *AXIS2_CALL
tcpmon_entry_arrived_data(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->arrived_data;
}

axis2_char_t *AXIS2_CALL
tcpmon_entry_arrived_headers(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->arrived_headers;
}

axis2_bool_t AXIS2_CALL
tcpmon_entry_is_success(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->is_success;
}

int AXIS2_CALL
tcpmon_entry_get_format_bit(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->format_bit;
}

int AXIS2_CALL
tcpmon_entry_get_sent_data_length(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->sent_data_length;
}

int AXIS2_CALL
tcpmon_entry_get_arrived_data_length(
    tcpmon_entry_t * entry,
    const axutil_env_t * env)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    return entry_impl->arrived_data_length;
}

axis2_status_t AXIS2_CALL
tcpmon_entry_set_format_bit(
    tcpmon_entry_t * entry,
    const axutil_env_t * env,
    int format_bit)
{
    tcpmon_entry_impl_t *entry_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    entry_impl->format_bit = format_bit;

    return AXIS2_SUCCESS;
}

/** implimentations for protected methods */

/** executes as new entry arises */
void *AXIS2_CALL
tcpmon_entry_new_entry_funct(
    axutil_thread_t * thd,
    void *data)
{
    tcpmon_entry_request_data_t *req_data = (tcpmon_entry_request_data_t *) data;
    const axutil_env_t *env = NULL;
    int client_socket = -1;
    int host_socket = -1;
    tcpmon_session_t *session;
    TCPMON_SESSION_TRANS_ERROR_FUNCT on_trans_fault_funct;
    TCPMON_SESSION_NEW_ENTRY_FUNCT on_new_entry;

    axutil_stream_t *client_stream = NULL;
    axutil_stream_t *host_stream = NULL;
    int buffer_size = 0;
    axis2_char_t *headers = NULL;
    axis2_char_t *content = NULL;
    axis2_char_t *buffer = NULL;
    time_t now;
    struct tm *localTime = NULL;
    int arrived_secs = 0;
    int sent_secs = 0;
    int time_diff_i = 0;
    int target_port = 0;
    axis2_char_t *target_host = NULL;
    int test_bit = 0;
    int format_bit = 0;
    tcpmon_entry_t *entry = NULL;
    tcpmon_entry_impl_t *entry_impl = NULL;

    env = req_data->env;
    client_socket = req_data->socket;
    session = req_data->session;
    on_trans_fault_funct = tcpmon_session_get_on_trans_fault(session, env);
    on_new_entry = tcpmon_session_get_on_new_entry(session, env);

    entry = tcpmon_entry_create(env);
    entry_impl = AXIS2_INTF_TO_IMPL(entry);

    target_port = TCPMON_SESSION_GET_TARGET_PORT(session, env);
    target_host = TCPMON_SESSION_GET_TARGET_HOST(session, env);
    if (target_port == -1 || target_host == NULL)
    {
        axutil_network_handler_close_socket(env, client_socket);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Missing target port and host"
                        "input missing");
        if (on_trans_fault_funct)
        {
            (on_trans_fault_funct) (env, "Missing target port and host");
        }
        if (thd)
        {
            AXIS2_FREE(env->allocator, thd);
        }
        if (data)
        {
            AXIS2_FREE(env->allocator, (tcpmon_entry_request_data_t *)data);
        }
        return NULL;
    }
    client_stream = axutil_stream_create_socket(env, client_socket);
    if (!client_stream)
    {
        axutil_network_handler_close_socket(env, client_socket);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Error in creating client stream" "handling response");

                          /** call the callback */
        if (on_trans_fault_funct)
        {
            (on_trans_fault_funct) (env, "error in creating the client stream");
        }
        if (thd)
        {
            AXIS2_FREE(env->allocator, thd);
        }
        if (data)
        {
            AXIS2_FREE(env->allocator, (tcpmon_entry_request_data_t *)data);
        }
        return NULL;
    }

    buffer = tcpmon_util_read_current_stream(env, client_stream, &buffer_size,
            &headers, &content);

    headers =(char *) tcpmon_util_str_replace(env, headers,"localhost", target_host);
    test_bit = TCPMON_SESSION_GET_TEST_BIT(session, env);

    if (test_bit)
    {
        tcpmon_util_write_to_file("reqest", buffer);
    }

    format_bit = TCPMON_SESSION_GET_FORMAT_BIT(session, env);
    TCPMON_ENTRY_SET_FORMAT_BIT(entry, env, format_bit);

    now = time(NULL);
    localTime = localtime(&now);

    sprintf(entry_impl->sent_time, "%d:%d:%d", localTime->tm_hour,
            localTime->tm_min, localTime->tm_sec);
    sent_secs =
        localTime->tm_hour * 60 * 60 + localTime->tm_min * 60 +
        localTime->tm_sec;

    /*free ( localTime); */

    entry_impl->sent_headers = headers; 
    entry_impl->sent_data = content;
    entry_impl->sent_data_length = buffer_size;

    if (on_new_entry)
    {
        (on_new_entry) (env, entry, 0);
    }

    host_socket =
        (int)axutil_network_handler_open_socket(env, target_host, target_port);
    if (-1 == host_socket)
    {
        axutil_stream_write(client_stream, env, NULL, 0);
        axutil_stream_free(client_stream, env);
        axutil_network_handler_close_socket(env, client_socket);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in creating host_socket"
                        "creating socket");

                          /** call the callback */
        if (on_trans_fault_funct)
        {
            (on_trans_fault_funct) (env, "error in creating the host socket");
        }
        if (thd)
        {
            AXIS2_FREE(env->allocator, thd);
        }
        if (data)
        {
            AXIS2_FREE(env->allocator, (tcpmon_entry_request_data_t *)data);
        }
        return NULL;
    }

    host_stream = axutil_stream_create_socket(env, host_socket);
    if (!host_stream)
    {
        axutil_stream_write(client_stream, env, NULL, 0);
        axutil_stream_free(client_stream, env);
        axutil_network_handler_close_socket(env, client_socket);
        axutil_network_handler_close_socket(env, host_socket);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error in creating host stream"
                        "handling response");

                          /** call the callback */
        if (on_trans_fault_funct)
        {
            (on_trans_fault_funct) (env, "error in creating the host stream");
        }
        if (thd)
        {
            AXIS2_FREE(env->allocator, thd);
        }
        if (data)
        {
            AXIS2_FREE(env->allocator, (tcpmon_entry_request_data_t *)data);
        }
        return NULL;
    }

    axutil_stream_write(host_stream, env, buffer, buffer_size);
    AXIS2_FREE(env->allocator, buffer);

    buffer = tcpmon_util_read_current_stream(env, host_stream, &buffer_size,
                                 &headers, &content);

    test_bit = TCPMON_SESSION_GET_TEST_BIT(session, env);
    if (test_bit)
    {
        tcpmon_util_write_to_file("response", buffer);
    }

    now = time(NULL);
    localTime = localtime(&now);

    sprintf(entry_impl->arrived_time, "%d:%d:%d", localTime->tm_hour,
            localTime->tm_min, localTime->tm_sec);
    arrived_secs =
        localTime->tm_hour * 60 * 60 + localTime->tm_min * 60 +
        localTime->tm_sec;
    /*free ( localTime); */

    time_diff_i = arrived_secs - sent_secs;
    if (time_diff_i < 0)
    {
        time_diff_i += 24 * 60 * 60;
    }
    sprintf(entry_impl->time_diff, "%d sec(s)", time_diff_i);

    entry_impl->arrived_headers = headers;
    entry_impl->arrived_data = content;
    entry_impl->arrived_data_length = buffer_size;
    if (buffer == NULL || buffer_size == 0)
    {
        entry_impl->is_success = 0;
    }
    else
    {
        entry_impl->is_success = 1;
    }

    if (on_new_entry)
    {
        (on_new_entry) (env, entry, 1);
    }

    axutil_stream_write(client_stream, env, buffer, buffer_size);
    AXIS2_FREE(env->allocator, buffer);

    axutil_stream_free(client_stream, env);
    axutil_stream_free(host_stream, env);
    axutil_network_handler_close_socket(env, client_socket);
    axutil_network_handler_close_socket(env, host_socket);

    if (entry_impl)
    {
        tcpmon_entry_free(&(entry_impl->entry), env);
    }
    if (thd)
    {
        AXIS2_FREE(env->allocator, thd);
    }
    if (data)
    {
        AXIS2_FREE(env->allocator, (tcpmon_entry_request_data_t *)data);
    }
    return NULL;
}


