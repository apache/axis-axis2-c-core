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

#ifndef AXIS2_AMQP_SENDER_H
#define AXIS2_AMQP_SENDER_H

#include <axutil_env.h>
#include <axis2_conf_ctx.h>
#include <axis2_transport_sender.h>
#include <axis2_qpid_sender_interface.h>

typedef struct axis2_amqp_sender_resource_pack
{
	axis2_transport_sender_t sender;
	axis2_conf_ctx_t* conf_ctx;
}
axis2_amqp_sender_resource_pack_t;

#define AXIS2_AMQP_SENDER_TO_RESOURCE_PACK(amqp_sender) \
	((axis2_amqp_sender_resource_pack_t*)(amqp_sender))

AXIS2_EXTERN axis2_transport_sender_t* AXIS2_CALL
axis2_amqp_sender_create(const axutil_env_t* env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_sender_init(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env,
	axis2_conf_ctx_t* conf_ctx,
	axis2_transport_out_desc_t* out_desc);

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_sender_invoke(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env,
	axis2_msg_ctx_t* msg_ctx);

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_sender_clean_up(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env,
	axis2_msg_ctx_t* msg_ctx);

AXIS2_EXTERN void AXIS2_CALL 
axis2_amqp_sender_free(
	axis2_transport_sender_t* sender,
	const axutil_env_t* env);

#endif
