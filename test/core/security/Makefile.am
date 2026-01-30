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

# Security test suite for Axis2/C
# Tests for XXE prevention, SSL/TLS hardening, and input validation

# Only build tests when Google Test is available
if GTEST_AVAILABLE
TESTS = test_xxe_prevention test_buffer_safety test_json_security
check_PROGRAMS = test_xxe_prevention test_buffer_safety test_json_security
noinst_PROGRAMS = test_xxe_prevention test_buffer_safety test_json_security

# ASAN preload for libtool-wrapped test execution
if ENABLE_ASAN
TESTS_ENVIRONMENT = LD_PRELOAD=$(ASAN_PRELOAD)
endif

SUBDIRS =
AM_CFLAGS = -g

# XXE (XML External Entity) prevention tests
test_xxe_prevention_SOURCES = test_xxe_prevention.cc

test_xxe_prevention_LDADD = \
			../../../util/src/libaxutil.la \
			../../../axiom/src/om/libaxis2_axiom.la \
			../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
			$(top_builddir)/$(GTEST)/libgtest.a \
			$(top_builddir)/$(GTEST)/libgtest_main.a \
			-lpthread

test_xxe_prevention_CPPFLAGS = \
			-I$(top_builddir)/include \
			-I ../../../util/include \
			-I ../../../axiom/include \
			-I $(GTEST_DIR)/include

# Buffer overflow and memory safety tests
test_buffer_safety_SOURCES = test_buffer_safety.cc

test_buffer_safety_LDADD = \
			../../../util/src/libaxutil.la \
			$(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
			$(top_builddir)/$(GTEST)/libgtest.a \
			$(top_builddir)/$(GTEST)/libgtest_main.a \
			-lpthread

test_buffer_safety_CPPFLAGS = \
			-I$(top_builddir)/include \
			-I ../../../util/include \
			-I $(GTEST_DIR)/include

# JSON security tests (depth limiting, payload size, malformed input)
test_json_security_SOURCES = test_json_security.cc

test_json_security_LDADD = \
			../../../util/src/libaxutil.la \
			$(top_builddir)/$(GTEST)/libgtest.a \
			$(top_builddir)/$(GTEST)/libgtest_main.a \
			-ljson-c \
			-lpthread

test_json_security_CPPFLAGS = \
			-I$(top_builddir)/include \
			-I ../../../util/include \
			$(JSON_CFLAGS) \
			-I $(GTEST_DIR)/include

endif
