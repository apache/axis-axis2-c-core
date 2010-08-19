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
TESTS = test_deployment
noinst_PROGRAMS = test_deployment
SUBDIRS =
AM_CFLAGS = $(CFLAGS) -g
test_deployment_SOURCES = test_deployment.c

test_deployment_LDADD   =  \
                    ../../../util/src/libaxutil.la \
                    ../../../axiom/src/om/libaxis2_axiom.la \
                    ../../../axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la \
		            $(top_builddir)/neethi/src/libneethi.la \
					$(top_builddir)/src/core/engine/libaxis2_engine.la \
					$(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la 

INCLUDES = -I ../../../include \
			-I$(CUTEST_HOME)/include \
            -I ../../../src/core/description \
            -I ../../../src/core/context \
            -I ../../../src/core/phaseresolver \
            -I ../../../src/core/deployment \
            -I ../../../src/core/engine \
            -I ../../../src/core/clientapi \
            -I ../../../src/core/util \
            -I ../../../util/include \
            -I ../../../axiom/include \
            -I ../../../neethi/include \
		 	-I ../../cutest/include
