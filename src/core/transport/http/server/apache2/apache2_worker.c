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

#include "axis2_apache2_worker.h"
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
#include "axis2_apache2_out_transport_info.h"
#include <axutil_url.h>
#include <http_core.h>
#include <http_protocol.h>
#include <axiom_soap.h>
#include <axutil_class_loader.h>

#define READ_SIZE  2048

struct axis2_apache2_worker
{
    axis2_conf_ctx_t *conf_ctx;
};

AXIS2_EXTERN axis2_apache2_worker_t *AXIS2_CALL
axis2_apache2_worker_create(
    const axutil_env_t * env,
    axis2_char_t * repo_path)
{
    axis2_apache2_worker_t *apache2_worker = NULL;
	axutil_hash_t* svc_map = NULL;
	axis2_conf_t* conf = NULL;
	axutil_hash_index_t *hi = NULL;
	void* svc = NULL;
    AXIS2_ENV_CHECK(env, NULL);
	AXIS2_LOG_INFO(env->log,"[Axis2] Axis2 worker created");
    apache2_worker = (axis2_apache2_worker_t *)
        AXIS2_MALLOC(env->allocator, sizeof(axis2_apache2_worker_t));

    if (!apache2_worker)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    apache2_worker->conf_ctx = axis2_build_conf_ctx(env, repo_path);

    if (!apache2_worker->conf_ctx)
    {
        axis2_apache2_worker_free((axis2_apache2_worker_t *) apache2_worker,
                                  env);
        return NULL;
    }

	/*
	 * we have to load all the services. This is because, before the fork (in linux)
	 * we should have a full code segment. Otherwise, we can't share function pointers of services
	 * between processed. In fork, the code segment will be duplicated across processes
	 */
	conf = axis2_conf_ctx_get_conf(apache2_worker->conf_ctx, env);
	if (!conf)
	{
		axis2_apache2_worker_free((axis2_apache2_worker_t *) apache2_worker, 
								env);
		return NULL;
	}

	svc_map = axis2_conf_get_all_svcs(conf, env);
	if (!svc_map)
	{
		axis2_apache2_worker_free((axis2_apache2_worker_t *) apache2_worker,
								env);
		return NULL;
	}

		
	for (hi = axutil_hash_first(svc_map, env);
			hi; hi = axutil_hash_next(env, hi))
	{
		void *impl_class = NULL;
		axutil_param_t *impl_info_param = NULL;

		axutil_hash_this(hi, NULL, NULL, &svc);
		if (!svc)
			continue;
		impl_class = axis2_svc_get_impl_class(svc, env);
		if (impl_class)
			continue;
		impl_info_param = axis2_svc_get_param(svc, env, AXIS2_SERVICE_CLASS);
		if (!impl_info_param)
			continue;

		axutil_class_loader_init(env);
		impl_class = axutil_class_loader_create_dll(env, impl_info_param);
		axis2_svc_set_impl_class(svc, env, impl_class);
		if (impl_class)
		{
			AXIS2_SVC_SKELETON_INIT((axis2_svc_skeleton_t *) impl_class, env);
		}
	}

	AXIS2_LOG_INFO(env->log,"[Axis2] Axis2 worker created");

    return apache2_worker;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_apache2_worker_free(
    axis2_apache2_worker_t * apache2_worker,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, void);

    if (apache2_worker->conf_ctx)
    {
        axis2_conf_ctx_free(apache2_worker->conf_ctx, env);
        apache2_worker->conf_ctx = NULL;
    }

    AXIS2_FREE(env->allocator, apache2_worker);

    return;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_apache2_worker_process_request(
    axis2_apache2_worker_t * apache2_worker,
    const axutil_env_t * env,
    request_rec * request)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axutil_stream_t *request_body = NULL;
    axutil_stream_t *out_stream = NULL;
    axis2_transport_out_desc_t *out_desc = NULL;
    axis2_transport_in_desc_t *in_desc = NULL;
    axis2_char_t *http_version = NULL;
    axutil_string_t *soap_action = NULL;
    axis2_bool_t processed = AXIS2_FALSE;
    int content_length = -1;
    axis2_char_t *encoding_header_value = NULL;
    axis2_char_t *url_external_form = NULL;
    axis2_char_t *body_string = NULL;
    unsigned int body_string_len = 0;
    int send_status = DECLINED;
    axis2_char_t *content_type = NULL;
    axis2_http_out_transport_info_t *apache2_out_transport_info = NULL;
    axis2_char_t *ctx_uuid = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_char_t *peer_ip = NULL;
    axutil_property_t *peer_property = NULL;
    axutil_url_t *request_url = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
    AXIS2_PARAM_CHECK(env->error, request, AXIS2_CRITICAL_FAILURE);

    conf_ctx = apache2_worker->conf_ctx;

    if (!conf_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
                        AXIS2_FAILURE);
        return AXIS2_CRITICAL_FAILURE;
    }
    content_length = (int)request->remaining;
    /* We are sure that the difference lies within the int range */
    http_version = request->protocol;

    request_url = axutil_url_create(env, "http",
                                    request->hostname,
                                    request->parsed_uri.port,
                                    request->unparsed_uri);
    if (request_url)
    {
        url_external_form = axutil_url_to_external_form(request_url, env);
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, url_external_form);
        axutil_url_free(request_url, env);
        request_url = NULL;
    }
    else
    {
        send_status = OK;
        request->status = HTTP_BAD_REQUEST;
        return send_status;
    }

    content_type = (axis2_char_t *) apr_table_get(request->headers_in,
                                                  AXIS2_HTTP_HEADER_CONTENT_TYPE);
    if (!content_type)
    {
        content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN;
    }
    request->content_type = content_type;
    if (request->read_chunked == AXIS2_TRUE && 0 == content_length)
    {
        content_length = -1;
        request->chunked = 1;
    }
    if (!http_version)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_HTTP_VERSION,
                        AXIS2_FAILURE);
        return AXIS2_CRITICAL_FAILURE;
    }
    out_stream = axutil_stream_create_basic(env);
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Client HTTP version %s",
                    http_version);

    encoding_header_value = (axis2_char_t *) request->content_encoding;

    out_desc = axis2_conf_get_transport_out(axis2_conf_ctx_get_conf
                                            (apache2_worker->conf_ctx, env),
                                            env, AXIS2_TRANSPORT_ENUM_HTTP);
    in_desc =
        axis2_conf_get_transport_in(axis2_conf_ctx_get_conf
                                    (apache2_worker->conf_ctx, env), env,
                                    AXIS2_TRANSPORT_ENUM_HTTP);

    msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);

    peer_ip = request->connection->remote_ip;
    
    if (peer_ip)
    {
        peer_property = axutil_property_create(env);
        axutil_property_set_value(peer_property, env,
                                  axutil_strdup(env, peer_ip));
        axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_SVR_PEER_IP_ADDR,
                                   peer_property);
    }

    axis2_msg_ctx_set_transport_out_stream(msg_ctx, env, out_stream);

    ctx_uuid = axutil_uuid_gen(env);
    if (ctx_uuid)
    {
        axutil_string_t *uuid_str =
            axutil_string_create_assume_ownership(env, &ctx_uuid);
        axis2_msg_ctx_set_svc_grp_ctx_id(msg_ctx, env, uuid_str);
        axutil_string_free(uuid_str, env);
    }

    apache2_out_transport_info = axis2_apache2_out_transport_info_create(env,
                                                                         request);
    axis2_msg_ctx_set_http_out_transport_info(msg_ctx, env,
                                              apache2_out_transport_info);

    soap_action = axutil_string_create(env,
                                       (axis2_char_t *) apr_table_get(request->
                                                                      headers_in,
                                                                      AXIS2_HTTP_HEADER_SOAP_ACTION));
    request_body = axutil_stream_create_apache2(env, request);
    if (!request_body)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occured in"
                        " creating input stream.");
        return AXIS2_CRITICAL_FAILURE;
    }
    if (M_GET == request->method_number || M_DELETE == request->method_number)
    {
        if (M_DELETE == request->method_number)
        {
            processed = axis2_http_transport_utils_process_http_delete_request
                (env, msg_ctx, request_body, out_stream,
                 content_type, soap_action,
                 url_external_form,
                 conf_ctx,
                 axis2_http_transport_utils_get_request_params(env,
                                                               (axis2_char_t *)
                                                               url_external_form));
        }
        else if (request->header_only)
        {
            processed = axis2_http_transport_utils_process_http_head_request
                (env, msg_ctx, request_body, out_stream,
                 content_type, soap_action,
                 url_external_form,
                 conf_ctx,
                 axis2_http_transport_utils_get_request_params(env,
                                                               (axis2_char_t *)
                                                               url_external_form));
        }
        else
        {
            processed = axis2_http_transport_utils_process_http_get_request
                (env, msg_ctx, request_body, out_stream,
                 content_type, soap_action,
                 url_external_form,
                 conf_ctx,
                 axis2_http_transport_utils_get_request_params(env,
                                                               (axis2_char_t *)
                                                               url_external_form));
        }
        if (AXIS2_FALSE == processed)
        {
            axis2_char_t *wsdl = NULL;
            axis2_bool_t is_services_path = AXIS2_FALSE;
            if (M_DELETE != request->method_number)
            {
                axis2_char_t *temp = NULL;
                temp = strstr(url_external_form, AXIS2_REQUEST_URL_PREFIX);
                if (temp)
                {
                    temp += strlen(AXIS2_REQUEST_URL_PREFIX);
                    if (*temp == '/')
                    {
                        temp++;
                    }
                    if (!*temp || *temp == '?' || *temp == '#')
                    {
                        is_services_path = AXIS2_TRUE;
                    }
                }
            }
            wsdl = strstr(url_external_form, AXIS2_REQUEST_WSDL);
            if (is_services_path)
            {
                body_string =
                    axis2_http_transport_utils_get_services_html(env, conf_ctx);
                request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
            }
            else if (M_DELETE != request->method_number && wsdl)
            {
                body_string =
                    axis2_http_transport_utils_get_services_static_wsdl(env,
                                                                        conf_ctx,
                                                                        (axis2_char_t
                                                                         *)
                                                                        url_external_form);
                request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
                           
            }
            else if (env->error->error_number == AXIS2_ERROR_SVC_OR_OP_NOT_FOUND)
            {
                axutil_array_list_t *method_list = NULL;
                int size = 0;
                method_list = axis2_msg_ctx_get_supported_rest_http_methods(msg_ctx, env);
                size = axutil_array_list_size(method_list, env);
                if (method_list && size)
                {
                    axis2_char_t *method_list_str = NULL;
                    axis2_char_t *temp;
                    int i = 0;
                    method_list_str = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 29);
                    temp = method_list_str;
                    request->allowed_methods->method_mask = 0;
                    for (i = 0; i < size; i++)
                    {
                        if (i)
                        {
                            sprintf(temp, ", ");
                            temp += 2;
                        }
                        sprintf(temp, "%s", (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i));
                        temp += strlen(temp);
                        /* Conditions below is to assist down-stream modules */
                        if (!strcasecmp(AXIS2_HTTP_PUT, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_PUT;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_POST, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_POST;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_GET, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_GET;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_HEAD, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            /* Apache Can't differentiate between HEAD and GET */
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_GET;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_DELETE, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_DELETE;
                        }
                    }
                    *temp = '\0';
                    apr_table_set(request->err_headers_out, AXIS2_HTTP_HEADER_ALLOW, method_list_str);
                    AXIS2_FREE(env->allocator, method_list_str);
                    body_string =
                        axis2_http_transport_utils_get_method_not_allowed(env, conf_ctx);
                    request->status = HTTP_METHOD_NOT_ALLOWED;
                }
                else
                {
                    body_string =
                        axis2_http_transport_utils_get_not_found(env, conf_ctx);
                    request->status = HTTP_NOT_FOUND;
                }
                request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
            }
            else
            {
                body_string =
                    axis2_http_transport_utils_get_internal_server_error(env, conf_ctx);
                request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
                request->status = HTTP_INTERNAL_SERVER_ERROR;
            }

            if (body_string)
            {
                body_string_len = axutil_strlen(body_string);
            }
            send_status = OK;
        }
    }
    else if (M_POST == request->method_number || M_PUT == request->method_number)
    {
        axis2_status_t status = AXIS2_FAILURE;
        if (M_POST == request->method_number)
        {
            status = axis2_http_transport_utils_process_http_post_request
                (env, msg_ctx, request_body, out_stream,
                 content_type, content_length,
                 soap_action, (axis2_char_t *) url_external_form);
        }
        else
        {
            status = axis2_http_transport_utils_process_http_put_request
                (env, msg_ctx, request_body, out_stream,
                 content_type, content_length,
                 soap_action, (axis2_char_t *) url_external_form);
        }
        if (AXIS2_FAILURE == status && (M_PUT == request->method_number || 
            axis2_msg_ctx_get_doing_rest(msg_ctx, env)))
        {
            if (env->error->error_number == AXIS2_ERROR_SVC_OR_OP_NOT_FOUND)
            {
                axutil_array_list_t *method_list = NULL;
                int size = 0;
                method_list = axis2_msg_ctx_get_supported_rest_http_methods(msg_ctx, env);
                size = axutil_array_list_size(method_list, env);
                if (method_list && size)
                {
                    axis2_char_t *method_list_str = NULL;
                    axis2_char_t *temp;
                    int i = 0;
                    method_list_str = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 29);
                    temp = method_list_str;
                    request->allowed_methods->method_mask = 0;
                    for (i = 0; i < size; i++)
                    {
                        if (i)
                        {
                            sprintf(temp, ", ");
                            temp += 2;
                        }
                        sprintf(temp, "%s", (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i));
                        temp += strlen(temp);
                        /* Conditions below is to assist down-stream modules */
                        if (!strcasecmp(AXIS2_HTTP_PUT, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_PUT;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_POST, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_POST;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_GET, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_GET;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_HEAD, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            /* Apache Can't differentiate between HEAD and GET */
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_GET;
                        }
                        else if (!strcasecmp(AXIS2_HTTP_DELETE, (axis2_char_t *) 
                                axutil_array_list_get(method_list, env, i)))
                        {
                            request->allowed_methods->method_mask |= AP_METHOD_BIT << M_DELETE;
                        }
                    }
                    *temp = '\0';
                    apr_table_set(request->err_headers_out, AXIS2_HTTP_HEADER_ALLOW, method_list_str);
                    AXIS2_FREE(env->allocator, method_list_str);
                    body_string =
                        axis2_http_transport_utils_get_method_not_allowed(env, conf_ctx);
                    request->status = HTTP_METHOD_NOT_ALLOWED;
                }
                else
                {
                    body_string =
                        axis2_http_transport_utils_get_not_found(env, conf_ctx);
                    request->status = HTTP_NOT_FOUND;
                }
                request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
            }
            else
            {
                body_string =
                    axis2_http_transport_utils_get_internal_server_error(env, conf_ctx);
                request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
                request->status = HTTP_INTERNAL_SERVER_ERROR;
            }

            if (body_string)
            {
                body_string_len = axutil_strlen(body_string);
            }
            send_status = OK;
        }
        else if (status == AXIS2_FAILURE)
        {
            axis2_msg_ctx_t *fault_ctx = NULL;
            axis2_char_t *fault_code = NULL;
            axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
            if (!engine)
            {
                /* Critical error, cannot proceed, Apache will send default 
                   document for 500
                 */
                return AXIS2_CRITICAL_FAILURE;
            }
            if (axis2_msg_ctx_get_is_soap_11(msg_ctx, env))
            {
                fault_code = AXIOM_SOAP_DEFAULT_NAMESPACE_PREFIX ":"
                    AXIOM_SOAP11_FAULT_CODE_SENDER;
            }
            else
            {
                fault_code = AXIOM_SOAP_DEFAULT_NAMESPACE_PREFIX ":"
                    AXIOM_SOAP12_SOAP_FAULT_VALUE_SENDER;
            }
            fault_ctx = axis2_engine_create_fault_msg_ctx(engine, env, msg_ctx,
                                                          fault_code,
                                                          axutil_error_get_message
                                                          (env->error));
            axis2_engine_send_fault(engine, env, fault_ctx);
            if (out_stream)
            {
                body_string = axutil_stream_get_buffer(out_stream, env);
                body_string_len = axutil_stream_get_len(out_stream, env);
            }

            /* In case of a SOAP Fault, we have to set the status to 500,
               but still return OK because the module has handled the error 
             */
            send_status = OK;
            request->status = HTTP_INTERNAL_SERVER_ERROR;
        }
    }
    else
    {
        body_string =
            axis2_http_transport_utils_get_not_implemented(env, conf_ctx);
        request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;

        if (body_string)
        {
            body_string_len = axutil_strlen(body_string);
        }
        send_status = OK;
        request->status = HTTP_NOT_IMPLEMENTED;
    }

    op_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, env);

    if (send_status == DECLINED)
    {
        axis2_bool_t do_rest = AXIS2_FALSE;
        axis2_char_t *accept_header_value = NULL;
        axis2_char_t *accept_charset_header_value = NULL;
        if (M_POST != request->method_number ||
            axis2_msg_ctx_get_doing_rest(msg_ctx, env))
        {
            do_rest = AXIS2_TRUE;
        }
        accept_header_value = (axis2_char_t *)
            apr_table_get(request->headers_in, AXIS2_HTTP_HEADER_ACCEPT);
        accept_charset_header_value = (axis2_char_t *)
            apr_table_get(request->headers_in, AXIS2_HTTP_HEADER_ACCEPT_CHARSET);
        if ((accept_header_value || accept_charset_header_value) && do_rest)
        {
            axis2_char_t *content_type_header_value = NULL;
            axis2_char_t *temp = NULL;
            content_type_header_value = (axis2_char_t *) request->content_type;
            if (content_type_header_value)
            {
                temp = axutil_strdup(env, content_type_header_value);
            }
            if (temp)
            {
                axis2_char_t *content_type = NULL;
                axis2_char_t *char_set = NULL;
                axis2_char_t *temp2 = NULL;
                
                temp2 = strchr(temp, ';');
                if (temp2)
                {
                    *temp2 = '\0';
                    temp2++;
                    char_set = strstr(temp2, AXIS2_HTTP_CHAR_SET_ENCODING);
                }
                if (char_set)
                {
                    char_set = axutil_strltrim(env, char_set, " \t=");
                }
                if (char_set)
                {
                    temp2 = strchr(char_set, ';');
                }
                if (temp2)
                {
                    *temp2 = '\0';
                }
                content_type = axutil_strtrim(env, temp, NULL);

                if (temp)
                {
                    AXIS2_FREE(env->allocator, temp);
                    temp = NULL;
                }
                if (content_type && accept_header_value &&
                    !strstr(accept_header_value, content_type))
                {
                    temp2 = strchr(content_type, '/');
                    if (temp2)
                    {
                        *temp2 = '\0';
                        temp = AXIS2_MALLOC(env->allocator,
                                   sizeof(axis2_char_t) * ((int)strlen(content_type) + 3));
                        if (!temp)
                        {
                            AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
                            return AXIS2_FALSE;
                        }
                        sprintf(temp, "%s/*", content_type);
                        if (!strstr(accept_header_value, temp) &&
                            !strstr(accept_header_value, AXIS2_HTTP_HEADER_ACCEPT_ALL))
                        {
                            body_string =
                                axis2_http_transport_utils_get_not_acceptable(env, conf_ctx);
                            request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;

                            if (body_string)
                            {
                                body_string_len = axutil_strlen(body_string);
                            }
                            send_status = OK;
                            request->status = HTTP_NOT_ACCEPTABLE;
                        }
                        AXIS2_FREE(env->allocator, temp);
                    }
                }
                if (content_type)
                {
                    AXIS2_FREE(env->allocator, content_type);
                }
                if (char_set && accept_charset_header_value && 
                    !strstr(accept_charset_header_value, char_set))
                {
                    body_string =
                        axis2_http_transport_utils_get_not_acceptable(env, conf_ctx);
                    request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;

                    if (body_string)
                    {
                        body_string_len = axutil_strlen(body_string);
                    }
                    send_status = OK;
                    request->status = HTTP_NOT_ACCEPTABLE;
                }
                if (char_set)
                {
                    AXIS2_FREE(env->allocator, char_set);
                }
            }
        }
    }
    if (send_status == DECLINED)
    {
        axis2_bool_t do_rest = AXIS2_FALSE;
        if (M_POST != request->method_number ||
            axis2_msg_ctx_get_doing_rest(msg_ctx, env))
        {
            do_rest = AXIS2_TRUE;
        }
        if (op_ctx && axis2_op_ctx_get_response_written(op_ctx, env))
        {
            if (do_rest)
            {
                axis2_msg_ctx_t *out_msg_ctx = NULL;
                axis2_msg_ctx_t *in_msg_ctx = NULL;
                axis2_msg_ctx_t **msg_ctx_map = NULL;

                msg_ctx_map = axis2_op_ctx_get_msg_ctx_map(op_ctx, env);
                out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];
                in_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN];
                if (in_msg_ctx)
                {
                    /* TODO: Add neccessary handling */
                }
                if (out_msg_ctx)
                {
                    /* TODO: Add neccessary handling */
                }
            }
            if (send_status == DECLINED)
            {
                send_status = OK;
                if (out_stream)
                {
                    body_string = axutil_stream_get_buffer(out_stream, env);
                    body_string_len = axutil_stream_get_len(out_stream, env);
                }
            }
        }
        else if (op_ctx)
        {
            if (do_rest)
            {
                axis2_msg_ctx_t *out_msg_ctx = NULL;
                axis2_msg_ctx_t *in_msg_ctx = NULL;
                axis2_msg_ctx_t **msg_ctx_map = NULL;

                msg_ctx_map = axis2_op_ctx_get_msg_ctx_map(op_ctx, env);
                out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];
                in_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN];
                if (in_msg_ctx)
                {
                    /* TODO: Add neccessary handling */
                }
                if (out_msg_ctx)
                {
                    if (axis2_msg_ctx_get_no_content(out_msg_ctx, env))
                    {
                        request->status = HTTP_NO_CONTENT;
                        send_status = DONE;
                    }
                }
            }
            if (send_status == DECLINED)
            {
                request->status = HTTP_ACCEPTED;
                send_status = DONE;
            }
        }
        else
        {
            send_status = DONE;
            request->status = HTTP_ACCEPTED;
        }
    }

    if (op_ctx)
    {
        axis2_msg_ctx_t *out_msg_ctx = NULL,
            *in_msg_ctx = NULL;
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
            msg_id =
                axutil_strdup(env, axis2_msg_ctx_get_msg_id(in_msg_ctx, env));
            conf_ctx = axis2_msg_ctx_get_conf_ctx(in_msg_ctx, env);
            axis2_msg_ctx_reset_http_out_transport_info(in_msg_ctx, env);
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
    if (body_string)
    {
        ap_rwrite(body_string, body_string_len, request);
        body_string = NULL;
    }

    if (request_body)
    {
        axutil_stream_free(request_body, env);
        request_body = NULL;
    }

    axutil_string_free(soap_action, env);

    msg_ctx = NULL;
    return send_status;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_apache2_worker_get_bytes(
    const axutil_env_t * env,
    axutil_stream_t * stream)
{

    axutil_stream_t *tmp_stream = NULL;
    int return_size = -1;
    axis2_char_t *buffer = NULL;
    axis2_bool_t loop_status = AXIS2_TRUE;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, stream, NULL);

    tmp_stream = axutil_stream_create_basic(env);
    while (loop_status)
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
        return_size = axutil_stream_read(tmp_stream, env, buffer,
                                         return_size + 1);
        buffer[return_size + 1] = '\0';
    }
    axutil_stream_free(tmp_stream, env);
    return buffer;
}
