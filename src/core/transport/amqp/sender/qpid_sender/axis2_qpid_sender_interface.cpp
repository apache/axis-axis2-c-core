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

AXIS2_EXTERN axis2_amqp_binary_data_buffer_t* AXIS2_CALL
axis2_qpid_client_send_receive(
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

	/* Get Response */
	Axis2QpidSender qpid_sender(destination_info->broker_ip, 
			destination_info->broker_port, env);
	bool status = qpid_sender.ClientSendReceive(request_content, destination_info->queue_name, 
			is_soap_11, content_type, soap_action, mime_parts);

	AXIS2_FREE(env->allocator, destination_info->broker_ip);
	AXIS2_FREE(env->allocator, destination_info->queue_name);
	AXIS2_FREE(env->allocator, destination_info);

	if (!status)
	{
		return NULL;
	}

	/* Create a Copy and Return */
	axis2_amqp_binary_data_buffer_t* binary_data_buffer = (axis2_amqp_binary_data_buffer_t*)
		AXIS2_MALLOC(env->allocator, sizeof(axis2_amqp_binary_data_buffer_t));

	/* Data */
	binary_data_buffer->data = AXIS2_MALLOC(env->allocator, qpid_sender.responseContent.size());
	memcpy(binary_data_buffer->data, qpid_sender.responseContent.c_str(), 
			qpid_sender.responseContent.size());

	/* Length */
	binary_data_buffer->length = qpid_sender.responseContent.size();

	/* ContentType */
	binary_data_buffer->content_type = (axis2_char_t*)
		AXIS2_MALLOC(env->allocator, qpid_sender.responseContentType.size() + 1);
	strcpy(binary_data_buffer->content_type, qpid_sender.responseContentType.c_str());

	return binary_data_buffer;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_qpid_client_send_robust(
	const axis2_char_t* request_content,
	const axutil_env_t* env,
	const axis2_char_t* content_type,
	const axis2_char_t* soap_action,
	axis2_msg_ctx_t* msg_ctx)
{
	axis2_amqp_destination_info_t* destination_info = NULL;
	axis2_status_t status =  AXIS2_FAILURE;

	destination_info = axis2_amqp_util_msg_ctx_get_destination_info(msg_ctx, env);

	if (!destination_info || !destination_info->broker_ip || 
		!destination_info->broker_port || !destination_info->queue_name)
	{
		return AXIS2_FAILURE;
	}

	axis2_bool_t is_soap_11 = axis2_msg_ctx_get_is_soap_11(msg_ctx, env);
	axutil_array_list_t* mime_parts = axis2_msg_ctx_get_mime_parts(msg_ctx, env);
	
	Axis2QpidSender qpid_sender(destination_info->broker_ip, 
			destination_info->broker_port, env);

	status = qpid_sender.ClientSendRobust(request_content, destination_info->queue_name, 
			is_soap_11, content_type, soap_action, mime_parts);

	AXIS2_FREE(env->allocator, destination_info->broker_ip);
	AXIS2_FREE(env->allocator, destination_info->queue_name);
	AXIS2_FREE(env->allocator, destination_info);

	return status;
}
 

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_qpid_client_send_dual(
	const axis2_char_t* request_content,
	const axutil_env_t* env,
	const axis2_char_t* reply_to_queue_name,
	const axis2_char_t* content_type,
	const axis2_char_t* soap_action,
	axis2_msg_ctx_t* msg_ctx)
{
	axis2_amqp_destination_info_t* destination_info = NULL;
	axis2_status_t status =  AXIS2_FAILURE;

	destination_info = axis2_amqp_util_msg_ctx_get_destination_info(msg_ctx, env);

	if (!destination_info || !destination_info->broker_ip || 
		!destination_info->broker_port || !destination_info->queue_name)
	{
		return AXIS2_FAILURE;
	}

	axis2_bool_t is_soap_11 = axis2_msg_ctx_get_is_soap_11(msg_ctx, env);
	axutil_array_list_t* mime_parts = axis2_msg_ctx_get_mime_parts(msg_ctx, env);
	
	Axis2QpidSender qpid_sender(destination_info->broker_ip, 
			destination_info->broker_port, env);

	status = qpid_sender.ClientSendDual(request_content, destination_info->queue_name, 
			reply_to_queue_name, is_soap_11, content_type, soap_action, mime_parts);

	AXIS2_FREE(env->allocator, destination_info->broker_ip);
	AXIS2_FREE(env->allocator, destination_info->queue_name);
	AXIS2_FREE(env->allocator, destination_info);

	return status;
}
 

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_qpid_server_send(
	const axis2_char_t* request_content,
	const axutil_env_t* env,
	const axis2_char_t* content_type,
	const axis2_char_t* soap_action,
	axis2_msg_ctx_t* msg_ctx)
{
	axis2_amqp_destination_info_t* destination_info = NULL;
	axis2_status_t status =  AXIS2_FAILURE;

	destination_info = axis2_amqp_util_msg_ctx_get_destination_info(msg_ctx, env);

	if (!destination_info || !destination_info->broker_ip || 
		!destination_info->broker_port || !destination_info->queue_name)
	{
		return AXIS2_FAILURE;
	}

	axis2_bool_t is_soap_11 = axis2_msg_ctx_get_is_soap_11(msg_ctx, env);
	axutil_array_list_t* mime_parts = axis2_msg_ctx_get_mime_parts(msg_ctx, env);

	Axis2QpidSender qpid_sender(destination_info->broker_ip, 
			destination_info->broker_port, env);

	status = qpid_sender.ServerSend(request_content, destination_info->queue_name, 
			is_soap_11, content_type, soap_action, mime_parts);

	AXIS2_FREE(env->allocator, destination_info->broker_ip);
	AXIS2_FREE(env->allocator, destination_info->queue_name);
	AXIS2_FREE(env->allocator, destination_info);

	return status;
}

#ifdef __cplusplus
}
#endif
