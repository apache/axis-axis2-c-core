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
 * @file axis2_msg_info_headers.h
 * @brief Axis2 core addressing interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2.h>
#include <axis2_array_list.h>
#include <axis2_endpoint_ref.h>
#include <axis2_any_content_type.h>
#include <axis2_svc_name.h>
#include <axis2_relates_to.h>
#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

    struct axis2_msg_info_headers;
    struct axis2_msg_info_headers_ops;
    
/**
 * @defgroup axis2_msg_info_headers Addressing relates to header
 * @ingroup axis2_addr
 * @{
 */

/**
 *   \brief Dispatcher operations struct
 */
 AXIS2_DECLARE_DATA typedef struct axis2_msg_info_headers_ops
    { 
        /**
         * Method getTo
         *
         * @return
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *get_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env); 
        /**
         * Method set_to
         *
         * @param to
         */
        axis2_status_t (AXIS2_CALL *set_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_endpoint_ref_t *to);
        /**
         * Method getFrom
         *
         * @return
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *get_from)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
        /**
         * Method set_from
         *
         * @param from
         */
        axis2_status_t (AXIS2_CALL *set_from)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_endpoint_ref_t *from);
        /**
         * Method getReply_to
         *
         * @return
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *get_reply_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
        /**
         * Method set_reply_to
         *
         * @param reply_to
         */
        axis2_status_t (AXIS2_CALL *set_reply_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_endpoint_ref_t *reply_to);
        /**
         * Method getFault_to
         *
         * @return
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *get_fault_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
        /**
         * Method set_fault_to
         *
         * @param fault_to
         */
        axis2_status_t (AXIS2_CALL *set_fault_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_endpoint_ref_t *fault_to);
        /**
         * Method get_action
         *
         * @return
         */
        axis2_char_t* (AXIS2_CALL *get_action)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
        /**
         * Method set_action
         *
         * @param action
         */
        axis2_status_t (AXIS2_CALL *set_action)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_char_t *action);
        /**
         * Method get_message_id
         *
         * @return
         */
        axis2_char_t* (AXIS2_CALL *get_message_id)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
        /**
         * Method set_message_id
         *
         * @param message_id
         */
        axis2_status_t (AXIS2_CALL *set_message_id)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_char_t *message_id);
        /**
         * Method get_relates_to
         *
         * @return
         */
        axis2_relates_to_t* (AXIS2_CALL *get_relates_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
        /**
         * Method set_relates_to
         *
         * @param relates_to
         */
        axis2_status_t (AXIS2_CALL *set_relates_to)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_relates_to_t *relates_to);
        axis2_array_list_t* (AXIS2_CALL *get_ref_params)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
        axis2_status_t (AXIS2_CALL *add_ref_param)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env, axis2_om_node_t *ref_param);
        axis2_status_t (AXIS2_CALL *free)(struct axis2_msg_info_headers *msg_info_headers, 
                                                       axis2_env_t **env);
    } axis2_msg_info_headers_ops_t;
	
   /** 
    * \brief Dispatcher struct
    */
    typedef struct axis2_msg_info_headers
    {
        /** Dispatcher related operations */
        axis2_msg_info_headers_ops_t *ops;
    } axis2_msg_info_headers_t;


/**
 * creates msg_info_headers struct
 *
 * @param value
 * @param relationship_type
 */
AXIS2_DECLARE(axis2_msg_info_headers_t*) axis2_msg_info_headers_create(axis2_env_t **env, axis2_endpoint_ref_t *to,
                                           axis2_char_t *action);
    

#define AXIS2_MSG_INFO_HEADERS_GET_TO(msg_info_headers, env) ((msg_info_headers)->ops->get_to(msg_info_headers, env)) 
#define AXIS2_MSG_INFO_HEADERS_SET_TO(msg_info_headers, env, to) ((msg_info_headers)->ops->set_to(msg_info_headers, env, to))
#define AXIS2_MSG_INFO_HEADERS_GET_FROM(msg_info_headers, env) ((msg_info_headers)->ops->get_from(msg_info_headers, env))
#define AXIS2_MSG_INFO_HEADERS_SET_FROM(msg_info_headers, env, from) ((msg_info_headers)->ops->set_from(msg_info_headers, env, from))
#define AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(msg_info_headers, env) ((msg_info_headers)->ops->get_reply_to(msg_info_headers, env))
#define AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(msg_info_headers, env, reply_to) ((msg_info_headers)->ops->set_reply_to(msg_info_headers, env, reply_to))
#define AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(msg_info_headers, env) ((msg_info_headers)->ops->get_fault_to(msg_info_headers, env))
#define AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(msg_info_headers, env, fault_to) ((msg_info_headers)->ops->set_fault_to(msg_info_headers, env, fault_to))
#define AXIS2_MSG_INFO_HEADERS_GET_ACTION(msg_info_headers, env) ((msg_info_headers)->ops->get_action(msg_info_headers, env))
#define AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_info_headers, env, action) ((msg_info_headers)->ops->set_action(msg_info_headers, env, action))
#define AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(msg_info_headers, env) ((msg_info_headers)->ops->get_message_id(msg_info_headers, env))
#define AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(msg_info_headers, env, message_id) ((msg_info_headers)->ops->set_message_id(msg_info_headers, env, message_id))
#define AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO(msg_info_headers, env) ((msg_info_headers)->ops->get_relates_to(msg_info_headers, env))
#define AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(msg_info_headers, env, relates_to) ((msg_info_headers)->ops->set_relates_to(msg_info_headers, env, relates_to))
#define AXIS2_MSG_INFO_HEADERS_GET_REF_PARAMS(msg_info_headers, env) ((msg_info_headers)->ops->get_ref_params(msg_info_headers, env))
#define AXIS2_MSG_INFO_HEADERS_ADD_REF_PARAM(msg_info_headers, env, ref_param) ((msg_info_headers)->ops->add_ref_param(msg_info_headers, env, ref_param))
#define AXIS2_MSG_INFO_HEADERS_FREE(msg_info_headers, env) ((msg_info_headers)->ops->free(msg_info_headers, env))

/** @} */
    
#ifdef __cplusplus
}
#endif

#endif    /* AXIS2_MSG_INFO_HEADERS_H */
