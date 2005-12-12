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
 
#include <axis2_msg_ctx.h>
#include <axis2_operation.h>
#include <axis2_svc.h>
#include <axis2_svc_grp.h>
#include <axis2_engine_config.h>

typedef struct axis2_msg_ctx_impl axis2_msg_ctx_impl_t;

/** 
 * @brief Message Context struct impl
 * Axis2 Message Context impl  
 */  
struct axis2_msg_ctx_impl
{
    /** message context interface struct */
	axis2_msg_ctx_t msg_ctx;
    /** base context struct */
    axis2_ctx_t *base;
    /** parent of message context is an operation context instance */
    struct axis2_operation_ctx *parent;
    /** process fault enabled? */    
    axis2_bool_t process_fault;
    /**
     * Addressing Information for Axis 2
     * Following Properties will be kept inside this, these fields will be initially filled by
     * the transport. Then later an addressing handler will make relevant changes to this, if addressing
     * information is present in the SOAP header.
     */
    axis2_msg_info_headers_t *msg_info_headers;

    struct axis2_operation_ctx *operation_ctx;
    struct axis2_svc_ctx *svc_ctx;
    struct axis2_svc_grp_ctx *svc_grp_ctx;
    struct axis2_conf_ctx *conf_ctx;
    /** session context */
    struct axis2_session_ctx *session_ctx;
    
    /** operation */
    axis2_operation_t *operation;
    /** service */
    axis2_svc_t *svc;
    /** service group */
    axis2_svc_grp_t *svc_grp;
    
    /*
    axis2_transport_in_desc_t *transport_in_desc;
    axis2_transport_out_desc_t *transport_out_desc;
    */
    /** SOAP envelope */
    struct axis2_soap_envelope *soap_envelope;
    /** response written? */
    axis2_bool_t response_written;
    /** in fault flow? */
    axis2_bool_t in_fault_flow;
    /** is this server side? */
    axis2_bool_t server_side;
    /** message ID */
    axis2_char_t *message_id;
    /** new thread required? */
    axis2_bool_t new_thread_required;
    /** paused */
    axis2_bool_t paused;
    /** output written? */
    axis2_bool_t output_written;
    /** service context ID */
    axis2_char_t *svc_ctx_id;
    /** paused phase ame */
    axis2_char_t *paused_phase_name;
    /** paused handler name */
    axis2_qname_t *paused_handler_name;
    /** SOAP action */
    axis2_char_t *soap_action;
    /** are we doing MTOM now? */
    axis2_bool_t doing_mtom;
    /** are we doing REST now? */
    axis2_bool_t doing_rest;
    /** Rest through HTTP POST? */
    axis2_bool_t do_rest_through_post;
    /** use SOAP 1.1? */
    axis2_bool_t is_soap_11;
    /** service group context id */
    axis2_char_t *svc_grp_ctx_id;
    /** qname of transport in */
    axis2_qname_t *transport_in_desc_qname;
    /** qname of transport out */
    axis2_qname_t *transport_out_desc_qname;;
    /** service group id */
    axis2_char_t *svc_grp_id;
    /** service description qname */
    axis2_qname_t *svc_qname;
    /** operation qname */
    axis2_qname_t *operation_qname;
};

#define AXIS2_INTF_TO_IMPL(msg_ctx) ((axis2_msg_ctx_impl_t *) msg_ctx)

/*************************** Function headers *********************************/

struct axis2_ctx* AXIS2_CALL
axis2_msg_ctx_get_base(struct axis2_msg_ctx *msg_ctx, 
                        axis2_env_t **env);
struct axis2_operation_ctx* AXIS2_CALL
axis2_msg_ctx_get_parent(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_parent(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            struct axis2_operation_ctx *parent);

axis2_status_t AXIS2_CALL
axis2_msg_ctx_free (axis2_msg_ctx_t *msg_ctx,
                        axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_init(struct axis2_msg_ctx *msg_ctx, 
                    axis2_env_t **env, 
                    struct axis2_engine_config *engine_config);
/*axis2_endpoint_ref_t *AXIS2_CALL 
axis2_msg_ctx_get_fault_to(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
*/
/*axis2_endpoint_ref_t *AXIS2_CALL
axis2_msg_ctx_get_from(struct axis2_msg_ctx *msg_ctx, 
                        axis2_env_t **env);
*/
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_in_fault_flow(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
struct axis2_soap_envelope* AXIS2_CALL
axis2_msg_ctx_get_soap_envelope(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_char_t* AXIS2_CALL
axis2_msg_ctx_get_msg_id(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_process_fault(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_relates_to_t* AXIS2_CALL
axis2_msg_ctx_get_relates_to(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);

/*axis2_endpoint_ref_t *AXIS2_CALL
axis2_msg_ctx_get_reply_to(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
*/
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_response_written(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_server_side(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
struct axis2_session_ctx* AXIS2_CALL
axis2_msg_ctx_get_session_ctx(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_endpoint_ref_t *AXIS2_CALL
axis2_msg_ctx_get_to(struct axis2_msg_ctx *msg_ctx, 
                        axis2_env_t **env);

/*axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_fault_to(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            axis2_endpoint_ref_t *reference);
*/
/*
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_from(struct axis2_msg_ctx *msg_ctx, 
                        axis2_env_t **env, 
                        axis2_endpoint_ref_t *reference);
*/
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_in_fault_flow(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env, 
                                axis2_bool_t in_fault_flow);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_envelope(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            struct axis2_soap_envelope *soap_envelope);
/*axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_message_id(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env, 
                                axis2_char_t *message_id);
*/
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_process_fault(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env, 
                                axis2_bool_t process_fault);
/*axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_relates_to(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env,
                                RelatesTo reference);
*/
/*axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_reply_to(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            axis2_endpoint_ref_t *referance);
*/
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_response_written(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env, 
                                    axis2_bool_t response_written);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_server_side(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env,
                                axis2_bool_t server_side);
/*axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_to(struct axis2_msg_ctx *msg_ctx, 
                        axis2_env_t **env, 
                        axis2_endpoint_ref_t *referance);
*/
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_new_thread_required(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_new_thread_required(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env, 
                                        axis2_bool_t new_thread_required);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_wsa_action(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env, 
                                axis2_char_t *action_uri);
axis2_char_t* AXIS2_CALL
axis2_msg_ctx_get_wsa_action(struct axis2_msg_ctx *msg_ctx, 
                             axis2_env_t **env);
/*
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_wsa_message_id(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env, 
                                    axis2_char_t *message_id);
axis2_char_t* AXIS2_CALL
axis2_msg_ctx_get_wsa_message_id(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);
*/

/*axis2_msg_info_headers_t* AXIS2_CALL
axis2_msg_ctx_get_msg_info_headers(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);
                                    */
                                    
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_paused(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_paused(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            axis2_bool_t paused);
struct axis2_transport_in_desc* AXIS2_CALL
axis2_msg_ctx_get_transport_in_desc(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);
struct axis2_transport_out_desc* AXIS2_CALL
axis2_msg_ctx_get_transport_out_desc(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_transport_in_desc(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env, 
                                    struct axis2_transport_in_desc *transport_in_desc);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_transport_out_desc(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env, 
                                        struct axis2_transport_out_desc *transport_out_desc); 
struct axis2_operation_ctx* AXIS2_CALL
axis2_msg_ctx_get_operation_ctx(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_operation_ctx(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env, 
                                struct axis2_operation_ctx * operation_ctx);
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_output_written(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_output_written(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env,
                                    axis2_bool_t output_written);
axis2_char_t* AXIS2_CALL
axis2_msg_ctx_get_svc_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_svc_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env,
                                axis2_char_t *svc_ctx_id);
struct axis2_conf_ctx* AXIS2_CALL
axis2_msg_ctx_get_conf_ctx(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
struct axis2_svc_ctx* AXIS2_CALL
axis2_msg_ctx_get_svc_ctx(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_conf_ctx(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env,
                            struct axis2_conf_ctx *conf_ctx);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_svc_ctx(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            struct axis2_svc_ctx *svc_ctx);
/*axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_msg_info_headers(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env, 
                                    axis2_msg_info_headers_t *msg_info_headers);
                            */
axis2_param_t* AXIS2_CALL
axis2_msg_ctx_get_parameter(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, axis2_char_t *key);
axis2_param_t* AXIS2_CALL
axis2_msg_ctx_get_module_parameter(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env, 
                                    axis2_char_t *key, axis2_char_t *module_name, 
                                    axis2_handler_desc_t *handler_desc);
void* AXIS2_CALL
axis2_msg_ctx_get_property(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            axis2_char_t *key, axis2_bool_t persistent);
axis2_qname_t* AXIS2_CALL
axis2_msg_ctx_get_paused_handler_name(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env);
axis2_char_t* AXIS2_CALL
axis2_msg_ctx_get_paused_phase_name(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_paused_phase_name(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env, 
                                    axis2_char_t *paused_phase_name);
axis2_char_t* AXIS2_CALL
axis2_msg_ctx_get_soap_action(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_soap_action(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env,
                                axis2_char_t *soap_action);
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_doing_mtom(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_doing_mtom(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env,
                                axis2_bool_t doing_mtom);
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_doing_rest(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_doing_rest(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t doing_rest);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_do_rest_through_post(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env, 
                                        axis2_bool_t do_rest_through_post);
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_do_rest_through_post(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env);
axis2_bool_t AXIS2_CALL
axis2_msg_ctx_get_is_soap_11(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
struct axis2_svc_grp_ctx* AXIS2_CALL
axis2_msg_ctx_get_svc_grp_ctx(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);    
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_svc_grp_ctx(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env, 
                                struct axis2_svc_grp_ctx *svc_grp_ctx);
axis2_operation_t* AXIS2_CALL
axis2_msg_ctx_get_operation(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_operation(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            axis2_operation_t *operation);
axis2_svc_t* AXIS2_CALL
axis2_msg_ctx_get_svc(struct axis2_msg_ctx *msg_ctx, 
                        axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_svc(struct axis2_msg_ctx *msg_ctx, 
                        axis2_env_t **env, 
                        axis2_svc_t *svc);
axis2_svc_grp_t* AXIS2_CALL
axis2_msg_ctx_get_svc_grp(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_svc_grp(struct axis2_msg_ctx *msg_ctx, 
                            axis2_env_t **env, 
                            axis2_svc_grp_t *svc_grp);
axis2_char_t* AXIS2_CALL
axis2_msg_ctx_get_svc_grp_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env);
axis2_status_t AXIS2_CALL
axis2_msg_ctx_set_svc_grp_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                    axis2_env_t **env,
                                    axis2_char_t *svc_grp_ctx_id);

axis2_bool_t AXIS2_CALL
axis2_msg_ctx_is_paused(axis2_msg_ctx_t *msg_ctx,
                        axis2_env_t **env);

axis2_svc_t* AXIS2_CALL axis2_msg_ctx_find_svc(axis2_msg_ctx_t *msg_ctx, 
                axis2_env_t **env);
axis2_operation_t* AXIS2_CALL axis2_msg_ctx_find_operation(axis2_msg_ctx_t *msg_ctx,
                            axis2_env_t **env,
                            axis2_svc_t *svc);


/************************* End of function headers ****************************/	

axis2_msg_ctx_t * AXIS2_CALL
axis2_msg_ctx_create (axis2_env_t **env,
                        struct axis2_conf_ctx *conf_ctx,
                        struct axis2_transport_in_desc *transport_in_desc,
                        struct axis2_transport_out_desc *transport_out_desc,
                        struct axis2_session_ctx *session_ctx)
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    msg_ctx_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_msg_ctx_impl_t) );
    if (!msg_ctx_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    msg_ctx_impl->msg_ctx.ops = NULL;
    msg_ctx_impl->base = NULL;
    msg_ctx_impl->process_fault = AXIS2_FALSE;
    /*msg_ctx_impl->msg_info_headers = NULL;*/
    msg_ctx_impl->operation_ctx = NULL;
    msg_ctx_impl->svc_ctx = NULL;
    msg_ctx_impl->svc_grp_ctx = NULL;
    msg_ctx_impl->conf_ctx = NULL;
    msg_ctx_impl->session_ctx = NULL;
    msg_ctx_impl->operation = NULL;
    msg_ctx_impl->svc = NULL;
    msg_ctx_impl->svc_grp = NULL;    
    /*msg_ctx_impl->transport_in_desc = NULL;
    msg_ctx_impl->transport_out_desc = NULL;*/
    msg_ctx_impl->soap_envelope = NULL;
    msg_ctx_impl->response_written = AXIS2_FALSE;
    msg_ctx_impl->in_fault_flow = AXIS2_FALSE;
    msg_ctx_impl->server_side = AXIS2_FALSE;
    msg_ctx_impl->message_id = NULL;
    msg_ctx_impl->new_thread_required = AXIS2_FALSE;
    msg_ctx_impl->paused = AXIS2_FALSE;
    msg_ctx_impl->output_written = AXIS2_FALSE;
    msg_ctx_impl->svc_ctx_id = NULL;
    msg_ctx_impl->paused_phase_name = NULL;
    msg_ctx_impl->paused_handler_name = NULL;
    msg_ctx_impl->soap_action = NULL;
    msg_ctx_impl->doing_mtom = AXIS2_FALSE;
    msg_ctx_impl->doing_rest = AXIS2_FALSE;
    msg_ctx_impl->do_rest_through_post = AXIS2_TRUE;
    msg_ctx_impl->is_soap_11 = AXIS2_TRUE;
    msg_ctx_impl->svc_grp_ctx_id = NULL;
    msg_ctx_impl->transport_in_desc_qname = NULL;
    msg_ctx_impl->transport_out_desc_qname = NULL;
    msg_ctx_impl->svc_grp_id = NULL;
    msg_ctx_impl->svc_qname = NULL;
    msg_ctx_impl->operation_qname = NULL;
    
    msg_ctx_impl->base = axis2_ctx_create(env);
    if (!(msg_ctx_impl->base))
    {
        axis2_msg_ctx_free(&(msg_ctx_impl->msg_ctx), env);
        return NULL;
    }
    
    /*if (transport_in_desc)
        msg_ctx_impl->transport_in_desc = transport_in_desc;
    if (transport_out_desc)
        msg_ctx_impl->transport_out_desc = transport_out_desc;*/
    if (conf_ctx)
        msg_ctx_impl->conf_ctx = conf_ctx;

    /*if (msg_ctx_impl->transport_in_desc)
        msg_ctx_impl->transport_in_desc_qname = AXIS2_TRANSPORT_IN_GET_QNAME(transport_in_desc, env);
    if (msg_ctx_impl->transport_out_desc)
        msg_ctx_impl->transport_out_desc_qname = AXIS2_TRANSPORT_OUT_GET_QNAME(transport_out_desc, env);*/
    
    if (session_ctx) 
    {
        msg_ctx_impl->session_ctx = session_ctx;
    } 
    else 
    {
        /*msg_ctx_impl->session_ctx = axis2_session_ctx_create(env);
        if (!(msg_ctx_impl->session_ctx))
        {
            axis2_msg_ctx_free(&(msg_ctx_impl->msg_ctx), env);
            return NULL;
        }*/
    }
    
    /*msg_ctx_impl->msg_info_headers = axis2_msg_info_headers_create(env);
    if (!(msg_ctx_impl->msg_info_headers))
    {
        axis2_msg_ctx_free(&(msg_ctx_impl->msg_ctx), env);
        return NULL;
    }*/
    
    /* initialize operations */
    msg_ctx_impl->msg_ctx.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_msg_ctx_ops_t) );
    if (!msg_ctx_impl->msg_ctx.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_msg_ctx_free(&(msg_ctx_impl->msg_ctx), env);
        return NULL;        
    }

    msg_ctx_impl->msg_ctx.ops->get_base = axis2_msg_ctx_get_base;
    msg_ctx_impl->msg_ctx.ops->get_parent = axis2_msg_ctx_get_parent;
    msg_ctx_impl->msg_ctx.ops->set_parent = axis2_msg_ctx_set_parent;
    msg_ctx_impl->msg_ctx.ops->free = axis2_msg_ctx_free;
    msg_ctx_impl->msg_ctx.ops->init = axis2_msg_ctx_init;
/*
    msg_ctx_impl->msg_ctx.ops->get_fault_to = axis2_msg_ctx_get_fault_to;
    msg_ctx_impl->msg_ctx.ops->get_from = axis2_msg_ctx_get_from;
*/
    msg_ctx_impl->msg_ctx.ops->get_in_fault_flow = axis2_msg_ctx_get_in_fault_flow;
    msg_ctx_impl->msg_ctx.ops->get_soap_envelope = axis2_msg_ctx_get_soap_envelope;
    msg_ctx_impl->msg_ctx.ops->get_msg_id = axis2_msg_ctx_get_msg_id;
    msg_ctx_impl->msg_ctx.ops->get_process_fault = axis2_msg_ctx_get_process_fault;
    

    msg_ctx_impl->msg_ctx.ops->get_relates_to = axis2_msg_ctx_get_relates_to;

/*
    msg_ctx_impl->msg_ctx.ops->get_reply_to = axis2_msg_ctx_get_reply_to;
*/
    msg_ctx_impl->msg_ctx.ops->get_response_written = axis2_msg_ctx_get_response_written;
    msg_ctx_impl->msg_ctx.ops->get_server_side = axis2_msg_ctx_get_server_side;
    msg_ctx_impl->msg_ctx.ops->get_session_ctx = axis2_msg_ctx_get_session_ctx;
    msg_ctx_impl->msg_ctx.ops->get_to = axis2_msg_ctx_get_to;
/*
    msg_ctx_impl->msg_ctx.ops->set_fault_to = axis2_msg_ctx_set_fault_to;
*/
/*
    msg_ctx_impl->msg_ctx.ops->set_from = axis2_msg_ctx_set_from;
*/
    msg_ctx_impl->msg_ctx.ops->set_in_fault_flow = axis2_msg_ctx_set_in_fault_flow;
    msg_ctx_impl->msg_ctx.ops->set_envelope = axis2_msg_ctx_set_envelope;
/*
    msg_ctx_impl->msg_ctx.ops->set_message_id = axis2_msg_ctx_set_message_id;
*/
    msg_ctx_impl->msg_ctx.ops->set_process_fault = axis2_msg_ctx_set_process_fault;
    
/*
    msg_ctx_impl->msg_ctx.ops->set_relates_to = axis2_msg_ctx_set_relates_to;
*/
/*
    msg_ctx_impl->msg_ctx.ops->set_reply_to = axis2_msg_ctx_set_reply_to;
*/
    msg_ctx_impl->msg_ctx.ops->set_response_written = axis2_msg_ctx_set_response_written;
    msg_ctx_impl->msg_ctx.ops->set_server_side = axis2_msg_ctx_set_server_side;
/*
    msg_ctx_impl->msg_ctx.ops->set_to = axis2_msg_ctx_set_to;
*/
    msg_ctx_impl->msg_ctx.ops->get_new_thread_required = axis2_msg_ctx_get_new_thread_required;
    msg_ctx_impl->msg_ctx.ops->set_new_thread_required = axis2_msg_ctx_set_new_thread_required;

    msg_ctx_impl->msg_ctx.ops->set_wsa_action = axis2_msg_ctx_set_wsa_action;
    msg_ctx_impl->msg_ctx.ops->get_wsa_action = axis2_msg_ctx_get_wsa_action;
/*    msg_ctx_impl->msg_ctx.ops->set_wsa_message_id = axis2_msg_ctx_set_wsa_message_id;
    msg_ctx_impl->msg_ctx.ops->get_wsa_message_id = axis2_msg_ctx_get_wsa_message_id;
*/
    /*msg_ctx_impl->msg_ctx.ops->get_msg_info_headers = axis2_msg_ctx_get_msg_info_headers;*/
    msg_ctx_impl->msg_ctx.ops->get_paused = axis2_msg_ctx_get_paused;
    msg_ctx_impl->msg_ctx.ops->set_paused = axis2_msg_ctx_set_paused;
    msg_ctx_impl->msg_ctx.ops->get_transport_in_desc = axis2_msg_ctx_get_transport_in_desc;
    msg_ctx_impl->msg_ctx.ops->get_transport_out_desc = axis2_msg_ctx_get_transport_out_desc;
    msg_ctx_impl->msg_ctx.ops->set_transport_in_desc = axis2_msg_ctx_set_transport_in_desc;
    msg_ctx_impl->msg_ctx.ops->set_transport_out_desc = axis2_msg_ctx_set_transport_out_desc;
    msg_ctx_impl->msg_ctx.ops->get_operation_ctx = axis2_msg_ctx_get_operation_ctx;
    msg_ctx_impl->msg_ctx.ops->set_operation_ctx = axis2_msg_ctx_set_operation_ctx;
    msg_ctx_impl->msg_ctx.ops->get_output_written = axis2_msg_ctx_get_output_written;
    msg_ctx_impl->msg_ctx.ops->set_output_written = axis2_msg_ctx_set_output_written;
    msg_ctx_impl->msg_ctx.ops->get_svc_ctx_id = axis2_msg_ctx_get_svc_ctx_id;
    msg_ctx_impl->msg_ctx.ops->set_svc_ctx_id = axis2_msg_ctx_set_svc_ctx_id;
    msg_ctx_impl->msg_ctx.ops->get_conf_ctx = axis2_msg_ctx_get_conf_ctx;
    msg_ctx_impl->msg_ctx.ops->get_svc_ctx = axis2_msg_ctx_get_svc_ctx;
    msg_ctx_impl->msg_ctx.ops->set_conf_ctx = axis2_msg_ctx_set_conf_ctx;
    msg_ctx_impl->msg_ctx.ops->set_svc_ctx = axis2_msg_ctx_set_svc_ctx;
    /*msg_ctx_impl->msg_ctx.ops->set_msg_info_headers = axis2_msg_ctx_set_msg_info_headers;*/
    msg_ctx_impl->msg_ctx.ops->get_parameter = axis2_msg_ctx_get_parameter;
    msg_ctx_impl->msg_ctx.ops->get_module_parameter = axis2_msg_ctx_get_module_parameter;
    msg_ctx_impl->msg_ctx.ops->get_property = axis2_msg_ctx_get_property;
    msg_ctx_impl->msg_ctx.ops->get_paused_handler_name = axis2_msg_ctx_get_paused_handler_name;
    msg_ctx_impl->msg_ctx.ops->get_paused_phase_name = axis2_msg_ctx_get_paused_phase_name;
    msg_ctx_impl->msg_ctx.ops->set_paused_phase_name = axis2_msg_ctx_set_paused_phase_name;
    msg_ctx_impl->msg_ctx.ops->get_soap_action = axis2_msg_ctx_get_soap_action;
    msg_ctx_impl->msg_ctx.ops->set_soap_action = axis2_msg_ctx_set_soap_action;
    msg_ctx_impl->msg_ctx.ops->get_doing_mtom = axis2_msg_ctx_get_doing_mtom;
    msg_ctx_impl->msg_ctx.ops->set_doing_mtom = axis2_msg_ctx_set_doing_mtom;
    msg_ctx_impl->msg_ctx.ops->get_doing_rest = axis2_msg_ctx_get_doing_rest;
    msg_ctx_impl->msg_ctx.ops->set_doing_rest = axis2_msg_ctx_set_doing_rest;
    msg_ctx_impl->msg_ctx.ops->set_do_rest_through_post = axis2_msg_ctx_set_do_rest_through_post;
    msg_ctx_impl->msg_ctx.ops->get_do_rest_through_post = axis2_msg_ctx_get_do_rest_through_post;
    msg_ctx_impl->msg_ctx.ops->get_is_soap_11 = axis2_msg_ctx_get_is_soap_11;
    msg_ctx_impl->msg_ctx.ops->get_svc_grp_ctx = axis2_msg_ctx_get_svc_grp_ctx;
    msg_ctx_impl->msg_ctx.ops->set_svc_grp_ctx = axis2_msg_ctx_set_svc_grp_ctx;
    msg_ctx_impl->msg_ctx.ops->get_operation = axis2_msg_ctx_get_operation;
    msg_ctx_impl->msg_ctx.ops->set_operation = axis2_msg_ctx_set_operation;
    msg_ctx_impl->msg_ctx.ops->get_svc = axis2_msg_ctx_get_svc;
    msg_ctx_impl->msg_ctx.ops->set_svc = axis2_msg_ctx_set_svc;
    msg_ctx_impl->msg_ctx.ops->get_svc_grp = axis2_msg_ctx_get_svc_grp;
    msg_ctx_impl->msg_ctx.ops->set_svc_grp = axis2_msg_ctx_set_svc_grp;
    msg_ctx_impl->msg_ctx.ops->get_svc_grp_ctx_id = axis2_msg_ctx_get_svc_grp_ctx_id;
    msg_ctx_impl->msg_ctx.ops->set_svc_grp_ctx_id = axis2_msg_ctx_set_svc_grp_ctx_id;
    msg_ctx_impl->msg_ctx.ops->is_paused = axis2_msg_ctx_is_paused;
    msg_ctx_impl->msg_ctx.ops->find_svc = axis2_msg_ctx_find_svc;
    msg_ctx_impl->msg_ctx.ops->find_operation = axis2_msg_ctx_find_operation;
    
    return &(msg_ctx_impl->msg_ctx);
}

/******************************************************************************/
struct axis2_ctx* AXIS2_CALL axis2_msg_ctx_get_base(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->base;
}


struct axis2_operation_ctx * AXIS2_CALL axis2_msg_ctx_get_parent(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->parent;
}

/**
 * @param context
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_parent(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, struct axis2_operation_ctx *parent) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (parent)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->parent = parent;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_msg_ctx_free (axis2_msg_ctx_t *msg_ctx,
                        axis2_env_t **env)
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);

    if(msg_ctx_impl->msg_ctx.ops)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->msg_ctx.ops);
        msg_ctx_impl->msg_ctx.ops = NULL;
    }
    
    if (msg_ctx_impl->base)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->base);
        msg_ctx_impl->base = NULL;
    }
    
    /*if (msg_ctx_impl->msg_info_headers)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->msg_info_headers);
        msg_ctx_impl->msg_info_headers = NULL;
    }*/

    if (msg_ctx_impl->message_id)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->message_id);
        msg_ctx_impl->message_id = NULL;
    }
    
    if (msg_ctx_impl->svc_ctx_id)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->svc_ctx_id);
        msg_ctx_impl->svc_ctx_id = NULL;
    }
    
    if (msg_ctx_impl->paused_phase_name)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->paused_phase_name);
        msg_ctx_impl->paused_phase_name = NULL;
    }

    if (msg_ctx_impl->soap_action)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->soap_action);
        msg_ctx_impl->soap_action = NULL;
    }

    if (msg_ctx_impl->svc_grp_ctx_id)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->svc_grp_ctx_id);
        msg_ctx_impl->svc_grp_ctx_id = NULL;
    }

    if (msg_ctx_impl->svc_grp_id)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->svc_grp_id);
        msg_ctx_impl->svc_grp_id = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, msg_ctx_impl);
    
    return AXIS2_SUCCESS;
}

/**
 * The method is used to do the intialization of the EngineContext
 *
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_init(struct axis2_msg_ctx *msg_ctx, 
                    axis2_env_t **env, 
                    struct axis2_engine_config *engine_config)
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, engine_config, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    /*if (msg_ctx_impl->transport_in_desc_qname)
    {
        msg_ctx_impl->transport_in_desc = AXIS2_ENGINE_CONFIG_GET_TRANSPORT_IN(engine_config, env, msg_ctx_impl->transport_in_desc_qname);
    }*/
    
    /*if (msg_ctx_impl->transport_out_desc_qname)
    {
        msg_ctx_impl->transport_out_desc = AXIS2_ENGINE_CONFIG_GET_TRANSPORT_IN(engine_config, env, msg_ctx_impl->transport_out_desc_qname);
    }*/
    
    if (msg_ctx_impl->svc_grp_id)
    {
        /*msg_ctx_impl->svc_grp = AXIS2_ENGINE_CONFIG_GET_SVC_GRP(engine_config, env, msg_ctx_impl->svc_grp_id);*/
    }
    
    if (msg_ctx_impl->svc_qname)
    {
        /*msg_ctx_impl->svc = AXIS2_ENGINE_CONFIG_GET_SVC(engine_config, env, AXIS2_QNAME_GET_LOCALPART(msg_ctx_impl->svc_qname, env));*/
    }
    
    if (msg_ctx_impl->operation_qname)
    {
        /*if (msg_ctx_impl->svc)
            msg_ctx_impl->operation = AXIS2_SVC_GET_OPERATION_WITH_QNAME(msg_ctx_impl->svc, env, msg_ctx_impl->operation_qname);*/
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @return
 */
/*axis2_endpoint_ref_t *AXIS2_CALL axis2_msg_ctx_get_fault_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) {
    return msg_info_headersgetFaultTo();
}
*/

/**
 * @return
 */
/*axis2_endpoint_ref_t *AXIS2_CALL axis2_msg_ctx_get_from(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    return msg_info_headersgetFrom();
}
*/

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_in_fault_flow(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->in_fault_flow;
}

/**
 * @return
 */
struct axis2_soap_envelope *AXIS2_CALL axis2_msg_ctx_get_soap_envelope(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->soap_envelope;
}

/**
 * @return
 */
axis2_char_t *AXIS2_CALL axis2_msg_ctx_get_msg_id(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->msg_info_headers)
    {
        return AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(msg_ctx_impl->msg_info_headers, env);
    }
    
    return NULL;
}

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_process_fault(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->process_fault;
}

/**
 * @return
 */
axis2_relates_to_t* AXIS2_CALL axis2_msg_ctx_get_relates_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->msg_info_headers)
    {
        return AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO(msg_ctx_impl->msg_info_headers, env);
    }
    
    return NULL;
}


/**
 * @return
 */
/*axis2_endpoint_ref_t *AXIS2_CALL axis2_msg_ctx_get_reply_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) {
    return msg_info_headersgetReplyTo();
}
*/

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_response_written(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->response_written;
}

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_server_side(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->server_side;
}

/**
 * @return
 */
struct axis2_session_ctx * AXIS2_CALL axis2_msg_ctx_get_session_ctx(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->session_ctx;
}

/**
 * @return
 */
axis2_endpoint_ref_t *AXIS2_CALL axis2_msg_ctx_get_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->msg_info_headers)
    {
        return AXIS2_MSG_INFO_HEADERS_GET_TO(msg_ctx_impl->msg_info_headers, env);
    }
    
    return NULL;
}


/**
 * @param reference
 */
/*axis2_status_t AXIS2_CALL axis2_msg_ctx_set_fault_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_endpoint_ref_t *reference) {
    msg_info_headers.setFaultTo(reference);
}
*/

/**
 * @param reference
 */
/*
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_from(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_endpoint_ref_t *reference) {
    msg_info_headers.setFrom(reference);
}
*/

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_in_fault_flow(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t in_fault_flow) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->in_fault_flow = in_fault_flow;
    return  AXIS2_SUCCESS;
}

/**
 * @param soap_envelope
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_envelope(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, struct axis2_soap_envelope *soap_envelope) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (soap_envelope)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->soap_envelope  = soap_envelope ;
        /*
        axis2_char_t *soapNamespaceURI = soap_envelopegetNamespace()getName();
        if (SOAP12Constants
                .SOAP_ENVELOPE_NAMESPACE_URI
                .equals(soapNamespaceURI)) 
        {
            is_soap_11 = AXIS2_FALSE;
        } 
        else if (SOAP11Constants.SOAP_ENVELOPE_NAMESPACE_URI.equals(
                        soapNamespaceURI)) 
        {
            is_soap_11 = AXIS2_TRUE;
        } 
        else 
        {
            // set error 
        }
        */
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @param string
 */
/*axis2_status_t AXIS2_CALL axis2_msg_ctx_set_message_id(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_char_t *message_id) 
{
    msg_info_headers.setMessageId(message_id);
}
*/

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_process_fault(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t process_fault) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->process_fault  = process_fault;
    return  AXIS2_SUCCESS;
}

/**
 * @param reference
 */
/*axis2_status_t AXIS2_CALL axis2_msg_ctx_set_relates_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, RelatesTo reference) 
{
    msg_info_headers.setRelatesTo(reference);
}
*/

/**
 * @param referance
 */
/*axis2_status_t AXIS2_CALL axis2_msg_ctx_set_reply_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_endpoint_ref_t *referance) {
    msg_info_headers.setReplyTo(referance);
}
*/

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_response_written(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t response_written ) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->response_written  = response_written ;
    return  AXIS2_SUCCESS;
}

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_server_side(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t server_side) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->server_side = server_side;
    return  AXIS2_SUCCESS;
}

/**
 * @param referance
 */
/*axis2_status_t AXIS2_CALL axis2_msg_ctx_set_to(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_endpoint_ref_t *referance) 
{
    msg_info_headers.set_to(referance);
}
*/

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_new_thread_required(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->new_thread_required;
}

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_new_thread_required(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t new_thread_required) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->new_thread_required = new_thread_required;
    return AXIS2_SUCCESS;
}

/**
 * Method AXIS2_CALL axis2_msg_ctx_getExecutionChain
 */

axis2_status_t AXIS2_CALL axis2_msg_ctx_set_wsa_action(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_char_t *action_uri) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    
    if (msg_ctx_impl->msg_info_headers)
    {
        return AXIS2_MSG_INFO_HEADERS_SET_ACTION(msg_ctx_impl->msg_info_headers, env, action_uri);
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t *AXIS2_CALL axis2_msg_ctx_get_wsa_action(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    
    if (msg_ctx_impl->msg_info_headers)
    {
        return AXIS2_MSG_INFO_HEADERS_GET_ACTION(msg_ctx_impl->msg_info_headers, env);
    }
    
    return NULL;
}

/*
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_wsa_message_id(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_char_t *message_id) 
{
    msg_info_headerssetMessageId(message_id);
}

axis2_char_t *AXIS2_CALL axis2_msg_ctx_get_wsa_message_id(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->msg_info_headers.getMessageId();
}
*/

/*axis2_msg_info_headers_t *AXIS2_CALL axis2_msg_ctx_get_msg_info_headers(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->msg_info_headers;
}
*/

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_paused(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->paused;
}

/**
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_paused(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t paused) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->paused = paused;
    return AXIS2_SUCCESS;
}

/**
 * @return
 */
struct axis2_transport_in_desc * AXIS2_CALL axis2_msg_ctx_get_transport_in_desc(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return NULL;/*AXIS2_INTF_TO_IMPL(msg_ctx)->transport_in_desc;*/
}

/**
 * @return
 */
struct axis2_transport_out_desc * AXIS2_CALL axis2_msg_ctx_get_transport_out_desc(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return NULL; /*AXIS2_INTF_TO_IMPL(msg_ctx)->transport_out_desc;*/
}

/**
 * @param in
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_transport_in_desc(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, struct axis2_transport_in_desc *transport_in_desc)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    /*if (transport_in_desc)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->transport_in_desc = transport_in_desc;
        AXIS2_INTF_TO_IMPL(msg_ctx)->transport_in_desc_qname = AXIS2_TRANSPORT_IN_GET_QNAME(transport_in_desc, env);
    }*/
    
    return AXIS2_SUCCESS;
}

/**
 * @param out
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_transport_out_desc(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, struct axis2_transport_out_desc *transport_out_desc) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    /*if (transport_out_desc)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->transport_out_desc = transport_out_desc;
        AXIS2_INTF_TO_IMPL(msg_ctx)->transport_out_desc_qname = AXIS2_TRANSPORT_OUT_GET_QNAME(transport_out_desc, env);
    }*/
    
    return AXIS2_SUCCESS;
}

struct axis2_operation_ctx * AXIS2_CALL axis2_msg_ctx_get_operation_ctx(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->operation_ctx;
}

/**
 * @param context
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_operation_ctx(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, struct axis2_operation_ctx * operation_ctx) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (operation_ctx)
    {
        msg_ctx_impl->operation_ctx = operation_ctx;
        
        if (msg_ctx_impl->svc_ctx)
        {
            /*if (!(AXIS2_OPERATION_CTX_GET_PARENT(msg_ctx_impl->operation_ctx, env)))
            {
                AXIS2_OPERATION_CTX_SET_PARENT(msg_ctx_impl->operation_ctx, env, msg_ctx_impl->svc_ctx);
            }*/
        }
        axis2_msg_ctx_set_parent(msg_ctx, env, operation_ctx);
        /*axis2_msg_ctx_set_operation(msg_ctx, env, AXIS2_OPERATION_CTX_GET_OPRATION(operation_ctx, env));    */
    }
    return AXIS2_SUCCESS;
}

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_output_written(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->output_written;
}

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_output_written(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env,
                                                axis2_bool_t output_written) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->output_written = output_written;
    return AXIS2_SUCCESS;
}

/**
 * @return Returns the svc_ctx_id.
 */
axis2_char_t *AXIS2_CALL axis2_msg_ctx_get_svc_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->svc_ctx_id;
}

/**
 * @param svc_ctx_id The svc_ctx_id to AXIS2_CALL axis2_msg_ctx_set.
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_svc_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env,
                                            axis2_char_t *svc_ctx_id) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->svc_ctx_id)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->svc_ctx_id);
        msg_ctx_impl->svc_ctx_id = NULL;
    } 
    
    if (svc_ctx_id)
    {
        msg_ctx_impl->svc_ctx_id = AXIS2_STRDUP(svc_ctx_id, env);
        if (!(msg_ctx_impl->svc_ctx_id))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

struct axis2_conf_ctx *AXIS2_CALL axis2_msg_ctx_get_conf_ctx(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->conf_ctx;
}

/**
 * @return
 */
struct axis2_svc_ctx *AXIS2_CALL axis2_msg_ctx_get_svc_ctx(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->svc_ctx;
}

/**
 * @param context
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_conf_ctx(struct axis2_msg_ctx *msg_ctx, 
                                                    axis2_env_t **env,
                                                    struct axis2_conf_ctx *conf_ctx) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (conf_ctx)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->conf_ctx = conf_ctx;
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @param context
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_svc_ctx(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, 
                                            struct axis2_svc_ctx *svc_ctx) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;

    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);

    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (svc_ctx)
    {
        msg_ctx_impl->svc_ctx = svc_ctx;
        
        if(msg_ctx_impl->operation_ctx)
        {
            /*if (!AXIS2_OPERATION_CTX_GET_PARENT(msg_ctx_impl->operation_ctx, env))
                AXIS2_OPERATION_CTX_SET_PARENT(msg_ctx_impl->operation_ctx, env, svc_ctx);*/
        }
        /*set_svc(msg_ctx, env, AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);*/
    }
    
    return AXIS2_SUCCESS;
}

/**
 * @param collection
 */
/*axis2_status_t AXIS2_CALL axis2_msg_ctx_set_msg_info_headers(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env, 
                                                axis2_msg_info_headers_t *msg_info_headers) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (msg_info_headers)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->msg_info_headers = msg_info_headers;
    }
    
    return AXIS2_SUCCESS;
}
*/

/**
 * To retrive configuration descriptor parameters , it is posible to AXIS2_CALL axis2_msg_ctx_get paramater specify at
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
axis2_param_t *AXIS2_CALL axis2_msg_ctx_get_parameter(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_char_t *key) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    axis2_param_t *param = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->operation) 
    {
        /*param = AXIS2_OPERATION_GET_PARAM(msg_ctx_impl->operation, env, key);*/
        if (param)
        {
            return param;
        }
    }
    
    if (msg_ctx_impl->svc)
    {
        param = AXIS2_SVC_GET_PARAM(msg_ctx_impl->svc, env, key);
        if (param)
        {
            return param;
        }
    }
    
    if (msg_ctx_impl->svc_grp)
    {
        param = AXIS2_SVC_GRP_GET_PARAM(msg_ctx_impl->svc_grp, env, key);
        if (param)
        {
            return param;
        }
    }    
    
    if (msg_ctx_impl->conf_ctx) 
    {
        /*axis2_engine_config_t *engine_config = AXIS2_CONFIG_CTX_GET_CONFIG(msg_ctx_impl->conf_ctx, env);
        param = AXIS2_ENGINE_CONFIG_GET_PARAM(engine_config, env, key);*/
    }
    
    return param;
}


/**
 * This method is to retrive both module configuration parameters and othere paramerts
 * The searching procedure is as follows;
 * 1. Search in module configurations inside corresponding operation descripton if its three
 * 2. Search in corresponding operation if its there
 * 3. Search in module configurations inside corresponding service description if its there
 * 4. Next search in Corresponding Service description if its there
 * 5. Next sercah in module configurations inside engine_config
 * 6. Search in axis2_engine_config_t *for paramters
 * 7. Next AXIS2_CALL axis2_msg_ctx_get the corresponding module and search for the paramters
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
axis2_param_t * AXIS2_CALL axis2_msg_ctx_get_module_parameter(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, 
                                            axis2_char_t *key, axis2_char_t *module_name, 
                                            axis2_handler_desc_t *handler_desc) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    axis2_param_t *param = NULL;
    /*axis2_module_config_t *module_config = NULL;*/
    /*axis2_module_t *module = NULL;*/
    axis2_qname_t *qname = NULL;
    axis2_engine_config_t *engine_config = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    qname = axis2_qname_create(env, module_name, NULL, NULL);
    
    if (msg_ctx_impl->operation)
    {
        /*module_config = AXIS2_OPERATION_GET_MODULE_CONFIG(msg_ctx_impl->operation, env, qname);*/
        AXIS2_QNAME_FREE(qname, env);
        /*if (module_config) 
        {
            param = AXIS2_MODULE_CONFIG_GET_PARAM(module_config, env, key);
            if (param)
            {
                return param;
            } 
            else 
            {
                param = AXIS2_OPERATION_GET_PARAM(msg_ctx_impl->operation, env, key);
                if (param)
                {
                    return param;
                }
            }
        }*/
    }
    
    if (msg_ctx_impl->svc)
    {
        /*module_config = AXIS2_SVC_GET_MODULE_CONFIG(msg_ctx_impl->svc, env, qname);*/
        AXIS2_QNAME_FREE(qname, env);
        /*if (module_config) 
        {
            param = AXIS2_MODULE_CONFIG_GET_PARAM(module_config, env, key);
            if (param)
            {
                return param;
            } 
            else 
            {
                param = AXIS2_SVC_GET_PARAM(msg_ctx_impl->svc, env, key);
                if (param)
                {
                    return param;
                }
            }
        }*/
    }
    
    if (msg_ctx_impl->svc_grp)
    {
        /*module_config = AXIS2_SVC_GRP_GET_MODULE_CONFIG(msg_ctx_impl->svc_grp, env, qname);*/
        AXIS2_QNAME_FREE(qname, env);
        /*if (module_config) 
        {
            param = AXIS2_MODULE_CONFIG_GET_PARAM(module_config, env, key);
            if (param)
            {
                return param;
            } 
            else 
            {
                param = AXIS2_SVC_GRP_GET_PARAM(msg_ctx_impl->svc_grp, env, key);
                if (param)
                {
                    return param;
                }
            }
        }*/
    }
    
    /*engine_config = AXIS2_CONFIG_CTX_GET_CONFIG(msg_ctx_impl->conf_ctx, env);*/
    if (engine_config)
    {
        /*module_config = AXIS2_ENGINE_CONFIG_GET_MODULE_CONFIG(engine_config, env, qname);*/
        AXIS2_QNAME_FREE(qname, env);
    }    
    
    /*if (module_config) 
    {
        param = AXIS2_MODULE_CONFIG_GET_PARAM(module_config, env, key);
        if (param)
        {
            return param;
        } 
        else 
        {
            param = AXIS2_ENGINE_CONFIG_GET_PARAM(engine_config, env, key);
            if (param)
            {
                return param;
            }
        }
    }*/
    
    if (engine_config)
    {
        /*module = AXIS2_ENGINE_CONFIG_GET_MODULE(engine_config, env, qname);*/
        AXIS2_QNAME_FREE(qname, env);
    }
    
    /*if (module) 
    {
        param = AXIS2_MODULE_GET_PARAM(module, env, key);
        if (param)
        {
            return param;
        }
    }*/
    
    param = AXIS2_HANDLER_DESC_GET_PARAM(handler_desc, env, key);
    
    return param;
}

/* (non-Javadoc)
* @see org.apache.axis2.context.AbstractContext#AXIS2_CALL axis2_msg_ctx_getProperty(java.lang.Object, axis2_bool_t)
*/

/**
 * To acess any property AXIS2_CALL axis2_msg_ctx_set at the run time , a handler can add property to wherever he wants
 * to MesageContext , to struct axis2_operation_ctx * , to struct axis2_svc_ctx *and to ConfigurationContext.
 * This method is to retrive those properties NOT paramters
 *
 * @param key        : property Name
 * @param persistent : need to be persistent even when server re-start
 * @return Object
 */
void* AXIS2_CALL axis2_msg_ctx_get_property(struct axis2_msg_ctx *msg_ctx, 
                                axis2_env_t **env, 
                                axis2_char_t *key, axis2_bool_t persistent) 
{
/*    // search in MC
    void* obj = supergetProperty(key, persistent);
    if(obj !=null){
        return obj;
    }
    //The context hirachy might not have constructed fully, the check should
    //look for the disconnected grandparents
    // Search in Operation Context
    if (operation_ctx != null ) {
        obj = operation_ctxgetProperty(key, persistent);
        if(obj !=null){
            return obj;
        }
    }
    //Search in ServiceContext
    if (svc_ctx != null ) {
        obj = svc_ctxgetProperty(key, persistent);
        if(obj !=null){
            return obj;
        }
    }
    if (svc_grp_ctx != null ) {
        obj = svc_grp_ctxgetProperty(key, persistent);
        if(obj !=null){
            return obj;
        }
    }
    if (conf_ctx != null ) {
        // search in Configuration Context
        obj = conf_ctxgetProperty(key, persistent);
        if(obj !=null){
            return obj;
        }
    }
    return obj;
    */
    return NULL;
}

/**
 * @return
 */
axis2_qname_t *AXIS2_CALL axis2_msg_ctx_get_paused_handler_name(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->paused_handler_name;
}

/**
 * @return
 */
axis2_char_t * AXIS2_CALL axis2_msg_ctx_get_paused_phase_name(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->paused_phase_name;
}

/**
 * @param name
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_paused_phase_name(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_char_t *paused_phase_name) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->paused_phase_name)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->paused_phase_name);
        msg_ctx_impl->paused_phase_name = NULL;
    } 
    
    if (paused_phase_name)
    {
        msg_ctx_impl->paused_phase_name = AXIS2_STRDUP(paused_phase_name, env);
        if (!(msg_ctx_impl->paused_phase_name))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

/**
 * @return
 */
axis2_char_t* AXIS2_CALL axis2_msg_ctx_get_soap_action(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->soap_action;
}

/**
 * @param string
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_soap_action(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env,
                                            axis2_char_t *soap_action) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->soap_action)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->soap_action);
        msg_ctx_impl->soap_action = NULL;
    } 
    
    if (soap_action)
    {
        msg_ctx_impl->soap_action = AXIS2_STRDUP(soap_action, env);
        if (!(msg_ctx_impl->soap_action))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_doing_mtom(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->doing_mtom;
}

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_doing_mtom(struct axis2_msg_ctx *msg_ctx, 
                                        axis2_env_t **env,
                                        axis2_bool_t doing_mtom) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->doing_mtom = doing_mtom;
    return AXIS2_SUCCESS;
}

/**
 * @return
 */
axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_doing_rest(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->doing_rest;
}

/**
 * @param b
 */
axis2_status_t AXIS2_CALL axis2_msg_ctx_set_doing_rest(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t doing_rest) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->doing_rest = doing_rest;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_msg_ctx_set_do_rest_through_post(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_bool_t do_rest_through_post) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(msg_ctx)->do_rest_through_post = do_rest_through_post;
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_do_rest_through_post(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->do_rest_through_post;
}

axis2_bool_t AXIS2_CALL axis2_msg_ctx_get_is_soap_11(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->is_soap_11;
}

struct axis2_svc_grp_ctx *AXIS2_CALL axis2_msg_ctx_get_svc_grp_ctx(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->svc_grp_ctx;
}

axis2_status_t AXIS2_CALL axis2_msg_ctx_set_svc_grp_ctx(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, struct axis2_svc_grp_ctx *svc_grp_ctx) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (svc_grp_ctx)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->svc_grp_ctx = svc_grp_ctx;
    }
    
    return AXIS2_SUCCESS;
}

axis2_operation_t *AXIS2_CALL axis2_msg_ctx_get_operation(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->operation;
}

axis2_status_t AXIS2_CALL axis2_msg_ctx_set_operation(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_operation_t *operation) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (operation)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->operation = operation;
        AXIS2_INTF_TO_IMPL(msg_ctx)->operation_qname = AXIS2_OPERATION_GET_NAME(operation, env);
    }
    
    return AXIS2_SUCCESS;
}

axis2_svc_t *AXIS2_CALL axis2_msg_ctx_get_svc(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->svc;
}

axis2_status_t AXIS2_CALL axis2_msg_ctx_set_svc(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_svc_t *svc) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (svc)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->svc = svc;
        AXIS2_INTF_TO_IMPL(msg_ctx)->svc_qname = AXIS2_SVC_GET_NAME(svc, env);
    }
    
    return AXIS2_SUCCESS;
}

axis2_svc_grp_t * AXIS2_CALL axis2_msg_ctx_get_svc_grp(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->svc_grp;
}

axis2_status_t AXIS2_CALL axis2_msg_ctx_set_svc_grp(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env, axis2_svc_grp_t *svc_grp) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    if (svc_grp)
    {
        AXIS2_INTF_TO_IMPL(msg_ctx)->svc_grp = svc_grp;
        AXIS2_INTF_TO_IMPL(msg_ctx)->svc_grp_id = AXIS2_SVC_GRP_GET_NAME(svc_grp, env);
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL axis2_msg_ctx_get_svc_grp_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, NULL);    
    return AXIS2_INTF_TO_IMPL(msg_ctx)->svc_grp_ctx_id;
}

axis2_status_t AXIS2_CALL axis2_msg_ctx_set_svc_grp_ctx_id(struct axis2_msg_ctx *msg_ctx, 
                                                axis2_env_t **env,
                                                axis2_char_t *svc_grp_ctx_id) 
{
    axis2_msg_ctx_impl_t *msg_ctx_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    
    msg_ctx_impl = AXIS2_INTF_TO_IMPL(msg_ctx);
    
    if (msg_ctx_impl->svc_grp_ctx_id)
    {
        AXIS2_FREE((*env)->allocator, msg_ctx_impl->svc_grp_ctx_id);
        msg_ctx_impl->svc_grp_ctx_id = NULL;
    } 
    
    if (svc_grp_ctx_id)
    {
        msg_ctx_impl->svc_grp_ctx_id = AXIS2_STRDUP(svc_grp_ctx_id, env);
        if (!(msg_ctx_impl->svc_grp_ctx_id))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
    return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL
axis2_msg_ctx_is_paused(axis2_msg_ctx_t *msg_ctx,
                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FALSE);
    return AXIS2_INTF_TO_IMPL(msg_ctx)->paused;
}

/** TODO: there are many block to be uncommented once external depandancies are resoled,
    that is when thos structs are implemented */

/** find_svc and find_operation methods has to implemented by the dispatchers and
    the function pointers assigined to the ops of message context.
    The following are just dummy implemntations.
*/
axis2_svc_t* AXIS2_CALL axis2_msg_ctx_find_svc(axis2_msg_ctx_t *msg_ctx, 
                axis2_env_t **env)
{
    return NULL;
}

axis2_operation_t* AXIS2_CALL axis2_msg_ctx_find_operation(axis2_msg_ctx_t *msg_ctx,
                            axis2_env_t **env,
                            axis2_svc_t *svc)
{
    return NULL;
}
