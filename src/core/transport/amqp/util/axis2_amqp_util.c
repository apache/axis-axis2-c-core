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

#include <axiom.h>
#include <axiom_mime_parser.h>
#include <axis2_util.h>
#include <axis2_addr.h>
#include <axutil_http_chunked_stream.h>
#include <axis2_amqp_defines.h>
#include <axis2_amqp_util.h>

AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_in_desc_conf_value_string(
	axis2_transport_in_desc_t* in_desc,
	const axutil_env_t* env,
	const axis2_char_t* param_name)
{
	axutil_param_t* param = NULL;
    axis2_char_t* value = NULL;

	param = (axutil_param_t*)
			axutil_param_container_get_param(
				axis2_transport_in_desc_param_container(in_desc, env),
				env,
				param_name);
    if (param)
    {
		value = axutil_param_get_value(param, env);
    }

	return value;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_amqp_util_get_in_desc_conf_value_int(
	axis2_transport_in_desc_t* in_desc,
	const axutil_env_t* env,
	const axis2_char_t* param_name)
{
	axis2_char_t* value_str = NULL;
	int value = AXIS2_QPID_NULL_CONF_INT;

	value_str = axis2_amqp_util_get_in_desc_conf_value_string(
			in_desc, env, param_name);
	if (value_str)
	{
		value = atoi(value_str);
	}

	return value;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_amqp_util_get_out_desc_conf_value_string(
	axis2_transport_out_desc_t* out_desc,
	const axutil_env_t* env,
	const axis2_char_t* param_name)
{
	axutil_param_t* param = NULL;
    axis2_char_t* value = NULL;

	param = (axutil_param_t*)
			axutil_param_container_get_param(
				axis2_transport_out_desc_param_container(out_desc, env),
				env,
				param_name);
    if (param)
    {
		value = axutil_param_get_value(param, env);
    }

	return value;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_amqp_util_get_out_desc_conf_value_int(
	axis2_transport_out_desc_t* out_desc,
	const axutil_env_t* env,
	const axis2_char_t* param_name)
{
	axis2_char_t* value_str = NULL;
	int value = AXIS2_QPID_NULL_CONF_INT;

	value_str = axis2_amqp_util_get_out_desc_conf_value_string(
			out_desc, env, param_name);
	if (value_str)
	{
		value = atoi(value_str);
	}

	return value;
}


AXIS2_EXTERN axiom_soap_envelope_t* AXIS2_CALL
axis2_amqp_util_get_soap_envelope(
	axis2_amqp_response_t* response,
	const axutil_env_t* env,
	axis2_msg_ctx_t* msg_ctx)
{
	axiom_xml_reader_t* xml_reader = NULL;
	axiom_stax_builder_t* stax_builder = NULL;
	axiom_soap_builder_t* soap_builder = NULL;
	axiom_soap_envelope_t* soap_envelope = NULL;
	const axis2_char_t* soap_ns_uri = NULL;
	axis2_char_t *soap_body_str = NULL;
	int soap_body_len = 0;
	axis2_bool_t is_mtom = AXIS2_FALSE;
	axutil_hash_t *binary_data_map = NULL;
	axis2_bool_t is_soap_11 = AXIS2_FALSE;

	if (!response || !response->data || !response->content_type)
	{
		return NULL;
	}

	is_soap_11 = axis2_msg_ctx_get_is_soap_11(msg_ctx, env);

	/* Handle MTOM */
	if (strstr(response->content_type, AXIS2_AMQP_HEADER_ACCEPT_MULTIPART_RELATED))
	{
		axis2_char_t* mime_boundary = axis2_amqp_util_get_value_from_content_type(env,
				response->content_type,
				AXIS2_AMQP_HEADER_CONTENT_TYPE_MIME_BOUNDARY);
		
		if (mime_boundary)
		{
			axiom_mime_parser_t *mime_parser = NULL;
			int soap_body_len = 0;
			axutil_param_t *buffer_size_param = NULL;
			axutil_param_t *max_buffers_param = NULL;
			axutil_param_t *attachment_dir_param = NULL;
			axis2_char_t *value_size = NULL;
			axis2_char_t *value_num = NULL;
			axis2_char_t *value_dir = NULL;
			int size = 0;
			int num = 0;
			
			mime_parser = axiom_mime_parser_create(env);
			
			buffer_size_param = axis2_msg_ctx_get_parameter(msg_ctx, 
					env, AXIS2_MTOM_BUFFER_SIZE);
			
			if (buffer_size_param)
			{
				value_size = (axis2_char_t *)axutil_param_get_value(buffer_size_param, env);
				
				if (value_size)
				{
					size = atoi(value_size);
					axiom_mime_parser_set_buffer_size(mime_parser, env, size);
				}
			}

            max_buffers_param = axis2_msg_ctx_get_parameter(msg_ctx, 
					env, AXIS2_MTOM_MAX_BUFFERS);

            if (max_buffers_param)
            {
                value_num = (axis2_char_t*)axutil_param_get_value(max_buffers_param, env);

                if(value_num)
                {
                    num = atoi(value_num);
                    axiom_mime_parser_set_max_buffers(mime_parser, env, num);
                }
            }

            attachment_dir_param = axis2_msg_ctx_get_parameter(msg_ctx,
					env, AXIS2_ATTACHMENT_DIR);

            if (attachment_dir_param)
            {
                value_dir = (axis2_char_t*)axutil_param_get_value(attachment_dir_param, env);

                if (value_dir)
                {
                    axiom_mime_parser_set_attachment_dir(mime_parser, env, value_dir);
                }
            }

            if (mime_parser)
            {
				axis2_callback_info_t* callback_ctx = NULL;
				axutil_stream_t* stream = NULL;

				callback_ctx = (axis2_callback_info_t*)
					AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t));

            	stream = axutil_stream_create_basic(env);

            	if (stream)
            	{
					axutil_stream_write(stream, env, response->data, 
							response->length);
                	callback_ctx->env = env;
                	callback_ctx->in_stream = stream;
                	callback_ctx->content_length = response->length;
                	callback_ctx->unread_len = response->length;
                	callback_ctx->chunked_stream = NULL;
            	}

                binary_data_map = 
                    axiom_mime_parser_parse(mime_parser, env,
                                            axis2_amqp_util_on_data_request,
                                            (void*)callback_ctx,
                                            mime_boundary);

                if (!binary_data_map)
                {
                    return AXIS2_FAILURE;
                }

				soap_body_len =
                    axiom_mime_parser_get_soap_body_len(mime_parser, env);

                soap_body_str =
                    axiom_mime_parser_get_soap_body_str(mime_parser, env);

				axutil_stream_free(stream, env);
				AXIS2_FREE(env->allocator, callback_ctx);
				axiom_mime_parser_free(mime_parser, env);
            }

			AXIS2_FREE(env->allocator, mime_boundary);
		}

		is_mtom = AXIS2_TRUE;
	}
	else
	{
		soap_body_str = response->data;
		soap_body_len = axutil_strlen(response->data);
	}

	soap_body_len = axutil_strlen(soap_body_str);

	xml_reader = axiom_xml_reader_create_for_memory(env, soap_body_str,
			soap_body_len, NULL, AXIS2_XML_PARSER_TYPE_BUFFER);
    if (!xml_reader)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create XML Reader");
        return NULL;
    }

    stax_builder = axiom_stax_builder_create(env, xml_reader);
    if (!stax_builder)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create StAX Builder");
        return NULL;
    }

	soap_ns_uri = is_soap_11 ? AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI : 
		AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;

    soap_builder = axiom_soap_builder_create(env, stax_builder, soap_ns_uri);
    if (!soap_builder)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create SOAP Builder");
        return NULL;
    }

	if (binary_data_map)
	{
		axiom_soap_builder_set_mime_body_parts(soap_builder, env, binary_data_map);
	}

	soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);
	
	if (soap_envelope)
	{
		/* hack to get around MTOM problem */
		axiom_soap_body_t *soap_body = 
			axiom_soap_envelope_get_body(soap_envelope, env);
		
		if (soap_body)
		{
			axiom_soap_body_has_fault(soap_body, env);
		}
	}
	
	return soap_envelope;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_amqp_util_conf_ctx_get_server_side(
	axis2_conf_ctx_t* conf_ctx,
	const axutil_env_t* env)
{
	axutil_property_t* property = NULL;
	axis2_char_t* value = NULL;
	
	property = axis2_conf_ctx_get_property(conf_ctx, env, AXIS2_IS_SVR_SIDE);
	if (!property)
		return AXIS2_TRUE;

	value = (axis2_char_t*)axutil_property_get_value(property, env);
	if (!value)
		return AXIS2_TRUE;

	return (axutil_strcmp(value, AXIS2_VALUE_TRUE) == 0) ? AXIS2_TRUE : AXIS2_FALSE;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_amqp_util_get_value_from_content_type(
    const axutil_env_t * env,
    const axis2_char_t * content_type,
    const axis2_char_t * key)
{
    axis2_char_t *tmp = NULL;
    axis2_char_t *tmp_content_type = NULL;
    axis2_char_t *tmp2 = NULL;

    AXIS2_PARAM_CHECK(env->error, content_type, NULL);
    AXIS2_PARAM_CHECK(env->error, key, NULL);

    tmp_content_type = axutil_strdup(env, content_type);

    if (!tmp_content_type)
    {
        return NULL;
    }

    tmp = strstr(tmp_content_type, key);

    if (!tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return NULL;
    }

    tmp = strchr(tmp, AXIS2_AMQP_EQ);
    tmp2 = strchr(tmp, AXIS2_AMQP_SEMI_COLON);

    if (tmp2)
    {
        *tmp2 = AXIS2_AMQP_ESC_NULL;
    }

    if (!tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return NULL;
    }

    tmp2 = axutil_strdup(env, tmp + 1);

    AXIS2_FREE(env->allocator, tmp_content_type);

    if (*tmp2 == AXIS2_AMQP_DOUBLE_QUOTE)
    {
        tmp = tmp2;
        tmp2 = axutil_strdup(env, tmp + 1);
        tmp2[strlen(tmp2) - 1] = AXIS2_AMQP_ESC_NULL;
        
	    if(tmp)
        {
            AXIS2_FREE(env->allocator, tmp); 
            tmp = NULL;
        }
    }

    /* handle XOP */
    if(*tmp2 == AXIS2_AMQP_B_SLASH && *(tmp2 + 1) == '\"')
    {
        tmp = tmp2;
        tmp2 = axutil_strdup(env, tmp + 2);
        tmp2[strlen(tmp2) - 3] = AXIS2_AMQP_ESC_NULL;

        if(tmp)
        {
            AXIS2_FREE(env->allocator, tmp);
            tmp = NULL;
        }
    }

    return tmp2;
}


AXIS2_EXTERN int AXIS2_CALL
axis2_amqp_util_on_data_request(
    char* buffer,
    int size,
    void* ctx)
{
    const axutil_env_t* env = NULL;
    int len = -1;
    axis2_callback_info_t* cb_ctx = (axis2_callback_info_t*)ctx;
    axutil_stream_t* in_stream = NULL;

    if (!buffer || !ctx)
    {
        return 0;
    }

	if (cb_ctx->unread_len <= 0 && -1 != cb_ctx->content_length)
    {
        return 0;
    }

	env = ((axis2_callback_info_t*)ctx)->env;

    in_stream = (axutil_stream_t*)((axis2_callback_info_t *) ctx)->in_stream;
    --size;                         /* reserve space to insert trailing null */

    len = axutil_stream_read(in_stream, env, buffer, size);

	if (len > 0)
	{
		buffer[len] = AXIS2_AMQP_ESC_NULL;
		((axis2_callback_info_t*)ctx)->unread_len -= len;
    }
    else if(len == 0)
    {
		((axis2_callback_info_t*)ctx)->unread_len = 0;
	}

    return len;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
axis2_amqp_util_conf_ctx_get_dual_channel_queue_name(
	axis2_conf_ctx_t* conf_ctx,
	const axutil_env_t* env)
{
	axutil_property_t* property = NULL;
	axis2_char_t* queue_name = NULL;
	axis2_char_t* value = NULL;

	/* Get property */
	property = axis2_conf_ctx_get_property(conf_ctx, env, 
			AXIS2_AMQP_CONF_CTX_PROPERTY_QUEUE_NAME);
	if (!property) /* Very first call */
	{
		property = axutil_property_create(env);
		
		axis2_conf_ctx_set_property(conf_ctx, env, 
				AXIS2_AMQP_CONF_CTX_PROPERTY_QUEUE_NAME, property);
	}

	/* Get queue name */
	value = (axis2_char_t*)axutil_property_get_value(property, env);
	
	/* AMQP listener and the sender are the two parties that are 
	 * interested in the queue. Either party can create the queue.
	 * If the queue is already created by one party, "value" is 
	 * not NULL. If "value" is NULL, that mean the caller of 
	 * this method is supposed to create the queue */
	if (value)
	{
		queue_name = (axis2_char_t*)
			AXIS2_MALLOC(env->allocator, axutil_strlen(value) + 1);
		strcpy(queue_name, value);

		/*axutil_property_set_value(property, env, NULL);*/
	}
	else
	{
		/* Create new queue name */
		queue_name = axutil_stracat(env, AXIS2_AMQP_TEMP_QUEUE_NAME_PREFIX, 
				axutil_uuid_gen(env));
		
		/* Put queue name in the conf_ctx so that the sender will know */
		axutil_property_set_value(property, env, (void*)queue_name);
	}

	return queue_name;
}


AXIS2_EXTERN axis2_char_t* AXIS2_CALL 
axis2_amqp_util_conf_ctx_get_qpid_broker_ip(
	axis2_conf_ctx_t* conf_ctx,
	const axutil_env_t* env)
{
	axutil_property_t* property = NULL;
	void* value = NULL;
	axis2_char_t* broker_ip = AXIS2_QPID_DEFAULT_BROKER_IP;

	property = axis2_conf_ctx_get_property(conf_ctx, env, 
			AXIS2_AMQP_CONF_CTX_PROPERTY_BROKER_IP);

	if (property)
	{
		value = axutil_property_get_value(property, env);

		if (value)
		{
			broker_ip = (axis2_char_t*)value;
		}
	}

	return broker_ip;
}


AXIS2_EXTERN int AXIS2_CALL 
axis2_amqp_util_conf_ctx_get_qpid_broker_port(
	axis2_conf_ctx_t* conf_ctx,
	const axutil_env_t* env)
{
	axutil_property_t* property = NULL;
	void* value = NULL;
	int broker_port = AXIS2_QPID_DEFAULT_BROKER_PORT;

	property = axis2_conf_ctx_get_property(conf_ctx, env, 
			AXIS2_AMQP_CONF_CTX_PROPERTY_BROKER_PORT);

	if (property)
	{
		value = axutil_property_get_value(property, env);

		if (value)
		{
			broker_port = *(int*)value;
		}
	}

	return broker_port;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
axis2_amqp_util_msg_ctx_get_use_separate_listener(
	axis2_msg_ctx_t* msg_ctx,
	const axutil_env_t* env)
{
	axutil_property_t* property = NULL;
	axis2_char_t* value = NULL;
	axis2_bool_t use_separate_listener = AXIS2_FALSE;

	property = axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_USE_SEPARATE_LISTENER);
	
	if (property)
	{
		value = (axis2_char_t*)axutil_property_get_value(property, env);
	
		if (value && (axutil_strcmp(AXIS2_VALUE_TRUE, value) == 0))
		{
			use_separate_listener = AXIS2_TRUE;
		}
	}
	
	return use_separate_listener;
}


AXIS2_EXTERN axis2_amqp_destination_info_t* AXIS2_CALL 
axis2_amqp_util_msg_ctx_get_destination_info(
	axis2_msg_ctx_t* msg_ctx,
	const axutil_env_t* env)
{
	/* The destination URI that is expected by this method 
	 * should be of one of the following formats 
	 * 1. amqp://IP:PORT/services/SERVICE_NAME 
	 * 2. jms:/SERVICE_NAME?java.naming.provider.url=tcp://IP:PORT...
	 * 3. TempQueue... */

	axis2_endpoint_ref_t* endpoint_ref = NULL;
	axis2_amqp_destination_info_t* destination_info = NULL;

	destination_info = (axis2_amqp_destination_info_t*)
		AXIS2_MALLOC(env->allocator, sizeof(axis2_amqp_destination_info_t));

	destination_info->broker_ip = NULL;
	destination_info->broker_port = AXIS2_QPID_NULL_CONF_INT;
	destination_info->queue_name = NULL;
	
	endpoint_ref = axis2_msg_ctx_get_to(msg_ctx, env);
	
	if (endpoint_ref)
	{
		const axis2_char_t* endpoint_address_original = NULL;
		axis2_char_t* endpoint_address = NULL;
		char* substr = NULL;
		char* token = NULL;
		endpoint_address_original = axis2_endpoint_ref_get_address(endpoint_ref, env);
	
		if (!endpoint_address_original)
			return NULL;

		endpoint_address = (axis2_char_t*)AXIS2_MALLOC(env->allocator, 
				(sizeof(axis2_char_t) * axutil_strlen(endpoint_address_original)) + 1);
		strcpy((char*)endpoint_address, (char*)endpoint_address_original);
		
		if ((substr = strstr(endpoint_address, AXIS2_AMQP_EPR_PREFIX))) /* Start with amqp: */
		{
			if (strstr(endpoint_address, AXIS2_AMQP_EPR_ANON_SERVICE_NAME))
			{
				/* Server reply to dual-channel client */
				axutil_property_t* property = NULL;
				property = axis2_msg_ctx_get_property(msg_ctx, env,
						AXIS2_AMQP_MSG_CTX_PROPERTY_REPLY_TO);

				if (property)
				{
					axis2_char_t* queue_name = (axis2_char_t*)
						axutil_property_get_value(property, env);

					if (queue_name)
					{
						destination_info->queue_name = (axis2_char_t*)AXIS2_MALLOC(
								env->allocator, (sizeof(axis2_char_t) * strlen(queue_name)) + 1);
						strcpy(destination_info->queue_name, queue_name);
					}
				}
			}
			else
			{
				substr+= strlen(AXIS2_AMQP_EPR_PREFIX) + 2; /* 2 -> "//" */
				if (substr) /* IP:PORT/services/SERVICE_NAME */
				{
					token = strtok(substr, ":");
					if (token) /* IP */
					{
						axis2_char_t* broker_ip = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
								(sizeof(axis2_char_t) * strlen(token)) + 1);
						strcpy(broker_ip, token);
						destination_info->broker_ip = broker_ip;

						token = strtok(NULL, "/"); /* PORT */
						if (token)
						{
							destination_info->broker_port = atoi(token);

							token = strtok(NULL, "#"); /* ... services/SERVICE_NAME */
							if (token)
							{
								if ((substr = strstr(token, AXIS2_AMQP_EPR_SERVICE_PREFIX)))
								{
									substr+= strlen(AXIS2_AMQP_EPR_SERVICE_PREFIX) + 1; /* 1 -> "/" */
									if (substr)
									{
										axis2_char_t* queue_name = (axis2_char_t*)AXIS2_MALLOC(env->allocator,
												(sizeof(axis2_char_t) * strlen(substr)) + 1);
										strcpy(queue_name, substr);
										destination_info->queue_name = queue_name;
									}
								}
							}
						}
					}
				}
			}
		}
		else if (0 == strcmp(endpoint_address, AXIS2_WSA_ANONYMOUS_URL)) /* Required to work with Sandesha2 */
		{
			axutil_property_t* property = NULL;
			property = axis2_msg_ctx_get_property(msg_ctx, env, 
					AXIS2_AMQP_MSG_CTX_PROPERTY_REPLY_TO);
		
			if (property)
			{
				axis2_char_t* queue_name = (axis2_char_t*)
					axutil_property_get_value(property, env);
			
				if (queue_name)
				{
					destination_info->queue_name = (axis2_char_t*)AXIS2_MALLOC(
							env->allocator, (sizeof(axis2_char_t) * strlen(queue_name)) + 1);
					strcpy(destination_info->queue_name, queue_name);
				}
			}
		}
		else if ((substr = strstr(endpoint_address, "jms:/")) &&
			 	 (substr == endpoint_address))
		{

		}
		
		AXIS2_FREE(env->allocator, endpoint_address);
	}
	else
	{
		/* Single-channel blocking */
		axutil_property_t* property = NULL;
		property = axis2_msg_ctx_get_property(msg_ctx, env, 
				AXIS2_AMQP_MSG_CTX_PROPERTY_REPLY_TO);
		
		if (property)
		{
			axis2_char_t* queue_name = (axis2_char_t*)
				axutil_property_get_value(property, env);
			
			if (queue_name)
			{
				destination_info->queue_name = (axis2_char_t*)AXIS2_MALLOC(
						env->allocator, (sizeof(axis2_char_t) * strlen(queue_name)) + 1);
				strcpy(destination_info->queue_name, queue_name);
			}
		}
	}

	/* Get broker IP/Port from conf_ctx if they are not 
	 * found in the destination URI */
	if (!destination_info->broker_ip)
	{
		axis2_conf_ctx_t* conf_ctx = NULL;

		conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
		if (conf_ctx)
		{
			axutil_property_t* property = NULL;
			property = axis2_conf_ctx_get_property(conf_ctx, env, 
					AXIS2_AMQP_CONF_CTX_PROPERTY_BROKER_IP);

			if (property)
			{
				axis2_char_t* broker_ip = (axis2_char_t*)
					axutil_property_get_value(property, env);

				if (broker_ip)
				{
					destination_info->broker_ip = (axis2_char_t*)AXIS2_MALLOC(
							env->allocator, (sizeof(axis2_char_t) * strlen(broker_ip)) + 1);
					strcpy(destination_info->broker_ip, broker_ip);
				}
			}

		}
	}

	if (AXIS2_QPID_NULL_CONF_INT == destination_info->broker_port)
	{
		axis2_conf_ctx_t* conf_ctx = NULL;

		conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
		if (conf_ctx)
		{
			axutil_property_t* property = NULL;
			property = axis2_conf_ctx_get_property(conf_ctx, env, 
					AXIS2_AMQP_CONF_CTX_PROPERTY_BROKER_PORT);

			if (property)
			{
				void* value = axutil_property_get_value(property, env);

				if (value)
				{
					destination_info->broker_port = *(int*)value;
				}
			}
		}
	}

	return destination_info;
}


AXIS2_EXTERN int AXIS2_CALL 
axis2_amqp_util_msg_ctx_get_request_timeout(
	axis2_msg_ctx_t* msg_ctx,
	const axutil_env_t* env)
{
	axis2_conf_ctx_t* conf_ctx = NULL;
	axutil_property_t* property = NULL;
	void* value = NULL;
	int request_timeout = AXIS2_QPID_DEFAULT_REQUEST_TIMEOUT;

	conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);

	if (conf_ctx)
	{
		property = axis2_conf_ctx_get_property(conf_ctx, env, 
				AXIS2_AMQP_CONF_CTX_PROPERTY_REQUEST_TIMEOUT);
		
		if (property)
		{
			value = axutil_property_get_value(property, env);
			
			if (value)
			{
				request_timeout = *(int*)value;
			}
		}
	}

	return request_timeout;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL 
axis2_amqp_util_msg_ctx_get_server_side(
	axis2_msg_ctx_t* msg_ctx,
	const axutil_env_t* env)
{
	axis2_conf_ctx_t* conf_ctx = NULL;
	axis2_bool_t is_server = AXIS2_FALSE;

	conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);

	if (conf_ctx)
	{
		is_server = 
			axis2_amqp_util_conf_ctx_get_server_side(
					conf_ctx, env);
	}

	return is_server;
}


AXIS2_EXTERN void AXIS2_CALL 
axis2_amqp_response_free(
	axis2_amqp_response_t* response,
	const axutil_env_t* env)
{
	if (response)
	{
		if (response->data)
		{
			AXIS2_FREE(env->allocator, response->data);
		}

		if (response->content_type)
		{
			AXIS2_FREE(env->allocator, response->content_type);
		}

		AXIS2_FREE(env->allocator, response);
	}
}


AXIS2_EXTERN void AXIS2_CALL 
axis2_amqp_destination_info_free(
	axis2_amqp_destination_info_t* destination_info,
	const axutil_env_t* env)
{
	if (destination_info)
	{
		if (destination_info->broker_ip)
		{
			AXIS2_FREE(env->allocator, destination_info->broker_ip);
		}

		if (destination_info->queue_name)
		{
			AXIS2_FREE(env->allocator, destination_info->queue_name);
		}
		
		AXIS2_FREE(env->allocator, destination_info);
	}
}

