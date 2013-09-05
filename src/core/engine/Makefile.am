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

libaxis2_engine_la_SOURCES= ../transport/transport_receiver.c handler.c \
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
                        	engine.c

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
					$(top_builddir)/neethi/src/libneethi.la \
					$(top_builddir)/axiom/src/om/libaxis2_axiom.la 
libaxis2_engine_la_LIBADD+=$(top_builddir)/src/core/transport/http/common/libaxis2_http_common.la \
			   $(top_builddir)/src/core/transport/http/util/libaxis2_http_util.la


INCLUDES =  -I$(top_builddir)/include \
            -I$(top_builddir)/src/wsdl \
            -I$(top_builddir)/src/core/description \
            -I$(top_builddir)/src/core/engine \
            -I$(top_builddir)/src/core/phaseresolver \
            -I$(top_builddir)/src/core/deployment \
            -I$(top_builddir)/src/core/context \
            -I$(top_builddir)/src/core/util \
            -I$(top_builddir)/src/core/clientapi \
            -I$(top_builddir)/util/include \
            -I$(top_builddir)/neethi/include \
            -I$(top_builddir)/axiom/include

EXTRA_DIST=axis2_disp_checker.h

if AXIS2_JSON_ENABLED
libaxis2_engine_la_LIBADD += $(JSON_LIBS)
endif
