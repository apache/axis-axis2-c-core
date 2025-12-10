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
 * @file login_json_handler.h
 * @brief Apache Axis2/C HTTP/2 Pure JSON Login Handler Interface
 *
 * SOAP-free JSON handler interface for HTTP/2 authentication service.
 * Eliminates all service skeleton patterns for pure JSON processing.
 */

#ifndef LOGIN_JSON_HANDLER_H
#define LOGIN_JSON_HANDLER_H

#include <axutil_env.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Initialize Login JSON Handler
 * Replaces the traditional service skeleton creation pattern
 * @param env pointer to environment struct
 * @return AXIS2_SUCCESS on success, else AXIS2_FAILURE
 */
AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_login_json_handler_init(const axutil_env_t *env);

/**
 * Cleanup Login JSON Handler
 * Replaces the traditional service skeleton free pattern
 * @param env pointer to environment struct
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_login_json_handler_cleanup(const axutil_env_t *env);

/**
 * Pure JSON doLogin operation handler
 * Processes authentication requests without SOAP skeleton patterns
 * @param env pointer to environment struct
 * @param json_request JSON string containing login request
 * @return JSON string containing login response, NULL on failure
 * @note Caller must free the returned string
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_login_json_do_login(
    const axutil_env_t *env,
    const axis2_char_t *json_request);

/**
 * Get supported operations list in JSON format
 * @param env pointer to environment struct
 * @return JSON string containing operations list, NULL on failure
 * @note Caller must free the returned string
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
axis2_login_json_get_operations(const axutil_env_t *env);

#ifdef __cplusplus
}
#endif

#endif /* LOGIN_JSON_HANDLER_H */