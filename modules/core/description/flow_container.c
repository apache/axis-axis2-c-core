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
 
#include <axis2_flow_container.h>

/** 
 * @brief Flow container struct impl
 *   Container for flows  
 */ 
typedef struct axis2_flow_container_impl
{
    axis2_flow_container_t flow_container;
    axis2_flow_t *in;
    axis2_flow_t *out;
    axis2_flow_t *in_fault;
    axis2_flow_t *out_fault;
    
} axis2_flow_container_impl_t;

#define AXIS2_INTF_TO_IMPL(flow_container) \
      ((axis2_flow_container_impl_t *)flow_container)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_flow_container_free(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env);

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_in_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_in_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *in_flow);

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_out_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_out_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *out_flow);

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_fault_in_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_in_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *falut_in_flow);

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_fault_out_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_out_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *fault_out_flow);


AXIS2_EXTERN axis2_flow_container_t *AXIS2_CALL
axis2_flow_container_create (
    const axis2_env_t *env)
{
    axis2_flow_container_impl_t *flow_container_impl = NULL;
    
   AXIS2_ENV_CHECK(env, NULL);
   
   flow_container_impl = (axis2_flow_container_impl_t *) AXIS2_MALLOC(env->
        allocator, sizeof(axis2_flow_container_impl_t));
   
   
   if(NULL == flow_container_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    flow_container_impl->flow_container.ops = NULL;
   flow_container_impl->in = NULL;
    flow_container_impl->out = NULL;
    flow_container_impl->in_fault = NULL;
    flow_container_impl->out_fault = NULL;      
    
   flow_container_impl->flow_container.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_flow_container_ops_t));
   if(NULL == flow_container_impl->flow_container.ops)
    {
        axis2_flow_container_free(&(flow_container_impl->flow_container), env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   flow_container_impl->flow_container.ops->free =  axis2_flow_container_free;
   flow_container_impl->flow_container.ops->get_in_flow = 
        axis2_flow_container_get_in_flow;
    flow_container_impl->flow_container.ops->set_in_flow = 
        axis2_flow_container_set_in_flow;
    flow_container_impl->flow_container.ops->get_out_flow = 
        axis2_flow_container_get_out_flow;
    flow_container_impl->flow_container.ops->set_out_flow = 
        axis2_flow_container_set_out_flow;
    flow_container_impl->flow_container.ops->get_fault_in_flow = 
        axis2_flow_container_get_fault_in_flow;
    flow_container_impl->flow_container.ops->set_fault_in_flow = 
        axis2_flow_container_set_fault_in_flow;
    flow_container_impl->flow_container.ops->get_fault_out_flow = 
        axis2_flow_container_get_fault_out_flow;
    flow_container_impl->flow_container.ops->set_fault_out_flow = 
        axis2_flow_container_set_fault_out_flow;
      
   return &(flow_container_impl->flow_container);
}

axis2_status_t AXIS2_CALL
axis2_flow_container_free(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);
    
    if(NULL != container_impl->in)
    {
        AXIS2_FLOW_FREE(container_impl->in, env);
        container_impl->in = NULL;
    }

    if(NULL != container_impl->out)
    {
        AXIS2_FLOW_FREE(container_impl->out, env);
        container_impl->out = NULL;
    }
    
    if(NULL != container_impl->in_fault)
    {
        AXIS2_FLOW_FREE(container_impl->in_fault, env);
        container_impl->in_fault = NULL;
    }
    
    if(NULL != container_impl->out_fault)
    {
        AXIS2_FLOW_FREE(container_impl->out_fault, env);
        container_impl->out_fault = NULL;
    }

    if(NULL != flow_container->ops)
        AXIS2_FREE(env->allocator, flow_container->ops);
    
    if(container_impl)
    { 
        AXIS2_FREE(env->allocator, container_impl);
        container_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_in_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->in;
}

axis2_status_t AXIS2_CALL
axis2_flow_container_set_in_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *in_flow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE); 
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);

    if(container_impl->in)
    {
        AXIS2_FLOW_FREE(container_impl->in, env);
        container_impl->in = NULL;
    }
    container_impl->in = in_flow;
    return AXIS2_SUCCESS;    
}

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_out_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->out;
}

axis2_status_t AXIS2_CALL
axis2_flow_container_set_out_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *out_flow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);
    if(container_impl->out)
    {
        AXIS2_FLOW_FREE(container_impl->out, env);
        container_impl->out = NULL;
    }
    container_impl->out = out_flow;
    return AXIS2_SUCCESS;
}

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_fault_in_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->in_fault;
}

axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_in_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *falut_in_flow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);
    if(container_impl->in_fault)
    {
        AXIS2_FLOW_FREE(container_impl->in_fault, env);
        container_impl->in_fault = NULL;
    }
    container_impl->in_fault = falut_in_flow;
    return AXIS2_SUCCESS;
}

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_fault_out_flow(
    const axis2_flow_container_t *flow_container,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->out_fault;
}

axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_out_flow(
    axis2_flow_container_t *flow_container,
    const axis2_env_t *env,
    axis2_flow_t *fault_out_flow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_out_flow, AXIS2_FAILURE);
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);
    if(container_impl->out_fault)
    {
        AXIS2_FLOW_FREE(container_impl->out_fault, env);
        container_impl->out_fault = NULL;
    }
    container_impl->out_fault = fault_out_flow;
    return AXIS2_SUCCESS;
}
