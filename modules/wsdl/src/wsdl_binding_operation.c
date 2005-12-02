#include <axis2_wsdl_binding_operation.h>
#include <string.h>

/** 
 * @brief Wsdl Component struct impl
 *	Axis2 Wsdl Component impl  
 */
typedef struct axis2_wsdl_binding_operation_impl
{
	axis2_wsdl_binding_operation_t binding_operation;
	    /**
     * Field name
     */
    axis2_qname_t *qname;


    /**
     * Field infaults
     */
    axis2_linked_list_t *infaults;

    /**
     * Field outfaults
     */
    axis2_linked_list_t *outfaults;

    /**
     * Field operation
     */
    void *operation;

    /**
     * Field input
     */
    struct axis2_wsdl_binding_msg_ref *input;

    /**
     * Field output
     */
    struct axis2_wsdl_binding_msg_ref *output;
} axis2_wsdl_binding_operation_impl_t;

#define AXIS2_INTF_TO_IMPL(wsdl_binding_operation) \
        ((axis2_wsdl_binding_operation_impl_t *)(wsdl_binding_operation))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_free (axis2_wsdl_binding_operation_t *wsdl_binding_operation,
                            axis2_env_t **env);


struct axis2_wsdl_binding_msg_ref * AXIS2_CALL
axis2_wsdl_binding_operation_get_input(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_input(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_binding_msg_ref *input);

void *AXIS2_CALL
axis2_wsdl_binding_operation_get_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            void *operation);

struct axis2_wsdl_binding_msg_ref * AXIS2_CALL
axis2_wsdl_binding_operation_get_output(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_output(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_binding_msg_ref *output);

axis2_qname_t *AXIS2_CALL
axis2_wsdl_binding_operation_get_qname(axis2_wsdl_binding_operation_t *binding_operation,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_qname(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_add_infault(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_wsdl_binding_fault_t *infault);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_add_outfault(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_wsdl_binding_fault_t *outfault);

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_operation_get_infaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_infaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_linked_list_t *infaults);

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_operation_get_outfaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_outfaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_linked_list_t *outfaults);

		
/***************************** End of function headers ************************/

axis2_wsdl_binding_operation_t * AXIS2_CALL 
axis2_wsdl_binding_operation_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
	axis2_wsdl_binding_operation_impl_t *binding_operation_impl = 
        (axis2_wsdl_binding_operation_impl_t *) AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_wsdl_binding_operation_impl_t));
    
	if(NULL == binding_operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    binding_operation_impl->qname = NULL;
    binding_operation_impl->operation = NULL;
    binding_operation_impl->input = NULL;
    binding_operation_impl->output = NULL;
	
    binding_operation_impl->infaults = axis2_linked_list_create (env);
    
	if(NULL == binding_operation_impl->infaults)
	{
		AXIS2_FREE ((*env)->allocator, binding_operation_impl);
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
    binding_operation_impl->outfaults = axis2_linked_list_create (env);
    
	if(NULL == binding_operation_impl->outfaults)
	{
        AXIS2_LINKED_LIST_FREE(binding_operation_impl->infaults, env);
		AXIS2_FREE ((*env)->allocator, binding_operation_impl);
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
    binding_operation_impl->binding_operation.extensible_component = 
        axis2_wsdl_extensible_component_create(env);
    
	if(NULL == binding_operation_impl->binding_operation.extensible_component)
	{
        AXIS2_LINKED_LIST_FREE(binding_operation_impl->infaults, env);
        AXIS2_LINKED_LIST_FREE(binding_operation_impl->outfaults, env);
		AXIS2_FREE ((*env)->allocator, binding_operation_impl);
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
	binding_operation_impl->binding_operation.ops = (axis2_wsdl_binding_operation_ops_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_wsdl_binding_operation_ops_t));
    
	if(NULL == binding_operation_impl->binding_operation.ops)
	{
        AXIS2_LINKED_LIST_FREE(binding_operation_impl->infaults, env);
        AXIS2_LINKED_LIST_FREE(binding_operation_impl->outfaults, env);
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(binding_operation_impl->
            binding_operation.extensible_component, env);
		AXIS2_FREE ((*env)->allocator, binding_operation_impl);
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
	binding_operation_impl->binding_operation.ops->free = axis2_wsdl_binding_operation_free;
    
	binding_operation_impl->binding_operation.ops->get_input =
        axis2_wsdl_binding_operation_get_input;
    
    binding_operation_impl->binding_operation.ops->set_input =
        axis2_wsdl_binding_operation_set_input;
    
    binding_operation_impl->binding_operation.ops->get_operation =
        axis2_wsdl_binding_operation_get_operation;
    
    binding_operation_impl->binding_operation.ops->set_operation =
        axis2_wsdl_binding_operation_set_operation;
    
    binding_operation_impl->binding_operation.ops->get_output =
        axis2_wsdl_binding_operation_get_output;
    
    binding_operation_impl->binding_operation.ops->set_output =
        axis2_wsdl_binding_operation_set_output;
    
    binding_operation_impl->binding_operation.ops->get_qname =
        axis2_wsdl_binding_operation_get_qname;
    
    binding_operation_impl->binding_operation.ops->set_qname =
        axis2_wsdl_binding_operation_set_qname;
        
    binding_operation_impl->binding_operation.ops->add_infault =
        axis2_wsdl_binding_operation_add_infault;
    
    binding_operation_impl->binding_operation.ops->add_outfault =
        axis2_wsdl_binding_operation_add_outfault;

    binding_operation_impl->binding_operation.ops->get_infaults =
        axis2_wsdl_binding_operation_get_infaults; 
        
    binding_operation_impl->binding_operation.ops->set_infaults =
        axis2_wsdl_binding_operation_set_infaults; 

    binding_operation_impl->binding_operation.ops->get_outfaults =
        axis2_wsdl_binding_operation_get_outfaults; 
        
    binding_operation_impl->binding_operation.ops->set_outfaults =
        axis2_wsdl_binding_operation_set_outfaults; 
    
	return &(binding_operation_impl->binding_operation);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_operation_free (axis2_wsdl_binding_operation_t *binding_operation, 
                            axis2_env_t **env)
{
    axis2_wsdl_binding_operation_impl_t *binding_operation_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    
    binding_operation_impl = AXIS2_INTF_TO_IMPL(binding_operation);
    
	if(NULL != binding_operation->ops)
        AXIS2_FREE((*env)->allocator, binding_operation->ops);
    
    if(NULL != binding_operation_impl->infaults)
       AXIS2_LINKED_LIST_FREE(binding_operation_impl->infaults, env);
    
    if(NULL != binding_operation_impl->outfaults)
       AXIS2_LINKED_LIST_FREE(binding_operation_impl->outfaults, env);
    
    if(NULL != binding_operation->extensible_component)
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(binding_operation->
            extensible_component, env);
    
    AXIS2_FREE((*env)->allocator, binding_operation_impl);
    
	return AXIS2_SUCCESS;
}

struct axis2_wsdl_binding_msg_ref * AXIS2_CALL
axis2_wsdl_binding_operation_get_input(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_operation)->input;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_input(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_binding_msg_ref *input) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, input, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(binding_operation)->input = input;
    return AXIS2_SUCCESS;
}

void *AXIS2_CALL
axis2_wsdl_binding_operation_get_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_operation)->operation;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            void *operation) 
{
    struct axis2_operation *ops = NULL;
    axis2_wsdl_binding_operation_impl_t *binding_operation_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, operation, AXIS2_FAILURE);
    
    binding_operation_impl = AXIS2_INTF_TO_IMPL(binding_operation);
    
    if(NULL != binding_operation_impl->operation)
    {
        ops = (struct axis2_operation *) (binding_operation_impl->operation);
        AXIS2_OPERATION_FREE(ops, env);
        binding_operation_impl->operation = NULL;
    }
    binding_operation_impl->operation = operation;
    return AXIS2_SUCCESS;
}

struct axis2_wsdl_binding_msg_ref * AXIS2_CALL
axis2_wsdl_binding_operation_get_output(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_operation)->output;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_output(axis2_wsdl_binding_operation_t *binding_operation,
                                        axis2_env_t **env,
                                        struct axis2_wsdl_binding_msg_ref *output) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, output, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(binding_operation)->output = output;
    return AXIS2_SUCCESS;
}

axis2_qname_t *AXIS2_CALL
axis2_wsdl_binding_operation_get_qname(axis2_wsdl_binding_operation_t *binding_operation,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_operation)->qname;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_qname(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_qname_t *qname) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(binding_operation)->qname = qname;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_add_infault(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_wsdl_binding_fault_t *infault) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, infault, AXIS2_FAILURE);
    
    return AXIS2_LINKED_LIST_ADD(AXIS2_INTF_TO_IMPL(binding_operation)->infaults,
        env, infault);
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_add_outfault(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_wsdl_binding_fault_t *outfault) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outfault, AXIS2_FAILURE);
    
    return AXIS2_LINKED_LIST_ADD(AXIS2_INTF_TO_IMPL(binding_operation)->outfaults,
        env, outfault);
}

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_operation_get_infaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_operation)->infaults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_infaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_linked_list_t *infaults) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, infaults, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(binding_operation)->infaults = infaults;
    return AXIS2_SUCCESS;
}

axis2_linked_list_t * AXIS2_CALL
axis2_wsdl_binding_operation_get_outfaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_operation)->outfaults;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_outfaults(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_linked_list_t *outfaults) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outfaults, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(binding_operation)->outfaults = outfaults;
    return AXIS2_SUCCESS;
}
