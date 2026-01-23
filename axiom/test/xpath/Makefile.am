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

SUBDIRS =

# Only build tests if gtest is available from source
if GTEST_AVAILABLE
TESTS = test_xpath test_xpath_streaming
noinst_PROGRAMS = test_xpath test_xpath_streaming
check_PROGRAMS = test_xpath test_xpath_streaming

# ASAN preload for libtool-wrapped test execution
if ENABLE_ASAN
TESTS_ENVIRONMENT = LD_PRELOAD=$(ASAN_PRELOAD)
endif

test_xpath_SOURCES = test_xpath.cc
test_xpath_streaming_SOURCES = test_xpath_streaming.cc

test_xpath_LDADD   =	$(top_builddir)/../util/src/libaxutil.la \
					$(top_builddir)/src/om/libaxis2_axiom.la \
					$(top_builddir)/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					$(top_builddir)/src/xpath/libaxis2_xpath.la  \
					$(top_builddir)/$(GTEST)/libgtest.a \
					$(top_builddir)/$(GTEST)/libgtest_main.a

test_xpath_streaming_LDADD   =	$(top_builddir)/../util/src/libaxutil.la \
					$(top_builddir)/src/om/libaxis2_axiom.la \
					$(top_builddir)/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
					$(top_builddir)/src/xpath/libaxis2_xpath.la \
					$(top_builddir)/$(GTEST)/libgtest.a \
					$(top_builddir)/$(GTEST)/libgtest_main.a

AM_CPPFLAGS = -I$(top_srcdir)/include \
            -I$(top_srcdir)/src/parser \
			-I$(top_srcdir)/../include \
			-I$(top_srcdir)/../util/include \
			-I $(GTEST_DIR)/include
else
# Skip tests when gtest is not available
TESTS =
noinst_PROGRAMS =
check_PROGRAMS =
endif