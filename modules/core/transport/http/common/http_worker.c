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
#include <axis2_uuid_gen.h>
#include <axis2_url.h>
#include <axis2_property.h>

struct axis2_http_worker
{
    axis2_conf_ctx_t *conf_ctx;
    int svr_port;
};

static axis2_status_t 
axis2_http_worker_set_response_headers(
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env,
    axis2_simple_http_svr_conn_t *svr_conn,
    axis2_http_simple_request_t *simple_request,
    axis2_http_simple_response_t *simple_response,
    axis2_ssize_t content_length);

static axis2_status_t 
axis2_http_worker_set_transport_out_config(
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    axis2_http_simple_response_t *simple_response);

static axis2_hash_t *
axis2_http_worker_get_headers(
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env,
    axis2_http_simple_request_t *request);

AXIS2_EXTERN axis2_http_worker_t *AXIS2_CALL
axis2_http_worker_create(
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx)
{
    axis2_http_worker_t *http_worker = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    http_worker = (axis2_http_worker_t *)
            AXIS2_MALLOC(env->allocator, sizeof(axis2_http_worker_t));

    if (! http_worker)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    http_worker->conf_ctx = conf_ctx;
    http_worker->svr_port = 9090; /* default - must set later*/

    return http_worker;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_http_worker_free(
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env)
{
    AXIS2_ENV_CHECK(env, void);

    http_worker->conf_ctx = NULL;

    AXIS2_FREE(env->allocator, http_worker);

    return;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_worker_process_request(
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env,
    axis2_simple_http_svr_conn_t *svr_conn,
    axis2_http_simple_request_t *simple_request)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axis2_stream_t *request_body = NULL;
    axis2_stream_t *out_stream = axis2_stream_create_basic(env);
    axis2_http_simple_response_t *response = NULL;
    axis2_transport_out_desc_t *out_desc = NULL;
    axis2_transport_in_desc_t *in_desc = NULL;
    axis2_char_t *http_version = NULL;
    axis2_char_t *soap_action = NULL;
    axis2_string_t *soap_action_str = NULL;
    axis2_bool_t processed = AXIS2_FALSE;
    axis2_status_t status = AXIS2_FAILURE;
    int content_length = -1;
    axis2_http_header_t *encoding_header = NULL;
    axis2_char_t *encoding_header_value = NULL;
    axis2_op_ctx_t *op_ctx = NULL;
    axis2_char_t *svr_ip = NULL;
    axis2_char_t *peer_ip = NULL;
    axis2_url_t *request_url = NULL;
    axis2_http_out_transport_info_t *http_out_transport_info = NULL;
    axis2_hash_t *headers = NULL;
    axis2_char_t *url_external_form = NULL;
    axis2_char_t *svc_grp_uuid = NULL;
    axis2_char_t *path = NULL;
    axis2_property_t *peer_property = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svr_conn, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_request, AXIS2_FAILURE);

    conf_ctx = http_worker->conf_ctx;

    if (! conf_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
                AXIS2_FAILURE);
        return AXIS2_FALSE;
    }

    content_length = AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_LENGTH(simple_request
            , env);
    http_version = AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION
            (AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE
                    (simple_request, env), env);
    if (! http_version)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_HTTP_VERSION,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Client HTTP version %s",
            http_version);

    response = axis2_http_simple_response_create_default(env);
    encoding_header = AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request,
            env, AXIS2_HTTP_HEADER_TRANSFER_ENCODING);
    if (encoding_header)
    {
        encoding_header_value = AXIS2_HTTP_HEADER_GET_VALUE(encoding_header,
                env);
    }
    if (content_length < 0 && (encoding_header_value && 0 != axis2_strcmp
            (encoding_header_value,
                    AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED)))
    {
        if (0 == axis2_strcasecmp(AXIS2_HTTP_REQUEST_LINE_GET_METHOD(
                    AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
                        simple_request, env), env), AXIS2_HTTP_HEADER_POST))
        {
            AXIS2_HTTP_SIMPLE_RESPONSE_SET_STATUS_LINE(response, env,
                    http_version, 411, "Length Required");
            status = AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env,
                    response);
            AXIS2_HTTP_SIMPLE_RESPONSE_FREE(response, env);
            response = NULL;
            return status;
        }

    }
    request_body = AXIS2_HTTP_SIMPLE_REQUEST_GET_BODY(simple_request, env);

    out_desc =  axis2_conf_get_transport_out( axis2_conf_ctx_get_conf
            (http_worker->conf_ctx, env), env,
            AXIS2_TRANSPORT_ENUM_HTTP);
    in_desc =  axis2_conf_get_transport_in( axis2_conf_ctx_get_conf
            (http_worker->conf_ctx, env), env,
            AXIS2_TRANSPORT_ENUM_HTTP);
    msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);


    if (0 == axis2_strcasecmp(http_version, AXIS2_HTTP_HEADER_PROTOCOL_11))
    {
        axis2_http_worker_set_transport_out_config(http_worker, env, conf_ctx,
                response);
    }

    svr_ip = AXIS2_SIMPLE_HTTP_SVR_CONN_GET_SVR_IP(svr_conn, env);
    peer_ip = AXIS2_SIMPLE_HTTP_SVR_CONN_GET_PEER_IP(svr_conn, env);

    if (peer_ip)
    {
        peer_property = axis2_property_create (env);
        axis2_property_set_value (peer_property, env, axis2_strdup (env, peer_ip));
        axis2_msg_ctx_set_property (msg_ctx, env, AXIS2_SVR_PEER_IP_ADDR, peer_property); 
    }

    path =   AXIS2_HTTP_REQUEST_LINE_GET_URI(
	AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
	    simple_request, env), env);

    request_url = axis2_url_create(env, "http", svr_ip,
				   http_worker->svr_port,
				   path);

	url_external_form = axis2_url_to_external_form(request_url, env);
    
    axis2_msg_ctx_set_transport_out_stream(msg_ctx, env, out_stream);

    headers = axis2_http_worker_get_headers(http_worker, env, simple_request);
    axis2_msg_ctx_set_transport_headers(msg_ctx, env, headers);

    svc_grp_uuid = axis2_uuid_gen(env);
    if (svc_grp_uuid)
    {
        axis2_string_t *svc_grp_uuid_str = axis2_string_create_assume_ownership(env, &svc_grp_uuid);
         axis2_msg_ctx_set_svc_grp_ctx_id(msg_ctx, env, svc_grp_uuid_str);
        axis2_string_free(svc_grp_uuid_str, env);
    }

    http_out_transport_info = axis2_http_out_transport_info_create(env, response);
    axis2_msg_ctx_set_http_out_transport_info(msg_ctx, env, http_out_transport_info);

    if (AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request, env,
            AXIS2_HTTP_HEADER_SOAP_ACTION))
    {
        soap_action = AXIS2_HTTP_HEADER_GET_VALUE(
                    AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(
                        simple_request, env, AXIS2_HTTP_HEADER_SOAP_ACTION),
                    env);
        soap_action_str = axis2_string_create(env, soap_action);
    }
    if (0 == axis2_strcasecmp(AXIS2_HTTP_REQUEST_LINE_GET_METHOD(
                AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
                    simple_request, env), env), AXIS2_HTTP_HEADER_GET))
    {
        processed = axis2_http_transport_utils_process_http_get_request
                (env, msg_ctx, request_body, out_stream,
                        AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_TYPE(
                            simple_request, env) , soap_action_str,
                        axis2_url_to_external_form(request_url, env),
                        conf_ctx,
                        axis2_http_transport_utils_get_request_params(env,
                                AXIS2_HTTP_REQUEST_LINE_GET_URI(
                                    AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
                                        simple_request, env), env)));
        if (AXIS2_FALSE == processed)
        {
            axis2_http_header_t *cont_len = NULL;
            axis2_char_t *body_string = NULL;
            AXIS2_HTTP_SIMPLE_RESPONSE_SET_STATUS_LINE(response, env,
                    http_version, AXIS2_HTTP_RESPONSE_OK_CODE_VAL, "OK");
    
            body_string = axis2_http_transport_utils_get_services_html(env,
                    conf_ctx);
            if (body_string)
            {
                axis2_char_t str_len[10];
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STRING(response, env,
                        body_string);
                sprintf(str_len, "%d", axis2_strlen(body_string));
                cont_len = axis2_http_header_create(env,
                        AXIS2_HTTP_HEADER_CONTENT_LENGTH, str_len);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(response, env, cont_len);
            }
            axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
                    simple_request, response, 0);
            AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env, response);
            AXIS2_HTTP_SIMPLE_RESPONSE_FREE(response, env);
            return AXIS2_TRUE;
        }
    }
    else if (0 == axis2_strcasecmp(AXIS2_HTTP_REQUEST_LINE_GET_METHOD(
                AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(
                    simple_request, env), env), AXIS2_HTTP_HEADER_POST))
    {
        status = axis2_http_transport_utils_process_http_post_request
                (env, msg_ctx, request_body, out_stream,
                        AXIS2_HTTP_SIMPLE_REQUEST_GET_CONTENT_TYPE(
                            simple_request, env) , content_length, soap_action_str,
                        url_external_form);
        if (status == AXIS2_FAILURE)
        {
            axis2_msg_ctx_t *fault_ctx = NULL;
            axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
            axis2_http_request_line_t *req_line = NULL;
            axis2_http_status_line_t *tmp_stat_line = NULL;
            axis2_char_t status_line_str[100];
            axis2_property_t *http_error_property = NULL;
            axis2_char_t *http_error_value = NULL;
            if (! engine)
            {
                return AXIS2_FALSE;
            }

            http_error_property =  axis2_msg_ctx_get_property(msg_ctx, env,
                AXIS2_HTTP_TRANSPORT_ERROR);

            if (http_error_property)
                http_error_value = (axis2_char_t *)axis2_property_get_value(
                    http_error_property, env);

            fault_ctx =  axis2_engine_create_fault_msg_ctx(engine, env, msg_ctx);
            req_line = AXIS2_HTTP_SIMPLE_REQUEST_GET_REQUEST_LINE(simple_request
                , env);
            if (req_line)
            {
                if (!http_error_value)
                {
                    sprintf(status_line_str, "%s 500 Internal Server Error\r\n",
                            AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION(req_line,
                                env));
                }
                else
                {
                    sprintf(status_line_str, "%s %s",
                        AXIS2_HTTP_REQUEST_LINE_GET_HTTP_VERSION(req_line,env),
                        http_error_value);
                }
            }
            else
            {
                sprintf(status_line_str, "HTTP/1.1 500 Internal Server Error\
                        \r\n");
            }

            tmp_stat_line = axis2_http_status_line_create(env,
                    status_line_str);

            if (!http_error_value)
                axis2_engine_send_fault(engine, env, fault_ctx);

            AXIS2_HTTP_SIMPLE_RESPONSE_SET_STATUS_LINE(response, env,
                    AXIS2_HTTP_STATUS_LINE_GET_HTTP_VERSION(tmp_stat_line, env),
                    AXIS2_HTTP_STATUS_LINE_GET_STATUS_CODE(tmp_stat_line, env) ,
                    AXIS2_HTTP_STATUS_LINE_GET_REASON_PHRASE(tmp_stat_line, env));
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

    op_ctx =  axis2_msg_ctx_get_op_ctx(msg_ctx, env);
    if (axis2_op_ctx_get_response_written(op_ctx, env))
    {
        AXIS2_HTTP_SIMPLE_RESPONSE_SET_STATUS_LINE(response, env, http_version,
                AXIS2_HTTP_RESPONSE_OK_CODE_VAL, "OK");
        AXIS2_HTTP_SIMPLE_RESPONSE_SET_BODY_STREAM(response, env, out_stream);
    }
    else
    {
        AXIS2_HTTP_SIMPLE_RESPONSE_SET_STATUS_LINE(response, env, http_version,
                AXIS2_HTTP_RESPONSE_ACK_CODE_VAL, "Accepted");
    }
    axis2_http_worker_set_response_headers(http_worker, env, svr_conn,
            simple_request, response, AXIS2_STREAM_BASIC_GET_LEN(
                out_stream, env));

    status = AXIS2_SIMPLE_HTTP_SVR_CONN_WRITE_RESPONSE(svr_conn, env, response);
    AXIS2_FREE(env->allocator, url_external_form);
    url_external_form = NULL;
    op_ctx =  axis2_msg_ctx_get_op_ctx(msg_ctx, env);
    if (op_ctx) 
    {
        axis2_msg_ctx_t *out_msg_ctx = NULL, *in_msg_ctx = NULL;
        axis2_msg_ctx_t **msg_ctx_map = NULL;

        msg_ctx_map =  axis2_op_ctx_get_msg_ctx_map(op_ctx, env);

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
            axis2_msg_ctx_free(in_msg_ctx, env);
            in_msg_ctx = NULL;
            msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_IN] = NULL;
        }

    } /* Done freeing message contexts */
    
    msg_ctx = NULL;
    axis2_url_free(request_url, env);
    axis2_string_free(soap_action_str, env);
    request_url = NULL;
    return status;
}

static axis2_status_t 
axis2_http_worker_set_response_headers(
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env,
    axis2_simple_http_svr_conn_t *svr_conn,
    axis2_http_simple_request_t *simple_request,
    axis2_http_simple_response_t *simple_response,
    axis2_ssize_t content_length)
{
    axis2_http_header_t *conn_header = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, svr_conn, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_request, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_response, AXIS2_FAILURE);
    if (AXIS2_FALSE == AXIS2_HTTP_SIMPLE_RESPONSE_CONTAINS_HEADER
            (simple_response, env, AXIS2_HTTP_HEADER_CONNECTION))
    {
        conn_header = AXIS2_HTTP_SIMPLE_REQUEST_GET_FIRST_HEADER(simple_request,
                env, AXIS2_HTTP_HEADER_CONNECTION);
        if (conn_header)
        {
            if (0 == axis2_strcasecmp(AXIS2_HTTP_HEADER_GET_VALUE(conn_header,
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
            if (0 == axis2_strcasecmp(AXIS2_HTTP_HEADER_GET_VALUE(conn_header,
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
            if (AXIS2_HTTP_SIMPLE_RESPONSE_GET_HTTP_VERSION(simple_response, env)
                    && axis2_strcasecmp(
                        AXIS2_HTTP_SIMPLE_RESPONSE_GET_HTTP_VERSION(
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
        if (AXIS2_FALSE == AXIS2_HTTP_SIMPLE_REQUEST_CONTAINS_HEADER(
                    simple_request, env,
                    AXIS2_HTTP_HEADER_TRANSFER_ENCODING))
        {
            if (0 != content_length)
            {
                axis2_char_t content_len_str[10];
                axis2_http_header_t *content_len_hdr = NULL;

                sprintf(content_len_str, "%d", content_length);
                content_len_hdr = axis2_http_header_create(
                            env, AXIS2_HTTP_HEADER_CONTENT_LENGTH, content_len_str);
                AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env,
                        content_len_hdr);
            }
        }
        else
        {
            axis2_http_header_t *transfer_enc_header = axis2_http_header_create(
                        env, AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
                        AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED);
            AXIS2_HTTP_SIMPLE_RESPONSE_SET_HEADER(simple_response, env,
                    transfer_enc_header);
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
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    axis2_http_simple_response_t *simple_response)
{
    axis2_conf_t *config = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, simple_response, AXIS2_FAILURE);

    config =  axis2_conf_ctx_get_conf(conf_ctx, env);

    /*
        TODO implement the method
    */
    return AXIS2_SUCCESS;
}

static axis2_hash_t *
axis2_http_worker_get_headers(
    axis2_http_worker_t *http_worker,
    const axis2_env_t *env,
    axis2_http_simple_request_t *request)
{
    axis2_array_list_t *header_list = NULL;
    int hdr_count = 0;
    int i = 0;
    axis2_hash_t *header_map = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, request, AXIS2_FAILURE);

    header_list = AXIS2_HTTP_SIMPLE_REQUEST_GET_HEADERS(request, env);
    if (!header_list)
    {
        return NULL;
    }
    hdr_count = axis2_array_list_size(header_list, env);
    if (0 == hdr_count)
    {
        return NULL;
    }

    for (i = 0; i < hdr_count; i++)
    {
        axis2_http_header_t *tmp_hdr = NULL;
        tmp_hdr = axis2_array_list_get(header_list, env, i);
        if (!tmp_hdr)
        {
            continue;
        }
        if (!header_map)
        {
            header_map = axis2_hash_make(env);
            if (!header_map)
            {
                return NULL;
            }
        }
        axis2_hash_set(header_map, AXIS2_HTTP_HEADER_GET_NAME(tmp_hdr, env),
                AXIS2_HASH_KEY_STRING, tmp_hdr);
    }
    return header_map;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_worker_set_svr_port(
    axis2_http_worker_t *worker,
    const axis2_env_t *env,
    int port)
{
    worker->svr_port = port;
    return AXIS2_SUCCESS;
}
