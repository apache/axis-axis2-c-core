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
 
#include <axis2_op.h>
#include <axis2_property.h>

/** 
 * @brief Operaton struct impl
 *	Axis2 Operations  
 */ 
typedef struct axis2_op_impl
{
	axis2_op_t op;
    
	axis2_svc_t *parent;
	axis2_msg_recv_t *msg_recv;
    axis2_wsdl_op_t *wsdl_op;
        
    axis2_array_list_t *remaining_phases_inflow;
    axis2_array_list_t *phases_outflow;
    axis2_array_list_t *phases_in_fault_flow;
    axis2_array_list_t *phases_out_fault_flow;
    int mep;
    /*To store deploytime module refs */
    axis2_array_list_t *modulerefs;
    
} axis2_op_impl_t;

#define AXIS2_INTF_TO_IMPL(op) ((axis2_op_impl_t *)op)
	
/*************************** Function headers *********************************/

axis2_status_t AXIS2_CALL
axis2_op_free (axis2_op_t *op, 
                        axis2_env_t **env);	

axis2_status_t AXIS2_CALL 
axis2_op_add_param (axis2_op_t *op, 
                            axis2_env_t **env,
                            axis2_param_t *param);

axis2_param_t * AXIS2_CALL
axis2_op_get_param (axis2_op_t *op, 
                            axis2_env_t **env,
                            axis2_char_t *name);

axis2_array_list_t * AXIS2_CALL
axis2_op_get_params (axis2_op_t *op, 
                                axis2_env_t **env);

axis2_bool_t AXIS2_CALL
axis2_op_is_param_locked(axis2_op_t *op, 
                                    axis2_env_t **env
    ,                               axis2_char_t *param_name);

axis2_status_t AXIS2_CALL
axis2_op_set_parent (axis2_op_t *op, 
                                axis2_env_t **env,
                                axis2_svc_t *svc);

axis2_svc_t * AXIS2_CALL
axis2_op_get_parent (axis2_op_t *op, 
                                axis2_env_t **env);
		
axis2_status_t AXIS2_CALL
axis2_op_set_msg_recv (axis2_op_t *op, 
                                axis2_env_t **env,
                                struct axis2_msg_recv *msg_recv);

struct axis2_msg_recv * AXIS2_CALL
axis2_op_get_msg_recv (axis2_op_t *op, 
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_qname (axis2_op_t *op, 
                            axis2_env_t **env,
                            axis2_qname_t *qname);
                            
axis2_qname_t * AXIS2_CALL
axis2_op_get_qname (void *op, 
                    axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_msg_exchange_pattern (axis2_op_t *op, 
                                            axis2_env_t **env,
                                            axis2_char_t *pattern);
		
axis2_char_t * AXIS2_CALL
axis2_op_get_msg_exchange_pattern (axis2_op_t *op, 
                                            axis2_env_t **env);

axis2_char_t * AXIS2_CALL
axis2_op_get_style (axis2_op_t *op,
                            axis2_env_t **env);

axis2_status_t  AXIS2_CALL
axis2_op_set_style (axis2_op_t *op,
                            axis2_env_t **env,
                            axis2_char_t *style);

axis2_status_t AXIS2_CALL
axis2_op_engage_module(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_module_desc_t *moduleref);

axis2_status_t AXIS2_CALL
axis2_op_add_to_engage_module_list(axis2_op_t *op,
                                            axis2_env_t **env,
                                            axis2_module_desc_t *module_name);

axis2_array_list_t *AXIS2_CALL
axis2_op_get_modules(axis2_op_t *op,
                                axis2_env_t **env);

int AXIS2_CALL
axis2_op_get_axis_specific_mep_const(axis2_op_t *op,
                                                axis2_env_t **env);

axis2_array_list_t *AXIS2_CALL
axis2_op_get_phases_in_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env);


axis2_array_list_t *AXIS2_CALL
axis2_op_get_phases_out_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env);

axis2_array_list_t * AXIS2_CALL
axis2_op_get_phases_outflow(axis2_op_t *op,
                                        axis2_env_t **env);


axis2_array_list_t *AXIS2_CALL
axis2_op_get_remaining_phases_inflow(axis2_op_t *op,
                                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_phases_in_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_op_set_phases_out_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_op_set_phases_outflow(axis2_op_t *op,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_op_set_remaining_phases_inflow(axis2_op_t *op,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list);

axis2_status_t AXIS2_CALL
axis2_op_add_module(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_qname_t *module_name);

axis2_array_list_t *AXIS2_CALL
axis2_op_get_module_refs(axis2_op_t *op,
                                    axis2_env_t **env);

axis2_linked_list_t *AXIS2_CALL
axis2_op_get_in_faults(axis2_op_t *op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_in_faults(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_linked_list_t *in_faults);

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_op_get_input_msg(axis2_op_t *op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_input_msg(axis2_op_t *op,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *input_msg); 


axis2_linked_list_t *AXIS2_CALL
axis2_op_get_out_faults(axis2_op_t *op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_out_faults(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_linked_list_t *out_faults);

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_op_get_output_msg(axis2_op_t *op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_output_msg(axis2_op_t *op,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *output_msg); 


axis2_bool_t AXIS2_CALL
axis2_op_is_safe(axis2_op_t *op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_set_safety(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_bool_t safe);

axis2_char_t *AXIS2_CALL
axis2_op_get_target_namespace(axis2_op_t *op,
                                        axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_op_add_in_fault(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *in_fault);

axis2_status_t AXIS2_CALL
axis2_op_add_out_fault(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *out_fault);

axis2_status_t AXIS2_CALL
axis2_op_add_feature(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_feature_t *feature);

axis2_linked_list_t *AXIS2_CALL
axis2_op_get_features(axis2_op_t *op,
                                axis2_env_t **env); 


axis2_status_t AXIS2_CALL
axis2_op_add_property(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_property_t *wsdl_property);

axis2_linked_list_t *AXIS2_CALL
axis2_op_get_Properties(axis2_op_t *op,
                                axis2_env_t **env);

axis2_status_t AXIS2_CALL 
axis2_op_set_wsdl_op(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_op_t *wsdl_op);

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
 * In the event that MEP of this op is different from the two MEPs
 * deafulted above the decession of creating a new or this message relates
 * to a MEP which already in business is decided by looking at the WSA
 * Relates TO of the incomming message.
 *
 * @param msgContext
 */
struct axis2_op_ctx *AXIS2_CALL
axis2_op_find_op_ctx(axis2_op_t *op,
                        axis2_env_t **env,
                        struct axis2_msg_ctx *msg_ctx, 
                        struct axis2_svc_ctx *svc_ctx);

/**
 * This will not create a new op context if there is no one already.
 * @param msgContext
 * @return
 * @throws AxisFault
 */
axis2_op_ctx_t *AXIS2_CALL
axis2_op_find_for_existing_op_ctx(axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_msg_ctx *msg_ctx);
                                        
axis2_status_t AXIS2_CALL
axis2_op_register_op_ctx(axis2_op_t *op,
                            axis2_env_t **env,
                            struct axis2_msg_ctx *msg_ctx,
                            struct axis2_op_ctx *op_ctx);

axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_in_only(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx);

axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_out_only(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx);

axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_in_out(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx);
    
axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_out_in(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx);                            
                                
/************************* End of function headers ****************************/	

AXIS2_DECLARE(axis2_op_t *)
axis2_op_create (axis2_env_t **env)
{
    axis2_param_container_t *param_container_l = NULL;
    axis2_array_list_t *array_list_l = NULL;
    axis2_op_impl_t *op_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_phase_t *policy_determination = NULL;
    axis2_phase_t *message_processing = NULL;
    axis2_phase_t *message_out = NULL;
    axis2_property_t *property = NULL;
 
    AXIS2_ENV_CHECK(env, NULL);
	
	op_impl =  (axis2_op_impl_t *) AXIS2_MALLOC ((*env)->allocator,
		sizeof (axis2_op_impl_t));
     
	if(NULL == op_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    op_impl->parent = NULL;
	op_impl->msg_recv = NULL;
    op_impl->mep = AXIS2_MEP_CONSTANT_INVALID;
    op_impl->op.param_container = NULL;
    op_impl->wsdl_op = NULL;
    op_impl->remaining_phases_inflow = NULL;
    op_impl->phases_outflow = NULL;
    op_impl->phases_in_fault_flow = NULL;
    op_impl->phases_out_fault_flow = NULL;
    op_impl->modulerefs = NULL;
    op_impl->op.ops = NULL;
    
    op_impl->op.param_container = (axis2_param_container_t *)
		axis2_param_container_create(env);		
	if(NULL == op_impl->op.param_container)
	{
        axis2_op_free(&(op_impl->op), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);	
        return NULL;        
	}

	op_impl->wsdl_op = (axis2_wsdl_op_t *) axis2_wsdl_op_create(env);		
	if(NULL == op_impl->wsdl_op)
	{
        axis2_op_free(&(op_impl->op), env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;		
	}
    /* Set the function pointers of the base */
    op_impl->op.base.ops = AXIS2_MALLOC((*env)->allocator, 
        sizeof(axis2_wsdl_op_ops_t));
    op_impl->op.base.ops->free_void_arg = axis2_op_free_void_arg;
    op_impl->op.base.ops->get_qname = axis2_op_get_qname;

    op_impl->remaining_phases_inflow = axis2_array_list_create(env, 0);
    if(NULL == op_impl->remaining_phases_inflow)
    {
        axis2_op_free(&(op_impl->op), env);
        return NULL;
    }
    policy_determination = axis2_phase_create(env, 
        AXIS2_PHASE_POLICY_DETERMINATION);
    status = AXIS2_ARRAY_LIST_ADD(op_impl->remaining_phases_inflow, env, 
        policy_determination);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_PHASE_FREE(policy_determination, env);
        policy_determination = NULL;
        return NULL;
    }
    policy_determination = NULL;
    message_processing = axis2_phase_create(env, AXIS2_PHASE_MESSAGE_PROCESSING);
    /* TODO
     * Add soap processing model checker handler to this when the handler once
     * handler is written
     */

    status = AXIS2_ARRAY_LIST_ADD(op_impl->remaining_phases_inflow, env,
        message_processing);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_PHASE_FREE(policy_determination, env);
        AXIS2_PHASE_FREE(message_processing, env);
        policy_determination = NULL;
        message_processing = NULL;
        return NULL;
    }
    message_processing = NULL;
    
    
    op_impl->phases_outflow = axis2_array_list_create(env, 0);
    if(NULL == op_impl->phases_outflow)
    {
        axis2_op_free(&(op_impl->op), env);
        return NULL;
    }
   
    policy_determination = axis2_phase_create(env, 
        AXIS2_PHASE_POLICY_DETERMINATION);
    status = AXIS2_ARRAY_LIST_ADD(op_impl->phases_outflow, env, 
        policy_determination);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_PHASE_FREE(policy_determination, env);
        policy_determination = NULL;
        return NULL;
    }
    policy_determination = NULL;
    
    message_out = axis2_phase_create(env, AXIS2_PHASE_MESSAGE_OUT);
    status = AXIS2_ARRAY_LIST_ADD(op_impl->phases_outflow, env, message_out);
    if(AXIS2_SUCCESS != status)
    {
        AXIS2_PHASE_FREE(policy_determination, env);
        AXIS2_PHASE_FREE(message_out, env);
        policy_determination = NULL;
        message_out = NULL;
        return NULL;
    }
    message_out = NULL;
    
    axis2_op_set_msg_exchange_pattern(&(op_impl->op), env, 
        (axis2_char_t *) AXIS2_MEP_URI_IN_OUT);
    
    param_container_l = axis2_param_container_create(env);
    if(NULL == param_container_l)
    {
        return NULL;
    }
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, 
        axis2_param_container_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, param_container_l);
    status = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(op_impl->wsdl_op->
        extensible_component->wsdl_component, env, AXIS2_PARAMETER_KEY, 
            property);
    if(AXIS2_SUCCESS != status)
    {
        axis2_op_free(&(op_impl->op), env);
        return NULL;
    }
    
    array_list_l = axis2_array_list_create(env, 0);
    if(NULL == array_list_l) return NULL;
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, 
        axis2_array_list_free_void_arg);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_VALUE(property, env, array_list_l);
    
    status = AXIS2_WSDL_COMPONENT_SET_COMPONENT_PROPERTY(op_impl->wsdl_op->
        extensible_component->wsdl_component, env, AXIS2_MODULEREF_KEY, property);
    if(AXIS2_SUCCESS != status)
    {
        axis2_op_free(&(op_impl->op), env);
        return NULL;
    }

    op_impl->op.ops = AXIS2_MALLOC((*env)->allocator, sizeof(axis2_op_ops_t));
	if(NULL == op_impl->op.ops)
	{
        axis2_op_free(&(op_impl->op), env);
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
	
	op_impl->op.ops->free = axis2_op_free;
	op_impl->op.ops->free_void_arg = axis2_op_free_void_arg;
	op_impl->op.ops->add_param = axis2_op_add_param;
	op_impl->op.ops->get_param = axis2_op_get_param;
	op_impl->op.ops->get_params = axis2_op_get_params;
	op_impl->op.ops->set_parent = axis2_op_set_parent;
	op_impl->op.ops->get_parent = axis2_op_get_parent;
	op_impl->op.ops->set_msg_recv = axis2_op_set_msg_recv;
	op_impl->op.ops->get_msg_recv = axis2_op_get_msg_recv;
    op_impl->op.ops->set_qname = axis2_op_set_qname;
	op_impl->op.ops->get_qname = axis2_op_get_qname; 
	op_impl->op.ops->set_msg_exchange_pattern 
		= axis2_op_set_msg_exchange_pattern; 
	op_impl->op.ops->get_msg_exchange_pattern
		= axis2_op_get_msg_exchange_pattern;
    op_impl->op.ops->set_style = axis2_op_set_style;
	op_impl->op.ops->get_style = axis2_op_get_style; 
    op_impl->op.ops->engage_module = axis2_op_engage_module;
    op_impl->op.ops->add_to_engage_module_list = axis2_op_add_to_engage_module_list;
    op_impl->op.ops->get_modules = axis2_op_get_modules;
    op_impl->op.ops->get_axis_specific_mep_const = axis2_op_get_axis_specific_mep_const;
    op_impl->op.ops->get_phases_in_fault_flow = axis2_op_get_phases_in_fault_flow;
    op_impl->op.ops->get_phases_out_fault_flow = axis2_op_get_phases_out_fault_flow;
    op_impl->op.ops->get_phases_outflow = axis2_op_get_phases_outflow;
    op_impl->op.ops->get_remaining_phases_inflow = axis2_op_get_remaining_phases_inflow;
    op_impl->op.ops->set_phases_in_fault_flow = axis2_op_set_phases_in_fault_flow;
    op_impl->op.ops->set_phases_out_fault_flow = axis2_op_set_phases_out_fault_flow;
    op_impl->op.ops->set_phases_outflow = axis2_op_set_phases_outflow;
    op_impl->op.ops->set_remaining_phases_inflow = axis2_op_set_remaining_phases_inflow;
    op_impl->op.ops->add_module = axis2_op_add_module;
    op_impl->op.ops->get_module_refs = axis2_op_get_module_refs;
    op_impl->op.ops->get_in_faults = axis2_op_get_in_faults;
    op_impl->op.ops->set_in_faults = axis2_op_set_in_faults;
    op_impl->op.ops->get_input_msg = axis2_op_get_input_msg;
    op_impl->op.ops->set_input_msg = axis2_op_set_input_msg;
    op_impl->op.ops->get_out_faults = axis2_op_get_out_faults;
    op_impl->op.ops->set_out_faults = axis2_op_set_out_faults;
    op_impl->op.ops->get_output_msg = axis2_op_get_output_msg;
    op_impl->op.ops->set_output_msg = axis2_op_set_output_msg;
    op_impl->op.ops->is_safe = axis2_op_is_safe;
    op_impl->op.ops->set_safety = axis2_op_set_safety;
    op_impl->op.ops->get_target_namespace = axis2_op_get_target_namespace;
    op_impl->op.ops->add_in_fault = axis2_op_add_in_fault;
    op_impl->op.ops->add_out_fault = axis2_op_add_out_fault;
    op_impl->op.ops->add_feature = axis2_op_add_feature;
    op_impl->op.ops->get_features = axis2_op_get_features;
    op_impl->op.ops->add_property = axis2_op_add_property;
    op_impl->op.ops->get_Properties = axis2_op_get_Properties;
    op_impl->op.ops->set_wsdl_op = axis2_op_set_wsdl_op;
	op_impl->op.ops->find_op_ctx = axis2_op_find_op_ctx;	
    op_impl->op.ops->find_for_existing_op_ctx = axis2_op_find_for_existing_op_ctx;
    op_impl->op.ops->register_op_ctx = axis2_op_register_op_ctx;
    
	return &(op_impl->op);
}

axis2_op_t * AXIS2_CALL
axis2_op_create_with_qname (axis2_env_t **env, axis2_qname_t *qname)
{
	axis2_op_impl_t *op_impl = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, qname, AXIS2_FAILURE);	
    
    op_impl = (axis2_op_impl_t *) axis2_op_create(env);
    
	if(NULL == op_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    if(!op_impl->wsdl_op)
    {
        op_impl->wsdl_op = (axis2_wsdl_op_t *) axis2_wsdl_op_create(env);		
        if(NULL == op_impl->wsdl_op)
        {
            axis2_op_free(&(op_impl->op), env);
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;		
        }
    }
	status = axis2_op_set_qname(&(op_impl->op), env, qname);
    if(AXIS2_SUCCESS != status)
    {
        axis2_op_free(&(op_impl->op), env);
        return NULL;
    }
    
	return &(op_impl->op);	
}

axis2_op_t * AXIS2_CALL
axis2_op_create_with_wsdl_op (axis2_env_t **env, 
                                            axis2_wsdl_op_t *wsdl_op)
{
	axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_op, NULL);
    
    op_impl = (axis2_op_impl_t *) axis2_op_create(env);
    
	if(NULL == op_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
   
	if(NULL != op_impl->wsdl_op)
    {
        AXIS2_WSDL_OP_FREE(op_impl->wsdl_op, env);
    }
    op_impl->wsdl_op = wsdl_op;
       
	return &(op_impl->op);	
}

/*************************** Start of op impls *************************/

axis2_status_t AXIS2_CALL 
axis2_op_free (axis2_op_t *op, axis2_env_t **env)
{ 
    axis2_op_impl_t *op_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);

    if(op_impl->remaining_phases_inflow)
    {
        int i = 0;
        int size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->remaining_phases_inflow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->remaining_phases_inflow, env, 
                i);
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            }
      
        }
   
        AXIS2_ARRAY_LIST_FREE(op_impl->remaining_phases_inflow, env);
        op_impl->remaining_phases_inflow = NULL;
    }
    
    if(NULL != op_impl->phases_outflow)
    {
        int i = 0;
        int size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->phases_outflow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->phases_outflow, env, i);
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            }
               
        }
        AXIS2_ARRAY_LIST_FREE(op_impl->phases_outflow, env);
        op_impl->phases_outflow = NULL;
    }
    
    if(NULL != op_impl->phases_in_fault_flow)
    {
        int i = 0;
        int size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->phases_in_fault_flow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->phases_in_fault_flow, env, i);
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            }
               
        }
        AXIS2_ARRAY_LIST_FREE(op_impl->phases_in_fault_flow, env);
        op_impl->phases_in_fault_flow = NULL;
    }
    
    if(NULL != op_impl->phases_out_fault_flow)
    {
        int i = 0;
        int size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->phases_out_fault_flow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->phases_out_fault_flow, env, i);
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            }
              
        }
        AXIS2_ARRAY_LIST_FREE(op_impl->phases_out_fault_flow, env);
        op_impl->phases_out_fault_flow = NULL;
    }

    if(NULL != op->param_container)
    {
	    AXIS2_PARAM_CONTAINER_FREE(op->param_container, env);
        op->param_container = NULL;
    }
    
    op_impl->parent = NULL;

    if(op_impl->msg_recv)
    {
        AXIS2_MSG_RECV_FREE(op_impl->msg_recv, env);
        op_impl->msg_recv = NULL;
    } 
    
    if(NULL != op_impl->modulerefs)
    {
        int i = 0;
        for (i = 0; i < AXIS2_ARRAY_LIST_SIZE(op_impl->modulerefs, env); i++)
        {
            axis2_qname_t *module_ref = NULL;
            module_ref = AXIS2_ARRAY_LIST_GET(op_impl->modulerefs, env, i);
            
            if (module_ref)
            {
                AXIS2_QNAME_FREE (module_ref, env);
                module_ref = NULL;
            }       
        }
        AXIS2_ARRAY_LIST_FREE(op_impl->modulerefs, env);
        op_impl->modulerefs = NULL;
    }
    
    if(NULL != op_impl->wsdl_op)
    {
	    AXIS2_WSDL_OP_FREE(op_impl->wsdl_op, env);
        op_impl->wsdl_op = NULL;
    }
    
    if (op_impl->op.base.ops)
    {
        AXIS2_FREE((*env)->allocator, op_impl->op.base.ops);
        op_impl->op.base.ops = NULL;
    }
    
    if (op_impl->op.ops)
    {
        AXIS2_FREE((*env)->allocator, op_impl->op.ops);
        op_impl->op.ops = NULL;
    }

    if(op_impl)    
    {
        AXIS2_FREE((*env)->allocator, op_impl);
        op_impl = NULL;
    }
    
    return AXIS2_SUCCESS;
}	

axis2_status_t AXIS2_CALL
axis2_op_free_void_arg(void *op,
                        axis2_env_t **env)
{
    axis2_op_t *op_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_l = (axis2_op_t *) op;
    return axis2_op_free(op_l, env);
}

axis2_status_t AXIS2_CALL 
axis2_op_add_param (axis2_op_t *op, 
                                axis2_env_t **env,       
		                        axis2_param_t *param)
{
    axis2_op_impl_t *op_impl = NULL;
    axis2_param_container_t *param_container_l = NULL;
    axis2_char_t *param_name = NULL;
    axis2_property_t *property = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param, AXIS2_FALSE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
	
    param_name = AXIS2_PARAM_GET_NAME(param, env);
    if(AXIS2_TRUE == axis2_op_is_param_locked(op, env, param_name))
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_PARAMETER_LOCKED_CANNOT_OVERRIDE, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    else
    {
        property = (axis2_property_t *)
            AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(
                op_impl->wsdl_op->extensible_component->wsdl_component, env, 
                (axis2_char_t *) AXIS2_PARAMETER_KEY);
        param_container_l = (axis2_param_container_t *) 
            AXIS2_PROPERTY_GET_VALUE(property, env);
        return AXIS2_PARAM_CONTAINER_ADD_PARAM(param_container_l, env, param);
    }
    
    return AXIS2_SUCCESS;
}

axis2_param_t * AXIS2_CALL
axis2_op_get_param (axis2_op_t *op, 
                                axis2_env_t **env,
		                        axis2_char_t *param_name)
{
    axis2_op_impl_t *op_impl = NULL;
    axis2_param_container_t *param_container_l = NULL;
    axis2_property_t *property = NULL;
 
	AXIS2_ENV_CHECK(env, AXIS2_FALSE);
	AXIS2_PARAM_CHECK((*env)->error, param_name, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);

    property = (axis2_property_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(op_impl->wsdl_op->
            extensible_component->wsdl_component, env, (axis2_char_t *) 
                AXIS2_PARAMETER_KEY);
	
    param_container_l = (axis2_param_container_t *) 
        AXIS2_PROPERTY_GET_VALUE(property, env);
	return AXIS2_PARAM_CONTAINER_GET_PARAM(param_container_l, env, param_name);
}

axis2_array_list_t * AXIS2_CALL
axis2_op_get_params(axis2_op_t *op, 
                                axis2_env_t **env)
{
    axis2_op_impl_t *op_impl = NULL;
    /*axis2_param_container_t *param_container_l = NULL; */
    
	AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, op->param_container, AXIS2_FALSE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
   
    return AXIS2_PARAM_CONTAINER_GET_PARAMS(op->param_container, env);
}

axis2_bool_t AXIS2_CALL 
axis2_op_is_param_locked(axis2_op_t *op, 
                                    axis2_env_t **env,
		                            axis2_char_t *param_name)
{
    axis2_svc_t *parent_l = NULL;
    axis2_param_t *param_l = NULL;
    axis2_bool_t locked = AXIS2_FALSE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    AXIS2_PARAM_CHECK((*env)->error, param_name, AXIS2_FALSE);
    
    /* checking the locked value of parent*/
    parent_l = axis2_op_get_parent(op, env);
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
        param_l = axis2_op_get_param(op, env, param_name);   
    }
	return AXIS2_PARAM_IS_LOCKED(param_l, env);
}

axis2_status_t AXIS2_CALL 
axis2_op_set_parent (axis2_op_t *op, 
                                axis2_env_t **env,
		                        axis2_svc_t *svc)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svc, AXIS2_FAILURE);
    
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    if(op_impl->parent)
    {
        op_impl->parent = NULL;
    }
    op_impl->parent = svc;
	return AXIS2_SUCCESS;
}

axis2_svc_t * AXIS2_CALL
axis2_op_get_parent (axis2_op_t *op, 
                                axis2_env_t **env)
{           
	AXIS2_ENV_CHECK(env, NULL);
    
	return AXIS2_INTF_TO_IMPL(op)->parent;
}

axis2_status_t AXIS2_CALL 
axis2_op_set_msg_recv (axis2_op_t *op, 
                                axis2_env_t **env,
		                        struct axis2_msg_recv *msg_recv)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_recv, AXIS2_FAILURE);
    
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    if(op_impl->msg_recv)
    {
        op_impl->msg_recv = NULL;
    }
    
	op_impl->msg_recv = msg_recv;
	
	return AXIS2_SUCCESS;
}

struct axis2_msg_recv * AXIS2_CALL
axis2_op_get_msg_recv (axis2_op_t *op, 
                                axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op)->msg_recv;
}
 
axis2_status_t AXIS2_CALL
axis2_op_set_qname (axis2_op_t *op, 
                            axis2_env_t **env,
                            axis2_qname_t *qname)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_QNAME(op_impl->wsdl_op, env, qname);
}
    
axis2_qname_t * AXIS2_CALL
axis2_op_get_qname (void *op, 
                    axis2_env_t **env)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
	
    return AXIS2_WSDL_OP_GET_QNAME(op_impl->wsdl_op,
        env);
}

axis2_status_t AXIS2_CALL 
axis2_op_set_msg_exchange_pattern (axis2_op_t *op, 
                                            axis2_env_t **env,
		                                    axis2_char_t *pattern)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, pattern, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_MSG_EXCHANGE_PATTERN(op_impl->wsdl_op, env, 
        pattern);
}

axis2_char_t * AXIS2_CALL
axis2_op_get_msg_exchange_pattern (axis2_op_t *op, 
                                            axis2_env_t **env)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_GET_MSG_EXCHANGE_PATTERN(op_impl->wsdl_op, env);
}

axis2_char_t * AXIS2_CALL
axis2_op_get_style (axis2_op_t *op,
                            axis2_env_t **env)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    return AXIS2_WSDL_OP_GET_STYLE(op_impl->wsdl_op, env);
}

axis2_status_t  AXIS2_CALL
axis2_op_set_style (axis2_op_t *op,
                            axis2_env_t **env,
                            axis2_char_t *style)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, style, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_STYLE(op_impl->wsdl_op, env, style);
}

axis2_status_t AXIS2_CALL
axis2_op_engage_module(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_module_desc_t *moduleref) 
{
    axis2_op_impl_t *op_impl = NULL;
    int index = 0;
    int size = 0;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_array_list_t *collection_module = NULL;
    axis2_module_desc_t *module_desc = NULL;
    axis2_phase_resolver_t *pr = NULL;
    axis2_property_t *property = NULL;
        
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, moduleref, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    property = (axis2_property_t *)
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(op_impl->wsdl_op->
        extensible_component->wsdl_component, env, AXIS2_MODULEREF_KEY);
    collection_module = (axis2_array_list_t *) 
        AXIS2_PROPERTY_GET_VALUE(property, env);
    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);
    if(AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_FAILURE;
    }
    
    for(index = 0; index < size; index++)
    {
        module_desc = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(
            collection_module, env, index);
        if(!module_desc)
        {
            return AXIS2_FAILURE;
        }
        if(AXIS2_QNAME_EQUALS(AXIS2_MODULE_DESC_GET_NAME(module_desc, env), env,
                AXIS2_MODULE_DESC_GET_NAME(moduleref, env)))
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OP, AXIS2_FAILURE);
            return AXIS2_SUCCESS;
        }

    }
    pr = axis2_phase_resolver_create(env);
    if(pr)
    {
        status = AXIS2_PHASE_RESOLVER_ENGAGE_MODULE_TO_OP(pr, env, 
            op, moduleref);
        if(AXIS2_FAILURE == status)
        {
            AXIS2_PHASE_RESOLVER_FREE(pr, env);
            pr = NULL;
            return AXIS2_FAILURE;
        }
    }
    else
    {
        return AXIS2_FAILURE;
    }
    AXIS2_PHASE_RESOLVER_FREE(pr, env);
    pr = NULL;
    return AXIS2_ARRAY_LIST_ADD(collection_module, env, moduleref);
}

axis2_status_t AXIS2_CALL
axis2_op_add_to_engage_module_list(axis2_op_t *op,
                                            axis2_env_t **env,
                                            axis2_module_desc_t *module_desc) 
{
    axis2_op_impl_t *op_impl = NULL;
    axis2_array_list_t *collection_module = NULL;
    axis2_module_desc_t *module_desc_l = NULL;
    int size = 0;
    int index = 0;
    axis2_property_t *property = NULL;
    axis2_qname_t *module_qname = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_desc, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    property = (axis2_property_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(op_impl->wsdl_op->
            extensible_component->wsdl_component, env, AXIS2_MODULEREF_KEY);
    
    collection_module = (axis2_array_list_t *) AXIS2_PROPERTY_GET_VALUE(
        property, env);
    size = AXIS2_ARRAY_LIST_SIZE(collection_module, env);
    
    if(AXIS2_SUCCESS != AXIS2_ERROR_GET_STATUS_CODE((*env)->error))
    {
        return AXIS2_ERROR_GET_STATUS_CODE((*env)->error);
    }
    module_qname = AXIS2_MODULE_DESC_GET_NAME(module_desc, env);
    for(index = 0; index < size; index++)
    {
        axis2_qname_t *module_qname_l = NULL;

        module_desc_l = (axis2_module_desc_t *) AXIS2_ARRAY_LIST_GET(
            collection_module, env, index);
        module_qname_l = AXIS2_MODULE_DESC_GET_NAME(module_desc_l, env);        
        if(AXIS2_QNAME_EQUALS(module_qname, env, module_qname_l))
        {
            AXIS2_ERROR_SET((*env)->error, 
                AXIS2_ERROR_MODULE_ALREADY_ENGAGED_TO_OP, AXIS2_FAILURE);
            return AXIS2_SUCCESS;
        }

    }
    return AXIS2_ARRAY_LIST_ADD(collection_module, env, module_desc);
}

axis2_array_list_t *AXIS2_CALL
axis2_op_get_modules(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    axis2_property_t *property = NULL;
    axis2_array_list_t *modules = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    property =  (axis2_property_t *) 
        AXIS2_WSDL_COMPONENT_GET_COMPONENT_PROPERTY(op_impl->wsdl_op->
            extensible_component->wsdl_component, env, AXIS2_MODULEREF_KEY);
    modules = (axis2_array_list_t *) AXIS2_PROPERTY_GET_VALUE(property, env);
    return modules;
}

int AXIS2_CALL
axis2_op_get_axis_specific_mep_const(axis2_op_t *op,
                                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    int temp = 0;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    if (op_impl->mep != AXIS2_MEP_CONSTANT_INVALID) 
    {
        return op_impl->mep;
    }

    temp = AXIS2_MEP_CONSTANT_INVALID;

    if (AXIS2_STRCMP(AXIS2_MEP_URI_IN_OUT, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_IN_OUT;
    } 
    else if (AXIS2_STRCMP(AXIS2_MEP_URI_IN_ONLY, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_IN_ONLY;
    } 
    else if (AXIS2_STRCMP(AXIS2_MEP_URI_IN_OPTIONAL_OUT, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_IN_OPTIONAL_OUT;
    } 
    else if (AXIS2_STRCMP(AXIS2_MEP_URI_OUT_IN, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_OUT_IN;
    } 
    else if (AXIS2_STRCMP(AXIS2_MEP_URI_OUT_ONLY, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_OUT_ONLY;
    } 
    else if (AXIS2_STRCMP(AXIS2_MEP_URI_OUT_OPTIONAL_IN, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_OUT_OPTIONAL_IN;
    } 
    else if (AXIS2_STRCMP(AXIS2_MEP_URI_ROBUST_IN_ONLY, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_ROBUST_IN_ONLY;
    } 
    else if (AXIS2_STRCMP(AXIS2_MEP_URI_ROBUST_OUT_ONLY, 
            axis2_op_get_msg_exchange_pattern(op, env))) 
    {
        temp = AXIS2_MEP_CONSTANT_ROBUST_OUT_ONLY;
    }

    if (temp == AXIS2_MEP_CONSTANT_INVALID) 
    {
        AXIS2_ERROR_SET((*env)->error, 
            AXIS2_ERROR_COULD_NOT_MAP_MEP_URI_TO_MEP_CONSTANT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    op_impl->mep = temp;
    return op_impl->mep;

}

axis2_array_list_t *AXIS2_CALL
axis2_op_get_phases_in_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op)->phases_in_fault_flow;
}


axis2_array_list_t *AXIS2_CALL
axis2_op_get_phases_out_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op)->phases_out_fault_flow;
}


axis2_array_list_t * AXIS2_CALL
axis2_op_get_phases_outflow(axis2_op_t *op,
                                        axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op)->phases_outflow;
}


axis2_array_list_t *AXIS2_CALL
axis2_op_get_remaining_phases_inflow(axis2_op_t *op,
                                                axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
 
    return AXIS2_INTF_TO_IMPL(op)->remaining_phases_inflow;
}

axis2_status_t AXIS2_CALL
axis2_op_set_phases_in_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    if(op_impl->phases_in_fault_flow)
    {
        int i = 0;
        int size = 0;
        
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->phases_in_fault_flow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->phases_in_fault_flow, env, i);
            
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            }
               
        }
        AXIS2_WSDL_OP_FREE(op_impl->phases_in_fault_flow, env);
        op_impl->phases_in_fault_flow = NULL;
    }
    
    op_impl->phases_in_fault_flow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_set_phases_out_fault_flow(axis2_op_t *op,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    if(op_impl->phases_out_fault_flow)
    {
        int i = 0;
        int size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->phases_out_fault_flow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->phases_out_fault_flow, env, i);
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            } 
        }
        AXIS2_WSDL_OP_FREE(op_impl->phases_out_fault_flow, env);
        op_impl->phases_out_fault_flow = NULL;
    }
    
    op_impl->phases_out_fault_flow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_set_phases_outflow(axis2_op_t *op,
                                        axis2_env_t **env,
                                        axis2_array_list_t *list) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    if(op_impl->phases_outflow)
    {
        int i = 0;
        int size = 0;
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->phases_outflow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->phases_outflow, env, i);
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            }
        }
        AXIS2_WSDL_OP_FREE(op_impl->phases_outflow, env);
        op_impl->phases_outflow = NULL;
    }
    
    op_impl->phases_outflow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_set_remaining_phases_inflow(axis2_op_t *op,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, list, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    if(op_impl->remaining_phases_inflow)
    {
        int i = 0;
        int size = 0;
        
        size = AXIS2_ARRAY_LIST_SIZE(op_impl->remaining_phases_inflow, env);
        for (i = 0; i < size; i++)
        {
            axis2_phase_t *phase = NULL;
            phase = AXIS2_ARRAY_LIST_GET(op_impl->remaining_phases_inflow, env, i);
            if (phase)
            {
                AXIS2_PHASE_FREE (phase, env);
                phase = NULL;
            }
               
        }
        AXIS2_ARRAY_LIST_FREE(op_impl->remaining_phases_inflow, env);
        op_impl->remaining_phases_inflow = NULL;
    }
    op_impl->remaining_phases_inflow = list;
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_add_module(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_qname_t *module_qname) 
{
    axis2_op_impl_t *op_impl = NULL;
    axis2_qname_t *module_qname_l = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, module_qname, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    module_qname_l = AXIS2_QNAME_CLONE(module_qname, env);
    
    return AXIS2_ARRAY_LIST_ADD(op_impl->modulerefs, env, module_qname_l);
}

axis2_array_list_t *AXIS2_CALL
axis2_op_get_module_refs(axis2_op_t *op,
                                    axis2_env_t **env) 
{
    AXIS2_ENV_CHECK(env, NULL);
    return AXIS2_INTF_TO_IMPL(op)->modulerefs;
}

axis2_linked_list_t *AXIS2_CALL
axis2_op_get_in_faults(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_GET_IN_FAULTS(op_impl->wsdl_op, env);
}

axis2_status_t AXIS2_CALL
axis2_op_set_in_faults(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_linked_list_t *in_faults) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, in_faults, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_IN_FAULTS(op_impl->wsdl_op, env, in_faults);
}

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_op_get_input_msg(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_GET_INPUT_MSG(op_impl->wsdl_op, env);
}

axis2_status_t AXIS2_CALL
axis2_op_set_input_msg(axis2_op_t *op,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *input_msg) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, input_msg, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_INPUT_MSG(op_impl->wsdl_op, env, input_msg);
}

axis2_linked_list_t *AXIS2_CALL
axis2_op_get_out_faults(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_GET_OUT_FAULTS(op_impl->wsdl_op, env);
}

axis2_status_t AXIS2_CALL
axis2_op_set_out_faults(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_linked_list_t *out_faults) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, out_faults, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_OUT_FAULTS(op_impl->wsdl_op, env, out_faults);
}

struct axis2_wsdl_msg_ref *AXIS2_CALL
axis2_op_get_output_msg(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_GET_OUTPUT_MSG(op_impl->wsdl_op, env);
}

axis2_status_t AXIS2_CALL
axis2_op_set_output_msg(axis2_op_t *op,
                                axis2_env_t **env,
                                struct axis2_wsdl_msg_ref *output_msg) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, output_msg, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_OUTPUT_MSG(op_impl->wsdl_op, env, output_msg);
}

axis2_bool_t AXIS2_CALL
axis2_op_is_safe(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_IS_SAFE(op_impl->wsdl_op, env);
}

axis2_status_t AXIS2_CALL
axis2_op_set_safety(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_bool_t safe) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_SET_SAFETY(op_impl->wsdl_op, env, safe);
}

axis2_char_t *AXIS2_CALL
axis2_op_get_target_namespace(axis2_op_t *op,
                                        axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_GET_TARGET_NAMESPACE(op_impl->wsdl_op,
        env);
}

axis2_status_t AXIS2_CALL
axis2_op_add_in_fault(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *in_fault) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, in_fault, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_ADD_IN_FAULT(op_impl->wsdl_op, env, in_fault);
}

axis2_status_t AXIS2_CALL
axis2_op_add_out_fault(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_fault_ref_t *out_fault) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, out_fault, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_OP_ADD_OUT_FAULT(op_impl->wsdl_op, env, out_fault);
}

axis2_status_t AXIS2_CALL
axis2_op_add_feature(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_feature_t *feature) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, feature, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_ADD_FEATURE(op_impl->wsdl_op->
        extensible_component, env, feature);
}

axis2_linked_list_t *AXIS2_CALL
axis2_op_get_features(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_GET_FEATURES(op_impl->wsdl_op->
        extensible_component, env);
}

axis2_status_t AXIS2_CALL
axis2_op_add_property(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_property_t *wsdl_property) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_property, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_ADD_PROPERTY(op_impl->wsdl_op->
        extensible_component, env, wsdl_property);
}

axis2_linked_list_t *AXIS2_CALL
axis2_op_get_Properties(axis2_op_t *op,
                                axis2_env_t **env) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    return AXIS2_WSDL_EXTENSIBLE_COMPONENT_GET_PROPERTIES(op_impl->wsdl_op->
        extensible_component, env);
}

axis2_status_t AXIS2_CALL 
axis2_op_set_wsdl_op(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_wsdl_op_t *wsdl_op) 
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, wsdl_op, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    
    if(op_impl->wsdl_op)
    {
        AXIS2_WSDL_OP_FREE(op_impl->wsdl_op, env);
        op_impl->wsdl_op = NULL;
    }
    
    op_impl->wsdl_op = wsdl_op;
    op->base.ops->free_void_arg = axis2_op_free_void_arg;
    op->base.ops->get_qname = axis2_op_get_qname;
    return AXIS2_SUCCESS;
}

axis2_op_ctx_t *AXIS2_CALL
axis2_op_find_op_ctx(axis2_op_t *op,
                        axis2_env_t **env,
                        struct axis2_msg_ctx *msg_ctx, 
                        struct axis2_svc_ctx *svc_ctx)
{
    axis2_op_impl_t *op_impl = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_relates_to_t *relates_to = NULL;
    axis2_status_t status = AXIS2_FAILURE;
        
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, NULL);
    AXIS2_PARAM_CHECK((*env)->error, svc_ctx, NULL);

    op_impl = AXIS2_INTF_TO_IMPL(op);
    relates_to = AXIS2_MSG_CTX_GET_RELATES_TO(msg_ctx, env);
    if(NULL == relates_to)
    {
        /* Its a new incomming message so get_ the factory to create a new
            one */
        op_ctx = axis2_op_ctx_create(env, op, svc_ctx);
        if(!op_ctx)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
            return NULL;
        }
    }
    else
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_char_t *value = NULL;
            
        /* So this message is part of an ongoing MEP
        			opContext = */
        conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        value = AXIS2_RELATES_TO_GET_VALUE(relates_to, env);
        op_ctx = AXIS2_CONF_CTX_GET_OP_CTX(conf_ctx, env, value);
        if(NULL == op_ctx)
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CANNOT_CORRELATE_MSG, 
                AXIS2_FAILURE);
            return NULL;
        }
    }
    
    status = axis2_op_register_op_ctx(op, env, msg_ctx, op_ctx);
    if(AXIS2_FAILURE == status)
    {
        AXIS2_OP_CTX_FREE(op_ctx, env);
        return NULL;
    }
    else
        return op_ctx;
}

axis2_op_ctx_t *AXIS2_CALL
axis2_op_find_for_existing_op_ctx(axis2_op_t *op,
                                    axis2_env_t **env,
                                    axis2_msg_ctx_t *msg_ctx)
{
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_relates_to_t *relates_to = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, NULL);
    
    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    if(NULL != op_ctx) 
    {
        return op_ctx;
    }

    relates_to = AXIS2_MSG_CTX_GET_RELATES_TO(msg_ctx, env);
    if (NULL == relates_to) 
    {
        return NULL;
    } 
    else 
    {
        axis2_conf_ctx_t *conf_ctx = NULL;
        axis2_char_t *value = NULL;
        /* So this message is part of an ongoing MEP
        			opContext = */
        conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
        op_ctx = AXIS2_CONF_CTX_GET_OP_CTX(conf_ctx, env, value);

        if (NULL == op_ctx) 
        {
            AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_CANNOT_CORRELATE_MSG, 
                AXIS2_FAILURE);
            return NULL;
        }
    }

    return op_ctx;

}

axis2_status_t AXIS2_CALL
axis2_op_register_op_ctx(axis2_op_t *op,
                            axis2_env_t **env,
                            axis2_msg_ctx_t *msg_ctx,
                            axis2_op_ctx_t *op_ctx)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_char_t *msg_id = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op_ctx, AXIS2_FAILURE);
    
    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);
    if(!conf_ctx)
    {
        return AXIS2_FAILURE;
    }
    msg_id = AXIS2_MSG_CTX_GET_MSG_ID(msg_ctx, env);
    if(!msg_id)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_CONF_CTX_REGISTER_OP_CTX(conf_ctx, env, msg_id, op_ctx);
    if(AXIS2_FAILURE == status)
    {
        return AXIS2_FAILURE;
    }
    status = AXIS2_MSG_CTX_SET_OP_CTX(msg_ctx, env, op_ctx);
    if(AXIS2_FAILURE == status)
    {
        axis2_hash_t *op_ctx_map = NULL;
        op_ctx_map = (axis2_hash_t *) AXIS2_CONF_CTX_GET_OP_CTX_MAP(conf_ctx, env);
        axis2_hash_set(op_ctx_map, msg_id, AXIS2_HASH_KEY_STRING, NULL); 
    }
    if(AXIS2_TRUE == AXIS2_OP_CTX_GET_IS_COMPLETE(op_ctx, env))
    {
        AXIS2_OP_CTX_CLEANUP(op_ctx, env);
    }
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_in_only(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx)
{
    axis2_op_impl_t *op_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op_ctx, AXIS2_FAILURE);
    op_impl = AXIS2_INTF_TO_IMPL(op);
    if(AXIS2_TRUE != AXIS2_OP_CTX_GET_IS_COMPLETE(op_ctx, env))
    {
        axis2_hash_t *msg_ctxs = NULL;
        msg_ctxs = AXIS2_OP_CTX_GET_MSG_CTX_MAP(op_ctx, env);
        axis2_hash_set(msg_ctxs, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE,
            AXIS2_HASH_KEY_STRING, msg_ctx);
        AXIS2_OP_CTX_SET_IS_COMPLETE(op_ctx, env, AXIS2_TRUE);
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MESSAGE_ADDITION,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_out_only(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op_ctx, AXIS2_FAILURE);
    
    if(AXIS2_TRUE != AXIS2_OP_CTX_GET_IS_COMPLETE(op_ctx, env))
    {
        axis2_hash_t *msg_ctxs = NULL;
        msg_ctxs = AXIS2_OP_CTX_GET_MSG_CTX_MAP(op_ctx, env);
        axis2_hash_set(msg_ctxs, AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE,
            AXIS2_HASH_KEY_STRING, msg_ctx);
        AXIS2_OP_CTX_SET_IS_COMPLETE(op_ctx, env, AXIS2_TRUE);
    }
    else
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MESSAGE_ADDITION,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_in_out(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx)
{
    axis2_hash_t *mep = NULL;
    axis2_msg_ctx_t *in_msg_ctx = NULL;
    axis2_msg_ctx_t *out_msg_ctx = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op_ctx, AXIS2_FAILURE);
    
    mep = AXIS2_OP_CTX_GET_MSG_CTX_MAP(op_ctx, env);
    in_msg_ctx = (axis2_msg_ctx_t *) axis2_hash_get(mep, 
        AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING);
    out_msg_ctx = (axis2_msg_ctx_t *) axis2_hash_get(mep, 
        AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING);
    if(NULL != in_msg_ctx && NULL != out_msg_ctx)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MESSAGE_ADDITION,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if(NULL == in_msg_ctx)
    {
        axis2_hash_set(mep, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING,
            msg_ctx);        
    }
    else
    {
        axis2_hash_set(mep, AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING,
            msg_ctx);
        AXIS2_OP_CTX_SET_IS_COMPLETE(op_ctx, env, AXIS2_TRUE);        
    }
    return AXIS2_SUCCESS;
}
    
axis2_status_t AXIS2_CALL
axis2_op_add_msg_ctx_out_in(axis2_op_t *op,
                                axis2_env_t **env,
                                axis2_msg_ctx_t *msg_ctx,
                                axis2_op_ctx_t *op_ctx)
{
    axis2_hash_t *mep = NULL;
    axis2_msg_ctx_t *in_msg_ctx = NULL;
    axis2_msg_ctx_t *out_msg_ctx = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, op_ctx, AXIS2_FAILURE);
    
    mep = AXIS2_OP_CTX_GET_MSG_CTX_MAP(op_ctx, env);
    in_msg_ctx = (axis2_msg_ctx_t *) axis2_hash_get(mep, 
        AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING);
    out_msg_ctx = (axis2_msg_ctx_t *) axis2_hash_get(mep, 
        AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING);
    if(NULL != in_msg_ctx && NULL != out_msg_ctx)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_INVALID_MESSAGE_ADDITION,
            AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    if(NULL == out_msg_ctx)
    {
        axis2_hash_set(mep, AXIS2_WSDL_MESSAGE_LABEL_OUT_VALUE, AXIS2_HASH_KEY_STRING,
            msg_ctx);        
    }
    else
    {
        axis2_hash_set(mep, AXIS2_WSDL_MESSAGE_LABEL_IN_VALUE, AXIS2_HASH_KEY_STRING,
            msg_ctx);
        AXIS2_OP_CTX_SET_IS_COMPLETE(op_ctx, env, AXIS2_TRUE);        
    }
    return AXIS2_SUCCESS;
}
