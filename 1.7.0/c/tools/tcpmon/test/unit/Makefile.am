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
TESTS =
prgbindir=$(prefix)/bin/unit_test
prgbin_PROGRAMS = tcpmon_unit_test_suite

tcpmon_unit_test_suite_SOURCES = main.c tcpmon_test.c

tcpmon_unit_test_suite_LDADD   =   \
                     $(top_builddir)/src/libaxis2_tcpmon.la \
                     -L$(CUTEST_HOME)/lib \
                     -lcutest \
					 ../../../../util/src/libaxutil.la \
					 ../../../../axiom/src/om/libaxis2_axiom.la \
                     $(NULL)


INCLUDES = -I${CUTEST_HOME}/include \
           -I$(top_builddir)/include \
		   -I ../../../../util/include \
           $(NULL)
