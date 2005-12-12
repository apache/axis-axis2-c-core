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
 *	Wsdl features  
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
    struct axis2_wsdl_interface *bound_interface;

    /**
     * Field bindingFaults
     */
    axis2_hash_t *binding_faults;

    /**
     * Field bindingOperations
     */
    axis2_hash_t *binding_operations;
    
} axis2_wsdl_binding_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_binding) \
		((axis2_wsdl_binding_impl_t *)wsdl_binding)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_wsdl_binding_free (axis2_wsdl_binding_t *wsdl_binding,
									axis2_env_t **env);


struct axis2_wsdl_interface * AXIS2_CALL
axis2_wsdl_binding_get_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_interface *bound_interface);

axis2_qname_t * AXIS2_CALL
axis2_wsdl_binding_get_name(axis2_wsdl_binding_t *wsdl_binding,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_name(axis2_wsdl_binding_t *wsdl_binding,
                            axis2_env_t **env,
                            axis2_qname_t *qname);

axis2_char_t * AXIS2_CALL 
axis2_wsdl_binding_get_target_namespace(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env);

axis2_hash_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        axis2_hash_t *binding_faults);

axis2_hash_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_operations(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_operations(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env,
                                            axis2_hash_t *binding_operations);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_operation(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env,
                                            struct axis2_wsdl_binding_operation *binding_operation);

struct axis2_wsdl_binding_operation *AXIS2_CALL
axis2_wsdl_binding_get_binding_operation(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env,
                                            axis2_qname_t *qName);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_binding_fault *binding_fault);

axis2_wsdl_binding_fault_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        axis2_qname_t *ref);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_binding_t * AXIS2_CALL 
axis2_wsdl_binding_create (axis2_env_t **env)
{
    axis2_wsdl_binding_impl_t *wsdl_binding_impl = NULL;
	AXIS2_ENV_CHECK(env, NULL);
	
	wsdl_binding_impl = (axis2_wsdl_binding_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_wsdl_binding_impl_t));
	
	if(NULL == wsdl_binding_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
        return NULL;
    }
    
    wsdl_binding_impl->qname = NULL;
	wsdl_binding_impl->binding_operations = NULL;
    wsdl_binding_impl->binding_faults = NULL;
    wsdl_binding_impl->wsdl_binding.extensible_component = NULL;
    wsdl_binding_impl->wsdl_binding.ops = NULL;
    
    wsdl_binding_impl->binding_operations = axis2_hash_make(env);
    if(NULL == wsdl_binding_impl->binding_operations)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
    wsdl_binding_impl->binding_faults = axis2_hash_make(env);
    if(NULL == wsdl_binding_impl->binding_faults)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
    wsdl_binding_impl->wsdl_binding.extensible_component = axis2_wsdl_extensible_component_create(env);
    if(NULL == wsdl_binding_impl->wsdl_binding.extensible_component)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
	wsdl_binding_impl->wsdl_binding.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_binding_ops_t));
	if(NULL == wsdl_binding_impl->wsdl_binding.ops)
    {
        axis2_wsdl_binding_free(&(wsdl_binding_impl->wsdl_binding), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
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
        
    wsdl_binding_impl->wsdl_binding.ops->get_binding_operations = 
        axis2_wsdl_binding_get_binding_operations;

    wsdl_binding_impl->wsdl_binding.ops->set_binding_operations = 
        axis2_wsdl_binding_set_binding_operations;
        
    wsdl_binding_impl->wsdl_binding.ops->add_binding_operation = 
        axis2_wsdl_binding_add_binding_operation;
        
    wsdl_binding_impl->wsdl_binding.ops->get_binding_operation = 
        axis2_wsdl_binding_get_binding_operation;
        
    wsdl_binding_impl->wsdl_binding.ops->add_binding_fault = 
        axis2_wsdl_binding_add_binding_fault;
        
    wsdl_binding_impl->wsdl_binding.ops->get_binding_fault = 
        axis2_wsdl_binding_get_binding_fault;       
	
	return &(wsdl_binding_impl->wsdl_binding);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_free (axis2_wsdl_binding_t *wsdl_binding, 
                            axis2_env_t **env)
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
	if(NULL != wsdl_binding->ops)
    {
        AXIS2_FREE((*env)->allocator, wsdl_binding->ops);
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
            struct axis2_wsdl_binding_fault *fault = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            fault = (struct axis2_wsdl_binding_fault *) val;
            if (fault)
                AXIS2_WSDL_BINDING_FAULT_FREE (fault, env);
            
            val = NULL;
            fault = NULL;
               
        }
        axis2_hash_free(binding_impl->binding_faults, env);
        binding_impl->binding_faults = NULL;
    }
    
    if(NULL != binding_impl->binding_operations)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (binding_impl->binding_operations, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_wsdl_binding_operation *binding_optr = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            binding_optr = (struct axis2_wsdl_binding_operation *) val;
            if (binding_optr)
            {
                AXIS2_WSDL_BINDING_OPERATION_FREE (binding_optr, env);
            }
            
            val = NULL;
            binding_optr = NULL;
               
        }
        axis2_hash_free(binding_impl->binding_operations, env);
        binding_impl->binding_operations = NULL;
    }
    
    if(NULL != wsdl_binding->extensible_component)
    {
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(wsdl_binding->
            extensible_component, env);
        wsdl_binding->extensible_component = NULL;
    }
    
    if(binding_impl)
        AXIS2_FREE((*env)->allocator, binding_impl);
    binding_impl = NULL;
    
	return AXIS2_SUCCESS;
}

struct axis2_wsdl_interface * AXIS2_CALL
axis2_wsdl_binding_get_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->bound_interface;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_bound_interface(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_interface *bound_interface) 
{ 
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, bound_interface, AXIS2_FAILURE);
    
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
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->qname;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_name(axis2_wsdl_binding_t *wsdl_binding,
                            axis2_env_t **env,
                            axis2_qname_t *qname) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    if(binding_impl->qname)
    {
        AXIS2_QNAME_FREE(binding_impl->qname, env);
        binding_impl->qname = NULL;
    }
    binding_impl->qname = qname;
    return AXIS2_SUCCESS;
}

axis2_char_t * AXIS2_CALL 
axis2_wsdl_binding_get_target_namespace(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, NULL);
    return AXIS2_QNAME_GET_URI(AXIS2_INTF_TO_IMPL(wsdl_binding)->qname, env);
}

axis2_hash_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env) 
{
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->binding_faults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_faults(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        axis2_hash_t *binding_faults) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, binding_faults, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    if(NULL != binding_impl->binding_faults)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (binding_impl->binding_faults, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_wsdl_binding_fault *fault = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            fault = (struct axis2_wsdl_binding_fault *) val;
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
axis2_wsdl_binding_get_binding_operations(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, NULL);
    return AXIS2_INTF_TO_IMPL(wsdl_binding)->binding_operations;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_set_binding_operations(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env,
                                            axis2_hash_t *binding_operations) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, binding_operations, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    if(NULL != binding_impl->binding_operations)
    {
        axis2_hash_index_t *hi = NULL;
        void *val = NULL;
        for (hi = axis2_hash_first (binding_impl->binding_operations, env); hi;
                 hi = axis2_hash_next ( env, hi))
        {
            struct axis2_wsdl_binding_operation *binding_optr = NULL;
            axis2_hash_this (hi, NULL, NULL, &val);
            binding_optr = (struct axis2_wsdl_binding_operation *) val;
            if (binding_optr)
            {
                AXIS2_WSDL_BINDING_OPERATION_FREE (binding_optr, env);
            }
            
            val = NULL;
            binding_optr = NULL;
               
        }
        axis2_hash_free(binding_impl->binding_operations, env);
        binding_impl->binding_operations = NULL;
    }
    binding_impl->binding_operations = binding_operations;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_operation(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env,
                                            struct axis2_wsdl_binding_operation *binding_operation) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    axis2_qname_t *qname = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, binding_operation, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    qname = AXIS2_WSDL_BINDING_OPERATION_GET_QNAME(binding_operation, env);
    if(NULL == qname)
    {
        return AXIS2_FAILURE;
    }
    axis2_hash_set(binding_impl->binding_operations, qname, 
        sizeof(axis2_qname_t), binding_operation);
    
    return AXIS2_SUCCESS;
}

struct axis2_wsdl_binding_operation *AXIS2_CALL
axis2_wsdl_binding_get_binding_operation(axis2_wsdl_binding_t *wsdl_binding,
                                            axis2_env_t **env,
                                            axis2_qname_t *qName) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, NULL);
    return (struct axis2_wsdl_binding_operation *) axis2_hash_get(
        AXIS2_INTF_TO_IMPL(wsdl_binding)->binding_operations, qName,
            sizeof(axis2_qname_t));
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_add_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_binding_fault *binding_fault) 
{
    axis2_wsdl_binding_impl_t *binding_impl = NULL;
    axis2_qname_t *fault_ref = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, binding_fault, AXIS2_FAILURE);
    
    binding_impl = AXIS2_INTF_TO_IMPL(wsdl_binding);
    
    fault_ref = AXIS2_WSDL_BINDING_FAULT_GET_REF(binding_fault, env);
    if(!fault_ref)
    {
        return AXIS2_FAILURE;
    }
    axis2_hash_set(binding_impl->binding_faults, fault_ref, sizeof(axis2_qname_t),
        binding_fault);
    return AXIS2_SUCCESS;
}

axis2_wsdl_binding_fault_t * AXIS2_CALL
axis2_wsdl_binding_get_binding_fault(axis2_wsdl_binding_t *wsdl_binding,
                                        axis2_env_t **env,
                                        axis2_qname_t *ref) 
{
    AXIS2_FUNC_PARAM_CHECK(wsdl_binding, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, ref, NULL);
    
    return (axis2_wsdl_binding_fault_t *) axis2_hash_get(
        AXIS2_INTF_TO_IMPL(wsdl_binding)->binding_faults, ref, sizeof(axis2_qname_t));
}
