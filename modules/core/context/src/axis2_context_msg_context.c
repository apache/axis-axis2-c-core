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
 
#include <axis2_context_msg_context.h>

/**
  * @struct axis2_context_msg_ctx_s
  * @brief CONTEXT msg_ctx
  * This holds the information about msg_ctx.
  */
struct axis2_context_msg_ctx_s
{
	axis2_context_msg_ctx_ops_t *ops;
};
	
/*************************** Function headers *********************************/

axis2_status_t axis2_context_msg_ctx_ops_free (axis2_environment_t *env
		, axis2_context_msg_ctx_t *msg_ctx);	


		
/************************* End of function headers ****************************/	

axis2_context_msg_ctx_t *axis2_context_msg_ctx_get_ops
		(axis2_environment_t *env, axis2_context_msg_ctx_t *msg_ctx)
{
	if(!msg_ctx)
	{
		env->error->errorno = AXIS2_ERROR_INVALID_NULL_PARAMETER;
		return NULL;	
	}
	return (axis2_context_msg_ctx_t *) msg_ctx->ops;
}

axis2_context_msg_ctx_t *axis2_context_msg_ctx_create 
		(axis2_environment_t *env)
{
	axis2_context_msg_ctx_ops_t *ops = NULL;
	axis2_context_msg_ctx_t *msg_ctx = 
		(axis2_context_msg_ctx_t *) axis2_malloc (env->allocator
		, sizeof (axis2_context_msg_ctx_t));
	if(!msg_ctx)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}
	ops = (axis2_context_msg_ctx_ops_t *) axis2_malloc(env->allocator,
		sizeof(axis2_context_msg_ctx_ops_t));
	if(!ops)
	{
		env->error->errorno = AXIS2_ERROR_NO_MEMORY;
		return NULL;
	}	
	ops->free = axis2_context_msg_ctx_ops_free;
	
	msg_ctx->ops = ops;
						
	return msg_ctx;
}

/******************************************************************************/

axis2_status_t axis2_context_msg_ctx_ops_free (axis2_environment_t *env
		, axis2_context_msg_ctx_t *msg_ctx)
{
	if(msg_ctx){
		free(msg_ctx);
		return AXIS2_SUCCESS;
	}
	return AXIS2_ERROR_UNALLOCATED_MEMEORY_RELEASE_REQUESTED;
}
