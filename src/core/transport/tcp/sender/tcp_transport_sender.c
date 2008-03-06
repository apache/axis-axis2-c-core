
/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      tcp://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <axis2_tcp_transport_sender.h>
#include <axutil_string.h>
#include <axis2_endpoint_ref.h>
#include <axis2_addr.h>
#include <axiom_xml_writer.h>
#include <axiom_output.h>
#include <axis2_tcp_transport.h>
#include <axiom_soap_body.h>
#include <axiom_soap.h>
#include <axutil_generic_obj.h>
#include <axutil_types.h>
#include <axutil_url.h>
#include <axutil_network_handler.h>

#define RES_BUFF 50

/**
 * TCP Transport Sender struct impl
 * Axis2 TCP Transport Sender impl
 */

typedef struct axis2_tcp_transport_sender_impl
{
    axis2_transport_sender_t transport_sender;
    int connection_timeout;
    int so_timeout;
} axis2_tcp_transport_sender_impl_t;

#define AXIS2_INTF_TO_IMPL(transport_sender)    \
    ((axis2_tcp_transport_sender_impl_t *)      \
     (transport_sender))

/***************************** Function headers *******************************/
axis2_status_t AXIS2_CALL axis2_tcp_transport_sender_invoke(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx);

axis2_status_t AXIS2_CALL axis2_tcp_transport_sender_clean_up(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx);

axis2_status_t AXIS2_CALL axis2_tcp_transport_sender_init(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_transport_out_desc_t * out_desc);

axis2_status_t AXIS2_CALL axis2_tcp_transport_sender_write_message(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axis2_endpoint_ref_t * epr,
    axiom_soap_envelope_t * out,
    axiom_output_t * om_output);

void AXIS2_CALL axis2_tcp_transport_sender_free(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env);

static const axis2_transport_sender_ops_t tcp_transport_sender_ops_var = {
    axis2_tcp_transport_sender_init,
    axis2_tcp_transport_sender_invoke,
    axis2_tcp_transport_sender_clean_up,
    axis2_tcp_transport_sender_free
};

axis2_transport_sender_t *AXIS2_CALL
axis2_tcp_transport_sender_create(
    const axutil_env_t * env)
{
    axis2_tcp_transport_sender_impl_t *transport_sender_impl = NULL;
    AXIS2_ENV_CHECK(env, NULL);

    transport_sender_impl = (axis2_tcp_transport_sender_impl_t *) AXIS2_MALLOC
        (env->allocator, sizeof(axis2_tcp_transport_sender_impl_t));

    if (!transport_sender_impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    transport_sender_impl->connection_timeout =
        AXIS2_TCP_DEFAULT_CONNECTION_TIMEOUT;
    transport_sender_impl->so_timeout = AXIS2_TCP_DEFAULT_SO_TIMEOUT;
    transport_sender_impl->transport_sender.ops = &tcp_transport_sender_ops_var;
    return &(transport_sender_impl->transport_sender);
}

void AXIS2_CALL
axis2_tcp_transport_sender_free(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env)
{
    axis2_tcp_transport_sender_impl_t *transport_sender_impl = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    transport_sender_impl = AXIS2_INTF_TO_IMPL(transport_sender);
    AXIS2_FREE(env->allocator, transport_sender_impl);
    return;
}

axis2_status_t AXIS2_CALL
axis2_tcp_transport_sender_invoke(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx)
{
    axis2_op_t *op = NULL;
    const axis2_char_t *mep_uri = NULL;
    axis2_bool_t is_server = AXIS2_TRUE;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axiom_xml_writer_t *xml_writer = NULL;
    axiom_output_t *om_output = NULL;
    axis2_char_t *buffer = NULL;
    axutil_stream_t *out_stream = NULL;
    int buffer_size = 0;
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_conf_t *conf = NULL;
    axis2_transport_out_desc_t *trans_desc = NULL;
    axutil_param_t *write_xml_declaration_param = NULL;
    axutil_hash_t *transport_attrs = NULL;
    axis2_bool_t write_xml_declaration = AXIS2_FALSE;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                    "start:tcp transport sender invoke");

    op = axis2_msg_ctx_get_op(msg_ctx, env);
    mep_uri = axis2_op_get_msg_exchange_pattern(op, env);

    is_server = axis2_msg_ctx_get_server_side(msg_ctx, env);

    soap_envelope = axis2_msg_ctx_get_soap_envelope(msg_ctx, env);

    xml_writer = axiom_xml_writer_create_for_memory(env, NULL,
                                                    AXIS2_TRUE, 0,
                                                    AXIS2_XML_PARSER_TYPE_BUFFER);
    if (!xml_writer)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[tcp]Failed to create XML writer");
        return AXIS2_FAILURE;
    }

    om_output = axiom_output_create(env, xml_writer);
    if (!om_output)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[tcp]Failed to create OM output");
        axiom_xml_writer_free(xml_writer, env);
        xml_writer = NULL;
        return AXIS2_FAILURE;
    }

    conf_ctx = axis2_msg_ctx_get_conf_ctx (msg_ctx, env);
    if (conf_ctx)
    {
        conf = axis2_conf_ctx_get_conf (conf_ctx, env);
    }
    if (conf)
    {
        trans_desc = axis2_conf_get_transport_out (conf,
                                                   env, AXIS2_TRANSPORT_ENUM_TCP);
    }
    if (trans_desc)
    {
        write_xml_declaration_param =
            axutil_param_container_get_param
            (axis2_transport_out_desc_param_container (trans_desc, env), env,
             AXIS2_XML_DECLARATION);
    }
    if (write_xml_declaration_param)
    {
        transport_attrs = axutil_param_get_attributes (write_xml_declaration_param, env);
        if (transport_attrs)
        {
            axutil_generic_obj_t *obj = NULL;
            axiom_attribute_t *write_xml_declaration_attr = NULL;
            axis2_char_t *write_xml_declaration_attr_value = NULL;

            obj = axutil_hash_get (transport_attrs, AXIS2_ADD_XML_DECLARATION,
                           AXIS2_HASH_KEY_STRING);
            if (obj)
            {
                write_xml_declaration_attr = (axiom_attribute_t *) axutil_generic_obj_get_value (obj,
                                                                                         env);
            }
            if (write_xml_declaration_attr)
            {
                write_xml_declaration_attr_value = axiom_attribute_get_value (write_xml_declaration_attr, env);
            }
            if (write_xml_declaration_attr_value && 0 == axutil_strcasecmp (write_xml_declaration_attr_value, AXIS2_VALUE_TRUE))
            {
                write_xml_declaration = AXIS2_TRUE;
            }
        }
    }

    if (write_xml_declaration)
    {
        axiom_output_write_xml_version_encoding (om_output, env);
    }

    axiom_soap_envelope_serialize(soap_envelope, env, om_output, AXIS2_FALSE);

    buffer = (axis2_char_t *) axiom_xml_writer_get_xml(xml_writer, env);
    if (!buffer)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "[tcp]Failed to serialize the SOAP envelope");
        return AXIS2_FAILURE;
    }
    buffer_size = axiom_xml_writer_get_xml_size(xml_writer, env);
    buffer[buffer_size] = 0;
    if (is_server)
    {
        out_stream = axis2_msg_ctx_get_transport_out_stream(msg_ctx, env);
        axutil_stream_write(out_stream, env, buffer, buffer_size);
    }
    else
    {
        axis2_endpoint_ref_t *to = NULL;
        axutil_url_t *to_url = NULL;
        const axis2_char_t *to_str = NULL;
        const axis2_char_t *host = NULL;
        int port = 0;
        int socket = -1;
        axutil_stream_t *stream;
        int write = -1;
        int read = -1;
        axis2_char_t buff[1];
        axis2_char_t *res_buffer = (axis2_char_t *) AXIS2_MALLOC(env->allocator,
                                                                 RES_BUFF);
        int res_size = 0;
        int size = 0;
        axiom_xml_reader_t *reader = NULL;
        axiom_stax_builder_t *builder = NULL;
        axiom_soap_builder_t *soap_builder = NULL;
        axiom_soap_envelope_t *soap_envelope = NULL;

        to = axis2_msg_ctx_get_to(msg_ctx, env);

        if (!to)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "To epr not presant");
            return AXIS2_FAILURE;
        }

        to_str = axis2_endpoint_ref_get_address(to, env);
        if (!to_str)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "unable to convert epr to string");
            return AXIS2_FAILURE;
        }

        to_url = axutil_url_parse_string(env, to_str);

        if (!to_url)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "unable to parser string to url");
            return AXIS2_FAILURE;
        }

        host = axutil_url_get_host(to_url, env);
        if (!host)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "retrieving host failed");
            return AXIS2_FAILURE;
        }

        port = axutil_url_get_port(to_url, env);
        if (!port)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "retrieving port failed");
            return AXIS2_FAILURE;
        }
        socket = (int)axutil_network_handler_open_socket(env, (char *) host, port);
        if (!socket)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "socket creation failed");
            return AXIS2_FAILURE;
        }

        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                        "open socket for host:%s port:%d", host, port);

        stream = axutil_stream_create_socket(env, socket);
        if (!stream)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "stream creation failed");
            return AXIS2_FAILURE;
        }

        write = axutil_stream_write(stream, env, buffer, buffer_size);
        if (write < 0)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "stream write error");
            return AXIS2_FAILURE;
        }
        AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "stream wrote soap msg: %s",
                        buffer);
        write = axutil_stream_write(stream, env, "\r\n\r\n", 4);
        size = RES_BUFF;
        while ((read = axutil_stream_read(stream, env, &buff, 1)) > 0)
        {
            if (res_size >= size)
            {
                axis2_char_t *tmp_buff = NULL;
                size <<= 2;
                tmp_buff = AXIS2_MALLOC(env->allocator, size);
                memcpy(tmp_buff, res_buffer, res_size);
                AXIS2_FREE(env->allocator, res_buffer);
                res_buffer = tmp_buff;
            }
            memcpy(res_buffer + res_size, buff, 1);
            res_size++;
        }

        axutil_network_handler_close_socket(env, stream->socket);
        axutil_stream_close(stream, env);
        axutil_stream_free(stream, env);

        AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI, "%s", res_buffer);

        reader =
            axiom_xml_reader_create_for_memory(env, res_buffer, (res_size - 1),
                                               NULL,
                                               AXIS2_XML_PARSER_TYPE_BUFFER);

        if (!reader)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Failed to create XML reader");
            return AXIS2_FAILURE;
        }

        builder = axiom_stax_builder_create(env, reader);
        if (!builder)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Failed to create Stax builder");
            return AXIS2_FAILURE;
        }

        soap_builder = axiom_soap_builder_create(env, builder,
                                                 AXIOM_SOAP12_SOAP_ENVELOPE_NAMESPACE_URI);
        if (!soap_builder)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Failed to create SOAP builder");
            return AXIS2_FAILURE;
        }
        soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);

        if (!soap_envelope)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                            "Failed to create SOAP envelope");
            return AXIS2_FAILURE;
        }

        axis2_msg_ctx_set_response_soap_envelope(msg_ctx, env, soap_envelope);

    }
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "end:tcp transport sender invoke");
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_tcp_transport_sender_clean_up(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx)
{
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, msg_ctx, AXIS2_FAILURE);
    /*
     * Clean up is not used. If the tcp sender needs
     * to be cleaned up it should be done here.
     */
    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_tcp_transport_sender_init(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx,
    axis2_transport_out_desc_t * out_desc)
{
    axis2_char_t *temp = NULL;
    axutil_param_t *temp_param = NULL;
    AXIS2_ENV_CHECK(env, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, conf_ctx, AXIS2_FAILURE);
    AXIS2_PARAM_CHECK(env->error, out_desc, AXIS2_FAILURE);

    temp_param =
        axutil_param_container_get_param
        (axis2_transport_out_desc_param_container(out_desc, env), env,
         AXIS2_TCP_SO_TIMEOUT);
    if (temp_param)
    {
        temp = axutil_param_get_value(temp_param, env);
    }
    if (temp)
    {
        AXIS2_INTF_TO_IMPL(transport_sender)->so_timeout = AXIS2_ATOI(temp);
    }
    temp =
        (axis2_char_t *)
        axutil_param_container_get_param
        (axis2_transport_out_desc_param_container(out_desc, env), env,
         AXIS2_TCP_CONNECTION_TIMEOUT);
    if (temp_param)
    {
        temp = axutil_param_get_value(temp_param, env);
    }
    if (temp)
    {
        AXIS2_INTF_TO_IMPL(transport_sender)->connection_timeout =
            AXIS2_ATOI(temp);
    }

    return AXIS2_SUCCESS;
}

axis2_status_t AXIS2_CALL
axis2_tcp_transport_sender_write_message(
    axis2_transport_sender_t * transport_sender,
    const axutil_env_t * env,
    axis2_msg_ctx_t * msg_ctx,
    axis2_endpoint_ref_t * epr,
    axiom_soap_envelope_t * out,
    axiom_output_t * om_output)
{

    return AXIS2_TRUE;
}

/**
 * Following block distinguish the exposed part of the dll.
 */

/* When building for static deployment, give the get and remove methods
 * unique names.  This avoids having the linker fail with duplicate symbol
 * errors.
 */

AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
 axis2_get_instance(
#else
axis2_tcp_transport_sender_get_instance(
#endif
    struct axis2_transport_sender **inst,
    const axutil_env_t * env)
{
    *inst = axis2_tcp_transport_sender_create(env);
    if (!(*inst))
    {
        return AXIS2_FAILURE;
    }

    return AXIS2_SUCCESS;
}

AXIS2_EXPORT int
#ifndef AXIS2_STATIC_DEPLOY
 axis2_remove_instance(
#else
axis2_tcp_transport_sender_remove_instance(
#endif
    axis2_transport_sender_t * inst,
    const axutil_env_t * env)
{
    if (inst)
    {
        AXIS2_TRANSPORT_SENDER_FREE(inst, env);
    }
    return AXIS2_SUCCESS;
}
