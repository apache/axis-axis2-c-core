
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
#include <axutil_string.h>
#include <axis2_msg_ctx.h>
#include <axis2_http_out_transport_info.h>
#include <axis2_http_transport_utils.h>
#include <axis2_op_ctx.h>
#include <axis2_engine.h>
#include <axutil_uuid_gen.h>
#include <axis2_conf_init.h>
#include <axutil_url.h>
#include <axiom_soap.h>
#include "axis2_iis_out_transport_info.h"
#include "axis2_iis_stream.h"
#include "axis2_iis_worker.h"

/* Files from iis */
#include <httpfilt.h>
#include <httpext.h>

#include "axis2_iis_constants.h"

#define READ_SIZE  2048

axis2_status_t AXIS2_CALL 
axis2_worker_get_original_url(char url[],
                              char ret_url[]);


axis2_char_t * AXIS2_CALL 
axis2_iis_worker_get_bytes(const axutil_env_t * env,
                           axutil_stream_t * stream);


axis2_status_t AXIS2_CALL 
start_response(const axutil_env_t * env, 
			   LPEXTENSION_CONTROL_BLOCK lpECB,
               int status, 
               const char *reason,
               axutil_array_list_t *headers
               );


axis2_status_t 
write_response(LPEXTENSION_CONTROL_BLOCK lpECB, 
               const void *b,
               unsigned int l);

axutil_hash_t *axis2_iis_worker_read_http_headers(const axutil_env_t * env, LPEXTENSION_CONTROL_BLOCK lpECB);

AXIS2_IMPORT extern axis2_char_t *axis2_request_url_prefix;

static struct reasons
{    
    axis2_char_t * status_code;
    int status_len;
} reasons[] = {
    {"200 OK", 6}, 
    {"202 Accepted", 12}, 
    {"500 Internal Server Error", 25} 
};

struct axis2_iis_worker 
{
    axis2_conf_ctx_t * conf_ctx;
};

char *status_reason(int status);

axis2_iis_worker_t * AXIS2_CALL 
axis2_iis_worker_create(const axutil_env_t * env,
                        axis2_char_t * repo_path) 
{
    axis2_iis_worker_t * iis_worker = NULL;

    iis_worker = (axis2_iis_worker_t *) AXIS2_MALLOC(env->allocator, sizeof(axis2_iis_worker_t));
    if (!iis_worker)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    iis_worker->conf_ctx = axis2_build_conf_ctx(env, repo_path);
    if (!iis_worker->conf_ctx)
    {        
        axis2_iis_worker_free((axis2_iis_worker_t *) iis_worker, env);
        return NULL;
    }
    return iis_worker;
}


void AXIS2_CALL 
axis2_iis_worker_free(axis2_iis_worker_t * iis_worker,
                      const axutil_env_t * env) 
{
    if (iis_worker->conf_ctx)
    {
        axis2_conf_ctx_free(iis_worker->conf_ctx, env);
        iis_worker->conf_ctx = NULL;
    }
    AXIS2_FREE(env->allocator, iis_worker);
    return;
}


int AXIS2_CALL 
axis2_iis_worker_process_request(axis2_iis_worker_t * iis_worker,
                                 const axutil_env_t * env,
                                 LPEXTENSION_CONTROL_BLOCK lpECB) 
{
    axis2_conf_ctx_t * conf_ctx = NULL;
    axutil_stream_t * out_stream = NULL;
    axis2_transport_out_desc_t * out_desc = NULL;
    axis2_transport_in_desc_t * in_desc = NULL;
    axis2_char_t soap_action[INTERNET_MAX_URL_LENGTH];    
    axis2_char_t original_url[INTERNET_MAX_URL_LENGTH];
    axis2_char_t req_url[INTERNET_MAX_URL_LENGTH];
    DWORD cbSize = 0;
    CHAR server_name[MAX_SERVERNAME];
    axis2_char_t port[MAX_TCP_PORT_LEN];
    axis2_char_t redirect_url[INTERNET_MAX_PATH_LENGTH];
	axis2_char_t accept_language[INTERNET_MAX_PATH_LENGTH];
    axutil_hash_t *headers = NULL;
    axis2_char_t peer_ip[50];
	axis2_char_t accept_header[INTERNET_MAX_URL_LENGTH];
	axis2_char_t accept_charset[INTERNET_MAX_URL_LENGTH];
    /*axutil_property_t *peer_property = NULL;*/
	
	axis2_http_header_t *content_type_header = NULL;
	axis2_http_header_t *content_length_header = NULL;

	/* New Code variables */
	axis2_http_transport_in_t request;
	axis2_http_transport_out_t response;

	/* initialize tranport in structure */
	axis2_http_transport_utils_transport_in_init(&request, env);

	/* initialize tranport out structure */
	axis2_http_transport_utils_transport_out_init(&response, env);

    soap_action[0] = '\0';
    
    /*Check the parameters*/
    if (!lpECB)
    {
        AXIS2_ERROR_SET_ERROR_NUMBER(env->error, AXIS2_ERROR_INVALID_NULL_PARAM);
        return AXIS2_FAILURE;
    }
    conf_ctx = iis_worker->conf_ctx;
    if (!conf_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    cbSize = INTERNET_MAX_PATH_LENGTH;
    if (lpECB->GetServerVariable(lpECB->ConnID,  "SERVER_NAME",  server_name, &cbSize) == FALSE)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot get server name from IIS.");        
		return AXIS2_FAILURE;
	}
    cbSize = MAX_TCP_PORT_LEN;
	if (lpECB->GetServerVariable(lpECB->ConnID, "SERVER_PORT", port, &cbSize) == FALSE)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot get server port from IIS.");        
		return AXIS2_FAILURE;
	}
    request.svr_port = port;

    cbSize = INTERNET_MAX_PATH_LENGTH;
    if(lpECB->GetServerVariable(lpECB->ConnID, "HTTP_URL", redirect_url, &cbSize) == FALSE)
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot get server port from IIS.");        
		return AXIS2_FAILURE;
	}

	/* We have a mapped URL only when the server version is 5 or less than that. */
	if (server_version <= 5)
	{
		axis2_worker_get_original_url(redirect_url, original_url);
		/* create the url using the above variables */                     
		sprintf(req_url, "%s%s%s%s%s", "http://", server_name, ":", port, original_url);
	}
	else
	{
		sprintf(req_url, "%s%s%s%s%s", "http://", server_name, ":", port, redirect_url);
	}
	/* Set the request url */
	request.request_uri = req_url;

    out_stream = axutil_stream_create_basic(env);
    out_desc = axis2_conf_get_transport_out(
        axis2_conf_ctx_get_conf (iis_worker->conf_ctx, env), env, AXIS2_TRANSPORT_ENUM_HTTP);
    in_desc = axis2_conf_get_transport_in(
        axis2_conf_ctx_get_conf (iis_worker->conf_ctx, env), env, AXIS2_TRANSPORT_ENUM_HTTP);

	/* Create the in message context */
    request.msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
    axis2_msg_ctx_set_server_side(request.msg_ctx, env, AXIS2_TRUE);
    axis2_msg_ctx_set_transport_out_stream(request.msg_ctx, env, out_stream);
    
	/* Get the SOAPAction Header */
	cbSize = INTERNET_MAX_URL_LENGTH;
    if (lpECB->GetServerVariable(lpECB->ConnID, "HTTP_SOAPAction", soap_action, &cbSize))
    {		
		request.soap_action = soap_action;
    }

	/* Create the in stream */
    request.in_stream = axutil_stream_create_iis(env, lpECB);
    if (!request.in_stream)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occured in creating input stream.");
        return AXIS2_FAILURE;
    }

	/* Get the Remote Adrress */
    if (lpECB->GetServerVariable(lpECB->ConnID, "REMOTE_ADDR", peer_ip, &cbSize))
	{
		request.remote_ip = peer_ip;
	}    

    /* Set the http headers into the message context */
    headers = axis2_iis_worker_read_http_headers(env, lpECB);
    if (axis2_msg_ctx_set_transport_headers(request.msg_ctx, env, headers) == AXIS2_FAILURE)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "IIS: Error occured in setting transport headers.");
    }
	/* Set the content length */
	request.content_length = lpECB->cbTotalBytes;
	/* Set the HTTP method */
	if (axutil_strcasecmp(lpECB->lpszMethod, "POST") == 0)
	{
		request.request_method = AXIS2_HTTP_METHOD_POST;
	}
	else if (axutil_strcasecmp(lpECB->lpszMethod, "GET") == 0)
	{
		request.request_method = AXIS2_HTTP_METHOD_GET;
	}
	else if (axutil_strcasecmp(lpECB->lpszMethod, "HEAD") == 0)
	{
		request.request_method = AXIS2_HTTP_METHOD_HEAD;
	}
	else if (axutil_strcasecmp(lpECB->lpszMethod, "PUT") == 0)
	{
		request.request_method = AXIS2_HTTP_METHOD_PUT;
	}
	else if (axutil_strcasecmp(lpECB->lpszMethod, "DELETE") == 0)
	{
		request.request_method = AXIS2_HTTP_METHOD_DELETE;
	}
	else
	{
		AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "IIS: Unsupported HTTP Method.");
		return AXIS2_FAILURE;
	}
	/* Set the URL prefix. axis2_request_url_prefix is a global variable set at the init time */
	request.request_url_prefix = axis2_request_url_prefix;
	/* Create the transport out info */
	request.out_transport_info = axis2_iis_out_transport_info_create(env, lpECB);
	/* Set the content type */
	request.content_type = lpECB->lpszContentType;
	
	/* Get accept headaer */
	cbSize = INTERNET_MAX_PATH_LENGTH;
    if(lpECB->GetServerVariable(lpECB->ConnID, "HTTP_Accept", accept_header, &cbSize))
	{
		request.accept_header = accept_header;
	}	
	
	/* Get the accept langauge */
	cbSize = INTERNET_MAX_PATH_LENGTH;
    if (lpECB->GetServerVariable(lpECB->ConnID, "HTTP_Accept-Language", accept_language, &cbSize))
	{
		request.accept_language_header = accept_language;
	}	
	
	cbSize = INTERNET_MAX_PATH_LENGTH;
    if (lpECB->GetServerVariable(lpECB->ConnID, "HTTP_Accept-Charset", accept_charset, &cbSize))
	{
		request.accept_charset_header = accept_charset;
	}

	/* Now we have set everything. We can call process method to process the request */
	if (axis2_http_transport_utils_process_request(env, conf_ctx, &request, &response) == AXIS2_FAILURE)
	{
		return AXIS2_FAILURE;
	}
		
	/* Write the response */
	if (response.response_data && response.response_data_length > 0)
	{	
		axis2_char_t content_length_str[16]={0}; 
		axis2_bool_t is_out_headers_created = AXIS2_FALSE;
		if (!response.output_headers)
		{
			response.output_headers = axutil_array_list_create(env, 2);
			is_out_headers_created = AXIS2_TRUE;
		}
		sprintf(content_length_str, "%d", response.response_data_length);
		if (!response.content_type)
		{
			content_type_header = axis2_http_header_create(env, "Content-Type", axis2_iis_out_transport_get_content(request.out_transport_info));
		}
		else
		{
			content_type_header = axis2_http_header_create(env, "Content-Type", response.content_type);
		}
		content_length_header = axis2_http_header_create(env, "Content-Length", content_length_str);
		axutil_array_list_add(response.output_headers, env, content_length_header);
		axutil_array_list_add(response.output_headers, env, content_type_header);
		/* Write the headers */
		start_response(env, lpECB, response.http_status_code, response.http_status_code_name, response.output_headers);
		/* Write the response body */
		if(write_response(lpECB, response.response_data, response.response_data_length) == AXIS2_FAILURE)
		{
			AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "IIS: Writing data to IIS");
			return AXIS2_FAILURE;
		}
		if (is_out_headers_created)
		{
			if (content_length_header)
			{
				axis2_http_header_free(content_length_header, env);
			}
			if (content_type_header)
			{
				axis2_http_header_free(content_type_header, env);
			}
			axutil_array_list_free(response.output_headers, env);
		}
	}
	else
	{
		/* If we don't have a body we should write the HTTP headers */
		start_response(env, lpECB, response.http_status_code, response.http_status_code_name, response.output_headers);
		AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Response is NULL");
	}
	
	/* Do some cleaning */
	axis2_http_transport_utils_transport_in_uninit(&request, env);
	axis2_http_transport_utils_transport_out_uninit(&response, env);
	return AXIS2_SUCCESS;
}



axis2_status_t write_response(LPEXTENSION_CONTROL_BLOCK lpECB, const void *b,
                              unsigned int l) 
{
    if (lpECB && b)
    {
        if (l)
        {
            unsigned int written = 0;
            char *buf = (char *) b;
            /* If couldn't write the data at onece try again until all the data is written.*/
            while (written < l)
            {
                DWORD try_to_write = l - written;
                if (!lpECB->
                    WriteClient(lpECB->ConnID, 
                    buf + written, &try_to_write,
                    0))
                {
                    return AXIS2_FAILURE;
                }
                written += try_to_write;
            }
        }
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}



axis2_status_t AXIS2_CALL start_response(const axutil_env_t *env, 
										 LPEXTENSION_CONTROL_BLOCK lpECB,
                                         int status, 
                                         const char *reason,
                                         axutil_array_list_t *headers
                                         ) 
{
    static char crlf[3] = { (char) 13, (char) 10, '\0' };
	unsigned int num_of_headers = 0;

    if (status < 100 || status > 1000)
    {
        return AXIS2_FAILURE;
    }
    if (lpECB)
    {
        size_t len_of_status;
        char *status_str;
        char *headers_str;

        /*
        * Create the status line
        */ 
        if (reason)
        {
            status_str = (char *) _alloca((6 + strlen(reason)) * sizeof(char));
            sprintf(status_str, "%d %s", status, reason);
            len_of_status = strlen(status_str);
        }
        else
        {
            switch (status)
            {
            case 200:
                status_str = reasons[0].status_code;
                len_of_status = reasons[0].status_len;
                break;
            case 202:
                status_str = reasons[1].status_code;
                len_of_status = reasons[1].status_len;
                break;
            case 500:
                status_str = reasons[2].status_code;
                len_of_status = reasons[2].status_len;
                break;
            default:
                status_str = reasons[0].status_code;
                len_of_status = reasons[0].status_len;
                break;
            }
        }
        /*
        * Create response headers string
        */ 
        if (headers && (num_of_headers = axutil_array_list_size(headers, env)) > 0)
        {
            size_t i, len_of_headers;
			axis2_http_header_t *header = NULL;
            for (i = 0, len_of_headers = 0; i < num_of_headers; i++)
            {
				header = axutil_array_list_get(headers, env, (int)i);
                len_of_headers += strlen(axis2_http_header_get_name(header, env));
                len_of_headers += strlen(axis2_http_header_get_value(header, env));
                len_of_headers += 4;   /* extra for colon, space and crlf */
            }
            len_of_headers += 3;  /* crlf and terminating null char */
            headers_str = (char *) _alloca(len_of_headers * sizeof(char));
            headers_str[0] = '\0';
            for (i = 0; i < num_of_headers; i++)
            {
				header = axutil_array_list_get(headers, env, (int)i);
                strcat(headers_str, axis2_http_header_get_name(header, env));
                strcat(headers_str, ": ");
                strcat(headers_str, axis2_http_header_get_value(header, env));
                strcat(headers_str, "\r\n");
            }
            strcat(headers_str, "\r\n");
        }
        else
        {
            headers_str = crlf;
        }
        if (!lpECB->
            ServerSupportFunction(lpECB->ConnID,
            HSE_REQ_SEND_RESPONSE_HEADER, 
            status_str,
            (LPDWORD) & len_of_status,
            (LPDWORD) headers_str))
        {
            return AXIS2_FAILURE;
        }
        return AXIS2_SUCCESS;
    }
    return AXIS2_FAILURE;
}

axis2_status_t AXIS2_CALL axis2_worker_get_original_url(char url[],
                                                        char ret_url[]) 
{
	extern axis2_char_t *axis2_location;
	strcpy(ret_url, axis2_location);
	strcat(ret_url, &url[25]);
    return URI_MATCHED;
}

axis2_char_t * AXIS2_CALL 
axis2_iis_worker_get_bytes(
                           const axutil_env_t * env,
                           axutil_stream_t * stream) 
{
    axutil_stream_t * tmp_stream = NULL;
    int return_size = -1;
    axis2_char_t * buffer = NULL;
    axis2_bool_t loop_state = AXIS2_TRUE;
    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, stream, NULL);

    tmp_stream = axutil_stream_create_basic(env);
    while (loop_state)
    {
        int read = 0;
        int write = 0;
        char buf[READ_SIZE];

        read = axutil_stream_read(stream, env, buf, READ_SIZE);
        if (read < 0)
        {
            break;
        }
        write = axutil_stream_write(tmp_stream, env, buf, read);
        if (read < (READ_SIZE - 1))
        {
            break;
        }
    }
    return_size = axutil_stream_get_len(tmp_stream, env);
    if (return_size > 0)
    {
        buffer = (char *) AXIS2_MALLOC(env->allocator, sizeof(char) * 
            (return_size + 2));
        return_size =
            axutil_stream_read(tmp_stream, env, buffer, 
            return_size + 1);
        buffer[return_size + 1] = '\0';
    }
    axutil_stream_free(tmp_stream, env);
    return buffer;
}

/** Read all HTTP headers.
  */
axutil_hash_t *axis2_iis_worker_read_http_headers(const axutil_env_t * env, 
												  LPEXTENSION_CONTROL_BLOCK lpECB)
{
    const char szHTTP_[] = "HTTP_";
    char szBuffer[4096];
    DWORD dwBufferSize = sizeof szBuffer;
    axutil_hash_t *headers = NULL;
    axis2_http_header_t* http_header = NULL;

    BOOL bGet = lpECB->GetServerVariable(lpECB->ConnID, "ALL_HTTP", szBuffer, &dwBufferSize);    
    if (bGet)
    {
        /* Find lines, split key/data pair and write them as output */
        LPTSTR pOpts = NULL;
        LPTSTR pEnd = NULL;
        LPTSTR pChar = NULL;
        char szTmpBuf[512];
        char szTmpName[256];

        headers = axutil_hash_make(env);
        szTmpBuf[0] = 0;
        for (pChar = szBuffer; '\0' != *pChar;)
        {
            if (*pChar == '\r' || *pChar == '\n')
            {
                pChar++;
                continue;
            }
            pOpts = strchr(pChar, ':');/* findseparator */
            if (pOpts && *pOpts)
            {
                pEnd = pOpts;
                while (*pEnd && *pEnd != '\r' && *pEnd != '\n')
                {
                    pEnd++;
                }
                *pOpts = '\0'; /* split the strings */
                *pEnd = '\0';
                if (0 == strncmp(pChar, szHTTP_, strlen(szHTTP_)))
                {
                  pChar += strlen(szHTTP_);
                }
                strcpy(szTmpName, pChar);
                axutil_string_replace(szTmpName, '_', '-');
                http_header = axis2_http_header_create(env, szTmpName, pOpts + 1);
                axutil_hash_set(headers, axutil_strdup(env, szTmpName), AXIS2_HASH_KEY_STRING, http_header);                
                pChar = pEnd + 1;
            }
        }
    }
    else
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "axis2_iis_worker_read_http_headers: no http headers");
    }
    return headers;
}

