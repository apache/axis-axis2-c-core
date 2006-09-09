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
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axiom_soap_const.h>
#include <axiom_soap_envelope.h>
#include <axiom_soap_body.h>
#include <axiom_soap_fault.h>
#include <axiom_soap_header.h>
#include <axiom_soap_header_block.h>
#include <axis2_transport_sender.h>
#include <axis2_http_transport.h>
#include <axis2_addr.h>
#include <axis2_uuid_gen.h>


typedef struct axis2_engine_impl
{
    /** context base struct */
    axis2_engine_t engine;
    /** configuration context */
    axis2_conf_ctx_t *conf_ctx;
}
axis2_engine_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(engine) ((axis2_engine_impl_t *)engine)

axis2_status_t AXIS2_CALL 
axis2_engine_send(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_engine_receive(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_engine_send_fault(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_engine_receive_fault(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx);

axis2_msg_ctx_t *AXIS2_CALL 
axis2_engine_create_fault_msg_ctx(
    axis2_engine_t *engine, 
    const axis2_env_t *env,
    axis2_msg_ctx_t *processing_context);

axis2_status_t AXIS2_CALL 
axis2_engine_extract_fault_info_from_msg_ctx(
    axis2_engine_t *engine, 
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    struct axiom_soap_fault *fault);

axis2_status_t AXIS2_CALL 
axis2_engine_verify_ctx_built(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_engine_invoke_phases(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_array_list_t *phases, 
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_engine_resume_invocation_phases(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_array_list_t *phases, 
    axis2_msg_ctx_t *msg_ctx);

const axis2_char_t *AXIS2_CALL 
axis2_engine_get_sender_fault_code(
    const axis2_engine_t *engine, 
    const axis2_env_t *env, 
    const axis2_char_t *soap_namespace);

const axis2_char_t *AXIS2_CALL 
axis2_engine_get_receiver_fault_code(
    const axis2_engine_t *engine, 
    const axis2_env_t *env, 
    const axis2_char_t *soap_namespace);

axis2_status_t AXIS2_CALL 
axis2_engine_free(
    axis2_engine_t *engine, 
    const axis2_env_t *env);

axis2_status_t 
axis2_engine_check_must_understand_headers(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_engine_resume_receive(
    axis2_engine_t *engine,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_engine_resume_send(
    axis2_engine_t *engine,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

AXIS2_EXTERN axis2_engine_t *AXIS2_CALL
axis2_engine_create(
    const axis2_env_t *env, 
    axis2_conf_ctx_t *conf_ctx)
{
    axis2_engine_impl_t *engine_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    engine_impl = AXIS2_MALLOC( env->allocator, sizeof(axis2_engine_impl_t) );
    if (!engine_impl)
    { 
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

    engine_impl->engine.ops = NULL;
    engine_impl->conf_ctx = NULL;
    
    if (conf_ctx)
    {
        engine_impl->conf_ctx =  conf_ctx;
    }
    
    /* initialize ops */
    engine_impl->engine.ops  = AXIS2_MALLOC( env->allocator, sizeof(axis2_engine_ops_t) );
    if (!engine_impl->engine.ops)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_engine_free(&(engine_impl->engine), env);
        return NULL;        
    }

    engine_impl->engine.ops->send = 
        axis2_engine_send;
    
    engine_impl->engine.ops->receive = 
        axis2_engine_receive;
        
    engine_impl->engine.ops->send_fault = 
        axis2_engine_send_fault;
        
    engine_impl->engine.ops->receive_fault = 
        axis2_engine_receive_fault;
        
    engine_impl->engine.ops->create_fault_msg_ctx = 
        axis2_engine_create_fault_msg_ctx;
        
    engine_impl->engine.ops->extract_fault_info_from_msg_ctx = 
        axis2_engine_extract_fault_info_from_msg_ctx;
        
    engine_impl->engine.ops->verify_ctx_built = 
        axis2_engine_verify_ctx_built;
        
    engine_impl->engine.ops->invoke_phases = 
        axis2_engine_invoke_phases;
        
    engine_impl->engine.ops->resume_invocation_phases = 
        axis2_engine_resume_invocation_phases;
        
    engine_impl->engine.ops->get_sender_fault_code = 
        axis2_engine_get_sender_fault_code;
        
    engine_impl->engine.ops->get_receiver_fault_code = 
        axis2_engine_get_receiver_fault_code;
        
    engine_impl->engine.ops->free = 
        axis2_engine_free;
    
    engine_impl->engine.ops->resume_receive = axis2_engine_resume_receive;    
    engine_impl->engine.ops->resume_send = axis2_engine_resume_send;
    
    return &(engine_impl->engine);
}

axis2_status_t AXIS2_CALL 
axis2_engine_free(
    axis2_engine_t *engine, 
    const axis2_env_t *env)
{
    axis2_engine_impl_t *engine_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(engine);
    
    if (engine_impl->engine.ops)
    {
        AXIS2_FREE(env->allocator, engine_impl->engine.ops);
        engine_impl->engine.ops = NULL;
    }
    
    AXIS2_FREE(env->allocator, engine_impl);
    engine_impl = NULL;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_send(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_engine_impl_t *engine_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_array_list_t *phases = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "axis2_engine_send start");

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    
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
            phases = AXIS2_OP_GET_OUT_FLOW(op, env);
        }
    }
    

    if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
    {
        /* message has paused, so rerun it from the position it stopped. 
           The handler which paused the message will be the first one to resume 
           invocation
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
        axis2_status_t status = AXIS2_FAILURE;
        status = axis2_engine_invoke_phases(engine, env, phases, msg_ctx);
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
    
    if (!(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)))
    {
        /* write the message to wire */
        axis2_transport_sender_t *transport_sender = NULL;
        axis2_transport_out_desc_t *transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env);
        
        if (transport_out)
        {
            transport_sender = AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out, env);
            if (transport_sender)
                AXIS2_TRANSPORT_SENDER_INVOKE(transport_sender, env, msg_ctx);
            else
                return AXIS2_FAILURE;
        }
        else
        {
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Transport out is not set in message context");
            return AXIS2_FAILURE;
        }
    }
    
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Axis2 engine send successful");
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "axis2_engine_send end");
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_receive(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_engine_impl_t *engine_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_op_t *op = NULL;
    axis2_array_list_t *pre_calculated_phases = NULL;
    axis2_array_list_t *op_specific_phases = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(engine);
    
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
    
    conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
    
    pre_calculated_phases = 
        AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env);
    
    if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
    {
        /* the message has paused, so re-run them from the position they stopped. */
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
            op_specific_phases = AXIS2_OP_GET_IN_FLOW(op, env);
            axis2_engine_resume_invocation_phases(engine, env, op_specific_phases, msg_ctx);
            if (AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
            {
                return AXIS2_SUCCESS;
            }
        }
    } 
    else 
    {
        status = axis2_engine_invoke_phases(engine, env, pre_calculated_phases, msg_ctx);
        if (status != AXIS2_SUCCESS)
        {
            if (AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env))
                return status;
        }

        if (AXIS2_TRUE == AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
        {
            return AXIS2_SUCCESS;
        }

        axis2_engine_verify_ctx_built(engine, env, msg_ctx);   /* TODO : Remove me. I'm redundant */
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        if (op_ctx)
        {
            op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
            op_specific_phases = AXIS2_OP_GET_IN_FLOW(op, env);
            status = axis2_engine_invoke_phases(engine, env, op_specific_phases, msg_ctx);
            if (status != AXIS2_SUCCESS)
            {
                return status;
            }
                                        
            if (AXIS2_TRUE == AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
            {
                return AXIS2_SUCCESS;
            }
        }
    }

    if ( (AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env)) && !(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))) 
    {
        axis2_msg_recv_t *receiver = NULL;
       
        status = axis2_engine_check_must_understand_headers(env, msg_ctx);
        if (status != AXIS2_SUCCESS)
            return status;
        
        /* invoke the message receivers */
        if (!op)
            return AXIS2_FAILURE;
        receiver = AXIS2_OP_GET_MSG_RECV(op, env);
        if (!receiver)
        {
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Message receiver not set in operation description");
            return AXIS2_FAILURE;
        }
        status = AXIS2_MSG_RECV_RECEIVE(receiver, env, msg_ctx, receiver->derived);        
    }
    else
    {
        status = AXIS2_SUCCESS;
    }
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Axis2 engine receive successful");

    return status;
}

axis2_status_t AXIS2_CALL 
axis2_engine_send_fault(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_ctx_t *op_ctx = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    
    if (!(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))) 
    {
        /* send the SOAP Fault*/
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_transport_sender_t *transport_sender = NULL;
        axis2_transport_out_desc_t *transport_out  = NULL;
        conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        if (conf_ctx)
        {
            axis2_conf_t *conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
            if (conf)
            {
                 axis2_array_list_t *phases = AXIS2_CONF_GET_OUT_FAULT_FLOW(conf, env);
                 if (phases)
                 {
                    axis2_engine_invoke_phases(engine, env, phases, msg_ctx);
                 }
            }
        }
        
        transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env);
        
        if (transport_out)
            transport_sender = AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out, env);
        if (transport_sender)
            AXIS2_TRANSPORT_SENDER_INVOKE(transport_sender, env, msg_ctx);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_receive_fault(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_ctx_t *op_ctx = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    
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
        axis2_array_list_t *phases = AXIS2_OP_GET_FAULT_IN_FLOW(op, env);
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

axis2_msg_ctx_t *AXIS2_CALL 
axis2_engine_create_fault_msg_ctx(
    axis2_engine_t *engine, 
    const axis2_env_t *env,
    axis2_msg_ctx_t *processing_context)
{
    axis2_msg_ctx_t *fault_ctx = NULL;
    axis2_engine_impl_t *engine_impl = NULL;
    axis2_endpoint_ref_t *fault_to = NULL;
    axis2_endpoint_ref_t *reply_to = NULL;
    axis2_property_t *property = NULL;
    axiom_soap_envelope_t *envelope = NULL;
    const axis2_char_t *wsa_action = NULL;
    const axis2_char_t *msg_id = NULL;
    axis2_relates_to_t *relates_to = NULL;
    axis2_char_t *msg_uuid = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, processing_context, AXIS2_FAILURE);
    
    engine_impl = AXIS2_INTF_TO_IMPL(engine);
    
    if (AXIS2_MSG_CTX_GET_PROCESS_FAULT(processing_context, env)) 
    {
        AXIS2_ERROR_SET(env->error, 
            AXIS2_ERROR_INVALID_STATE_PROCESSING_FAULT_ALREADY, AXIS2_FAILURE);
        return NULL;
    }

    fault_ctx = axis2_msg_ctx_create(env, engine_impl->conf_ctx,
                    AXIS2_MSG_CTX_GET_TRANSPORT_IN_DESC(processing_context, env),
                    AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(processing_context, env));

    AXIS2_MSG_CTX_SET_PROCESS_FAULT(fault_ctx, env, AXIS2_TRUE);
    
    fault_to = AXIS2_MSG_CTX_GET_FAULT_TO(processing_context, env);
    if (fault_to)
    {
        const axis2_char_t *address = AXIS2_ENDPOINT_REF_GET_ADDRESS(fault_to, env);
        if (!address)
        {
            fault_to = NULL;
        }
        else if (AXIS2_STRCMP(AXIS2_WSA_NONE_URL, address) == 0 ||
            AXIS2_STRCMP(AXIS2_WSA_NONE_URL_SUBMISSION, address) == 0)
        {
            reply_to = AXIS2_MSG_CTX_GET_REPLY_TO(processing_context, env);
            if (reply_to)
            {
               AXIS2_MSG_CTX_SET_FAULT_TO(fault_ctx, env, reply_to); 
            }
            else
            {
                AXIS2_MSG_CTX_SET_FAULT_TO(fault_ctx, env, fault_to);
            }
        }
        else
        {
            AXIS2_MSG_CTX_SET_FAULT_TO(fault_ctx, env, fault_to);
        }
        
    }
    
    property = AXIS2_MSG_CTX_GET_PROPERTY(processing_context, env, 
            AXIS2_TRANSPORT_OUT, AXIS2_FALSE);
    if(property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(fault_ctx, env, AXIS2_TRANSPORT_OUT, property, 
            AXIS2_FALSE); 
    }
   
    if (!fault_to && !property)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NOWHERE_TO_SEND_FAULT, AXIS2_FAILURE);
        return NULL;
    }
    property = NULL;

    /* set soap action */
    wsa_action = AXIS2_MSG_CTX_GET_SOAP_ACTION(processing_context, env);
    if (!wsa_action)
    {
        wsa_action = "http://www.w3.org/2005/08/addressing/fault";
    }
    AXIS2_MSG_CTX_SET_WSA_ACTION(fault_ctx, env, wsa_action);

    /* set relates to */
    msg_id = AXIS2_MSG_CTX_GET_MSG_ID(processing_context, env);
    relates_to = axis2_relates_to_create(env, msg_id,
            AXIS2_WSA_RELATES_TO_RELATIONSHIP_TYPE_DEFAULT_VALUE);
    AXIS2_MSG_CTX_SET_RELATES_TO(fault_ctx, env, relates_to);
    
    /* set msg id */
    msg_uuid =  axis2_uuid_gen(env);
    AXIS2_MSG_CTX_SET_MESSAGE_ID(fault_ctx, env, msg_uuid);
    if(NULL != msg_uuid)
    {
        AXIS2_FREE(env->allocator, msg_uuid);
        msg_uuid = NULL;
    }

    AXIS2_MSG_CTX_SET_OP_CTX(fault_ctx, env, AXIS2_MSG_CTX_GET_OP_CTX(processing_context, env));
    AXIS2_MSG_CTX_SET_PROCESS_FAULT(fault_ctx, env, AXIS2_TRUE);
    AXIS2_MSG_CTX_SET_SERVER_SIDE(fault_ctx, env, AXIS2_TRUE);
    
    property = AXIS2_MSG_CTX_GET_PROPERTY(processing_context, env, 
        AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_FALSE);
    if(property)
    {
        AXIS2_MSG_CTX_SET_PROPERTY(fault_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, 
            property , AXIS2_FALSE);
        property = NULL;
    }

    envelope = AXIS2_MSG_CTX_GET_FAULT_SOAP_ENVELOPE(processing_context, env);
    
    if (!envelope)
    {
        if (AXIS2_MSG_CTX_GET_IS_SOAP_11(processing_context, env)) 
        {
            envelope = axiom_soap_envelope_create_default_soap_envelope(env, AXIOM_SOAP11);
            
        } 
        else 
        {
            envelope = axiom_soap_envelope_create_default_soap_envelope(env, AXIOM_SOAP12);
        }

        if (envelope)
        {
            axiom_soap_body_t *body = AXIOM_SOAP_ENVELOPE_GET_BODY(envelope, env);
            if (body)
            {
                axiom_soap_fault_t *fault = AXIOM_SOAP_BODY_GET_FAULT(body, env);
                axis2_engine_extract_fault_info_from_msg_ctx(engine, env, 
                        processing_context, fault);
            }
        }
        else
        {
            return NULL;
        }
    }
    
    AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(fault_ctx, env, envelope);
    AXIS2_MSG_CTX_SET_PROPERTY(fault_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, 
        AXIS2_MSG_CTX_GET_PROPERTY(processing_context, env, AXIS2_HTTP_OUT_TRANSPORT_INFO, AXIS2_FALSE), AXIS2_FALSE);
    return fault_ctx;
}

axis2_status_t AXIS2_CALL 
axis2_engine_extract_fault_info_from_msg_ctx(
    axis2_engine_t *engine, 
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    struct axiom_soap_fault *fault)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault, AXIS2_FAILURE);

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_verify_ctx_built(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_msg_ctx_t *msg_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);    
    
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

axis2_status_t AXIS2_CALL 
axis2_engine_invoke_phases(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_array_list_t *phases, 
    axis2_msg_ctx_t *msg_ctx)
{
    int i = 0;
    int count = 0;
    axis2_status_t status = AXIS2_SUCCESS;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, phases, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);    
    
    if(phases)
        count = AXIS2_ARRAY_LIST_SIZE(phases, env);
    for (i = 0; (i < count && !(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))); i++) 
    {
        axis2_phase_t *phase = (axis2_phase_t *) 
            AXIS2_ARRAY_LIST_GET(phases, env, i);
        
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
            "Invoking phase %s", AXIS2_PHASE_GET_NAME(phase, env));
        
        status = AXIS2_PHASE_INVOKE(phase, env, msg_ctx);
        if (status != AXIS2_SUCCESS)
        {
            return status;
        }
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_resume_invocation_phases(
    axis2_engine_t *engine, 
    const axis2_env_t *env, 
    axis2_array_list_t *phases, 
    axis2_msg_ctx_t *msg_ctx)
{
    int i = 0;
    int count = 0;
    axis2_bool_t found_match = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, phases, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);    
    
    AXIS2_MSG_CTX_SET_PAUSED(msg_ctx, env, AXIS2_FALSE);
    
    count = AXIS2_ARRAY_LIST_SIZE(phases, env);

    for (i = 0; i < count && !(AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)); i++) 
    {
        axis2_phase_t *phase = (axis2_phase_t *) AXIS2_ARRAY_LIST_GET(phases, 
            env, i);
        const axis2_char_t *phase_name = AXIS2_PHASE_GET_NAME(phase, env);
        const axis2_char_t *paused_phase_name = AXIS2_MSG_CTX_GET_PAUSED_PHASE_NAME(
            msg_ctx, env);
        /* skip invoking handlers until we find the paused phase */
        if (phase_name && paused_phase_name && 0 == AXIS2_STRCMP(phase_name, 
            paused_phase_name))
        {
            int paused_handler_i = -1;
            found_match = AXIS2_TRUE;

            paused_handler_i = AXIS2_MSG_CTX_GET_CURRENT_HANDLER_INDEX(msg_ctx, 
                env);
            /* invoke the paused handler and rest of the handlers of the paused 
             * phase */
            AXIS2_PHASE_INVOKE_START_FROM_HANDLER(phase, env, paused_handler_i, 
                msg_ctx);
        }
        else /* now we have found the paused phase and invoked the rest of the
              * handlers of that phase, invoke all the phases after that */ 
        {
            if (found_match) 
            {
                AXIS2_PHASE_INVOKE(phase, env, msg_ctx);
            }
       }
    }
    
    return AXIS2_SUCCESS;
}

const axis2_char_t *AXIS2_CALL 
axis2_engine_get_sender_fault_code(
    const axis2_engine_t *engine, 
    const axis2_env_t *env, 
    const axis2_char_t *soap_namespace) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_namespace, AXIS2_FAILURE);
    
    return NULL;
}

const axis2_char_t *AXIS2_CALL 
axis2_engine_get_receiver_fault_code(
    const axis2_engine_t *engine, 
    const axis2_env_t *env, 
    const axis2_char_t *soap_namespace) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_namespace, AXIS2_FAILURE);
    
    if (AXIS2_STRCMP(AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI, soap_namespace))
        return AXIOM_SOAP12_FAULT_CODE_RECEIVER;
    else
        return AXIOM_SOAP11_FAULT_CODE_RECEIVER;
    return NULL;
}

axis2_status_t 
axis2_engine_check_must_understand_headers(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx) 
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_header_t *soap_header = NULL;
    axis2_hash_t *header_block_ht = NULL;
    axis2_hash_index_t *hash_index = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    soap_envelope = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if (!soap_envelope)
        return AXIS2_FAILURE;

    soap_header = AXIOM_SOAP_ENVELOPE_GET_HEADER(soap_envelope, env);
    if (!soap_header)
        return AXIS2_SUCCESS;

    header_block_ht = AXIOM_SOAP_HEADER_GET_ALL_HEADER_BLOCKS(soap_header, env);
    if(!header_block_ht)
        return AXIS2_SUCCESS;            
    
    for(hash_index = axis2_hash_first(header_block_ht, env); hash_index;
            hash_index = axis2_hash_next(env, hash_index))
    {   
        void *hb = NULL;
        axiom_soap_header_block_t *header_block = NULL;
        axis2_char_t *role = NULL;
        
        axis2_hash_this(hash_index, NULL, NULL, &hb);
        header_block = (axiom_soap_header_block_t *)hb;

        if (header_block)
        {
            if (AXIOM_SOAP_HEADER_BLOCK_IS_PROCESSED(header_block , env) ||
                    !AXIOM_SOAP_HEADER_BLOCK_GET_MUST_UNDERSTAND(header_block, env))
            {
                continue;
            }
            
            /* if this header block is not targeted to me then its not my
               problem. Currently this code only supports the "next" role; we
               need to fix this to allow the engine/service to be in one or more
               additional roles and then to check that any headers targeted for
               that role too have been dealt with. */
            role = AXIOM_SOAP_HEADER_BLOCK_GET_ROLE(header_block, env);
            
            if (AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env) != AXIS2_TRUE )
            {
                /* SOAP 1.2 */
                if (!role || AXIS2_STRCMP(role, AXIOM_SOAP12_SOAP_ROLE_NEXT) != 0 )
                {
                    axiom_soap_envelope_t *temp_env = 
                            axiom_soap_envelope_create_default_soap_fault_envelope(env,
                                    "soapenv:MustUnderstand",
                                    "Header not understood",
                                    AXIOM_SOAP12, NULL, NULL);
                    AXIS2_MSG_CTX_SET_FAULT_SOAP_ENVELOPE(msg_ctx, env, temp_env);
                    AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env,
                             "http://www.w3.org/2005/08/addressing/fault");
                    return AXIS2_FAILURE;
                }
            }
            else
            {
                /* SOAP 1.1 */
                if (!role || AXIS2_STRCMP(role, AXIOM_SOAP11_SOAP_ACTOR_NEXT) != 0 )
                {
                    axiom_soap_envelope_t *temp_env = 
                            axiom_soap_envelope_create_default_soap_fault_envelope(env,
                                    "soapenv:MustUnderstand",
                                    "Header not understood",
                                    AXIOM_SOAP11, NULL, NULL);
                    AXIS2_MSG_CTX_SET_FAULT_SOAP_ENVELOPE(msg_ctx, env, temp_env);
                    AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env,
                             "http://www.w3.org/2005/08/addressing/fault");
                    return AXIS2_FAILURE;
                }

            }
            
        }
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_engine_resume_receive(
    axis2_engine_t *engine,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_array_list_t *phases = NULL;
    
    /* find and invoke the phases */
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
    conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
    phases =
        AXIS2_CONF_GET_IN_PHASES_UPTO_AND_INCLUDING_POST_DISPATCH(conf, env);
    
    axis2_engine_resume_invocation_phases(engine, env, phases, msg_ctx);
    /* invoking the message receiver */
    if (AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env) && !AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env)) 
    {
        /* invoke the message receivers */
        axis2_op_ctx_t *op_ctx = NULL;

        status = axis2_engine_check_must_understand_headers(env, msg_ctx);
        if (status != AXIS2_SUCCESS)
            return status;
            
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        if (op_ctx)
        {
            axis2_op_t *op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
            if (op)
            {
                axis2_msg_recv_t *receiver = NULL;
                receiver = AXIS2_OP_GET_MSG_RECV(op, env);
                if (!receiver)
                {
                    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Message receiver not set in operation description");
                    return AXIS2_FAILURE;
                }
                status = AXIS2_MSG_RECV_RECEIVE(receiver, env, msg_ctx, receiver->derived);
            }
        }
    }

    return status;
}

axis2_status_t AXIS2_CALL 
axis2_engine_resume_send(
    axis2_engine_t *engine,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_array_list_t *phases = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    /* invoke the phases */
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);    
    if (op_ctx)
    {
        axis2_op_t *op = AXIS2_OP_CTX_GET_OP(op_ctx, env);
        if (op)
        {
            phases = AXIS2_OP_GET_OUT_FLOW(op, env);
        }
    }
    axis2_engine_resume_invocation_phases(engine, env, phases, msg_ctx);
    
    /* invoking transport sender */
    if (!AXIS2_MSG_CTX_IS_PAUSED(msg_ctx, env))
    {
        /* write the message to the wire */
        axis2_transport_out_desc_t *transport_out = NULL;
        axis2_transport_sender_t *sender = NULL;
        transport_out = AXIS2_MSG_CTX_GET_TRANSPORT_OUT_DESC(msg_ctx, env);
        if (transport_out)
        {
            sender = AXIS2_TRANSPORT_OUT_DESC_GET_SENDER(transport_out, env);
            if (sender)
            {
                status = AXIS2_TRANSPORT_SENDER_INVOKE(sender, env, msg_ctx);
            }
        }
    }
    
    return status;
}
