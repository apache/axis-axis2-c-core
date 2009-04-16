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

#ifndef AXIS2_QPID_RECEIVER_INTERFACE_H
#define AXIS2_QPID_RECEIVER_INTERFACE_H

#include <axis2_util.h>
#include <axis2_conf_init.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct axis2_qpid_receiver_resource_pack
	{
		void* qpid_receiver;
	}axis2_qpid_receiver_resource_pack_t;

	AXIS2_EXTERN axis2_qpid_receiver_resource_pack_t* AXIS2_CALL
	axis2_qpid_receiver_create(
		const axutil_env_t* env,
		axis2_conf_ctx_t* conf_ctx);

	AXIS2_EXTERN axis2_status_t AXIS2_CALL
		axis2_qpid_receiver_start(
		axis2_qpid_receiver_resource_pack_t* receiver_resource_pack,
		const axutil_env_t* env);

	AXIS2_EXTERN axis2_bool_t AXIS2_CALL
		axis2_qpid_receiver_is_running(
		axis2_qpid_receiver_resource_pack_t* receiver_resource_pack,
		const axutil_env_t* env);

	AXIS2_EXTERN void AXIS2_CALL
		axis2_qpid_receiver_free(
		axis2_qpid_receiver_resource_pack_t* receiver_resource_pack,
		const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif
