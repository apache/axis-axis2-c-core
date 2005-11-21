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
 
#include <axis2_module_desc.h>

/** 
 * @brief Module Descripton struct impl
 *	Module Descripton 
 */ 
typedef struct axis2_module_desc_impl
{
	axis2_module_desc_t module_desc;
    
    axis2_qname_t *qname;
    
    axis2_engine_config_t *parent;
    
    /** 
     * To store module opeartions , which are suppose to be added to a service 
     * if it is engaged to a service
     */
    axis2_hash_t *operations;

} axis2_module_desc_impl_t;

#define AXIS2_INTF_TO_IMPL(module_desc) ((axis2_module_desc_impl_t *)module_desc)

/************************* Function prototypes ********************************/

axis2_status_t AXIS2_CALL
axis2_module_desc_free(axis2_module_desc_t *module_desc,
                            axis2_env_t **env);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_in_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_in_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *in_flow);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_out_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_out_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *out_flow);

axis2_flow_t * AXIS2_CALL
axis2_module_desc_get_fault_in_flow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_in_flow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_flow_t *falut_in_flow);

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_out_flow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_out_flow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env,
                                            axis2_flow_t *fault_out_flow);

axis2_qname_t * AXIS2_CALL
axis2_module_desc_get_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname);

axis2_status_t AXIS2_CALL
axis2_module_desc_add_operation (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_operation_t *operation);

axis2_hash_t * AXIS2_CALL
axis2_module_desc_get_operations (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_engine_config_t * AXIS2_CALL
axis2_module_desc_get_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_module_desc_set_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_engine_config_t *parent);

axis2_status_t AXIS2_CALL
axis2_module_desc_add_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_module_desc_get_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    const axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_module_desc_get_params (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_module_desc_is_param_locked (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    const axis2_char_t *param_name);

/************************** End of function prototypes ************************/

axis2_module_desc_t * AXIS2_CALL 
axis2_module_desc_create (axis2_env_t **env)
{
	AXIS2_ENV_CHECK(env, NULL);
	
	axis2_module_desc_impl_t *module_desc_impl = 
		(axis2_module_desc_impl_t *) AXIS2_MALLOC((*env)->allocator,
			sizeof(axis2_module_desc_impl_t));
		
	if(NULL == module_desc_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL); 
    
    module_desc_impl->module_desc.params = axis2_param_container_create(env);
    if(NULL == module_desc_impl->module_desc.params)
    {
        AXIS2_FREE((*env)->allocator, module_desc_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    module_desc_impl->module_desc.flow_container = axis2_flow_container_create(env);
    if(NULL == module_desc_impl->module_desc.flow_container)
    {
        AXIS2_PARAM_CONTAINER_FREE(module_desc_impl->module_desc.params, env);
        AXIS2_FREE((*env)->allocator, module_desc_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
    module_desc_impl->operations = axis2_hash_make(env);
    if(NULL == module_desc_impl->operations)
    {
        AXIS2_PARAM_CONTAINER_FREE(module_desc_impl->module_desc.params, env);
        AXIS2_FLOW_CONTAINER_FREE(module_desc_impl->module_desc.flow_container, env); 
        AXIS2_FREE((*env)->allocator, module_desc_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);        
    }
   
	module_desc_impl->module_desc.ops = 
		AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_module_desc_ops_t));
	if(NULL == module_desc_impl->module_desc.ops)
    {
        AXIS2_PARAM_CONTAINER_FREE(module_desc_impl->module_desc.params, env);
        AXIS2_FLOW_CONTAINER_FREE(module_desc_impl->module_desc.flow_container, env);
        axis2_hash_free(module_desc_impl->operations, env);
        AXIS2_FREE((*env)->allocator, module_desc_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    }
    
	module_desc_impl->module_desc.ops->free = axis2_module_desc_free;
    module_desc_impl->module_desc.ops->get_in_flow = axis2_module_desc_get_in_flow;
    module_desc_impl->module_desc.ops->set_in_flow = axis2_module_desc_set_in_flow;
    module_desc_impl->module_desc.ops->get_out_flow = axis2_module_desc_get_out_flow;
    module_desc_impl->module_desc.ops->set_out_flow = axis2_module_desc_set_out_flow;
    
    module_desc_impl->module_desc.ops->get_fault_in_flow = 
        axis2_module_desc_get_fault_in_flow;
    
    module_desc_impl->module_desc.ops->set_fault_in_flow = 
        axis2_module_desc_set_fault_in_flow;
    
    module_desc_impl->module_desc.ops->get_fault_out_flow = 
        axis2_module_desc_get_fault_out_flow;
    
    module_desc_impl->module_desc.ops->set_fault_out_flow = 
        axis2_module_desc_set_fault_out_flow;
    
    module_desc_impl->module_desc.ops->get_name = axis2_module_desc_get_name;
    module_desc_impl->module_desc.ops->set_name = axis2_module_desc_set_name;
    
    module_desc_impl->module_desc.ops->add_operation = 
        axis2_module_desc_add_operation;
    
    module_desc_impl->module_desc.ops->get_operations = 
        axis2_module_desc_get_operations;
    
    module_desc_impl->module_desc.ops->get_parent = axis2_module_desc_get_parent;
    module_desc_impl->module_desc.ops->set_parent = axis2_module_desc_set_parent;
    module_desc_impl->module_desc.ops->add_param = axis2_module_desc_add_param;
    module_desc_impl->module_desc.ops->get_param = axis2_module_desc_get_param;
    module_desc_impl->module_desc.ops->get_params = axis2_module_desc_get_params;
    
    module_desc_impl->module_desc.ops->is_param_locked = 
        axis2_module_desc_is_param_locked;
	
    module_desc_impl->qname = NULL;

    module_desc_impl->parent = NULL;	
    
	return &(module_desc_impl->module_desc);
}

axis2_module_desc_t * AXIS2_CALL 
axis2_module_desc_create_with_qname (axis2_env_t **env, axis2_qname_t *qname)
{
	AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, NULL);
	
	axis2_module_desc_impl_t *module_desc_impl = 
        AXIS2_INTF_TO_IMPL(axis2_module_desc_create(env));
    if(NULL == module_desc_impl)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
    
    module_desc_impl->qname = qname;
		
	return &(module_desc_impl->module_desc);
}


/*************************** Start of operation impls *************************/

axis2_status_t AXIS2_CALL
axis2_module_desc_free(axis2_module_desc_t *module_desc,
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    
    if(NULL != module_desc->ops)
        AXIS2_FREE((*env)->allocator, module_desc->ops);
    
    if(NULL == module_desc->params)
        AXIS2_PARAM_CONTAINER_FREE(module_desc->params, env);
    
    if(NULL == module_desc->flow_container)
        AXIS2_FLOW_CONTAINER_FREE(module_desc->flow_container, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(module_desc)->parent)
        AXIS2_ENGINE_CONFIG_FREE(AXIS2_INTF_TO_IMPL(module_desc)->parent, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(module_desc)->qname)
        AXIS2_QNAME_FREE(AXIS2_INTF_TO_IMPL(module_desc)->qname, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(module_desc)->operations)
        axis2_hash_free(AXIS2_INTF_TO_IMPL(module_desc)->operations, env);
    
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(module_desc));
    
    return AXIS2_SUCCESS;
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_in_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_FLOW_CONTAINER_GET_IN_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_in_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *in_flow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, in_flow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_IN_FLOW(module_desc->flow_container, env, 
        in_flow);    
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_out_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_FLOW_CONTAINER_GET_OUT_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_out_flow(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_flow_t *out_flow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, out_flow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_OUT_FLOW(module_desc->flow_container, env, 
        out_flow);
}

axis2_flow_t * AXIS2_CALL
axis2_module_desc_get_fault_in_flow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_FLOW_CONTAINER_GET_FAULT_IN_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_in_flow(axis2_module_desc_t *module_desc,
                                        axis2_env_t **env,
                                        axis2_flow_t *falut_in_flow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, falut_in_flow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_FAULT_IN_FLOW(module_desc->flow_container,
        env, falut_in_flow);
}

axis2_flow_t *AXIS2_CALL
axis2_module_desc_get_fault_out_flow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    return AXIS2_FLOW_CONTAINER_GET_FAULT_OUT_FLOW(module_desc->flow_container, env);
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_fault_out_flow(axis2_module_desc_t *module_desc,
                                            axis2_env_t **env,
                                            axis2_flow_t *fault_out_flow)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, fault_out_flow, AXIS2_FAILURE);
    
    return AXIS2_FLOW_CONTAINER_SET_FAULT_OUT_FLOW(module_desc->flow_container, 
        env, fault_out_flow);
}

axis2_qname_t * AXIS2_CALL
axis2_module_desc_get_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_INTF_TO_IMPL(module_desc)->qname;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_name (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_qname_t *qname)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(module_desc)->qname = qname;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_add_operation (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_operation_t *operation)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, operation, AXIS2_FAILURE);
    
    if (NULL == (AXIS2_INTF_TO_IMPL(module_desc)->operations))
	{                    
		AXIS2_INTF_TO_IMPL(module_desc)->operations = axis2_hash_make (env);
	}	
    
    axis2_hash_set(AXIS2_INTF_TO_IMPL(module_desc)->operations, 
        AXIS2_OPERATION_GET_NAME(operation, env), sizeof(axis2_qname_t),
            operation);
    
    return AXIS2_SUCCESS;
}

axis2_hash_t * AXIS2_CALL
axis2_module_desc_get_operations (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(module_desc)->operations;
}

axis2_engine_config_t * AXIS2_CALL
axis2_module_desc_get_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    return AXIS2_INTF_TO_IMPL(module_desc)->parent;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_set_parent (axis2_module_desc_t *module_desc,
                                axis2_env_t **env,
                                axis2_engine_config_t *parent)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, parent, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(module_desc)->parent = parent;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_module_desc_add_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    axis2_param_t *param)
{
    axis2_char_t *param_name_l = NULL;
    axis2_status_t ret_status = AXIS2_FAILURE;
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FAILURE);
    
    param_name_l = AXIS2_PARAM_GET_NAME(param, env);
    if(NULL == param_name_l)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_PARAM, 
            AXIS2_FAILURE);
    if(AXIS2_TRUE == axis2_module_desc_is_param_locked(module_desc, env, param_name_l))
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
    }
    else
    {        
        ret_status = AXIS2_PARAM_CONTAINER_ADD_PARAM(module_desc->params, env, param);
    } 
    return ret_status;    
}

axis2_param_t * AXIS2_CALL
axis2_module_desc_get_param(axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    const axis2_char_t *name)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, name, NULL);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAM(module_desc->params, env, name);
}

axis2_array_list_t * AXIS2_CALL
axis2_module_desc_get_params (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, NULL);
    
    return AXIS2_PARAM_CONTAINER_GET_PARAMS(module_desc->params, env);
}

axis2_bool_t AXIS2_CALL
axis2_module_desc_is_param_locked (axis2_module_desc_t *module_desc,
                                    axis2_env_t **env,
                                    const axis2_char_t *param_name)
{
    axis2_bool_t locked = AXIS2_FALSE;
    axis2_bool_t ret_state = AXIS2_FALSE;
    axis2_param_t *param_l = NULL;
    AXIS2_FUNC_PARAM_CHECK(module_desc, env, AXIS2_FALSE);
    
    
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
    /* checking the locked value of parent*/
    if(NULL != AXIS2_INTF_TO_IMPL(module_desc)->parent)
        locked = AXIS2_ENGINE_CONFIG_IS_PARAM_LOCKED(AXIS2_INTF_TO_IMPL(
                    module_desc)->parent, env, param_name);
    
    if(AXIS2_TRUE == locked)
    {
        ret_state = AXIS2_TRUE;
    }
    else
    {
        param_l = axis2_module_desc_get_param(module_desc, env, 
            param_name);
        if(NULL != param_l && AXIS2_TRUE == AXIS2_PARAM_IS_LOCKED(param_l, env))
            ret_state = AXIS2_TRUE;
        else
            ret_state = AXIS2_FALSE;
        
    }  
    return ret_state;    
}
