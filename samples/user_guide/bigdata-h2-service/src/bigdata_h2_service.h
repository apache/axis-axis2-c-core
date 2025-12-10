/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements. See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BIGDATA_H2_SERVICE_H
#define BIGDATA_H2_SERVICE_H

/**
 * @file bigdata_h2_service.h
 * @brief Apache Axis2/C HTTP/2 Big Data Service Header
 *
 * Port of Axis2/Java Spring Boot BigDataH2Service to Axis2/C with Apache httpd.
 * Uses pure json-c library for JSON processing (no XML/SOAP dependencies).
 *
 * Features:
 * - HTTP/2 transport with streaming optimization
 * - Pure json-c JSON processing for large payloads (50MB+)
 * - Three processing modes: standard, multiplexing, streaming
 * - Memory-efficient processing within constraints
 * - Performance metrics and optimization indicators
 * - Compatible with Axis2/Java BigDataH2Service API
 */

#include <axutil_env.h>
#include <axutil_hash.h>
#include <json-c/json.h>
#include <time.h>
/* HTTP/2 Pure JSON Architecture - axiom_node.h removed */

#ifdef __cplusplus
extern "C"
{
#endif

/** Processing mode constants - matches Axis2/Java */
#define BIGDATA_H2_MODE_STANDARD    "standard"
#define BIGDATA_H2_MODE_MULTIPLEXING "multiplexing"
#define BIGDATA_H2_MODE_STREAMING   "streaming"

/** Size thresholds - matches Axis2/Java */
#define BIGDATA_H2_STANDARD_THRESHOLD    (10 * 1024 * 1024)  /* 10MB */
#define BIGDATA_H2_MULTIPLEXING_THRESHOLD (50 * 1024 * 1024) /* 50MB */
#define BIGDATA_H2_MEMORY_CONSTRAINT     (2048 * 1024 * 1024) /* 2GB */

/** Analytics types - matches Axis2/Java */
#define BIGDATA_H2_ANALYTICS_STANDARD    "standard_analytics"
#define BIGDATA_H2_ANALYTICS_ADVANCED    "advanced_analytics"
#define BIGDATA_H2_ANALYTICS_ENTERPRISE  "enterprise_big_data"
#define BIGDATA_H2_ANALYTICS_CONCURRENT  "concurrent_analytics"

/** Status constants */
#define BIGDATA_H2_STATUS_SUCCESS    "SUCCESS"
#define BIGDATA_H2_STATUS_FAILED     "FAILED"
#define BIGDATA_H2_STATUS_PENDING    "PENDING"

/**
 * @brief Big Data Request structure (equivalent to BigDataH2Request.java)
 */
typedef struct bigdata_h2_request
{
    /** Dataset identifier */
    axis2_char_t *dataset_id;

    /** Dataset size in bytes */
    long dataset_size;

    /** Analytics type */
    axis2_char_t *analytics_type;

    /** Processing mode (auto-determined or explicit) */
    axis2_char_t *processing_mode;

    /** Enable HTTP/2 optimization */
    axis2_bool_t enable_http2_optimization;

    /** Memory constraint awareness */
    axis2_bool_t enable_memory_optimization;

    /** Custom properties */
    axutil_hash_t *custom_properties;

} bigdata_h2_request_t;

/**
 * @brief Data Record structure (equivalent to BigDataH2Service.DataRecord.java)
 */
typedef struct bigdata_h2_data_record
{
    /** Record ID */
    axis2_char_t *record_id;

    /** Record data (JSON string) */
    axis2_char_t *data;

    /** Record timestamp */
    time_t timestamp;

    /** Record size in bytes */
    long size_bytes;

} bigdata_h2_data_record_t;

/**
 * @brief Big Data Response structure (equivalent to BigDataH2Response.java)
 */
typedef struct bigdata_h2_response
{
    /** Processing status */
    axis2_char_t *status;

    /** Error message (if any) */
    axis2_char_t *error_message;

    /** Processing time in milliseconds */
    long processing_time_ms;

    /** Result data - processed records */
    bigdata_h2_data_record_t **processed_records;

    /** Number of processed records */
    int processed_record_count;

    /** Total processed bytes */
    long total_processed_bytes;

    /** Result summary */
    axis2_char_t *result_summary;

    /** HTTP/2 optimization indicators */
    axis2_bool_t memory_optimized;
    axis2_bool_t http2_optimized;
    axis2_char_t *optimization_details;

    /** Performance metrics */
    long memory_used_bytes;
    double throughput_mbps;
    int concurrent_streams;

} bigdata_h2_response_t;

/**
 * @brief Create Big Data Request using pure json-c parsing
 */
AXIS2_EXTERN bigdata_h2_request_t* AXIS2_CALL
bigdata_h2_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string);

/**
 * @brief Create Big Data Request
 */
AXIS2_EXTERN bigdata_h2_request_t* AXIS2_CALL
bigdata_h2_request_create(
    const axutil_env_t *env,
    const axis2_char_t *dataset_id,
    long dataset_size);

/**
 * @brief Free Big Data Request
 */
AXIS2_EXTERN void AXIS2_CALL
bigdata_h2_request_free(
    bigdata_h2_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Create Big Data Response
 */
AXIS2_EXTERN bigdata_h2_response_t* AXIS2_CALL
bigdata_h2_response_create(
    const axutil_env_t *env);

/**
 * @brief Convert response to pure JSON string using json-c
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_response_to_json(
    const bigdata_h2_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Free Big Data Response
 */
AXIS2_EXTERN void AXIS2_CALL
bigdata_h2_response_free(
    bigdata_h2_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Main Big Data Processing Service (equivalent to BigDataH2Service.processBigDataSet)
 *
 * This is the main service operation that processes big data sets using
 * HTTP/2 transport with different optimization modes based on payload size.
 * Uses pure json-c for all JSON processing (no XML/axiom dependencies).
 *
 * @param env Environment
 * @param request Big data processing request
 * @return Big data processing response
 */
AXIS2_EXTERN bigdata_h2_response_t* AXIS2_CALL
bigdata_h2_service_process_big_data_set(
    const axutil_env_t *env,
    bigdata_h2_request_t *request);

/**
 * @brief Get formatted dataset size (equivalent to Java getFormattedDatasetSize)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_request_get_formatted_dataset_size(
    const bigdata_h2_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Check if dataset requires streaming (>50MB)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_request_requires_streaming(
    const bigdata_h2_request_t *request);

/**
 * @brief Check if dataset benefits from multiplexing (10-50MB)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_request_benefits_from_multiplexing(
    const bigdata_h2_request_t *request);

/**
 * @brief Determine optimal processing mode based on size
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_request_determine_optimal_processing_mode(
    const bigdata_h2_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Calculate throughput in response
 */
AXIS2_EXTERN void AXIS2_CALL
bigdata_h2_response_calculate_throughput(
    bigdata_h2_response_t *response);

/**
 * @brief Get formatted processing time
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_response_get_formatted_processing_time(
    const bigdata_h2_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Get formatted processed size
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_response_get_formatted_processed_size(
    const bigdata_h2_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Check if response was successful
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_response_is_successful(
    const bigdata_h2_response_t *response);

/**
 * @brief Check if large dataset processing was used (>50MB)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_response_is_large_dataset_processing(
    const bigdata_h2_response_t *response);

/**
 * @brief Generate test JSON payload using pure json-c (for testing/demo purposes)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_generate_test_json_payload(
    const axutil_env_t *env,
    long target_size_bytes);

/**
 * @brief Pure JSON processing functions for enableJSONOnly=true
 * Following Axis2/Java JsonRpcMessageReceiver pattern
 */

/**
 * @brief Process big data set with pure JSON (enableJSONOnly=true)
 * No AXIOM conversion - direct json-c processing
 * @param env Environment
 * @param json_request JSON request string
 * @return JSON response string (caller must free)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_service_process_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

/**
 * @brief Get service metadata as JSON (enableJSONOnly=true)
 * Pure JSON response without AXIOM conversion
 * @param env Environment
 * @param json_request JSON request string (ignored for metadata)
 * @return JSON metadata response (caller must free)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_service_get_metadata_json_only(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

#ifdef __cplusplus
}
#endif

#endif /* BIGDATA_H2_SERVICE_H */