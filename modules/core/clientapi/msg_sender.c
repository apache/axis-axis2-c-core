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

#include <axis2_msg_sender.h>
#include <axis2_msg_ctx.h>
#include <axis2_svc_ctx.h>
#include <axis2_engine.h>
#include <axis2_msg_info_headers.h>

typedef struct axis2_msg_sender_impl axis2_msg_sender_impl_t;

struct axis2_msg_sender_impl
{
    axis2_msg_sender_t msg_sender;
    axis2_mep_client_t *base;
    axis2_transport_out_desc_t *sender_transport;
    axis2_op_t *op_template;
	axis2_msg_info_headers_t *msg_info_headers;
};

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(msg_sender) ((axis2_msg_sender_impl_t *)msg_sender)

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL 
axis2_msg_sender_send(axis2_msg_sender_t *msg_sender, axis2_env_t **env,
						axis2_op_t *op,
						axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_msg_sender_set_transport_info(axis2_msg_sender_t *msg_sender, 
						axis2_env_t **env,
						axis2_char_t *sender_transport);

axis2_status_t AXIS2_CALL 
axis2_msg_sender_send_with_om(struct axis2_msg_sender *msg_sender, 
						axis2_env_t **env,
						axis2_char_t *op_name, 
						axis2_om_node_t *om_node_to_send);

axis2_status_t AXIS2_CALL 
axis2_msg_sender_send_with_soap(axis2_msg_sender_t *msg_sender, 
						axis2_env_t **env,
						axis2_char_t *op_name, 
						axis2_soap_envelope_t *envelope);
axis2_msg_info_headers_t* AXIS2_CALL 
axis2_msg_sender_get_msg_info_headers(axis2_msg_sender_t *msg_sender, 
						axis2_env_t **env);
					
axis2_status_t AXIS2_CALL 
axis2_msg_sender_free(struct axis2_msg_sender *msg_sender, axis2_env_t **env);
	
/***************************** End of function headers ************************/

axis2_msg_sender_t* AXIS2_CALL axis2_msg_sender_create(axis2_env_t **env, 
						axis2_svc_ctx_t *svc_ctx) 
{
    axis2_msg_sender_impl_t *msg_sender_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_ctx, NULL);
    
    msg_sender_impl = AXIS2_MALLOC( (*env)->allocator, 
						sizeof(axis2_msg_sender_impl_t));
	
    if (NULL == msg_sender_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }
    
    msg_sender_impl->msg_sender.ops = NULL;
    msg_sender_impl->base = NULL;
    msg_sender_impl->sender_transport = NULL;
    msg_sender_impl->op_template = NULL;
	msg_sender_impl->msg_info_headers = NULL;
        
    msg_sender_impl->base = axis2_mep_client_create(env, svc_ctx, 
						MEP_URI_IN_ONLY);
    if (NULL == msg_sender_impl->base)
    {
        axis2_msg_sender_free(&(msg_sender_impl->msg_sender), env);
        return NULL;
    }
	msg_sender_impl->msg_info_headers = axis2_msg_info_headers_create(env, 
						NULL, NULL);
    
    /* initialize ops */    
    msg_sender_impl->msg_sender.ops  = AXIS2_MALLOC((*env)->allocator, 
						sizeof(axis2_msg_sender_ops_t));
    if (NULL == msg_sender_impl->msg_sender.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_msg_sender_free(&(msg_sender_impl->msg_sender), env);
        return NULL;        
    }

    msg_sender_impl->msg_sender.ops->send = axis2_msg_sender_send;
    msg_sender_impl->msg_sender.ops->set_transport_info = 
						axis2_msg_sender_set_transport_info;
    msg_sender_impl->msg_sender.ops->send_with_om = 
						axis2_msg_sender_send_with_om;
    msg_sender_impl->msg_sender.ops->send_with_soap = 
						axis2_msg_sender_send_with_soap;
	msg_sender_impl->msg_sender.ops->get_msg_info_headers = 
						axis2_msg_sender_get_msg_info_headers;
    msg_sender_impl->msg_sender.ops->free = axis2_msg_sender_free;

    return &(msg_sender_impl->msg_sender);
}

axis2_status_t AXIS2_CALL axis2_msg_sender_free(struct axis2_msg_sender *msg_sender, 
                                   axis2_env_t **env)
{
    axis2_msg_sender_impl_t *msg_sender_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_sender, env, AXIS2_FAILURE);
    
    msg_sender_impl = AXIS2_INTF_TO_IMPL(msg_sender);
    
    if (NULL != msg_sender_impl->msg_sender.ops)
    {
        AXIS2_FREE((*env)->allocator, msg_sender_impl->msg_sender.ops);
        msg_sender_impl->msg_sender.ops = NULL;
    }
    
    if (NULL != msg_sender_impl->base)
    {
        AXIS2_MEP_CLIENT_FREE(msg_sender_impl->base, env);
        msg_sender_impl->base = NULL;
    }    
    
    AXIS2_FREE((*env)->allocator, msg_sender_impl);
    msg_sender_impl = NULL;
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_msg_sender_send(axis2_msg_sender_t *msg_sender, axis2_env_t **env,
						axis2_op_t *op,
						axis2_msg_ctx_t *msg_ctx)
{
    axis2_msg_sender_impl_t *msg_sender_impl = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    axis2_svc_ctx_t *svc_ctx = NULL;
    axis2_char_t *message_id = NULL;
	axis2_conf_ctx_t *sys_context = NULL;
	axis2_engine_t *engine = NULL;
		
    AXIS2_FUNC_PARAM_CHECK(msg_sender, env, AXIS2_FAILURE);
    
    msg_sender_impl = AXIS2_INTF_TO_IMPL(msg_sender);
    
    status = AXIS2_MEP_CLIENT_PREPARE_INVOCATION(msg_sender_impl->base, env, 
						op, msg_ctx);
    if (status != AXIS2_SUCCESS)
        return AXIS2_FAILURE;
  	AXIS2_MSG_CTX_SET_MSG_INFO_HEADERS(msg_ctx, env, 
						msg_sender_impl->msg_info_headers);
	message_id = "uuid:"; /* TODO UUIDGenerator.getUUID()*/
    AXIS2_MSG_CTX_SET_MESSAGE_ID(msg_ctx, env, message_id);
	
    svc_ctx = AXIS2_MEP_CLIENT_GET_SVC_CTX(msg_sender_impl->base, env);
    if (NULL == svc_ctx)
        return AXIS2_FAILURE;
	
	if(NULL == msg_sender_impl->sender_transport)
	{
		msg_sender_impl->sender_transport = AXIS2_MEP_CLIENT_INFER_TRANSPORT(
						msg_sender_impl->base, env, 
						AXIS2_MSG_INFO_HEADERS_GET_TO( 
						msg_sender_impl->msg_info_headers, env));		
	}
	AXIS2_MSG_CTX_SET_TRANSPORT_OUT_DESC(msg_ctx, env, 
						msg_sender_impl->sender_transport);
	
	sys_context = AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env);
	if(NULL == sys_context)
		return AXIS2_FAILURE;
	AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, AXIS2_OP_FIND_OP_CTX(op, env, 
						msg_ctx, svc_ctx));
	engine = axis2_engine_create(env, sys_context);
	if(NULL == engine)
		return AXIS2_FAILURE;
	return AXIS2_ENGINE_SEND(engine, env, msg_ctx);
}

axis2_status_t AXIS2_CALL 
axis2_msg_sender_set_transport_info(axis2_msg_sender_t *msg_sender, 
						axis2_env_t **env,
						axis2_char_t *sender_transport)
{
    axis2_msg_sender_impl_t *msg_sender_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_qname_t *qname = NULL;
	axis2_svc_ctx_t *svc_ctx = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(msg_sender, env, AXIS2_FAILURE);
    msg_sender_impl = AXIS2_INTF_TO_IMPL(msg_sender);
    
	svc_ctx = AXIS2_MEP_CLIENT_GET_SVC_CTX(msg_sender_impl->base, env);
    conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env);
    if (NULL == conf_ctx)
    {
        return AXIS2_FAILURE;
	}
	conf = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
	if (NULL == conf)
	{
		return AXIS2_FAILURE;
	}
	qname = axis2_qname_create(env, sender_transport, NULL, NULL);
	if (NULL == qname)
		return AXIS2_FAILURE;
	msg_sender_impl->sender_transport = AXIS2_CONF_GET_TRANSPORT_OUT(
				conf, env, qname); 
	AXIS2_QNAME_FREE(qname, env);
	if(NULL == msg_sender_impl->sender_transport)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_UNKNOWN_TRANSPORT, 
				AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	return AXIS2_SUCCESS;	
}


axis2_status_t AXIS2_CALL 
axis2_msg_sender_send_with_om(struct axis2_msg_sender *msg_sender, 
						axis2_env_t **env,
						axis2_char_t *op_name, 
						axis2_om_node_t *om_node_to_send)
{
    axis2_msg_sender_impl_t *msg_sender_impl = NULL;
    axis2_soap_envelope_t *soap_envelope = NULL;
	axis2_status_t status = AXIS2_FAILURE;
	
    AXIS2_FUNC_PARAM_CHECK(msg_sender, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, op_name, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, om_node_to_send, AXIS2_FAILURE);
	
    msg_sender_impl = AXIS2_INTF_TO_IMPL(msg_sender);
	soap_envelope = AXIS2_MEP_CLIENT_CREATE_DEFAULT_SOAP_ENVELOPE(
						msg_sender_impl->base, env);
	/* TODO AXIS2_SOAP_BODY_ADD_CHILD(AXIS2_SOAP_ENVELOPE_GET_BODY(soap_envelope, env), 
						env, om_node_to_send); */
    
	status =  axis2_msg_sender_send_with_soap(msg_sender, env, op_name, 
						soap_envelope);
	AXIS2_SOAP_ENVELOPE_FREE(soap_envelope, env);
	return status;
}

axis2_status_t AXIS2_CALL 
axis2_msg_sender_send_with_soap(axis2_msg_sender_t *msg_sender, 
						axis2_env_t **env,
						axis2_char_t *op_name, 
						axis2_soap_envelope_t *envelope)
{
    axis2_msg_sender_impl_t *msg_sender_impl = NULL;
    axis2_qname_t *op_qname = NULL;
    axis2_svc_t *svc = NULL;
    axis2_op_t *op = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
	axis2_svc_ctx_t *svc_ctx = NULL;
	axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(msg_sender, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, op_name, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, envelope, AXIS2_FAILURE);
    
    msg_sender_impl = AXIS2_INTF_TO_IMPL(msg_sender);

	svc_ctx = AXIS2_MEP_CLIENT_GET_SVC_CTX(msg_sender_impl->base, env);
    op_qname = axis2_qname_create(env, op_name, NULL, NULL);
	
	if(NULL == op_qname)
	{
		return AXIS2_FAILURE;
	}
	op = AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname); 
	if(NULL == op)
	{
		op = axis2_op_create_with_qname(env, op_qname);
		if(NULL == op)
		{
			return AXIS2_FAILURE;
		}
		AXIS2_OP_SET_REMAINING_PHASES_INFLOW(op, env, 
						AXIS2_OP_GET_REMAINING_PHASES_INFLOW(
						msg_sender_impl->op_template, env));
		AXIS2_OP_SET_PHASES_OUTFLOW(op, env, AXIS2_OP_GET_PHASES_OUTFLOW(
						msg_sender_impl->op_template, env));
		AXIS2_OP_SET_PHASES_IN_FAULT_FLOW(op, env, 
						AXIS2_OP_GET_PHASES_IN_FAULT_FLOW(
						msg_sender_impl->op_template, env));
		AXIS2_OP_SET_PHASES_OUT_FAULT_FLOW(op, env, 
						AXIS2_OP_GET_PHASES_OUT_FAULT_FLOW(
						msg_sender_impl->op_template, env));
		AXIS2_SVC_ADD_OP(AXIS2_SVC_CTX_GET_SVC(svc_ctx, env), env, op);
		
	}
	conf_ctx = AXIS2_SVC_CTX_GET_CONF_CTX(svc_ctx, env);
	msg_ctx = axis2_msg_ctx_create(env, conf_ctx, NULL, NULL);
	AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, envelope);
	status = axis2_msg_sender_send(msg_sender, env, op, msg_ctx);
	
	AXIS2_MSG_CTX_FREE(msg_ctx, env);
	msg_ctx = NULL;
	
   	return status;
}

axis2_msg_info_headers_t* AXIS2_CALL 
axis2_msg_sender_get_msg_info_headers(axis2_msg_sender_t *msg_sender, 
						axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(msg_sender, env, AXIS2_FAILURE);
	return AXIS2_INTF_TO_IMPL(msg_sender)->msg_info_headers;
}
