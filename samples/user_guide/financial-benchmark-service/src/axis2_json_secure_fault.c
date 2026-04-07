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
 * @file axis2_json_secure_fault.c
 * @brief Secure JSON error responses with /dev/urandom correlation IDs (D2)
 *
 * Design notes:
 *
 * /dev/urandom vs /dev/random:
 *   We use /dev/urandom (non-blocking) intentionally.  /dev/random can block
 *   on low-entropy systems (headless servers, embedded devices).  Correlation
 *   IDs need uniqueness, not cryptographic-grade entropy.
 *
 * Fallback PRNG:
 *   On Android (and other restricted environments) /dev/urandom may be
 *   unavailable due to SELinux policy.  The fallback mixes clock(), time(),
 *   and getpid() with XOR shifts to produce a usable 64-bit value.  It is
 *   not cryptographically random but is adequate for log correlation.
 *
 * JSON safety:
 *   Both corr_id and safe_message are inserted via json_object_new_string()
 *   which handles all JSON escaping internally.  No manual string building.
 */

#include "axis2_json_secure_fault.h"

#include <axutil_string.h>
#include <json-c/json.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifdef __linux__
#include <unistd.h>   /* getpid() */
#endif

/* ============================================================================
 * Correlation ID generation
 * ============================================================================
 */

AXIS2_EXTERN void AXIS2_CALL
axis2_json_corr_id_generate(char *buf, size_t len)
{
    if (!buf || len < AXIS2_JSON_CORR_ID_LEN) {
        if (buf && len > 0) buf[0] = '\0';
        return;
    }

    uint8_t rand_bytes[8];
    int got_random = 0;

    /* Primary source: /dev/urandom (non-blocking kernel CSPRNG) */
    FILE *urandom = fopen("/dev/urandom", "rb");
    if (urandom) {
        size_t nread = fread(rand_bytes, 1, sizeof(rand_bytes), urandom);
        fclose(urandom);
        if (nread == sizeof(rand_bytes)) {
            got_random = 1;
        }
    }

    if (!got_random) {
        /* Fallback: mix time, clock, and pid with XOR shifts.
         * Not cryptographically random — adequate for log correlation only. */
        uint64_t v = (uint64_t)time(NULL);
        v ^= (uint64_t)clock() << 17;
#ifdef __linux__
        v ^= (uint64_t)getpid() << 32;
#endif
        /* XOR-shift to spread bits */
        v ^= v >> 13;
        v ^= v << 7;
        v ^= v >> 17;
        memcpy(rand_bytes, &v, sizeof(rand_bytes));
    }

    /* Encode as 16 lowercase hex digits */
    static const char hex[] = "0123456789abcdef";
    for (int i = 0; i < 8; i++) {
        buf[i * 2]     = hex[(rand_bytes[i] >> 4) & 0x0f];
        buf[i * 2 + 1] = hex[ rand_bytes[i]       & 0x0f];
    }
    buf[16] = '\0';
}


/* ============================================================================
 * Secure JSON fault response
 * ============================================================================
 */

/* Returned verbatim when json-c allocation fails (static, never freed) */
static const char FAULT_ALLOC_FALLBACK[] =
    "{\"status\":\"ERROR\",\"error\":\"internal error\"}";

AXIS2_EXTERN axis2_char_t *AXIS2_CALL
axis2_json_secure_fault(
    const axutil_env_t *env,
    const char         *corr_id,
    const char         *safe_message)
{
    const char *safe_corr  = corr_id      ? corr_id      : "unknown";
    const char *safe_msg   = safe_message ? safe_message
                                          : "an internal error occurred";

    json_object *root = json_object_new_object();
    if (!root) {
        /* json-c allocation failed — return static fallback (never NULL) */
        return (axis2_char_t *)(uintptr_t)FAULT_ALLOC_FALLBACK;
    }

    json_object_object_add(root, "status",
        json_object_new_string("ERROR"));
    json_object_object_add(root, "correlation_id",
        json_object_new_string(safe_corr));
    json_object_object_add(root, "error",
        json_object_new_string(safe_msg));

    const char *json_cstr = json_object_to_json_string_ext(
            root, JSON_C_TO_STRING_PLAIN);

    axis2_char_t *result = NULL;
    if (json_cstr && env) {
        result = axutil_strdup(env, json_cstr);
    }

    json_object_put(root);   /* free entire tree */

    if (!result) {
        /* axutil_strdup failed or env was NULL */
        return (axis2_char_t *)(uintptr_t)FAULT_ALLOC_FALLBACK;
    }

    return result;
}
