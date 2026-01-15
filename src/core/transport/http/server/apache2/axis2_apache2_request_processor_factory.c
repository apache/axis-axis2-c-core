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
#include <axutil_string.h>
#include <axis2_http_transport.h>
#include <httpd.h>
#include <http_log.h>
#include <apr_strings.h>

/**
 * @file axis2_apache2_request_processor_factory.c
 * @brief Factory implementation for Apache2 Request Processor Interface
 *
 * This factory provides intelligent processor selection based on request characteristics,
 * enabling zero-impact migration where existing SOAP requests continue using original
 * code paths while new JSON HTTP/2 requests get optimized, thread-safe processing.
 */

/**
 * @brief Request analysis result for processor selection
 */
typedef struct axis2_request_analysis_result
{
    axis2_bool_t is_http2;                    /**< HTTP/2 protocol detected */
    axis2_bool_t is_json_content;             /**< JSON content type detected */
    axis2_bool_t is_soap_content;             /**< SOAP content type detected */
    axis2_bool_t requires_thread_safety;     /**< Thread-safe processing required */
    const axis2_char_t* content_type;        /**< Full content type string */
    const axis2_char_t* protocol_version;    /**< HTTP protocol version */
} axis2_request_analysis_result_t;

/* Forward declarations of internal functions */
static axis2_request_analysis_result_t*
axis2_apache2_request_processor_factory_analyze_request(
    const axutil_env_t* env,
    request_rec* request);

static void
axis2_apache2_request_processor_factory_free_analysis_result(
    const axutil_env_t* env,
    axis2_request_analysis_result_t* result);

/**
 * @brief Factory method implementation for creating appropriate processor
 *
 * Decision matrix:
 * 1. HTTP/2 + JSON Content-Type → JSON Processor (thread-safe)
 * 2. HTTP/2 + Any Content-Type → JSON Processor (assume modern client)
 * 3. HTTP/1.1 + SOAP Content-Type → SOAP Processor (legacy compatibility)
 * 4. HTTP/1.1 + JSON Content-Type → SOAP Processor (for now, safe fallback)
 * 5. Default/Unknown → SOAP Processor (maximum compatibility)
 */
AXIS2_EXTERN axis2_apache2_request_processor_t* AXIS2_CALL
axis2_apache2_request_processor_factory_create(
    const axutil_env_t* env,
    request_rec* request)
{
    axis2_request_analysis_result_t* analysis = NULL;
    axis2_apache2_request_processor_t* processor = NULL;

    AXIS2_ENV_CHECK(env, NULL);
    AXIS2_PARAM_CHECK(env->error, request, NULL);

    /* Analyze request characteristics */
    analysis = axis2_apache2_request_processor_factory_analyze_request(env, request);
    if (!analysis)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "🚨 REQUEST PROCESSOR FACTORY: Failed to analyze request - defaulting to SOAP processor");
        return axis2_apache2_request_processor_create_soap_impl(env);
    }

    AXIS2_LOG_INFO(env->log,
        "🏭 REQUEST PROCESSOR FACTORY: Request analysis - HTTP2=%s, JSON=%s, SOAP=%s, ThreadSafe=%s",
        analysis->is_http2 ? "YES" : "NO",
        analysis->is_json_content ? "YES" : "NO",
        analysis->is_soap_content ? "YES" : "NO",
        analysis->requires_thread_safety ? "REQUIRED" : "OPTIONAL");

    /* Processor selection logic */
    if (analysis->is_http2 && analysis->is_json_content)
    {
        /* HTTP/2 JSON: Use optimized thread-safe processor */
        AXIS2_LOG_INFO(env->log,
            "REQUEST PROCESSOR FACTORY: Creating JSON HTTP/2 processor for thread-safe processing");
        processor = axis2_apache2_request_processor_create_json_impl(env);
    }
    else if (analysis->is_http2)
    {
        /* HTTP/2 non-JSON: Assume modern client, use JSON processor for thread safety */
        AXIS2_LOG_INFO(env->log,
            "🔄 REQUEST PROCESSOR FACTORY: Creating JSON processor for HTTP/2 thread safety (content-type: %s)",
            analysis->content_type ? analysis->content_type : "unknown");
        processor = axis2_apache2_request_processor_create_json_impl(env);
    }
    else if (analysis->is_json_content)
    {
        /* HTTP/1.1 JSON: Use SOAP processor for now (safe fallback) */
        AXIS2_LOG_INFO(env->log,
            "⚖️ REQUEST PROCESSOR FACTORY: Using SOAP processor for HTTP/1.1 JSON (safe fallback)");
        processor = axis2_apache2_request_processor_create_soap_impl(env);
    }
    else
    {
        /* Default case: Traditional SOAP/REST over HTTP/1.1 */
        AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
            "🔒 REQUEST PROCESSOR FACTORY: Creating SOAP processor for traditional request (content-type: %s)",
            analysis->content_type ? analysis->content_type : "unknown");
        processor = axis2_apache2_request_processor_create_soap_impl(env);
    }

    /* Fallback safety */
    if (!processor)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
            "🚨 REQUEST PROCESSOR FACTORY: Primary processor creation failed - falling back to SOAP processor");
        processor = axis2_apache2_request_processor_create_soap_impl(env);
    }

    axis2_apache2_request_processor_factory_free_analysis_result(env, analysis);
    return processor;
}

/**
 * @brief Analyze request characteristics for processor selection
 */
static axis2_request_analysis_result_t*
axis2_apache2_request_processor_factory_analyze_request(
    const axutil_env_t* env,
    request_rec* request)
{
    axis2_request_analysis_result_t* result = NULL;
    const axis2_char_t* content_type = NULL;
    const axis2_char_t* protocol = NULL;

    result = AXIS2_MALLOC(env->allocator, sizeof(axis2_request_analysis_result_t));
    if (!result)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_NO_MEMORY, AXIS2_FAILURE);
        return NULL;
    }

    /* Initialize result structure */
    result->is_http2 = AXIS2_FALSE;
    result->is_json_content = AXIS2_FALSE;
    result->is_soap_content = AXIS2_FALSE;
    result->requires_thread_safety = AXIS2_FALSE;
    result->content_type = NULL;
    result->protocol_version = NULL;

    /* Analyze HTTP protocol version */
    protocol = request->protocol;
    if (protocol)
    {
        result->protocol_version = apr_pstrdup(request->pool, protocol);

        /* Check for HTTP/2 */
        if (strstr(protocol, "HTTP/2") || strstr(protocol, "HTTP/2.0"))
        {
            result->is_http2 = AXIS2_TRUE;
            result->requires_thread_safety = AXIS2_TRUE;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "REQUEST ANALYSIS: HTTP/2 detected - protocol: %s", protocol);
        }
    }

    /* Analyze Content-Type header */
    content_type = apr_table_get(request->headers_in, "Content-Type");
    if (!content_type)
    {
        content_type = apr_table_get(request->headers_in, "content-type");
    }

    if (content_type)
    {
        result->content_type = apr_pstrdup(request->pool, content_type);

        /* Check for JSON content types */
        if (strstr(content_type, "application/json") ||
            strstr(content_type, "text/json") ||
            strstr(content_type, "application/hal+json") ||
            strstr(content_type, "application/vnd.api+json"))
        {
            result->is_json_content = AXIS2_TRUE;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "REQUEST ANALYSIS: JSON content detected - content-type: %s", content_type);
        }
        /* Check for SOAP content types */
        else if (strstr(content_type, "text/xml") ||
                 strstr(content_type, "application/xml") ||
                 strstr(content_type, "application/soap+xml"))
        {
            result->is_soap_content = AXIS2_TRUE;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "REQUEST ANALYSIS: SOAP content detected - content-type: %s", content_type);
        }
    }

    /* Additional HTTP/2 detection methods */
    if (!result->is_http2)
    {
        /* Check for HTTP/2 specific headers */
        const axis2_char_t* upgrade_header = apr_table_get(request->headers_in, "Upgrade");
        if (upgrade_header && strstr(upgrade_header, "h2c"))
        {
            result->is_http2 = AXIS2_TRUE;
            result->requires_thread_safety = AXIS2_TRUE;
            AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
                "REQUEST ANALYSIS: HTTP/2 detected via Upgrade header: %s", upgrade_header);
        }
    }

    AXIS2_LOG_DEBUG(env->log, AXIS2_LOG_SI,
        "REQUEST ANALYSIS COMPLETE: HTTP2=%d, JSON=%d, SOAP=%d, ThreadSafe=%d",
        result->is_http2, result->is_json_content, result->is_soap_content, result->requires_thread_safety);

    return result;
}

/**
 * @brief Free request analysis result
 */
static void
axis2_apache2_request_processor_factory_free_analysis_result(
    const axutil_env_t* env,
    axis2_request_analysis_result_t* result)
{
    if (result)
    {
        /* Note: content_type and protocol_version are allocated from request pool,
         * so they don't need explicit freeing */
        AXIS2_FREE(env->allocator, result);
    }
}

/**
 * @brief Utility function implementation for JSON HTTP/2 detection
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_apache2_request_processor_is_json_http2_request(
    request_rec* request)
{
    const axis2_char_t* content_type = NULL;
    const axis2_char_t* protocol = NULL;
    axis2_bool_t result = AXIS2_FALSE;

    if (!request) {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, request->server,
            "[JSON_HTTP2_DETECTION] Request is NULL");
        return AXIS2_FALSE;
    }

    /* Check protocol version */
    protocol = request->protocol;
    ap_log_error(APLOG_MARK, APLOG_INFO, 0, request->server,
        "[JSON_HTTP2_DETECTION] Protocol: '%s'", protocol ? protocol : "NULL");

    if (!protocol || (!strstr(protocol, "HTTP/2") && !strstr(protocol, "HTTP/2.0")))
    {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, request->server,
            "[JSON_HTTP2_DETECTION] Protocol check FAILED - not HTTP/2");
        return AXIS2_FALSE;
    }

    ap_log_error(APLOG_MARK, APLOG_INFO, 0, request->server,
        "[JSON_HTTP2_DETECTION] Protocol check PASSED - is HTTP/2");

    /* Check content type */
    content_type = apr_table_get(request->headers_in, "Content-Type");
    if (!content_type)
    {
        content_type = apr_table_get(request->headers_in, "content-type");
    }

    ap_log_error(APLOG_MARK, APLOG_INFO, 0, request->server,
        "[JSON_HTTP2_DETECTION] Content-Type: '%s'", content_type ? content_type : "NULL");

    if (!content_type) {
        ap_log_error(APLOG_MARK, APLOG_ERR, 0, request->server,
            "[JSON_HTTP2_DETECTION] Content-Type check FAILED - no content type");
        return AXIS2_FALSE;
    }

    /* JSON content type detection */
    result = (strstr(content_type, "application/json") != NULL ||
              strstr(content_type, "text/json") != NULL ||
              strstr(content_type, "application/hal+json") != NULL ||
              strstr(content_type, "application/vnd.api+json") != NULL) ?
             AXIS2_TRUE : AXIS2_FALSE;

    ap_log_error(APLOG_MARK, APLOG_INFO, 0, request->server,
        "[JSON_HTTP2_DETECTION] Final result: %s", result ? "TRUE (interface processing)" : "FALSE (legacy processing)");

    return result;
}