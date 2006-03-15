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
 
#include <axis2_soap_over_http_sender.h>
#include <axis2_string.h>
#include <axis2_http_transport.h>
#include <string.h>
#include <axis2_om_output.h>
#include <axis2_op_ctx.h>
#include <axis2_ctx.h>
#include <axis2_http_client.h>
#include <axis2_xml_writer.h>
#include <axis2_property.h>

/** 
 * @brief SOAP over HTTP sender struct impl
 *	Axis2 SOAP over HTTP sender impl  
 */
typedef struct axis2_soap_over_http_sender_impl 
						axis2_soap_over_http_sender_impl_t;  
  
struct axis2_soap_over_http_sender_impl
{
	axis2_soap_over_http_sender_t sender;
	axis2_char_t *http_version;
	axis2_bool_t chunked;
	int so_timeout;
	int connection_timeout;
	axis2_om_output_t *om_output;
	axis2_http_client_t *client;
};

#define AXIS2_INTF_TO_IMPL(sender) \
	                    ((axis2_soap_over_http_sender_impl_t *)(sender))

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_get_header_info 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
						axis2_http_simple_response_t *response);

axis2_status_t AXIS2_CALL
axis2_soap_over_http_sender_process_response 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
						axis2_http_simple_response_t *response);

axis2_status_t AXIS2_CALL
axis2_soap_over_http_sender_get_timeout_values 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_send 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
						axis2_soap_envelope_t *out, axis2_char_t *str_url, 
						axis2_char_t *soap_action);

axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_set_chunked
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_bool_t chunked);

axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_set_om_output
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_om_output_t *om_output);
						
axis2_status_t AXIS2_CALL
axis2_soap_over_http_sender_set_http_version
									(axis2_soap_over_http_sender_t *sender, 
									axis2_env_t **env, axis2_char_t *version);

axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_free
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env);
/***************************** End of function headers ************************/

axis2_soap_over_http_sender_t * AXIS2_CALL 
axis2_soap_over_http_sender_create(axis2_env_t **env)
{
    axis2_soap_over_http_sender_impl_t *sender_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    sender_impl =  (axis2_soap_over_http_sender_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_soap_over_http_sender_impl_t));
	
    if(NULL == sender_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
	
    sender_impl->http_version = AXIS2_HTTP_HEADER_PROTOCOL_11;
    sender_impl->so_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;
    sender_impl->connection_timeout = AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT;
	/* unlike the java impl we don't have a default om output
	 * it should be explicitly set and it's a MUST
	 */
	sender_impl->om_output = NULL;
	sender_impl->chunked = AXIS2_FALSE;
	sender_impl->client = NULL;
    
    sender_impl->sender.ops = AXIS2_MALLOC((*env)->allocator,
                        sizeof(axis2_soap_over_http_sender_ops_t));
    if(NULL == sender_impl->sender.ops)
	{
		axis2_soap_over_http_sender_free((axis2_soap_over_http_sender_t*)
						sender_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    sender_impl->sender.ops->send =
                        axis2_soap_over_http_sender_send;
    sender_impl->sender.ops->set_chunked =
                        axis2_soap_over_http_sender_set_chunked;
    sender_impl->sender.ops->set_om_output =
                        axis2_soap_over_http_sender_set_om_output;
	sender_impl->sender.ops->set_http_version =
						axis2_soap_over_http_sender_set_http_version;
    sender_impl->sender.ops->free =
                        axis2_soap_over_http_sender_free;
	return &(sender_impl->sender);
}

axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_free (axis2_soap_over_http_sender_t *sender, 
                        axis2_env_t **env)
{
    axis2_soap_over_http_sender_impl_t *sender_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    sender_impl = AXIS2_INTF_TO_IMPL(sender);
    if(NULL != sender_impl->http_version)
    {
        AXIS2_FREE((*env)->allocator, sender_impl->http_version);
        sender_impl->http_version= NULL;
    }
    /* This is a shallow copy, thus should not be freed - Samisa
    if(NULL != sender_impl->om_output)
    {
        AXIS2_OM_OUTPUT_FREE(sender_impl->om_output, env);
        sender_impl->om_output = NULL;
    }*/

    if (sender_impl->client)
    {
        AXIS2_HTTP_CLIENT_FREE(sender_impl->client, env);
        sender_impl->client = NULL;
    }
    
    if(NULL != sender->ops)
        AXIS2_FREE((*env)->allocator, sender->ops);
    
	AXIS2_FREE((*env)->allocator, sender_impl);
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_send 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx,
						axis2_soap_envelope_t *out, axis2_char_t *str_url, 
						axis2_char_t *soap_action)
{
	axis2_http_simple_request_t *request = NULL;
	axis2_http_request_line_t *request_line = NULL;
	axis2_url_t *url = NULL;
	axis2_soap_over_http_sender_impl_t *sender_impl = NULL;
	axis2_xml_writer_t *xml_writer = NULL;
	axis2_char_t *buffer = NULL;
	axis2_char_t *char_set_enc = NULL;
	int status_code = -1;
	axis2_http_header_t *http_header = NULL;
	axis2_http_simple_response_t *response = NULL;
	axis2_char_t *content_type = NULL;
    axis2_property_t *property = NULL;
		
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, out, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, str_url, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, soap_action, AXIS2_FAILURE);
	
	url = axis2_url_parse_string(env, str_url);
	sender_impl = AXIS2_INTF_TO_IMPL(sender);
	if(NULL == url)
	{
		return AXIS2_FAILURE;
	}
    
    if (sender_impl->client)
    {
        AXIS2_HTTP_CLIENT_FREE(sender_impl->client, env);
        sender_impl->client = NULL;
    }
    
	sender_impl->client = axis2_http_client_create(env, url);
	if(NULL == sender_impl->client)
	{
		return AXIS2_FAILURE;
	}

	if(NULL == sender_impl->om_output)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_OM_OUTPUT, 
						AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	xml_writer = AXIS2_OM_OUTPUT_GET_XML_WRITER(sender_impl->om_output, env);
	
	property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
							AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);
    if(property)
    {
        char_set_enc = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
	if(NULL == char_set_enc)
	{
		char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
	}
	/* AXIS2_OM_OUTPUT_SET_DO_OPTIMIZE(om_output, env, 
	 *				AXIS2_MSG_CTX_GET_IS_DOING_MTOM(msg_ctx, env);
	 */
	AXIS2_SOAP_ENVELOPE_SERIALIZE (out, env, sender_impl->om_output, 
						AXIS2_FALSE);
	buffer = AXIS2_XML_WRITER_GET_XML(xml_writer, env);
	
	request_line = axis2_http_request_line_create(env, "POST", 
						AXIS2_URL_GET_PATH(url, env), 
						sender_impl->http_version);
	request = axis2_http_simple_request_create(env, request_line, NULL, 0, 
						NULL);
	
	http_header = axis2_http_header_create(env, AXIS2_HTTP_HEADER_USER_AGENT, 
						"Axis2/C");
	AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, http_header);
	http_header = axis2_http_header_create(env, AXIS2_HTTP_HEADER_SOAP_ACTION, 
						soap_action);
	AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, http_header);
	if(AXIS2_FALSE == sender_impl->chunked)
	{
		axis2_char_t tmp_buf[10];
		sprintf(tmp_buf, "%d", strlen(buffer));
		http_header = axis2_http_header_create(env, 
						AXIS2_HTTP_HEADER_CONTENT_LENGTH, tmp_buf);
		AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, http_header);
	}
	else
	{
		http_header = axis2_http_header_create(env, 
						AXIS2_HTTP_HEADER_TRANSFER_ENCODING, 
						AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED);
		AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, http_header);
	}
	/* TODO we need to set the content type with soap action header for soap12*/
	if(AXIS2_TRUE == AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env))
	{
		content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
	}
	else
	{
		content_type = AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP;
	}
	http_header = axis2_http_header_create(env, AXIS2_HTTP_HEADER_CONTENT_TYPE, 
						content_type);
	AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, http_header);
	if(0 == AXIS2_STRCMP(sender_impl->http_version, 
		AXIS2_HTTP_HEADER_PROTOCOL_11))
	{
		http_header = axis2_http_header_create(env, 
						AXIS2_HTTP_HEADER_HOST, 
						AXIS2_URL_GET_SERVER(url, env));
		AXIS2_HTTP_SIMPLE_REQUEST_ADD_HEADER(request, env, http_header);
	}
	AXIS2_HTTP_SIMPLE_REQUEST_SET_BODY_STRING(request, env, buffer);
	axis2_soap_over_http_sender_get_timeout_values(sender, env, msg_ctx);
	AXIS2_HTTP_CLIENT_SET_TIMEOUT(sender_impl->client, env, 
						sender_impl->so_timeout);
	
	status_code = AXIS2_HTTP_CLIENT_SEND(sender_impl->client, env, request);
	
	AXIS2_HTTP_SIMPLE_REQUEST_FREE(request, env);
	request = NULL;
	
	status_code = AXIS2_HTTP_CLIENT_RECIEVE_HEADER(sender_impl->client, env);
	if(status_code < 0)
	{
		return AXIS2_FAILURE;
	}
	response = AXIS2_HTTP_CLIENT_GET_RESPONSE(sender_impl->client, env);
	return axis2_soap_over_http_sender_process_response(sender, env, msg_ctx,
						response);    
}


axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_set_chunked
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_bool_t chunked)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(sender)->chunked = chunked;
	return AXIS2_SUCCESS;	
}


axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_set_om_output
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_om_output_t *om_output)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(sender)->om_output = om_output;
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_get_header_info 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
						axis2_http_simple_response_t *response)
{
	axis2_array_list_t *headers = NULL;
	axis2_char_t *charset = NULL;
	axis2_soap_over_http_sender_impl_t *sender_impl = NULL;
	int i = 0;
	axis2_bool_t response_chunked = AXIS2_FALSE;
	int *content_length = NULL;
    axis2_property_t *property = NULL;
	
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, response, AXIS2_FAILURE);
	
	sender_impl = AXIS2_INTF_TO_IMPL(sender);
	
	/*
	 * TODO MTOM support (MIME header)
	 */
	headers = AXIS2_HTTP_SIMPLE_RESPONSE_GET_HEADERS(response, env);
	if(headers == NULL)
	{
		return AXIS2_SUCCESS;
	}
	for(i = 0; i < AXIS2_ARRAY_LIST_SIZE(headers, env); i++)
	{
		axis2_http_header_t *header = AXIS2_ARRAY_LIST_GET(headers, env, i);
		axis2_char_t *name = AXIS2_HTTP_HEADER_GET_NAME((axis2_http_header_t *)
						header, env);
		if(NULL != name)
		{
			if(0 == AXIS2_STRCMP(name, AXIS2_HTTP_HEADER_TRANSFER_ENCODING) && 
						0 == AXIS2_STRCMP(AXIS2_HTTP_HEADER_GET_VALUE(header
						, env), AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
			{
                axis2_char_t *transfer_encoding = NULL;

                transfer_encoding = 
                    AXIS2_STRDUP(AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED, 
                    env);
                property = axis2_property_create(env);
                AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
                AXIS2_PROPERTY_SET_VALUE(property, env, transfer_encoding);
				AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, 
						AXIS2_HTTP_HEADER_TRANSFER_ENCODING, 
						property,
						AXIS2_FALSE);
				response_chunked = AXIS2_TRUE;
			}
			if(0 != AXIS2_STRCMP(name, AXIS2_HTTP_HEADER_CONTENT_TYPE))
			{
				axis2_char_t *tmp_charset = NULL;
				axis2_char_t *content_type = AXIS2_HTTP_HEADER_GET_VALUE(header, 
						env);
				tmp_charset = strstr(content_type, AXIS2_HTTP_CHAR_SET_ENCODING);
				if(NULL != charset)
				{
					charset = AXIS2_STRDUP(tmp_charset, env);
					break;
				}
			}
		}
	}
	if(NULL != charset)
	{
		axis2_ctx_t *axis_ctx = AXIS2_OP_CTX_GET_BASE(AXIS2_MSG_CTX_GET_OP_CTX(
						msg_ctx, env), env);
		if(NULL != axis_ctx)
		{
            property = axis2_property_create(env);
            AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
            AXIS2_PROPERTY_SET_VALUE(property, env, charset); 
			AXIS2_CTX_SET_PROPERTY(axis_ctx, env, AXIS2_CHARACTER_SET_ENCODING, 
						property, AXIS2_FALSE);
		}
	}
	if(AXIS2_FALSE == response_chunked)
	{
		int tmp_len = 0;
		content_length = AXIS2_MALLOC((*env)->allocator, sizeof(int));
		if(NULL == content_length)
		{
			return AXIS2_FAILURE;
		}
		tmp_len = AXIS2_HTTP_SIMPLE_RESPONSE_GET_CONTENT_LENGTH(response, env);
		memcpy(content_length, &tmp_len, sizeof(int));
        property = axis2_property_create(env);
        AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
        AXIS2_PROPERTY_SET_VALUE(property, env, content_length);
		AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, 
						AXIS2_HTTP_HEADER_CONTENT_LENGTH, property, 
						AXIS2_FALSE);
	}
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_soap_over_http_sender_process_response 
								(axis2_soap_over_http_sender_t *sender, 
                                axis2_env_t **env, axis2_msg_ctx_t *msg_ctx, 
								axis2_http_simple_response_t *response)
{
    axis2_stream_t *in_stream = NULL;
	axis2_ctx_t *axis_ctx = NULL;
    axis2_property_t *property = NULL;
	
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, response, AXIS2_FAILURE);
	
	in_stream = AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY(response, env);
	if(NULL == in_stream)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_STREAM_IN_RESPONSE_BODY, 
								AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	
	axis2_soap_over_http_sender_get_header_info(sender, env, msg_ctx, response);
	axis_ctx = AXIS2_OP_CTX_GET_BASE(AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env), 
						env);
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_stream_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, in_stream);
	AXIS2_CTX_SET_PROPERTY(axis_ctx, env, AXIS2_TRANSPORT_IN, property, 
						AXIS2_FALSE);
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_soap_over_http_sender_get_timeout_values 
								(axis2_soap_over_http_sender_t *sender, 
                                axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_char_t *so_str = NULL;
	axis2_char_t *connection_str = NULL;
	
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	
	so_str = (axis2_char_t*)AXIS2_MSG_CTX_GET_PARAMETER(msg_ctx, 
						env, AXIS2_HTTP_SO_TIMEOUT);
	connection_str = (axis2_char_t*)AXIS2_MSG_CTX_GET_PARAMETER(msg_ctx, 
						env, AXIS2_HTTP_CONNECTION_TIMEOUT);
	if(NULL != so_str)
	{
		AXIS2_INTF_TO_IMPL(sender)->so_timeout = atoi(so_str);
	}
	if(NULL != connection_str)
	{
		AXIS2_INTF_TO_IMPL(sender)->connection_timeout = atoi(connection_str);
	}
    
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_soap_over_http_sender_set_http_version
									(axis2_soap_over_http_sender_t *sender, 
									axis2_env_t **env, axis2_char_t *version)
{
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
	AXIS2_INTF_TO_IMPL(sender)->http_version =  AXIS2_STRDUP(version, env);
	if(NULL == AXIS2_INTF_TO_IMPL(sender)->http_version)
	{
		return AXIS2_FAILURE;
	}
	return AXIS2_SUCCESS;
}
