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
prgbindir=$(prefix)/samples/bin
prgbin_PROGRAMS = ntlm_post_with_check ntlm_post
AM_CFLAGS = -g -O2
ntlm_post_with_check_SOURCES = ntlm_post_with_check.c ntlm_util.c
ntlm_post_SOURCES = ntlm_post.c ntlm_util.c

LINK_FLAGS   = $(LDFLAGS) \
                -L$(AXIS2C_HOME)/lib \
					-laxutil \
                    -laxis2_axiom \
                    -laxis2_engine \
                    -laxis2_parser \
                    -laxis2_http_sender \
                    -laxis2_http_receiver \
					-laxis2_ntlm \
                    $(GUTHTHILA_LIBS)

ntlm_post_with_check_LDADD   = $(LINK_FLAGS)
ntlm_post_LDADD   = $(LINK_FLAGS)
AM_CPPFLAGS = @AXIS2INC@
EXTRA_DIST=ntlm_util.h README ntlm.mk
