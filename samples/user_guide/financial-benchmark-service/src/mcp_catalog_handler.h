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
 * @file mcp_catalog_handler.h
 * @brief MCP catalog HTTP handler for Axis2/C — GET /_mcp/openapi-mcp.json
 *
 * Generates an MCP 2025-03-26-compatible tool catalog by walking all deployed
 * Axis2/C services at request time.  Reads per-operation parameters from
 * services.xml:
 *
 *   mcpDescription   — human-readable tool description (operation > service)
 *   mcpInputSchema   — literal JSON Schema string (operation > service)
 *   mcpReadOnly      — sets readOnlyHint    (default: false)
 *   mcpIdempotent    — sets idempotentHint  (default: false)
 *   mcpDestructive   — sets destructiveHint (default: false)
 *   mcpOpenWorld     — sets openWorldHint   (default: false)
 *
 * The catalog format matches the Java OpenApiSpecGenerator output exactly,
 * so an MCP-aware client (e.g., axis2-mcp-bridge) can point at either a
 * Java or C deployment and receive the same schema shape.
 *
 * Output format:
 * @code
 * {
 *   "_meta": {
 *     "axis2JsonRpcFormat": "{\"<operationName>\":[{\"arg0\":{<params>}}]}",
 *     "contentType": "application/json",
 *     "authHeader": "Authorization: Bearer <token>",
 *     "tokenEndpoint": "POST /services/loginService/doLogin"
 *   },
 *   "tools": [
 *     {
 *       "name": "portfolioVariance",
 *       "description": "...",
 *       "inputSchema": { "type": "object", "properties": {...}, "required": [...] },
 *       "endpoint": "POST /services/FinancialBenchmarkService/portfolioVariance",
 *       "x-axis2-payloadTemplate": "{\"portfolioVariance\":[{\"arg0\":{}}]}",
 *       "x-requiresAuth": true,
 *       "annotations": {
 *         "readOnlyHint": true, "destructiveHint": false,
 *         "idempotentHint": true, "openWorldHint": false
 *       }
 *     }
 *   ]
 * }
 * @endcode
 */

#ifndef MCP_CATALOG_HANDLER_H
#define MCP_CATALOG_HANDLER_H

#include <axutil_env.h>
#include <axis2_conf.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * URL path that triggers the MCP catalog response.
 * Checked by mcp_catalog_check_and_respond() against the incoming request URI.
 */
#define MCP_CATALOG_PATH "/_mcp/openapi-mcp.json"

/**
 * HTTP path prefix that identifies all Axis2 services.
 * Combined with service and operation names to build endpoint strings.
 */
#define MCP_SERVICES_PREFIX "/services/"

/**
 * Generate the MCP tool catalog JSON string.
 *
 * Walks all services registered in @p conf, reads MCP metadata parameters,
 * and builds the catalog JSON using json-c.  Services named "Version",
 * "AdminService", or starting with "__" are filtered out (system services).
 *
 * @param conf  Axis2 configuration object (from axis2_conf_ctx_get_conf)
 * @param env   Axis2 environment
 * @return      Heap-allocated JSON string (caller must free with AXIS2_FREE),
 *              or NULL on allocation failure.  On json-c failure returns a
 *              minimal fallback: @c {"tools":[]}.
 */
AXIS2_EXTERN axis2_char_t *AXIS2_CALL
mcp_catalog_generate_json(
    axis2_conf_t *conf,
    const axutil_env_t *env);

/**
 * Check whether @p request_path matches MCP_CATALOG_PATH and, if so,
 * write the catalog JSON to @p response_buf / @p response_len.
 *
 * Intended to be called near the top of a service handler's invoke()
 * before normal operation dispatch:
 *
 * @code
 * axis2_char_t *resp = NULL;
 * if (mcp_catalog_check_and_respond(env, conf, request_path, &resp)) {
 *     // write resp to HTTP response and return AXIS2_SUCCESS
 *     AXIS2_FREE(env->allocator, resp);
 *     return AXIS2_SUCCESS;
 * }
 * // ... normal dispatch
 * @endcode
 *
 * @param env           Axis2 environment
 * @param conf          Axis2 configuration
 * @param request_path  URI path from the incoming request (may be NULL)
 * @param out_json      Output: heap-allocated JSON string on match; unchanged on no-match
 * @return              AXIS2_TRUE if the path matched and *out_json was set;
 *                      AXIS2_FALSE otherwise.
 */
AXIS2_EXTERN axis2_bool_t AXIS2_CALL
mcp_catalog_check_and_respond(
    const axutil_env_t *env,
    axis2_conf_t *conf,
    const axis2_char_t *request_path,
    axis2_char_t **out_json);

#ifdef __cplusplus
}
#endif

#endif /* MCP_CATALOG_HANDLER_H */
