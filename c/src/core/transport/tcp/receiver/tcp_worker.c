
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

#include <axis2_tcp_worker.h>
#include <axutil_string.h>
#include <axis2_tcp_transport.h>
#include <axis2_conf.h>
#include <axutil_string.h>
#include <axutil_stream.h>
#include <axis2_msg_ctx.h>
#include <axis2_op_ctx.h>
#include <axis2_engine.h>
#include <axutil_uuid_gen.h>
#include <axutil_url.h>
#include <axutil_property.h>
#include <string.h>
#include <axiom_soap.h>
#include <axiom.h>
#include <axis2_simple_tcp_svr_conn.h>

struct axis2_tcp_worker
{
    axis2_conf_ctx_t *conf_ctx;
    int svr_port;
};

AXIS2_EXTERN axis2_tcp_worker_t *AXIS2_CALL
axis2_tcp_worker_create(
    const axutil_env_t * env,
    axis2_conf_ctx_t * conf_ctx)
{
    axis2_tcp_worker_t *tcp_worker = NULL;
    AXIS2_ENV_CHECK(env, NULL);
    tcp_worker = (axis2_tcp_worker_t *)
        AXIS2_MALLOC(env->allocator, sizeof(axis2_tcp_worker_t));

    if (!tcp_worker)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }
    tcp_worker->conf_ctx = conf_ctx;
    tcp_worker->svr_port = 9090;    /* default - must set later */

    return tcp_worker;
}

AXIS2_EXTERN void AXIS2_CALL
axis2_tcp_worker_free(
    axis2_tcp_worker_t * tcp_worker,
    const axutil_env_t * env)
{
    AXIS2_ENV_CHECK(env, void);

    tcp_worker->conf_ctx = NULL;

    AXIS2_FREE(env->allocator, tcp_worker);

    return;
}

AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_tcp_worker_process_request(
    axis2_tcp_worker_t * tcp_worker,
    const axutil_env_t * env,
    axis2_simple_tcp_svr_conn_t * svr_conn,
    axis2_char_t * simple_request)
{
    axis2_conf_ctx_t *conf_ctx = NULL;
    axis2_transport_out_desc_t *out_desc = NULL;
    axis2_transport_in_desc_t *in_desc = NULL;
    axis2_msg_ctx_t *msg_ctx = NULL;
    axiom_xml_reader_t *reader = NULL;
    axiom_stax_builder_t *builder = NULL;
    axiom_soap_builder_t *soap_builder = NULL;
    axiom_soap_envelope_t *soap_envelope = NULL;
    axis2_engine_t *engine = NULL;
    axis2_status_t status = AXIS2_FALSE;
    axutil_stream_t *svr_stream = NULL;
    axis2_char_t *buffer = NULL;
    int len = 0;
    int write = -1;
    axutil_stream_t *out_stream = NULL;

    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI,
                    "start:axis2_tcp_worker_process_request");

    out_stream = axutil_stream_create_basic(env);
    reader = axiom_xml_reader_create_for_memory(env, simple_request,
                                                axutil_strlen(simple_request),
                                                NULL,
                                                AXIS2_XML_PARSER_TYPE_BUFFER);
    if (!reader)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to create XML reader");
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

    conf_ctx = tcp_worker->conf_ctx;

    if (!conf_ctx)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "conf ctx not available");
        return AXIS2_FAILURE;
    }

    out_desc =
        axis2_conf_get_transport_out(axis2_conf_ctx_get_conf(conf_ctx, env),
                                     env, AXIS2_TRANSPORT_ENUM_TCP);
    if (!out_desc)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Transport out not set");
        return AXIS2_FAILURE;
    }

    in_desc =
        axis2_conf_get_transport_in(axis2_conf_ctx_get_conf(conf_ctx, env), env,
                                    AXIS2_TRANSPORT_ENUM_TCP);

    msg_ctx = axis2_msg_ctx_create(env, conf_ctx, in_desc, out_desc);
    axis2_msg_ctx_set_server_side(msg_ctx, env, AXIS2_TRUE);
    axis2_msg_ctx_set_transport_out_stream(msg_ctx, env, out_stream);

    soap_envelope = axiom_soap_builder_get_soap_envelope(soap_builder, env);
    axis2_msg_ctx_set_soap_envelope(msg_ctx, env, soap_envelope);

    engine = axis2_engine_create(env, conf_ctx);
    status = axis2_engine_receive(engine, env, msg_ctx);

    svr_stream = axis2_simple_tcp_svr_conn_get_stream(svr_conn, env);
    buffer = out_stream->buffer;
    len = out_stream->len;
    buffer[len] = 0;
    if (svr_stream && buffer)
    {
        write = axutil_stream_write(svr_stream, env, buffer, len + 1);
        if (write < 0)
        {
            AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "stream write failed");
            return AXIS2_FAILURE;
        }
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI, "stream wrote:%s", buffer);
    }
    AXIS2_LOG_TRACE(env->log, AXIS2_LOG_SI,
                    "end:axis2_tcp_worker_process_request");
    return AXIS2_SUCCESS;
}

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_tcp_worker_set_svr_port(
    axis2_tcp_worker_t * worker,
    const axutil_env_t * env,
    int port)
{
    worker->svr_port = port;
    return AXIS2_SUCCESS;
}
