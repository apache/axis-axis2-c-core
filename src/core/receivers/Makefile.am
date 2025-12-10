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
noinst_LTLIBRARIES = libaxis2_receivers.la

# Revolutionary JSON Components - HTTP/2 conditional build
if WITH_NGHTTP2
libaxis2_receivers_la_SOURCES = msg_recv.c \
                                axis2_json_rpc_msg_recv.c \
                                svr_callback.c
libaxis2_receivers_la_CPPFLAGS = -I$(top_srcdir)/include \
                                -I$(top_srcdir)/src/core/engine \
                                -I$(top_srcdir)/src/core/util \
                                -I$(top_srcdir)/util/include \
                                -I$(top_srcdir)/axiom/include \
                                -I/usr/include/json-c \
                                -DAXIS2_JSON_ENABLED \
                                -DAXIS2_H2_JSON_ONLY_BUILD
libaxis2_receivers_la_LIBADD = -ljson-c
else
libaxis2_receivers_la_SOURCES = msg_recv.c \
                                raw_xml_in_out_msg_recv.c \
                                svr_callback.c
libaxis2_receivers_la_CPPFLAGS = -I$(top_srcdir)/include \
                                -I$(top_srcdir)/src/core/engine \
                                -I$(top_srcdir)/src/core/util \
                                -I$(top_srcdir)/util/include \
                                -I$(top_srcdir)/axiom/include
endif