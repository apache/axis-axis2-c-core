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
  * @file axis2_operation.h
  * @brief axis2 operation interface
  */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_wsdl_operation.h>
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

/** @defgroup axis2_operation Operation Description
  * @ingroup axis2_core_description
  * @{
  */

struct axis2_wsdl_operation;    
struct axis2_svc;
struct axis2_msg_recv;
struct axis2_param_container;
struct axis2_module_desc; 
struct axis2_operation;    
typedef struct axis2_operation_ops axis2_operation_ops_t;    
typedef struct axis2_operation axis2_operation_t;    
    
/** 
 * @brief Operation operations struct
 * Encapsulator struct for operations of axis2_operation
 */    
AXIS2_DECLARE_DATA struct axis2_operation_ops
{
    /** De-allocate memory
  	 * @return status code
  	 */
	axis2_status_t (AXIS2_CALL *
    free) (axis2_operation_t *operation, 
           axis2_env_t **env);

    /**
     * Method addParameter
     *
     * @param param Parameter that will be added
     */
	axis2_status_t (AXIS2_CALL *
    add_param)(axis2_operation_t *operation, 
                axis2_env_t **env, 
                axis2_param_t *param);
    
    /**
     * Method getParameter
     *
     * @param name Name of the parameter
     */
	axis2_param_t *(AXIS2_CALL *
    get_param)(axis2_operation_t *operation, 
                axis2_env_t **env,
                const axis2_char_t *name);


	axis2_array_list_t *(AXIS2_CALL *
    get_params) (axis2_operation_t *operation, 
                    axis2_env_t **env);

    /**
     * To check whether a given paramter is locked
     */
	axis2_bool_t (AXIS2_CALL *
    is_param_locked) (axis2_operation_t *operation, 
                        axis2_env_t **env,
                        const axis2_char_t *param_name);
	
	axis2_status_t (AXIS2_CALL *
    set_parent) (axis2_operation_t *operation, 
                                     axis2_env_t **env,
                                     struct axis2_svc *service_desc);

	struct axis2_svc *(AXIS2_CALL *
    get_parent) (axis2_operation_t *operation, 
                 axis2_env_t **env);

    axis2_status_t (AXIS2_CALL *
    set_name) (axis2_operation_t *operation, 
                axis2_env_t **env,
                axis2_qname_t *qname);
    
	axis2_qname_t *(AXIS2_CALL *
    get_name) (axis2_operation_t *operation, 
                axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
    set_msg_exchange_pattern) (axis2_operation_t *operation, 
                                axis2_env_t **env,
                                const axis2_char_t *pattern);

	axis2_char_t *(AXIS2_CALL *
    get_msg_exchange_pattern) (axis2_operation_t *operation, 
                                axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
    set_msg_recv) (axis2_operation_t *operation, 
                    axis2_env_t **env,
                    struct axis2_msg_recv *msg_recv);

	struct axis2_msg_recv *(AXIS2_CALL *
        get_msg_recv) (axis2_operation_t *operation, 
                       axis2_env_t **env);
                                        
    axis2_char_t *(AXIS2_CALL *
    get_style) (axis2_operation_t *operation,
                axis2_env_t **env);

    axis2_status_t  (AXIS2_CALL *
    set_style) (axis2_operation_t *operation,
                axis2_env_t **env,
                axis2_char_t *style);
                                                
    /**
     * To ebgage a module it is reuired to use this method
     *
     * @param moduleref
     * @throws org.apache.axis2.AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    engage_module) (axis2_operation_t *operation,
                        axis2_env_t **env,
                        struct axis2_module_desc *moduleref);
    
    axis2_status_t (AXIS2_CALL *
    add_to_engage_module_list) (axis2_operation_t *operation,
                                axis2_env_t **env,
                                struct axis2_module_desc *module_name);
    
    /**
     * Method getEngadgedModules
     *
     */
    axis2_array_list_t *(AXIS2_CALL *
    get_modules) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    /**
     * This method will simply map the String URI of the Message exchange
     * pattern to a integer. Further in the first lookup it will cash the looked
     * up value so that the subsequent method calls will be extremely efficient.
     *
     */
    int (AXIS2_CALL *
    get_axis_specific_MEP_constant) (axis2_operation_t *operation,
                                                    axis2_env_t **env);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_phases_in_fault_flow) (axis2_operation_t *operation,
                                                axis2_env_t **env);
    
    
    axis2_array_list_t *(AXIS2_CALL *
    get_phases_out_fault_flow) (axis2_operation_t *operation,
                                                axis2_env_t **env);
    
    
    axis2_array_list_t * (AXIS2_CALL *
    get_phases_outflow) (axis2_operation_t *operation,
                                            axis2_env_t **env);
    
    
    axis2_array_list_t *(AXIS2_CALL *
    get_remaining_phases_inflow) (axis2_operation_t *operation,
                                                    axis2_env_t **env);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_phases_in_fault_flow) (axis2_operation_t *operation,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_phases_out_fault_flow) (axis2_operation_t *operation,
                                                axis2_env_t **env,
                                                axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_phases_outflow) (axis2_operation_t *operation,
                                            axis2_env_t **env,
                                            axis2_array_list_t *list);
    
    /**
     * @param list
     */
    axis2_status_t (AXIS2_CALL *
    set_remaining_phases_inflow) (axis2_operation_t *operation,
                                                    axis2_env_t **env,
                                                    axis2_array_list_t *list);
                                                    
    axis2_status_t (AXIS2_CALL *
    add_module) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_qname_t *module_name);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_module_refs) (axis2_operation_t *operation,
                                        axis2_env_t **env);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_infaults) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_infaults) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *infaults);
    
    struct axis2_wsdl_msg_ref *(AXIS2_CALL *
    get_input_msg) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_input_msg) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_msg_ref *input_msg);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_outfaults) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_outfaults) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_linked_list_t *outfaults);
    
    struct axis2_wsdl_msg_ref *(AXIS2_CALL *
    get_output_msg) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_output_msg) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_msg_ref *output_msg);
    
    axis2_bool_t (AXIS2_CALL *
    is_safe) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_safety) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_bool_t safe);
                                    
    axis2_char_t *(AXIS2_CALL *
    get_target_namespace) (axis2_operation_t *operation,
                                            axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_infault) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_wsdl_fault_ref_t *infault);
    
    axis2_status_t (AXIS2_CALL *
    add_outfault) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_wsdl_fault_ref_t *outfault);
    
    axis2_status_t (AXIS2_CALL *
    add_feature) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_wsdl_feature_t *feature);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_features) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    add_property) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    axis2_wsdl_property_t *wsdl_property);
    
    axis2_linked_list_t *(AXIS2_CALL *
    get_Properties) (axis2_operation_t *operation,
                                    axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL * 
    set_wsdl_opeartion) (axis2_operation_t *operation,
                                    axis2_env_t **env,
                                    struct axis2_wsdl_operation *wsdl_operation);                                           
};

/** 
 * @brief Operaton struct
 *	Axis2 Operation   
 */  
AXIS2_DECLARE_DATA struct axis2_operation
{
	axis2_operation_ops_t *ops;
    struct axis2_wsdl_operation *wsdl_operation;
    struct axis2_param_container *param_container;
};

/** 
 * Creates operation struct
 * @return pointer to newly created operation
 */
AXIS2_DECLARE(axis2_operation_t *)
axis2_operation_create (axis2_env_t **env);

/** 
 * Creates operation struct with name
 * @param name operation name
 * @return pointer to newly created operation
 */
AXIS2_DECLARE(axis2_operation_t *)
axis2_operation_create_with_name (axis2_env_t **env, 
                                                axis2_qname_t *name);

/** 
 * Creates operation struct with wsdl operation
 * @param wsdl_operation wsdl operation
 * @return pointer to newly created operation
 */
AXIS2_DECLARE(axis2_operation_t *) 
axis2_operation_create_with_wsdl_operation (axis2_env_t **env, 
                                            struct axis2_wsdl_operation *wsdl_operation);

/************************** Start of function macros **************************/

#define AXIS2_OPERATION_FREE(operation, env) \
        ((operation->ops)->free (operation, env))

#define AXIS2_OPERATION_ADD_PARAM(operation, env, param) \
		((operation->ops)->add_param (operation, env, param))

#define AXIS2_OPERATION_GET_PARAM(operation, env) \
		((operation->ops)->get_param (operation, env))

#define AXIS2_OPERATION_GET_PARAMS(operation, env) \
		((operation->ops)->get_params (operation, env))

#define AXIS2_OPERATION_IS_PARAM_LOCKED(operation, env, param_name) \
        ((operation->ops)->is_param_locked(operation, env, param_name))

#define AXIS2_OPERATION_SET_PARENT(operation, env, service_desc) \
        ((operation->ops)->set_parent (operation, env, service_desc))

#define AXIS2_OPERATION_GET_PARENT(operation, env) \
		((operation->ops)->get_parent (operation, env))

#define AXIS2_OPERATION_SET_MSG_RECEIVER(operation, env, msg_recv) \
        ((operation->ops)->set_msg_recv (operation, env, msg_recv))

#define AXIS2_OPERATION_GET_MSG_RECEIVER(operation, env) \
		((operation->ops)->get_msg_recv (operation, env))

#define AXIS2_OPERATION_SET_NAME(operation, env, qname) \
		((operation->ops)->set_name (operation, env, qname))
        
#define AXIS2_OPERATION_GET_NAME(operation, env) \
		((operation->ops)->get_name (operation, env))

#define AXIS2_OPERATION_SET_MSG_EXCHANGE_PATTERN(operation , env, \
        msg_exchange_pattern) \
		((operation->ops)->set_msg_exchange_pattern (operation, env, \
        msg_exchange_pattern))

#define AXIS2_OPERATION_GET_MSG_EXCHANGE_PATTERN(operation, env) \
		((operation->ops)->get_msg_exchange_pattern (operation, env))
        
#define AXIS2_OPERATION_GET_STYLE(operation, env) \
		((operation->ops)->get_style (operation, env))
        
#define AXIS2_OPERATION_SET_STYLE(operation, env, style) \
		((operation->ops)->set_style (operation, env, style))   

#define AXIS2_OPERATION_ENGAGE_MODULE(operation, env, moduleref) \
		((operation->ops)->engage_module (operation, env, moduleref))
        
#define AXIS2_OPERATION_ADD_TO_ENGAGE_MODULE_LIST(operation, env, module_name) \
		((operation->ops)->add_to_engage_module_list (operation, env, module_name))

#define AXIS2_OPERATION_GET_MODULES(operation, env) \
		((operation->ops)->get_modules (operation, env))
        
#define AXIS2_OPERATION_GET_AXIS_SPECIFIC_MEP_CONSTANT(operation, env) \
		((operation->ops)->get_axis_specific_MEP_constant (operation, env))

#define AXIS2_OPERATION_GET_PHASES_IN_FAULT_FLOW(operation, env) \
		((operation->ops)->get_phases_in_fault_flow (operation, env))
        
#define AXIS2_OPERATION_GET_PHASES_OUT_FAULT_FLOW(operation, env) \
		((operation->ops)->get_phases_out_fault_flow (operation, env))

#define AXIS2_OPERATION_GET_PHASES_OUTFLOW(operation, env) \
		((operation->ops)->get_phases_outflow (operation, env))
 
#define AXIS2_OPERATION_GET_REMAINING_PHASES_INFLOW(operation, env) \
		((operation->ops)->get_remaining_phases_inflow (operation, env))    

#define AXIS2_OPERATION_SET_PHASES_IN_FAULT_FLOW(operation, env, list) \
		((operation->ops)->set_phases_in_fault_flow (operation, env, list))    

#define AXIS2_OPERATION_SET_PHASES_OUT_FAULT_FLOW(operation, env, list) \
		((operation->ops)->set_phases_out_fault_flow (operation, env, list))   

#define AXIS2_OPERATION_SET_PHASES_OUTFLOW(operation, env, list) \
		((operation->ops)->set_phases_outflow (operation, env, list)) 
        
#define AXIS2_OPERATION_SET_REMAINING_PHASES_INFLOW(operation, env, list) \
		((operation->ops)->set_remaining_phases_inflow (operation, env, list))
        
#define AXIS2_OPERATION_ADD_MODULE(operation, env, module_name) \
		((operation->ops)->add_module (operation, env, module_name))

#define AXIS2_OPERATION_GET_MODULE_REFS(operation, env) \
		((operation->ops)->get_module_refs (operation, env))
        
#define AXIS2_OPERATION_GET_INFAULTS(operation, env) \
		((operation->ops)->get_infaults (operation, env))

#define AXIS2_OPERATION_SET_INFAULTS(operation, env) \
		((operation->ops)->set_infaults (operation, env, infaults))
        
#define AXIS2_OPERATION_GET_INPUT_MSG(operation, env) \
		((operation->ops)->get_input_msg (operation, env))

#define AXIS2_OPERATION_SET_INPUT_MSG(operation, env) \
		((operation->ops)->set_input_msg (operation, env, input_msg))
        
#define AXIS2_OPERATION_GET_OUTFAULTS(operation, env) \
		((operation->ops)->get_outfaults (operation, env))

#define AXIS2_OPERATION_SET_OUTFAULTS(operation, env) \
		((operation->ops)->set_outfaults (operation, env, outfaults))
        
#define AXIS2_OPERATION_GET_OUTPUT_MSG(operation, env) \
		((operation->ops)->get_output_msg (operation, env))

#define AXIS2_OPERATION_SET_OUTPUT_MSG(operation, env) \
		((operation->ops)->set_output_msg (operation, env, output_msg))
        
#define AXIS2_OPERATION_IS_SAFE(operation, env) \
		((operation->ops)->is_safe (operation, env))

#define AXIS2_OPERATION_SET_SAFETY(operation, env, safe) \
		((operation->ops)->set_safety (operation, env, safe))
        
#define AXIS2_OPERATION_GET_TARGET_NAMESPACE(operation, env) \
		((operation->ops)->get_target_namespace (operation, env))

#define AXIS2_OPERATION_ADD_INFAULT(operation, env, infault) \
		((operation->ops)->add_infault (operation, env, infault))
        
#define AXIS2_OPERATION_ADD_OUTFAULT(operation, env, outfault) \
		((operation->ops)->add_outfault (operation, env, outfault))

#define AXIS2_OPERATION_ADD_FEATURE(operation, env, feature) \
		((operation->ops)->add_feature (operation, env, feature))
        
#define AXIS2_OPERATION_GET_FEATURES(operation, env) \
		((operation->ops)->get_features(operation, env))

#define AXIS2_OPERATION_ADD_PROPERTY(operation, env, wsdl_property) \
		((operation->ops)->add_property (operation, env, wsdl_property))
        
#define AXIS2_OPERATION_GET_PROPERTIES(operation, env) \
		((operation->ops)->get_properties (operation, env, properties))

#define AXIS2_OPERATION_SET_WSDL_OPERATION(operation, env) \
		((operation->ops)->set_wsdl_opeartion (operation, env))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_OPERATION_H */
