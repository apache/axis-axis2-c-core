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

# Removed features (January 2026 - attack surface reduction):
# - NTLM authentication - security vulnerabilities
# - libcurl backend - redundant with native HTTP client

SUBDIRS=ssl
DIST_SUBDIRS=ssl
lib_LTLIBRARIES = libaxis2_http_sender.la

if AXIS2_SSL_ENABLED
SSL_SOURCES = ssl/ssl_stream.c\
               ssl/ssl_utils.c
SSL_LIBS = @OPENSSLLIB@ -lssl -lcrypto
SSL_INC = @OPENSSLINC@
else
SSL_SOURCES=
SSL_LIBS=
SSL_INC =
endif

libaxis2_http_sender_la_SOURCES = http_transport_sender.c \
                                  http_sender.c \
                                  http_client.c \
								  $(SSL_SOURCES)

libaxis2_http_sender_la_LIBADD = $(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la\
                                 $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la\
				 				 $(top_builddir)/axiom/src/om/libaxis2_axiom.la\
				 				 $(top_builddir)/util/src/libaxutil.la\
								 $(top_builddir)/src/core/engine/libaxis2_engine.la\
								 $(SSL_LIBS)


libaxis2_http_sender_la_LDFLAGS = -version-info $(VERSION_NO)

libaxis2_http_sender_la_CPPFLAGS = -I$(top_srcdir)/include \
								   -I$(top_srcdir)/src/core/transport\
								   -I$(top_srcdir)/src/core/transport/http \
								   -I$(top_srcdir)/src/core/description \
								   -I$(top_srcdir)/src/core/context \
								   -I$(top_srcdir)/src/core/phaseresolver \
								   -I$(top_srcdir)/src/core/engine \
								   -I$(top_srcdir)/src/core/deployment \
								   -I$(top_srcdir)/util/include \
								   -I$(top_srcdir)/axiom/include \
								   $(SSL_INC)


if AXIS2_JSON_ENABLED
# Include JSON writer for backward compatibility with existing tests
libaxis2_http_sender_la_SOURCES += axis2_json_writer.c
libaxis2_http_sender_la_LIBADD += $(JSON_LIBS)
libaxis2_http_sender_la_CPPFLAGS += $(JSON_CFLAGS)
endif
