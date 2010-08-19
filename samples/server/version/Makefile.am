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
prglibdir=$(prefix)/services/version
prglib_LTLIBRARIES = libversion.la
prglib_DATA=services.xml 
noinst_HEADERS = version.h
SUBDIRS =
libversion_la_SOURCES = version.c version_skel.c
libversion_la_LIBADD  =
INCLUDES = @AXIS2INC@
EXTRA_DIST = services.xml version.h

