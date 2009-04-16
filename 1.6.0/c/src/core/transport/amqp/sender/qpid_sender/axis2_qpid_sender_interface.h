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

#ifndef AXIS2_QPID_SENDER_INTERFACE_H
#define AXIS2_QPID_SENDER_INTERFACE_H

#include <axis2_util.h>
#include <axis2_conf_init.h>
#include <axis2_amqp_util.h>

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
	axis2_msg_ctx_t* msg_ctx);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_qpid_send(
	const axis2_char_t* request_content,
	const axutil_env_t* env,
	const axis2_char_t* content_type,
	const axis2_char_t* soap_action,
	axis2_msg_ctx_t* msg_ctx);

#ifdef __cplusplus
}
#endif

#endif
