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

/** @defgroup axis2_svc service
 * @ingroup axis2_desc
 * service represents the static structure of a service in a service group. 
 * In Axis2 description hierarchy, a service lives inside the service group to 
 * which it belongs. 
 * services are configured in services.xml files located in the respective 
 * service group folders of the services folder in the repository. 
 * In services.xml file, services are declared in association with a given 
 * service group or at top level as a stand alone service. In cases where a 
 * service is configured without an associated service group, a service group 
 * with the same name as that of the service would be created by the deployment 
 * engine and the service would be associated with that newly created service 
 * group. The deployment engine would create service instances to represent 
 * those configured services in services.xml files and would associate them with
 * the respective service group in the configuration.
 * service encapsulates data on engaged module information, the XML schema 
 * defined in WSDL that is associated with the service and the operations of 
 * the service.
 * @{
 */

/**
 * @file axis2_svc.h
 */

#include <axis2_param_container.h>
#include <axis2_flow_container.h>
#include <axis2_wsdl_svc.h>
#include <axis2_op.h>
#include <axis2_svc_grp.h>
#include <axis2_qname.h>
#include <axis2_error.h>
#include <axis2_array_list.h>
#include <axis2_const.h>
#include <axis2_phase_resolver.h>
#include <axis2_module_desc.h>
#include <axis2_conf.h>
#include <axis2_wsdl_soap_op.h>
#include <axis2_string.h>
#include <axis2_wsdl_endpoint.h>
#include <xml_schema.h>
#include <xml_schema_external.h>
#include <axis2_stream.h>
#include <xml_schema_element.h>

#ifdef __cplusplus
extern "C"
{
#endif


    /** Type name for struct axis2_svc_ops */
    typedef struct axis2_svc_ops axis2_svc_ops_t;
    /** Type name for struct axis2_svc */
    typedef struct axis2_svc axis2_svc_t;

    struct axis2_wsdl_endpoint;
    struct axis2_svc_grp;
/*    struct axis2_op;*/
    struct axis2_flow_container;
    struct axis2_param_container;
    struct axis2_wsdl_svc;
    struct axis2_wsdl_interface;
    struct axis2_module_desc;
    struct axis2_conf;
    struct axis2_wsdl_soap_op;

    /**
     * service ops struct.
     * Encapsulator struct for ops of axis2_svc.
     */
    struct axis2_svc_ops
    {
        /**
         * Frees service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env);
        /**
         * Adds operation.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param op pointer to operation struct, service assumes ownership of 
         * operation
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_op)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_op *op);

        /**
         * Gets operation corresponding to the given QName.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param op_qname pointer to QName representing operation QName
         * @return pointer to operation corresponding to given QName
         */
        struct axis2_op *(AXIS2_CALL *
                get_op_with_qname)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_qname_t *op_qname);

        /**
         * Gets operation corresponding to the name.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param op_name operation name string
         * @return pointer to operation corresponding to given name
         */
        struct axis2_op *(AXIS2_CALL *
                get_op_with_name)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *op_name);

        /**
         * Gets all operations of service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to hash map containing all operations of the service
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_ops)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets parent which is of type service group.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param svc_grp pointer to parent service group
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_svc_grp *svc_grp);

        /**
         * Gets parent which is of type service group.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to parent service group
         */
        struct axis2_svc_grp *(AXIS2_CALL *
                get_parent)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets QName.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param qname pointer to QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

        /**
         * Gets QName.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to QName
         */
        const axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Adds given parameter to operation.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param param pointer to parameter, service assumes ownership of
         * parameter
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets named parameter.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param name name string
         * @return pointer to named parameter if exists, else NULL. Returns a 
         * reference, not a cloned copy         
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * Gets all parameters stored within service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to array list of parameters, returns a reference,
         * not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_params)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Checks if the named parameter is locked.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param param_name parameter name
         * @return AXIS2_TRUE if the named parameter is locked, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Sets WSDL interface for service. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param wsdl_interface pointer to wsdl interface struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_wsdl_interface)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_wsdl_interface *wsdl_interface);

        /**
         * Gets WSDL interface for service. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to wsdl interface 
         */
        struct axis2_wsdl_interface *(AXIS2_CALL *
                get_wsdl_interface)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);


        /**
         * Engages given module to service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param  module_desc pointer to module description to be engaged, 
         * service does not assume the ownership of module 
         * @param conf pointer to configuration, it is configuration that holds 
         * module information
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module_desc,
                    struct axis2_conf *conf);

        /**
         * Adds operations defined in a module to this service. It is possible 
         * to define operations that are associated to a module in a module.xml 
         * file. These operations in turn could be invoked in relation to a 
         * service. This method allows those module related operation to be 
         * added to a service. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param module_desc pointer to module description containing module
         * related operation information. service does not assume the ownership
         * of module description
         * @param conf pointer to configuration, it is configuration that stores 
         * the modules
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_module_ops)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module_desc,
                    struct axis2_conf *axis2_config);

        /**
         * Adds given module description to engaged module list.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param module_desc pointer to module description, service does not 
         * assume the ownership of module description
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_to_engaged_module_list)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module_desc);

        /**
         * Gets all engaged modules.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to array list containing all engaged modules
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_engaged_modules)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Gets the WSDL operation element in service interface.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param op_qname pointer to QName of required operation
         * @return  pointer to WSDL operation as a void pointer
         */
        void *(AXIS2_CALL *
                get_wsdl_op)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_qname_t *op_qname);

        /**
         * Sets style. Style can be either RPC or document literal.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param style style of service as defined in WSDL
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_style)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *style);

        /**
         * Gets style. Style can be either RPC or document literal.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return string representing the style of service
         */
        const axis2_char_t *(AXIS2_CALL *
                get_style)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Gets in flow. In flow is the list of phases invoked
         * along in path.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to flow representing in flow
         */
        struct axis2_flow *(AXIS2_CALL *
                get_in_flow)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets in flow. In flow is the list of phases invoked
         * along in path.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param in_flow pointer to flow representing in flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_in_flow)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_flow *in_flow);

        /**
         * Gets out flow. Out flow is the list of phases invoked
         * along out path.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to flow representing out flow
         */
        struct axis2_flow *(AXIS2_CALL *
                get_out_flow)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets out flow. Out flow is the list of phases invoked
         * along out path.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return out_flow pointer to flow representing out flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_out_flow)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_flow *out_flow);

        /**
         * Gets fault in flow. Fault in flow is the list of phases invoked
         * along in path if a fault happens.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to flow representing fault in flow
         */
        struct axis2_flow *(AXIS2_CALL *
                get_fault_in_flow)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets fault in flow. Fault in flow is the list of phases invoked
         * along in path if a fault happens.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param fault_flow pointer to flow representing fault in flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_in_flow)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_flow *fault_flow);

        /**
         * Gets fault out flow. Fault out flow is the list of phases invoked
         * along out path if a fault happens.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to flow representing fault out flow
         */
        struct axis2_flow *(AXIS2_CALL *
                get_fault_out_flow)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets fault out flow. Fault out flow is the list of phases invoked
         * along out path if a fault happens.         
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param fault_flow pointer to flow representing fault out flow
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_out_flow)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_flow *fault_flow);

        /**
         * Gets operation corresponding to given SOAP Action.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param soap_action SOAP action string
         * @return pointer to operation corresponding to given SOAP action if 
         * one exists, else NULL. Returns a reference, not a cloned copy
         */
        struct axis2_op *(AXIS2_CALL *
                get_op_by_soap_action)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *soap_action);

        /**
         * Gets operation corresponding to given SOAP Action and endpoint QName.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param soap_action SOAP action string
         * @param endpoint pointer QName representing endpoint URI
         * @return pointer operation corresponding to given SOAP Action and 
         * endpoint QName.
         */
        struct axis2_op *(AXIS2_CALL *
                get_op_by_soap_action_and_endpoint)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *soap_action,
                    const axis2_qname_t *endpoint);

        /**
         * Gets service name.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return service name string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_name)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets service name.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param svc_name service name string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_name)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *svc_name);

        /**
         * Sets current time as last update time of the service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_last_update)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Gets last update time of the service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return last updated time in seconds
         */
        long (AXIS2_CALL *
				  get_last_update)(
						const axis2_svc_t *svc,
						const axis2_env_t *env);

		  /**
			*Get the description of the services, which is in the
			*service.xml, <description> tag
			*@param svc pointer to service struct
			*@param env pointer to environment struct
			*@return services description string
			*/

		  const axis2_char_t *(AXIS2_CALL *
									  get_svc_desc) (
											const axis2_svc_t *svc,
											const axis2_env_t *env);

		  /**
			*Set the description of the service which is in service.xml
			*@param svc pointer to service struct
			*@param env pointer to environment struct
			*@return AXIS2_SUCCESS on success, else AXIS2_FAILURE */
		  axis2_status_t (AXIS2_CALL *
								set_svc_desc) (
									 axis2_svc_t *svc,
									 const axis2_env_t *env,
									 const axis2_char_t *svc_desc);

        /**
         * Gets the name of the file that holds the implementation of the 
         * service. service implementation is compiled into shared libraries 
         * and is placed in the respective sub folder in the services folder 
         * of Axis2 repository.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return file name string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_file_name)(  
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets the name of the file that holds the implementation of the 
         * service. service implementation is compiled into shared libraries 
         * and is placed in the respective sub folder in the services folder 
         * of Axis2 repository.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param file_name file name string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_file_name)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *file_name);

        /**
         * Gets all endpoints associated with the service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to hash map containing all endpoints
         */
        axis2_hash_t *(AXIS2_CALL *
                get_all_endpoints)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets the list of endpoints associated with the service. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param endpoints pointer to hash map containing all endpoints
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_all_endpoints)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    axis2_hash_t *endpoints);

        /**
         * Adds the given endpoint to the list of endpoints associated with the 
         * service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param endpoint pointer to WSDL endpoint, service assumes ownership 
         * of endpoint
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_endpoint)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    struct axis2_wsdl_endpoint *endpoint);

        /**
         * Gets endpoint corresponding to given name from list of endpoints 
         * associated with the service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to WSDL endpoint, returns a reference, not a cloned 
         * copy
         */
        struct axis2_wsdl_endpoint *(AXIS2_CALL *
                get_endpoint)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

        /**
         * Gets namespace. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return namespace URI string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_namespace)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Adds WS-Addressing mapping for a given operation. The services.xml
         * configuration file could specify a SOAP action that would map to 
         * one of the service operations. This method could be used to register 
         * that mapping against operations. WS-Addressing based dispatcher 
         * makes use of this mapping to identify the operation to be invoked,
         * given WSA action.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param wsa_action WSA action string
         * @param op pointer to operation that maps to the given WSA action
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_mapping)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *wsa_action ,
                    struct axis2_op *axis2_op);

        /**
         * Adds a module qname to list of module QNames associated with service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param module_qname pointer to QName to be added, this method clones
         * the QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_module_qname)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_qname_t *module_qname);

        /**
         * Gets all module QNames associated with the service as a list.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to array list containing QNames
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_module_qnames)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Checks if the XML schema location is adjusted.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if XML schema is adjusted, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_schema_location_adjusted)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env);


        /**
         * Sets the bool value indicating if the XML schema location is adjusted.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param adjusted AXIS2_TRUE if XML schema is adjusted, else AXIS2_FALSE
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_schema_location_adjusted)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_bool_t adjusted);

        /**
         * Gets XML schema mapping table for service. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to hash map with XML schema mappings, returns a 
         * reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_schema_mapping_table)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets XML schema mapping table for service. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param table pointer to hash map with XML schema mappings, service 
         * assumes ownership of the map
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_schema_mapping_table)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    axis2_hash_t *table);

        /**
         * Gets custom schema prefix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return custom schema prefix string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_custom_schema_prefix)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets custom schema prefix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param prefix custom schema prefix string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_custom_schema_prefix)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *prefix);

        /**
         * Gets custom schema suffix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return custom schema suffix string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_custom_schema_suffix)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets custom schema suffix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param suffix custom schema suffix string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_custom_schema_suffix)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *suffix);

        /**
         * Prints the schema to given stream.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param out_stream stream to print to
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                print_schema)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    axis2_stream_t *out_stream);

        /**
         * Gets the XML schema at the given index of XML schema array list.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param index index of the XML schema to be retrieved
         * @return pointer to XML schema, returns a reference, not a cloned copy
         */
        xml_schema_t *(AXIS2_CALL *
                get_schema)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const int index);

        /**
         * Adds all namespaces in the namespace map to the XML schema at 
         * the given index of the XML schema array list.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param index index of the XML schema to be processed
         * @return pointer to XML schema with namespaces added, 
         * returns a reference, not a cloned copy         
         */
        xml_schema_t *(AXIS2_CALL *
                add_all_namespaces)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    int index);

        /**
         * Gets the list of XML schemas associated with service. 
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to array list of XML schemas, returns a reference,
         * not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_schemas)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Adds the given XML schema to the list of XML schemas associated 
         * with the service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param schema pointer to XML schema struct, service assumes the 
         * ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_schema)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    xml_schema_t *schema);

        /**
         * Adds the list of all XML schemas to service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param schemas pointer to array list containing XML schemas
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                add_all_schemas)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    axis2_array_list_t *schemas);

        /**
         * Gets XML schema's target namespace.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return XML schema target namespace string 
         */
        const axis2_char_t *(AXIS2_CALL *
                get_schema_target_ns)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets XML schema's target namespace.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param ns namespace string 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_schema_target_ns)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *ns);

        /**
         * Gets XML schema's target namespace prefix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return XML schema target namespace prefix string 
         */
        const axis2_char_t *(AXIS2_CALL *
                get_schema_target_ns_prefix)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets XML schema's target namespace prefix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param prefix namespace prefix string 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_schema_target_ns_prefix)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *prefix);

        /**
         * Gets target namespace.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return target namespace as a string 
         */
        const axis2_char_t *(AXIS2_CALL *
                get_target_ns)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets target namespace.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param ns target namespace as a string 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_target_ns)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *ns);

        /**
         * Gets target namespace prefix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return target namespace prefix string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_target_ns_prefix)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets target namespace prefix.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param prefix target namespace prefix string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_target_ns_prefix)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_char_t *prefix);

        /**
         * Gets XML schemas element corresponding to the given QName.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param qname QName of the XML schema element to be retrieved
         * @return pointer to XML schema element, returns a reference, not a 
         * cloned copy
         */
        xml_schema_element_t *(AXIS2_CALL *
                get_schema_element)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

        /**
         * Gets the namespace map with all namespaces related to service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return pointer to hash map containing all namespaces, returns a 
         * reference, not a cloned copy
         */
        axis2_hash_t *(AXIS2_CALL *
                get_ns_map)(
                    const axis2_svc_t *svc,
                    const axis2_env_t *env);

        /**
         * Sets the namespace map with all namespaces related to service.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @param ns_map pointer to hash map containing all namespaces
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_ns_map)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env,
                    axis2_hash_t *ns_map);

        /**
         * Populates the schema mappings. This method is used in code generation
         * and WSDL generation (WSDL2C and C2WSDL). This method deals with the 
         * imported schemas that would be there in the WSDL.
         * @param svc pointer to service struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                populate_schema_mappings)(
                    axis2_svc_t *svc,
                    const axis2_env_t *env);
    };

    /**
     * service struct.
     */
    struct axis2_svc
    {
        /** Operations of service */
        axis2_svc_ops_t *ops;
        /** parameter container to hold service related parameters */
        struct axis2_param_container *param_container;
        /** flow container that encapsulates the flow related data */
        struct axis2_flow_container *flow_container;
        /** WSDL service that holds WSDL related information of the service */
        struct axis2_wsdl_svc *wsdl_svc;

    };

    /**
     * Creates a service struct instance.
     * @param env pointer to environment struct
     * @return pointer to newly created service
     */
    AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
    axis2_svc_create(
        const axis2_env_t *env);

    /**
     * Creates a service struct with given QName.
     * @param env pointer to environment struct
     * @param qname service QName
     * @return pointer to newly created service
     */
    AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
    axis2_svc_create_with_qname(
        const axis2_env_t *env,
        const axis2_qname_t *qname);

    /**
     * Creates service struct with given WSDL service.
     * @param env pointer to environment struct
     * @param wsdl_svc pointer to WSDL service struct
     * @return pointer to newly created service
     */
    AXIS2_EXTERN axis2_svc_t *AXIS2_CALL
    axis2_svc_create_with_wsdl_svc(
        const axis2_env_t *env,
        struct axis2_wsdl_svc *wsdl_svc);


/** Frees service.
    @sa axis2_svc_ops#free */
#define AXIS2_SVC_FREE(svc, env) \
        ((svc)->ops->free (svc, env))

/** Adds operation.
    @sa axis2_svc_ops#add_op */
#define AXIS2_SVC_ADD_OP(svc, env, op) \
        ((svc)->ops->add_op (svc, env, op))

/** Gets operation with given QName.
    @sa axis2_svc_ops#get_op_with_qname */
#define AXIS2_SVC_GET_OP_WITH_QNAME(svc, env, op_qname) \
        ((svc)->ops->get_op_with_qname (svc, env, op_qname))

/** Gets operation with given name string.
    @sa axis2_svc_ops#get_op_with_name */
#define AXIS2_SVC_GET_OP_WITH_NAME(svc, env, op_name) \
        ((svc)->ops->get_op_with_name (svc, env, op_name))

/** Gets all operations.
    @sa axis2_svc_ops#get_all_ops */
#define AXIS2_SVC_GET_ALL_OPS(svc, env) \
        ((svc)->ops->get_all_ops (svc, env))

/** Sets parent which is of type service group.
    @sa axis2_svc_ops#set_parent */
#define AXIS2_SVC_SET_PARENT(svc, env , svc_grp) \
        ((svc)->ops->set_parent (svc, env, svc_grp))

/** Gets parent which is of type service group.
    @sa axis2_svc_ops#get_parent */
#define AXIS2_SVC_GET_PARENT(svc, env) \
        ((svc)->ops->get_parent (svc, env))

/** Sets QName.
    @sa axis2_svc_ops#set_qname */
#define AXIS2_SVC_SET_QNAME(svc, env, qname) \
        ((svc)->ops->set_qname(svc , env, qname))

/** Gets QName.
    @sa axis2_svc_ops#get_qname */
#define AXIS2_SVC_GET_QNAME(svc, env) \
        ((svc)->ops->get_qname(svc , env))

/** Adds given parameter to parameter list.
    @sa axis2_svc_ops#add_param */
#define AXIS2_SVC_ADD_PARAM(svc, env, param) \
        ((svc)->ops->add_param(svc , env, param))

/** Gets parameter with given name.
    @sa axis2_svc_ops#get_param */
#define AXIS2_SVC_GET_PARAM(svc, env, name) \
        ((svc)->ops->get_param(svc , env, name))

/** Gets all parameters.
    @sa axis2_svc_ops#get_all_params */
#define AXIS2_SVC_GET_ALL_PARAMS(svc, env) \
        ((svc)->ops->get_all_params(svc , env))

/** Checks if the named parameter is locked.
    @sa axis2_svc_ops#is_param_locked */
#define AXIS2_SVC_IS_PARAM_LOCKED(svc, env, param_name) \
        ((svc)->ops->is_param_locked(svc, env, param_name))

/** Sets WSDL service interface.
    @sa axis2_svc_ops#set_wsdl_interface */
#define AXIS2_SVC_SET_WSDL_INTERFACE(svc, env, wsdl_interface) \
        ((svc)->ops->set_wsdl_interface(svc, env, wsdl_interface))

/** Gets WSDL service interface.
    @sa axis2_svc_ops#get_wsdl_interface */
#define AXIS2_SVC_GET_WSDL_INTERFACE(svc, env) \
        ((svc)->ops->get_wsdl_interface(svc, env))

/** Engages given module to service.
    @sa axis2_svc_ops#engage_module */
#define AXIS2_SVC_ENGAGE_MODULE(svc, env, module_desc, axis2_config) \
        ((svc)->ops->engage_module(svc, env, module_desc, axis2_config))

/** Adds operations defined in module configuration to service.
    @sa axis2_svc_ops#add_module_ops */
#define AXIS2_SVC_ADD_MODULE_OPS(svc, env, module_desc, axis2_config) \
        ((svc)->ops->add_module_ops(svc, env, module_desc, axis2_config))

/** Adds the named module to engaged module list.
    @sa axis2_svc_ops#add_to_engaged_module_list */
#define AXIS2_SVC_ADD_TO_ENGAGED_MODULE_LIST(svc, env, module_name) \
        ((svc)->ops->add_to_engaged_module_list(svc, env, module_name))

/** Gets all engaged modules.
    @sa axis2_svc_ops#get_all_engaged_modules */
#define AXIS2_SVC_GET_ALL_ENGAGED_MODULES(svc, env) \
        ((svc)->ops->get_all_engaged_modules(svc, env))

/** Gets wsdl operation related to service corresponding to given operation name.
    @sa axis2_svc_ops#get_wsdl_op */
#define AXIS2_SVC_GET_WSDL_OP(svc, env, op_name) \
        ((svc)->ops->get_wsdl_op(svc, env, op_name))

/** Sets service style.
    @sa axis2_svc_ops#set_style */
#define AXIS2_SVC_SET_STYLE(svc, env, style) \
        ((svc)->ops->set_style(svc, env, style))

/** Gets service style.
    @sa axis2_svc_ops#get_style */
#define AXIS2_SVC_GET_STYLE(svc, env) \
        ((svc)->ops->get_style(svc, env))

/** Gets in flow.
    @sa axis2_svc_ops#get_in_flow */
#define AXIS2_SVC_GET_IN_FLOW(svc, env) \
        ((svc)->ops->get_in_flow(svc, env))

/** Sets in flow.
    @sa axis2_svc_ops#set_in_flow */
#define AXIS2_SVC_SET_IN_FLOW(svc, env, in_flow) \
        ((svc)->ops->set_in_flow(svc, env, in_flow))

/** Gets out flow.
    @sa axis2_svc_ops#get_out_flow */
#define AXIS2_SVC_GET_OUT_FLOW(svc, env) \
        ((svc)->ops->get_out_flow(svc, env))

/** Sets out flow.
    @sa axis2_svc_ops#set_out_flow */
#define AXIS2_SVC_SET_OUT_FLOW(svc, env, out_flow) \
        ((svc)->ops->set_out_flow(svc, env, out_flow))

/** Gets fault in flow.
    @sa axis2_svc_ops#get_fault_in_flow */
#define AXIS2_SVC_GET_FAULT_IN_FLOW(svc, env) \
        ((svc)->ops->get_fault_in_flow(svc, env))

/** Sets fault in flow.
    @sa axis2_svc_ops#set_fault_in_flow */
#define AXIS2_SVC_SET_FAULT_IN_FLOW(svc, env, fault_in_flow) \
        ((svc)->ops->set_fault_in_flow(svc, env, fault_in_flow))

/** Gets fault out flow.
    @sa axis2_svc_ops#get_fault_out_flow */
#define AXIS2_SVC_GET_FAULT_OUT_FLOW(svc, env) \
        ((svc)->ops->get_fault_out_flow(svc, env))

/** Sets fault out flow.
    @sa axis2_svc_ops#set_fault_out_flow */
#define AXIS2_SVC_SET_FAULT_OUT_FLOW(svc, env, fault_out_flow) \
        ((svc)->ops->set_fault_out_flow(svc, env, fault_out_flow))

/** Gets operation corresponding to given soap action.
    @sa axis2_svc_ops#get_op_by_soap_action */
#define AXIS2_SVC_GET_OP_BY_SOAP_ACTION(svc, env, soap_action) \
        ((svc)->ops->get_op_by_soap_action(svc, env, soap_action))

/** Gets operation corresponding to given soap action and endpoint.
    @sa axis2_svc_ops#get_op_by_soap_action_and_endpoint */
#define AXIS2_SVC_GET_OP_BY_SOAP_ACTION_AND_ENDPOINT(svc, env, soap_action, endpoint) \
        ((svc)->ops->get_op_by_soap_action_and_endpoint(svc, env, soap_action, endpoint))

/** Gets name.
    @sa axis2_svc_ops#get_name */
#define AXIS2_SVC_GET_NAME(svc, env) \
        ((svc)->ops->get_name(svc, env))

/** Sets name.
    @sa axis2_svc_ops#set_name */
#define AXIS2_SVC_SET_NAME(svc, env, axis2_svc_name) \
        ((svc)->ops->set_name(svc, env, axis2_svc_name))

/** Sets last update time to current time.
    @sa axis2_svc_ops#set_last_update */
#define AXIS2_SVC_SET_LAST_UPDATE(svc, env) \
        ((svc)->ops->set_last_update(svc, env))

/** Gets last update time.
    @sa axis2_svc_ops#get_last_update */
#define AXIS2_SVC_GET_LAST_UPDATE(svc, env) \
        ((svc)->ops->get_last_update(svc, env))

/** Gets service description.
		  @sa axis2_svc_ops#get_svc_desc*/
#define AXIS2_SVC_GET_SVC_DESC(svc, env) \
        ((svc)->ops->get_svc_desc(svc, env))

/** Sets service description.
	 @sa axis2_svc_ops#set_svc_desc */
#define AXIS2_SVC_SET_SVC_DESC(svc, env, svc_desc) \
        ((svc)->ops->set_svc_desc(svc, env, svc_desc))

/** Gets file name.
    @sa axis2_svc_ops#get_file_name */
#define AXIS2_SVC_GET_FILE_NAME(svc, env) \
        ((svc)->ops->get_file_name(svc, env))

/** Sets file name.
    @sa axis2_svc_ops#set_file_name */
#define AXIS2_SVC_SET_FILE_NAME(svc, env, filename) \
        ((svc)->ops->set_file_name(svc, env, filename))

/** Gets all endpoints related to service.
    @sa axis2_svc_ops#get_all_endpoints */
#define AXIS2_SVC_GET_ALL_ENDPOINTS(svc, env) \
        ((svc)->ops->get_all_endpoints(svc, env))

/** Sets all endpoints related to service.
    @sa axis2_svc_ops#set_all_endpoints */
#define AXIS2_SVC_SET_ALL_ENDPOINTS(svc, env, endpoints) \
        ((svc)->ops->set_all_endpoints(svc, env, endpoints))

/** Sets endpoint.
    @sa axis2_svc_ops#set_endpoint */
#define AXIS2_SVC_SET_ENDPOINT(svc, env, endpoint) \
        ((svc)->ops->set_endpoint(svc, env, endpoint))

/** Gets endpoint.
    @sa axis2_svc_ops#get_endpoint */
#define AXIS2_SVC_GET_ENDPOINT(svc, env, qname) \
        ((svc)->ops->get_endpoint(svc, env, qname))

/** Gets namespace.
    @sa axis2_svc_ops#get_namespace */
#define AXIS2_SVC_GET_NAMESPACE(svc, env) \
        ((svc)->ops->get_namespace(svc, env))

/** Adds operation to key mapping for a given key and operation.
    @sa axis2_svc_ops#add_mapping */
#define AXIS2_SVC_ADD_MAPPING(svc, env, mapping_key, axis2_opt) \
        ((svc)->ops->add_mapping(svc, env, mapping_key, axis2_opt))

/** Adds module QName.
    @sa axis2_svc_ops#add_module_qname */
#define AXIS2_SVC_ADD_MODULE_QNAME(svc, env, module_qname) \
        ((svc)->ops->add_module_qname(svc, env, module_qname))

/** Gets all module QNames.
    @sa axis2_svc_ops#get_all_module_qnames */
#define AXIS2_SVC_GET_ALL_MODULE_QNAMES(svc, env) \
        ((svc)->ops->get_all_module_qnames(svc, env))

/** Checks if the XML schema location is adjusted.
    @sa axis2_svc_ops#is_schema_location_adjusted */
#define AXIS2_SVC_IS_SCHEMA_LOCATION_ADJUSTED(svc, env) \
        ((svc)->ops->is_schema_location_adjusted(svc, env))

/** Sets bool value indicating that XML schema location is adjusted.
    @sa axis2_svc_ops#set_schema_location_adjusted */
#define AXIS2_SVC_SET_SCHEMA_LOCATION_ADJUSTED(svc, env, adjusted) \
        ((svc)->ops->set_schema_location_adjusted(svc, env, adjusted))

/** Gets XML schema mapping table.
    @sa axis2_svc_ops#get_schema_mapping_table */
#define AXIS2_SVC_GET_SCHEMA_MAPPING_TABLE(svc, env) \
        ((svc)->ops->get_schema_mapping_table(svc, env))

/** Sets XML schema mapping table.
    @sa axis2_svc_ops#set_schema_mapping_table */
#define AXIS2_SVC_SET_SCHEMA_MAPPING_TABLE(svc, env, table) \
        ((svc)->ops->set_schema_mapping_table(svc, env, table))

/** Gets custom XML schema prefix.
    @sa axis2_svc_ops#get_custom_schema_prefix */
#define AXIS2_SVC_GET_CUSTOM_SCHEMA_PREFIX(svc, env) \
        ((svc)->ops->get_custom_schema_prefix(svc, env))

/** Sets custom XML schema prefix.
    @sa axis2_svc_ops#set_custom_schema_prefix */
#define AXIS2_SVC_SET_CUSTOM_SCHEMA_PREFIX(svc, env, prefix) \
        ((svc)->ops->set_custom_schema_prefix(svc, env, prefix))

/** Gets custom XML schema suffix.
    @sa axis2_svc_ops#get_custom_schema_suffix */
#define AXIS2_SVC_GET_CUSTOM_SCHEMA_SUFFIX(svc, env) \
        ((svc)->ops->get_custom_schema_suffix(svc, env))

/** Sets custom XML schema suffix.
    @sa axis2_svc_ops#set_custom_schema_suffix */
#define AXIS2_SVC_SET_CUSTOM_SCHEMA_SUFFIX(svc, env, suffix) \
        ((svc)->ops->set_custom_schema_suffix(svc, env, suffix))

/** Prints XML schema to given stream.
    @sa axis2_svc_ops#print_schema */
#define AXIS2_SVC_PRINT_SCHEMA(svc, env) \
        ((svc)->ops->print_schema(svc, env))

/** Gets XML schema at the given index of the XML schema list.
    @sa axis2_svc_ops#get_schema */
#define AXIS2_SVC_GET_SCHEMA(svc, env, index) \
        ((svc)->ops->get_schema(svc, env, index))

/** Adds all namespaces to the XML schema at the given index of the XML 
    schema list.
    @sa axis2_svc_ops#add_all_namespaces */
#define AXIS2_SVC_ADD_ALL_NAMESPACES(svc, env, index) \
        ((svc)->ops->add_all_namespaces(svc, env, index))

/** Gets all XML schemas.
    @sa axis2_svc_ops#get_all_schemas */
#define AXIS2_SVC_GET_ALL_SCHEMAS(svc, env) \
        ((svc)->ops->get_all_schemas(svc, env))

/** Adds given XML schema.
    @sa axis2_svc_ops#add_schema */
#define AXIS2_SVC_ADD_SCHEMA(svc, env, schema) \
        ((svc)->ops->add_schema(svc, env, schema))

/** Adds the given list as the list of XML schemas.
    @sa axis2_svc_ops#add_all_schemas */
#define AXIS2_SVC_ADD_ALL_SCHEMAS(svc, env, schemas) \
        ((svc)->ops->add_all_schemas(svc, env, schemas))

/** Gets XML schema target namespace.
    @sa axis2_svc_ops#get_schema_target_name_space */
#define AXIS2_SVC_GET_SCHEMA_TARGET_NAME_SPACE(svc, env) \
        ((svc)->ops->get_schema_target_ns(svc, env))

/** Sets XML schema target namespace.
    @sa axis2_svc_ops#set_schema_target_name_space */
#define AXIS2_SVC_SET_SCHEMA_TARGET_NAME_SPACE(svc, env, ns) \
        ((svc)->ops->set_schema_target_ns(svc, env, ns))

/** Gets XML schema target namespace prefix.
    @sa axis2_svc_ops#get_schema_target_name_spaces_prefix */
#define AXIS2_SVC_GET_SCHEMA_TARGET_NAME_SPACE_PREFIX(svc, env) \
        ((svc)->ops->get_schema_target_ns_prefix(svc, env))

/** Sets XML schema target namespace prefix.
    @sa axis2_svc_ops#set_schema_target_name_space_prefix */
#define AXIS2_SVC_SET_SCHEMA_TARGET_NAME_SPACE_PREFIX(svc, env, prefix) \
        ((svc)->ops->set_schema_target_ns_prefix(svc, env, prefix))

/** Gets target namespace.
    @sa axis2_svc_ops#get_target_ns */
#define AXIS2_SVC_GET_TARGET_NS(svc, env) \
        ((svc)->ops->get_target_ns(svc, env))

/** Sets target namespace.
    @sa axis2_svc_ops#set_target_ns */
#define AXIS2_SVC_SET_TARGET_NS(svc, env, ns) \
        ((svc)->ops->set_target_ns(svc, env, ns))

/** Gets target namespace prefix.
    @sa axis2_svc_ops#get_target_ns_prefix */
#define AXIS2_SVC_GET_TARGET_NS_PREFIX(svc, env) \
        ((svc)->ops->get_target_ns_prefix(svc, env))

/** Sets target namespace prefix.
    @sa axis2_svc_ops#set_target_ns_prefix */
#define AXIS2_SVC_SET_TARGET_NS_PREFIX(svc, env, prefix) \
        ((svc)->ops->set_target_ns_prefix(svc, env, prefix))

/** Gets XML schema element.
    @sa axis2_svc_ops#get_schema_element */
#define AXIS2_SVC_GET_SCHEMA_ELEMENT(svc, env) \
        ((svc)->ops->get_schema_element(svc, env))

/** Gets namespace map.
    @sa axis2_svc_ops#get_ns_map */
#define AXIS2_SVC_GET_NS_MAP(svc, env) \
        ((svc)->ops->get_ns_map(svc, env))

/** Sets namespace map.
    @sa axis2_svc_ops#set_ns_map */
#define AXIS2_SVC_SET_NS_MAP(svc, env, ns_map) \
        ((svc)->ops->set_ns_map(svc, env, ns_map))

/** Populates XML schema mappings.
    @sa axis2_svc_ops#populate_schema_mappings */
#define AXIS2_SVC_POPULATE_SCHEMA_MAPPINGS(svc, env) \
        ((svc)->ops->populate_schema_mappings(svc, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif                          /* AXIS2_SVC_H */
