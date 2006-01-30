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

#include <axis2_engine.h>
#include <axis2.h>
#include <axis2_hash.h>
#include <axis2_soap.h>
#include <axis2_transport_sender.h>
#include <axis2_http_transport.h>

/**
 * There is only one engine for the Server and the Client. the send() and receive()
 * Methods are the basic ops the Sync, Async messageing are build on top.
 */


typedef struct axis2_engine_impl
{
    /** context base struct */
    axis2_engine_t engine;
    /** configuration context */
    axis2_conf_ctx_t *conf_ctx;
} axis2_engine_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(engine) ((axis2_engine_impl_t *)engine)

axis2_status_t AXIS2_CALL axis2_engine_send(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
axis2_status_t AXIS2_CALL axis2_engine_receive(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
axis2_status_t AXIS2_CALL axis2_engine_send_fault(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
axis2_status_t AXIS2_CALL axis2_engine_receive_fault(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
axis2_msg_ctx_t* AXIS2_CALL axis2_engine_create_fault_msg_ctx(struct axis2_engine *engine, axis2_env_t **env,
        axis2_msg_ctx_t *processing_context);
axis2_status_t AXIS2_CALL axis2_engine_extract_fault_info_from_msg_ctx(
        struct axis2_engine *engine, axis2_env_t **env,
        axis2_msg_ctx_t *msg_ctx,
        struct axis2_soap_fault *fault);
axis2_status_t AXIS2_CALL axis2_engine_verify_ctx_built(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
axis2_status_t AXIS2_CALL axis2_engine_invoke_phases(struct axis2_engine *engine, axis2_env_t **env, axis2_array_list_t *phases, axis2_msg_ctx_t *msg_ctx);
axis2_status_t AXIS2_CALL axis2_engine_resume_invocation_phases(struct axis2_engine *engine, axis2_env_t **env, axis2_array_list_t *phases, axis2_msg_ctx_t *msg_ctx);
axis2_char_t* AXIS2_CALL axis2_engine_get_sender_fault_code(struct axis2_engine *engine, axis2_env_t **env, axis2_char_t *soap_namespace);
axis2_char_t* AXIS2_CALL axis2_engine_get_receiver_fault_code(struct axis2_engine *engine, axis2_env_t **env, axis2_char_t *soap_namespace);
axis2_status_t AXIS2_CALL axis2_engine_free(struct axis2_engine *engine, 
                                   axis2_env_t **env);
    

axis2_engine_t* AXIS2_CALL axis2_engine_create(axis2_env_t **env, axis2_conf_ctx_t *conf_ctx)
{
    axis2_engine_impl_t *engine_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    engine_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_engine_impl_t) );
    if (!engine_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    engine_impl->engine.ops = NULL;
    engine_impl->conf_ctx = NULL;
    
    if (conf_ctx)
    {
        engine_impl->conf_ctx =  conf_ctx;
    }
    
    /* initialize ops */
    engine_impl->engine.ops  = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_engine_ops_t) );
    if (!engine_impl->engine.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_engine_free(&(engine_impl->engine), env);
        return NULL;        
    }

    engine_impl->engine.ops->send = axis2_engine_send;
    engine_impl->engine.ops->receive = axis2_engine_receive;
    engine_impl->engine.ops->send_fault = axis2_engine_send_fault;
    engine_impl->engine.ops->receive_fault = axis2_engine_receive_fault;
    engine_impl->engine.ops->create_fault_msg_ctx = axis2_engine_create_fault_msg_ctx;
    engine_impl->engine.ops->extract_fault_info_from_msg_ctx = axis2_engine_extract_fault_info_from_msg_ctx;
    engine_impl->engine.ops->verify_ctx_built = axis2_engine_verify_ctx_built;
    engine_impl->engine.ops->invoke_phases = axis2_engine_invoke_phases;
    engine_impl->engine.ops->resume_invocation_phases = axis2_engine_resume_invocation_phases;
    engine_impl->engine.ops->get_sender_fault_code = axis2_engine_get_sender_fault_code;
    engine_impl->engine.ops->get_receiver_fault_code = axis2_engine_get_receiver_fault_code;
    engine_impl->engine.ops->free = axis2_engine_free;

    AXIS2_LOG(env, "Axis2 Engine Started", AXIS2_LOG_LEVEL_INFO);
    
    return &(engine_impl->engine);
}

axis2_status_t AXIS2_CALL axis2_engine_free(struct axis2_engine *engine, 
                                   axis2_env_t **env)
{
    axis2_engine_impl_t *engine_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(engine);
    
    if (engine_impl->engine.ops)
    {
        AXIS2_FREE((*env)->allocator, engine_impl->engine.ops);
        engine_impl->engine.ops = NULL;
    }
    
    AXIS2_FREE((*env)->allocator, engine_impl);
    engine_impl = NULL;
    
    return AXIS2_SUCCESS;
}

/**
 * This methods represents the outflow of the Axis, this could be either at the server side or the client side.
 * Here the <code>ExecutionChain</code> is created using the Phases. The Handlers at the each Phases is ordered in
 * deployment time by the deployment module
 *
 * @param msgContext
 * @throws AxisFault
 * @see MessageContext
 * @see Phase
 * @see Handler
 */
axis2_status_t AXIS2_CALL axis2_engine_send(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_engine_impl_t *engine_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_array_list_t *phases = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(engine);

    status = axis2_engine_verify_ctx_built(engine, env, msg_ctx);
    if (status != AXIS2_SUCCESS)
        return status;

    /* find and invoke the phases */
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);    
    if (op_ctx)
    {
        axis2_op_t *op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
        if (op)
        {
            phases = AXIS2_OP_GET_PHASES_OUTFLOW(op, env);
        }
    }
    
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    /*axis2_array_list_t *global_out_phase = NULL;*/

    if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
    {
        /* the message has paused, so rerun them from the position they stoped. The Handler
        //who paused the Message will be the first one to run
        //resume fixed, global precalulated phases
        */
        status = axis2_engine_resume_invocation_phases(engine, env, phases, msg_ctx);
        if (status != AXIS2_SUCCESS)
        {
            return status;
        }
       
        conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        if (conf_ctx)
        {
            conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                axis2_array_list_t *global_out_phase = AXIS2_CONF_GET_OUT_PHASES(conf, env);
                axis2_engine_invoke_phases(engine, env, global_out_phase, msg_ctx);
            }
        }
    } 
    else 
    {
        axis2_engine_invoke_phases(engine, env, phases, msg_ctx);
        
        conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        if (conf_ctx)
        {
            conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                axis2_array_list_t *global_out_phase = AXIS2_CONF_GET_OUT_PHASES(conf, env);
                axis2_engine_invoke_phases(engine, env, global_out_phase, msg_ctx);
            }
        }
    }
    
    if (!(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)))
    {
        /* write the message to the wire */
        axis2_transport_sender_t *transport_sender = NULL;
        axis2_transport_out_desc_t *transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env);
        
        if (transport_out)
        {
            transport_sender = AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out, env);
            if (transport_sender)
                AXIS2_TRANSPORT_SENDER_INVOKE(transport_sender, env, msg_ctx);
        }
        else
        {
            axis2_char_t message[1024];
            sprintf(message, "%s:%d - Transport out is not set in message context", __FILE__, __LINE__); 
            AXIS2_LOG(env, message, AXIS2_LOG_LEVEL_DEBUG);
            return AXIS2_FAILURE;
        }
    }
    
    return AXIS2_SUCCESS;
}

/**
 * This methods represents the inflow of the Axis, this could be either at the server side or the client side.
 * Here the <code>ExecutionChain</code> is created using the Phases. The Handlers at the each Phases is ordered in
 * deployment time by the deployment module
 *
 * @throws AxisFault
 * @see MessageContext
 * @see Phase
 * @see Handler
 */
axis2_status_t AXIS2_CALL axis2_engine_receive(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_engine_impl_t *engine_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_op_t *op = NULL;
    axis2_array_list_t *pre_calculated_phases = NULL;
    axis2_array_list_t *op_specific_phases = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(engine);
    
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
    
    conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
    
    pre_calculated_phases = AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env);
    
    if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
    {
        /* the message has paused, so re-run them from the position they stoped. The Handler
           who paused the Message will be the first one to run
           resume fixed, global precalulated phases */
        axis2_engine_resume_invocation_phases(engine, env, pre_calculated_phases, msg_ctx);
        if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
        {
            return AXIS2_SUCCESS;
        }
        
        axis2_engine_verify_ctx_built(engine, env, msg_ctx);
        /* resume op specific phases */
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        if (op_ctx)
        {
            op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
            op_specific_phases = AXIS2_OP_GET_REMAINING_PHASES_INFLOW(op, env);
            axis2_engine_resume_invocation_phases(engine, env, op_specific_phases, msg_ctx);
            if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
            {
                return AXIS2_SUCCESS;
            }
        }
    } 
    else 
    {
        axis2_engine_invoke_phases(engine, env, pre_calculated_phases, msg_ctx);
        if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
        {
            return AXIS2_SUCCESS;
        }

        axis2_engine_verify_ctx_built(engine, env, msg_ctx);   /* TODO : Chinthaka remove me. I'm redundant */
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        if (op_ctx)
        {
            op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
            op_specific_phases = AXIS2_OP_GET_REMAINING_PHASES_INFLOW(op, env);
            axis2_engine_invoke_phases(engine, env, op_specific_phases, msg_ctx);
            if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
            {
                return AXIS2_SUCCESS;
            }
        }
    }

    if ( (AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env)) && !(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))) 
    {
        axis2_msg_recv_t *receiver = NULL;
        /* invoke the Message Receivers */
        if (!op)
            return AXIS2_FAILURE;
        receiver = AXIS2_OP_GET_MSG_RECEIVER(op, env);
        if (!receiver)
        {
            axis2_char_t message[1024];
            sprintf(message, "%s:%d - Message receiver not set in operation description", __FILE__, __LINE__); 
            AXIS2_LOG(env, message, AXIS2_LOG_LEVEL_DEBUG);
            return AXIS2_FAILURE;
        }
        AXIS2_MSG_RECV_RECEIVE(receiver, env, msg_ctx);        
    }
    return AXIS2_SUCCESS;
}

/**
 * Sends the SOAP Fault to another SOAP node.
 *
 * @param msg_ctx
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL axis2_engine_send_fault(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_ctx_t *op_ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    
    /* find and execute the Fault Out Flow Handlers */
    if (op_ctx) 
    {
        axis2_op_t *op = AXIS2_OP_CTX_GET_OP(op_ctx, env);        
        axis2_array_list_t *phases = AXIS2_OP_GET_PHASES_OUTFLOW(op, env);
        
        if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
        {
            axis2_engine_resume_invocation_phases(engine, env, phases, msg_ctx);
        } 
        else 
        {
            axis2_engine_invoke_phases(engine, env, phases, msg_ctx);
        }
    }
    /* it is possible that op context is NULL as the error occered before the
    dispatcher. We do not run Handlers in that case */

    if (!(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))) 
    {
        /* send the SOAP Fault*/
        axis2_transport_sender_t *transport_sender = NULL;
        axis2_transport_out_desc_t *transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env);
        if (transport_out)
            transport_sender = AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out, env);
        /*TODO:Uncomment this once the implementation done*/
        if (transport_sender)
            AXIS2_TRANSPORT_SENDER_INVOKE(transport_sender, env, msg_ctx);
    }
    return AXIS2_SUCCESS;
}

/**
 * This is invoked when a SOAP Fault is received from a Other SOAP Node
 * Receives a SOAP fault from another SOAP node.
 *
 * @param msg_ctx
 * @throws AxisFault
 */
axis2_status_t AXIS2_CALL axis2_engine_receive_fault(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_ctx_t *op_ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    
    if (!op_ctx) 
    {
        /* if we do not have an op context that means this may be an incoming
           dual channel response. So try to dispatch the service */
        axis2_conf_ctx_t *conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        if (conf_ctx)
        {
            axis2_conf_t *conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                axis2_array_list_t *phases = AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env);
                if (phases)
                {
                    if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)) 
                    {
                        axis2_engine_resume_invocation_phases(engine, env, phases, msg_ctx);
                    } 
                    else 
                    {
                        axis2_engine_invoke_phases(engine, env, phases, msg_ctx);
                    }
                    axis2_engine_verify_ctx_built(engine, env, msg_ctx);
                }
            }
        }
    }
    
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    /* find and execute the fault in flow handlers */
    if (op_ctx) 
    {
        axis2_op_t *op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
        axis2_array_list_t *phases = AXIS2_OP_GET_PHASES_IN_FAULT_FLOW(op, env);
        if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)) 
        {
            axis2_engine_resume_invocation_phases(engine, env, phases, msg_ctx);
        } 
        else 
        {
            axis2_engine_invoke_phases(engine, env, phases, msg_ctx);
        }
    }
    return AXIS2_SUCCESS;
}

/**
 * This method is called to handle any error that occurs at inflow or outflow. But if the
 * method is called twice, it implies that sending the error handling has failed, in which case
 * the method logs the error and exists.
 *
 * @param processingContext
 * @param e
 * @throws AxisFault
 */
axis2_msg_ctx_t* AXIS2_CALL axis2_engine_create_fault_msg_ctx(struct axis2_engine *engine, axis2_env_t **env,
        axis2_msg_ctx_t *processing_context)
{
    axis2_msg_ctx_t *fault_ctx = NULL;
    axis2_engine_impl_t *engine_impl = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, processing_context, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(engine);
    
    if (AXIS2_MSG_CTX_GET_PROCESS_FAULT(processing_context, env)) 
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_PROCESSING_FAULT_ALREADY, AXIS2_FAILURE);
        return NULL;
    }

    fault_ctx = axis2_msg_ctx_create(env, engine_impl->conf_ctx,
                    AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(processing_context, env),
                    AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(processing_context, env));

    AXIS2_MSG_CTX_SET_PROCESS_FAULT(fault_ctx, env, AXIS2_TRUE);
    
    fault_to = AXIS2_MSG_CTX_GET_FAULT_TO(processing_context, env);
    if (fault_to)
    {
        AXIS2_MSG_CTX_SET_FAULT_TO(fault_ctx, env, fault_to);
    }
    else
    {
        void *writer = AXIS2_MSG_CTX_GET_PROPERTY(processing_context, env, AXIS2_TRANSPORT_OUT, AXIS2_TRUE);
        if (writer) 
        {
            AXIS2_MSG_CTX_SET_PROPERTY(fault_ctx, env, AXIS2_TRANSPORT_OUT, writer, AXIS2_TRUE); 
        } 
        else 
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NOWHERE_TO_SEND_FAULT, AXIS2_FAILURE);
            return NULL;
        }
    }

    AXIS2_MSG_CTX_SET_OP_CTX(fault_ctx, env, AXIS2_MSG_CTX_GET_OP_CTX(processing_context, env));
    AXIS2_MSG_CTX_SET_PROCESS_FAULT(fault_ctx, env, AXIS2_TRUE);
    AXIS2_MSG_CTX_SET_SERVER_SIDE(fault_ctx, env, AXIS2_TRUE);
    AXIS2_MSG_CTX_SET_PROPERTY(fault_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, 
        AXIS2_MSG_CTX_GET_PROPERTY(processing_context, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_TRUE), AXIS2_TRUE );

    
    /*axis2_soap_envelope_t *envelope = NULL;
    if (AXIS2_MSG_CTX_GET_IS_SOAP_11(processing_context, env)) 
    {
        envelope = axis2_create_default_fault_soap_envelope(env, AXIS2_SOAP_11);
        
    } 
    else 
    {
        envelope = axis2_create_default_fault_soap_envilope(env, AXIS2_SOAP_12);
    }

    if (envelope)
    {
        axis2_soap_body_t *body = AXIS2_SOAP_ENVELOPE_GET_BODY(envelope, env);
        if (body)
        {
            axis2_soap_fault_t *fault = AXIS2_SOAP_BODY_GET_FAULT(body, env);
            
        }
        extract_fault_info_from_msg_ctx( engine, env, 
                processing_context,fault);
    }
    else
    {
        return NULL;
    }

    AXIS2_MSG_CTX_SET_ENVELOPE(fault_ctx, env, envelope);
    AXIS2_MSG_CTX_SET_PROPERTY(fault_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, 
        AXIS2_MSG_CTX_GET_PROPERTY(processing_context, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_TRUE) );*/
    return fault_ctx;
}

/**
 * Information to create the SOAPFault can be extracted from different places.
 * 1. Those information may have been put in to the message context by some handler. When someone
 * is putting like that, he must make sure the SOAPElements he is putting must be from the
 * correct SOAP Version.
 * 2. SOAPProcessingException is flexible enough to carry information about the fault. For example
 * it has an attribute to store the fault code. The fault reason can be extracted from the
 * message of the exception. I opted to put the stacktrace under the detail element.
 * eg : <Detail>
 * <Exception> stack trace goes here </Exception>
 * <Detail>
 * <p/>
 * If those information can not be extracted from any of the above places, I default the soap
 * fault values to following.
 * <Fault>
 * <Code>
 * <Value>env:Receiver</Value>
 * </Code>
 * <Reason>
 * <Text>unknown</Text>
 * </Reason>
 * <Role/>
 * <Node/>
 * <Detail/>
 * </Fault>
 * <p/>
 * -- EC
 *
 * @param context
 * @param fault
 * @param e
 */
axis2_status_t AXIS2_CALL axis2_engine_extract_fault_info_from_msg_ctx(
        struct axis2_engine *engine, axis2_env_t **env,
        axis2_msg_ctx_t *msg_ctx,
        struct axis2_soap_fault *fault)
{
    /*axis2_char_t *soap_namespace_uri = NULL;*/
    
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault, AXIS2_FAILURE);
    
    /* get the current SOAP version */
    /*if (AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env))
    {
        soap_namespace_uri = AXIS2_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
    }
    else
    {
        soap_namespace_uri = AXI2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
    }

    void *fault_code = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, AXIS2_SOAP12_SOAP_FAULT_CODE_LOCAL_NAME);
    axis2_char_t *soap_fault_code = "";
    if (fault_code) 
    {
        AXIS2_SOAP_FAULT_SET_CODE(fault, env, fault_code);
    }*/

    /* defaulting to fault code Sender, if no message is available */
    /*soap_fault_code = get_sender_fault_code(soap_namespace_uri);
     fault.getCode().getValue().setText(soap_fault_code); 

    void *fault_Reason = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, AXIS2_SOAP12_SOAP_FAULT_REASON_LOCAL_NAME);
    axis2_char_t * message = "";
    if (fault_Reason) 
    {
        AXIS2_SOAP_FAULT_SET_REASON(fault, env, fault_Reason);
    } 
*/
    /* defaulting to reason, unknown, if no reason is available */
  /*  message = "unknown";
     fault.getReason().getSOAPText().setText(message); 

    void *fault_role = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, AXIS2_SOAP12_SOAP_FAULT_ROLE_LOCAL_NAME);
    if (fault_role) 
    {
        fault.getRole().setText((axis2_char_t *) fault_role); 
    } 
    else 
    {
         get the role of this server and assign it here
        fault.getRole().setText("http://myAxisServer/role/default"); 
    }

    void *fault_node = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, AXIS2_SOAP12_SOAP_FAULT_NODE_LOCAL_NAME);
    if (fault_node) 
    {
        fault.getNode().setText((axis2_char_t *) fault_node);
    }
    else 
    {
         get the node of this server and assign it here
        fault.getNode().setText("http://myAxisServer/role/default"); 
    }

    void *fault_detail = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, AXIS2_SOAP12_SOAP_FAULT_DETAIL_LOCAL_NAME);
    if (fault_detail)
    {
        AXIS2_SOAP_FAULT_SET_DETAIL(fault, env, fault_detail);
    } 
    */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_engine_verify_ctx_built(struct axis2_engine *engine, axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);    
    
    if (!AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env))
    {
        return AXIS2_FAILURE;
    }
    if (!AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env))
    {
        return AXIS2_FAILURE;
    }
    if (!AXIS2_MSG_CTX_GET_SVC_CTX(msg_ctx, env))
    {
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_engine_invoke_phases(struct axis2_engine *engine, axis2_env_t **env, axis2_array_list_t *phases, axis2_msg_ctx_t *msg_ctx)
{
    int i = 0;
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, phases, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);    
    
    int count = AXIS2_ARRAY_LIST_SIZE(phases, env);
    for (i = 0; (i < count && !(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))); i++) 
    {
        axis2_phase_t *phase = (axis2_phase_t *) AXIS2_ARRAY_LIST_GET(phases, env, i);
        AXIS2_PHASE_INVOKE(phase, env, msg_ctx);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL axis2_engine_resume_invocation_phases(struct axis2_engine *engine, axis2_env_t **env, axis2_array_list_t *phases, axis2_msg_ctx_t *msg_ctx)
{
    int i = 0;
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, phases, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);    
    
    AXIS2_MSG_CTX_SET_PAUSED(msg_ctx, env, AXIS2_FALSE);
    
    int count = AXIS2_ARRAY_LIST_SIZE(phases, env);
    axis2_bool_t found_match = AXIS2_FALSE;

    for (i = 0; i < count && !(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)); i++) 
    {
        axis2_phase_t *phase = (axis2_phase_t *) AXIS2_ARRAY_LIST_GET(phases, env, i);
        axis2_char_t* phase_name = AXIS2_PHASE_GET_NAME(phase, env);
        axis2_char_t* paused_phase_name = AXIS2_MSG_CTX_GET_PAUSED_PHASE_NAME(msg_ctx, env);
        if (phase_name && paused_phase_name)
        {
            if (AXIS2_STRCMP(phase_name, paused_phase_name) == 0)
            {
                found_match = AXIS2_TRUE;
                AXIS2_PHASE_INVOKE_START_FROM_HANDLER(phase, env, AXIS2_MSG_CTX_GET_PAUSED_HANDLER_NAME(msg_ctx, env), msg_ctx);
            }
        }
        else 
        {
            if (found_match) 
            {
                AXIS2_PHASE_INVOKE(phase, env, msg_ctx);
            }

       }
    }
    
    return AXIS2_SUCCESS;
}

axis2_char_t* AXIS2_CALL axis2_engine_get_sender_fault_code(struct axis2_engine *engine, axis2_env_t **env, axis2_char_t *soap_namespace) 
{
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_namespace, AXIS2_FAILURE);
    
    /*if (AXIS2_STRCMP(AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, soap_namespace))
        return AXIS2_SOAP12_FAULT_CODE_SENDER;
    else
        return AXIS2_SOAP11_FAULT_CODE_SENDER;
        */
    return NULL;
}

axis2_char_t* AXIS2_CALL axis2_engine_get_receiver_fault_code(struct axis2_engine *engine, axis2_env_t **env, axis2_char_t *soap_namespace) 
{
    AXIS2_FUNC_PARAM_CHECK(engine, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, soap_namespace, AXIS2_FAILURE);
    
    if (AXIS2_STRCMP(AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, soap_namespace))
        return AXIS2_SOAP12_FAULT_CODE_RECEIVER;
    else
        return AXIS2_SOAP11_FAULT_CODE_RECEIVER;
    return NULL;
}
