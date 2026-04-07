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
 * @file finbench_mcp_main.c
 * @brief Entry point for the financial-benchmark-mcp standalone binary (C1)
 *
 * Claude Desktop config (~/.config/claude/claude_desktop_config.json):
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
 * Environment variables:
 *   FINBENCH_LOG_FILE  Path for Axis2/C log output.
 *                      Default: /tmp/finbench_mcp.log
 *                      MUST NOT point to stdout — that channel is reserved
 *                      for JSON-RPC 2.0 responses.
 *
 * This binary is separate from the httpd shared library
 * (libfinancial_benchmark_service.so). Both are built from the same
 * financial_benchmark_service.c source — the MCP binary links it statically
 * rather than as a loadable module.
 */

#include "financial_benchmark_service.h"
#include "finbench_mcp.h"

#include <axutil_env.h>
#include <axutil_log.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    /* Resolve log file — must NOT be stdout */
    const char *log_file = getenv("FINBENCH_LOG_FILE");
    if (!log_file || log_file[0] == '\0') {
        log_file = "/tmp/finbench_mcp.log";
    }

    /*
     * Create Axis2/C environment.
     * AXIS2_LOG_LEVEL_WARNING suppresses INFO/DEBUG noise to keep the
     * log file small during normal operation.
     */
    const axutil_env_t *env = axutil_env_create_all(
        log_file, AXIS2_LOG_LEVEL_WARNING);

    if (!env) {
        /* Write to stderr — stdout is reserved for JSON-RPC */
        fprintf(stderr,
            "finbench_mcp: failed to create Axis2/C environment "
            "(log_file=%s)\n", log_file);
        return 1;
    }

    /* Run the MCP JSON-RPC 2.0 stdio loop until stdin closes */
    finbench_run_mcp_stdio(env);

    axutil_env_free((axutil_env_t *)env);
    return 0;
}
