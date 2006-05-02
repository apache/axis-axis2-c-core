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

#include <axis2_options.h>
#include <axis2_const.h>
#include <axis2_hash.h>
#include <axis2_soap_const.h>
#include <axis2_msg_info_headers.h>
#include <axis2_array_list.h>

typedef struct axis2_options_impl
{
    /** options base struct */
    axis2_options_t options;

	/** parent options */
	axis2_options_t *parent;

	axis2_hash_t *properties;

	axis2_char_t *soap_version_uri;

	long timeout_in_milli_seconds;
	
	axis2_bool_t use_seperate_listener;

	/** addressing specific properties */
	axis2_msg_info_headers_t *msg_info_headers;

	axis2_transport_receiver_t *receiver;

	axis2_transport_in_desc_t *transport_in;

	axis2_char_t *transport_in_protocol;


	/** for sending and receiving messages */

	axis2_transport_out_desc_t *transport_out;

	axis2_char_t *sender_transport_protocol;

	axis2_bool_t manage_session;
} axis2_options_impl_t;

/** Interface to implementation conversion macro */
#define AXIS2_INTF_TO_IMPL(options) ((axis2_options_impl_t *)options)

/** private function prototypes */
static void axis2_options_init_data(axis2_options_impl_t *options);
static void axis2_options_init_ops(struct axis2_options *options);


/** public function prototypes */
axis2_char_t* AXIS2_CALL 
axis2_options_get_action(struct axis2_options *options,
                		axis2_env_t **env);


axis2_endpoint_ref_t* AXIS2_CALL 
axis2_options_get_fault_to(struct axis2_options *options,
                    		axis2_env_t **env);


axis2_endpoint_ref_t* AXIS2_CALL
axis2_options_get_from(struct axis2_options *options,
            			axis2_env_t **env);

axis2_transport_receiver_t* AXIS2_CALL 
axis2_options_get_transport_receiver(struct axis2_options *options,
                            		axis2_env_t **env);


axis2_transport_in_desc_t* AXIS2_CALL 
axis2_options_get_transport_in(struct axis2_options *options,
                            	axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_options_get_transport_in_protocol(struct axis2_options *options,
                                			axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_options_get_message_id(struct axis2_options *options,
                    			axis2_env_t **env);

axis2_hash_t* AXIS2_CALL 
axis2_options_get_properties(struct axis2_options *options,
                    			axis2_env_t **env);

void* AXIS2_CALL 
axis2_options_get_property(struct axis2_options *options,
                    		axis2_env_t **env,
                    		axis2_char_t *key);

axis2_relates_to_t* AXIS2_CALL 
axis2_options_get_relates_to(struct axis2_options *options,
                    			axis2_env_t **env);


axis2_endpoint_ref_t* AXIS2_CALL 
axis2_options_get_reply_to(struct axis2_options *options,
                    		axis2_env_t **env);

axis2_transport_out_desc_t* AXIS2_CALL 
axis2_options_get_transport_out(struct axis2_options *options,
                        		axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_options_get_sender_transport_protocol(struct axis2_options *options,
                                    		axis2_env_t **env);

axis2_char_t* AXIS2_CALL 
axis2_options_get_soap_version_uri(struct axis2_options *options,
                            		axis2_env_t **env);

long AXIS2_CALL 
axis2_options_get_timeout_in_milli_seconds(struct axis2_options *options,
											axis2_env_t **env);

axis2_endpoint_ref_t* AXIS2_CALL 
axis2_options_get_to(struct axis2_options *options,
                    	axis2_env_t **env);


axis2_bool_t AXIS2_CALL 
axis2_options_is_use_seperate_listener(struct axis2_options *options,
                                		axis2_env_t **env);

struct axis2_options * AXIS2_CALL 
axis2_options_get_parent(struct axis2_options *options,
                			axis2_env_t **env);

void AXIS2_CALL 
axis2_options_set_parent(struct axis2_options *options,
                    		axis2_env_t **env,
                    		struct axis2_options *parent);

void AXIS2_CALL 
axis2_options_set_action(struct axis2_options *options,
                			axis2_env_t **env,
                			axis2_char_t *action);

void AXIS2_CALL 
axis2_options_set_fault_to(struct axis2_options *options,
                			axis2_env_t **env,
                			axis2_endpoint_ref_t *fault_to);

void AXIS2_CALL 
axis2_options_set_from(struct axis2_options *options,
                		axis2_env_t **env,
                		axis2_endpoint_ref_t *from);

void AXIS2_CALL 
axis2_options_set_to(struct axis2_options *options,
                		axis2_env_t **env,
                		axis2_endpoint_ref_t *to);

void AXIS2_CALL 
axis2_options_set_transport_receiver(struct axis2_options *options,
                            			axis2_env_t **env,
                            			axis2_transport_receiver_t *receiver);

void AXIS2_CALL 
axis2_options_set_transport_in(struct axis2_options *options,
                        		axis2_env_t **env,
                        		axis2_transport_in_desc_t *transport_in);

void AXIS2_CALL 
axis2_options_set_transport_in_protocol(struct axis2_options *options,
                                		axis2_env_t **env,
                                		axis2_char_t *transport_in_protocol);


void AXIS2_CALL 
axis2_options_set_message_id(struct axis2_options *options,
                    			axis2_env_t **env,
                    			axis2_char_t *message_id);

axis2_status_t AXIS2_CALL 
axis2_options_set_properties(struct axis2_options *options,
                    			axis2_env_t **env,
                    			axis2_hash_t *properties);

void AXIS2_CALL 
axis2_options_set_property(struct axis2_options *options,
                    		axis2_env_t **env,
                    		axis2_char_t *property_key,
                    		void *property);


void AXIS2_CALL 
axis2_options_set_relates_to(struct axis2_options *options,
                    			axis2_env_t **env,
                    			axis2_relates_to_t *relates_to);

void AXIS2_CALL 
axis2_options_set_reply_to(struct axis2_options *options,
                    		axis2_env_t **env,
                    		axis2_endpoint_ref_t *reply_to);

void AXIS2_CALL 
axis2_options_set_transport_out(struct axis2_options *options,
                        		axis2_env_t **env,
                        		axis2_transport_out_desc_t *transport_out);


axis2_status_t AXIS2_CALL 
axis2_options_set_sender_transport(struct axis2_options *options,
                            		axis2_env_t **env,
                            		axis2_char_t *sender_transport,
                            		axis2_conf_t *conf);

void AXIS2_CALL 
axis2_options_set_soap_version_uri(struct axis2_options *options,
                            		axis2_env_t **env,
                            		axis2_char_t *soap_version_uri);

void AXIS2_CALL 
axis2_options_set_timeout_in_milli_seconds(struct axis2_options *options,
                                    		axis2_env_t **env,
                                    		long timeout_in_milli_seconds);


axis2_status_t AXIS2_CALL 
axis2_options_set_transport_info(struct axis2_options *options,
                        		axis2_env_t **env,
                        		axis2_char_t *sender_transport,
                        		axis2_char_t *receiver_transport,
                        		axis2_bool_t use_seperate_listener);

void AXIS2_CALL 
axis2_options_set_use_seperate_listener(struct axis2_options *options,
                                		axis2_env_t **env,
                                		axis2_bool_t use_seperate_listener);
/*TODO:check - changed om_element to om_node*/
void AXIS2_CALL 
axis2_options_add_reference_parameter(struct axis2_options *options,
                            axis2_env_t **env,
                            axis2_om_node_t *reference_parameter);

void AXIS2_CALL 
axis2_options_set_manage_session(struct axis2_options *options,
                            axis2_env_t **env,
                            axis2_bool_t manage_session);

axis2_bool_t AXIS2_CALL 
axis2_options_is_manage_session(struct axis2_options *options,
                            axis2_env_t **env);

axis2_msg_info_headers_t* AXIS2_CALL 
axis2_options_get_msg_info_headers(struct axis2_options *options,
                            axis2_env_t **env);

axis2_status_t AXIS2_CALL
axis2_options_free (struct axis2_options *options,
		                     axis2_env_t **env);


axis2_options_t* AXIS2_CALL 
axis2_options_create(axis2_env_t **env)
{
    axis2_options_impl_t *options_impl = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    
    options_impl = AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_options_impl_t) );
    if (!options_impl)
    { 
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;        
    }

	/** initialize private data */
    axis2_options_init_data(options_impl);
	
	options_impl->msg_info_headers = axis2_msg_info_headers_create(env, NULL, NULL);
	if (!(options_impl->msg_info_headers))
	{
		axis2_options_free(&(options_impl->options), env);
		return NULL;
	}

	options_impl->properties = axis2_hash_make(env);
	if (!(options_impl->properties))
	{
		axis2_options_free(&(options_impl->options), env);
		return NULL;
	}

    /** initialize ops */    
    options_impl->options.ops  = 
        AXIS2_MALLOC( (*env)->allocator, sizeof(axis2_options_ops_t) );
    
    if (!options_impl->options.ops)
    {
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        axis2_options_free(&(options_impl->options), env);
        return NULL;        
    }

	axis2_options_init_ops(&(options_impl->options));

    return &(options_impl->options);
}

axis2_options_t* AXIS2_CALL
axis2_options_create_with_parent(axis2_env_t **env,
									struct axis2_options *parent)
{

	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(axis2_options_create(env));

	if (options_impl)
	{
		options_impl->parent = parent;
	}
	return &(options_impl->options);
}

axis2_char_t* AXIS2_CALL 
axis2_options_get_action(struct axis2_options *options, 
                                  axis2_env_t **env) 
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_char_t *action = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	action = AXIS2_MSG_INFO_HEADERS_GET_ACTION(options_impl->msg_info_headers, env);

	if (action == NULL && options_impl->parent)
	{
		return axis2_options_get_action(options_impl->parent, env);
	}

	return action;
}

axis2_endpoint_ref_t* AXIS2_CALL 
axis2_options_get_fault_to(struct axis2_options *options,
                    		axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_endpoint_ref_t *fault_to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	fault_to = AXIS2_MSG_INFO_HEADERS_GET_FAULT_TO(options_impl->msg_info_headers, env);

	if (fault_to == NULL && options_impl->parent)
	{
		return axis2_options_get_fault_to(options_impl->parent, env);
	}

	return fault_to;
}


axis2_endpoint_ref_t* AXIS2_CALL
axis2_options_get_from(struct axis2_options *options,
            			axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_endpoint_ref_t *from = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	from = AXIS2_MSG_INFO_HEADERS_GET_FROM(options_impl->msg_info_headers, env);

	if (from == NULL && options_impl->parent)
	{
		return axis2_options_get_from(options_impl->parent, env);
	}

	return from;
}

axis2_transport_receiver_t* AXIS2_CALL 
axis2_options_get_transport_receiver(struct axis2_options *options,
                            		axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->receiver == NULL && options_impl->parent)
	{
		return axis2_options_get_transport_receiver(options_impl->parent, env);
	}

	return options_impl->receiver;
}


axis2_transport_in_desc_t* AXIS2_CALL 
axis2_options_get_transport_in(struct axis2_options *options,
                            	axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->transport_in == NULL && options_impl->parent)
	{
		return axis2_options_get_transport_in(options_impl->parent, env);
	}

	return options_impl->transport_in;
}

axis2_char_t* AXIS2_CALL 
axis2_options_get_transport_in_protocol(struct axis2_options *options,
                                			axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->transport_in_protocol == NULL && options_impl->parent)
	{
		return axis2_options_get_transport_in_protocol(options_impl->parent, env);
	}

	return options_impl->transport_in_protocol;
}

axis2_char_t* AXIS2_CALL 
axis2_options_get_message_id(struct axis2_options *options,
                    			axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_char_t *message_id = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	message_id = AXIS2_MSG_INFO_HEADERS_GET_MESSAGE_ID(options_impl->msg_info_headers, env);

	if (message_id == NULL && options_impl->parent)
	{
		return axis2_options_get_message_id(options_impl->parent, env);
	}

	return message_id;
}

/*TODO:pointer is returned; do we need to copy to a new axis2_hash_t 
  as java does??
  */
axis2_hash_t* AXIS2_CALL 
axis2_options_get_properties(struct axis2_options *options,
                    			axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (axis2_hash_count(options_impl->properties) == 0 && options_impl->parent)
	{
		return axis2_options_get_properties(options_impl->parent, env);
	}

	return options_impl->properties;
}

void* AXIS2_CALL 
axis2_options_get_property(struct axis2_options *options,
                    		axis2_env_t **env,
                    		axis2_char_t *key)
{
	void *property = NULL;
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);
	property = axis2_hash_get(options_impl->properties, key, AXIS2_HASH_KEY_STRING);

	if (property == NULL && options_impl->parent)
	{
		return axis2_options_get_property(options_impl->parent, env, key);
	}

	return property;
}

axis2_relates_to_t* AXIS2_CALL 
axis2_options_get_relates_to(struct axis2_options *options,
                    			axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_relates_to_t *relates_to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	relates_to = AXIS2_MSG_INFO_HEADERS_GET_RELATES_TO(options_impl->msg_info_headers, env);

	if (relates_to == NULL && options_impl->parent)
	{
		return axis2_options_get_relates_to(options_impl->parent, env);
	}

	return relates_to;
}


axis2_endpoint_ref_t* AXIS2_CALL 
axis2_options_get_reply_to(struct axis2_options *options,
                    		axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_endpoint_ref_t *reply_to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	reply_to = AXIS2_MSG_INFO_HEADERS_GET_REPLY_TO(options_impl->msg_info_headers, env);

	if (reply_to == NULL && options_impl->parent)
	{
		return axis2_options_get_reply_to(options_impl->parent, env);
	}

	return reply_to;
}

axis2_transport_out_desc_t* AXIS2_CALL 
axis2_options_get_transport_out(struct axis2_options *options,
                        		axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->transport_out == NULL && options_impl->parent)
	{
		return axis2_options_get_transport_out(options_impl->parent, env);
	}

	return options_impl->transport_out;
}

axis2_char_t* AXIS2_CALL 
axis2_options_get_sender_transport_protocol(struct axis2_options *options,
                                    		axis2_env_t **env)
{

	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->sender_transport_protocol == NULL && options_impl->parent)
	{
		return axis2_options_get_sender_transport_protocol(options_impl->parent, env);
	}

	return options_impl->sender_transport_protocol;
}

axis2_char_t* AXIS2_CALL 
axis2_options_get_soap_version_uri(struct axis2_options *options,
                            		axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->soap_version_uri == NULL && options_impl->parent)
	{
		return axis2_options_get_soap_version_uri(options_impl->parent, env);
	}

	if (options_impl->soap_version_uri)
	{
		return options_impl->soap_version_uri;
	}
	/**TODO:java returns 11- isn't the default is 12?*/
	return AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI; 

}

long AXIS2_CALL 
axis2_options_get_timeout_in_milli_seconds(struct axis2_options *options,
											axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->timeout_in_milli_seconds == -1 && options_impl->parent)
	{
		return axis2_options_get_timeout_in_milli_seconds(options_impl->parent, env);
	}

	if (options_impl->timeout_in_milli_seconds == -1)
	{
		return AXIS2_DEFAULT_TIMEOUT_MILLISECONDS;
	}
	
	return options_impl->timeout_in_milli_seconds;
}


axis2_endpoint_ref_t* AXIS2_CALL 
axis2_options_get_to(struct axis2_options *options,
                    	axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_endpoint_ref_t *to = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	to = AXIS2_MSG_INFO_HEADERS_GET_TO(options_impl->msg_info_headers, env);

	if (to == NULL && options_impl->parent)
	{
		return axis2_options_get_to(options_impl->parent, env);
	}

	return to;
}


axis2_bool_t AXIS2_CALL 
axis2_options_is_use_seperate_listener(struct axis2_options *options,
                                		axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    /*AXIS2_ENV_CHECK(env, NULL);*/

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->use_seperate_listener == -1 && options_impl->parent)
	{
		return axis2_options_is_use_seperate_listener(options_impl->parent, env);
	}

	if (options_impl->use_seperate_listener == -1)
	{
		return AXIS2_FALSE;
	}
	
	return options_impl->use_seperate_listener;
}

struct axis2_options * AXIS2_CALL 
axis2_options_get_parent(struct axis2_options *options,
                			axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    /*AXIS2_ENV_CHECK(env, NULL);*/

	options_impl = AXIS2_INTF_TO_IMPL(options);

	return options_impl->parent;
}

void AXIS2_CALL 
axis2_options_set_parent(struct axis2_options *options,
                    		axis2_env_t **env,
                    		struct axis2_options *parent)
{
	axis2_options_impl_t *options_impl = NULL;
    /*AXIS2_ENV_CHECK(env, NULL);*/

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->parent = parent;
}

void AXIS2_CALL 
axis2_options_set_action(struct axis2_options *options,
                			axis2_env_t **env,
                			axis2_char_t *action)
{
	axis2_options_impl_t *options_impl = NULL;
    /*AXIS2_ENV_CHECK(env, NULL);*/

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_SET_ACTION(options_impl->msg_info_headers, env, action);
}

void AXIS2_CALL 
axis2_options_set_fault_to(struct axis2_options *options,
                			axis2_env_t **env,
                			axis2_endpoint_ref_t *fault_to)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_SET_FAULT_TO(options_impl->msg_info_headers, env, fault_to);
}

void AXIS2_CALL 
axis2_options_set_from(struct axis2_options *options,
                		axis2_env_t **env,
                		axis2_endpoint_ref_t *from)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_SET_FROM(options_impl->msg_info_headers, env, from);
}

void AXIS2_CALL 
axis2_options_set_to(struct axis2_options *options,
                		axis2_env_t **env,
                		axis2_endpoint_ref_t *to)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_SET_TO(options_impl->msg_info_headers, env, to);
}

void AXIS2_CALL 
axis2_options_set_transport_receiver(struct axis2_options *options,
                            			axis2_env_t **env,
                            			axis2_transport_receiver_t *receiver)
{

	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->receiver = receiver;
}

void AXIS2_CALL 
axis2_options_set_transport_in(struct axis2_options *options,
                        		axis2_env_t **env,
                        		axis2_transport_in_desc_t *transport_in)
{

	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->transport_in = transport_in;
}

void AXIS2_CALL 
axis2_options_set_transport_in_protocol(struct axis2_options *options,
                                		axis2_env_t **env,
                                		axis2_char_t *transport_in_protocol)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->transport_in_protocol = transport_in_protocol;
}


void AXIS2_CALL 
axis2_options_set_message_id(struct axis2_options *options,
                    			axis2_env_t **env,
                    			axis2_char_t *message_id)
{

	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_SET_MESSAGE_ID(options_impl->msg_info_headers, env, message_id);
}

axis2_status_t AXIS2_CALL 
axis2_options_set_properties(struct axis2_options *options,
                    			axis2_env_t **env,
                    			axis2_hash_t *properties)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	/*TODO:what happens to hash objects when freeing??*/
	if (options_impl->properties)
	{
		axis2_hash_free(options_impl->properties, env);
	}
	options_impl->properties = properties;
	return AXIS2_SUCCESS;
}

void AXIS2_CALL 
axis2_options_set_property(struct axis2_options *options,
                    		axis2_env_t **env,
                    		axis2_char_t *property_key,
                    		void *property)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);
	axis2_hash_set(options_impl->properties, property_key,
			AXIS2_HASH_KEY_STRING, property);
}


void AXIS2_CALL 
axis2_options_set_relates_to(struct axis2_options *options,
                    			axis2_env_t **env,
                    			axis2_relates_to_t *relates_to)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_SET_RELATES_TO(options_impl->msg_info_headers, env, relates_to);
}

void AXIS2_CALL 
axis2_options_set_reply_to(struct axis2_options *options,
                    		axis2_env_t **env,
                    		axis2_endpoint_ref_t *reply_to)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_SET_REPLY_TO(options_impl->msg_info_headers, env, reply_to);
}

void AXIS2_CALL 
axis2_options_set_transport_out(struct axis2_options *options,
                        		axis2_env_t **env,
                        		axis2_transport_out_desc_t *transport_out)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->transport_out = transport_out;
}


axis2_status_t AXIS2_CALL 
axis2_options_set_sender_transport(struct axis2_options *options,
                            		axis2_env_t **env,
                            		axis2_char_t *sender_transport,
                            		axis2_conf_t *conf)
{
	axis2_options_impl_t *options_impl = NULL;
	axis2_qname_t *tmp = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	options_impl = AXIS2_INTF_TO_IMPL(options);
	
	tmp = axis2_qname_create(env, sender_transport, NULL, NULL);
	if (!tmp)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	
	options_impl->transport_out = AXIS2_CONF_GET_TRANSPORT_OUT(conf, env, tmp);
	AXIS2_QNAME_FREE(tmp, env);

	if (!options_impl->transport_out)
	{
		/*TODO:error*/
		return AXIS2_FAILURE;
	}
	return AXIS2_SUCCESS;
}

void AXIS2_CALL 
axis2_options_set_soap_version_uri(struct axis2_options *options,
                            		axis2_env_t **env,
                            		axis2_char_t *soap_version_uri)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->soap_version_uri = soap_version_uri;
}

void AXIS2_CALL 
axis2_options_set_timeout_in_milli_seconds(struct axis2_options *options,
                                    		axis2_env_t **env,
                                    		long timeout_in_milli_seconds)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->timeout_in_milli_seconds = timeout_in_milli_seconds;
}


axis2_status_t AXIS2_CALL 
axis2_options_set_transport_info(struct axis2_options *options,
                        		axis2_env_t **env,
                        		axis2_char_t *sender_transport,
                        		axis2_char_t *receiver_transport,
                        		axis2_bool_t use_seperate_listener)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

	options_impl = AXIS2_INTF_TO_IMPL(options);
	/**
	  here we check for the legal combination
	  */
	if (!use_seperate_listener)
	{
		if (0 != strcmp(sender_transport, receiver_transport))	
		{
			/*TODO:error*/
			return AXIS2_FAILURE;
		}
		/*TODO: check the transport is either HTTP or TCP*/
	}
	else
	{
		axis2_options_set_use_seperate_listener(&(options_impl->options),
					env, use_seperate_listener);
	}
	axis2_options_set_transport_in_protocol(&(options_impl->options), env, receiver_transport);
	options_impl->sender_transport_protocol = sender_transport;

	return AXIS2_SUCCESS;
}

void AXIS2_CALL 
axis2_options_set_use_seperate_listener(struct axis2_options *options,
                                		axis2_env_t **env,
                                		axis2_bool_t use_seperate_listener)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->use_seperate_listener = use_seperate_listener;
}

void AXIS2_CALL
axis2_options_add_reference_parameter(struct axis2_options *options,
                            axis2_env_t **env,
                            axis2_om_node_t *reference_parameter)
{

	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	AXIS2_MSG_INFO_HEADERS_ADD_REF_PARAM(options_impl->msg_info_headers,
				env, reference_parameter);
}


void AXIS2_CALL 
axis2_options_set_manage_session(struct axis2_options *options,
                            axis2_env_t **env,
                            axis2_bool_t manage_session)
{
	axis2_options_impl_t *options_impl = NULL;

	options_impl = AXIS2_INTF_TO_IMPL(options);

	options_impl->manage_session = manage_session;
}

axis2_bool_t AXIS2_CALL 
axis2_options_is_manage_session(struct axis2_options *options,
                            axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FALSE);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	if (options_impl->manage_session == -1 && options_impl->parent)
	{
		return axis2_options_is_manage_session(options_impl->parent, env);
	}

	if (options_impl->manage_session == -1)
	{
		return AXIS2_FALSE;
	}
	
	return options_impl->manage_session;
}

axis2_msg_info_headers_t* AXIS2_CALL 
axis2_options_get_msg_info_headers(struct axis2_options *options,
                            axis2_env_t **env)
{
	axis2_options_impl_t *options_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

	options_impl = AXIS2_INTF_TO_IMPL(options);

	return options_impl->msg_info_headers;
}

axis2_status_t AXIS2_CALL 
axis2_options_free (struct axis2_options *options, 
                     axis2_env_t **env)
{
    axis2_options_impl_t *options_impl = NULL;
    
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    options_impl = AXIS2_INTF_TO_IMPL(options);
    
    if (options_impl->options.ops)
    {
        AXIS2_FREE((*env)->allocator, options_impl->options.ops);
        options_impl->options.ops = NULL;
    }

	if (options_impl->properties)
	{
		axis2_hash_free(options_impl->properties, env);
		options_impl->properties = NULL;
	}

    AXIS2_FREE((*env)->allocator, options_impl);
    options_impl = NULL;
    
    return AXIS2_SUCCESS;
}

static void axis2_options_init_data(axis2_options_impl_t *options_impl)
{
    options_impl->options.ops = NULL;
	options_impl->parent = NULL;
	options_impl->properties = NULL;
	options_impl->soap_version_uri = NULL;
	/*AXIS2_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;*/
	options_impl->timeout_in_milli_seconds = -1;
	/*AXIS2_DEFAULT_TIMEOUT_MILLISECONDS;*/
	options_impl->use_seperate_listener = -1;
	/*AXIS2_FALSE;*/

	options_impl->receiver = NULL;
	options_impl->transport_in = NULL;
	options_impl->transport_in_protocol = NULL;
	options_impl->transport_out = NULL;
	options_impl->sender_transport_protocol = NULL;
	options_impl->manage_session = -1;
}

static void axis2_options_init_ops(struct axis2_options *options)
{

	options->ops->get_action = axis2_options_get_action;
	options->ops->get_fault_to = axis2_options_get_fault_to;
	options->ops->get_from = axis2_options_get_from;
	options->ops->get_transport_receiver = axis2_options_get_transport_receiver;
	options->ops->get_transport_in = axis2_options_get_transport_in;
	options->ops->get_transport_in_protocol = axis2_options_get_transport_in_protocol;
	options->ops->get_message_id = axis2_options_get_message_id;
	options->ops->get_properties = axis2_options_get_properties;
	options->ops->get_property = axis2_options_get_property;
	options->ops->get_relates_to = axis2_options_get_relates_to;
	options->ops->get_reply_to = axis2_options_get_reply_to;
	options->ops->get_transport_out = axis2_options_get_transport_out;
	options->ops->get_sender_transport_protocol = axis2_options_get_sender_transport_protocol;
	options->ops->get_soap_version_uri = axis2_options_get_soap_version_uri;
	options->ops->get_timeout_in_milli_seconds = axis2_options_get_timeout_in_milli_seconds;
	options->ops->get_to = axis2_options_get_to;
	options->ops->is_use_seperate_listener = axis2_options_is_use_seperate_listener;
	options->ops->get_parent = axis2_options_get_parent;
	options->ops->set_parent = axis2_options_set_parent;
	options->ops->set_action = axis2_options_set_action;
	options->ops->set_fault_to = axis2_options_set_fault_to;
	options->ops->set_from = axis2_options_set_from;
	options->ops->set_to = axis2_options_set_to;
	options->ops->set_transport_receiver = axis2_options_set_transport_receiver;
	options->ops->set_transport_in = axis2_options_set_transport_in;
	options->ops->set_transport_in_protocol = axis2_options_set_transport_in_protocol;
	options->ops->set_message_id = axis2_options_set_message_id;
	options->ops->set_properties = axis2_options_set_properties;
	options->ops->set_property = axis2_options_set_property;
	options->ops->set_relates_to = axis2_options_set_relates_to;
	options->ops->set_reply_to = axis2_options_set_reply_to;
	options->ops->set_transport_out = axis2_options_set_transport_out;
	options->ops->set_sender_transport = axis2_options_set_sender_transport;
	options->ops->set_soap_version_uri = axis2_options_set_soap_version_uri;
	options->ops->set_timeout_in_milli_seconds = axis2_options_set_timeout_in_milli_seconds;
	options->ops->set_transport_info = axis2_options_set_transport_info;
	options->ops->set_use_seperate_listener = axis2_options_set_use_seperate_listener;
	options->ops->add_reference_parameter = axis2_options_add_reference_parameter;
	options->ops->set_manage_session = axis2_options_set_manage_session;
	options->ops->is_manage_session = axis2_options_is_manage_session;
	options->ops->free = axis2_options_free;
}
