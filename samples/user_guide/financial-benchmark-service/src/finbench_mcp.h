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
 * Implements JSON-RPC 2.0 over stdin/stdout for the three financial benchmark
 * operations: portfolioVariance, monteCarlo, scenarioAnalysis.
 *
 * This is a thin wrapper around the existing finbench_*_json_only() functions.
 * The same JSON request/response schemas are reused — MCP adds only the
 * JSON-RPC 2.0 framing (method, id, params.arguments, result.content).
 *
 * Usage (Claude Desktop claude_desktop_config.json):
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
 * Returns when stdin reaches EOF (Claude Desktop terminates the process).
 *
 * @param env Axis2/C environment. The associated logger MUST write to a file,
 *            not stdout. Use axutil_env_create_all("/tmp/finbench_mcp.log", ...).
 */
void finbench_run_mcp_stdio(const axutil_env_t *env);

#ifdef __cplusplus
}
#endif

#endif /* FINBENCH_MCP_H */
