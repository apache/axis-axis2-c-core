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

#ifndef AXIS2_OP_H
#define AXIS2_OP_H

/** 
 * @defgroup axis2_op operation
 * @ingroup axis2_desc
 * @{
 */
/**
 * @file axis2_op.h
 * @brief axis2 op interface
 */

#include <axis2_param_container.h>
#include <axis2_wsdl_op.h>
#include <axis2_svc.h>
#include <axis2_msg_recv.h>
#include <axis2_array_list.h>
#include <axis2_module_desc.h>
#include <axis2_wsdl_msg_ref.h>
#include <axis2_description.h>
#include <axis2_phase_meta.h>
#include <axis2_relates_to.h>
#include <axis2_msg_ctx.h>
#include <axis2_op_ctx.h>
#include <axis2_svc_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif


    struct axis2_wsdl_op;
    struct axis2_svc;
    struct axis2_msg_recv;
    struct axis2_param_container;
    struct axis2_module_desc;
    struct axis2_op;
    struct axis2_wsdl_feature;
    struct axis2_wsdl_property;
    struct axis2_wsdl_fault_ref;
    struct axis2_relates_to;
    struct axis2_op_ctx;
    struct axis2_svc_ctx;
    struct axis2_msg_ctx;
    struct axis2_msg;
    struct axis2_conf;
    /** Type name for struct axis2_op_ops */
    typedef struct axis2_op_ops axis2_op_ops_t;
    /** Type name for struct axis2_op */
    typedef struct axis2_op axis2_op_t;

#define AXIS2_SOAP_ACTION "soapAction"

    /**
     * Operation ops struct
     * Encapsulator struct for ops of axis2_op
     */
    AXIS2_DECLARE_DATA struct axis2_op_ops
    {
        /**
         * De-allocate memory
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                free_void_arg)(
                    void *op,
                    const axis2_env_t *env);


        /**
         * Method addParameter
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param param Parameter that will be added
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Method getParameter
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param name Name of the parameter
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_params)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * To check whether a given paramter is locked
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param param_name pointer to param name
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_char_t *param_name);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param service_desc pointer to service description
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_svc *service_desc);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        struct axis2_svc *(AXIS2_CALL *
                get_parent)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param qname pointer to qname
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_qname_t *qname);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    void *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param pattern pointer to pattern
         */
        axis2_status_t (AXIS2_CALL *
                set_msg_exchange_pattern)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *pattern);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_msg_exchange_pattern)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param msg_recv pointer to message receiver
         */
        axis2_status_t (AXIS2_CALL *
                set_msg_recv)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_recv *msg_recv);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        struct axis2_msg_recv *(AXIS2_CALL *
                get_msg_recv)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_style)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param style pointer to style
         */
        axis2_status_t (AXIS2_CALL *
                set_style)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_char_t *style);

        /**
         * To ebgage a module it is reuired to use this method
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param moduleref pointer to module ref
         * @param conf pointer to configuration
         */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_module_desc *moduleref,
                    struct axis2_conf *conf);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param module_name pointer to module name
         */
        axis2_status_t (AXIS2_CALL *
                add_to_engage_module_list)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module_name);

        /**
         * Method getEngadgedModules
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_modules)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * This method will simply map the String URI of the Message exchange
         * pattern to a integer. Further in the first lookup it will cash the looked
         * up value so that the subsequent method calls will be extremely efficient.
         *
         */
        int (AXIS2_CALL *
                get_axis_specific_mep_const)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_phases_in_fault_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env);


        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_phases_out_fault_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_phases_outflow)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_remaining_phases_inflow)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                set_phases_in_fault_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param list pointer to list
         */
        axis2_status_t (AXIS2_CALL *
                set_phases_out_fault_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param list pointer to list
         */
        axis2_status_t (AXIS2_CALL *
                set_phases_outflow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);
        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param list pointer to list
         */
        axis2_status_t (AXIS2_CALL *
                set_remaining_phases_inflow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param module_name pointer to module name
         */
        axis2_status_t (AXIS2_CALL *
                add_module)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_qname_t *module_name);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_module_refs)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_in_faults)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param in_faults pointer to in faults
         */
        axis2_status_t (AXIS2_CALL *
                set_in_faults)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_linked_list_t *in_faults);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        struct axis2_wsdl_msg_ref *(AXIS2_CALL *
                get_input_msg)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param input_msg pointer to input message
         */
        axis2_status_t (AXIS2_CALL *
                set_input_msg)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_msg_ref *input_msg);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_out_faults)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param out_faults pointer to out faults
         */
        axis2_status_t (AXIS2_CALL *
                set_out_faults)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_linked_list_t *out_faults);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        struct axis2_wsdl_msg_ref *(AXIS2_CALL *
                get_output_msg)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param output_msg pointer to output message
         */
        axis2_status_t (AXIS2_CALL *
                set_output_msg)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_msg_ref *output_msg);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_bool_t (AXIS2_CALL *
                is_safe)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param safe safe
         */
        axis2_status_t (AXIS2_CALL *
                set_safety)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_bool_t safe);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_char_t *(AXIS2_CALL *
                get_target_namespace)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param in_fault pointer to in fault
         */
        axis2_status_t (AXIS2_CALL *
                add_in_fault)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_fault_ref *in_fault);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param out_fault pointer to out fault
         */
        axis2_status_t (AXIS2_CALL *
                add_out_fault)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_fault_ref *out_fault);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param feature pointer to feature
         */
        axis2_status_t (AXIS2_CALL *
                add_feature)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_feature *feature);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_features)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param wsdl_property pointer to wsdl property
         */
        axis2_status_t (AXIS2_CALL *
                add_property)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_property *wsdl_property);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_Properties)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param wsdl_op pointer to wsdl op
         */
        axis2_status_t (AXIS2_CALL *
                set_wsdl_op)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_op *wsdl_op);

        /**
         * This method is responsible for finding a MEPContext for an incomming
         * messages. An incomming message can be of two states.
         * <p/>
         * 1)This is a new incomming message of a given MEP. 2)This message is a
         * part of an MEP which has already begun.
         * <p/>
         * The method is special cased for the two MEPs
         * <p/>
         * #IN_ONLY #IN_OUT
         * <p/>
         * for two reasons. First reason is the wide usage and the second being that
         * the need for the MEPContext to be saved for further incomming messages.
         * <p/>
         * In the event that MEP of this op is different from the two MEPs
         * deafulted above the decession of creating a new or this message relates
         * to a MEP which already in business is decided by looking at the WSA
         * Relates TO of the incomming message.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @param svc_ctx pointer to service context
         */
        struct axis2_op_ctx *(AXIS2_CALL *
                find_op_ctx)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx,
                    struct axis2_svc_ctx *svc_ctx);

        /**
         * This will not create a new op context if there is no one already.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         */
        struct axis2_op_ctx *(AXIS2_CALL *
                find_for_existing_op_ctx)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        axis2_status_t (AXIS2_CALL *
                register_op_ctx)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx,
                    struct axis2_op_ctx *op_ctx);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         */
        struct axis2_msg *(AXIS2_CALL *
                get_msg)(
                    const axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *label);

        /**
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param label pointer to label
         * @param msg pointer to message 
         */
        axis2_status_t (AXIS2_CALL *
                add_msg)(
                    const axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *label,
                    const struct axis2_msg *msg);

    };

    /**
     * Operaton struct
     */
    AXIS2_DECLARE_DATA struct axis2_op
    {
        axis2_wsdl_op_t base;
        axis2_op_ops_t *ops;
        struct axis2_param_container *param_container;
    };

    /**
     * Creates op struct
     * @param env pointer to environment struct
     * @return pointer to newly created op
     */
    AXIS2_EXTERN axis2_op_t *AXIS2_CALL
    axis2_op_create(
        const axis2_env_t *env);

    /**
     * Creates op struct with name
     * @param env pointer to environment struct
     * @param name pointer to name
     * @return pointer to newly created op
     */
    AXIS2_EXTERN axis2_op_t *AXIS2_CALL
    axis2_op_create_with_qname(
        const axis2_env_t *env,
        axis2_qname_t *name);

    /**
     * Creates op struct with wsdl op
     * @param wsdl_op wsdl op
     * @return pointer to newly created op
     */
    AXIS2_EXTERN axis2_op_t *AXIS2_CALL
    axis2_op_create_with_wsdl_op(
        const axis2_env_t *env,
        struct axis2_wsdl_op *wsdl_op);

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_op_free_void_arg(
        void *op,
        const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Frees the operation.
    @sa axis2_op_ops#free */
#define AXIS2_OP_FREE(op, env) \
        ((op)->ops->free (op, env))

/** Adds the param.
    @sa axis2_op_ops#add_param */
#define AXIS2_OP_ADD_PARAM(op, env, param) \
      ((op)->ops->add_param (op, env, param))

/** Gets the param.
    @sa axis2_op_ops#get_param */
#define AXIS2_OP_GET_PARAM(op, env, key) \
      ((op)->ops->get_param (op, env, key))

/** Gets the params.
    @sa axis2_op_ops#get_params */
#define AXIS2_OP_GET_PARAMS(op, env) \
      ((op)->ops->get_params (op, env))

/** Is param locked.
    @sa axis2_op_ops#is_param_locked */
#define AXIS2_OP_IS_PARAM_LOCKED(op, env, param_name) \
        ((op)->ops->is_param_locked(op, env, param_name))

/** Set parent.
    @sa axis2_op_ops#set_parent */
#define AXIS2_OP_SET_PARENT(op, env, service_desc) \
        ((op)->ops->set_parent (op, env, service_desc))

/** Get parent.
    @sa axis2_op_ops#get_parent */
#define AXIS2_OP_GET_PARENT(op, env) \
      ((op)->ops->get_parent (op, env))

/** Sets the message receiver.
    @sa axis2_op_ops#set_msg_recv */
#define AXIS2_OP_SET_MSG_RECV(op, env, msg_recv) \
        ((op)->ops->set_msg_recv (op, env, msg_recv))

/** Gets the message receiver.
    @sa axis2_op_ops#get_msg_recv */
#define AXIS2_OP_GET_MSG_RECV(op, env) \
      ((op)->ops->get_msg_recv (op, env))

/** Sets the qname.
    @sa axis2_op_ops#set_qname */
#define AXIS2_OP_SET_QNAME(op, env, qname) \
      ((op)->ops->set_qname (op, env, qname))

/** Gets the qname.
    @sa axis2_op_ops#get_qname */
#define AXIS2_OP_GET_QNAME(op, env) \
      (((axis2_op_t *) op)->ops->get_qname (op, env))

/** Sets the message exchange pattern.
    @sa axis2_op_ops#set_msg_exchange_pattern */
#define AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op , env, \
        msg_exchange_pattern) \
      ((op)->ops->set_msg_exchange_pattern (op, env, \
        msg_exchange_pattern))

/** Gets the message exchange pattern.
    @sa axis2_op_ops#get_msg_exchange_pattern */
#define AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env) \
      ((op)->ops->get_msg_exchange_pattern (op, env))

/** Gets the style.
    @sa axis2_op_ops#get_style */
#define AXIS2_OP_GET_STYLE(op, env) \
      ((op)->ops->get_style (op, env))

/** Sets the style.
    @sa axis2_op_ops#set_style */
#define AXIS2_OP_SET_STYLE(op, env, style) \
      ((op)->ops->set_style (op, env, style))

/** Engages the module.
    @sa axis2_op_ops#engage_module */
#define AXIS2_OP_ENGAGE_MODULE(op, env, moduleref, conf) \
      ((op)->ops->engage_module (op, env, moduleref, conf))

/** Add to engage module list.
    @sa axis2_op_ops#add_to_engage_module_list */
#define AXIS2_OP_ADD_TO_ENGAGE_MODULE_LIST(op, env, module_name) \
      ((op)->ops->add_to_engage_module_list (op, env, module_name))

/** Get modules.
    @sa axis2_op_ops#get_modules */
#define AXIS2_OP_GET_MODULES(op, env) \
      ((op)->ops->get_modules (op, env))

/** Gets the axis specific mep const
    @sa axis2_op_ops#get_axis_specific_mep_const */
#define AXIS2_OP_GET_AXIS_SPECIFIC_MEP_CONST(op, env) \
      ((op)->ops->get_axis_specific_mep_const (op, env))

/** Gets the in fault flow.
    @sa axis2_op_ops#get_phases_in_fault_flow */
#define AXIS2_OP_GET_PHASES_IN_FAULT_FLOW(op, env) \
      ((op)->ops->get_phases_in_fault_flow (op, env))

/** Gets the phases out flow.
    @sa axis2_op_ops#get_phases_out_fault_flow */
#define AXIS2_OP_GET_PHASES_OUT_FAULT_FLOW(op, env) \
      ((op)->ops->get_phases_out_fault_flow (op, env))

/** Gets the phases outflow.
    @sa axis2_op_ops#get_phases_outflow */
#define AXIS2_OP_GET_PHASES_OUTFLOW(op, env) \
      ((op)->ops->get_phases_outflow (op, env))

/** Gets the remaining phases in flow.
    @sa axis2_op_ops#get_ramaining_phases_inflow */
#define AXIS2_OP_GET_REMAINING_PHASES_INFLOW(op, env) \
      ((op)->ops->get_remaining_phases_inflow (op, env))

/** Sets the phases in fault flow.
    @sa axis2_op_ops#set_phases_in_fault_flow */
#define AXIS2_OP_SET_PHASES_IN_FAULT_FLOW(op, env, list) \
      ((op)->ops->set_phases_in_fault_flow (op, env, list))

/** Sets the phases out fault flow.
    @sa axis2_op_ops#set_phases_out_fault_flow */
#define AXIS2_OP_SET_PHASES_OUT_FAULT_FLOW(op, env, list) \
      ((op)->ops->set_phases_out_fault_flow (op, env, list))

/** Sets the phases out flow.
    @sa axis2_op_ops#set_phases_outflow */
#define AXIS2_OP_SET_PHASES_OUTFLOW(op, env, list) \
      ((op)->ops->set_phases_outflow (op, env, list))

/** Sets the remaining phases in flow.
    @sa axis2_op_ops#set_remaining_phases_inflow */
#define AXIS2_OP_SET_REMAINING_PHASES_INFLOW(op, env, list) \
      ((op)->ops->set_remaining_phases_inflow (op, env, list))

/** Adds the module.
    @sa axis2_op_ops#add_module */
#define AXIS2_OP_ADD_MODULE(op, env, module_name) \
      ((op)->ops->add_module (op, env, module_name))

/** Gets the module refs.
    @sa axis2_op_ops#get_module_refs */
#define AXIS2_OP_GET_MODULE_REFS(op, env) \
      ((op)->ops->get_module_refs (op, env))

/** Gets the in faults.
    @sa axis2_op_ops#get_in_faults */
#define AXIS2_OP_GET_IN_FAULTS(op, env) \
      ((op)->ops->get_in_faults (op, env))

/** Sets the in faults.
    @sa axis2_op_ops#set_in_faults */
#define AXIS2_OP_SET_IN_FAULTS(op, env) \
      ((op)->ops->set_in_faults (op, env, in_faults))

/** Sets the input message.
    @sa axis2_op_ops#get_input_msg */
#define AXIS2_OP_GET_INPUT_MSG(op, env) \
      ((op)->ops->get_input_msg (op, env))

/** Sets the input message.
    @sa axis2_op_ops#set_input_msg */
#define AXIS2_OP_SET_INPUT_MSG(op, env) \
      ((op)->ops->set_input_msg (op, env, input_msg))

/** Sets the out faults.
    @sa axis2_op_ops#get_out_faults */
#define AXIS2_OP_GET_OUT_FAULTS(op, env) \
      ((op)->ops->get_out_faults (op, env))

/** Sets the out faults.
    @sa axis2_op_ops#set_out_faults */
#define AXIS2_OP_SET_OUT_FAULTS(op, env) \
      ((op)->ops->set_out_faults (op, env, out_faults))

/** Gets the output message.
    @sa axis2_op_ops#get_output_msg */
#define AXIS2_OP_GET_OUTPUT_MSG(op, env) \
      ((op)->ops->get_output_msg (op, env))

/** Sets the output message.
    @sa axis2_op_ops#set_output_msg */
#define AXIS2_OP_SET_OUTPUT_MSG(op, env) \
      ((op)->ops->set_output_msg (op, env, output_msg))

/** Is safe.
    @sa axis2_op_ops#is_safe */
#define AXIS2_OP_IS_SAFE(op, env) \
      ((op)->ops->is_safe (op, env))

/** Set safety.
    @sa axis2_op_ops#set_safety */
#define AXIS2_OP_SET_SAFETY(op, env, safe) \
      ((op)->ops->set_safety (op, env, safe))

/** Gets the target namespace.
    @sa axis2_op_ops#get_target_namespace */
#define AXIS2_OP_GET_TARGET_NAMESPACE(op, env) \
      ((op)->ops->get_target_namespace (op, env))

/** Adds the in fault.
    @sa axis2_op_ops#add_in_fault */
#define AXIS2_OP_ADD_IN_FAULT(op, env, in_fault) \
      ((op)->ops->add_in_fault (op, env, in_fault))

/** Adds the out fault.
    @sa axis2_op_ops#add_out_fault */
#define AXIS2_OP_ADD_OUT_FAULT(op, env, out_fault) \
      ((op)->ops->add_out_fault (op, env, out_fault))

/** Adds the feature.
    @sa axis2_op_ops#add_feature */
#define AXIS2_OP_ADD_FEATURE(op, env, feature) \
      ((op)->ops->add_feature (op, env, feature))

/** Gets the features.
    @sa axis2_op_ops#get_features */
#define AXIS2_OP_GET_FEATURES(op, env) \
      ((op)->ops->get_features(op, env))

/** Adds the property.
    @sa axis2_op_ops#add_property */
#define AXIS2_OP_ADD_PROPERTY(op, env, wsdl_property) \
      ((op)->ops->add_property (op, env, wsdl_property))

/** Gets the properties.
    @sa axis2_op_ops#get_properties */
#define AXIS2_OP_GET_PROPERTIES(op, env) \
      ((op)->ops->get_properties (op, env, properties))

/** Sets the wsdl operation.
    @sa axis2_op_ops#set_wsdl_op */
#define AXIS2_OP_SET_WSDL_OP(op, env, wsdl_op) \
      ((op)->ops->set_wsdl_op (op, env, wsdl_op))


/** Finds the operation context.
    @sa axis2_op_ops#find_op_ctx */
#define AXIS2_OP_FIND_OP_CTX(op, env, msg_ctx, svc_ctx) \
      ((op)->ops->find_op_ctx (op, env, msg_ctx, svc_ctx))

/** Finds for existing operation context.
    @sa axis2_op_ops#find_for_existing_op_ctx */
#define AXIS2_OP_FIND_FOR_EXISTING_OP_CTX(op, env, wsdl_op) \
      ((op)->ops->find_for_existing_op_ctx (op, env, wsdl_op))

/** Registers the operation conetxt.
    @sa axis2_op_ops#_register_op_ctx */
#define AXIS2_OP_REGISTER_OP_CTX(op, env, msg_ctx, wsdl_op) \
      ((op)->ops->register_op_ctx (op, env, msg_ctx, wsdl_op))

/** Gets the message.
    @sa axis2_op_ops#get_msg */
#define AXIS2_OP_GET_MSG(op, env, label) \
      ((op)->ops->get_msg(op, env, label))

/** Adds the message.
    @sa axis2_op_ops#add_msg */
#define AXIS2_OP_ADD_MSG(op, env, label, msg) \
      ((op)->ops->add_msg(op, env, label, msg))


/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_OP_H */
