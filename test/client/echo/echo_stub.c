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
 
#include "echo_stub.h"
#include <axis2_om_node.h>

/************************* Function prototypes ********************************/
axis2_status_t AXIS2_CALL
axis2_echo_stub_invoke_blocking(axis2_stub_t *echo_stub,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL
axis2_echo_stub_echo(axis2_stub_t *echo_stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node);

/************************** End of function prototypes ************************/

axis2_stub_t *AXIS2_CALL 
axis2_echo_stub_create_with_endpoint_ref(axis2_env_t **env,
                                            axis2_endpoint_ref_t *endpoint_ref)
{
    axis2_stub_t *stub = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	stub = (axis2_stub_t *) axis2_stub_create_with_endpoint_ref(env, endpoint_ref);
	if(NULL == stub)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    stub->ops->invoke_blocking = axis2_echo_stub_invoke_blocking;
	
	return stub;
}

axis2_stub_t *AXIS2_CALL 
axis2_echo_stub_create_with_endpoint_uri(axis2_env_t **env,
                                            axis2_char_t *endpoint_uri)
{
    axis2_stub_t *stub = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	stub = (axis2_stub_t *) axis2_stub_create_with_endpoint_uri(env, endpoint_uri);
	if(NULL == stub)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    stub->ops->invoke_blocking = axis2_echo_stub_invoke_blocking;
	
	return stub;
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL
axis2_echo_stub_invoke_blocking(axis2_stub_t *echo_stub,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx)
{
    return 0;
}

axis2_status_t AXIS2_CALL
axis2_echo_stub_echo(axis2_stub_t *echo_stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node)
{
    return 0;
}

