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

#ifndef AXIS2_OPERATION_H
#define AXIS2_OPERATION_H

/**
  * @file axis2_op.h
  * @brief axis2 op interface
  */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_wsdl_op.h>
#include <axis2_svc.h>
#include <axis2_msg_recv.h>
#include <axis2_array_list.h>
#include <axis2_module_desc.h>
#include <axis2_wsdl_msg_ref.h>
#include <axis2_description.h>
#include <axis2_phase_meta.h>


#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_op Operation Description
  * @ingroup axis2_core_description
  * @{
  */

struct axis2_wsdl_op;    
struct axis2_svc;
struct axis2_msg_recv;
struct axis2_param_container;
struct axis2_module_desc; 
struct axis2_op;
struct axis2_wsdl_feature;
struct axis2_wsdl_property; 
struct axis2_wsdl_fault_ref;    
typedef struct axis2_op_ops axis2_op_ops_t;    
typedef struct axis2_op axis2_op_t;    
    
/** 
 * @brief Operation ops struct
 * Encapsulator struct for ops of axis2_op
 */    
AXIS2_DECLARE_DATA struct axis2_op_ops
{
    /** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free) (axis2_op_t *op, 
           axis2_env_t **env);

    /**
     * Method addParameter
     *
     * @param param Parameter that will be added
     */
	axis2_status_t (AXIS2_CALL *
    add_param)(axis2_op_t *op, 
                axis2_env_t **env, 
                axis2_param_t *param);
    
    /**
     * Method getParameter
     *
     * @param name Name of the parameter
     */
	axis2_param_t *(AXIS2_CALL *
    get_param)(axis2_op_t *op, 
                axis2_env_t **env,
                axis2_char_t *name);


	axis2_array_list_t *(AXIS2_CALL *
    get_params) (axis2_op_t *op, 
                    axis2_env_t **env);

    /**
     * To check whether a given paramter is locked
     */
	axis2_bool_t (AXIS2_CALL *
    is_param_locked) (axis2_op_t *op, 
                        axis2_env_t **env,
                        axis2_char_t *param_name);
	
	axis2_status_t (AXIS2_CALL *
    set_parent) (axis2_op_t *op, 
                                     axis2_env_t **env,
                                     struct axis2_svc *service_desc);

	struct axis2_svc *(AXIS2_CALL *
    get_parent) (axis2_op_t *op, 
                 axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_name) (axis2_op_t *op, 
                axis2_env_t **env,
                axis2_qname_t *qname);
    
	axis2_qname_t *(AXIS2_CALL *
    get_name) (axis2_op_t *op, 
                axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
    set_msg_exchange_pattern) (axis2_op_t *op, 
                                axis2_env_t **env,
                                const axis2_char_t *pattern);

	axis2_char_t *(AXIS2_CALL *
    get_msg_exchange_pattern) (axis2_op_t *op, 
                                axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
    set_msg_recv) (axis2_op_t *op, 
                    axis2_env_t **env,
                    struct axis2_msg_recv *msg_recv);

	struct axis2_msg_recv *(AXIS2_CALL *
        get_msg_recv) (axis2_op_t *op, 
                       axis2_env_t **env);
                                        
    axis2_char_t *(AXIS2_CALL *
    get_style) (axis2_op_t *op,
                axis2_env_t **env);

    axis2_status_t  (AXIS2_CALL *
    set_style) (axis2_op_t *op,
                axis2_env_t **env,
                axis2_char_t *style);
                                                
    /**
     * To ebgage a module it is reuired to use this method
     *
     * @param moduleref
     * @throws org.apache.axis2.AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    engage_module) (axis2_op_t *op,
                        axis2_env_t **env,
                        struct axis2_module_desc *moduleref);
    
    axis2_status_t (AXIS2_CALL *
    add_to_engage_module_list) (axis2_op_t *op,
                                axis2_env_t **env,
                                struct axis2_module_desc *module_name);
    
    /**
     * Method getEngadgedModules
     *
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_modules) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    /**
     * This method will simply map the String URI of the Message exchange
     * pattern to a integer. Further in the first lookup it will cash the looked
     * up value so that the subsequent method calls will be extremely efficient.
     *
     */
    int (AXIS2_CALL *
    get_axis_specific_MEP_constant) (axis2_op_t *op,
                                                    axis2_env_t **env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_phases_in_fault_flow) (axis2_op_t *op,
                                                axis2_env_t **env);
    
    
    axis2_array_list_t *(AXIS2_CALL *
    get_phases_out_fault_flow) (axis2_op_t *op,
                                                axis2_env_t **env);
    
    
    axis2_array_list_t * (AXIS2_CALL *
    get_phases_outflow) (axis2_op_t *op,
                                            axis2_env_t **env);
    
    
    axis2_array_list_t *(AXIS2_CALL *
    get_remaining_phases_inflow) (axis2_op_t *op,
                                                    axis2_env_t **env);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_phases_in_fault_flow) (axis2_op_t *op,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_phases_out_fault_flow) (axis2_op_t *op,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_phases_outflow) (axis2_op_t *op,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_remaining_phases_inflow) (axis2_op_t *op,
                                                    axis2_env_t **env,
                                                    axis2_array_list_t *list);
                                                    
    axis2_status_t (AXIS2_CALL *
    add_module) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_name);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_module_refs) (axis2_op_t *op,
                                        axis2_env_t **env);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_in_faults) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_in_faults) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *in_faults);
    
    struct axis2_wsdl_msg_ref *(AXIS2_CALL *
    get_input_msg) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_input_msg) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_msg_ref *input_msg);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_out_faults) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_out_faults) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *out_faults);
    
    struct axis2_wsdl_msg_ref *(AXIS2_CALL *
    get_output_msg) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_output_msg) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_msg_ref *output_msg);
    
    axis2_bool_t (AXIS2_CALL *
    is_safe) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_safety) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    axis2_bool_t safe);
                                    
    axis2_char_t *(AXIS2_CALL *
    get_target_namespace) (axis2_op_t *op,
                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_in_fault) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_fault_ref *in_fault);
    
    axis2_status_t (AXIS2_CALL *
    add_out_fault) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_fault_ref *out_fault);
    
    axis2_status_t (AXIS2_CALL *
    add_feature) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_feature *feature);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_features) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_property) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_property *wsdl_property);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_Properties) (axis2_op_t *op,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL * 
    set_wsdl_opeartion) (axis2_op_t *op,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_op *wsdl_op);                                           
};

/** 
 * @brief Operaton struct
 *	Axis2 Operation   
 */  
AXIS2_DECLARE_DATA struct axis2_op
{
	axis2_op_ops_t *ops;
    struct axis2_wsdl_op *wsdl_op;
    struct axis2_param_container *param_container;
};

/** 
 * Creates op struct
 * @return pointer to newly created op
 */
AXIS2_DECLARE(axis2_op_t *)
axis2_op_create (axis2_env_t **env);

/** 
 * Creates op struct with name
 * @param name op name
 * @return pointer to newly created op
 */
AXIS2_DECLARE(axis2_op_t *)
axis2_op_create_with_name (axis2_env_t **env, 
                                                axis2_qname_t *name);

/** 
 * Creates op struct with wsdl op
 * @param wsdl_op wsdl op
 * @return pointer to newly created op
 */
AXIS2_DECLARE(axis2_op_t *) 
axis2_op_create_with_wsdl_op (axis2_env_t **env, 
                                            struct axis2_wsdl_op *wsdl_op);

/************************** Start of function macros **************************/

#define AXIS2_OPERATION_FREE(op, env) \
        ((op->ops)->free (op, env))

#define AXIS2_OPERATION_ADD_PARAM(op, env, param) \
		((op->ops)->add_param (op, env, param))

#define AXIS2_OPERATION_GET_PARAM(op, env) \
		((op->ops)->get_param (op, env))

#define AXIS2_OPERATION_GET_PARAMS(op, env) \
		((op->ops)->get_params (op, env))

#define AXIS2_OPERATION_IS_PARAM_LOCKED(op, env, param_name) \
        ((op->ops)->is_param_locked(op, env, param_name))

#define AXIS2_OPERATION_SET_PARENT(op, env, service_desc) \
        ((op->ops)->set_parent (op, env, service_desc))

#define AXIS2_OPERATION_GET_PARENT(op, env) \
		((op->ops)->get_parent (op, env))

#define AXIS2_OPERATION_SET_MSG_RECEIVER(op, env, msg_recv) \
        ((op->ops)->set_msg_recv (op, env, msg_recv))

#define AXIS2_OPERATION_GET_MSG_RECEIVER(op, env) \
		((op->ops)->get_msg_recv (op, env))

#define AXIS2_OPERATION_SET_NAME(op, env, qname) \
		((op->ops)->set_name (op, env, qname))
        
#define AXIS2_OPERATION_GET_NAME(op, env) \
		((op->ops)->get_name (op, env))

#define AXIS2_OPERATION_SET_MSG_EXCHANGE_PATTERN(op , env, \
        msg_exchange_pattern) \
		((op->ops)->set_msg_exchange_pattern (op, env, \
        msg_exchange_pattern))

#define AXIS2_OPERATION_GET_MSG_EXCHANGE_PATTERN(op, env) \
		((op->ops)->get_msg_exchange_pattern (op, env))
        
#define AXIS2_OPERATION_GET_STYLE(op, env) \
		((op->ops)->get_style (op, env))
        
#define AXIS2_OPERATION_SET_STYLE(op, env, style) \
		((op->ops)->set_style (op, env, style))   

#define AXIS2_OPERATION_ENGAGE_MODULE(op, env, moduleref) \
		((op->ops)->engage_module (op, env, moduleref))
        
#define AXIS2_OPERATION_ADD_TO_ENGAGE_MODULE_LIST(op, env, module_name) \
		((op->ops)->add_to_engage_module_list (op, env, module_name))

#define AXIS2_OPERATION_GET_MODULES(op, env) \
		((op->ops)->get_modules (op, env))
        
#define AXIS2_OPERATION_GET_AXIS_SPECIFIC_MEP_CONSTANT(op, env) \
		((op->ops)->get_axis_specific_MEP_constant (op, env))

#define AXIS2_OPERATION_GET_PHASES_IN_FAULT_FLOW(op, env) \
		((op->ops)->get_phases_in_fault_flow (op, env))
        
#define AXIS2_OPERATION_GET_PHASES_OUT_FAULT_FLOW(op, env) \
		((op->ops)->get_phases_out_fault_flow (op, env))

#define AXIS2_OPERATION_GET_PHASES_OUTFLOW(op, env) \
		((op->ops)->get_phases_outflow (op, env))
 
#define AXIS2_OPERATION_GET_REMAINING_PHASES_INFLOW(op, env) \
		((op->ops)->get_remaining_phases_inflow (op, env))    

#define AXIS2_OPERATION_SET_PHASES_IN_FAULT_FLOW(op, env, list) \
		((op->ops)->set_phases_in_fault_flow (op, env, list))    

#define AXIS2_OPERATION_SET_PHASES_OUT_FAULT_FLOW(op, env, list) \
		((op->ops)->set_phases_out_fault_flow (op, env, list))   

#define AXIS2_OPERATION_SET_PHASES_OUTFLOW(op, env, list) \
		((op->ops)->set_phases_outflow (op, env, list)) 
        
#define AXIS2_OPERATION_SET_REMAINING_PHASES_INFLOW(op, env, list) \
		((op->ops)->set_remaining_phases_inflow (op, env, list))
        
#define AXIS2_OPERATION_ADD_MODULE(op, env, module_name) \
		((op->ops)->add_module (op, env, module_name))

#define AXIS2_OPERATION_GET_MODULE_REFS(op, env) \
		((op->ops)->get_module_refs (op, env))
        
#define AXIS2_OPERATION_GET_INFAULTS(op, env) \
		((op->ops)->get_in_faults (op, env))

#define AXIS2_OPERATION_SET_INFAULTS(op, env) \
		((op->ops)->set_in_faults (op, env, in_faults))
        
#define AXIS2_OPERATION_GET_INPUT_MSG(op, env) \
		((op->ops)->get_input_msg (op, env))

#define AXIS2_OPERATION_SET_INPUT_MSG(op, env) \
		((op->ops)->set_input_msg (op, env, input_msg))
        
#define AXIS2_OPERATION_GET_OUTFAULTS(op, env) \
		((op->ops)->get_out_faults (op, env))

#define AXIS2_OPERATION_SET_OUTFAULTS(op, env) \
		((op->ops)->set_out_faults (op, env, out_faults))
        
#define AXIS2_OPERATION_GET_OUTPUT_MSG(op, env) \
		((op->ops)->get_output_msg (op, env))

#define AXIS2_OPERATION_SET_OUTPUT_MSG(op, env) \
		((op->ops)->set_output_msg (op, env, output_msg))
        
#define AXIS2_OPERATION_IS_SAFE(op, env) \
		((op->ops)->is_safe (op, env))

#define AXIS2_OPERATION_SET_SAFETY(op, env, safe) \
		((op->ops)->set_safety (op, env, safe))
        
#define AXIS2_OPERATION_GET_TARGET_NAMESPACE(op, env) \
		((op->ops)->get_target_namespace (op, env))

#define AXIS2_OPERATION_ADD_INFAULT(op, env, in_fault) \
		((op->ops)->add_in_fault (op, env, in_fault))
        
#define AXIS2_OPERATION_ADD_OUTFAULT(op, env, out_fault) \
		((op->ops)->add_out_fault (op, env, out_fault))

#define AXIS2_OPERATION_ADD_FEATURE(op, env, feature) \
		((op->ops)->add_feature (op, env, feature))
        
#define AXIS2_OPERATION_GET_FEATURES(op, env) \
		((op->ops)->get_features(op, env))

#define AXIS2_OPERATION_ADD_PROPERTY(op, env, wsdl_property) \
		((op->ops)->add_property (op, env, wsdl_property))
        
#define AXIS2_OPERATION_GET_PROPERTIES(op, env) \
		((op->ops)->get_properties (op, env, properties))

#define AXIS2_OPERATION_SET_WSDL_OPERATION(op, env) \
		((op->ops)->set_wsdl_opeartion (op, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_OPERATION_H */
