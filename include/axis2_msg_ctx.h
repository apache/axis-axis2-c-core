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

#ifndef AXIS2_MSG_CTX_H
#define AXIS2_MSG_CTX_H

/** @defgroup axis2_msg_ctx message context
 * @ingroup axis2_context
 * Descripton.
 * @{
 */

/**
 * @file axis2_msg_ctx.h
 * @brief axis2 Message Context interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_ctx.h>
#include <axis2_relates_to.h>
#include <axis2_param.h>
#include <axis2_handler_desc.h>
#include <axis2_qname.h>
#include <axis2_msg_info_headers.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** axis2 transport headers */
#define AXIS2_TRANSPORT_HEADERS "AXIS2_TRANSPORT_HEADERS" 

/** axis2 transport in */
#define AXIS2_TRANSPORT_OUT "AXIS2_TRANSPORT_OUT" 

/** axis2 transport out */
#define AXIS2_TRANSPORT_IN "AXIS2_TRANSPORT_IN" 

/** axis2 character set encoding */
#define AXIS2_CHARACTER_SET_ENCODING "AXIS2_CHARACTER_SET_ENCODING" 

/** axis2 UTF_8; This is the 'utf-8' value for AXIS2_CHARACTER_SET_ENCODING property  */
#define AXIS2_UTF_8 "UTF-8" 

/** axis2 UTF_16; This is the 'utf-16' value for AXIS2_CHARACTER_SET_ENCODING property  */
#define AXIS2_UTF_16 "utf-16" 

/** axis2 default char set encoding; This is the default value for AXIS2_CHARACTER_SET_ENCODING property */
#define AXIS2_DEFAULT_CHAR_SET_ENCODING "UTF-8" 

/** axis2 transport succeeded */
#define AXIS2_TRANSPORT_SUCCEED "AXIS2_TRANSPORT_SUCCEED" 

/** HTTP Client */
#define AXIS2_HTTP_CLIENT "AXIS2_HTTP_CLIENT"

/** Transport url */
#define AXIS2_TRANSPORT_URL "TransportURL" 

/* Message flows */
/** In flow */
#define AXIS2_IN_FLOW 1

/** In fault flow */
#define AXIS2_IN_FAULT_FLOW 2

/** Out flow */
#define AXIS2_OUT_FLOW 3

/** Out fault flow */
#define AXIS2_OUT_FAULT_FLOW 4

    /** Type name for struct axis2_msg_ctx_ops */
    typedef struct axis2_msg_ctx_ops axis2_msg_ctx_ops_t;
    /** Type name for struct axis2_msg_ctx */
    typedef struct axis2_msg_ctx axis2_msg_ctx_t;

    struct axis2_svc;
    struct axis2_op;

    struct axis2_conf_ctx;
    struct axis2_svc_grp_ctx;
    struct axis2_svc_ctx;
    struct axis2_op_ctx;
    struct axis2_conf;
    struct axiom_soap_envelope;
    struct axis2_options;

    /**
     * @brief Message Context ops struct
     * Encapsulator struct for ops of axis2_msg_ctx
     */
    struct axis2_msg_ctx_ops
    {
	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_ctx* (AXIS2_CALL *
                get_base)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_op_ctx* (AXIS2_CALL *
                get_parent)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param parent pointer to parent
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_op_ctx *parent);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                free_fn)(
		    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);
	/**
         * The method is used to do the intialization of the engine context
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param conf pointer to conf
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                init)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_conf *conf);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_endpoint_ref_t *(AXIS2_CALL *
                get_fault_to)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_endpoint_ref_t *(AXIS2_CALL *
                get_from)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_in_fault_flow)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axiom_soap_envelope* (AXIS2_CALL *
                get_soap_envelope)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axiom_soap_envelope* (AXIS2_CALL *
                get_fault_soap_envelope)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_char_t* (AXIS2_CALL *
                get_msg_id)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_process_fault)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_relates_to_t* (AXIS2_CALL *
                get_relates_to)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_endpoint_ref_t *(AXIS2_CALL *
                get_reply_to)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_response_written)(
	            struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_server_side)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_endpoint_ref_t* (AXIS2_CALL *
                get_to)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param reference pointer to reference
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_fault_to)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *reference);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param reference pointer to reference
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_from)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *reference);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param in_fault_flow pointer to in_fault_flow
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_in_fault_flow)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t in_fault_flow);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param soap_envelope pointer to soap_envelope
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_soap_envelope)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axiom_soap_envelope *soap_envelope);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param soap_envelope pointer to soap_envelope
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_fault_soap_envelope)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axiom_soap_envelope *soap_envelope);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param message_id pointer to message id
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_message_id)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *message_id);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param process_fault process fault
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_process_fault)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t process_fault);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param reference pointer to reference
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_relates_to)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_relates_to_t *reference);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param reference pointer to reference
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_reply_to)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *referance);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param response_written respoinse written
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_response_written)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t response_written);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param server_side server side 
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_server_side)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t server_side);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param reference poinnter to reference
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_to)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *referance);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_new_thread_required)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param new_thread_required new thread required
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_new_thread_required)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t new_thread_required);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param action_uri pointer to action uri
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_wsa_action)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *action_uri);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_char_t* (AXIS2_CALL *
                get_wsa_action)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param message_id pointer to message id
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_wsa_message_id)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *message_id);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_char_t* (AXIS2_CALL *
                get_wsa_message_id)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_msg_info_headers_t* (AXIS2_CALL *
                get_msg_info_headers)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_paused)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param paused paused
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_paused)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t paused);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_transport_in_desc* (AXIS2_CALL *
                get_transport_in_desc)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_transport_out_desc* (AXIS2_CALL *
                get_transport_out_desc)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param transport_in_desc pointer to transport in description
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_transport_in_desc)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_transport_in_desc *transport_in_desc);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param transport_out_desc pointer to transport out description
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_transport_out_desc)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_transport_out_desc *transport_out_desc);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_op_ctx* (AXIS2_CALL *
                get_op_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param op_ctx pointer to operation context
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_op_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_op_ctx * op_ctx);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_output_written)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param output_written
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_output_written)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t output_written);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
         * @return Returns the svc_ctx_id.
	 */
        axis2_char_t* (AXIS2_CALL *
                get_svc_ctx_id)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
         * @param svc_ctx_id The svc_ctx_id to AXIS2_CALL set.
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_svc_ctx_id)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *svc_ctx_id);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_conf_ctx* (AXIS2_CALL *
                get_conf_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_svc_ctx* (AXIS2_CALL *
                get_svc_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param conf_ctx pointer to configuration context
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_conf_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_conf_ctx *conf_ctx);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param svc_ctx pointer to service context
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_svc_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_svc_ctx *svc_ctx);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param msg_info_headers pointer to message info headers
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_msg_info_headers)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_msg_info_headers_t *msg_info_headers);

        /**
         * To retrive configuration descriptor parameters , it is posible to 
	 * AXIS2_CALL get paramater specify at * any levle via this method , 
	 * and the preferance is as follows,
         * 1. Search in op description if its there
         * 2. if the paramter not found or op_ctx is null will search in * _svc
         * 3. If the svc is null or , the paramter does not found will serach in
         * AxisConfiguration
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
         * @return Paramter <code>Paramter</code>
	 */
        axis2_param_t* (AXIS2_CALL *
                get_parameter)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *key);

        /**
         * This method is to retrive both module configuration parameters and othere paramerts
         * The searching procedure is as follows;
         * 1. Search in module configurations inside corresponding op descripton if its three
         * 2. Search in corresponding op if its there
         * 3. Search in module configurations inside corresponding service description if its there
         * 4. Next search in Corresponding Service description if its there
         * 5. Next sercah in module configurations inside conf
         * 6. Search in axis2_conf_t *for paramters
         * 7. Next AXIS2_CALL get the corresponding module and search for the paramters
         * 8. Search in axis2_handler_desc_t *for the paramter
         * and the way of specifing mdoule configuration is as follows
         * <moduleConfig name="addressing">
         * <parameter name="addressingPara" locked="false">N/A</parameter>
         * </moduleConfig>
	 * @param msg_ctx pointer to message context
	 * @param env pointer to environment struct
         * @param key   Paramter name
         * @param module_name  Name of the module
         * @param handler_desc    <code>HandlerDescription</code>
         * @return axis2_param_t *<code>Parameter</code>
         */
        axis2_param_t* (AXIS2_CALL *
                get_module_parameter)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *key,
                    axis2_char_t *module_name,
                    axis2_handler_desc_t *handler_desc);

        /**
         * To acess any property AXIS2_CALL set at the run time , a handler 
	 * can add property to wherever he wants * to MesageContext , to 
	 * struct axis2_op_ctx * , to struct axis2_svc_ctx *and to ConfigurationContext.
         * This method is to retrive those properties NOT paramters
	 * @param msg_ctx pointer to message context
	 * @param env pointer to environment struct
         * @param key property Name
         * @param persistent need to be persistent even when server re-start
         * @return Object
	 */
        axis2_property_t *(AXIS2_CALL *
                get_property)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *key,
                    const axis2_bool_t persistent);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param key pointer to key
	 * @param value pointer to value
	 * @param persistent persistent
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_property)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    const axis2_char_t *key,
                    axis2_property_t *value,
                    axis2_bool_t persistent);
	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_qname_t* (AXIS2_CALL *
                get_paused_handler_name)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_char_t* (AXIS2_CALL *
                get_paused_phase_name)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param paused_phase_name pointer to paused phase name
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_paused_phase_name)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *paused_phase_name);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_char_t* (AXIS2_CALL *
                get_soap_action)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param soap_action pointer to soap action
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_soap_action)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *soap_action);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_doing_mtom)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param doing_mtom pointer to doing_mtom
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_doing_mtom)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t doing_mtom);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_doing_rest)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param doing_rest pointer to doing_rest
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_doing_rest)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t doing_rest);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param do_rest_through_post do rest through post
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_do_rest_through_post)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t do_rest_through_post);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_do_rest_through_post)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                get_is_soap_11)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param is_soap11 is soap11
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_is_soap_11)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_bool_t is_soap11);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_svc_grp_ctx* (AXIS2_CALL *
                get_svc_grp_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param svc_grp_ctx pointer to service group context
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_svc_grp_ctx)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_svc_grp_ctx *svc_grp_ctx);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_op* (AXIS2_CALL *
                get_op)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param op pointer to op
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_op)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_op *op);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_svc* (AXIS2_CALL *
                get_svc)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param svc pointer to service
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_svc)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_svc *svc);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_svc_grp* (AXIS2_CALL *
                 get_svc_grp)(
                     struct axis2_msg_ctx *msg_ctx,
                     const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param svc_grp pointer to service group
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_svc_grp)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_svc_grp *svc_grp);


	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_char_t* (AXIS2_CALL *
                get_svc_grp_ctx_id)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param svc_grp_ctx_id pointer to service group context id
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_svc_grp_ctx_id)(
                    struct axis2_msg_ctx *msg_ctx,
                    const axis2_env_t *env,
                    axis2_char_t *svc_grp_ctx_id);

        /**
         * finds the service
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        struct axis2_svc* (AXIS2_CALL *
                find_svc)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);
        /**
         * finds the op
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param svc pointer to service 
	 */
        struct axis2_op* (AXIS2_CALL *
                find_op)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_svc *svc);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_bool_t (AXIS2_CALL *
                is_paused)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param options pointer to options
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_options )(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    struct axis2_options *options);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param flow flow
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_flow )(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    int flow);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        int (AXIS2_CALL *
                get_flow )(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param excution_chain pointer to execution chain
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_execution_chain)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    axis2_array_list_t *execution_chain);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        axis2_array_list_t *(AXIS2_CALL *
                get_execution_chain)(
                    const axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param index index
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_current_handler_index)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    const int index);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        int (AXIS2_CALL *
                get_current_handler_index)(
                    const axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 * @param index index 
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
	 */
        axis2_status_t (AXIS2_CALL *
                set_current_phase_index)(
                    axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env,
                    const int index);

	/**
	 * @param msg_ctx message context
	 * @param env pointer to environment struct
	 */
        int (AXIS2_CALL *
                get_current_phase_index)(
                    const axis2_msg_ctx_t *msg_ctx,
                    const axis2_env_t *env);
    };

    /**
     * message context struct
     */
    struct axis2_msg_ctx
    {
	/** operations of message context */
        axis2_msg_ctx_ops_t *ops;
    };


    /**
     * Creates a message context struct
     * @param env pointer to environment struct
     * @param conf_ctx pointer to configuration context
     * @param transport_in_desc pointer to tarnsport in description
     * @param transport_out_desc pointer to transport out description
     */
    AXIS2_EXTERN axis2_msg_ctx_t * AXIS2_CALL
    axis2_msg_ctx_create (
            const axis2_env_t *env,
            struct axis2_conf_ctx *conf_ctx,
            struct axis2_transport_in_desc *transport_in_desc,
            struct axis2_transport_out_desc *transport_out_des);

/************************** Start of function macros **************************/

/** Get base.
    @sa axis2_msg_ctx_ops#get_base */
#define AXIS2_MSG_CTX_GET_BASE(msg_ctx, env) \
      ((msg_ctx)->ops->get_base(msg_ctx, env))

/** Get parent.
    @sa axis2_msg_ctx_ops#get_parent */
#define AXIS2_MSG_CTX_GET_PARENT(msg_ctx, env) \
      ((msg_ctx)->ops->get_parent(msg_ctx, env))

/** Set parent.
    @sa axis2_msg_ctx_ops#set_parent */
#define AXIS2_MSG_CTX_SET_PARENT(msg_ctx, env, parent) \
      ((msg_ctx)->ops->get_parent(msg_ctx, env, parent))

/** Frees the message context.
    @sa axis2_msg_ctx_ops#free */
#define AXIS2_MSG_CTX_FREE(msg_ctx, env) \
      ((msg_ctx)->ops->free_fn(msg_ctx, env))

/** Initialize the message context.
    @sa axis2_msg_ctx_ops#init */
#define AXIS2_MSG_CTX_INIT(msg_ctx, env, conf) \
      ((msg_ctx)->ops->init(msg_ctx, env, conf))

/** Get fault to.
    @sa axis2_msg_ctx_ops#get_fault_to */
#define AXIS2_MSG_CTX_GET_FAULT_TO(msg_ctx, env) \
      ((msg_ctx)->ops->get_fault_to(msg_ctx, env))

/** Get from.
    @sa axis2_msg_ctx_ops#get_from */
#define AXIS2_MSG_CTX_GET_FROM(msg_ctx, env) \
      ((msg_ctx)->ops->get_from(msg_ctx, env))

/** Get in fault flow.
    @sa axis2_msg_ctx_ops#get_in_fault_flow */
#define AXIS2_MSG_CTX_GET_IN_FAULT_FLOW(msg_ctx, env) \
      ((msg_ctx)->ops->get_in_fault_flow(msg_ctx, env))

/** Gets the soap envelope.
    @sa axis2_msg_ctx_ops#get_soap_envelope */
#define AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env) \
      ((msg_ctx)->ops->get_soap_envelope(msg_ctx, env))

/** Get fault soap envelope.
    @sa axis2_msg_ctx_ops#get_fault_soap_envelope */
#define AXIS2_MSG_CTX_GET_FAULT_SOAP_ENVELOPE(msg_ctx, env) \
      ((msg_ctx)->ops->get_fault_soap_envelope(msg_ctx, env))

/** Gets the message id.
    @sa axis2_msg_ctx_ops#get_msg_id */
#define AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env) \
      ((msg_ctx)->ops->get_msg_id(msg_ctx, env))

/** Get process fault.
    @sa axis2_msg_ctx_ops#get_process_fault */
#define AXIS2_MSG_CTX_GET_PROCESS_FAULT(msg_ctx, env) \
      ((msg_ctx)->ops->get_process_fault(msg_ctx, env))

/** Get relates to.
    @sa axis2_msg_ctx_ops#get_relates_to */
#define AXIS2_MSG_CTX_GET_RELATES_TO(msg_ctx, env) \
      ((msg_ctx)->ops->get_relates_to(msg_ctx, env))

/** Get reply to.
    @sa axis2_msg_ctx_ops#get_reply_to */
#define AXIS2_MSG_CTX_GET_REPLY_TO(msg_ctx, env) \
      ((msg_ctx)->ops->get_reply_to(msg_ctx, env))

/** Get response written.
    @sa axis2_msg_ctx_ops#get_response_written */
#define AXIS2_MSG_CTX_GET_RESPONSE_WRITTEN(msg_ctx, env) \
      ((msg_ctx)->ops->get_response_written(msg_ctx, env))

/** Get server side.
    @sa axis2_msg_ctx_ops#get_server_side */
#define AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env) \
      ((msg_ctx)->ops->get_server_side(msg_ctx, env))

/** Get to.
    @sa axis2_msg_ctx_ops#get_to */
#define AXIS2_MSG_CTX_GET_TO(msg_ctx, env) \
      ((msg_ctx)->ops->get_to(msg_ctx, env))

/** Set fault to.
    @sa axis2_msg_ctx_ops#set_fault_to */
#define AXIS2_MSG_CTX_SET_FAULT_TO(msg_ctx, env, reference) \
      ((msg_ctx)->ops->set_fault_to(msg_ctx, env, reference))

/** Set from.
    @sa axis2_msg_ctx_ops#set_from */
#define AXIS2_MSG_CTX_SET_FROM(msg_ctx, env, reference) \
      ((msg_ctx)->ops->set_from(msg_ctx, env, reference))

/** Set in fault flow.
    @sa axis2_msg_ctx_ops#set_in_fault_flow */
#define AXIS2_MSG_CTX_SET_IN_FAULT_FLOW(msg_ctx, env, in_fault_flow) \
      ((msg_ctx)->ops->get_in_fault_flow(msg_ctx, env, in_fault_flow))

/** Set soap envelope.
    @sa axis2_msg_ctx_ops#set_soap_envelope */
#define AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, soap_envelope) \
      ((msg_ctx)->ops->set_soap_envelope(msg_ctx, env, soap_envelope))

/** Set fault soap envelope.
    @sa axis2_msg_ctx_ops#set_fault_soap_envelope */
#define AXIS2_MSG_CTX_SET_FAULT_SOAP_ENVELOPE(msg_ctx, env, soap_envelope) \
      ((msg_ctx)->ops->set_fault_soap_envelope(msg_ctx, env, soap_envelope))

/** Sets the message id.
    @sa axis2_msg_ctx_ops#set_message_id */
#define AXIS2_MSG_CTX_SET_MESSAGE_ID(msg_ctx, env, message_id) \
      ((msg_ctx)->ops->set_message_id(msg_ctx, env, message_id))

/** Set process fault.
    @sa axis2_msg_ctx_ops#set_process_fault */
#define AXIS2_MSG_CTX_SET_PROCESS_FAULT(msg_ctx, env, process_fault) \
      ((msg_ctx)->ops->set_process_fault(msg_ctx, env, process_fault))

/** Set reply to.
    @sa axis2_msg_ctx_ops#set_relates_to */
#define AXIS2_MSG_CTX_SET_RELATES_TO(msg_ctx, env, reference) \
      ((msg_ctx)->ops->set_relates_to(msg_ctx, env, reference))

/** Set reply to.
    @sa axis2_msg_ctx_ops#set_reply_to */
#define AXIS2_MSG_CTX_SET_REPLY_TO(msg_ctx, env, reference) \
      ((msg_ctx)->ops->set_reply_to(msg_ctx, env, reference))

/** Set response written.
    @sa axis2_msg_ctx_ops#set_response_written */
#define AXIS2_MSG_CTX_SET_RESPONSE_WRITTEN(msg_ctx, env, response_written) \
      ((msg_ctx)->ops->set_response_written(msg_ctx, env, response_written))

/** Set server side.
    @sa axis2_msg_ctx_ops#set_server_side */
#define AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, server_side) \
      ((msg_ctx)->ops->set_server_side(msg_ctx, env, server_side))

/** Set to.
    @sa axis2_msg_ctx_ops#set_to */
#define AXIS2_MSG_CTX_SET_TO(msg_ctx, env, reference) \
      ((msg_ctx)->ops->set_to(msg_ctx, env, reference))

/** Get new thread required.
    @sa axis2_msg_ctx_ops#get_new_thread_required */
#define AXIS2_MSG_CTX_GET_NEW_THREAD_REQUIRED(msg_ctx, env) \
      ((msg_ctx)->ops->set_response_written(msg_ctx, env))

/** Set new thread required.
    @sa axis2_msg_ctx_ops#set_new_thread_required */
#define AXIS2_MSG_CTX_SET_NEW_THREAD_REQUIRED(msg_ctx, env, new_thread_required) \
      ((msg_ctx)->ops->set_response_written(msg_ctx, env, new_thread_required))

/** Sets the wsa action.
    @sa axis2_msg_ctx_ops#set_wsa_action */
#define AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env, action_uri) \
      ((msg_ctx)->ops->set_wsa_action(msg_ctx, env, action_uri))

/** Gets the wsa action.
    @sa axis2_msg_ctx_ops#get_wsa_action */
#define AXIS2_MSG_CTX_GET_WSA_ACTION(msg_ctx, env) \
      ((msg_ctx)->ops->get_wsa_action(msg_ctx, env))

/** Sets the wsa message id.
    @sa axis2_msg_ctx_ops#set_wsa_message_id */
#define AXIS2_MSG_CTX_SET_WSA_MESSAGE_ID(msg_ctx, env, message_id) \
      ((msg_ctx)->ops->set_wsa_message_id(msg_ctx, env, message_id))

/** Gets the wsa message id.
    @sa axis2_msg_ctx_ops#get_wsa_message_id */
#define AXIS2_MSG_CTX_GET_WSA_MESSAGE_ID(msg_ctx, env) \
      ((msg_ctx)->ops->get_wsa_message_id(msg_ctx, env))

/** Gets the message info headers.
    @sa axis2_msg_ctx_ops#get_msg_info_headers */
#define AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, env) \
      ((msg_ctx)->ops->get_msg_info_headers(msg_ctx, env))

/** Get paused.
    @sa axis2_msg_ctx_ops#get_paused */
#define AXIS2_MSG_CTX_GET_PAUSED(msg_ctx, env) \
      ((msg_ctx)->ops->get_paused(msg_ctx, env))

/** Set paused.
    @sa axis2_msg_ctx_ops#set_paused */
#define AXIS2_MSG_CTX_SET_PAUSED(msg_ctx, env, paused) \
      ((msg_ctx)->ops->set_paused(msg_ctx, env, paused))

/** Gets the transport in description.
    @sa axis2_msg_ctx_ops#get_transport_in_desc */
#define AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(msg_ctx, env) \
      ((msg_ctx)->ops->get_transport_in_desc(msg_ctx, env))

/** Get the transport out description.
    @sa axis2_msg_ctx_ops#get_transport_out_desc */
#define AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env) \
      ((msg_ctx)->ops->get_transport_out_desc(msg_ctx, env))

/** Sets the transport in description.
    @sa axis2_msg_ctx_ops#set_transport_in_desc */
#define AXIS2_MSG_CTX_SET_TRANSPORT_IN_DESC(msg_ctx, env, transport_in_desc) \
      ((msg_ctx)->ops->set_transport_in_desc(msg_ctx, env, transport_in_desc))

/** Sets the transport out description.
    @sa axis2_msg_ctx_ops#set_transport_out_desc */
#define AXIS2_MSG_CTX_SET_TRANSPORT_OUT_DESC(msg_ctx, env, transport_out_desc) \
      ((msg_ctx)->ops->set_transport_out_desc(msg_ctx, env, transport_out_desc))

/** Gets the operation context.
    @sa axis2_msg_ctx_ops#get_op_ctx */
#define AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env) \
      ((msg_ctx)->ops->get_op_ctx(msg_ctx, env))

/** Sets the operation context.
    @sa axis2_msg_ctx_ops#set_op_ctx */
#define AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, op_ctx) \
      ((msg_ctx)->ops->set_op_ctx(msg_ctx, env, op_ctx))

/** Get output written.
    @sa axis2_msg_ctx_ops#get_output_written */
#define AXIS2_MSG_CTX_GET_OUTPUT_WRITTEN(msg_ctx, env) \
      ((msg_ctx)->ops->get_output_written(msg_ctx, env))

/** Set output written.
    @sa axis2_msg_ctx_ops#set_output_written */
#define AXIS2_MSG_CTX_SET_OUTPUT_WRITTEN(msg_ctx, env, output_written) \
      ((msg_ctx)->ops->set_output_written(msg_ctx, env, output_written))

/** Gets the service context id.
    @sa axis2_msg_ctx_ops#get_svc_ctx_id */
#define AXIS2_MSG_CTX_GET_SVC_CTX_ID(msg_ctx, env) \
      ((msg_ctx)->ops->get_svc_ctx_id(msg_ctx, env))

/** Sets the service context id.
    @sa axis2_msg_ctx_ops#set_svc_ctx_id */
#define AXIS2_MSG_CTX_SET_SVC_CTX_ID(msg_ctx, env, svc_ctx_id) \
      ((msg_ctx)->ops->set_svc_ctx_id(msg_ctx, env, svc_ctx_id))

/** Gets the configuration context.
    @sa axis2_msg_ctx_ops#get_conf_ctx */
#define AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env) \
      ((msg_ctx)->ops->get_conf_ctx(msg_ctx, env))

/** Sets the configuration context.
    @sa axis2_msg_ctx_ops#set_conf_ctx */
#define AXIS2_MSG_CTX_SET_CONF_CTX(msg_ctx, env, conf_ctx) \
      ((msg_ctx)->ops->set_conf_ctx(msg_ctx, env, conf_ctx))

/** Gets the service context.
    @sa axis2_msg_ctx_ops#get_svc_ctx */
#define AXIS2_MSG_CTX_GET_SVC_CTX(msg_ctx, env) \
      ((msg_ctx)->ops->get_svc_ctx(msg_ctx, env))

/** Sets the service context.
    @sa axis2_msg_ctx_ops#set_svc_ctx */
#define AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx) \
      ((msg_ctx)->ops->set_svc_ctx(msg_ctx, env, svc_ctx))

/** Sets the message info headers.
    @sa axis2_msg_ctx_ops#set_msg_info_headers */
#define AXIS2_MSG_CTX_SET_MSG_INFO_HEADERS(msg_ctx, env, msg_info_headers) \
      ((msg_ctx)->ops->set_msg_info_headers(msg_ctx, env, msg_info_headers))

/** Gets the parameter.
    @sa axis2_msg_ctx_ops#get_parameter */
#define AXIS2_MSG_CTX_GET_PARAMETER(msg_ctx, env, key) \
      ((msg_ctx)->ops->get_parameter(msg_ctx, env, key))

/** Gets the module parameter.
    @sa axis2_msg_ctx_ops#get_module_parameter */
#define AXIS2_MSG_CTX_GET_MODULE_PARAMETER(msg_ctx, env, key, module_name, handler_desc) \
      ((msg_ctx)->ops->get_module_parameter(msg_ctx, env, key, module_name, handler_desc))

/** Get property..
    @sa axis2_msg_ctx_ops#get_property */
#define AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, key, persistent) \
      ((msg_ctx)->ops->get_property(msg_ctx, env, key, persistent))

/** Set property.
    @sa axis2_msg_ctx_ops#set_property */
#define AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, key, value, persistent) \
      ((msg_ctx)->ops->set_property(msg_ctx, env, key, value, persistent))

/** Gets the paused handler name.
    @sa axis2_msg_ctx_ops#get_paused_handler_name */
#define AXIS2_MSG_CTX_GET_PAUSED_HANDLER_NAME(msg_ctx, env) \
      ((msg_ctx)->ops->get_paused_handler_name(msg_ctx, env))

/** Gets the paused phase name.
    @sa axis2_msg_ctx_ops#get_paused_phase_name */
#define AXIS2_MSG_CTX_GET_PAUSED_PHASE_NAME(msg_ctx, env) \
      ((msg_ctx)->ops->get_paused_phase_name(msg_ctx, env))

/** Sets the paused phase name.
    @sa axis2_msg_ctx_ops#set_paused_phase_name */
#define AXIS2_MSG_CTX_SET_PAUSED_PHASE_NAME(msg_ctx, env, paused_phase_name) \
      ((msg_ctx)->ops->set_paused_phase_name(msg_ctx, env, paused_phase_name))

/** Gets the soap action.
    @sa axis2_msg_ctx_ops#get_soap_action */
#define AXIS2_MSG_CTX_GET_SOAP_ACTION(msg_ctx, env) \
      ((msg_ctx)->ops->get_soap_action(msg_ctx, env))

/** Sets the soap action.
    @sa axis2_msg_ctx_ops#set_soap_action */
#define AXIS2_MSG_CTX_SET_SOAP_ACTION(msg_ctx, env, soap_action) \
      ((msg_ctx)->ops->set_soap_action(msg_ctx, env, soap_action))

/** Get doing mtom.
    @sa axis2_msg_ctx_ops#get_doing_mtom */
#define AXIS2_MSG_CTX_GET_DOING_MTOM(msg_ctx, env) \
      ((msg_ctx)->ops->get_doing_mtom(msg_ctx, env))

/** Set doing mtom.
    @sa axis2_msg_ctx_ops#set_doing_mtom */
#define AXIS2_MSG_CTX_SET_DOING_MTOM(msg_ctx, env, doing_mtom) \
      ((msg_ctx)->ops->set_doing_mtom(msg_ctx, env, doing_mtom))

/** Get doing rest.
    @sa axis2_msg_ctx_ops#get_doing_rest */
#define AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env) \
      ((msg_ctx)->ops->get_doing_rest(msg_ctx, env))

/** Set doing rest.
    @sa axis2_msg_ctx_ops#set_doing_rest */
#define AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx, env, doing_rest) \
      ((msg_ctx)->ops->set_doing_rest(msg_ctx, env, doing_rest))

/** Set do rest through post.
    @sa axis2_msg_ctx_ops#set_do_rest_through_post */
#define AXIS2_MSG_CTX_SET_DO_REST_THROUGH_POST(msg_ctx, env, do_rest_through_post) \
      ((msg_ctx)->ops->set_do_rest_through_post(msg_ctx, env, do_rest_through_post))

/** Get is soap11.
    @sa axis2_msg_ctx_ops#set_is_soap_11 */
#define AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env) \
      ((msg_ctx)->ops->get_is_soap_11(msg_ctx, env))

/** Set is soap11.
    @sa axis2_msg_ctx_ops#set_is_soap_11 */
#define AXIS2_MSG_CTX_SET_IS_SOAP_11(msg_ctx, env, is_soap11) \
      ((msg_ctx)->ops->set_is_soap_11(msg_ctx, env, is_soap11))

/** Gets the service group context.
    @sa axis2_msg_ctx_ops#get_svc_grp_ctx */
#define AXIS2_MSG_CTX_GET_SVC_GRP_CTX(msg_ctx, env) \
      ((msg_ctx)->ops->get_svc_grp_ctx(msg_ctx, env))

/** Sets the service group context.
    @sa axis2_msg_ctx_ops#set_svc_grp_ctx */
#define AXIS2_MSG_CTX_SET_SVC_GRP_CTX(msg_ctx, env, svc_grp_ctx) \
      ((msg_ctx)->ops->set_svc_grp_ctx(msg_ctx, env, svc_grp_ctx))

/** Gets the operation.
    @sa axis2_msg_ctx_ops#get_op */
#define AXIS2_MSG_CTX_GET_OP(msg_ctx, env) \
      ((msg_ctx)->ops->get_op(msg_ctx, env))

/** Sets the operation.
    @sa axis2_msg_ctx_ops#set_op */
#define AXIS2_MSG_CTX_SET_OP(msg_ctx, env, op) \
      ((msg_ctx)->ops->set_op(msg_ctx, env, op))

/** Gets the service.
    @sa axis2_msg_ctx_ops#get_svc */
#define AXIS2_MSG_CTX_GET_SVC(msg_ctx, env) \
      ((msg_ctx)->ops->get_svc(msg_ctx, env))

/** Sets the service.
    @sa axis2_msg_ctx_ops#set_svc */
#define AXIS2_MSG_CTX_SET_SVC(msg_ctx, env, svc) \
      ((msg_ctx)->ops->set_svc(msg_ctx, env, svc))

/** Gets the service group.
    @sa axis2_msg_ctx_ops#get_svc_grp */
#define AXIS2_MSG_CTX_GET_SVC_GRP(msg_ctx, env) \
      ((msg_ctx)->ops->get_svc_grp(msg_ctx, env))

/** Sets the service group.
    @sa axis2_msg_ctx_ops#set_svc_grp */
#define AXIS2_MSG_CTX_SET_SVC_GRP(msg_ctx, env, svc_grp) \
      ((msg_ctx)->ops->set_svc_grp(msg_ctx, env, svc_grp))

/** Gets the service group context id.
    @sa axis2_msg_ctx_ops#get_svc_grp_ctx_id */
#define AXIS2_MSG_CTX_GET_SVC_GRP_CTX_ID(msg_ctx, env) \
      ((msg_ctx)->ops->get_svc_grp_ctx_id(msg_ctx, env))

/** Sets the service group context id.
    @sa axis2_msg_ctx_ops#set_svc_grp_ctx_id */
#define AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env, svc_grp_ctx_id) \
      ((msg_ctx)->ops->set_svc_grp_ctx_id(msg_ctx, env, svc_grp_ctx_id))

/** Is paused.
    @sa axis2_msg_ctx_ops#is_paused */
#define AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env) \
      ((msg_ctx)->ops->is_paused(msg_ctx, env))

/** Find svc.
    @sa axis2_msg_ctx_ops#find_svc */
#define AXIS2_MSG_CTX_FIND_SVC(msg_ctx, env) \
      ((msg_ctx)->ops->find_svc(msg_ctx, env))

/** Find op.
    @sa axis2_msg_ctx_ops#find_op */
#define AXIS2_MSG_CTX_FIND_OP(msg_ctx, env, svc) \
      ((msg_ctx)->ops->find_op(msg_ctx, env, svc))

/** Set options.
    @sa axis2_msg_ctx_ops#set_options */
#define AXIS2_MSG_CTX_SET_OPTIONS(msg_ctx, env, options) \
      ((msg_ctx)->ops->set_options(msg_ctx, env, options))

/** Set flow.
    @sa axis2_msg_ctx_ops#set_flow */
#define AXIS2_MSG_CTX_SET_FLOW(msg_ctx, env, flow) \
      ((msg_ctx)->ops->set_flow(msg_ctx, env, flow))

/** Get flow.
    @sa axis2_msg_ctx_ops#get_flow */
#define AXIS2_MSG_CTX_GET_FLOW(msg_ctx, env) \
      ((msg_ctx)->ops->get_flow(msg_ctx, env))

/** Sets the execution chain.
    @sa axis2_msg_ctx_ops#set_execution_chain */
#define AXIS2_MSG_CTX_SET_EXECUTION_CHAIN(msg_ctx, env, chain) \
      ((msg_ctx)->ops->set_execution_chain(msg_ctx, env, chain))

/** Gets the execution chain.
    @sa axis2_msg_ctx_ops#get_execution_chain */
#define AXIS2_MSG_CTX_GET_EXECUTION_CHAIN(msg_ctx, env) \
      ((msg_ctx)->ops->get_execution_chain(msg_ctx, env))

/** Sets the current handler index.
    @sa axis2_msg_ctx_ops#set_current_handler_index */
#define AXIS2_MSG_CTX_SET_CURRENT_HANDLER_INDEX(msg_ctx, env, index) \
      ((msg_ctx)->ops->set_current_handler_index(msg_ctx, env, index))

/** Gets the current handler index.
    @sa axis2_msg_ctx_ops#get_current_handler_index */
#define AXIS2_MSG_CTX_GET_CURRENT_HANDLER_INDEX(msg_ctx, env) \
      ((msg_ctx)->ops->get_current_handler_index(msg_ctx, env))

/** Sets the current phase index.
    @sa axis2_msg_ctx_ops#set_current_phase_index */
#define AXIS2_MSG_CTX_SET_CURRENT_PHASE_INDEX(msg_ctx, env, index) \
      ((msg_ctx)->ops->set_current_phase_index(msg_ctx, env, index))

/** Gets the current phase index.
    @sa axis2_msg_ctx_ops#get_current_phase_index */
#define AXIS2_MSG_CTX_GET_CURRENT_PHASE_INDEX(msg_ctx, env) \
      ((msg_ctx)->ops->get_current_phase_index(msg_ctx, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_MSG_CTX_H */
