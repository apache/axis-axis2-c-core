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

#include <axis2_apache2_request_processor.h>

/**
 * @file axis2_apache2_request_processor_soap_impl_simple.c
 * @brief Simplified SOAP Processor - Delegates to Original Code
 */

/** SOAP processor implementation structure */
typedef struct axis2_apache2_soap_processor_impl
{
    axis2_apache2_request_processor_t interface;  /* Must be first - allows casting */
} axis2_apache2_soap_processor_impl_t;

/* Forward declarations */
static axis2_apache2_processing_result_t
axis2_apache2_soap_processor_process_accept_headers_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    request_rec* request,
    axis2_msg_ctx_t* msg_ctx,
    axis2_apache2_processing_context_t* processing_ctx);

static axis2_apache2_processing_result_t
axis2_apache2_soap_processor_process_request_body_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    request_rec* request,
    axis2_msg_ctx_t* msg_ctx,
    axis2_apache2_processing_context_t* processing_ctx);

static axis2_bool_t
axis2_apache2_soap_processor_is_thread_safe_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env);

static const axis2_char_t*
axis2_apache2_soap_processor_get_protocol_id_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env);

static axis2_apache2_processing_context_t*
axis2_apache2_soap_processor_create_processing_context_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    const axis2_char_t* stream_id);

static void
axis2_apache2_soap_processor_free_processing_context_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    axis2_apache2_processing_context_t* processing_ctx);

static void
axis2_apache2_soap_processor_free_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env);

/**
 * @brief Factory function for creating simplified SOAP processor
 */
AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
axis2_apache2_request_processor_create_soap_impl(
    const axutil_env_t* env)
{
    axis2_apache2_soap_processor_impl_t* impl = NULL;

    AXIS2_ENV_CHECK(env, NULL);

    impl = AXIS2_MALLOC(env->allocator, sizeof(axis2_apache2_soap_processor_impl_t));
    if (!impl)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /* Wire up interface function pointers */
    impl->interface.process_accept_headers = axis2_apache2_soap_processor_process_accept_headers_impl;
    impl->interface.process_request_body = axis2_apache2_soap_processor_process_request_body_impl;
    impl->interface.is_thread_safe = axis2_apache2_soap_processor_is_thread_safe_impl;
    impl->interface.get_protocol_id = axis2_apache2_soap_processor_get_protocol_id_impl;
    impl->interface.create_processing_context = axis2_apache2_soap_processor_create_processing_context_impl;
    impl->interface.free_processing_context = axis2_apache2_soap_processor_free_processing_context_impl;
    impl->interface.free = axis2_apache2_soap_processor_free_impl;

    AXIS2_LOG_INFO(env->log, "🧽 SOAP PROCESSOR: Created simplified SOAP processor");

    return (axis2_apache2_request_processor_t*)impl;
}

/**
 * @brief Simplified Accept header processing - delegates to original code
 */
static axis2_apache2_processing_result_t
axis2_apache2_soap_processor_process_accept_headers_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    request_rec* request,
    axis2_msg_ctx_t* msg_ctx,
    axis2_apache2_processing_context_t* processing_ctx)
{
    AXIS2_LOG_INFO(env->log, "🧽 SOAP PROCESSOR: Delegating to original apache2_worker.c Accept header processing");

    /* Return "not handled" so original code path is used */
    return AXIS2_APACHE2_PROCESSING_NOT_HANDLED;
}

/**
 * @brief Process request body - delegate to original code
 */
static axis2_apache2_processing_result_t
axis2_apache2_soap_processor_process_request_body_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    request_rec* request,
    axis2_msg_ctx_t* msg_ctx,
    axis2_apache2_processing_context_t* processing_ctx)
{
    AXIS2_LOG_INFO(env->log, "🧽 SOAP PROCESSOR: Delegating to original body processing");
    return AXIS2_APACHE2_PROCESSING_NOT_HANDLED;
}

/**
 * @brief Thread safety check - SOAP processor is NOT thread safe (honest assessment)
 */
static axis2_bool_t
axis2_apache2_soap_processor_is_thread_safe_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env)
{
    AXIS2_LOG_INFO(env->log, "⚠️ SOAP PROCESSOR: NOT thread-safe (uses original dangerous patterns)");
    return AXIS2_FALSE;
}

/**
 * @brief Get protocol identifier
 */
static const axis2_char_t*
axis2_apache2_soap_processor_get_protocol_id_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env)
{
    return "SOAP/HTTP1.1";
}

/**
 * @brief Create minimal processing context
 */
static axis2_apache2_processing_context_t*
axis2_apache2_soap_processor_create_processing_context_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    const axis2_char_t* stream_id)
{
    axis2_apache2_processing_context_t* ctx = NULL;

    ctx = AXIS2_MALLOC(env->allocator, sizeof(axis2_apache2_processing_context_t));
    if (!ctx)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /* Minimal context setup */
    ctx->accept_records = NULL;
    ctx->accept_charset_records = NULL;
    ctx->accept_language_records = NULL;
    ctx->stream_id = stream_id ? axutil_strdup(env, stream_id) : NULL;
    ctx->processing_start = NULL;
    ctx->processing_error = NULL;

    return ctx;
}

/**
 * @brief Free processing context
 */
static void
axis2_apache2_soap_processor_free_processing_context_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env,
    axis2_apache2_processing_context_t* processing_ctx)
{
    if (processing_ctx)
    {
        if (processing_ctx->stream_id)
        {
            AXIS2_FREE(env->allocator, processing_ctx->stream_id);
        }
        AXIS2_FREE(env->allocator, processing_ctx);
    }
}

/**
 * @brief Free SOAP processor implementation
 */
static void
axis2_apache2_soap_processor_free_impl(
    axis2_apache2_request_processor_t* processor,
    const axutil_env_t* env)
{
    if (processor)
    {
        AXIS2_LOG_INFO(env->log, "🧽 SOAP PROCESSOR: Freeing simplified SOAP processor");
        AXIS2_FREE(env->allocator, processor);
    }
}