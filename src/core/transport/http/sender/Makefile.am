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
SUBDIRS=ssl libcurl $(NTLM_DIR)
DIST_SUBDIRS=ssl libcurl ntlm
lib_LTLIBRARIES = libaxis2_http_sender.la



if AXIS2_LIBCURL_ENABLED
LIBCURL_SOURCES=libcurl/axis2_libcurl.c\
                libcurl/libcurl_stream.c 
LIBCURL_LIBS=-lssl -lcrypto -lcurl -ldl -lz
else
LIBCURL_SOURCES=
LIBCURL_LIBS=
endif


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
								  $(SSL_SOURCES) \
								  $(LIBCURL_SOURCES)

libaxis2_http_sender_la_LIBADD = $(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la\
                                 $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la\
				 				 $(top_builddir)/axiom/src/om/libaxis2_axiom.la\
				 				 $(top_builddir)/util/src/libaxutil.la\
								 $(top_builddir)/src/core/engine/libaxis2_engine.la\
								 ${NTLM_LIB}\
								 $(LIBCURL_LIBS)\
								 $(SSL_LIBS)


libaxis2_http_sender_la_LDFLAGS = -version-info $(VERSION_NO)

INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/src/core/transport\
           -I$(top_builddir)/src/core/transport/http \
           -I$(top_builddir)/src/core/transport/http/sender/libcurl \
           -I$(top_builddir)/src/core/description \
           -I$(top_builddir)/src/core/context \
           -I$(top_builddir)/src/core/phaseresolver \
           -I$(top_builddir)/src/core/engine \
           -I$(top_builddir)/src/core/deployment \
           -I$(top_builddir)/util/include \
           -I$(top_builddir)/axiom/include \
		   $(SSL_INC)
