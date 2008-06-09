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

#include <signal.h>
#include <axiom.h>
#include <axiom_soap.h>
#include <axis2_engine.h>
#include <axis2_amqp_defines.h>
#include <axis2_amqp_request_processor.h>

void* AXIS2_THREAD_FUNC
axis2_amqp_request_processor_thread_function (axutil_thread_t* thread,
						   					  void*			   request_data)
{
	axis2_status_t status = AXIS2_FAILURE;
	axutil_env_t* env = NULL;
	axutil_env_t* thread_env = NULL;
	axis2_amqp_request_processor_resource_pack_t* request_resource_pack = NULL;

#ifndef WIN32
#ifdef AXIS2_SVR_MULTI_THREADED
	signal (SIGPIPE, SIG_IGN);
#endif
#endif

	request_resource_pack = (axis2_amqp_request_processor_resource_pack_t*)request_data;
	
	env = request_resource_pack->env;
	thread_env = axutil_init_thread_env (env);

	/* Process Request */
	status = axis2_amqp_process_request (thread_env, request_resource_pack);

	if (status == AXIS2_SUCCESS)
	{
		AXIS2_LOG_INFO (thread_env->log, "Request Processed Successfully");
	}
	else
	{
		 AXIS2_LOG_WARNING (thread_env->log, AXIS2_LOG_SI, "Error while Processing Request");
	}

	AXIS2_FREE (thread_env->allocator, request_resource_pack->request_content);
	AXIS2_FREE (thread_env->allocator, request_resource_pack->reply_to);
	AXIS2_FREE (thread_env->allocator, request_resource_pack);

	if (thread_env)
	{
		thread_env = NULL;
	}

#ifdef AXIS2_SVR_MULTI_THREADED
	axutil_thread_pool_exit_thread (env->thread_pool, thread);
#endif

	return NULL;
}


axis2_status_t
axis2_amqp_process_request (const axutil_env_t* 						  env,
							axis2_amqp_request_processor_resource_pack_t* request_resource_pack)
{
	axiom_xml_reader_t* xml_reader = NULL;
	axiom_stax_builder_t* stax_builder = NULL;
	axiom_soap_builder_t* soap_builder = NULL;
	axis2_transport_out_desc_t* out_desc = NULL;
	axis2_transport_in_desc_t* in_desc = NULL;
	axis2_msg_ctx_t* msg_ctx = NULL;
	axiom_soap_envelope_t* soap_envelope = NULL;
	axutil_property_t* property = NULL;
	axis2_engine_t* engine = NULL;

	xml_reader = axiom_xml_reader_create_for_memory (env,
													 request_resource_pack->request_content,
													 axutil_strlen (request_resource_pack->request_content),
													 NULL,
													 AXIS2_XML_PARSER_TYPE_BUFFER);
	if (!xml_reader)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create XML Reader");
		return AXIS2_FAILURE;
	}
	
	stax_builder = axiom_stax_builder_create (env, xml_reader);
	if (!stax_builder)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create StAX Builder");
		return AXIS2_FAILURE;
	}
	
	soap_builder = axiom_soap_builder_create (env,
											  stax_builder,
											  AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
	if (!soap_builder)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Failed to Create SOAP Builder");
		return AXIS2_FAILURE;
	}

	if (!request_resource_pack->conf_ctx)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Conf Context not Available");
		return AXIS2_FAILURE;
	}

	out_desc = axis2_conf_get_transport_out (axis2_conf_ctx_get_conf (request_resource_pack->conf_ctx,
																	  env),
											 env,
											 AXIS2_TRANSPORT_ENUM_AMQP);
	if (!out_desc)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Transport Out Descriptor not Found");
		return AXIS2_FAILURE;
	}

	in_desc = axis2_conf_get_transport_in (axis2_conf_ctx_get_conf (request_resource_pack->conf_ctx,
																	env),
										   env,
										   AXIS2_TRANSPORT_ENUM_AMQP);
	if (!in_desc)
	{
		AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "Transport In Descriptor not Found");
		return AXIS2_FAILURE;
	}

	msg_ctx = axis2_msg_ctx_create (env,
									request_resource_pack->conf_ctx,
									in_desc,
									out_desc);
	axis2_msg_ctx_set_server_side (msg_ctx, env, AXIS2_TRUE);

	soap_envelope = axiom_soap_builder_get_soap_envelope (soap_builder, env);
	axis2_msg_ctx_set_soap_envelope (msg_ctx, env, soap_envelope);

	/* Set ReplyTo in the msg_ctx */
	property = axutil_property_create (env);
	axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
	axutil_property_set_value (property, env, request_resource_pack->reply_to);
	axis2_msg_ctx_set_property (msg_ctx, env, AXIS2_AMQP_MSG_CTX_PROPERTY_REPLY_TO, property);

	/* Set Qpid Broker IP in the msg_ctx */
	property = axutil_property_create (env);
	axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
	axutil_property_set_value (property, env, request_resource_pack->qpid_broker_ip);
	axis2_msg_ctx_set_property (msg_ctx, env, AXIS2_AMQP_MSG_CTX_PROPERTY_QPID_BROKER_IP, property);

	/* Set Qpid Broker Port in the msg_ctx */
	property = axutil_property_create (env);
	axutil_property_set_scope (property, env, AXIS2_SCOPE_REQUEST);
	axutil_property_set_value (property, env, &request_resource_pack->qpid_broker_port);
	axis2_msg_ctx_set_property (msg_ctx, env, AXIS2_AMQP_MSG_CTX_PROPERTY_QPID_BROKER_PORT, property);

	engine = axis2_engine_create (env, request_resource_pack->conf_ctx);

	return axis2_engine_receive (engine, env, msg_ctx);
}
