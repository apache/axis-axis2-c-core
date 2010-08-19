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
noinst_LTLIBRARIES = libaxis2_clientapi.la
libaxis2_clientapi_la_SOURCES = async_result.c \
                                callback.c \
                                listener_manager.c \
                                callback_recv.c \
                                stub.c \
				options.c \
				op_client.c \
				svc_client.c

INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/core/engine \
            -I$(top_builddir)/src/core/deployment \
            -I$(top_builddir)/util/include \
           -I$(top_builddir)/util/include/platforms \
            -I$(top_builddir)/axiom/include \
			-I$(top_builddir)/neethi/include
