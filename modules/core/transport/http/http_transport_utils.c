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

#include <axis2_http_transport_utils.h>
#include <string.h>
#include <ctype.h>
#include <axis2_conf.h>
#include <axis2_op.h>
#include <axis2_qname.h>
#include <axis2_http_transport.h>
#include <axiom_soap_builder.h>
#include <axis2_engine.h>
#include <axiom_soap_body.h>
#include <axis2_utils.h>
#include <axiom_namespace.h>
#include <axiom_node.h>
#include <axis2_hash.h>
#include <axiom_soap_const.h>
#include <axis2_http_header.h>
#include <axis2_property.h>
#include <axis2_utils.h>
#include <axiom_mime_parser.h>
#include <xml_schema_type.h>
#include <xml_schema_group_base.h>
#include <xml_schema_obj_collection.h>
#include <xml_schema_particle.h>
#include <xml_schema_complex_type.h>
#include <axis2_disp.h>
#include <axis2_msg.h>

#define AXIOM_MIME_BOUNDARY_BYTE 45

/***************************** Function headers *******************************/

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_process_http_post_request(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_stream_t *in_stream,
    axis2_stream_t *out_stream,
    const axis2_char_t *content_type,
    const int content_length,
    axis2_char_t *soap_action_header,
    const axis2_char_t *request_uri);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_get_request(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_stream_t *in_stream,
    axis2_stream_t *out_stream,
    const axis2_char_t *content_type,
    axis2_char_t *soap_action_header,
    const axis2_char_t *request_uri,
    axis2_conf_ctx_t *conf_ctx,
    axis2_hash_t *request_params);

AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL
axis2_http_transport_utils_select_builder_for_mime(
    const axis2_env_t *env,
    axis2_char_t *request_uri,
    axis2_msg_ctx_t *msg_ctx,
    axis2_stream_t *in_stream,
    axis2_char_t *content_type);

AXIS2_EXTERN axis2_bool_t  AXIS2_CALL
axis2_http_transport_utils_is_optimized(
    const axis2_env_t *env,
    axiom_element_t *om_element);

AXIS2_EXTERN axis2_bool_t  AXIS2_CALL
axis2_http_transport_utils_do_write_mtom(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

AXIS2_EXTERN axis2_bool_t  AXIS2_CALL
axis2_http_transport_utils_is_doing_rest(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_is_doing_rest_through_post(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_strdecode(
    const axis2_env_t *env,
    axis2_char_t *dest,
    axis2_char_t *src);

AXIS2_EXTERN int AXIS2_CALL
axis2_http_transport_utils_hexit(
    axis2_char_t c);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_services_html(
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_charset_enc(
    const axis2_env_t *env,
    const axis2_char_t *content_type);

int AXIS2_CALL
axis2_http_transport_utils_on_data_request(
    char *buffer,
    int size,
    void *ctx);

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_create_soap_msg(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    const axis2_char_t *soap_ns_uri);

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_value_from_content_type(
    const axis2_env_t *env,
    const axis2_char_t *content_type,
    const axis2_char_t *key);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_dispatch_and_verify(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_handle_media_type_url_encoded(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_hash_t *param_map,
    axis2_char_t *method,
    xml_schema_element_t *schema_element);

/***************************** End of function headers ************************/

AXIS2_EXTERN axis2_status_t  AXIS2_CALL
axis2_http_transport_utils_process_http_post_request(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_stream_t *in_stream,
    axis2_stream_t *out_stream,
    const axis2_char_t *content_type,
    const int content_length,
    axis2_char_t *soap_action_header,
    const axis2_char_t *request_uri)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_soap_builder_t *soap_builder = NULL;
    axiom_stax_builder_t *om_builder = NULL;
    axis2_bool_t is_soap11 = AXIS2_FALSE;
    axiom_xml_reader_t *xml_reader = NULL;
    axis2_char_t *char_set = NULL;
    /*axis2_char_t *xml_char_set = NULL;*/
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_callback_info_t *callback_ctx;
    axis2_hash_t *headers = NULL;
    axis2_engine_t *engine = NULL;
    axiom_soap_body_t *soap_body = NULL;
    axis2_property_t *property = NULL;
    axis2_status_t status = AXIS2_FAILURE;
    axis2_hash_t *binary_data_map = NULL;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, in_stream, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_stream, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, content_type, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FAILURE);

    conf_ctx = AXIS2_MSG_CTX_GET_CONF_CTX(msg_ctx, env);

    callback_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t));
    /* Note: the memory created above is freed in xml reader free function
       as this is passed on to the reader */
    callback_ctx->in_stream = in_stream;
    callback_ctx->env = env;
    callback_ctx->content_length = content_length;
    callback_ctx->unread_len = content_length;
    callback_ctx->chunked_stream = NULL;

    if (soap_action_header && (strlen(soap_action_header) > 0))
    {
        /* remove leading and trailing " s */
        if ('"' == soap_action_header[0])
        {
            memmove(soap_action_header, soap_action_header + sizeof(axis2_char_t),
                    strlen(soap_action_header) + sizeof(axis2_char_t));
        }
        if ('"' == soap_action_header[strlen(soap_action_header) -1])
        {
            soap_action_header[strlen(soap_action_header) -1] = '\0';
        }
    }
    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, AXIS2_TRANSPORT_HEADERS,
            AXIS2_FALSE);
    if (property)
    {
        headers = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
    if (headers)
    {
        axis2_http_header_t *encoding_header = NULL;
        encoding_header = (axis2_http_header_t *)axis2_hash_get(headers,
                AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
                AXIS2_HASH_KEY_STRING);
        if (encoding_header)
        {
            axis2_char_t *encoding_value = NULL;
            encoding_value = AXIS2_HTTP_HEADER_GET_VALUE(encoding_header, env);
            if (encoding_value && 0 == AXIS2_STRCASECMP(encoding_value,
                    AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                callback_ctx->chunked_stream = axis2_http_chunked_stream_create(
                            env, in_stream);
                if (NULL == callback_ctx->chunked_stream)
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
        axis2_property_t *property = NULL;
        axis2_char_t *value = NULL;
        property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env, 
            AXIS2_HTTP_HEADER_TRANSFER_ENCODING,
            AXIS2_FALSE);
        if (property)
            value = (axis2_char_t *)AXIS2_PROPERTY_GET_VALUE(property, env);

        if (value && AXIS2_STRSTR(value, AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
        {
            /* this is an UGLY hack to get some of the trnaports working 
                e.g. PHP transport where it strips the chunking info in case of chunking 
                and also gives out a content lenght of 0.
                We need to fix the transport design to fix sutuations like this.
                */
            callback_ctx->content_length = 1000000;
            callback_ctx->unread_len = callback_ctx->content_length;
        }
    }

    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_MULTIPART_RELATED))
    {
        /* get mime boundry */
        axis2_char_t *mime_boundary =
            axis2_http_transport_utils_get_value_from_content_type(env,
                    content_type, AXIS2_HTTP_HEADER_CONTENT_TYPE_MIME_BOUNDARY);

        if (mime_boundary)
        {
            axiom_mime_parser_t *mime_parser = NULL;
            axis2_stream_t *stream = NULL;
            int soap_body_len = 0;
            axis2_char_t *soap_body_str = NULL;

            mime_parser = axiom_mime_parser_create(env);
            if (mime_parser)
            {
                binary_data_map = AXIOM_MIME_PARSER_PARSE(mime_parser, env,
                        axis2_http_transport_utils_on_data_request,
                        (void *) callback_ctx, mime_boundary);

                soap_body_len = AXIOM_MIME_PARSER_GET_SOAP_BODY_LENGTH(
                            mime_parser, env);
                soap_body_str = AXIOM_MIME_PARSER_GET_SOAP_BODY_STR(
                            mime_parser, env);
            }

            stream = axis2_stream_create_basic(env);
            if (stream)
            {
                AXIS2_STREAM_WRITE(stream, env, soap_body_str, soap_body_len);
                /*AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, 
                    "axis2_http_transport_utils_process_http_post_request soap_body_str = %s...%d soap_body_len=%d", 
                    soap_body_str, strlen(soap_body_str), soap_body_len);*/
                callback_ctx->in_stream = stream;
                callback_ctx->chunked_stream = NULL;
                callback_ctx->content_length = soap_body_len;
                callback_ctx->unread_len = soap_body_len;
            }
            AXIOM_MIME_PARSER_FREE(mime_parser, env);
            mime_parser = NULL;
        }
        AXIS2_FREE(env->allocator, mime_boundary);
    }
    AXIS2_MSG_CTX_SET_WSA_ACTION(msg_ctx, env, soap_action_header);
    AXIS2_MSG_CTX_SET_SOAP_ACTION(msg_ctx, env, soap_action_header);
    AXIS2_MSG_CTX_SET_TO(msg_ctx, env, axis2_endpoint_ref_create(env,
            request_uri));

    AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);

    char_set = axis2_http_transport_utils_get_charset_enc(env, content_type);
    xml_reader = axiom_xml_reader_create_for_io(env,
            axis2_http_transport_utils_on_data_request, NULL,
            (void *) callback_ctx, char_set);

    if (NULL == xml_reader)
    {
        return AXIS2_FAILURE;
    }

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_VALUE(property, env, char_set);
    AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env, AXIS2_CHARACTER_SET_ENCODING,
            property, AXIS2_TRUE);

    om_builder = axiom_stax_builder_create(env, xml_reader);
    if (NULL == om_builder)
    {
        AXIOM_XML_READER_FREE(xml_reader, env);
        xml_reader = NULL;
        return AXIS2_FAILURE;
    }

    if (strstr(content_type, AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP))
    {
        is_soap11 = AXIS2_FALSE;
        soap_builder = axiom_soap_builder_create(env, om_builder,
                AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
        if (NULL == soap_builder)
        {
            /* We should not be freeing om_builder here as it is done by
               axiom_soap_builder_create in case of error - Samisa*/
            /*AXIOM_STAX_BUILDER_FREE(om_builder, env);*/
            om_builder = NULL;
            xml_reader = NULL;
            return AXIS2_FAILURE;
        }

        soap_envelope = AXIOM_SOAP_BUILDER_GET_SOAP_ENVELOPE(soap_builder,
                env);
        if (NULL == soap_envelope)
        {
            AXIOM_STAX_BUILDER_FREE(om_builder, env);
            om_builder = NULL;
            xml_reader = NULL;
            AXIOM_SOAP_BUILDER_FREE(soap_builder, env);
            soap_builder = NULL;
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
            if (NULL == soap_builder)
            {
                /* We should not be freeing om_builder here as it is done by
                   axiom_soap_builder_create in case of error - Samisa*/
                /*AXIOM_STAX_BUILDER_FREE(om_builder, env);*/
                om_builder = NULL;
                xml_reader = NULL;
                return AXIS2_FAILURE;
            }
            soap_envelope = AXIOM_SOAP_BUILDER_GET_SOAP_ENVELOPE(
                        soap_builder, env);
            if (NULL == soap_envelope)
            {
                AXIOM_SOAP_BUILDER_FREE(soap_builder, env);
                om_builder = NULL;
                xml_reader = NULL;
                soap_builder = NULL;
                return AXIS2_FAILURE;
            }
        }
        else
        {
            /* REST support */
            axis2_param_t *rest_param = AXIS2_MSG_CTX_GET_PARAMETER(msg_ctx, env
                    , AXIS2_ENABLE_REST);
            if (rest_param && 0 == AXIS2_STRCMP(AXIS2_VALUE_TRUE,
                    AXIS2_PARAM_GET_VALUE(rest_param, env)))
            {
                /* TODO we have to check for NULLs */
                axiom_soap_body_t *def_body = NULL;
                axiom_document_t *om_doc = NULL;
                axiom_node_t *root_node = NULL;
                soap_envelope = axiom_soap_envelope_create_default_soap_envelope
                        (env, AXIOM_SOAP11);
                def_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
                om_doc = AXIOM_STAX_BUILDER_GET_DOCUMENT(om_builder, env);
                root_node = AXIOM_DOCUMENT_BUILD_ALL(om_doc, env);
                AXIOM_SOAP_BODY_ADD_CHILD(def_body, env, root_node);
                AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx, env, AXIS2_TRUE);
            }
        }

    }

    if (binary_data_map)
    {
        AXIOM_SOAP_BUILDER_SET_MIME_BODY_PARTS(soap_builder, env,
                binary_data_map);
    }

    /* xml_char_set = AXIOM_DOCUMENT_GET_CHARSET_ENC(
     *               AXIOM_STAX_BUILDER_GET_DOCUMENT(env om_builder),
     *               env);
     *
     *if(0 != AXIS2_STRCMP(char_set, xml_char_set))
     *{
     *   AXIS2_ERROR_SET(env->error, AXIS2_ERROR_CHARSET_MISMATCH, 
     *               AXIS2_FAILURE);
     *   AXIOM_SOAP_ENVELOPE_FREE(envelope, env);
     *   envelope = NULL;
     *   AXIOM_XML_READER_FREE(xml_reader, env);
     *   xml_reader = NULL;
     *   AXIOM_STAX_BUILDER_FREE(om_builder, env);
     *   om_builder = NULL;
     *   if( soap_builder)
     *   {
     *       AXIOM_SOAP_BUILDER_FREE(soap_builder, env);
     *       soap_builder = NULL;
     *   }
     *   return AXIS2_FAILURE;
     *}
     */

    AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, soap_envelope);

    engine = axis2_engine_create(env, conf_ctx);

    if (!soap_envelope)
        return AXIS2_FAILURE;

    soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope,
            env);

    if (NULL == soap_body)
        return AXIS2_FAILURE;

    if (AXIS2_TRUE == AXIOM_SOAP_BODY_HAS_FAULT(soap_body, env))
    {
        status = AXIS2_ENGINE_RECEIVE_FAULT(engine, env, msg_ctx);
    }
    else
    {
        status = AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx);
    }
    if (NULL == AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env) &&
            AXIS2_FALSE == is_soap11)
    {
        axiom_soap_envelope_t *def_envelope =
            axiom_soap_envelope_create_default_soap_envelope(env,
                    AXIOM_SOAP12);
        AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, def_envelope);
    }
    if (engine)
    {
        AXIS2_ENGINE_FREE(engine, env);
    }
    return status;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_process_http_get_request(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_stream_t *in_stream,
    axis2_stream_t *out_stream,
    const axis2_char_t *content_type,
    axis2_char_t *soap_action_header,
    const axis2_char_t *request_uri,
    axis2_conf_ctx_t *conf_ctx,
    axis2_hash_t *request_params)
{
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_engine_t *engine = NULL;
    axis2_property_t *property = NULL;
    axis2_op_t *op = NULL;
    xml_schema_element_t *schema_element = NULL;

    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, in_stream, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, out_stream, AXIS2_FALSE);
    /*AXIS2_PARAM_CHECK(env->error, content_type, AXIS2_FALSE);*/
    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FALSE);
    AXIS2_PARAM_CHECK(env->error, request_params, AXIS2_FALSE);


    AXIS2_MSG_CTX_SET_TO(msg_ctx, env, axis2_endpoint_ref_create(env,
            request_uri));

    property = axis2_property_create(env);
    AXIS2_PROPERTY_SET_SCOPE(property, env, AXIS2_SCOPE_REQUEST);
    AXIS2_PROPERTY_SET_FREE_FUNC(property, env, axis2_stream_free_void_arg);
    AXIS2_PROPERTY_SET_VALUE(property, env, out_stream);
    AXIS2_MSG_CTX_SET_PROPERTY(msg_ctx, env,
            AXIS2_TRANSPORT_OUT, property, AXIS2_FALSE);

    AXIS2_MSG_CTX_SET_SERVER_SIDE(msg_ctx, env, AXIS2_TRUE);
    if (AXIS2_SUCCESS != axis2_http_transport_utils_dispatch_and_verify(env,
            msg_ctx))
    {
        return AXIS2_FALSE;
    }
    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);
    if (op)
    {
        axis2_msg_t *msg = NULL;
        msg = AXIS2_OP_GET_MSG(op, env, AXIS2_MSG_IN);
        schema_element = AXIS2_MSG_GET_SCHEMA_ELEMENT(msg, env);
    }
    soap_envelope = axis2_http_transport_utils_handle_media_type_url_encoded(
                env, msg_ctx, request_params, AXIS2_HTTP_HEADER_GET,
                schema_element);
    if (NULL == soap_envelope)
    {
        return AXIS2_FALSE;
    }
    else
    {
        AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx, env, AXIS2_TRUE);
        AXIS2_MSG_CTX_SET_SOAP_ENVELOPE(msg_ctx, env, soap_envelope);
        engine = axis2_engine_create(env, conf_ctx);
        AXIS2_ENGINE_RECEIVE(engine, env, msg_ctx);
        return AXIS2_TRUE;
    }
    return AXIS2_FALSE;
}

AXIS2_EXTERN axiom_stax_builder_t *AXIS2_CALL
axis2_http_transport_utils_select_builder_for_mime(
    const axis2_env_t *env,
    axis2_char_t *request_uri,
    axis2_msg_ctx_t *msg_ctx,
    axis2_stream_t *in_stream,
    axis2_char_t *content_type)
{
    /*
         TODO implement when MTOM support is added
     */
    return NULL;
}

AXIS2_EXTERN axis2_bool_t  AXIS2_CALL
axis2_http_transport_utils_is_optimized(
    const axis2_env_t *env,
    axiom_element_t *om_element)
{
    /*
         TODO implement when MTOM support is added
     */
    return AXIS2_FALSE;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_do_write_mtom(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_property_t *property = NULL;
    axis2_param_t *param = NULL;
    axis2_char_t *value = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    param = AXIS2_MSG_CTX_GET_PARAMETER(msg_ctx, env, AXIS2_ENABLE_MTOM);
    if (param)
        value = AXIS2_PARAM_GET_VALUE(param, env);

    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env,
            AXIS2_ENABLE_MTOM, AXIS2_FALSE);
    if (property)
        value = (axis2_char_t *)AXIS2_PROPERTY_GET_VALUE(property, env);

    if (value)
    {
        return (AXIS2_STRCMP(value, AXIS2_VALUE_TRUE) == 0);
    }
    return AXIS2_FALSE;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_is_doing_rest(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}


AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_http_transport_utils_is_doing_rest_through_post(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    /*
        TODO implement when REST support is added
    */
    return AXIS2_FALSE;
}

AXIS2_EXTERN axis2_hash_t *AXIS2_CALL
axis2_http_transport_utils_get_request_params(
    const axis2_env_t *env,
    axis2_char_t *request_uri)
{


    axis2_char_t *query_str = NULL;
    axis2_char_t *tmp = strchr(request_uri, '?');
    axis2_char_t *tmp2 = NULL;
    axis2_char_t *tmp_name = NULL;
    axis2_char_t *tmp_value = NULL;
    axis2_hash_t *ret = NULL;

    AXIS2_PARAM_CHECK(env->error, request_uri, AXIS2_FAILURE);

    if (NULL == tmp || '\0' == *(tmp + 1))
    {
        return NULL;
    }
    query_str = AXIS2_STRDUP(tmp + 1, env);

    for (tmp2 = tmp = query_str; *tmp != '\0'; ++tmp)
    {
        if ('=' == *tmp)
        {
            *tmp = '\0';
            tmp_name = AXIS2_STRDUP(tmp2, env);
            axis2_http_transport_utils_strdecode(env, tmp_name, tmp_name);
            tmp2 = tmp + 1;
        }
        if ('&' == *tmp)
        {
            *tmp = '\0';
            tmp_value = AXIS2_STRDUP(tmp2, env);
            axis2_http_transport_utils_strdecode(env, tmp_value, tmp_value);
            tmp2 = tmp + 1;
        }
        if (tmp_name && NULL != tmp_value)
        {
            if (NULL == ret)
            {
                ret = axis2_hash_make(env);
            }
            axis2_hash_set(ret, tmp_name, AXIS2_HASH_KEY_STRING, tmp_value);
            tmp_name = NULL;
            tmp_value = NULL;
        }
    }
    if (tmp_name && '\0' != *tmp2)
    {
        if (NULL == ret)
        {
            ret = axis2_hash_make(env);
        }
        tmp_value = AXIS2_STRDUP(tmp2, env);
        axis2_http_transport_utils_strdecode(env, tmp_value, tmp_value);
        axis2_hash_set(ret, tmp_name, AXIS2_HASH_KEY_STRING, tmp_value);
    }

    return ret;
}


AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_strdecode(
    const axis2_env_t *env,
    axis2_char_t *dest,
    axis2_char_t *src)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, dest, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, src, AXIS2_FAILURE);

    for (; *src != '\0'; ++dest, ++src)
    {
        if (src[0] == '%' && isxdigit(src[1]) && isxdigit(src[2]))
        {
            *dest = axis2_http_transport_utils_hexit(src[1]) * 16 +
                    axis2_http_transport_utils_hexit(src[2]);
            src += 2;
        }
        else
        {
            *dest = *src;
        }
    }
    *dest = '\0';

    return AXIS2_SUCCESS;
}


AXIS2_EXTERN int  AXIS2_CALL
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
    return 0;           /* shouldn't happen, we're guarded by isxdigit() */
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_services_html(
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx)
{
    axis2_hash_t *services_map = NULL;
    axis2_hash_t *errorneous_svc_map = NULL;
    axis2_char_t *ret = NULL;
    axis2_char_t *tmp2 = (axis2_char_t *)"<h2>Deployed Services</h2>";
    axis2_hash_index_t *hi = NULL;
    axis2_bool_t svcs_exists = AXIS2_FALSE;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, conf_ctx, NULL);

    services_map = AXIS2_CONF_GET_ALL_SVCS(AXIS2_CONF_CTX_GET_CONF(conf_ctx, env),
            env);
    errorneous_svc_map = AXIS2_CONF_GET_ALL_FAULTY_SVCS(AXIS2_CONF_CTX_GET_CONF(
                conf_ctx, env), env);
    if (services_map && 0 != axis2_hash_count(services_map))
    {
        void *service = NULL;
        axis2_char_t *sname = NULL;
        axis2_hash_t *ops = NULL;
        svcs_exists = AXIS2_TRUE;

        for (hi = axis2_hash_first(services_map, env);
                hi; hi = axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, NULL, NULL, &service);
            sname = AXIS2_QNAME_GET_LOCALPART(AXIS2_SVC_GET_QNAME(
                        ((axis2_svc_t *)service), env), env);
            ret = AXIS2_STRACAT(tmp2, "<h3><u>", env);
            tmp2 = ret;
            ret = AXIS2_STRACAT(tmp2, sname, env);
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
            ret  = AXIS2_STRACAT(tmp2, "</u></h3>", env);
				tmp2 = ret;
				ret = AXIS2_STRACAT (tmp2, "<p>", env);
				tmp2 = ret;
							 /**
							  *setting services description */
				ret = AXIS2_STRACAT (tmp2, AXIS2_SVC_GET_SVC_DESC ((axis2_svc_t *)service, env), env);
				tmp2 = ret;
				ret = AXIS2_STRACAT (tmp2, "</p>", env);
				tmp2 = ret;
            ops = AXIS2_SVC_GET_ALL_OPS(((axis2_svc_t *)service), env);
            if (ops && 0 != axis2_hash_count(ops))
            {
                axis2_hash_index_t *hi2 = NULL;
                void *op = NULL;
                axis2_char_t *oname = NULL;

                ret = AXIS2_STRACAT(tmp2, "<i>Available Operations</i> <ul>",
                        env);
                AXIS2_FREE(env->allocator, tmp2);
                tmp2 = ret;
                for (hi2 = axis2_hash_first(ops, env);  hi2;
                        hi2 = axis2_hash_next(env, hi2))
                {
                    axis2_hash_this(hi2, NULL, NULL, &op);
                    oname = AXIS2_QNAME_GET_LOCALPART(AXIS2_OP_GET_QNAME(
                                ((axis2_op_t *)op), env), env);
                    ret = AXIS2_STRACAT(tmp2, "<li>", env);
                    AXIS2_FREE(env->allocator, tmp2);
                    tmp2 = ret;

                    ret = AXIS2_STRACAT(tmp2, oname, env);
                    AXIS2_FREE(env->allocator, tmp2);
                    tmp2 = ret;
                    ret = AXIS2_STRACAT(tmp2, "</li>", env);
                    AXIS2_FREE(env->allocator, tmp2);
                    tmp2 = ret;
                }
                ret = AXIS2_STRACAT(tmp2, "</ul>", env);
                AXIS2_FREE(env->allocator, tmp2);
                tmp2 = ret;
            }
            else
            {
                ret = AXIS2_STRACAT(tmp2, "No operations Available", env);
                /*AXIS2_FREE(env->allocator, tmp);*/
                tmp2 = ret;
            }
        }
    }
    if (errorneous_svc_map && 0 != axis2_hash_count(errorneous_svc_map))
    {
        void *fsname = NULL;
        svcs_exists = AXIS2_TRUE;
        ret = AXIS2_STRACAT(tmp2, "<hr><h2><font color=\"red\">Faulty \
                Services</font></h2>"
                , env);
        AXIS2_FREE(env->allocator, tmp2);
        tmp2 = ret;

        for (hi = axis2_hash_first(errorneous_svc_map, env);  hi;
                axis2_hash_next(env, hi))
        {
            axis2_hash_this(hi, (const void **)&fsname, NULL, NULL);
            ret = AXIS2_STRACAT(tmp2, "<h3><font color=\"red\">", env);
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
            ret = AXIS2_STRACAT(tmp2, (axis2_char_t *)fsname, env);
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
            ret = AXIS2_STRACAT(tmp2, "</font></h3>", env);
            AXIS2_FREE(env->allocator, tmp2);
            tmp2 = ret;
        }
    }
    if (AXIS2_FALSE == svcs_exists)
    {
        ret = AXIS2_STRDUP("<h2>There are no services deployed</h2>", env);
    }
    ret = AXIS2_STRACAT("<html><head><title>Axis2C :: Services</title></head>"
            "<body><font face=\"courier\">"
            , tmp2, env);
    /*AXIS2_FREE(env->allocator, tmp2);*/
    tmp2 = ret;
    ret = AXIS2_STRACAT(tmp2, "</font></body></html>\r\n", env);
    /*AXIS2_FREE(env->allocator, tmp);*/

    return ret;
}

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_charset_enc(
    const axis2_env_t *env,
    const axis2_char_t *content_type)
{
    axis2_char_t *tmp = NULL;
    axis2_char_t *tmp_content_type = NULL;
    axis2_char_t *tmp2 = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, content_type, NULL);

    tmp_content_type = AXIS2_STRDUP(content_type, env);
    if (NULL == tmp_content_type)
    {
        return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
    }
    tmp = strstr(tmp_content_type, AXIS2_HTTP_CHAR_SET_ENCODING);
    if (NULL == tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
    }
    tmp = strchr(tmp, '=');
    tmp2 = strchr(tmp, ';');
    if (tmp2)
    {
        if ('\'' == *(tmp2 - sizeof(axis2_char_t)) ||
                '\"' == *(tmp2 - sizeof(axis2_char_t)))
        {
            tmp2 -= sizeof(axis2_char_t);
        }
        *tmp2 = '\0';
    }
    if (NULL == tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return AXIS2_STRDUP(AXIS2_HTTP_HEADER_DEFAULT_CHAR_ENCODING, env);
    }
    /* Following formats are acceptable
     * charset="UTF-8"
     * charser='UTF-8'
     * charset=UTF-8
     * But for our requirements charset we get should be UTF-8
     */
    if ('\'' == *(tmp + sizeof(axis2_char_t)) || '\"' == *(tmp +
            sizeof(axis2_char_t)))
    {
        tmp += 2 * sizeof(axis2_char_t);
    }
    else
    {
        tmp += sizeof(axis2_char_t);
    }
    tmp2 =  AXIS2_STRDUP(tmp, env);
    AXIS2_FREE(env->allocator, tmp_content_type);
    return tmp2;
}

int AXIS2_CALL
axis2_http_transport_utils_on_data_request(
    char *buffer,
    int size,
    void *ctx)
{
    const axis2_env_t *env = NULL;
    int len = -1;
    axis2_callback_info_t *cb_ctx = (axis2_callback_info_t *)ctx;

    if (NULL == buffer || NULL == ctx)
    {
        return 0;
    }
    env = ((axis2_callback_info_t *)ctx)->env;
    if (cb_ctx->unread_len <= 0 && -1 != cb_ctx->content_length)
    {
        return 0;
    }
    if (cb_ctx->chunked_stream)
    {
        len = AXIS2_HTTP_CHUNKED_STREAM_READ(cb_ctx->chunked_stream, env,
                buffer, size);
        buffer[len] = '\0';
        return len;
    }
    else
    {
        axis2_stream_t *in_stream = NULL;
        int read_len = size;
        in_stream = (axis2_stream_t *)((axis2_callback_info_t *)ctx)->in_stream;
        /* For managed streams such as Apache2 streams we do not need to
         * calculate lenghts
         */
        /*if(AXIS2_STREAM_MANAGED == AXIS2_STREAM_GET_TYPE(in_stream, env))
        {
            read_len = size;
        }
        else
        {
            if(size > ((axis2_callback_info_t *)ctx)->unread_len &&
                    -1 != ((axis2_callback_info_t *)ctx)->unread_len)
            {
                read_len = ((axis2_callback_info_t *)ctx)->unread_len;
            }
            else
            {
                read_len = size;
            }
        }*/
        len = AXIS2_STREAM_READ(in_stream, env, buffer, read_len);
        if (len > 0)
        {
            buffer[len] = '\0';
            /*if(AXIS2_STREAM_MANAGED != AXIS2_STREAM_GET_TYPE(in_stream, env))
            {*/
            ((axis2_callback_info_t *)ctx)->unread_len -= len;
            /*}*/
        }
        return len;
    }
    return 0;
}

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_create_soap_msg(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    const axis2_char_t *soap_ns_uri)
{
    axis2_op_ctx_t *op_ctx = NULL;
    const axis2_char_t *char_set_enc = NULL;
    axis2_char_t *content_type = NULL;
    axis2_stream_t *in_stream = NULL;
    axis2_callback_info_t *callback_ctx = NULL;
    axis2_char_t *trans_enc = NULL;
    int *content_length = NULL;
    axis2_property_t *property = NULL;
    axis2_hash_t *binary_data_map = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, NULL);
    AXIS2_PARAM_CHECK(env->error, soap_ns_uri, NULL);


    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env,
            AXIS2_TRANSPORT_IN, AXIS2_FALSE);
    if (property)
    {
        in_stream = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
    callback_ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_callback_info_t));
    /* Note: the memory created above is freed in xml reader free function
       as this is passed on to the reader */
    if (NULL == callback_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    callback_ctx->in_stream = in_stream;
    callback_ctx->env = env;
    callback_ctx->content_length = -1;
    callback_ctx->unread_len = -1;
    callback_ctx->chunked_stream = NULL;

    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env,
            AXIS2_HTTP_HEADER_CONTENT_LENGTH, AXIS2_FALSE);
    if (property)
    {
        content_length = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
    if (content_length)
    {
        callback_ctx->content_length = *content_length;
        callback_ctx->unread_len = *content_length;
    }
    if (NULL == in_stream)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_IN_STREAM_IN_MSG_CTX,
                AXIS2_FAILURE);
        AXIS2_FREE(env->allocator, callback_ctx);
        return NULL;
    }
    property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env,
            AXIS2_HTTP_HEADER_TRANSFER_ENCODING, AXIS2_FALSE);
    if (property)
    {
        trans_enc = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }
    if (trans_enc && 0 == AXIS2_STRCMP(trans_enc,
            AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
    {
        callback_ctx->chunked_stream = axis2_http_chunked_stream_create(env,
                in_stream);
        if (NULL == callback_ctx->chunked_stream)
        {
            return NULL;
        }
    }


    op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
    if (op_ctx)
    {
        axis2_ctx_t *ctx = AXIS2_OP_CTX_GET_BASE(op_ctx, env);
        if (ctx)
        {
            property = AXIS2_CTX_GET_PROPERTY(ctx, env,
                    AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);
            if (property)
            {
                char_set_enc = AXIS2_PROPERTY_GET_VALUE(property, env);
                property = NULL;
            }
            property = AXIS2_CTX_GET_PROPERTY(ctx, env,
                    MTOM_RECIVED_CONTENT_TYPE, AXIS2_FALSE);
            if (property)
            {
                content_type = AXIS2_PROPERTY_GET_VALUE(property, env);
                property = NULL;
            }

        }
    }
    if (NULL == char_set_enc)
    {
        char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
    }
    if (content_type)
    {
        axis2_char_t *mime_boundary = NULL;
        AXIS2_MSG_CTX_SET_DOING_MTOM(msg_ctx, env, AXIS2_TRUE);
        /* get mime boundry */
        mime_boundary = axis2_http_transport_utils_get_value_from_content_type(env, content_type, AXIS2_HTTP_HEADER_CONTENT_TYPE_MIME_BOUNDARY);

        if (mime_boundary)
        {
            axiom_mime_parser_t *mime_parser = NULL;
            axis2_stream_t *stream = NULL;
            int soap_body_len = 0;
            axis2_char_t *soap_body_str = NULL;

            mime_parser = axiom_mime_parser_create(env);
            if (mime_parser)
            {
                binary_data_map = AXIOM_MIME_PARSER_PARSE(mime_parser, env,
                        axis2_http_transport_utils_on_data_request,
                        (void *)callback_ctx, mime_boundary);

                soap_body_len = AXIOM_MIME_PARSER_GET_SOAP_BODY_LENGTH(
                            mime_parser, env);
                soap_body_str = AXIOM_MIME_PARSER_GET_SOAP_BODY_STR(
                            mime_parser, env);
            }

            stream = axis2_stream_create_basic(env);
            if (stream)
            {
                AXIS2_STREAM_WRITE(stream, env, soap_body_str, soap_body_len);
                callback_ctx->in_stream = stream;
                callback_ctx->chunked_stream = NULL;
                callback_ctx->content_length = soap_body_len;
                callback_ctx->unread_len = soap_body_len;
            }

            AXIOM_MIME_PARSER_FREE(mime_parser, env);
            mime_parser = NULL;
        }
    }

    if (AXIS2_TRUE != AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env))
    {
        axiom_xml_reader_t *xml_reader = NULL;
        axiom_stax_builder_t *om_builder = NULL;
        axiom_soap_builder_t *soap_builder = NULL;
        axiom_soap_envelope_t *soap_envelope = NULL;

        xml_reader = axiom_xml_reader_create_for_io(env,
                axis2_http_transport_utils_on_data_request, NULL,
                (void *)callback_ctx, char_set_enc);
        if (NULL == xml_reader)
        {
            return NULL;
        }
        om_builder = axiom_stax_builder_create(env, xml_reader);
        if (NULL == om_builder)
        {
            AXIOM_XML_READER_FREE(xml_reader, env);
            xml_reader = NULL;
            return NULL;
        }
        soap_builder = axiom_soap_builder_create(env, om_builder,
                soap_ns_uri);
        if (NULL == soap_builder)
        {
            /* We should not be freeing om_builder here as it is done by
               axiom_soap_builder_create in case of error - Samisa*/
            /*AXIOM_STAX_BUILDER_FREE(om_builder, env);*/
            om_builder = NULL;
            xml_reader = NULL;
            return NULL;
        }

        soap_envelope = AXIOM_SOAP_BUILDER_GET_SOAP_ENVELOPE(soap_builder, env);

        if (binary_data_map)
        {
            AXIOM_SOAP_BUILDER_SET_MIME_BODY_PARTS(soap_builder, env,
                    binary_data_map);
        }

        if (soap_envelope && content_type)
        {
            /* hack to get around MTOM problem */
            axiom_soap_body_t *soap_body =
                AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);

            if (soap_body)
            {
                AXIOM_SOAP_BODY_HAS_FAULT(soap_body, env);
            }
        }

        return soap_envelope;
    }
    else
    {
        axiom_xml_reader_t *xml_reader = NULL;
        axiom_stax_builder_t *om_builder = NULL;
        axiom_soap_envelope_t *soap_envelope = NULL;
        axiom_soap_body_t *def_body = NULL;
        axiom_document_t *om_doc = NULL;
        axiom_node_t *root_node = NULL;

        xml_reader = axiom_xml_reader_create_for_io(env,
                axis2_http_transport_utils_on_data_request, NULL,
                (void *)callback_ctx, char_set_enc);
        if (NULL == xml_reader)
        {
            return NULL;
        }
        om_builder = axiom_stax_builder_create(env, xml_reader);
        if (NULL == om_builder)
        {
            AXIOM_XML_READER_FREE(xml_reader, env);
            xml_reader = NULL;
            return NULL;
        }
        soap_envelope = axiom_soap_envelope_create_default_soap_envelope
                (env, AXIOM_SOAP11);
        def_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_envelope, env);
        om_doc = AXIOM_STAX_BUILDER_GET_DOCUMENT(om_builder, env);
        root_node = AXIOM_DOCUMENT_BUILD_ALL(om_doc, env);
        AXIOM_SOAP_BODY_ADD_CHILD(def_body, env, root_node);

        return soap_envelope;
    }
    return NULL;
}


AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_http_transport_utils_get_value_from_content_type(
    const axis2_env_t *env,
    const axis2_char_t *content_type,
    const axis2_char_t *key)
{
    axis2_char_t *tmp = NULL;
    axis2_char_t *tmp_content_type = NULL;
    axis2_char_t *tmp2 = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, content_type, NULL);
    AXIS2_PARAM_CHECK(env->error, key, NULL);

    tmp_content_type = AXIS2_STRDUP(content_type, env);
    if (NULL == tmp_content_type)
    {
        return NULL;
    }
    tmp = strstr(tmp_content_type, key);
    if (NULL == tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return NULL;
    }

    tmp = strchr(tmp, '=');
    tmp2 = strchr(tmp, ';');

    if (tmp2)
    {
        *tmp2 = '\0';
    }
    if (NULL == tmp)
    {
        AXIS2_FREE(env->allocator, tmp_content_type);
        return NULL;
    }
    tmp2 =  AXIS2_STRDUP(tmp + 1, env);
    
    AXIS2_FREE(env->allocator, tmp_content_type);
    if (*tmp2 == '"')
    {
        tmp = tmp2;
        tmp2 =  AXIS2_STRDUP(tmp + 1, env);
        tmp2[strlen(tmp2) - 1] = '\0';
    }
    return tmp2;
}

AXIS2_EXTERN axiom_soap_envelope_t *AXIS2_CALL
axis2_http_transport_utils_handle_media_type_url_encoded(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_hash_t *param_map,
    axis2_char_t *method,
    xml_schema_element_t *schema_element)
{
    axiom_soap_envelope_t *soap_env = NULL;
    axiom_soap_body_t *soap_body = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, NULL);
    AXIS2_PARAM_CHECK(env->error, method, NULL);


    soap_env = axiom_soap_envelope_create_default_soap_envelope(env,
            AXIOM_SOAP11);
    soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(soap_env, env);
    if (NULL == soap_body)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                AXIS2_FAILURE);
        return NULL;
    }
    if (NULL == schema_element)
    {
        axiom_element_t *body_child = NULL;
        axiom_node_t *body_child_node = NULL;

        body_child = axiom_element_create_with_qname(env, NULL,
                AXIS2_OP_GET_QNAME(AXIS2_MSG_CTX_GET_OP(msg_ctx, env),
                        env), &body_child_node);
        AXIOM_SOAP_BODY_ADD_CHILD(soap_body, env, body_child_node);
        if (param_map)
        {
            axis2_hash_index_t *hi = NULL;
            for (hi = axis2_hash_first(param_map, env); hi ;
                    hi = axis2_hash_next(env, hi))
            {
                void *name = NULL;
                void *value = NULL;
                axiom_node_t *node = NULL;
                axiom_element_t *element = NULL;

                axis2_hash_this(hi, (const void **)&name, NULL, (void **)&value);
                element = axiom_element_create(env, NULL, (axis2_char_t *)name,
                        NULL, &node);
                AXIOM_ELEMENT_SET_TEXT(element, env, (axis2_char_t *)value, node);
                AXIOM_NODE_ADD_CHILD(body_child_node, env, node);
            }
        }
    }
    else
    {
        axis2_char_t *target_ns = NULL;
        axis2_qname_t *bfc_qname = NULL;
        axiom_element_t *body_child = NULL;
        axiom_node_t *body_child_node = NULL;

        target_ns = AXIS2_QNAME_GET_URI(XML_SCHEMA_ELEMENT_GET_QNAME(
                    schema_element, env), env);
        bfc_qname = axis2_qname_create(env, XML_SCHEMA_ELEMENT_GET_NAME(
                    schema_element, env), target_ns, NULL);

        body_child = axiom_element_create_with_qname(env, NULL, bfc_qname,
                &body_child_node);
        AXIOM_SOAP_BODY_ADD_CHILD(soap_body, env, body_child_node);
        if (0 == AXIS2_STRCMP(method, AXIS2_HTTP_HEADER_GET) ||
                0 == AXIS2_STRCMP(method, AXIS2_HTTP_HEADER_POST))
        {
            xml_schema_type_t *schema_type = NULL;
            schema_type = XML_SCHEMA_ELEMENT_GET_SCHEMA_TYPE(schema_element,
                    env);
            if (XML_SCHEMA_COMPLEX_TYPE == XML_SCHEMA_TYPE_GET_TYPE(schema_type,
                    env))
            {
                xml_schema_particle_t *schema_particle = NULL;
                schema_particle = XML_SCHEMA_COMPLEX_TYPE_GET_PARTICLE(
                            schema_type, env);
                if (XML_SCHEMA_SEQUENCE ==
                        XML_SCHEMA_PARTICLE_GET_TYPE(schema_particle, env))
                {
                    xml_schema_group_base_t *sequence = NULL;
                    xml_schema_obj_collection_t *seq_items = NULL;
                    int count = 0;
                    int i = 0;

                    sequence = (xml_schema_group_base_t *)schema_particle;
                    seq_items = XML_SCHEMA_GROUP_BASE_GET_ITEMS(sequence, env);
                    count = XML_SCHEMA_OBJ_COLLECTION_GET_COUNT(seq_items, env);
                    for (i = 0; i < count; i++)
                    {
                        xml_schema_element_t *inner_element = NULL;
                        axis2_char_t *element_name = NULL;
                        axis2_char_t *param_val = NULL;
                        axiom_element_t *om_element = NULL;
                        axiom_node_t *om_node = NULL;

                        inner_element = (xml_schema_element_t *)
                                XML_SCHEMA_OBJ_COLLECTION_GET_ITEM(seq_items, env,
                                        i);
                        element_name = XML_SCHEMA_ELEMENT_GET_NAME(inner_element,
                                env);
                        param_val = axis2_hash_get(param_map, element_name,
                                AXIS2_HASH_KEY_STRING);
                        if (NULL == param_val)
                        {
                            AXIS2_ERROR_SET(env->error,
                                    AXIS2_ERROR_REQD_PARAM_MISSING, AXIS2_FAILURE);
                            return NULL;
                        }
                        om_element = axiom_element_create(env, body_child_node,
                                element_name, NULL, &om_node);
                        AXIOM_ELEMENT_SET_TEXT(om_element, env, param_val,
                                om_node);
                    }
                }
            }
            else
            {
                AXIS2_ERROR_SET(env->error, AXIS2_ERROR_UNSUPPORTED_SCHEMA_TYPE,
                        AXIS2_FAILURE);
                return NULL;
            }
        }
    }
    return soap_env;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_http_transport_utils_dispatch_and_verify(
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    axis2_disp_t *req_uri_disp = NULL;
    axis2_handler_t *handler = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    req_uri_disp = axis2_req_uri_disp_create(env);
    handler = AXIS2_DISP_GET_BASE(req_uri_disp, env);
    AXIS2_HANDLER_INVOKE(handler, env, msg_ctx);

    if (NULL == AXIS2_MSG_CTX_GET_SVC(msg_ctx, env) || NULL ==
            AXIS2_MSG_CTX_GET_OP(msg_ctx, env))
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_SVC_OR_OP_NOT_FOUND,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }
    return AXIS2_SUCCESS;
}
