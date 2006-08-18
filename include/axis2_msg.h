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

#ifndef AXIS2_MSG_H
#define AXIS2_MSG_H

/** 
 * @defgroup axis2_msg message
 * @ingroup axis2_desc
 * @{
 */

/**
  * @file axis2_msg.h
  * @brief axis2 msg interface
  */

#include <axis2_param_container.h>
#include <axis2_op.h>
#include <axis2_array_list.h>
#include <axis2_description.h>
#include <axis2_phase_meta.h>

#define AXIS2_MSG_IN            "InMessage"
#define AXIS2_MSG_OUT           "OutMessage"
#define AXIS2_MSG_IN_FAULT      "InFaultMessage"
#define AXIS2_MSG_OUT_FAULT     "OutFaultMessage"


#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_msg_ops */ 
    typedef struct axis2_msg_ops axis2_msg_ops_t;
    /** Type name for struct axis2_msg */ 
    typedef struct axis2_msg axis2_msg_t;

    /**
     * message ops struct
     * Encapsulator struct for ops of axis2_msg
     */
    struct axis2_msg_ops
    {
        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @param name pointer to name
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env,
                    axis2_char_t *name);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_params)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @param param_name pointer to param name
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env,
                    axis2_char_t *param_name);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @param op pointer to operation
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env,
                    axis2_op_t *op);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         */
        axis2_op_t *(AXIS2_CALL *
                get_parent)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_flow)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @param flow pointer to flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_flow)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env,
                    axis2_array_list_t *flow);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_direction)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @param direction pointer to direction
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_direction)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env,
                    const axis2_char_t *direction);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_element_qname)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @param element_qname pointer to element qname
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_element_qname)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env,
                    axis2_qname_t *element_qname);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_name)(
                    axis2_msg_t *msg,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /** 
         * @param msg pointer to message
         * @param env pointer to environment struct
         */
        xml_schema_element_t *(AXIS2_CALL *
                get_schema_element)(
                    const axis2_msg_t *msg,
                    const axis2_env_t *env);
    };

    /**
     * message struct
     */
    struct axis2_msg
    {
        /** Operations of message struct */
        axis2_msg_ops_t *ops;
        struct axis2_param_container *param_container;
    };

    /**
     * Creates msg struct
     * @param env pointer to environment struct
     * @return pointer to newly created msg
     */
    AXIS2_EXTERN axis2_msg_t *AXIS2_CALL
    axis2_msg_create(
        const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Frees the message.
    @sa axis2_msg_ops#get_svc */
#define AXIS2_MSG_FREE(msg, env) \
        ((msg)->ops->free (msg, env))

/** Add param.
    @sa axis2_msg_ops#add_param */
#define AXIS2_MSG_ADD_PARAM(msg, env, param) \
      ((msg)->ops->add_param (msg, env, param))

/** Get param.
    @sa axis2_msg_ops#get_param */
#define AXIS2_MSG_GET_PARAM(msg, env, key) \
      ((msg)->ops->get_param (msg, env, key))

/** Get params.
    @sa axis2_msg_ops#get_params */
#define AXIS2_MSG_GET_PARAMS(msg, env) \
      ((msg)->ops->get_params (msg, env))

/** Is param locked.
    @sa axis2_msg_ops#is_param_locked */
#define AXIS2_MSG_IS_PARAM_LOCKED(msg, env, param_name) \
        ((msg)->ops->is_param_locked(msg, env, param_name))

/** Set parent.
    @sa axis2_msg_ops#set_parent */
#define AXIS2_MSG_SET_PARENT(msg, env, service_desc) \
        ((msg)->ops->set_parent (msg, env, service_desc))

/** Get parent.
    @sa axis2_msg_ops#get_parent */
#define AXIS2_MSG_GET_PARENT(msg, env) \
      ((msg)->ops->get_parent (msg, env))

/** Get flow.
    @sa axis2_msg_ops#get_flow */
#define AXIS2_MSG_GET_FLOW(msg, env) \
      ((msg)->ops->get_flow (msg, env))

/** Set flow.
    @sa axis2_msg_ops#set_flow */
#define AXIS2_MSG_SET_FLOW(msg, env, flow) \
      ((msg)->ops->set_flow (msg, env, flow))

/** Get direction.
    @sa axis2_msg_ops#get_direction */
#define AXIS2_MSG_GET_DIRECTION(msg, env) \
      ((msg)->ops->get_direction (msg, env))

/** Set direction.
    @sa axis2_msg_ops#set_direction */
#define AXIS2_MSG_SET_DIRECTION(msg, env, direction) \
      ((msg)->ops->set_direction (msg, env, direction))

/** Get element qname.
    @sa axis2_msg_ops#get_element_qname */
#define AXIS2_MSG_GET_ELEMENT_QNAME(msg, env) \
      ((msg)->ops->get_element_qname(msg, env))

/** Set element qname.
    @sa axis2_msg_ops#set_element_qname */
#define AXIS2_MSG_SET_ELEMENT_QNAME(msg, env, element_qname) \
      ((msg)->ops->set_element_qname(msg, env, element_qname))

/** Get name.
    @sa axis2_msg_ops#get_name */
#define AXIS2_MSG_GET_NAME(msg, env) \
      ((msg)->ops->get_name(msg, env))

/** Set name.
    @sa axis2_msg_ops#set_name */
#define AXIS2_MSG_SET_NAME(msg, env, name) \
      ((msg)->ops->set_name(msg, env, name))

/** Get schema element.
    @sa axis2_msg_ops#get_schema_element */
#define AXIS2_MSG_GET_SCHEMA_ELEMENT(msg, env) \
      ((msg)->ops->get_schema_element(msg, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_MSG_H */
