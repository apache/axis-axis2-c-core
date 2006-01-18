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
 
#include <axis2_http_worker.h>
#include <axis2_string.h>
#include <axis2_http_transport.h>
#include <axis2_conf.h>
#include <axis2_string.h>
#include <axis2_msg_ctx.h>
#include <axis2_http_request_line.h>
#include <axis2_http_out_transport_info.h>
#include <axis2_http_transport_utils.h>
#include <axis2_op_ctx.h>
#include <axis2_engine.h>

/** 
 * @brief HTTP Worker struct impl
 *	Axis2 HTTP Worker impl  
 */
typedef struct axis2_http_worker_impl axis2_http_worker_impl_t;  
  
struct axis2_http_worker_impl
{
	axis2_http_worker_t http_worker;
	axis2_conf_ctx_t *conf_ctx;
};

#define AXIS2_INTF_TO_IMPL(http_worker) ((axis2_http_worker_impl_t *)\
                                                                (http_worker))

/***************************** Function headers *******************************/

axis2_bool_t AXIS2_CALL 
axis2_http_worker_process_request(axis2_http_worker_t *http_worker, 
							axis2_env_t **env, 
							axis2_simple_http_svr_conn_t *svr_conn, 
							axis2_http_simple_request_t *simple_request);
    
axis2_status_t AXIS2_CALL 
axis2_http_worker_set_response_headers(axis2_http_worker_t *http_worker, 
							axis2_env_t **env, 
							axis2_simple_http_svr_conn_t *svr_conn, 
							axis2_http_simple_request_t *simple_request,
							axis2_http_simple_response_t *simple_response,
							axis2_ssize_t content_length);

axis2_status_t AXIS2_CALL 
axis2_http_worker_set_transport_out_config(axis2_http_worker_t *http_worker, 
							axis2_env_t **env, 
							axis2_conf_ctx_t *conf_ctx, 
							axis2_http_simple_response_t *simple_response);
							
axis2_hash_t* AXIS2_CALL
axis2_http_worker_get_headers(axis2_http_worker_t *http_worker, 
							axis2_env_t **env, 
							axis2_http_simple_request_t *request);
	
axis2_status_t AXIS2_CALL 
axis2_http_worker_free(axis2_http_worker_t *http_worker, 
							axis2_env_t **env);
								
/***************************** End of function headers ************************/

axis2_http_worker_t * AXIS2_CALL 
axis2_http_worker_create (axis2_env_t **env, axis2_conf_ctx_t *conf_ctx)
{
    AXIS2_ENV_CHECK(env, NULL);
    axis2_http_worker_impl_t *http_worker_impl = (axis2_http_worker_impl_t *)
        AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_http_worker_impl_t));
	
    if(NULL == http_worker_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    http_worker_impl->conf_ctx = conf_ctx;
    
    http_worker_impl->http_worker.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_http_worker_ops_t));
    if(NULL == http_worker_impl->http_worker.ops)
	{
		axis2_http_worker_free((axis2_http_worker_t*)http_worker_impl, env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    http_worker_impl->http_worker.ops->process_request = 
                                    axis2_http_worker_process_request;
    http_worker_impl->http_worker.ops->free = axis2_http_worker_free;
    
	return &(http_worker_impl->http_worker);
}

axis2_status_t AXIS2_CALL
axis2_http_worker_free (axis2_http_worker_t *http_worker, axis2_env_t **env)
{
	AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    
    AXIS2_INTF_TO_IMPL(http_worker)->conf_ctx = NULL;
    
    if(NULL != http_worker->ops)
        AXIS2_FREE((*env)->allocator, http_worker->ops);
    
	AXIS2_FREE((*env)->allocator, AXIS2_INTF_TO_IMPL(http_worker));
    
	return AXIS2_SUCCESS;
}

axis2_bool_t AXIS2_CALL 
axis2_http_worker_process_request(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, 
						axis2_simple_http_svr_conn_t *svr_conn, 
						axis2_http_simple_request_t *simple_request)
{
    axis2_http_worker_impl_t *http_worker_impl = NULL;
	axis2_conf_ctx_t *conf_ctx = NULL;
	axis2_msg_ctx_t *msg_ctx = NULL;
	axis2_stream_t *request_body = NULL;
	axis2_stream_t *out_stream = axis2_stream_create_basic(env);
	axis2_http_simple_response_t *response = NULL;
	axis2_transport_out_desc_t *out_desc = NULL;
	axis2_transport_in_desc_t *in_desc = NULL;
	axis2_char_t *http_version = NULL;
	axis2_char_t *soap_action = NULL;
	axis2_bool_t processed = AXIS2_FALSE;
	axis2_char_t *ctx_written = NULL;
	axis2_status_t status = AXIS2_FAILURE;
	
	AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svr_conn, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_request, AXIS2_FAILURE);
	
	http_worker_impl = AXIS2_INTF_TO_IMPL(http_worker);
	conf_ctx = http_worker_impl->conf_ctx;
	    
	if(NULL == conf_ctx)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
						AXIS2_FAILURE);
		return AXIS2_FALSE;
	}
	request_body = AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY(simple_request, env);
	response = axis2_http_simple_response_create_default(env);
	out_desc = AXIS2_CONF_GET_TRANSPORT_OUT(AXIS2_CONF_CTX_GET_CONF
						(http_worker_impl->conf_ctx, env), env, 
						axis2_qname_create(env, AXIS2_TRANSPORT_HTTP, NULL, 
						NULL));
	in_desc = AXIS2_CONF_GET_TRANSPORT_IN(AXIS2_CONF_CTX_GET_CONF
						(http_worker_impl->conf_ctx, env), env, 
						axis2_qname_create(env, AXIS2_TRANSPORT_HTTP, NULL,
						NULL));
	msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
	AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);
	
	http_version = AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION
						(AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE
						(simple_request, env), env);
	
	if(NULL == http_version)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_HTTP_VERSION, 
						AXIS2_FAILURE);
	}
	if(0 == AXIS2_STRCASECMP(http_version, AXIS2_HTTP_HEADER_PROTOCOL_11))
	{
		axis2_http_worker_set_transport_out_config(http_worker, env, conf_ctx,
						response);
	}
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_TRANSPORT_OUT, out_stream, 
						AXIS2_FALSE);
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_TRANSPORT_HEADERS, 
						axis2_http_worker_get_headers(http_worker, env, 
						simple_request), AXIS2_FALSE);
	/*
	 * TODO uuidgen
	 * AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(uuid, env);
	 */
	AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO,
						axis2_http_out_transport_info_create(env, response), 
						AXIS2_FALSE);
	if(NULL != AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request, env, 
						AXIS2_HTTP_HEADER_SOAP_ACTION))
	{
		soap_action = AXIS2_HTTP_HEADER_GET_VALUE(
						AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(
						simple_request, env, AXIS2_HTTP_HEADER_SOAP_ACTION), 
						env);
	}
	if(0 == AXIS2_STRCASECMP(AXIS2_HTTP_REQUEST_LINE_GET_METHOD(
						AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
						simple_request, env), env), AXIS2_HTTP_HEADER_GET))
	{
		processed = axis2_http_transport_utils_process_http_get_request
                        (env, msg_ctx, request_body, out_stream,
						AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_TYPE(
						simple_request, env) ,soap_action,
						AXIS2_HTTP_REQUEST_LINE_GET_URI(
						AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
						simple_request, env), env),
                        conf_ctx, 
                        axis2_http_transport_utils_get_request_params(env,
						AXIS2_HTTP_REQUEST_LINE_GET_URI(
						AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
						simple_request, env), env)));
		if(AXIS2_FALSE == processed)
		{
			axis2_http_header_t *cont_len = NULL;
			axis2_char_t *body_string = NULL;
			AXIS2_HTTP_SIMPLE_RESPONSE_SET_STAUTUS_LINE(response, env,
						http_version, AXIS2_HTTP_RESPONSE_OK_CODE_VAL, "OK");
			body_string = axis2_http_transport_utils_get_services_html(env, 
						conf_ctx);
			if(NULL != body_string)
			{
				axis2_char_t str_len[10];
				AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STRING(response, env, 
						body_string);
				sprintf(str_len, "%d", AXIS2_STRLEN(body_string));
				cont_len = axis2_http_header_create(env, 
						AXIS2_HTTP_HEADER_CONTENT_LENGTH, 
						str_len);
				AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(response, env, cont_len);
			}					
			
			axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
						simple_request, response, 0);
			AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env, response);
			AXIS2_HTTP_SIMPLE_RESPONSE_FREE(response, env);
			return AXIS2_TRUE;			
		}
		
	}
	else if(0 == AXIS2_STRCASECMP(AXIS2_HTTP_REQUEST_LINE_GET_METHOD(
						AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
						simple_request, env), env), AXIS2_HTTP_HEADER_POST))
	{
		status = axis2_http_transport_utils_process_http_post_request
                        (env, msg_ctx, request_body, out_stream,
						AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_TYPE(
						simple_request, env) ,soap_action,
						AXIS2_HTTP_REQUEST_LINE_GET_URI
						(AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
						simple_request, env), env));
		if(status == AXIS2_FAILURE)
		{
			axis2_msg_ctx_t *fault_ctx = NULL;
			axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
			axis2_http_request_line_t *req_line = NULL;
			axis2_http_status_line_t *tmp_stat_line = NULL;
			axis2_char_t status_line_str[100];
			if(NULL == engine)
			{
				return AXIS2_FALSE;
			}
			fault_ctx = AXIS2_ENGINE_CREATE_FAULT_MSG_CTX(engine, env, msg_ctx);
			req_line = AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(simple_request
						, env);
			if(NULL != req_line)
			{
				sprintf(status_line_str, "%s 500 Internal Server Error\r\n", 
						AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION(req_line, 
						env));
			}
			else
			{
				sprintf(status_line_str, "HTTP/1.1 500 Internal Server Error\
						\r\n");
			}
			tmp_stat_line = axis2_http_status_line_create(env, 
						status_line_str);
			AXIS2_ENGINE_SEND_FAULT(engine, env, fault_ctx);
			AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STREAM(response, env, 
						out_stream);
			axis2_http_worker_set_response_headers(http_worker, env, svr_conn, 
						simple_request, response, AXIS2_STREAM_BASIC_GET_LEN(
						out_stream, env));
			
			status = AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env, 
						response);
			return status;			
			
		}
	}
	axis2_op_ctx_t *op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
	if(NULL != op_ctx)
	{
		ctx_written = AXIS2_OP_CTX_GET_PROPERTY(op_ctx, env, 
						AXIS2_RESPONSE_WRITTEN);
	}
	if(NULL != ctx_written && AXIS2_STRCASECMP(ctx_written, "TRUE"))
	{
		AXIS2_HTTP_SIMPLE_RESPONSE_SET_STAUTUS_LINE(response, env, http_version,
						AXIS2_HTTP_RESPONSE_OK_CODE_VAL, "OK");
	}
	else
	{
		AXIS2_HTTP_SIMPLE_RESPONSE_SET_STAUTUS_LINE(response, env, http_version,
						AXIS2_HTTP_RESPONSE_ACK_CODE_VAL, "Accepted");
	}
	AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STREAM(response, env, out_stream);
	axis2_http_worker_set_response_headers(http_worker, env, svr_conn, 
						simple_request, response, AXIS2_STREAM_BASIC_GET_LEN(
						out_stream, env));
	
	status = AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env, response);
	AXIS2_MSG_CTX_FREE(msg_ctx, env);
	msg_ctx = NULL;
	AXIS2_HTTP_SIMPLE_RESPONSE_FREE(response, env);
	response = NULL;
	
    return status;
}


axis2_status_t AXIS2_CALL 
axis2_http_worker_set_response_headers(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, 
						axis2_simple_http_svr_conn_t *svr_conn, 
						axis2_http_simple_request_t *simple_request,
						axis2_http_simple_response_t *simple_response,
						axis2_ssize_t content_length)
{
    axis2_http_header_t *conn_header = NULL;
	
	AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, svr_conn, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_request, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_response, AXIS2_FAILURE);
    if(AXIS2_FALSE == AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER
                    (simple_response, env, AXIS2_HTTP_HEADER_CONNECTION))
    {
        conn_header = AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request,
                    env, AXIS2_HTTP_HEADER_CONNECTION);
        if(NULL != conn_header)
        {
            if(0 == AXIS2_STRCASECMP(AXIS2_HTTP_HEADER_GET_VALUE(conn_header, 
                    env), AXIS2_HTTP_HEADER_CONNECTION_KEEPALIVE))
            {
                axis2_http_header_t *header = axis2_http_header_create(env,
                    AXIS2_HTTP_HEADER_CONNECTION, 
                    AXIS2_HTTP_HEADER_CONNECTION_KEEPALIVE);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env, 
                    header);
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_TRUE);
            }
            if(0 == AXIS2_STRCASECMP(AXIS2_HTTP_HEADER_GET_VALUE(conn_header, 
                    env), AXIS2_HTTP_HEADER_CONNECTION_CLOSE))
            {
                axis2_http_header_t *header = axis2_http_header_create(env,
                    AXIS2_HTTP_HEADER_CONNECTION,
					AXIS2_HTTP_HEADER_CONNECTION_CLOSE);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env, 
                    header);
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_FALSE);
            }
        }
        else
        {
            if(AXIS2_HTTP_SIMPLE_RESPONSE_GET_HTTP_VERSION(simple_response, env) &&
                    AXIS2_STRCASECMP(AXIS2_HTTP_SIMPLE_RESPONSE_GET_HTTP_VERSION(
                    simple_response, env), AXIS2_HTTP_HEADER_PROTOCOL_11))
            {
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_TRUE);               
            }
            else
            {
                AXIS2_SIMPLE_HTTP_SVR_CONN_SET_KEEP_ALIVE(svr_conn, env, 
                    AXIS2_FALSE);
            }
        }
        if(AXIS2_TRUE == AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER(
                    simple_response, env, AXIS2_HTTP_HEADER_TRANSFER_ENCODING))
        {
            if(0 != content_length)
            {
                axis2_char_t content_len_str[10];
				axis2_http_header_t *content_len_hdr = NULL;
				
                sprintf(content_len_str, "%10d", content_length);
                content_len_hdr = axis2_http_header_create(
                    env, AXIS2_HTTP_HEADER_CONTENT_LENGTH, content_len_str);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env,
                    content_len_hdr);
            }
        }
    }
    return AXIS2_SUCCESS;
}

/*
 * This is only called for HTTP/1.1 to enable 1.1 specific parameters.
 *
 */
axis2_status_t AXIS2_CALL 
axis2_http_worker_set_transport_out_config(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, 
						axis2_conf_ctx_t *conf_ctx, 
						axis2_http_simple_response_t *simple_response)
{
    axis2_conf_t *config = NULL;
	axis2_hash_t *tranport_outs = NULL;
	
	AXIS2_FUNC_PARAM_CHECK(http_worker, env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, conf_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, simple_response, AXIS2_FAILURE);
	
	config = AXIS2_CONF_CTX_GET_CONF(conf_ctx, env);
    tranport_outs = AXIS2_CONF_GET_TRANSPORTS_OUT(config, env);
	

    /*
        TODO implement the method
    */
    return AXIS2_SUCCESS;
}

axis2_hash_t* AXIS2_CALL
axis2_http_worker_get_headers(axis2_http_worker_t *http_worker, 
						axis2_env_t **env, axis2_http_simple_request_t *request)
{
	/* TODO implement */
	return NULL;
}
