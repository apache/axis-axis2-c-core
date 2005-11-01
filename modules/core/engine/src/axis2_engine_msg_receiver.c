/*
 * Copyright 2004,2005 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#include <axis2_engine_msg_receiver.h>

/**
  * @struct axis2_engine_msg_receiver_s
  * @brief ENGINE msg_receiver
  * This holds the information about msg_receiver.
  */
struct axis2_engine_msg_receiver_s
{
	axis2_engine_msg_receiver_ops_t *ops;
};
	
/*************************** Function headers *********************************/

axis2_status_t axis2_engine_msg_receiver_ops_free (axis2_env_t *env
		, axis2_engine_msg_receiver_t *msg_receiver);	

axis2_status_t axis2_engine_msg_receiver_ops_receive (axis2_env_t *env
		, axis2_engine_msg_receiver_t *msg_receiver
		, axis2_context_msg_ctx_t *msg_ctx);
		
/************************* End of function headers ****************************/	

axis2_engine_msg_receiver_t *axis2_engine_msg_receiver_get_ops
		(axis2_env_t *env, axis2_engine_msg_receiver_t *msg_receiver)
{
	if(!msg_receiver)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;	
	}
	return (axis2_engine_msg_receiver_t *) msg_receiver->ops;
}

axis2_engine_msg_receiver_t *axis2_engine_msg_receiver_create 
		(axis2_env_t *env)
{
	axis2_engine_msg_receiver_ops_t *ops = NULL;
	axis2_engine_msg_receiver_t *msg_receiver = 
		(axis2_engine_msg_receiver_t *) axis2_malloc (env->allocator
		, sizeof (axis2_engine_msg_receiver_t));
	if(!msg_receiver)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	ops = (axis2_engine_msg_receiver_ops_t *) axis2_malloc(env->allocator,
		sizeof(axis2_engine_msg_receiver_ops_t));
	if(!ops)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}	
	ops->free = axis2_engine_msg_receiver_ops_free;
	
	msg_receiver->ops = ops;
						
	return msg_receiver;
}

/******************************************************************************/

axis2_status_t axis2_engine_msg_receiver_ops_free (axis2_env_t *env
		, axis2_engine_msg_receiver_t *msg_receiver)
{
	if(msg_receiver){
		free(msg_receiver);
		return AXIS2_SUCCESS;
	}
	return AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED;
}

axis2_status_t axis2_engine_msg_receive(axis2_env_t *env
		, axis2_engine_msg_receiver_t *msg_receiver
		, axis2_context_msg_ctx_t *msg_ctx)
{
	return AXIS2_SUCCESS;
}
