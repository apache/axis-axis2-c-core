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
lib_LTLIBRARIES = libaxis2_parser.la


libaxis2_parser_la_SOURCES = ../xml_reader.c ../xml_writer.c \
							libxml2_reader_wrapper.c  libxml2_writer_wrapper.c

libaxis2_parser_la_LIBADD = @LIBXML2_LIBS@ \
							../../../../util/src/libaxutil.la

libaxis2_parser_la_LDFLAGS = -version-info $(VERSION_NO)


INCLUDES = -I$(top_builddir)/include \
            -I$(top_builddir)/src/parser \
            -I ../../../../util/include \
			@LIBXML2_CFLAGS@
