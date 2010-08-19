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

libaxis2_http_common_la_LIBADD=$(top_builddir)/util/src/libaxutil.la\
                                 $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la

libaxis2_http_common_la_SOURCES = http_header.c\
                                  http_out_transport_info.c\
                                  http_request_line.c\
                                  http_simple_request.c\
                                  http_simple_response.c\
                                  http_status_line.c\
                                  http_accept_record.c\
                                  http_response_writer.c\
                                  simple_http_svr_conn.c\
                                  http_worker.c


libaxis2_http_common_la_LDFLAGS = -version-info $(VERSION_NO)

INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/src/core/transport\
	       -I$(top_builddir)/src/core/transport/http \
           -I$(top_builddir)/src/core/description \
           -I$(top_builddir)/src/core/context \
           -I$(top_builddir)/src/core/phaseresolver \
           -I$(top_builddir)/src/core/engine \
           -I$(top_builddir)/src/core/deployment \
           -I$(top_builddir)/util/include \
           -I$(top_builddir)/axiom/include
