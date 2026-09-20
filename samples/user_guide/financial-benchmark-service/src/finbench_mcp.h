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

#ifndef FINBENCH_MCP_H
#define FINBENCH_MCP_H

/**
 * @file finbench_mcp.h
 * @brief MCP stdio transport for the Financial Benchmark Service (C1)
 *
 * Implements JSON-RPC 2.0 over stdin/stdout for the five financial benchmark
 * operations: portfolioVariance, composeCovariance, covarianceFromReturns,
 * monteCarlo, scenarioAnalysis.
 *
 * This is a thin wrapper around the existing finbench_*_json_only() functions.
 * The same JSON request/response schemas are reused — MCP adds only the
 * JSON-RPC 2.0 framing (method, id, params.arguments, result.content).
 *
 * Example MCP client config (path varies by client; Claude Desktop's
 * ~/.config/claude/claude_desktop_config.json shown):
 * @code
 * {
 *   "mcpServers": {
 *     "axis2c-financial": {
 *       "command": "/usr/local/axis2c/bin/financial-benchmark-mcp",
 *       "args": []
 *     }
 *   }
 * }
 * @endcode
 *
 * IMPORTANT: stdout is reserved for JSON-RPC 2.0 responses.
 * Set FINBENCH_LOG_FILE to redirect Axis2/C log output away from stdout.
 * Default log file: /tmp/finbench_mcp.log
 *
 * Protocol version: 2024-11-05
 *
 * Tools exposed:
 *   - portfolioVariance  (O(n^2) covariance matrix multiplication)
 *   - composeCovariance  (Sigma = D*R*D from vols + correlations, Cholesky-checked)
 *   - covarianceFromReturns (sample Sigma from a return history, complete-case)
 *   - monteCarlo         (GBM Value-at-Risk simulation)
 *   - scenarioAnalysis   (expected return + HashMap vs ArrayList benchmark)
 */

#include <axutil_env.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Run the MCP JSON-RPC 2.0 stdio loop.
 *
 * Reads newline-delimited JSON requests from stdin, dispatches to the
 * financial benchmark operations, and writes JSON-RPC 2.0 responses to stdout.
 * Returns when stdin reaches EOF (the MCP client terminates the process).
 *
 * @param env Axis2/C environment. The associated logger MUST write to a file,
 *            not stdout. Use axutil_env_create_all("/tmp/finbench_mcp.log", ...).
 */
void finbench_run_mcp_stdio(const axutil_env_t *env);

/**
 * @brief One MCP tool: its name, its description, and the JSON Schema for its
 *        arguments as a string.
 *
 * The built-in catalog is an array of these ending in a { NULL, NULL, NULL }
 * sentinel. An embedder's extra catalog has the same shape.
 */
typedef struct finbench_mcp_tool {
    const char *name;
    const char *description;
    const char *input_schema_json;
} finbench_mcp_tool_t;

/**
 * @brief Extra tools contributed by the application embedding this transport.
 *
 * The default definition is weak and returns NULL, so a plain build of this
 * sample exposes only its own operations. An application that links the
 * transport can define this function (and finbench_mcp_extra_dispatch) to add
 * tools of its own: the Kanaha Calcs app, for one, adds a tool that reads a
 * CSV the phone already holds and hands the returns to covarianceFromReturns,
 * which this sample cannot do because it has no file of its own to read.
 *
 * Same contract as the built-in catalog: a static array ending in a
 * { NULL, NULL, NULL } sentinel, or NULL for none. Requires a toolchain with
 * weak symbols (GCC, Clang); elsewhere the defaults cannot be overridden.
 *
 * @return Array of tools ending in a NULL-name sentinel, or NULL.
 */
AXIS2_EXTERN const finbench_mcp_tool_t* AXIS2_CALL
finbench_mcp_extra_tools(void);

/**
 * @brief Run one of the extra tools.
 *
 * Called only for a tool name that is not one of the built-ins. The default
 * definition is weak and returns NULL, which the caller reports as an unknown
 * tool.
 *
 * @param env       Axis2/C environment.
 * @param tool_name The tool being called.
 * @param args_json The arguments object, serialized.
 * @return Response JSON allocated with the env's allocator (the caller frees
 *         it with AXIS2_FREE), or NULL if this tool is not known here.
 */
AXIS2_EXTERN axis2_char_t* AXIS2_CALL
finbench_mcp_extra_dispatch(
    const axutil_env_t *env,
    const axis2_char_t *tool_name,
    const axis2_char_t *args_json);

#ifdef __cplusplus
}
#endif

#endif /* FINBENCH_MCP_H */
