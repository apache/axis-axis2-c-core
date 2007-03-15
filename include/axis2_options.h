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
/** Timeout in seconds waiting for a response envelope */
#define AXIS2_TIMEOUT_IN_SECONDS "time_out"
/** Copy properties */
#define AXIS2_COPY_PROPERTIES   "copy_properties"

#ifdef __cplusplus
extern "C"
{
#endif

    /** Type name for struct axis2_options */
    typedef struct axis2_options axis2_options_t;

    /**
     * Gets Web Services Addressing (WSA) action.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return WSA action string if set, else NULL
     */
    AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
    axis2_options_get_action(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets WSA fault to address.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference struct representing fault to 
     * address if set, else NULL
     */
    AXIS2_EXTERN axis2_endpoint_ref_t* AXIS2_CALL
    axis2_options_get_fault_to( const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets WSA from address.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference struct representing from 
     * address if set, else NULL
     */
    AXIS2_EXTERN axis2_endpoint_ref_t* AXIS2_CALL
    axis2_options_get_from(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets transport receiver.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to transport receiver struct if set, else NULL
     */
    AXIS2_EXTERN axis2_transport_receiver_t* AXIS2_CALL
    axis2_options_get_transport_receiver(const axis2_options_t *options,
        const axis2_env_t *env);


    /**
     * Gets transport in.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to transport in struct if set, else NULL
     */
    AXIS2_EXTERN axis2_transport_in_desc_t* AXIS2_CALL
    axis2_options_get_transport_in(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets transport in protocol.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to transport in protocol string if set, else NULL
     */
    AXIS2_EXTERN AXIS2_TRANSPORT_ENUMS AXIS2_CALL
    axis2_options_get_transport_in_protocol(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets message ID.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to message ID string if set, else NULL
     */
    AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
    axis2_options_get_message_id(const axis2_options_t *options_t,
        const axis2_env_t *env);

    /**
     * Gets the properties hash map.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to properties hash map if set, else NULL
     */
    AXIS2_EXTERN axis2_hash_t* AXIS2_CALL
    axis2_options_get_properties(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets a property corresponding to the given key.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param key key of the property to be returned
     * @return value corresponding to the given key
     */
    AXIS2_EXTERN void* AXIS2_CALL
    axis2_options_get_property(const axis2_options_t *options,
        const axis2_env_t *env,
        const axis2_char_t *key);

    /**
     * Gets relates to information.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to relates to struct if set, else NULL
     */
    AXIS2_EXTERN axis2_relates_to_t* AXIS2_CALL
    axis2_options_get_relates_to(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets WSA reply to address.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference struct representing reply to
     * address if set, else NULL
     */
    AXIS2_EXTERN axis2_endpoint_ref_t* AXIS2_CALL
    axis2_options_get_reply_to(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets transport out.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to transport out struct if set, else NULL
     */
    AXIS2_EXTERN axis2_transport_out_desc_t* AXIS2_CALL
    axis2_options_get_transport_out(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets transport out protocol.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to transport out protocol string if set, else NULL
     */
    AXIS2_EXTERN AXIS2_TRANSPORT_ENUMS AXIS2_CALL
    axis2_options_get_sender_transport_protocol(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets SOAP version URI.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return string representing SOAP version URI
     */
    AXIS2_EXTERN const axis2_char_t* AXIS2_CALL
    axis2_options_get_soap_version_uri(const axis2_options_t *options,
        const axis2_env_t *env);


    /**
     * Gets the wait time after which a client times out in a blocking scenario.
     * The default is AXIS2_DEFAULT_TIMEOUT_MILLISECONDS.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return timeout in milliseconds
     */
    AXIS2_EXTERN long AXIS2_CALL
    axis2_options_get_timeout_in_milli_seconds(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets WSA to address.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to endpoint reference struct representing to
     * address if set, else NULL
     */
    AXIS2_EXTERN axis2_endpoint_ref_t* AXIS2_CALL
    axis2_options_get_to(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets use separate listener status.
     * @param options pointer to options struct 
     * @param env pointer to environment struct
     * @return AXIS2_TRUE if using separate listener, else AXIS2_FALSE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_options_get_use_separate_listener(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets the parent options.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to the parent options struct if set, else NULL
     */
    AXIS2_EXTERN axis2_options_t *AXIS2_CALL
    axis2_options_get_parent(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Sets the parent options.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param parent pointer to parent options struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_parent(axis2_options_t *options,
        const axis2_env_t *env,
        const axis2_options_t *parent);

    /**
     * Sets WSA action
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param action pointer to action string
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_action(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_fault_to(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_from(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_to(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_transport_receiver(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_transport_in(axis2_options_t *options,
        const axis2_env_t *env,
        axis2_transport_in_desc_t *transport_in);

    /**
     * Sets transport in protocol.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param in_protocol pointer to in_protocol struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_transport_in_protocol(axis2_options_t *options,
        const axis2_env_t *env,
        const AXIS2_TRANSPORT_ENUMS transport_in_protocol);

    /**
     * Sets message ID.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param message_id pointer to message_id struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_message_id(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_properties(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_property(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_relates_to(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_reply_to(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_transport_out(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_sender_transport(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_soap_version_uri(axis2_options_t *options,
        const axis2_env_t *env,
        const axis2_char_t *soap_version_uri);

    /**
     * Sets timeout in Milli seconds.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param timeout_in_milli_seconds timeout in milli seconds
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_timeout_in_milli_seconds(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_transport_info(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_use_separate_listener(axis2_options_t *options,
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
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_add_reference_parameter(axis2_options_t *options,
        const axis2_env_t *env,
        axiom_node_t *reference_parameter);

    /**
     * Gets manage session bool value.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return AXIS2_TRUE if session is managed, else AXIS2_FALSE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_options_get_manage_session(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Sets manage session bool value.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param manage_session manage session bool value
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_manage_session(axis2_options_t *options,
        const axis2_env_t *env,
        const axis2_bool_t manage_session);

    /**
     * Sets WSA message information headers.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param pointer to message information headers struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_msg_info_headers(axis2_options_t *options,
        const axis2_env_t *env,
        axis2_msg_info_headers_t *msg_info_headers);

    /**
     * Gets WSA message information headers.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return pointer to message information headers struct if set, 
     * else NULL
     */
    AXIS2_EXTERN axis2_msg_info_headers_t* AXIS2_CALL
    axis2_options_get_msg_info_headers(const axis2_options_t *options,
        const axis2_env_t *env);

    /** 
     * Gets SOAP version.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return AXIOM_SOAP_11 if SOAP version 1.1 is in use, else AXIOM_SOAP_12
     */
    AXIS2_EXTERN int AXIS2_CALL
    axis2_options_get_soap_version(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Sets SOAP version.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param soap_version soap version, either AXIOM_SOAP_11 or AXIOM_SOAP_12
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_soap_version(axis2_options_t *options,
        const axis2_env_t *env,
        const int soap_version);

    /**
     * Enable/disable MTOM handling.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param enable_mtom AXIS2_TRUE if MTOM is to be enabled, AXIS2_FALSE
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */

    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_enable_mtom(axis2_options_t *options,
        const axis2_env_t *env,
        axis2_bool_t enable_mtom);

    /**
     * Gets Enable/disable MTOM status.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return AXIS2_TRUE if MTOM enabled, else AXIS2_FALSE
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_options_get_enable_mtom(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Gets SOAP action.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return SOAP Action string if set, else NULL
     */
    AXIS2_EXTERN axis2_string_t* AXIS2_CALL
    axis2_options_get_soap_action(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Sets SOAP action
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param action pointer to SOAP action string
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_soap_action(axis2_options_t *options,
        const axis2_env_t *env,
        axis2_string_t *soap_action);

    /**
     * Sets xml parser reset
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @param reset flag is a boolean value
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_set_xml_parser_reset(axis2_options_t *options,
        const axis2_env_t *env,
        const axis2_bool_t paser_reset_flag);

    /**
     * Gets xml parser reset value,
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return xml parser reset boolean value
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_options_get_xml_parser_reset(const axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Frees options struct.
     * @param options pointer to options struct
     * @param env pointer to environment struct
     * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
     */
    AXIS2_EXTERN axis2_status_t AXIS2_CALL
    axis2_options_free(axis2_options_t *options,
        const axis2_env_t *env);

    /**
     * Creates the options struct.
     * @param env pointer to environment struct
     * @return a pointer to newly created options struct, or NULL on error 
     * with error code set in environment's error.
     */
    AXIS2_EXTERN axis2_options_t* AXIS2_CALL 
    axis2_options_create(const axis2_env_t *env);
    
    /**
     * Creates the options struct with given parent.
     * @param env pointer to environment struct
     * @param parent pointer to parent struct
     * @return a pointer to newly created options struct. Newly created options 
     * assumes ownership of the parent 
     * or NULL on error with error code set in environment's error.
     */
    AXIS2_EXTERN axis2_options_t* AXIS2_CALL 
    axis2_options_create_with_parent(const axis2_env_t *env,
        axis2_options_t *parent);

/** Gets WSA action. */
#define AXIS2_OPTIONS_GET_ACTION(options, env) \
      axis2_options_get_action(options, env)

/** Get WSA fault to address. */
#define AXIS2_OPTIONS_GET_FAULT_TO(options, env) \
      axis2_options_get_fault_to(options, env)

/** Gets WSA from address. */
#define AXIS2_OPTIONS_GET_FROM(options, env) \
      axis2_options_get_from(options, env)

/** Gets the transport receiver. */
#define AXIS2_OPTIONS_GET_TRANSPORT_RECEIVER(options, env) \
      axis2_options_get_transport_receiver(options, env)

/** Gets transport in. */
#define AXIS2_OPTIONS_GET_TRANSPORT_IN(options, env) \
      axis2_options_get_transport_in(options, env)

/** Gets the transport in protocol. */
#define AXIS2_OPTIONS_GET_TRANSPORT_IN_PROTOCOL(options, env) \
      axis2_options_get_transport_in_protocol(options, env)

/** Gets the message id. */
#define AXIS2_OPTIONS_GET_MESSAGE_ID(options, env) \
      axis2_options_get_message_id(options, env)

/** Gets the properties. */
#define AXIS2_OPTIONS_GET_PROPERTIES(options, env) \
      axis2_options_get_properties(options, env)

/** Gets a property corresponding to given key. */
#define AXIS2_OPTIONS_GET_PROPERTY(options, env, key) \
      axis2_options_get_property(options, env, key)

/** Get WSA relates to information. */
#define AXIS2_OPTIONS_GET_RELATES_TO(options, env) \
      axis2_options_get_relates_to(options, env)

/** Get WSA reply to address. */
#define AXIS2_OPTIONS_GET_REPLY_TO(options, env) \
      axis2_options_get_reply_to(options, env)

/** Gets the transport out. */
#define AXIS2_OPTIONS_GET_TRANSPORT_OUT(options, env) \
      axis2_options_get_transport_out(options, env)

/** Gets the sender protocol. */
#define AXIS2_OPTIONS_GET_SENDER_PROTOCOL(options, env) \
      axis2_options_get_sender_transport_protocol(options, env)

/** Gets the soap version uri. */
#define AXIS2_OPTIONS_GET_SOAP_VERSION_URI(options, env) \
      axis2_options_get_soap_version_uri(options, env)

/** Gets the timeout in milli seconds. */
#define AXIS2_OPTIONS_GET_TIMEOUT_IN_MILLI_SECONDS(options, env) \
      axis2_options_get_timeout_in_milli_seconds(options, env)

/** Get WSA to address. */
#define AXIS2_OPTIONS_GET_TO(options, env) \
      axis2_options_get_to(options, env)

/** Get xml parser reset value */
#define AXIS2_OPTIONS_GET_XML_PARSER_RESET(options, env) \
      axis2_options_get_xml_parser_reset(options, env)

/** Whether to use a separate listener. */
#define AXIS2_OPTIONS_GET_USE_SEPERATE_LISTENER(options, env) \
      axis2_options_get_use_separate_listener(options, env)

/** Gets the parent. */
#define AXIS2_OPTIONS_GET_PARENT(options, env) \
      axis2_options_get_parent(options, env)

/** Sets the parent. */
#define AXIS2_OPTIONS_SET_PARENT(options, env, parent) \
      axis2_options_set_parent(options, env, parent)

/** Sets the WSA action. */
#define AXIS2_OPTIONS_SET_ACTION(options, env, action) \
      axis2_options_set_action(options, env, action)

/** Set WSA fault to address. */
#define AXIS2_OPTIONS_SET_FAULT_TO(options, env, fault_to) \
      axis2_options_set_fault_to(options, env, fault_to)

/** Set WSA from address. */
#define AXIS2_OPTIONS_SET_FROM(options, env, from) \
      axis2_options_set_from(options, env, from)

/** Set WSA to address. */
#define AXIS2_OPTIONS_SET_TO(options, env, to) \
      axis2_options_set_to(options, env, to)

/** Sets the transport receiver. */
#define AXIS2_OPTIONS_SET_TRANSPORT_RECEIVER(options, env, receiver) \
      axis2_options_set_transport_receiver(options, env, receiver)

/** Set transport in. */
#define AXIS2_OPTIONS_SET_TRANSPORT_IN(options, env, transport_in) \
      axis2_options_set_transport_in(options, env, transport_in)

/** Sets the transport in protocol. */
#define AXIS2_OPTIONS_SET_TRANSPORT_IN_PROTOCOL(options, env, transport_in_protocol) \
      axis2_options_set_transport_in_protocol(options, env, transport_in_protocol)

/** Sets the message id. */
#define AXIS2_OPTIONS_SET_MESSAGE_ID(options, env, message_id) \
      axis2_options_set_message_id(options, env, message_id)

/** Sets the properties. */
#define AXIS2_OPTIONS_SET_PROPERTIES(options, env, properties) \
      axis2_options_set_properties(options, env, properties)

/** Sets a property with a given key. */
#define AXIS2_OPTIONS_SET_PROPERTY(options, env, key, property) \
      axis2_options_set_property(options, env, key, property)

/** Set WSA relates to information. */
#define AXIS2_OPTIONS_SET_RELATES_TO(options, env, relates_to) \
      axis2_options_set_relates_to(options, env, relates_to)

/** Set WSA reply to address. */
#define AXIS2_OPTIONS_SET_REPLY_TO(options, env, reply_to) \
      axis2_options_set_reply_to(options, env, reply_to)

/** Sets transport out. */
#define AXIS2_OPTIONS_SET_TRANSPORT_OUT(options, env, transport_out) \
      axis2_options_set_transport_out(options, env, transport_out)

/** Sets sender transport. */
#define AXIS2_OPTIONS_SET_SENDER_TRANSPORT(options, env, sender_transport, conf) \
      axis2_options_set_sender_transport(options, env, sender_transport, conf)

/** Sets SOAP version uri. */
#define AXIS2_OPTIONS_SET_SOAP_VERSION_URI(options, env, soap_version_uri) \
      axis2_options_set_soap_version_uri(options, env, soap_version_uri)

/** Sets timeout in milli seconds. */
#define AXIS2_OPTIONS_SET_TIMEOUT_IN_MILLI_SECONDS(options, env, timeout_in_milli_seconds) \
      axis2_options_set_timeout_in_milli_seconds(options, env, timeout_in_milli_seconds)

/** Sets transport information. */
#define AXIS2_OPTIONS_SET_TRANSPORT_INFO(options, env, sender_transport, \
    receiver_transport, user_separate_listener) \
    axis2_options_set_tranport_info(options, env, sender_transport, \
    receiver_transport, user_separate_listener)

/** Sets use separate listener. */
#define AXIS2_OPTIONS_SET_USE_SEPARATE_LISTENER(options, env, use_separate_listener) \
      axis2_options_set_use_separate_listener(options, env, use_separate_listener)

/** Adds a reference parameter. */
#define AXIS2_OPTIONS_ADD_REFERENCE_PARAMETER(options, env, reference_parameter) \
      axis2_options_add_reference_parameter(options, env, reference_parameter)

/** Whether to manage session. */
#define AXIS2_OPTIONS_GET_MANAGE_SESSION(options, env) \
      axis2_options_get_manage_session(options, env)

/** Sets manage session. */
#define AXIS2_OPTIONS_SET_MANAGE_SESSION(options, env, manage_session) \
      axis2_options_set_manage_session(options, env, manage_session)

/** Sets message information headers. */
#define AXIS2_OPTIONS_SET_MSG_INFO_HEADERS(options, env, msg_info_headers) \
      axis2_options_set_msg_info_headers(options, env, msg_info_headers)

/** Gets message information headers. */
#define AXIS2_OPTIONS_GET_MSG_INFO_HEADERS(options, env) \
      axis2_options_get_msg_info_headers(options, env)

/** Sets SOAP version. */
#define AXIS2_OPTIONS_SET_SOAP_VERSION(options, env, soap_version) \
      axis2_options_set_soap_version(options, env, soap_version)

/** Gets SOAP version. */
#define AXIS2_OPTIONS_GET_SOAP_VERSION(options, env) \
      axis2_options_get_soap_version(options, env)

/** Sets enable mtom. */
#define AXIS2_OPTIONS_SET_ENABLE_MTOM(options, env, enable_mtom) \
      axis2_options_set_enable_mtom(options, env, enable_mtom)

/** Gets enable mtom. */
#define AXIS2_OPTIONS_GET_ENABLE_MTOM(options, env) \
      axis2_options_get_enable_mtom(options, env)

/** Gets SOAP action. */
#define AXIS2_OPTIONS_GET_SOAP_ACTION(options, env) \
      axis2_options_get_soap_action(options, env)

/** Sets the SOAP action. */
#define AXIS2_OPTIONS_SET_SOAP_ACTION(options, env, action) \
      axis2_options_set_soap_action(options, env, action)

/** Sets xml parser reset value  */
#define AXIS2_OPTIONS_SET_XML_PARSER_RESET(options, env, flag) \
      axis2_options_set_xml_parser_reset(options, env, flag)

/** Frees the options struct. */
#define AXIS2_OPTIONS_FREE(options, env) \
      axis2_options_free(options, env)

/** @} */
#ifdef __cplusplus
}
#endif

#endif                          /* AXIS2_OPTIONS_H */
