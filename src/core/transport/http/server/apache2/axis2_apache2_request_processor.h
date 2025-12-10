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

#ifndef AXIS2_APACHE2_REQUEST_PROCESSOR_H
#define AXIS2_APACHE2_REQUEST_PROCESSOR_H

#include <axutil_env.h>
#include <axutil_array_list.h>
#include <axutil_date_time.h>
#include <axis2_msg_ctx.h>
#include <httpd.h>
#include <http_request.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file axis2_apache2_request_processor.h
 * @brief Apache2 Request Processor Interface
 *
 * This header defines the interface pattern for Apache2 request processing,
 * enabling intelligent selection between thread-safe JSON HTTP/2 processors
 * and legacy SOAP HTTP/1.1 processors.
 */

/** Forward declarations */
typedef struct axis2_apache2_request_processor axis2_apache2_request_processor_t;
typedef struct axis2_apache2_processing_context axis2_apache2_processing_context_t;

/**
 * @brief Processing result enumeration
 */
typedef enum axis2_apache2_processing_result
{
    /** Processing completed successfully */
    AXIS2_APACHE2_PROCESSING_SUCCESS = 0,

    /** Processing failed with error */
    AXIS2_APACHE2_PROCESSING_FAILURE = 1,

    /** Memory allocation error during processing */
    AXIS2_APACHE2_PROCESSING_MEMORY_ERROR = 2,

    /** Processor cannot handle this request type - delegate to original code */
    AXIS2_APACHE2_PROCESSING_NOT_HANDLED = 3

} axis2_apache2_processing_result_t;

/**
 * @brief Processing context for stream isolation
 */
struct axis2_apache2_processing_context
{
    /** Stream-specific accept records (isolated per HTTP/2 stream) */
    axutil_array_list_t* accept_records;

    /** Stream-specific accept-charset records */
    axutil_array_list_t* accept_charset_records;

    /** Stream-specific accept-language records */
    axutil_array_list_t* accept_language_records;

    /** HTTP/2 stream identifier for isolation */
    axis2_char_t* stream_id;

    /** Processing start time for performance tracking */
    axutil_date_time_t* processing_start;

    /** Error information if processing fails */
    axis2_char_t* processing_error;
};

/**
 * @brief Request Processor Interface (Function Pointer Table)
 *
 * This interface enables polymorphic behavior in C by using function pointers,
 * similar to virtual method tables in object-oriented languages.
 */
struct axis2_apache2_request_processor
{
    /**
     * @brief Process Accept headers for request
     * @param processor The processor instance
     * @param env The environment
     * @param request Apache request record
     * @param msg_ctx Axis2 message context
     * @param processing_ctx Stream-isolated processing context
     * @return Processing result
     */
    axis2_apache2_processing_result_t (*process_accept_headers)(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env,
        request_rec* request,
        axis2_msg_ctx_t* msg_ctx,
        axis2_apache2_processing_context_t* processing_ctx);

    /**
     * @brief Process request body
     * @param processor The processor instance
     * @param env The environment
     * @param request Apache request record
     * @param msg_ctx Axis2 message context
     * @param processing_ctx Stream-isolated processing context
     * @return Processing result
     */
    axis2_apache2_processing_result_t (*process_request_body)(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env,
        request_rec* request,
        axis2_msg_ctx_t* msg_ctx,
        axis2_apache2_processing_context_t* processing_ctx);

    /**
     * @brief Check if processor is thread-safe
     * @param processor The processor instance
     * @param env The environment
     * @return AXIS2_TRUE if thread-safe, AXIS2_FALSE otherwise
     */
    axis2_bool_t (*is_thread_safe)(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env);

    /**
     * @brief Get protocol identifier
     * @param processor The processor instance
     * @param env The environment
     * @return Protocol string (e.g., "JSON/HTTP2", "SOAP/HTTP1.1")
     */
    const axis2_char_t* (*get_protocol_id)(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env);

    /**
     * @brief Create stream-isolated processing context
     * @param processor The processor instance
     * @param env The environment
     * @param stream_id HTTP/2 stream ID for isolation
     * @return New processing context or NULL on error
     */
    axis2_apache2_processing_context_t* (*create_processing_context)(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env,
        const axis2_char_t* stream_id);

    /**
     * @brief Free processing context
     * @param processor The processor instance
     * @param env The environment
     * @param processing_ctx Context to free
     */
    void (*free_processing_context)(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env,
        axis2_apache2_processing_context_t* processing_ctx);

    /**
     * @brief Free processor instance
     * @param processor The processor instance
     * @param env The environment
     */
    void (*free)(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env);
};

/**
 * @brief Factory function for creating request processors
 *
 * Intelligently selects processor based on request characteristics:
 * - HTTP/2 + JSON → Thread-safe JSON processor
 * - HTTP/2 + Any → Thread-safe JSON processor (assume modern client)
 * - HTTP/1.1 + SOAP → Legacy SOAP processor
 * - Default → Legacy SOAP processor (maximum compatibility)
 *
 * @param env The environment
 * @param request Apache request record for analysis
 * @return Appropriate processor instance or NULL on error
 */
AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
axis2_apache2_request_processor_factory_create(
    const axutil_env_t* env,
    request_rec* request);

/**
 * @brief Create JSON HTTP/2 processor (thread-safe)
 * @param env The environment
 * @return JSON processor instance or NULL on error
 */
AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
axis2_apache2_request_processor_create_json_impl(
    const axutil_env_t* env);

/**
 * @brief Create SOAP HTTP/1.1 processor (legacy compatibility)
 * @param env The environment
 * @return SOAP processor instance or NULL on error
 */
AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
axis2_apache2_request_processor_create_soap_impl(
    const axutil_env_t* env);

/**
 * @brief Utility function to detect JSON HTTP/2 requests
 * @param request Apache request record
 * @return AXIS2_TRUE if JSON HTTP/2 request, AXIS2_FALSE otherwise
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_apache2_request_processor_is_json_http2_request(
    request_rec* request);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_APACHE2_REQUEST_PROCESSOR_H */