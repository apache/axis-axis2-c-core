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
lib_LTLIBRARIES = libaxis2_qpid_sender.la

libaxis2_qpid_sender_la_SOURCES = axis2_qpid_sender.cpp \
								  axis2_qpid_sender_interface.cpp

libaxis2_qpid_sender_la_LIBADD = $(top_builddir)/util/src/libaxutil.la \
								 $(top_builddir)/src/core/transport/amqp/util/libaxis2_amqp_util.la \
								 $(QPID_HOME)/lib/libqpidclient.la

libaxis2_qpid_sender_la_LDFLAGS = g++ -version-info $(VERSION_NO)

libaxis2_qpid_sender_la_CPPFLAGS = -I$(top_srcdir)/include \
								   -I$(top_srcdir)/src/core/transport/amqp/sender/qpid_sender \
								   -I$(top_srcdir)/src/core/transport/amqp/util \
								   -I$(top_srcdir)/src/core/description \
								   -I$(top_srcdir)/src/core/context \
								   -I$(top_srcdir)/src/core/phaseresolver \
								   -I$(top_srcdir)/src/core/engine \
								   -I$(top_srcdir)/src/core/deployment \
								   -I$(top_srcdir)/util/include \
								   -I$(top_srcdir)/axiom/include \
								   -I$(QPID_HOME)/include

