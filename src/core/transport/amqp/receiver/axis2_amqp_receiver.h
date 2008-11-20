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

#ifndef AXIS2_AMQP_RECEIVER_H
#define AXIS2_AMQP_RECEIVER_H

#include <axis2_transport_receiver.h>
#include <axis2_conf_init.h>
#include <axis2_qpid_receiver_interface.h>

typedef struct axis2_amqp_receiver_resource_pack
{
    axis2_transport_receiver_t  		 receiver;
	axis2_qpid_receiver_resource_pack_t* qpid_receiver;
    axis2_conf_ctx_t*           		 conf_ctx;
    axis2_conf_ctx_t*           		 conf_ctx_private;
}
axis2_amqp_receiver_resource_pack_t;

#define AXIS2_AMQP_RECEIVER_TO_RESOURCE_PACK(amqp_receiver) \
                ((axis2_amqp_receiver_resource_pack_t*)(amqp_receiver))

AXIS2_EXTERN axis2_transport_receiver_t* AXIS2_CALL
axis2_amqp_receiver_create(
	const axutil_env_t* env,
	const axis2_char_t* repo,
	const axis2_char_t* qpid_broker_ip,
	int qpid_broker_port);

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_init(
	axis2_transport_receiver_t* receiver,
	const axutil_env_t* env,
	axis2_conf_ctx_t* conf_ctx,
	axis2_transport_in_desc_t* in_desc);

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_start(
	axis2_transport_receiver_t* receiver,
	const axutil_env_t* env);

AXIS2_EXTERN axis2_endpoint_ref_t* AXIS2_CALL 
axis2_amqp_receiver_get_reply_to_epr(
	axis2_transport_receiver_t* receiver,
	const axutil_env_t* env,
	const axis2_char_t* svc_name);

AXIS2_EXTERN axis2_conf_ctx_t* AXIS2_CALL 
axis2_amqp_receiver_get_conf_ctx(
	axis2_transport_receiver_t* receiver,
	const axutil_env_t* env);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
axis2_amqp_receiver_is_running(
	axis2_transport_receiver_t* receiver,
	const axutil_env_t* env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL 
axis2_amqp_receiver_stop(
	axis2_transport_receiver_t* receiver,
	const axutil_env_t* env);

AXIS2_EXTERN void AXIS2_CALL 
axis2_amqp_receiver_free(
	axis2_transport_receiver_t* receiver,
	const axutil_env_t* env);

#endif
