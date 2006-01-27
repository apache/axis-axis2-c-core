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
 
#include <axis2_stub.h>
#include <string.h>

/** 
 * @brief
 */
typedef struct axis2_stub_impl
{
	axis2_stub_t stub;
    
    axis2_call_t *call;
    	
} axis2_stub_impl_t;

#define AXIS2_INTF_TO_IMPL(stub) \
    ((axis2_stub_impl_t *) stub)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL 
axis2_stub_free (axis2_stub_t *stub, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_ref (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_endpoint_ref_t *endpoint_ref);

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_uri (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_char_t *endpoint_uri);


/************************** End of function prototypes ************************/

axis2_stub_t *AXIS2_CALL 
axis2_stub_create (axis2_env_t **env)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	stub_impl = (axis2_stub_impl_t *) AXIS2_MALLOC((*env)->
        allocator, sizeof(axis2_stub_impl_t));
	
	if(NULL == stub_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    stub_impl->call = NULL;
    stub_impl->stub.ops = NULL;
    
	stub_impl->stub.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_stub_ops_t));
	if(NULL == stub_impl->stub.ops)
    {
        axis2_stub_free(&(stub_impl->stub), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	stub_impl->stub.ops->free = axis2_stub_free;
    stub_impl->stub.ops->set_endpoint_ref = axis2_stub_set_endpoint_ref;
    stub_impl->stub.ops->set_endpoint_uri = axis2_stub_set_endpoint_uri;
	
	return &(stub_impl->stub);
}

axis2_stub_t * AXIS2_CALL 
axis2_stub_create_with_endpoint_ref (axis2_env_t **env,
                                        axis2_endpoint_ref_t *endpoint_ref)
{
    axis2_stub_impl_t *stub_impl = NULL;
   
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_ref, NULL);
    
    stub_impl = (axis2_stub_impl_t *) axis2_stub_create(env);
    if(!stub_impl)
    {
        axis2_stub_free(&(stub_impl->stub), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    /* create call without passing svc_ctx_t struct */
    stub_impl->call = axis2_call_create(env, NULL);
    if(!stub_impl->call)
    {
        axis2_stub_free(&(stub_impl->stub), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    AXIS2_CALL_SET_TO(stub_impl->call, env, endpoint_ref);
    
    return &(stub_impl->stub);
}

axis2_stub_t * AXIS2_CALL 
axis2_stub_create_with_endpoint_uri (axis2_env_t **env,
                                        axis2_char_t *endpoint_uri)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
   
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_uri, NULL);
 
    endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);
    if(!endpoint_ref)
    {
        return NULL;
    }
    stub_impl = (axis2_stub_impl_t *) axis2_stub_create_with_endpoint_ref(env,
        endpoint_ref);
    
    if(!stub_impl)
    {
        axis2_stub_free(&(stub_impl->stub), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;  
    }
    
    return &(stub_impl->stub);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_stub_free (axis2_stub_t *stub, 
                            axis2_env_t **env)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(stub, env, AXIS2_FAILURE);
    
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
	if(NULL != stub->ops)
    {
        AXIS2_FREE((*env)->allocator, stub->ops);
        stub->ops = NULL;
    }

    if(NULL != stub_impl->call)
    {
        AXIS2_CALL_FREE(stub_impl->call, env);
        stub_impl->call = NULL;
    }
    
    if(stub_impl)
    {
        AXIS2_FREE((*env)->allocator, stub_impl);
        stub_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_ref (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_endpoint_ref_t *endpoint_ref)
{
    axis2_stub_impl_t *stub_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(stub,env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_ref, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    return AXIS2_CALL_SET_TO(stub_impl->call, env, endpoint_ref);
}

axis2_status_t AXIS2_CALL
axis2_stub_set_endpoint_uri (axis2_stub_t *stub,
                                axis2_env_t **env,
                                axis2_char_t *endpoint_uri)
{
    axis2_stub_impl_t *stub_impl = NULL;
    axis2_endpoint_ref_t *endpoint_ref = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(stub,env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, endpoint_uri, AXIS2_FAILURE);
    stub_impl = AXIS2_INTF_TO_IMPL(stub);
    
    endpoint_ref = axis2_endpoint_ref_create(env, endpoint_uri);
    if(!endpoint_ref)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_CALL_SET_TO(stub_impl->call, env, endpoint_ref);
}
