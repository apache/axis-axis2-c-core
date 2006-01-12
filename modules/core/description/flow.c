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
 
#include <axis2_flow.h>

/** 
 * @brief Flow struct impl
 *	Axis2 Flow impl  
 */
typedef struct axis2_flow_impl
{
	axis2_flow_t flow;
    /**
     * Field list
     */
    axis2_array_list_t *list;
    
} axis2_flow_impl_t;

#define AXIS2_INTF_TO_IMPL(flow) ((axis2_flow_impl_t *)(flow))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_flow_free (axis2_flow_t *flow, axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_flow_add_handler (axis2_flow_t *flow,
                        axis2_env_t **env,
                        axis2_handler_desc_t *handler);
axis2_handler_desc_t * AXIS2_CALL
axis2_flow_get_handler (axis2_flow_t *flow,
                        axis2_env_t **env,
                        int index);

int AXIS2_CALL
axis2_flow_get_handler_count(axis2_flow_t *flow,
                                axis2_env_t **env);

/************************** End of Function headers ************************/

axis2_flow_t * AXIS2_CALL 
axis2_flow_create (axis2_env_t **env)
{
    axis2_flow_impl_t *flow_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	flow_impl = (axis2_flow_impl_t *) AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_flow_impl_t));
		
	if(NULL == flow_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
	
    flow_impl->list = NULL;
    flow_impl->flow.ops = NULL;
    
    /*Create the list with the default size of 16 */
	flow_impl->list = axis2_array_list_create (env, 20);
    if(NULL == flow_impl->list)
    {
        axis2_flow_free(&(flow_impl->flow), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }       
    
	flow_impl->flow.ops = AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_flow_ops_t));
	if(NULL == flow_impl->flow.ops)
    {
        axis2_flow_free(&(flow_impl->flow), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
	flow_impl->flow.ops->free =  axis2_flow_free;
    flow_impl->flow.ops->add_handler =  axis2_flow_add_handler;
    flow_impl->flow.ops->get_handler =  axis2_flow_get_handler;
    flow_impl->flow.ops->get_handler_count =  axis2_flow_get_handler_count;
    	
	return &(flow_impl->flow);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_flow_free (axis2_flow_t *flow, axis2_env_t **env)
{
    axis2_flow_impl_t *flow_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(flow, env, AXIS2_FAILURE);
    flow_impl = AXIS2_INTF_TO_IMPL(flow);
    
    if(NULL != flow->ops)
    {
        AXIS2_FREE((*env)->allocator, flow->ops);
        flow->ops = NULL;
    }
    
    if(NULL != flow_impl->list)
    {
        AXIS2_ARRAY_LIST_FREE(flow_impl->list, env);
        flow_impl->list = NULL;
    }
    
    if(flow_impl)
    {
        AXIS2_FREE((*env)->allocator, flow_impl);
        flow_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_flow_add_handler (axis2_flow_t *flow,
                        axis2_env_t **env,
                        axis2_handler_desc_t *handler)
{
    axis2_flow_impl_t *flow_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(flow, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, handler, AXIS2_FAILURE);
  
    flow_impl = AXIS2_INTF_TO_IMPL(flow);
    
    if(!flow_impl->list)
    {
        flow_impl->list = axis2_array_list_create (env, 0);
        if(NULL == flow_impl->list)
        {
            axis2_flow_free(&(flow_impl->flow), env);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return AXIS2_FAILURE;
        } 
    }
    return AXIS2_ARRAY_LIST_ADD(AXIS2_INTF_TO_IMPL(flow)->list, env, handler);
}

axis2_handler_desc_t * AXIS2_CALL
axis2_flow_get_handler (axis2_flow_t *flow,
                        axis2_env_t **env,
                        int index)
{
    AXIS2_FUNC_PARAM_CHECK(flow, env, NULL);
    
    return AXIS2_ARRAY_LIST_GET(AXIS2_INTF_TO_IMPL(flow)->list, env, index);
}

int AXIS2_CALL
axis2_flow_get_handler_count(axis2_flow_t *flow,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(flow, env, AXIS2_FAILURE);
    
    return AXIS2_ARRAY_LIST_SIZE(AXIS2_INTF_TO_IMPL(flow)->list, env);
}
