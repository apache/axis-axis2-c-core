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
lib_LTLIBRARIES = libguththila.la

libguththila_la_LDFLAGS = -version-info $(VERSION_NO)

libguththila_la_SOURCES = guththila_buffer.c \
			  guththila_namespace.c \
	 		  guththila_token.c \
			  guththila_reader.c \
			  guththila_attribute.c \
			  guththila_xml_parser.c \
			  guththila_stack.c \
			  guththila_xml_writer.c

libguththila_la_LIBADD = ../../util/src/libaxutil.la

INCLUDES = -I$(top_builddir)/include \
			-I ../../util/include

