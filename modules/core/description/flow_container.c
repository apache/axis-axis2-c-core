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
    /**
     * Field in
     */
    axis2_flow_t *in;
    /**
     * Field out
     */
    axis2_flow_t *out;
    /**
     * Field fault
     */
    axis2_flow_t *in_fault;

    axis2_flow_t *out_fault;
    
} axis2_flow_container_impl_t;

#define AXIS2_INTF_TO_IMPL(flow_container) \
      ((axis2_flow_container_impl_t *)flow_container)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_flow_container_free(axis2_flow_container_t *flow_container,
                            const axis2_env_t *env);

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_inflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_inflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env,
                                    axis2_flow_t *inflow);

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_outflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_outflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env,
                                    axis2_flow_t *outflow);

axis2_flow_t * AXIS2_CALL
axis2_flow_container_get_fault_inflow(axis2_flow_container_t *flow_container,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_inflow(axis2_flow_container_t *flow_container,
                                        const axis2_env_t *env,
                                        axis2_flow_t *falut_inflow);

axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_fault_outflow(axis2_flow_container_t *flow_container,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_outflow(axis2_flow_container_t *flow_container,
                                            const axis2_env_t *env,
                                            axis2_flow_t *fault_outflow);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_flow_container_t * AXIS2_CALL
axis2_flow_container_create (const axis2_env_t *env)
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
   flow_container_impl->flow_container.ops->get_inflow = 
        axis2_flow_container_get_inflow;
    flow_container_impl->flow_container.ops->set_inflow = 
        axis2_flow_container_set_inflow;
    flow_container_impl->flow_container.ops->get_outflow = 
        axis2_flow_container_get_outflow;
    flow_container_impl->flow_container.ops->set_outflow = 
        axis2_flow_container_set_outflow;
    flow_container_impl->flow_container.ops->get_fault_inflow = 
        axis2_flow_container_get_fault_inflow;
    flow_container_impl->flow_container.ops->set_fault_inflow = 
        axis2_flow_container_set_fault_inflow;
    flow_container_impl->flow_container.ops->get_fault_outflow = 
        axis2_flow_container_get_fault_outflow;
    flow_container_impl->flow_container.ops->set_fault_outflow = 
        axis2_flow_container_set_fault_outflow;
      
   return &(flow_container_impl->flow_container);
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL
axis2_flow_container_free(axis2_flow_container_t *flow_container,
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

/**
 * Get fault out flow
 * @return in flow
 */
axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_inflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->in;
}

/**
 * Set in flow
 * @param inflow in flow
 */
axis2_status_t AXIS2_CALL
axis2_flow_container_set_inflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env,
                                    axis2_flow_t *inflow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE); 
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);

    if(container_impl->in)
    {
        AXIS2_FLOW_FREE(container_impl->in, env);
        container_impl->in = NULL;
    }
    container_impl->in = inflow;
    return AXIS2_SUCCESS;    
}

/**
 * Get out flow
 * @return out flow
 */
axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_outflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->out;
}

/**
 * Set out flow
 * @param outflow out flow
 */
axis2_status_t AXIS2_CALL
axis2_flow_container_set_outflow(axis2_flow_container_t *flow_container,
                                    const axis2_env_t *env,
                                    axis2_flow_t *outflow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);
    if(container_impl->out)
    {
        AXIS2_FLOW_FREE(container_impl->out, env);
        container_impl->out = NULL;
    }
    container_impl->out = outflow;
    return AXIS2_SUCCESS;
}

/**
 * Get fault in flow
 * @return fault in flow
 */
axis2_flow_t * AXIS2_CALL
axis2_flow_container_get_fault_inflow(axis2_flow_container_t *flow_container,
                                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->in_fault;
}

/**
 * set fault in flow
 * @param falut_inflow falut in flow
 */
axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_inflow(axis2_flow_container_t *flow_container,
                                        const axis2_env_t *env,
                                        axis2_flow_t *falut_inflow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);
    if(container_impl->in_fault)
    {
        AXIS2_FLOW_FREE(container_impl->in_fault, env);
        container_impl->in_fault = NULL;
    }
    container_impl->in_fault = falut_inflow;
    return AXIS2_SUCCESS;
}

/**
 * Get fault out flow
 * @return fault out flow
 */
axis2_flow_t *AXIS2_CALL
axis2_flow_container_get_fault_outflow(axis2_flow_container_t *flow_container,
                                            const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(flow_container)->out_fault;
}

/**
 * Set fault out flow
 * @param fault_outflow fault out flow
 */
axis2_status_t AXIS2_CALL
axis2_flow_container_set_fault_outflow(axis2_flow_container_t *flow_container,
                                            const axis2_env_t *env,
                                            axis2_flow_t *fault_outflow)
{
    axis2_flow_container_impl_t *container_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, fault_outflow, AXIS2_FAILURE);
    container_impl = AXIS2_INTF_TO_IMPL(flow_container);
    if(container_impl->out_fault)
    {
        AXIS2_FLOW_FREE(container_impl->out_fault, env);
        container_impl->out_fault = NULL;
    }
    container_impl->out_fault = fault_outflow;
    return AXIS2_SUCCESS;
}
