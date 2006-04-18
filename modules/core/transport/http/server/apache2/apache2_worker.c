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
 
#include "axis2_apache2_worker.h"
#include <axis2_http_transport.h>
#include <axis2_conf.h>
#include <axis2_string.h>
#include <axis2_msg_ctx.h>
#include <axis2_http_out_transport_info.h>
#include <axis2_http_transport_utils.h>
#include <axis2_op_ctx.h>
#include <axis2_engine.h>
#include <axis2_uuid_gen.h>
#include <axis2_conf_init.h>
#include "axis2_apache2_out_transport_info.h"
#include <axis2_url.h>
#include <http_core.h>
#include <http_protocol.h>


#define READ_SIZE  32
/** 
 * @brief Apahche2 Worker struct impl
 *	Axis2 Apache2 Worker impl  
 */
typedef struct axis2_apache2_worker_impl axis2_apache2_worker_impl_t;  
  
struct axis2_apache2_worker_impl
{
	axis2_apache2_worker_t apache2_worker;
	axis2_conf_ctx_t *conf_ctx;
};

#define AXIS2_INTF_TO_IMPL(apache2_worker) ((axis2_apache2_worker_impl_t *)\
                        (apache2_worker))

/***************************** Function headers *******************************/

int AXIS2_CALL 
axis2_apache2_worker_process_request(axis2_apache2_worker_t *apache2_worker, 
							axis2_env_t **env, request_rec *req);
    
AXIS2_DECLARE(axis2_char_t*)
axis2_apache2_worker_get_bytes(axis2_env_t **env, 
                        axis2_stream_t *stream);
                        
axis2_status_t AXIS2_CALL 
axis2_apache2_worker_free(axis2_apache2_worker_t *apache2_worker, 
							axis2_env_t **env);
								
/***************************** End of function headers ************************/

AXIS2_DECLARE(axis2_apache2_worker_t *)
axis2_apache2_worker_create (axis2_env_t **env, axis2_char_t *repo_path)
{
    axis2_apache2_worker_impl_t *apache2_worker_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    apache2_worker_impl = (axis2_apache2_worker_impl_t *)
        AXIS2_MALLOC ((*env)->allocator, sizeof(axis2_apache2_worker_impl_t));
	
    if(NULL == apache2_worker_impl)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    apache2_worker_impl->apache2_worker.ops = NULL;
    apache2_worker_impl->conf_ctx = build_conf_ctx(env, repo_path);
        
    if(NULL == apache2_worker_impl->conf_ctx)
    {
        axis2_apache2_worker_free((axis2_apache2_worker_t*)apache2_worker_impl,
                                   env);
        return NULL;
    }
    apache2_worker_impl->apache2_worker.ops = AXIS2_MALLOC((*env)->allocator,
        sizeof(axis2_apache2_worker_ops_t));
    if(NULL == apache2_worker_impl->apache2_worker.ops)
	{
		axis2_apache2_worker_free((axis2_apache2_worker_t*)apache2_worker_impl,
                        env);
        AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
	}
    
    apache2_worker_impl->apache2_worker.ops->process_request = 
                                    axis2_apache2_worker_process_request;
    apache2_worker_impl->apache2_worker.ops->free = axis2_apache2_worker_free;
    
	return &(apache2_worker_impl->apache2_worker);
}

axis2_status_t AXIS2_CALL
axis2_apache2_worker_free (axis2_apache2_worker_t *apache2_worker, 
                        axis2_env_t **env)
{
    axis2_apache2_worker_impl_t *worker_impl = NULL;
	AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    
    worker_impl = AXIS2_INTF_TO_IMPL(apache2_worker);
    if(worker_impl->conf_ctx != NULL)
    {
        AXIS2_CONF_CTX_FREE(worker_impl->conf_ctx, env);
        worker_impl->conf_ctx = NULL;
    }
    
    if(NULL != apache2_worker->ops)
        AXIS2_FREE((*env)->allocator, apache2_worker->ops);
    
    AXIS2_FREE((*env)->allocator, worker_impl->conf_ctx);
    
	return AXIS2_SUCCESS;
}

int AXIS2_CALL 
axis2_apache2_worker_process_request(axis2_apache2_worker_t *apache2_worker, 
                        axis2_env_t **env, request_rec *request)
{
    axis2_apache2_worker_impl_t *apache2_worker_impl = NULL;
	axis2_conf_ctx_t *conf_ctx = NULL;
	axis2_msg_ctx_t *msg_ctx = NULL;
	axis2_stream_t *request_body = NULL;
    axis2_stream_t *out_stream = NULL;
	axis2_transport_out_desc_t *out_desc = NULL;
	axis2_transport_in_desc_t *in_desc = NULL;
	axis2_char_t *http_version = NULL;
	axis2_char_t *soap_action = NULL;
	axis2_bool_t processed = AXIS2_FALSE;
	axis2_char_t *ctx_written = NULL;
	int content_length = -1;
	axis2_char_t *encoding_header_value = NULL;
	axis2_op_ctx_t *op_ctx = NULL;
    axis2_char_t *req_url = NULL;
    axis2_char_t *body_string = NULL;
    int send_status = -1;
    axis2_char_t *content_type = NULL;
    axis2_property_t *property = NULL;
    axis2_url_t *url = NULL;
    axis2_http_out_transport_info_t *apache2_out_transport_info = NULL;
    axis2_qname_t *transport_qname = NULL;
    axis2_char_t *ctx_uuid = NULL;
	
    AXIS2_ENV_CHECK(env, AXIS2_CRTICAL_FAILURE);
    AXIS2_PARAM_CHECK((*env)->error, request, AXIS2_CRTICAL_FAILURE);
	
	apache2_worker_impl = AXIS2_INTF_TO_IMPL(apache2_worker);
	conf_ctx = apache2_worker_impl->conf_ctx;
    url = axis2_url_create(env, "http",
                        (axis2_char_t*)ap_get_server_name(request), 
                        ap_get_server_port(request), request->unparsed_uri);
	if(NULL == conf_ctx)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
                          AXIS2_FAILURE);
        return AXIS2_CRTICAL_FAILURE;
	}
    content_length = request->remaining;
    http_version = request->protocol;
    req_url = AXIS2_URL_TO_EXTERNAL_FORM(url, env);
    
    content_type = (axis2_char_t*)apr_table_get(request->headers_in, 
                        AXIS2_HTTP_HEADER_CONTENT_TYPE);
    request->content_type = content_type;
    if(request->read_chunked == AXIS2_TRUE && 0 == content_length)
    {
        content_length = -1;
        request->chunked = 1;
    }
	if(NULL == http_version)
	{
		AXIS2_ERROR_SET((*env)->error, AXIS2_ERROR_NULL_HTTP_VERSION, 
						AXIS2_FAILURE);
        return AXIS2_CRTICAL_FAILURE;
	}
    out_stream = axis2_stream_create_basic(env);
	AXIS2_LOG_DEBUG((*env)->log, AXIS2_LOG_SI, "Client HTTP version %s", 
						http_version);
	
	encoding_header_value = (axis2_char_t*)request->content_encoding;

    transport_qname = axis2_qname_create(env, AXIS2_TRANSPORT_HTTP, NULL,
                        NULL);		
	out_desc = AXIS2_CONF_GET_TRANSPORT_OUT(AXIS2_CONF_CTX_GET_CONF
						(apache2_worker_impl->conf_ctx, env), env, 
                        transport_qname);
	in_desc = AXIS2_CONF_GET_TRANSPORT_IN(AXIS2_CONF_CTX_GET_CONF
						(apache2_worker_impl->conf_ctx, env), env, 
						transport_qname);
    AXIS2_QNAME_FREE(transport_qname, env);

	msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
	AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);
	
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_stream_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, out_stream);
    AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_TRANSPORT_OUT, property,
                               AXIS2_FALSE);
    
	/*AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_TRANSPORT_HEADERS, 
						axis2_apache2_worker_get_headers(apache2_worker, env, 
                        simple_request), AXIS2_FALSE);*/
    ctx_uuid = axis2_uuid_gen(env);
	AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env, ctx_uuid);
    AXIS2_FREE((*env)->allocator, ctx_uuid);
    
    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, 
                        axis2_apache2_out_transport_info_free_void_arg);
    apache2_out_transport_info = axis2_apache2_out_transport_info_create(env, 
                        request);
    AXIS2_PROPERTY_SET_VALUE(property, env, apache2_out_transport_info);
    AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO,
                        property, AXIS2_FALSE);
    
    soap_action = (axis2_char_t*)apr_table_get(request->headers_in, 
                        AXIS2_HTTP_HEADER_SOAP_ACTION);
    request_body = axis2_stream_create_apache2(env, request);
    if(NULL == request_body)
    {
        AXIS2_LOG_ERROR((*env)->log, AXIS2_LOG_SI, "Error occured in"
                " creating input stream.");
        return AXIS2_CRTICAL_FAILURE;
    }
    if(M_GET == request->method_number)
	{
		processed = axis2_http_transport_utils_process_http_get_request
                        (env, msg_ctx, request_body, out_stream,
                        content_type ,soap_action,
						req_url,
                        conf_ctx, 
                        axis2_http_transport_utils_get_request_params(env,
                        (axis2_char_t*)req_url));
		if(AXIS2_FALSE == processed)
		{
			body_string = axis2_http_transport_utils_get_services_html(env, 
						conf_ctx);
            request->content_type = "text/html";
            /*axis2_apache2_worker_set_response_headers(apache2_worker, env, svr_conn,
            simple_request, response, 0);*/
            send_status = OK;			
		}
		
	}
    else if(M_POST == request->method_number)
	{
        axis2_status_t status = AXIS2_FAILURE;
		status = axis2_http_transport_utils_process_http_post_request
                        (env, msg_ctx, request_body, out_stream,
                        content_type , content_length,
                        soap_action,
                        (axis2_char_t*)req_url);
		if(status == AXIS2_FAILURE)
		{
			axis2_msg_ctx_t *fault_ctx = NULL;
			axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
			if(NULL == engine)
			{
                send_status =  HTTP_INTERNAL_SERVER_ERROR;
			}
			fault_ctx = AXIS2_ENGINE_CREATE_FAULT_MSG_CTX(engine, env, msg_ctx);
            AXIS2_ENGINE_SEND_FAULT(engine, env, fault_ctx);
            body_string = axis2_apache2_worker_get_bytes(env, out_stream);
            send_status =  HTTP_INTERNAL_SERVER_ERROR;
            AXIS2_MSG_CTX_FREE(fault_ctx, env);
		}
	}
    if(-1 == send_status)
    {
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        if(NULL != op_ctx)
        {
            axis2_ctx_t *ctx = AXIS2_OP_CTX_GET_BASE(AXIS2_MSG_CTX_GET_OP_CTX(
                                    msg_ctx, env), env);
            if (NULL != ctx)
            {
                property = AXIS2_CTX_GET_PROPERTY(ctx, env, 
                        AXIS2_RESPONSE_WRITTEN, AXIS2_FALSE);
                if(property)
                {
                    ctx_written = AXIS2_PROPERTY_GET_VALUE(property, env);
                    property = NULL;
                }
            }
        }
        if(NULL != ctx_written && AXIS2_STRCASECMP(ctx_written, "TRUE") == 0)
        {
            send_status = OK;
            body_string = axis2_apache2_worker_get_bytes(env, out_stream);
        }
        else
        {
            send_status = HTTP_ACCEPTED;
        }
    }
    if(NULL != body_string)
    {
        ap_rputs(body_string, request);
        AXIS2_FREE((*env)->allocator, body_string);
        body_string = NULL;
    }
    if(NULL != url)
    {
        AXIS2_URL_FREE(url, env);
        url = NULL;
    }
    if(NULL != req_url)
    {
        AXIS2_FREE((*env)->allocator, req_url);
        req_url = NULL;
    }
    if(NULL != request_body)
    {
        AXIS2_STREAM_FREE(request_body, env);
        request_body = NULL;
    }
    AXIS2_MSG_CTX_FREE(msg_ctx, env);
    msg_ctx = NULL;
	return send_status;
}

AXIS2_DECLARE(axis2_char_t*)
axis2_apache2_worker_get_bytes(axis2_env_t **env, 
                        axis2_stream_t *stream)
{
    
    axis2_stream_t *tmp_stream = NULL;
    int return_size = -1;
    axis2_char_t *buffer = NULL;
    
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK((*env)->error, stream, NULL);
    
    tmp_stream = axis2_stream_create_basic(env);
    while(1)
    {
        int read = 0;
        int write = 0;
        
        char buf[READ_SIZE];
        read = AXIS2_STREAM_READ(stream, env, buf, READ_SIZE);
        if(read < 0)
        {
            break;
        }
        write = AXIS2_STREAM_WRITE(tmp_stream, env, buf, read);
        if(read < (READ_SIZE -1))
        {
            break;
        }
    }
    return_size = AXIS2_STREAM_BASIC_GET_LEN(tmp_stream, env);

    if(return_size > 0)
    {
        buffer = (char *)AXIS2_MALLOC((*env)->allocator, sizeof(char)*
                (return_size +2));
        return_size = AXIS2_STREAM_READ(tmp_stream, env, buffer,
                                        return_size + 1);
        buffer[return_size + 1] = '\0';
    }
    AXIS2_STREAM_FREE(tmp_stream, env);
    return buffer;
}

