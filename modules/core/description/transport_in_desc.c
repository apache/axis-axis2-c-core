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
 
#include <axis2_transport_in_desc.h>
#include <axis2_transport_receiver.h>

/** 
 * @brief Description transport in struct impl
 * Transport in
 * Represents a incoming transport deployed in AXis2
 */ 
typedef struct axis2_transport_in_desc_impl
{
   axis2_transport_in_desc_t transport_in;    
    /** 
     * Field flowInclude 
     * This will have a shallow copy and will not be freed by the descructor
     */
    axis2_flow_t *inflow;
    /** 
     * Field flowInclude 
     * This will have a shallow copy and will not be freed by the descructor
     */
    axis2_flow_t *faultflow;
    /** 
     * Field name
     * This will have a shallow copy and will not be freed by the descructor
     */
    axis2_qname_t *qname;
    /**
     * This will have a shallow copy, but will be freed by the descructor
     */
    axis2_transport_receiver_t *recv;
    /** to store handler in inFlow */
    axis2_phase_t *in_phase;
    /** to store handler Fault in inFlow */
    axis2_phase_t *faultphase;
    
}
axis2_transport_in_desc_impl_t;

#define AXIS2_INTF_TO_IMPL(transport_in) ((axis2_transport_in_desc_impl_t *)transport_in)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_free(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env);

axis2_qname_t *AXIS2_CALL
axis2_transport_in_desc_get_qname(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_qname(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_qname_t *qname);

axis2_flow_t *AXIS2_CALL
axis2_transport_in_desc_get_inflow(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_inflow(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_flow_t *inflow);

axis2_flow_t *AXIS2_CALL
axis2_transport_in_desc_get_faultflow(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_faultflow(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_flow_t *faultflow);

axis2_transport_receiver_t *AXIS2_CALL
axis2_transport_in_desc_get_recv(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_recv(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_transport_receiver_t *recv);

axis2_phase_t *AXIS2_CALL
axis2_transport_in_desc_get_in_phase(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_in_phase(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_phase_t *in_phase);
                                            
axis2_phase_t *AXIS2_CALL
axis2_transport_in_desc_get_faultphase(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_faultphase(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_phase_t *faultphase);

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_add_param(
    axis2_transport_in_desc_t *transport_in_desc,
    const axis2_env_t *env,
    axis2_param_t *param);

axis2_param_t *AXIS2_CALL
axis2_transport_in_desc_get_param(
    const axis2_transport_in_desc_t *transport_in_desc,
    const axis2_env_t *env,
    axis2_char_t *param_name);

axis2_bool_t AXIS2_CALL
axis2_transport_in_desc_is_param_locked(
    axis2_transport_in_desc_t *
    transport_in_desc,
    const axis2_env_t *env,
    axis2_char_t *param_name);

/************************** End of function prototypes ************************/

AXIS2_EXTERN axis2_transport_in_desc_t *AXIS2_CALL 
axis2_transport_in_desc_create_with_qname(
    const axis2_env_t *env, 
    axis2_qname_t *qname)
{
    axis2_transport_in_desc_impl_t *transport_in_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    
    transport_in_impl = (axis2_transport_in_desc_impl_t *) AXIS2_MALLOC(env->
        allocator, sizeof(axis2_transport_in_desc_impl_t));
   
   if(NULL == transport_in_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    transport_in_impl->qname = NULL;
    transport_in_impl->in_phase = NULL;
    transport_in_impl->faultphase = NULL;
    transport_in_impl->inflow = NULL;
    transport_in_impl->faultflow = NULL;
    transport_in_impl->recv = NULL;
    transport_in_impl->transport_in.ops = NULL; 
    transport_in_impl->transport_in.param_container = NULL;
    
    transport_in_impl->transport_in.param_container = axis2_param_container_create(env);
    if(NULL == transport_in_impl->transport_in.param_container)
    {
        axis2_transport_in_desc_free(&(transport_in_impl->transport_in), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    transport_in_impl->qname = AXIS2_QNAME_CLONE(qname, env);
    
   transport_in_impl->transport_in.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_transport_in_desc_ops_t));
   if(NULL == transport_in_impl->transport_in.ops)
    {
        axis2_transport_in_desc_free(&(transport_in_impl->transport_in), env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    transport_in_impl->transport_in.ops->free = axis2_transport_in_desc_free;
    transport_in_impl->transport_in.ops->free_void_arg = 
        axis2_transport_in_desc_free_void_arg;
    
   transport_in_impl->transport_in.ops->get_qname =  
        axis2_transport_in_desc_get_qname;
   transport_in_impl->transport_in.ops->set_qname =  
        axis2_transport_in_desc_set_qname;
   transport_in_impl->transport_in.ops->get_inflow =  
        axis2_transport_in_desc_get_inflow;
   transport_in_impl->transport_in.ops->set_inflow = 
        axis2_transport_in_desc_set_inflow;
    transport_in_impl->transport_in.ops->get_faultflow = 
        axis2_transport_in_desc_get_faultflow;
    transport_in_impl->transport_in.ops->set_faultflow = 
        axis2_transport_in_desc_set_faultflow;
    transport_in_impl->transport_in.ops->get_recv = 
        axis2_transport_in_desc_get_recv;
    transport_in_impl->transport_in.ops->set_recv = 
        axis2_transport_in_desc_set_recv;
    transport_in_impl->transport_in.ops->get_in_phase = 
        axis2_transport_in_desc_get_in_phase;
    transport_in_impl->transport_in.ops->set_in_phase = 
        axis2_transport_in_desc_set_in_phase;
    transport_in_impl->transport_in.ops->get_faultphase = 
        axis2_transport_in_desc_get_faultphase;
    transport_in_impl->transport_in.ops->set_faultphase = 
        axis2_transport_in_desc_set_faultphase;
    transport_in_impl->transport_in.ops->add_param = 
    axis2_transport_in_desc_add_param;
    transport_in_impl->transport_in.ops->get_param = 
    axis2_transport_in_desc_get_param;
    transport_in_impl->transport_in.ops->is_param_locked = 
    axis2_transport_in_desc_is_param_locked;
    
   return &(transport_in_impl->transport_in);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_transport_in_desc_free(
    axis2_transport_in_desc_t *transport_in, 
    const axis2_env_t *env)
{
    axis2_transport_in_desc_impl_t *transport_in_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    
    if (transport_in_impl->recv)
    {
        AXIS2_TRANSPORT_RECEIVER_FREE(transport_in_impl->recv, env);
        transport_in_impl->recv = NULL;
    }
      
   if(NULL != transport_in->ops)
    {
        AXIS2_FREE(env->allocator, transport_in->ops);
        transport_in->ops = NULL;
    }
    
    if(NULL != transport_in->param_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(transport_in->param_container, env);
        transport_in->param_container = NULL;
    }
    
    if(NULL != transport_in_impl->qname)
    {
        AXIS2_QNAME_FREE(transport_in_impl->qname, env);
        transport_in_impl->qname = NULL;
    }
    
    if(transport_in_impl->inflow)
    {   
        AXIS2_FLOW_FREE(transport_in_impl->inflow, env);        
        transport_in_impl->inflow = NULL;
    }
    
    if(transport_in_impl->faultflow)
    {   
        AXIS2_FLOW_FREE(transport_in_impl->faultflow, env);        
        transport_in_impl->faultflow = NULL;
    }
    
    if(transport_in_impl->in_phase)
    {   
        AXIS2_FLOW_FREE(transport_in_impl->in_phase, env);        
        transport_in_impl->in_phase = NULL;
    }
    
    if(transport_in_impl->faultphase)
    {   
        AXIS2_FLOW_FREE(transport_in_impl->faultphase, env);        
        transport_in_impl->faultphase = NULL;
    }
    
    AXIS2_FREE(env->allocator, transport_in_impl);
    
   return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_free_void_arg(
        void *transport_in,
        const axis2_env_t *env)
{
    axis2_transport_in_desc_t *transport_in_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    transport_in_l = (axis2_transport_in_desc_t *) transport_in;
    return axis2_transport_in_desc_free(transport_in_l, env);
}

axis2_qname_t *AXIS2_CALL
axis2_transport_in_desc_get_qname(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(transport_in)->qname;
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_qname(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_qname_t *qname) 
{
    axis2_transport_in_desc_impl_t *transport_in_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    
    transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    
    if(transport_in_impl->qname)
    {
        AXIS2_QNAME_FREE(transport_in_impl->qname, env);
        transport_in_impl->qname = NULL;
    }
    transport_in_impl->qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_flow_t *AXIS2_CALL
axis2_transport_in_desc_get_inflow(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(transport_in)->inflow;
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_inflow(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_flow_t *inflow) 
{
    axis2_transport_in_desc_impl_t *transport_in_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, inflow, AXIS2_FAILURE);
    
    transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    if(transport_in_impl->inflow)
    {
        AXIS2_FLOW_FREE(transport_in_impl->inflow, env);
        transport_in_impl->inflow = NULL;
    }
    transport_in_impl->inflow = inflow;
    return AXIS2_SUCCESS;
}

axis2_flow_t *AXIS2_CALL
axis2_transport_in_desc_get_faultflow(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env) 
{
    return AXIS2_INTF_TO_IMPL(transport_in)->faultflow;
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_faultflow(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_flow_t *faultflow) 
{
    axis2_transport_in_desc_impl_t *transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, faultflow, AXIS2_FAILURE);
    
    transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    if(transport_in_impl->faultflow)
    {
        AXIS2_FLOW_FREE(transport_in_impl->faultflow, env);
        transport_in_impl->faultflow = NULL;
    }
    transport_in_impl->faultflow = faultflow;
    return AXIS2_SUCCESS;
}

axis2_transport_receiver_t *AXIS2_CALL
axis2_transport_in_desc_get_recv(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(transport_in)->recv;
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_recv(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_transport_receiver_t *recv) 
{
    axis2_transport_in_desc_impl_t *transport_in_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, recv, AXIS2_FAILURE);
   
    transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    
    if (transport_in_impl->recv)
    {
        AXIS2_TRANSPORT_RECEIVER_FREE(transport_in_impl->recv, env);
        transport_in_impl->recv = NULL;
    }
      
    transport_in_impl->recv = recv;
    return AXIS2_SUCCESS;
}

axis2_phase_t *AXIS2_CALL
axis2_transport_in_desc_get_in_phase(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(transport_in)->in_phase;
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_in_phase(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_phase_t *in_phase) 
{
    axis2_transport_in_desc_impl_t *transport_in_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_phase, AXIS2_FAILURE);
    
    transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    if(transport_in_impl->in_phase)
    {
        AXIS2_FLOW_FREE(transport_in_impl->in_phase, env);
        transport_in_impl->in_phase = NULL;
    }
    transport_in_impl->in_phase = in_phase;
    return AXIS2_SUCCESS;
}

axis2_phase_t *AXIS2_CALL
axis2_transport_in_desc_get_faultphase(
    const axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(transport_in)->faultphase;
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_set_faultphase(
    axis2_transport_in_desc_t *transport_in,
    const axis2_env_t *env,
    axis2_phase_t *faultphase) 
{
    axis2_transport_in_desc_impl_t *transport_in_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, faultphase, AXIS2_FAILURE);
    
    transport_in_impl = AXIS2_INTF_TO_IMPL(transport_in);
    if(transport_in_impl->faultphase)
    {
        AXIS2_FLOW_FREE(transport_in_impl->faultphase, env);
        transport_in_impl->faultphase = NULL;
    }
    transport_in_impl->faultphase = faultphase;
    return AXIS2_SUCCESS;
    
}

axis2_status_t AXIS2_CALL
axis2_transport_in_desc_add_param(
    axis2_transport_in_desc_t *transport_in_desc,
    const axis2_env_t *env,
    axis2_param_t *param)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param, AXIS2_FAILURE);    
    
    return AXIS2_PARAM_CONTAINER_ADD_PARAM(transport_in_desc->
            param_container, env, param);
}

axis2_param_t *AXIS2_CALL
axis2_transport_in_desc_get_param(
    const axis2_transport_in_desc_t *transport_in_desc,
    const axis2_env_t *env,
    axis2_char_t *param_name)
{
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, param_name, NULL);    
    
    return AXIS2_PARAM_CONTAINER_GET_PARAM(transport_in_desc->param_container, 
        env, param_name);
}

axis2_bool_t AXIS2_CALL
axis2_transport_in_desc_is_param_locked(
    axis2_transport_in_desc_t *
    transport_in_desc,
    const axis2_env_t *env,
    axis2_char_t *param_name)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, param_name, AXIS2_FAILURE);    

    return AXIS2_PARAM_CONTAINER_IS_PARAM_LOCKED(transport_in_desc->
        param_container, env, param_name);
}
