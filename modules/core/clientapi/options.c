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

#include <axis2_options.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axiom_soap_const.h>
#include <axis2_msg_info_headers.h>
#include <axis2_array_list.h>

typedef struct axis2_options_impl
{
    /** options base struct */
    axis2_options_t options;

    /** parent options */
    axis2_options_t *parent;

    axis2_hash_t *properties;

    axis2_char_t *soap_version_uri;
    
    axis2_endpoint_ref_t *target_epr;

    int soap_version;

    long timeout_in_milli_seconds;

    axis2_bool_t use_separate_listener;

    /** addressing specific properties */
    axis2_msg_info_headers_t *msg_info_headers;

    axis2_transport_receiver_t *receiver;

    axis2_transport_in_desc_t *transport_in;

    axis2_char_t *transport_in_protocol;

    /** for sending and receiving messages */
    axis2_transport_out_desc_t *transport_out;
    axis2_char_t *sender_transport_protocol;

    axis2_bool_t manage_session;
    axis2_bool_t enable_mtom;
}
axis2_options_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(options) ((axis2_options_impl_t *)options)

/** private function prototypes */
static void
axis2_options_init_data(
    axis2_options_impl_t *options);

static void
axis2_options_init_ops(
    axis2_options_t *options);


/** public function prototypes */
const axis2_char_t *AXIS2_CALL
axis2_options_get_action(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_fault_to(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_from(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_transport_receiver_t *AXIS2_CALL
axis2_options_get_transport_receiver(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_transport_in_desc_t *AXIS2_CALL
axis2_options_get_transport_in(
    const axis2_options_t *options,
    const axis2_env_t *env);

const axis2_char_t *AXIS2_CALL
axis2_options_get_transport_in_protocol(
    const axis2_options_t *options,
    const axis2_env_t *env);

const axis2_char_t *AXIS2_CALL
axis2_options_get_message_id(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_hash_t *AXIS2_CALL
axis2_options_get_properties(
    const axis2_options_t *options,
    const axis2_env_t *env);

void *AXIS2_CALL
axis2_options_get_property(
    const axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *key);

axis2_relates_to_t *AXIS2_CALL
axis2_options_get_relates_to(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_reply_to(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_transport_out_desc_t *AXIS2_CALL
axis2_options_get_transport_out(
    const axis2_options_t *options,
    const axis2_env_t *env);

const axis2_char_t *AXIS2_CALL
axis2_options_get_sender_transport_protocol(
    const axis2_options_t *options,
    const axis2_env_t *env);

const axis2_char_t *AXIS2_CALL
axis2_options_get_soap_version_uri(
    const axis2_options_t *options,
    const axis2_env_t *env);

long AXIS2_CALL
axis2_options_get_timeout_in_milli_seconds(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_to(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_target_epr(
    const axis2_options_t *options,
    const axis2_env_t *env);


axis2_bool_t AXIS2_CALL
axis2_options_get_use_separate_listener(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_options_t *AXIS2_CALL
axis2_options_get_parent(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_options_set_parent(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_options_t *parent);

axis2_status_t AXIS2_CALL
axis2_options_set_action(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *action);

axis2_status_t AXIS2_CALL
axis2_options_set_fault_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *fault_to);

axis2_status_t AXIS2_CALL
axis2_options_set_from(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *from);

axis2_status_t AXIS2_CALL
axis2_options_set_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *to);

axis2_status_t AXIS2_CALL
axis2_options_set_target_epr(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *epr);

axis2_status_t AXIS2_CALL
axis2_options_set_transport_receiver(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_transport_receiver_t *receiver);

axis2_status_t AXIS2_CALL
axis2_options_set_transport_in(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_transport_in_desc_t *transport_in);

axis2_status_t AXIS2_CALL
axis2_options_set_transport_in_protocol(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *transport_in_protocol);


axis2_status_t AXIS2_CALL
axis2_options_set_message_id(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *message_id);

axis2_status_t AXIS2_CALL
axis2_options_set_properties(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_hash_t *properties);

axis2_status_t AXIS2_CALL
axis2_options_set_property(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *property_key,
    const void *property);


axis2_status_t AXIS2_CALL
axis2_options_set_relates_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_relates_to_t *relates_to);

axis2_status_t AXIS2_CALL
axis2_options_set_reply_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *reply_to);

axis2_status_t AXIS2_CALL
axis2_options_set_transport_out(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_transport_out_desc_t *transport_out);


axis2_status_t AXIS2_CALL
axis2_options_set_sender_transport(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *sender_transport,
    axis2_conf_t *conf);

axis2_status_t AXIS2_CALL
axis2_options_set_soap_version_uri(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *soap_version_uri);

axis2_status_t AXIS2_CALL
axis2_options_set_timeout_in_milli_seconds(
    axis2_options_t *options,
    const axis2_env_t *env,
    long timeout_in_milli_seconds);

axis2_status_t AXIS2_CALL
axis2_options_set_transport_info(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *sender_transport,
    const axis2_char_t *receiver_transport,
    const axis2_bool_t use_separate_listener);

axis2_status_t AXIS2_CALL
axis2_options_set_use_separate_listener(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_bool_t use_separate_listener);

axis2_status_t AXIS2_CALL
axis2_options_add_reference_parameter(
    axis2_options_t *options,
    const axis2_env_t *env,
    axiom_node_t *reference_parameter);

axis2_status_t AXIS2_CALL
axis2_options_set_manage_session(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_bool_t manage_session);

axis2_bool_t AXIS2_CALL
axis2_options_get_manage_session(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_msg_info_headers_t *AXIS2_CALL
axis2_options_get_msg_info_headers(
    const axis2_options_t *options,
    const axis2_env_t *env);

int AXIS2_CALL
axis2_options_get_soap_version(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_options_set_soap_version(
    axis2_options_t *options,
    const axis2_env_t *env,
    int soap_version);

axis2_status_t AXIS2_CALL
axis2_options_free(
    axis2_options_t *options,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_options_set_enable_mtom(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_bool_t enable_mtom);

axis2_bool_t AXIS2_CALL
axis2_options_get_enable_mtom(
    const axis2_options_t *options,
    const axis2_env_t *env);

axis2_options_t *AXIS2_CALL
axis2_options_create(
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_MALLOC(env->allocator, sizeof(axis2_options_impl_t));
    if (!options_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /** initialize private data */
    axis2_options_init_data(options_impl);

    options_impl->msg_info_headers = axis2_msg_info_headers_create(env, NULL, NULL);
    if (!(options_impl->msg_info_headers))
    {
        axis2_options_free(&(options_impl->options), env);
        return NULL;
    }

    options_impl->properties = axis2_hash_make(env);
    if (!(options_impl->properties))
    {
        axis2_options_free(&(options_impl->options), env);
        return NULL;
    }

    /** initialize ops */
    options_impl->options.ops =
        AXIS2_MALLOC(env->allocator, sizeof(axis2_options_ops_t));

    if (!options_impl->options.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_options_free(&(options_impl->options), env);
        return NULL;
    }

    axis2_options_init_ops(&(options_impl->options));

    return &(options_impl->options);
}

axis2_options_t *AXIS2_CALL
axis2_options_create_with_parent(
    const axis2_env_t *env,
    axis2_options_t *parent)
{

    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(axis2_options_create(env));

    if (options_impl)
    {
        options_impl->parent = parent;
    }
    return &(options_impl->options);
}

const axis2_char_t *AXIS2_CALL
axis2_options_get_action(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    const axis2_char_t *action = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    action = AXIS2_MSG_INFO_HEADERS_GET_ACTION(options_impl->msg_info_headers, env);

    if (action == NULL && options_impl->parent)
    {
        return axis2_options_get_action(options_impl->parent, env);
    }

    return action;
}

axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_fault_to(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    fault_to = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(options_impl->msg_info_headers, env);

    if (fault_to == NULL && options_impl->parent)
    {
        return axis2_options_get_fault_to(options_impl->parent, env);
    }

    return fault_to;
}


axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_from(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    axis2_endpoint_ref_t *from = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    from = AXIS2_MSG_INFO_HEADERS_GET_FROM(options_impl->msg_info_headers, env);

    if (from == NULL && options_impl->parent)
    {
        return axis2_options_get_from(options_impl->parent, env);
    }

    return from;
}

axis2_transport_receiver_t *AXIS2_CALL
axis2_options_get_transport_receiver(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->receiver == NULL && options_impl->parent)
    {
        return axis2_options_get_transport_receiver(options_impl->parent, env);
    }

    return options_impl->receiver;
}


axis2_transport_in_desc_t *AXIS2_CALL
axis2_options_get_transport_in(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->transport_in == NULL && options_impl->parent)
    {
        return axis2_options_get_transport_in(options_impl->parent, env);
    }

    return options_impl->transport_in;
}

const axis2_char_t *AXIS2_CALL
axis2_options_get_transport_in_protocol(const axis2_options_t *options,
        const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->transport_in_protocol == NULL && options_impl->parent)
    {
        return axis2_options_get_transport_in_protocol(options_impl->parent, env);
    }

    return options_impl->transport_in_protocol;
}

const axis2_char_t *AXIS2_CALL
axis2_options_get_message_id(const axis2_options_t *options,
        const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    const axis2_char_t *message_id = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    message_id = AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(options_impl->msg_info_headers, env);

    if (message_id == NULL && options_impl->parent)
    {
        return axis2_options_get_message_id(options_impl->parent, env);
    }

    return message_id;
}

axis2_hash_t *AXIS2_CALL
axis2_options_get_properties(const axis2_options_t *options,
        const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (axis2_hash_count(options_impl->properties) == 0 && options_impl->parent)
    {
        return axis2_options_get_properties(options_impl->parent, env);
    }

    return options_impl->properties;
}

void *AXIS2_CALL
axis2_options_get_property(const axis2_options_t *options,
        const axis2_env_t *env,
        const axis2_char_t *key)
{
    void *property = NULL;
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);
    property = axis2_hash_get(options_impl->properties, key, AXIS2_HASH_KEY_STRING);

    if (property == NULL && options_impl->parent)
    {
        return axis2_options_get_property(options_impl->parent, env, key);
    }

    return property;
}

axis2_relates_to_t *AXIS2_CALL
axis2_options_get_relates_to(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    axis2_relates_to_t *relates_to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    relates_to = AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO(options_impl->msg_info_headers, env);

    if (relates_to == NULL && options_impl->parent)
    {
        return axis2_options_get_relates_to(options_impl->parent, env);
    }

    return relates_to;
}


axis2_endpoint_ref_t *AXIS2_CALL
axis2_options_get_reply_to(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    reply_to = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(options_impl->msg_info_headers, env);

    if (reply_to == NULL && options_impl->parent)
    {
        return axis2_options_get_reply_to(options_impl->parent, env);
    }

    return reply_to;
}

axis2_transport_out_desc_t *AXIS2_CALL
axis2_options_get_transport_out(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->transport_out == NULL && options_impl->parent)
    {
        return axis2_options_get_transport_out(options_impl->parent, env);
    }

    return options_impl->transport_out;
}

const axis2_char_t *AXIS2_CALL
axis2_options_get_sender_transport_protocol(
    const axis2_options_t *options,
    const axis2_env_t *env)
{

    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->sender_transport_protocol == NULL && options_impl->parent)
    {
        return axis2_options_get_sender_transport_protocol(options_impl->parent, env);
    }

    return options_impl->sender_transport_protocol;
}

const axis2_char_t *AXIS2_CALL
axis2_options_get_soap_version_uri(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->soap_version_uri == NULL && options_impl->parent)
    {
        return axis2_options_get_soap_version_uri(options_impl->parent, env);
    }

    if (options_impl->soap_version_uri)
    {
        return options_impl->soap_version_uri;
    }
    return AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;

}

long AXIS2_CALL
axis2_options_get_timeout_in_milli_seconds(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->timeout_in_milli_seconds == -1 && options_impl->parent)
    {
        return axis2_options_get_timeout_in_milli_seconds(options_impl->parent, env);
    }

    if (options_impl->timeout_in_milli_seconds == -1)
    {
        return AXIS2_DEFAULT_TIMEOUT_MILLISECONDS;
    }

    return options_impl->timeout_in_milli_seconds;
}


axis2_endpoint_ref_t* AXIS2_CALL
axis2_options_get_to(const axis2_options_t *options,
        const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    axis2_endpoint_ref_t *to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    to = AXIS2_MSG_INFO_HEADERS_GET_TO(options_impl->msg_info_headers, env);

    if (to == NULL && options_impl->parent)
    {
        return axis2_options_get_to(options_impl->parent, env);
    }

    return to;
}

axis2_endpoint_ref_t* AXIS2_CALL
axis2_options_get_target_epr(const axis2_options_t *options,
        const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    return options_impl->target_epr;
}


axis2_bool_t AXIS2_CALL
axis2_options_get_use_separate_listener(const axis2_options_t *options,
        const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    /*AXIS2_ENV_CHECK(env, NULL);*/

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->use_separate_listener == -1 && options_impl->parent)
    {
        return axis2_options_get_use_separate_listener(options_impl->parent, env);
    }

    if (options_impl->use_separate_listener == -1)
    {
        return AXIS2_FALSE;
    }

    return options_impl->use_separate_listener;
}

axis2_options_t *AXIS2_CALL
axis2_options_get_parent(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    return options_impl->parent;
}

axis2_status_t AXIS2_CALL
axis2_options_set_parent(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_options_t *parent)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    options_impl->parent = (axis2_options_t *)parent;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_action(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *action)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    AXIS2_MSG_INFO_HEADERS_SET_ACTION(options_impl->msg_info_headers, env, action);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_fault_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *fault_to)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(options_impl->msg_info_headers, env, fault_to);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_from(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *from)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    AXIS2_MSG_INFO_HEADERS_SET_FROM(options_impl->msg_info_headers, env, from);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *to)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    AXIS2_MSG_INFO_HEADERS_SET_TO(options_impl->msg_info_headers, env, to);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_target_epr(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *epr)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    options_impl->target_epr = epr;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_transport_receiver(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_transport_receiver_t *receiver)
{

    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    options_impl->receiver = receiver;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_transport_in(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_transport_in_desc_t *transport_in)
{

    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    options_impl->transport_in = transport_in;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_transport_in_protocol(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *transport_in_protocol)
{
    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    options_impl->transport_in_protocol = (axis2_char_t *)transport_in_protocol;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_options_set_message_id(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *message_id)
{

    axis2_options_impl_t *options_impl = NULL;
    options_impl = AXIS2_INTF_TO_IMPL(options);
    AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(options_impl->msg_info_headers, env, message_id);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_properties(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_hash_t *properties)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->properties)
    {
        axis2_hash_free(options_impl->properties, env);
    }
    options_impl->properties = properties;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_property(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *property_key,
    const void *property)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);
    axis2_hash_set(options_impl->properties, property_key,
            AXIS2_HASH_KEY_STRING, property);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_options_set_relates_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_relates_to_t *relates_to)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(options_impl->msg_info_headers, env, relates_to);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_reply_to(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_endpoint_ref_t *reply_to)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(options_impl->msg_info_headers, env, reply_to);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_transport_out(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_transport_out_desc_t *transport_out)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    options_impl->transport_out = transport_out;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_sender_transport(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *sender_transport,
    axis2_conf_t *conf)
{
    axis2_options_impl_t *options_impl = NULL;
    axis2_qname_t *tmp = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    tmp = axis2_qname_create(env, sender_transport, NULL, NULL);
    if (!tmp)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    options_impl->transport_out = AXIS2_CONF_GET_TRANSPORT_OUT(conf, env, tmp);
    AXIS2_QNAME_FREE(tmp, env);

    if (!(options_impl->transport_out))
    {
        /*TODO:error*/
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_soap_version_uri(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *soap_version_uri)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->soap_version_uri)
    {
        AXIS2_FREE(env->allocator, options_impl->soap_version_uri);
        options_impl->soap_version_uri = NULL;
    }

    if (soap_version_uri)
    {
        options_impl->soap_version_uri = AXIS2_STRDUP(soap_version_uri, env);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_timeout_in_milli_seconds(
    axis2_options_t *options,
    const axis2_env_t *env,
    long timeout_in_milli_seconds)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    options_impl->timeout_in_milli_seconds = timeout_in_milli_seconds;
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_options_set_transport_info(
    axis2_options_t *options,
    const axis2_env_t *env,
    const axis2_char_t *sender_transport,
    const axis2_char_t *receiver_transport,
    const axis2_bool_t use_separate_listener)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    options_impl = AXIS2_INTF_TO_IMPL(options);
    /**
      here we check for the legal combination
      */
    if (!use_separate_listener)
    {
        if (0 != strcmp(sender_transport, receiver_transport))
        {
            /*TODO:error*/
            return AXIS2_FAILURE;
        }
        /*TODO: check the transport is either HTTP or TCP*/
    }
    else
    {
        axis2_options_set_use_separate_listener(&(options_impl->options),
                env, use_separate_listener);
    }
    axis2_options_set_transport_in_protocol(&(options_impl->options), env, receiver_transport);
    options_impl->sender_transport_protocol = (axis2_char_t *)sender_transport;

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_use_separate_listener(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_bool_t use_separate_listener)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    options_impl->use_separate_listener = use_separate_listener;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_add_reference_parameter(
    axis2_options_t *options,
    const axis2_env_t *env,
    axiom_node_t *reference_parameter)
{

    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    AXIS2_MSG_INFO_HEADERS_ADD_REF_PARAM(options_impl->msg_info_headers,
            env, reference_parameter);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_options_set_manage_session(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_bool_t manage_session)
{
    axis2_options_impl_t *options_impl = NULL;

    options_impl = AXIS2_INTF_TO_IMPL(options);

    options_impl->manage_session = manage_session;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_options_get_manage_session(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->manage_session == -1 && options_impl->parent)
    {
        return axis2_options_get_manage_session(options_impl->parent, env);
    }

    if (options_impl->manage_session == -1)
    {
        return AXIS2_FALSE;
    }

    return options_impl->manage_session;
}

axis2_msg_info_headers_t *AXIS2_CALL
axis2_options_get_msg_info_headers(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    return options_impl->msg_info_headers;
}

axis2_status_t AXIS2_CALL
axis2_options_free(
    axis2_options_t *options,
    const axis2_env_t *env)
{
    axis2_options_impl_t *options_impl = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (options_impl->options.ops)
    {
        AXIS2_FREE(env->allocator, options_impl->options.ops);
        options_impl->options.ops = NULL;
    }

    if (options_impl->properties)
    {
        axis2_hash_free(options_impl->properties, env);
        options_impl->properties = NULL;
    }

    if (options_impl->soap_version_uri)
    {
        AXIS2_FREE(env->allocator, options_impl->soap_version_uri);
        options_impl->soap_version_uri = NULL;
    }

    if (options_impl->transport_in_protocol)
    {
        AXIS2_FREE(env->allocator, options_impl->transport_in_protocol);
        options_impl->transport_in_protocol = NULL;
    }

    if (options_impl->sender_transport_protocol)
    {
        AXIS2_FREE(env->allocator, options_impl->sender_transport_protocol);
        options_impl->sender_transport_protocol = NULL;
    }

    if (options_impl->msg_info_headers)
    {
        AXIS2_MSG_INFO_HEADERS_FREE(options_impl->msg_info_headers, env);
        options_impl->msg_info_headers = NULL;
    }

    AXIS2_FREE(env->allocator, options_impl);
    options_impl = NULL;

    return AXIS2_SUCCESS;
}

static void
axis2_options_init_data(
    axis2_options_impl_t *options_impl)
{
    options_impl->options.ops = NULL;
    options_impl->parent = NULL;
    options_impl->properties = NULL;
    options_impl->soap_version_uri = NULL;
    options_impl->timeout_in_milli_seconds = -1;
    options_impl->use_separate_listener = -1;

    options_impl->receiver = NULL;
    options_impl->transport_in = NULL;
    options_impl->transport_in_protocol = NULL;
    options_impl->transport_out = NULL;
    options_impl->sender_transport_protocol = NULL;
    options_impl->manage_session = -1;
    options_impl->soap_version = AXIOM_SOAP12;
    options_impl->enable_mtom = AXIS2_FALSE;
    options_impl->target_epr = NULL;
}

static void
axis2_options_init_ops(
    axis2_options_t *options)
{

    options->ops->get_action = axis2_options_get_action;
    options->ops->get_fault_to = axis2_options_get_fault_to;
    options->ops->get_from = axis2_options_get_from;
    options->ops->get_transport_receiver = axis2_options_get_transport_receiver;
    options->ops->get_transport_in = axis2_options_get_transport_in;
    options->ops->get_transport_in_protocol = axis2_options_get_transport_in_protocol;
    options->ops->get_message_id = axis2_options_get_message_id;
    options->ops->get_properties = axis2_options_get_properties;
    options->ops->get_property = axis2_options_get_property;
    options->ops->get_relates_to = axis2_options_get_relates_to;
    options->ops->get_reply_to = axis2_options_get_reply_to;
    options->ops->get_transport_out = axis2_options_get_transport_out;
    options->ops->get_sender_transport_protocol = axis2_options_get_sender_transport_protocol;
    options->ops->get_soap_version_uri = axis2_options_get_soap_version_uri;
    options->ops->get_timeout_in_milli_seconds = axis2_options_get_timeout_in_milli_seconds;
    options->ops->get_to = axis2_options_get_to;
    options->ops->get_use_separate_listener = axis2_options_get_use_separate_listener;
    options->ops->get_parent = axis2_options_get_parent;
    options->ops->set_parent = axis2_options_set_parent;
    options->ops->set_action = axis2_options_set_action;
    options->ops->set_fault_to = axis2_options_set_fault_to;
    options->ops->set_from = axis2_options_set_from;
    options->ops->set_to = axis2_options_set_to;
    options->ops->set_transport_receiver = axis2_options_set_transport_receiver;
    options->ops->set_transport_in = axis2_options_set_transport_in;
    options->ops->set_transport_in_protocol = axis2_options_set_transport_in_protocol;
    options->ops->set_message_id = axis2_options_set_message_id;
    options->ops->set_properties = axis2_options_set_properties;
    options->ops->set_property = axis2_options_set_property;
    options->ops->set_relates_to = axis2_options_set_relates_to;
    options->ops->set_reply_to = axis2_options_set_reply_to;
    options->ops->set_transport_out = axis2_options_set_transport_out;
    options->ops->set_sender_transport = axis2_options_set_sender_transport;
    options->ops->set_soap_version_uri = axis2_options_set_soap_version_uri;
    options->ops->set_timeout_in_milli_seconds = axis2_options_set_timeout_in_milli_seconds;
    options->ops->set_transport_info = axis2_options_set_transport_info;
    options->ops->set_use_separate_listener = axis2_options_set_use_separate_listener;
    options->ops->add_reference_parameter = axis2_options_add_reference_parameter;
    options->ops->set_manage_session = axis2_options_set_manage_session;
    options->ops->get_manage_session = axis2_options_get_manage_session;
    options->ops->get_msg_info_headers = axis2_options_get_msg_info_headers;
    options->ops->set_soap_version = axis2_options_set_soap_version;
    options->ops->get_soap_version = axis2_options_get_soap_version;
    options->ops->set_enable_mtom = axis2_options_set_enable_mtom;
    options->ops->get_enable_mtom = axis2_options_get_enable_mtom;
    options->ops->set_target_epr = axis2_options_set_target_epr;
    options->ops->get_target_epr = axis2_options_get_target_epr;
    options->ops->free = axis2_options_free;
}

int AXIS2_CALL
axis2_options_get_soap_version(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(options)->soap_version;

}

axis2_status_t AXIS2_CALL
axis2_options_set_soap_version(
    axis2_options_t *options,
    const axis2_env_t *env,
    int soap_version)
{
    axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    options_impl = AXIS2_INTF_TO_IMPL(options);

    if (soap_version == AXIOM_SOAP11)
    {
        options_impl->soap_version = soap_version;
        axis2_options_set_soap_version_uri(options, env,
                AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI);
    }
    else
    {
        options_impl->soap_version = AXIOM_SOAP12;
        axis2_options_set_soap_version_uri(options, env,
                AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_options_set_enable_mtom(
    axis2_options_t *options,
    const axis2_env_t *env,
    axis2_bool_t enable_mtom)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(options)->enable_mtom = enable_mtom;

    if (enable_mtom)
    {
        axis2_property_t *property = axis2_property_create(env);
        if (property)
        {
            AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
            AXIS2_PROPERTY_SET_VALUE(property, env, AXIS2_STRDUP(AXIS2_VALUE_TRUE, env));
            axis2_options_set_property(options, env, AXIS2_ENABLE_MTOM, property);
        }
    }
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_options_get_enable_mtom(
    const axis2_options_t *options,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(options)->enable_mtom;
}
