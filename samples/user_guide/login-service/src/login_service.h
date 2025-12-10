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

#ifndef LOGIN_SERVICE_H
#define LOGIN_SERVICE_H

/**
 * @file login_service.h
 * @brief Apache Axis2/C HTTP/2 Login Service Header
 *
 * Pure JSON authentication service using json-c library (no XML/SOAP dependencies).
 * Port of Axis2/Java Spring Boot LoginService to Axis2/C with Apache httpd.
 *
 * Features:
 * - JWT token generation and validation
 * - OWASP ESAPI-style input validation
 * - HTTP/2 transport optimization
 * - Pure JSON processing (no axiom/XML)
 * - Secure credential handling
 * - Compatible with Axis2/Java LoginService API
 */

#include <axutil_env.h>
#include <json-c/json.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Authentication status constants */
#define LOGIN_STATUS_SUCCESS    "LOGIN_SUCCESS"
#define LOGIN_STATUS_FAILED     "LOGIN_FAILED"
#define LOGIN_STATUS_INVALID    "INVALID_REQUEST"

/** Token constants */
#define LOGIN_TOKEN_TYPE_BEARER "Bearer"
#define LOGIN_TOKEN_EXPIRY_SECONDS 3600  /* 1 hour */

/** Validation limits */
#define LOGIN_EMAIL_MAX_LENGTH    255
#define LOGIN_PASSWORD_MAX_LENGTH 100

/**
 * @brief Login Request structure (equivalent to LoginRequest.java)
 */
typedef struct login_request
{
    /** User email address */
    axis2_char_t *email;

    /** User password */
    axis2_char_t *password;

    /** Request timestamp */
    time_t request_time;

    /** Client IP address (for security logging) */
    axis2_char_t *client_ip;

} login_request_t;

/**
 * @brief Login Response structure (equivalent to LoginResponse.java)
 */
typedef struct login_response
{
    /** Authentication status */
    axis2_char_t *status;

    /** Response message */
    axis2_char_t *message;

    /** JWT token (if successful) */
    axis2_char_t *token;

    /** Token type */
    axis2_char_t *token_type;

    /** Token expiration time in seconds */
    int expires_in;

    /** Response timestamp */
    time_t response_time;

} login_response_t;

/**
 * @brief Create Login Request from pure JSON using json-c
 */
AXIS2_EXTERN login_request_t* AXIS2_CALL
login_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string);

/**
 * @brief Create Login Request
 */
AXIS2_EXTERN login_request_t* AXIS2_CALL
login_request_create(
    const axutil_env_t *env,
    const axis2_char_t *email,
    const axis2_char_t *password);

/**
 * @brief Free Login Request
 */
AXIS2_EXTERN void AXIS2_CALL
login_request_free(
    login_request_t *request,
    const axutil_env_t *env);

/**
 * @brief Create Login Response
 */
AXIS2_EXTERN login_response_t* AXIS2_CALL
login_response_create(
    const axutil_env_t *env);

/**
 * @brief Convert response to pure JSON string using json-c
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_response_to_json(
    const login_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Free Login Response
 */
AXIS2_EXTERN void AXIS2_CALL
login_response_free(
    login_response_t *response,
    const axutil_env_t *env);

/**
 * @brief Main Login Service Operation (equivalent to LoginService.doLogin)
 *
 * Authenticates user credentials and generates JWT token.
 * Uses pure json-c for all JSON processing (no XML/axiom dependencies).
 *
 * @param env Environment
 * @param request Login request with credentials
 * @return Login response with authentication result
 */
AXIS2_EXTERN login_response_t* AXIS2_CALL
login_service_do_login(
    const axutil_env_t *env,
    login_request_t *request);

/**
 * @brief Validate email format (RFC 5322 compliant)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_validate_email(
    const axutil_env_t *env,
    const axis2_char_t *email);

/**
 * @brief Validate password strength
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_validate_password(
    const axutil_env_t *env,
    const axis2_char_t *password);

/**
 * @brief Generate JWT token using pure JSON
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_generate_jwt_token(
    const axutil_env_t *env,
    const axis2_char_t *email,
    time_t expiry_time);

/**
 * @brief Validate JWT token
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_validate_jwt_token(
    const axutil_env_t *env,
    const axis2_char_t *token);

/**
 * @brief Authenticate user credentials (demo implementation)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_authenticate_user(
    const axutil_env_t *env,
    const axis2_char_t *email,
    const axis2_char_t *password);

/**
 * @brief Get user display name from email
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_get_user_display_name(
    const axutil_env_t *env,
    const axis2_char_t *email);

/**
 * @brief HTML-encode string for XSS prevention
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_html_encode(
    const axutil_env_t *env,
    const axis2_char_t *input);

/**
 * @brief Sanitize input string for security
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_sanitize_input(
    const axutil_env_t *env,
    const axis2_char_t *input,
    int max_length);

#ifdef __cplusplus
}
#endif

#endif /* LOGIN_SERVICE_H */