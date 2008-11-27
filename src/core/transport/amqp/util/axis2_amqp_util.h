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

#ifndef AXIS2_AMQP_UTIL_H
#define AXIS2_AMQP_UTIL_H

#include <axis2_transport_in_desc.h>
#include <axutil_param_container.h>
#include <axiom_soap.h>
#include <axis2_conf_ctx.h>
#include <axutil_stream.h>
#include <axis2_amqp_defines.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct axis2_amqp_response
	{
		void* data;
		int length;
		axis2_char_t* content_type;
	} axis2_amqp_response_t;

	typedef struct axis2_amqp_destination_info
	{
		axis2_char_t* broker_ip;
		int broker_port;
		axis2_char_t* queue_name;
	} axis2_amqp_destination_info_t;

	AXIS2_EXTERN axis2_char_t* AXIS2_CALL
	axis2_amqp_util_get_in_desc_conf_value_string(
		axis2_transport_in_desc_t* in_desc,
		const axutil_env_t* env,
		const axis2_char_t* param_name);

	AXIS2_EXTERN int AXIS2_CALL
	axis2_amqp_util_get_in_desc_conf_value_int(
		axis2_transport_in_desc_t* in_desc,
		const axutil_env_t* env,
		const axis2_char_t* param_name);

	AXIS2_EXTERN axis2_char_t* AXIS2_CALL
	axis2_amqp_util_get_out_desc_conf_value_string(
		axis2_transport_out_desc_t* out_desc,
		const axutil_env_t* env,
		const axis2_char_t* param_name);

	AXIS2_EXTERN int AXIS2_CALL
	axis2_amqp_util_get_out_desc_conf_value_int(
		axis2_transport_out_desc_t* out_desc,
		const axutil_env_t* env,
		const axis2_char_t* param_name);

	AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
	axis2_amqp_util_get_soap_envelope(
		axis2_amqp_response_t* response,
		const axutil_env_t* env,
		axis2_msg_ctx_t* msg_ctx);

	AXIS2_EXTERN axis2_bool_t AXIS2_CALL
	axis2_amqp_util_conf_ctx_get_server_side(
		axis2_conf_ctx_t* conf_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN axis2_char_t* AXIS2_CALL
	axis2_amqp_util_get_value_from_content_type(
		const axutil_env_t * env,
		const axis2_char_t * content_type,
		const axis2_char_t * key);

	AXIS2_EXTERN int AXIS2_CALL 
	axis2_amqp_util_on_data_request(
		char *buffer,
		int size,
		void *ctx);

	AXIS2_EXTERN axis2_char_t* AXIS2_CALL
	axis2_amqp_util_conf_ctx_get_dual_channel_queue_name(
		axis2_conf_ctx_t* conf_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN axis2_char_t* AXIS2_CALL
	axis2_amqp_util_conf_ctx_get_qpid_broker_ip(
		axis2_conf_ctx_t* conf_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN int AXIS2_CALL
	axis2_amqp_util_conf_ctx_get_qpid_broker_port(
		axis2_conf_ctx_t* conf_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN axis2_bool_t AXIS2_CALL
	axis2_amqp_util_msg_ctx_get_use_separate_listener(
		axis2_msg_ctx_t* msg_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN axis2_amqp_destination_info_t* AXIS2_CALL 
	axis2_amqp_util_msg_ctx_get_destination_info(
		axis2_msg_ctx_t* msg_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN int AXIS2_CALL 
	axis2_amqp_util_msg_ctx_get_request_timeout(
		axis2_msg_ctx_t* msg_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
	axis2_amqp_util_msg_ctx_get_server_side(
		axis2_msg_ctx_t* msg_ctx,
		const axutil_env_t* env);

	AXIS2_EXTERN void AXIS2_CALL 
	axis2_amqp_response_free(
		axis2_amqp_response_t* response,
		const axutil_env_t* env);

	AXIS2_EXTERN void AXIS2_CALL 
	axis2_amqp_destination_info_free(
		axis2_amqp_destination_info_t* destination_info,
		const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif
