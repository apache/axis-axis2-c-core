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

#ifndef AXIS2_SVC_CLIENT_H
#define AXIS2_SVC_CLIENT_H


/**
  * @file axis2_svc_client.h
  * @brief axis2 Service Client interface
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_svc.h>
#include <axis2_options.h>
#include <axis2_qname.h>
#include <axis2_om_element.h>
#include <axis2_callback.h>
#include <axis2_endpoint_ref.h>
#include <axis2_svc_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_op_client.h>

#define AXIS2_ANON_SERVICE 				"__ANONYMOUS_SERVICE__"
#define AXIS2_ANON_OUT_ONLY_OP 			"__OPERATION_OUT_ONLY__"
#define AXIS2_ANON_ROBUST_OUT_ONLY_OP	"__OPERATION_ROBUST_OUT_ONLY__"
#define AXIS2_ANON_OUT_IN_OP			"__OPERATION_OUT_IN__"


#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_svc_client Service Client 
 * @ingroup axis2_core_clientapi
 * @{
 */
    
typedef struct axis2_svc_client_ops axis2_svc_client_ops_t;
typedef struct axis2_svc_client axis2_svc_client_t; 

    
/** 
 * @brief Service Client ops struct
 * Encapsulator struct for ops of axis2_svc_client
 */  
struct axis2_svc_client_ops
{
	 /**
     * Return the axis2_svc_t this is a client for. This is primarily useful
     * when the AxisService is created anonymously or from WSDL as otherwise the
     * user had the AxisService to start with.
     *
     * @return the axisService
     */
	axis2_svc_t* (AXIS2_CALL *
	get_axis_service)(struct axis2_svc_client *svc_client);

	/**
	  * Set the client configuration related to this service interaction.
	  */
	axis2_status_t (AXIS2_CALL *
	set_options)(struct axis2_svc_client *svc_client,
				axis2_env_t **env,
				axis2_options_t *options);	

	/**
     * Get the client configuration from this service interaction.
     *
     * @return set of options set earlier.
     */
    axis2_options_t* (AXIS2_CALL *
	get_options)(struct axis2_svc_client *svc_client);

    /**
     * Set the client configuration related to this service interaction to
     * override any options that the underlying operation client may have.
     */
	axis2_status_t (AXIS2_CALL *
	set_override_options)(struct axis2_svc_client *svc_client,
						axis2_env_t **env,
						axis2_options_t *override_options);    

    /**
     * Get the client configuration from this service interaction which have
     * been used to overide operation client options as well.
     *
     * @return set of options set earlier.
     */
	axis2_options_t* (AXIS2_CALL *
	get_override_options)(struct axis2_svc_client *svc_client);    


	axis2_status_t (AXIS2_CALL *
	engage_module)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_qname_t *module_name);

	axis2_status_t (AXIS2_CALL *
	disengage_module)(struct axis2_svc_client *svc_client,
						axis2_env_t **env,
						axis2_qname_t *module_name);
	
	/**
     * Add an XML element as a header to be sent with interactions. This allows
     * users to go a bit beyond the dirt simple XML in/out pattern using this
     * simplified API. A header
     *
     * @param header The header to be added for interactions. Must not be null.
     */
	axis2_status_t (AXIS2_CALL *
	add_header)(struct axis2_svc_client *svc_client,
				axis2_env_t **env,
				axis2_om_node_t *header);

	/**
     * To remove all the headers in svc_client_t
     */
	axis2_status_t (AXIS2_CALL *
	remove_headers)(struct axis2_svc_client *svc_client,
					axis2_env_t **env);

	/**
     * Add a simple header consisting of some text (and a header name; duh) to
     * be sent with interactions.
     *
     * @param header_name
     * @param header_text
     */
	axis2_status_t (AXIS2_CALL *
	add_header_with_text)(struct axis2_svc_client *svc_client,
						axis2_env_t **env,
						axis2_qname_t *header_name,
						axis2_char_t *header_text); 	

    /**
     * This is a simple client API to invoke a service operation who's MEP is
     * Robust In-Only. This API can be used to simply send a bit of XML.
     * If you need more control over this interaction
     * then you need to create a client (@see create_client()) for the operation
     * and use that instead.
     *
     * @param elem The XML to send
     */	
	axis2_status_t (AXIS2_CALL *
	send_robust)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_om_node_t *payload);

	
	axis2_status_t (AXIS2_CALL *
	send_robust_with_operation)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_qname_t *operation,
					axis2_om_node_t *payload);
	
	/**
     * Send a bit of XML and forget about it. This API is used to interact with
     * a service operation who's MEP is In-Only. That is, there is no
     * opportunity to get an error from the service via this API; one may still
     * get client-side errors, such as host unknown etc.
     *
     * @param elem The XML element to send to the service
     */
	void (AXIS2_CALL *
	fire_and_forget)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_om_node_t *payload);

	
	void (AXIS2_CALL *
	fire_and_forget_with_operation)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_qname_t *operation,
					axis2_om_node_t *payload);

	axis2_om_node_t* (AXIS2_CALL *
	send_receive)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_om_node_t *payload);

	
	axis2_om_node_t* (AXIS2_CALL *
	send_receive_with_operation)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_qname_t *operation,	
					axis2_om_node_t *payload);
	

	void (AXIS2_CALL *
	send_receive_non_blocking)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_om_node_t *payload,
					axis2_callback_t *callback);


	void (AXIS2_CALL *
	send_receive_non_blocking_with_operation)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_qname_t *operation,
					axis2_om_node_t *payload,
					axis2_callback_t *callback);
	
	/**
     * Create a MEP client for a specific operation. This is the way one can
     * create a full function MEP client which can be used to exchange messages
     * for this specific operation. If you're using this then you must know what
     * you're doing and need the full capabilities of Axis2's client
     * architecture. This is meant for people with deep skin and not the light
     * user.
     *
     * @param operation The axis2_qname_t of the operation to create a client for.
     * @return a MEP client configured to talk to the given operation or null if
     *         the operation name is not found.
     */
	axis2_op_client_t* (AXIS2_CALL *
	create_client)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_qname_t *operation);

	/**
     * This will close the out put stream or , and remove entry from waiting
     * queue of the transport Listener queue
     *
     */
	axis2_status_t (AXIS2_CALL *
	finalize_invoke)(struct axis2_svc_client *svc_client,
						axis2_env_t **env);

	/**
     * To get the EPR that the service is running
     * transport : can be null , if it is null then epr will be craetd using any available
     * transports
     */
    axis2_endpoint_ref_t* (AXIS2_CALL *
	get_my_epr)(struct axis2_svc_client *svc_client,
				axis2_env_t **env,
				axis2_char_t *transport);				
		
    /**
     * To get the Targert EPR if any in service conetext
     * and reference paramters in TEPR can send back , in the same time this epr can use to manage
     * session across mutiple svc_client
     *
     * @return <code>End point Reference</code>
     */
	axis2_endpoint_ref_t* (AXIS2_CALL *
	get_target_epr)(struct axis2_svc_client *svc_client,
					axis2_env_t **env);

	axis2_status_t (AXIS2_CALL *
	set_target_epr)(struct axis2_svc_client *svc_client,
					axis2_env_t **env,
					axis2_endpoint_ref_t *target_epr);		
    /**
     * To get the service context
     *
     * @return Service Context
     */
	axis2_svc_ctx_t* (AXIS2_CALL *
	get_svc_ctx)(struct axis2_svc_client *svc_client,
	   				axis2_env_t **env);		

	axis2_status_t (AXIS2_CALL *
	free)(struct axis2_svc_client *svc_client,
							axis2_env_t **env);	
};

/** 
 * @brief Serivce Client struct
  *	Axis2 Service Client
 */
struct axis2_svc_client
{
    axis2_svc_client_ops_t *ops;    
};

AXIS2_DECLARE(axis2_svc_client_t*) axis2_svc_client_create(axis2_env_t **env, axis2_char_t *client_home);

AXIS2_DECLARE(axis2_svc_client_t*) axis2_svc_client_create_with_conf_ctx_and_svc(axis2_env_t **env,
										 axis2_conf_ctx_t *conf_ctx,
										 axis2_svc_t *svc,
										 axis2_char_t *client_home);
   
/*TODO: add wsdl base create functions */

/************************** Start of function macros **************************/

#define AXIS2_SVC_CLIENT_GET_AXIS_SERVICE(svc_client) \
		((svc_client)->ops->get_axis_service(svc_client))

#define AXIS2_SVC_CLIENT_SET_OPTIONS(svc_client, env, options) \
		((svc_client)->ops->set_options(svc_client, env, options))

#define AXIS2_SVC_CLIENT_GET_OPTIONS(svc_client) \
		((svc_client)->ops->get_options(svc_client))

#define AXIS2_SVC_CLIENT_SET_OVERRIDE_OPTIONS(svc_client, env, override_options) \
		((svc_client)->ops->set_override_options(svc_client, env, override_options))

#define AXIS2_SVC_CLIENT_GET_OVERRIDE_OPTIONS(svc_client) \
	    ((svc_client)->ops->get_override_options(svc_client))

#define AXIS2_SVC_CLIENT_ENGAGE_MODULE(svc_client, env, module_name) \
		((svc_client)->ops->engage_module(svc_client, env, module_name))

#define AXIS2_SVC_CLIENT_DISENGAGE_MODULE(svc_client, env, module_name) \
		((svc_client)->ops->disengage_module(svc_client, env, module_name))

#define AXIS2_SVC_CLIENT_ADD_HEADER(svc_client, env, header) \
		((svc_client)->ops->add_header(svc_client, env, header))

#define AXIS2_SVC_CLIENT_REMOVE_HEADERS(svc_client, env) \
		((svc_client)->ops->remove_headers(svc_client, env))

#define AXIS2_SVC_CLIENT_ADD_HEADER_WITH_TEXT(svc_client, env, header_name, header_text) \
		((svc_client)->ops->add_header_with_text(svc_client, env, header_name, header_text))

#define AXIS2_SVC_CLIENT_SEND_ROBUST(svc_client, env, payload) \
		((svc_client)->ops->send_robust(svc_client, env, payload))

#define AXIS2_SVC_CLIENT_SEND_ROBUST_WITH_OPERATION(svc_client, env, operation, payload) \
		((svc_client)->ops->send_robust_with_operation(svc_client, env, operation, payload))

#define AXIS2_SVC_CLIENT_FIRE_AND_FORGET(svc_client, env, payload) \
		((svc_client)->ops->fire_and_forget(svc_client, env, payload))

#define AXIS2_SVC_CLIENT_FIRE_AND_FORGET_WITH_OPERATION(svc_client, env, operation, payload) \
		((svc_client)->ops->fire_and_forget_with_operation(svc_client, env, operation, payload))

#define AXIS2_SVC_CLIENT_SEND_RECEIVE(svc_client, env, payload) \
		((svc_client)->ops->send_receive(svc_client, env, payload))

#define AXIS2_SVC_CLIENT_SEND_RECEIVE_WITH_OPERATION(svc_client, env, operation, payload) \
		((svc_client)->ops->send_receive_with_operation(svc_client, env, operation, payload))

#define AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING(svc_client, env, payload, callback) \
		((svc_client)->ops->send_receive_non_blocking(svc_client, env, payload, callback))

#define AXIS2_SVC_CLIENT_SEND_RECEIVE_NON_BLOCKING_WITH_OPERATION(svc_client, env, operation, payload, callback) \
		((svc_client)->ops->send_receive_non_blocking_with_operation(svc_client, env, operation, payload, callback))
	
#define AXIS2_SVC_CLIENT_CREATE_CLIENT(svc_client, env, operation) \
		((svc_client)->ops->create_client(svc_client, env, operation))

#define AXIS2_SVC_CLIENT_FINALIZE_INVOKE(svc_client, env) \
		((svc_client)->ops->finalize_invoke(svc_client, env))

#define AXIS2_SVC_CLIENT_GET_MY_EPR(svc_client, env, transport) \
		((svc_client)->ops->get_my_epr(svc_client, env, transport))

#define AXIS2_SVC_CLIENT_GET_TARGET_EPR(svc_client, env) \
		((svc_client)->ops->get_target_epr(svc_client, env))

#define AXIS2_SVC_CLIENT_SET_TARGET_EPR(svc_client, env, target_epr) \
		((svc_client)->ops->set_target_epr(svc_client, env, target_epr))

#define AXIS2_SVC_CLIENT_GET_SERVICE_CONTEXT(svc_client, env) \
		((svc_client)->ops->get_svc_ctx(svc_client, env))


#define AXIS2_SVC_CLIENT_FREE(svc_client, env) \
		((svc_client)->ops->free(svc_client, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_SVC_CLIENT_H */
