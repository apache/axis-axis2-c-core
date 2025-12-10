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
lib_LTLIBRARIES = libaxis2_http_common.la

# REVOLUTIONARY HTTP/2 JSON: Add context and addr dependencies for msg functions
# Add AXIOM dependency to resolve axiom_element_set_node and other AXIOM symbols needed for SOAP envelope creation
# Add engine dependency to resolve service provider implementation symbols at runtime (circular dependency eliminated)
libaxis2_http_common_la_LIBADD=$(top_builddir)/util/src/libaxutil.la\
                                 $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la\
                                 $(top_builddir)/src/core/context/libaxis2_context.la\
                                 $(top_builddir)/src/core/addr/libaxis2_addr.la\
                                 $(top_builddir)/axiom/src/om/libaxis2_axiom.la\
                                 $(top_builddir)/src/core/engine/libaxis2_engine.la

libaxis2_http_common_la_SOURCES = http_header.c\
                                  http_out_transport_info.c\
                                  http_request_line.c\
                                  http_simple_request.c\
                                  http_simple_response.c\
                                  http_status_line.c\
                                  http_accept_record.c\
                                  http_response_writer.c\
                                  simple_http_svr_conn.c\
                                  http_worker.c\
                                  http_socket_timeout.c


libaxis2_http_common_la_LDFLAGS = -version-info $(VERSION_NO)

libaxis2_http_common_la_CPPFLAGS = -I$(top_srcdir)/include \
								   -I$(top_srcdir)/src/core/transport\
								   -I$(top_srcdir)/src/core/transport/http \
								   -I$(top_srcdir)/src/core/description \
								   -I$(top_srcdir)/src/core/context \
								   -I$(top_srcdir)/src/core/phaseresolver \
								   -I$(top_srcdir)/src/core/engine \
								   -I$(top_srcdir)/src/core/deployment \
								   -I$(top_srcdir)/util/include \
								   -I$(top_srcdir)/axiom/include

# Ensure proper cleanup of libtool artifacts
CLEANFILES = *.o *.lo *.la
DISTCLEANFILES = *.o *.lo *.la
MAINTAINERCLEANFILES = Makefile.in

# Force removal of libtool .libs directory and all contents
clean-local:
	rm -rf .libs

distclean-local: clean-local
	rm -f *.o *.lo *.la
