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
 
#include <axis2_msg_ctx.h>

typedef axis2_msg_ctx_impl_s axis2_msg_ctx_impl_t;

/** 
 * @brief Message Context struct impl
 * Axis2 Message Context impl  
 */  
struct axis2_msg_ctx_impl_s
{
	axis2_msg_ctx_t msg_ctx;
};

#define AXIS2_INTF_TO_IMPL(msg_ctx) ((axis2_msg_ctx_impl_t *)msg_ctx)

/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_msg_ctx_free (axis2_msg_ctx_t *msg_ctx,
                    axis2_env_t **env);	


		
/************************* End of function headers ****************************/	

axis2_msg_ctx_t * AXIS2_CALL
axis2_msg_ctx_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
	axis2_context_msg_ctx_ops_t *ops = NULL;
    
	axis2_msg_ctx_impl_t *msg_ctx_impl = (axis2_msg_ctx_impl_t *) AXIS2_MALLOC (
        env->allocator, sizeof (axis2_msg_ctx_impl_t));
    
	if(NULL == msg_ctx_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	
	msg_ctx_impl->msg_ctx.ops = (axis2_msg_ctx_ops_t *) AXIS2_MALLOC(env->allocator,
		sizeof(axis2_msg_ctx_ops_t));
	if(NULL == msg_ctx_impl->msg_ctx.ops)
	{
        AXIS2_FREE((*env)->allocato, msg_ctx_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	msg_ctx_impl->msg_ctx.ops->free = axis2_msg_ctx_free;
						
	return &(msg_ctx_impl->msg_ctx);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_msg_ctx_free (axis2_msg_ctx_t *msg_ctx,
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_ctx, env, AXIS2_FAILURE);
    if(NULL == msg_ctx->ops)
        AXIS2_FREE((*env)->allocator, msg_ctx->ops);
    
    AXIS2_FREE((env*)->allocator, AXIS2_INTF_TO_IMPL(msg_ctx));
    
    return AXIS2_SUCCESS;
}
