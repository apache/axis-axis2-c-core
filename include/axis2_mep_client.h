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

#ifndef AXIS2_MEP_CLIENT_H
#define AXIS2_MEP_CLIENT_H


/**
  * @file axis2_mep_client.h
  * @brief axis2 Message Context interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_error.h>
#include <axis2_op.h>
#include <axis2_om_node.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_mep_client Message Context 
 * @ingroup axis2_core_context
 * @{
 */
    
typedef struct axis2_mep_client_ops axis2_mep_client_ops_t;
typedef struct axis2_mep_client axis2_mep_client_t; 

    
/** 
 * @brief Message Context ops struct
 * Encapsulator struct for ops of axis2_mep_client
 */  
struct axis2_mep_client_ops
{
    axis2_char_t* (AXIS2_CALL *get_soap_action)(struct axis2_mep_client *mep_client, 
        axis2_env_t **env);
    /**
     * prepare the message context for invocation, here the properties kept in the
     * MEPClient copied to the axis2_msg_ctx_t *
     */
    axis2_status_t (AXIS2_CALL *prepare_invocation)(struct axis2_mep_client *mep_client, 
        axis2_env_t **env, 
        axis2_op_t *op, 
        axis2_msg_ctx_t *msg_ctx);
    /**
     * This class prepare the SOAP Envelope using the payload
     *
     * @param toSend
     * @return
     * @
     */
    axis2_msg_ctx_t* (AXIS2_CALL *prepare_soap_envelope)(struct axis2_mep_client *mep_client, 
        axis2_env_t **env, 
        axis2_om_node_t *to_send);
    /**
     * try to infer the transport looking at the URL, the URL can be http://
     * tcp:// mail:// local://. The method will look for the trnasport name as the
     * protocol part of the transport.
     *
     * @param epr
     * @return
     * @
     */
    axis2_transport_out_desc_t* (AXIS2_CALL *infer_transport)(struct axis2_mep_client *mep_client, axis2_env_t **env, 
                axis2_endpoint_ref_t *epr);
    /**
     * create write SOAPEvelope(in terms of version) based on the values set.
     *
     * @return
     * @
     */
    /* TODO axis2_soap_envelope_t* (AXIS2_CALL *create_default_soap_envelope)(struct axis2_mep_client *mep_client, 
        axis2_env_t **env);*/
    /**
     * Engage a given Module to the current invocation. But to call this method the
     * Module *MUST* be enable (picked up by the deployment and known to Axis2) else
     * Exception will be thrown. To be detected put the moduels to the AXIS2_REPOSITORY/modules directory
     *
     * @param name
     * @
     */
    axis2_status_t (AXIS2_CALL *engage_module)(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_qname_t *qname);
    /**
     * @param string
     */
    axis2_status_t (AXIS2_CALL *set_soap_version_uri)(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *soap_version_uri);
    /**
     * @param string
     */
    axis2_status_t (AXIS2_CALL *set_soap_action)(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *soap_action);
    /**
     * @param string
     */
    axis2_status_t (AXIS2_CALL *set_wsa_action)(struct axis2_mep_client *mep_client, axis2_env_t **env, axis2_char_t *wsa_action);
    axis2_svc_ctx_t* (AXIS2_CALL *get_svc_ctx)(struct axis2_mep_client *mep_client, axis2_env_t **env);
    axis2_status_t (AXIS2_CALL *free)(struct axis2_mep_client *mep_client, 
                                       axis2_env_t **env);
};

/** 
 * @brief Message Context struct
  *	Axis2 Message Context
 */
struct axis2_mep_client
{
    axis2_mep_client_ops_t *ops;    
};

AXIS2_DECLARE(axis2_mep_client_t*) axis2_mep_client_create(axis2_env_t **env, 
    axis2_svc_ctx_t *svc_ctx, 
    axis2_char_t *mep);

axis2_msg_ctx_t* AXIS2_CALL axis2_two_way_send(axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);
    
/************************** Start of function macros **************************/

#define AXIS2_MEP_CLIENT_GET_SOAP_ACTION(mep_client, env) ((mep_client)->ops->get_soap_action(mep_client, env))
#define AXIS2_MEP_CLIENT_PREPARE_INVOCATION(mep_client, env, op, msg_ctx) ((mep_client)->ops->prepare_invocation(mep_client, env, op, msg_ctx))
#define AXIS2_MEP_CLIENT_PREPARE_SOAP_ENVELOPE(mep_client, env, to_send) ((mep_client)->ops->prepare_soap_envelope(mep_client, env, to_send))
#define AXIS2_MEP_CLIENT_INFER_TRANSPORT(mep_client, env, epr) ((mep_client)->ops->infer_transport(mep_client, env, epr))
#define AXIS2_MEP_CLIENT_CREATE_DEFAULT_SOAP_ENVELOPE(mep_client, env) ((mep_client)->ops->create_default_soap_envelope(mep_client, env))
#define AXIS2_MEP_CLIENT_ENGAGE_MODULE(mep_client, env, qname) ((mep_client)->ops->engage_module(mep_client, env, qname))
#define AXIS2_MEP_CLIENT_SET_SOAP_VERSION_URI(mep_client, env, soap_version_uri) ((mep_client)->ops->set_soap_version_uri(mep_client, env, soap_version_uri))
#define AXIS2_MEP_CLIENT_SET_SOAP_ACTION(mep_client, env, soap_action) ((mep_client)->ops->set_soap_action(mep_client, env, soap_action))
#define AXIS2_MEP_CLIENT_SET_WSA_ACTION(mep_client, env, wsa_action) ((mep_client)->ops->set_wsa_action(mep_client, env, wsa_action))
#define AXIS2_MEP_CLIENT_GET_SVC_CTX(mep_client, env) ((mep_client)->ops->get_svc_ctx(mep_client, env))
#define AXIS2_MEP_CLIENT_FREE(mep_client, env) ((mep_client)->ops->free (mep_client, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_MEP_CLIENT_H */
