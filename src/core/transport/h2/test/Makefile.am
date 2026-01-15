# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied. See the License for the
# specific language governing permissions and limitations
# under the License.

# HTTP/2 Pure JSON Architecture - HTTP/2 transport unit tests
# These tests focus exclusively on HTTP/2 JSON processing without SOAP dependencies

if WITH_NGHTTP2
check_PROGRAMS = h2_json_integration_test h2_performance_benchmark_test
noinst_PROGRAMS = h2_json_integration_test h2_performance_benchmark_test
TESTS = h2_json_integration_test h2_performance_benchmark_test

# HTTP/2 JSON Integration Test
h2_json_integration_test_SOURCES = h2_json_integration_test.c \
                                   large_json_payload_generator.c

h2_json_integration_test_LDADD = \
    $(LDFLAGS) \
    $(top_builddir)/util/src/libaxutil.la \
    $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
    $(top_builddir)/axiom/src/parser/@WRAPPER_DIR@/libaxis2_parser.la \
    $(top_builddir)/src/core/engine/libaxis2_engine.la \
    $(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
    $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
    $(JSON_LIBS)

h2_json_integration_test_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/src/core/transport/http/util \
    -I$(top_srcdir)/util/include \
    $(JSON_CFLAGS) \
    $(NGHTTP2_CFLAGS) \
    -DAXIS2_H2_JSON_TESTS

# HTTP/2 Performance Benchmark Test
h2_performance_benchmark_test_SOURCES = h2_performance_benchmark_test.c \
                                        large_json_payload_generator.c

h2_performance_benchmark_test_LDADD = \
    $(LDFLAGS) \
    $(top_builddir)/util/src/libaxutil.la \
    $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
    $(top_builddir)/axiom/src/parser/@WRAPPER_DIR@/libaxis2_parser.la \
    $(top_builddir)/src/core/engine/libaxis2_engine.la \
    $(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
    $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
    $(JSON_LIBS) \
    -lm

h2_performance_benchmark_test_CPPFLAGS = \
    -I$(top_srcdir)/include \
    -I$(top_srcdir)/src/core/transport/http/util \
    -I$(top_srcdir)/util/include \
    $(JSON_CFLAGS) \
    $(NGHTTP2_CFLAGS) \
    -DAXIS2_H2_PERFORMANCE_TESTS

# Test data files
EXTRA_DIST = large_json_payload_generator.h

# Custom test targets
check-h2-json:
	./h2_json_integration_test

check-h2-performance:
	./h2_performance_benchmark_test

check-h2-all: check-h2-json check-h2-performance
	@echo "All HTTP/2 Pure JSON tests completed"

.PHONY: check-h2-json check-h2-performance check-h2-all

endif