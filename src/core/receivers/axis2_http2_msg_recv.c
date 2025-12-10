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

/**
 * @file axis2_http2_msg_recv.c
 * @brief Revolutionary HTTP/2 Message Receiver - AXIOM-FREE Implementation
 *
 * This implementation provides the essential message receiver interface
 * without AXIOM dependencies, following Java interface patterns to avoid
 * legacy SOAP baggage while maintaining compatibility with Axis2/C engine.
 *
 * REVOLUTIONARY APPROACH:
 * - No AXIOM headers or dependencies
 * - Compatible with engine linking requirements
 * - HTTP/2 JSON processing without XML/SOAP overhead
 * - Interface-based design for clean separation
 */

#include <axis2_msg_recv.h>
#include <axutil_param.h>
#include <axis2_description.h>
#include <axutil_class_loader.h>
#include <axis2_engine.h>
#include <axis2_core_utils.h>
#include <axutil_property.h>
#include <axutil_thread.h>
/* REVOLUTIONARY: NO AXIOM INCLUDES - eliminates SOAP/XML baggage */

/**
 * @brief Revolutionary Business Logic for HTTP/2 JSON Processing
 * Direct JSON response without SOAP/XML transformation
 */
static axis2_status_t AXIS2_CALL
axis2_http2_msg_recv_invoke_business_logic_sync(
    axis2_msg_recv_t *msg_recv,
    const axutil_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_msg_ctx_t *new_msg_ctx);

/**
 * @brief Create Revolutionary HTTP/2 Message Receiver
 * AXIOM-free implementation following existing Axis2/C receiver patterns
 */
AXIS2_EXTERN axis2_msg_recv_t *AXIS2_CALL
axis2_http2_msg_recv_create(
    const axutil_env_t *env)
{
    axis2_msg_recv_t *msg_recv = NULL;
    axis2_status_t status = AXIS2_FAILURE;

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "🚀 REVOLUTIONARY: Creating HTTP/2 message receiver - AXIOM-FREE implementation");

    /* Use base message receiver create function */
    msg_recv = axis2_msg_recv_create(env);
    if (!msg_recv)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /* Set application scope for HTTP/2 JSON processing */
    status = axis2_msg_recv_set_scope(msg_recv, env, AXIS2_APPLICATION_SCOPE);
    if (!status)
    {
        axis2_msg_recv_free(msg_recv, env);
        return NULL;
    }

    /* Set revolutionary HTTP/2 business logic */
    status = axis2_msg_recv_set_invoke_business_logic(msg_recv, env,
        axis2_http2_msg_recv_invoke_business_logic_sync);
    if (!status)
    {
        axis2_msg_recv_free(msg_recv, env);
        return NULL;
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "✅ REVOLUTIONARY SUCCESS: HTTP/2 message receiver created - Ready for JSON processing");

    return msg_recv;
}

/**
 * @brief Revolutionary HTTP/2 Business Logic Implementation
 * Direct JSON processing without SOAP/XML transformation pipeline
 */
static axis2_status_t AXIS2_CALL
axis2_http2_msg_recv_invoke_business_logic_sync(
    axis2_msg_recv_t *msg_recv,
    const axutil_env_t *env,
    axis2_msg_ctx_t *msg_ctx,
    axis2_msg_ctx_t *new_msg_ctx)
{
    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "🔥 REVOLUTIONARY: HTTP/2 business logic - Direct JSON processing, bypassing SOAP");

    if (!msg_recv || !msg_ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return AXIS2_FAILURE;
    }

    /* REVOLUTIONARY: Direct HTTP/2 JSON processing */
    /* Note: Actual JSON processing happens in transport layer (http_transport_utils.c) */
    /* This receiver provides engine compatibility without SOAP overhead */

    /* For HTTP/2 JSON requests, the response is already generated in transport layer */
    /* This business logic just provides a clean return path for engine compatibility */

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "✅ REVOLUTIONARY: HTTP/2 business logic completed - Direct JSON response ready");

    return AXIS2_SUCCESS;
}

