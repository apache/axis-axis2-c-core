#include <axis2_phases_info.h>

/** 
 * @brief Phases Info struct impl
 *	Axis2 Phases Info impl  
 */
typedef struct axis2_phases_info_impl
{
	axis2_phases_info_t phases_info;
    axis2_array_list_t *inphases;
    axis2_array_list_t *outphases;
    axis2_array_list_t *in_faultphases;
    axis2_array_list_t *out_faultphases;
    
} axis2_phases_info_impl_t;

#define AXIS2_INTF_TO_IMPL(phases_info) \
        ((axis2_phases_info_impl_t *)(phases_info))

/***************************** Function headers *******************************/

axis2_status_t AXIS2_CALL
axis2_phases_info_free (axis2_phases_info_t *phases_info,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_inphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env,
                                axis2_array_list_t *inphases);

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_outphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env,
                                axis2_array_list_t *outphases);

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_in_faultphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env,
                                axis2_array_list_t *in_faultphases);
                                
axis2_status_t AXIS2_CALL 
axis2_phases_info_set_out_faultphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env,
                                        axis2_array_list_t * out_faultphases);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_inphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_outphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_in_faultphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_out_faultphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_inphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_outphases(axis2_phases_info_t *phases_info,
                                            axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_in_faultphases(axis2_phases_info_t *phases_info,
                                                axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_out_faultphases(axis2_phases_info_t *phases_info,
                                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_operation_phases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env,
                                        struct axis2_operation *axis2_opt);
                                    
/***************************** End of function headers ************************/

axis2_phases_info_t * AXIS2_CALL 
axis2_phases_info_create (axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    
	axis2_phases_info_impl_t *phases_info_impl = 
        (axis2_phases_info_impl_t *) AXIS2_MALLOC ((*env)->allocator, 
        sizeof(axis2_phases_info_impl_t));
    
	if(NULL == phases_info_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
       
    phases_info_impl->inphases = NULL;
    phases_info_impl->outphases = NULL;
    phases_info_impl->in_faultphases = NULL;
    phases_info_impl->out_faultphases = NULL;
	
	phases_info_impl->phases_info.ops = (axis2_phases_info_ops_t *)
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_phases_info_ops_t));
    
	if(NULL == phases_info_impl->phases_info.ops)
	{
		AXIS2_FREE ((*env)->allocator, phases_info_impl);
        phases_info_impl = NULL;
		AXIS2_ERROR_SET ((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);	
	}
    
	phases_info_impl->phases_info.ops->free = axis2_phases_info_free;
    
	phases_info_impl->phases_info.ops->set_inphases =
        axis2_phases_info_set_inphases;
    
    phases_info_impl->phases_info.ops->set_outphases =
        axis2_phases_info_set_outphases;
    
    phases_info_impl->phases_info.ops->set_in_faultphases =
        axis2_phases_info_set_in_faultphases;
    
    phases_info_impl->phases_info.ops->set_out_faultphases =
        axis2_phases_info_set_out_faultphases;
    
    phases_info_impl->phases_info.ops->get_inphases = 
            axis2_phases_info_get_inphases;
    
    phases_info_impl->phases_info.ops->get_outphases = 
            axis2_phases_info_get_outphases;
	
    phases_info_impl->phases_info.ops->get_in_faultphases = 
            axis2_phases_info_get_in_faultphases;
    
    phases_info_impl->phases_info.ops->get_out_faultphases = 
            axis2_phases_info_get_out_faultphases;

    phases_info_impl->phases_info.ops->get_operation_inphases = 
            axis2_phases_info_get_operation_inphases;
                                        
    phases_info_impl->phases_info.ops->get_operation_outphases = 
            axis2_phases_info_get_operation_outphases;                                   
                                        
    phases_info_impl->phases_info.ops->get_operation_in_faultphases = 
            axis2_phases_info_get_operation_in_faultphases;                                    
    
    phases_info_impl->phases_info.ops->get_operation_out_faultphases = 
            axis2_phases_info_get_operation_out_faultphases;

    phases_info_impl->phases_info.ops->set_operation_phases = 
            axis2_phases_info_set_operation_phases;                                      
    
	return &(phases_info_impl->phases_info);
}

/******************************************************************************/

axis2_status_t AXIS2_CALL 
axis2_phases_info_free (axis2_phases_info_t *phases_info, 
                            axis2_env_t **env)
{
    axis2_phases_info_impl_t *phases_info_impl = NULL;
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, AXIS2_FAILURE);
    phases_info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    
	if(NULL != phases_info->ops)
    {
        AXIS2_FREE((*env)->allocator, phases_info->ops);
        phases_info->ops = NULL;
    }
    
    if(NULL != phases_info_impl)
    {
        AXIS2_FREE((*env)->allocator, phases_info_impl);
        phases_info_impl = NULL;
    }
    
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_phases_info_set_inphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env,
                                axis2_array_list_t *inphases) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, inphases, AXIS2_FAILURE);
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    if(info_impl->inphases)
        AXIS2_ARRAY_LIST_FREE(info_impl->inphases, env);
    
    info_impl->inphases = inphases;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_outphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env,
                                axis2_array_list_t *outphases) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outphases, AXIS2_FAILURE);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    if(info_impl->outphases)
        AXIS2_ARRAY_LIST_FREE(info_impl->outphases, env);
    info_impl->outphases = outphases;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_in_faultphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env,
                                axis2_array_list_t *in_faultphases) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, in_faultphases, AXIS2_FAILURE);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    if(info_impl->in_faultphases)
        AXIS2_ARRAY_LIST_FREE(info_impl->in_faultphases, env);
    info_impl->in_faultphases = in_faultphases;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_out_faultphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env,
                                        axis2_array_list_t * out_faultphases) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, out_faultphases, AXIS2_FAILURE);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    if(info_impl->out_faultphases)
        AXIS2_ARRAY_LIST_FREE(info_impl->out_faultphases, env);
    
    info_impl->out_faultphases = out_faultphases;
    return AXIS2_SUCCESS;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_inphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(phases_info)->inphases;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_outphases(axis2_phases_info_t *phases_info,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(phases_info)->outphases;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_in_faultphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(phases_info)->in_faultphases;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_out_faultphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(phases_info)->out_faultphases;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_inphases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    struct axis2_phase *phase = NULL;
    int i = 0; 
    axis2_char_t *phase_name = NULL;
    axis2_array_list_t * operation_inphases = NULL; 
    axis2_status_t status = AXIS2_FAILURE;    
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    operation_inphases = axis2_array_list_create(env, 0);
    phase = axis2_phase_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    
    status = AXIS2_ARRAY_LIST_ADD(operation_inphases, env, phase);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_PHASE_FREE(phase, env);
        phase = NULL;
        AXIS2_ARRAY_LIST_FREE(operation_inphases, env);
        operation_inphases = NULL;
        return NULL;
    }
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(info_impl->inphases, env); i++) 
    {
        phase_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(info_impl->inphases, env, i);
        if (0 == AXIS2_STRCMP(AXIS2_PHASE_TRANSPORTIN, phase_name) ||
                0 == AXIS2_STRCMP(AXIS2_PHASE_PRE_DISPATCH, phase_name) ||
                0 == AXIS2_STRCMP(AXIS2_PHASE_DISPATCH, phase_name) ||
                0 == AXIS2_STRCMP(AXIS2_PHASE_POST_DISPATCH, phase_name)) 
        {
            /* Do nothing */
        } else 
        {
            phase = axis2_phase_create(env, phase_name);
            status = AXIS2_ARRAY_LIST_ADD(operation_inphases, env, phase);
            if(AXIS2_FAILURE == status)
            {
                AXIS2_PHASE_FREE(phase, env);
                phase = NULL;
                AXIS2_ARRAY_LIST_FREE(operation_inphases, env);
                operation_inphases = NULL;
                return NULL;
            }
        }
    }
    return operation_inphases;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_outphases(axis2_phases_info_t *phases_info,
                                            axis2_env_t **env) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    struct axis2_phase *phase = NULL;
    int i = 0; 
    axis2_char_t *phase_name = NULL;
    axis2_array_list_t * operation_outphases = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    operation_outphases = axis2_array_list_create(env, 0);
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(info_impl->outphases, env); i++) 
    {
        phase_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(info_impl->outphases, env, i);
        if (0 == AXIS2_STRCMP(AXIS2_PHASE_TRANSPORT_OUT, phase_name)) 
        {
            /* Do Nothing */
        } else 
        {
            phase = axis2_phase_create(env, phase_name);
            status = AXIS2_ARRAY_LIST_ADD(operation_outphases, env, phase);
            if(AXIS2_FAILURE == status)
            {
                AXIS2_PHASE_FREE(phase, env);
                phase = NULL;
                AXIS2_ARRAY_LIST_FREE(operation_outphases, env);
                operation_outphases = NULL;
                return NULL;
            }
        }
    }
    phase = axis2_phase_create(env, AXIS2_PHASE_POLICY_DETERMINATION);
    status = AXIS2_ARRAY_LIST_ADD(operation_outphases, env, phase);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_PHASE_FREE(phase, env);
        phase = NULL;
        AXIS2_ARRAY_LIST_FREE(operation_outphases, env);
        operation_outphases = NULL;
        return NULL;
    }
    phase = axis2_phase_create(env, AXIS2_PHASE_MESSAGE_OUT);
    status = AXIS2_ARRAY_LIST_ADD(operation_outphases, env, phase);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_PHASE_FREE(phase, env);
        phase = NULL;
        AXIS2_ARRAY_LIST_FREE(operation_outphases, env);
        operation_outphases = NULL;
        return NULL;
        
    }
    return operation_outphases;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_in_faultphases(axis2_phases_info_t *phases_info,
                                                axis2_env_t **env) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *phase_name = NULL;
    axis2_array_list_t * operation_in_faultphases = NULL;
    struct axis2_phase *phase = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    operation_in_faultphases = axis2_array_list_create(env, 0);
    
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(info_impl->in_faultphases, env); i++) 
    {
        phase_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(info_impl->in_faultphases, env, i);
        phase = axis2_phase_create(env, phase_name);
        status = AXIS2_ARRAY_LIST_ADD(operation_in_faultphases, env, phase);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_PHASE_FREE(phase, env);
            phase = NULL;
            AXIS2_ARRAY_LIST_FREE(operation_in_faultphases, env);
            operation_in_faultphases = NULL;
            return NULL;
            
        }
    }
    return operation_in_faultphases;
}

axis2_array_list_t *AXIS2_CALL 
axis2_phases_info_get_operation_out_faultphases(axis2_phases_info_t *phases_info,
                                                axis2_env_t **env) 
{
    axis2_phases_info_impl_t *info_impl = NULL;
    int i = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_char_t *phase_name = NULL;
    axis2_array_list_t * operation_out_faultphases = NULL;
    struct axis2_phase *phase = NULL;
        
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, NULL);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    operation_out_faultphases = axis2_array_list_create(env, 0);
    
    for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(info_impl->out_faultphases, env); i++) 
    {
        phase_name = (axis2_char_t *) AXIS2_ARRAY_LIST_GET(info_impl->out_faultphases, env, i);
        phase = axis2_phase_create(env, phase_name);
        status = AXIS2_ARRAY_LIST_ADD(operation_out_faultphases, env, phase);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_PHASE_FREE(phase, env);
            phase = NULL;
            AXIS2_ARRAY_LIST_FREE(operation_out_faultphases, env);
            operation_out_faultphases = NULL;
            return NULL;
            
        }
    }
    return operation_out_faultphases;
}

axis2_status_t AXIS2_CALL 
axis2_phases_info_set_operation_phases(axis2_phases_info_t *phases_info,
                                        axis2_env_t **env,
                                        struct axis2_operation *axis2_opt) 
{
    axis2_status_t status = AXIS2_FAILURE;
    axis2_array_list_t *operation_inphases = NULL;
    axis2_array_list_t *operation_outphases = NULL;
    axis2_array_list_t *operation_in_faultphases = NULL;
    axis2_array_list_t *operation_out_faultphases = NULL;
    
    axis2_phases_info_impl_t *info_impl = NULL;
    
    AXIS2_FUNC_PARAM_CHECK(phases_info, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, axis2_opt, AXIS2_FAILURE);
    
    info_impl = AXIS2_INTF_TO_IMPL(phases_info);
    
    operation_inphases = axis2_phases_info_get_operation_inphases(phases_info, env);
    if(NULL == operation_inphases)
        return AXIS2_FAILURE;
    
    operation_outphases = axis2_phases_info_get_operation_outphases(phases_info, env);
    if(NULL == operation_outphases)
        return AXIS2_FAILURE;
    
    operation_in_faultphases = axis2_phases_info_get_operation_in_faultphases(phases_info, env);
    if(NULL == operation_in_faultphases)
        return AXIS2_FAILURE;
    
    operation_out_faultphases = axis2_phases_info_get_operation_out_faultphases(phases_info, env);
    if(NULL == operation_out_faultphases)
        return AXIS2_FAILURE;
    
    status = AXIS2_OPERATION_SET_REMAINING_PHASES_INFLOW(axis2_opt, env, operation_inphases);
    if(AXIS2_FAILURE == status)
        return status;
    status = AXIS2_OPERATION_SET_PHASES_OUTFLOW(axis2_opt, env, operation_outphases);
    if(AXIS2_FAILURE == status)
        return status;
    status = AXIS2_OPERATION_SET_PHASES_IN_FAULT_FLOW(axis2_opt, env, operation_in_faultphases);
    if(AXIS2_FAILURE == status)
        return status;
    status = AXIS2_OPERATION_SET_PHASES_OUT_FAULT_FLOW(axis2_opt, env, operation_out_faultphases);
    
    return status;
    
}
