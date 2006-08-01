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

#ifndef AXIS2_DICLIENT_H
#define AXIS2_DICLIENT_H

/** @defgroup axis2_diclient dynamic invocation client
 * @ingroup axis2_client_api
 * @{
 */

/**
 * @file axis2_diclient.h
 * @brief axis2 Dynamic Invocation Client interface
 */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_error.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_diclient_ops */
    typedef struct axis2_diclient_ops axis2_diclient_ops_t;
    /** Type name for struct axis2_diclient */
    typedef struct axis2_diclient axis2_diclient_t;

    struct axiom_node;
    struct axis2_op;

    /**
     * Dynamic invocation client ops struct
     * Encapsulator struct for ops of axis2_diclient
     */
    struct axis2_diclient_ops
    {
        axis2_status_t (AXIS2_CALL *
                free)(struct axis2_diclient *diclient,
                        const axis2_env_t *env);

        /**
         * Invoke the operation by passing om node and axis2 operation as 
         * parameters
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @param node pointer to node
         * @param op pointer to axis2 operation
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                invoke) (axis2_diclient_t *diclient,
                        const axis2_env_t *env,
                        axiom_node_t *node,
                        struct axis2_op *op);

        /**
         * Do the initialization work. Here wsdl parser is created and
         * Pumping of wsdl object model from Wsdl Pull Parser into the
         * Axis2 Wsdl Object modle is initiated. Also operation map and
         * endpoint map is populated
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @param wsdl_file_name pointer to wsdl file name
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                init) (axis2_diclient_t *diclient,
                        const axis2_env_t *env,
                        axis2_char_t *wsdl_file_name);

        /**
         * By passing the operation qualified name and the endpoint qualified
         * name operations action and address is set
	 * @param diclient pointer to dicient struct
	 * @param env pointer to environment struct
	 * @param op_qname pointer to oepration qulified name
	 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t *(AXIS2_CALL *
                set_address_and_action_for_op) (axis2_diclient_t *diclient,
                        const axis2_env_t *env,
                        axis2_qname_t *op_qname);


        /**
         * Return all the endpoints associated with the wsdl
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @return table of endpoints associated with the wsdl
         */
        axis2_hash_t *(AXIS2_CALL *
                get_endpoints) (const axis2_diclient_t *diclient,
                        const axis2_env_t *env);

        /**
         * Return all the services associated with the wsdl
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @return table of services associated with the wsdl
         */
        axis2_hash_t *(AXIS2_CALL *
                get_services) (const axis2_diclient_t *diclient,
                        const axis2_env_t *env);

        /**
         * Return the all operations associated with the wsdl
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @return table of operations associated with the wsdl
         */
        axis2_hash_t *(AXIS2_CALL *
                get_operations) (const axis2_diclient_t *diclient,
                        const axis2_env_t *env);

        /**
         * Return the operation parameter's localname. This method
         * can be used in document style operations where operation has
         * only one parameter
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @param op_qname pointer to operation qualified name
         * @return parameter's localname
         */
        axis2_char_t *(AXIS2_CALL *
                get_param_localname) (const axis2_diclient_t *diclient,
                        const axis2_env_t *env,
                        axis2_qname_t *op_qname);

        /**
         * Return the operation parameter's namespace. This method
         * can be used in document style operations where operation has
         * only one parameter
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @param op_qname pointer to operation qualified name
         * @return parameter's namespace
         */
        axis2_char_t *(AXIS2_CALL *
                get_param_namespace) (const axis2_diclient_t *diclient,
                        const axis2_env_t *env,
                        axis2_qname_t *op_qname);

        /**
         * Return the set of parameters associated with the operation.
         * This can be used in rpc style operations where operation has
         * multiple parameters.
	 * @param diclient pointer to diclient struct
	 * @param env pointer to environment struct
         * @param op_qname pointer to operation qualified name
         * @return set of parameters assoicated with the operation
         */
        axis2_hash_t *(AXIS2_CALL *
                get_params) (const axis2_diclient_t *diclient,
                        const axis2_env_t *env,
                        axis2_qname_t *op_qname);
    };

    /**
     * Dynamic invocation client struct 
     */
    struct axis2_diclient
    {
	/** Operations of dynamic invocation client struct */
        axis2_diclient_ops_t *ops;
    };

    /** 
     * Creates Dynamic infocation client struct.
     * @param env pointer to environment struct
     */
    AXIS2_EXTERN axis2_diclient_t* AXIS2_CALL
    axis2_diclient_create(const axis2_env_t *env);

/************************** Start of function macros **************************/

/** Frees the diclient.
    @sa axis2_diclient_ops#free */
#define AXIS2_DICLIENT_FREE(diclient, env) \
      ((diclient)->ops->free (diclient, env))

/** Invokes the diclient.
    @sa axis2_diclient_ops#invoke */
#define AXIS2_DICLIENT_INVOKE(diclient, env, node, op) \
      ((diclient)->ops->invoke (diclient, env, node, op))

/** Initialize the dicient.
    @sa axis2_diclient_ops#init */
#define AXIS2_DICLIENT_INIT(diclient, env, wsdl_file_name) \
      ((diclient)->ops->init (diclient, env, wsdl_file_name))

/** Sets the address and action for op.
    @sa axis2_diclient_ops#set_address_and_action_for_op */
#define AXIS2_DICLIENT_SET_ADDRESS_AND_ACTION_FOR_OP(diclient, env, op_qname) \
      ((diclient)->ops->set_address_and_action_for_op (diclient, env, \
                op_qname))

/** Gets the endpoints.
    @sa axis2_diclient_ops#get_endpoints */
#define AXIS2_DICLIENT_GET_ENDPOINTS(diclient, env) \
      ((diclient)->ops->get_endpoints (diclient, env))

/** Gets the services.
    @sa axis2_diclient_ops#get_services */
#define AXIS2_DICLIENT_GET_SERVICES(diclient, env) \
      ((diclient)->ops->get_services (diclient, env))

/** Gets the operations.
    @sa axis2_diclient_ops#get_operations */
#define AXIS2_DICLIENT_GET_OPERATIONS(diclient, env) \
      ((diclient)->ops->get_operations (diclient, env))

/** Get param localname.
    @sa axis2_diclient_ops#get_param_localname */
#define AXIS2_DICLIENT_GET_PARAM_LOCALNAME(diclient, env, op_qname) \
      ((diclient)->ops->get_param_localname (diclient, env, op_qname))

/** Get param namespace.
    @sa axis2_diclient_ops#get_param_namespace */
#define AXIS2_DICLIENT_GET_PARAM_NAMESPACE(diclient, env, op_qname) \
      ((diclient)->ops->get_param_namespace (diclient, env, op_qname))

/** Get params.
    @sa axis2_diclient_ops#get_params */
#define AXIS2_DICLIENT_GET_PARAMS(diclient, env, op_qname) \
      ((diclient)->ops->get_params (diclient, env, op_qname))

/************************** End of function macros ****************************/

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_DICLIENT_H */

