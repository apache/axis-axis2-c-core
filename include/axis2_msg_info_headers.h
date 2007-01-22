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

#ifndef AXIS2_MSG_INFO_HEADERS_H
#define AXIS2_MSG_INFO_HEADERS_H

/**
 * @defgroup axis2_msg_info_headers message information headers
 * @ingroup axis2_addr
 * message information headers encapsulates properties that enable the 
 * identification and location of the endpoints involved in an interaction. 
 * The basic interaction pattern from which all others are composed is 
 * "one way". In this pattern a source sends a message to a destination 
 * without any further definition of the interaction. "Request Reply" is a 
 * common interaction pattern that consists of an initial message sent by 
 * a source endpoint (the request) and a subsequent message sent from the 
 * destination of the request back to the source (the reply). 
 * A reply can be either an application message, a fault, or any other message.
 * message information headers capture addressing information related to these
 * interaction patterns such as from, to, reply to and fault to addresses. 
 * @{
 */

/**
 * @file axis2_msg_info_headers.h
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_const.h>
#include <axis2_array_list.h>
#include <axis2_endpoint_ref.h>
#include <axis2_any_content_type.h>
#include <axis2_svc_name.h>
#include <axis2_relates_to.h>
#include <axiom_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_msg_info_headers */
    typedef struct axis2_msg_info_headers axis2_msg_info_headers_t;

    /**
     * Creates message information headers struct.
     * @param env pointer to environment struct
     * @param to pointer to endpoint reference representing to endpoint
     * @param action WS-Addressing action string
     * @return pointer to newly created message information headers struct
     */
    AXIS2_EXTERN axis2_msg_info_headers_t *AXIS2_CALL
    axis2_msg_info_headers_create(
        const axis2_env_t *env,
        axis2_endpoint_ref_t *to,
        const axis2_char_t *action);

    /**
     * Gets to endpoint. to endpoint represents the address of the 
     * intended receiver of this message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference representing to address, 
     * returns a reference, not a cloned copy
     */
    AXIS2_EXTERN axis2_endpoint_ref_t *AXIS2_CALL
    axis2_msg_info_headers_get_to(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets to endpoint. to endpoint represents the address of the 
     * intended receiver of this message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param to pointer to endpoint reference representing to address, 
     * message information headers assumes ownership of the endpoint
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_to(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        axis2_endpoint_ref_t *to);

    /**
     * Gets from endpoint. from endpoint represents the address of the 
     * endpoint where the message originated from.         
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference representing from address, 
     * returns a reference, not a cloned copy
     */
    AXIS2_EXTERN axis2_endpoint_ref_t *AXIS2_CALL
    axis2_msg_info_headers_get_from(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets from endpoint. from endpoint represents the address of the 
     * endpoint where the message originated from.         
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param from pointer to endpoint reference representing from address, 
     * message information headers assumes ownership of the endpoint
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_from(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        axis2_endpoint_ref_t *from);

    /**
     * Gets reply to endpoint. reply to endpoint identifies the intended 
     * receiver for replies to a message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference representing reply to address, 
     * returns a reference, not a cloned copy
     */
    AXIS2_EXTERN axis2_endpoint_ref_t *AXIS2_CALL
    axis2_msg_info_headers_get_reply_to(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets reply to endpoint. reply to endpoint identifies the intended 
     * receiver for replies to a message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param reply_to pointer to endpoint reference representing reply to 
     * address, message information headers assumes ownership of the endpoint
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_reply_to(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        axis2_endpoint_ref_t *reply_to);

    /**
     * Sets the bool value indicating whether the reply to endpoint should 
     * be none. reply to endpoint identifies the intended receiver for 
     * replies to a message. The URI "http://www.w3.org/2005/08/addressing/none"
     * in the reply to address indicates that no reply should be sent.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param none AXIS2_TRUE if http://www.w3.org/2005/08/addressing/none
     * is to be used as reply to URI, else AXIS2_FALSE
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_reply_to_none(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env, 
        const axis2_bool_t none);

    /**
     * Gets the bool value indicating whether the reply to endpoint should 
     * be none. reply to endpoint identifies the intended receiver for 
     * replies related to a message. The URI 
     * "http://www.w3.org/2005/08/addressing/none"
     * in the reply to address indicates that no reply should be sent.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return AXIS2_TRUE if http://www.w3.org/2005/08/addressing/none
     * is to be used as reply to URI, else AXIS2_FALSE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_msg_info_headers_get_reply_to_none(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets the bool value indicating whether the reply to endpoint should 
     * be anonymous. reply to endpoint identifies the intended receiver for 
     * replies related to a message. The URI 
     * "http://www.w3.org/2005/08/addressing/anonymous"
     * in the reply to address indicates that reply should be sent to 
     * from address.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param anonymous AXIS2_TRUE if 
     * http://www.w3.org/2005/08/addressing/anonymous
     * is to be used as reply to URI, else AXIS2_FALSE
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_reply_to_anonymous(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env, 
        const axis2_bool_t anonymous);

    /**
     * Gets the bool value indicating whether the reply to endpoint should 
     * be anonymous. reply to endpoint identifies the intended receiver for 
     * replies related to a message. The URI 
     * "http://www.w3.org/2005/08/addressing/anonymous"
     * in the reply to address indicates that reply should be sent to 
     * from address.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return AXIS2_TRUE if http://www.w3.org/2005/08/addressing/anonymous
     * is to be used as reply to URI, else AXIS2_FALSE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_msg_info_headers_get_reply_to_anonymous(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Gets fault to endpoint. fault to endpoint identifies the intended 
     * receiver for faults related to a message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference representing fault to address, 
     * returns a reference, not a cloned copy         
     */
    AXIS2_EXTERN axis2_endpoint_ref_t *AXIS2_CALL
    axis2_msg_info_headers_get_fault_to(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets fault to endpoint. fault to endpoint identifies the intended 
     * receiver for faults related to a message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param fault pointer to endpoint reference representing fault to 
     * address, message information headers assumes ownership of the endpoint
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_fault_to(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        axis2_endpoint_ref_t *fault_to);

    /**
     * Sets the bool value indicating whether the fault to endpoint should 
     * be none. fault to endpoint identifies the intended receiver for 
     * faults related to a message. The URI 
     * "http://www.w3.org/2005/08/addressing/none"
     * in the fault to address indicates that no fault should be sent back.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param none AXIS2_TRUE if http://www.w3.org/2005/08/addressing/none
     * is to be used as fault to URI, else AXIS2_FALSE
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_fault_to_none(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env, 
        const axis2_bool_t none);

    /**
     * Gets the bool value indicating whether the fault to endpoint should 
     * be none. fault to endpoint identifies the intended receiver for 
     * faults related to a message. The URI 
     * "http://www.w3.org/2005/08/addressing/none"
     * in the fault to address indicates that no fault should be sent back.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return AXIS2_TRUE if http://www.w3.org/2005/08/addressing/none
     * is to be used as fault to URI, else AXIS2_FALSE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_msg_info_headers_get_fault_to_none(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets the bool value indicating whether the fault to endpoint should 
     * be anonymous. fault to endpoint identifies the intended receiver for 
     * faults related to a message. The URI 
     * "http://www.w3.org/2005/08/addressing/anonymous"
     * in the fault to address indicates that fault should be sent to 
     * from address.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param anonymous AXIS2_TRUE if 
     * http://www.w3.org/2005/08/addressing/anonymous
     * is to be used as fault to URI, else AXIS2_FALSE
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_fault_to_anonymous(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env, 
        const axis2_bool_t anonymous);

    /**
     * Gets the bool value indicating whether the fault to endpoint should 
     * be anonymous. fault to endpoint identifies the intended receiver for 
     * faults related to a message. The URI 
     * "http://www.w3.org/2005/08/addressing/anonymous"
     * in the fault to address indicates that fault should be sent to 
     * from address.         
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return AXIS2_TRUE if http://www.w3.org/2005/08/addressing/anonymous
     * is to be used as fault to URI, else AXIS2_FALSE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_msg_info_headers_get_fault_to_anonymous(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Gets WS-Addressing action. action is an absolute IRI 
     * (Internationalized Resource Identifier) that uniquely identifies 
     * the semantics implied by this message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return WS-Addressing action string
     */
    AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axis2_msg_info_headers_get_action(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets WS-Addressing action. action is an absolute IRI 
     * (Internationalized Resource Identifier) that uniquely identifies 
     * the semantics implied by this message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param action WS-Addressing action string
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_action(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        const axis2_char_t *action);

    /**
     * Gets message ID. message ID is an absolute IRI that uniquely 
     * identifies the message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return message ID string. 
     */
    AXIS2_EXTERN const axis2_char_t *AXIS2_CALL
    axis2_msg_info_headers_get_message_id(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets message ID. message ID is an absolute IRI that uniquely 
     * identifies the message.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param message_id message ID string
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_message_id(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        const axis2_char_t *message_id);

    /**
     * Gets relates to information. 
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return pointer to relates to struct, returns a reference, not a 
     * cloned copy
     * @sa axis2_relates_to
     */
    AXIS2_EXTERN axis2_relates_to_t *AXIS2_CALL
    axis2_msg_info_headers_get_relates_to(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Sets relates to information. 
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param relates_to pointer to relates to struct, message information
     * headers assumes ownership of struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_set_relates_to(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        axis2_relates_to_t *relates_to);

    /**
     * Gets all reference parameters associated with message information 
     * headers.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return pointer to array list containing all reference parameters
     */
    AXIS2_EXTERN axis2_array_list_t *AXIS2_CALL
    axis2_msg_info_headers_get_all_ref_params(const axis2_msg_info_headers_t *msg_info_headers,
        const axis2_env_t *env);

    /**
     * Adds a reference parameter in the form of an AXIOM node.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @param ref_param pointer to AXIOM node representing reference 
     * parameter, message information header does not assume ownership of
     * node
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_add_ref_param(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env,
        axiom_node_t *ref_param);

    /**
     * Frees message information header struct.
     * @param msg_info_headers pointer to message information headers struct
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_msg_info_headers_free(struct axis2_msg_info_headers *msg_info_headers,
        const axis2_env_t *env);

/** Gets to endpoint. */
#define AXIS2_MSG_INFO_HEADERS_GET_TO(msg_info_headers, env) \
        axis2_msg_info_headers_get_to(msg_info_headers, env)

/** Sets to endpoint. */
#define AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, to) \
        axis2_msg_info_headers_set_to(msg_info_headers, env, to)

/** Gets from endpoint. */
#define AXIS2_MSG_INFO_HEADERS_GET_FROM(msg_info_headers, env) \
        axis2_msg_info_headers_get_from(msg_info_headers, env)

/** Sets from endpoint. */
#define AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, from) \
        axis2_msg_info_headers_set_from(msg_info_headers, env, from)

/** Gets reply to endpoint. */
#define AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(msg_info_headers, env) \
        axis2_msg_info_headers_get_reply_to(msg_info_headers, env)

/** Sets reply to endpoint. */
#define AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(msg_info_headers, env, reply_to) \
        axis2_msg_info_headers_set_reply_to(msg_info_headers, env, reply_to)

/** Sets bool value indicating if reply to is none. */
#define AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO_NONE(msg_info_headers, env, none) \
        axis2_msg_info_headers_set_reply_to_none(msg_info_headers, env, none)

/** Gets bool value indicating if reply to is none. */
#define AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO_NONE(msg_info_headers, env) \
        axis2_msg_info_headers_get_reply_to_none(msg_info_headers, env)

/** Sets bool value indicating if reply to is anonymous. */
#define AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO_ANONYMOUS(msg_info_headers, env, anonymous) \
        axis2_msg_info_headers_set_reply_to_anonymous(msg_info_headers, env, anonymous)

/** Gets bool value indicating if reply to is anonymous. */
#define AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO_ANONYMOUS(msg_info_headers, env) \
        axis2_msg_info_headers_get_reply_to_anonymous(msg_info_headers, env)

/** Gets fault to endpoint. */
#define AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(msg_info_headers, env) \
        axis2_msg_info_headers_get_fault_to(msg_info_headers, env)

/** Sets fault to endpoint. */
#define AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, fault_to) \
        axis2_msg_info_headers_set_fault_to(msg_info_headers, env, fault_to)

/** Sets bool value indicating if fault to is none. */
#define AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO_NONE(msg_info_headers, env, none) \
        axis2_msg_info_headers_set_fault_to_none(msg_info_headers, env, none)

/** Gets bool value indicating if fault to is none. */
#define AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO_NONE(msg_info_headers, env) \
        axis2_msg_info_headers_get_fault_to_none(msg_info_headers, env)

/** Sets bool value indicating if fault to is anonymous. */
#define AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO_ANONYMOUS(msg_info_headers, env, anonymous) \
        axis2_msg_info_headers_set_fault_to_anonymous(msg_info_headers, env, anonymous)

/** Gets bool value indicating if fault to is anonymous. */
#define AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO_ANONYMOUS(msg_info_headers, env) \
        axis2_msg_info_headers_get_fault_to_anonymous(msg_info_headers, env)

/** Gets WS-Addressing action. */
#define AXIS2_MSG_INFO_HEADERS_GET_ACTION(msg_info_headers, env) \
        axis2_msg_info_headers_get_action(msg_info_headers, env)

/** Sets WS-Addressing action. */
#define AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, action) \
        axis2_msg_info_headers_set_action(msg_info_headers, env, action)

/** Gets message ID. */
#define AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(msg_info_headers, env) \
        axis2_msg_info_headers_get_message_id(msg_info_headers, env)

/** Sets message ID. */
#define AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(msg_info_headers, env, message_id) \
        axis2_msg_info_headers_set_message_id(msg_info_headers, env, message_id)

/** Gets relates to information for the message. */
#define AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO(msg_info_headers, env) \
        axis2_msg_info_headers_get_relates_to(msg_info_headers, env)

/** Sets relates to information for the message. */
#define AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(msg_info_headers, env, relates_to) \
        axis2_msg_info_headers_set_relates_to(msg_info_headers, env, relates_to)

/** Gets all reference parameters. */
#define AXIS2_MSG_INFO_HEADERS_GET_ALL_REF_PARAMS(msg_info_headers, env) \
        axis2_msg_info_headers_get_all_ref_params(msg_info_headers, env)

/** Adds reference parameter. */
#define AXIS2_MSG_INFO_HEADERS_ADD_REF_PARAM(msg_info_headers, env, ref_param) \
        axis2_msg_info_headers_add_ref_param(msg_info_headers, env, ref_param)

/** Frees message information headers struct. */
#define AXIS2_MSG_INFO_HEADERS_FREE(msg_info_headers, env) \
        axis2_msg_info_headers_free(msg_info_headers, env)

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_MSG_INFO_HEADERS_H */
