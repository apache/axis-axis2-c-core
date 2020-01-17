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
SUBDIRS = secpolicy rmpolicy
lib_LTLIBRARIES=libneethi.la

libneethi_la_SOURCES= all.c   \
				assertion.c \
				engine.c \
				exactlyone.c \
				operator.c \
				policy.c \
				reference.c \
				registry.c \
				assertion_builder.c \
				mtom_assertion_checker.c \
				util.c

libneethi_la_LIBADD  = $(top_builddir)/../axiom/src/om/libaxis2_axiom.la \
			$(top_builddir)/../util/src/libaxutil.la \
			$(top_builddir)/src/secpolicy/builder/librp_builder.la \
			$(top_builddir)/src/secpolicy/model/librp_model.la \
			$(top_builddir)/src/rmpolicy/librm_policy.la


libneethi_la_LDFLAGS = -version-info $(VERSION_NO)

libneethi_la_CPPFLAGS = -I$(top_srcdir)/include \
						-I $(top_srcdir)/../util/include \
						-I $(top_srcdir)/../axiom/include \
						-I $(top_srcdir)/../include

