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

#include <axis2_wsdl_binding_op.h>

/** 
 * @brief Wsdl Component struct impl
 *   Axis2 Wsdl Component impl  
 */
typedef struct axis2_wsdl_binding_op_impl
{
   axis2_wsdl_binding_op_t binding_op;
   /**
     * Field name
     */
    axis2_qname_t *qname;
    /**
     * Field in_faults
     */
    axis2_linked_list_t *in_faults;
    /**
     * Field out_faults
     */
    axis2_linked_list_t *out_faults;
    /**
     * Field op
     */
    void *op;
    /**
     * Field input
     */
    axis2_wsdl_binding_msg_ref_t *input;
    /**
     * Field output
     */
    axis2_wsdl_binding_msg_ref_t *output;
        
} axis2_wsdl_binding_op_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_binding_op) \
        ((axis2_wsdl_binding_op_impl_t *)(wsdl_binding_op))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_free (axis2_wsdl_binding_op_t *wsdl_binding_op,
                            const axis2_env_t *env);


axis2_wsdl_binding_msg_ref_t * AXIS2_CALL
axis2_wsdl_binding_op_get_input(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_input(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env,
                                        axis2_wsdl_binding_msg_ref_t *input);

void *AXIS2_CALL
axis2_wsdl_binding_op_get_op(axis2_wsdl_binding_op_t *binding_op,
                                    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_op(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            void *op);

axis2_wsdl_binding_msg_ref_t * AXIS2_CALL
axis2_wsdl_binding_op_get_output(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_output(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env,
                                        axis2_wsdl_binding_msg_ref_t *output);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_binding_op_get_qname(axis2_wsdl_binding_op_t *binding_op,
                                    const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_qname(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_add_in_fault(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_wsdl_binding_fault_t *in_fault);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_add_out_fault(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_wsdl_binding_fault_t *out_fault);

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_op_get_in_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_in_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_linked_list_t *in_faults);

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_op_get_out_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_out_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_linked_list_t *out_faults);

      
/***************************** End of function headers ************************/

axis2_wsdl_binding_op_t * AXIS2_CALL 
axis2_wsdl_binding_op_create (const axis2_env_t *env)
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
   binding_op_impl = (axis2_wsdl_binding_op_impl_t *) 
        AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_binding_op_impl_t));
    
   if(NULL == binding_op_impl)
   {
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
   }
    
    binding_op_impl->qname = NULL;
    binding_op_impl->op = NULL;
    binding_op_impl->input = NULL;
    binding_op_impl->output = NULL;
    binding_op_impl->in_faults = NULL;
    binding_op_impl->out_faults = NULL;
    binding_op_impl->binding_op.extensible_component = NULL;
    binding_op_impl->binding_op.ops = NULL;
   
    binding_op_impl->in_faults = axis2_linked_list_create (env);
    
   if(NULL == binding_op_impl->in_faults)
   {
      axis2_wsdl_binding_op_free(&(binding_op_impl->binding_op),
            env);
      AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
   }
    
    binding_op_impl->out_faults = axis2_linked_list_create (env);
    
   if(NULL == binding_op_impl->out_faults)
   {
        axis2_wsdl_binding_op_free(&(binding_op_impl->binding_op),
            env);
      AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);   
        return NULL;
   }
    
    binding_op_impl->binding_op.extensible_component = 
        axis2_wsdl_extensible_component_create(env);
    
   if(NULL == binding_op_impl->binding_op.extensible_component)
   {
        axis2_wsdl_binding_op_free(&(binding_op_impl->binding_op),
            env);
      AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
   }
    
   binding_op_impl->binding_op.ops = (axis2_wsdl_binding_op_ops_t *)
      AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_binding_op_ops_t));
    
   if(NULL == binding_op_impl->binding_op.ops)
   {
        axis2_wsdl_binding_op_free(&(binding_op_impl->binding_op),
            env);
      AXIS2_ERROR_SET (env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
   }
    
   binding_op_impl->binding_op.ops->free = axis2_wsdl_binding_op_free;
    
   binding_op_impl->binding_op.ops->get_input =
        axis2_wsdl_binding_op_get_input;
    
    binding_op_impl->binding_op.ops->set_input =
        axis2_wsdl_binding_op_set_input;
    
    binding_op_impl->binding_op.ops->get_op =
        axis2_wsdl_binding_op_get_op;
    
    binding_op_impl->binding_op.ops->set_op =
        axis2_wsdl_binding_op_set_op;
    
    binding_op_impl->binding_op.ops->get_output =
        axis2_wsdl_binding_op_get_output;
    
    binding_op_impl->binding_op.ops->set_output =
        axis2_wsdl_binding_op_set_output;
    
    binding_op_impl->binding_op.ops->get_qname =
        axis2_wsdl_binding_op_get_qname;
    
    binding_op_impl->binding_op.ops->set_qname =
        axis2_wsdl_binding_op_set_qname;
        
    binding_op_impl->binding_op.ops->add_in_fault =
        axis2_wsdl_binding_op_add_in_fault;
    
    binding_op_impl->binding_op.ops->add_out_fault =
        axis2_wsdl_binding_op_add_out_fault;

    binding_op_impl->binding_op.ops->get_in_faults =
        axis2_wsdl_binding_op_get_in_faults; 
        
    binding_op_impl->binding_op.ops->set_in_faults =
        axis2_wsdl_binding_op_set_in_faults; 

    binding_op_impl->binding_op.ops->get_out_faults =
        axis2_wsdl_binding_op_get_out_faults; 
        
    binding_op_impl->binding_op.ops->set_out_faults =
        axis2_wsdl_binding_op_set_out_faults; 
    
   return &(binding_op_impl->binding_op);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_op_free (axis2_wsdl_binding_op_t *binding_op, 
                            const axis2_env_t *env)
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    
   if(NULL != binding_op->ops)
    {
        AXIS2_FREE(env->allocator, binding_op->ops);
        binding_op->ops = NULL;
    }
    
    if(NULL != binding_op_impl->in_faults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(binding_op_impl->in_faults, env); i++)
        {
            axis2_wsdl_binding_fault_t *binding_fault = NULL;
            binding_fault = AXIS2_LINKED_LIST_GET(binding_op_impl->in_faults, env, i);
            
            binding_fault = (axis2_wsdl_binding_fault_t *) val;
            if (binding_fault)
               AXIS2_WSDL_BINDING_FAULT_FREE (binding_fault, env);
            
            val = NULL;
            binding_fault = NULL;
               
        }
        AXIS2_LINKED_LIST_FREE(binding_op_impl->in_faults, env);
        binding_op_impl->in_faults = NULL;
    }
    
    if(NULL != binding_op_impl->out_faults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(binding_op_impl->out_faults, env); i++)
        {
            axis2_wsdl_binding_fault_t *binding_fault = NULL;
            binding_fault = AXIS2_LINKED_LIST_GET(binding_op_impl->out_faults, env, i);
            
            binding_fault = (axis2_wsdl_binding_fault_t *) val;
            if (binding_fault)
               AXIS2_WSDL_BINDING_FAULT_FREE (binding_fault, env);
            
            val = NULL;
            binding_fault = NULL;
               
        }
        AXIS2_LINKED_LIST_FREE(binding_op_impl->out_faults, env);
        binding_op_impl->out_faults = NULL;
    }
    
    if(NULL != binding_op->extensible_component)
    {
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(binding_op->
            extensible_component, env);
        binding_op->extensible_component = NULL;
    }
    
    if(binding_op_impl)
        AXIS2_FREE(env->allocator, binding_op_impl);
    binding_op_impl = NULL;
    
   return AXIS2_SUCCESS;
}

axis2_wsdl_binding_msg_ref_t * AXIS2_CALL
axis2_wsdl_binding_op_get_input(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_op)->input;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_input(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env,
                                        axis2_wsdl_binding_msg_ref_t *input) 
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, input, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    if(binding_op_impl->input)
    {
        AXIS2_WSDL_BINDING_MSG_REF_FREE(binding_op_impl->input, env);
        binding_op_impl->input = NULL;
    }
    binding_op_impl->input = input;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_wsdl_binding_op_get_op(axis2_wsdl_binding_op_t *binding_op,
                                    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_op)->op;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_op(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            void *op) 
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, op, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    
    if(NULL != binding_op_impl->op)
    {
        axis2_wsdl_op_free_void_arg(op, env);
        binding_op_impl->op = NULL;
    }
    binding_op_impl->op = op;
    return AXIS2_SUCCESS;
}

axis2_wsdl_binding_msg_ref_t * AXIS2_CALL
axis2_wsdl_binding_op_get_output(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_op)->output;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_output(axis2_wsdl_binding_op_t *binding_op,
                                        const axis2_env_t *env,
                                        axis2_wsdl_binding_msg_ref_t *output) 
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, output, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    if(binding_op_impl->output)
    {
        AXIS2_WSDL_BINDING_MSG_REF_FREE(binding_op_impl->output, env);
        binding_op_impl->output = NULL;
    }
    binding_op_impl->output = output;
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_binding_op_get_qname(axis2_wsdl_binding_op_t *binding_op,
                                    const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_op)->qname;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_qname(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_qname_t *qname) 
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    
    if(binding_op_impl->qname)
    {
        AXIS2_QNAME_FREE(binding_op_impl->qname, env);
        binding_op_impl->qname = NULL;
    }
    binding_op_impl->qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_add_in_fault(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_wsdl_binding_fault_t *in_fault) 
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_fault, AXIS2_FAILURE);
    
    return AXIS2_LINKED_LIST_ADD(AXIS2_INTF_TO_IMPL(binding_op)->in_faults,
        env, in_fault);
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_add_out_fault(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_wsdl_binding_fault_t *out_fault) 
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_fault, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    
    return AXIS2_LINKED_LIST_ADD(binding_op_impl->out_faults,
        env, out_fault);
}

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_op_get_in_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_op)->in_faults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_in_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_linked_list_t *in_faults) 
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_faults, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    
    if(NULL != binding_op_impl->out_faults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(binding_op_impl->out_faults, env); i++)
        {
            axis2_wsdl_binding_fault_t *binding_fault = NULL;
            binding_fault = AXIS2_LINKED_LIST_GET(binding_op_impl->out_faults, env, i);
            
            binding_fault = (axis2_wsdl_binding_fault_t *) val;
            if (binding_fault)
               AXIS2_WSDL_BINDING_FAULT_FREE (binding_fault, env);
            
            val = NULL;
            binding_fault = NULL;
               
        }
        AXIS2_LINKED_LIST_FREE(binding_op_impl->out_faults, env);
        binding_op_impl->out_faults = NULL;
    }
    binding_op_impl->in_faults = in_faults;
    return AXIS2_SUCCESS;
}

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_op_get_out_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_op)->out_faults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_op_set_out_faults(axis2_wsdl_binding_op_t *binding_op,
                                            const axis2_env_t *env,
                                            axis2_linked_list_t *out_faults) 
{
    axis2_wsdl_binding_op_impl_t *binding_op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_faults, AXIS2_FAILURE);
    
    binding_op_impl = AXIS2_INTF_TO_IMPL(binding_op);
    
    if(NULL != binding_op_impl->out_faults)
    {
        void *val = NULL;
        int i = 0;
        for (i = 0; i < AXIS2_LINKED_LIST_SIZE(binding_op_impl->out_faults, env); i++)
        {
            axis2_wsdl_binding_fault_t *binding_fault = NULL;
            binding_fault = AXIS2_LINKED_LIST_GET(binding_op_impl->out_faults, env, i);
            
            binding_fault = (axis2_wsdl_binding_fault_t *) val;
            if (binding_fault)
               AXIS2_WSDL_BINDING_FAULT_FREE (binding_fault, env);
            
            val = NULL;
            binding_fault = NULL;
               
        }
        AXIS2_LINKED_LIST_FREE(binding_op_impl->out_faults, env);
        binding_op_impl->out_faults = NULL;
    }
    
    binding_op_impl->out_faults = out_faults;
    return AXIS2_SUCCESS;
}
