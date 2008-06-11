/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axiom_soap.h>
#include <axis2_amqp_defines.h>
#include <axis2_amqp_util.h>
#include <axis2_amqp_sender.h>

static const axis2_transport_sender_ops_t amqp_sender_ops = {
	axis2_amqp_sender_init,
	axis2_amqp_sender_invoke,
	axis2_amqp_sender_clean_up,
	axis2_amqp_sender_free};

axis2_transport_sender_t* AXIS2_CALL
axis2_amqp_sender_create (const axutil_env_t* env)
{
	AXIS2_ENV_CHECK (env, NULL);

	axis2_amqp_sender_resource_pack_t* sender_resource_pack = NULL;

	sender_resource_pack = (axis2_amqp_sender_resource_pack_t*)
							AXIS2_MALLOC (env->allocator,
										  sizeof (axis2_amqp_sender_resource_pack_t));

	if (!sender_resource_pack)
	{
		AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}

	sender_resource_pack->sender.ops = &amqp_sender_ops;

	return &(sender_resource_pack->sender);
}


axis2_status_t AXIS2_CALL 
axis2_amqp_sender_init (axis2_transport_sender_t*   sender,
					    const axutil_env_t* 		env,
					    axis2_conf_ctx_t* 		    conf_ctx,
					    axis2_transport_out_desc_t* out_desc)
{
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_amqp_sender_invoke (axis2_transport_sender_t* sender,
						  const axutil_env_t* 	    env,
						  axis2_msg_ctx_t* 		    msg_ctx)
{
	AXIS2_ENV_CHECK (env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK (env->error, msg_ctx, AXIS2_FAILURE);

	axiom_soap_envelope_t* request_soap_envelope = NULL;
	axiom_xml_writer_t* xml_writer = NULL;
	axiom_output_t* request_om_output = NULL;
	axis2_char_t* request_content = NULL;
	axis2_char_t* response_content = NULL;
	axiom_soap_envelope_t* response_soap_envelope = NULL;
	axis2_op_t* op = NULL;
	const axis2_char_t* mep = NULL;
	axis2_bool_t is_server = AXIS2_TRUE;
	axis2_char_t* qpid_broker_ip = NULL;
	int qpid_broker_port = 0;
	axis2_char_t* send_to = NULL;

	request_soap_envelope = axis2_msg_ctx_get_soap_envelope (msg_ctx, env);
	
	xml_writer = axiom_xml_writer_create_for_memory (env,
													 NULL,
													 AXIS2_TRUE,
													 0,
													 AXIS2_XML_PARSER_TYPE_BUFFER);
	if (!xml_writer)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create XML Writer");
		return AXIS2_FAILURE;
	}

	request_om_output = axiom_output_create (env, xml_writer);
	if (!request_om_output)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create OM Output");
		axiom_xml_writer_free (xml_writer, env);
		xml_writer = NULL;
		return AXIS2_FAILURE;
	}

	axiom_soap_envelope_serialize (request_soap_envelope, env, request_om_output, AXIS2_FALSE);

	request_content = (axis2_char_t*)axiom_xml_writer_get_xml (xml_writer, env);
	if (!request_content)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Serialize the SOAP Envelope");
		return AXIS2_FAILURE;
	}
	
	op = axis2_msg_ctx_get_op (msg_ctx, env);
	mep = axis2_op_get_msg_exchange_pattern (op, env);

	is_server = axis2_msg_ctx_get_server_side (msg_ctx, env);

	if (is_server)
	{
		/* Get Qpid Broker IP/Port/ReplyTo from the msg_ctx */
		qpid_broker_ip = (axis2_char_t*)axis2_msg_ctx_get_property_value (msg_ctx,
																		  env,
																		  AXIS2_AMQP_MSG_CTX_PROPERTY_QPID_BROKER_IP);

		qpid_broker_port = *(int*)axis2_msg_ctx_get_property_value (msg_ctx,
																	env,
																	AXIS2_AMQP_MSG_CTX_PROPERTY_QPID_BROKER_PORT);
		
		send_to = (axis2_char_t*)axis2_msg_ctx_get_property_value (msg_ctx,
																   env,
																   AXIS2_AMQP_MSG_CTX_PROPERTY_REPLY_TO);

		axis2_qpid_server_send (request_content,
								qpid_broker_ip,
								qpid_broker_port,
								send_to);
	}
	else
	{
		/* Get Qpid Broker IP/Port from the Request URI */
		qpid_broker_ip = axis2_amqp_util_get_qpid_broker_ip (msg_ctx, env);
		qpid_broker_port = axis2_amqp_util_get_qpid_broker_port (msg_ctx, env);

		if (axutil_strcmp (mep, AXIS2_MEP_URI_OUT_ONLY) == 0 || 
			axutil_strcmp (mep, AXIS2_MEP_URI_ROBUST_OUT_ONLY) == 0)
		{
			axis2_qpid_client_send (request_content,
									qpid_broker_ip,
									qpid_broker_port);
		}
		else
		{
			response_content = axis2_qpid_client_request (request_content,
														  qpid_broker_ip,
														  qpid_broker_port,
														  env);

			if (response_content)
			{
				response_soap_envelope = axis2_amqp_util_get_soap_envelope (response_content, env);
				
				if (response_soap_envelope)
					axis2_msg_ctx_set_response_soap_envelope (msg_ctx, env, response_soap_envelope);

				/*AXIS2_FREE (env->allocator, response_content);*/
			}
        }
	}
	
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_amqp_sender_clean_up (axis2_transport_sender_t* sender,
						    const axutil_env_t* 	  env,
						    axis2_msg_ctx_t* 		  msg_ctx)
{
	return AXIS2_SUCCESS;
}


void AXIS2_CALL 
axis2_amqp_sender_free (axis2_transport_sender_t* sender,
					    const axutil_env_t* 	  env)
{
	AXIS2_ENV_CHECK (env, void);

	axis2_amqp_sender_resource_pack_t* sender_resource_pack = NULL;
	sender_resource_pack = AXIS2_AMQP_SENDER_TO_RESOURCE_PACK (sender);

	AXIS2_FREE (env->allocator, sender_resource_pack);
}

/* Library Exports */

AXIS2_EXPORT int 
#ifndef AXIS2_STATIC_DEPLOY
axis2_get_instance (
#else
axis2_amqp_sender_get_instance (
#endif
    struct axis2_transport_sender** inst,
    const axutil_env_t*             env)
{
    int status = AXIS2_SUCCESS;

    *inst = axis2_amqp_sender_create (env);
    if (!(*inst))
    {
        status = AXIS2_FAILURE;
    }

    return status;
}


AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
axis2_remove_instance (
#else
axis2_amqp_sender_remove_instance (
#endif
    axis2_transport_sender_t* inst,
    const axutil_env_t*       env)
{
    if (inst)
    {
        axis2_transport_sender_free (inst, env);
    }

    return AXIS2_SUCCESS;
}

