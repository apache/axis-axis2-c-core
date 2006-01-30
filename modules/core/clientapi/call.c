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

#include <axis2_call.h>
#include <axis2.h>
#include <axis2_transport_receiver.h>
#include <axis2_transport_in_desc.h>
#include <axis2_transport_out_desc.h>
#include <listener_manager.h>
#include <callback_recv.h>
#include <axis2_engine.h>
#include <axis2_soap_body.h>

typedef struct axis2_call_impl
{
    /** configuration context interface struct */
    axis2_call_t call;
    /** base context struct */
    axis2_mep_client_t *base;
    /** referanc to base service context member of base for conveniance */
    axis2_svc_ctx_t *svc_ctx;
    /** timeout in milli seconds */
    long timeout_ms;
    /** transport listener */
    axis2_transport_receiver_t *transport_listener;
    /** transport that should be used for sending messages */
    axis2_transport_out_desc_t *sender_transport;
    /** transport that should be used for reciving messages */
    axis2_transport_in_desc_t *listener_transport;
    /**
     * Should the two SOAPMessage be sent over same channel over seperate channels.
     * The value of this variable depends on the transport specified.
     * e.g. If the transports are different this is true by default.
     * HTTP transport support both cases
     * SMTP transport support only two channel case
     */
    axis2_bool_t use_separate_listener;
    /** address the message should be sent to */
    axis2_endpoint_ref_t *to;
    /** calback receiver for receiving the async messages */
    axis2_callback_recv_t *callback_recv;
    /** listener manager */
    axis2_listener_manager_t *listener_manager;
    /** operation template */
    axis2_op_t *op_template;
    /** last response message context */
    axis2_msg_ctx_t *last_res_msg_ctx;

} axis2_call_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(call) ((axis2_call_impl_t *)call)

axis2_msg_ctx_t* AXIS2_CALL 
axis2_call_invoke_blocking(struct axis2_call *call, 
                                    axis2_env_t **env,
                                    axis2_op_t *op,
                                    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_call_invoke_non_blocking(struct axis2_call *call, 
                                    axis2_env_t **env,
                                    axis2_op_t *op,
                                    axis2_msg_ctx_t *msg_ctx,
                                    axis2_callback_t *callback);

axis2_status_t AXIS2_CALL 
axis2_call_set_to(struct axis2_call *call, 
                    axis2_env_t **env,
                    axis2_endpoint_ref_t *to);

axis2_status_t AXIS2_CALL 
axis2_call_set_transport_info(struct axis2_call *call, 
                            axis2_env_t **env,
                            axis2_char_t *sender_transport,
                             axis2_char_t *listener_transport,
                             axis2_bool_t use_separate_listener);

axis2_status_t AXIS2_CALL 
axis2_call_check_transport(struct axis2_call *call, 
                            axis2_env_t **env,
                            axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_call_close(struct axis2_call *call, 
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_call_set_time(struct axis2_call *call, 
                    axis2_env_t **env,
                    long timeout_ms);

axis2_om_node_t* AXIS2_CALL 
axis2_call_invoke_blocking_with_om(struct axis2_call *call, 
                                    axis2_env_t **env,
                                    axis2_char_t *op_name, 
                                    axis2_om_node_t *om_node_to_send);

axis2_soap_envelope_t* AXIS2_CALL 
axis2_call_invoke_blocking_with_soap(struct axis2_call *call, 
                                        axis2_env_t **env,
                                        axis2_char_t *op_name, 
                                        axis2_soap_envelope_t *envelope);

axis2_status_t AXIS2_CALL 
axis2_call_invoke_non_blocking_with_om(struct axis2_call *call, 
                                        axis2_env_t **env,
                                        axis2_char_t *op_name,
                                        axis2_om_node_t *om_node_to_send,
                                        axis2_callback_t *callback);

axis2_status_t AXIS2_CALL 
axis2_call_invoke_non_blocking_with_soap(struct axis2_call *call, 
                                            axis2_env_t **env,
                                            axis2_char_t *op_name,
                                            axis2_soap_envelope_t *envelope,
                                            axis2_callback_t * callback);

axis2_op_t* AXIS2_CALL 
axis2_call_create_op_fill_flow(struct axis2_call *call, 
                                axis2_env_t **env,
                                axis2_char_t *op_name);

axis2_msg_ctx_t* AXIS2_CALL 
axis2_call_get_last_res_msg_ctx(struct axis2_call *call, 
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_call_set(axis2_call_t *call,
                axis2_env_t **env,
                axis2_char_t *key,
                void *value);

axis2_status_t AXIS2_CALL axis2_call_free(struct axis2_call *call, 
                                   axis2_env_t **env);

axis2_call_t* AXIS2_CALL axis2_call_create(axis2_env_t **env, axis2_svc_ctx_t *svc_ctx) 
{
    axis2_call_impl_t *call_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    /*AXIS2_PARAM_CHECK((*env)->error, svc_ctx, NULL);*/
    
    call_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_call_impl_t) );
    if (!call_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    call_impl->call.ops = NULL;
    call_impl->base = NULL;
    call_impl->timeout_ms = 2000;
    call_impl->transport_listener = NULL;
    call_impl->sender_transport = NULL;
    call_impl->listener_transport = NULL;
    call_impl->use_separate_listener = AXIS2_FALSE;
    call_impl->to = NULL;
    call_impl->callback_recv = NULL;
    call_impl->listener_manager = NULL;
    call_impl->op_template = NULL;
    call_impl->last_res_msg_ctx = NULL;
    call_impl->svc_ctx = NULL;
    
    if(svc_ctx)
    {
        call_impl->svc_ctx = svc_ctx;
    }
    
    call_impl->base = axis2_mep_client_create(env, svc_ctx, AXIS2_MEP_URI_OUT_IN);
    if (!(call_impl->base))
    {
        axis2_call_free(&(call_impl->call), env);
        return NULL;
    }
    
    call_impl->callback_recv = axis2_callback_recv_create(env);
    if (!(call_impl->callback_recv))
    {
        axis2_call_free(&(call_impl->call), env);
        return NULL;
    }
    
    /* initialize ops */    
    call_impl->call.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_call_ops_t) );
    if (!call_impl->call.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_call_free(&(call_impl->call), env);
        return NULL;        
    }
    call_impl->call.ops->set_to = axis2_call_set_to;
    call_impl->call.ops->set_transport_info = axis2_call_set_transport_info;
    call_impl->call.ops->check_transport = axis2_call_check_transport;
    call_impl->call.ops->close = axis2_call_close;
    call_impl->call.ops->set_time = axis2_call_set_time;
    call_impl->call.ops->invoke_blocking_with_om = axis2_call_invoke_blocking_with_om;
    call_impl->call.ops->invoke_blocking_with_soap = axis2_call_invoke_blocking_with_soap;
    call_impl->call.ops->invoke_non_blocking_with_om = axis2_call_invoke_non_blocking_with_om;
    call_impl->call.ops->invoke_non_blocking_with_soap = axis2_call_invoke_non_blocking_with_soap;
    call_impl->call.ops->create_op_fill_flow = axis2_call_create_op_fill_flow;
    call_impl->call.ops->get_last_res_msg_ctx = axis2_call_get_last_res_msg_ctx;    
    call_impl->call.ops->set = axis2_call_set;
    call_impl->call.ops->free = axis2_call_free;

    return &(call_impl->call);
}

axis2_status_t AXIS2_CALL axis2_call_free(struct axis2_call *call, 
                                   axis2_env_t **env)
{
    axis2_call_impl_t *call_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    if (call_impl->call.ops)
    {
        AXIS2_FREE((*env)->allocator, call_impl->call.ops);
        call_impl->call.ops = NULL;
    }
    
    if (call_impl->base)
    {
        AXIS2_MEP_CLIENT_FREE(call_impl->base, env);
        call_impl->base = NULL;
    }    
    
    if (call_impl->callback_recv)
    {
        AXIS2_CALLBACK_RECV_FREE(call_impl->callback_recv, env);
        call_impl->callback_recv = NULL;
    }    
    
    AXIS2_FREE((*env)->allocator, call_impl);
    call_impl = NULL;
    
    return AXIS2_SUCCESS;
}



/**
 * This accepts a ServiceContext, and the axis2_svc_ctx_t *should have all the parents set in to it right
 * Ideall this should be generated from a WSDL, we do not have it yet.
 * <p/>
 * Follwoing code works for the time been
 * <code>
 * ConfigurationContextFactory efac = new ConfigurationContextFactory();
 * ConfigurationContext sysContext = efac.buildClientConfigurationContext(null);
 * // above line "null" may be a file name if you know the client repssitory
 * <p/>
 * //create new service
 * QName assumedServiceName = new QName("Your Service");
 * AxisService axisService = new AxisService(assumedServiceName);
 * sysContext.getEngineConfig().addService(axisService);
 * axis2_svc_ctx_t *service = sysContext.createServiceContext(assumedServiceName);
 * return service;
 * <p/>
 * </code>
 *
 * @param svc_ctx
 */



/**
 * This invocation done via this method blocks till the result arrives, 
 * using this method does not indicate
 * anyhting about the transport used or the nature of the transport.
 * e.g. invocation done with this method might
 * <ol>
 * <li>send request via http and recevie the response via the return path of the same http connection</li>
 * <li>send request via http and recevie the response different http connection</li>
 * <li>send request via an email smtp and recevie the response via an email</li>
 * </ol>
 */

axis2_msg_ctx_t* AXIS2_CALL axis2_call_invoke_blocking(struct axis2_call *call, 
                                    axis2_env_t **env,
                                    axis2_op_t *op,
                                    axis2_msg_ctx_t *msg_ctx)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_svc_t *svc = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    svc = AXIS2_SVC_CTX_GET_SVC(call_impl->svc_ctx, env);
    
    status = AXIS2_MEP_CLIENT_PREPARE_INVOCATION(call_impl->base, env, op, msg_ctx);
    if (status != AXIS2_SUCCESS)
        return NULL;
    
    svc_ctx = AXIS2_MEP_CLIENT_GET_SVC_CTX(call_impl->base, env);
    if (!svc_ctx)
        return NULL;
    
    /* The message ID is sent all the time */
    axis2_char_t *message_id = "uuid:"; /* TODO UUIDGenerator.getUUID()*/
    AXIS2_MSG_CTX_SET_MESSAGE_ID(msg_ctx, env, message_id);

    if (call_impl->use_separate_listener) 
    {
        axis2_callback_t *callback = NULL;
        axis2_status_t status = AXIS2_FAILURE;
        
        long index = 0;
        /* This means doing a Request-Response invocation using two channels. 
        If the transport is a two way transport (e.g. http), only one channel is used
        (e.g. in http cases 202 OK is sent to say no repsone avalible). 
        Axis2 gets blocked and return when the response is avalible.
        */
        
        callback = axis2_callback_create(env);
        if (!callback)
            return NULL;
        
        /* call two channel non blocking invoke to do the work and wait on the callbck */
        status = axis2_call_invoke_non_blocking(call, env, op, msg_ctx, callback);
        if (status != AXIS2_SUCCESS)
            return NULL;
        
        index = call_impl->timeout_ms / 1000;
        while (!(AXIS2_CALLBACK_GET_COMPLETE(callback, env))) 
        {
            /*wait till the reponse arrives*/
            if (index-- >= 0) 
            {
                /* TODO AXIS2_THREAD_SLEEP(env, 1); */
            } 
            else 
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_RESPONSE_TIMED_OUT, AXIS2_FAILURE);
                return NULL;
            }
        }
        /* process the result of the invocation */
        if (AXIS2_CALLBACK_GET_ENVELOPE(callback, env))
        {
            axis2_msg_ctx_t *response_msg_ctx =
                    axis2_msg_ctx_create(env, AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env), NULL, NULL);
            if (!response_msg_ctx)
                return NULL;
            AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(response_msg_ctx, env, AXIS2_CALLBACK_GET_ENVELOPE(callback, env));
            return response_msg_ctx;
        } 
        else 
        {
            if (AXIS2_CALLBACK_GET_ERROR(callback, env) != AXIS2_ERROR_NONE)
            {
                AXIS2_ERROR_SET((*env)->error, AXIS2_CALLBACK_GET_ERROR(callback, env), AXIS2_FAILURE);
                return NULL;
            }
        }
    } 
    else 
    {        
        axis2_op_ctx_t *op_ctx = NULL;
        axis2_msg_ctx_t *response = NULL;
        axis2_soap_envelope_t *response_envelope = NULL;        
        
        /* Usual Request-Response Sync implemetation */
        AXIS2_MSG_CTX_SET_TO(msg_ctx, env, call_impl->to);
        AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx);
        AXIS2_MSG_CTX_SET_CONF_CTX(msg_ctx, env, AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env));

        status = axis2_call_check_transport(call, env, msg_ctx);
        if (status != AXIS2_SUCCESS)
            return NULL;
        
        op_ctx = axis2_op_ctx_create(env, op, svc_ctx);
        AXIS2_OP_REGISTER_OP_CTX(op, env, msg_ctx, op_ctx);

        /*Send the SOAP Message and receive a response */
        response = axis2_two_way_send(env, msg_ctx);
        if (!response)
            return NULL;
        
        /*check for a fault and return the result */
        response_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(response, env);
        if (response_envelope)
        {
            axis2_soap_body_t *soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(response_envelope, env);
            if (soap_body)
            {
                if (AXIS2_SOAP_BODY_HAS_FAULT(soap_body, env))
                {
                    axis2_soap_fault_t *soap_fault = AXIS2_SOAP_BODY_GET_FAULT(soap_body, env);
                    AXIS2_SOAP_FAULT_GET_EXCEPTION(soap_fault, env);
                    /* TODO set the fault in env error */
                }
            }            
        }
        
        return response;
    }
    
    return NULL;
}

/**
 * This invocation done via this method blocks till the result arrives, using this method does not indicate
 * anyhting about the transport used or the nature of the transport.
 */
axis2_status_t AXIS2_CALL axis2_call_invoke_non_blocking(struct axis2_call *call, 
                                    axis2_env_t **env,
                                    axis2_op_t *op,
                                    axis2_msg_ctx_t *msg_ctx,
                                    axis2_callback_t *callback)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_engine_t *engine = NULL;
    axis2_char_t *message_id =NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    status = AXIS2_MEP_CLIENT_PREPARE_INVOCATION(call_impl->base, env, op, msg_ctx);
    if (status != AXIS2_SUCCESS)
        return AXIS2_FAILURE;
    
    svc_ctx = AXIS2_MEP_CLIENT_GET_SVC_CTX(call_impl->base, env);
    if (!svc_ctx)
        return AXIS2_FAILURE;
    
    AXIS2_MSG_CTX_SET_TO(msg_ctx, env, call_impl->to);
    
    conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env);
    if (!conf_ctx)
        return AXIS2_FAILURE;

    engine = axis2_engine_create(env, conf_ctx);
    
    status = axis2_call_check_transport(call, env, msg_ctx);
    if (status != AXIS2_SUCCESS)
            return status;
    
    /* Use message id all the time! */
    message_id = "uuid:"; /* TODO UUIDGenerator.getUUID() */
    AXIS2_MSG_CTX_SET_MESSAGE_ID(msg_ctx, env, message_id);
    
    if (call_impl->use_separate_listener) 
    {
        axis2_relates_to_t *relates_to = NULL;
        axis2_svc_t *svc = NULL;
        /*
        the invocation happen via a seperate channel, so we should set up the
        information needed to correlate the response message and invoke the call back
        */

        AXIS2_OP_SET_MSG_RECEIVER(op, env, AXIS2_CALLBACK_RECV_GET_BASE(call_impl->callback_recv, env));
        AXIS2_CALLBACK_RECV_ADD_CALLBACK(call_impl->callback_recv,env, message_id, callback);
        
        /* set the relates_to such that the response will arrive at the transport_listener started */
        svc = AXIS2_SVC_CTX_GET_SVC(svc_ctx, env);
        if (svc)
        {
            axis2_char_t *svc_name = NULL, *temp = NULL;
            axis2_qname_t *svc_qname = AXIS2_SVC_GET_QNAME(svc, env);
            axis2_qname_t *op_qname = AXIS2_OP_GET_QNAME(svc, env);
            axis2_qname_t *transport_qname = AXIS2_TRANSPORT_IN_DESC_GET_QNAME(call_impl->listener_transport, env);
            axis2_endpoint_ref_t *epr = NULL;
            
            temp = AXIS2_STRACAT(AXIS2_QNAME_GET_LOCALPART(svc_qname, env), "/", env);
            svc_name = AXIS2_STRACAT(temp, AXIS2_QNAME_GET_LOCALPART(op_qname, env), env);
            if (temp)
                AXIS2_FREE((*env)->allocator, temp);
            
            epr = AXIS2_LISTNER_MANAGER_REPLY_TO_EPR(call_impl->listener_manager, env, svc_name,
                        AXIS2_QNAME_GET_LOCALPART(transport_qname, env));
            if (epr)
            {
                AXIS2_MSG_CTX_SET_REPLY_TO(msg_ctx, env, epr);
            }
            
            if (svc_name)
                AXIS2_FREE((*env)->allocator, svc_name);
            AXIS2_MSG_CTX_SET_RELATES_TO(msg_ctx, env, relates_to);
            
        }
        
        AXIS2_MSG_CTX_SET_TO(msg_ctx, env, call_impl->to);
        /* create and set the Operation context */
        AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, AXIS2_OP_FIND_OP_CTX(op, env, msg_ctx, svc_ctx));
        AXIS2_MSG_CTX_SET_SVC_CTX(msg_ctx, env, svc_ctx);

        /* send the message */
        AXIS2_ENGINE_SEND(engine, env, msg_ctx);
    } 
    else 
    {
        /* here a bloking invocation happens in a new thread, so the progamming model is non blocking */
         /* TODO svc_ctx.getConfigurationContext().getThreadPool().execute(new NonBlockingInvocationWorker(callback, op, msg_ctx)); */
    }

    return AXIS2_SUCCESS;
}

/**
 * @param to
 */
axis2_status_t AXIS2_CALL axis2_call_set_to(struct axis2_call *call, 
    axis2_env_t **env,
    axis2_endpoint_ref_t *to) 
{
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(call)->to = to;
    return AXIS2_SUCCESS;
}

/**
 * Set transport information to the the Call, for find how the each parameter acts see the commant at the instance
 * variables. The senarios supoorted are as follows.
 * [sender_transport, listener_transport, use_separate_listener]
 * http, http, true
 * http, http, false
 * http,smtp,true
 * smtp,http,true
 * smtp,smtp,true
 *
 * @param sender_transport
 * @param listener_transport
 * @param use_separate_listener
 * @throws AxisFault
 */

axis2_status_t AXIS2_CALL axis2_call_set_transport_info(struct axis2_call *call, 
                            axis2_env_t **env,
                            axis2_char_t *sender_transport,
                             axis2_char_t *listener_transport,
                             axis2_bool_t use_separate_listener)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_qname_t *qname = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    /*
    here we check for a legal combination, for and example if the sendertransport is http and listener
    transport is smtp the invocation must using seperate transport 
    */
    
    if (!use_separate_listener) 
    {
        /*
        TODO
        Check if sender and listener transports are the same */
        
    } 
    else 
    {
        call_impl->use_separate_listener = use_separate_listener;

    }

    /* find and set the transport details */
    conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(call_impl->svc_ctx, env);
    if (conf_ctx)
    {
        conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
        if (conf)
        {
            qname = axis2_qname_create(env, listener_transport, NULL, NULL);
            if (!qname)
                return AXIS2_FAILURE;
            call_impl->listener_transport = AXIS2_CONF_GET_TRANSPORT_IN(conf, env, qname);
            
            AXIS2_QNAME_FREE(qname, env);
            qname = axis2_qname_create(env, sender_transport, NULL, NULL);
            if (!qname)
                return AXIS2_FAILURE;
            call_impl->sender_transport = AXIS2_CONF_GET_TRANSPORT_OUT(conf, env, qname);
            AXIS2_QNAME_FREE(qname, env);
        }
    }
    
    if (!(call_impl->sender_transport)) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_UNKNOWN_TRANSPORT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    if (!(call_impl->listener_transport)) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_UNKNOWN_TRANSPORT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    /* if seperate transport is used, start the required listeners */
    if (use_separate_listener) 
    {
        axis2_qname_t *qname = axis2_qname_create(env, AXIS2_MODULE_ADDRESSING, NULL, NULL);
        
        if (!conf || !(AXIS2_CONF_IS_ENGAGED(conf, env, qname)))
        {
            AXIS2_QNAME_FREE(qname, env);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_TWO_WAY_CHANNEL_NEEDS_ADDRESSING, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        AXIS2_QNAME_FREE(qname, env);
        
        AXIS2_LISTNER_MANAGER_MAKE_SURE_STARTED(call_impl->listener_manager, env, listener_transport, conf_ctx);
    }
    return AXIS2_SUCCESS;
}

/**
 * Check has the transports are identified correctly
 *
 * @param msg_ctx
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL axis2_call_check_transport(struct axis2_call *call, 
    axis2_env_t **env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_call_impl_t *call_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    if (!(call_impl->sender_transport)) 
    {
        call_impl->sender_transport = AXIS2_MEP_CLIENT_INFER_TRANSPORT(call_impl->base, env, call_impl->to);
    }
    
    if (!(call_impl->listener_transport))
    {
        axis2_conf_ctx_t *conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(call_impl->svc_ctx, env);
        if (conf_ctx)
        {
            axis2_conf_t *conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                call_impl->listener_transport = AXIS2_CONF_GET_TRANSPORT_IN(conf, env, 
                        AXIS2_TRANSPORT_OUT_DESC_GET_QNAME(call_impl->sender_transport, env));
            }
        }
    }

    if (!(AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(msg_ctx, env)))
    {
        AXIS2_MSG_CTX_SET_TRANSPORT_IN_DESC(msg_ctx, env, call_impl->listener_transport);
    }
    
    if (!(AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env)))
    {
        AXIS2_MSG_CTX_SET_TRANSPORT_OUT_DESC(msg_ctx, env, call_impl->sender_transport);
    }
    return AXIS2_SUCCESS;
}



/**
 * Closing the Call, this will stop the started Transport Listeners. If there are multiple
 * request to send the Call should be kept open closing only when done
 */
axis2_status_t AXIS2_CALL axis2_call_close(struct axis2_call *call, 
    axis2_env_t **env) 
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_char_t *transport_name = NULL;
    axis2_qname_t *transport_qname = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    transport_qname = AXIS2_TRANSPORT_IN_DESC_GET_QNAME(call_impl->listener_transport, env);
    if (transport_qname)
        transport_name = AXIS2_QNAME_GET_LOCALPART(transport_qname, env);
    
    if (!transport_name)
        return AXIS2_FAILURE;
    
    return AXIS2_LISTNER_MANAGER_STOP(call_impl->listener_manager, env, transport_name);
}

/**
 * This Class is the workhorse for a Non Blocking invocation that uses a
 * two way transport
 */
/*typedef struct NonBlockingInvocationWorker implements Runnable {

    private Callback callback;
    private axis2_op_t *op;
    private axis2_msg_ctx_t *msg_ctx;

    NonBlockingInvocationWorker(Callback callback,
                                       axis2_op_t *op,
                                       axis2_msg_ctx_t *msg_ctx) {
        this.callback = callback;
        this.op = op;
        this.msg_ctx = msg_ctx;
    }

    axis2_status_t run() {
        try {
            axis2_op_ctx_t *opcontxt = new OperationContext(op,svc_ctx);
            AXIS2_MSG_CTX_SET_(msg_ctx, env,   tOperationContext(opcontxt);
            AXIS2_MSG_CTX_SET_(msg_ctx, env,   tServiceContext(svc_ctx);
            //send the request and wait for reponse
            axis2_msg_ctx_t *response =
                    TwoWayTransportBasedSender.send(msg_ctx, listener_transport);
            //call the callback                        
            axis2_soap_envelope_t *response_envelope = response.getEnvelope();
            SOAPBody body = response_envelope.getBody();
            if (body.hasFault()){
                Exception ex = body.getFault().getException();
                if (ex !=null){
                    callback.reportError(ex);
                }else{
                    //todo this needs to be fixed
                    callback.reportError(new Exception(body.getFault().getReason().getText()));
                }
            }else{
                AsyncResult asyncResult = new AsyncResult(response);
                callback.onComplete(asyncResult);
            }

            callback.setComplete(true);
        } catch (Exception e) {
            callback.reportError(e);
        }
    }
}*/

/**
 * This will be used in invoke blocking scenario. Client will wait the amount of time specified here
 * and if there is no response, call will timeout. This should be given in multiples of 100 and defaults to 2000.
 * @param timeout_ms
 */
axis2_status_t AXIS2_CALL axis2_call_set_time(struct axis2_call *call, 
    axis2_env_t **env,
    long timeout_ms) 
{
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(call)->timeout_ms = timeout_ms;
    return AXIS2_SUCCESS;
}

/**
 * Invoke the blocking/Synchronous call
 *
 * @param op - this will be used to identify the operation in the client side, without dispatching
 * @param toSend - This should be OM Element (payload)
 * @return
 * @throws AxisFault
 */
axis2_om_node_t* AXIS2_CALL axis2_call_invoke_blocking_with_om(struct axis2_call *call, 
    axis2_env_t **env,
    axis2_char_t *op_name, axis2_om_node_t *om_node_to_send)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_qname_t *op_qname = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);

    op_qname = axis2_qname_create(env, op_name, NULL, NULL);
    if (op_qname)
    {
        axis2_msg_ctx_t *msg_ctx = NULL;
                
        if (call_impl->svc_ctx)
            svc = AXIS2_SVC_CTX_GET_SVC(call_impl->svc_ctx, env);
        if (svc)
        {
            op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname);            
        }
        
        /* now free qname as it is no more required */
        AXIS2_QNAME_FREE(op_qname, env);
        
        if (!op)
        {
            op = axis2_call_create_op_fill_flow(call, env, op_name);
        }
        
        if (!op || !svc)
            return NULL;
        
        /* when we come here op and svc are not NULL */
        AXIS2_OP_SET_PARENT(op, env, svc);
        
        msg_ctx = AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(call_impl->base, env, om_node_to_send);
        if (msg_ctx)
        {
            call_impl->last_res_msg_ctx = axis2_call_invoke_blocking(call, env, op, msg_ctx);
            if (call_impl->last_res_msg_ctx)
            {
                axis2_soap_body_t *soap_body = NULL;
                axis2_soap_envelope_t *res_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(call_impl->last_res_msg_ctx, env);
                soap_body = AXIS2_SOAP_ENVELOPE_GET_BODY(res_envelope, env);
                if (soap_body)
                {
                    axis2_om_node_t *soap_node = NULL;
                    soap_node = AXIS2_SOAP_BODY_GET_BASE_NODE(soap_body, env);
                    if (soap_node)
                    {
                        return AXIS2_OM_NODE_GET_FIRST_CHILD(soap_node, env);
                    }
                }
            }
        }
        
    }
    
    return NULL;
}

/**
 * Invoke the blocking/Synchronous call
 *
 * @param op_name - this will be used to identify the operation in the client side, without dispatching
 * @param envelope - This should be SOAPEnvelope
 * @return
 * @throws AxisFault
 */
axis2_soap_envelope_t* AXIS2_CALL axis2_call_invoke_blocking_with_soap(struct axis2_call *call, 
    axis2_env_t **env,
    axis2_char_t *op_name, axis2_soap_envelope_t *envelope)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_qname_t *op_qname = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);

    op_qname = axis2_qname_create(env, op_name, NULL, NULL);
    if (op_qname)
    {
        if (call_impl->svc_ctx)
            svc = AXIS2_SVC_CTX_GET_SVC(call_impl->svc_ctx, env);
        if (svc)
        {
            op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname);            
        }
        
        /* now free qname as it is no more required */
        AXIS2_QNAME_FREE(op_qname, env);
        
        if (!op)
        {
            op = axis2_call_create_op_fill_flow(call, env, op_name);
        }
        
        if (!op)
            return NULL;
        
        if (!envelope || !(AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env)))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL, AXIS2_FAILURE);
            return NULL;
        }
        
        conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(call_impl->svc_ctx, env);
        
        msg_ctx = axis2_msg_ctx_create(env, conf_ctx, NULL, NULL);
        if (msg_ctx)
        {
            AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, envelope);
            call_impl->last_res_msg_ctx = axis2_call_invoke_blocking(call, env, op, msg_ctx);
            return AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(call_impl->last_res_msg_ctx, env);
        }

    }
    
    return NULL;
}

/**
 * Invoke the nonblocking/Asynchronous call
 *
 * @param op_name
 * @param om_node_to_send   -  This should be OM Element (payload)
 *                 invocation behaves accordingly
 * @param callback
 * @throws org.apache.axis2.AxisFault
 */

axis2_status_t AXIS2_CALL axis2_call_invoke_non_blocking_with_om(struct axis2_call *call, 
    axis2_env_t **env,
    axis2_char_t *op_name,
    axis2_om_node_t *om_node_to_send,
    axis2_callback_t *callback)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_qname_t *op_qname = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);

        op_qname = axis2_qname_create(env, op_name, NULL, NULL);
    if (op_qname)
    {
        if (call_impl->svc_ctx)
            svc = AXIS2_SVC_CTX_GET_SVC(call_impl->svc_ctx, env);
        if (svc)
        {
            op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname);            
        }
        
        /* now free qname as it is no more required */
        AXIS2_QNAME_FREE(op_qname, env);
        
        if (!op)
        {
            op = axis2_call_create_op_fill_flow(call, env, op_name);
        }
        
        if (!op)
            return AXIS2_FAILURE;

        msg_ctx = AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(call_impl->base, env, om_node_to_send);
    
        /*call the underline implementation*/
        return axis2_call_invoke_non_blocking(call, env, op, msg_ctx, callback);
    }
    
    return AXIS2_FAILURE;
}
/**
 * Invoke the nonblocking/Asynchronous call
 *
 * @param op_name
 * @param envelope   -  This should be a SOAP Envelope
 *                 invocation behaves accordingly
 * @param callback
 * @throws org.apache.axis2.AxisFault
 */

axis2_status_t AXIS2_CALL axis2_call_invoke_non_blocking_with_soap(struct axis2_call *call, 
    axis2_env_t **env,
    axis2_char_t *op_name,
    axis2_soap_envelope_t *envelope,
    axis2_callback_t * callback)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_qname_t *op_qname = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    op_qname = axis2_qname_create(env, op_name, NULL, NULL);
    if (op_qname)
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        if (call_impl->svc_ctx)
            svc = AXIS2_SVC_CTX_GET_SVC(call_impl->svc_ctx, env);
        if (svc)
        {
            op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname);            
        }
        
        /* now free qname as it is no more required */
        AXIS2_QNAME_FREE(op_qname, env);
        
        if (!op)
        {
            op = axis2_call_create_op_fill_flow(call, env, op_name);
        }
        
        if (!op)
            return AXIS2_FAILURE;
        
        if (!envelope || !(AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env)))
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
        
        conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(call_impl->svc_ctx, env);
        
        msg_ctx = axis2_msg_ctx_create(env, conf_ctx, NULL, NULL);
        if (msg_ctx)
        {
            AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, envelope);
            return axis2_call_invoke_non_blocking(call, env, op, msg_ctx, callback);
        }

    }

    return AXIS2_FAILURE;
}

/**
 * This method create a operation desc if it null and copy the flows from the template operation
 * @param op
 * @param axisOp
 */
axis2_op_t* AXIS2_CALL axis2_call_create_op_fill_flow(struct axis2_call *call, 
    axis2_env_t **env,
    axis2_char_t *op_name)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_qname_t *op_qname = NULL;
    axis2_op_t *op = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    /* if the operation is not alrady define we will copy the
    crated Phases from the templete operation to the this Operation */

    op_qname = axis2_qname_create(env, op_name, NULL, NULL);
    if (op_qname)
    {
        axis2_svc_t *svc = NULL;
        
        op = axis2_op_create_with_qname(env, op_qname);
        if(!op)
        {
            return AXIS2_FAILURE;   
        }
        AXIS2_OP_SET_REMAINING_PHASES_INFLOW(op, env, 
            AXIS2_OP_GET_REMAINING_PHASES_INFLOW(call_impl->op_template, env));
        AXIS2_OP_SET_PHASES_OUTFLOW(op, env, 
            AXIS2_OP_GET_PHASES_OUTFLOW(call_impl->op_template, env));
        AXIS2_OP_SET_PHASES_IN_FAULT_FLOW(op, env, 
            AXIS2_OP_GET_PHASES_IN_FAULT_FLOW(call_impl->op_template, env));
        AXIS2_OP_SET_PHASES_OUT_FAULT_FLOW(op, env, 
            AXIS2_OP_GET_PHASES_OUT_FAULT_FLOW(call_impl->op_template, env));
        if (call_impl->svc_ctx)
        {
            svc = AXIS2_SVC_CTX_GET_SVC(call_impl->svc_ctx, env);
            if (svc)
            {
                AXIS2_SVC_ADD_OP(svc, env, op);
            }
        }
    }
        
    return op;
}

axis2_status_t AXIS2_CALL
axis2_call_set(axis2_call_t *call,
                axis2_env_t **env,
                axis2_char_t *key,
                void *value)
{
    axis2_call_impl_t *call_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_ctx_t *conf_ctx_base = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, key, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, value, AXIS2_FAILURE);
    call_impl = AXIS2_INTF_TO_IMPL(call);
    
    conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(call_impl->svc_ctx, env);
    if(!conf_ctx)
    {
        return AXIS2_FAILURE;
    }
    conf_ctx_base = AXIS2_CONF_CTX_GET_BASE(conf_ctx, env);
    if(!conf_ctx_base)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_CTX_SET_PROPERTY(conf_ctx_base, env, key, value, AXIS2_FALSE);
    return status;
}

/**
 * @return
 */
axis2_msg_ctx_t* AXIS2_CALL axis2_call_get_last_res_msg_ctx(struct axis2_call *call, 
    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(call, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(call)->last_res_msg_ctx;
}
