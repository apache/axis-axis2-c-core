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
    axis2_wsdl_operation_t *operation;

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

axis2_wsdl_operation_t *AXIS2_CALL
axis2_wsdl_binding_operation_get_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_wsdl_operation_t *operation);

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
    
    
	binding_operation_impl->qname = NULL;
    binding_operation_impl->operation = NULL;
    binding_operation_impl->input = NULL;
    binding_operation_impl->output = NULL;
	
	return &(binding_operation_impl->binding_operation);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_wsdl_binding_operation_free (axis2_wsdl_binding_operation_t *binding_operation, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
	if(NULL != binding_operation->ops)
        AXIS2_FREE((*env)->allocator, binding_operation->ops);
    
    if(NULL != AXIS2_INTF_TO_IMPL(binding_operation)->qname)
       AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(binding_operation)->qname, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(binding_operation)->infaults)
       AXIS2_LINKED_LIST_FREE(AXIS2_INTF_TO_IMPL(binding_operation)->infaults, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(binding_operation)->outfaults)
       AXIS2_LINKED_LIST_FREE(AXIS2_INTF_TO_IMPL(binding_operation)->outfaults, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(binding_operation)->operation)
       AXIS2_WSDL_OPERATION_FREE(AXIS2_INTF_TO_IMPL(binding_operation)->operation, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(binding_operation)->input)
       AXIS2_WSDL_BINDING_MSG_REF_FREE(AXIS2_INTF_TO_IMPL(binding_operation)->input, env);
    
    if(NULL != binding_operation->extensible_component)
        AXIS2_WSDL_EXTENSIBLE_COMPONENT_FREE(binding_operation->
            extensible_component, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(binding_operation)->output)
       AXIS2_WSDL_BINDING_MSG_REF_FREE(AXIS2_INTF_TO_IMPL(binding_operation)->output, env);
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(binding_operation));
    
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

axis2_wsdl_operation_t *AXIS2_CALL
axis2_wsdl_binding_operation_get_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(binding_operation)->operation;
}

axis2_status_t AXIS2_CALL
axis2_wsdl_binding_operation_set_operation(axis2_wsdl_binding_operation_t *binding_operation,
                                            axis2_env_t **env,
                                            axis2_wsdl_operation_t *operation) 
{
    AXIS2_FUNC_PARAM_CHECK(binding_operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, operation, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(binding_operation)->operation = operation;
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
