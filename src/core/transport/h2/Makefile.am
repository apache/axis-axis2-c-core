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

# HTTP/2 Pure JSON Architecture - Main HTTP/2 transport directory
# Server component removed - Apache integration handled by existing mod_axis2.so
SUBDIRS = util sender test

noinst_LTLIBRARIES = libaxis2_h2_transport.la
libaxis2_h2_transport_la_SOURCES =

libaxis2_h2_transport_la_CPPFLAGS = -I$(top_srcdir)/include \
                                    -I$(top_srcdir)/util/include \
                                    -I$(top_srcdir)/src/core/engine \
                                    $(NGHTTP2_CFLAGS) \
                                    $(JSON_C_CFLAGS)

libaxis2_h2_transport_la_LIBADD = util/libaxis2_h2_util.la \
                                  sender/libaxis2_h2_sender.la