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
};

#define AXIS2_INTF_TO_IMPL(sender) \
	                    ((axis2_soap_over_http_sender_impl_t *)(sender))

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_get_header_info 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx);

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
						axis2_om_node_t *output, axis2_char_t *url, 
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
axis2_soap_over_http_sender_free
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env);
/***************************** End of function headers ************************/

axis2_soap_over_http_sender_t * AXIS2_CALL 
axis2_soap_over_http_sender_create(axis2_env_t **env)
{
    AXIS2_ENV_CHECK(env, NULL);

    axis2_soap_over_http_sender_impl_t *sender_impl = 
                        (axis2_soap_over_http_sender_impl_t *)AXIS2_MALLOC 
                        ((*env)->allocator, sizeof(
                        axis2_soap_over_http_sender_impl_t));
	
    if(NULL == sender_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
	
    sender_impl->http_version = NULL;
    sender_impl->so_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;
    sender_impl->connection_timeout = AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT;
	/* unlike the java impl we don't have a default om output
	 * it should be explicitly set and it's a MUST
	 */
	sender_impl->om_output = NULL;
	sender_impl->chunked = AXIS2_FALSE;
    
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
    sender_impl->sender.ops->free =
                        axis2_soap_over_http_sender_free;
	return &(sender_impl->sender);
}

axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_free (axis2_soap_over_http_sender_t *sender, 
                        axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(sender, env, AXIS2_FAILURE);
    axis2_soap_over_http_sender_impl_t *sender_impl =
                        AXIS2_INTF_TO_IMPL(sender);
    if(NULL != sender_impl->http_version)
    {
        AXIS2_FREE((*env)->allocator, sender_impl->http_version);
        sender_impl->http_version= NULL;
    }
    if(NULL != sender_impl->om_output)
    {
        AXIS2_OM_OUTPUT_FREE(sender_impl->om_output, env);
        sender_impl->om_output = NULL;
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
						axis2_om_node_t *output, axis2_char_t *url, 
						axis2_char_t *soap_action)
{
    AXIS2_FUNC_PARAM_CHECK(sender, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, output, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, url, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, soap_action, AXIS2_FAILURE);
	
	/*
	 * TODO create the http client and send 
	 */	
	return AXIS2_SUCCESS;
    
}


axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_set_chunked
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_bool_t chunked)
{
    AXIS2_FUNC_PARAM_CHECK(sender, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(sender)->chunked = chunked;
	return AXIS2_SUCCESS;	
}


axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_set_om_output
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_om_output_t *om_output)
{
    AXIS2_FUNC_PARAM_CHECK(sender, env, AXIS2_FAILURE);
    AXIS2_INTF_TO_IMPL(sender)->om_output = om_output;
	return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL 
axis2_soap_over_http_sender_get_header_info 
						(axis2_soap_over_http_sender_t *sender, 
						axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_http_simple_response_t *response = NULL;
	axis2_array_list_t *headers = NULL;
	axis2_char_t *charset = NULL;
	int i = 0;
	
	AXIS2_FUNC_PARAM_CHECK(sender, env, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	/*
	 * TODO MUST add the http client and client->get_response;
	 * TODO MTOM support (MIME header)
	 */
	headers = AXIS2_HTTP_SIMPLE_RESPONSE_GET_HEADERS(response, env);
	for(i = 0; i < AXIS2_ARRAY_LIST_SIZE(headers, env); i++)
	{
		axis2_http_header_t *header = AXIS2_ARRAY_LIST_GET(headers, env, i);
		axis2_char_t *name = AXIS2_HTTP_HEADER_GET_NAME((axis2_http_header_t *)
						header, env);
		if(NULL != name && 0 != AXIS2_STRCMP(name, 
						AXIS2_HTTP_HEADER_CONTENT_TYPE))
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
	if(NULL != charset)
	{
		AXIS2_CTX_SET_PROPERTY(AXIS2_OP_CTX_GET_BASE(AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env), env), env, AXIS2_CHARACTER_SET_ENCODING, 
						charset, AXIS2_FALSE);
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
	
	AXIS2_FUNC_PARAM_CHECK(sender, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, msg_ctx, AXIS2_FAILURE);
	AXIS2_PARAM_CHECK((*env)->error, response, AXIS2_FAILURE);
	
	in_stream = AXIS2_HTTP_SIMPLE_RESPONSE_GET_BODY(response, env);
	if(NULL == in_stream)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_IN_STREAM_IN_RESPONSE, 
								AXIS2_FAILURE);
		return AXIS2_FAILURE;
	}
	
	axis2_soap_over_http_sender_get_header_info(sender, env, msg_ctx);
	AXIS2_CTX_SET_PROPERTY(AXIS2_OP_CTX_GET_BASE(AXIS2_MSG_CTX_GET_OP_CTX
								(msg_ctx, env), env), env, AXIS2_TRANSPORT_IN, 
								in_stream, AXIS2_FALSE);
	return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_soap_over_http_sender_get_timeout_values 
								(axis2_soap_over_http_sender_t *sender, 
                                axis2_env_t **env, axis2_msg_ctx_t *msg_ctx)
{
    axis2_char_t *so_str = NULL;
	axis2_char_t *connection_str = NULL;
	
	AXIS2_FUNC_PARAM_CHECK(sender, env, AXIS2_FAILURE);
	
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
