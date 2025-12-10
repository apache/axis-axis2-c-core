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
#include <axis2_msg_ctx.h>
#include <httpd.h>
#include <http_request.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file axis2_apache2_request_processor.h
 * @brief Apache2 Request Processor Interface - Java-style interface pattern in C
 *
 * This interface enables protocol-specific request processing for Apache Axis2/C,
 * separating SOAP and JSON processing concerns while maintaining thread safety
 * for HTTP/2 multiplexed requests.
 *
 * Similar to Java interface:
 * public interface Apache2RequestProcessor {
 *     ProcessingResult processAcceptHeaders(HttpRequest request, MessageContext ctx);
 *     ProcessingResult processRequestBody(HttpRequest request, MessageContext ctx);
 *     void cleanup();
 * }
 *
 * Implementations:
 * - SoapRequestProcessor: Handles traditional SOAP/HTTP 1.1 with existing logic
 * - JsonRequestProcessor: Thread-safe HTTP/2 JSON processing with proper concurrency
 *
 * Benefits:
 * ✅ Zero risk to existing apache2_worker.c (no modifications needed)
 * ✅ Thread-safe HTTP/2 processing through isolated contexts
 * ✅ Protocol specialization (SOAP vs JSON optimizations)
 * ✅ Extensible design for future protocols (GraphQL, gRPC)
 */

    /** Forward declaration of the interface */
    typedef struct axis2_apache2_request_processor axis2_apache2_request_processor_t;

    /**
     * @brief Processing result status codes
     *
     * Provides detailed feedback on processing outcomes for better error handling
     * and debugging in HTTP/2 multiplexed environments.
     */
    typedef enum axis2_apache2_processing_result
    {
        AXIS2_APACHE2_PROCESSING_SUCCESS = 0,          /**< Processing completed successfully */
        AXIS2_APACHE2_PROCESSING_FAILURE = -1,         /**< Processing failed - general error */
        AXIS2_APACHE2_PROCESSING_CONCURRENT_ERROR = -2,/**< Thread safety violation detected */
        AXIS2_APACHE2_PROCESSING_PROTOCOL_ERROR = -3,  /**< Protocol-specific error (SOAP vs JSON) */
        AXIS2_APACHE2_PROCESSING_MEMORY_ERROR = -4     /**< Memory allocation failure */
    } axis2_apache2_processing_result_t;

    /**
     * @brief Request processing context for thread isolation
     *
     * Each HTTP/2 stream gets its own processing context to prevent
     * concurrent modification issues that caused the original array bounds errors.
     */
    typedef struct axis2_apache2_processing_context
    {
        /** Private accept header list - no sharing between streams */
        axutil_array_list_t* accept_records;

        /** Private accept-charset list - no sharing between streams */
        axutil_array_list_t* accept_charset_records;

        /** Private accept-language list - no sharing between streams */
        axutil_array_list_t* accept_language_records;

        /** Stream ID for debugging and tracing */
        axis2_char_t* stream_id;

        /** Processing start timestamp for performance monitoring */
        axutil_date_time_t* processing_start;

        /** Error context for detailed diagnostics */
        axutil_error_t* processing_error;

    } axis2_apache2_processing_context_t;

    /**
     * @brief Apache2 Request Processor Interface
     *
     * This interface abstracts HTTP request processing to enable protocol-specific
     * implementations while maintaining clean separation of concerns.
     *
     * The pattern eliminates the dangerous concurrent array modification that caused
     * "Array list index out of bounds" errors by providing thread-safe implementations.
     */
    struct axis2_apache2_request_processor
    {
        /**
         * @brief Process HTTP Accept headers in a thread-safe manner
         *
         * This method handles Accept, Accept-Charset, and Accept-Language headers
         * without the dangerous concurrent modification patterns found in the original
         * apache2_worker.c code.
         *
         * @param processor The request processor instance
         * @param env Axis2 environment
         * @param request Apache request record
         * @param msg_ctx Axis2 message context
         * @param processing_ctx Thread-isolated processing context
         * @return Processing result status
         */
        axis2_apache2_processing_result_t (*process_accept_headers)(
            struct axis2_apache2_request_processor* processor,
            const axutil_env_t* env,
            request_rec* request,
            axis2_msg_ctx_t* msg_ctx,
            axis2_apache2_processing_context_t* processing_ctx);

        /**
         * @brief Process HTTP request body with protocol-specific logic
         *
         * Delegates to appropriate protocol handler (SOAP vs JSON) based on
         * Content-Type and other request characteristics.
         *
         * @param processor The request processor instance
         * @param env Axis2 environment
         * @param request Apache request record
         * @param msg_ctx Axis2 message context
         * @param processing_ctx Thread-isolated processing context
         * @return Processing result status
         */
        axis2_apache2_processing_result_t (*process_request_body)(
            struct axis2_apache2_request_processor* processor,
            const axutil_env_t* env,
            request_rec* request,
            axis2_msg_ctx_t* msg_ctx,
            axis2_apache2_processing_context_t* processing_ctx);

        /**
         * @brief Validate request processor state for thread safety
         *
         * Performs runtime checks to ensure no shared state exists that could
         * cause concurrent modification issues under HTTP/2 load.
         *
         * @param processor The request processor instance
         * @param env Axis2 environment
         * @return AXIS2_TRUE if thread-safe, AXIS2_FALSE otherwise
         */
        axis2_bool_t (*is_thread_safe)(
            struct axis2_apache2_request_processor* processor,
            const axutil_env_t* env);

        /**
         * @brief Get processor protocol identifier
         *
         * Returns protocol type for debugging and performance monitoring.
         * Examples: "SOAP/HTTP1.1", "JSON/HTTP2", "REST/HTTP1.1"
         *
         * @param processor The request processor instance
         * @param env Axis2 environment
         * @return Protocol identifier string
         */
        const axis2_char_t* (*get_protocol_id)(
            struct axis2_apache2_request_processor* processor,
            const axutil_env_t* env);

        /**
         * @brief Create thread-isolated processing context
         *
         * Factory method for creating processing contexts that prevent the
         * concurrent modification issues found in the original apache2_worker.c.
         *
         * @param processor The request processor instance
         * @param env Axis2 environment
         * @param stream_id Optional HTTP/2 stream identifier
         * @return New processing context or NULL on failure
         */
        axis2_apache2_processing_context_t* (*create_processing_context)(
            struct axis2_apache2_request_processor* processor,
            const axutil_env_t* env,
            const axis2_char_t* stream_id);

        /**
         * @brief Free processing context and associated resources
         *
         * Ensures proper cleanup of thread-isolated resources to prevent
         * memory leaks in high-concurrency HTTP/2 environments.
         *
         * @param processor The request processor instance
         * @param env Axis2 environment
         * @param processing_ctx Processing context to free
         */
        void (*free_processing_context)(
            struct axis2_apache2_request_processor* processor,
            const axutil_env_t* env,
            axis2_apache2_processing_context_t* processing_ctx);

        /**
         * @brief Interface cleanup and resource deallocation
         *
         * Implements proper resource management following RAII-like patterns for C.
         *
         * @param processor The request processor instance to free
         * @param env Axis2 environment
         */
        void (*free)(
            struct axis2_apache2_request_processor* processor,
            const axutil_env_t* env);
    };

    /**
     * @brief Create SOAP-compatible request processor
     *
     * This implementation wraps the existing apache2_worker.c logic for backward
     * compatibility while providing the interface abstraction.
     *
     * Handles:
     * - Traditional SOAP/HTTP 1.1 requests
     * - REST API calls
     * - Existing Accept header processing (with original patterns)
     *
     * Thread Safety: Limited (uses original code paths)
     * Performance: Same as current implementation
     * Compatibility: 100% backward compatible
     *
     * @param env Axis2 environment
     * @return New SOAP processor instance or NULL on failure
     */
    AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
    axis2_apache2_request_processor_create_soap_impl(
        const axutil_env_t* env);

    /**
     * @brief Create JSON HTTP/2 optimized request processor
     *
     * This implementation provides thread-safe processing for HTTP/2 JSON requests
     * without the concurrent modification issues of the original code.
     *
     * Features:
     * - Thread-safe Accept header processing
     * - HTTP/2 stream isolation
     * - JSON-optimized request handling
     * - Performance monitoring and tracing
     * - Proper error reporting (JSON errors, not SOAP faults)
     *
     * Thread Safety: Full (isolated processing contexts)
     * Performance: Optimized for HTTP/2 multiplexing
     * Compatibility: New feature, no backward compatibility concerns
     *
     * @param env Axis2 environment
     * @return New JSON processor instance or NULL on failure
     */
    AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
    axis2_apache2_request_processor_create_json_impl(
        const axutil_env_t* env);

    /**
     * @brief Factory method for creating appropriate processor based on request
     *
     * Analyzes incoming request characteristics to select the optimal processor:
     * - Content-Type: application/json → JSON processor
     * - Content-Type: text/xml, application/soap+xml → SOAP processor
     * - HTTP Version: 2.0 + JSON content → JSON processor
     * - Default: SOAP processor (backward compatibility)
     *
     * This factory enables zero-impact migration where existing requests continue
     * using the original code paths while new JSON HTTP/2 requests get the
     * optimized, thread-safe implementation.
     *
     * @param env Axis2 environment
     * @param request Apache request record (for Content-Type analysis)
     * @return Appropriate processor instance or NULL on failure
     */
    AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
    axis2_apache2_request_processor_factory_create(
        const axutil_env_t* env,
        request_rec* request);

    /**
     * @brief Utility function to determine if request is HTTP/2 JSON
     *
     * Helper function for the factory to identify requests that should use
     * the thread-safe JSON processor instead of the legacy SOAP processor.
     *
     * @param request Apache request record
     * @return AXIS2_TRUE if HTTP/2 JSON request, AXIS2_FALSE otherwise
     */
    AXIS2_EXTERN axis2_bool_t AXIS2_CALL
    axis2_apache2_request_processor_is_json_http2_request(
        request_rec* request);

    /**
     * @brief Get processor implementation statistics
     *
     * Provides runtime statistics for monitoring and performance analysis:
     * - Total requests processed
     * - Average processing time
     * - Thread safety violations detected
     * - Memory allocation patterns
     *
     * @param processor The request processor instance
     * @param env Axis2 environment
     * @return Statistics hash table or NULL if not supported
     */
    AXIS2_EXTERN axutil_hash_t* AXIS2_CALL
    axis2_apache2_request_processor_get_statistics(
        axis2_apache2_request_processor_t* processor,
        const axutil_env_t* env);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_APACHE2_REQUEST_PROCESSOR_H */