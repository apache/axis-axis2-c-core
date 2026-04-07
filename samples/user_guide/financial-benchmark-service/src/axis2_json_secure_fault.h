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

#ifndef AXIS2_JSON_SECURE_FAULT_H
#define AXIS2_JSON_SECURE_FAULT_H

/**
 * @file axis2_json_secure_fault.h
 * @brief Secure JSON error responses with /dev/urandom correlation IDs (D2)
 *
 * Provides two building blocks:
 *
 * 1. axis2_json_corr_id_generate()
 *    Reads 8 random bytes from /dev/urandom and encodes them as a 16-char
 *    lowercase hex string (e.g. "e1a2b3c4d5e6f7a8").  Falls back to a
 *    timestamp-seeded value if /dev/urandom is unavailable (embedded targets).
 *
 * 2. axis2_json_secure_fault()
 *    Returns a heap-allocated JSON error string:
 *      {"status":"ERROR","correlation_id":"<id>","error":"<safe_msg>"}
 *    The correlation_id should be logged alongside internal diagnostic
 *    details that must not appear in the HTTP response body.
 *
 * Usage pattern:
 * @code
 *   char corr_id[17];
 *   axis2_json_corr_id_generate(corr_id, sizeof(corr_id));
 *   AXIS2_LOG_ERROR(env->log, AXIS2_LOG_SI,
 *       "[%s] catalog generation failed: %s", corr_id, internal_detail);
 *   return axis2_json_secure_fault(env, corr_id,
 *       "catalog generation failed — see server log");
 * @endcode
 *
 * Caller owns the returned string and must free it with AXIS2_FREE.
 */

#include <axutil_env.h>
#include <axis2_defines.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Minimum buffer size for axis2_json_corr_id_generate() output:
 * 16 hex digits + NUL terminator.
 */
#define AXIS2_JSON_CORR_ID_LEN 17

/**
 * Generate a random correlation ID string.
 *
 * Reads 8 bytes from /dev/urandom, formats them as 16 lowercase hex digits,
 * and writes the NUL-terminated result to @p buf.
 *
 * Falls back to a XOR mix of clock() and time() when /dev/urandom cannot be
 * opened (e.g. Android devices with restricted /dev access).  The fallback is
 * NOT cryptographically random but provides reasonable uniqueness for log
 * correlation purposes.
 *
 * @param buf  Output buffer; must be at least AXIS2_JSON_CORR_ID_LEN bytes.
 * @param len  Size of @p buf in bytes.
 */
AXIS2_EXTERN void AXIS2_CALL
axis2_json_corr_id_generate(char *buf, size_t len);

/**
 * Build a secure JSON fault response.
 *
 * Returns a heap-allocated string of the form:
 *   {"status":"ERROR","correlation_id":"<corr_id>","error":"<safe_message>"}
 *
 * Both @p corr_id and @p safe_message are JSON-escaped by json-c so they
 * cannot inject content into the response.  @p safe_message MUST NOT contain
 * internal detail (stack traces, file paths, SQL, etc.) — only a human-
 * readable description suitable for API consumers.
 *
 * @param env          Axis2/C environment (allocator used for the result).
 * @param corr_id      16-char hex correlation ID from axis2_json_corr_id_generate().
 *                     NULL produces "unknown" in the response.
 * @param safe_message Public-safe error description.  NULL uses a generic message.
 * @return Heap-allocated JSON string; caller frees with AXIS2_FREE.
 *         Returns a minimal static fallback on allocation failure (never NULL).
 */
AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_json_secure_fault(
    const axutil_env_t *env,
    const char         *corr_id,
    const char         *safe_message);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_JSON_SECURE_FAULT_H */
