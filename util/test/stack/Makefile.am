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
TESTS = stack_test
check_PROGRAMS = stack_test
noinst_PROGRAMS = stack_test
stack_test_SOURCES = stack_test.cc

stack_test_LDADD   =   \
				$(top_builddir)/src/libaxutil.la  \
				$(top_builddir)/$(GTEST)/libgtest.a \
				$(top_builddir)/$(GTEST)/libgtest_main.a

AM_CPPFLAGS = -I$(top_srcdir)/include \
			-I ../../../axiom/include \
			-I ../../../include \
			-I $(GTEST_DIR)/include


