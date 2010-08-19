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
TESTS = test_thread test_util
noinst_PROGRAMS = test_util test_thread
noinst_HEADERS = test_log.h \
                 test_thread.h \
		 create_env.h\
                 test_md5.h
check_PROGRAMS = test_util test_thread
SUBDIRS =
test_util_SOURCES = test_util.c test_log.c test_md5.c create_env.c
test_thread_SOURCES = test_thread.c

test_util_LDADD   =  $(top_builddir)/src/libaxutil.la 

test_thread_LDADD   =  $(top_builddir)/src/libaxutil.la 

INCLUDES = -I$(top_builddir)/include \
			 -I$(CUTEST_HOME)/include \
		 	-I ../../../test/cutest/include
EXTRA_DIST = test_string.c
