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
 
#include <axis2_msg_recv.h>

typedef struct axis2_msg_recv_impl_s axis2_msg_recv_impl_t;
/** 
 * @brief Message Receiver struct impl
 * Axis2 Message Receiver impl  
 */ 
struct axis2_msg_recv_impl_s
{
	axis2_msg_recv_t msg_recv;
};

#define AXIS2_INTF_TO_IMPL(msg_recv) ((axis2_msg_recv_impl_t *) msg_recv)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_msg_recv_free (axis2_msg_recv_t *msg_recv,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_msg_recv_receive (axis2_msg_recv_t *msg_recv,
                                axis2_env_t **env,
		                        struct axis2_msg_ctx_s *msg_ctx);
		
/************************* End of function headers ****************************/	

axis2_msg_recv_t * AXIS2_CALL
axis2_msg_recv_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
	axis2_msg_recv_ops_t *ops = NULL;
	axis2_msg_recv_impl_t *msg_recv_impl = 
        (axis2_msg_recv_impl_t *) AXIS2_MALLOC ((*env)->allocator
		    , sizeof (axis2_msg_recv_impl_t));
	if(NULL == msg_recv_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	ops = (axis2_msg_recv_ops_t *) AXIS2_MALLOC((*env)->allocator,
		sizeof(axis2_msg_recv_ops_t));
    
    msg_recv_impl->msg_recv.ops = (axis2_msg_recv_ops_t *) AXIS2_MALLOC(
        (*env)->allocator, sizeof(axis2_msg_recv_ops_t));
	if(NULL == msg_recv_impl->msg_recv.ops)
	{
        AXIS2_FREE((*env)->allocator, msg_recv_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
	msg_recv_impl->msg_recv.ops->free = axis2_msg_recv_free;
						
	return &(msg_recv_impl->msg_recv);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL
axis2_msg_recv_free (axis2_msg_recv_t *msg_recv,
                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(msg_recv, env, AXIS2_FAILURE);
	if(NULL != msg_recv->ops)
		AXIS2_FREE((*env)->allocator, msg_recv->ops);
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(msg_recv));
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_msg_receive (axis2_msg_recv_t *msg_recv,
                    axis2_env_t **env,
		            struct axis2_msg_ctx_s *msg_ctx)
{
	return AXIS2_SUCCESS;
}
