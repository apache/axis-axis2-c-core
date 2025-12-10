/*
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef AXIS2_JSON_READER_H
#define AXIS2_JSON_READER_H

#include <axutil_utils.h>

#ifdef WITH_NGHTTP2
#include <json-c/json.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct axutil_stream axutil_stream_t;
/* HTTP/2 Pure JSON Architecture - axiom_node_t typedef removed */
typedef struct axutil_env axutil_env_t;
typedef struct axis2_json_reader axis2_json_reader_t;

/**
 * @brief Creates JSON reader to read data from stream
 * @param env Environment
 * @return void* pointer to JSON object (cast to json_object* for json-c usage)
 * @param stream Stream to read data from
 */
AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_stream(
        const axutil_env_t* env,
        axutil_stream_t* stream);


/**
 * @brief Creates JSON reader to read data from string
 * @param env Environment
 * @return void* pointer to JSON object (cast to json_object* for json-c usage)
 * @param json_string JSON string
 * @param json_string_size JSON string
 */
AXIS2_EXTERN axis2_json_reader_t* AXIS2_CALL
axis2_json_reader_create_for_memory(
        const axutil_env_t* env,
        const axis2_char_t* json_string,
        int json_string_size);


/**
 * @brief Destroys reader
 *  does not free root node.
 *  user must free axiom tree using axiom_node_free_tree()
 * @param reader JSON reader
 * @param env Environment
 * @return void* pointer to JSON object (cast to json_object* for json-c usage)
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_reader_free(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);


/**
 * @brief Starts reading from string or stream
 * @param reader JSON reader
 * @param env Environment
 * @return void* pointer to JSON object (cast to json_object* for json-c usage)
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_json_reader_read(
        axis2_json_reader_t* reader,
        const axutil_env_t* env);


/**
 * @brief Gets root JSON data of parsed data (HTTP/2 Pure JSON Architecture)
 * @param reader JSON reader
 * @param env Environment
 * @return void* pointer to JSON object (cast to json_object* for json-c usage)
 */
AXIS2_EXTERN void* AXIS2_CALL
axis2_json_reader_get_root_node(
    axis2_json_reader_t* reader,
    const axutil_env_t* env);

/* REVOLUTIONARY HTTP/2 JSON PROCESSING ENHANCEMENTS - Option B Framework */

#ifdef WITH_NGHTTP2
/**
 * REVOLUTIONARY: HTTP/2 Streaming JSON Reader - A Breed Apart
 * Enhanced for high-performance HTTP/2 multiplexed streams
 * Option B compliant - enhances existing functionality without replacement
 *
 * @param reader JSON reader instance
 * @param env Environment
 * @param stream HTTP/2 multiplexed stream
 * @param enable_streaming Enable streaming mode for large payloads
 * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on error
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_json_reader_read_http2_stream(
        axis2_json_reader_t* reader,
        const axutil_env_t* env,
        axutil_stream_t* stream,
        axis2_bool_t enable_streaming);

/**
 * REVOLUTIONARY: HTTP/2 Direct JSON Response Generator - Breed Apart Architecture
 * Bypasses XML/AXIOM transformation entirely for maximum performance
 * Option B compliant - enhances existing framework with revolutionary capabilities
 *
 * @param env Environment
 * @param service_name Name of the service generating the response
 * @param request_json Original request JSON object (can be NULL)
 * @param response_json_str Output parameter for generated JSON response string
 * @return AXIS2_SUCCESS on success, AXIS2_FAILURE on error
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_json_reader_generate_http2_response(
        const axutil_env_t* env,
        const char* service_name,
        json_object* request_json,
        char** response_json_str);

/**
 * REVOLUTIONARY: HTTP/2 JSON Validation - Enhanced Schema Processing
 * Option B compliant validation with HTTP/2 performance optimization
 *
 * @param env Environment
 * @param json_obj JSON object to validate
 * @param service_name Service name for service-specific validation
 * @return AXIS2_TRUE if valid, AXIS2_FALSE if invalid
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
axis2_json_reader_validate_http2_request(
        const axutil_env_t* env,
        json_object* json_obj,
        const char* service_name);
#endif /* WITH_NGHTTP2 */

#ifdef __cplusplus
}
#endif

#endif
