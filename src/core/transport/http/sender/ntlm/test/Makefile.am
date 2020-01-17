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
SUBDIRS = mock_server
noinst_PROGRAMS = test_ntlm mock_client
check_PROGRAMS = test_ntlm mock_client
test_ntlm_SOURCES = client.c
mock_client_SOURCES = mock_client.c

LINK_FLAGS   =	$(top_builddir)/src/core/transport/http/sender/ntlm/$(NTLM_WRAPPER_DIR)/libaxis2_ntlm.la \
					$(top_builddir)/util/src/libaxutil.la

PREPROCESSOR_FLAGS = -I$(top_srcdir)/include \
					 -I$(top_srcdir)/util/include

test_ntlm_LDADD   =	$(LINK_FLAGS)
mock_client_LDADD   =	$(LINK_FLAGS)

test_ntlm_CPPFLAGS   =	$(PREPROCESSOR_FLAGS)
mock_client_CPPFLAGS   =	$(PREPROCESSOR_FLAGS)
