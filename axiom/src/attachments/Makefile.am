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
noinst_LTLIBRARIES = libaxis2_attachments.la
libaxis2_attachments_la_SOURCES = mime_part.c \
					data_handler.c \
					mime_body_part.c \
                    mime_parser.c

libaxis2_attachments_la_LIBADD = $(top_builddir)/../util/src/libaxutil.la 

libaxis2_attachments_la_CPPFLAGS =	-I$(top_srcdir)/include \
									-I$(top_srcdir)/parser \
									-I$(top_srcdir)/../util/include

EXTRA_DIST = axiom_mime_body_part.h 

