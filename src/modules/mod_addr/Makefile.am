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
prglibdir=$(prefix)/modules/addressing
prglib_LTLIBRARIES = libaxis2_mod_addr.la
prglib_DATA= module.xml
EXTRA_DIST = module.xml
libaxis2_mod_addr_la_SOURCES = addr_in_handler.c \
                                addr_out_handler.c \
                                mod_addr.c

libaxis2_mod_addr_la_LIBADD = $(top_builddir)/axiom/src/om/libaxis2_axiom.la \
			      $(top_builddir)/util/src/libaxutil.la \
			      $(top_builddir)/src/core/engine/libaxis2_engine.la 

libaxis2_mod_addr_la_LDFLAGS = -version-info $(VERSION_NO)

libaxis2_mod_addr_la_CPPFLAGS = -I$(top_srcdir)/include \
								-I$(top_srcdir)/src/core/description \
								-I$(top_srcdir)/src/core/context \
								-I$(top_srcdir)/src/core/phaseresolver \
								-I$(top_srcdir)/src/core/engine \
								-I$(top_srcdir)/src/core/deployment \
								-I$(top_srcdir)/util/include \
								-I$(top_srcdir)/axiom/include
