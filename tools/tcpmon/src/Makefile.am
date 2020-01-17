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
prgbindir=$(prefix)/bin/tools/tcpmon

prgbin_PROGRAMS = tcpmon

tcpmon_SOURCES =  tcpmon.c \
            entry.c \
            session.c \
            util.c

tcpmon_LDADD =  \
			 $(top_builddir)/util/src/libaxutil.la \
			 $(top_builddir)//axiom/src/om/libaxis2_axiom.la \
			 $(top_builddir)/axiom/src/parser/$(WRAPPER_DIR)/libaxis2_parser.la

tcpmon_CPPFLAGS = -I$(top_srcdir)/tools/tcpmon/include \
				  -I $(top_srcdir)/util/include \
				  -I $(top_srcdir)/axiom/include \
				  -I $(top_srcdir)/include 

EXTRA_DIST=tcpmon_entry_local.h tcpmon_session_local.h
