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

#ifndef AXIS2_MEP_CLIENT_H
#define AXIS2_MEP_CLIENT_H

/**
 * @defgroup axis2_mep_client MEP client
 * @ingroup axis2_client_api
 * MEP client handles the message exchange pattern (MEP) related information
 * when consuming services.
 */

/**
  * @file axis2_mep_client.h
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_op.h>
#include <axiom_node.h>
#include <axiom_soap_envelope.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_mep_client_ops */
    typedef struct axis2_mep_client_ops axis2_mep_client_ops_t;
    /** Type name for struct axis2_mep_client */
    typedef struct axis2_mep_client axis2_mep_client_t;


    /**
     * MEP client ops struct.
     * Encapsulator struct for ops of axis2_mep_client.
     */
    struct axis2_mep_client_ops
    {
        /**
         * Gets SOAP action.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @return a pointer to SOAP action string
         */
        axis2_string_t *(AXIS2_CALL *
                get_soap_action)(
                    const axis2_mep_client_t *mep_client,
                    const axis2_env_t *env);
        
        /**
         * Prepares the message context for invocation. Here the properties kept
         * in the mep_client are copied to the message context.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @param op pointer operation to be invoked
         * @param msg_ctx pointer to message context to be filled
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                prepare_invocation)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env,
                    axis2_op_t *op,
                    axis2_msg_ctx_t *msg_ctx);

        /**
         * Prepares the SOAP envelope using the payload. 
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @param to_send payload to be sent in AXIOM node format
         * @return a pointer to message context struct filled with the SOAP 
         * envelope to be sent
         */
        axis2_msg_ctx_t* (AXIS2_CALL *
                prepare_soap_envelope)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env,
                    axiom_node_t *to_send);

        /**
         * Tries to infer the transport looking at the URL, the URL can be http://
         * tcp:// mail:// local://. The method will look for the transport name as the
         * protocol part of the transport.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @param epr endpoint reference struct representing the endpoint URL
         * @return pointer to the transport description with inferred information
         */
        axis2_transport_out_desc_t *(AXIS2_CALL *
                infer_transport)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *epr);

        /**
         * Creates default SOAP envelope.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @return pointer to default SOAP envelope created
         */
        axiom_soap_envelope_t *(AXIS2_CALL *
                create_default_soap_envelope)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env);

        /**
         * Engage named module. The named module must have been configured in 
         * the Axis2 configuration. For a module to be detected by the 
         * deployment engine, the modules has to be placed in the 
         * AXIS2_REPOSITORY/modules directory.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @param qname QName representing the module name 
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                engage_module)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env,
                    const axis2_qname_t *qname);
        
        /**
         * Sets SOAP version URI.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @param soap_version_uri SOAP version URI
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_soap_version_uri)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env,
                    const axis2_char_t *soap_version_uri);
        
        /**
         * Sets SOAP action.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @param soap_action SOAP action
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_soap_action)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env,
                    axis2_string_t *soap_action);
        
        /**
         * Sets WSA action.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @param wsa_action Web services Addressing action
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_wsa_action)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env,
                    const axis2_char_t *wsa_action);

        /**
         * Gets service context.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct         
         * @return pointer to service context struct if set, else NULL
         */
        axis2_svc_ctx_t *(AXIS2_CALL *
                get_svc_ctx)(
                    const axis2_mep_client_t *mep_client,
                    const axis2_env_t *env);


        /**
         * Frees MEP client struct.
         * @param mep_client pointer to mep client struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_mep_client_t *mep_client,
                    const axis2_env_t *env);
    };

    /**
     * mep client struct.
     */
    struct axis2_mep_client
    {
        /** operations of mep client */
        axis2_mep_client_ops_t *ops;
    };

    /**
     * Creates a mep client struct for a specified service context.
     * @param env pointer to environment struct
     * @param svc_ctx pointer to service context. Newly created client 
     * assumes ownership of the svc_ctx
     * @param mep URI representing the message exchange pattern
     * @return a pointer to newly created mep client struct,
     *         or NULL on error with error code set in environment's error
     */
    AXIS2_EXTERN axis2_mep_client_t *AXIS2_CALL 
    axis2_mep_client_create(
        const axis2_env_t *env,
        axis2_svc_ctx_t *svc_ctx,
        const axis2_char_t *mep);

    /**
     * Sends a message represented by the given message context and captures
     * the response in return message context.
     * @param env pointer to environment struct
     * @param msg_ctx pointer to message context representing the message to 
     * be sent
     * @return message context representing the received response
     */    
    axis2_msg_ctx_t *AXIS2_CALL
    axis2_mep_client_two_way_send(
        const axis2_env_t *env,
        axis2_msg_ctx_t *msg_ctx);

    /**
     * Receives a message corresponding to a request depicted by given 
     * message context.
     * @param env pointer to environment struct
     * @param msg_ctx pointer to message context representing the response to 
     * be received
     * @return message context representing the received response
     */
    axis2_msg_ctx_t *AXIS2_CALL
    axis2_mep_client_receive(
        const axis2_env_t *env,
        axis2_msg_ctx_t *msg_ctx);

/** Gets SOAP action.
    @sa axis2_mep_client_ops#get_soap_action */
#define AXIS2_MEP_CLIENT_GET_SOAP_ACTION(mep_client, env) \
        ((mep_client)->ops->get_soap_action(mep_client, env))

/** Prepares mep client for invocation.
    @sa axis2_mep_client_ops#prepare_invocation */
#define AXIS2_MEP_CLIENT_PREPARE_INVOCATION(mep_client, env, op, msg_ctx) \
      ((mep_client)->ops->prepare_invocation(mep_client, env, op, msg_ctx))

/** Prepares SOAP envelope.
    @sa axis2_mep_client_ops#prepare_soap_envelope */
#define AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(mep_client, env, to_send) \
      ((mep_client)->ops->prepare_soap_envelope(mep_client, env, to_send))

/** Infers transport.
    @sa axis2_mep_client_ops#infer_transport */
#define AXIS2_MEP_CLIENT_INFER_TRANSPORT(mep_client, env, epr) \
      ((mep_client)->ops->infer_transport(mep_client, env, epr))

/** Creates default SOAP envelope.
    @sa axis2_mep_client_ops#create_default_soap_envelope */
#define AXIS2_MEP_CLIENT_CREATE_DEFAULT_SOAP_ENVELOPE(mep_client, env) \
      ((mep_client)->ops->create_default_soap_envelope(mep_client, env))

/** Engages named module.
    @sa axis2_mep_client_ops#engage_module */
#define AXIS2_MEP_CLIENT_ENGAGE_MODULE(mep_client, env, qname) \
      ((mep_client)->ops->engage_module(mep_client, env, qname))

/** Sets SOAP version URI.
    @sa axis2_mep_client_ops#set_soap_version_uri */
#define AXIS2_MEP_CLIENT_SET_SOAP_VERSION_URI(mep_client, env, soap_version_uri)\
      ((mep_client)->ops->set_soap_version_uri(mep_client, env, soap_version_uri))

/** Sets SOAP action.
    @sa axis2_mep_client_ops#set_soap_action */
#define AXIS2_MEP_CLIENT_SET_SOAP_ACTION(mep_client, env, soap_action) \
      ((mep_client)->ops->set_soap_action(mep_client, env, soap_action))

/** Sets WSA action.
    @sa axis2_mep_client_ops#set_wsa_action */
#define AXIS2_MEP_CLIENT_SET_WSA_ACTION(mep_client, env, wsa_action) \
      ((mep_client)->ops->set_wsa_action(mep_client, env, wsa_action))

/** Gets service context.
    @sa axis2_mep_client_ops#get_svc_ctx */
#define AXIS2_MEP_CLIENT_GET_SVC_CTX(mep_client, env) \
      ((mep_client)->ops->get_svc_ctx(mep_client, env))

/** Frees mep client.
    @sa axis2_mep_client_ops#free */
#define AXIS2_MEP_CLIENT_FREE(mep_client, env) \
      ((mep_client)->ops->free (mep_client, env))


    /** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_MEP_CLIENT_H */
