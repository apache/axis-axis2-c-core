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
lib_LTLIBRARIES = libaxis2_minizip.la

libaxis2_minizip_la_SOURCES =	ioapi.c \
							unzip.c \
							archive_extract.c \
							crypt.c

libaxis2_minizip_la_LDFLAGS = -version-info $(VERSION_NO)

INCLUDES = 	-I$(top_builddir)/include \
            -I$(top_builddir)/platforms

EXTRA_DIST=	axis2_archive_extract.h \
			axis2_crypt.h  \
			axis2_ioapi.h \
			axis2_iowin32.h \
			axis2_unzip.h \
			iowin32.c
