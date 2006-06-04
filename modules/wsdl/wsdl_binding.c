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
 
#include <axis2_wsdl_binding.h>

/** 
 * @brief Wsdl binding struct impl
 *   Wsdl features  
 */ 
typedef struct axis2_wsdl_binding_impl
{
   axis2_wsdl_binding_t wsdl_binding;
    
    /**
     * Field name
     */
    axis2_qname_t *qname;

    /**
     * Field boundInterface
     */
    axis2_wsdl_interface_t *bound_interface;

    /**
     * Field bindingFaults
     */
    axis2_hash_t *binding_faults;

    /**
     * Field bindingOperations
     */
    axis2_hash_t *binding_ops;
    
} axis2_wsdl_binding_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_binding) \
      ((axis2_wsdl_binding_impl_t *)wsdl_binding)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
   axis2_wsdl_binding_free (axis2_wsdl_binding_t *wsdl_binding,
                           const axis2_env_t *env);


axis2_wsdl_interface_t * AXIS2_CALL
axis2_wsdl_binding_get_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_wsdl_interface_t *bound_interface);

axis2_qname_t * AXIS2_CALL
axis2_wsdl_binding_get_name(axis2_wsdl_binding_t *wsdl_binding,
                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_name(axis2_wsdl_binding_t *wsdl_binding,
                            const axis2_env_t *env,
                            axis2_qname_t *qname);

axis2_char_t * AXIS2_CALL 
axis2_wsdl_binding_get_target_namespace(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env);

axis2_hash_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_hash_t *binding_faults);

axis2_hash_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_ops(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_ops(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env,
                                            axis2_hash_t *binding_ops);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_op(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env,
                                            axis2_wsdl_binding_op_t *binding_op);

axis2_wsdl_binding_op_t *AXIS2_CALL
axis2_wsdl_binding_get_binding_op(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env,
                                            axis2_qname_t *qName);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_wsdl_binding_fault_t *binding_fault);

axis2_wsdl_binding_fault_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_qname_t *ref);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_binding_t * AXIS2_CALL 
axis2_wsdl_binding_create (const axis2_env_t *env)
{
    axis2_wsdl_binding_impl_t *wsdl_binding_impl = NULL;
   AXIS2_ENV_CHECK(env, NULL);
   
   wsdl_binding_impl = (axis2_wsdl_binding_impl_t *) AXIS2_MALLOC(env->allocator,
         sizeof(axis2_wsdl_binding_impl_t));
   
   if(NULL == wsdl_binding_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE); 
        return NULL;
    }
    
    wsdl_binding_impl->qname = NULL;
    wsdl_binding_impl->bound_interface = NULL;
   wsdl_binding_impl->binding_ops = NULL;
    wsdl_binding_impl->binding_faults = NULL;
    wsdl_binding_impl->wsdl_binding.extensible_component = NULL;
    wsdl_binding_impl->wsdl_binding.ops = NULL;
    
    wsdl_binding_impl->binding_ops = axis2_hash_make(env);
    if(NULL == wsdl_binding_impl->binding_ops)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    wsdl_binding_impl->binding_faults = axis2_hash_make(env);
    if(NULL == wsdl_binding_impl->binding_faults)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
    wsdl_binding_impl->wsdl_binding.extensible_component = axis2_wsdl_extensible_component_create(env);
    if(NULL == wsdl_binding_impl->wsdl_binding.extensible_component)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   wsdl_binding_impl->wsdl_binding.ops = 
      AXIS2_MALLOC (env->allocator, sizeof(axis2_wsdl_binding_ops_t));
   if(NULL == wsdl_binding_impl->wsdl_binding.ops)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
      AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    
   wsdl_binding_impl->wsdl_binding.ops->free =  axis2_wsdl_binding_free;
    
   wsdl_binding_impl->wsdl_binding.ops->get_bound_interface = 
        axis2_wsdl_binding_get_bound_interface;
    
    wsdl_binding_impl->wsdl_binding.ops->set_bound_interface = 
        axis2_wsdl_binding_set_bound_interface;
    
   wsdl_binding_impl->wsdl_binding.ops->get_name = 
        axis2_wsdl_binding_get_name;
    
    wsdl_binding_impl->wsdl_binding.ops->set_name = 
        axis2_wsdl_binding_set_name;
    
    wsdl_binding_impl->wsdl_binding.ops->get_target_namespace = 
        axis2_wsdl_binding_get_target_namespace;
    
    wsdl_binding_impl->wsdl_binding.ops->get_binding_faults = 
        axis2_wsdl_binding_get_binding_faults;
   
    wsdl_binding_impl->wsdl_binding.ops->set_binding_faults = 
        axis2_wsdl_binding_set_binding_faults;
        
    wsdl_binding_impl->wsdl_binding.ops->get_binding_ops = 
        axis2_wsdl_binding_get_binding_ops;

    wsdl_binding_impl->wsdl_binding.ops->set_binding_ops = 
        axis2_wsdl_binding_set_binding_ops;
        
    wsdl_binding_impl->wsdl_binding.ops->add_binding_op = 
        axis2_wsdl_binding_add_binding_op;
        
    wsdl_binding_impl->wsdl_binding.ops->get_binding_op = 
        axis2_wsdl_binding_get_binding_op;
        
    wsdl_binding_impl->wsdl_binding.ops->add_binding_fault = 
        axis2_wsdl_binding_add_binding_fault;
        
    wsdl_binding_impl->wsdl_binding.ops->get_binding_fault = 
        axis2_wsdl_binding_get_binding_fault;       
   
   return &(wsdl_binding_impl->wsdl_binding);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_free (axis2_wsdl_binding_t *wsdl_binding, 
                            const axis2_env_t *env)
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
   if(NULL != wsdl_binding->ops)
    {
        AXIS2_FREE(env->allocator, wsdl_binding->ops);
        wsdl_binding->ops = NULL;
    }
    
    if(NULL != binding_impl->qname)
    {
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(wsdl_binding)->qname, env);
        binding_impl->qname = NULL;
    }
    
    if(NULL != binding_impl->bound_interface)
    {
        AXIS2_WSDL_INTERFACE_FREE(binding_impl->bound_interface, env);
        binding_impl->bound_interface = NULL;
    }
    
    if(NULL != binding_impl->binding_faults)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (binding_impl->binding_faults, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_wsdl_binding_fault_t *fault = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            fault = (axis2_wsdl_binding_fault_t *) val;
            if (fault)
                AXIS2_WSDL_BINDING_FAULT_FREE (fault, env);
            
            val = NULL;
            fault = NULL;
               
        }
        axis2_hash_free(binding_impl->binding_faults, env);
        binding_impl->binding_faults = NULL;
    }
    
    if(NULL != binding_impl->binding_ops)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (binding_impl->binding_ops, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_wsdl_binding_op_t *binding_optr = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            binding_optr = (axis2_wsdl_binding_op_t *) val;
            if (binding_optr)
            {
                AXIS2_WSDL_BINDING_OP_FREE (binding_optr, env);
            }
            
            val = NULL;
            binding_optr = NULL;
               
        }
        axis2_hash_free(binding_impl->binding_ops, env);
        binding_impl->binding_ops = NULL;
    }
    
    if(NULL != wsdl_binding->extensible_component)
    {
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(wsdl_binding->
            extensible_component, env);
        wsdl_binding->extensible_component = NULL;
    }
    
    if(binding_impl)
        AXIS2_FREE(env->allocator, binding_impl);
    binding_impl = NULL;
    
   return AXIS2_SUCCESS;
}

axis2_wsdl_interface_t * AXIS2_CALL
axis2_wsdl_binding_get_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->bound_interface;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_wsdl_interface_t *bound_interface) 
{ 
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, bound_interface, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    if(binding_impl->bound_interface)
    {
        AXIS2_WSDL_INTERFACE_FREE(binding_impl->bound_interface, env);
        binding_impl->bound_interface = NULL;
    }
        
    binding_impl->bound_interface = bound_interface;
    return AXIS2_SUCCESS;
}

axis2_qname_t * AXIS2_CALL
axis2_wsdl_binding_get_name(axis2_wsdl_binding_t *wsdl_binding,
                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->qname;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_name(axis2_wsdl_binding_t *wsdl_binding,
                            const axis2_env_t *env,
                            axis2_qname_t *qname) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, qname, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    if(binding_impl->qname)
    {
        AXIS2_QNAME_FREE(binding_impl->qname, env);
        binding_impl->qname = NULL;
    }
    binding_impl->qname = AXIS2_QNAME_CLONE(qname, env);
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL 
axis2_wsdl_binding_get_target_namespace(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_QNAME_GET_URI(AXIS2_INTF_TO_IMPL(wsdl_binding)->qname, env);
}

axis2_hash_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env) 
{
    
    AXIS2_ENV_CHECK(env, NULL);
    
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->binding_faults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_hash_t *binding_faults) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding_faults, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    if(NULL != binding_impl->binding_faults)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (binding_impl->binding_faults, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_wsdl_binding_fault_t *fault = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            fault = (axis2_wsdl_binding_fault_t *) val;
            if (fault)
                AXIS2_WSDL_BINDING_FAULT_FREE (fault, env);
            
            val = NULL;
            fault = NULL;
               
        }
        axis2_hash_free(binding_impl->binding_faults, env);
        binding_impl->binding_faults = NULL;
    }
    binding_impl->binding_faults = binding_faults;
    return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_ops(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->binding_ops;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_ops(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env,
                                            axis2_hash_t *binding_ops) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding_ops, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    if(NULL != binding_impl->binding_ops)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (binding_impl->binding_ops, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            axis2_wsdl_binding_op_t *binding_optr = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            binding_optr = (axis2_wsdl_binding_op_t *) val;
            if (binding_optr)
            {
                AXIS2_WSDL_BINDING_OP_FREE (binding_optr, env);
            }
            
            val = NULL;
            binding_optr = NULL;
               
        }
        axis2_hash_free(binding_impl->binding_ops, env);
        binding_impl->binding_ops = NULL;
    }
    binding_impl->binding_ops = binding_ops;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_op(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env,
                                            axis2_wsdl_binding_op_t *binding_op) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    axis2_qname_t *qname = NULL;
    axis2_char_t *name = NULL;
   
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding_op, AXIS2_FAILURE);
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    qname = AXIS2_WSDL_BINDING_OP_GET_QNAME(binding_op, env);
    if(NULL == qname)
    {
        return AXIS2_FAILURE;
    }
    name = AXIS2_QNAME_TO_STRING(qname, env);
    axis2_hash_set(binding_impl->binding_ops, name, AXIS2_HASH_KEY_STRING,
        binding_op);
    
    return AXIS2_SUCCESS;
}

axis2_wsdl_binding_op_t *AXIS2_CALL
axis2_wsdl_binding_get_binding_op(axis2_wsdl_binding_t *wsdl_binding,
                                            const axis2_env_t *env,
                                            axis2_qname_t *qname) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    axis2_char_t *name = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, qname, NULL);
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    name = AXIS2_QNAME_TO_STRING(qname, env);
    return (axis2_wsdl_binding_op_t *) axis2_hash_get(binding_impl->binding_ops, 
        name, AXIS2_HASH_KEY_STRING);
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_wsdl_binding_fault_t *binding_fault) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    axis2_qname_t *fault_ref = NULL;
    axis2_char_t *fault_ref_name = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, binding_fault, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    fault_ref = AXIS2_WSDL_BINDING_FAULT_GET_REF(binding_fault, env);
    if(!fault_ref)
    {
        return AXIS2_FAILURE;
    }
    fault_ref_name = AXIS2_QNAME_TO_STRING(fault_ref, env);
    axis2_hash_set(binding_impl->binding_faults, fault_ref_name,
        AXIS2_HASH_KEY_STRING, binding_fault);
    return AXIS2_SUCCESS;
}

axis2_wsdl_binding_fault_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        const axis2_env_t *env,
                                        axis2_qname_t *ref) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    axis2_char_t *ref_name = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, ref, NULL);
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    ref_name = AXIS2_QNAME_TO_STRING(ref, env);

    return (axis2_wsdl_binding_fault_t *) axis2_hash_get(
        binding_impl->binding_faults, ref_name, AXIS2_HASH_KEY_STRING);
}
