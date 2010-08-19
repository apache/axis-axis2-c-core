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
lib_LTLIBRARIES = libaxis2_xpath.la
libaxis2_xpath_la_SOURCES = xpath.c \
			xpath_functions.c \
			xpath_internals.c \
			xpath_internals_engine.c \
			xpath_internals_iterators.c \
			xpath_internals_parser.c \
			xpath_streaming.c

libaxis2_xpath_la_LDFLAGS = -version-info $(VERSION_NO)

libaxis2_xpath_la_LIBADD = 
INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
            -I$(top_builddir)/src/om \
			-I ../../../util/include \
			-I ../../../include

EXTRA_DIST =    xpath_functions.h  xpath_internals_engine.h \
		xpath_internals.h  xpath_internals_iterators.h \
		xpath_internals_parser.h  xpath_streaming.h

