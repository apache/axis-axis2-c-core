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

#include <axis2_http_sender.h>
#include <axis2_soap_over_http_sender.h>
#include <axis2_rest_sender.h>
#include <axis2_string.h>
#include <axis2_http_transport.h>
#include <string.h>
#include <axiom_output.h>
#include <axis2_op_ctx.h>
#include <axis2_ctx.h>
#include <axis2_conf_ctx.h>
#include <axis2_http_client.h>
#include <axiom_xml_writer.h>
#include <axis2_property.h>
#include <axis2_param.h>
#include <axis2_types.h>
#include <axis2_generic_obj.h>
#include <axis2_const.h>
#include <axis2_util.h>
#include <axiom_soap.h>
#include "axis2_http_sender_util.h"

#ifdef AXIS2_LIBCURL_ENABLED
#include "libcurl/axis2_libcurl.h"
#endif

struct axis2_http_sender
{
    axis2_char_t *http_version;
    axis2_bool_t chunked;
    int so_timeout;
    int connection_timeout;
    axiom_output_t *om_output;
    axis2_http_client_t *client;
	axis2_bool_t is_soap;
};

static axis2_char_t *
axis2_url_encode (
	const axis2_env_t *env, 
	axis2_char_t *dest, 
	axis2_char_t *buff, 
	int len);

static int 
is_safe_or_unreserve (
	char c);

static axis2_status_t
axis2_http_sender_configure_proxy(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

static axis2_status_t 
axis2_http_sender_configure_server_cert(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

static axis2_status_t
axis2_http_sender_configure_key_file(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

AXIS2_EXTERN axis2_http_sender_t *AXIS2_CALL
axis2_http_sender_create(
    const axis2_env_t *env)
{
    axis2_http_sender_t *sender = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    sender = (axis2_http_sender_t *)AXIS2_MALLOC
		(env->allocator, sizeof(
			axis2_http_sender_t));

    if (! sender)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    sender->http_version = (axis2_char_t *)AXIS2_HTTP_HEADER_PROTOCOL_11;
    sender->so_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;
    sender->connection_timeout = AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT;
    /* unlike the java impl we don't have a default om output
     * it should be explicitly set and it's a MUST
     */
    sender->om_output = NULL;
    sender->chunked = AXIS2_FALSE;
    sender->client = NULL;
    
    return sender;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_free(
    axis2_http_sender_t *sender,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    if (sender->http_version)
    {
        AXIS2_FREE(env->allocator, sender->http_version);
    }

    /* Do not free this here since it will be required in later processing
     * of the response soap message
     */
    sender->client = NULL;
    AXIS2_FREE(env->allocator, sender);
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_send(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
	axiom_soap_envelope_t *out,
    const axis2_char_t *str_url,
    const axis2_char_t *soap_action)
{
#ifdef AXIS2_LIBCURL_ENABLED
    axis2_libcurl_http_send(sender, env, msg_ctx, output, url,soap_action);
#else

    axis2_http_simple_request_t *request = NULL;
    axis2_http_request_line_t *request_line = NULL;
    axis2_url_t *url = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axis2_char_t *buffer = NULL;
    unsigned int buffer_size = 0;
    const axis2_char_t *char_set_enc = NULL;
    axis2_string_t *char_set_enc_str = NULL;
    int status_code = -1;
	axis2_http_simple_response_t *response = NULL;
    axis2_char_t *content_type = NULL;
    axis2_byte_t *output_stream = NULL;
    int output_stream_size = 0;
    axis2_bool_t doing_mtom = AXIS2_FALSE;
    axis2_property_t *dump_property = NULL;
    axis2_param_t *ssl_pp_param = NULL; /* ssl passphrase */
    axis2_char_t *ssl_pp = NULL;
	axis2_property_t *content_type_property = NULL;
	axis2_hash_t *content_type_hash = NULL;
	axis2_char_t *content_type_value = NULL;
	axis2_property_t *method = NULL;
	axis2_char_t *method_value = NULL;
	axis2_bool_t send_via_get = AXIS2_FALSE;
	axiom_node_t *data_out = NULL;
	axiom_node_t *body_node = NULL;
	axiom_soap_body_t *soap_body = NULL;
	axis2_bool_t is_soap = AXIS2_TRUE;
	soap_body = axiom_soap_envelope_get_body(out, env);

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, str_url, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, soap_action, AXIS2_FAILURE);

    if (AXIS2_TRUE ==  axis2_msg_ctx_get_doing_rest(msg_ctx, env))
		is_soap = AXIS2_FALSE;
	else
		is_soap = AXIS2_TRUE;

    url = axis2_url_parse_string(env, str_url);

	if (!is_soap)
	{
        if (! soap_body)
        {
            AXIS2_ERROR_SET(env->error,
							AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
							AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s",
							AXIS2_ERROR_GET_MESSAGE(env->error));
            return AXIS2_FAILURE;
        }
        body_node = axiom_soap_body_get_base_node(soap_body, env);
        if (! body_node)
        {
            return AXIS2_FAILURE;
        }
        data_out = AXIOM_NODE_GET_FIRST_ELEMENT(body_node, env);

		method = (axis2_property_t *) axis2_msg_ctx_get_property(msg_ctx, env,
            AXIS2_HTTP_METHOD);
		if (method)
			method_value = (axis2_char_t *) axis2_property_get_value (method, env);

		/* The default is POST */
		if (method_value && 0 == axis2_strcmp(method_value, AXIS2_HTTP_HEADER_GET))
		{
			send_via_get = AXIS2_TRUE;
		}
	}

    if (! url)
    {
        return AXIS2_FAILURE;
    }

    if (sender->client)
    {
        AXIS2_HTTP_CLIENT_FREE(sender->client, env);
        sender->client = NULL;
    }

    sender->client = axis2_http_client_create(env, url);
    if (! sender->client)
    {
        return AXIS2_FAILURE;
    }
	
	/* configure proxy settings if we have set so
	 */
	
	axis2_http_sender_configure_proxy(sender, env, msg_ctx);

	if (!send_via_get)
	{

		/* We put the client into msg_ctx so that we can free it once the processing
		 * is done at client side
		 */
		/*property = axis2_property_create(env);
		  axis2_property_set_scope(property, env, AXIS2_SCOPE_REQUEST);
		  axis2_property_set_free_func(property, env,
		  axis2_http_client_free_void_arg);
		  axis2_property_set_value(property, env, sender->client);
		   axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_HTTP_CLIENT,
		  property);*/

		doing_mtom =  axis2_msg_ctx_get_doing_mtom(msg_ctx, env);

		if (! sender->om_output)
		{
			AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_OM_OUTPUT,
							AXIS2_FAILURE);
			return AXIS2_FAILURE;
		}
		xml_writer = axiom_output_get_xml_writer(sender->om_output, env);
    
		char_set_enc_str = axis2_msg_ctx_get_charset_encoding(msg_ctx, env);
		if (!char_set_enc_str)
		{
			char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
		}
		else
		{
			char_set_enc = axis2_string_get_buffer(char_set_enc_str, env);
		}

		if (is_soap)
		{
			dump_property =  axis2_msg_ctx_get_property(msg_ctx, env, 
                AXIS2_DUMP_INPUT_MSG_TRUE);
			if(dump_property)
			{
				axis2_char_t *dump_true = axis2_property_get_value(dump_property, env);
				if(0 == axis2_strcmp(dump_true, AXIS2_VALUE_TRUE))
				{
					AXIS2_HTTP_CLIENT_SET_DUMP_INPUT_MSG(sender->client, env, AXIS2_TRUE);
				}
			}

			axiom_output_set_do_optimize(sender->om_output, env,
										 doing_mtom); 
			axiom_soap_envelope_serialize(out, env, sender->om_output,
										  AXIS2_FALSE);
		}
		else
		{
			AXIOM_NODE_SERIALIZE(data_out, env, sender->om_output);
		}


		if (doing_mtom)
		{
			axiom_output_flush(sender->om_output, env, &output_stream,
							   &output_stream_size);
		}
		else
		{
			buffer = AXIOM_XML_WRITER_GET_XML(xml_writer, env);
		}

		if (! buffer && !doing_mtom)
		{
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "NULL xml returned"
							"from xml writer");
			return AXIS2_FAILURE;
		}

		request_line = axis2_http_request_line_create(env, "POST",
													  axis2_url_get_path(url, env),
													  sender->http_version);
	}
	else
	{
		axis2_char_t *request_params = NULL;
        axis2_char_t *path = NULL;

        request_params = axis2_http_sender_get_param_string( sender, env, msg_ctx);
        path = axis2_strcat(env, axis2_url_get_path(url, env), "?",
							request_params, NULL);
        request_line = axis2_http_request_line_create(env, "GET", path,
													  sender->http_version);
	}

    request = axis2_http_simple_request_create(env, request_line, NULL, 0,
											   NULL);

	axis2_http_sender_util_add_header (env, request, AXIS2_HTTP_HEADER_USER_AGENT, "Axis2/C");
    
	if (AXIS2_TRUE ==  axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
    {
		if ('\"' != *soap_action)
        {
            axis2_char_t *tmp_soap_action = NULL;
            tmp_soap_action = AXIS2_MALLOC(env->allocator, (axis2_strlen(soap_action) + 5) * sizeof(axis2_char_t));
            sprintf(tmp_soap_action, "\"%s\"", soap_action);
			axis2_http_sender_util_add_header (env, request, AXIS2_HTTP_HEADER_SOAP_ACTION, tmp_soap_action);
            AXIS2_FREE(env->allocator, tmp_soap_action);
        }
        else
        {
			axis2_http_sender_util_add_header (env, request,
                                               AXIS2_HTTP_HEADER_SOAP_ACTION, 
                                               (const axis2_char_t *)soap_action);
        }
    }

	if (!send_via_get)
	{
		buffer_size = AXIOM_XML_WRITER_GET_XML_SIZE(xml_writer, env);

		if (AXIS2_FALSE == sender->chunked)
		{
			axis2_char_t tmp_buf[10];
			if (!buffer)
			{
				buffer_size = output_stream_size;
			}

			if (buffer_size)
			{
				sprintf(tmp_buf, "%d", buffer_size);
				axis2_http_sender_util_add_header (env, request, AXIS2_HTTP_HEADER_CONTENT_LENGTH, tmp_buf);
			}
		}
		else
		{
			axis2_http_sender_util_add_header (env, request, AXIS2_HTTP_HEADER_TRANSFER_ENCODING, 
					AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED);
		}
    
		if (is_soap)
		{
			if (doing_mtom)
			{
				content_type = (axis2_char_t *)axiom_output_get_content_type(sender->om_output,
																			 env);
				if (AXIS2_TRUE !=  axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
				{
					/* handle SOAP action for SOAP 1.2 case */
					if (axis2_strcmp(soap_action, ""))
					{
						axis2_char_t *temp_content_type = NULL;
						temp_content_type = axis2_stracat(content_type, ";action=", env);
						AXIS2_FREE(env->allocator, content_type);
						content_type = temp_content_type;
						temp_content_type = axis2_stracat(content_type, soap_action, env);
						AXIS2_FREE(env->allocator, content_type);
						content_type = temp_content_type;
					}
				}
			}
			else if (AXIS2_TRUE ==  axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
			{
				axis2_char_t *temp_content_type = NULL;
				content_type = (axis2_char_t *)AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
				content_type = axis2_stracat(content_type, ";charset=", env);
				temp_content_type = axis2_stracat(content_type, char_set_enc, env);
				AXIS2_FREE(env->allocator, content_type);
				content_type = temp_content_type;
			}
			else
			{
				axis2_char_t *temp_content_type = NULL;
				content_type = (axis2_char_t *)AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP;
				content_type = axis2_stracat(content_type, ";charset=", env);
				temp_content_type = axis2_stracat(content_type, char_set_enc, env);
				AXIS2_FREE(env->allocator, content_type);
				content_type = temp_content_type;
				if (axis2_strcmp(soap_action, ""))
				{
					temp_content_type = axis2_stracat(content_type, ";action=", env);
					AXIS2_FREE(env->allocator, content_type);
					content_type = temp_content_type;
					temp_content_type = axis2_stracat(content_type, soap_action, env);
					AXIS2_FREE(env->allocator, content_type);
					content_type = temp_content_type;
				}
				temp_content_type = axis2_stracat(content_type, ";", env);
				AXIS2_FREE(env->allocator, content_type);
				content_type = temp_content_type;
			}
		}
		else
		{
			content_type_property  = (axis2_property_t *) axis2_msg_ctx_get_property(
				msg_ctx, env,
				AXIS2_USER_DEFINED_HTTP_HEADER_CONTENT_TYPE);
		
			if (content_type_property)
			{
				content_type_hash = (axis2_hash_t *) axis2_property_get_value (content_type_property, env);
				if (content_type_hash)
					content_type_value = (char *) axis2_hash_get (content_type_hash, 
																  AXIS2_HTTP_HEADER_CONTENT_TYPE, 
																  AXIS2_HASH_KEY_STRING);
			}
			if (content_type_value)
				content_type = content_type_value;
			else
				content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
		}

		axis2_http_sender_util_add_header (env, request, AXIS2_HTTP_HEADER_CONTENT_TYPE, content_type);

		/*if (content_type)
		{
			AXIS2_FREE(env->allocator, content_type);
			content_type = NULL;
		}*/
	}

    if (0 == axis2_strcmp(sender->http_version,
						  AXIS2_HTTP_HEADER_PROTOCOL_11))
    {
        axis2_char_t *header = NULL;
        header = AXIS2_MALLOC(env->allocator, axis2_strlen(
								  axis2_url_get_server(url, env)) + 10 * sizeof(
									  axis2_char_t));
        sprintf(header, "%s:%d", axis2_url_get_server(url, env),
                axis2_url_get_port(url, env));
		axis2_http_sender_util_add_header (env, request, AXIS2_HTTP_HEADER_HOST, header);
        AXIS2_FREE(env->allocator, header);
        header = NULL;
    }

    if (doing_mtom)
    {
        axis2_stream_t *stream = AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY(request, env);
        if (stream)
        {
            AXIS2_STREAM_WRITE(stream, env, output_stream, output_stream_size);
        }
    }
    else
    {
        AXIS2_HTTP_SIMPLE_REQUEST_SET_BODY_STRING(request, env, buffer, buffer_size);
    }

	axis2_http_sender_configure_server_cert(sender, env, msg_ctx);
	
    axis2_http_sender_configure_key_file(sender, env, msg_ctx);

    axis2_http_sender_get_timeout_values(sender, env, msg_ctx);
    AXIS2_HTTP_CLIENT_SET_TIMEOUT(sender->client, env,
								  sender->so_timeout);
    
    ssl_pp_param =  axis2_msg_ctx_get_parameter(msg_ctx, env, AXIS2_SSL_PASSPHRASE);

    if (ssl_pp_param)
    {
        ssl_pp = axis2_param_get_value(ssl_pp_param, env);
    }

    status_code = AXIS2_HTTP_CLIENT_SEND(sender->client, env, request, ssl_pp);

    /*AXIS2_FREE(env->allocator, buffer);
	  buffer = NULL;*/

    AXIS2_HTTP_SIMPLE_REQUEST_FREE(request, env);
    request = NULL;
    
    AXIS2_FREE(env->allocator, output_stream);
    output_stream = NULL;

    status_code = AXIS2_HTTP_CLIENT_RECIEVE_HEADER(sender->client, env);

    if (status_code < 0)
    {
        return AXIS2_FAILURE;
    }
    response = AXIS2_HTTP_CLIENT_GET_RESPONSE(sender->client, env);
    if (AXIS2_HTTP_RESPONSE_OK_CODE_VAL == status_code ||
		AXIS2_HTTP_RESPONSE_ACK_CODE_VAL == status_code)
    {
        return axis2_http_sender_process_response(sender, env,
												  msg_ctx, response);
    }
    else if (AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL == status_code)
    {
        axis2_http_header_t *tmp_header = NULL;
        axis2_char_t *tmp_header_val = NULL;
        axis2_op_t *op = NULL;

        op =  axis2_msg_ctx_get_op(msg_ctx, env);
        if (op)
        {
            const axis2_char_t *mep = axis2_op_get_msg_exchange_pattern(op, env);
            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
							AXIS2_FAILURE);
            /* handle one way case */
            
            if (axis2_strcmp(mep, AXIS2_MEP_URI_OUT_ONLY) == 0 ||
                axis2_strcmp(mep, AXIS2_MEP_URI_ROBUST_OUT_ONLY) == 0)
            {
                return AXIS2_FAILURE;
            }
        }
            
        /* set an error to indicate error code status */
        tmp_header = AXIS2_HTTP_SIMPLE_RESPONSE_GET_FIRST_HEADER(response, env,
																 AXIS2_HTTP_HEADER_CONTENT_TYPE);
        if (tmp_header)
        {
            tmp_header_val = AXIS2_HTTP_HEADER_GET_VALUE(tmp_header, env);
        }
        
        
        if (tmp_header_val && (axis2_strstr(tmp_header_val,
											AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP) || axis2_strstr(
												tmp_header_val, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML)))
        {
            return axis2_http_sender_process_response(sender, env,
													  msg_ctx, response);
        }
    }
    AXIS2_ERROR_SET(env->error, AXIS2_ERROR_HTTP_CLIENT_TRANSPORT_ERROR,
					AXIS2_FAILURE);
    return AXIS2_FAILURE;
#endif
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_set_chunked(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_bool_t chunked)
{
    sender->chunked = chunked;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_set_om_output(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axiom_output_t *om_output)
{
    sender->om_output = om_output;
    return AXIS2_SUCCESS;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_get_header_info(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_http_simple_response_t *response)
{
    axis2_array_list_t *headers = NULL;
    axis2_char_t *charset = NULL;
    int i = 0;
    axis2_bool_t response_chunked = AXIS2_FALSE;
    int *content_length = NULL;
    axis2_property_t *property = NULL;
    axis2_char_t *content_type = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, response, AXIS2_FAILURE);

    /*
     * TODO MTOM support (MIME header)
     */
    headers = AXIS2_HTTP_SIMPLE_RESPONSE_GET_HEADERS(response, env);
    if (headers == NULL)
    {
        return AXIS2_SUCCESS;
    }
    for (i = 0; i < axis2_array_list_size(headers, env); i++)
    {
        axis2_http_header_t *header = axis2_array_list_get(headers, env, i);
        axis2_char_t *name = AXIS2_HTTP_HEADER_GET_NAME((axis2_http_header_t *)
														header, env);
        if (name)
        {
            if (0 == axis2_strcmp(name, AXIS2_HTTP_HEADER_TRANSFER_ENCODING) &&
				0 == axis2_strcmp(AXIS2_HTTP_HEADER_GET_VALUE(header
															  , env), AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                axis2_char_t *transfer_encoding = NULL;
/*                transfer_encoding = */
/*                     axis2_strdup(AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED, */
/* 								 env); */
/*                 property = axis2_property_create(env); */
/*                 axis2_property_set_scope(property, env, AXIS2_SCOPE_REQUEST); */
/*                 axis2_property_set_value(property, env, transfer_encoding); */
/*                  axis2_msg_ctx_set_property(msg_ctx, env, */
/* 										   AXIS2_HTTP_HEADER_TRANSFER_ENCODING, */
/* 										   property); */
/*                 response_chunked = AXIS2_TRUE; */
                transfer_encoding =
                    axis2_strdup(AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED,
								 env);
                response_chunked = AXIS2_TRUE;
                axis2_msg_ctx_set_transfer_encoding(msg_ctx, env, transfer_encoding);
 
            }
            if (0 != axis2_strcmp(name, AXIS2_HTTP_HEADER_CONTENT_TYPE))
            {
                axis2_char_t *tmp_charset = NULL;
                axis2_char_t *content_type = AXIS2_HTTP_HEADER_GET_VALUE(header,
																		 env);
                tmp_charset = strstr(content_type, AXIS2_HTTP_CHAR_SET_ENCODING);
                if (charset)
                {
                    charset = axis2_strdup(tmp_charset, env);
                    break;
                }
            }
        }
    }
    content_type = (axis2_char_t *)AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_TYPE(
		response, env);
    if (content_type)
    {
        if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED)
			&& strstr(content_type, AXIS2_HTTP_HEADER_XOP_XML))
        {
            axis2_ctx_t *axis_ctx = axis2_op_ctx_get_base(
				 axis2_msg_ctx_get_op_ctx(msg_ctx, env), env);
            property = axis2_property_create(env);
            axis2_property_set_scope(property, env, AXIS2_SCOPE_REQUEST);
            axis2_property_set_value(property, env, axis2_strdup(content_type,
																 env));
            axis2_ctx_set_property(axis_ctx, env, MTOM_RECIVED_CONTENT_TYPE,
								   property);
        }
    }
    if (charset)
    {
        axis2_ctx_t *axis_ctx = axis2_op_ctx_get_base( axis2_msg_ctx_get_op_ctx(
														  msg_ctx, env), env);
        if (axis_ctx)
        {
            property = axis2_property_create(env);
            axis2_property_set_scope(property, env, AXIS2_SCOPE_REQUEST);
            axis2_property_set_value(property, env, charset);
            axis2_ctx_set_property(axis_ctx, env, AXIS2_CHARACTER_SET_ENCODING,
								   property);
        }
    }
    if (AXIS2_FALSE == response_chunked)
    {
        int tmp_len = 0;
        content_length = AXIS2_MALLOC(env->allocator, sizeof(int));
        if (! content_length)
        {
            return AXIS2_FAILURE;
        }
        tmp_len = AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_LENGTH(response, env);
        memcpy(content_length, &tmp_len, sizeof(int));
        property = axis2_property_create(env);
        axis2_property_set_scope(property, env, AXIS2_SCOPE_REQUEST);
        axis2_property_set_value(property, env, content_length);
         axis2_msg_ctx_set_property(msg_ctx, env,
								   AXIS2_HTTP_HEADER_CONTENT_LENGTH, property);
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_process_response(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_http_simple_response_t *response)
{
    axis2_stream_t *in_stream = NULL;
    axis2_property_t *property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, response, AXIS2_FAILURE);

    in_stream = AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY(response, env);
    if (! in_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY,
						AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    axis2_http_sender_get_header_info(sender, env, msg_ctx, response);
    /*axis_ctx = axis2_op_ctx_get_base( axis2_msg_ctx_get_op_ctx(msg_ctx, env),
	  env);*/
    property = axis2_property_create(env);
    axis2_property_set_scope(property, env, AXIS2_SCOPE_REQUEST);
    axis2_property_set_free_func(property, env, axis2_stream_free_void_arg);
    axis2_property_set_value(property, env, in_stream);
    /*axis2_ctx_set_property(axis_ctx, env, AXIS2_TRANSPORT_IN, property);*/
    axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_TRANSPORT_IN, property);
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_get_timeout_values(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_char_t *so_str = NULL;
    axis2_char_t *connection_str = NULL;
    axis2_param_t *tmp_param = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);


    tmp_param =  axis2_msg_ctx_get_parameter(msg_ctx,
											env, AXIS2_HTTP_SO_TIMEOUT);

    if (tmp_param)
    {
        so_str = (axis2_char_t *)axis2_param_get_value(tmp_param, env);
        if (so_str)
        {
            sender->so_timeout = AXIS2_ATOI(so_str);
        }
    }
    tmp_param =  axis2_msg_ctx_get_parameter(msg_ctx, env,
											AXIS2_HTTP_CONNECTION_TIMEOUT);
    if (tmp_param)
    {
        connection_str = (axis2_char_t *)axis2_param_get_value(tmp_param, env);
        if (connection_str)
        {
            sender->connection_timeout =
                AXIS2_ATOI(connection_str);
        }
    }
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_sender_set_http_version(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_char_t *version)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    sender->http_version =  axis2_strdup(version, env);
    if (! sender->http_version)
    {
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}

static axis2_status_t
axis2_http_sender_configure_proxy(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_transport_out_desc_t *trans_desc = NULL;
    axis2_param_t *proxy_param = NULL;
    axis2_hash_t *transport_attrs = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    conf_ctx =  axis2_msg_ctx_get_conf_ctx(msg_ctx, env);
    if (! conf_ctx)
    {
        return AXIS2_FAILURE;
    }
    conf =  axis2_conf_ctx_get_conf(conf_ctx, env);
    if (! conf)
    {
        return AXIS2_FAILURE;
    }
    
    trans_desc =  axis2_conf_get_transport_out(conf, env, AXIS2_TRANSPORT_ENUM_HTTP);
    if (! trans_desc)
    {
        return AXIS2_FAILURE;
    }

    proxy_param = axis2_param_container_get_param(
        axis2_transport_out_desc_param_container(trans_desc, env), 
	    env, AXIS2_HTTP_PROXY);
    if (proxy_param)
    {
        transport_attrs = axis2_param_get_attributes(proxy_param, env);
        if (transport_attrs)
        {
            axis2_generic_obj_t *obj = NULL;
            axiom_attribute_t *host_attr = NULL;
            axiom_attribute_t *port_attr = NULL;
            axis2_char_t *proxy_host = NULL;
            axis2_char_t *proxy_port = NULL;

            obj = axis2_hash_get(transport_attrs, AXIS2_PROXY_HOST_NAME,
								 AXIS2_HASH_KEY_STRING);
            if (! obj)
            {
                return AXIS2_FAILURE;
            }
            host_attr = (axiom_attribute_t *) axis2_generic_obj_get_value(obj,
																		 env);
            if (! host_attr)
            {
                return AXIS2_FAILURE;
            }
            proxy_host = axiom_attribute_get_value(host_attr, env);
            if (! proxy_host)
            {
                return AXIS2_FAILURE;
            }
            /* Now we get the port */
            obj = NULL;

            obj = axis2_hash_get(transport_attrs, AXIS2_PROXY_HOST_PORT,
								 AXIS2_HASH_KEY_STRING);
            port_attr = (axiom_attribute_t*) axis2_generic_obj_get_value(obj,
																		env);
            if (! port_attr)
            {
                return AXIS2_FAILURE;
            }
            proxy_port = axiom_attribute_get_value(port_attr, env);
            if (! proxy_port)
            {
                return AXIS2_FAILURE;
            }
            if (proxy_host && NULL != proxy_port)
            {
                AXIS2_HTTP_CLIENT_SET_PROXY(sender->client, env, proxy_host,
											AXIS2_ATOI(proxy_port));
                return AXIS2_SUCCESS;
            }
        }
    }
    return AXIS2_SUCCESS;
}

static axis2_status_t 
axis2_http_sender_configure_server_cert(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
	axis2_property_t *server_cert_property = NULL;
	axis2_param_t *server_cert_param = NULL;
	axis2_char_t *server_cert = NULL;
	axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    server_cert_property =  axis2_msg_ctx_get_property(msg_ctx, env, 
        AXIS2_SSL_SERVER_CERT);
    if(server_cert_property)
    {
		server_cert = (axis2_char_t *) axis2_property_get_value(
			server_cert_property, env);
    }
    else
    {
        server_cert_param =  axis2_msg_ctx_get_parameter(msg_ctx, env, 
														AXIS2_SSL_SERVER_CERT);
        if(server_cert_param)
        {
            server_cert = (axis2_char_t *) axis2_param_get_value(
				server_cert_param, env);
        }
    }
    
    if(server_cert)
    {
        status = AXIS2_HTTP_CLIENT_SET_SERVER_CERT(sender->client, 
												   env, server_cert);
    }

    return status;
}

static axis2_status_t
axis2_http_sender_configure_key_file(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
	axis2_property_t *key_file_property = NULL;
	axis2_param_t *key_file_param = NULL;
    axis2_char_t *key_file = NULL;
	axis2_status_t status = AXIS2_FAILURE;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    key_file_property =  axis2_msg_ctx_get_property(msg_ctx, env, 
        AXIS2_SSL_KEY_FILE);
    if(key_file_property)
    {
		key_file = (axis2_char_t *) axis2_property_get_value(
			key_file_property, env);
    }
    else
    {
        key_file_param =  axis2_msg_ctx_get_parameter(msg_ctx, env,
													 AXIS2_SSL_KEY_FILE);
        if(key_file_param)
        {
            key_file = (axis2_char_t *) axis2_param_get_value(
				key_file_param, env);
        }
    }

    if (key_file)
    {
		status = AXIS2_HTTP_CLIENT_SET_KEY_FILE(sender->client, 
												env, key_file);
    }

	return status;
}

#ifdef AXIS2_LIBCURL_ENABLED
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_libcurl_http_send(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
	axiom_soap_envelope_t *out,
    const axis2_char_t *str_url,
    const axis2_char_t *soap_action)
{
	return axis2_libcurl_send (sender->om_output, env, msg_ctx, out, str_url, soap_action);
}
#endif

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_sender_get_param_string(
    axis2_http_sender_t *sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axiom_soap_envelope_t *soap_env = NULL;
    axiom_node_t *body_node = NULL;
    axiom_node_t *data_node = NULL;
    axiom_element_t *data_element = NULL;
    axiom_child_element_iterator_t *iterator = NULL;
    axis2_array_list_t *param_list = NULL;
    axis2_char_t *param_string = NULL;
    int i = 0;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, NULL);

    soap_env =  axis2_msg_ctx_get_soap_envelope(msg_ctx, env);
    if (! soap_env)
    {
        return NULL;
    }
    body_node = axiom_soap_body_get_base_node(
		axiom_soap_envelope_get_body(soap_env, env), env);
    data_node = AXIOM_NODE_GET_FIRST_CHILD(body_node, env);
    if (! data_node)
    {
        return NULL;
    }
    param_list = axis2_array_list_create(env, AXIS2_ARRAY_LIST_DEFAULT_CAPACITY);

	data_element = AXIOM_NODE_GET_DATA_ELEMENT(data_node, env);

	iterator = axiom_element_get_child_elements(data_element, env, data_node);

	if(iterator){
		while (AXIS2_TRUE == AXIOM_CHILD_ELEMENT_ITERATOR_HAS_NEXT(iterator, env))
		{
			axiom_node_t *node = NULL;
			axiom_element_t *element = NULL;
			axis2_char_t *name = NULL;
			axis2_char_t *value = NULL;
			axis2_char_t *encoded_value = NULL;
	 
			node = AXIOM_CHILD_ELEMENT_ITERATOR_NEXT(iterator, env);
			element = AXIOM_NODE_GET_DATA_ELEMENT(node, env);
			name = axiom_element_get_localname(element, env);
			value = axiom_element_get_text(element, env, node);
			if(value){
				encoded_value = (axis2_char_t *) AXIS2_MALLOC (env->allocator, strlen (value));
				memset (encoded_value, 0, strlen (value));
				encoded_value = axis2_url_encode (env, encoded_value, value, strlen (value));

				axis2_array_list_add(param_list, env, axis2_strcat(env, name, "=",
																   encoded_value, NULL));
			}
		}
	}
    for (i = 0; i < axis2_array_list_size(param_list, env); i++)
    {
        axis2_char_t *tmp_string = NULL;
        axis2_char_t *pair = NULL;

        pair = axis2_array_list_get(param_list, env, i);
		if(i ==0)
			tmp_string = axis2_stracat(param_string, pair, env);
		else
			tmp_string = axis2_strcat(env, param_string, "&", pair, NULL);				

        if (param_string)
        {
            AXIS2_FREE(env->allocator, param_string);
            param_string = NULL;
        }
        AXIS2_FREE(env->allocator, pair);
        param_string = tmp_string;
    }
    axis2_array_list_free(param_list, env);
    return param_string;
}

static axis2_char_t *
axis2_url_encode (
	const axis2_env_t *env, 
	axis2_char_t *dest, 
	axis2_char_t *buff, 
	int len)
{
	axis2_char_t string[4];
	axis2_char_t *expand_buffer;
    int i;
    for (i = 0; i < len  && buff[i]; i++)
    {
        if (isalnum (buff[i]) || is_safe_or_unreserve (buff[i]))
        {
            sprintf (string,"%c", buff[i]);
        }
        else
        {
            sprintf (string, "%%%x", buff[i]);
        }

		if ((strlen (dest) + 4) > len)
		{
			expand_buffer = (axis2_char_t *) AXIS2_MALLOC (env->allocator, len*2);
			memset (expand_buffer, 0, len*2);
			len *= 2;
			dest = memmove (expand_buffer, dest, strlen (dest));
		}
		strcat (dest, string);
    }
    return dest;
}

static int 
is_safe_or_unreserve (
	char c)
{
    char safe [] = {'$' , '-' , '_' , '.' , '+'};
    char reserve [] = {';', '/', '?' ,':', '@',  '&', '='};

/* reserved       = ";" | "/" | "?" | ":" | "@" | "&" | "="
   safe           = "$" | "-" | "_" | "." | "+" */

    int flag = 0;
    int i = 0;

    int size = sizeof (safe)/sizeof (safe[0]);
    for (i = 0; i < size; i++)
    {
        if (c == safe[i])
        {
            flag = 1;
            return flag;
        }
    }

    size = sizeof (reserve)/sizeof (reserve[0]);
    for (i = 0; i < size; i++)
    {
        if (c == reserve[i])
        {
            flag = 0;
            return flag;
        }
    }
    return flag;
}
