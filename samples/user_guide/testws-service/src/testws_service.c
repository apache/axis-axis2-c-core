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
 * @file testws_service.c
 * @brief Apache Axis2/C HTTP/2 TestWS Service Implementation
 *
 * Pure JSON XSS protection demonstration service using json-c library.
 * Port of Axis2/Java Spring Boot TestwsService to Axis2/C with Apache httpd.
 *
 * Features:
 * - XSS attack prevention through HTML encoding
 * - OWASP ESAPI-style input validation
 * - HTTP/2 transport optimization
 * - Pure JSON processing (no axiom/XML)
 * - Educational security demonstration
 */

#include "testws_service.h"
#include <axutil_string.h>
#include <axutil_utils.h>
#include <axutil_uuid_gen.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* XSS attack patterns for detection */
static const char *xss_patterns[] = {
    "<script",
    "</script",
    "javascript:",
    "vbscript:",
    "onload=",
    "onerror=",
    "onclick=",
    "onmouseover=",
    "<img",
    "<iframe",
    "<object",
    "<embed",
    "eval(",
    "document.cookie",
    "document.write",
    NULL
};

/* Internal utility functions */
static axis2_char_t* generate_uuid(const axutil_env_t *env);
static axis2_bool_t contains_xss_pattern(const axis2_char_t *input);
static int calculate_encoded_length(const axis2_char_t *input);

/**
 * Create TestWS Request from JSON string using pure json-c
 */
AXIS2_EXTERN testws_request_t* AXIS2_CALL
testws_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    testws_request_t *request = NULL;
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
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse JSON TestWS request");
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(testws_request_t));
    if (!request)
    {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(testws_request_t));

    /* Extract message_in (supporting both messagein and messageIn variants) */
    if (json_object_object_get_ex(json_obj, "messagein", &value_obj) ||
        json_object_object_get_ex(json_obj, "messageIn", &value_obj) ||
        json_object_object_get_ex(json_obj, "message", &value_obj))
    {
        const char *message_in = json_object_get_string(value_obj);
        if (message_in)
        {
            /* Store raw input for processing (don't sanitize yet) */
            request->message_in = axutil_strdup(env, message_in);
        }
    }

    request->request_time = time(NULL);

    json_object_put(json_obj);
    return request;
}

/**
 * Create TestWS Request
 */
AXIS2_EXTERN testws_request_t* AXIS2_CALL
testws_request_create(
    const axutil_env_t *env,
    const axis2_char_t *message_in)
{
    testws_request_t *request = NULL;

    if (!env || !message_in)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(testws_request_t));
    if (!request)
    {
        return NULL;
    }
    memset(request, 0, sizeof(testws_request_t));

    request->message_in = axutil_strdup(env, message_in);
    request->request_time = time(NULL);

    return request;
}

/**
 * Free TestWS Request
 */
AXIS2_EXTERN void AXIS2_CALL
testws_request_free(
    testws_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env)
        return;

    if (request->message_in)
        AXIS2_FREE(env->allocator, request->message_in);

    if (request->client_ip)
        AXIS2_FREE(env->allocator, request->client_ip);

    AXIS2_FREE(env->allocator, request);
}

/**
 * Create TestWS Response
 */
AXIS2_EXTERN testws_response_t* AXIS2_CALL
testws_response_create(
    const axutil_env_t *env)
{
    testws_response_t *response = NULL;

    if (!env)
    {
        return NULL;
    }

    response = AXIS2_MALLOC(env->allocator, sizeof(testws_response_t));
    if (!response)
    {
        return NULL;
    }
    memset(response, 0, sizeof(testws_response_t));

    response->status = axutil_strdup(env, TESTWS_STATUS_INVALID);
    response->response_time = time(NULL);

    return response;
}

/**
 * Convert response to pure JSON string using json-c
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_response_to_json(
    const testws_response_t *response,
    const axutil_env_t *env)
{
    json_object *json_response = NULL;
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

    /* Add message_out */
    if (response->message_out)
    {
        json_object_object_add(json_response, "messageout",
                               json_object_new_string(response->message_out));
    }

    /* Add security details */
    if (response->security_details)
    {
        json_object_object_add(json_response, "securityDetails",
                               json_object_new_string(response->security_details));
    }

    /* Add timestamp */
    json_object_object_add(json_response, "responseTime",
                           json_object_new_int64(response->response_time));

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
 * Free TestWS Response
 */
AXIS2_EXTERN void AXIS2_CALL
testws_response_free(
    testws_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env)
        return;

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);

    if (response->message_out)
        AXIS2_FREE(env->allocator, response->message_out);

    if (response->security_details)
        AXIS2_FREE(env->allocator, response->security_details);

    AXIS2_FREE(env->allocator, response);
}

/**
 * Main TestWS Service Operation - XSS Protection Demonstration
 */
AXIS2_EXTERN testws_response_t* AXIS2_CALL
testws_service_do_testws(
    const axutil_env_t *env,
    testws_request_t *request)
{
    testws_response_t *response = NULL;
    axis2_char_t *uuid = NULL;
    axis2_char_t *log_prefix = NULL;
    axis2_bool_t is_valid_input = AXIS2_FALSE;
    axis2_char_t *security_details = NULL;

    if (!env || !request)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    /* Generate UUID for request tracking */
    uuid = generate_uuid(env);
    log_prefix = axutil_stracat(env, "TestwsService.doTestws() , uuid: ", uuid ? uuid : "unknown");

    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "%s , starting on request: %s",
                      log_prefix ? log_prefix : "TestwsService",
                      request->message_in ? request->message_in : "null");

    response = testws_response_create(env);
    if (!response)
    {
        if (uuid) AXIS2_FREE(env->allocator, uuid);
        if (log_prefix) AXIS2_FREE(env->allocator, log_prefix);
        return NULL;
    }

    /* Validate request */
    if (!request->message_in)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "%s , missing message input", log_prefix);
        if (response->status) AXIS2_FREE(env->allocator, response->status);
        response->status = axutil_strdup(env, TESTWS_STATUS_INVALID);
        response->security_details = axutil_strdup(env, "Missing required message input");
        goto cleanup;
    }

    /* All data is evil! - OWASP ESAPI-style validation */
    is_valid_input = testws_service_validate_input(env, request->message_in);
    if (!is_valid_input)
    {
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
                        "%s , returning with failure status on invalid messagein: %s",
                        log_prefix, request->message_in);

        if (response->status) AXIS2_FREE(env->allocator, response->status);
        response->status = axutil_strdup(env, TESTWS_STATUS_ERROR);
        response->security_details = axutil_strdup(env, "Input validation failed - potential security risk detected");
        goto cleanup;
    }

    /* Set success status */
    if (response->status) AXIS2_FREE(env->allocator, response->status);
    response->status = axutil_strdup(env, TESTWS_STATUS_OK);

    /* XSS Protection Demonstration:
     * Like the Java version, we intentionally return malicious content
     * to demonstrate the importance of proper output encoding */
    response->message_out = axutil_strdup(env,
        "<script xmlns=\"http://www.w3.org/1999/xhtml\">alert('Hello');</script> \">");

    /* Generate security analysis */
    response->security_details = testws_service_get_xss_analysis(env,
                                                                 request->message_in,
                                                                 response->message_out);

    AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "%s , returning response with messageout: %s",
                      log_prefix, response->message_out);

cleanup:
    if (uuid) AXIS2_FREE(env->allocator, uuid);
    if (log_prefix) AXIS2_FREE(env->allocator, log_prefix);
    return response;
}

/**
 * HTML-encode string to prevent XSS attacks
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_html_encode(
    const axutil_env_t *env,
    const axis2_char_t *input)
{
    int len, i, j, new_len = 0;
    axis2_char_t *encoded = NULL;

    if (!env || !input)
        return NULL;

    len = strlen(input);
    new_len = calculate_encoded_length(input);

    encoded = AXIS2_MALLOC(env->allocator, new_len + 1);
    if (!encoded)
        return NULL;

    /* Encode dangerous characters */
    for (i = 0, j = 0; i < len; i++)
    {
        switch (input[i])
        {
            case '<':
                strcpy(&encoded[j], "&lt;"); j += 4; break;
            case '>':
                strcpy(&encoded[j], "&gt;"); j += 4; break;
            case '&':
                strcpy(&encoded[j], "&amp;"); j += 5; break;
            case '"':
                strcpy(&encoded[j], "&quot;"); j += 6; break;
            case '\'':
                strcpy(&encoded[j], "&#x27;"); j += 6; break;
            default:
                encoded[j++] = input[i]; break;
        }
    }
    encoded[j] = '\0';

    return encoded;
}

/**
 * Validate input message for security (OWASP ESAPI-style)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
testws_service_validate_input(
    const axutil_env_t *env,
    const axis2_char_t *message)
{
    int len;

    if (!env || !message)
        return AXIS2_FALSE;

    len = strlen(message);

    /* Length validation */
    if (len <= 0 || len > TESTWS_MESSAGE_MAX_LENGTH)
        return AXIS2_FALSE;

    /* Character validation - allow alphanumeric, spaces, basic punctuation */
    /* This is intentionally strict for security demonstration */
    for (int i = 0; i < len; i++)
    {
        char c = message[i];
        if (!isalnum(c) && c != ' ' && c != '.' && c != '!' && c != '?' &&
            c != ',' && c != ':' && c != ';' && c != '-' && c != '_')
        {
            /* Found potentially dangerous character */
            return AXIS2_FALSE;
        }
    }

    /* XSS pattern detection */
    if (testws_service_detect_xss_patterns(env, message))
        return AXIS2_FALSE;

    return AXIS2_TRUE;
}

/**
 * Detect potential XSS attack patterns
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
testws_service_detect_xss_patterns(
    const axutil_env_t *env,
    const axis2_char_t *input)
{
    int i;
    axis2_char_t *lower_input = NULL;
    axis2_bool_t found = AXIS2_FALSE;

    if (!env || !input)
        return AXIS2_FALSE;

    /* Convert to lowercase for case-insensitive matching */
    lower_input = axutil_strdup(env, input);
    if (!lower_input)
        return AXIS2_FALSE;

    for (i = 0; lower_input[i]; i++)
    {
        lower_input[i] = tolower(lower_input[i]);
    }

    /* Check against known XSS patterns */
    for (i = 0; xss_patterns[i] != NULL; i++)
    {
        if (strstr(lower_input, xss_patterns[i]))
        {
            found = AXIS2_TRUE;
            break;
        }
    }

    AXIS2_FREE(env->allocator, lower_input);
    return found;
}

/**
 * Sanitize input string for security
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_sanitize_input(
    const axutil_env_t *env,
    const axis2_char_t *input,
    int max_length)
{
    axis2_char_t *sanitized = NULL;
    int len, i, j;

    if (!env || !input)
        return NULL;

    len = strlen(input);
    if (len > max_length)
        len = max_length;

    sanitized = AXIS2_MALLOC(env->allocator, len + 1);
    if (!sanitized)
        return NULL;

    /* Remove control characters and limit to safe characters */
    for (i = 0, j = 0; i < len && j < max_length; i++)
    {
        char c = input[i];

        /* Skip control characters */
        if (iscntrl(c))
            continue;

        /* Skip null bytes */
        if (c == '\0')
            break;

        sanitized[j++] = c;
    }
    sanitized[j] = '\0';

    return sanitized;
}

/**
 * Get XSS prevention analysis
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_get_xss_analysis(
    const axutil_env_t *env,
    const axis2_char_t *original_input,
    const axis2_char_t *encoded_output)
{
    axis2_char_t *analysis = NULL;
    axis2_char_t *temp = NULL;
    axis2_bool_t has_xss_patterns = AXIS2_FALSE;

    if (!env)
        return NULL;

    has_xss_patterns = testws_service_detect_xss_patterns(env, encoded_output);

    temp = axutil_stracat(env, "XSS Analysis: Input validated successfully. ", "");
    analysis = axutil_stracat(env, temp,
        "Output contains intentional XSS payload for demonstration purposes. ");

    if (temp) AXIS2_FREE(env->allocator, temp);
    temp = analysis;

    if (has_xss_patterns)
    {
        analysis = axutil_stracat(env, temp,
            "WARNING: Output contains potential XSS patterns - proper HTML encoding required in production. ");
    }
    else
    {
        analysis = axutil_stracat(env, temp,
            "Output appears safe from XSS patterns. ");
    }

    if (temp) AXIS2_FREE(env->allocator, temp);
    temp = analysis;

    analysis = axutil_stracat(env, temp,
        "This service demonstrates OWASP ESAPI-style input validation and the importance of output encoding.");

    if (temp) AXIS2_FREE(env->allocator, temp);

    return analysis;
}

/**
 * Generate security demonstration report
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
testws_service_generate_security_report(
    const axutil_env_t *env,
    const axis2_char_t *input,
    const axis2_char_t *output)
{
    axis2_char_t *report = NULL;
    axis2_char_t *encoded_input = NULL;
    axis2_char_t *temp = NULL;
    axis2_bool_t input_safe = AXIS2_FALSE;
    axis2_bool_t output_safe = AXIS2_FALSE;

    if (!env)
        return NULL;

    input_safe = testws_service_validate_input(env, input);
    output_safe = !testws_service_detect_xss_patterns(env, output);
    encoded_input = testws_service_html_encode(env, input ? input : "null");

    report = axutil_strdup(env, "=== XSS Protection Security Report ===\n");

    temp = report;
    report = axutil_stracat(env, temp, "Input Validation: ");
    if (temp) AXIS2_FREE(env->allocator, temp);

    temp = report;
    report = axutil_stracat(env, temp, input_safe ? "PASSED\n" : "FAILED\n");
    if (temp) AXIS2_FREE(env->allocator, temp);

    temp = report;
    report = axutil_stracat(env, temp, "Output Safety: ");
    if (temp) AXIS2_FREE(env->allocator, temp);

    temp = report;
    report = axutil_stracat(env, temp, output_safe ? "SAFE\n" : "UNSAFE (Contains XSS patterns)\n");
    if (temp) AXIS2_FREE(env->allocator, temp);

    temp = report;
    report = axutil_stracat(env, temp, "HTML-Encoded Input: ");
    if (temp) AXIS2_FREE(env->allocator, temp);

    temp = report;
    report = axutil_stracat(env, temp, encoded_input ? encoded_input : "null");
    if (temp) AXIS2_FREE(env->allocator, temp);

    if (encoded_input) AXIS2_FREE(env->allocator, encoded_input);

    return report;
}

/* Internal utility functions */

static axis2_char_t* generate_uuid(const axutil_env_t *env)
{
    if (!env)
        return NULL;

    /* Use simplified UUID generation API */
    return axutil_uuid_gen(env);
}

static axis2_bool_t contains_xss_pattern(const axis2_char_t *input)
{
    int i;

    if (!input)
        return AXIS2_FALSE;

    for (i = 0; xss_patterns[i] != NULL; i++)
    {
        if (strstr(input, xss_patterns[i]))
        {
            return AXIS2_TRUE;
        }
    }

    return AXIS2_FALSE;
}

static int calculate_encoded_length(const axis2_char_t *input)
{
    int len, i, new_len = 0;

    if (!input)
        return 0;

    len = strlen(input);

    for (i = 0; i < len; i++)
    {
        switch (input[i])
        {
            case '<': new_len += 4; break;  /* &lt; */
            case '>': new_len += 4; break;  /* &gt; */
            case '&': new_len += 5; break;  /* &amp; */
            case '"': new_len += 6; break;  /* &quot; */
            case '\'': new_len += 6; break; /* &#x27; */
            default: new_len++; break;
        }
    }

    return new_len;
}