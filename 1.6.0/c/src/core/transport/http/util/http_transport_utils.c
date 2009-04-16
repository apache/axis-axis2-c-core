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

#include <axis2_http_transport_utils.h>
#include <string.h>
#include <ctype.h>
#include <axis2_conf.h>
#include <axis2_op.h>
#include <axutil_qname.h>
#include <axis2_http_transport.h>
#include <axiom_soap_builder.h>
#include <axis2_engine.h>
#include <axiom_soap_body.h>
#include <axutil_utils.h>
#include <axiom_namespace.h>
#include <axiom_node.h>
#include <axutil_hash.h>
#include <axiom_soap_const.h>
#include <axis2_http_header.h>
#include <axutil_property.h>
#include <axutil_utils.h>
#include <axiom_mime_parser.h>
#include <axis2_http_accept_record.h>
#include <axis2_disp.h>
#include <axis2_msg.h>
#include <axutil_string_util.h>
#include <stdlib.h>
#include <axutil_uuid_gen.h>
#include <platforms/axutil_platform_auto_sense.h>
#include <axiom_mime_part.h>
#include <axutil_class_loader.h>

#define AXIOM_MIME_BOUNDARY_BYTE 45

/** Size of the buffer to be used when reading a file */
#ifndef AXIS2_FILE_READ_SIZE
#define AXIS2_FILE_READ_SIZE 2048
#endif

/** Content length value to be used in case of chunked content  */
#ifndef AXIS2_CHUNKED_CONTENT_LENGTH
#define AXIS2_CHUNKED_CONTENT_LENGTH 100000000
#endif

const axis2_char_t *AXIS2_TRANS_UTIL_DEFAULT_CHAR_ENCODING =
    AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING;

/***************************** Function headers *******************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_process_http_post_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    const int content_length,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_process_http_put_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    const int content_length,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_get_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri,
    axis2_conf_ctx_t * conf_ctx,
    axutil_hash_t * request_params);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_head_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri,
    axis2_conf_ctx_t * conf_ctx,
    axutil_hash_t * request_params);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_delete_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri,
    axis2_conf_ctx_t * conf_ctx,
    axutil_hash_t * request_params);


AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL
axis2_http_transport_utils_select_builder_for_mime(
    const axutil_env_t * env,
    axis2_char_t * request_uri,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axis2_char_t * content_type);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL axis2_http_transport_utils_is_optimized(
    const axutil_env_t * env,
    axiom_element_t * om_element);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL axis2_http_transport_utils_do_write_mtom(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx);

AXIS2_EXTERN axis2_status_t AXIS2_CALL axis2_http_transport_utils_strdecode(
    const axutil_env_t * env,
    axis2_char_t * dest,
    axis2_char_t * src);

AXIS2_EXTERN int AXIS2_CALL axis2_http_transport_utils_hexit(
    axis2_char_t c);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_services_html(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_services_static_wsdl(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_char_t * request_url);

AXIS2_EXTERN axutil_string_t *AXIS2_CALL
axis2_http_transport_utils_get_charset_enc(
    const axutil_env_t * env,
    const axis2_char_t * content_type);

int AXIS2_CALL axis2_http_transport_utils_on_data_request(
    char *buffer,
    int size,
    void *ctx);

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_create_soap_msg(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    const axis2_char_t * soap_ns_uri);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_value_from_content_type(
    const axutil_env_t * env,
    const axis2_char_t * content_type,
    const axis2_char_t * key);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_dispatch_and_verify(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx);

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_handle_media_type_url_encoded(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_hash_t * param_map,
    axis2_char_t * method);

static axis2_status_t
axis2_http_transport_utils_send_attachment_using_file(
    const axutil_env_t * env,
    axutil_http_chunked_stream_t *chunked_stream,
    FILE *fp,
    axis2_byte_t *buffer,
    int buffer_size);

static axis2_status_t
axis2_http_transport_utils_send_attachment_using_callback(
    const axutil_env_t * env,
    axutil_http_chunked_stream_t *chunked_stream,
    axiom_mtom_sending_callback_t *callback,
    void *handler,
    void *user_param);


/***************************** End of function headers ************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_transport_out_init(axis2_http_transport_out_t *response, 
											const axutil_env_t *env)
{
	response->http_status_code_name = NULL;
	response->http_status_code = 0;
	response->msg_ctx = NULL;
	response->response_data = NULL;
	response->content_type = NULL;
	response->response_data_length = 0;
	response->content_language = NULL;
	response->output_headers = NULL;
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_transport_out_uninit(axis2_http_transport_out_t *response, 
											const axutil_env_t *env)
{	
	if (response->msg_ctx)
	{
		axis2_msg_ctx_free(response->msg_ctx, env);
	}
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_transport_in_init(axis2_http_transport_in_t *request, 
											const axutil_env_t *env)
{
	request->content_type = NULL;
	request->content_length = 0;
	request->msg_ctx = NULL;
	request->soap_action = NULL;
	request->request_uri = NULL;
	request->in_stream = NULL;
	request->remote_ip = NULL;
	request->svr_port = NULL;
	request->transfer_encoding = NULL;
	request->accept_header = NULL;
	request->accept_language_header = NULL;
	request->accept_charset_header = NULL;
	request->request_method = 0;
	request->out_transport_info = NULL;
	request->request_url_prefix = NULL;
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_transport_in_uninit(axis2_http_transport_in_t *request, 
											   const axutil_env_t *env)
{
	if (request->msg_ctx)
	{
		axis2_msg_ctx_reset_out_transport_info(request->msg_ctx, env);
		axis2_msg_ctx_free(request->msg_ctx, env);
	}
	return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_process_http_post_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    const int content_length,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_builder_t *soap_builder = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axis2_bool_t is_soap11 = AXIS2_FALSE;
    axiom_xml_reader_t *xml_reader = NULL;
    axutil_string_t *char_set_str = NULL;

    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_callback_info_t *callback_ctx = NULL;
    axis2_callback_info_t *mime_cb_ctx = NULL;
    axutil_hash_t *headers = NULL;
    axis2_engine_t *engine = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axutil_hash_t *binary_data_map = NULL;
    axis2_char_t *soap_body_str = NULL;
    axutil_stream_t *stream = NULL;
    axis2_bool_t do_rest = AXIS2_FALSE;
    axis2_bool_t run_as_get = AXIS2_FALSE;
    axis2_char_t *soap_action = NULL;
    unsigned int soap_action_len = 0;
    axutil_property_t *http_error_property = NULL;
    axiom_mime_parser_t *mime_parser = NULL;
    axis2_bool_t is_svc_callback = AXIS2_FALSE;


    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_stream, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_stream, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FAILURE);

    conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);

    callback_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t));
    /* Note: the memory created above is freed in xml reader free function
       as this is passed on to the reader */
    callback_ctx->in_stream = in_stream;
    callback_ctx->env = env;
    callback_ctx->content_length = content_length;
    callback_ctx->unread_len = content_length;
    callback_ctx->chunked_stream = NULL;

    soap_action =
        (axis2_char_t *) axutil_string_get_buffer(soap_action_header, env);
    soap_action_len = axutil_string_get_length(soap_action_header, env);

    if (soap_action && (soap_action_len > 0))
    {
        /* remove leading and trailing " s */
        if (AXIS2_DOUBLE_QUOTE == soap_action[0])
        {
            memmove(soap_action, soap_action + sizeof(char),
                    soap_action_len - 1 + sizeof(char));
        }
        if (AXIS2_DOUBLE_QUOTE == soap_action[soap_action_len - 2])
        {
            soap_action[soap_action_len - 2] = AXIS2_ESC_NULL;
        }
    }else{
    /** soap action is null, check whether soap action is in content_type header */
        soap_action = axis2_http_transport_utils_get_value_from_content_type(env,
                content_type, AXIS2_ACTION);
    }


    headers = axis2_msg_ctx_get_transport_headers(msg_ctx, env);
    if (headers)
    {
        axis2_http_header_t *encoding_header = NULL;
        encoding_header = (axis2_http_header_t *) axutil_hash_get(
            headers,
            AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
            AXIS2_HASH_KEY_STRING);

        if (encoding_header)
        {
            axis2_char_t *encoding_value = NULL;
            encoding_value = axis2_http_header_get_value(encoding_header, env);
            if (encoding_value && 
                0 == axutil_strcasecmp(encoding_value,
                                       AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                callback_ctx->chunked_stream =
                    axutil_http_chunked_stream_create(env, in_stream);

                if (!callback_ctx->chunked_stream)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occurred in"
                                    " creating in chunked stream.");
                    return AXIS2_FAILURE;
                }

                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "HTTP"
                                " stream chunked");
            }
        }
    }
    else
    {
        /* Encoding header is not available */
        /* check content encoding from msg ctx property */
        axis2_char_t *value = axis2_msg_ctx_get_transfer_encoding(msg_ctx, env);

        if (value && axutil_strstr(value, AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
        {
            /* In case Transfer encoding is set to message context, some streams strip chunking meta
			data, so chunked streams should not be created */
            
			callback_ctx->content_length = -1;
			callback_ctx->unread_len = -1;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "[chunked ] setting length to -1");
        }
    }

    /* when the message contains does not contain pure XML we can't send it 
     * directly to the parser, First we need to separate the SOAP part from
     * the attachment */
    
    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED))
    {
        /* get mime boundary */
        axis2_char_t *mime_boundary =
            axis2_http_transport_utils_get_value_from_content_type(
                env,
                content_type,
                AXIS2_HTTP_HEADER_CONTENT_TYPE_MIME_BOUNDARY);

        if (mime_boundary)
        {
            /*axiom_mime_parser_t *mime_parser = NULL;*/
            int soap_body_len = 0;
            axutil_param_t *buffer_size_param = NULL;
            axutil_param_t *max_buffers_param = NULL;
            axutil_param_t *attachment_dir_param = NULL;
            axutil_param_t *callback_name_param = NULL;
            axutil_param_t *enable_service_callback_param = NULL;
            axis2_char_t *value_size = NULL;
            axis2_char_t *value_num = NULL;
            axis2_char_t *value_dir = NULL;
            axis2_char_t *value_callback = NULL;
            axis2_char_t *value_enable_service_callback = NULL; 
            int size = 0;
            int num = 0;

            mime_parser = axiom_mime_parser_create(env);
            /* This is the size of the buffer we keep inside mime_parser
             * when parsing. */

            enable_service_callback_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                               env,
                                                               AXIS2_ENABLE_MTOM_SERVICE_CALLBACK);
            if(enable_service_callback_param)
            {
                value_enable_service_callback = (axis2_char_t *) axutil_param_get_value 
                    (enable_service_callback_param, env);
                if(value_enable_service_callback)
                {
                    if(!axutil_strcmp(value_enable_service_callback, AXIS2_VALUE_TRUE))
                    {
                        is_svc_callback = AXIS2_TRUE;        
                    }
                }    
            }


            buffer_size_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                               env,
                                                               AXIS2_MTOM_BUFFER_SIZE);
            if (buffer_size_param)
            {
                value_size =
                    (axis2_char_t *) axutil_param_get_value (buffer_size_param, env);
                if(value_size)
                {
                    size = atoi(value_size);
                    axiom_mime_parser_set_buffer_size(mime_parser, env, size);
                }
            }
            
            /* We create an array of buffers in order to conatin SOAP data inside
             * mime_parser. This is the number of sucj buffers */
            max_buffers_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                               env,
                                                               AXIS2_MTOM_MAX_BUFFERS);
            if (max_buffers_param)
            {
                value_num =
                    (axis2_char_t *) axutil_param_get_value (max_buffers_param, env);
                if(value_num)
                {
                    num = atoi(value_num);
                    axiom_mime_parser_set_max_buffers(mime_parser, env, num);
                }
            }
            /* If this paramter is there mime_parser will cached the attachment 
             * using to the directory for large attachments. */    

            callback_name_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                                   env,
                                                                   AXIS2_MTOM_CACHING_CALLBACK);
            if(callback_name_param)
            {
                value_callback =
                    (axis2_char_t *) axutil_param_get_value (callback_name_param, env);
                if(value_callback)
                {
                    axiom_mime_parser_set_caching_callback_name(mime_parser, env, value_callback);
                }
            }

            attachment_dir_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                                   env,
                                                                   AXIS2_ATTACHMENT_DIR);

            if(attachment_dir_param)
            {
                value_dir =
                    (axis2_char_t *) axutil_param_get_value (attachment_dir_param, env);
                if(value_dir)
                {
                    axiom_mime_parser_set_attachment_dir(mime_parser, env, value_dir);
                }
            }

            axiom_mime_parser_set_mime_boundary(mime_parser, env, mime_boundary);

            if(axiom_mime_parser_parse_for_soap(mime_parser, env, 
                    axis2_http_transport_utils_on_data_request, 
                    (void *) callback_ctx,
                    mime_boundary) == AXIS2_FAILURE)
            {
                return AXIS2_FAILURE;
            }
            
            if(!is_svc_callback)
            {
                binary_data_map = 
                    axiom_mime_parser_parse_for_attachments(mime_parser, env,
                                        axis2_http_transport_utils_on_data_request,
                                        (void *) callback_ctx,
                                        mime_boundary,
                                        NULL);
                if(!binary_data_map)
                {
                    return AXIS2_FAILURE;
                }
            }
            soap_body_len =
                axiom_mime_parser_get_soap_body_len(mime_parser, env);
            soap_body_str =
                axiom_mime_parser_get_soap_body_str(mime_parser, env);

            if(!is_svc_callback)
            {
                if(callback_ctx->chunked_stream)
                {
                    axutil_http_chunked_stream_free(callback_ctx->chunked_stream, env);
                    callback_ctx->chunked_stream = NULL;
                }
            }    
            else
            {
                mime_cb_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t)); 
                if(mime_cb_ctx)
                {
                    mime_cb_ctx->in_stream = callback_ctx->in_stream;
                    mime_cb_ctx->env = callback_ctx->env;
                    mime_cb_ctx->content_length = callback_ctx->content_length;
                    mime_cb_ctx->unread_len = callback_ctx->unread_len;
                    mime_cb_ctx->chunked_stream = callback_ctx->chunked_stream;
                }
            }

            stream = axutil_stream_create_basic(env);
            if (stream)
            {
                axutil_stream_write(stream, env, soap_body_str, soap_body_len);
                callback_ctx->in_stream = stream;
                callback_ctx->chunked_stream = NULL;
                callback_ctx->content_length = soap_body_len;
                callback_ctx->unread_len = soap_body_len;
            }
            /*axiom_mime_parser_free(mime_parser, env);
            mime_parser = NULL;*/

            /*AXIS2_FREE(env->allocator, mime_boundary);*/
        }
        /*AXIS2_FREE(env->allocator, mime_boundary);*/
    }

    if(soap_action_header)
    {
        axis2_msg_ctx_set_soap_action(msg_ctx, env, soap_action_header);

    }else if (soap_action)
    {
        axutil_string_t *soap_action_str = NULL;
        soap_action_str = axutil_string_create(env,soap_action);
        axis2_msg_ctx_set_soap_action(msg_ctx, env, soap_action_str);
        axutil_string_free(soap_action_str, env);
    }
    axis2_msg_ctx_set_to(msg_ctx, env, axis2_endpoint_ref_create(env,
                                                                 request_uri));

    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);

    char_set_str =
        axis2_http_transport_utils_get_charset_enc(env, content_type);
    xml_reader =
        axiom_xml_reader_create_for_io(env,
                                       axis2_http_transport_utils_on_data_request,
                                       NULL, (void *) callback_ctx,
                                       axutil_string_get_buffer(char_set_str,
                                                                env));

    if (!xml_reader)
    {
        return AXIS2_FAILURE;
    }

    axis2_msg_ctx_set_charset_encoding(msg_ctx, env, char_set_str);

    om_builder = axiom_stax_builder_create(env, xml_reader);
    if (!om_builder)
    {
        axiom_xml_reader_free(xml_reader, env);
        xml_reader = NULL;
        return AXIS2_FAILURE;
    }

    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
    {
        is_soap11 = AXIS2_FALSE;
        soap_builder = axiom_soap_builder_create(env, om_builder,
                                                 AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
        if (!soap_builder)
        {
            /* We should not be freeing om_builder here as it is done by
               axiom_soap_builder_create in case of error - Samisa */
            om_builder = NULL;
            xml_reader = NULL;
            axis2_msg_ctx_set_is_soap_11(msg_ctx, env, is_soap11);
            return AXIS2_FAILURE;
        }

        soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);
        if (!soap_envelope)
        {
            axiom_stax_builder_free(om_builder, env);
            om_builder = NULL;
            xml_reader = NULL;
            axiom_soap_builder_free(soap_builder, env);
            soap_builder = NULL;
            axis2_msg_ctx_set_is_soap_11(msg_ctx, env, is_soap11);
            return AXIS2_FAILURE;
        }
    }
    else if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML))
    {
        is_soap11 = AXIS2_TRUE;
        if (soap_action_header)
        {
            soap_builder = axiom_soap_builder_create(env, om_builder,
                                                     AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI);
            if (!soap_builder)
            {
                /* We should not be freeing om_builder here as it is done by
                   axiom_soap_builder_create in case of error - Samisa */
                om_builder = NULL;
                xml_reader = NULL;
                axis2_msg_ctx_set_is_soap_11(msg_ctx, env, is_soap11);
                return AXIS2_FAILURE;
            }
            soap_envelope =
                axiom_soap_builder_get_soap_envelope(soap_builder, env);
            if (!soap_envelope)
            {
                axiom_soap_builder_free(soap_builder, env);
                om_builder = NULL;
                xml_reader = NULL;
                soap_builder = NULL;
                axis2_msg_ctx_set_is_soap_11(msg_ctx, env, is_soap11);
                return AXIS2_FAILURE;
            }
        }
        else
        {
            /* REST support */
            do_rest = AXIS2_TRUE;
        }
    }

    else if (strstr
             (content_type, AXIS2_HTTP_HEADER_ACCEPT_X_WWW_FORM_URLENCODED))
    {
        /* REST support */
        do_rest = AXIS2_TRUE;
        run_as_get = AXIS2_TRUE;
    }
    else
    {
        http_error_property = axutil_property_create(env);
        axutil_property_set_value(http_error_property, env,
                                  AXIS2_HTTP_UNSUPPORTED_MEDIA_TYPE);
        axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_HTTP_TRANSPORT_ERROR,
                                   http_error_property);
    }

    if(soap_builder)
    {
        if(mime_parser)
        {
            axiom_soap_builder_set_mime_parser(soap_builder, env, mime_parser);
            if(mime_cb_ctx)
            {
                axiom_soap_builder_set_callback_ctx(soap_builder, env, mime_cb_ctx);
                axiom_soap_builder_set_callback_function(soap_builder, env,
                    axis2_http_transport_utils_on_data_request);
            }
            else if(is_svc_callback)
            {
                return AXIS2_FAILURE;
            }
        }
    }

    if (do_rest)
    {
        /* REST support */
        axutil_param_t *rest_param =
            axis2_msg_ctx_get_parameter(msg_ctx, env, AXIS2_ENABLE_REST);
        if (rest_param &&
            0 == axutil_strcmp(AXIS2_VALUE_TRUE,
                               axutil_param_get_value(rest_param, env)))
        {
            axiom_soap_body_t *def_body = NULL;
            axiom_document_t *om_doc = NULL;
            axiom_node_t *root_node = NULL;
            if (!run_as_get)
            {
                soap_envelope = axiom_soap_envelope_create_default_soap_envelope
                    (env, AXIOM_SOAP11);
                def_body = axiom_soap_envelope_get_body(soap_envelope, env);
                om_doc = axiom_stax_builder_get_document(om_builder, env);
                root_node = axiom_document_build_all(om_doc, env);
                axiom_soap_body_add_child(def_body, env, root_node);
            }
            axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_TRUE);
            axis2_msg_ctx_set_rest_http_method(msg_ctx, env, AXIS2_HTTP_POST);
			axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);
        }
        else
        {
            return AXIS2_FAILURE;
        }
        if (AXIS2_SUCCESS != axis2_http_transport_utils_dispatch_and_verify(env,
                                                                            msg_ctx))
        {
            return AXIS2_FAILURE;
        }
    }

    if (run_as_get)
    {
        axis2_char_t *buffer = NULL;
        axis2_char_t *new_url = NULL;
        buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (content_length + 1));
        if (!buffer)
        {
            return AXIS2_FAILURE;
        }
        axis2_http_transport_utils_on_data_request(buffer, content_length, (void *) callback_ctx);
        
        new_url = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 
            ((int)(strlen(request_uri) + strlen(buffer)) + 2));
        if (!new_url)
        {
            return AXIS2_FAILURE;
        }
        sprintf(new_url, "%s?%s", request_uri, buffer);
        AXIS2_FREE(env->allocator, buffer);

        soap_envelope =
            axis2_http_transport_utils_handle_media_type_url_encoded(env, msg_ctx,
            axis2_http_transport_utils_get_request_params(env, new_url), AXIS2_HTTP_POST);
    }

    if (binary_data_map)
    {
        axiom_soap_builder_set_mime_body_parts(soap_builder, env,
                                               binary_data_map);
    }

    axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);

    engine = axis2_engine_create(env, conf_ctx);

    if (!soap_envelope)
        return AXIS2_FAILURE;

    soap_body = axiom_soap_envelope_get_body(soap_envelope, env);

    if (!soap_body)
        return AXIS2_FAILURE;

    if(!is_svc_callback)
    {
        if (AXIS2_TRUE == axiom_soap_body_has_fault(soap_body, env))
        {
            status = axis2_engine_receive_fault(engine, env, msg_ctx);
        }
        else
        {
            status = axis2_engine_receive(engine, env, msg_ctx);
        }
    }
    else
    {
        status = axis2_engine_receive(engine, env, msg_ctx);
    }

    if (!axis2_msg_ctx_get_soap_envelope(msg_ctx, env) &&
        AXIS2_FALSE == is_soap11)
    {
        axiom_soap_envelope_t *def_envelope =
            axiom_soap_envelope_create_default_soap_envelope(env,
                                                             AXIOM_SOAP12);
        axis2_msg_ctx_set_soap_envelope(msg_ctx, env, def_envelope);
    }

    if (engine)
    {
        axis2_engine_free(engine, env);
    }

    if (soap_body_str)
    {
        AXIS2_FREE(env->allocator, soap_body_str);
    }

    if (stream)
    {
        axutil_stream_free(stream, env);
    }

    if (char_set_str)
    {
        axutil_string_free(char_set_str, env);
    }
    if (!soap_builder && om_builder)
    {
        axiom_stax_builder_free_self(om_builder, env);
        om_builder = NULL;
    }
    if(!axutil_string_get_buffer(soap_action_header, env) && soap_action)
    {
        AXIS2_FREE(env->allocator, soap_action);
        soap_action = NULL;
    }
    return status;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_process_http_put_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    const int content_length,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_builder_t *soap_builder = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axis2_bool_t is_soap11 = AXIS2_FALSE;
    axiom_xml_reader_t *xml_reader = NULL;
    axutil_string_t *char_set_str = NULL;

    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_callback_info_t *callback_ctx;
    axutil_hash_t *headers = NULL;
    axis2_engine_t *engine = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axutil_hash_t *binary_data_map = NULL;
    axis2_char_t *soap_body_str = NULL;
    axutil_stream_t *stream = NULL;
    axis2_bool_t do_rest = AXIS2_FALSE;
    axis2_bool_t run_as_get = AXIS2_FALSE;
    axutil_property_t *http_error_property = NULL;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_stream, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_stream, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FAILURE);

    conf_ctx = axis2_msg_ctx_get_conf_ctx(msg_ctx, env);

    callback_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t));
    /* Note: the memory created above is freed in xml reader free function
       as this is passed on to the reader */
    callback_ctx->in_stream = in_stream;
    callback_ctx->env = env;
    callback_ctx->content_length = content_length;
    callback_ctx->unread_len = content_length;
    callback_ctx->chunked_stream = NULL;


    headers = axis2_msg_ctx_get_transport_headers(msg_ctx, env);
    if (headers)
    {
        axis2_http_header_t *encoding_header = NULL;
        encoding_header = (axis2_http_header_t *) axutil_hash_get(
            headers,
            AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
            AXIS2_HASH_KEY_STRING);

        if (encoding_header)
        {
            axis2_char_t *encoding_value = NULL;
            encoding_value = axis2_http_header_get_value(encoding_header, env);
            if (encoding_value && 
                0 == axutil_strcasecmp(encoding_value,
                                       AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                callback_ctx->chunked_stream =
                    axutil_http_chunked_stream_create(env, in_stream);
                if (!callback_ctx->chunked_stream)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Error occured in"
                                    " creating in chunked stream.");
                    return AXIS2_FAILURE;
                }
                AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "HTTP"
                                " stream chunked");
            }
        }
    }
    else
    {
        /* check content encoding from msg ctx property */
        axis2_char_t *value = axis2_msg_ctx_get_transfer_encoding(msg_ctx, env);

        if (value &&
            axutil_strstr(value, AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
        {
            /* this is an UGLY hack to get some of the transports working 
               e.g. PHP transport where it strips the chunking info in case of chunking 
               and also gives out a content lenght of 0.
               We need to fix the transport design to fix sutuations like this.
             */
            callback_ctx->content_length = AXIS2_CHUNKED_CONTENT_LENGTH;
            callback_ctx->unread_len = callback_ctx->content_length;
        }
    }

    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED))
    {
        /* get mime boundry */
        axis2_char_t *mime_boundary =
            axis2_http_transport_utils_get_value_from_content_type(
                env,
                content_type,
                AXIS2_HTTP_HEADER_CONTENT_TYPE_MIME_BOUNDARY);

        if (mime_boundary)
        {
            axiom_mime_parser_t *mime_parser = NULL;
            int soap_body_len = 0;
            axutil_param_t *buffer_size_param = NULL;
            axutil_param_t *max_buffers_param = NULL;
            axutil_param_t *attachment_dir_param = NULL;
            axutil_param_t *callback_name_param = NULL;
            axis2_char_t *value_size = NULL;
            axis2_char_t *value_num = NULL;
            axis2_char_t *value_dir = NULL;
            axis2_char_t *value_callback = NULL;
            int size = 0;
            int num = 0;

            mime_parser = axiom_mime_parser_create(env);

            buffer_size_param = 
                axis2_msg_ctx_get_parameter (msg_ctx,
                                             env,
                                             AXIS2_MTOM_BUFFER_SIZE);
            if (buffer_size_param)
            {
                value_size =
                    (axis2_char_t *) axutil_param_get_value (buffer_size_param, env);
                if(value_size)
                {
                    size = atoi(value_size);
                    axiom_mime_parser_set_buffer_size(mime_parser, env, size);
                }
            }

            max_buffers_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                               env,
                                                               AXIS2_MTOM_MAX_BUFFERS);
            if (max_buffers_param)
            {
                value_num =
                    (axis2_char_t *) axutil_param_get_value (max_buffers_param, env);
                if(value_num)
                {
                    num = atoi(value_num);
                    axiom_mime_parser_set_max_buffers(mime_parser, env, num);
                }
            }

            callback_name_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                                   env,
                                                                   AXIS2_MTOM_CACHING_CALLBACK);
            if(callback_name_param)
            {
                value_callback =
                    (axis2_char_t *) axutil_param_get_value (callback_name_param, env);
                if(value_callback)
                {
                    axiom_mime_parser_set_caching_callback_name(mime_parser, env, value_callback);
                }
            }

   
            attachment_dir_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                                   env,
                                                                   AXIS2_ATTACHMENT_DIR);

            if(attachment_dir_param)
            {
                value_dir =
                    (axis2_char_t *) axutil_param_get_value (attachment_dir_param, env);
                if(value_dir)
                {
                    axiom_mime_parser_set_attachment_dir(mime_parser, env, value_dir);
                }
            }


            if (mime_parser)
            {
                /*binary_data_map = 
                    axiom_mime_parser_parse(mime_parser, env,
                                            axis2_http_transport_utils_on_data_request,
                                            (void *) callback_ctx,
                                            mime_boundary);*/
                if (!binary_data_map)
                {
                    return AXIS2_FAILURE;
                }
                soap_body_len =
                    axiom_mime_parser_get_soap_body_len(mime_parser, env);
                soap_body_str =
                    axiom_mime_parser_get_soap_body_str(mime_parser, env);
            }

            stream = axutil_stream_create_basic(env);
            if (stream)
            {
                axutil_stream_write(stream, env, soap_body_str, soap_body_len);
                callback_ctx->in_stream = stream;
                callback_ctx->chunked_stream = NULL;
                callback_ctx->content_length = soap_body_len;
                callback_ctx->unread_len = soap_body_len;
            }
            axiom_mime_parser_free(mime_parser, env);
            mime_parser = NULL;
        }
        AXIS2_FREE(env->allocator, mime_boundary);
    }

    axis2_msg_ctx_set_to(msg_ctx, env, axis2_endpoint_ref_create(env,
                                                                 request_uri));

    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);

    char_set_str =
        axis2_http_transport_utils_get_charset_enc(env, content_type);
    xml_reader =
        axiom_xml_reader_create_for_io(env,
                                       axis2_http_transport_utils_on_data_request,
                                       NULL, (void *) callback_ctx,
                                       axutil_string_get_buffer(char_set_str,
                                                                env));

    if (!xml_reader)
    {
        return AXIS2_FAILURE;
    }

    axis2_msg_ctx_set_charset_encoding(msg_ctx, env, char_set_str);

    om_builder = axiom_stax_builder_create(env, xml_reader);
    if (!om_builder)
    {
        axiom_xml_reader_free(xml_reader, env);
        xml_reader = NULL;
        return AXIS2_FAILURE;
    }

    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
    {
        is_soap11 = AXIS2_FALSE;
        soap_builder = axiom_soap_builder_create(env, om_builder,
                                                 AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
        if (!soap_builder)
        {
            /* We should not be freeing om_builder here as it is done by
               axiom_soap_builder_create in case of error - Samisa */
            om_builder = NULL;
            xml_reader = NULL;
            axis2_msg_ctx_set_is_soap_11(msg_ctx, env, is_soap11);
            return AXIS2_FAILURE;
        }

        soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);
        if (!soap_envelope)
        {
            axiom_stax_builder_free(om_builder, env);
            om_builder = NULL;
            xml_reader = NULL;
            axiom_soap_builder_free(soap_builder, env);
            soap_builder = NULL;
            axis2_msg_ctx_set_is_soap_11(msg_ctx, env, is_soap11);
            return AXIS2_FAILURE;
        }
    }
    else if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML))
    {
        do_rest = AXIS2_TRUE;
        if (soap_action_header)
        {
            return AXIS2_FAILURE;
        }
        else
        {
            /* REST support */
            do_rest = AXIS2_TRUE;
        }
    }
    else if (strstr
             (content_type, AXIS2_HTTP_HEADER_ACCEPT_X_WWW_FORM_URLENCODED))
    {
        /* REST support */
        do_rest = AXIS2_TRUE;
        run_as_get = AXIS2_TRUE;
    }
    else
    {
        http_error_property = axutil_property_create(env);
        axutil_property_set_value(http_error_property, env,
                                  AXIS2_HTTP_UNSUPPORTED_MEDIA_TYPE);
        axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_HTTP_TRANSPORT_ERROR,
                                   http_error_property);
    }

    if (do_rest)
    {
        /* REST support */
        axutil_param_t *rest_param =
            axis2_msg_ctx_get_parameter(msg_ctx, env, AXIS2_ENABLE_REST);
        if (rest_param &&
            0 == axutil_strcmp(AXIS2_VALUE_TRUE,
                               axutil_param_get_value(rest_param, env)))
        {
            axiom_soap_body_t *def_body = NULL;
            axiom_document_t *om_doc = NULL;
            axiom_node_t *root_node = NULL;
            if (!run_as_get)
            {
                soap_envelope = axiom_soap_envelope_create_default_soap_envelope
                    (env, AXIOM_SOAP11);
                def_body = axiom_soap_envelope_get_body(soap_envelope, env);
                om_doc = axiom_stax_builder_get_document(om_builder, env);
                root_node = axiom_document_build_all(om_doc, env);
                axiom_soap_body_add_child(def_body, env, root_node);
            }
            axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_TRUE);
            axis2_msg_ctx_set_rest_http_method(msg_ctx, env, AXIS2_HTTP_PUT);
			axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);
        }
        else
        {
            return AXIS2_FAILURE;
        }
        if (AXIS2_SUCCESS != axis2_http_transport_utils_dispatch_and_verify(env,
                                                                            msg_ctx))
        {
            return AXIS2_FAILURE;
        }
    }

    if (run_as_get)
    {
        axis2_char_t *buffer = NULL;
        axis2_char_t *new_url = NULL;
        buffer = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * (content_length + 1));
        if (!buffer)
        {
            return AXIS2_FAILURE;
        }
        axis2_http_transport_utils_on_data_request(buffer, content_length, (void *) callback_ctx);
        
        new_url = AXIS2_MALLOC(env->allocator, sizeof(axis2_char_t) * 
            ((int)(strlen(request_uri) + strlen(buffer)) + 2));
        if (!new_url)
        {
            return AXIS2_FAILURE;
        }
        sprintf(new_url, "%s?%s", request_uri, buffer);
        AXIS2_FREE(env->allocator, buffer);

        soap_envelope =
            axis2_http_transport_utils_handle_media_type_url_encoded(env, msg_ctx,
            axis2_http_transport_utils_get_request_params(env, new_url), AXIS2_HTTP_POST);
    }

    if (binary_data_map)
    {
        axiom_soap_builder_set_mime_body_parts(soap_builder, env,
                                               binary_data_map);
    }

    axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);

    engine = axis2_engine_create(env, conf_ctx);

    if (!soap_envelope)
        return AXIS2_FAILURE;

    soap_body = axiom_soap_envelope_get_body(soap_envelope, env);

    if (!soap_body)
        return AXIS2_FAILURE;

    if (AXIS2_TRUE == axiom_soap_body_has_fault(soap_body, env))
    {
        status = axis2_engine_receive_fault(engine, env, msg_ctx);
    }
    else
    {
        status = axis2_engine_receive(engine, env, msg_ctx);
    }
    if (!axis2_msg_ctx_get_soap_envelope(msg_ctx, env) &&
        AXIS2_FALSE == is_soap11)
    {
        axiom_soap_envelope_t *def_envelope =
            axiom_soap_envelope_create_default_soap_envelope(env,
                                                             AXIOM_SOAP12);
        axis2_msg_ctx_set_soap_envelope(msg_ctx, env, def_envelope);
    }

    if (engine)
    {
        axis2_engine_free(engine, env);
    }

    if (soap_body_str)
    {
        AXIS2_FREE(env->allocator, soap_body_str);
    }

    if (stream)
    {
        axutil_stream_free(stream, env);
    }

    if (char_set_str)
    {
        axutil_string_free(char_set_str, env);
    }
    if (!soap_builder && om_builder)
    {
        axiom_stax_builder_free_self(om_builder, env);
        om_builder = NULL;
    }
    return status;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_head_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri,
    axis2_conf_ctx_t * conf_ctx,
    axutil_hash_t * request_params)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_engine_t *engine = NULL;
    axis2_bool_t do_rest = AXIS2_TRUE;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, in_stream, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, out_stream, AXIS2_FALSE);

    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FALSE);

    axis2_msg_ctx_set_to(msg_ctx, env, axis2_endpoint_ref_create(env,
                                                                 request_uri));
    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);
    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML))
    {
        if (soap_action_header)
        {
            do_rest = AXIS2_FALSE;
        }
    }
    else if (strstr
        (content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
    {
        do_rest = AXIS2_FALSE;
    }

    if (do_rest)
    {
        axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_TRUE);
        axis2_msg_ctx_set_rest_http_method(msg_ctx, env, AXIS2_HTTP_HEAD);
    }
    else
    {
        axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_FALSE);
    }
    if (AXIS2_SUCCESS != axis2_http_transport_utils_dispatch_and_verify(env, msg_ctx))
    {
		return AXIS2_FALSE;
    }

    soap_envelope = axis2_http_transport_utils_handle_media_type_url_encoded(env, msg_ctx, 
   		 request_params, AXIS2_HTTP_HEAD);
    if (!soap_envelope)
    {
        return AXIS2_FALSE;
    }
    axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);
    engine = axis2_engine_create(env, conf_ctx);
    axis2_engine_receive(engine, env, msg_ctx);
    return AXIS2_TRUE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_get_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri,
    axis2_conf_ctx_t * conf_ctx,
    axutil_hash_t * request_params)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_engine_t *engine = NULL;
    axis2_bool_t do_rest = AXIS2_TRUE;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, in_stream, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, out_stream, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FALSE);

    axis2_msg_ctx_set_to(msg_ctx, env, axis2_endpoint_ref_create(env,
                                                                 request_uri));
    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);
    if (content_type && strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML))
    {
        if (soap_action_header)
        {
            do_rest = AXIS2_FALSE;
        }
    }
    else if (content_type && strstr
        (content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
    {
        do_rest = AXIS2_FALSE;
    }

    if (do_rest)
    {
        axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_TRUE);
        axis2_msg_ctx_set_rest_http_method(msg_ctx, env, AXIS2_HTTP_GET);
    }
    else
    {
        axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_FALSE);
    }

	if (AXIS2_SUCCESS != axis2_http_transport_utils_dispatch_and_verify(env, msg_ctx))
	{
		return AXIS2_FALSE;
	}
    soap_envelope = axis2_http_transport_utils_handle_media_type_url_encoded(env, msg_ctx,
                                                                 request_params,
                                                                 AXIS2_HTTP_GET);
    if (!soap_envelope)
    {
        return AXIS2_FALSE;
    }
    axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);
	
    engine = axis2_engine_create(env, conf_ctx);
    axis2_engine_receive(engine, env, msg_ctx);
    return AXIS2_TRUE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_delete_request(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_stream_t * in_stream,
    axutil_stream_t * out_stream,
    const axis2_char_t * content_type,
    axutil_string_t * soap_action_header,
    const axis2_char_t * request_uri,
    axis2_conf_ctx_t * conf_ctx,
    axutil_hash_t * request_params)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_engine_t *engine = NULL;
    axis2_bool_t do_rest = AXIS2_TRUE;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, in_stream, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, out_stream, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FALSE);

    axis2_msg_ctx_set_to(msg_ctx, env, axis2_endpoint_ref_create(env,
                                                                 request_uri));
    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);
    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML))
    {
        if (soap_action_header)
        {
            do_rest = AXIS2_FALSE;
        }
    }
    else if (strstr
        (content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
    {
        do_rest = AXIS2_FALSE;
    }

    if (do_rest)
    {
        axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_TRUE);
        axis2_msg_ctx_set_rest_http_method(msg_ctx, env, AXIS2_HTTP_DELETE);
    }
    else
    {
        axis2_msg_ctx_set_doing_rest(msg_ctx, env, AXIS2_FALSE);
    }


    soap_envelope =
        axis2_http_transport_utils_handle_media_type_url_encoded(env, msg_ctx,
                                                                 request_params,
                                                                 AXIS2_HTTP_DELETE);
    if (!soap_envelope)
    {
        return AXIS2_FALSE;
    }
    
	axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);


	if (AXIS2_SUCCESS != axis2_http_transport_utils_dispatch_and_verify(env, msg_ctx))
	{
		return AXIS2_FALSE;
	}
    engine = axis2_engine_create(env, conf_ctx);
    axis2_engine_receive(engine, env, msg_ctx);
    return AXIS2_TRUE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_do_write_mtom(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx)
{
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    return (axis2_msg_ctx_get_doing_mtom(msg_ctx, env));
}

AXIS2_EXTERN axutil_hash_t *AXIS2_CALL
axis2_http_transport_utils_get_request_params(
    const axutil_env_t * env,
    axis2_char_t * request_uri)
{

    axis2_char_t *query_str = NULL;
    axis2_char_t *tmp = strchr(request_uri, AXIS2_Q_MARK);
    axis2_char_t *tmp2 = NULL;
    axis2_char_t *tmp_name = NULL;
    axis2_char_t *tmp_value = NULL;
    axutil_hash_t *ret = NULL;

    AXIS2_PARAM_CHECK(env->error, request_uri, NULL);

    if (!tmp || AXIS2_ESC_NULL == *(tmp + 1))
    {
        return NULL;
    }
    query_str = axutil_strdup(env, tmp + 1);

    for (tmp2 = tmp = query_str; *tmp != AXIS2_ESC_NULL; ++tmp)
    {
        if (AXIS2_EQ == *tmp)
        {
            *tmp = AXIS2_ESC_NULL;
            tmp_name = axutil_strdup(env, tmp2);
            axis2_http_transport_utils_strdecode(env, tmp_name, tmp_name);
            tmp2 = tmp + 1;
        }
        if (AXIS2_AND == *tmp)
        {
            *tmp = AXIS2_ESC_NULL;
            tmp_value = axutil_strdup(env, tmp2);
            axis2_http_transport_utils_strdecode(env, tmp_value, tmp_value);
            tmp2 = tmp + 1;
        }
        if (tmp_name && NULL != tmp_value)
        {
            if (!ret)
            {
                ret = axutil_hash_make(env);
            }
            axutil_hash_set(ret, tmp_name, AXIS2_HASH_KEY_STRING, tmp_value);
            tmp_name = NULL;
            tmp_value = NULL;
        }
    }
    if (tmp_name && AXIS2_ESC_NULL != *tmp2)
    {
        if (!ret)
        {
            ret = axutil_hash_make(env);
        }
        tmp_value = axutil_strdup(env, tmp2);
        axis2_http_transport_utils_strdecode(env, tmp_value, tmp_value);
        axutil_hash_set(ret, tmp_name, AXIS2_HASH_KEY_STRING, tmp_value);
    }

    return ret;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_strdecode(
    const axutil_env_t * env,
    axis2_char_t * dest,
    axis2_char_t * src)
{
    AXIS2_PARAM_CHECK(env->error, dest, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, src, AXIS2_FAILURE);

    for (; *src != AXIS2_ESC_NULL; ++dest, ++src)
    {
        if (src[0] == AXIS2_PERCENT && isxdigit((int)src[1]) && isxdigit((int)src[2]))
        {
            *dest = (axis2_char_t)(axis2_http_transport_utils_hexit(src[1]) * 16 +
                axis2_http_transport_utils_hexit(src[2]));
            /* We are sure that the conversion is valid */
            src += 2;
        }
        else
        {
            *dest = *src;
        }
    }
    *dest = AXIS2_ESC_NULL;

    return AXIS2_SUCCESS;
}

AXIS2_EXTERN int AXIS2_CALL
axis2_http_transport_utils_hexit(
    axis2_char_t c)
{
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    if (c >= 'A' && c <= 'F')
    {
        return c - 'A' + 10;
    }
    return 0;                   /* shouldn't happen, we're guarded by isxdigit() */
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_not_found(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_NOT_FOUND;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_not_implemented(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_NOT_IMPLEMENTED;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_internal_server_error(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_INTERNAL_SERVER_ERROR;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_method_not_allowed(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_METHOD_NOT_ALLOWED;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_not_acceptable(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_NOT_ACCEPTABLE;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_bad_request(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_BAD_REQUEST;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_request_timeout(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_REQUEST_TIMEOUT;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_conflict(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_CONFLICT;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_gone(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_GONE;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_precondition_failed(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_PRECONDITION_FAILED;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_request_entity_too_large(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_TOO_LARGE;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_service_unavailable(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    return AXIS2_HTTP_SERVICE_UNAVILABLE;
}


AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_services_html(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    axutil_hash_t *services_map = NULL;
    axutil_hash_t *errorneous_svc_map = NULL;
    axis2_char_t *ret = NULL;
    axis2_char_t *tmp2 = (axis2_char_t *) "<h2>Deployed Services</h2>";
    axutil_hash_index_t *hi = NULL;
    axis2_bool_t svcs_exists = AXIS2_FALSE;
    axis2_conf_t *conf = NULL;
    AXIS2_PARAM_CHECK(env->error, conf_ctx, NULL);

    conf = axis2_conf_ctx_get_conf(conf_ctx, env);
    services_map = axis2_conf_get_all_svcs(conf, env);
    errorneous_svc_map = axis2_conf_get_all_faulty_svcs(conf, env);
    if (services_map && 0 != axutil_hash_count(services_map))
    {
        void *service = NULL;
        axis2_char_t *sname = NULL;
        axutil_hash_t *ops = NULL;
        svcs_exists = AXIS2_TRUE;

        for (hi = axutil_hash_first(services_map, env);
             hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &service);
            sname = axutil_qname_get_localpart(axis2_svc_get_qname(((axis2_svc_t
                                                                     *)
                                                                    service),
                                                                   env), env);
            ret = axutil_stracat(env, tmp2, "<h3><u>");
            tmp2 = ret;
            ret = axutil_stracat(env, tmp2, sname);
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
            ret = axutil_stracat(env, tmp2, "</u></h3>");
            tmp2 = ret;
            ret = axutil_stracat(env, tmp2, "<p>");
            tmp2 = ret;

                             /**
							  *setting services description */
            ret = axutil_stracat(env, tmp2, axis2_svc_get_svc_desc((axis2_svc_t
                                                                    *) service,
                                                                   env));
            tmp2 = ret;
            ret = axutil_stracat(env, tmp2, "</p>");
            tmp2 = ret;
            ops = axis2_svc_get_all_ops(((axis2_svc_t *) service), env);
            if (ops && 0 != axutil_hash_count(ops))
            {
                axutil_hash_index_t *hi2 = NULL;
                void *op = NULL;
                axis2_char_t *oname = NULL;

                ret =
                    axutil_stracat(env, tmp2,
                                   "<i>Available Operations</i> <ul>");
                AXIS2_FREE(env->allocator, tmp2);
                tmp2 = ret;
                for (hi2 = axutil_hash_first(ops, env); hi2;
                     hi2 = axutil_hash_next(env, hi2))
                {
                    axutil_hash_this(hi2, NULL, NULL, &op);
                    oname = axutil_qname_get_localpart(
                        axis2_op_get_qname(((axis2_op_t *) op), env), 
                        env);
                    ret = axutil_stracat(env, tmp2, "<li>");
                    AXIS2_FREE(env->allocator, tmp2);
                    tmp2 = ret;

                    ret = axutil_stracat(env, tmp2, oname);
                    AXIS2_FREE(env->allocator, tmp2);
                    tmp2 = ret;
                    ret = axutil_stracat(env, tmp2, "</li>");
                    AXIS2_FREE(env->allocator, tmp2);
                    tmp2 = ret;
                }
                ret = axutil_stracat(env, tmp2, "</ul>");
                AXIS2_FREE(env->allocator, tmp2);
                tmp2 = ret;
            }
            else
            {
                ret = axutil_stracat(env, tmp2, "No operations Available");
                tmp2 = ret;
            }
        }
    }

    if (errorneous_svc_map && 0 != axutil_hash_count(errorneous_svc_map))
    {
        void *fsname = NULL;
        svcs_exists = AXIS2_TRUE;
        ret = axutil_stracat(env, tmp2, "<hr><h2><font color=\"red\">Faulty \
                Services</font></h2>");
        AXIS2_FREE(env->allocator, tmp2);
        tmp2 = ret;

        for (hi = axutil_hash_first(errorneous_svc_map, env); hi;
             axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, (const void **) &fsname, NULL, NULL);
            ret = axutil_stracat(env, tmp2, "<h3><font color=\"red\">");
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
            ret = axutil_stracat(env, tmp2, (axis2_char_t *) fsname);
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
            ret = axutil_stracat(env, tmp2, "</font></h3>");
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
        }
    }
    if (AXIS2_FALSE == svcs_exists)
    {
        ret = axutil_strdup(env, "<h2>There are no services deployed</h2>");
    }
    ret =
        axutil_stracat(env,
                       "<html><head><title>Axis2C :: Services</title></head>"
                       "<body><font face=\"courier\">", tmp2);
    tmp2 = ret;
    ret = axutil_stracat(env, tmp2, "</font></body></html>\r\n");

    return ret;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_services_static_wsdl(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_char_t * request_url)
{
    axis2_char_t *wsdl_string = NULL;
    axis2_char_t *wsdl_path = NULL;
    axis2_char_t **url_tok = NULL;
    unsigned int len = 0;
    axis2_char_t *svc_name = NULL;
    axis2_conf_t *conf = NULL;
    axutil_hash_t *services_map = NULL;
    axutil_hash_index_t *hi = NULL;

    AXIS2_PARAM_CHECK(env->error, conf_ctx, NULL);
    AXIS2_PARAM_CHECK(env->error, request_url, NULL);

    url_tok = axutil_parse_request_url_for_svc_and_op(env, request_url);
    if (url_tok[0])
    {
        len = (int)strlen(url_tok[0]);
        /* We are sure that the difference lies within the int range */
        url_tok[0][len - 5] = 0;
        svc_name = url_tok[0];
    }

    conf = axis2_conf_ctx_get_conf(conf_ctx, env);
    services_map = axis2_conf_get_all_svcs(conf, env);

    if (services_map && 0 != axutil_hash_count(services_map))
    {
        void *service = NULL;
        axis2_char_t *sname = NULL;

        for (hi = axutil_hash_first(services_map, env);
             hi; hi = axutil_hash_next(env, hi))
        {
            axutil_hash_this(hi, NULL, NULL, &service);
            sname = axutil_qname_get_localpart(axis2_svc_get_qname(((axis2_svc_t
                                                                     *)
                                                                    service),
                                                                   env), env);
            if (!axutil_strcmp(svc_name, sname))
            {
                wsdl_path = (axis2_char_t *) axutil_strdup(env, 
                                axis2_svc_get_svc_wsdl_path((axis2_svc_t *)
                                                            service, env));
                if (!wsdl_path)
                {
                    wsdl_path = axutil_strcat(env, 
                                    axis2_svc_get_svc_folder_path((axis2_svc_t *)
                                     service, env), AXIS2_PATH_SEP_STR,
                                    svc_name, ".wsdl", NULL);
                }
                break;
            }

        }
    }

    if (wsdl_path)
    {
        FILE *wsdl_file;
        axis2_char_t *content = NULL;
        int c;
        int size = AXIS2_FILE_READ_SIZE;
        axis2_char_t *tmp;
        int i = 0;

        content = (axis2_char_t *) AXIS2_MALLOC(env->allocator, size);
        wsdl_file = fopen(wsdl_path, "r");
        if (wsdl_file)
        {
            c = fgetc(wsdl_file);
            while (c != EOF)
            {
                if (i >= size)
                {
                    size = size * 3;
                    tmp = (axis2_char_t *) AXIS2_MALLOC(env->allocator, size);
                    memcpy(tmp, content, i);
                    AXIS2_FREE(env->allocator, content);
                    content = tmp;
                }
                content[i++] = (axis2_char_t)c;
                c = fgetc(wsdl_file);
            }
            content[i] = AXIS2_ESC_NULL;
            wsdl_string = (axis2_char_t *)content;
        }
        AXIS2_FREE(env->allocator, wsdl_path);
    }
    else
    {
        wsdl_string = axutil_strdup(env, "Unable to retrieve wsdl for this service");
    }

    return wsdl_string;
}

AXIS2_EXTERN axutil_string_t *AXIS2_CALL
axis2_http_transport_utils_get_charset_enc(
    const axutil_env_t * env,
    const axis2_char_t * content_type)
{
    axis2_char_t *tmp = NULL;
    axis2_char_t *tmp_content_type = NULL;
    axis2_char_t *tmp2 = NULL;
    axutil_string_t *str = NULL;

    AXIS2_PARAM_CHECK(env->error, content_type, NULL);

    tmp_content_type = (axis2_char_t *) content_type;
    if (!tmp_content_type)
    {
        return axutil_string_create_const(env,
                                          (axis2_char_t **) &
                                          AXIS2_TRANS_UTIL_DEFAULT_CHAR_ENCODING);
    }

    tmp = strstr(tmp_content_type, AXIS2_HTTP_CHAR_SET_ENCODING);

    if (tmp)
    {
        tmp = strchr(tmp, AXIS2_EQ);
        if (tmp)
        {
            tmp2 = strchr(tmp, AXIS2_SEMI_COLON);
            if (!tmp2)
            {
               tmp2 = tmp + strlen(tmp); 
            }
        }

        if (tmp2)
        {
            if ('\'' == *(tmp2 - sizeof(axis2_char_t)) ||
                '\"' == *(tmp2 - sizeof(axis2_char_t)))
            {
                tmp2 -= sizeof(axis2_char_t);
            }
            *tmp2 = AXIS2_ESC_NULL;
        }
    }

    if (tmp)
    {
        /* Following formats are acceptable
         * charset="UTF-8"
         * charser='UTF-8'
         * charset=UTF-8
         * But for our requirements charset we get should be UTF-8
         */
        if (AXIS2_ESC_SINGLE_QUOTE == *(tmp + sizeof(axis2_char_t)) || 
            AXIS2_ESC_DOUBLE_QUOTE == *(tmp + sizeof(axis2_char_t)))
        {
            tmp += 2 * sizeof(axis2_char_t);
        }
        else
        {
            tmp += sizeof(axis2_char_t);
        }
    }

    if (tmp)
    {
        str = axutil_string_create(env, tmp);
    }
    else
    {
        str =
            axutil_string_create_const(env,
                                       (axis2_char_t **) &
                                       AXIS2_TRANS_UTIL_DEFAULT_CHAR_ENCODING);
    }
    return str;
}

int AXIS2_CALL
axis2_http_transport_utils_on_data_request(
    char *buffer,
    int size,
    void *ctx)
{
    const axutil_env_t *env = NULL;
    int len = -1;
    axis2_callback_info_t *cb_ctx = (axis2_callback_info_t *) ctx;

    if (!buffer || !ctx)
    {
        return 0;
    }
    env = ((axis2_callback_info_t *) ctx)->env;
    if (cb_ctx->unread_len <= 0 && -1 != cb_ctx->content_length)
    {
        return 0;
    }
    if (cb_ctx->chunked_stream)
    {
        --size;                         
        /* reserve space to insert trailing null */
        len = axutil_http_chunked_stream_read(cb_ctx->chunked_stream, env,
                                             buffer, size);
        if (len >= 0)
        {
            buffer[len] = AXIS2_ESC_NULL;
        }
    }
    else
    {
        axutil_stream_t *in_stream = NULL;
        in_stream =
            (axutil_stream_t *) ((axis2_callback_info_t *) ctx)->in_stream;
        --size;                         /* reserve space to insert trailing null */
        len = axutil_stream_read(in_stream, env, buffer, size);
        if (len > 0)
        {
            buffer[len] = AXIS2_ESC_NULL;
            ((axis2_callback_info_t *) ctx)->unread_len -= len;
        }
        else if(len == 0)
        {
            ((axis2_callback_info_t *) ctx)->unread_len = 0;
        }
    }
    return len;
}

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_create_soap_msg(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    const axis2_char_t * soap_ns_uri)
{
    axis2_op_ctx_t *op_ctx = NULL;
    const axis2_char_t *char_set_enc = NULL;
    axis2_char_t *content_type = NULL;
    axutil_stream_t *in_stream = NULL;
    axis2_callback_info_t *callback_ctx = NULL;
    axis2_char_t *trans_enc = NULL;
    int *content_length = NULL;
    axutil_property_t *property = NULL;
    axutil_hash_t *binary_data_map = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
	axutil_stream_t *stream = NULL;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, NULL);
    AXIS2_PARAM_CHECK(env->error, soap_ns_uri, NULL);

    property = axis2_msg_ctx_get_property(msg_ctx, env, AXIS2_TRANSPORT_IN);
    if (property)
    {
        in_stream = axutil_property_get_value(property, env);
        property = NULL;
    }
    callback_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t));
    /* Note: the memory created above is freed in xml reader free function
       as this is passed on to the reader */
    if (!callback_ctx)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    callback_ctx->in_stream = in_stream;
    callback_ctx->env = env;
    callback_ctx->content_length = -1;
    callback_ctx->unread_len = -1;
    callback_ctx->chunked_stream = NULL;

    property = axis2_msg_ctx_get_property(msg_ctx, env,
                                          AXIS2_HTTP_HEADER_CONTENT_LENGTH);
    if (property)
    {
        content_length = axutil_property_get_value(property, env);
        property = NULL;
    }

    if (content_length)
    {
        callback_ctx->content_length = *content_length;
        callback_ctx->unread_len = *content_length;
    }

    if (!in_stream)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_NULL_IN_STREAM_IN_MSG_CTX,
                           AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, callback_ctx);
        return NULL;
    }

    trans_enc = axis2_msg_ctx_get_transfer_encoding(msg_ctx, env);

    if (trans_enc && 0 == axutil_strcmp(trans_enc,
                                        AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
    {
        callback_ctx->chunked_stream = axutil_http_chunked_stream_create(env,
                                                                        in_stream);
        if (!callback_ctx->chunked_stream)
        {
            return NULL;
        }
    }

    op_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, env);
    if (op_ctx)
    {
        axis2_ctx_t *ctx = axis2_op_ctx_get_base(op_ctx, env);
        if (ctx)
        {
            property = axis2_ctx_get_property(ctx, env,
                                              AXIS2_CHARACTER_SET_ENCODING);
            if (property)
            {
                char_set_enc = axutil_property_get_value(property, env);
                property = NULL;
            }
            property = axis2_ctx_get_property(ctx, env,
                                              MTOM_RECIVED_CONTENT_TYPE);
            if (property)
            {
                content_type = axutil_property_get_value(property, env);
                property = NULL;
            }

        }
    }

    if (!char_set_enc)
    {
        char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
    }
    if (content_type)
    {
        axis2_char_t *mime_boundary = NULL;
        axis2_msg_ctx_set_doing_mtom(msg_ctx, env, AXIS2_TRUE);
        /* get mime boundry */
        mime_boundary =
            axis2_http_transport_utils_get_value_from_content_type(
                env,
                content_type,
                AXIS2_HTTP_HEADER_CONTENT_TYPE_MIME_BOUNDARY);

        if (mime_boundary)
        {
            axiom_mime_parser_t *mime_parser = NULL;
            int soap_body_len = 0;
            axis2_char_t *soap_body_str = NULL;
            axutil_param_t *buffer_size_param = NULL;
            axutil_param_t *max_buffers_param = NULL;
            axutil_param_t *attachment_dir_param = NULL;
            axutil_param_t *callback_name_param = NULL;
            axis2_char_t *value_size = NULL;
            axis2_char_t *value_num = NULL;
            axis2_char_t *value_dir = NULL;
            axis2_char_t *value_callback = NULL;
            int size = 0;
            int num = 0;
 
            mime_parser = axiom_mime_parser_create(env);

            /* Following are the parameters in the axis2.xml regarding MTOM */


            buffer_size_param = 
                axis2_msg_ctx_get_parameter (msg_ctx,
                                             env,
                                             AXIS2_MTOM_BUFFER_SIZE);
            if (buffer_size_param)
            {
                value_size =
                    (axis2_char_t *) axutil_param_get_value (buffer_size_param, env);

                if(value_size)
                {
                    size = atoi(value_size);
                    axiom_mime_parser_set_buffer_size(mime_parser, env, size);
                }
            }

            max_buffers_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                                   env,
                                                                   AXIS2_MTOM_MAX_BUFFERS);
            if(max_buffers_param)
            {
                value_num =
                    (axis2_char_t *) axutil_param_get_value (max_buffers_param, env);
                if(value_num)
                {
                    num = atoi(value_num);
                    axiom_mime_parser_set_max_buffers(mime_parser, env, num);
                }
            }

            callback_name_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                                   env,
                                                                   AXIS2_MTOM_CACHING_CALLBACK);
            if(callback_name_param)
            {
                value_callback =
                    (axis2_char_t *) axutil_param_get_value (callback_name_param, env);
                if(value_callback)
                {
                    axiom_mime_parser_set_caching_callback_name(mime_parser, env, value_callback);
                }
            }
           
            attachment_dir_param = axis2_msg_ctx_get_parameter (msg_ctx,
                                                                   env,
                                                                   AXIS2_ATTACHMENT_DIR);
            if(attachment_dir_param)
            {
                value_dir =
                    (axis2_char_t *) axutil_param_get_value (attachment_dir_param, env);
                if(value_dir)
                {
                    axiom_mime_parser_set_attachment_dir(mime_parser, env, value_dir);
                }
            }
   
 
            if (mime_parser)
            {
                if(axiom_mime_parser_parse_for_soap(mime_parser, env,
                        axis2_http_transport_utils_on_data_request,
                        (void *) callback_ctx,
                        mime_boundary) == AXIS2_FAILURE)
                {
                    return NULL;
                }

                binary_data_map =
                    axiom_mime_parser_parse_for_attachments(mime_parser, env,
                                            axis2_http_transport_utils_on_data_request,
                                            (void *) callback_ctx,
                                            mime_boundary,
                                            NULL);
                if(!binary_data_map)
                {
                    return NULL;
                }

                if(!binary_data_map)
                {
                    return NULL;
                }
    
                soap_body_len =
                    axiom_mime_parser_get_soap_body_len(mime_parser, env);
                soap_body_str =
                    axiom_mime_parser_get_soap_body_str(mime_parser, env);
            }

            if(callback_ctx->chunked_stream)
            {
                axutil_http_chunked_stream_free(callback_ctx->chunked_stream, env);
                callback_ctx->chunked_stream = NULL;
            }

            stream = axutil_stream_create_basic(env);
            if (stream)
            {
                axutil_stream_write(stream, env, soap_body_str, soap_body_len);
                callback_ctx->in_stream = stream;
                callback_ctx->chunked_stream = NULL;
                callback_ctx->content_length = soap_body_len;
                callback_ctx->unread_len = soap_body_len;
            }

            axiom_mime_parser_free(mime_parser, env);
            mime_parser = NULL;
			if(mime_boundary)
			{
	    		AXIS2_FREE(env->allocator, mime_boundary);
			}

			if(soap_body_str)
			{
			    AXIS2_FREE(env->allocator, soap_body_str); 
			}
        }
    }

    if (AXIS2_TRUE != axis2_msg_ctx_get_doing_rest(msg_ctx, env))
    {
        axiom_xml_reader_t *xml_reader = NULL;
        axiom_stax_builder_t *om_builder = NULL;
        axiom_soap_builder_t *soap_builder = NULL;
        
        xml_reader = 
            axiom_xml_reader_create_for_io(env,
                                           axis2_http_transport_utils_on_data_request,
                                           NULL, (void *) callback_ctx,
                                           char_set_enc);
        if (!xml_reader)
        {
            return NULL;
        }
        om_builder = axiom_stax_builder_create(env, xml_reader);
        if (!om_builder)
        {
            axiom_xml_reader_free(xml_reader, env);
            xml_reader = NULL;
            return NULL;
        }
        soap_builder = axiom_soap_builder_create(env, om_builder, soap_ns_uri);
        if (!soap_builder)
        {
            /* We should not be freeing om_builder here as it is done by
               axiom_soap_builder_create in case of error - Samisa */
            om_builder = NULL;
            xml_reader = NULL;
            return NULL;
        }

        soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);

        if (binary_data_map)
        {
            axiom_soap_builder_set_mime_body_parts(soap_builder, env,
                                                   binary_data_map);
        }

        if (soap_envelope)
        {
            /* hack to get around MTOM problem */
            axiom_soap_body_t *soap_body =
                axiom_soap_envelope_get_body(soap_envelope, env);

            if (soap_body)
            {
                axiom_soap_body_has_fault(soap_body, env);
            }
        }
		if(stream)
		{
			axutil_stream_free(stream, env);
			callback_ctx->in_stream = NULL;
		}
        if(callback_ctx->chunked_stream)
        {
            axutil_http_chunked_stream_free(callback_ctx->chunked_stream, env);
            callback_ctx->chunked_stream = NULL;
        }

    }
    else
    {
        /* REST processing */
        axiom_xml_reader_t *xml_reader = NULL;
        axiom_stax_builder_t *om_builder = NULL;
        axiom_soap_body_t *def_body = NULL;
        axiom_document_t *om_doc = NULL;
        axiom_node_t *root_node = NULL;

        xml_reader = 
            axiom_xml_reader_create_for_io(env,
                                           axis2_http_transport_utils_on_data_request,
                                           NULL, (void *) callback_ctx,
                                           char_set_enc);
        if (!xml_reader)
        {
            return NULL;
        }
        om_builder = axiom_stax_builder_create(env, xml_reader);
        if (!om_builder)
        {
            axiom_xml_reader_free(xml_reader, env);
            xml_reader = NULL;
            return NULL;
        }
        soap_envelope = axiom_soap_envelope_create_default_soap_envelope
            (env, AXIOM_SOAP11);
        def_body = axiom_soap_envelope_get_body(soap_envelope, env);
        om_doc = axiom_stax_builder_get_document(om_builder, env);
        root_node = axiom_document_build_all(om_doc, env);
        axiom_soap_body_add_child(def_body, env, root_node);
        axiom_stax_builder_free_self(om_builder, env);
    }

    return soap_envelope;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_value_from_content_type(
    const axutil_env_t * env,
    const axis2_char_t * content_type,
    const axis2_char_t * key)
{
    axis2_char_t *tmp = NULL;
    axis2_char_t *tmp_content_type = NULL;
    axis2_char_t *tmp2 = NULL;

    AXIS2_PARAM_CHECK(env->error, content_type, NULL);
    AXIS2_PARAM_CHECK(env->error, key, NULL);

    tmp_content_type = axutil_strdup(env, content_type);
    if (!tmp_content_type)
    {
        return NULL;
    }
    tmp = strstr(tmp_content_type, key);
    if (!tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return NULL;
    }

    tmp = strchr(tmp, AXIS2_EQ);
    tmp2 = strchr(tmp, AXIS2_SEMI_COLON);

    if (tmp2)
    {
        *tmp2 = AXIS2_ESC_NULL;
    }
    if (!tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return NULL;
    }

    tmp2 = axutil_strdup(env, tmp + 1);

    AXIS2_FREE(env->allocator, tmp_content_type);
    if (*tmp2 == AXIS2_DOUBLE_QUOTE)
    {
        tmp = tmp2;
        tmp2 = axutil_strdup(env, tmp + 1);
        tmp2[strlen(tmp2) - 1] = AXIS2_ESC_NULL;
        if(tmp)
        {
            AXIS2_FREE(env->allocator, tmp); 
            tmp = NULL;
        }

    }
    /* handle XOP */
    if(*tmp2 == AXIS2_B_SLASH && *(tmp2 + 1) == '\"')
    {
        tmp = tmp2;
        tmp2 = axutil_strdup(env, tmp + 2);
        tmp2[strlen(tmp2) - 3] = AXIS2_ESC_NULL;
        if(tmp)
        {
            AXIS2_FREE(env->allocator, tmp);
            tmp = NULL;
        }
    }
    return tmp2;
}


AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_handle_media_type_url_encoded(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axutil_hash_t * param_map,
    axis2_char_t * method)
{
    axiom_soap_envelope_t *soap_env = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axiom_element_t *body_child = NULL;
    axiom_node_t *body_child_node = NULL;
    axiom_node_t *body_element_node = NULL;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, NULL);
    AXIS2_PARAM_CHECK(env->error, method, NULL);

    soap_env = axis2_msg_ctx_get_soap_envelope(msg_ctx, env);

    if (!soap_env)
    {
        soap_env = axiom_soap_envelope_create_default_soap_envelope(env,
                                                                    AXIOM_SOAP11);
    }
    soap_body = axiom_soap_envelope_get_body(soap_env, env);
    if (!soap_body)
    {
        AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                           AXIS2_FAILURE);
        return NULL;
    }

    body_element_node = axiom_soap_body_get_base_node(soap_body, env);

    if (body_element_node)
    {
        body_child_node = axiom_node_get_first_child(body_element_node, env);
    }

    if (!body_child_node)
    {
        if (!axis2_msg_ctx_get_op(msg_ctx, env))
        {
            return NULL;
        }
        body_child = axiom_element_create_with_qname(env, NULL,
                                                     axis2_op_get_qname
                                                     (axis2_msg_ctx_get_op
                                                      (msg_ctx, env), env),
                                                     &body_child_node);
        axiom_soap_body_add_child(soap_body, env, body_child_node);
    }    
    if (param_map)
    {
        axutil_hash_index_t *hi = NULL;
        for (hi = axutil_hash_first(param_map, env); hi;
             hi = axutil_hash_next(env, hi))
        {
            void *name = NULL;
            void *value = NULL;
            axiom_node_t *node = NULL;
            axiom_element_t *element = NULL;

            axutil_hash_this(hi, (const void **) &name, NULL, (void **) &value);
            element = axiom_element_create(env, NULL, (axis2_char_t *) name,
                                           NULL, &node);
            axiom_element_set_text(element, env, (axis2_char_t *) value, node);
            axiom_node_add_child(body_child_node, env, node);
        }
    }
    return soap_env;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_dispatch_and_verify(
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx)
{
    axis2_disp_t *rest_disp = NULL;
    axis2_handler_t *handler = NULL;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    if (axis2_msg_ctx_get_doing_rest(msg_ctx, env))
    {
        rest_disp = axis2_rest_disp_create(env);
        handler = axis2_disp_get_base(rest_disp, env);
        axis2_handler_invoke(handler, env, msg_ctx);

        if (!axis2_msg_ctx_get_svc(msg_ctx, env) ||
            !axis2_msg_ctx_get_op(msg_ctx, env))
        {
            AXIS2_HANDLE_ERROR(env, AXIS2_ERROR_SVC_OR_OP_NOT_FOUND,
                               AXIS2_FAILURE);
            return AXIS2_FAILURE;
        }
    }
   
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axutil_array_list_t *AXIS2_CALL
axis2_http_transport_utils_process_accept_headers(
	const axutil_env_t *env,
	axis2_char_t *accept_value )
{
	axutil_array_list_t *accept_field_list = NULL;
	axutil_array_list_t *accept_record_list = NULL;
	accept_field_list = axutil_tokenize(env, accept_value, ',');
	if (accept_field_list && axutil_array_list_size(accept_field_list, env) > 0)
		{
		axis2_char_t *token = NULL;
		accept_record_list = axutil_array_list_create(env, 
			axutil_array_list_size(accept_field_list, env));
		do
			{
			if (token)
				{
				axis2_http_accept_record_t *rec = NULL;
				rec = axis2_http_accept_record_create(env, token);
				if (rec)
					{
					axutil_array_list_add(accept_field_list, env, rec);
					}
				AXIS2_FREE(env->allocator, token);
				}
			token = (axis2_char_t *)axutil_array_list_remove(accept_field_list, env, 0);
			}
			while(token);
		}
	if (accept_record_list &&  axutil_array_list_size(accept_record_list, env) > 0)
	{
		return accept_record_list;
	}
	return NULL;
}

int axis2_http_transport_utils_check_status_code(int status_code)
{
	int status = AXIS2_HTTP_RESPONSE_OK_CODE_VAL;
	switch (status_code)
	{
		case AXIS2_HTTP_RESPONSE_CONTINUE_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_CONTINUE_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_ACK_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_ACK_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_MOVED_PERMANENTLY_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_MULTIPLE_CHOICES_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_SEE_OTHER_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_TEMPORARY_REDIRECT_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_GONE_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_GONE_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL;
			break;
		case AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL:
			status = AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL;
			break;
	}
	return status;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_process_request(
	const axutil_env_t *env,
	axis2_conf_ctx_t *conf_ctx,
	axis2_http_transport_in_t *request,
	axis2_http_transport_out_t *response)
{
	axis2_status_t status = AXIS2_FAILURE;
	axis2_msg_ctx_t *msg_ctx = NULL;
	axutil_stream_t *out_stream = NULL;
	axutil_string_t *soap_action = NULL;
	axis2_bool_t processed = AXIS2_FALSE;
	axis2_char_t *body_string = NULL;
	axis2_char_t *ctx_uuid = NULL;
	axis2_op_ctx_t *op_ctx = NULL;
	axis2_char_t *peer_ip = NULL;
	axutil_property_t *peer_property = NULL;
	axis2_msg_ctx_t *out_msg_ctx = NULL;
	axis2_msg_ctx_t **msg_ctx_map = NULL; 


	AXIS2_ENV_CHECK(env, AXIS2_CRITICAL_FAILURE);
	AXIS2_PARAM_CHECK(env->error, request, AXIS2_CRITICAL_FAILURE);
	AXIS2_PARAM_CHECK(env->error, request->request_uri, AXIS2_FALSE);

	if (!conf_ctx)
	{
		AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_CONFIGURATION_CONTEXT,
			AXIS2_FAILURE);
		return AXIS2_CRITICAL_FAILURE;
	}

	if (!request->content_type)
	{
		request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_PLAIN;
	}

	/** creating the out stream */
	out_stream = axutil_stream_create_basic(env);

	if(request->transfer_encoding)
		axis2_msg_ctx_set_transfer_encoding(request->msg_ctx, env, request->transfer_encoding);

	axis2_msg_ctx_set_server_side(request->msg_ctx , env, AXIS2_TRUE);
	msg_ctx = request->msg_ctx;
	peer_ip = request->remote_ip;

	if (peer_ip)
	{
		peer_property = axutil_property_create(env);
		axutil_property_set_value(peer_property, env, axutil_strdup(env, peer_ip));
		axis2_msg_ctx_set_property(msg_ctx, env, AXIS2_SVR_PEER_IP_ADDR, peer_property);
	}

	axis2_msg_ctx_set_transport_out_stream(msg_ctx, env, out_stream);

	ctx_uuid = axutil_uuid_gen(env);
	if (ctx_uuid)
	{
		axutil_string_t *uuid_str = axutil_string_create_assume_ownership(env, &ctx_uuid);
		axis2_msg_ctx_set_svc_grp_ctx_id(msg_ctx, env, uuid_str);
		axutil_string_free(uuid_str, env);
	}

	axis2_msg_ctx_set_out_transport_info(msg_ctx, env, &(request->out_transport_info->out_transport));

	if (request->accept_header)
	{
		axutil_array_list_t *accept_record_list = NULL;
		accept_record_list = axis2_http_transport_utils_process_accept_headers(env, 
			request->accept_header);
		if (accept_record_list &&  axutil_array_list_size(accept_record_list, env) > 0)
		{
			axis2_msg_ctx_set_http_accept_record_list(msg_ctx, env, accept_record_list);
		}
	}
	if(request->accept_charset_header)
	{
		axutil_array_list_t *accept_charset_record_list = NULL;
		accept_charset_record_list = axis2_http_transport_utils_process_accept_headers(env,
			request->accept_charset_header);
		if(accept_charset_record_list)
		{
			axis2_msg_ctx_set_http_accept_charset_record_list(msg_ctx, env,
				accept_charset_record_list);
		}
	}
	if(request->accept_language_header)
	{
		axutil_array_list_t *accept_language_record_list = NULL;
		accept_language_record_list = axis2_http_transport_utils_process_accept_headers(env,
			request->accept_language_header);
		if(accept_language_record_list)
		{
			axis2_msg_ctx_set_http_accept_language_record_list(msg_ctx, env,
				accept_language_record_list);
		}
	}
	
	if(request->soap_action)
	{
		soap_action = axutil_string_create(env, request->soap_action);	
	}

	if (request->request_method == AXIS2_HTTP_METHOD_GET || 
		request->request_method == AXIS2_HTTP_METHOD_DELETE ||
		request->request_method == AXIS2_HTTP_METHOD_HEAD)
	{

		if (request->request_method == AXIS2_HTTP_METHOD_DELETE)
		{
			processed = axis2_http_transport_utils_process_http_delete_request
				(env, request->msg_ctx, request->in_stream, out_stream,
				request->content_type, soap_action, request->request_uri, conf_ctx,
				axis2_http_transport_utils_get_request_params(env, request->request_uri));
		}
		else if (request->request_method == AXIS2_HTTP_METHOD_HEAD)
		{
			processed = axis2_http_transport_utils_process_http_head_request
				(env, request->msg_ctx, request->in_stream, out_stream,
				request->content_type, soap_action, request->request_uri,
				conf_ctx, axis2_http_transport_utils_get_request_params(env, request->request_uri));	
		}
		else if(request->request_method == AXIS2_HTTP_METHOD_GET)
		{
			processed = axis2_http_transport_utils_process_http_get_request
				(env, request->msg_ctx, request->in_stream, out_stream,
				request->content_type, soap_action, request->request_uri ,
				conf_ctx, 
				axis2_http_transport_utils_get_request_params(env, request->request_uri));
		}
		if (AXIS2_FALSE == processed)
		{
			axis2_bool_t is_services_path = AXIS2_FALSE;
			int msg_ctx_status_code = axis2_msg_ctx_get_status_code(msg_ctx, env);
			if (request->request_method != AXIS2_HTTP_METHOD_DELETE && request->request_url_prefix)
			{
				axis2_char_t *temp = NULL;
				temp = strstr(request->request_uri, request->request_url_prefix);
				if (temp)
				{
					temp += strlen(request->request_url_prefix);
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
			if (is_services_path)
			{
				body_string = axis2_http_transport_utils_get_services_html(env, conf_ctx);
				response->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
				response->http_status_code = AXIS2_HTTP_RESPONSE_OK_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_OK_CODE_NAME;
			}
			else if (AXIS2_HTTP_METHOD_DELETE != request->request_method)
			{		
				axis2_char_t *wsdl = NULL;
				wsdl = strstr(request->request_uri, AXIS2_REQUEST_WSDL);
				if(wsdl)
				{
					body_string = axis2_http_transport_utils_get_services_static_wsdl(env,
									conf_ctx, request->request_uri);
					request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML;
					response->http_status_code = AXIS2_HTTP_RESPONSE_OK_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_OK_CODE_NAME;
				}
			}
			else if (env->error->error_number == AXIS2_ERROR_SVC_OR_OP_NOT_FOUND)
			{
				axutil_array_list_t *method_list = NULL;
				int size = 0;
				method_list = axis2_msg_ctx_get_supported_rest_http_methods(msg_ctx, env);
				size = axutil_array_list_size(method_list, env);

				if (method_list && size)
				{
					/** 405 */
					body_string = axis2_http_transport_utils_get_method_not_allowed(env, conf_ctx);

					response->http_status_code = AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_NAME;
				}else 
				{
					/** 404  */
					body_string = axis2_http_transport_utils_get_not_found(env, conf_ctx);
					response->http_status_code = AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_NAME;
				}
			}
			else if (msg_ctx_status_code == AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL)
			{
				/* 400, Bad Request */
				body_string = axis2_http_transport_utils_get_bad_request(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_BAD_REQUEST_CODE_NAME;

			}
			else if (msg_ctx_status_code == AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL)
			{
				/* 408, Request Timeout */
				body_string = axis2_http_transport_utils_get_request_timeout(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_REQUEST_TIMEOUT_CODE_NAME;
			}
			else if (msg_ctx_status_code == AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL)
			{
				/* 409, Conflict Types */
				body_string = axis2_http_transport_utils_get_conflict(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_CONFLICT_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_CONFLICT_CODE_NAME;
			}
			else if(msg_ctx_status_code == AXIS2_HTTP_RESPONSE_GONE_CODE_VAL)
			{
				/* 410, Gone. Resource no longer available */
				body_string = axis2_http_transport_utils_get_gone(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_GONE_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_GONE_CODE_NAME;

			}
			else if(msg_ctx_status_code == AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL)
			{
				/*410, Precondition for the url failed  */
				body_string = axis2_http_transport_utils_get_precondition_failed(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_PRECONDITION_FAILED_CODE_NAME;

			}
			else if(msg_ctx_status_code == AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL)
			{
				/* 413, Request entity too large */
				body_string = axis2_http_transport_utils_get_request_entity_too_large(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_REQUEST_ENTITY_TOO_LARGE_CODE_NAME;
			}
			else if(msg_ctx_status_code == AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL)
			{	
				/* 513, Service Unavailable */
				body_string = axis2_http_transport_utils_get_service_unavailable(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_SERVICE_UNAVAILABLE_CODE_NAME;
			}
			else
			{
				/* 500, Internal Server Error */
				body_string = axis2_http_transport_utils_get_internal_server_error(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_NAME;
			}

			if (body_string)
			{
				response->response_data = body_string;
				response->response_data_length = axutil_strlen(body_string);
			}
			status = AXIS2_SUCCESS;
		}
	}
	else if (AXIS2_HTTP_METHOD_POST == request->request_method || 
		AXIS2_HTTP_METHOD_PUT == request->request_method)
	{
		if (AXIS2_HTTP_METHOD_POST == request->request_method)
		{
			processed = axis2_http_transport_utils_process_http_post_request
				(env, request->msg_ctx, request->in_stream, out_stream, request->content_type, 
					request->content_length, soap_action, request->request_uri);
		}
		else
		{
			processed = axis2_http_transport_utils_process_http_put_request
				(env, request->msg_ctx, request->in_stream, out_stream, request->content_type, 
				request->content_length, soap_action, request->request_uri);
		}
		if (AXIS2_FAILURE == processed && (AXIS2_HTTP_METHOD_PUT == request->request_method || 
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
					/** 405 */
					body_string = axis2_http_transport_utils_get_method_not_allowed(env, conf_ctx);
					response->http_status_code = AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_METHOD_NOT_ALLOWED_CODE_NAME;
				}
				else
				{
					/** 404  */
					body_string = axis2_http_transport_utils_get_not_found(env, conf_ctx);
					response->http_status_code = AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_NOT_FOUND_CODE_NAME;
				}
				request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
			}
			else
			{
				/* 500, Internal Server Error */
				body_string = axis2_http_transport_utils_get_internal_server_error(env, conf_ctx);
				response->http_status_code = AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_NAME;

			}

			if (body_string)
			{
				response->response_data = body_string;
				response->response_data_length = axutil_strlen(body_string);
			}
			status = AXIS2_SUCCESS;
		}
		else if (processed == AXIS2_FAILURE)
		{
			axis2_msg_ctx_t *fault_ctx = NULL;
			axis2_char_t *fault_code = NULL;
			axis2_engine_t *engine = axis2_engine_create(env, conf_ctx);
			if (!engine)
			{
				/* Critical error, cannot proceed, send defaults document for 500 */
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
			fault_ctx = axis2_engine_create_fault_msg_ctx(engine, env, request->msg_ctx,
				fault_code, axutil_error_get_message(env->error));

			axis2_engine_send_fault(engine, env, fault_ctx);
			if (out_stream)
			{
				response->response_data  = axutil_stream_get_buffer(out_stream, env);				
				response->response_data_length = axutil_stream_get_len (out_stream, env);								
			}
			/* In case of a SOAP Fault, we have to set the status to 500, but still return */
			status = AXIS2_SUCCESS;
			response->http_status_code = AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_VAL;
			response->http_status_code_name = AXIS2_HTTP_RESPONSE_INTERNAL_SERVER_ERROR_CODE_NAME;
		}
	}
	else
	{
		response->response_data = axis2_http_transport_utils_get_not_implemented(env, conf_ctx);
		request->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;

		if (response->response_data)
		{
			response->response_data_length = axutil_strlen(response->response_data);
		}
		response->http_status_code = AXIS2_HTTP_RESPONSE_NOT_IMPLEMENTED_CODE_VAL;
		response->http_status_code_name = AXIS2_HTTP_RESPONSE_NOT_IMPLEMENTED_CODE_NAME;
		status = AXIS2_SUCCESS;
	}

	op_ctx = axis2_msg_ctx_get_op_ctx(msg_ctx, env);

	if (op_ctx)
	{
		msg_ctx_map = axis2_op_ctx_get_msg_ctx_map(op_ctx, env);
		out_msg_ctx = msg_ctx_map[AXIS2_WSDL_MESSAGE_LABEL_OUT];
		response->msg_ctx = out_msg_ctx;
	}

	if (status == AXIS2_FAILURE)
	{
		axis2_bool_t do_rest = AXIS2_FALSE;
		if (AXIS2_HTTP_METHOD_POST != request->request_method || 
			axis2_msg_ctx_get_doing_rest(msg_ctx, env))
		{
			do_rest = AXIS2_TRUE;
		}
		if ((request->accept_header || request->accept_charset_header ||
			request->accept_language_header) && do_rest)
		{
			axis2_char_t *content_type_header_value = NULL;
			axis2_char_t *temp = NULL;
			axis2_char_t *language_header_value = NULL;

			content_type_header_value = (axis2_char_t *) request->content_type;
			language_header_value = axis2_msg_ctx_get_content_language(out_msg_ctx,env);
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
					char_set = axutil_strcasestr(temp2, AXIS2_HTTP_CHAR_SET_ENCODING);
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
				if (content_type && request->accept_header &&
					!axutil_strcasestr(request->accept_header, content_type))
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
						if (!axutil_strcasestr(request->accept_header, temp) &&
							!strstr(request->accept_header, AXIS2_HTTP_HEADER_ACCEPT_ALL))
						{
							response->response_data = 
								axis2_http_transport_utils_get_not_acceptable(env, conf_ctx);
							response->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
							if (response->response_data)
							{
								response->response_data_length = axutil_strlen(response->response_data);
							}
							response->http_status_code = AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_VAL;
							response->http_status_code_name = AXIS2_HTTP_RESPONSE_NOT_IMPLEMENTED_CODE_NAME;
							status = AXIS2_TRUE;
						}
						AXIS2_FREE(env->allocator, temp);
					}
				}
				if (content_type)
				{
					AXIS2_FREE(env->allocator, content_type);
				}
				if (char_set && request->accept_charset_header && 
					!axutil_strcasestr(request->accept_charset_header , char_set))
				{
					response->response_data = 
						axis2_http_transport_utils_get_not_acceptable(env, conf_ctx);
					response->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;

					if (response->response_data)
					{
						response->response_data_length= axutil_strlen(response->response_data);
					}
					status = AXIS2_SUCCESS;
					response->http_status_code = AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_NAME;
				}
				if (char_set)
				{
					AXIS2_FREE(env->allocator, char_set);
				}
			}
			if (language_header_value)
			{
				if (request->accept_language_header &&
					!axutil_strcasestr(request->accept_language_header	, language_header_value))
				{
					response->response_data = 
						axis2_http_transport_utils_get_not_acceptable(env, conf_ctx);
					response->content_type = AXIS2_HTTP_HEADER_ACCEPT_TEXT_HTML;
					if (response->response_data)
					{
						response->response_data_length = axutil_strlen(response->response_data);
					}
					response->http_status_code = AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_NOT_ACCEPTABLE_CODE_NAME;
				}
			}
		}
	}
	if (status == AXIS2_FAILURE)
	{
		axis2_bool_t do_rest = AXIS2_FALSE;
		if (AXIS2_HTTP_METHOD_POST != request->request_method ||
			axis2_msg_ctx_get_doing_rest(msg_ctx, env))
		{
			do_rest = AXIS2_TRUE;
		}
		if (op_ctx && axis2_op_ctx_get_response_written(op_ctx, env))
		{
			if (do_rest)
			{
				if (out_msg_ctx)
				{
					int size = 0;
					axutil_array_list_t *output_header_list = NULL;
					output_header_list = axis2_msg_ctx_get_http_output_headers(out_msg_ctx, env);
					if (output_header_list)
					{
						size = axutil_array_list_size(output_header_list, env);
						response->output_headers = output_header_list;
					}
					
					if (axis2_msg_ctx_get_status_code(out_msg_ctx, env))
					{
						int status_code = axis2_msg_ctx_get_status_code(out_msg_ctx, env);
						response->http_status_code = 
							axis2_http_transport_utils_check_status_code(status_code);
						status = AXIS2_SUCCESS;
					}
				}
			}
			if (status == AXIS2_FAILURE)
			{
				status = AXIS2_SUCCESS;
				if (out_stream)
				{
					response->response_data = axutil_stream_get_buffer(out_stream, env);
					response->response_data_length = axutil_stream_get_len(out_stream, env);
					response->http_status_code = AXIS2_HTTP_RESPONSE_OK_CODE_VAL;
					response->http_status_code_name = AXIS2_HTTP_RESPONSE_OK_CODE_NAME;
				}
			}
		}
		else if (op_ctx)
		{
			if (do_rest)
			{
				if (out_msg_ctx)
				{
					int size = 0;
					axutil_array_list_t *output_header_list = NULL;
					output_header_list = axis2_msg_ctx_get_http_output_headers(out_msg_ctx, env);
					if (output_header_list)
					{
						size = axutil_array_list_size(output_header_list, env);
						response->output_headers = output_header_list;
					}
					if (axis2_msg_ctx_get_no_content(out_msg_ctx, env))
					{
						if (axis2_msg_ctx_get_status_code(out_msg_ctx, env))
						{
							int status_code = axis2_msg_ctx_get_status_code(out_msg_ctx, env);
							switch (status_code)
							{
								case AXIS2_HTTP_RESPONSE_RESET_CONTENT_CODE_VAL:
									response->http_status_code = AXIS2_HTTP_RESPONSE_RESET_CONTENT_CODE_VAL;				
									break;
								case AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL:
									response->http_status_code = AXIS2_HTTP_RESPONSE_NOT_MODIFIED_CODE_VAL;				
									break;
								default:
									response->http_status_code = AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_VAL;
									break;
								}
							}
							else
							{
								response->http_status_code = AXIS2_HTTP_RESPONSE_NO_CONTENT_CODE_VAL;
							}
							status = AXIS2_SUCCESS;
					}
					else if (axis2_msg_ctx_get_status_code(out_msg_ctx, env))
					{
						int status_code = axis2_msg_ctx_get_status_code(out_msg_ctx, env);
						response->http_status_code = 
							axis2_http_transport_utils_check_status_code(status_code);
						status = AXIS2_SUCCESS;
					}
				}
			}
			if (status == AXIS2_FAILURE)
			{
				response->http_status_code = AXIS2_HTTP_RESPONSE_ACK_CODE_VAL;
				response->http_status_code_name = AXIS2_HTTP_RESPONSE_ACK_CODE_NAME;
				status = AXIS2_SUCCESS;
			}
		}
		else
		{
			status = AXIS2_SUCCESS;
			response->http_status_code = AXIS2_HTTP_RESPONSE_ACK_CODE_VAL;
			response->http_status_code_name = AXIS2_HTTP_RESPONSE_ACK_CODE_NAME;
		}
	}
	axutil_string_free(soap_action, env);
	msg_ctx = NULL;
	
	return status;
}

/* This method takes an array_list as the input. It has items some 
   may be buffers and some may be files. This will send these part
   one by one to the wire using the chunked stream.*/

AXIS2_EXTERN axis2_status_t AXIS2_CALL  
axis2_http_transport_utils_send_mtom_message(
    axutil_http_chunked_stream_t * chunked_stream,
    const axutil_env_t * env,
    axutil_array_list_t *mime_parts,
    axis2_char_t *sending_callback_name)
{
    int i = 0;
    axiom_mime_part_t *mime_part = NULL;
    axis2_status_t status = AXIS2_SUCCESS;
    int written = 0;
    int len = 0;    

    if(mime_parts)
    {
        for(i = 0; i < axutil_array_list_size
                (mime_parts, env); i++)
        {
            mime_part = (axiom_mime_part_t *)axutil_array_list_get(
                mime_parts, env, i);
            
            /* If it is a buffer just write it to the wire. This includes mime_bounadaries,
             * mime_headers and SOAP */
            
            if((mime_part->type) == AXIOM_MIME_PART_BUFFER)
            {
                written = 0;
                while(written < mime_part->part_size)
                {
                    len = 0;
                    len = axutil_http_chunked_stream_write(chunked_stream, env,
                        mime_part->part + written, mime_part->part_size - written);
                    if (len == -1)
                    {
                        status = AXIS2_FAILURE;
                        break;
                    }
                    else
                    {
                        written += len;
                    }
                }
            }
            
            /* If it is a file we load a very little portion to memory 
             * and send it as chunked , we keep on doing this until we find
             * the end of the file */ 
            else if((mime_part->type) == AXIOM_MIME_PART_FILE)
            {
                FILE *f = NULL;
                axis2_byte_t *output_buffer = NULL;                
                int output_buffer_size = 0;

                f = fopen(mime_part->file_name, "rb");
                if (!f)
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "Error opening file %s for reading",
                    mime_part->file_name);
                    return AXIS2_FAILURE;
                }
                
                /*If the part_size is less than the defined buffer size then 
                 *from the first write to the wire we can send the file */
                
                if(mime_part->part_size > AXIS2_MTOM_OUTPUT_BUFFER_SIZE)
                {
                    output_buffer_size = AXIS2_MTOM_OUTPUT_BUFFER_SIZE;
                }
                else
                {
                    output_buffer_size = mime_part->part_size;
                }
               
                output_buffer =  AXIS2_MALLOC(env->allocator, 
                    (output_buffer_size + 1) * sizeof(axis2_char_t));
 
                /*This is the method responsible for writing to the wire */    
                status = axis2_http_transport_utils_send_attachment_using_file(env, chunked_stream, 
                    f, output_buffer, output_buffer_size);
                if(status == AXIS2_FAILURE)
                {
                    return status;
                }
            }
            else if((mime_part->type) == AXIOM_MIME_PART_CALLBACK) 
            {
                void *handler = NULL;
                axiom_mtom_sending_callback_t *callback = NULL;

                handler = axis2_http_transport_utils_initiate_callback(env, 
                    sending_callback_name, mime_part->user_param, &callback);

                if(handler)
                {
                    status = axis2_http_transport_utils_send_attachment_using_callback(env, 
                        chunked_stream, callback, handler, mime_part->user_param);
                }
                else
                {
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "MTOM Sending Callback loading failed");
                    status = AXIS2_FAILURE;
                }               

                if(callback)
                {
                    axutil_param_t *param = NULL;
                    
                    param = callback->param;

                    AXIOM_MTOM_SENDING_CALLBACK_FREE(callback, env);
                    callback = NULL;
                    if(param)
                    {
                        axutil_param_free(param, env);
                        param = NULL;    
                    }
                }

                if(status == AXIS2_FAILURE)
                {
                    return status;
                }
            }
            else
            {
                AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Unknown mime_part.");
                return AXIS2_FAILURE;
            }
            if(status == AXIS2_FAILURE)
            {
                break;
            }
        }
        if(status == AXIS2_SUCCESS)
        {
           /* send the end of chunk */
            axutil_http_chunked_stream_write_last_chunk(chunked_stream, env);
            return AXIS2_SUCCESS;
        }
        else
        {
            return status;
        }
    }    
    else
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Cannot send the attachment.Mime"
                "Parts are not set properly.");
        return AXIS2_FAILURE;
    }    
}


static axis2_status_t
axis2_http_transport_utils_send_attachment_using_file(
    const axutil_env_t * env,
    axutil_http_chunked_stream_t *chunked_stream,
    FILE *fp,
    axis2_byte_t *buffer,
    int buffer_size)
{

    int count = 0;     
    int len = 0;
    int written = 0;
    axis2_status_t status = AXIS2_SUCCESS;   
 
    /*We do not load the whole file to memory. Just load a buffer_size portion
     *and send it. Keep on doing this until the end of file */
    
    do
    {
        count = (int)fread(buffer, 1, buffer_size + 1, fp);
        if (ferror(fp))
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Error in reading file containg the attachment");
            if (buffer)
            {
                AXIS2_FREE(env->allocator, buffer);
                buffer = NULL;
            }
            fclose(fp);
            return AXIS2_FAILURE;
        }

        /*Writing the part we loaded to memory to the wire*/
        if(count > 0)
        {
            written = 0;
            while(written < count)
            {
                len = 0;
                len = axutil_http_chunked_stream_write(chunked_stream, env,
                    buffer + written, count - written);
                if (len == -1)
                {
                    status = AXIS2_FAILURE;
                    break;
                }
                else
                {
                    written += len;
                }
            }
        }
        else
        {
            if (buffer)
            {
                AXIS2_FREE(env->allocator, buffer);
                buffer = NULL;
            }
            fclose(fp);
            return AXIS2_FAILURE;
        }  
        
        /*We keep on loading the next part to the same buffer. So need to reset 
         * te buffer */
        memset(buffer, 0, buffer_size);    
        if(status == AXIS2_FAILURE)
        {
            if (buffer)
            {
                AXIS2_FREE(env->allocator, buffer);
                buffer = NULL;
            }
            fclose(fp);
            return AXIS2_FAILURE;
        } 
    }
    while(!feof(fp));
    
    if(buffer)
    {
        AXIS2_FREE(env->allocator, buffer);    
        buffer = NULL;
    }

    fclose(fp);
    return AXIS2_SUCCESS;    
}

AXIS2_EXTERN void AXIS2_CALL axis2_http_transport_utils_destroy_mime_parts(
    axutil_array_list_t *mime_parts,
    const axutil_env_t *env)
{
    if (mime_parts)
    {
        int i = 0;
        for (i = 0; i < axutil_array_list_size(mime_parts, env); i++)
        {
            axiom_mime_part_t *mime_part = NULL;
            mime_part = (axiom_mime_part_t *)
                axutil_array_list_get(mime_parts, env, i);
            if (mime_part)
            {
                axiom_mime_part_free(mime_part, env);
            }
        }
        axutil_array_list_free(mime_parts, env);
    }
}

AXIS2_EXTERN void *AXIS2_CALL axis2_http_transport_utils_initiate_callback(
    const axutil_env_t *env,
    axis2_char_t *callback_name,
    void *user_param,
    axiom_mtom_sending_callback_t **callback)
{

    axutil_dll_desc_t *dll_desc = NULL;
    axutil_param_t *impl_info_param = NULL;
    void *ptr = NULL;

    if(callback_name)
    {
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "Trying to load module = %s",
                callback_name);
        dll_desc = axutil_dll_desc_create(env);
        axutil_dll_desc_set_name(dll_desc, env, callback_name);
        impl_info_param = axutil_param_create(env, NULL, dll_desc);
        /*Set the free function*/
        axutil_param_set_value_free(impl_info_param, env, axutil_dll_desc_free_void_arg);
        axutil_class_loader_init(env);
        ptr = axutil_class_loader_create_dll(env, impl_info_param);

        if (!ptr)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                "Unable to load the module %s. ERROR", callback_name);
            return NULL;
        }

        *callback =  (axiom_mtom_sending_callback_t *)ptr;
        (*callback)->param = impl_info_param;

        return AXIOM_MTOM_SENDING_CALLBACK_INIT_HANDLER(*callback, env, user_param);
    }

    else
    {
        return NULL;
    }
}

static axis2_status_t
axis2_http_transport_utils_send_attachment_using_callback(
    const axutil_env_t * env,
    axutil_http_chunked_stream_t *chunked_stream,
    axiom_mtom_sending_callback_t *callback,
    void *handler,
    void *user_param)
{
    int count = 0;     
    int len = 0;
    int written = 0;
    axis2_status_t status = AXIS2_SUCCESS;   
    axis2_char_t *buffer = NULL;
 
    /* Keep on loading the data in a loop until 
     * all the data is sent */   

    while((count = AXIOM_MTOM_SENDING_CALLBACK_LOAD_DATA(callback, env, handler, &buffer)) > 0)
    {
        written = 0;
        while(written < count)
        {
            len = 0;
            len = axutil_http_chunked_stream_write(chunked_stream, env,
            buffer + written, count - written);
            if(len == -1)
            {
                status = AXIS2_FAILURE;
                break;
            }
            else
            {
                written += len;
            }
        }
    }

    if (status == AXIS2_FAILURE)
    {
        AXIOM_MTOM_SENDING_CALLBACK_CLOSE_HANDLER(callback, env, handler);
        return status;
    }
    
    status = AXIOM_MTOM_SENDING_CALLBACK_CLOSE_HANDLER(callback, env, handler);
    return status;    
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL axis2_http_transport_utils_is_callback_required(
    const axutil_env_t *env,
    axutil_array_list_t *mime_parts)
{
    int size = 0;
    int i = 0;
    axiom_mime_part_t *mime_part = NULL;
    axis2_bool_t is_required = AXIS2_FALSE;

    size = axutil_array_list_size(mime_parts, env);

    for(i = 0; i < size; i++)
    {
        mime_part = (axiom_mime_part_t *)axutil_array_list_get(mime_parts, env, i);    
        if(mime_part)
        {
            if(mime_part->type == AXIOM_MIME_PART_CALLBACK)
            {
                is_required = AXIS2_TRUE;
                break;
            }    
        }
    }

    return is_required;

}
