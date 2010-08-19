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
prglibdir=$(prefix)/services/sg_math
prglib_LTLIBRARIES = libadd.la libsub.la libmul.la libdiv.la
prglib_DATA=services.xml
EXTRA_DIST = services.xml sg_math.mk add.h div.h mul.h sub.h

noinst_HEADERS = add.h sub.h mul.h div.h
libadd_la_SOURCES = add.c add_skeleton.c
libsub_la_SOURCES = sub.c sub_skeleton.c
libmul_la_SOURCES = mul.c mul_skeleton.c
libdiv_la_SOURCES = div.c div_skeleton.c
INCLUDES = @AXIS2INC@
