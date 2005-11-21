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
 
#include <axis2_operation.h>

/** 
 * @brief Operaton struct impl
 *	Axis2 Operations  
 */ 
typedef struct axis2_operation_impl
{
	axis2_operation_t operation;
    
	struct axis2_svc *parent;
	struct axis2_msg_recv *msg_recv;
        
    axis2_array_list_t *remaining_phases_in_flow;
    axis2_array_list_t *phases_out_flow;
    axis2_array_list_t *phases_in_fault_flow;
    axis2_array_list_t *phases_out_fault_flow;
    int mep;
    /*To store deploytime module refs */
    axis2_array_list_t *modulerefs;
    
} axis2_operation_impl_t;

#define AXIS2_INTF_TO_IMPL(operation) ((axis2_operation_impl_t *)operation)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_operation_free (axis2_operation_t *operation, 
                        axis2_env_t **env);	

axis2_status_t AXIS2_CALL 
axis2_operation_add_param (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_operation_get_param (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            const axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_operation_get_params (axis2_operation_t *operation, 
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_operation_is_param_locked(axis2_operation_t *operation, 
                                    axis2_env_t **env
    ,                               const axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_operation_set_parent (axis2_operation_t *operation, 
                                axis2_env_t **env,
                                axis2_svc_t *svc);

axis2_svc_t * AXIS2_CALL
axis2_operation_get_parent (axis2_operation_t *operation, 
                                axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_operation_set_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env,
                                struct axis2_msg_recv *msg_recv);

struct axis2_msg_recv * AXIS2_CALL
axis2_operation_get_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_name (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            axis2_qname_t *qname);
                            
axis2_qname_t * AXIS2_CALL
axis2_operation_get_name (axis2_operation_t *operation, 
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env,
                                            const axis2_char_t *pattern);
		
axis2_char_t * AXIS2_CALL
axis2_operation_get_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env);

axis2_char_t * AXIS2_CALL
axis2_operation_get_style (axis2_operation_t *operation,
                            axis2_env_t **env);

axis2_status_t  AXIS2_CALL
axis2_operation_set_style (axis2_operation_t *operation,
                            axis2_env_t **env,
                            axis2_char_t *style);

axis2_status_t AXIS2_CALL
axis2_operation_engage_module(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_module_desc_t *moduleref);

axis2_status_t AXIS2_CALL
axis2_operation_add_to_engage_module_list(axis2_operation_t *operation,
                                            axis2_env_t **env,
                                            axis2_module_desc_t *module_name);

axis2_array_list_t *AXIS2_CALL
axis2_operation_get_modules(axis2_operation_t *operation,
                                axis2_env_t **env);

int AXIS2_CALL
axis2_operation_get_axis_specific_MEP_constant(axis2_operation_t *operation,
                                                axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_operation_get_phases_in_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env);


axis2_array_list_t *AXIS2_CALL
axis2_operation_get_phases_out_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_operation_get_phases_out_flow(axis2_operation_t *operation,
                                        axis2_env_t **env);


axis2_array_list_t *AXIS2_CALL
axis2_operation_get_remaining_phases_in_flow(axis2_operation_t *operation,
                                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_phases_in_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_operation_set_phases_out_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_operation_set_phases_out_flow(axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_operation_set_remaining_phases_in_flow(axis2_operation_t *operation,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_operation_add_module(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_qname_t *module_name);

axis2_array_list_t *AXIS2_CALL
axis2_operation_get_module_refs(axis2_operation_t *operation,
                                    axis2_env_t **env);

axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_infaults(axis2_operation_t *operation,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_infaults(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_linked_list_t *infaults);

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_operation_get_input_msg(axis2_operation_t *operation,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_input_msg(axis2_operation_t *operation,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *input_msg); 


axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_outfaults(axis2_operation_t *operation,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_outfaults(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_linked_list_t *outfaults);

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_operation_get_output_msg(axis2_operation_t *operation,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_output_msg(axis2_operation_t *operation,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *output_msg); 


axis2_bool_t AXIS2_CALL
axis2_operation_is_safe(axis2_operation_t *operation,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_set_safety(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_bool_t safe);

axis2_char_t *AXIS2_CALL
axis2_operation_get_target_namespace(axis2_operation_t *operation,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_operation_add_infault(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *infault);

axis2_status_t AXIS2_CALL
axis2_operation_add_outfault(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *outfault);

axis2_status_t AXIS2_CALL
axis2_operation_add_feature(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_feature_t *feature);

axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_features(axis2_operation_t *operation,
                                axis2_env_t **env); 


axis2_status_t AXIS2_CALL
axis2_operation_add_property(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_property_t *wsdl_property);

axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_Properties(axis2_operation_t *operation,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_operation_set_wsdl_opeartion(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_operation_t *wsdl_operation);

/************************* End of function headers ****************************/	

axis2_operation_t * AXIS2_CALL
axis2_operation_create (axis2_env_t **env)
{
    struct axis2_param_container *param_container_l = NULL;
    axis2_array_list_t *array_list_l = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
	axis2_operation_impl_t *operation_impl = 
		(axis2_operation_impl_t *) AXIS2_MALLOC ((*env)->allocator,
		sizeof (axis2_operation_impl_t));
     
	if(NULL == operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    operation_impl->operation.param_container = (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == operation_impl->operation.param_container)
	{
        AXIS2_FREE((*env)->allocator, operation_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

	operation_impl->operation.wsdl_operation = (axis2_wsdl_operation_t *)
		axis2_wsdl_operation_create(env);		
	if(NULL == operation_impl->operation.wsdl_operation)
	{
        AXIS2_PARAM_CONTAINER_FREE(operation_impl->operation.param_container, env);
        AXIS2_FREE((*env)->allocator, operation_impl);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);		
	}

    operation_impl->remaining_phases_in_flow = axis2_array_list_create(env, 0);
    if(NULL == operation_impl->remaining_phases_in_flow)
        axis2_operation_free(&(operation_impl->operation), env);
    
    operation_impl->phases_out_flow = axis2_array_list_create(env, 0);
    if(NULL == operation_impl->phases_out_flow)
        axis2_operation_free(&(operation_impl->operation), env);
    
    operation_impl->phases_in_fault_flow = axis2_array_list_create(env, 0);
    if(NULL == operation_impl->phases_in_fault_flow)
        axis2_operation_free(&(operation_impl->operation), env);
    
    operation_impl->modulerefs = axis2_array_list_create(env, 0);
    if(NULL == operation_impl->modulerefs)
        axis2_operation_free(&(operation_impl->operation), env);
    
    operation_impl->phases_out_fault_flow = axis2_array_list_create(env, 0);
    if(NULL == operation_impl->phases_out_fault_flow)
        axis2_operation_free(&(operation_impl->operation), env);
    
    operation_impl->operation.ops = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_operation_ops_t));
	if(NULL == operation_impl->operation.ops)
	{
        AXIS2_PARAM_CONTAINER_FREE(operation_impl->operation.param_container, env);
        AXIS2_WSDL_OPERATION_FREE(operation_impl->operation.wsdl_operation, env);
        AXIS2_FREE((*env)->allocator, operation_impl);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
	
	operation_impl->operation.ops->free = axis2_operation_free;
	operation_impl->operation.ops->add_param = axis2_operation_add_param;
	operation_impl->operation.ops->get_param = axis2_operation_get_param;
	operation_impl->operation.ops->get_params = axis2_operation_get_params;
	operation_impl->operation.ops->set_parent = axis2_operation_set_parent;
	operation_impl->operation.ops->get_parent = axis2_operation_get_parent;
  
	operation_impl->operation.ops->set_msg_recv = axis2_operation_set_msg_recv;
	operation_impl->operation.ops->get_msg_recv = axis2_operation_get_msg_recv;
    
    operation_impl->operation.ops->set_name = axis2_operation_set_name;
	operation_impl->operation.ops->get_name = axis2_operation_get_name;
    
	operation_impl->operation.ops->set_msg_exchange_pattern 
		= axis2_operation_set_msg_exchange_pattern;
    
	operation_impl->operation.ops->get_msg_exchange_pattern
		= axis2_operation_get_msg_exchange_pattern;
        
    operation_impl->operation.ops->set_style = axis2_operation_set_style;
    
	operation_impl->operation.ops->get_style = axis2_operation_get_style; 

    operation_impl->operation.ops->engage_module = axis2_operation_engage_module;
    operation_impl->operation.ops->add_to_engage_module_list = axis2_operation_add_to_engage_module_list;
    operation_impl->operation.ops->get_modules = axis2_operation_get_modules;
    operation_impl->operation.ops->get_axis_specific_MEP_constant = axis2_operation_get_axis_specific_MEP_constant;
    operation_impl->operation.ops->get_phases_in_fault_flow = axis2_operation_get_phases_in_fault_flow;
    operation_impl->operation.ops->get_phases_out_fault_flow = axis2_operation_get_phases_out_fault_flow;
    operation_impl->operation.ops->get_phases_out_flow = axis2_operation_get_phases_out_flow;
    operation_impl->operation.ops->get_remaining_phases_in_flow = axis2_operation_get_remaining_phases_in_flow;
    operation_impl->operation.ops->set_phases_in_fault_flow = axis2_operation_set_phases_in_fault_flow;
    operation_impl->operation.ops->set_phases_out_fault_flow = axis2_operation_set_phases_out_fault_flow;
    operation_impl->operation.ops->set_phases_out_flow = axis2_operation_set_phases_out_flow;
    operation_impl->operation.ops->set_remaining_phases_in_flow = axis2_operation_set_remaining_phases_in_flow;
    operation_impl->operation.ops->add_module = axis2_operation_add_module;
    operation_impl->operation.ops->get_module_refs = axis2_operation_get_module_refs;
    operation_impl->operation.ops->get_infaults = axis2_operation_get_infaults;
    operation_impl->operation.ops->set_infaults = axis2_operation_set_infaults;
    operation_impl->operation.ops->get_input_msg = axis2_operation_get_input_msg;
    operation_impl->operation.ops->set_input_msg = axis2_operation_set_input_msg;
    operation_impl->operation.ops->get_outfaults = axis2_operation_get_outfaults;
    operation_impl->operation.ops->set_outfaults = axis2_operation_set_outfaults;
    operation_impl->operation.ops->get_output_msg = axis2_operation_get_output_msg;
    operation_impl->operation.ops->set_output_msg = axis2_operation_set_output_msg;
    operation_impl->operation.ops->is_safe = axis2_operation_is_safe;
    operation_impl->operation.ops->set_safety = axis2_operation_set_safety;
    operation_impl->operation.ops->get_target_namespace = axis2_operation_get_target_namespace;
    operation_impl->operation.ops->add_infault = axis2_operation_add_infault;
    operation_impl->operation.ops->add_outfault = axis2_operation_add_outfault;
    operation_impl->operation.ops->add_feature = axis2_operation_add_feature;
    operation_impl->operation.ops->get_features = axis2_operation_get_features;
    operation_impl->operation.ops->add_property = axis2_operation_add_property;
    operation_impl->operation.ops->get_Properties = axis2_operation_get_Properties;
    operation_impl->operation.ops->set_wsdl_opeartion = axis2_operation_set_wsdl_opeartion;


	operation_impl->parent = NULL;
	
	operation_impl->msg_recv = NULL;
    
    operation_impl->mep = MEP_CONSTANT_INVALID;
    
    axis2_operation_set_msg_exchange_pattern(&(operation_impl->operation), env, 
        (axis2_char_t *) AXIS2_STRDUP(MEP_URI_IN_OUT, env));
    
    param_container_l = axis2_param_container_create(env);
    if(NULL == param_container_l)
        return NULL;
    AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(operation_impl->operation.wsdl_operation->
        extensible_component->wsdl_component, env, PARAMETER_KEY, param_container_l);
    
    array_list_l = axis2_array_list_create(env, 0);
    if(NULL == array_list_l) return NULL;
    AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(operation_impl->operation.wsdl_operation->
        extensible_component->wsdl_component, env, MODULEREF_KEY, array_list_l);
						
	return &(operation_impl->operation);
}

axis2_operation_t * AXIS2_CALL
axis2_operation_create_with_name (axis2_env_t **env, axis2_qname_t *qname)
{
	axis2_operation_impl_t *operation_impl = 
        AXIS2_INTF_TO_IMPL(axis2_operation_create(env));
    
	if(NULL == operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);	
	axis2_operation_set_name(&(operation_impl->operation), env, qname);
       
	return &(operation_impl->operation);	
}

axis2_operation_t * AXIS2_CALL
axis2_operation_create_with_wsdl_operation (axis2_env_t **env, 
                                            axis2_wsdl_operation_t *wsdl_operation)
{
	axis2_operation_impl_t *operation_impl = 
        AXIS2_INTF_TO_IMPL(axis2_operation_create(env));
    
	if(NULL == operation_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, NULL);
	}
    
    AXIS2_PARAM_CHECK((*env)->error, wsdl_operation, NULL);	
	if(NULL != operation_impl->operation.wsdl_operation)
        AXIS2_WSDL_OPERATION_FREE(operation_impl->operation.wsdl_operation, env);
    operation_impl->operation.wsdl_operation = wsdl_operation;
       
	return &(operation_impl->operation);	
}

/*************************** Start of operation impls *************************/

axis2_status_t AXIS2_CALL 
axis2_operation_free (axis2_operation_t *operation, axis2_env_t **env)
{ 
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    
	if(NULL != operation->ops)
		AXIS2_FREE((*env)->allocator, operation->ops);
    
    if(NULL != operation->param_container)
	    AXIS2_PARAM_CONTAINER_FREE(operation->param_container
        , env);
    
    if(NULL != operation->wsdl_operation)
	    AXIS2_WSDL_OPERATION_FREE(operation->wsdl_operation
        , env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->parent)
	    AXIS2_SVC_FREE(AXIS2_INTF_TO_IMPL(operation)->parent, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->msg_recv)
	    AXIS2_MSG_RECV_FREE(AXIS2_INTF_TO_IMPL(operation)->msg_recv, env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->remaining_phases_in_flow)
        AXIS2_ARRAY_LIST_FREE(AXIS2_INTF_TO_IMPL(operation)->remaining_phases_in_flow,
            env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->phases_out_flow)
        AXIS2_ARRAY_LIST_FREE(AXIS2_INTF_TO_IMPL(operation)->phases_out_flow,
            env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->phases_in_fault_flow)
        AXIS2_ARRAY_LIST_FREE(AXIS2_INTF_TO_IMPL(operation)->phases_in_fault_flow,
            env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->phases_out_fault_flow)
        AXIS2_ARRAY_LIST_FREE(AXIS2_INTF_TO_IMPL(operation)->phases_out_fault_flow,
            env);
    
    if(NULL != AXIS2_INTF_TO_IMPL(operation)->modulerefs)
        AXIS2_ARRAY_LIST_FREE(AXIS2_INTF_TO_IMPL(operation)->modulerefs, env);
        
    AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(operation));
    
    return AXIS2_SUCCESS;
}	
	
axis2_status_t AXIS2_CALL 
axis2_operation_add_param (axis2_operation_t *operation, 
                                axis2_env_t **env,       
		                        axis2_param_t *param)
{
    axis2_param_container_t *param_container_l = NULL;
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FALSE);
	
    if(AXIS2_TRUE == axis2_operation_is_param_locked(operation, env, 
            AXIS2_PARAM_GET_NAME(param, env)))
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
    }
    else
    {
        param_container_l = (axis2_param_container_t *) 
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                operation->wsdl_operation->extensible_component->wsdl_component, env, 
                (axis2_char_t *) PARAMETER_KEY);
        return AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container_l, env, param);
    }
    
    return AXIS2_SUCCESS;
}

axis2_param_t * AXIS2_CALL
axis2_operation_get_param (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        const axis2_char_t *param_name)
{
    axis2_param_container_t *param_container_l = NULL;
	AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, param_name, NULL);
	
    param_container_l = (axis2_param_container_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(operation->wsdl_operation->
            extensible_component->wsdl_component, env, (axis2_char_t *) PARAMETER_KEY);
	
	return AXIS2_PARAM_CONTAINER_GET_PARAM(param_container_l, env, param_name);
}

axis2_array_list_t * AXIS2_CALL
axis2_operation_get_params(axis2_operation_t *operation, 
                                axis2_env_t **env)
{
    axis2_param_container_t *param_container_l = NULL;
	AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, operation->param_container, 
        AXIS2_FALSE);
	
    param_container_l = (axis2_param_container_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(operation->wsdl_operation->
            extensible_component->wsdl_component, env, (axis2_char_t *) PARAMETER_KEY);
	return AXIS2_PARAM_CONTAINER_GET_PARAMS(param_container_l, env);
}

axis2_bool_t AXIS2_CALL 
axis2_operation_is_param_locked(axis2_operation_t *operation, 
                                    axis2_env_t **env,
		                            const axis2_char_t *param_name)
{
    axis2_svc_t *parent_l = NULL;
    axis2_param_t *param_l = NULL;
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    if(operation->param_container)
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_STATE_OPERATION, 
            AXIS2_FALSE);
	
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
	
    axis2_bool_t locked = AXIS2_FALSE;
    /* checking the locked value of parent*/
    parent_l = axis2_operation_get_parent(operation, env);
    if(NULL != parent_l)
    {
        locked = AXIS2_SVC_IS_PARAM_LOCKED(parent_l, env, param_name);
    }
    if(AXIS2_TRUE == locked)
    {
        return AXIS2_TRUE;
    }        
    else
    {
        param_l = axis2_operation_get_param(operation, env, param_name);   
    }
	return AXIS2_PARAM_IS_LOCKED(param_l, env);
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_parent (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        axis2_svc_t *svc)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    if(operation_impl->parent)
        AXIS2_SVC_FREE(operation_impl->parent, env);
    operation_impl->parent = svc;
	return AXIS2_SUCCESS;
}

axis2_svc_t * AXIS2_CALL
axis2_operation_get_parent (axis2_operation_t *operation, 
                                axis2_env_t **env)
{           
	AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
	return AXIS2_INTF_TO_IMPL(operation)->parent;
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env,
		                        struct axis2_msg_recv *msg_recv)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_recv, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    if(operation_impl->msg_recv)
        AXIS2_MSG_RECV_FREE(operation_impl->msg_recv, env);
    
	operation_impl->msg_recv = msg_recv;
	
	return AXIS2_SUCCESS;
}

struct axis2_msg_recv * AXIS2_CALL
axis2_operation_get_msg_recv (axis2_operation_t *operation, 
                                axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->msg_recv;
}
 
axis2_status_t AXIS2_CALL
axis2_operation_set_name (axis2_operation_t *operation, 
                            axis2_env_t **env,
                            axis2_qname_t *qname)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
	
    return AXIS2_WSDL_OPERATION_SET_NAME(operation->wsdl_operation, env, qname);
}
    
axis2_qname_t * AXIS2_CALL
axis2_operation_get_name (axis2_operation_t *operation, 
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
	
    return AXIS2_WSDL_OPERATION_GET_NAME(operation->wsdl_operation,
        env);
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env,
		                                    const axis2_char_t *pattern)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, pattern, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_MSG_EXCHANGE_PATTERN(operation->
        wsdl_operation, env, pattern);
}

axis2_char_t * AXIS2_CALL
axis2_operation_get_msg_exchange_pattern (axis2_operation_t *operation, 
                                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    
    return AXIS2_WSDL_OPERATION_GET_MSG_EXCHANGE_PATTERN
        (operation->wsdl_operation, env);
}

axis2_char_t * AXIS2_CALL
axis2_operation_get_style (axis2_operation_t *operation,
                            axis2_env_t **env)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    
    return AXIS2_WSDL_OPERATION_GET_STYLE(operation->wsdl_operation, env);
}

axis2_status_t  AXIS2_CALL
axis2_operation_set_style (axis2_operation_t *operation,
                            axis2_env_t **env,
                            axis2_char_t *style)
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_STYLE(operation->wsdl_operation, env, style);
}

axis2_status_t AXIS2_CALL
axis2_operation_engage_module(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_module_desc_t *moduleref) 
{
    int index = 0;
    int size = 0;
    axis2_array_list_t *collection_module = NULL;
    struct axis2_module_desc *module_desc = NULL;
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, moduleref, AXIS2_FAILURE);
    
    collection_module = (axis2_array_list_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(operation->wsdl_operation->
        extensible_component->wsdl_component, env, MODULEREF_KEY);
    
    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);
    for(index = 0; index < size; index++)
    {
        module_desc = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(
            collection_module, env, index);
        
        if(AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(module_desc, env), env,
                AXIS2_MODULE_DESC_GET_NAME(moduleref, env)))
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OPERATION, AXIS2_FAILURE);

    }
    /*    new PhaseResolver().engageModuleToOperation(this, moduleref);*/
    return AXIS2_ARRAY_LIST_ADD(collection_module, env, moduleref);
}

axis2_status_t AXIS2_CALL
axis2_operation_add_to_engage_module_list(axis2_operation_t *operation,
                                            axis2_env_t **env,
                                            axis2_module_desc_t *module_name) 
{
    axis2_array_list_t *collection_module = NULL;
    axis2_module_desc_t *module_desc = NULL;
    int size = 0;
    int index = 0;
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FAILURE);
    
    collection_module = (axis2_array_list_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(operation->wsdl_operation->
            extensible_component->wsdl_component, env, MODULEREF_KEY);
    
    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);
    for(index = 0; index < size; index++)
    {
        module_desc = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(
            collection_module, env, index);
        
        if(AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(module_desc, env), env,
                AXIS2_MODULE_DESC_GET_NAME(module_name, env)))
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OPERATION, AXIS2_FAILURE);

    }
    return AXIS2_ARRAY_LIST_ADD(collection_module, env, module_name);
}

axis2_array_list_t *AXIS2_CALL
axis2_operation_get_modules(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return (axis2_array_list_t *) AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
        operation->wsdl_operation->extensible_component->wsdl_component, env, 
            MODULEREF_KEY);
}

int AXIS2_CALL
axis2_operation_get_axis_specific_MEP_constant(axis2_operation_t *operation,
                                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    
    if (operation_impl->mep != MEP_CONSTANT_INVALID) 
    {
        return operation_impl->mep;
    }

    int temp = MEP_CONSTANT_INVALID;

    if (AXIS2_STRCMP(MEP_URI_IN_OUT, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_IN_OUT;
    } 
    else if (AXIS2_STRCMP(MEP_URI_IN_ONLY, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_IN_ONLY;
    } 
    else if (AXIS2_STRCMP(MEP_URI_IN_OPTIONAL_OUT, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_IN_OPTIONAL_OUT;
    } 
    else if (AXIS2_STRCMP(MEP_URI_OUT_IN, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_OUT_IN;
    } 
    else if (AXIS2_STRCMP(MEP_URI_OUT_ONLY, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_OUT_ONLY;
    } 
    else if (AXIS2_STRCMP(MEP_URI_OUT_OPTIONAL_IN, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_OUT_OPTIONAL_IN;
    } 
    else if (AXIS2_STRCMP(MEP_URI_ROBUST_IN_ONLY, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_ROBUST_IN_ONLY;
    } 
    else if (AXIS2_STRCMP(MEP_URI_ROBUST_OUT_ONLY, 
            axis2_operation_get_msg_exchange_pattern(operation, env))) 
    {
        temp = MEP_CONSTANT_ROBUST_OUT_ONLY;
    }

    if (temp == MEP_CONSTANT_INVALID) 
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_COULD_NOT_MAP_MEP_URI_TO_MEP_CONSTANT, AXIS2_FAILURE);
    }
    operation_impl->mep = temp;
    return operation_impl->mep;

}

axis2_array_list_t *AXIS2_CALL
axis2_operation_get_phases_in_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->phases_in_fault_flow;
}


axis2_array_list_t *AXIS2_CALL
axis2_operation_get_phases_out_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->phases_out_fault_flow;
}


axis2_array_list_t * AXIS2_CALL
axis2_operation_get_phases_out_flow(axis2_operation_t *operation,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->phases_out_flow;
}


axis2_array_list_t *AXIS2_CALL
axis2_operation_get_remaining_phases_in_flow(axis2_operation_t *operation,
                                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->remaining_phases_in_flow;
}

axis2_status_t AXIS2_CALL
axis2_operation_set_phases_in_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    if(operation_impl->phases_in_fault_flow)
       AXIS2_WSDL_OPERATION_FREE(operation_impl->phases_in_fault_flow, env);
    
    operation_impl->phases_in_fault_flow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_operation_set_phases_out_fault_flow(axis2_operation_t *operation,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    if(operation_impl->phases_out_fault_flow)
       AXIS2_WSDL_OPERATION_FREE(operation_impl->phases_out_fault_flow, env);
    
    operation_impl->phases_out_fault_flow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_operation_set_phases_out_flow(axis2_operation_t *operation,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    if(operation_impl->phases_out_flow)
       AXIS2_WSDL_OPERATION_FREE(operation_impl->phases_out_flow, env);
    
    operation_impl->phases_out_flow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_operation_set_remaining_phases_in_flow(axis2_operation_t *operation,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    if(operation_impl->remaining_phases_in_flow)
       AXIS2_LINKED_LIST_FREE(operation_impl->remaining_phases_in_flow, env);
    operation_impl->remaining_phases_in_flow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_operation_add_module(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_qname_t *module_name) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_name, AXIS2_FAILURE);
    axis2_operation_impl_t *operation_impl = AXIS2_INTF_TO_IMPL(operation);
    return AXIS2_ARRAY_LIST_ADD(operation_impl->modulerefs, env, module_name);
}

axis2_array_list_t *AXIS2_CALL
axis2_operation_get_module_refs(axis2_operation_t *operation,
                                    axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_INTF_TO_IMPL(operation)->modulerefs;
}

axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_infaults(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_WSDL_OPERATION_GET_INFAULTS(operation->wsdl_operation, env);
}

axis2_status_t AXIS2_CALL
axis2_operation_set_infaults(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_linked_list_t *infaults) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, infaults, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_INFAULTS(operation->wsdl_operation, env, infaults);
}

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_operation_get_input_msg(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_WSDL_OPERATION_GET_INPUT_MSG(operation->wsdl_operation, env);
}

axis2_status_t AXIS2_CALL
axis2_operation_set_input_msg(axis2_operation_t *operation,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *input_msg) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, input_msg, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_INPUT_MSG(operation->wsdl_operation, env, input_msg);
}

axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_outfaults(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_WSDL_OPERATION_GET_OUTFAULTS(operation->wsdl_operation, env);
}

axis2_status_t AXIS2_CALL
axis2_operation_set_outfaults(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_linked_list_t *outfaults) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outfaults, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_OUTFAULTS(operation->wsdl_operation, env, outfaults);
}

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_operation_get_output_msg(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_WSDL_OPERATION_GET_OUTPUT_MSG(operation->wsdl_operation, env);
}

axis2_status_t AXIS2_CALL
axis2_operation_set_output_msg(axis2_operation_t *operation,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *output_msg) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, output_msg, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_SET_OUTPUT_MSG(operation->wsdl_operation, env, output_msg);
}

axis2_bool_t AXIS2_CALL
axis2_operation_is_safe(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FALSE);
    return AXIS2_WSDL_OPERATION_IS_SAFE(operation->wsdl_operation, env);
}

axis2_status_t AXIS2_CALL
axis2_operation_set_safety(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_bool_t safe) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    return AXIS2_WSDL_OPERATION_SET_SAFETY(operation->wsdl_operation, env, safe);
}

axis2_char_t *AXIS2_CALL
axis2_operation_get_target_namespace(axis2_operation_t *operation,
                                        axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_WSDL_OPERATION_GET_TARGET_NAMESPACE(operation->wsdl_operation,
        env);
}

axis2_status_t AXIS2_CALL
axis2_operation_add_infault(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *infault) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, infault, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_ADD_INFAULT(operation->wsdl_operation, env, infault);
}

axis2_status_t AXIS2_CALL
axis2_operation_add_outfault(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *outfault) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, outfault, AXIS2_FAILURE);
    
    return AXIS2_WSDL_OPERATION_ADD_OUTFAULT(operation->wsdl_operation, env, outfault);
}

axis2_status_t AXIS2_CALL
axis2_operation_add_feature(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_feature_t *feature) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, feature, AXIS2_FAILURE);
    
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_ADD_FEATURE(operation->wsdl_operation->
        extensible_component, env, feature);
}

axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_features(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_GET_FEATURES(operation->
        wsdl_operation->extensible_component, env);
}

axis2_status_t AXIS2_CALL
axis2_operation_add_property(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_property_t *wsdl_property) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_property, AXIS2_FAILURE);
    
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_ADD_PROPERTY(operation->wsdl_operation->
        extensible_component, env, wsdl_property);
}

axis2_linked_list_t *AXIS2_CALL
axis2_operation_get_Properties(axis2_operation_t *operation,
                                axis2_env_t **env) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, NULL);
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_GET_PROPERTIES(operation->
        wsdl_operation->extensible_component, env);
}

axis2_status_t AXIS2_CALL 
axis2_operation_set_wsdl_opeartion(axis2_operation_t *operation,
                                axis2_env_t **env,
                                axis2_wsdl_operation_t *wsdl_operation) 
{
    AXIS2_FUNC_PARAM_CHECK(operation, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_operation, AXIS2_FAILURE);
    
    if(operation->wsdl_operation)
       AXIS2_WSDL_OPERATION_FREE(operation->wsdl_operation, env);
    
    operation->wsdl_operation = wsdl_operation;
    return AXIS2_SUCCESS;
}

/**
 * This method is responsible for finding a MEPContext for an incomming
 * messages. An incomming message can be of two states.
 * <p/>
 * 1)This is a new incomming message of a given MEP. 2)This message is a
 * part of an MEP which has already begun.
 * <p/>
 * The method is special cased for the two MEPs
 * <p/>
 * #IN_ONLY #IN_OUT
 * <p/>
 * for two reasons. First reason is the wide usage and the second being that
 * the need for the MEPContext to be saved for further incomming messages.
 * <p/>
 * In the event that MEP of this operation is different from the two MEPs
 * deafulted above the decession of creating a new or this message relates
 * to a MEP which already in business is decided by looking at the WSA
 * Relates TO of the incomming message.
 *
 * @param msgContext
 */
/*OperationContext findOperationContext(MessageContext msgContext, ServiceContext serviceContext) throws AxisFault {
    OperationContext operationContext ;

    if (null == msgContext.get_RelatesTo()) {
        //Its a new incomming message so get_ the factory to create a new
        // one
       operationContext =  new OperationContext(this,serviceContext);
    } else {
        // So this message is part of an ongoing MEP
        //			operationContext =
        ConfigurationContext configContext = msgContext.get_SystemContext();
        operationContext =
                configContext.get_OperationContext( msgContext.get_RelatesTo().get_Value());

        if (null == operationContext) {
            throw new AxisFault(Messages.get_Message("cannotCorrelateMsg",
                    this.get_Name().toString(),msgContext.get_RelatesTo().get_Value()));
        }

    }

    registerOperationContext(msgContext, operationContext);

    return operationContext;

}
*/

/**
 * This will not create a new operation context if there is no one already.
 * @param msgContext
 * @return
 * @throws AxisFault
 */
/*axis2_operation_ctx_t *AXIS2_CALL
OperationContext 
findForExistingOperationContext(MessageContext msgContext) throws AxisFault {
    OperationContext operationContext = null;

    if((operationContext = msgContext.get_OperationContext()) != null) {
        return operationContext;
    }

    if (null == msgContext.get_RelatesTo()) {
        return null;
    } else {
        // So this message is part of an ongoing MEP
        //			operationContext =
        ConfigurationContext configContext = msgContext.get_SystemContext();
        operationContext = configContext.get_OperationContext(msgContext.get_RelatesTo().get_Value());

        if (null == operationContext) {
            throw new AxisFault(Messages.get_Message("cannotCorrealteMsg",
                    this.get_Name().toString(),msgContext.get_RelatesTo().get_Value()));
        }

    }


    return operationContext;

}
*/
/*
axis2_status_t AXIS2_CALL
registerOperationContext(MessageContext msgContext, OperationContext operationContext) throws AxisFault {
    msgContext.get_SystemContext().registerOperationContext(
            msgContext.get_MessageID(), operationContext);
    operationContext.addMessageContext(msgContext);
    msgContext.setOperationContext(operationContext);
    if (operationContext.isComplete()) {
        operationContext.cleanup();
    }
}
*/
