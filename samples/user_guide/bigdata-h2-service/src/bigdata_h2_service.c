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

/**
 * @file bigdata_h2_service.c
 * @brief Apache Axis2/C HTTP/2 Big Data Service Implementation
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
 */

#include "bigdata_h2_service.h"
#include "axis2_json_rpc_msg_recv_native.h"
#include <axutil_string.h>
#include <axutil_utils.h>
#include <axutil_hash.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/* Internal utility functions */
static long get_current_time_ms(void);
static axis2_char_t* format_size_bytes(long bytes, const axutil_env_t *env);
static axis2_char_t* format_processing_time(long time_ms, const axutil_env_t *env);

/**
 * Create Big Data Request from JSON string using pure json-c
 */
AXIS2_EXTERN bigdata_h2_request_t* AXIS2_CALL
bigdata_h2_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    bigdata_h2_request_t *request = NULL;
    json_object *json_obj = NULL;
    json_object *value_obj = NULL;

    if (!env || !json_string)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    /* Parse JSON using json-c */
    json_obj = json_tokener_parse(json_string);
    if (!json_obj)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse JSON request");
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(bigdata_h2_request_t));
    if (!request)
    {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(bigdata_h2_request_t));

    /* Extract dataset_id */
    if (json_object_object_get_ex(json_obj, "datasetId", &value_obj))
    {
        const char *dataset_id = json_object_get_string(value_obj);
        if (dataset_id)
        {
            request->dataset_id = axutil_strdup(env, dataset_id);
        }
    }

    /* Extract dataset_size */
    if (json_object_object_get_ex(json_obj, "datasetSize", &value_obj))
    {
        request->dataset_size = json_object_get_int64(value_obj);
    }

    /* Extract analytics_type */
    if (json_object_object_get_ex(json_obj, "analyticsType", &value_obj))
    {
        const char *analytics_type = json_object_get_string(value_obj);
        if (analytics_type)
        {
            request->analytics_type = axutil_strdup(env, analytics_type);
        }
    }
    else
    {
        /* Default analytics type */
        request->analytics_type = axutil_strdup(env, BIGDATA_H2_ANALYTICS_STANDARD);
    }

    /* Extract processing_mode (optional - auto-determined if not provided) */
    if (json_object_object_get_ex(json_obj, "processingMode", &value_obj))
    {
        const char *processing_mode = json_object_get_string(value_obj);
        if (processing_mode)
        {
            request->processing_mode = axutil_strdup(env, processing_mode);
        }
    }

    /* Extract HTTP/2 optimization flag */
    if (json_object_object_get_ex(json_obj, "enableHttp2Optimization", &value_obj))
    {
        request->enable_http2_optimization = json_object_get_boolean(value_obj) ? AXIS2_TRUE : AXIS2_FALSE;
    }
    else
    {
        request->enable_http2_optimization = AXIS2_TRUE; /* Default enabled */
    }

    /* Extract memory optimization flag */
    if (json_object_object_get_ex(json_obj, "enableMemoryOptimization", &value_obj))
    {
        request->enable_memory_optimization = json_object_get_boolean(value_obj) ? AXIS2_TRUE : AXIS2_FALSE;
    }
    else
    {
        request->enable_memory_optimization = AXIS2_TRUE; /* Default enabled */
    }

    /* Initialize custom properties hash */
    request->custom_properties = axutil_hash_make(env);

    json_object_put(json_obj);
    return request;
}

/**
 * Create Big Data Request
 */
AXIS2_EXTERN bigdata_h2_request_t* AXIS2_CALL
bigdata_h2_request_create(
    const axutil_env_t *env,
    const axis2_char_t *dataset_id,
    long dataset_size)
{
    bigdata_h2_request_t *request = NULL;

    if (!env || !dataset_id)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(bigdata_h2_request_t));
    if (!request)
    {
        return NULL;
    }
    memset(request, 0, sizeof(bigdata_h2_request_t));

    request->dataset_id = axutil_strdup(env, dataset_id);
    request->dataset_size = dataset_size;
    request->analytics_type = axutil_strdup(env, BIGDATA_H2_ANALYTICS_STANDARD);
    request->enable_http2_optimization = AXIS2_TRUE;
    request->enable_memory_optimization = AXIS2_TRUE;
    request->custom_properties = axutil_hash_make(env);

    return request;
}

/**
 * Free Big Data Request
 */
AXIS2_EXTERN void AXIS2_CALL
bigdata_h2_request_free(
    bigdata_h2_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env)
        return;

    if (request->dataset_id)
        AXIS2_FREE(env->allocator, request->dataset_id);

    if (request->analytics_type)
        AXIS2_FREE(env->allocator, request->analytics_type);

    if (request->processing_mode)
        AXIS2_FREE(env->allocator, request->processing_mode);

    if (request->custom_properties)
        axutil_hash_free(request->custom_properties, env);

    AXIS2_FREE(env->allocator, request);
}

/**
 * Create Big Data Response
 */
AXIS2_EXTERN bigdata_h2_response_t* AXIS2_CALL
bigdata_h2_response_create(
    const axutil_env_t *env)
{
    bigdata_h2_response_t *response = NULL;

    if (!env)
    {
        return NULL;
    }

    response = AXIS2_MALLOC(env->allocator, sizeof(bigdata_h2_response_t));
    if (!response)
    {
        return NULL;
    }
    memset(response, 0, sizeof(bigdata_h2_response_t));

    response->status = axutil_strdup(env, BIGDATA_H2_STATUS_PENDING);
    response->memory_optimized = AXIS2_FALSE;
    response->http2_optimized = AXIS2_FALSE;
    response->concurrent_streams = 1;

    return response;
}

/**
 * Convert response to pure JSON string using json-c
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_response_to_json(
    const bigdata_h2_response_t *response,
    const axutil_env_t *env)
{
    json_object *json_response = NULL;
    json_object *json_array = NULL;
    const char *json_string = NULL;
    axis2_char_t *result = NULL;

    if (!response || !env)
    {
        return NULL;
    }

    json_response = json_object_new_object();
    if (!json_response)
    {
        return NULL;
    }

    /* Add status */
    if (response->status)
    {
        json_object_object_add(json_response, "status",
                               json_object_new_string(response->status));
    }

    /* Add error message if present */
    if (response->error_message)
    {
        json_object_object_add(json_response, "errorMessage",
                               json_object_new_string(response->error_message));
    }

    /* Add processing time */
    json_object_object_add(json_response, "processingTimeMs",
                           json_object_new_int64(response->processing_time_ms));

    /* Add processed record count */
    json_object_object_add(json_response, "processedRecordCount",
                           json_object_new_int(response->processed_record_count));

    /* Add total processed bytes */
    json_object_object_add(json_response, "totalProcessedBytes",
                           json_object_new_int64(response->total_processed_bytes));

    /* Add result summary if present */
    if (response->result_summary)
    {
        json_object_object_add(json_response, "resultSummary",
                               json_object_new_string(response->result_summary));
    }

    /* Add optimization indicators */
    json_object_object_add(json_response, "memoryOptimized",
                           json_object_new_boolean(response->memory_optimized));
    json_object_object_add(json_response, "http2Optimized",
                           json_object_new_boolean(response->http2_optimized));

    if (response->optimization_details)
    {
        json_object_object_add(json_response, "optimizationDetails",
                               json_object_new_string(response->optimization_details));
    }

    /* Add performance metrics */
    json_object_object_add(json_response, "memoryUsedBytes",
                           json_object_new_int64(response->memory_used_bytes));
    json_object_object_add(json_response, "throughputMbps",
                           json_object_new_double(response->throughput_mbps));
    json_object_object_add(json_response, "concurrentStreams",
                           json_object_new_int(response->concurrent_streams));

    /* Convert to string */
    json_string = json_object_to_json_string(json_response);
    if (json_string)
    {
        result = axutil_strdup(env, json_string);
    }

    json_object_put(json_response);
    return result;
}

/**
 * Free Big Data Response
 */
AXIS2_EXTERN void AXIS2_CALL
bigdata_h2_response_free(
    bigdata_h2_response_t *response,
    const axutil_env_t *env)
{
    int i;

    if (!response || !env)
        return;

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);

    if (response->error_message)
        AXIS2_FREE(env->allocator, response->error_message);

    if (response->result_summary)
        AXIS2_FREE(env->allocator, response->result_summary);

    if (response->optimization_details)
        AXIS2_FREE(env->allocator, response->optimization_details);

    /* Free processed records */
    if (response->processed_records)
    {
        for (i = 0; i < response->processed_record_count; i++)
        {
            if (response->processed_records[i])
            {
                if (response->processed_records[i]->record_id)
                    AXIS2_FREE(env->allocator, response->processed_records[i]->record_id);
                if (response->processed_records[i]->data)
                    AXIS2_FREE(env->allocator, response->processed_records[i]->data);
                AXIS2_FREE(env->allocator, response->processed_records[i]);
            }
        }
        AXIS2_FREE(env->allocator, response->processed_records);
    }

    AXIS2_FREE(env->allocator, response);
}

/**
 * Main Big Data Processing Service
 */
AXIS2_EXTERN bigdata_h2_response_t* AXIS2_CALL
bigdata_h2_service_process_big_data_set(
    const axutil_env_t *env,
    bigdata_h2_request_t *request)
{
    bigdata_h2_response_t *response = NULL;
    long start_time, end_time;
    axis2_char_t *processing_mode = NULL;
    axis2_char_t *optimization_details = NULL;
    int simulated_record_count = 0;

    if (!env || !request)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "Processing big data set: dataset_id=%s, size=%ld bytes",
                   request->dataset_id ? request->dataset_id : "unknown",
                   request->dataset_size);

    response = bigdata_h2_response_create(env);
    if (!response)
    {
        return NULL;
    }

    start_time = get_current_time_ms();

    /* Determine optimal processing mode if not explicitly set */
    if (!request->processing_mode)
    {
        processing_mode = bigdata_h2_request_determine_optimal_processing_mode(request, env);
        request->processing_mode = processing_mode;
    }

    /* Simulate big data processing based on mode */
    if (axutil_strcmp(request->processing_mode, BIGDATA_H2_MODE_STREAMING) == 0)
    {
        /* Streaming mode for very large datasets (>50MB) */
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Using HTTP/2 streaming mode for large dataset");

        response->http2_optimized = AXIS2_TRUE;
        response->memory_optimized = AXIS2_TRUE;
        response->concurrent_streams = 10; /* HTTP/2 multiplexing */

        /* Simulate streaming processing */
        simulated_record_count = (int)(request->dataset_size / 1024); /* 1 record per KB */
        if (simulated_record_count > 100000)
            simulated_record_count = 100000; /* Cap for demo */

        optimization_details = axutil_strdup(env, "HTTP/2 streaming with 10 concurrent streams, "
                                                  "memory-efficient chunk processing");
    }
    else if (axutil_strcmp(request->processing_mode, BIGDATA_H2_MODE_MULTIPLEXING) == 0)
    {
        /* Multiplexing mode for medium datasets (10-50MB) */
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Using HTTP/2 multiplexing mode for medium dataset");

        response->http2_optimized = AXIS2_TRUE;
        response->memory_optimized = AXIS2_TRUE;
        response->concurrent_streams = 5; /* Moderate multiplexing */

        simulated_record_count = (int)(request->dataset_size / 512); /* 1 record per 512 bytes */
        if (simulated_record_count > 50000)
            simulated_record_count = 50000;

        optimization_details = axutil_strdup(env, "HTTP/2 multiplexing with 5 concurrent streams, "
                                                  "batch processing optimization");
    }
    else
    {
        /* Standard mode for smaller datasets (<10MB) */
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Using standard processing mode for small dataset");

        response->http2_optimized = request->enable_http2_optimization;
        response->memory_optimized = request->enable_memory_optimization;
        response->concurrent_streams = 1;

        simulated_record_count = (int)(request->dataset_size / 256); /* 1 record per 256 bytes */
        if (simulated_record_count > 10000)
            simulated_record_count = 10000;

        optimization_details = axutil_strdup(env, "Standard processing with optional HTTP/2 features");
    }

    /* Simulate processing delay based on dataset size */
    if (request->dataset_size > BIGDATA_H2_MULTIPLEXING_THRESHOLD)
    {
        /* Large datasets: simulate longer processing */
        usleep(50000); /* 50ms */
    }
    else if (request->dataset_size > BIGDATA_H2_STANDARD_THRESHOLD)
    {
        /* Medium datasets */
        usleep(20000); /* 20ms */
    }
    else
    {
        /* Small datasets */
        usleep(5000); /* 5ms */
    }

    end_time = get_current_time_ms();

    /* Build response */
    if (response->status)
        AXIS2_FREE(env->allocator, response->status);
    response->status = axutil_strdup(env, BIGDATA_H2_STATUS_SUCCESS);

    response->processing_time_ms = end_time - start_time;
    response->processed_record_count = simulated_record_count;
    response->total_processed_bytes = request->dataset_size;
    response->optimization_details = optimization_details;

    /* Calculate performance metrics */
    bigdata_h2_response_calculate_throughput(response);
    response->memory_used_bytes = request->dataset_size / 4; /* Simulated memory usage */

    /* Create result summary */
    response->result_summary = axutil_stracat(env,
        "Successfully processed ",
        axutil_strcat(env,
            format_size_bytes(request->dataset_size, env),
            " in ",
            format_processing_time(response->processing_time_ms, env)));

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI,
                   "Big data processing completed: %d records, %ld bytes, %ld ms",
                   response->processed_record_count,
                   response->total_processed_bytes,
                   response->processing_time_ms);

    return response;
}

/**
 * Get formatted dataset size
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_request_get_formatted_dataset_size(
    const bigdata_h2_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env)
        return NULL;

    return format_size_bytes(request->dataset_size, env);
}

/**
 * Check if dataset requires streaming (>50MB)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_request_requires_streaming(
    const bigdata_h2_request_t *request)
{
    if (!request)
        return AXIS2_FALSE;

    return request->dataset_size > BIGDATA_H2_MULTIPLEXING_THRESHOLD ? AXIS2_TRUE : AXIS2_FALSE;
}

/**
 * Check if dataset benefits from multiplexing (10-50MB)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_request_benefits_from_multiplexing(
    const bigdata_h2_request_t *request)
{
    if (!request)
        return AXIS2_FALSE;

    return (request->dataset_size > BIGDATA_H2_STANDARD_THRESHOLD &&
            request->dataset_size <= BIGDATA_H2_MULTIPLEXING_THRESHOLD) ? AXIS2_TRUE : AXIS2_FALSE;
}

/**
 * Determine optimal processing mode based on size
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_request_determine_optimal_processing_mode(
    const bigdata_h2_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env)
        return NULL;

    if (bigdata_h2_request_requires_streaming(request))
    {
        return axutil_strdup(env, BIGDATA_H2_MODE_STREAMING);
    }
    else if (bigdata_h2_request_benefits_from_multiplexing(request))
    {
        return axutil_strdup(env, BIGDATA_H2_MODE_MULTIPLEXING);
    }
    else
    {
        return axutil_strdup(env, BIGDATA_H2_MODE_STANDARD);
    }
}

/**
 * Calculate throughput in response
 */
AXIS2_EXTERN void AXIS2_CALL
bigdata_h2_response_calculate_throughput(
    bigdata_h2_response_t *response)
{
    if (!response || response->processing_time_ms <= 0)
        return;

    /* Calculate MB/s throughput */
    double mb_processed = response->total_processed_bytes / (1024.0 * 1024.0);
    double seconds = response->processing_time_ms / 1000.0;
    response->throughput_mbps = mb_processed / seconds;
}

/**
 * Get formatted processing time
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_response_get_formatted_processing_time(
    const bigdata_h2_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env)
        return NULL;

    return format_processing_time(response->processing_time_ms, env);
}

/**
 * Get formatted processed size
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_response_get_formatted_processed_size(
    const bigdata_h2_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env)
        return NULL;

    return format_size_bytes(response->total_processed_bytes, env);
}

/**
 * Check if response was successful
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_response_is_successful(
    const bigdata_h2_response_t *response)
{
    if (!response || !response->status)
        return AXIS2_FALSE;

    return axutil_strcmp(response->status, BIGDATA_H2_STATUS_SUCCESS) == 0 ? AXIS2_TRUE : AXIS2_FALSE;
}

/**
 * Check if large dataset processing was used (>50MB)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
bigdata_h2_response_is_large_dataset_processing(
    const bigdata_h2_response_t *response)
{
    if (!response)
        return AXIS2_FALSE;

    return response->total_processed_bytes > BIGDATA_H2_MULTIPLEXING_THRESHOLD ? AXIS2_TRUE : AXIS2_FALSE;
}

/**
 * Generate test JSON payload using pure json-c
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
bigdata_h2_generate_test_json_payload(
    const axutil_env_t *env,
    long target_size_bytes)
{
    json_object *json_root = NULL;
    json_object *json_records = NULL;
    json_object *json_record = NULL;
    const char *json_string = NULL;
    axis2_char_t *result = NULL;
    int record_count = 0;
    int i;
    char record_id[64];
    char data_field[256];

    if (!env || target_size_bytes <= 0)
        return NULL;

    /* Estimate records needed (approximate 200 bytes per record) */
    record_count = (int)(target_size_bytes / 200);
    if (record_count > 1000000)  /* Cap for safety */
        record_count = 1000000;

    json_root = json_object_new_object();
    if (!json_root)
        return NULL;

    /* Add metadata */
    json_object_object_add(json_root, "datasetId", json_object_new_string("test_dataset"));
    json_object_object_add(json_root, "datasetSize", json_object_new_int64(target_size_bytes));
    json_object_object_add(json_root, "analyticsType", json_object_new_string(BIGDATA_H2_ANALYTICS_STANDARD));
    json_object_object_add(json_root, "generatedRecords", json_object_new_int(record_count));

    /* Create records array */
    json_records = json_object_new_array();
    if (json_records)
    {
        for (i = 0; i < record_count; i++)
        {
            json_record = json_object_new_object();
            if (json_record)
            {
                snprintf(record_id, sizeof(record_id), "record_%06d", i + 1);
                snprintf(data_field, sizeof(data_field),
                         "Sample data payload for record %d with HTTP/2 JSON processing capabilities",
                         i + 1);

                json_object_object_add(json_record, "recordId", json_object_new_string(record_id));
                json_object_object_add(json_record, "data", json_object_new_string(data_field));
                json_object_object_add(json_record, "timestamp", json_object_new_int64(get_current_time_ms()));
                json_object_object_add(json_record, "sizeBytes", json_object_new_int(strlen(data_field)));

                json_object_array_add(json_records, json_record);
            }
        }
        json_object_object_add(json_root, "records", json_records);
    }

    /* Convert to string */
    json_string = json_object_to_json_string(json_root);
    if (json_string)
    {
        result = axutil_strdup(env, json_string);
    }

    json_object_put(json_root);
    return result;
}

/* Internal utility functions */

static long get_current_time_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

static axis2_char_t* format_size_bytes(long bytes, const axutil_env_t *env)
{
    axis2_char_t *formatted = NULL;

    if (!env)
        return NULL;

    formatted = AXIS2_MALLOC(env->allocator, 64);
    if (!formatted)
        return NULL;

    if (bytes >= (1024 * 1024 * 1024))
    {
        snprintf(formatted, 64, "%.2f GB", bytes / (1024.0 * 1024.0 * 1024.0));
    }
    else if (bytes >= (1024 * 1024))
    {
        snprintf(formatted, 64, "%.2f MB", bytes / (1024.0 * 1024.0));
    }
    else if (bytes >= 1024)
    {
        snprintf(formatted, 64, "%.2f KB", bytes / 1024.0);
    }
    else
    {
        snprintf(formatted, 64, "%ld bytes", bytes);
    }

    return formatted;
}

static axis2_char_t* format_processing_time(long time_ms, const axutil_env_t *env)
{
    axis2_char_t *formatted = NULL;

    if (!env)
        return NULL;

    formatted = AXIS2_MALLOC(env->allocator, 64);
    if (!formatted)
        return NULL;

    if (time_ms >= 1000)
    {
        snprintf(formatted, 64, "%.2f seconds", time_ms / 1000.0);
    }
    else
    {
        snprintf(formatted, 64, "%ld ms", time_ms);
    }

    return formatted;
}

/**
 * @brief Export custom message receiver creation function for Axis2/C dynamic loading
 * This ensures axis2_json_rpc_msg_recv_simple_create is available in the symbol table
 */

/* Forward declaration */
AXIS2_EXTERN axis2_msg_recv_t* AXIS2_CALL
axis2_json_rpc_msg_recv_simple_create(const axutil_env_t* env);

/* The actual implementation is in axis2_json_rpc_msg_recv_simple.c */