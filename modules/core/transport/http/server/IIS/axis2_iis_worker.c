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
#include <axis2_url.h>

#include "axis2_iis_out_transport_info.h"
#include "axis2_iis_stream.h"
#include "axis2_iis_worker.h"

// Files from iis
#include <httpfilt.h>
#include <httpext.h>

#include "axis2_iis_constants.h"

#define READ_SIZE  32

typedef struct axis2_iis_worker_impl
{
    axis2_iis_worker_t iis_worker;
    axis2_conf_ctx_t *conf_ctx;
}
axis2_iis_worker_impl_t;


#define AXIS2_INTF_TO_IMPL(iis_worker) ((axis2_iis_worker_impl_t *)\
                        (iis_worker))
int AXIS2_CALL
axis2_iis_worker_process_request(
    axis2_iis_worker_t *iis_worker,
    const axis2_env_t *env,
    LPEXTENSION_CONTROL_BLOCK lpECB);

axis2_char_t *AXIS2_CALL
axis2_iis_worker_get_bytes(
    const axis2_env_t *env,
    axis2_stream_t *stream);

axis2_status_t AXIS2_CALL
axis2_iis_worker_free(
    axis2_iis_worker_t *iis_worker,
    const axis2_env_t *env);

axis2_status_t axis2_worker_get_original_url(char url[], char ret_url[]);
axis2_status_t write_response(LPEXTENSION_CONTROL_BLOCK lpECB, const void *b, unsigned int l);
axis2_status_t start_response(LPEXTENSION_CONTROL_BLOCK lpECB,
                                    int status,
                                    const char *reason,
                                    const char *const *header_names,
                                    const char *const *header_values,
                                    unsigned int num_of_headers);
char *status_reason(int status);

axis2_iis_worker_t * AXIS2_CALL
axis2_iis_worker_create(
    const axis2_env_t *env, 
    axis2_char_t *repo_path)
{
    axis2_iis_worker_impl_t *iis_worker_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    iis_worker_impl = (axis2_iis_worker_impl_t *)
            AXIS2_MALLOC(env->allocator, sizeof(axis2_iis_worker_impl_t));

    if (NULL == iis_worker_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    iis_worker_impl->iis_worker.ops = NULL;
    iis_worker_impl->conf_ctx = build_conf_ctx(env, repo_path);

    if (NULL == iis_worker_impl->conf_ctx)
    {
        axis2_iis_worker_free((axis2_iis_worker_t *)iis_worker_impl,
                env);
        return NULL;
    }
    iis_worker_impl->iis_worker.ops = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_iis_worker_ops_t));

    if (NULL == iis_worker_impl->iis_worker.ops)
    {
        axis2_iis_worker_free((axis2_iis_worker_t *)iis_worker_impl,
                env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

	// Set the operations to the functions
    iis_worker_impl->iis_worker.ops->process_request =
        axis2_iis_worker_process_request;
    iis_worker_impl->iis_worker.ops->free = axis2_iis_worker_free;

    return &(iis_worker_impl->iis_worker);	
}

axis2_status_t AXIS2_CALL
axis2_iis_worker_free(
    axis2_iis_worker_t *iis_worker,
    const axis2_env_t *env)
{
    axis2_iis_worker_impl_t *worker_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);

    worker_impl = AXIS2_INTF_TO_IMPL(iis_worker);
    if (worker_impl->conf_ctx)
    {
        AXIS2_CONF_CTX_FREE(worker_impl->conf_ctx, env);
        worker_impl->conf_ctx = NULL;
    }

    if (iis_worker->ops)
        AXIS2_FREE(env->allocator, iis_worker->ops);

    AXIS2_FREE(env->allocator, worker_impl->conf_ctx);

    return AXIS2_SUCCESS;
}

int AXIS2_CALL
axis2_iis_worker_process_request(
    axis2_iis_worker_t *iis_worker,
    const axis2_env_t *env,
    LPEXTENSION_CONTROL_BLOCK lpECB)
{
	axis2_iis_worker_impl_t *iis_worker_impl = NULL;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_stream_t *request_body = NULL;
    axis2_stream_t *out_stream = NULL;
    axis2_transport_out_desc_t *out_desc = NULL;
    axis2_transport_in_desc_t *in_desc = NULL;
    axis2_bool_t processed = AXIS2_FALSE;
    axis2_char_t *ctx_written = NULL;
    //int content_length = -1;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_char_t *req_url = NULL;
    axis2_char_t *body_string = NULL;
    int send_status = -1;
    axis2_property_t *property = NULL;
    axis2_url_t *url = NULL;
    axis2_http_out_transport_info_t *iis_out_transport_info = NULL;
    axis2_qname_t *transport_qname = NULL;
    axis2_char_t *ctx_uuid = NULL;

	axis2_char_t http_version[MAX_HTTP_VERSION_LEN];
    axis2_char_t soap_action[INTERNET_MAX_URL_LENGTH];
	axis2_char_t content_type[MAX_HTTP_CONTENT_TYPE_LEN];
	axis2_char_t encoding_header_value[MAX_CONTENT_ENCODING_LEN];
	axis2_char_t http_method[MAX_HTTP_METHOD_LEN];
	axis2_char_t content_length[MAX_CONTENT_LEN];
	axis2_char_t original_url[INTERNET_MAX_URL_LENGTH];

	char readBytes[10000];
	int content_length_int = 0;

	DWORD ret_val = 0;
	DWORD cbSize = 0;
	CHAR server_name[MAX_SERVERNAME]; 


	axis2_char_t port[MAX_TCP_PORT_LEN];
	CHAR redirect_url[INTERNET_MAX_PATH_LENGTH];

	// Check the parameters
    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    AXIS2_PARAM_CHECK(env->error, lpECB, AXIS2_CRITICAL_FAILURE);

	iis_worker_impl = AXIS2_INTF_TO_IMPL(iis_worker);
	conf_ctx = iis_worker_impl->conf_ctx;	

	if (NULL == conf_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
                AXIS2_FAILURE);
        return AXIS2_CRITICAL_FAILURE;
    }	
	// TODO check for errors
	cbSize = INTERNET_MAX_PATH_LENGTH; 
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, 
                         "SERVER_NAME", 
                         server_name, 
                         &cbSize); 	

	cbSize = MAX_TCP_PORT_LEN;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, "SERVER_PORT", port, &cbSize);

	cbSize = INTERNET_MAX_PATH_LENGTH;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, "HTTP_URL", redirect_url, &cbSize);

	axis2_worker_get_original_url(redirect_url, original_url);
	// create the url using the above variables
	url = axis2_url_create(env, "http",
						server_name, 
						atoi(port), 
						original_url);								

	cbSize = MAX_CONTENT_LEN;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, "CONTENT_LENGTH", content_length, &cbSize);
	
	// Retrive the http version as a string e.g HTTP/1.1
	cbSize = MAX_HTTP_VERSION_LEN;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, "HTTP_VERSION", http_version, &cbSize);

	req_url = AXIS2_URL_TO_EXTERNAL_FORM(url, env);			
		
	// Retrieve the http content type as a string e.g GET or POST 
	cbSize = MAX_HTTP_CONTENT_TYPE_LEN;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, "CONTENT_TYPE", content_type, &cbSize);
	
	// Retrieve the encoding of the http request
	cbSize = MAX_CONTENT_ENCODING_LEN;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, "CONTENT_ENCODING", encoding_header_value, &cbSize);  

    out_stream = axis2_stream_create_basic(env);	
 
    out_desc = AXIS2_CONF_GET_TRANSPORT_OUT(AXIS2_CONF_CTX_GET_CONF
            (iis_worker_impl->conf_ctx, env), env,
            AXIS2_TRANSPORT_ENUM_HTTP);
    in_desc = AXIS2_CONF_GET_TRANSPORT_IN(AXIS2_CONF_CTX_GET_CONF
            (iis_worker_impl->conf_ctx, env), env,
            AXIS2_TRANSPORT_ENUM_HTTP);

	msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
    AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_APPLICATION);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_stream_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, out_stream);
    AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_TRANSPORT_OUT, property,
            AXIS2_FALSE);
	//axis2_msg_ctx_set_transport_out_stream(msg_ctx, env, out_stream);

	ctx_uuid = axis2_uuid_gen(env);
    AXIS2_MSG_CTX_SET_SVC_GRP_CTX_ID(msg_ctx, env, ctx_uuid);
    AXIS2_FREE(env->allocator, ctx_uuid);

    property = axis2_property_create(env);	
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_APPLICATION);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env,
            axis2_iis_out_transport_info_free_void_arg);
    iis_out_transport_info = axis2_iis_out_transport_info_create(env, lpECB);
    AXIS2_PROPERTY_SET_VALUE(property, env, iis_out_transport_info);
    AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_HTTP_OUT_TRANSPORT_INFO,
            property, AXIS2_FALSE);

	
	cbSize = INTERNET_MAX_URL_LENGTH;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, AXIS2_HTTP_HEADER_SOAP_ACTION, soap_action, &cbSize);
	request_body = axis2_stream_create_iis(env, lpECB);
	//request_body->ops->read(request_body, env, readBytes, content_length_int);
	if (NULL == request_body)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occured in"
                " creating input stream.");
        return AXIS2_CRITICAL_FAILURE;
    }
	
	cbSize = MAX_HTTP_METHOD_LEN;
	ret_val = lpECB->GetServerVariable(lpECB->ConnID, "HTTP_METHOD", http_method, &cbSize);
		
	if (stricmp(http_method, "GET") == 0)
    {
        processed = axis2_http_transport_utils_process_http_get_request
                (env, msg_ctx, request_body, out_stream,
                        content_type , soap_action,
                        req_url,
                        conf_ctx,
                        axis2_http_transport_utils_get_request_params(env,
                                (axis2_char_t *)req_url));

		// If this is not a valid GET request display the list of displayed services.
		if (processed == AXIS2_FAILURE)
        {
            body_string = axis2_http_transport_utils_get_services_html(env,
                    conf_ctx);
            iis_out_transport_info->ops->set_content_type(iis_out_transport_info, env, "text/html");
            send_status = OK;
        }
    }
    else if (stricmp(http_method, "POST") == 0)
    {
        axis2_status_t status = AXIS2_FAILURE;
		content_length_int = atoi(content_length);
        status = axis2_http_transport_utils_process_http_post_request
                (env, msg_ctx, request_body, out_stream,
                        content_type , content_length_int,
                        soap_action,
                        (axis2_char_t *)req_url);

		// generate a soap fault and send it
        if (status == AXIS2_FAILURE)
        {
            axis2_msg_ctx_t *fault_ctx = NULL;
            axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
            if (NULL == engine)
            {
                send_status =  HTTP_INTERNAL_SERVER_ERROR;
            }
            fault_ctx = AXIS2_ENGINE_CREATE_FAULT_MSG_CTX(engine, env, msg_ctx);
            AXIS2_ENGINE_SEND_FAULT(engine, env, fault_ctx);
            body_string = axis2_iis_worker_get_bytes(env, out_stream);
            send_status =  HTTP_INTERNAL_SERVER_ERROR;
            AXIS2_MSG_CTX_FREE(fault_ctx, env);
        }
    }
	// Nothing wrong has happen. So proceed with the request
    if (-1 == send_status)
    {
        op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
        if (op_ctx)
        {
            axis2_ctx_t *ctx = AXIS2_OP_CTX_GET_BASE(AXIS2_MSG_CTX_GET_OP_CTX(
                        msg_ctx, env), env);
            if (ctx)
            {
                property = AXIS2_CTX_GET_PROPERTY(ctx, env,
                        AXIS2_RESPONSE_WRITTEN, AXIS2_FALSE);
                if (property)
                {
                    ctx_written = AXIS2_PROPERTY_GET_VALUE(property, env);
                    property = NULL;
                }
            }
        }
        if (ctx_written && AXIS2_STRCASECMP(ctx_written, "TRUE") == 0)
        {
            send_status = OK;
            body_string = axis2_iis_worker_get_bytes(env, out_stream);
        }
        else
        {
            send_status = HTTP_ACCEPTED;
        }
    }

	if (body_string)
	{		
		const char *headers_names[] = {
					"Content-Type",
					"Content-Length"
					};
		const char *headers_vhtml[2];
					
		headers_vhtml[1] = (char *)malloc(16);
		headers_vhtml[0] = axis2_http_out_transport_get_content(iis_out_transport_info);
		sprintf(headers_vhtml[1], "%d", strlen(body_string));
		if(!start_response(lpECB, send_status, NULL, headers_names, headers_vhtml, 2))
		{
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occured in"
                " writing response.");			
		}

		cbSize = strlen(body_string);
		if(!write_response(lpECB, body_string, cbSize))
		{
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occured in"
                " writing response.");
		}
        AXIS2_FREE(env->allocator, body_string);
        body_string = NULL;
    } 
	else 
	{		
		if(!start_response(lpECB, send_status, NULL, NULL, NULL, 0))
		{
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occured in"
                " writing response.");			
		}
	}
		

    if (url)
    {
        AXIS2_URL_FREE(url, env);
        url = NULL;
    }
    if (req_url)
    {
        AXIS2_FREE(env->allocator, req_url);
        req_url = NULL;
    }
    if (request_body)
    {
        AXIS2_STREAM_FREE(request_body, env);
        request_body = NULL;
    }
    msg_ctx = NULL;
    return send_status;
}

axis2_status_t write_response(LPEXTENSION_CONTROL_BLOCK lpECB, const void *b, unsigned int l)
{    
    if (lpECB && b) {
        if (l) {
            unsigned int written = 0;
            char *buf = (char *)b;
			// If couldn't write the data at onece try again until all the data is written.
            while (written < l) {
                DWORD try_to_write = l - written;
                if (!lpECB->WriteClient(lpECB->ConnID,
                                           buf + written, &try_to_write, 0)) {
                    return FALSE;
                }
                written += try_to_write;
            }
        }        
        return TRUE;
    }    
    return FALSE;
}

axis2_status_t start_response(LPEXTENSION_CONTROL_BLOCK lpECB,
                                    int status,
                                    const char *reason,
                                    const char *const *header_names,
                                    const char *const *header_values,
                                    unsigned int num_of_headers)
{
    static char crlf[3] = { (char)13, (char)10, '\0' };
    
    if (status < 100 || status > 1000) {        
        return FALSE;
    }

    if (lpECB) {
        size_t len_of_status;
        char *status_str;
        char *headers_str;

        /*
         * Create the status line
         */
        if (!reason) {
            reason = status_reason(status);
        }
        status_str = (char *)_alloca((6 + strlen(reason)) * sizeof(char));
        sprintf(status_str, "%d %s", status, reason);
        len_of_status = strlen(status_str);

        /*
         * Create response headers string
         */
        if (num_of_headers) {
            size_t i, len_of_headers;
            for (i = 0, len_of_headers = 0; i < num_of_headers; i++) {
                len_of_headers += strlen(header_names[i]);
                len_of_headers += strlen(header_values[i]);
                len_of_headers += 4;        /* extra for colon, space and crlf */
            }

            len_of_headers += 3;    /* crlf and terminating null char */
            headers_str = (char *)_alloca(len_of_headers * sizeof(char));
            headers_str[0] = '\0';

            for (i = 0; i < num_of_headers; i++) {
                strcat(headers_str, header_names[i]);
                strcat(headers_str, ": ");
                strcat(headers_str, header_values[i]);
                strcat(headers_str, "\r\n");
            }
            strcat(headers_str, "\r\n");
        }
        else {
            headers_str = crlf;
        }

        if (!lpECB->ServerSupportFunction(lpECB->ConnID,
                                             HSE_REQ_SEND_RESPONSE_HEADER,
                                             status_str,
                                             (LPDWORD) &len_of_status,
                                             (LPDWORD) headers_str)) {            
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

char *status_reason(int status)
{
    static struct reasons {
        int status;
        char *reason;
    } *r, reasons[] = {
        { 100, "Continue" },
        { 101, "Switching Protocols" },
        { 200, "OK" },
        { 201, "Created" },
        { 202, "Accepted" },
        { 203, "Non-Authoritative Information" },
        { 204, "No Content" },
        { 205, "Reset Content" },
        { 206, "Partial Content" },
        { 300, "Multiple Choices" },
        { 301, "Moved Permanently" },
        { 302, "Moved Temporarily" },
        { 303, "See Other" },
        { 304, "Not Modified" },
        { 305, "Use Proxy" },
        { 400, "Bad Request" },
        { 401, "Unauthorized" },
        { 402, "Payment Required" },
        { 403, "Forbidden" },
        { 404, "Not Found" },
        { 405, "Method Not Allowed" },
        { 406, "Not Acceptable" },
        { 407, "Proxy Authentication Required" },
        { 408, "Request Timeout" },
        { 409, "Conflict" },
        { 410, "Gone" },
        { 411, "Length Required" },
        { 412, "Precondition Failed" },
        { 413, "Request Entity Too Large" },
        { 414, "Request-URI Too Long" },
        { 415, "Unsupported Media Type" },
        { 500, "Internal Server Error" },
        { 501, "Not Implemented" },
        { 502, "Bad Gateway" },
        { 503, "Service Unavailable" },
        { 504, "Gateway Timeout" },
        { 505, "HTTP Version Not Supported" },
        { 000, NULL}
    };

    r = reasons;
    while (r->status <= status)
        if (r->status == status)
            return r->reason;
        else
            r++;
    return "No Reason";
}

axis2_status_t axis2_worker_get_original_url(char url[], char ret_url[])
{
	int i = 0;
	for (i = 0; i < 7 ; i++)
		ret_url[i] = url[i];

	for (i = 7; url[i] != '\0'; i++)
		ret_url[i] = url[i + 17];

	ret_url[i] = '\0';
		
	return URI_MATCHED;
}

axis2_char_t *AXIS2_CALL
axis2_iis_worker_get_bytes(
    const axis2_env_t *env,
    axis2_stream_t *stream)
{

    axis2_stream_t *tmp_stream = NULL;
    int return_size = -1;
    axis2_char_t *buffer = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, stream, NULL);

    tmp_stream = axis2_stream_create_basic(env);
    while (1)
    {
        int read = 0;
        int write = 0;

        char buf[READ_SIZE];
        read = AXIS2_STREAM_READ(stream, env, buf, READ_SIZE);
        if (read < 0)
        {
            break;
        }
        write = AXIS2_STREAM_WRITE(tmp_stream, env, buf, read);
        if (read < (READ_SIZE - 1))
        {
            break;
        }
    }
    return_size = AXIS2_STREAM_BASIC_GET_LEN(tmp_stream, env);

    if (return_size > 0)
    {
        buffer = (char *)AXIS2_MALLOC(env->allocator, sizeof(char) *
                (return_size + 2));
        return_size = AXIS2_STREAM_READ(tmp_stream, env, buffer,
                return_size + 1);
        buffer[return_size + 1] = '\0';
    }
    AXIS2_STREAM_FREE(tmp_stream, env);
    return buffer;
}


