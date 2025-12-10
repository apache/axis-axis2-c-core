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
lib_LTLIBRARIES=libaxis2_engine.la

libaxis2_engine_la_SOURCES= $(top_srcdir)/src/core/transport/transport_receiver.c \
							handler.c \
							conf.c \
							phase.c \
							disp_checker.c \
							addr_disp.c \
							rest_disp.c \
							req_uri_disp.c \
							disp.c \
							soap_action_disp.c \
							soap_body_disp.c \
							ctx_handler.c \
							engine.c \
							axis2_engine_service_provider.c

libaxis2_engine_la_LDFLAGS = -version-info $(VERSION_NO)

libaxis2_engine_la_LIBADD=$(top_builddir)/src/core/description/libaxis2_description.la \
						  $(top_builddir)/src/core/receivers/libaxis2_receivers.la \
						  $(top_builddir)/src/core/deployment/libaxis2_deployment.la \
						  $(top_builddir)/src/core/context/libaxis2_context.la \
						  $(top_builddir)/src/core/addr/libaxis2_addr.la \
						  $(top_builddir)/src/core/clientapi/libaxis2_clientapi.la \
						  $(top_builddir)/src/core/phaseresolver/libaxis2_phaseresolver.la \
						  $(top_builddir)/src/core/util/libaxis2_core_utils.la \
						  $(top_builddir)/util/src/libaxutil.la \
						  $(top_builddir)/neethi/src/libneethi.la

# Include axiom for HTTP/1.1 SOAP support (HTTP/2 uses pure JSON)
libaxis2_engine_la_LIBADD += $(top_builddir)/axiom/src/om/libaxis2_axiom.la
# ARCHITECTURAL NOTE: HTTP dependencies removed to prevent circular dependency crashes
# Samples requiring HTTP functions should link HTTP libraries directly


libaxis2_engine_la_CPPFLAGS =  -I$(top_srcdir)/include \
							   -I$(top_srcdir)/src/wsdl \
							   -I$(top_srcdir)/src/core/description \
							   -I$(top_srcdir)/src/core/engine \
							   -I$(top_srcdir)/src/core/phaseresolver \
							   -I$(top_srcdir)/src/core/deployment \
							   -I$(top_srcdir)/src/core/context \
							   -I$(top_srcdir)/src/core/util \
							   -I$(top_srcdir)/src/core/clientapi \
							   -I$(top_srcdir)/util/include \
							   -I$(top_srcdir)/neethi/include

# Include axiom headers for HTTP/1.1 SOAP support (HTTP/2 uses pure JSON)
libaxis2_engine_la_CPPFLAGS += -I$(top_srcdir)/axiom/include

EXTRA_DIST=axis2_disp_checker.h

if AXIS2_JSON_ENABLED
libaxis2_engine_la_LIBADD += $(JSON_LIBS)
endif

# Ensure proper cleanup of libtool artifacts
CLEANFILES = *.o *.lo *.la
DISTCLEANFILES = *.o *.lo *.la
MAINTAINERCLEANFILES = Makefile.in

# Force removal of libtool .libs directory and all contents
clean-local:
	rm -rf .libs

distclean-local: clean-local
	rm -f *.o *.lo *.la
