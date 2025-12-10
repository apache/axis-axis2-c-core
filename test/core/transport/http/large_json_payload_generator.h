/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef LARGE_JSON_PAYLOAD_GENERATOR_H
#define LARGE_JSON_PAYLOAD_GENERATOR_H

#include <axutil_utils_defines.h>
#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @file large_json_payload_generator.h
 * @brief Large JSON payload generator for testing HTTP/2 transport with big data structures.
 *
 * This utility generates realistic big data structures for enterprise applications:
 * - 50MB+ JSON payloads for performance testing
 * - Realistic data structures (records, calculations, metrics)
 * - Memory-efficient generation for 2GB heap constraints
 * - Streaming-friendly JSON structure for HTTP/2 flow control
 */

/**
 * Generate large JSON payload simulating big data structures
 *
 * @param env Environment
 * @param target_size_bytes Target size in bytes
 * @return JSON string of approximately the target size (caller must free)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
large_json_payload_generator_generate_big_data_structure(
    const axutil_env_t* env,
    int target_size_bytes);

/**
 * Generate streaming-optimized JSON for flow control testing
 *
 * @param env Environment
 * @param target_size_bytes Target size in bytes
 * @return JSON string structured for efficient streaming (caller must free)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
large_json_payload_generator_generate_streaming_json(
    const axutil_env_t* env,
    int target_size_bytes);

/**
 * Generate simple large JSON with repeated structures
 *
 * @param env Environment
 * @param target_size_bytes Target size in bytes
 * @return Simple JSON string for basic testing (caller must free)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
large_json_payload_generator_generate_simple_large_json(
    const axutil_env_t* env,
    int target_size_bytes);

/**
 * Calculate actual byte size of a string
 *
 * @param json_string JSON string to measure
 * @return Actual byte size
 */
AXIS2_EXTERN int AXIS2_CALL
large_json_payload_generator_get_actual_byte_size(
    const axis2_char_t* json_string);

/**
 * Validate generated JSON size is within tolerance
 *
 * @param json_string JSON string to validate
 * @param target_size Target size in bytes
 * @param tolerance Tolerance as decimal (e.g. 0.1 for 10%)
 * @return AXIS2_TRUE if within tolerance, AXIS2_FALSE otherwise
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
large_json_payload_generator_validate_size(
    const axis2_char_t* json_string,
    int target_size,
    double tolerance);

#ifdef __cplusplus
}
#endif

#endif