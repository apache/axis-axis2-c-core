# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements. See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License. You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Financial Benchmark Service - Makefile.am
#
# Builds the HTTP/2 JSON Financial Benchmark Service for Axis2/C
# Target: Resource-constrained devices (old Android phones, embedded systems)

# Only build if JSON is enabled (required for json-c library)
if AXIS2_JSON_ENABLED

# ─── Httpd shared library (loaded by mod_axis2) ───────────────────────────────

servicesdir = $(prefix)/services/FinancialBenchmarkService

lib_LTLIBRARIES = libfinancial_benchmark_service.la

libfinancial_benchmark_service_la_SOURCES = \
    src/financial_benchmark_service.c \
    src/financial_benchmark_service_handler.c \
    src/mcp_catalog_handler.c \
    src/axis2_json_secure_fault.c

libfinancial_benchmark_service_la_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/axiom/include \
    -I$(top_srcdir)/neethi/include \
    $(JSON_CFLAGS)

libfinancial_benchmark_service_la_LIBADD = \
    $(top_builddir)/src/core/engine/libaxis2_engine.la \
    $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
    $(top_builddir)/util/src/libaxutil.la \
    $(JSON_LIBS) \
    -lm

libfinancial_benchmark_service_la_LDFLAGS = \
    -module \
    -avoid-version \
    -no-undefined \
    -shared

# ─── MCP stdio binary (C1) ────────────────────────────────────────────────────
#
# Standalone binary launched by a stdio-based MCP client as an MCP subprocess.
# Links financial_benchmark_service.c directly (no shared library dependency).
#
# Example MCP client config (path varies by client; Claude Desktop's
# ~/.config/claude/claude_desktop_config.json shown):
#   { "mcpServers": { "axis2c-financial": {
#       "command": "/usr/local/axis2c/bin/financial-benchmark-mcp" } } }

bin_PROGRAMS = financial-benchmark-mcp

financial_benchmark_mcp_SOURCES = \
    src/financial_benchmark_service.c \
    src/finbench_mcp.c \
    src/finbench_mcp_main.c

financial_benchmark_mcp_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/util/include \
    -I$(top_srcdir)/axiom/include \
    -I$(top_srcdir)/neethi/include \
    $(JSON_CFLAGS)

financial_benchmark_mcp_LDADD = \
    $(top_builddir)/src/core/engine/libaxis2_engine.la \
    $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
    $(top_builddir)/util/src/libaxutil.la \
    $(JSON_LIBS) \
    -lm

# ─── Install ──────────────────────────────────────────────────────────────────

# Install services.xml to the services directory
services_DATA = services.xml

# Install headers for potential extensions
include_HEADERS = \
    src/financial_benchmark_service.h \
    src/finbench_mcp.h \
    src/mcp_catalog_handler.h \
    src/axis2_json_secure_fault.h

# Extra files to distribute
EXTRA_DIST = \
    services.xml \
    src/financial_benchmark_service.h \
    src/financial_benchmark_service_handler.c \
    src/finbench_mcp.h \
    src/mcp_catalog_handler.h \
    src/axis2_json_secure_fault.h \
    README.md

endif # AXIS2_JSON_ENABLED
