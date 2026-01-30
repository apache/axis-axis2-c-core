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

# Only build tests when Google Test is available
if GTEST_AVAILABLE
TESTS = test_axis2c_1527
check_PROGRAMS = test_axis2c_1527
noinst_PROGRAMS = test_axis2c_1527

# ASAN preload for libtool-wrapped test execution
if ENABLE_ASAN
AM_LDFLAGS = -fsanitize=address
TESTS_ENVIRONMENT = LD_PRELOAD=$(ASAN_PRELOAD)
endif

SUBDIRS =
AM_CFLAGS = -g

# AXIS2C-1527: Date time local to UTC conversion test
test_axis2c_1527_SOURCES = test_axis2c_1527.cc

test_axis2c_1527_LDADD = \
			../../../util/src/libaxutil.la \
			$(top_builddir)/$(GTEST)/libgtest.a \
			$(top_builddir)/$(GTEST)/libgtest_main.a \
			-lpthread

test_axis2c_1527_CPPFLAGS = \
			-I$(top_builddir)/include \
			-I ../../../util/include \
			-I $(GTEST_DIR)/include

endif
