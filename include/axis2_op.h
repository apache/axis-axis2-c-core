/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_OP_H
#define AXIS2_OP_H

/** 
 * @defgroup axis2_op operation
 * @ingroup axis2_desc
 * operation represents the static structure of an operation in a service. 
 * In Axis2 description hierarchy, an operation lives inside the service to 
 * which it belongs. 
 * operations are configured in services.xml files located in the respective 
 * service group folders of the services folder in the repository. 
 * In services.xml file, operations are declared in association with a given 
 * service. The deployment engine would create operation instances to represent 
 * those configured operations and would associate them with the respective 
 * service in the configuration.
 * operation encapsulates data on message exchange pattern (MEP), the 
 * execution flows, engaged module information, and the message receiver 
 * associated with the operation.
 * @{
 */

/**
 * @file axis2_op.h
 */

#include <axis2_param_container.h>
#include <axis2_wsdl_op.h>
#include <axis2_svc.h>
#include <axis2_msg_recv.h>
#include <axis2_array_list.h>
#include <axis2_module_desc.h>
#include <axis2_wsdl_msg_ref.h>
#include <axis2_description.h>
#include <axis2_phase_meta.h>
#include <axis2_relates_to.h>
#include <axis2_msg_ctx.h>
#include <axis2_op_ctx.h>
#include <axis2_svc_ctx.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_op_ops */
    typedef struct axis2_op_ops axis2_op_ops_t;
    /** Type name for struct axis2_op */
    typedef struct axis2_op axis2_op_t;

    struct axis2_wsdl_op;
    struct axis2_svc;
    struct axis2_msg_recv;
    struct axis2_param_container;
    struct axis2_module_desc;
    struct axis2_op;
    struct axis2_wsdl_feature;
    struct axis2_wsdl_property;
    struct axis2_wsdl_fault_ref;
    struct axis2_relates_to;
    struct axis2_op_ctx;
    struct axis2_svc_ctx;
    struct axis2_msg_ctx;
    struct axis2_msg;
    struct axis2_conf;

/** SOAP action string constant */
#define AXIS2_SOAP_ACTION "soapAction"

    /**
     * operation ops struct.
     * Encapsulator struct for ops of axis2_op.
     */
     struct axis2_op_ops
    {
        /**
         * Frees operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Frees operation given as a void pointer.
         * @param op pointer to operation as a void pointer
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free_void_arg)(
                    void *op,
                    const axis2_env_t *env);


        /**
         * Adds a parameter to method.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param param pointer parameter to be added, operation assumes 
         * ownership of parameter
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_param)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_param_t *param);

        /**
         * Gets named parameter.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param name name of parameter to be retrieved as a string 
         * @return pointer to named parameter if exists, else NULL. Returns a
         * reference, not a cloned copy
         */
        axis2_param_t *(AXIS2_CALL *
                get_param)(
                    const axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *name);

        /**
         * Gets all parameters.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to array list containing all parameters, returns
         * a reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_params)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Checks if the named parameter is locked.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param param_name name of the parameter to be checked
         * @return AXIS2_TRUE if named parameter is locked, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                is_param_locked)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *param_name);

        /**
         * Sets parent. Parent of an operation is of type service.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param svc pointer to parent service, operation does not assume
         * ownership of service 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_svc *svc);

        /**
         * Gets parent. Parent of an operation is of type service.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to parent service, returns a reference, not a cloned 
         * copy
         */
        struct axis2_svc *(AXIS2_CALL *
                get_parent)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets operation QName.
         * @param op pointer to operation as a void pointer, as this would be 
         * assigned to get_qname() method of base WSDL operation
         * @param env pointer to environment struct
         * @param qname pointer to QName, this method creates a clone of the 
         * QName
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_qname)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);

        /**
         * Gets operation QName.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to QName, returns a reference, not a cloned copy
         */
        const axis2_qname_t *(AXIS2_CALL *
                get_qname)(
                    void *op,
                    const axis2_env_t *env);

        /**
         * Sets operation message exchange pattern (MEP).
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param pattern message exchange pattern string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_msg_exchange_pattern)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *pattern);

        /**
         * Gets operation message exchange pattern (MEP).
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return MEP string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_msg_exchange_pattern)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets message receiver. message receiver is responsible for invoking
         * the business logic associated with the operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param msg_recv pointer to message receiver, operation assumes 
         * ownership of message receiver
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_msg_recv)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_recv *msg_recv);

        /**
         * Gets message receiver. message receiver is responsible for invoking
         * the business logic associated with the operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to message receiver, returns a reference, not a 
         * cloned copy
         */
        struct axis2_msg_recv *(AXIS2_CALL *
                get_msg_recv)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Gets style of operation. Style is that mentioned in WSDL, either 
         * RPC or document literal.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return string representing style
         */
        const axis2_char_t *(AXIS2_CALL *
                get_style)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets style of operation. Style is that mentioned in WSDL, either 
         * RPC or document literal.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param style string representing style
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_style)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *style);

        /**
         * Engages given module to operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param module_desc pointer to module description, operation does not
         * assume ownership of struct
         * @param conf pointer to configuration, operation does not assume 
         * ownership of configuration
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module_desc,
                    struct axis2_conf *conf);

        /**
         * Adds module description to engaged module list.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param module_dec pointer to module description, operation does not
         * assume ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_to_engaged_module_list)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_module_desc *module_dec);

        /**
         * Gets all modules associated to operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to array list containing module descriptions
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_modules)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Gets Axis specific MEP constant. This method simply maps the string 
         * URI of the MEP to an integer. 
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return integer representing MEP
         */
        int (AXIS2_CALL *
                get_axis_specific_mep_const)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Gets fault in flow. Fault in flow is the list of phases invoked
         * when a fault happens along in path.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to array list containing phases, returns a reference,
         * not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_fault_in_flow)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);


        /**
         * Gets fault out flow. Fault out flow is the list of phases invoked
         * when a fault happens along out path.         
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to array list containing phases, returns a reference,
         * not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_fault_out_flow)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Gets out flow. Out flow is the list of phases invoked
         * along out path.         
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to array list containing phases, returns a reference,
         * not a cloned copy         
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_out_flow)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Gets in flow. In flow is the list of phases 
         * invoked along in path.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to array list containing phases, returns a reference,
         * not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_in_flow)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets fault in flow. Fault in flow is the list of phases invoked
         * when a fault happens along in path.         
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param list pointer to array list containing phases, operation takes
         * over the ownership of list         
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_in_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * Sets fault out flow. Fault out flow is the list of phases invoked
         * when a fault happens along out path.         
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param list pointer to array list containing phases, operation takes
         * over the ownership of list
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_out_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * Sets out flow. Out flow is the list of phases invoked
         * along out path.         
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param list pointer to array list containing phases, operation takes
         * over the ownership of list
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);
        /**
         * Sets in flow. In flow is the list of phases 
         * invoked along in path.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param list pointer to array list containing phases, operation takes
         * over the ownership of list
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_in_flow)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_array_list_t *list);

        /**
         * Adds given QName to module QName list.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param module_name pointer to module QName, QName would be cloned by 
         * this method 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_module_qname)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_qname_t *module_qname);

        /**
         * Gets all module QNames as a list.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to array list containing module QNames, 
         * returns a reference, not a cloned copy
         */
        axis2_array_list_t *(AXIS2_CALL *
                get_all_module_qnames)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Gets all in faults as a list.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to linked list containing in faults, 
         * returns a reference, not a cloned copy
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_all_in_faults)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets all in faults as a list.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param in_faults pointer to linked list containing in faults, 
         * operation does not assume the ownership of list
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_in_faults)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_linked_list_t *in_faults);

        /**
         * Gets input message as a WSDL message reference.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to WSDL message reference struct, returns a reference
         * not a cloned copy
         */
        struct axis2_wsdl_msg_ref *(AXIS2_CALL *
                get_input_msg)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets input message as a WSDL message reference.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param input_msg pointer to WSDL message reference, operation takes 
         * over the ownership of the struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_input_msg)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_msg_ref *input_msg);

        /**
         * Gets all out faults as a list.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to linked list containing out faults, 
         * returns a reference, not a cloned copy
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_all_out_faults)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets all out faults as a list.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param out_faults pointer to linked list containing out faults, 
         * operation does not assume the ownership of list
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_out_faults)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    axis2_linked_list_t *out_faults);

        /**
         * Gets output message as a WSDL message reference.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to WSDL message reference struct, returns a reference
         * not a cloned copy
         */
        struct axis2_wsdl_msg_ref *(AXIS2_CALL *
                get_output_msg)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Gets output message as a WSDL message reference.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param output_msg pointer to WSDL message reference struct, 
         * operation assumes ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_output_msg)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_msg_ref *output_msg);

        /**
         * Gets target namespace associated with operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return namespace URI string
         */
        const axis2_char_t *(AXIS2_CALL *
                get_target_namespace)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Adds an in fault.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param in_fault pointer to WSDL fault reference struct, operation
         * takes over the ownership of the struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_in_fault)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_fault_ref *in_fault);

        /**
         * Adds an out fault.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param out_fault pointer to WSDL fault reference struct, operation
         * assumes the ownership of the struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_out_fault)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_fault_ref *out_fault);

        /**
         * Adds a WSDL feature associated with operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param feature pointer to WSDL feature struct, operation assumes 
         * ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_feature)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_feature *feature);

        /**
         * Gets list of all WSDL features associated with operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to linked list containing all WSDL features
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_all_features)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Adds given WSDL property to operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param wsdl_property pointer to WSDL property struct, operation 
         * assumes ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_property)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_property *wsdl_property);

        /**
         * Gets all properties associated with operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to linked list containing all properties, returns 
         * a reference, not a cloned copy
         */
        axis2_linked_list_t *(AXIS2_CALL *
                get_all_properties)(
                    const axis2_op_t *op,
                    const axis2_env_t *env);

        /**
         * Sets WSDL operation that corresponds to this operation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param wsdl_op pointer to WSDL operation struct, operation assumes
         * ownership of struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_wsdl_op)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_wsdl_op *wsdl_op);

        /**
         * Finds operation context related to this operation using given message
         * context and service context. This method would create a new operation
         * context related to the operation, if one could not be found.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @param svc_ctx pointer to service context
         * @return pointer to operation context, returns
         * a reference, not a cloned copy
         */
        struct axis2_op_ctx *(AXIS2_CALL *
                find_op_ctx)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx,
                    struct axis2_svc_ctx *svc_ctx);

        /**
         * Finds operation context related to this operation using given message
         * context. This method will not create a new operation context if 
         * an associated operation context could not be found.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @return pointer to operation context if found, else NULL. Returns
         * a reference, not a cloned copy
         
         */
        struct axis2_op_ctx *(AXIS2_CALL *
                find_existing_op_ctx)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx);

        /**
         * Registers given operation context against this operation. Registration
         * happens within the given message context, as it is the message context
         * that captures the state information of a given invocation.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param msg_ctx pointer to message context
         * @param op_ctx pointer to operation context, operation does not assume
         * ownership of operation context
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                register_op_ctx)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    struct axis2_msg_ctx *msg_ctx,
                    struct axis2_op_ctx *op_ctx);

        /**
         * Gets message with given label.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @return pointer to message corresponding to given label, returns 
         * a reference, not a cloned copy
         */
        struct axis2_msg *(AXIS2_CALL *
                get_msg)(
                    const axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *label);

        /**
         * Adds given message with the given label.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * @param label label string
         * @param msg pointer to message 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_msg)(
                    axis2_op_t *op,
                    const axis2_env_t *env,
                    const axis2_char_t *label,
                    const struct axis2_msg *msg);

        /**
         * Checks if the operation is from a module.
         * @param op pointer to operation
         * @param env pointer to environment struct
         * AXIS2_TRUE if the operation is from a module, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
        is_from_module)(
            const axis2_op_t *op,
            const axis2_env_t *env);
    };

    /**
     * operation struct.
     */
     struct axis2_op
    {
        /** base WSDL operation */
        axis2_wsdl_op_t base;
        /** operations of operation struct */
        axis2_op_ops_t *ops;
        /** parameter container to hold operation related parameters  */
        struct axis2_param_container *param_container;
    };

    /**
     * Creates operation struct.
     * @param env pointer to environment struct
     * @return pointer to newly created operation
     */
    AXIS2_EXTERN axis2_op_t *AXIS2_CALL
    axis2_op_create(
        const axis2_env_t *env);

    /**
     * Creates operation struct for an operation defined in a module.
     * @param env pointer to environment struct
     * @return pointer to newly created operation
     */
    AXIS2_EXTERN axis2_op_t *AXIS2_CALL
    axis2_op_create_from_module(
        const axis2_env_t *env);

    /**
     * Creates operation struct with given QName.
     * @param env pointer to environment struct
     * @param name pointer to QName
     * @return pointer to newly created operation
     */
    AXIS2_EXTERN axis2_op_t *AXIS2_CALL
    axis2_op_create_with_qname(
        const axis2_env_t *env,
        const axis2_qname_t *name);

    /**
     * Creates operation struct with given WSDL operation as base.
     * @param env pointer to environment struct
     * @param wsdl_op pointer to WSDL operation
     * @return pointer to newly created operation
     */
    AXIS2_EXTERN axis2_op_t *AXIS2_CALL
    axis2_op_create_with_wsdl_op(
        const axis2_env_t *env,
        struct axis2_wsdl_op *wsdl_op);

    /**
     * Frees the operation given as a void pointer. This method would cast the 
     * void parameter to an operation pointer and then call free method.
     * @param pointer to operation as a void pointer
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_op_free_void_arg(
        void *op,
        const axis2_env_t *env);

/** Frees operation.
    @sa axis2_op_ops#free */
#define AXIS2_OP_FREE(op, env) \
        ((op)->ops->free (op, env))

/** Adds parameter.
    @sa axis2_op_ops#add_param */
#define AXIS2_OP_ADD_PARAM(op, env, param) \
      ((op)->ops->add_param (op, env, param))

/** Gets named parameter.
    @sa axis2_op_ops#get_param */
#define AXIS2_OP_GET_PARAM(op, env, key) \
      ((op)->ops->get_param (op, env, key))

/** Gets all the parameters as a list.
    @sa axis2_op_ops#get_all_params */
#define AXIS2_OP_GET_ALL_PARAMS(op, env) \
      ((op)->ops->get_all_params (op, env))

/** Checks if the named parameter is locked.
    @sa axis2_op_ops#is_param_locked */
#define AXIS2_OP_IS_PARAM_LOCKED(op, env, param_name) \
        ((op)->ops->is_param_locked(op, env, param_name))

/** Sets parent service.
    @sa axis2_op_ops#set_parent */
#define AXIS2_OP_SET_PARENT(op, env, service_desc) \
        ((op)->ops->set_parent (op, env, service_desc))

/** Gets parent service.
    @sa axis2_op_ops#get_parent */
#define AXIS2_OP_GET_PARENT(op, env) \
      ((op)->ops->get_parent (op, env))

/** Sets message receiver.
    @sa axis2_op_ops#set_msg_recv */
#define AXIS2_OP_SET_MSG_RECV(op, env, msg_recv) \
        ((op)->ops->set_msg_recv (op, env, msg_recv))

/** Gets message receiver.
    @sa axis2_op_ops#get_msg_recv */
#define AXIS2_OP_GET_MSG_RECV(op, env) \
        ((op)->ops->get_msg_recv (op, env))

/** Sets QName.
    @sa axis2_op_ops#set_qname */
#define AXIS2_OP_SET_QNAME(op, env, qname) \
        ((op)->ops->set_qname (op, env, qname))

/** Gets QName.
    @sa axis2_op_ops#get_qname */
#define AXIS2_OP_GET_QNAME(op, env) \
        (((axis2_op_t *) op)->ops->get_qname (op, env))

/** Sets message exchange pattern.
    @sa axis2_op_ops#set_msg_exchange_pattern */
#define AXIS2_OP_SET_MSG_EXCHANGE_PATTERN(op , env, \
        msg_exchange_pattern) \
        ((op)->ops->set_msg_exchange_pattern (op, env, \
        msg_exchange_pattern))

/** Gets message exchange pattern.
    @sa axis2_op_ops#get_msg_exchange_pattern */
#define AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env) \
        ((op)->ops->get_msg_exchange_pattern (op, env))

/** Gets style of operation.
    @sa axis2_op_ops#get_style */
#define AXIS2_OP_GET_STYLE(op, env) \
        ((op)->ops->get_style (op, env))

/** Sets style of operation.
    @sa axis2_op_ops#set_style */
#define AXIS2_OP_SET_STYLE(op, env, style) \
        ((op)->ops->set_style (op, env, style))

/** Engages the given module.
    @sa axis2_op_ops#engage_module */
#define AXIS2_OP_ENGAGE_MODULE(op, env, moduleref, conf) \
        ((op)->ops->engage_module (op, env, moduleref, conf))

/** Adds the named module to engaged module list.
    @sa axis2_op_ops#add_to_engaged_module_list */
#define AXIS2_OP_ADD_TO_ENGAGED_MODULE_LIST(op, env, module_name) \
        ((op)->ops->add_to_engaged_module_list (op, env, module_name))

/** Gets all modules.
    @sa axis2_op_ops#get_all_modules */
#define AXIS2_OP_GET_ALL_MODULES(op, env) \
        ((op)->ops->get_all_modules (op, env))

/** Gets Axis specific MEP constant.
    @sa axis2_op_ops#get_axis_specific_mep_const */
#define AXIS2_OP_GET_AXIS_SPECIFIC_MEP_CONST(op, env) \
        ((op)->ops->get_axis_specific_mep_const (op, env))

/** Gets fault in flow.
    @sa axis2_op_ops#get_fault_in_flow */
#define AXIS2_OP_GET_FAULT_IN_FLOW(op, env) \
        ((op)->ops->get_fault_in_flow (op, env))

/** Gets fault out flow.
    @sa axis2_op_ops#get_fault_out_flow */
#define AXIS2_OP_GET_FAULT_OUT_FLOW(op, env) \
        ((op)->ops->get_fault_out_flow (op, env))

/** Gets out flow.
    @sa axis2_op_ops#get_out_flow */
#define AXIS2_OP_GET_OUT_FLOW(op, env) \
        ((op)->ops->get_out_flow (op, env))

/** Gets in flow.
    @sa axis2_op_ops#get_in_flow */
#define AXIS2_OP_GET_IN_FLOW(op, env) \
        ((op)->ops->get_in_flow (op, env))

/** Sets fault in flow.
    @sa axis2_op_ops#set_fault_in_flow */
#define AXIS2_OP_SET_FAULT_IN_FLOW(op, env, list) \
        ((op)->ops->set_fault_in_flow (op, env, list))

/** Sets fault out flow.
    @sa axis2_op_ops#set_fault_out_flow */
#define AXIS2_OP_SET_FAULT_OUT_FLOW(op, env, list) \
        ((op)->ops->set_fault_out_flow (op, env, list))

/** Sets out flow.
    @sa axis2_op_ops#set_out_flow */
#define AXIS2_OP_SET_OUT_FLOW(op, env, list) \
        ((op)->ops->set_out_flow (op, env, list))

/** Sets in flow.
    @sa axis2_op_ops#set_in_flow */
#define AXIS2_OP_SET_IN_FLOW(op, env, list) \
        ((op)->ops->set_in_flow (op, env, list))

/** Adds given module QName to module qname list.
    @sa axis2_op_ops#add_module_qname */
#define AXIS2_OP_ADD_MODULE_QNAME(op, env, module_name) \
        ((op)->ops->add_module_qname (op, env, module_name))

/** Gets all module descriptions.
    @sa axis2_op_ops#get_all_module_qnames */
#define AXIS2_OP_GET_ALL_MODULE_QNAMES(op, env) \
        ((op)->ops->get_all_module_qnames (op, env))

/** Gets in faults.
    @sa axis2_op_ops#get_all_in_faults */
#define AXIS2_OP_GET_ALL_IN_FAULTS(op, env) \
        ((op)->ops->get_all_in_faults (op, env))

/** Sets in faults.
    @sa axis2_op_ops#set_in_faults */
#define AXIS2_OP_SET_IN_FAULTS(op, env) \
        ((op)->ops->set_in_faults (op, env, in_faults))

/** Gets input message.
    @sa axis2_op_ops#get_input_msg */
#define AXIS2_OP_GET_INPUT_MSG(op, env) \
        ((op)->ops->get_input_msg (op, env))

/** Sets input message.
    @sa axis2_op_ops#set_input_msg */
#define AXIS2_OP_SET_INPUT_MSG(op, env) \
        ((op)->ops->set_input_msg (op, env, input_msg))

/** Gets out faults.
    @sa axis2_op_ops#get_all_out_faults */
#define AXIS2_OP_GET_ALL_OUT_FAULTS(op, env) \
        ((op)->ops->get_all_out_faults (op, env))

/** Sets out faults.
    @sa axis2_op_ops#set_out_faults */
#define AXIS2_OP_SET_OUT_FAULTS(op, env) \
        ((op)->ops->set_out_faults (op, env, out_faults))

/** Gets output message.
    @sa axis2_op_ops#get_output_msg */
#define AXIS2_OP_GET_OUTPUT_MSG(op, env) \
        ((op)->ops->get_output_msg (op, env))

/** Sets output message.
    @sa axis2_op_ops#set_output_msg */
#define AXIS2_OP_SET_OUTPUT_MSG(op, env) \
        ((op)->ops->set_output_msg (op, env, output_msg))

/** Gets target namespace.
    @sa axis2_op_ops#get_target_namespace */
#define AXIS2_OP_GET_TARGET_NAMESPACE(op, env) \
        ((op)->ops->get_target_namespace (op, env))

/** Adds in fault.
    @sa axis2_op_ops#add_in_fault */
#define AXIS2_OP_ADD_IN_FAULT(op, env, in_fault) \
        ((op)->ops->add_in_fault (op, env, in_fault))

/** Adds out fault.
    @sa axis2_op_ops#add_out_fault */
#define AXIS2_OP_ADD_OUT_FAULT(op, env, out_fault) \
        ((op)->ops->add_out_fault (op, env, out_fault))

/** Adds feature.
    @sa axis2_op_ops#add_feature */
#define AXIS2_OP_ADD_FEATURE(op, env, feature) \
        ((op)->ops->add_feature (op, env, feature))

/** Gets all features.
    @sa axis2_op_ops#get_all_features */
#define AXIS2_OP_GET_ALL_FEATURES(op, env) \
        ((op)->ops->get_all_features(op, env))

/** Adds property.
    @sa axis2_op_ops#add_property */
#define AXIS2_OP_ADD_PROPERTY(op, env, wsdl_property) \
        ((op)->ops->add_property (op, env, wsdl_property))

/** Gets all properties.
    @sa axis2_op_ops#get_all_properties */
#define AXIS2_OP_GET_ALL_PROPERTIES(op, env) \
        ((op)->ops->get_all_properties (op, env))

/** Sets wsdl operation.
    @sa axis2_op_ops#set_wsdl_op */
#define AXIS2_OP_SET_WSDL_OP(op, env, wsdl_op) \
        ((op)->ops->set_wsdl_op (op, env, wsdl_op))


/** Finds operation context related to this operation.
    @sa axis2_op_ops#find_op_ctx */
#define AXIS2_OP_FIND_OP_CTX(op, env, msg_ctx, svc_ctx) \
        ((op)->ops->find_op_ctx (op, env, msg_ctx, svc_ctx))

/** Finds existing operation context related to this operation.
    @sa axis2_op_ops#find_existing_op_ctx */
#define AXIS2_OP_FIND_FOR_EXISTING_OP_CTX(op, env, wsdl_op) \
        ((op)->ops->find_existing_op_ctx (op, env, wsdl_op))

/** Registers given operation context against this operation.
    @sa axis2_op_ops#_register_op_ctx */
#define AXIS2_OP_REGISTER_OP_CTX(op, env, msg_ctx, wsdl_op) \
        ((op)->ops->register_op_ctx (op, env, msg_ctx, wsdl_op))

/** Gets message corresponding to given label.
    @sa axis2_op_ops#get_msg */
#define AXIS2_OP_GET_MSG(op, env, label) \
        ((op)->ops->get_msg(op, env, label))

/** Adds message with given label.
    @sa axis2_op_ops#add_msg */
#define AXIS2_OP_ADD_MSG(op, env, label, msg) \
        ((op)->ops->add_msg(op, env, label, msg))

/** Checks is the operation is from a module.
    @sa axis2_op_ops#is_from_module */
#define AXIS2_OP_IS_FROM_MODULE(op, env) \
        ((op)->ops->is_from_module(op, env))

/** @} */
#ifdef __cplusplus
}
#endif
#endif  /* AXIS2_OP_H */
