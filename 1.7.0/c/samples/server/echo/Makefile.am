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
prglibdir=$(prefix)/services/echo
prglib_LTLIBRARIES = libecho.la
prglib_DATA= services.xml
EXTRA_DIST = services.xml echo.mk echo.h
noinst_HEADERS = echo.h
SUBDIRS =
libecho_la_SOURCES = echo.c echo_skeleton.c
libecho_la_LIBADD  =
INCLUDES = @AXIS2INC@
