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
 
#include <axis2_wsdl_binding_fault.h>

/** 
 * @brief Wsdl Binding Fault struct impl
 *	Wsdl Binding Fault  
 */ 
typedef struct axis2_wsdl_binding_fault_impl
{
	axis2_wsdl_binding_fault_t binding_fault;
    
    /**
     * Field ref
     */
    axis2_qname_t *ref;
    
} axis2_wsdl_binding_fault_impl_t;

#define AXIS2_INTF_TO_IMPL(binding_fault) \
		((axis2_wsdl_binding_fault_impl_t *)binding_fault)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
	axis2_binding_fault_free (axis2_wsdl_binding_fault_t *binding_fault,
									axis2_env_t **env);
axis2_qname_t *AXIS2_CALL
axis2_binding_fault_get_ref(axis2_wsdl_binding_fault_t *binding_fault,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_binding_fault_set_ref(axis2_wsdl_binding_fault_t *binding_fault,
                            axis2_env_t **env,
                            axis2_qname_t *ref);
                                
/************************** End of function prototypes ************************/

axis2_wsdl_binding_fault_t * AXIS2_CALL 
axis2_binding_fault_create (axis2_env_t **env)
{
    axis2_wsdl_binding_fault_impl_t *binding_fault_impl = NULL;
    
	AXIS2_ENV_CHECK(env, NULL);
	
	binding_fault_impl = (axis2_wsdl_binding_fault_impl_t *) 
        AXIS2_MALLOC((*env)->allocator, sizeof(axis2_wsdl_binding_fault_impl_t));
	
	
	if(NULL == binding_fault_impl)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
        return NULL;
    }
	
    binding_fault_impl->ref = NULL;
    binding_fault_impl->binding_fault.extensible_component = NULL;
    binding_fault_impl->binding_fault.ops = NULL;
    
    binding_fault_impl->binding_fault.extensible_component = 
        axis2_wsdl_extensible_component_create(env);
 
    if(NULL == binding_fault_impl->binding_fault.extensible_component)
    {
        axis2_binding_fault_free(&(binding_fault_impl->binding_fault), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
	binding_fault_impl->binding_fault.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_binding_fault_ops_t));
	if(NULL == binding_fault_impl->binding_fault.ops)
    {
        axis2_binding_fault_free(&(binding_fault_impl->binding_fault), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
        return NULL;
    }
    
	binding_fault_impl->binding_fault.ops->free =  axis2_binding_fault_free;
    
	binding_fault_impl->binding_fault.ops->get_ref =  
        axis2_binding_fault_get_ref;
    
	binding_fault_impl->binding_fault.ops->set_ref =  
        axis2_binding_fault_set_ref;
    
	return &(binding_fault_impl->binding_fault);
}

/***************************Function implementation****************************/

axis2_status_t AXIS2_CALL 
axis2_binding_fault_free (axis2_wsdl_binding_fault_t *binding_fault, 
                            axis2_env_t **env)
{
    axis2_wsdl_binding_fault_impl_t *binding_fault_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(binding_fault, env, AXIS2_FAILURE);
    
    binding_fault_impl = AXIS2_INTF_TO_IMPL(binding_fault);
	if(NULL != binding_fault->ops)
    {
        AXIS2_FREE((*env)->allocator, binding_fault->ops);
        binding_fault->ops = NULL;
    }
    
    if(NULL != binding_fault_impl->ref)
    {
        AXIS2_QNAME_FREE(binding_fault_impl->ref, env);
        binding_fault_impl->ref = NULL;
    }
    
    if(NULL != binding_fault->extensible_component)
    {
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(binding_fault->extensible_component, env);
        binding_fault->extensible_component = NULL;
    }
    if(binding_fault_impl)
        AXIS2_FREE((*env)->allocator, binding_fault_impl);
    binding_fault_impl = NULL;
	return AXIS2_SUCCESS;
}
    

/**
 * Method getRef
 *
 * @return
 */
axis2_qname_t *AXIS2_CALL
axis2_binding_fault_get_ref(axis2_wsdl_binding_fault_t *binding_fault,
                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_fault, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_fault)->ref;
}

/**
 * Method setRef
 *
 * @param ref
 */
axis2_status_t AXIS2_CALL
axis2_binding_fault_set_ref(axis2_wsdl_binding_fault_t *binding_fault,
                            axis2_env_t **env,
                            axis2_qname_t *ref) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_fault, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, ref, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(binding_fault)->ref = ref;
    return AXIS2_SUCCESS;
}
