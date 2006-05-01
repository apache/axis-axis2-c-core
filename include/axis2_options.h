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

#ifndef AXIS2_OPTIONS_H
#define AXIS2_OPTIONS_H


/**
  * @file axis2_options.h
  * @brief axis2 MEP client gets the parameters from this
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_transport_in_desc.h>
#include <axis2_endpoint_ref.h>
#include <axis2_hash.h>
#include <axis2_relates_to.h>
#include <axis2_transport_out_desc.h>
#include <axis2_transport_receiver.h> /*still in modules/core/transports*/
#include <axis2_om_element.h>
#include <axis2_msg_info_headers.h>

#define AXIS2_DEFAULT_TIMEOUT_MILLISECONDS 5000
#define AXIS2_COPY_PROPERTIES	"copy_properties"

#ifdef __cplusplus
extern "C"
{
#endif

/** @defgroup axis2_options Options 
 * @ingroup axis2_core_clientapi
 * @{
 */
    
typedef struct axis2_options_ops axis2_options_ops_t;
typedef struct axis2_options axis2_options_t; 

/** 
 * @brief Options ops struct
 * Encapsulator struct for ops of axis2_options
 */  
struct axis2_options_ops
{
	axis2_char_t* (AXIS2_CALL *
	get_action)(struct axis2_options *options,
				axis2_env_t **env);

	
	axis2_endpoint_ref_t* (AXIS2_CALL *
	get_fault_to)(struct axis2_options *options,
					axis2_env_t **env);

	
	axis2_endpoint_ref_t* (AXIS2_CALL *
	get_from)(struct axis2_options *options,
			axis2_env_t **env);

   	axis2_transport_receiver_t* (AXIS2_CALL *
	get_transport_receiver)(struct axis2_options *options,
							axis2_env_t **env);
	
							
	axis2_transport_in_desc_t* (AXIS2_CALL *
	get_transport_in)(struct axis2_options *options,
							axis2_env_t **env);		

	axis2_char_t* (AXIS2_CALL *
	get_transport_in_protocol)(struct axis2_options *options,
								axis2_env_t **env);

	axis2_char_t* (AXIS2_CALL *
	get_message_id)(struct axis2_options *options,
					axis2_env_t **env);

	axis2_hash_t* (AXIS2_CALL *
	get_properties)(struct axis2_options *options,
					axis2_env_t **env);	

    /**
      * @param key
      * @return the value realeted to this key. Null, if not found.
      */
	void* (AXIS2_CALL *
	get_property)(struct axis2_options *options,
					axis2_env_t **env,
					axis2_char_t *key);

	axis2_relates_to_t* (AXIS2_CALL *
	get_relates_to)(struct axis2_options *options,
					axis2_env_t **env);

	
	axis2_endpoint_ref_t* (AXIS2_CALL *
	get_reply_to)(struct axis2_options *options,
					axis2_env_t **env);

	axis2_transport_out_desc_t* (AXIS2_CALL *
	get_transport_out)(struct axis2_options *options,
						axis2_env_t **env);

	axis2_char_t* (AXIS2_CALL *
	get_sender_transport_protocol)(struct axis2_options *options,
									axis2_env_t **env);

	axis2_char_t* (AXIS2_CALL *
	get_soap_version_uri)(struct axis2_options *options,
							axis2_env_t **env);


	/**
     * Gets the wait time after which a client times out in a blocking scenario.
     * The default is AXIS2_DEFAULT_TIMEOUT_MILLISECONDS
     *
     * @return timeOutInMilliSeconds
     */
	long (AXIS2_CALL *
	get_timeout_in_milli_seconds)(struct axis2_options *options,
									axis2_env_t **env);

	axis2_endpoint_ref_t* (AXIS2_CALL *
	get_to)(struct axis2_options *options,
					axis2_env_t **env);

	/**not added - is_exception_to_be_thrown_on_soap_fault*/

	axis2_bool_t (AXIS2_CALL *
	is_use_seperate_listener)(struct axis2_options *options,
								axis2_env_t **env);

	struct axis2_options * (AXIS2_CALL *
	get_parent)(struct axis2_options *options,
				axis2_env_t **env);

	void (AXIS2_CALL *
	set_parent)(struct axis2_options *options,
					axis2_env_t **env,
					struct axis2_options *parent);

	void (AXIS2_CALL *
	set_action)(struct axis2_options *options,
				axis2_env_t **env,
				axis2_char_t *action);

	void (AXIS2_CALL *
	set_fault_to)(struct axis2_options *options,
				axis2_env_t **env,
				axis2_endpoint_ref_t *fault_to);

	void (AXIS2_CALL *
	set_from)(struct axis2_options *options,
				axis2_env_t **env,
				axis2_endpoint_ref_t *from);

	void (AXIS2_CALL *
	set_to)(struct axis2_options *options,
				axis2_env_t **env,
				axis2_endpoint_ref_t *to);

   	void (AXIS2_CALL *
	set_transport_receiver)(struct axis2_options *options,
 							axis2_env_t **env,
							axis2_transport_receiver_t *receiver);

	void (AXIS2_CALL *
	set_transport_in)(struct axis2_options *options,
						axis2_env_t **env,
						axis2_transport_in_desc_t *transport_in);

	void (AXIS2_CALL *
	set_transport_in_protocol)(struct axis2_options *options,
								axis2_env_t **env,
								axis2_char_t *transport_in_protocol);

		
	void (AXIS2_CALL *
	set_message_id)(struct axis2_options *options,
					axis2_env_t **env,
					axis2_char_t *message_id);

	
    /**
     * This will set the properties to the context. But in setting that one may
     * need to "copy" all the properties from the source properties to the
     * target properties. To enable this we introduced a property
     * AXIS2_COPY_PROPERTIES so that if set to
     * Boolean(true), this code will copy the whole thing, without just
     * referencing to the source.
     *
     * @param properties
     */
	axis2_status_t (AXIS2_CALL *
	set_properties)(struct axis2_options *options,
					axis2_env_t **env,
					axis2_hash_t *properties);

	/**
     * Properties you need to pass in to the message context must be set via
     * this. If there is a method to the set this property, within this class,
     * its encouraged to use that method, without duplicating stuff or making
     * room for bugs.
     *
     * @param property_key
     * @param property
     */		
	void (AXIS2_CALL *
	set_property)(struct axis2_options *options,
					axis2_env_t **env,
					axis2_char_t *property_key,
					void *property);		
				

	void (AXIS2_CALL *
	set_relates_to)(struct axis2_options *options,
					axis2_env_t **env,
					axis2_relates_to_t *relates_to);

	void (AXIS2_CALL *
	set_reply_to)(struct axis2_options *options,
					axis2_env_t **env,
					axis2_endpoint_ref_t *reply_to);

	void (AXIS2_CALL *
	set_transport_out)(struct axis2_options *options,
						axis2_env_t **env,
						axis2_transport_out_desc_t *transport_out);

	axis2_status_t (AXIS2_CALL *
	set_sender_transport)(struct axis2_options *options,
							axis2_env_t **env,
							axis2_char_t *sender_transport,
							axis2_conf_t *conf);
	
	void (AXIS2_CALL *	
	set_soap_version_uri)(struct axis2_options *options,
							axis2_env_t **env,
							axis2_char_t *soap_version_uri);

	void (AXIS2_CALL *
	set_timeout_in_milli_seconds)(struct axis2_options *options,
									axis2_env_t **env,
									long timeout_in_milli_seconds);
	

	axis2_status_t (AXIS2_CALL *
	set_transport_info)(struct axis2_options *options,
						axis2_env_t **env,
						axis2_char_t *sender_transport,
						axis2_char_t *receiver_transport,
						axis2_bool_t use_seperate_listener);

	void (AXIS2_CALL *
	set_use_seperate_listener)(struct axis2_options *options,
								axis2_env_t **env,
								axis2_bool_t use_seperate_listener);

	void (AXIS2_CALL *	
	add_reference_parameter)(struct axis2_options *options,
							axis2_env_t **env,
							axis2_om_node_t *reference_parameter);

	axis2_bool_t (AXIS2_CALL *	
	is_manage_session)(struct axis2_options *options,
							axis2_env_t **env);


	void (AXIS2_CALL *	
	set_manage_session)(struct axis2_options *options,
							axis2_env_t **env,
							axis2_bool_t manage_session);

	/**
	  * this method is used to set the msg info headers to msg ctx. not 
	  * intended to be used from outside
	  */
	axis2_msg_info_headers_t* (AXIS2_CALL *	
	get_msg_info_headers)(struct axis2_options *options,
							axis2_env_t **env);
	
	axis2_status_t (AXIS2_CALL *	
	free)(struct axis2_options *options,
							axis2_env_t **env);
};

/** 
 * @brief Options struct
  *	Axis2 Options
 */
struct axis2_options
{
    axis2_options_ops_t *ops;    
};

AXIS2_DECLARE(axis2_options_t*) axis2_options_create(axis2_env_t **env);

AXIS2_DECLARE(axis2_options_t*) axis2_options_create_with_parent(axis2_env_t **env,
																	axis2_options_t *parent);
    
/************************** Start of function macros **************************/

#define AXIS2_OPTIONS_GET_ACTION(options, env) \
		((options)->ops->get_action(options, env))
		 
#define AXIS2_OPTIONS_GET_FAULT_TO(options, env) \
		((options)->ops->get_fault_to(options, env))

#define AXIS2_OPTIONS_GET_FROM(options, env) \
		((options)->ops->get_from(options, env))

#define AXIS2_OPTIONS_GET_TRANSPORT_RECEIVER(options, env) \
		((options)->ops->get_transport_receiver(options, env))

#define AXIS2_OPTIONS_GET_TRANSPORT_IN(options, env) \
		((options)->ops->get_transport_in(options, env))	

#define AXIS2_OPTIONS_GET_TRANSPORT_IN_PROTOCOL(options, env) \
		((options)->ops->get_transport_in_protocol(options, env))

#define AXIS2_OPTIONS_GET_MESSAGE_ID(options, env) \
		((options)->ops->get_message_id(options, env))

#define AXIS2_OPTIONS_GET_PROPERTIES(options, env) \
		((options)->ops->get_properties(options, env))

#define AXIS2_OPTIONS_GET_PROPERTY(options, env, key) \
		((options)->ops->get_property(options, env, key))

#define AXIS2_OPTIONS_GET_RELATES_TO(options, env) \
		((options)->ops->get_relates_to(options, env))

#define AXIS2_OPTIONS_GET_REPLY_TO(options, env) \
		((options)->ops->get_reply_to(options, env))

#define AXIS2_OPTIONS_GET_TRANSPORT_OUT(options, env) \
		((options)->ops->get_transport_out(options, env))

#define AXIS2_OPTIONS_GET_SENDER_PROTOCOL(options, env) \
		((options)->ops->get_sender_transport_protocol(options, env))

#define AXIS2_OPTIONS_GET_SOAP_VERSION_URI(options, env) \
		((options)->ops->get_soap_version_uri(options, env))

#define AXIS2_OPTIONS_GET_TIMEOUT_IN_MILLI_SECONDS(options, env) \
		((options)->ops->get_timeout_in_milli_seconds(options, env))

#define AXIS2_OPTIONS_GET_TO(options, env) \
		((options)->ops->get_to(options, env))

#define AXIS2_OPTIONS_IS_SEPERATE_LISTNER(options, env) \
		((options)->ops->is_use_seperate_listener(options, env))

#define AXIS2_OPTIONS_GET_PARENT(options, env) \
		((options)->ops->get_parent(options, env))

#define AXIS2_OPTIONS_SET_PARENT(options, env, parent) \
		((options)->ops->set_parent(options, env, parent))

#define AXIS2_OPTIONS_SET_ACTION(options, env, action) \
		((options)->ops->set_action(options, env, action))

#define AXIS2_OPTIONS_SET_FAULT_TO(options, env, fault_to) \
		((options)->ops->set_fault_to(options, env, fault_to))

#define AXIS2_OPTIONS_SET_FROM(options, env, from) \
		((options)->ops->set_from(options, env, from))

#define AXIS2_OPTIONS_SET_TO(options, env, to) \
		((options)->ops->set_to(options, env, to))

#define AXIS2_OPTIONS_SET_TRANSPORT_RECEIVER(options, env, receiver) \
		((options)->ops->set_transport_receiver(options, env, receiver))

#define AXIS2_OPTIONS_SET_TRANSPORT_IN(options, env, transport_in) \
		((options)->ops->set_transport_in(options, env, transport_in))

#define AXIS2_OPTIONS_SET_TRANSPORT_IN_PROTOCOL(options, env, transport_in_protocol) \
		((options)->ops->set_transport_in_protocol(options, env, transport_in_protocol))

#define AXIS2_OPTIONS_SET_MESSAGE_ID(options, env, message_id) \
		((options)->ops->set_message_id(options, env, message_id))

#define AXIS2_OPTIONS_SET_PROPERTIES(options, env, properties) \
		((options)->ops->set_properties(options, env, properties))

#define AXIS2_OPTIONS_SET_PROPERTY(options, env, key, property) \
		((options)->ops->set_property(options, env, key, property))

#define AXIS2_OPTIONS_SET_RELATES_TO(options, env, relates_to) \
		((options)->ops->set_relates_to(options, env, relates_to))

#define AXIS2_OPTIONS_SET_REPLY_TO(options, env, reply_to) \
		((options)->ops->set_reply_to(options, env, reply_to))
		
#define AXIS2_OPTIONS_SET_TRANSPORT_OUT(options, env, transport_out) \
		((options)->ops->set_transport_out(options, env, transport_out))

#define AXIS2_OPTIONS_SET_SENDER_TRANSPORT(options, env, sender_transport, conf) \
		((options)->ops->set_sender_transport(options, env, sender_transport, conf))

#define AXIS2_OPTIONS_SET_SOAP_VERSION_URI(options, env, soap_version_uri) \
		((options)->ops->set_soap_version_uri(options, env, soap_version_uri))

#define AXIS2_OPTIONS_SET_TIMEOUT_IN_MILLI_SECONDS(options, env, timeout_in_milli_seconds) \
		((options)->ops->set_timeout_in_milli_seconds(options, env, timeout_in_milli_seconds))

#define AXIS2_OPTIONS_SET_TRANSPORT_INFO(options, env, sender_transport, receiver_transport, user_seperate_listener) \
		((options)->ops->set_tranport_info(options, env, sender_transport, receiver_transport, user_seperate_listener))

#define AXIS2_OPTIONS_SET_USE_SEPERATE_LISTENER(options, env, use_seperate_listener) \
		((options)->ops->set_use_seperate_listener(options, env, use_seperate_listener))

#define AXIS2_OPTIONS_ADD_REFERENCE_PARAMETER(options, env, reference_parameter) \
		((options)->ops->add_reference_parameter(options, env, reference_parameter))

#define AXIS2_OPTIONS_IS_MANAGE_SESSION(options, env) \
		((options)->ops->is_manage_session(options, env))

#define AXIS2_OPTIONS_SET_MANAGE_SESSION(options, env, manage_session) \
		((options)->ops->set_manage_session(options, env, manage_session))

#define AXIS2_OPTIONS_GET_MSG_INFO_HEADERS(options, env) \
		((options)->ops->get_msg_info_headers(options, env))

#define AXIS2_OPTIONS_FREE(options, env) \
		((options)->ops->free(options, env))

/************************** End of function macros ****************************/    

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OPTIONS_H */
