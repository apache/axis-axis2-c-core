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

#include "axis2_cgi_out_transport_info.h"
#include "axis2_cgi_stream.h"

#include <stdio.h>
#include <stdlib.h>
#include <axutil_error_default.h>
#include <axutil_log_default.h>
#include <axutil_thread_pool.h>
#include <axutil_types.h>
#include <axutil_version.h>
#include <axutil_file_handler.h>
#include <axutil_url.h>
#include <axutil_string.h>
#include <axutil_http_chunked_stream.h>
#include <platforms/axutil_platform_auto_sense.h>

#include <axis2_engine.h>
#include <axis2_conf_init.h>
#include <axis2_http_transport.h>
#include <axis2_http_transport_utils.h>

axis2_char_t *axis2_cgi_default_url_prefix = "/cgi-bin/axis2.cgi/";

/***************************** Function headers *******************************/

void
axis2_cgi_system_exit(
    axutil_env_t * env,
    int status);

void
axis2_cgi_set_request_vars(
    axis2_cgi_request_t *cgi_request);

axis2_status_t
axis2_cgi_write_response(
    const void *buffer,
    unsigned int length);

/***************************** End of function headers ************************/

int
main(
    int argc,
    char *argv[])
{
    axutil_allocator_t *allocator = NULL;
    axutil_env_t *env = NULL;
    const axis2_char_t *repo_path;
    axis2_conf_ctx_t *conf_ctx;
    axis2_cgi_request_t *cgi_request = NULL;
    int content_length = 0;
    axis2_char_t *request_url = NULL;
    /* axutil_stream_t *request_body_stream = NULL; */
    axutil_stream_t *out_stream = NULL;
    axis2_transport_in_desc_t *in_desc = NULL;
    axis2_transport_out_desc_t *out_desc = NULL;
    /* axis2_msg_ctx_t *msg_ctx = NULL; */
    /* axis2_bool_t processed = AXIS2_FALSE; */
    axis2_http_transport_in_t request;
    axis2_http_transport_out_t response;
    axutil_array_list_t *mime_parts = NULL;  /* AXIS2C-1404: MTOM support */
    /*axis2_http_header_t* http_header = NULL; */
    /*axutil_hash_t *headers = NULL; */
    /* axis2_char_t *axis2_cgi_url_prefix = NULL; */

    if(!(repo_path = AXIS2_GETENV("AXIS2C_HOME")))
    {
        fprintf(stderr, "Error: AXIS2C_HOME environment variable not set!");
        axis2_cgi_system_exit(env, -1);
    }

    allocator = axutil_allocator_init(NULL);
    env = axutil_env_create(allocator);

    if(axutil_file_handler_access(repo_path, AXIS2_R_OK) != AXIS2_SUCCESS)
    {
        fprintf(stderr, "Error reading repository: %s\nPlease check AXIS2C_HOME var", repo_path);
        axis2_cgi_system_exit(env, -1);
    }

    /* Set configuration */

    conf_ctx = axis2_build_conf_ctx(env, repo_path);
    if(!conf_ctx)
    {
        fprintf(stderr, "Error: Configuration not builded propertly\n");
        axis2_cgi_system_exit(env, -1);
    }

    axis2_http_transport_utils_transport_out_init(&response, env);
    axis2_http_transport_utils_transport_in_init(&request, env);

    /* Get input info */

    cgi_request = AXIS2_MALLOC(allocator, sizeof(axis2_cgi_request_t));
    axis2_cgi_set_request_vars(cgi_request);

    request_url = (axis2_char_t *)AXIS2_MALLOC(allocator, (7 + /* "http:" */
    strlen(cgi_request->server_name) + ((strlen(cgi_request->server_port)) ? 1 : 0) + /* ":"*/
    strlen(cgi_request->server_port) + strlen(cgi_request->script_name) + strlen(
        cgi_request->path_info) + ((strlen(cgi_request->path_info)) ? 1 : 0) + /* "?" */
    strlen(cgi_request->query_string)) * sizeof(axis2_char_t));

    request_url[0] = '\0';

    strcat(request_url, "http://");
    strcat(request_url, cgi_request->server_name);
    if(strlen(cgi_request->server_port))
        strcat(request_url, ":");
    strcat(request_url, cgi_request->server_port);
    strcat(request_url, cgi_request->script_name);
    strcat(request_url, cgi_request->path_info);
    if(strlen(cgi_request->query_string))
        strcat(request_url, "?");
    strcat(request_url, cgi_request->query_string);

    if(cgi_request->content_length && strlen(cgi_request->content_length) > 0)
        content_length = axutil_atoi(cgi_request->content_length);
    else
        content_length = -1;  /* AXIS2C-1404: Use -1 for unknown/chunked */

    /* Set streams */

    out_stream = axutil_stream_create_basic(env);

    /* Set message contexts */

    out_desc = axis2_conf_get_transport_out(axis2_conf_ctx_get_conf(conf_ctx, env), env,
        AXIS2_TRANSPORT_ENUM_HTTP);
    in_desc = axis2_conf_get_transport_in(axis2_conf_ctx_get_conf(conf_ctx, env), env,
        AXIS2_TRANSPORT_ENUM_HTTP);

    request.msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);

    axis2_msg_ctx_set_server_side(request.msg_ctx, env, AXIS2_TRUE);
    axis2_msg_ctx_set_transport_out_stream(request.msg_ctx, env, out_stream);
    axis2_msg_ctx_set_transport_headers(request.msg_ctx, env, NULL);

    /* Set request parameters */

    request.soap_action = cgi_request->soap_action;
    request.in_stream = axutil_stream_create_cgi(env, content_length);
    request.remote_ip = cgi_request->remote_addr;
    request.content_length = content_length;
    request.content_type = cgi_request->content_type;
    request.request_uri = request_url;
    request.svr_port = cgi_request->server_port;
    request.accept_header = AXIS2_GETENV("HTTP_ACCEPT");
    request.accept_language_header = AXIS2_GETENV("HTTP_ACCEPT_LANGUAGE");
    request.accept_charset_header = AXIS2_GETENV("HTTP_ACCEPT_CHARSET");
    request.request_url_prefix = (AXIS2_GETENV("AXIS2C_URL_PREFIX")) ? AXIS2_GETENV(
        "AXIS2C_URL_PREFIX") : axis2_cgi_default_url_prefix;

    if(axutil_strcasecmp(cgi_request->request_method, "POST") == 0)
    {
        request.request_method = AXIS2_HTTP_METHOD_POST;
    }
    else if(axutil_strcasecmp(cgi_request->request_method, "GET") == 0)
    {
        request.request_method = AXIS2_HTTP_METHOD_GET;
    }
    else if(axutil_strcasecmp(cgi_request->request_method, "HEAD") == 0)
    {
        request.request_method = AXIS2_HTTP_METHOD_HEAD;
    }
    else if(axutil_strcasecmp(cgi_request->request_method, "PUT") == 0)
    {
        request.request_method = AXIS2_HTTP_METHOD_PUT;
    }
    else if(axutil_strcasecmp(cgi_request->request_method, "DELETE") == 0)
    {
        request.request_method = AXIS2_HTTP_METHOD_DELETE;
    }

    request.out_transport_info = axis2_cgi_out_transport_info_create(env, cgi_request);

    /*Process request */
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "CGI: Processing request");
    axis2_http_transport_utils_process_request(env, conf_ctx, &request, &response);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "CGI: Done processing request");

    /*Send status */
    fprintf(stdout, "Status: %d %s" AXIS2_CRLF, response.http_status_code, response.http_status_code_name);

    if(response.content_type)
    {
        fprintf(stdout, "Content-type: %s" AXIS2_CRLF, response.content_type);
    }
    else if(axis2_cgi_out_transport_get_content(request.out_transport_info)
        && strlen(axis2_cgi_out_transport_get_content(request.out_transport_info)))
    {
        fprintf(stdout, "Content-type: %s" AXIS2_CRLF, axis2_cgi_out_transport_get_content(
            request.out_transport_info));
    }

    /* AXIS2C-1404: Handle MTOM responses with chunked transfer encoding */
    if((response.msg_ctx != NULL) &&
        (mime_parts = axis2_msg_ctx_get_mime_parts(response.msg_ctx, env)))
    {
        axutil_stream_t *stdout_stream = axutil_stream_create_file(env, stdout);
        axutil_http_chunked_stream_t *stdout_chunked_stream =
                axutil_http_chunked_stream_create(env, stdout_stream);
        axutil_param_t *callback_name_param = NULL;
        axis2_char_t *mtom_sending_callback_name = NULL;

        if((callback_name_param = axis2_msg_ctx_get_parameter(response.msg_ctx,
                    env, AXIS2_MTOM_SENDING_CALLBACK)))
        {
            mtom_sending_callback_name =
                    (axis2_char_t *)axutil_param_get_value(callback_name_param, env);
        }

        /* Write chunked transfer encoding header */
        fprintf(stdout, "Transfer-Encoding: chunked" AXIS2_CRLF AXIS2_CRLF);

        if(!axis2_http_transport_utils_send_mtom_message(stdout_chunked_stream, env,
                    mime_parts, mtom_sending_callback_name))
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "CGI: Error writing MTOM data");
            fprintf(stderr, "Error writing MTOM data\n");
        }

        /* Clean up chunked stream resources */
        if(stdout_chunked_stream)
        {
            axutil_http_chunked_stream_free(stdout_chunked_stream, env);
        }
        if(stdout_stream)
        {
            axutil_stream_free(stdout_stream, env);
        }
    }
    else
    {
        fprintf(stdout, AXIS2_CRLF); /* End of headers for server */

        /* Write data body */
        if(!axis2_cgi_write_response(response.response_data, response.response_data_length))
        {
            fprintf(stderr, "Error writing output data\n");
        }
    }

    axis2_http_transport_utils_transport_in_uninit(&request, env);
    axis2_http_transport_utils_transport_out_uninit(&response, env);

    return 0;
}

axis2_status_t
axis2_cgi_write_response(
    const void *buffer,
    unsigned int length)
{
    if(buffer && length)
    {
        size_t completed = 0;
        size_t written = 0;

        while(completed < length)
        {
            written = fwrite(buffer, sizeof(char), length - completed, stdout);
            if(written < 0)
                return AXIS2_FALSE;
            completed += written;
        }
    }
    return AXIS2_TRUE;
}

void
axis2_cgi_set_request_vars(
    axis2_cgi_request_t *cgi_request)
{
    cgi_request->server_name = (AXIS2_GETENV("SERVER_NAME")) ? AXIS2_GETENV("SERVER_NAME") : "";
    cgi_request->script_name = (AXIS2_GETENV("SCRIPT_NAME")) ? AXIS2_GETENV("SCRIPT_NAME") : "";
    cgi_request->path_info = (AXIS2_GETENV("PATH_INFO")) ? AXIS2_GETENV("PATH_INFO") : "";
    cgi_request->server_port = (AXIS2_GETENV("SERVER_PORT")) ? AXIS2_GETENV("SERVER_PORT") : "";
    cgi_request->server_protocol = (AXIS2_GETENV("SERVER_PROTOCOL")) ? AXIS2_GETENV(
        "SERVER_PROTOCOL") : "";
    cgi_request->content_length = (AXIS2_GETENV("CONTENT_LENGTH")) ? AXIS2_GETENV("CONTENT_LENGTH")
        : "";
    cgi_request->content_type = (AXIS2_GETENV("CONTENT_TYPE")) ? AXIS2_GETENV("CONTENT_TYPE") : "";
    cgi_request->request_method = (AXIS2_GETENV("REQUEST_METHOD")) ? AXIS2_GETENV("REQUEST_METHOD")
        : "";
    cgi_request->remote_addr = (AXIS2_GETENV("REMOTE_ADDR")) ? AXIS2_GETENV("REMOTE_ADDR") : "";
    cgi_request->soap_action = (AXIS2_GETENV("HTTP_SOAPACTION")) ? AXIS2_GETENV("HTTP_SOAPACTION")
        : NULL;
    cgi_request->query_string = (AXIS2_GETENV("QUERY_STRING")) ? AXIS2_GETENV("QUERY_STRING") : "";
}

void
axis2_cgi_system_exit(
    axutil_env_t * env,
    int status)
{
    axutil_allocator_t *allocator = NULL;

    if(env)
    {
        allocator = env->allocator;
        axutil_env_free(env);
    }
    exit(status);
}
