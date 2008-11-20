/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      tcp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_amqp_request_processor.h>
#include <axis2_amqp_defines.h>
#include <axis2_amqp_util.h>
#include <axis2_qpid_receiver_listener.h>
#include <axis2_qpid_receiver.h>
#include <string>

Axis2QpidReceiverListener::Axis2QpidReceiverListener(
		const axutil_env_t* env,
		axis2_conf_ctx_t*   conf_ctx)
{
	this->env = env;
	this->conf_ctx = conf_ctx;
}


Axis2QpidReceiverListener::~Axis2QpidReceiverListener(void)
{}


void Axis2QpidReceiverListener::received(Message& message)
{
	AXIS2_ENV_CHECK(env, void);

	axis2_amqp_request_processor_resource_pack_t* request_data = NULL;
#ifdef AXIS2_SVR_MULTI_THREADED
	axutil_thread_t* worker_thread = NULL;
#endif

	request_data = (axis2_amqp_request_processor_resource_pack_t*)
					AXIS2_MALLOC(env->allocator, 
								  sizeof(axis2_amqp_request_processor_resource_pack_t));

	if (!request_data)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Memory Allocation Error");
		return;
	}

	request_data->env = (axutil_env_t*)env;
	request_data->conf_ctx = conf_ctx;

	/* Create a Local Copy of Request Content */
	std::string message_data = message.getData();
	axis2_char_t* request_content = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
																message_data.size());
	memcpy(request_content, message_data.c_str(), message_data.size());

	request_data->request_content = request_content;
	request_data->content_length = message_data.size();

	/* Set ReplyTo */
	request_data->reply_to = NULL;
	if (message.getMessageProperties().hasReplyTo())
	{
		/* Create a Local Copy of ReplyTo */
		std::string reply_to_tmp = message.getMessageProperties().getReplyTo().getRoutingKey();
		axis2_char_t* reply_to = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
															 reply_to_tmp.size() + 1);
		strcpy(reply_to, reply_to_tmp.c_str());

		request_data->reply_to = reply_to;
	}

	/* Copy AMQP headers */
	/* Content-Type */
	request_data->content_type = NULL;
	std::string content_type_tmp = message.getHeaders().getString(AXIS2_AMQP_HEADER_CONTENT_TYPE);
	if (!content_type_tmp.empty())
	{
		axis2_char_t* content_type = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
															     content_type_tmp.size() + 1);
		strcpy(content_type, content_type_tmp.c_str());

		request_data->content_type = content_type;
	}
	
	/* SOAPAction */
	request_data->soap_action = NULL;
	std::string soap_action_tmp = message.getHeaders().getString(AXIS2_AMQP_HEADER_SOAP_ACTION);
	if (!soap_action_tmp.empty())
	{
		axis2_char_t* soap_action = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
															    soap_action_tmp.size() + 1);
		strcpy(soap_action, soap_action_tmp.c_str());

		request_data->soap_action = soap_action;
	}

#ifdef AXIS2_SVR_MULTI_THREADED
	worker_thread = axutil_thread_pool_get_thread(env->thread_pool,
												  axis2_amqp_request_processor_thread_function,
												  (void*)request_data);

	if (!worker_thread)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create Thread");
		return;
	}

	axutil_thread_pool_thread_detach(env->thread_pool, worker_thread);
#else
	axis2_amqp_request_processor_thread_function(NULL, (void*)request_data);
#endif
}
