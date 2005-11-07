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
 
#include <axis2_engine_msg_recv.h>

/**
  * @struct axis2_engine_msg_recv_s
  * @brief ENGINE msg_recv
  * This holds the information about msg_recv.
  */
struct axis2_engine_msg_recv_s
{
	axis2_engine_msg_recv_ops_t *ops;
};
	
/*************************** Function headers *********************************/

axis2_status_t axis2_engine_msg_recv_ops_free (axis2_env_t *env
		, axis2_engine_msg_recv_t *msg_recv);	

axis2_status_t axis2_engine_msg_recv_ops_receive (axis2_env_t *env
		, axis2_engine_msg_recv_t *msg_recv
		, axis2_context_msg_ctx_t *msg_ctx);
		
/************************* End of function headers ****************************/	

axis2_engine_msg_recv_t *axis2_engine_msg_recv_get_ops
		(axis2_env_t *env, axis2_engine_msg_recv_t *msg_recv)
{
	if(!msg_recv)
	{
		env->error->error_number = AXIS2_ERROR_INVALID_NULL_PARAM;
		return NULL;	
	}
	return (axis2_engine_msg_recv_t *) msg_recv->ops;
}

axis2_engine_msg_recv_t *axis2_engine_msg_recv_create 
		(axis2_env_t *env)
{
	axis2_engine_msg_recv_ops_t *ops = NULL;
	axis2_engine_msg_recv_t *msg_recv = 
		(axis2_engine_msg_recv_t *) AXIS2_MALLOC (env->allocator
		, sizeof (axis2_engine_msg_recv_t));
	if(!msg_recv)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	ops = (axis2_engine_msg_recv_ops_t *) AXIS2_MALLOC(env->allocator,
		sizeof(axis2_engine_msg_recv_ops_t));
	if(!ops)
	{
		env->error->error_number = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}	
	ops->free = axis2_engine_msg_recv_ops_free;
	
	msg_recv->ops = ops;
						
	return msg_recv;
}

/******************************************************************************/

axis2_status_t axis2_engine_msg_recv_ops_free (axis2_env_t *env
		, axis2_engine_msg_recv_t *msg_recv)
{
	if(msg_recv){
		free(msg_recv);
		return AXIS2_SUCCESS;
	}
	return AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED;
}

axis2_status_t axis2_engine_msg_receive(axis2_env_t *env
		, axis2_engine_msg_recv_t *msg_recv
		, axis2_context_msg_ctx_t *msg_ctx)
{
	return AXIS2_SUCCESS;
}
