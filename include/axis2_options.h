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

#ifndef AXIS2_OPTIONS_H
#define AXIS2_OPTIONS_H

/** @defgroup axis2_options options
  * @ingroup axis2_client_api
  * The options struct holds user options to be used by client when invocation 
  * services. In addition to the end point reference information, options 
  * struct also hold addressing, transport and timeout related information. 
  * User specific properties could also set on top of options.
  * @{
  */

/**
  * @file axis2_options.h
  */

#include <axis2_defines.h>
#include <axis2_env.h>
#include <axis2_transport_in_desc.h>
#include <axis2_endpoint_ref.h>
#include <axis2_hash.h>
#include <axis2_relates_to.h>
#include <axis2_transport_out_desc.h>
#include <axis2_transport_receiver.h> 
#include <axiom_element.h>
#include <axis2_msg_info_headers.h>

/** Default timeout */
#define AXIS2_DEFAULT_TIMEOUT_MILLISECONDS 30000
/** Copy properties */
#define AXIS2_COPY_PROPERTIES   "copy_properties"

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_options_ops */
    typedef struct axis2_options_ops axis2_options_ops_t;
    /** Type name for struct axis2_options */
    typedef struct axis2_options axis2_options_t;

    /**
     * options ops struct.
     * Encapsulator struct for ops of axis2_options.
     */
    struct axis2_options_ops
    {
        /**
         * Gets Web Services Addressing (WSA) action.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return WSA action string if set, else NULL
         */
        const axis2_char_t* (AXIS2_CALL *
                get_action)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets WSA fault to address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to endpoint reference struct representing fault to 
         * address if set, else NULL
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *
                get_fault_to)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets WSA from address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to endpoint reference struct representing from 
         * address if set, else NULL
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *
                get_from)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets transport receiver.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to transport receiver struct if set, else NULL
         */
        axis2_transport_receiver_t* (AXIS2_CALL *
                get_transport_receiver)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);


        /**
         * Gets transport in.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to transport in struct if set, else NULL
         */
        axis2_transport_in_desc_t* (AXIS2_CALL *
                get_transport_in)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets transport in protocol.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to transport in protocol string if set, else NULL
         */
        AXIS2_TRANSPORT_ENUMS (AXIS2_CALL *
                get_transport_in_protocol)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets message ID.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to message ID string if set, else NULL
         */
        const axis2_char_t* (AXIS2_CALL *
                get_message_id)(
                    const axis2_options_t *options_t,
                    const axis2_env_t *env);

        /**
         * Gets the properties hash map.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to properties hash map if set, else NULL
         */
        axis2_hash_t* (AXIS2_CALL *
                get_properties)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets a property corresponding to the given key.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param key key of the property to be returned
         * @return value corresponding to the given key
         */
        void* (AXIS2_CALL *
                get_property)(
                    const axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_char_t *key);

        /**
         * Gets relates to information.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to relates to struct if set, else NULL
         */
        axis2_relates_to_t* (AXIS2_CALL *
                get_relates_to)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets WSA reply to address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to endpoint reference struct representing reply to
         * address if set, else NULL
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *
                get_reply_to)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets transport out.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to transport out struct if set, else NULL
         */
        axis2_transport_out_desc_t* (AXIS2_CALL *
                get_transport_out)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets transport out protocol.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to transport out protocol string if set, else NULL
         */
        AXIS2_TRANSPORT_ENUMS (AXIS2_CALL *
                get_sender_transport_protocol)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets SOAP version URI.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return string representing SOAP version URI
         */
        const axis2_char_t* (AXIS2_CALL *
                get_soap_version_uri)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);


        /**
         * Gets the wait time after which a client times out in a blocking scenario.
         * The default is AXIS2_DEFAULT_TIMEOUT_MILLISECONDS.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return timeout in milliseconds
         */
        long (AXIS2_CALL *
                get_timeout_in_milli_seconds)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets WSA to address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to endpoint reference struct representing to
         * address if set, else NULL
         */
        axis2_endpoint_ref_t* (AXIS2_CALL *
                get_to)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets use separate listener status.
         * @param options pointer to options struct 
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if using separate listener, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                get_use_separate_listener)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets the parent options.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to the parent options struct if set, else NULL
         */
        axis2_options_t *(AXIS2_CALL *
                get_parent)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);
        /**
         * Sets the parent options.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param parent pointer to parent options struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_parent)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_options_t *parent);
        /**
         * Sets WSA action
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param action pointer to action string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_action)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_char_t *action);

        /**
         * Sets fault to address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param fault_to pointer to endpoint reference struct representing
         * fault to address. options takes over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_fault_to)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *fault_to);

        /**
         * Sets from address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param from pointer to endpoint reference struct representing
         * from to address. options takes over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_from)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *from);

        /**
         * sets from address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param to pointer to endpoint reference struct representing
         * to address. Options takes over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_to)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *to);
        /**
         * Sets transport receiver.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param receiver pointer to transport receiver struct. options takes 
         * over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_transport_receiver)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_transport_receiver_t *receiver);

        /**
         * Sets transport in description.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param transport_in pointer to transport_in struct. options takes 
         * over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_transport_in)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_transport_in_desc_t *transport_in);

        /**
         * Sets transport in protocol.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param in_protocol pointer to in_protocol struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_transport_in_protocol)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const AXIS2_TRANSPORT_ENUMS transport_in_protocol);

        /**
         * Sets message ID.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param message_id pointer to message_id struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_message_id)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_char_t *message_id);


        /**
         * Sets the properties hash map. 
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param properties pointer to properties hash map. options takes 
         * over the ownership of the hash struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_properties)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_hash_t *properties);

        /**
         * Sets a property with the given key value.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param property_key property key string
         * @param property pointer to property to be set
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_property)(  
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_char_t *property_key,
                    const void *property);

        /**
         * Sets relates to.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param relates_to pointer to relates_to struct. options takes 
         * over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_relates_to)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_relates_to_t *relates_to);

        /**
         * Sets reply to address.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param reply_to pointer to endpoint reference struct representing 
         * reply to address. options takes over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_reply_to)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_endpoint_ref_t *reply_to);

        /**
         * Sets the transport out description.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param transport_out pointer to transport out description struct. 
         * options takes over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE 
         */
        axis2_status_t (AXIS2_CALL *
                set_transport_out)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_transport_out_desc_t *transport_out);

        /**
         * Sets the sender transport.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param sender_transport name of the sender transport to be set
         * @param conf pointer to conf struct, it is from the conf that the 
         * transport is picked with the given name
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_sender_transport)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const AXIS2_TRANSPORT_ENUMS sender_transport,
                    axis2_conf_t *conf);

        /**
         * Sets the SOAP version URI.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param soap_version_uri URI of the SOAP version to be set, can be 
         * either AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI or 
         * AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_soap_version_uri)(  
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_char_t *soap_version_uri);

        /**
         * Sets timeout in Milli seconds.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param timeout_in_milli_seconds timeout in milli seconds
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_timeout_in_milli_seconds)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const long timeout_in_milli_seconds);

        /**
         * Sets transport information. Transport information includes the name 
         * of the sender transport, name of the receiver transport and if a 
         * separate listener to be used to receive response. 
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param sender_transport name of sender transport to be used
         * @param receiver_transport name of receiver transport to be used
         * @param use_separate_listener bool value indicating whether to use
         * a separate listener or not.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_transport_info)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const AXIS2_TRANSPORT_ENUMS sender_transport,
                    const AXIS2_TRANSPORT_ENUMS receiver_transport,
                    const axis2_bool_t use_separate_listener);

        /**
         * Sets the bool value indicating whether to use a separate listener or not.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param use_separate_listener bool value indicating whether to use
         * a separate listener or not.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_use_separate_listener)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_bool_t use_separate_listener);

        /**
         * Adds a WSA reference parameter.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param reference_parameter pointer to reference parameter in the form 
         * of an AXIOM tree. options takes over the ownership of the struct.
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                add_reference_parameter)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axiom_node_t *reference_parameter);

        /**
         * Gets manage session bool value.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if session is managed, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                get_manage_session)(    
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Sets manage session bool value.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param manage_session manage session bool value
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_manage_session)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_bool_t manage_session);

        /**
         * Sets WSA message information headers.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param pointer to message information headers struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_msg_info_headers) (
                    const axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_msg_info_headers_t *msg_info_headers);
        /**
         * Gets WSA message information headers.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return pointer to message information headers struct if set, 
         * else NULL
         */
        axis2_msg_info_headers_t* (AXIS2_CALL *
                get_msg_info_headers)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /** 
         * Gets SOAP version.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return AXIOM_SOAP_11 if SOAP version 1.1 is in use, else AXIOM_SOAP_12
         */
        int (AXIS2_CALL *
                get_soap_version)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Sets SOAP version.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param soap_version soap version, either AXIOM_SOAP_11 or AXIOM_SOAP_12
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_soap_version)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const int soap_version);

        /**
         * Enable/disable MTOM handling.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param enable_mtom AXIS2_TRUE if MTOM is to be enabled, AXIS2_FALSE
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_enable_mtom)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    axis2_bool_t enable_mtom);

        /**
         * Gets Enable/disable MTOM status.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return AXIS2_TRUE if MTOM enabled, else AXIS2_FALSE
         */
        axis2_bool_t (AXIS2_CALL *
                get_enable_mtom)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Gets SOAP action.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return SOAP Action string if set, else NULL
         */
        const axis2_char_t* (AXIS2_CALL *
                get_soap_action)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Sets SOAP action
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param action pointer to SOAP action string
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                set_soap_action)(
                    axis2_options_t *options,
                    const axis2_env_t *env,
                    const axis2_char_t *soap_action);

		/**
		 * Sets xml parser reset
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @param reset flag is a boolean value
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
		axis2_status_t (AXIS2_CALL *
						set_xml_parser_reset)(
							axis2_options_t *options,
							const axis2_env_t *env,
							const axis2_bool_t paser_reset_flag);

        /**
         * Gets xml parser reset value,
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return xml parser reset boolean value
         */
        axis2_bool_t (AXIS2_CALL *
                get_xml_parser_reset)(
                    const axis2_options_t *options,
                    const axis2_env_t *env);

        /**
         * Frees options struct.
         * @param options pointer to options struct
         * @param env pointer to environment struct
         * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
         */
        axis2_status_t (AXIS2_CALL *
                free)(
                    axis2_options_t *options,
                    const axis2_env_t *env);
    };

    /**
     * options struct.
     */
    struct axis2_options
    {
        /** operations of options struct */
        axis2_options_ops_t *ops;
    };

    /**
     * Creates the options struct.
     * @param env pointer to environment struct
     * @return a pointer to newly created options struct, or NULL on error 
     * with error code set in environment's error.
     */
    AXIS2_EXTERN axis2_options_t* AXIS2_CALL 
    axis2_options_create(
        const axis2_env_t *env);
    
    /**
     * Creates the options struct with given parent.
     * @param env pointer to environment struct
     * @param parent pointer to parent struct
     * @return a pointer to newly created options struct. Newly created options 
     * assumes ownership of the parent 
     * or NULL on error with error code set in environment's error.
     */
    AXIS2_EXTERN axis2_options_t* AXIS2_CALL 
    axis2_options_create_with_parent(
        const axis2_env_t *env,
        axis2_options_t *parent);

/** Gets WSA action.
    @sa axis2_options_ops#get_action */
#define AXIS2_OPTIONS_GET_ACTION(options, env) \
      ((options)->ops->get_action(options, env))

/** Get WSA fault to address.
    @sa axis2_options_ops#get_fault_to */
#define AXIS2_OPTIONS_GET_FAULT_TO(options, env) \
      ((options)->ops->get_fault_to(options, env))

/** Gets WSA from address.
    @sa axis2_options_ops#get_from */
#define AXIS2_OPTIONS_GET_FROM(options, env) \
      ((options)->ops->get_from(options, env))

/** Gets the transport receiver.
    @sa axis2_options_ops#get_transport_receiver */
#define AXIS2_OPTIONS_GET_TRANSPORT_RECEIVER(options, env) \
      ((options)->ops->get_transport_receiver(options, env))

/** Gets transport in.
    @sa axis2_options_ops#get_transport_in */
#define AXIS2_OPTIONS_GET_TRANSPORT_IN(options, env) \
      ((options)->ops->get_transport_in(options, env))

/** Gets the transport in protocol.
    @sa axis2_options_ops#get_transport_in_protocol */
#define AXIS2_OPTIONS_GET_TRANSPORT_IN_PROTOCOL(options, env) \
      ((options)->ops->get_transport_in_protocol(options, env))

/** Gets the message id.
    @sa axis2_options_ops#get_message_id */
#define AXIS2_OPTIONS_GET_MESSAGE_ID(options, env) \
      ((options)->ops->get_message_id(options, env))

/** Gets the properties.
    @sa axis2_options_ops#get_properties */
#define AXIS2_OPTIONS_GET_PROPERTIES(options, env) \
      ((options)->ops->get_properties(options, env))

/** Gets a property corresponding to given key.
    @sa axis2_options_ops#get_property */
#define AXIS2_OPTIONS_GET_PROPERTY(options, env, key) \
      ((options)->ops->get_property(options, env, key))

/** Get WSA relates to information.
    @sa axis2_options_ops#get_relates_to */
#define AXIS2_OPTIONS_GET_RELATES_TO(options, env) \
      ((options)->ops->get_relates_to(options, env))

/** Get WSA reply to address.
    @sa axis2_options_ops#get_reply_to */
#define AXIS2_OPTIONS_GET_REPLY_TO(options, env) \
      ((options)->ops->get_reply_to(options, env))

/** Gets the transport out.
    @sa axis2_options_ops#get_transport_out */
#define AXIS2_OPTIONS_GET_TRANSPORT_OUT(options, env) \
      ((options)->ops->get_transport_out(options, env))

/** Gets the sender protocol.
    @sa axis2_options_ops#get_sender_protocol */
#define AXIS2_OPTIONS_GET_SENDER_PROTOCOL(options, env) \
      ((options)->ops->get_sender_transport_protocol(options, env))

/** Gets the soap version uri.
    @sa axis2_options_ops#get_soap_version_uri */
#define AXIS2_OPTIONS_GET_SOAP_VERSION_URI(options, env) \
      ((options)->ops->get_soap_version_uri(options, env))

/** Gets the timeout in milli seconds.
    @sa axis2_options_ops#get_timeout_in_milli_seconds */
#define AXIS2_OPTIONS_GET_TIMEOUT_IN_MILLI_SECONDS(options, env) \
      ((options)->ops->get_timeout_in_milli_seconds(options, env))

/** Get WSA to address.
    @sa axis2_options_ops#get_to */
#define AXIS2_OPTIONS_GET_TO(options, env) \
      ((options)->ops->get_to(options, env))

/** Get xml parser reset value
    @sa axis2_options_opt#get_xml_parser_reset */
#define AXIS2_OPTIONS_GET_XML_PARSER_RESET(options, env) \
      ((options)->ops->get_xml_parser_reset(options, env))

/** Whether to use a separate listener.
    @sa axis2_options_ops#get_use_separate_listener */
#define AXIS2_OPTIONS_GET_USE_SEPERATE_LISTENER(options, env) \
      ((options)->ops->get_use_separate_listener(options, env))

/** Gets the parent.
    @sa axis2_options_ops#get_parent */
#define AXIS2_OPTIONS_GET_PARENT(options, env) \
      ((options)->ops->get_parent(options, env))

/** Sets the parent.
    @sa axis2_options_ops#set_parent */
#define AXIS2_OPTIONS_SET_PARENT(options, env, parent) \
      ((options)->ops->set_parent(options, env, parent))

/** Sets the WSA action.
    @sa axis2_options_ops#set_action */
#define AXIS2_OPTIONS_SET_ACTION(options, env, action) \
      ((options)->ops->set_action(options, env, action))

/** Set WSA fault to address.
    @sa axis2_options_ops#set_fault_to */
#define AXIS2_OPTIONS_SET_FAULT_TO(options, env, fault_to) \
      ((options)->ops->set_fault_to(options, env, fault_to))

/** Set WSA from address.
    @sa axis2_options_ops#set_from */
#define AXIS2_OPTIONS_SET_FROM(options, env, from) \
      ((options)->ops->set_from(options, env, from))

/** Set WSA to address.
    @sa axis2_options_ops#set_to */
#define AXIS2_OPTIONS_SET_TO(options, env, to) \
      ((options)->ops->set_to(options, env, to))

/** Sets the transport receiver.
    @sa axis2_options_ops#set_transport_receiver */
#define AXIS2_OPTIONS_SET_TRANSPORT_RECEIVER(options, env, receiver) \
      ((options)->ops->set_transport_receiver(options, env, receiver))

/** Set transport in.
    @sa axis2_options_ops#set_transport_in */
#define AXIS2_OPTIONS_SET_TRANSPORT_IN(options, env, transport_in) \
      ((options)->ops->set_transport_in(options, env, transport_in))

/** Sets the transport in protocol.
    @sa axis2_options_ops#set_transport_in_protocol */
#define AXIS2_OPTIONS_SET_TRANSPORT_IN_PROTOCOL(options, env, transport_in_protocol) \
      ((options)->ops->set_transport_in_protocol(options, env, transport_in_protocol))

/** Sets the message id.
    @sa axis2_options_ops#set_message_id */
#define AXIS2_OPTIONS_SET_MESSAGE_ID(options, env, message_id) \
      ((options)->ops->set_message_id(options, env, message_id))

/** Sets the properties.
    @sa axis2_options_ops#set_properties */
#define AXIS2_OPTIONS_SET_PROPERTIES(options, env, properties) \
      ((options)->ops->set_properties(options, env, properties))

/** Sets a property with a given key.
    @sa axis2_options_ops#set_property */
#define AXIS2_OPTIONS_SET_PROPERTY(options, env, key, property) \
      ((options)->ops->set_property(options, env, key, property))

/** Set WSA relates to information.
    @sa axis2_options_ops#set_relates_to */
#define AXIS2_OPTIONS_SET_RELATES_TO(options, env, relates_to) \
      ((options)->ops->set_relates_to(options, env, relates_to))

/** Set WSA reply to address.
    @sa axis2_options_ops#set_reply_to */
#define AXIS2_OPTIONS_SET_REPLY_TO(options, env, reply_to) \
      ((options)->ops->set_reply_to(options, env, reply_to))

/** Sets transport out.
    @sa axis2_options_ops#set_transport_out */
#define AXIS2_OPTIONS_SET_TRANSPORT_OUT(options, env, transport_out) \
      ((options)->ops->set_transport_out(options, env, transport_out))

/** Sets sender transport.
    @sa axis2_options_ops#set_sender_transport */
#define AXIS2_OPTIONS_SET_SENDER_TRANSPORT(options, env, sender_transport, conf) \
      ((options)->ops->set_sender_transport(options, env, sender_transport, conf))

/** Sets SOAP version uri.
    @sa axis2_options_ops#set_soap_version */
#define AXIS2_OPTIONS_SET_SOAP_VERSION_URI(options, env, soap_version_uri) \
      ((options)->ops->set_soap_version_uri(options, env, soap_version_uri))

/** Sets timeout in milli seconds.
    @sa axis2_options_ops#set_timeout_in_milli_seconds */
#define AXIS2_OPTIONS_SET_TIMEOUT_IN_MILLI_SECONDS(options, env, timeout_in_milli_seconds) \
      ((options)->ops->set_timeout_in_milli_seconds(options, env, timeout_in_milli_seconds))

/** Sets transport information.
    @sa axis2_options_ops#set_transport_info */
#define AXIS2_OPTIONS_SET_TRANSPORT_INFO(options, env, sender_transport, \
    receiver_transport, user_separate_listener) \
    ((options)->ops->set_tranport_info(options, env, sender_transport, \
    receiver_transport, user_separate_listener))

/** Sets use separate listener.
    @sa axis2_options_ops#set_use_separate_listener */
#define AXIS2_OPTIONS_SET_USE_SEPARATE_LISTENER(options, env, use_separate_listener) \
      ((options)->ops->set_use_separate_listener(options, env, use_separate_listener))

/** Adds a reference parameter.
    @sa axis2_options_ops#add_reference_parameter */
#define AXIS2_OPTIONS_ADD_REFERENCE_PARAMETER(options, env, reference_parameter) \
      ((options)->ops->add_reference_parameter(options, env, reference_parameter))

/** Whether to manage session.
    @sa axis2_options_ops#get_manage_session */
#define AXIS2_OPTIONS_GET_MANAGE_SESSION(options, env) \
      ((options)->ops->get_manage_session(options, env))

/** Sets manage session.
    @sa axis2_options_ops#set_manage_session*/
#define AXIS2_OPTIONS_SET_MANAGE_SESSION(options, env, manage_session) \
      ((options)->ops->set_manage_session(options, env, manage_session))

/** Sets message information headers.
    @sa axis2_options_ops#set_msg_info_headers */
#define AXIS2_OPTIONS_SET_MSG_INFO_HEADERS(options, env, msg_info_headers) \
      ((options)->ops->set_msg_info_headers(options, env, msg_info_headers))

/** Gets message information headers.
    @sa axis2_options_ops#get_msg_info_headers */
#define AXIS2_OPTIONS_GET_MSG_INFO_HEADERS(options, env) \
      ((options)->ops->get_msg_info_headers(options, env))

/** Sets SOAP version.
    @sa axis2_options_ops#set_soap_version */
#define AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, soap_version) \
      ((options)->ops->set_soap_version(options, env, soap_version))

/** Gets SOAP version.
    @sa axis2_options_ops#get_soap_version */
#define AXIS2_OPTIONS_GET_SOAP_VERSION(options, env) \
      ((options)->ops->get_soap_version(options, env))

/** Sets enable mtom.
    @sa axis2_options_ops#set_enable_mtom */
#define AXIS2_OPTIONS_SET_ENABLE_MTOM(options, env, enable_mtom) \
      ((options)->ops->set_enable_mtom(options, env, enable_mtom))

/** Gets enable mtom.
    @sa axis2_options_ops#get_enable_mtom */
#define AXIS2_OPTIONS_GET_ENABLE_MTOM(options, env) \
      ((options)->ops->get_enable_mtom(options, env))

/** Gets SOAP action.
    @sa axis2_options_ops#get_soap_action */
#define AXIS2_OPTIONS_GET_SOAP_ACTION(options, env) \
      ((options)->ops->get_soap_action(options, env))

/** Sets the SOAP action.
    @sa axis2_options_ops#set_soap_action */
#define AXIS2_OPTIONS_SET_SOAP_ACTION(options, env, action) \
      ((options)->ops->set_soap_action(options, env, action))

/** Sets xml parser reset value  */
#define AXIS2_OPTIONS_SET_XML_PARSER_RESET(options, env, flag) \
      ((options)->ops->set_xml_parser_reset(options, env, flag))

/** Frees the options struct.
    @sa axis2_options_ops#free*/
#define AXIS2_OPTIONS_FREE(options, env) \
      ((options)->ops->free(options, env))

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OPTIONS_H */
