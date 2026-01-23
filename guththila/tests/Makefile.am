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
TESTS = test_guththila test_attribute test_stack test_token
noinst_PROGRAMS = test_guththila test_attribute test_stack test_token
check_PROGRAMS = test_guththila test_attribute test_stack test_token

# ASAN preload for libtool-wrapped test execution
if ENABLE_ASAN
TESTS_ENVIRONMENT = LD_PRELOAD=$(ASAN_PRELOAD)
endif

# Set up test resources before running tests
check-local:
	$(MKDIR_P) $(builddir)/resources/om
	$(MKDIR_P) $(builddir)/resources/soap
	cp -f $(top_srcdir)/../axiom/test/resources/xml/om/axis.xml $(builddir)/resources/om/axis.xml
	cp -f $(top_srcdir)/../axiom/test/resources/xml/om/evaluate.xml $(builddir)/resources/om/evaluate.xml
	cp -f $(top_srcdir)/../axiom/test/resources/xml/om/numbers.xml $(builddir)/resources/om/numbers.xml
	cp -f $(top_srcdir)/../axiom/test/resources/xml/soap/soapmessage.xml $(builddir)/resources/soap/soapmessage.xml

# Clean up test resources
clean-local:
	-rm -rf $(builddir)/resources

# Ensure resources are copied before running any tests
$(TESTS): check-local

test_guththila_SOURCES = test_guththila.cc

AM_CPPFLAGS = -I$(top_srcdir)/include \
				-I$(top_srcdir)/../util/include \
				-I$(top_srcdir)/../axiom/include \
				-I$(GTEST_DIR)/include

test_guththila_LDADD = \
				 $(top_builddir)/src/libguththila.la \
				 $(top_builddir)/../util/src/libaxutil.la \
				 $(top_builddir)/$(GTEST)/libgtest.a \
				 $(top_builddir)/$(GTEST)/libgtest_main.a

test_attribute_SOURCES = test_attribute.cc

test_attribute_LDADD = \
				 $(top_builddir)/src/libguththila.la \
				 $(top_builddir)/../util/src/libaxutil.la \
				 $(top_builddir)/$(GTEST)/libgtest.a \
				 $(top_builddir)/$(GTEST)/libgtest_main.a

test_stack_SOURCES = test_stack.cc

test_stack_LDADD = \
				 $(top_builddir)/src/libguththila.la \
				 $(top_builddir)/../util/src/libaxutil.la \
				 $(top_builddir)/$(GTEST)/libgtest.a \
				 $(top_builddir)/$(GTEST)/libgtest_main.a

test_token_SOURCES = test_token.cc

test_token_LDADD = \
				 $(top_builddir)/src/libguththila.la \
				 $(top_builddir)/../util/src/libaxutil.la \
				 $(top_builddir)/$(GTEST)/libgtest.a \
				 $(top_builddir)/$(GTEST)/libgtest_main.a
else
# Skip tests when gtest is not available
TESTS =
noinst_PROGRAMS =
check_PROGRAMS =
endif