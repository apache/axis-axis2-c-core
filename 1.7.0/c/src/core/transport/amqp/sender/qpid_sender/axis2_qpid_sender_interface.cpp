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

#include <axis2_amqp_util.h>
#include <axis2_qpid_sender.h>
#include <axis2_qpid_sender_interface.h>

#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_EXTERN axis2_amqp_response_t* AXIS2_CALL
axis2_qpid_send_receive(
	const axis2_char_t* request_content,
	const axutil_env_t* env,
	const axis2_char_t* content_type,
	const axis2_char_t* soap_action,
	axis2_msg_ctx_t* msg_ctx)
{
	axis2_amqp_destination_info_t* destination_info = NULL;
	destination_info = axis2_amqp_util_msg_ctx_get_destination_info(msg_ctx, env);

	if (!destination_info || !destination_info->broker_ip || 
		!destination_info->broker_port || !destination_info->queue_name)
	{
		return NULL;
	}

	axis2_bool_t is_soap_11 = axis2_msg_ctx_get_is_soap_11(msg_ctx, env);
	axutil_array_list_t* mime_parts = axis2_msg_ctx_get_mime_parts(msg_ctx, env);
	int timeout = axis2_amqp_util_msg_ctx_get_request_timeout(msg_ctx, env);

	/* Get Response */
	Axis2QpidSender qpid_sender(destination_info->broker_ip, 
			destination_info->broker_port, env);
	
	bool status = qpid_sender.SendReceive(request_content, destination_info->queue_name, 
			is_soap_11, content_type, soap_action, mime_parts, timeout);

	axis2_amqp_destination_info_free(destination_info, env);

	if (!status)
	{
		return NULL;
	}

	/* Create response */
	axis2_amqp_response_t* response = (axis2_amqp_response_t*)AXIS2_MALLOC(
			env->allocator, sizeof(axis2_amqp_response_t));

	/* Data */
	response->data = AXIS2_MALLOC(env->allocator, qpid_sender.responseContent.size());
	memcpy(response->data, qpid_sender.responseContent.c_str(), 
			qpid_sender.responseContent.size());

	/* Length */
	response->length = qpid_sender.responseContent.size();

	/* ContentType */
	response->content_type = (axis2_char_t*)AXIS2_MALLOC(
			env->allocator, qpid_sender.responseContentType.size() + 1);
	strcpy(response->content_type, qpid_sender.responseContentType.c_str());

	return response;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_qpid_send(
	const axis2_char_t* request_content,
	const axutil_env_t* env,
	const axis2_char_t* content_type,
	const axis2_char_t* soap_action,
	axis2_msg_ctx_t* msg_ctx)
{
	axis2_amqp_destination_info_t* destination_info = NULL;
	axis2_status_t status =  AXIS2_FAILURE;
	string reply_to_queue_name = "";

	destination_info = axis2_amqp_util_msg_ctx_get_destination_info(msg_ctx, env);

	if (!destination_info || !destination_info->broker_ip || 
		!destination_info->broker_port || !destination_info->queue_name)
	{
		return AXIS2_FAILURE;
	}

	axis2_bool_t is_soap_11 = axis2_msg_ctx_get_is_soap_11(msg_ctx, env);
	axutil_array_list_t* mime_parts = axis2_msg_ctx_get_mime_parts(msg_ctx, env);

	/* If client side, find reply_to_queue_name */
	if (!axis2_msg_ctx_get_server_side(msg_ctx, env))
	{
		axis2_conf_ctx_t* conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);

		axis2_char_t* queue_name = 
			axis2_amqp_util_conf_ctx_get_dual_channel_queue_name(conf_ctx, env);
		if (queue_name)
			reply_to_queue_name = queue_name;
	}

	Axis2QpidSender qpid_sender(destination_info->broker_ip, 
			destination_info->broker_port, env);

	status = qpid_sender.Send(request_content, destination_info->queue_name, 
			reply_to_queue_name, is_soap_11, content_type, soap_action, mime_parts);

	axis2_amqp_destination_info_free(destination_info, env);

	return status;
}

#ifdef __cplusplus
}
#endif
