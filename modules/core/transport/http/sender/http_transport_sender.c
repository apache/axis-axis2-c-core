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

#include <axis2_http_transport_sender.h>
#include <axis2_string.h>
#include <axis2_endpoint_ref.h>
#include <axis2_addr.h>
#include <axiom_xml_writer.h>
#include <axiom_output.h>
#include <axis2_http_transport_utils.h>
#include <axis2_http_out_transport_info.h>
#include <axis2_http_transport.h>
#include <axis2_soap_over_http_sender.h>
#include <axiom_soap_body.h>
#include <axis2_rest_sender.h>
#include <axis2_types.h>


/**
 * HTTP Transport Sender struct impl
 * Axis2 HTTP Transport Sender impl
 */

typedef struct axis2_http_transport_sender_impl
{
    axis2_transport_sender_t transport_sender;
    axis2_char_t *http_version;
    axis2_bool_t chunked;
    int connection_timeout;
    int so_timeout;

}
axis2_http_transport_sender_impl_t;

#define AXIS2_INTF_TO_IMPL(transport_sender) \
                         ((axis2_http_transport_sender_impl_t *)\
                     (transport_sender))

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL
axis2_http_transport_sender_invoke(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL
axis2_http_transport_sender_clean_up(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx);

axis2_status_t AXIS2_CALL
axis2_http_transport_sender_init(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    axis2_transport_out_desc_t *out_desc);

axis2_status_t AXIS2_CALL
axis2_http_transport_sender_write_message(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_endpoint_ref_t *epr,
    axiom_soap_envelope_t *out,
    axiom_output_t *om_output);

axis2_status_t AXIS2_CALL
axis2_http_transport_sender_free(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env);

/***************************** End of function headers ************************/

axis2_transport_sender_t *AXIS2_CALL
axis2_http_transport_sender_create(
    const axis2_env_t *env)
{
    axis2_http_transport_sender_impl_t *transport_sender_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    transport_sender_impl = (axis2_http_transport_sender_impl_t *)AXIS2_MALLOC
            (env->allocator, sizeof(
                        axis2_http_transport_sender_impl_t));

    if (NULL == transport_sender_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    transport_sender_impl->http_version = AXIS2_STRDUP(
                AXIS2_HTTP_HEADER_PROTOCOL_11, env);
    transport_sender_impl->chunked = AXIS2_TRUE;
    transport_sender_impl->connection_timeout =
        AXIS2_HTTP_DEFAULT_CONNECTION_TIMEOUT;
    transport_sender_impl->so_timeout = AXIS2_HTTP_DEFAULT_SO_TIMEOUT;
    transport_sender_impl->transport_sender.ops = AXIS2_MALLOC(env->allocator
            , sizeof(axis2_transport_sender_ops_t));
    if (NULL == transport_sender_impl->transport_sender.ops)
    {
        axis2_http_transport_sender_free((axis2_transport_sender_t *)
                transport_sender_impl, env);
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    transport_sender_impl->transport_sender.ops->invoke =
        axis2_http_transport_sender_invoke;
    transport_sender_impl->transport_sender.ops->cleanup =
        axis2_http_transport_sender_clean_up;
    transport_sender_impl->transport_sender.ops->init =
        axis2_http_transport_sender_init;
    transport_sender_impl->transport_sender.ops->free =
        axis2_http_transport_sender_free;

    return &(transport_sender_impl->transport_sender);
}


axis2_status_t AXIS2_CALL
axis2_http_transport_sender_free(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env)
{
    axis2_http_transport_sender_impl_t *transport_sender_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    transport_sender_impl = AXIS2_INTF_TO_IMPL(transport_sender);

    if (transport_sender_impl->http_version)
    {
        AXIS2_FREE(env->allocator, transport_sender_impl->http_version);
        transport_sender_impl->http_version = NULL;
    }

    if (transport_sender->ops)
        AXIS2_FREE(env->allocator, transport_sender->ops);

    AXIS2_FREE(env->allocator, transport_sender_impl);
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_http_transport_sender_invoke(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    const axis2_char_t *char_set_enc = NULL;
    axis2_string_t *char_set_enc_str = NULL;
    axis2_endpoint_ref_t *epr = NULL;
    axis2_char_t *transport_url = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;
    axiom_soap_envelope_t *soap_data_out = NULL;
    axis2_bool_t do_mtom;
    axis2_property_t *property = NULL;
    axiom_node_t *data_out = NULL;
    axis2_byte_t *output_stream = NULL;
    int buffer_size = 0;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);

    /*property = AXIS2_MSG_CTX_GET_PROPERTY(msg_ctx, env,
            AXIS2_CHARACTER_SET_ENCODING, AXIS2_FALSE);
    if (property)
    {
        char_set_enc = AXIS2_PROPERTY_GET_VALUE(property, env);
        property = NULL;
    }*/
    char_set_enc_str = axis2_msg_ctx_get_charset_encoding(msg_ctx, env);
    if (char_set_enc_str)
    {
        char_set_enc = axis2_string_get_buffer(char_set_enc_str, env);
    }
    
    if (!char_set_enc)
    {
        axis2_op_ctx_t *op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
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
            }
        }
    }
    /**
     * If we still can't find the char set enc we will
     * use default
     */
    if (NULL == char_set_enc)
    {
        char_set_enc = AXIS2_DEFAULT_CHAR_SET_ENCODING;
    }

    do_mtom = axis2_http_transport_utils_do_write_mtom(env,
            msg_ctx);
    AXIS2_MSG_CTX_SET_DOING_MTOM(msg_ctx, env, do_mtom);
    /*do_mtom = AXIS2_MSG_CTX_GET_DOING_MTOM(msg_ctx, env);*/
    /*AXIS2_MSG_CTX_SET_DOING_REST(msg_ctx,
                      env, axis2_http_transport_utils_is_doing_rest(env, 
                      msg_ctx));*/

    transport_url = axis2_msg_ctx_get_transport_url(msg_ctx, env);
    if (transport_url)
    {
        epr = axis2_endpoint_ref_create(env, transport_url);
    }
    else
    {
        axis2_endpoint_ref_t *ctx_epr = AXIS2_MSG_CTX_GET_TO(msg_ctx, env);
        if (ctx_epr && 0 != AXIS2_STRCMP(
                    AXIS2_WSA_ANONYMOUS_URL_SUBMISSION,
                    AXIS2_ENDPOINT_REF_GET_ADDRESS(ctx_epr, env)) &&
                0 != AXIS2_STRCMP(AXIS2_WSA_ANONYMOUS_URL,
                        AXIS2_ENDPOINT_REF_GET_ADDRESS(ctx_epr, env)))
        {
            epr = ctx_epr;
        }
    }

    soap_data_out = AXIS2_MSG_CTX_GET_SOAP_ENVELOPE(msg_ctx, env);
    if (NULL == soap_data_out)
    {
        AXIS2_ERROR_SET(env->error,
                AXIS2_ERROR_NULL_SOAP_ENVELOPE_IN_MSG_CTX,
                AXIS2_FAILURE);
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s",
                AXIS2_ERROR_GET_MESSAGE(env->error));
        return AXIS2_FAILURE;
    }
    xml_writer = axiom_xml_writer_create_for_memory(env, NULL,
            AXIS2_TRUE, 0, AXIS2_XML_PARSER_TYPE_BUFFER);
    if (NULL == xml_writer)
    {
        return AXIS2_FAILURE;
    }
    om_output = axiom_output_create(env, xml_writer);
    if (NULL == om_output)
    {
        AXIOM_XML_WRITER_FREE(xml_writer, env);
        xml_writer = NULL;
        return AXIS2_FAILURE;
    }

    AXIOM_OUTPUT_SET_SOAP11(om_output, env, AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env));
    if (epr)
    {
        if (AXIS2_STRCMP(AXIS2_WSA_NONE_URL_SUBMISSION, AXIS2_ENDPOINT_REF_GET_ADDRESS(epr, env)) == 0 ||
                AXIS2_STRCMP(AXIS2_WSA_NONE_URL, AXIS2_ENDPOINT_REF_GET_ADDRESS(epr, env)) == 0)
        {
            epr = NULL;
        }
        else
        {
            axis2_http_transport_sender_write_message(transport_sender, env, msg_ctx
                    , epr, soap_data_out, om_output);
        }
    }

    if (!epr)
    {
        axis2_stream_t *out_stream = axis2_msg_ctx_get_transport_out_stream(msg_ctx, env);
        
        if (AXIS2_TRUE == AXIS2_MSG_CTX_GET_SERVER_SIDE(msg_ctx, env))
        {
            axis2_http_out_transport_info_t *out_info = NULL;
            axis2_bool_t is_soap11 = AXIS2_FALSE;
            axis2_op_ctx_t *op_ctx = NULL; 

            out_info = axis2_msg_ctx_get_http_out_transport_info(msg_ctx, env);

            if (NULL == out_info)
            {
                AXIS2_ERROR_SET(env->error,
                        AXIS2_ERROR_OUT_TRNSPORT_INFO_NULL, AXIS2_FAILURE);
                AXIOM_OUTPUT_FREE(om_output, env);
                om_output = NULL;
                xml_writer = NULL;
                return AXIS2_FAILURE;
            }
            is_soap11 = AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env);
            /* AXIOM_OUTPUT_SET_SOAP11(om_output, env, is_soap_11);
             */
            AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CHAR_ENCODING(out_info, env,
                    char_set_enc);
            if (AXIS2_TRUE == is_soap11)
            {
                AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CONTENT_TYPE(out_info, env,
                        AXIS2_HTTP_HEADER_ACCEPT_TEXT_XML);
            }
            else
            {
                AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CONTENT_TYPE(out_info, env,
                        AXIS2_HTTP_HEADER_ACCEPT_APPL_SOAP);
            }
            /*
            AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CONTENT_TYPE(out_info, env, 
                     AXIOM_OUTPUT_GET_CONTENT_TYPE(om_output, env));*/
            /* AXIOM_OUTPUT_SET_DO_OPTIMIZE(om_output, env,
             *            AXIS2_MSG_CTX_GET_IS_DOING_MTOM(msg_ctx, env);
             */
		
            if (AXIS2_TRUE == AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env))
            {
                axiom_node_t *body_node = NULL;
                axiom_soap_body_t *soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(
                            soap_data_out, env);

                if (NULL == soap_body)
                {
                    AXIS2_ERROR_SET(env->error,
                            AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                            AXIS2_FAILURE);
                    AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s",
                            AXIS2_ERROR_GET_MESSAGE(env->error));
                    AXIOM_OUTPUT_FREE(om_output, env);
                    om_output = NULL;
                    xml_writer = NULL;
                    return AXIS2_FAILURE;
                }
                body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
                if (NULL == body_node)
                {
                    AXIOM_OUTPUT_FREE(om_output, env);
                    om_output = NULL;
                    xml_writer = NULL;
                    return AXIS2_FAILURE;
                }
                data_out = AXIOM_NODE_GET_FIRST_ELEMENT(body_node, env);
                if (NULL == data_out || AXIOM_NODE_GET_NODE_TYPE(data_out, env)
                        != AXIOM_ELEMENT)
                {
                    AXIOM_OUTPUT_FREE(om_output, env);
                    om_output = NULL;
                    xml_writer = NULL;
                    return AXIS2_FAILURE;
                }
                AXIOM_NODE_SERIALIZE(data_out, env, om_output);
                buffer = (axis2_char_t*)AXIOM_XML_WRITER_GET_XML(xml_writer, env);
                buffer_size = AXIOM_XML_WRITER_GET_XML_SIZE(xml_writer, env);
            }
            else
            {
                AXIOM_OUTPUT_SET_DO_OPTIMIZE(om_output, env,
                        do_mtom);
                AXIOM_SOAP_ENVELOPE_SERIALIZE(soap_data_out, env, om_output,
                        AXIS2_FALSE);
                if (do_mtom)
                {
                    axis2_char_t *content_type = NULL;
                    AXIOM_OUTPUT_FLUSH(om_output, env, &output_stream,
                            &buffer_size);
                    content_type = (axis2_char_t *)AXIOM_OUTPUT_GET_CONTENT_TYPE(
                                om_output,
                                env);
                    AXIS2_HTTP_OUT_TRANSPORT_INFO_SET_CONTENT_TYPE(out_info,
                            env, content_type);
                    buffer = output_stream;
                }
                else
                {
                    buffer = (axis2_char_t *)AXIOM_XML_WRITER_GET_XML(xml_writer, env);
                    buffer_size = AXIOM_XML_WRITER_GET_XML_SIZE(xml_writer, env);
                }
            }
            AXIS2_STREAM_WRITE(out_stream, env, buffer, buffer_size);
            /*AXIS2_FREE(env->allocator, buffer);*/

            op_ctx = AXIS2_MSG_CTX_GET_OP_CTX(msg_ctx, env);
            axis2_op_ctx_set_response_written(op_ctx, env, AXIS2_TRUE);
        }
    }

	
    AXIOM_OUTPUT_FREE(om_output, env);
    om_output = NULL;
    xml_writer = NULL;

    if (transport_url)
    {
        if (epr)
        {
            AXIS2_ENDPOINT_REF_FREE(epr, env);
            epr = NULL;
        }
    }
    /*
     * TODO handle errors
     */
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_http_transport_sender_clean_up(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    /*
     * Clean up is not used. If the http sender needs
     * to be cleaned up it should be done here.
     */
    return AXIS2_SUCCESS;
}


axis2_status_t AXIS2_CALL
axis2_http_transport_sender_init(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_conf_ctx_t *conf_ctx,
    axis2_transport_out_desc_t *out_desc)
{
    axis2_param_t *version_param = NULL;
    axis2_char_t *version = NULL;
    axis2_char_t *temp = NULL;
    axis2_param_t *temp_param = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_desc, AXIS2_FAILURE);

    version_param = AXIS2_PARAM_CONTAINER_GET_PARAM(
                out_desc->param_container, env,
                AXIS2_HTTP_PROTOCOL_VERSION);
    if (version_param)
    {
        version = AXIS2_PARAM_GET_VALUE(version_param, env);
    }
    if (version)
    {
        if (0 == AXIS2_STRCMP(version, AXIS2_HTTP_HEADER_PROTOCOL_11))
        {
            axis2_char_t *encoding = NULL;
            axis2_param_t *encoding_param = NULL;
            if (AXIS2_INTF_TO_IMPL(transport_sender)->http_version)
            {
                AXIS2_FREE(env->allocator,
                        AXIS2_INTF_TO_IMPL(transport_sender)->http_version);
            }
            AXIS2_INTF_TO_IMPL(transport_sender)->http_version = AXIS2_STRDUP(
                        version, env);
            encoding_param = AXIS2_PARAM_CONTAINER_GET_PARAM(
                        out_desc->param_container, env,
                        AXIS2_HTTP_HEADER_TRANSFER_ENCODING);
            if (encoding_param)
            {
                encoding = AXIS2_PARAM_GET_VALUE(encoding_param, env);
            }
            if (encoding && 0 == AXIS2_STRCMP(encoding,
                    AXIS2_HTTP_HEADER_TRANSFER_ENCODING_CHUNKED))
            {
                AXIS2_INTF_TO_IMPL(transport_sender)->chunked = AXIS2_TRUE;
            }
            else
            {
                AXIS2_INTF_TO_IMPL(transport_sender)->chunked = AXIS2_FALSE;
            }
        }
        else if (0 == AXIS2_STRCMP(version, AXIS2_HTTP_HEADER_PROTOCOL_10))
        {
            if (AXIS2_INTF_TO_IMPL(transport_sender)->http_version)
            {
                AXIS2_FREE(env->allocator,
                        AXIS2_INTF_TO_IMPL(transport_sender)->http_version);
            }
            AXIS2_INTF_TO_IMPL(transport_sender)->http_version = AXIS2_STRDUP(
                        version, env);
            AXIS2_INTF_TO_IMPL(transport_sender)->chunked = AXIS2_FALSE;
        }
    }
    else
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NULL_HTTP_VERSION,
                AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }


    temp_param = AXIS2_PARAM_CONTAINER_GET_PARAM(
                out_desc->param_container, env,
                AXIS2_HTTP_SO_TIMEOUT);
    if (temp_param)
    {
        temp = AXIS2_PARAM_GET_VALUE(temp_param, env);
    }
    if (temp)
    {
        AXIS2_INTF_TO_IMPL(transport_sender)->so_timeout = AXIS2_ATOI(temp);
    }
    temp = (axis2_char_t *)AXIS2_PARAM_CONTAINER_GET_PARAM(
                out_desc->param_container, env,
                AXIS2_HTTP_CONNECTION_TIMEOUT);
    if (temp_param)
    {
        temp = AXIS2_PARAM_GET_VALUE(temp_param, env);
    }
    if (temp)
    {
        AXIS2_INTF_TO_IMPL(transport_sender)->connection_timeout =
            AXIS2_ATOI(temp);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_http_transport_sender_write_message(
    axis2_transport_sender_t *transport_sender,
    const axis2_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_endpoint_ref_t *epr,
    axiom_soap_envelope_t *out,
    axiom_output_t *om_output)
{
    const axis2_char_t *soap_action = NULL;
    const axis2_char_t *url = NULL;
    axis2_soap_over_http_sender_t *sender = NULL;
    axis2_status_t status = AXIS2_FAILURE;
	const axis2_char_t *soap_ns_uri = NULL;
	axiom_soap_envelope_t *response_envelope = NULL;
	axis2_op_t *op = NULL;

    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, epr, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, om_output, AXIS2_FAILURE);

    url = AXIS2_ENDPOINT_REF_GET_ADDRESS(epr, env);
    
    soap_action = axis2_string_get_buffer(AXIS2_MSG_CTX_GET_SOAP_ACTION(msg_ctx, env), env);
    
    if (NULL == soap_action)
    {
        soap_action = "";
    }
    
    if (AXIS2_TRUE == AXIS2_MSG_CTX_GET_DOING_REST(msg_ctx, env))
    {
        axiom_node_t *data_out = NULL;
        axiom_node_t *body_node = NULL;
        axiom_soap_body_t *soap_body = AXIOM_SOAP_ENVELOPE_GET_BODY(out, env);
        axis2_rest_sender_t *sender = NULL;

        if (NULL == soap_body)
        {
            AXIS2_ERROR_SET(env->error,
                    AXIS2_ERROR_SOAP_ENVELOPE_OR_SOAP_BODY_NULL,
                    AXIS2_FAILURE);
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s",
                    AXIS2_ERROR_GET_MESSAGE(env->error));
            return AXIS2_FAILURE;
        }
        body_node = AXIOM_SOAP_BODY_GET_BASE_NODE(soap_body, env);
        if (NULL == body_node)
        {
            return AXIS2_FAILURE;
        }
        data_out = AXIOM_NODE_GET_FIRST_ELEMENT(body_node, env);
        if (NULL == data_out || AXIOM_NODE_GET_NODE_TYPE(data_out, env)
                != AXIOM_ELEMENT)
        {
            return AXIS2_FAILURE;
        }
        sender = axis2_rest_sender_create(env);
        AXIS2_REST_SENDER_SET_CHUNKED(sender, env,
                AXIS2_INTF_TO_IMPL(transport_sender)->chunked);
        AXIS2_REST_SENDER_SET_OM_OUTPUT(sender, env, om_output);
        AXIS2_REST_SENDER_SET_HTTP_VERSION(sender, env,
                AXIS2_INTF_TO_IMPL(transport_sender)->http_version);
        status = AXIS2_REST_SENDER_SEND(sender, env, msg_ctx, data_out, url);

        AXIS2_REST_SENDER_FREE(sender, env);
    }
    else
    {
        sender = axis2_soap_over_http_sender_create(env);

        if (NULL == sender)
        {
            return AXIS2_FAILURE;
        }
        AXIS2_SOAP_OVER_HTTP_SENDER_SET_CHUNKED(sender, env,
                AXIS2_INTF_TO_IMPL(transport_sender)->chunked);
        AXIS2_SOAP_OVER_HTTP_SENDER_SET_OM_OUTPUT(sender, env, om_output);
        AXIOM_SOAP_OVER_SENDER_SET_HTTP_VERSION(sender, env,
                AXIS2_INTF_TO_IMPL(transport_sender)->http_version);
        status = AXIS2_SOAP_OVER_HTTP_SENDER_SEND(sender, env, msg_ctx, out, url
                , soap_action);

		/*
         * TODO check for errors
         */
        AXIS2_SOAP_OVER_HTTP_SENDER_FREE(sender, env);
        sender = NULL;
    }

    op = AXIS2_MSG_CTX_GET_OP(msg_ctx, env);
    if (op)
    {
        /* handle one way case */
        const axis2_char_t *mep = AXIS2_OP_GET_MSG_EXCHANGE_PATTERN(op, env);
        if (AXIS2_STRCMP(mep, AXIS2_MEP_URI_OUT_ONLY) == 0 ||
            AXIS2_STRCMP(mep, AXIS2_MEP_URI_ROBUST_OUT_ONLY) == 0)
        {
            return status;
        }
		else
		{
			soap_ns_uri = AXIS2_MSG_CTX_GET_IS_SOAP_11(msg_ctx, env) ?
				AXIOM_SOAP11_SOAP_ENVELOPE_NAMESPACE_URI : AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI;
			response_envelope = axis2_http_transport_utils_create_soap_msg(env,
																		   msg_ctx, soap_ns_uri);
			AXIS2_MSG_CTX_SET_RESPONSE_SOAP_ENVELOPE (msg_ctx, env, response_envelope);
		}
    }

    return status;
}

/**
 * Following block distinguish the exposed part of the dll.
 */
AXIS2_EXPORT int
axis2_get_instance(
    struct axis2_transport_sender **inst,
    const axis2_env_t *env)
{
    *inst = axis2_http_transport_sender_create(env);
    if (!(*inst))
    {
        printf("transport sender load not success\n");
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
axis2_remove_instance(
    axis2_transport_sender_t *inst,
    const axis2_env_t *env)
{
    axis2_status_t status = AXIS2_FAILURE;
    if (inst)
    {
        status = AXIS2_TRANSPORT_SENDER_FREE(inst, env);
    }
    return status;
}



