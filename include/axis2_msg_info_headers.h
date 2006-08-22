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

#ifndef AXIS2_MSG_INFO_HEADERS_H
#define AXIS2_MSG_INFO_HEADERS_H

/**
 * @defgroup axis2_msg_info_headers message information headers
 * @ingroup axis2_addr
 * @{
 */

/**
 * @file axis2_msg_info_headers.h
 * @brief Axis2 core addressing interface
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
    /** Type name for struct axis2_msg_info_headers_ops  */
    typedef struct axis2_msg_info_headers_ops axis2_msg_info_headers_ops_t;


    /**
     * Dispatcher ops struct
     */
    struct axis2_msg_info_headers_ops
    {
        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_endpoint_ref_t *(AXIS2_CALL *
                get_to)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param to to
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_to)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *to);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_endpoint_ref_t *(AXIS2_CALL *
                get_from)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param from pointer to from
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_from)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *from);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_endpoint_ref_t *(AXIS2_CALL *
                get_reply_to)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param reply_to 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_reply_to)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *reply_to);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param none none
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_reply_to_none)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env, 
                    axis2_bool_t none);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                get_reply_to_none)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param anonymous anonymous
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_reply_to_anonymous)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env, 
                    axis2_bool_t anonymous);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                get_reply_to_anonymous)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_endpoint_ref_t *(AXIS2_CALL *
                get_fault_to)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param fault fault_to
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_to)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *fault_to);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param none none
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_to_none)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env, 
                    axis2_bool_t none);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                get_fault_to_none)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param anonymous anonymous 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_to_anonymous)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env, 
                    axis2_bool_t anonymous);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                get_fault_to_anonymous)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_action)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param action pointer to action
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_action)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    const axis2_char_t *action);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_message_id)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param message_id pointer to message id
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_message_id)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    const axis2_char_t *message_id);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_relates_to_t *(AXIS2_CALL *
                get_relates_to)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param relates_to relates_to
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_relates_to)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    axis2_relates_to_t *relates_to);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_ref_params)(
                    const axis2_msg_info_headers_t *msg_info_headers,
                    const axis2_env_t *env);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @param ref_param pointer to reference param
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */

        axis2_status_t (AXIS2_CALL *
                add_ref_param)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env,
                    axiom_node_t *ref_param);

        /**
         * @param msg_info_headers pointer to message info headers
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    struct axis2_msg_info_headers *msg_info_headers,
                    const axis2_env_t *env);

    };

    /**
     * Dispatcher struct
     */
    struct axis2_msg_info_headers
    {
        /** Dispatcher related ops */
        axis2_msg_info_headers_ops_t *ops;
    };


    /**
     * creates msg_info_headers struct
     * @param env pointer to environment struct
     * @param to to 
     * @param action pointer to action
     */
    AXIS2_EXTERN axis2_msg_info_headers_t *AXIS2_CALL
    axis2_msg_info_headers_create(
        const axis2_env_t *env,
        axis2_endpoint_ref_t *to,
        const axis2_char_t *action);


/** Get to.
    @sa axis2_msg_info_headers_ops#get_to */
#define AXIS2_MSG_INFO_HEADERS_GET_TO(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_to(msg_info_headers, env))

/** Set to.
    @sa axis2_msg_info_headers_ops#set_to */
#define AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, to) \
        ((msg_info_headers)->ops->set_to(msg_info_headers, env, to))

/** Get from.
    @sa axis2_msg_info_headers_ops#get_from */
#define AXIS2_MSG_INFO_HEADERS_GET_FROM(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_from(msg_info_headers, env))

/** Set from.
    @sa axis2_msg_info_headers_ops#set_from */
#define AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, from) \
        ((msg_info_headers)->ops->set_from(msg_info_headers, env, from))

/** Get reply to.
    @sa axis2_msg_info_headers_ops#get_reply_to */
#define AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_reply_to(msg_info_headers, env))

/** Set reply to.
    @sa axis2_msg_info_headers_ops#set_reply_to */
#define AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(msg_info_headers, env, reply_to) \
        ((msg_info_headers)->ops->set_reply_to(msg_info_headers, env, reply_to))

/** Set reply to none.
    @sa axis2_msg_info_headers_ops#set_reply_to_none */
#define AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO_NONE(msg_info_headers, env, none) \
        ((msg_info_headers)->ops->set_reply_to_none(msg_info_headers, env, none))

/** Get reply to none.
    @sa axis2_msg_info_headers_ops#get_reply_to_none */
#define AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO_NONE(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_reply_to_none(msg_info_headers, env))

/** Set reply to anonymous.
    @sa axis2_msg_info_headers_ops#set_reply_to_anonymous */
#define AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO_ANONYMOUS(msg_info_headers, env, anonymous) \
        ((msg_info_headers)->ops->set_reply_to_anonymous(msg_info_headers, env, anonymous))

/** Get reply to anonymous.
    @sa axis2_msg_info_headers_ops#get_reply_to_anonymous */
#define AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO_ANONYMOUS(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_reply_to_anonymous(msg_info_headers, env))

/** Get fault to.
    @sa axis2_msg_info_headers_ops#get_fault_to */
#define AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_fault_to(msg_info_headers, env))

/** Set fault to.
    @sa axis2_msg_info_headers_ops#set_fault_to */
#define AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, fault_to) \
        ((msg_info_headers)->ops->set_fault_to(msg_info_headers, env, fault_to))

/** Set fault to none.
    @sa axis2_msg_info_headers_ops#set_fault_to_none */
#define AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO_NONE(msg_info_headers, env, none) \
        ((msg_info_headers)->ops->set_fault_to_none(msg_info_headers, env, none))

/** Get fault to none.
    @sa axis2_msg_info_headers_ops#get_fault_to_none */
#define AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO_NONE(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_fault_to_none(msg_info_headers, env))

/** Set fault to anonymous.
    @sa axis2_msg_info_headers_ops#set_fault_to_anonymous */
#define AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO_ANONYMOUS(msg_info_headers, env, anonymous) \
        ((msg_info_headers)->ops->set_fault_to_anonymous(msg_info_headers, env, anonymous))

/** Get fault to anonymous.
    @sa axis2_msg_info_headers_ops#get_fault_to_anonymous */
#define AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO_ANONYMOUS(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_fault_to_anonymous(msg_info_headers, env))

/** Get action.
    @sa axis2_msg_info_headers_ops#get_action */
#define AXIS2_MSG_INFO_HEADERS_GET_ACTION(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_action(msg_info_headers, env))

/** Set action.
    @sa axis2_msg_info_headers_ops#set_action */
#define AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, action) \
        ((msg_info_headers)->ops->set_action(msg_info_headers, env, action))

/** Get message id.
    @sa axis2_msg_info_headers_ops#get_message_id */
#define AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_message_id(msg_info_headers, env))

/** Set message id.
    @sa axis2_msg_info_headers_ops#set_message_id */
#define AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(msg_info_headers, env, message_id) \
        ((msg_info_headers)->ops->set_message_id(msg_info_headers, env, message_id))

/** Get relates to.
    @sa axis2_msg_info_headers_ops#get_relates_to */
#define AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_relates_to(msg_info_headers, env))

/** Set relates to.
    @sa axis2_msg_info_headers_ops#set_relates_to */
#define AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(msg_info_headers, env, relates_to) \
        ((msg_info_headers)->ops->set_relates_to(msg_info_headers, env, relates_to))

/** Get ref params.
    @sa axis2_msg_info_headers_ops#get_ref_params */
#define AXIS2_MSG_INFO_HEADERS_GET_REF_PARAMS(msg_info_headers, env) \
        ((msg_info_headers)->ops->get_ref_params(msg_info_headers, env))

/** Add ref param.
    @sa axis2_msg_info_headers_ops#add_ref_param */
#define AXIS2_MSG_INFO_HEADERS_ADD_REF_PARAM(msg_info_headers, env, ref_param) \
        ((msg_info_headers)->ops->add_ref_param(msg_info_headers, env, ref_param))

/** Frees the message info headers.
    @sa axis2_msg_info_headers_ops#free */
#define AXIS2_MSG_INFO_HEADERS_FREE(msg_info_headers, env) \
        ((msg_info_headers)->ops->free(msg_info_headers, env))

/** @} */

#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_MSG_INFO_HEADERS_H */
