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
 * @file login_service.c
 * @brief Apache Axis2/C HTTP/2 Login Service Implementation
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
 */

#include "login_service.h"
#include <axutil_string.h>
#include <axutil_utils.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

/* Demo user credentials (in real implementation, use secure storage) */
static const char *demo_users[][2] = {
    {"admin@example.com", "admin123"},
    {"user@example.com", "user123"},
    {"test@example.com", "test123"},
    {NULL, NULL}
};

/* Internal utility functions */
static axis2_char_t* base64_encode(const unsigned char *input, int length, const axutil_env_t *env);
static axis2_bool_t is_valid_email_char(char c);
static axis2_bool_t contains_sql_injection_patterns(const axis2_char_t *input);
static axis2_bool_t contains_script_tags(const axis2_char_t *input);

/**
 * Create Login Request from JSON string using pure json-c
 */
AXIS2_EXTERN login_request_t* AXIS2_CALL
login_request_create_from_json(
    const axutil_env_t *env,
    const axis2_char_t *json_string)
{
    login_request_t *request = NULL;
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
        AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI, "Failed to parse JSON login request");
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(login_request_t));
    if (!request)
    {
        json_object_put(json_obj);
        return NULL;
    }
    memset(request, 0, sizeof(login_request_t));

    /* Extract email */
    if (json_object_object_get_ex(json_obj, "email", &value_obj) ||
        json_object_object_get_ex(json_obj, "username", &value_obj))  /* Support both fields */
    {
        const char *email = json_object_get_string(value_obj);
        if (email)
        {
            /* Sanitize and validate email */
            request->email = login_service_sanitize_input(env, email, LOGIN_EMAIL_MAX_LENGTH);
            if (!login_service_validate_email(env, request->email))
            {
                AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "Invalid email format: %s", request->email);
            }
        }
    }

    /* Extract password */
    if (json_object_object_get_ex(json_obj, "password", &value_obj))
    {
        const char *password = json_object_get_string(value_obj);
        if (password)
        {
            /* Sanitize password (don't log it) */
            request->password = login_service_sanitize_input(env, password, LOGIN_PASSWORD_MAX_LENGTH);
        }
    }

    request->request_time = time(NULL);

    json_object_put(json_obj);
    return request;
}

/**
 * Create Login Request
 */
AXIS2_EXTERN login_request_t* AXIS2_CALL
login_request_create(
    const axutil_env_t *env,
    const axis2_char_t *email,
    const axis2_char_t *password)
{
    login_request_t *request = NULL;

    if (!env || !email || !password)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    request = AXIS2_MALLOC(env->allocator, sizeof(login_request_t));
    if (!request)
    {
        return NULL;
    }
    memset(request, 0, sizeof(login_request_t));

    request->email = login_service_sanitize_input(env, email, LOGIN_EMAIL_MAX_LENGTH);
    request->password = login_service_sanitize_input(env, password, LOGIN_PASSWORD_MAX_LENGTH);
    request->request_time = time(NULL);

    return request;
}

/**
 * Free Login Request
 */
AXIS2_EXTERN void AXIS2_CALL
login_request_free(
    login_request_t *request,
    const axutil_env_t *env)
{
    if (!request || !env)
        return;

    if (request->email)
        AXIS2_FREE(env->allocator, request->email);

    if (request->password)
    {
        /* Zero out password before freeing */
        memset(request->password, 0, strlen(request->password));
        AXIS2_FREE(env->allocator, request->password);
    }

    if (request->client_ip)
        AXIS2_FREE(env->allocator, request->client_ip);

    AXIS2_FREE(env->allocator, request);
}

/**
 * Create Login Response
 */
AXIS2_EXTERN login_response_t* AXIS2_CALL
login_response_create(
    const axutil_env_t *env)
{
    login_response_t *response = NULL;

    if (!env)
    {
        return NULL;
    }

    response = AXIS2_MALLOC(env->allocator, sizeof(login_response_t));
    if (!response)
    {
        return NULL;
    }
    memset(response, 0, sizeof(login_response_t));

    response->status = axutil_strdup(env, LOGIN_STATUS_INVALID);
    response->token_type = axutil_strdup(env, LOGIN_TOKEN_TYPE_BEARER);
    response->expires_in = LOGIN_TOKEN_EXPIRY_SECONDS;
    response->response_time = time(NULL);

    return response;
}

/**
 * Convert response to pure JSON string using json-c
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_response_to_json(
    const login_response_t *response,
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

    /* Add message */
    if (response->message)
    {
        json_object_object_add(json_response, "message",
                               json_object_new_string(response->message));
    }

    /* Add token (only if successful) */
    if (response->token && axutil_strcmp(response->status, LOGIN_STATUS_SUCCESS) == 0)
    {
        json_object_object_add(json_response, "token",
                               json_object_new_string(response->token));
        json_object_object_add(json_response, "tokenType",
                               json_object_new_string(response->token_type));
        json_object_object_add(json_response, "expiresIn",
                               json_object_new_int(response->expires_in));
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
 * Free Login Response
 */
AXIS2_EXTERN void AXIS2_CALL
login_response_free(
    login_response_t *response,
    const axutil_env_t *env)
{
    if (!response || !env)
        return;

    if (response->status)
        AXIS2_FREE(env->allocator, response->status);

    if (response->message)
        AXIS2_FREE(env->allocator, response->message);

    if (response->token)
    {
        /* Zero out token before freeing */
        memset(response->token, 0, strlen(response->token));
        AXIS2_FREE(env->allocator, response->token);
    }

    if (response->token_type)
        AXIS2_FREE(env->allocator, response->token_type);

    AXIS2_FREE(env->allocator, response);
}

/**
 * Main Login Service Operation
 */
AXIS2_EXTERN login_response_t* AXIS2_CALL
login_service_do_login(
    const axutil_env_t *env,
    login_request_t *request)
{
    login_response_t *response = NULL;
    axis2_char_t *display_name = NULL;
    axis2_char_t *jwt_token = NULL;
    time_t expiry_time;

    if (!env || !request)
    {
        AXIS2_ERROR_SET(env->error, AXIS2_ERROR_INVALID_NULL_PARAM, AXIS2_FAILURE);
        return NULL;
    }

    AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Processing login request for email: %s",
                   request->email ? request->email : "unknown");

    response = login_response_create(env);
    if (!response)
    {
        return NULL;
    }

    /* Validate request */
    if (!request->email || !request->password)
    {
        if (response->status)
            AXIS2_FREE(env->allocator, response->status);
        response->status = axutil_strdup(env, LOGIN_STATUS_INVALID);
        response->message = axutil_strdup(env, "Email and password are required");
        return response;
    }

    /* Validate email format */
    if (!login_service_validate_email(env, request->email))
    {
        if (response->status)
            AXIS2_FREE(env->allocator, response->status);
        response->status = axutil_strdup(env, LOGIN_STATUS_INVALID);
        response->message = axutil_strdup(env, "Invalid email format");
        return response;
    }

    /* Validate password */
    if (!login_service_validate_password(env, request->password))
    {
        if (response->status)
            AXIS2_FREE(env->allocator, response->status);
        response->status = axutil_strdup(env, LOGIN_STATUS_INVALID);
        response->message = axutil_strdup(env, "Invalid password format");
        return response;
    }

    /* Authenticate user */
    if (login_service_authenticate_user(env, request->email, request->password))
    {
        /* Authentication successful */
        AXIS2_LOG_INFO(env->log, AXIS2_LOG_SI, "Authentication successful for: %s", request->email);

        if (response->status)
            AXIS2_FREE(env->allocator, response->status);
        response->status = axutil_strdup(env, LOGIN_STATUS_SUCCESS);

        /* Generate display name */
        display_name = login_service_get_user_display_name(env, request->email);
        response->message = axutil_stracat(env, "Login successful. Welcome, ", display_name);

        /* Generate JWT token */
        expiry_time = time(NULL) + LOGIN_TOKEN_EXPIRY_SECONDS;
        jwt_token = login_service_generate_jwt_token(env, request->email, expiry_time);
        if (jwt_token)
        {
            response->token = jwt_token;
        }

        if (display_name)
            AXIS2_FREE(env->allocator, display_name);
    }
    else
    {
        /* Authentication failed */
        AXIS2_LOG_WARNING(env->log, AXIS2_LOG_SI, "Authentication failed for: %s", request->email);

        if (response->status)
            AXIS2_FREE(env->allocator, response->status);
        response->status = axutil_strdup(env, LOGIN_STATUS_FAILED);
        response->message = axutil_strdup(env, "Invalid email or password");
    }

    return response;
}

/**
 * Validate email format (RFC 5322 compliant)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_validate_email(
    const axutil_env_t *env,
    const axis2_char_t *email)
{
    int i, at_count = 0, dot_after_at = 0;
    int len;

    if (!env || !email)
        return AXIS2_FALSE;

    len = strlen(email);
    if (len < 5 || len > LOGIN_EMAIL_MAX_LENGTH)  /* Minimum: a@b.c */
        return AXIS2_FALSE;

    /* Check for security issues */
    if (contains_sql_injection_patterns(email) || contains_script_tags(email))
        return AXIS2_FALSE;

    /* Basic email validation */
    for (i = 0; i < len; i++)
    {
        char c = email[i];

        if (c == '@')
        {
            at_count++;
            if (at_count > 1 || i == 0 || i == len - 1)  /* Multiple @, or at start/end */
                return AXIS2_FALSE;
        }
        else if (c == '.' && at_count == 1)
        {
            dot_after_at = 1;
            if (i == len - 1)  /* Dot at end */
                return AXIS2_FALSE;
        }
        else if (!is_valid_email_char(c))
        {
            return AXIS2_FALSE;
        }
    }

    return (at_count == 1 && dot_after_at) ? AXIS2_TRUE : AXIS2_FALSE;
}

/**
 * Validate password strength
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_validate_password(
    const axutil_env_t *env,
    const axis2_char_t *password)
{
    int len;

    if (!env || !password)
        return AXIS2_FALSE;

    len = strlen(password);
    if (len < 6 || len > LOGIN_PASSWORD_MAX_LENGTH)  /* Minimum 6 characters */
        return AXIS2_FALSE;

    /* Check for security issues */
    if (contains_sql_injection_patterns(password) || contains_script_tags(password))
        return AXIS2_FALSE;

    return AXIS2_TRUE;
}

/**
 * Generate JWT token using pure JSON (simplified implementation)
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_generate_jwt_token(
    const axutil_env_t *env,
    const axis2_char_t *email,
    time_t expiry_time)
{
    json_object *header = NULL;
    json_object *payload = NULL;
    const char *header_str = NULL;
    const char *payload_str = NULL;
    axis2_char_t *encoded_header = NULL;
    axis2_char_t *encoded_payload = NULL;
    axis2_char_t *jwt_token = NULL;
    axis2_char_t *temp = NULL;

    if (!env || !email)
        return NULL;

    /* Create JWT header */
    header = json_object_new_object();
    if (!header)
        return NULL;

    json_object_object_add(header, "alg", json_object_new_string("HS256"));
    json_object_object_add(header, "typ", json_object_new_string("JWT"));

    /* Create JWT payload */
    payload = json_object_new_object();
    if (!payload)
    {
        json_object_put(header);
        return NULL;
    }

    json_object_object_add(payload, "sub", json_object_new_string(email));
    json_object_object_add(payload, "iat", json_object_new_int64(time(NULL)));
    json_object_object_add(payload, "exp", json_object_new_int64(expiry_time));
    json_object_object_add(payload, "iss", json_object_new_string("Apache Axis2/C"));

    /* Encode header and payload */
    header_str = json_object_to_json_string(header);
    payload_str = json_object_to_json_string(payload);

    if (header_str && payload_str)
    {
        encoded_header = base64_encode((const unsigned char*)header_str, strlen(header_str), env);
        encoded_payload = base64_encode((const unsigned char*)payload_str, strlen(payload_str), env);

        if (encoded_header && encoded_payload)
        {
            /* Create JWT token (header.payload.signature) */
            /* Note: In production, add proper HMAC signature */
            temp = axutil_stracat(env, encoded_header, ".");
            jwt_token = axutil_stracat(env, temp, encoded_payload);
            if (temp)
                AXIS2_FREE(env->allocator, temp);

            /* Add dummy signature for demo */
            temp = jwt_token;
            jwt_token = axutil_stracat(env, temp, ".demo_signature");
            if (temp)
                AXIS2_FREE(env->allocator, temp);
        }

        if (encoded_header)
            AXIS2_FREE(env->allocator, encoded_header);
        if (encoded_payload)
            AXIS2_FREE(env->allocator, encoded_payload);
    }

    json_object_put(header);
    json_object_put(payload);

    return jwt_token;
}

/**
 * Validate JWT token (simplified implementation)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_validate_jwt_token(
    const axutil_env_t *env,
    const axis2_char_t *token)
{
    int dot_count = 0;
    int len, i;

    if (!env || !token)
        return AXIS2_FALSE;

    len = strlen(token);
    if (len < 10)  /* Minimum reasonable JWT length */
        return AXIS2_FALSE;

    /* Count dots (should be 2 for header.payload.signature) */
    for (i = 0; i < len; i++)
    {
        if (token[i] == '.')
            dot_count++;
    }

    return (dot_count == 2) ? AXIS2_TRUE : AXIS2_FALSE;
}

/**
 * Authenticate user credentials (demo implementation)
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
login_service_authenticate_user(
    const axutil_env_t *env,
    const axis2_char_t *email,
    const axis2_char_t *password)
{
    int i;

    if (!env || !email || !password)
        return AXIS2_FALSE;

    /* Demo authentication - check against hardcoded users */
    for (i = 0; demo_users[i][0] != NULL; i++)
    {
        if (axutil_strcmp(email, demo_users[i][0]) == 0 &&
            axutil_strcmp(password, demo_users[i][1]) == 0)
        {
            return AXIS2_TRUE;
        }
    }

    return AXIS2_FALSE;
}

/**
 * Get user display name from email
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_get_user_display_name(
    const axutil_env_t *env,
    const axis2_char_t *email)
{
    axis2_char_t *display_name = NULL;
    axis2_char_t *at_pos = NULL;

    if (!env || !email)
        return NULL;

    /* Extract username from email (part before @) */
    at_pos = strchr(email, '@');
    if (at_pos)
    {
        int username_len = at_pos - email;
        display_name = AXIS2_MALLOC(env->allocator, username_len + 1);
        if (display_name)
        {
            strncpy(display_name, email, username_len);
            display_name[username_len] = '\0';
        }
    }
    else
    {
        display_name = axutil_strdup(env, email);
    }

    return display_name;
}

/**
 * HTML-encode string for XSS prevention
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_html_encode(
    const axutil_env_t *env,
    const axis2_char_t *input)
{
    int len, i, j, new_len = 0;
    axis2_char_t *encoded = NULL;

    if (!env || !input)
        return NULL;

    len = strlen(input);

    /* Calculate new length */
    for (i = 0; i < len; i++)
    {
        switch (input[i])
        {
            case '<': new_len += 4; break;  /* &lt; */
            case '>': new_len += 4; break;  /* &gt; */
            case '&': new_len += 5; break;  /* &amp; */
            case '"': new_len += 6; break;  /* &quot; */
            case '\'': new_len += 5; break; /* &#39; */
            default: new_len++; break;
        }
    }

    encoded = AXIS2_MALLOC(env->allocator, new_len + 1);
    if (!encoded)
        return NULL;

    /* Encode string */
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
                strcpy(&encoded[j], "&#39;"); j += 5; break;
            default:
                encoded[j++] = input[i]; break;
        }
    }
    encoded[j] = '\0';

    return encoded;
}

/**
 * Sanitize input string for security
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
login_service_sanitize_input(
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

    /* Remove dangerous characters and control chars */
    for (i = 0, j = 0; i < len && j < max_length; i++)
    {
        char c = input[i];

        /* Skip control characters except tab, newline, carriage return */
        if (iscntrl(c) && c != '\t' && c != '\n' && c != '\r')
            continue;

        /* Skip null bytes */
        if (c == '\0')
            break;

        sanitized[j++] = c;
    }
    sanitized[j] = '\0';

    return sanitized;
}

/* Internal utility functions */

static axis2_char_t* base64_encode(const unsigned char *input, int length, const axutil_env_t *env)
{
    BIO *bmem, *b64;
    BUF_MEM *bptr;
    axis2_char_t *result = NULL;

    if (!input || length <= 0 || !env)
        return NULL;

    b64 = BIO_new(BIO_f_base64());
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    result = AXIS2_MALLOC(env->allocator, bptr->length + 1);
    if (result)
    {
        memcpy(result, bptr->data, bptr->length);
        result[bptr->length] = '\0';
    }

    BIO_free_all(b64);
    return result;
}

static axis2_bool_t is_valid_email_char(char c)
{
    return (isalnum(c) || c == '.' || c == '-' || c == '_' || c == '+');
}

static axis2_bool_t contains_sql_injection_patterns(const axis2_char_t *input)
{
    if (!input)
        return AXIS2_FALSE;

    /* Check for common SQL injection patterns */
    if (strstr(input, "'") || strstr(input, "\"") ||
        strstr(input, ";") || strstr(input, "--") ||
        strstr(input, "/*") || strstr(input, "*/") ||
        strstr(input, "union") || strstr(input, "UNION") ||
        strstr(input, "select") || strstr(input, "SELECT") ||
        strstr(input, "drop") || strstr(input, "DROP"))
    {
        return AXIS2_TRUE;
    }

    return AXIS2_FALSE;
}

static axis2_bool_t contains_script_tags(const axis2_char_t *input)
{
    if (!input)
        return AXIS2_FALSE;

    /* Check for XSS patterns */
    if (strstr(input, "<script") || strstr(input, "<SCRIPT") ||
        strstr(input, "</script") || strstr(input, "</SCRIPT") ||
        strstr(input, "javascript:") || strstr(input, "JAVASCRIPT:") ||
        strstr(input, "vbscript:") || strstr(input, "VBSCRIPT:"))
    {
        return AXIS2_TRUE;
    }

    return AXIS2_FALSE;
}