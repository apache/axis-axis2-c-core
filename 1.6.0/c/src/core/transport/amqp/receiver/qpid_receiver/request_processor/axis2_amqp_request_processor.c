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

#include <signal.h>
#include <axiom.h>
#include <axiom_soap.h>
#include <axis2_engine.h>
#include <axiom_mime_parser.h>
#include <axutil_http_chunked_stream.h>
#include <axis2_amqp_defines.h>
#include <axis2_amqp_util.h>
#include <axis2_amqp_request_processor.h>

void* AXIS2_THREAD_FUNC
axis2_amqp_request_processor_thread_function(
	axutil_thread_t* thread,
	void* request_data)
{
	axis2_status_t status = AXIS2_FAILURE;
	axutil_env_t* env = NULL;
	axutil_env_t* thread_env = NULL;
	axis2_amqp_request_processor_resource_pack_t* request_resource_pack = NULL;

#ifndef WIN32
#ifdef AXIS2_SVR_MULTI_THREADED
	signal(SIGPIPE, SIG_IGN);
#endif
#endif

	request_resource_pack = (axis2_amqp_request_processor_resource_pack_t*)request_data;
	
	env = request_resource_pack->env;
	thread_env = axutil_init_thread_env(env);

	/* Process Request */
	status = axis2_amqp_process_request(thread_env, request_resource_pack);

	if (status == AXIS2_SUCCESS)
	{
		AXIS2_LOG_INFO(thread_env->log, "Request Processed Successfully");
	}
	else
	{
		 AXIS2_LOG_WARNING(thread_env->log, AXIS2_LOG_SI, "Error while Processing Request");
	}

	AXIS2_FREE(thread_env->allocator, request_resource_pack->request_content);
	AXIS2_FREE(thread_env->allocator, request_resource_pack->reply_to);
	AXIS2_FREE(thread_env->allocator, request_resource_pack->content_type);
	AXIS2_FREE(thread_env->allocator, request_resource_pack->soap_action);
	
	AXIS2_FREE(thread_env->allocator, request_resource_pack);

	if (thread_env)
	{
		thread_env = NULL;
	}

#ifdef AXIS2_SVR_MULTI_THREADED
	axutil_thread_pool_exit_thread(env->thread_pool, thread);
#endif

	return NULL;
}


axis2_status_t
axis2_amqp_process_request(
	const axutil_env_t* env,
	axis2_amqp_request_processor_resource_pack_t* request_resource_pack)
{
	axiom_xml_reader_t* xml_reader = NULL;
	axiom_stax_builder_t* stax_builder = NULL;
	axiom_soap_builder_t* soap_builder = NULL;
	axis2_transport_out_desc_t* out_desc = NULL;
	axis2_transport_in_desc_t* in_desc = NULL;
	axis2_msg_ctx_t* msg_ctx = NULL;
	axiom_soap_envelope_t* soap_envelope = NULL;
	axis2_engine_t* engine = NULL;
	const axis2_char_t* soap_ns_uri = NULL;
	axis2_bool_t is_soap_11 = AXIS2_FALSE;
	axis2_char_t *soap_body_str = NULL;
	int soap_body_len = 0;
	axis2_bool_t is_mtom = AXIS2_FALSE;
	axis2_status_t status = AXIS2_FAILURE;
	axutil_hash_t *binary_data_map = NULL;
	axiom_soap_body_t *soap_body = NULL;
	axutil_property_t* reply_to_property = NULL;

	/* Create msg_ctx */
	if (!request_resource_pack->conf_ctx)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Conf Context not Available");
		return AXIS2_FAILURE;
	}

	out_desc = axis2_conf_get_transport_out(
			axis2_conf_ctx_get_conf(request_resource_pack->conf_ctx, env),
			env, AXIS2_TRANSPORT_ENUM_AMQP);
	if (!out_desc)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Transport Out Descriptor not Found");
		return AXIS2_FAILURE;
	}

	in_desc = axis2_conf_get_transport_in(
			axis2_conf_ctx_get_conf(request_resource_pack->conf_ctx, env),
			env, AXIS2_TRANSPORT_ENUM_AMQP);
	if (!in_desc)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Transport In Descriptor not Found");
		return AXIS2_FAILURE;
	}

	/* Create msg_ctx */
	msg_ctx = axis2_msg_ctx_create(env, request_resource_pack->conf_ctx, in_desc, out_desc);
	
	axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);
	
	/* Handle MTOM */
	if (strstr(request_resource_pack->content_type, AXIS2_AMQP_HEADER_ACCEPT_MULTIPART_RELATED))
	{
		axis2_char_t* mime_boundary = 
			axis2_amqp_util_get_value_from_content_type(env,
					request_resource_pack->content_type,
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
            
			buffer_size_param = axis2_msg_ctx_get_parameter(msg_ctx, env, AXIS2_MTOM_BUFFER_SIZE);
            if (buffer_size_param)
            {
                value_size = (axis2_char_t*)axutil_param_get_value(buffer_size_param, env);
                if (value_size)
                {
                    size = atoi(value_size);
                    axiom_mime_parser_set_buffer_size(mime_parser, env, size);
                }
            }
            
            max_buffers_param = axis2_msg_ctx_get_parameter(msg_ctx, env, AXIS2_MTOM_MAX_BUFFERS);
            if (max_buffers_param)
            {
                value_num = (axis2_char_t*)axutil_param_get_value(max_buffers_param, env);
                if (value_num)
                {
                    num = atoi(value_num);
                    axiom_mime_parser_set_max_buffers(mime_parser, env, num);
                }
            }

            /* If this paramter is there mime_parser will cached the attachment 
             * using to the directory for large attachments. */    
            attachment_dir_param = axis2_msg_ctx_get_parameter(msg_ctx, env, AXIS2_ATTACHMENT_DIR);
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
				axis2_callback_info_t *callback_ctx = NULL;
				axutil_stream_t *stream = NULL;

				callback_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t));

            	stream = axutil_stream_create_basic(env);
            	if (stream)
            	{
					axutil_stream_write(stream, env, request_resource_pack->request_content, 
							request_resource_pack->content_length);
                	callback_ctx->env = env;
                	callback_ctx->in_stream = stream;
                	callback_ctx->content_length = request_resource_pack->content_length;
                	callback_ctx->unread_len = request_resource_pack->content_length;
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
                
                soap_body_str = axiom_mime_parser_get_soap_body_str(mime_parser, env);
				soap_body_len = axiom_mime_parser_get_soap_body_len(mime_parser, env);

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
		soap_body_str = request_resource_pack->request_content;
		soap_body_len = request_resource_pack->content_length;
	}

	soap_body_len = axutil_strlen(soap_body_str);

	xml_reader = axiom_xml_reader_create_for_memory(env, soap_body_str, soap_body_len,
			NULL, AXIS2_XML_PARSER_TYPE_BUFFER);
	if (!xml_reader)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create XML Reader");
		return AXIS2_FAILURE;
	}
	
	stax_builder = axiom_stax_builder_create(env, xml_reader);
	if (!stax_builder)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create StAX Builder");
		return AXIS2_FAILURE;
	}

	soap_ns_uri = AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;

	if (request_resource_pack->content_type)
	{
		if (strstr(request_resource_pack->content_type, AXIS2_AMQP_HEADER_ACCEPT_TEXT_XML))
		{
			is_soap_11 = AXIS2_TRUE;
			soap_ns_uri = AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
		}
		/*if (strstr(request_resource_pack->content_type, AXIS2_AMQP_HEADER_ACCEPT_APPL_SOAP))
		{
			is_soap_11 = AXIS2_FALSE;
			soap_ns_uri = AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
		}
		else if (strstr(request_resource_pack->content_type, AXIS2_AMQP_HEADER_ACCEPT_TEXT_XML))
		{
			is_soap_11 = AXIS2_TRUE;
			soap_ns_uri = AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI;
		}*/
	}

	soap_builder = axiom_soap_builder_create(env, stax_builder, soap_ns_uri);
	if (!soap_builder)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to Create SOAP Builder");
		return AXIS2_FAILURE;
	}
	
	if (binary_data_map)
	{
		axiom_soap_builder_set_mime_body_parts(soap_builder, env, binary_data_map);
	}

	soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);
	axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);

	soap_body = axiom_soap_envelope_get_body(soap_envelope, env);
	
	if (!soap_body)
	{
		return AXIS2_FAILURE;
	}

	/* SOAPAction */
	if (request_resource_pack->soap_action)
	{
		axis2_msg_ctx_set_soap_action(msg_ctx, env, 
				axutil_string_create(env, request_resource_pack->soap_action));
	}

	/* SOAP version */
	axis2_msg_ctx_set_is_soap_11(msg_ctx, env, is_soap_11);

	/* Set ReplyTo in the msg_ctx as a property. This is used by the server when
	 * 1. WS-A is not in use 
	 * 2. ReplyTo is an anonymous EPR - Sandesha2/Dual-channel */
	reply_to_property = axutil_property_create_with_args(env, AXIS2_SCOPE_REQUEST, 0, 0,
			(void*)request_resource_pack->reply_to);
	axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_AMQP_MSG_CTX_PROPERTY_REPLY_TO, 
			reply_to_property);

	engine = axis2_engine_create(env, request_resource_pack->conf_ctx);

	if (AXIS2_TRUE == axiom_soap_body_has_fault(soap_body, env))
	{
		status = axis2_engine_receive_fault(engine, env, msg_ctx);
	}
	else
	{
		status = axis2_engine_receive(engine, env, msg_ctx);
	}

	if (engine)
	{
		axis2_engine_free(engine, env);
	}

	if (soap_body_str && is_mtom)
	{
		AXIS2_FREE(env->allocator, soap_body_str);
	}

	return status;
}
