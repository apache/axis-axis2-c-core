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
SUBDIRS = qpid_sender

lib_LTLIBRARIES = libaxis2_amqp_sender.la

libaxis2_amqp_sender_la_SOURCES = axis2_amqp_sender.c

libaxis2_amqp_sender_la_LIBADD = $(top_builddir)/util/src/libaxutil.la \
								 $(top_builddir)/src/core/transport/amqp/util/libaxis2_amqp_util.la \
								 $(top_builddir)/src/core/transport/amqp/sender/qpid_sender/libaxis2_qpid_sender.la

libaxis2_amqp_sender_la_LDFLAGS = -version-info $(VERSION_NO)

INCLUDES = -I$(top_builddir)/include \
	       -I$(top_builddir)/src/core/transport/amqp/util \
	       -I$(top_builddir)/src/core/transport/amqp/receiver \
	       -I$(top_builddir)/src/core/transport/amqp/receiver/qpid_receiver \
	       -I$(top_builddir)/src/core/transport/amqp/sender \
	       -I$(top_builddir)/src/core/transport/amqp/sender/qpid_sender \
           -I$(top_builddir)/src/core/description \
           -I$(top_builddir)/src/core/context \
           -I$(top_builddir)/src/core/phaseresolver \
           -I$(top_builddir)/src/core/engine \
           -I$(top_builddir)/src/core/deployment \
           -I$(top_builddir)/util/include \
           -I$(top_builddir)/axiom/include

