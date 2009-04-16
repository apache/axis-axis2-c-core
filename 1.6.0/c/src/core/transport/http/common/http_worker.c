
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

#include <axis2_http_worker.h>
#include <axutil_string.h>
#include <axis2_http_transport.h>
#include <axis2_conf.h>
#include <axutil_string.h>
#include <axis2_msg_ctx.h>
#include <axis2_http_request_line.h>
#include <axis2_http_out_transport_info.h>
#include <axis2_http_transport_utils.h>
#include <axis2_http_accept_record.h>
#include <axis2_op_ctx.h>
#include <axis2_engine.h>
#include <axutil_uuid_gen.h>
#include <axutil_url.h>
#include <axutil_property.h>
#include <axiom_soap.h>
#include <string.h>
#include <axutil_string_util.h>
#include <stdio.h>
#include <stdlib.h>
#include <platforms/axutil_platform_auto_sense.h>

struct axis2_http_worker
{
    axis2_conf_ctx_t *conf_ctx;
    int svr_port;
};

static axis2_status_t axis2_http_worker_set_response_headers(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env,
    axis2_simple_http_svr_conn_t * svr_conn,
    axis2_http_simple_request_t * simple_request,
    axis2_http_simple_response_t * simple_response,
    axis2_ssize_t content_length);

static axis2_status_t axis2_http_worker_set_transport_out_config(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_http_simple_response_t * simple_response);

static axutil_hash_t *axis2_http_worker_get_headers(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env,
    axis2_http_simple_request_t * request);

static axis2_char_t *axis2_http_worker_get_server_time(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env);

AXIS2_EXTERN axis2_http_worker_t *AXIS2_CALL
axis2_http_worker_create(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    axis2_http_worker_t *http_worker = NULL;

    http_worker = (axis2_http_worker_t *)
        AXIS2_MALLOC(env->allocator, sizeof(axis2_http_worker_t));

    if (!http_worker)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    http_worker->conf_ctx = conf_ctx;
    http_worker->svr_port = 9090;   /* default - must set later */

    return http_worker;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_worker_free(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env)
{
    http_worker->conf_ctx = NULL;
    AXIS2_FREE(env->allocator, http_worker);
    return;
}

/* Each in-coming request is passed into this function for process. Basically http method to deliver
 * is deduced here and call appropriate http processing function. 
 * eg. transport_utils_process_http_post_request() function. Once this fuction call done it will go
 * through engine inflow phases and finally hit the message receiver for the operation.
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_worker_process_request(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env,
    axis2_simple_http_svr_conn_t * svr_conn,
    axis2_http_simple_request_t * simple_request)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axutil_stream_t *request_body = NULL;

    /* Creating out_stream as basic stream */
    axutil_stream_t *out_stream = axutil_stream_create_basic(env);
    axis2_http_simple_response_t *response = NULL;

    /* Transport in and out descriptions */
    axis2_transport_out_desc_t *out_desc = NULL;
    axis2_transport_in_desc_t *in_desc = NULL;

    axis2_char_t *http_version = NULL;
    axis2_char_t *soap_action = NULL;
    axutil_string_t *soap_action_str = NULL;
    axis2_bool_t processed = AXIS2_FALSE;
    axis2_status_t status = AXIS2_FAILURE;
    int content_length = -1;
    axis2_http_header_t *encoding_header = NULL;
    axis2_char_t *encoding_header_value = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_char_t *svr_ip = NULL;
    axis2_char_t *peer_ip = NULL;
    axutil_url_t *request_url = NULL;
    axis2_http_out_transport_info_t *http_out_transport_info = NULL;
    axutil_hash_t *headers = NULL;
    axis2_char_t *url_external_form = NULL;
    axis2_char_t *svc_grp_uuid = NULL;
    axis2_char_t *path = NULL;
    axutil_property_t *peer_property = NULL;

    /* REST processing variables */
    axis2_bool_t is_get = AXIS2_FALSE;
    axis2_bool_t is_head = AXIS2_FALSE;
    axis2_bool_t is_put = AXIS2_FALSE;
    axis2_bool_t is_delete = AXIS2_FALSE;
    axis2_bool_t request_handled = AXIS2_FALSE;

    /* HTTP and Proxy authentication */
    axis2_char_t *accept_header_value = NULL;
    axis2_http_header_t *accept_header = NULL;
    axis2_char_t *accept_charset_header_value = NULL;
    axis2_http_header_t *accept_charset_header = NULL;
    axis2_char_t *accept_language_header_value = NULL;
    axis2_http_header_t *accept_language_header = NULL;

    axis2_char_t *http_method = NULL;
    axis2_http_request_line_t *request_line = NULL;

    axutil_hash_t *request_params = NULL;
    axis2_char_t *request_uri = NULL;
    axis2_char_t *url_ext_form = NULL;
    const axis2_char_t *content_type = NULL;

    axis2_msg_ctx_t *out_msg_ctx = NULL;
    axis2_msg_ctx_t *in_msg_ctx = NULL;
    axis2_msg_ctx_t **msg_ctx_map = NULL;


    AXIS2_PARAM_CHECK(env->error, svr_conn, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, simple_request, AXIS2_FALSE);

    conf_ctx = http_worker->conf_ctx;
    if (!conf_ctx)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT, AXIS2_FAILURE);
        return AXIS2_FALSE;
    }

    content_length =
        axis2_http_simple_request_get_content_length(simple_request, env);

    request_line = axis2_http_simple_request_get_request_line (simple_request,
                                                               env);
    if (request_line)
    {
        http_method = axis2_http_request_line_get_method(request_line, env);
    }

    http_version = axis2_http_request_line_get_http_version (request_line, env);

    if (!http_version)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_HTTP_VERSION,
                           AXIS2_FAILURE);
        return AXIS2_FALSE;
    }
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Client HTTP version %s",
                    http_version);

    response = axis2_http_simple_response_create_default(env);
    encoding_header = axis2_http_simple_request_get_first_header(
        simple_request,
        env,
        AXIS2_HTTP_HEADER_TRANSFER_ENCODING);

    if (response)
    {
        axis2_http_header_t *server = NULL;
        axis2_http_header_t *server_date = NULL;
        axis2_char_t *date_str = NULL;
        char *date_str_tmp = NULL;
        
        date_str_tmp = axis2_http_worker_get_server_time(http_worker, env);
        date_str = AXIS2_MALLOC(env->allocator,
            sizeof(axis2_char_t) * (strlen(date_str_tmp) + 5));

        if (!date_str)
        {
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY,
                               AXIS2_FAILURE);
            return AXIS2_FALSE;

        }

        sprintf(date_str, "%s GMT", date_str_tmp);

        server_date = axis2_http_header_create(env,
                                          AXIS2_HTTP_HEADER_DATE, date_str);
        if(date_str)
        {
            AXIS2_FREE(env->allocator, date_str);
            date_str = NULL;
        }
        axis2_http_simple_response_set_header(response, env, server_date);

        server = axis2_http_header_create(env,
                                          AXIS2_HTTP_HEADER_SERVER,
                                          AXIS2_HTTP_HEADER_SERVER_AXIS2C
                                          AXIS2_HTTP_SERVER);
        axis2_http_simple_response_set_header(response, env, server);
    }

    if (encoding_header)
    {
        encoding_header_value = axis2_http_header_get_value(encoding_header,
                                                            env);
    }

    if (content_length < 0 && (encoding_header_value && 0 != axutil_strcmp
                               (encoding_header_value,
                                AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED)))
    {
        if (0 == axutil_strcasecmp(http_method, AXIS2_HTTP_POST) ||
            0 == axutil_strcasecmp(http_method, AXIS2_HTTP_PUT))
        {
            axis2_http_simple_response_set_status_line(
                response, env,
                http_version,
                AXIS2_HTTP_RESPONSE_LENGTH_REQUIRED_CODE_VAL,
                AXIS2_HTTP_RESPONSE_LENGTH_REQUIRED_CODE_NAME);

            status =
                axis2_simple_http_svr_conn_write_response(svr_conn, env,
                                                          response);
            axis2_http_simple_response_free(response, env);
            response = NULL;
            return status;
        }

    }

    request_body = axis2_http_simple_request_get_body(simple_request, env);

    out_desc = axis2_conf_get_transport_out(axis2_conf_ctx_get_conf
                                            (http_worker->conf_ctx, env), env,
                                            AXIS2_TRANSPORT_ENUM_HTTP);
    in_desc = axis2_conf_get_transport_in(axis2_conf_ctx_get_conf
                                          (http_worker->conf_ctx, env), env,
                                          AXIS2_TRANSPORT_ENUM_HTTP);

    msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);

    if (0 == axutil_strcasecmp(http_version, AXIS2_HTTP_HEADER_PROTOCOL_11))
    {
        axis2_http_worker_set_transport_out_config(http_worker, env, conf_ctx,
                                                   response);
    }

    /* Server and Peer IP's */
    svr_ip = axis2_simple_http_svr_conn_get_svr_ip(svr_conn, env);
    peer_ip = axis2_simple_http_svr_conn_get_peer_ip(svr_conn, env);

    if (peer_ip)
    {
        peer_property = axutil_property_create(env);
        axutil_property_set_value(peer_property, env,
                                  axutil_strdup(env, peer_ip));
        axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_SVR_PEER_IP_ADDR,
                                   peer_property);
    }

    path = axis2_http_request_line_get_uri (request_line, env);

    request_url = axutil_url_create(env, AXIS2_HTTP_PROTOCOL, svr_ip,
                                    http_worker->svr_port, path);
    if (request_url)
    {
        url_external_form = axutil_url_to_external_form(request_url, env);
    }

    if (!url_external_form)
    {
        axis2_http_simple_response_set_status_line(response, env,
                                                   http_version,
                                                   AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL,
                                                   AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_NAME);
        status =
            axis2_simple_http_svr_conn_write_response(svr_conn, env,
                                                          response);
        axis2_http_simple_response_free(response, env);
        response = NULL;
        return status;
    }

    accept_header = axis2_http_simple_request_get_first_header(
        simple_request,
        env,
        AXIS2_HTTP_HEADER_ACCEPT);
    if (accept_header)
    {
        accept_header_value = axis2_http_header_get_value(accept_header,
                                                          env);
    }
    if (accept_header_value)
    {
        axutil_array_list_t *accept_header_field_list = NULL;
        axutil_array_list_t *accept_record_list = NULL;
        accept_header_field_list = axutil_tokenize(env, accept_header_value, AXIS2_COMMA);
        if (accept_header_field_list && 
            axutil_array_list_size(accept_header_field_list, env) > 0)
        {
            axis2_char_t *token = NULL;
            accept_record_list =
                axutil_array_list_create(env,
                    axutil_array_list_size(accept_header_field_list, env));
            do
            {
                if (token)
                {
                    axis2_http_accept_record_t *rec = NULL;
                    rec = axis2_http_accept_record_create(env, token);
                    if (rec)
                    {
                        axutil_array_list_add(accept_record_list, env, rec);
                    }
                    AXIS2_FREE(env->allocator, token);
                }
                token = (axis2_char_t *)
                    axutil_array_list_remove(accept_header_field_list, env, 0);
            }
            while(token);
        }
        if (accept_record_list && 
            axutil_array_list_size(accept_record_list, env) > 0)
        {
            axis2_msg_ctx_set_http_accept_record_list(msg_ctx, env,
                accept_record_list);
        }
    }

    accept_charset_header = axis2_http_simple_request_get_first_header(
        simple_request,
        env,
        AXIS2_HTTP_HEADER_ACCEPT_CHARSET);
    if (accept_charset_header)
    {
        accept_charset_header_value = axis2_http_header_get_value(accept_charset_header,
                                                                  env);
    }
    if (accept_charset_header_value)
    {
        axutil_array_list_t *accept_charset_header_field_list = NULL;
        axutil_array_list_t *accept_charset_record_list = NULL;
        accept_charset_header_field_list =
            axutil_tokenize(env, accept_charset_header_value, AXIS2_COMMA);
        if (accept_charset_header_field_list && 
            axutil_array_list_size(accept_charset_header_field_list, env) > 0)
        {
            axis2_char_t *token = NULL;
            accept_charset_record_list =
                axutil_array_list_create(env,
                    axutil_array_list_size(accept_charset_header_field_list, env));
            do
            {
                if (token)
                {
                    axis2_http_accept_record_t *rec = NULL;
                    rec = axis2_http_accept_record_create(env, token);
                    if (rec)
                    {
                        axutil_array_list_add(accept_charset_record_list, env, rec);
                    }
                    AXIS2_FREE(env->allocator, token);
                }
                token = (axis2_char_t *)
                    axutil_array_list_remove(accept_charset_header_field_list, env, 0);
            }
            while(token);
        }

        if (accept_charset_record_list && 
            axutil_array_list_size(accept_charset_record_list, env) > 0)
        {
            axis2_msg_ctx_set_http_accept_charset_record_list(msg_ctx, env,
                accept_charset_record_list);
        }
    }

    accept_language_header = axis2_http_simple_request_get_first_header(
        simple_request,
        env,
        AXIS2_HTTP_HEADER_ACCEPT_LANGUAGE);
    if (accept_language_header)
    {
        accept_language_header_value = axis2_http_header_get_value(accept_language_header,
                                                                  env);
    }

    if (accept_language_header_value)
    {
        axutil_array_list_t *accept_language_header_field_list = NULL;
        axutil_array_list_t *accept_language_record_list = NULL;
        accept_language_header_field_list =
            axutil_tokenize(env, accept_language_header_value, AXIS2_COMMA);
        if (accept_language_header_field_list && 
            axutil_array_list_size(accept_language_header_field_list, env) > 0)
        {
            axis2_char_t *token = NULL;
            accept_language_record_list =
                axutil_array_list_create(env,
                    axutil_array_list_size(accept_language_header_field_list, env));
            do
            {
                if (token)
                {
                    axis2_http_accept_record_t *rec = NULL;
                    rec = axis2_http_accept_record_create(env, token);
                    if (rec)
                    {
                        axutil_array_list_add(accept_language_record_list, env, rec);
                    }
                    AXIS2_FREE(env->allocator, token);
                }
                token = (axis2_char_t *)
                    axutil_array_list_remove(accept_language_header_field_list, env, 0);
            }
            while(token);
        }
        if (accept_language_record_list && 
            axutil_array_list_size(accept_language_record_list, env) > 0)
        {
            axis2_msg_ctx_set_http_accept_language_record_list(msg_ctx, env,
                accept_language_record_list);
        }
    }

    /* Here out_stream is set into the in message context. out_stream is copied from in message context
     * into the out message context later in core_utils_create_out_msg_ctx() function. The buffer in
     * out_stream is finally filled with the soap envelope in http_transport_sender_invoke() function.
     * To avoid double freeing of out_stream we reset the out message context at the end of engine 
     * receive function.
     */
    axis2_msg_ctx_set_transport_out_stream(msg_ctx, env, out_stream);

    headers = axis2_http_worker_get_headers(http_worker, env, simple_request);
    axis2_msg_ctx_set_transport_headers(msg_ctx, env, headers);

    svc_grp_uuid = axutil_uuid_gen(env);
    if (svc_grp_uuid)
    {
        axutil_string_t *svc_grp_uuid_str =
            axutil_string_create_assume_ownership(env, &svc_grp_uuid);
        axis2_msg_ctx_set_svc_grp_ctx_id(msg_ctx, env, svc_grp_uuid_str);
        axutil_string_free(svc_grp_uuid_str, env);
    }

    http_out_transport_info =
        axis2_http_out_transport_info_create(env, response);
    axis2_msg_ctx_set_out_transport_info(msg_ctx, env,
        &(http_out_transport_info->out_transport));

    if (axis2_http_simple_request_get_first_header(simple_request, env,
                                                   AXIS2_HTTP_HEADER_SOAP_ACTION))
    {
        soap_action =
            axis2_http_header_get_value
            (axis2_http_simple_request_get_first_header
             (simple_request, env, AXIS2_HTTP_HEADER_SOAP_ACTION), env);
        soap_action_str = axutil_string_create(env, soap_action);
    }

    

    if (0 == axutil_strcasecmp(http_method, AXIS2_HTTP_GET))
    {
        is_get = AXIS2_TRUE;
    }
    else if (0 == axutil_strcasecmp(http_method, AXIS2_HTTP_HEAD))
    {
        is_head = AXIS2_TRUE;
    }
    else if (0 == axutil_strcasecmp(http_method, AXIS2_HTTP_DELETE))
    {
        is_delete = AXIS2_TRUE;
    }
    else if (0 == axutil_strcasecmp(http_method, AXIS2_HTTP_PUT))
    {
        is_put = AXIS2_TRUE;
    }

    request_uri = axis2_http_request_line_get_uri (request_line, env);
    request_params = 
        axis2_http_transport_utils_get_request_params(env,
                                                      request_uri);
    url_ext_form = axutil_url_to_external_form(request_url, env);
        
    content_type = 
        axis2_http_simple_request_get_content_type(simple_request, 
                                                   env);
    
    if (is_get || is_head || is_delete)
    {
        
        if (is_get)
        {  
            /* HTTP GET */
            processed = 
                axis2_http_transport_utils_process_http_get_request (env, 
                                                                     msg_ctx, 
                                                                     request_body, 
                                                                     out_stream,
                                                                     content_type,
                                                                     soap_action_str, 
                                                                     url_ext_form,
                                                                     conf_ctx, 
                                                                     request_params);
        }
        else if (is_delete)
        {
            /* HTTP DELETE */
            processed = 
                axis2_http_transport_utils_process_http_delete_request (env, 
                                                                        msg_ctx, 
                                                                        request_body, 
                                                                        out_stream,
                                                                        content_type,
                                                                        soap_action_str, 
                                                                        url_ext_form,
                                                                        conf_ctx, 
                                                                        request_params);
        }
        else if (is_head)
        {
            /* HTTP HEAD */
            processed = 
                axis2_http_transport_utils_process_http_head_request (env, 
                                                                      msg_ctx, 
                                                                      request_body, 
                                                                      out_stream,
                                                                      content_type,
                                                                      soap_action_str, 
                                                                      url_ext_form,
                                                                      conf_ctx, 
                                                                      request_params);
        }


        if (AXIS2_FALSE == processed)
        {
            axis2_http_header_t *cont_len = NULL;
            axis2_http_header_t *cont_type = NULL;
            axis2_char_t *body_string = NULL;
            axis2_char_t *wsdl = NULL;
            axis2_bool_t is_services_path = AXIS2_FALSE;

            if (!is_delete)
            {
                axis2_char_t *temp = NULL;
                /* check whether request url have "/services"  */
                temp = strstr(axutil_url_get_path(request_url, env), AXIS2_REQUEST_URL_PREFIX);
                if (temp)
                {
                    temp += strlen(AXIS2_REQUEST_URL_PREFIX);
                    if (*temp == AXIS2_F_SLASH)
                    {
                        temp++;
                    }
                    if (!*temp || *temp == AXIS2_Q_MARK || *temp == AXIS2_H_MARK)
                    {
                        is_services_path = AXIS2_TRUE;
                    }
                }

            }

            /* processing request for WSDL via "?wsdl" */
            wsdl = strstr(url_external_form, AXIS2_REQUEST_WSDL);
            if (is_services_path)
            {
                /* request for service */
                axis2_http_simple_response_set_status_line(response, env,
                                                           http_version,
                                                           AXIS2_HTTP_RESPONSE_OK_CODE_VAL,
                                                           AXIS2_HTTP_RESPONSE_OK_CODE_NAME);
                body_string = 
                    axis2_http_transport_utils_get_services_html(env,
                                                                 conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (!is_delete && wsdl)
            {
                /* Request is not for delete and ask for wsdl */
                axis2_http_simple_response_set_status_line(response, env,
                                                           http_version,
                                                           AXIS2_HTTP_RESPONSE_OK_CODE_VAL,
                                                           AXIS2_HTTP_RESPONSE_OK_CODE_NAME);
                body_string =
                    axis2_http_transport_utils_get_services_static_wsdl(env,
                                                                        conf_ctx,
                                                                        url_external_form);
                cont_type = axis2_http_header_create(env,
                                                     AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                                     AXIS2_HTTP_HEADER_ACCEPT_APPLICATION_XML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (env->error->error_number == AXIS2_ERROR_SVC_OR_OP_NOT_FOUND)
            {

                /* Processing SVC or Operation Not found case */
                axutil_array_list_t *method_list = NULL;
                int size = 0;
                method_list = axis2_msg_ctx_get_supported_rest_http_methods(msg_ctx, env);
                size = axutil_array_list_size(method_list, env);
                if (method_list && size)
                {
                    axis2_http_header_t *allow_header = NULL;
                    axis2_char_t *method_list_str = NULL;
                    axis2_char_t *temp;
                    int i = 0;
                    method_list_str = AXIS2_MALLOC(env->allocator, 
                                                   sizeof(axis2_char_t) * 29);

                    if (!method_list_str)
                    {
                        AXIS2_HANDLE_ERROR (env, AXIS2_ERROR_NO_MEMORY, AXIS2_FALSE);
                    }

                    temp = method_list_str;
                    for (i = 0; i < size; i++)
                    {
                        if (i)
                        {
                            sprintf(temp, AXIS2_COMMA_SPACE_STR);
                            temp += 2;
                        }
                        sprintf(temp, "%s", (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i));
                        temp += strlen(temp);
                    }
                    *temp = AXIS2_ESC_NULL;

                    axis2_http_simple_response_set_status_line(
                        response, env,
                        http_version,
                        AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_NAME);

                    body_string =
                        axis2_http_transport_utils_get_method_not_allowed(env,
                                                                          conf_ctx);
                    allow_header = axis2_http_header_create(env,
                                                            AXIS2_HTTP_HEADER_ALLOW,
                                                            method_list_str);
                    axis2_http_simple_response_set_header(response, 
                                                          env, allow_header);
                    AXIS2_FREE(env->allocator, method_list_str);
                }
                else 
                {
                    /* 404 Not Found */
                    axis2_http_simple_response_set_status_line(
                        response, env,
                        http_version,
                        AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_NAME);

                    body_string = axis2_http_transport_utils_get_not_found(env,
                                                                           conf_ctx);
                }
                cont_type = axis2_http_header_create(env,
                                                     AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                                     AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL)
            {
                /* 400 Bad Request */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_NAME);

                body_string = axis2_http_transport_utils_get_bad_request(env,
                                                                         conf_ctx);
                cont_type = axis2_http_header_create(
                    env,
                    AXIS2_HTTP_HEADER_CONTENT_TYPE,
                    AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL)
            {

                /* 408 , Request Time Out */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_request_timeout(env,
                                                                   conf_ctx);
                cont_type = axis2_http_header_create(
                    env,
                    AXIS2_HTTP_HEADER_CONTENT_TYPE,
                    AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL)
            {
                /* 409, Conflict */

                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_CONFLICT_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_conflict(env,
                                                            conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_GONE_CODE_VAL)
            {
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_GONE_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_GONE_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_gone(env,
                                                        conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL)
            {
                /* 412 Precondition failed */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_precondition_failed(env,
                                                                       conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL)
            {
                /* 413 entity too large */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_NAME);
                
                body_string = 
                    axis2_http_transport_utils_get_request_entity_too_large(env,
                                                                            conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL)
            {
                /* 503, Service Unavailable*/
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_service_unavailable(env,
                                                                       conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else
            {
                /* 500 Internal Server Error */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_internal_server_error(env,
                                                                         conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }

            if (body_string)
            {
                axis2_char_t str_len[10];
                if (!is_head)
                {
                    axis2_http_simple_response_set_body_string(response, env,
                                                               body_string);
                }
                sprintf(str_len, "%d", axutil_strlen(body_string));
                cont_len = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                             str_len);
                axis2_http_simple_response_set_header(response, env, cont_len);
            }

            axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                   simple_request, response, 0);

            axis2_simple_http_svr_conn_write_response(svr_conn, env, response);
            axis2_http_simple_response_free(response, env);
            request_handled = AXIS2_TRUE;
            status = AXIS2_TRUE;
        }
    }
    else if (0 == axutil_strcasecmp(http_method, AXIS2_HTTP_POST) || is_put)
    {
        if (is_put)
        {
            status = 
                axis2_http_transport_utils_process_http_put_request (env, 
                                                                     msg_ctx, 
                                                                     request_body, 
                                                                     out_stream,
                                                                     content_type,
                                                                     content_length,
                                                                     soap_action_str,
                                                                     url_ext_form);


        }
        else
        {
            status = 
                axis2_http_transport_utils_process_http_post_request (env, 
                                                                      msg_ctx, 
                                                                      request_body, 
                                                                      out_stream,
                                                                      content_type,
                                                                      content_length,
                                                                      soap_action_str,
                                                                      url_ext_form);

        }
        if(url_ext_form)
            AXIS2_FREE(env->allocator, url_ext_form);
        if (AXIS2_FAILURE == status && (is_put ||
            axis2_msg_ctx_get_doing_rest(msg_ctx, env)))
        {
            /* Failure Occure while processing REST */

            axis2_http_header_t *cont_len = NULL;
            axis2_http_header_t *cont_type = NULL;
            axis2_char_t *body_string = NULL;
            if (env->error->error_number == AXIS2_ERROR_SVC_OR_OP_NOT_FOUND)
            {
                axutil_array_list_t *method_list = NULL;
                int size = 0;
                method_list = 
                    axis2_msg_ctx_get_supported_rest_http_methods(msg_ctx, env);
                size = axutil_array_list_size(method_list, env);
                if (method_list && size)
                {
                    axis2_http_header_t *allow_header = NULL;
                    axis2_char_t *method_list_str = NULL;
                    axis2_char_t *temp;
                    int i = 0;
                    method_list_str = AXIS2_MALLOC(env->allocator, 
                                                   sizeof(axis2_char_t) * 29);
                    if (!method_list_str)
                    {
                        AXIS2_HANDLE_ERROR (env, AXIS2_ERROR_NO_MEMORY, AXIS2_FALSE);
                    }

                    temp = method_list_str;
                    for (i = 0; i < size; i++)
                    {
                        if (i)
                        {
                            sprintf(temp, AXIS2_COMMA_SPACE_STR);
                            temp += 2;
                        }
                        sprintf(temp, "%s", (axis2_char_t *)
                                axutil_array_list_get(method_list, env, i));
                        temp += strlen(temp);
                    }
                    *temp = AXIS2_ESC_NULL;

                    /* 405 Method Not Allowed */
                    axis2_http_simple_response_set_status_line(
                        response, env,
                        http_version,
                        AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_NAME);

                    body_string =
                        axis2_http_transport_utils_get_method_not_allowed(env,
                                                                          conf_ctx);
                    allow_header = axis2_http_header_create(env,
                                                            AXIS2_HTTP_HEADER_ALLOW,
                                                            method_list_str);
                    axis2_http_simple_response_set_header(response, env, allow_header);
                    AXIS2_FREE(env->allocator, method_list_str);
                }
                else
                {
                    /* 404 Not Found */
                    axis2_http_simple_response_set_status_line(
                        response, env,
                        http_version,
                        AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_NAME);

                    body_string = 
                        axis2_http_transport_utils_get_not_found(env,
                                                                 conf_ctx);
                }
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL)
            {
                /* 400, Bad Request */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_bad_request(env,
                                                               conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL)
            {
                /* 408, Request Timeout */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_request_timeout(env,
                                                                   conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL)
            {
                /* 409, Conflict Types */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_CONFLICT_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_conflict(env,
                                                            conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_GONE_CODE_VAL)
            {
                /* 410, Gone. Resource no longer available */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_GONE_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_GONE_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_gone(env,
                                                        conf_ctx);

                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL)
            {
                /*410, Precondition for the url failed  */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_precondition_failed(env,
                                                                       conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL)
            {
                /* 413, Request entity too large */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_request_entity_too_large(env,
                                                                            conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else if (axis2_msg_ctx_get_status_code(msg_ctx, env) == 
                AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL)
            {
                /* 513, Service Unavailable */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_service_unavailable(env,
                                                                       conf_ctx);
                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }
            else
            {
                /* 500, Internal Server Error */
                axis2_http_simple_response_set_status_line(
                    response, env,
                    http_version,
                    AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_NAME);

                body_string = 
                    axis2_http_transport_utils_get_internal_server_error(env,
                                                                         conf_ctx);

                cont_type = 
                    axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                axis2_http_simple_response_set_header(response, env, cont_type);
            }

            if (body_string)
            {
                axis2_char_t str_len[10];
                if (!is_head)
                {
                    axis2_http_simple_response_set_body_string(response, env,
                                                               body_string);
                }
                sprintf(str_len, "%d", axutil_strlen(body_string));
                cont_len = axis2_http_header_create(env,
                                                    AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                                    str_len);
                axis2_http_simple_response_set_header(response, env, cont_len);
            }
            axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                   simple_request, response, 0);
            axis2_simple_http_svr_conn_write_response(svr_conn, env, response);
            axis2_http_simple_response_free(response, env);
            request_handled = AXIS2_TRUE;
            status = AXIS2_TRUE;
        }
        else if (status == AXIS2_FAILURE)
        {
            axis2_msg_ctx_t *fault_ctx = NULL;
            axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
            axis2_http_request_line_t *req_line = NULL;
            axis2_http_status_line_t *tmp_stat_line = NULL;
            axis2_char_t status_line_str[100];
            axutil_property_t *http_error_property = NULL;
            axis2_char_t *http_error_value = NULL;
            axis2_char_t *fault_code = NULL;
            int status_code = 0;
            axis2_char_t *reason_phrase = NULL;
            int stream_len = 0;

			if (!engine)
            {
                return AXIS2_FALSE;
            }

            http_error_property = axis2_msg_ctx_get_property(msg_ctx, env,
                                                             AXIS2_HTTP_TRANSPORT_ERROR);

            if (http_error_property)
                http_error_value =
                    (axis2_char_t *)
                    axutil_property_get_value(http_error_property, env);

            if (axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
            {
                fault_code = AXIOM_SOAP_DEFAULT_NAMESPACE_PREFIX AXIS2_COLON_STR
                    AXIOM_SOAP11_FAULT_CODE_SENDER;
            }
            else
            {
                fault_code = AXIOM_SOAP_DEFAULT_NAMESPACE_PREFIX AXIS2_COLON_STR
                    AXIOM_SOAP12_SOAP_FAULT_VALUE_SENDER;
            }

            fault_ctx = axis2_engine_create_fault_msg_ctx(engine, env, msg_ctx,
                                                          fault_code,
                                                          axutil_error_get_message
                                                          (env->error));
            req_line =
                axis2_http_simple_request_get_request_line(simple_request, env);
            if (req_line)
            {
                if (!http_error_value)
                {
                    sprintf(status_line_str, "%s %s\r\n",
                            http_version,
                            AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR);
                }
                else
                {
                    sprintf(status_line_str, "%s %s",
                            http_version,
                            http_error_value);
                }
            }
            else
            {
                sprintf(status_line_str, "%s %s\r\n", 
                        AXIS2_HTTP_HEADER_PROTOCOL_11,
                        AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR);
            }

            tmp_stat_line = axis2_http_status_line_create(env, status_line_str);

            if (!http_error_value)
            {
                axis2_engine_send_fault(engine, env, fault_ctx);
            }
            
            status_code = axis2_http_status_line_get_status_code(tmp_stat_line, env);
            reason_phrase = axis2_http_status_line_get_reason_phrase(tmp_stat_line, env);

            axis2_http_simple_response_set_status_line(response, env,
                                                       http_version,
                                                       status_code,
                                                       reason_phrase);

            axis2_http_simple_response_set_body_stream(response, env,
                                                       out_stream);

            stream_len = axutil_stream_get_len (out_stream, env);
            axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                   simple_request, response,
                                                   stream_len);
             
            status = axis2_simple_http_svr_conn_write_response(svr_conn, env,
                                                               response);
            request_handled = AXIS2_TRUE;
            if(tmp_stat_line)
            {
                axis2_http_status_line_free(tmp_stat_line, env);
                tmp_stat_line = NULL;
            }
        }
    }
    else
    {
        /* Other case than, PUT, DELETE, HEAD, GET and POST */
        /* 501, Request method is not implemented */
        axis2_http_header_t *cont_len = NULL;
        axis2_http_header_t *cont_type = NULL;
        axis2_char_t *body_string = NULL;
        axis2_http_simple_response_set_status_line(
            response, env,
            http_version,
            AXIS2_HTTP_RESPONSE_NOT_IMPLEMENTED_CODE_VAL,
            AXIS2_HTTP_RESPONSE_NOT_IMPLEMENTED_CODE_NAME);

        body_string = 
            axis2_http_transport_utils_get_not_implemented(env,
                                                           conf_ctx);
        cont_type = axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                             AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);
        axis2_http_simple_response_set_header(response, env, cont_type);

        if (body_string)
        {
            axis2_char_t str_len[10];
            axis2_http_simple_response_set_body_string(response, env,
                                                       body_string);
            sprintf(str_len, "%d", axutil_strlen(body_string));
            cont_len = axis2_http_header_create(env,
                                                AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                                str_len);
            axis2_http_simple_response_set_header(response, env, cont_len);
        }

        axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                               simple_request, response, 0);
        axis2_simple_http_svr_conn_write_response(svr_conn, env, response);
        axis2_http_simple_response_free(response, env);
        request_handled = AXIS2_TRUE;
        status = AXIS2_TRUE;
    }

    op_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, env);
    if (op_ctx)
    {
        /*axis2_msg_ctx_t *out_msg_ctx = NULL;
        axis2_msg_ctx_t **msg_ctx_map = NULL;*/
        axis2_char_t *language_str = NULL;

        msg_ctx_map = axis2_op_ctx_get_msg_ctx_map(op_ctx, env);
        out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];
        if (out_msg_ctx)
        {
            language_str = axis2_msg_ctx_get_content_language(out_msg_ctx, env);
        }

        if (language_str && *language_str && !request_handled)
        {
            axis2_http_header_t *language = NULL;
            language = axis2_http_header_create(env,
                                                AXIS2_HTTP_HEADER_CONTENT_LANGUAGE,
                                                language_str);
            axis2_http_simple_response_set_header(response, env, language);    
        }
    }

    if (!request_handled)
    {
        axis2_bool_t do_rest = AXIS2_FALSE;
        axis2_bool_t response_written = AXIS2_FALSE;
        if (is_get || is_head || is_put || is_delete || 
            axis2_msg_ctx_get_doing_rest(msg_ctx, env))
        {
            do_rest = AXIS2_TRUE;
        }


        if ((accept_header_value || accept_charset_header_value ||
            accept_language_header_value) && do_rest)
        {
            axis2_char_t *content_type_header_value = NULL;
            axis2_http_header_t *content_type_header = NULL;
            axis2_char_t *temp = NULL;
            axis2_char_t *language_header_value = NULL;
            axis2_http_header_t *language_header = NULL;
            content_type_header = axis2_http_simple_response_get_first_header(
                response,
                env,
                AXIS2_HTTP_HEADER_CONTENT_TYPE);

            language_header = axis2_http_simple_response_get_first_header(
                response,
                env,
                AXIS2_HTTP_HEADER_CONTENT_LANGUAGE);

            if (content_type_header)
            {
                content_type_header_value = axis2_http_header_get_value(content_type_header,
                                                                        env);
            }

            if (content_type_header_value)
            {
                temp = axutil_strdup(env, content_type_header_value);
            }

            if (language_header)
            {
                language_header_value = axis2_http_header_get_value(language_header,
                                                                    env);
            }

            if (temp)
            {
                axis2_char_t *content_type = NULL;
                axis2_char_t *char_set = NULL;
                axis2_char_t *temp2 = NULL;
                
                temp2 = strchr(temp, AXIS2_SEMI_COLON);
                if (temp2)
                {
                    *temp2 = AXIS2_ESC_NULL;
                    temp2++;
                    char_set = axutil_strcasestr(temp2, AXIS2_HTTP_CHAR_SET_ENCODING);
                }

                if (char_set)
                {
                    char_set = axutil_strltrim(env, char_set, AXIS2_SPACE_TAB_EQ);
                }

                content_type = axutil_strtrim(env, temp, NULL);

                if (temp)
                {
                    AXIS2_FREE(env->allocator, temp);
                    temp = NULL;
                }

                if (content_type && accept_header_value &&
                    !axutil_strcasestr(accept_header_value, content_type))
                {
                    temp2 = strchr(content_type, AXIS2_F_SLASH);
                    if (temp2)
                    {
                        *temp2 = AXIS2_ESC_NULL;
                        temp = AXIS2_MALLOC(env->allocator,
                                            sizeof(axis2_char_t) * 
                                            ((int)strlen(content_type) + 3));
                        if (!temp)
                        {
                            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                            return AXIS2_FALSE;
                        }

                        sprintf(temp, "%s/*", content_type);
                        if (!axutil_strcasestr(accept_header_value, temp) &&
                            !strstr(accept_header_value, AXIS2_HTTP_HEADER_ACCEPT_ALL))
                        {
                            /* 406, Not Acceptable */
                            axis2_http_header_t *cont_len = NULL;
                            axis2_http_header_t *cont_type = NULL;
                            axis2_char_t *body_string = NULL;
                            axis2_http_simple_response_set_status_line(
                                response, env, http_version,
                                AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_VAL,
                                AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_NAME);

                            body_string = 
                                axis2_http_transport_utils_get_not_acceptable(env,
                                                                              conf_ctx);
                            cont_type = 
                                axis2_http_header_create(env,
                                                         AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                                         AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                            axis2_http_simple_response_set_header(response, env, cont_type);
                            if (body_string)
                            {
                                axis2_char_t str_len[10];
                                axis2_http_simple_response_set_body_string(response, env,
                                                                           body_string);
                                sprintf(str_len, "%d", axutil_strlen(body_string));
                                cont_len = 
                                    axis2_http_header_create(env,
                                                             AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                                             str_len);

                                axis2_http_simple_response_set_header(response, 
                                                                      env, 
                                                                      cont_len);
                            }

                            axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                                   simple_request, response, 0);
                            axis2_simple_http_svr_conn_write_response(svr_conn, env, response);
                            axis2_http_simple_response_free(response, env);
                            request_handled = AXIS2_TRUE;
                            status = AXIS2_TRUE;
                            response_written = AXIS2_TRUE;
                        }
                        AXIS2_FREE(env->allocator, temp);
                    }
                }

                if (content_type)
                {
                    AXIS2_FREE(env->allocator, content_type);
                }

                if (char_set)
                {
                    temp2 = strchr(char_set, AXIS2_EQ);
                }

                if (temp2)
                {
                    ++temp2;
                }

                if (char_set && accept_charset_header_value && 
                    !axutil_strcasestr(accept_charset_header_value, char_set) &&
                    !axutil_strcasestr(accept_charset_header_value, temp2))
                {
                    /* 406, Not Acceptable */
                    axis2_http_header_t *cont_len = NULL;
                    axis2_http_header_t *cont_type = NULL;
                    axis2_char_t *body_string = NULL;
                    axis2_http_simple_response_set_status_line(
                        response, env, http_version,
                        AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_NAME);
                    body_string = 
                        axis2_http_transport_utils_get_not_acceptable(env,
                                                                      conf_ctx);
                    cont_type = 
                        axis2_http_header_create(env,
                                                 AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                                 AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                    axis2_http_simple_response_set_header(response, env, cont_type);
                    if (body_string)
                    {
                        axis2_char_t str_len[10];
                        axis2_http_simple_response_set_body_string(response, env,
                                                                   body_string);
                        sprintf(str_len, "%d", axutil_strlen(body_string));
                        cont_len = axis2_http_header_create(env,
                                                            AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                                            str_len);
                        axis2_http_simple_response_set_header(response, env, cont_len);
                    }
                    axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                           simple_request, response, 0);
                    axis2_simple_http_svr_conn_write_response(svr_conn, env, response);
                    request_handled = AXIS2_TRUE;
                    status = AXIS2_TRUE;
                    response_written = AXIS2_TRUE;
                }
                if (char_set)
                {
                    AXIS2_FREE(env->allocator, char_set);
                }
            }

            if (language_header_value)
            {
                if (accept_language_header_value && 
                    !axutil_strcasestr(accept_language_header_value, 
                                       language_header_value))
                {
                    /* 406, Not acceptable */
                    axis2_http_header_t *cont_len = NULL;
                    axis2_http_header_t *cont_type = NULL;
                    axis2_char_t *body_string = NULL;
                    axis2_http_simple_response_set_status_line(
                        response, env, http_version,
                        AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_NAME);

                    body_string = 
                        axis2_http_transport_utils_get_not_acceptable(env,
                                                                      conf_ctx);
                    cont_type = 
                        axis2_http_header_create(env,
                                                 AXIS2_HTTP_HEADER_CONTENT_TYPE,
                                                 AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML);

                    axis2_http_simple_response_set_header(response, env, cont_type);
                    axis2_http_simple_response_remove_headers(
                        response, 
                        env, 
                        AXIS2_HTTP_HEADER_CONTENT_LANGUAGE);

                    if (body_string)
                    {
                        axis2_char_t str_len[10];
                        axis2_http_simple_response_set_body_string(response, env,
                                                                   body_string);
                        sprintf(str_len, "%d", axutil_strlen(body_string));
                        cont_len = axis2_http_header_create(env,
                                                            AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                                            str_len);
                        axis2_http_simple_response_set_header(response, env, cont_len);
                    }

                    axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                           simple_request, response, 0);
                    axis2_simple_http_svr_conn_write_response(svr_conn, env, response);
                    request_handled = AXIS2_TRUE;
                    status = AXIS2_TRUE;
                    response_written = AXIS2_TRUE;
                }
            }
        }

        if (!response_written)
        {
            /* If in there is a soap message is to to be sent in the back channel then we go inside this
             * block. Somewhere in the receiveing end axis2_op_ctx_set_response_written() function has
             * been called by this time to indicate to append the message into the http back channel.
             */
            if (op_ctx && axis2_op_ctx_get_response_written(op_ctx, env))
            {
                if (do_rest)
                {
                    /*axis2_msg_ctx_t *out_msg_ctx = NULL;
                    axis2_msg_ctx_t *in_msg_ctx = NULL;
                    axis2_msg_ctx_t **msg_ctx_map = NULL;*/

                    /*msg_ctx_map = axis2_op_ctx_get_msg_ctx_map(op_ctx, env);
                    out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];*/
                    in_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN];
                    if (in_msg_ctx)
                    {
                        /* TODO: Add neccessary handling */
                    }
                    if (out_msg_ctx)
                    {
                        int size = 0;
                        axutil_array_list_t *output_header_list = NULL;
                        output_header_list = 
                            axis2_msg_ctx_get_http_output_headers(out_msg_ctx,
                                                                  env);
                        if (output_header_list)
                        {
                            size = axutil_array_list_size(output_header_list, 
                                                          env);
                        }
                        while (size)
                        {
							axis2_http_header_t *simple_header = NULL;
                            size--;
                            simple_header = (axis2_http_header_t *) 
                                axutil_array_list_get(output_header_list, 
                                                      env, size);
                            axis2_http_simple_response_set_header(response, 
                                                                  env, 
                                                                  simple_header);
                        }

                        if (axis2_msg_ctx_get_status_code(out_msg_ctx, env))
                        {
                            int status_code = 0;
							axis2_char_t *status_code_str = NULL;
                            status_code = axis2_msg_ctx_get_status_code(out_msg_ctx, env);
                            
                            switch (status_code)
                            {
                            case AXIS2_HTTP_RESPONSE_CONTINUE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_CONTINUE_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_ACK_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_ACK_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_MOVED_PERMANENTLY_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_MOVED_PERMANENTLY_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_CONFLICT_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_GONE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_GONE_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_NAME;
                                break;
                            default:
                                status_code = AXIS2_HTTP_RESPONSE_OK_CODE_VAL;
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_OK_CODE_NAME;
                                break;
                            }

                            axis2_http_simple_response_set_status_line(response, env, 
                                                                       http_version,
                                                                       status_code,
                                                                       status_code_str);
                            request_handled = AXIS2_TRUE;
                        }
                    }
                }

                if (!request_handled)
                {
                    axis2_http_simple_response_set_status_line(
                        response, env, http_version,
                        AXIS2_HTTP_RESPONSE_OK_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_OK_CODE_NAME);

                    if (!is_head)
                    {
                        /* This is where we append the message into the http back channel.*/
                        axis2_http_simple_response_set_body_stream(response, env, out_stream);
                    }
                }
            }
            else if (op_ctx)
            {
                /* If response is not written */
                if (do_rest)
                {
                    /*axis2_msg_ctx_t *out_msg_ctx = NULL;
                    axis2_msg_ctx_t *in_msg_ctx = NULL;
                    axis2_msg_ctx_t **msg_ctx_map = NULL;*/

                    msg_ctx_map = axis2_op_ctx_get_msg_ctx_map(op_ctx, env);
                    out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];
                    in_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN];
                    if (in_msg_ctx)
                    {
                        /* TODO: Add neccessary handling */
                    }
                    if (out_msg_ctx)
                    {
                        int size = 0;
                        axutil_array_list_t *output_header_list = NULL;
                        output_header_list = 
                            axis2_msg_ctx_get_http_output_headers(out_msg_ctx, 
                                                                  env);
                        if (output_header_list)
                        {
                            size = axutil_array_list_size(output_header_list, 
                                                          env);
                        }

                        while (size)
                        {
							axis2_http_header_t *simeple_header = NULL;
                            size--;
                            simeple_header = (axis2_http_header_t *)
                                axutil_array_list_get(output_header_list,
                                                      env, size);
                            axis2_http_simple_response_set_header(response, env,
                                                                  simeple_header);
                        }

                        if (axis2_msg_ctx_get_no_content(out_msg_ctx, env))
                        {
                            if (axis2_msg_ctx_get_status_code(out_msg_ctx, env))
                            {
                                int status_code = axis2_msg_ctx_get_status_code(out_msg_ctx, env);
                                axis2_char_t *status_code_str = NULL;
                                switch (status_code)
                                {
                                case AXIS2_HTTP_RESPONSE_RESET_CONTENT_CODE_VAL:
                                    status_code_str = 
                                        AXIS2_HTTP_RESPONSE_RESET_CONTENT_CODE_NAME;
                                    break;
                                case AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL:
                                    status_code_str = 
                                        AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_NAME;
                                    break;
                                default:
                                    status_code = 
                                        AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_VAL;
                                    status_code_str = 
                                        AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_NAME;
                                    break;
                                }
                                axis2_http_simple_response_set_status_line(
                                    response, env, http_version,
                                    status_code,
                                    status_code_str);
                            }
                            else
                            {
                                /* status code not available in msg_ctx */
                                axis2_http_simple_response_set_status_line(
                                    response, env, http_version,
                                    AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_VAL,
                                    AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_NAME);
                            }

                            request_handled = AXIS2_TRUE;
                        }
                        else if (axis2_msg_ctx_get_status_code(out_msg_ctx, env))
                        {
                            int status_code = axis2_msg_ctx_get_status_code(out_msg_ctx, env);
                            axis2_char_t *status_code_str = NULL;
                            switch (status_code)
                            {
                            case AXIS2_HTTP_RESPONSE_CONTINUE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_CONTINUE_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_OK_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_OK_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_MOVED_PERMANENTLY_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_MOVED_PERMANENTLY_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_CONFLICT_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_GONE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_GONE_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_NAME;
                                break;
                            case AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL:
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_NAME;
                                break;
                            default:
                                status_code = AXIS2_HTTP_RESPONSE_ACK_CODE_VAL;
                                status_code_str = 
                                    AXIS2_HTTP_RESPONSE_ACK_CODE_NAME;
                                break;
                            }
                            axis2_http_simple_response_set_status_line(
                                response, env, http_version,
                                status_code,
                                status_code_str);

                            request_handled = AXIS2_TRUE;
                        }
                    }
                }
                if (!request_handled)
                {
                    axis2_http_simple_response_set_status_line(
                        response, env, http_version,
                        AXIS2_HTTP_RESPONSE_ACK_CODE_VAL,
                        AXIS2_HTTP_RESPONSE_ACK_CODE_NAME);
                }
            }
            else
            {
                axis2_http_simple_response_set_status_line(
                    response, env, http_version,
                    AXIS2_HTTP_RESPONSE_ACK_CODE_VAL,
                    AXIS2_HTTP_RESPONSE_ACK_CODE_NAME);
            }


            if (!response_written)
            {
                int stream_len = 0;
                stream_len = axutil_stream_get_len(out_stream, env);

                /*axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                       simple_request, response,
                                                       stream_len);*/
   
                /* This is where it actually write to the wire in the http back channel
                 * append case. */ 
                if(out_msg_ctx)
                {
                    axutil_array_list_t *mime_parts = NULL;
                    mime_parts = axis2_msg_ctx_get_mime_parts(out_msg_ctx, env);
                    /* If mime_parts is there then that means we send MTOM. So
                     * in order to send MTOM we are enabling HTTP1.1 and cunk transfer
                     * encoding */

                    if(mime_parts)
                    {
                        axis2_http_header_t *transfer_enc_header = NULL;   
                        axutil_param_t *callback_name_param = NULL;
                        axis2_char_t *mtom_sending_callback_name = NULL;

                        /* Getting the sender callback name paramter if it is 
                         * specified in the configuration file */

                        callback_name_param = axis2_msg_ctx_get_parameter(out_msg_ctx, env ,
                            AXIS2_MTOM_SENDING_CALLBACK);
                        if(callback_name_param)
                        {
                            mtom_sending_callback_name =
                                (axis2_char_t *) axutil_param_get_value (callback_name_param, env);
                            if(mtom_sending_callback_name)
                            {
                                axis2_http_simple_response_set_mtom_sending_callback_name(
                                    response, env, mtom_sending_callback_name);
                            }
                        }

                        axis2_http_simple_response_set_mime_parts(response, env, mime_parts);  

                        axis2_http_simple_response_set_http_version(response, env, 
                            AXIS2_HTTP_HEADER_PROTOCOL_11);
 
                        transfer_enc_header = axis2_http_header_create(env,
                                         AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
                                         AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED);

                        axis2_http_simple_response_set_header(response, env,
                                                  transfer_enc_header);

                        /* In the chunking case content-lenght is zero */
                        axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                       simple_request, response,
                                                       0);
                    }
                    else
                    {
                        axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                                                       simple_request, response,
                                                       stream_len);
                    }
                }
                status = axis2_simple_http_svr_conn_write_response(svr_conn, 
                                                                   env, 
                                                                   response);
            }
        }
    }

    if (url_external_form)
    {
        AXIS2_FREE(env->allocator, url_external_form);
        url_external_form = NULL;
    } 
    if (op_ctx)
    {
        axis2_msg_ctx_t *out_msg_ctx = NULL;
        axis2_msg_ctx_t *in_msg_ctx = NULL;
        axis2_msg_ctx_t **msg_ctx_map = NULL;
        axis2_char_t *msg_id = NULL;
        axis2_conf_ctx_t *conf_ctx = NULL;
        msg_ctx_map = axis2_op_ctx_get_msg_ctx_map(op_ctx, env);

        out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];
        in_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN];

        if (out_msg_ctx)
        {
            axis2_msg_ctx_free(out_msg_ctx, env);
            out_msg_ctx = NULL;
            msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT] = NULL;
        }

        if (in_msg_ctx)
        {
            msg_id = axutil_strdup(env, axis2_msg_ctx_get_msg_id(in_msg_ctx, env));
            conf_ctx = axis2_msg_ctx_get_conf_ctx(in_msg_ctx, env);

            axis2_msg_ctx_free(in_msg_ctx, env);
            in_msg_ctx = NULL;
            msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN] = NULL;
        }

        if (!axis2_op_ctx_is_in_use(op_ctx, env))
        {
            axis2_op_ctx_destroy_mutex(op_ctx, env);
            if (conf_ctx && msg_id)
            {
                axis2_conf_ctx_register_op_ctx(conf_ctx, env, msg_id, NULL);

                AXIS2_FREE(env->allocator, msg_id);
            }
            axis2_op_ctx_free(op_ctx, env);
        }

    }                           /* Done freeing message contexts */

    msg_ctx = NULL;
    axutil_url_free(request_url, env);
    axutil_string_free(soap_action_str, env);
    request_url = NULL;
    return status;
}

static axis2_status_t
axis2_http_worker_set_response_headers(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env,
    axis2_simple_http_svr_conn_t * svr_conn,
    axis2_http_simple_request_t * simple_request,
    axis2_http_simple_response_t * simple_response,
    axis2_ssize_t content_length)
{
    axis2_http_header_t *conn_header = NULL;

    AXIS2_PARAM_CHECK(env->error, svr_conn, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_request, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_response, AXIS2_FAILURE);

    if (AXIS2_FALSE == axis2_http_simple_response_contains_header
        (simple_response, env, AXIS2_HTTP_HEADER_CONNECTION))
    {
        conn_header = 
            axis2_http_simple_request_get_first_header(simple_request,
                                                       env,
                                                       AXIS2_HTTP_HEADER_CONNECTION);
        if (conn_header)
        {
            axis2_char_t *value = NULL;
            value = axis2_http_header_get_value(conn_header, env);

            if (0 == axutil_strcasecmp(value, 
                                       AXIS2_HTTP_HEADER_CONNECTION_KEEPALIVE))
            {
                axis2_http_header_t *header = axis2_http_header_create(
                    env,
                    AXIS2_HTTP_HEADER_CONNECTION,
                    AXIS2_HTTP_HEADER_CONNECTION_KEEPALIVE);

                axis2_http_simple_response_set_header(simple_response, env,
                                                      header);
                axis2_simple_http_svr_conn_set_keep_alive(svr_conn, env,
                                                          AXIS2_TRUE);
            }

            if (0 == axutil_strcasecmp(value,
                                       AXIS2_HTTP_HEADER_CONNECTION_CLOSE))
            {
                axis2_http_header_t *header = axis2_http_header_create(
                    env,
                    AXIS2_HTTP_HEADER_CONNECTION,
                    AXIS2_HTTP_HEADER_CONNECTION_CLOSE);

                axis2_http_simple_response_set_header(simple_response, env,
                                                      header);
                axis2_simple_http_svr_conn_set_keep_alive(svr_conn, env,
                                                          AXIS2_FALSE);
            }
        }
        else
        {   /* Connection Header not available */
            axis2_char_t *http_version = NULL;
            http_version = 
                axis2_http_simple_response_get_http_version(simple_response, env);
            if (http_version && axutil_strcasecmp(http_version,
                                                  AXIS2_HTTP_HEADER_PROTOCOL_11))
            {
                axis2_simple_http_svr_conn_set_keep_alive(svr_conn, env,
                                                          AXIS2_TRUE);
            }
            else
            {
                axis2_simple_http_svr_conn_set_keep_alive(svr_conn, env,
                                                          AXIS2_FALSE);
            }
        }

        if(!axis2_http_simple_response_contains_header(simple_response, env, 
            AXIS2_HTTP_HEADER_TRANSFER_ENCODING))
        {
            if (AXIS2_FALSE ==
                axis2_http_simple_request_contains_header(simple_request, env,
                                                      AXIS2_HTTP_HEADER_TRANSFER_ENCODING))
            {
                if (0 != content_length)
                {
                    axis2_char_t content_len_str[10];
                    axis2_http_header_t *content_len_hdr = NULL;

                    sprintf(content_len_str, "%d", content_length);
                    content_len_hdr =
                        axis2_http_header_create(env,
                                             AXIS2_HTTP_HEADER_CONTENT_LENGTH,
                                             content_len_str);
                    axis2_http_simple_response_set_header(simple_response, env,
                                                      content_len_hdr);
                }
            }
            else
            {
            /* Having Transfer encoding Header */
                axis2_http_header_t *transfer_enc_header =
                    axis2_http_header_create(env,
                                         AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
                                         AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED);

                axis2_http_simple_response_set_header(simple_response, env,
                                                  transfer_enc_header);
            }
        }
    }
    return AXIS2_SUCCESS;
}

/*
 * This is only called for HTTP/1.1 to enable 1.1 specific parameters.
 *
 */
static axis2_status_t
axis2_http_worker_set_transport_out_config(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_http_simple_response_t * simple_response)
{
    axis2_conf_t *config = NULL;

    AXIS2_PARAM_CHECK(env->error, conf_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_response, AXIS2_FAILURE);

    config = axis2_conf_ctx_get_conf(conf_ctx, env);

    return AXIS2_SUCCESS;
}

static axutil_hash_t *
axis2_http_worker_get_headers(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env,
    axis2_http_simple_request_t * request)
{
    axutil_array_list_t *header_list = NULL;
    int hdr_count = 0;
    int i = 0;
    axutil_hash_t *header_map = NULL;

    AXIS2_PARAM_CHECK(env->error, request, NULL);

    header_list = axis2_http_simple_request_get_headers(request, env);
    if (!header_list)
    {
        AXIS2_LOG_ERROR (env->log, AXIS2_LOG_SI, "http simple request"
                         "doesn't contain a header list");
        return NULL;
    }
    hdr_count = axutil_array_list_size(header_list, env);
    if (0 == hdr_count)
    {
        AXIS2_LOG_WARNING (env->log, AXIS2_LOG_SI, "http simple request , "
                           "header list contains zero headers");
        return NULL;
    }

    for (i = 0; i < hdr_count; i++)
    {
        axis2_http_header_t *tmp_hdr = NULL;
        tmp_hdr = axutil_array_list_get(header_list, env, i);
        if (!tmp_hdr)
        {
            continue;
       }
        if (!header_map)
        {
            header_map = axutil_hash_make(env);
            if (!header_map)
            {
                return NULL;
            }
        }
        axutil_hash_set(header_map, axis2_http_header_get_name(tmp_hdr, env),
                        AXIS2_HASH_KEY_STRING, tmp_hdr);
    }
    return header_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_worker_set_svr_port(
    axis2_http_worker_t * worker,
    const axutil_env_t * env,
    int port)
{
    worker->svr_port = port;
    return AXIS2_SUCCESS;
}

static axis2_char_t *axis2_http_worker_get_server_time(
    axis2_http_worker_t * http_worker,
    const axutil_env_t * env)
{
    time_t tp;
    char *time_str;
    tp = time(&tp);
    time_str = ctime(&tp);
    if (!time_str)
    {
        return NULL;
    }
    if (AXIS2_NEW_LINE == time_str[strlen(time_str) - 1])
    {
        time_str[strlen(time_str) - 1] = AXIS2_ESC_NULL;
    }
    /* We use the ANSI C Date Format, which is Legal according to RFC2616, 
     * Section 3.3.1. We are not a HTTP/1.1 only server, and thus, it suffices.
     */
    return time_str;
}


