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

#ifndef AXIS2_CALL_H
#define AXIS2_CALL_H


/**
  * @file axis2_call.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_mep_client.h>
#include <axis2_callback.h>
#include <axis2_soap_envelope.h>


#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_call Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_call_ops axis2_call_ops_t;
typedef struct axis2_call axis2_call_t; 
struct axis2_conf;
    
/** 
 * @brief Message Context ops struct
 * Encapsulator struct for ops of axis2_call
 */  
struct axis2_call_ops
{
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
     * @param to
     */
    axis2_status_t (AXIS2_CALL *set_to)(struct axis2_call *call, 
        axis2_env_t **env,
        axis2_endpoint_ref_t *to);
    /**
     * Set transport information to the the Call, for find how the each parameter 
     * acts see the commant at the instance variables. The senarios supoorted 
     * are as follows.
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
    axis2_status_t (AXIS2_CALL *set_transport_info)(struct axis2_call *call, 
                                axis2_env_t **env,
                                axis2_char_t *sender_transport,
                                 axis2_char_t *listener_transport,
                                 axis2_bool_t use_separate_listener);
    /**
     * Check has the transports are identified correctly
     *
     * @param msg_ctx
     * @throws AxisFault
     */
    axis2_status_t (AXIS2_CALL *check_transport)(struct axis2_call *call, 
        axis2_env_t **env,
        axis2_msg_ctx_t *msg_ctx);
    /**
     * Closing the Call, this will stop the started Transport Listeners. If there are multiple
     * request to send the Call should be kept open closing only when done
     */
    axis2_status_t (AXIS2_CALL *close)(struct axis2_call *call, 
        axis2_env_t **env);
    /**
     * This will be used in invoke blocking scenario. Client will wait the amount of time specified here
     * and if there is no response, call will timeout. This should be given in multiples of 100 and defaults to 2000.
     * @param time_out_ms
     */
    axis2_status_t (AXIS2_CALL *set_time)(struct axis2_call *call, 
        axis2_env_t **env,
        long time_out_ms);
    /**
     * Invoke the blocking/Synchronous call
     *
     * @param op - this will be used to identify the operation in the client side, without dispatching
     * @param toSend - This should be OM Element (payload)
     * @return
     * @throws AxisFault
     */
    axis2_om_node_t* (AXIS2_CALL *invoke_blocking_with_om)(struct axis2_call *call, 
        axis2_env_t **env,
        axis2_char_t *op_name, axis2_om_node_t *om_node_to_send);
    /**
     * Invoke the blocking/Synchronous call
     *
     * @param op_name - this will be used to identify the operation in the client side, without dispatching
     * @param envelope - This should be SOAPEnvelope
     * @return
     * @throws AxisFault
     */
    axis2_soap_envelope_t* (AXIS2_CALL *invoke_blocking_with_soap)(struct axis2_call *call, 
        axis2_env_t **env,
        axis2_char_t *op_name, axis2_soap_envelope_t *envelope);
    /**
     * Invoke the nonblocking/Asynchronous call
     *
     * @param op_name
     * @param om_node_to_send   -  This should be OM Element (payload)
     *                 invocation behaves accordingly
     * @param callback
     * @throws org.apache.axis2.AxisFault
     */
    axis2_status_t (AXIS2_CALL *invoke_non_blocking_with_om)(struct axis2_call *call, 
        axis2_env_t **env,
        axis2_char_t *op_name,
        axis2_om_node_t *om_node_to_send,
        axis2_callback_t *callback);
    /**
     * Invoke the nonblocking/Asynchronous call
     *
     * @param op_name
     * @param envelope   -  This should be a SOAP Envelope
     *                 invocation behaves accordingly
     * @param callback
     * @throws org.apache.axis2.AxisFault
     */
    
    axis2_status_t (AXIS2_CALL *invoke_non_blocking_with_soap)(struct axis2_call *call, 
        axis2_env_t **env,
        axis2_char_t *op_name,
        axis2_soap_envelope_t *envelope,
        axis2_callback_t * callback);
    /**
     * This method create a operation desc if it null and copy the flows from the template operation
     * @param op
     * @param axisOp
     */
    axis2_op_t* (AXIS2_CALL *create_op_fill_flow)(struct axis2_call *call, 
        axis2_env_t **env,
        axis2_char_t *op_name);
    /**
     * @return
     */
    axis2_msg_ctx_t* (AXIS2_CALL *get_last_res_msg_ctx)(struct axis2_call *call, 
        axis2_env_t **env);
    
    /**
     * @param key
     * @param value
     */
    axis2_status_t (AXIS2_CALL *
    set)(axis2_call_t *call,
                    axis2_env_t **env,
                    axis2_char_t *key,
                    void *value);
    
    axis2_status_t (AXIS2_CALL *free)(struct axis2_call *call, 
                                       axis2_env_t **env);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_call
{
    axis2_call_ops_t *ops;    
};

AXIS2_DECLARE(axis2_call_t*) axis2_call_create(axis2_env_t **env, 
    axis2_svc_ctx_t *svc_ctx, 
    axis2_char_t *client_home);

/************************** Start of function macros **************************/

#define AXIS2_CALL_SET_TO(call, env, to) ((call)->ops->set_to(call, env, to))
#define AXIS2_CALL_SET_TRANSPORT_INFO(call, env, sender_transport, listener_transport, use_separate_listener) ((call)->ops->set_transport_info(call, env, sender_transport, listener_transport, use_separate_listener))
#define AXIS2_CALL_CHECK_TRANSPORT(call, env, msg_ctx) ((call)->ops->check_transport(call, env, msg_ctx))
#define AXIS2_CALL_CLOSE(call, env) ((call)->ops->close(call, env))
#define AXIS2_CALL_SET_TIME(call, env, time_out_ms) ((call)->ops->set_time(call, env, time_out_ms))
#define AXIS2_CALL_INVOKE_BLOCKING_WITH_OM(call, env, op_name, om_node_to_send) ((call)->ops->invoke_blocking_with_om(call, env, op_name, om_node_to_send))
#define AXIS2_CALL_INVOKE_BLOCKING_WITH_SOAP(call, env, op_name, envelope) ((call)->ops->invoke_blocking_with_soap(call, env, op_name, envelope))
#define AXIS2_CALL_INVOKE_NON_BLOCKING_WITH_OM(call, env, op_name, om_node_to_send, callback) ((call)->ops->invoke_non_blocking_with_om(call, env, op_name, om_node_to_send, callback))
#define AXIS2_CALL_INVOKE_NON_BLOCKING_WITH_SOAP(call, env, op_name, envelope, callback) ((call)->ops->invoke_non_blocking_with_soap(call, env, op_name, envelope, callback))
#define AXIS2_CALL_CREATE_OP_FILL_FLOW(call, env, op_name) ((call)->ops->create_op_fill_flow(call, env, op_name))
#define AXIS2_CALL_GET_LAST_RES_MSG_CTX(call, env) ((call)->ops->get_last_res_msg_ctx(call, env))
#define AXIS2_CALL_SET(call, env, key, value) \
        ((call)->ops->set(call, env, key, value))
#define AXIS2_CALL_FREE(call, env) ((call)->ops->free(call, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_CALL_H */
