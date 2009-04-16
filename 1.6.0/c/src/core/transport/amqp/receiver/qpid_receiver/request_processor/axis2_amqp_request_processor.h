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

#ifndef AXIS2_AMQP_REQUEST_PROCESSOR_H
#define AXIS2_AMQP_REQUEST_PROCESSOR_H

#include <axutil_env.h>
#include <axis2_conf_init.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct axis2_amqp_request_processor_resource_pack
	{
		axutil_env_t* env;
		axis2_conf_ctx_t* conf_ctx;
		axis2_char_t* request_content;
		int content_length;
		axis2_char_t* reply_to;
		axis2_char_t* content_type;
		axis2_char_t* soap_action;
	} axis2_amqp_request_processor_resource_pack_t;

	/* The worker thread function */
	void* AXIS2_THREAD_FUNC
	axis2_amqp_request_processor_thread_function(
		axutil_thread_t* thread,
		void* request_data);

	axis2_status_t
	axis2_amqp_process_request(
		const axutil_env_t* env,
		axis2_amqp_request_processor_resource_pack_t* request_resource_pack);

#ifdef __cplusplus
}
#endif

#endif
