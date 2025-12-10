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

#ifndef TESTWS_SERVICE_H
#define TESTWS_SERVICE_H

/**
 * @file testws_service.h
 * @brief Apache Axis2/C HTTP/2 TestWS Service Header
 *
 * Pure JSON XSS protection demonstration service using json-c library.
 * Port of Axis2/Java Spring Boot TestwsService to Axis2/C with Apache httpd.
 * Eliminates all SOAP patterns (no skeleton, stub, epr, axiom).
 *
 * Features:
 * - XSS attack prevention through HTML encoding
 * - OWASP ESAPI-style input validation
 * - HTTP/2 transport optimization
 * - Pure JSON processing (no axiom/XML)
 * - Educational security demonstration
 * - Compatible with Axis2/Java TestwsService API
 */

#include <axutil_env.h>
#include <json-c/json.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Service status constants */
#define TESTWS_STATUS_OK           "OK"
#define TESTWS_STATUS_ERROR        "ERROR"
#define TESTWS_STATUS_INVALID      "INVALID_REQUEST"

/** Validation limits */
#define TESTWS_MESSAGE_MAX_LENGTH  100

/**
 * @brief TestWS Request structure (equivalent to TestwsRequest.java)
 */
typedef struct testws_request
{
    /** Input message (potentially containing XSS payloads) */
    axis2_char_t *message_in;

    /** Request timestamp */
    time_t request_time;

    /** Client IP address (for security logging) */
    axis2_char_t *client_ip;

} testws_request_t;

/**
 * @brief TestWS Response structure (equivalent to TestwsResponse.java)
 */
typedef struct testws_response
{
    /** Processing status */
    axis2_char_t *status;

    /** Output message (HTML-encoded for safety) */
    axis2_char_t *message_out;

    /** XSS prevention details */
    axis2_char_t *security_details;

    /** Response timestamp */
    time_t response_time;

} testws_response_t;

/**
 * @brief Create TestWS Request from pure JSON using json-c
 */
AXIS2_EXTERN testws_request_t* AXIS2_CALL
testws_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string);

/**
 * @brief Create TestWS Request
 */
AXIS2_EXTERN testws_request_t* AXIS2_CALL
testws_request_create(
    const axutil_env_t *env,
    const axis2_char_t *message_in);

/**
 * @brief Free TestWS Request
 */
AXIS2_EXTERN void AXIS2_CALL
testws_request_free(
    testws_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Create TestWS Response
 */
AXIS2_EXTERN testws_response_t* AXIS2_CALL
testws_response_create(
    const axutil_env_t *env);

/**
 * @brief Convert response to pure JSON string using json-c
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_response_to_json(
    const testws_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Free TestWS Response
 */
AXIS2_EXTERN void AXIS2_CALL
testws_response_free(
    testws_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Main TestWS Service Operation (equivalent to TestwsService.doTestws)
 *
 * Processes input message and demonstrates XSS prevention through HTML encoding.
 * Uses pure json-c for all JSON processing (no XML/axiom dependencies).
 *
 * @param env Environment
 * @param request TestWS request with potentially dangerous input
 * @return TestWS response with HTML-encoded safe output
 */
AXIS2_EXTERN testws_response_t* AXIS2_CALL
testws_service_do_testws(
    const axutil_env_t *env,
    testws_request_t *request);

/**
 * @brief HTML-encode string to prevent XSS attacks
 *
 * Encodes dangerous characters:
 * < becomes &lt;
 * > becomes &gt;
 * & becomes &amp;
 * " becomes &quot;
 * ' becomes &#x27;
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_html_encode(
    const axutil_env_t *env,
    const axis2_char_t *input);

/**
 * @brief Validate input message for security
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
testws_service_validate_input(
    const axutil_env_t *env,
    const axis2_char_t *message);

/**
 * @brief Detect potential XSS attack patterns
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
testws_service_detect_xss_patterns(
    const axutil_env_t *env,
    const axis2_char_t *input);

/**
 * @brief Sanitize input string for security
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_sanitize_input(
    const axutil_env_t *env,
    const axis2_char_t *input,
    int max_length);

/**
 * @brief Get XSS prevention analysis
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_get_xss_analysis(
    const axutil_env_t *env,
    const axis2_char_t *original_input,
    const axis2_char_t *encoded_output);

/**
 * @brief Generate security demonstration report
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_generate_security_report(
    const axutil_env_t *env,
    const axis2_char_t *input,
    const axis2_char_t *output);

#ifdef __cplusplus
}
#endif

#endif /* TESTWS_SERVICE_H */