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


/**
  * @file axis2_msg_ctx.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_ctx.h>
#include <axis2_core.h>
#include <axis2_relates_to.h>
#include <axis2_param.h>
#include <axis2_handler_desc.h>
#include <axis2_qname.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_msg_ctx Message Context 
 * @ingroup axis2_core_context
 * @{
 */

    /** axis2 transport headers */
    /*static const axis2_char_t *AXIS2_TRANSPORT_HEADERS = "AXIS2_TRANSPORT_HEADERS";*/
    /** axis2 transport in */
    /*static const axis2_char_t *AXIS2_TRANSPORT_OUT = "AXIS2_TRANSPORT_OUT";*/
    /** axis2 transport out */
    /*static const axis2_char_t *AXIS2_TRANSPORT_IN = "AXIS2_TRANSPORT_IN";*/
    /** axis2 character set encoding */
    /*static const axis2_char_t *AXIS2_CHARACTER_SET_ENCODING = "AXIS2_CHARACTER_SET_ENCODING";*/
    /** axis2 UTF_8; This is the 'utf-8' value for AXIS2_CHARACTER_SET_ENCODING property  */
    /*static const axis2_char_t *AXIS2_UTF_8 = "UTF-8";*/
    /** axis2 UTF_16; This is the 'utf-16' value for AXIS2_CHARACTER_SET_ENCODING property  */
    /*static const axis2_char_t *AXIS2_UTF_16 = "utf-16";*/
    /** axis2 default char set encoding; This is the default value for AXIS2_CHARACTER_SET_ENCODING property */
    /*static const axis2_char_t *AXIS2_DEFAULT_CHAR_SET_ENCODING = "UTF_8";*/
    /** axis2 transport succeeded */
    /*static const axis2_char_t *AXIS2_TRANSPORT_SUCCEED = "AXIS2_TRANSPORT_SUCCEED";*/
    
typedef struct axis2_msg_ctx_ops axis2_msg_ctx_ops_t;
typedef struct axis2_msg_ctx axis2_msg_ctx_t; 
struct axis2_svc;    
struct axis2_operation;
    
struct axis2_conf_ctx;
struct axis2_svc_grp_ctx;
struct axis2_svc_ctx;
struct axis2_operation_ctx;
struct axis2_session_ctx;
struct axis2_engine_config;
struct axis2_soap_envelope;
    
/** 
 * @brief Message Context operations struct
 * Encapsulator struct for operations of axis2_msg_ctx
 */  
struct axis2_msg_ctx_ops
{
    struct axis2_ctx* (AXIS2_CALL *get_base)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    struct axis2_operation_ctx* (AXIS2_CALL *get_parent)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env);
    
    /**
     * @param context
     */
    axis2_status_t (AXIS2_CALL *set_parent)(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, 
                                            struct axis2_operation_ctx *parent);
    
    axis2_status_t (AXIS2_CALL *free)(axis2_msg_ctx_t *msg_ctx,
                                                    axis2_env_t **env);
    /**
     * The method is used to do the intialization of the EngineContext
     *
     */
    axis2_status_t (AXIS2_CALL *init)(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env, 
                                        struct axis2_engine_config *engine_config);
    
    /**
     * @return
     */
    /*EndpointReference (AXIS2_CALL *get_fault_to)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env);
    */
    
    /**
     * @return
     */
    /*EndpointReference (AXIS2_CALL *get_from)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    */
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_in_fault_flow)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env);
    
    /**
     * @return
     */
    struct axis2_soap_envelope* (AXIS2_CALL *get_soap_envelope)(struct axis2_msg_ctx *msg_ctx, 
                                                                        axis2_env_t **env);
    
    /**
     * @return
     */
    /*axis2_char_t* (AXIS2_CALL *get_msg_id)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    */
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_process_fault)(struct axis2_msg_ctx *msg_ctx, 
                                                 axis2_env_t **env);
    
    /**
     * @return
     */
    /*RelatesTo (AXIS2_CALL *get_relates_to)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    */
    
    /**
     * @return
     */
    /*EndpointReference (AXIS2_CALL *get_reply_to)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env);
    */
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_response_written)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env);
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_server_side)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    /**
     * @return
     */
    struct axis2_session_ctx* (AXIS2_CALL *get_session_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env);
    
    /**
     * @return
     */
    /*EndpointReference (AXIS2_CALL *get_to)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    */
    
    /**
     * @param reference
     */
    /*axis2_status_t (AXIS2_CALL *set_fault_to)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, EndpointReference reference);
    */
    
    /**
     * @param reference
     */
    /*
    axis2_status_t (AXIS2_CALL *set_from)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, EndpointReference reference);
    */
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_in_fault_flow)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env, axis2_bool_t in_fault_flow);
    
    /**
     * @param soap_envelope
     */
    axis2_status_t (AXIS2_CALL *set_envelope)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                struct axis2_soap_envelope *soap_envelope);
    
    /**
     * @param string
     */
    /*axis2_status_t (AXIS2_CALL *set_message_id)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env, axis2_char_t *message_id);
    */
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_process_fault)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env, 
                                                    axis2_bool_t process_fault);
    
    /**
     * @param reference
     */
    /*axis2_status_t (AXIS2_CALL *set_relates_to)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env, 
                                                    RelatesTo reference);
    */
    
    /**
     * @param referance
     */
    /*axis2_status_t (AXIS2_CALL *set_reply_to)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                EndpointReference referance);
    */
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_response_written)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env, 
                                                        axis2_bool_t response_written);
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_server_side)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                axis2_bool_t server_side);
    
    /**
     * @param referance
     */
    /*axis2_status_t (AXIS2_CALL *set_to)(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, 
                                            EndpointReference referance);
    */
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_new_thread_required)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env);
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_new_thread_required)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env, 
                                                            axis2_bool_t new_thread_required);
    
    /**
     * Method AXIS2_CALL getExecutionChain
     */
    /*axis2_status_t (AXIS2_CALL *set_wsa_action)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env, 
                                                    axis2_char_t *action_uri);
    
    axis2_char_t* (AXIS2_CALL *get_wsa_action)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_wsa_message_id)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env, 
                                                    axis2_char_t *message_id);
    
    axis2_char_t* (AXIS2_CALL *get_wsa_message_id)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env);
    */
    
    /*axis2_msg_info_headers_t* (AXIS2_CALL *get_msg_info_headers)(struct axis2_msg_ctx *msg_ctx, 
                                                                axis2_env_t **env);
    */
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_paused)(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env);
    
    /**
     */
    axis2_status_t (AXIS2_CALL *set_paused)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                axis2_bool_t paused);
    
    /**
     * @return
     */
    struct axis2_transport_in_desc* (AXIS2_CALL *get_transport_in_desc)(struct axis2_msg_ctx *msg_ctx, 
                                                                        axis2_env_t **env);
    
    /**
     * @return
     */
    struct axis2_transport_out_desc* (AXIS2_CALL *get_transport_out_desc)(struct axis2_msg_ctx *msg_ctx, 
                                                                            axis2_env_t **env);
    
    /**
     * @param in
     */
    axis2_status_t (AXIS2_CALL *set_transport_in_desc)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env, 
                                                        struct axis2_transport_in_desc *transport_in_desc);
    
    /**
     * @param out
     */
    axis2_status_t (AXIS2_CALL *set_transport_out_desc)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env, 
                                                        struct axis2_transport_out_desc *transport_out_desc); 
    
    struct axis2_operation_ctx* (AXIS2_CALL *get_operation_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                                axis2_env_t **env);
    
    /**
     * @param context
     */
    axis2_status_t (AXIS2_CALL *set_operation_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env, 
                                                    struct axis2_operation_ctx * operation_ctx);
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_output_written)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env);
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_output_written)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env,
                                                    axis2_bool_t output_written);
    
    /**
     * @return Returns the svc_ctx_id.
     */
    axis2_char_t* (AXIS2_CALL *get_svc_ctx_id)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    /**
     * @param svc_ctx_id The svc_ctx_id to AXIS2_CALL set.
     */
    axis2_status_t (AXIS2_CALL *set_svc_ctx_id)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env,
                                                axis2_char_t *svc_ctx_id);
    
    struct axis2_conf_ctx* (AXIS2_CALL *get_conf_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                          axis2_env_t **env);
    
    /**
     * @return
     */
    struct axis2_svc_ctx* (AXIS2_CALL *get_svc_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                          axis2_env_t **env);
    
    /**
     * @param context
     */
    axis2_status_t (AXIS2_CALL *set_conf_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                         axis2_env_t **env,
                                                         struct axis2_conf_ctx *conf_ctx);
    
    /**
     * @param context
     */
    axis2_status_t (AXIS2_CALL *set_svc_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                struct axis2_svc_ctx *svc_ctx);
    
    /**
     * @param collection
     */
    /*axis2_status_t (AXIS2_CALL *set_msg_info_headers)(struct axis2_msg_ctx *msg_ctx, 
                                                      axis2_env_t **env, 
                                                      axis2_msg_info_headers_t *msg_info_headers);
    */
                                                
    /**
     * To retrive configuration descriptor parameters , it is posible to AXIS2_CALL get paramater specify at
     * any levle via this method , and the preferance is as follows,
     * 1. Search in operation description if its there
     * 2. if the paramter not found or operation_ctx is null will search in
     * _svc
     * 3. If the svc is null or , the paramter does not found will serach in
     * AxisConfiguration
     *
     * @param key
     * @return Paramter <code>Paramter</code>
     */
    axis2_param_t* (AXIS2_CALL *get_parameter)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, axis2_char_t *key);
    
    /**
     * This method is to retrive both module configuration parameters and othere paramerts
     * The searching procedure is as follows;
     * 1. Search in module configurations inside corresponding operation descripton if its three
     * 2. Search in corresponding operation if its there
     * 3. Search in module configurations inside corresponding service description if its there
     * 4. Next search in Corresponding Service description if its there
     * 5. Next sercah in module configurations inside engine_config
     * 6. Search in axis2_engine_config_t *for paramters
     * 7. Next AXIS2_CALL get the corresponding module and search for the paramters
     * 8. Search in axis2_handler_desc_t *for the paramter
     * <p/>
     * and the way of specifing mdoule configuration is as follows
     * <moduleConfig name="addressing">
     * <parameter name="addressingPara" locked="false">N/A</parameter>
     * </moduleConfig>
     *
     * @param key        : Paramtre Name
     * @param module_name : Name of the module
     * @param handler_desc    <code>HandlerDescription</code>
     * @return axis2_param_t *<code>Parameter</code>
     */
    axis2_param_t* (AXIS2_CALL *get_module_parameter)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env, 
                                                        axis2_char_t *key, 
                                                        axis2_char_t *module_name, 
                                                        axis2_handler_desc_t *handler_desc);
    
    /**
     * To acess any property AXIS2_CALL set at the run time , a handler can add property to wherever he wants
     * to MesageContext , to struct axis2_operation_ctx * , to struct axis2_svc_ctx *and to ConfigurationContext.
     * This method is to retrive those properties NOT paramters
     *
     * @param key        : property Name
     * @param persistent : need to be persistent even when server re-start
     * @return Object
     */
    void* (AXIS2_CALL *get_property)(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env, 
                                        axis2_char_t *key, 
                                        axis2_bool_t persistent);
    
    /**
     * @return
     */
    axis2_qname_t* (AXIS2_CALL *get_paused_handler_name)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env);
    
    /**
     * @return
     */
    axis2_char_t* (AXIS2_CALL *get_paused_phase_name)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env);
    
    /**
     * @param name
     */
    axis2_status_t (AXIS2_CALL *set_paused_phase_name)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env, 
                                                        axis2_char_t *paused_phase_name);
    
    /**
     * @return
     */
    axis2_char_t* (AXIS2_CALL *get_soap_action)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    /**
     * @param string
     */
    axis2_status_t (AXIS2_CALL *set_soap_action)(struct axis2_msg_ctx *msg_ctx, 
                                                 axis2_env_t **env,
                                                 axis2_char_t *soap_action);
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_doing_mtom)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_doing_mtom)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env,
                                                axis2_bool_t doing_mtom);
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *get_doing_rest)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    /**
     * @param b
     */
    axis2_status_t (AXIS2_CALL *set_doing_rest)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, axis2_bool_t doing_rest);
    
    axis2_status_t (AXIS2_CALL *set_do_rest_through_post)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env, 
                                                            axis2_bool_t do_rest_through_post);
    
    axis2_bool_t (AXIS2_CALL *get_do_rest_through_post)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env);
    
    axis2_bool_t (AXIS2_CALL *get_is_soap_11)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    struct axis2_svc_grp_ctx* (AXIS2_CALL *get_svc_grp_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_svc_grp_ctx)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                struct axis2_svc_grp_ctx *svc_grp_ctx);
    
    
    struct axis2_operation* (AXIS2_CALL *get_operation)(struct axis2_msg_ctx *msg_ctx, 
                                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_operation)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                struct axis2_operation *operation);
    
    struct axis2_svc* (AXIS2_CALL *get_svc)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_svc)(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, struct axis2_svc *svc);
    
    struct axis2_svc_grp* (AXIS2_CALL *get_svc_grp)(struct axis2_msg_ctx *msg_ctx, 
                                                        axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_svc_grp)(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                struct axis2_svc_grp *svc_grp);
    
    axis2_char_t* (AXIS2_CALL *get_svc_grp_ctx_id)(struct axis2_msg_ctx *msg_ctx, 
                                                 axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *set_svc_grp_ctx_id)(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env,
                                                    axis2_char_t *svc_grp_ctx_id);
    
    /**
     * @return
     */
    axis2_bool_t (AXIS2_CALL *
    is_paused) (axis2_msg_ctx_t *msg_ctx,
                            axis2_env_t **env) 
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_msg_ctx
{
    axis2_msg_ctx_ops_t *ops;    
};


AXIS2_DECLARE(axis2_msg_ctx_t *) AXIS2_CALL
axis2_msg_ctx_create (axis2_env_t **env,
                        struct axis2_conf_ctx *conf_ctx,
                        struct axis2_transport_in_desc *transport_in_desc,
                        struct axis2_transport_out_desc *transport_out_desc,
                        struct axis2_session_ctx *session_ctx);
    
/************************** Start of function macros **************************/


#define AXIS2_MSG_CTX_GET_BASE(msg_ctx, env) ((msg_ctx)->ops->get_base(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_PARENT(msg_ctx, env) ((msg_ctx)->ops->get_parent(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_PARENT(msg_ctx, env, parent) ((msg_ctx)->ops->get_parent(msg_ctx, env, parent))
#define AXIS2_MSG_CTX_FREE(msg_ctx, env) ((msg_ctx)->ops->free(msg_ctx, env))
#define AXIS2_MSG_CTX_INIT(msg_ctx, env, engine_config) ((msg_ctx)->ops->init(msg_ctx, env, engine_config))

/*
#define AXIS2_MSG_CTX_GET_FAULT_TO(msg_ctx, env) ((msg_ctx)->ops->get_fault_to(msg_ctx, env))
*/
/*
#define AXIS2_MSG_CTX_GET_FROM(msg_ctx, env) ((msg_ctx)->ops->get_from(msg_ctx, env))
*/
#define AXIS2_MSG_CTX_GET_IN_FAULT_FLOW(msg_ctx, env) ((msg_ctx)->ops->get_in_fault_flow(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_IN_GET_SOAP_ENVELOPE(msg_ctx, env) ((msg_ctx)->ops->get_soap_envelope(msg_ctx, env))
/*
#define AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env) ((msg_ctx)->ops->get_msg_id(msg_ctx, env))
*/
#define AXIS2_MSG_CTX_GET_PROCESS_FAULT(msg_ctx, env) ((msg_ctx)->ops->get_process_fault(msg_ctx, env))
/*
#define AXIS2_MSG_CTX_GET_RELATES_TO(msg_ctx, env) ((msg_ctx)->ops->get_relates_to(msg_ctx, env))
*/
/*
#define AXIS2_MSG_CTX_GET_REPLY_TO(msg_ctx, env) ((msg_ctx)->ops->get_reply_to(msg_ctx, env))
*/
#define AXIS2_MSG_CTX_GET_RESPONSE_WRITTEN(msg_ctx, env) ((msg_ctx)->ops->get_response_written(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env) ((msg_ctx)->ops->get_server_side(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_SESSION_CTX(msg_ctx, env) ((msg_ctx)->ops->get_session_ctx(msg_ctx, env))
/*
#define AXIS2_MSG_CTX_GET_TO(msg_ctx, env) ((msg_ctx)->ops->get_to(msg_ctx, env))
*/
/*
#define AXIS2_MSG_CTX_SET_FAULT_TO(msg_ctx, env, reference) ((msg_ctx)->ops->set_fault_to(msg_ctx, env, reference))
*/
/*
#define AXIS2_MSG_CTX_SET_FROM(msg_ctx, env, reference) ((msg_ctx)->ops->set_from(msg_ctx, env, reference))
*/
#define AXIS2_MSG_CTX_SET_IN_FAULT_FLOW(msg_ctx, env, in_fault_flow) ((msg_ctx)->ops->get_in_fault_flow(msg_ctx, env, in_fault_flow))
#define AXIS2_MSG_CTX_SET_ENVELOPE(msg_ctx, env, soap_envelope) ((msg_ctx)->ops->set_envelope(msg_ctx, env, soap_envelope))
/*
#define AXIS2_MSG_CTX_SET_MESSAGE_ID(msg_ctx, env, message_id) ((msg_ctx)->ops->set_message_id(msg_ctx, env, message_id))
*/
#define AXIS2_MSG_CTX_SET_PROCESS_FAULT(msg_ctx, env, process_fault) ((msg_ctx)->ops->set_process_fault(msg_ctx, env, process_fault))
/*
#define AXIS2_MSG_CTX_SET_RELATES_TO(msg_ctx, env, reference) ((msg_ctx)->ops->set_relates_to(msg_ctx, env, reference))
*/
/*
#define AXIS2_MSG_CTX_SET_REPLY_TO(msg_ctx, env, reference) ((msg_ctx)->ops->set_reply_to(msg_ctx, env, reference))
*/
#define AXIS2_MSG_CTX_SET_RESPONSE_WRITTEN(msg_ctx, env, response_written) ((msg_ctx)->ops->set_response_written(msg_ctx, env, response_written))
#define AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, server_side) ((msg_ctx)->ops->set_server_side(msg_ctx, env, server_side))
/*
#define AXIS2_MSG_CTX_SET_TO(msg_ctx, env, reference) ((msg_ctx)->ops->set_response_written(msg_ctx, env, reference))
*/
#define AXIS2_MSG_CTX_GET_NEW_THREAD_REQUIRED(msg_ctx, env) ((msg_ctx)->ops->set_response_written(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_NEW_THREAD_REQUIRED(msg_ctx, env, new_thread_required) ((msg_ctx)->ops->set_response_written(msg_ctx, env, new_thread_required))
/*
#define AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env, action_uri) ((msg_ctx)->ops->set_wsa_action(msg_ctx, env, action_uri))
#define AXIS2_MSG_CTX_GET_WSA_ACTION(msg_ctx, env) ((msg_ctx)->ops->get_wsa_action(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_WSA_MESSAGE_ID(msg_ctx, env, message_id) ((msg_ctx)->ops->set_wsa_message_id(msg_ctx, env, message_id))
#define AXIS2_MSG_CTX_GET_WSA_MESSAGE_ID(msg_ctx, env) ((msg_ctx)->ops->get_wsa_message_id(msg_ctx, env))
*/
#define AXIS2_MSG_CTX_GET_MSG_INFO_HEADERS(msg_ctx, env) ((msg_ctx)->ops->get_msg_info_headers(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_PAUSED(msg_ctx, env) ((msg_ctx)->ops->get_paused(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_PAUSED(msg_ctx, env, paused) ((msg_ctx)->ops->set_paused(msg_ctx, env, paused))
#define AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(msg_ctx, env) ((msg_ctx)->ops->get_transport_in_desc(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env) ((msg_ctx)->ops->get_transport_out_desc(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_TRANSPORT_IN_DESC(msg_ctx, env, transport_in_desc) ((msg_ctx)->ops->set_transport_in_desc(msg_ctx, env, transport_in_desc))
#define AXIS2_MSG_CTX_SET_TRANSPORT_OUT_DESC(msg_ctx, env, transport_out_desc) ((msg_ctx)->ops->set_transport_out_desc(msg_ctx, env, transport_out_desc))
#define AXIS2_MSG_CTX_GET_OPERATION_CTX(msg_ctx, env) ((msg_ctx)->ops->get_operation_ctx(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_OPERATION_CTX(msg_ctx, env, operation_ctx) ((msg_ctx)->ops->set_operation_ctx(msg_ctx, env, operation_ctx))
#define AXIS2_MSG_CTX_GET_OUTPUT_WRITTEN(msg_ctx, env) ((msg_ctx)->ops->get_output_written(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_OUTPUT_WRITTEN(msg_ctx, env, output_written) ((msg_ctx)->ops->set_output_written(msg_ctx, env, output_written))
#define AXIS2_MSG_CTX_GET_SVC_CTX_ID(msg_ctx, env) ((msg_ctx)->ops->get_svc_ctx_id(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_SVC_CTX_ID(msg_ctx, env, svc_ctx_id) ((msg_ctx)->ops->set_svc_ctx_id(msg_ctx, env, svc_ctx_id))
#define AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env) ((msg_ctx)->ops->get_conf_ctx(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_CONF_CTX(msg_ctx, env, conf_ctx) ((msg_ctx)->ops->set_conf_ctx(msg_ctx, env, conf_ctx))
#define AXIS2_MSG_CTX_GET_SVC_CTX(msg_ctx, env) ((msg_ctx)->ops->get_svc_ctx(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx) ((msg_ctx)->ops->set_svc_ctx(msg_ctx, env, svc_ctx))
#define AXIS2_MSG_CTX_SET_MSG_INFO_HEADERS(msg_ctx, env, msg_info_headers) ((msg_ctx)->ops->set_msg_info_headers(msg_ctx, env, msg_info_headers))
#define AXIS2_MSG_CTX_GET_PARAMETER(msg_ctx, env, key) ((msg_ctx)->ops->get_parameter(msg_ctx, env, key))
#define AXIS2_MSG_CTX_GET_MODULE_PARAMETER(msg_ctx, env, key, module_name, handler_desc) ((msg_ctx)->ops->get_module_parameter(msg_ctx, env, key, module_name, handler_desc))
#define AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, key, persistent) ((msg_ctx)->ops->get_parameter(msg_ctx, env, key, persistent))
#define AXIS2_MSG_CTX_GET_PAUSED_HANDLER_NAME(msg_ctx, env) ((msg_ctx)->ops->get_paused_handler_name(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_PAUSED_PHASE_NAME(msg_ctx, env) ((msg_ctx)->ops->get_paused_phase_name(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_PAUSED_PHASE_NAME(msg_ctx, env, paused_phase_name) ((msg_ctx)->ops->set_paused_phase_name(msg_ctx, env, paused_phase_name))
#define AXIS2_MSG_CTX_GET_SOAP_ACTION(msg_ctx, env) ((msg_ctx)->ops->get_soap_action(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_SOAP_ACTION(msg_ctx, env, soap_action) ((msg_ctx)->ops->set_soap_action(msg_ctx, env, soap_action))
#define AXIS2_MSG_CTX_GET_DOING_MTOM(msg_ctx, env) ((msg_ctx)->ops->get_doing_mtom(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_DOING_MTOM(msg_ctx, env, set_doing_mtom) ((msg_ctx)->ops->get_doing_mtom(msg_ctx, env, set_doing_mtom))
#define AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env) ((msg_ctx)->ops->get_doing_rest(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx, env, doing_rest) ((msg_ctx)->ops->get_doing_rest(msg_ctx, env, doing_rest))
#define AXIS2_MSG_CTX_SET_DO_REST_THROUGH_POST(msg_ctx, env, do_rest_through_post) ((msg_ctx)->ops->get_doing_rest(msg_ctx, env, do_rest_through_post))
#define AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env) ((msg_ctx)->ops->get_is_soap_11(msg_ctx, env))
#define AXIS2_MSG_CTX_GET_SVC_GRP_CTX(msg_ctx, env) ((msg_ctx)->ops->get_svc_grp_ctx(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_SVC_GRP_CTX(msg_ctx, env, svc_grp_ctx) ((msg_ctx)->ops->set_svc_grp_ctx(msg_ctx, env, svc_grp_ctx))
#define AXIS2_MSG_CTX_GET_OPERATION(msg_ctx, env) ((msg_ctx)->ops->get_operation(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_OPERATION(msg_ctx, env, operation) ((msg_ctx)->ops->get_operation(msg_ctx, env, operation))
#define AXIS2_MSG_CTX_GET_SVC(msg_ctx, env) ((msg_ctx)->ops->get_svc(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_SVC(msg_ctx, env, svc) ((msg_ctx)->ops->set_svc(msg_ctx, env, svc))
#define AXIS2_MSG_CTX_GET_SVC_GRP(msg_ctx, env) ((msg_ctx)->ops->get_svc_grp(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_SVC_GRP(msg_ctx, env, svc_grp) ((msg_ctx)->ops->set_svc_grp(msg_ctx, env, svc_grp))
#define AXIS2_MSG_CTX_GET_SVC_GRP_CTX_ID(msg_ctx, env) ((msg_ctx)->ops->get_svc_grp_ctx_id(msg_ctx, env))
#define AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env, svc_grp_ctx_id) ((msg_ctx)->ops->set_svc_grp_ctx_id(msg_ctx, env, svc_grp_ctx_id))
#define AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env) ((msg_ctx)->ops->is_paused(msg_ctx, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_MSG_CTX_H */
