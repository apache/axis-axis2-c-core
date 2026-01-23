# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Base configuration
SUBDIRS =

# Initialize program lists
check_PROGRAMS =
noinst_PROGRAMS =
TESTS =

# HTTP/2 Pure JSON Architecture - HTTP/2 tests removed (no axiom dependencies)
# Test functionality is handled by HTTP/2-specific test framework

# Only build gtest tests when Google Test is available AND not in HTTP/2 mode
# HTTP/2 mode doesn't build the receiver/sender libraries that this test needs
if GTEST_AVAILABLE
if !WITH_NGHTTP2
check_PROGRAMS += test_http_transport test_axis2c_1480 test_axis2c_1532
noinst_PROGRAMS += test_http_transport test_axis2c_1480 test_axis2c_1532
TESTS += test_http_transport test_axis2c_1480 test_axis2c_1532

AM_CFLAGS = -g -pthread
test_http_transport_SOURCES = test_http_transport.cc

test_http_transport_LDADD = \
							$(LDFLAGS) \
							../../../../util/src/libaxutil.la \
							../../../../axiom/src/om/libaxis2_axiom.la \
							$(top_builddir)/neethi/src/libneethi.la \
							../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
							$(top_builddir)/src/core/engine/libaxis2_engine.la \
							$(top_builddir)/src/core/transport/http/receiver/libaxis2_http_receiver.la \
							$(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la \
							$(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
							$(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
							$(top_builddir)/$(GTEST)/libgtest.a \
							$(top_builddir)/$(GTEST)/libgtest_main.a

test_http_transport_CPPFLAGS = \
			  -I$(top_builddir)/include \
			  -I ../../../../util/include \
			  -I ../../../../axiom/include \
			  -I $(GTEST_DIR)/include

# AXIS2C-1480: Buffered header reading tests with security coverage
test_axis2c_1480_SOURCES = test_axis2c_1480.cc

test_axis2c_1480_LDADD = \
							$(LDFLAGS) \
							../../../../util/src/libaxutil.la \
							../../../../axiom/src/om/libaxis2_axiom.la \
							$(top_builddir)/neethi/src/libneethi.la \
							../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
							$(top_builddir)/src/core/engine/libaxis2_engine.la \
							$(top_builddir)/src/core/transport/http/receiver/libaxis2_http_receiver.la \
							$(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la \
							$(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
							$(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
							$(top_builddir)/$(GTEST)/libgtest.a \
							$(top_builddir)/$(GTEST)/libgtest_main.a \
							-lpthread

test_axis2c_1480_CPPFLAGS = \
			  -I$(top_builddir)/include \
			  -I ../../../../util/include \
			  -I ../../../../axiom/include \
			  -I $(GTEST_DIR)/include

# AXIS2C-1532: Empty parameter value tests
test_axis2c_1532_SOURCES = test_axis2c_1532.cc

test_axis2c_1532_LDADD = \
							$(LDFLAGS) \
							../../../../util/src/libaxutil.la \
							../../../../axiom/src/om/libaxis2_axiom.la \
							$(top_builddir)/neethi/src/libneethi.la \
							../../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
							$(top_builddir)/src/core/engine/libaxis2_engine.la \
							$(top_builddir)/src/core/transport/http/receiver/libaxis2_http_receiver.la \
							$(top_builddir)/src/core/transport/http/sender/libaxis2_http_sender.la \
							$(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
							$(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la \
							$(top_builddir)/$(GTEST)/libgtest.a \
							$(top_builddir)/$(GTEST)/libgtest_main.a \
							-lpthread

# Ensure all shared libraries are linked even if not directly referenced by test code.
# The test only calls axis2_http_transport_utils_get_request_params() which is in
# libaxis2_http_util, but that library needs symbols from libaxis2_engine.
test_axis2c_1532_LDFLAGS = -Wl,--no-as-needed

test_axis2c_1532_CPPFLAGS = \
			  -I$(top_builddir)/include \
			  -I ../../../../util/include \
			  -I ../../../../axiom/include \
			  -I $(GTEST_DIR)/include

# Add JSON support conditionally
if AXIS2_JSON_ENABLED
test_http_transport_LDADD += $(JSON_LIBS)
test_http_transport_CPPFLAGS += $(JSON_CFLAGS)
test_axis2c_1480_LDADD += $(JSON_LIBS)
test_axis2c_1480_CPPFLAGS += $(JSON_CFLAGS)
endif
endif
endif

# AXIS2C-1371: libcurl debug logging tests
# These tests verify that libcurl verbose logging is properly integrated
if GTEST_AVAILABLE
if AXIS2_LIBCURL_ENABLED
check_PROGRAMS += test_libcurl_debug
noinst_PROGRAMS += test_libcurl_debug
TESTS += test_libcurl_debug

test_libcurl_debug_SOURCES = test_libcurl_debug.cc

test_libcurl_debug_LDADD = \
							$(LDFLAGS) \
							../../../../util/src/libaxutil.la \
							$(top_builddir)/$(GTEST)/libgtest.a \
							$(top_builddir)/$(GTEST)/libgtest_main.a \
							-lcurl \
							-lpthread

test_libcurl_debug_CPPFLAGS = \
			  -I$(top_builddir)/include \
			  -I ../../../../util/include \
			  -I $(GTEST_DIR)/include \
			  -DAXIS2_LIBCURL_ENABLED
endif
endif
