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

#include <axis2_qpid_receiver.h>
#include <axis2_qpid_receiver_interface.h>

#ifdef __cplusplus
extern "C"
{
#endif

AXIS2_EXTERN axis2_qpid_receiver_resource_pack_t* AXIS2_CALL
axis2_qpid_receiver_create(
	const axutil_env_t* env,
	axis2_conf_ctx_t* conf_ctx)
{
	AXIS2_ENV_CHECK(env, NULL);
	
	axis2_qpid_receiver_resource_pack_t* resource_pack = NULL;

	resource_pack = (axis2_qpid_receiver_resource_pack_t*)AXIS2_MALLOC
		(env->allocator, sizeof(axis2_qpid_receiver_resource_pack_t));

	if (!resource_pack)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return NULL;
	}

	/* Create Qpid Receiver */
	Axis2QpidReceiver* qpid_receiver = new Axis2QpidReceiver(env, conf_ctx);
	
	resource_pack->qpid_receiver = qpid_receiver;

	return resource_pack;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_qpid_receiver_start(
	axis2_qpid_receiver_resource_pack_t* receiver_resource_pack,
	const axutil_env_t* env)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	axis2_status_t status = AXIS2_FAILURE;

	/* Start Qpid Receiver */
	Axis2QpidReceiver* qpid_receiver = (Axis2QpidReceiver*)receiver_resource_pack->qpid_receiver;

	if ((qpid_receiver) && (qpid_receiver->start()))
	{
		status = AXIS2_SUCCESS;
	}

	return status;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_qpid_receiver_is_running(
	axis2_qpid_receiver_resource_pack_t* receiver_resource_pack,
	const axutil_env_t* env)
{
	return AXIS2_TRUE;
}


AXIS2_EXTERN void AXIS2_CALL
axis2_qpid_receiver_free(
	axis2_qpid_receiver_resource_pack_t* receiver_resource_pack,
	const axutil_env_t* env)
{
	AXIS2_ENV_CHECK(env, void);
	
	if (receiver_resource_pack)
	{
		Axis2QpidReceiver* qpid_receiver = (Axis2QpidReceiver*)receiver_resource_pack->qpid_receiver;
		if (qpid_receiver)
			delete qpid_receiver;
		
		AXIS2_FREE(env->allocator, receiver_resource_pack);
	}
}

#ifdef __cplusplus
}
#endif
