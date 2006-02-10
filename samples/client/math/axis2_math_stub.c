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
 
#include "axis2_math_stub.h"
#include <axis2_om_node.h>
#include <axis2_msg_ctx.h>
#include <axis2_call.h>


axis2_stub_t * 
axis2_math_stub_create_with_endpoint_ref_and_client_home(axis2_env_t **env,
                                            axis2_endpoint_ref_t *endpoint_ref,
                                            axis2_char_t *client_home)
{
    axis2_stub_t *stub = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	stub = (axis2_stub_t *) 
        axis2_stub_create_with_endpoint_ref_and_client_home(env, endpoint_ref,
            client_home);
	if(NULL == stub)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
	return stub;
}

axis2_stub_t *
axis2_math_stub_create_with_endpoint_uri_and_client_home(axis2_env_t **env,
                                            axis2_char_t *endpoint_uri,
                                            axis2_char_t *client_home)
{
    axis2_stub_t *stub = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	stub = (axis2_stub_t *) 
        axis2_stub_create_with_endpoint_uri_and_client_home(env, endpoint_uri,
            client_home);
	if(NULL == stub)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    
	return stub;
}

/***************************Function implementation****************************/

axis2_om_node_t *
axis2_math_stub_add(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node)
{
    axis2_call_t *call = NULL;
    axis2_om_node_t *ret_node = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    call = AXIS2_STUB_GET_CALL_OBJ(stub, env);
    ret_node = AXIS2_CALL_INVOKE_BLOCKING_WITH_OM(call, env, "add", node);
    
    return ret_node;
}

axis2_om_node_t *
axis2_math_stub_sub(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node)
{
    axis2_call_t *call = NULL;
    axis2_om_node_t *ret_node = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    call = AXIS2_STUB_GET_CALL_OBJ(stub, env);
    ret_node = AXIS2_CALL_INVOKE_BLOCKING_WITH_OM(call, env, "sub", node);
    
    return ret_node;
}

axis2_om_node_t *
axis2_math_stub_mul(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node)
{
    axis2_call_t *call = NULL;
    axis2_om_node_t *ret_node = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    call = AXIS2_STUB_GET_CALL_OBJ(stub, env);
    ret_node = AXIS2_CALL_INVOKE_BLOCKING_WITH_OM(call, env, "mul", node);
    
    return ret_node;
}

axis2_om_node_t *
axis2_math_stub_div(axis2_stub_t *stub,
                        axis2_env_t **env,
                        axis2_om_node_t *node)
{
    axis2_call_t *call = NULL;
    axis2_om_node_t *ret_node = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    call = AXIS2_STUB_GET_CALL_OBJ(stub, env);
    ret_node = AXIS2_CALL_INVOKE_BLOCKING_WITH_OM(call, env, "div", node);
    
    return ret_node;
}

