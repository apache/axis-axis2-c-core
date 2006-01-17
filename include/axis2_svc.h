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

#ifndef AXIS2_SVC_H
#define AXIS2_SVC_H

/**
  * @file axis2_svc.h
  * @brief axis2 service interface
  */

#include <axis2_core.h>
#include <axis2_param_container.h>
#include <axis2_flow_container.h>
#include <axis2_wsdl_svc.h>
#include <axis2_op.h>
#include <axis2_svc_grp.h>
#include <axis2_qname.h>
#include <axis2_error.h>
#include <axis2_array_list.h>
#include <axis2.h>
#include <axis2_phase_resolver.h>
#include <axis2_module_desc.h>
#include <axis2_conf.h>
#include <axis2_wsdl_soap_op.h>
#include <axis2_string.h>
#include <axis2_wsdl_endpoint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc Service description
  * @ingroup axis2_core_description
  * @{
  */

struct axis2_wsdl_endpoint;
struct axis2_svc_grp;
struct axis2_op;
struct axis2_flow_container;
struct axis2_param_container;
struct axis2_wsdl_svc;
struct axis2_wsdl_interface; 
struct axis2_module_desc; 
struct axis2_conf;
struct axis2_wsdl_soap_op;    
typedef struct axis2_svc_ops axis2_svc_ops_t;   
typedef struct axis2_svc axis2_svc_t;
    
/** 
 * @brief Service ops struct
 * Encapsulator struct for ops of axis2_svc
 */    
struct axis2_svc_ops
{
	axis2_status_t (AXIS2_CALL *
    free) (axis2_svc_t *svc, 
            axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
    add_op) (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    struct axis2_op *op);

	struct axis2_op *(AXIS2_CALL *
    get_op_with_qname) (axis2_svc_t *svc, 
                                  axis2_env_t **env,
                                  axis2_qname_t *op_qname);

	struct axis2_op *(AXIS2_CALL *
    get_op_with_name) (axis2_svc_t *svc, 
                                axis2_env_t **env,
                                const axis2_char_t * op_name);

	axis2_hash_t *(AXIS2_CALL *
    get_ops) (axis2_svc_t *svc, 
                     axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
    set_parent) (axis2_svc_t *svc, 
                    axis2_env_t **env,
                    struct axis2_svc_grp *svc_grp);

	struct axis2_svc_grp *(AXIS2_CALL *
    get_parent) (axis2_svc_t *svc, 
                 axis2_env_t **env);
	
    axis2_status_t (AXIS2_CALL *
    set_qname) (axis2_svc_t *svc, 
                axis2_env_t **env,
                axis2_qname_t *qname);
    
	axis2_qname_t *(AXIS2_CALL *
    get_qname) (const axis2_svc_t *svc, 
               axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
    add_param) (axis2_svc_t *svc, 
                axis2_env_t **env,
                axis2_param_t *param);

	axis2_param_t *(AXIS2_CALL *
    get_param) (axis2_svc_t *svc, 
                axis2_env_t **env,
                const axis2_char_t *name);

	axis2_array_list_t *(AXIS2_CALL *
    get_params) (axis2_svc_t *svc, 
                    axis2_env_t **env);
	
	axis2_bool_t (AXIS2_CALL *
    is_param_locked) (axis2_svc_t *svc, 
                        axis2_env_t **env,
                        axis2_char_t *param_name);
                                                
    axis2_status_t (AXIS2_CALL *
    set_svc_interface) (axis2_svc_t *svc,
                            axis2_env_t **env,
                            struct axis2_wsdl_interface *svc_interface);
                                
    struct axis2_wsdl_interface * (AXIS2_CALL *
    get_svc_interface) (axis2_svc_t *svc,
                                axis2_env_t **env);                               

                                
    /**
     * To ebgage a module it is reuired to use this method
     *
     * @param moduleref
     * @throws AxisFault
     */
    axis2_status_t (AXIS2_CALL *
    engage_module) (axis2_svc_t *svc,
                    axis2_env_t **env,
                    struct axis2_module_desc * moduleref,
                    struct axis2_conf * axis2_config);
    
    /**
     * To add a opeartion to a service if a module requird to do so
     *
     * @param module
     */
    axis2_status_t (AXIS2_CALL *
    add_module_ops) (axis2_svc_t *svc,
                                axis2_env_t **env,
                                struct axis2_module_desc * module_desc,
                                struct axis2_conf * axis2_config); 
                                    
    axis2_status_t (AXIS2_CALL *
    add_to_engaged_module_list) (axis2_svc_t *svc,
                                    axis2_env_t **env,
                                    struct axis2_module_desc *module_name);  
                                        
    /**
     * Method getEngadgedModules
     *
     * @return Collection
     */
    axis2_array_list_t * (AXIS2_CALL *
    get_engaged_modules) (axis2_svc_t *svc,
                          axis2_env_t **env);
    
    /**
     * To get the WSDL opeartion element in servic einterface
     * @param opName  <code>QName</cde>
     * @return  WSDLOperation <code>WSDLOperation</code>
     */
    void * (AXIS2_CALL *
    get_wsdl_op) (axis2_svc_t *svc,
                                    axis2_env_t **env,
                                    axis2_qname_t *op_name);
    
       /**
     * Method setContextPath
     *
     * @param contextPath
     */
    axis2_status_t (AXIS2_CALL *
    set_context_path) (axis2_svc_t *svc,
                        axis2_env_t **env,
                        axis2_char_t *context_path);
    
    /**
     * Method getContextPath
     *
     * @return  context path
     */
    axis2_char_t * (AXIS2_CALL *
    get_context_path) (axis2_svc_t *svc,
                       axis2_env_t **env);
    
    /**
     * Method setStyle
     *
     * @param style
     */
    axis2_status_t (AXIS2_CALL *
    set_style) (axis2_svc_t *svc,
                axis2_env_t **env,
                axis2_char_t * style);
    
    /**
     * Method getStyle
     *
     * @return axis2_char_t *
     */
    axis2_char_t * (AXIS2_CALL *
    get_style) (axis2_svc_t *svc,
                axis2_env_t **env);
    
    /**
     * Method getInFlow
     *
     * @return struct axis2_flow *
     */
    struct axis2_flow * (AXIS2_CALL *
    get_inflow) (axis2_svc_t *svc,
                 axis2_env_t **env);
    
    /**
     * Method setInFlow
     *
     * @param inFlow
     */
    axis2_status_t (AXIS2_CALL *
    set_inflow) (axis2_svc_t *svc,
                    axis2_env_t **env,
                    struct axis2_flow *inflow);
    
    /**
     * Method getOutFlow
     *
     * @return struct axis2_flow *
     */
    struct axis2_flow * (AXIS2_CALL *
    get_outflow) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    /**
     * Method setOutFlow
     *
     * @param outFlow
     */
    axis2_status_t (AXIS2_CALL *
    set_outflow) (axis2_svc_t *svc,
                                axis2_env_t **env,
                                struct axis2_flow *outflow);
    
    /**
     * Method getFaultInFlow
     *
     * @return struct axis2_flow *
     */
    struct axis2_flow *(AXIS2_CALL *
    get_fault_inflow) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    /**
     * Method setFaultInFlow
     *
     * @param fault_flow
     */
    axis2_status_t (AXIS2_CALL *
    set_fault_inflow) (axis2_svc_t *svc,
                                axis2_env_t **env,
                                struct axis2_flow *fault_flow);
    
    struct axis2_flow * (AXIS2_CALL *
    get_fault_outflow) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_fault_outflow) (axis2_svc_t *svc,
                        axis2_env_t **env,
                        struct axis2_flow *fault_flow);
    
    /**
     * This method will return the op given particular SOAP Action.
     * This method should only be called if there is only one Endpoint is defined
     * for this Service. If more than one Endpoint exists one of them will be picked.
     * If more than one Operation is found with the given
     * SOAP Action; null will be ruturned. If no particular Operation is found with
     * the given SOAP Action; null will be returned.
     *
     * @param soapAction SOAP Action defined for the particular Operation
     * @return A struct axis2_op * if a unque Operation can be found with the given SOAP Action
     *         otherwise will return null.
     */
    struct axis2_op * (AXIS2_CALL *
    get_op_by_soap_action) (axis2_svc_t *svc,
                                    axis2_env_t **env,
                                    axis2_char_t *soap_action);
    
    /**
     * This method will return the op given the particular endpoing and the
     * particular SOAP Action. If more than one Operation is found with the given
     * SOAP Action; null will be ruturned. If no particular Operation is found with
     * the given SOAP Action; null will be returned
     *
     * @param endpoint   Particular Enpoint in which the bining is defined with the particular SOAP
     *                   Action.
     * @param soapAction SOAP Action defined for the particular Operation
     * @return A struct axis2_op * if a unque Operation can be found with the given SOAP Action
     *         otherwise will return null.
     */
    struct axis2_op * (AXIS2_CALL *
    get_op_by_soap_action_and_endpoint) (axis2_svc_t *svc,
                                    axis2_env_t **env,
                                    axis2_char_t *soap_action,
                                    axis2_qname_t * endpoint);       
    
     /**
     * To get the description about the service
     *                                                                  
     * @return axis2_char_t *
     */
    axis2_char_t * (AXIS2_CALL *
    get_axis2_svc_name) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    /**
     * Set the description about the service
     *
     * @param axissvcname
     */
    axis2_status_t (AXIS2_CALL *
    set_axis2_svc_name) (axis2_svc_t *svc,
                                axis2_env_t **env,
                                axis2_char_t *axis2_svc_name);
    
    /**
     * This method will set the current time as last update time of the service
     */
    axis2_status_t (AXIS2_CALL *
    set_last_update) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    long (AXIS2_CALL *
    get_last_update) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    axis2_char_t * (AXIS2_CALL *
    get_filename) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_filename) (axis2_svc_t *svc,
                                axis2_env_t **env,
                                axis2_char_t *filename);
    
    axis2_hash_t * (AXIS2_CALL *
    get_endpoints) (axis2_svc_t *svc,
                                axis2_env_t **env);
    
    axis2_status_t (AXIS2_CALL *
    set_endpoints) (axis2_svc_t *svc,
                    axis2_env_t **env,
                    axis2_hash_t * endpoints);
    
    axis2_status_t (AXIS2_CALL *
    set_endpoint)(axis2_svc_t *svc,
                    axis2_env_t **env,
                    struct axis2_wsdl_endpoint * endpoint);
    
    struct axis2_wsdl_endpoint * (AXIS2_CALL *
    get_endpoint) (axis2_svc_t *svc,
                    axis2_env_t **env,
                    axis2_qname_t * qname);
    
    axis2_char_t * (AXIS2_CALL *
    get_namespace) (axis2_svc_t *svc,
                  axis2_env_t **env);
    
    /**
     * To add the was action paramater into has map so that was action based dispatch can support
     */
    axis2_status_t (AXIS2_CALL *
    add_mapping) (axis2_svc_t *svc,
                axis2_env_t **env,
                axis2_char_t * mapping_key , 
                struct axis2_op * axis2_op);
 
    axis2_status_t (AXIS2_CALL *
    add_module_ref) (axis2_svc_t *svc,
                                axis2_env_t **env,
                                axis2_qname_t *moduleref);
    
    axis2_array_list_t *(AXIS2_CALL *
    get_modules) (axis2_svc_t *svc,
                            axis2_env_t **env);                
};

/** 
 * @brief Service struct
  *	Axis2 Service  
 */
struct axis2_svc
{
	axis2_svc_ops_t *ops; 
    struct axis2_param_container *param_container;
    struct axis2_flow_container *flow_container;
    struct axis2_wsdl_svc *wsdl_svc;    

};

/** 
 * Creates service struct with qname
 * @return pointer to newly created service
 */
AXIS2_DECLARE(axis2_svc_t *)
axis2_svc_create (axis2_env_t **env);

/** 
 * Creates service struct with qname
 * @param qname ualified name
 * @return pointer to newly created service
 */
AXIS2_DECLARE(axis2_svc_t *)
axis2_svc_create_with_qname (axis2_env_t **env, 
                                axis2_qname_t *qname);

/** 
 * Creates service struct with wsdl service
 * @param wsdl_svc wsdl service
 * @return pointer to newly created service
 */
axis2_svc_t * AXIS2_CALL
axis2_svc_create_with_wsdl_svc (axis2_env_t **env, 
                                struct axis2_wsdl_svc *wsdl_svc);

/**************************** Start of function macros ************************/

#define AXIS2_SVC_FREE(svc, env) ((svc)->ops->free (svc, env))

#define AXIS2_SVC_ADD_OP(svc, env, op) \
		((svc)->ops->add_op (svc, env, op))

#define AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname) \
		((svc)->ops->get_op_with_qname (svc, env, op_qname))

#define AXIS2_SVC_GET_OP_WITH_NAME(svc, env, op_name) \
		((svc)->ops->get_op_with_name (svc, env, op_name))

#define AXIS2_SVC_GET_OPS(svc, env) ((svc)->ops->get_ops (svc, env))

#define AXIS2_SVC_SET_PARENT(svc, env , svc_grp) ((svc)->ops->set_parent \
        (svc, env, svc_grp))

#define AXIS2_SVC_GET_PARENT(svc, env) ((svc)->ops->get_parent (svc, env))

#define AXIS2_SVC_SET_QNAME(svc, env, qname) ((svc)->ops->set_qname(svc , env, qname))

#define AXIS2_SVC_GET_QNAME(svc, env) ((svc)->ops->get_qname(svc , env))
		
#define AXIS2_SVC_ADD_PARAM(svc, env, param) ((svc)->ops->add_param(svc , env, param))
		
#define AXIS2_SVC_GET_PARAM(svc, env, name) ((svc)->ops->get_param(svc , env, name))
		
#define AXIS2_SVC_GET_PARAMS(svc, env) ((svc)->ops->get_params(svc , env))
		
#define AXIS2_SVC_IS_PARAM_LOCKED(svc, env, param_name) \
        ((svc)->ops->is_param_locked(svc, env, param_name))

#define AXIS2_SVC_SET_SVC_INTERFACE(svc, env, svc_interface) \
        ((svc)->ops->set_svc_interface(svc, env, svc_interface))

#define AXIS2_SVC_GET_SVC_INTERFACE(svc, env) \
        ((svc)->ops->get_svc_interface(svc, env))       

#define AXIS2_SVC_ENGAGE_MODULE(svc, env, moduleref, axis2_config) \
        ((svc)->ops->engage_module(svc, env, moduleref, axis2_config))

#define AXIS2_SVC_ADD_MODULE_OPS(svc, env, module_desc, axis2_config) \
        ((svc)->ops->add_module_ops(svc, env, module_desc, axis2_config))

#define AXIS2_SVC_ADD_TO_ENGAGED_MODULE_LIST(svc, env, module_name) \
        ((svc)->ops->add_to_engaged_module_list(svc, env, module_name))

#define AXIS2_SVC_GET_ENGAGED_MODULES(svc, env) \
        ((svc)->ops->get_engaged_modules(svc, env))

#define AXIS2_SVC_GET_WSDL_OP(svc, env, op_name) \
        ((svc)->ops->get_wsdl_op(svc, env, op_name))

#define AXIS2_SVC_SET_CONTEXT_PATH(svc, env, context_path) \
        ((svc)->ops->set_context_path(svc, env, context_path))

#define AXIS2_SVC_GET_CONTEXT_PATH(svc, env) \
        ((svc)->ops->get_context_path(svc, env))

#define AXIS2_SVC_SET_STYLE(svc, env, style) \
        ((svc)->ops->set_style(svc, env, style))

#define AXIS2_SVC_GET_STYLE(svc, env) \
        ((svc)->ops->get_style(svc, env))

#define AXIS2_SVC_GET_INFLOW(svc, env) \
        ((svc)->ops->get_inflow(svc, env))

#define AXIS2_SVC_SET_INFLOW(svc, env, inflow) \
        ((svc)->ops->set_inflow(svc, env, inflow))

#define AXIS2_SVC_GET_OUTFLOW(svc, env) \
        ((svc)->ops->get_outflow(svc, env))

#define AXIS2_SVC_SET_OUTFLOW(svc, env, outflow) \
        ((svc)->ops->set_outflow(svc, env, outflow))

#define AXIS2_SVC_GET_FAULT_INFLOW(svc, env) \
        ((svc)->ops->get_fault_inflow(svc, env))

#define AXIS2_SVC_SET_FAULT_INFLOW(svc, env, fault_inflow) \
        ((svc)->ops->set_fault_inflow(svc, env, fault_inflow))

#define AXIS2_SVC_GET_FAULT_OUTFLOW(svc, env) \
        ((svc)->ops->get_fault_outflow(svc, env))

#define AXIS2_SVC_SET_FAULT_OUTFLOW(svc, env, fault_outflow) \
        ((svc)->ops->set_fault_outflow(svc, env, fault_outflow))

#define AXIS2_SVC_GET_OP_BY_SOAP_ACTION(svc, env, soap_action) \
        ((svc)->ops->get_op_by_soap_action(svc, env, soap_action))

#define AXIS2_SVC_GET_OP_BY_SOAP_ACTION_AND_ENDPOINT(svc, env, soap_action, endpoint) \
        ((svc)->ops->get_op_by_soap_action_and_endpoint(svc, env, soap_action, endpoint))

#define AXIS2_SVC_GET_AXIS2_SVC_NAME(svc, env) \
        ((svc)->ops->get_axis2_svc_name(svc, env))

#define AXIS2_SVC_SET_AXIS2_SVC_NAME(svc, env, axis2_svc_name) \
        ((svc)->ops->set_axis2_svc_name(svc, env, axis2_svc_name))

#define AXIS2_SVC_SET_LAST_UPDATE(svc, env) \
        ((svc)->ops->set_last_update(svc, env))

#define AXIS2_SVC_GET_LAST_UPDATE(svc, env) \
        ((svc)->ops->get_last_update(svc, env))

#define AXIS2_SVC_GET_FILENAME(svc, env) \
        ((svc)->ops->get_filename(svc, env))

#define AXIS2_SVC_SET_FILENAME(svc, env, filename) \
        ((svc)->ops->set_filename(svc, env, filename))

#define AXIS2_SVC_GET_ENDPOINTS(svc, env) \
        ((svc)->ops->get_endpoints(svc, env))

#define AXIS2_SVC_SET_ENDPOINTS(svc, env, endpoints) \
        ((svc)->ops->set_endpoints(svc, env, endpoints))

#define AXIS2_SVC_SET_ENDPOINT(svc, env, endpoint) \
        ((svc)->ops->set_endpoint(svc, env, endpoint))

#define AXIS2_SVC_GET_ENDPOINT(svc, env, qname) \
        ((svc)->ops->get_endpoint(svc, env, qname))

#define AXIS2_SVC_GET_QNAMESPACE(svc, env) \
        ((svc)->ops->get_namespace(svc, env))

#define AXIS2_SVC_ADD_MAPPING(svc, env, mapping_key, axis2_opt) \
        ((svc)->ops->add_mapping(svc, env, mapping_key, axis2_opt))

#define AXIS2_SVC_ADD_MODULE_REF(svc, env, moduleref) \
        ((svc)->ops->add_module_ref(svc, env, moduleref))
        
#define AXIS2_SVC_GET_MODULES(svc, env) \
        ((svc)->ops->get_modules(svc, env))

/**************************** End of function macros **************************/

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_SVC_H */
