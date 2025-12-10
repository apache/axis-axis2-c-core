/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AXIS2_STUB_H
#define AXIS2_STUB_H

/*
 * HTTP/2 Pure JSON Architecture - SOAP Stub Header Removed
 *
 * This header has been emptied as part of HTTP/2 transport pure JSON architecture.
 * SOAP stub patterns are incompatible with HTTP/2 JSON-only design.
 *
 * HTTP/2 transport uses RESTful JSON clients instead of SOAP stubs:
 * - Direct HTTP/2 JSON requests via nghttp2
 * - REST URL-based service calls
 * - JSON payload serialization/deserialization
 * - No SOAP envelope wrapping
 * - No endpoint reference abstractions
 *
 * For SOAP services, use the HTTP/1.1 transport instead.
 */

#ifdef __cplusplus
extern "C"
{
#endif

/* Option B: Minimal stub definitions for functional baseline compatibility */

#include <axis2_defines.h>
#include <axutil_env.h>
#include <axis2_svc_client.h>
#include <axis2_options.h>
#include <axis2_endpoint_ref.h>
#include <axis2_svc_ctx.h>

/* Forward declaration for stub structure */
typedef struct axis2_stub axis2_stub_t;

/* Essential stub functions for Option B functional baseline */
AXIS2_EXTERN axis2_stub_t *AXIS2_CALL
axis2_stub_create(
    const axutil_env_t *env);

AXIS2_EXTERN void AXIS2_CALL
axis2_stub_free(
    axis2_stub_t *stub,
    const axutil_env_t *env);

AXIS2_EXTERN axis2_svc_client_t *AXIS2_CALL
axis2_stub_get_svc_client(
    const axis2_stub_t *stub,
    const axutil_env_t *env);

AXIS2_EXTERN axis2_status_t AXIS2_CALL
axis2_stub_set_svc_client(
    axis2_stub_t *stub,
    const axutil_env_t *env,
    axis2_svc_client_t *svc_client);

#ifdef __cplusplus
}
#endif

#endif /* AXIS2_STUB_H */