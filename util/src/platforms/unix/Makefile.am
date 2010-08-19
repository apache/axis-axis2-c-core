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
noinst_LTLIBRARIES = libaxis2_unix.la

libaxis2_unix_la_SOURCES = uuid_gen_unix.c\
                           thread_unix.c date_time_util_unix.c 

INCLUDES = -I$(top_builddir)/include \
           -I$(top_builddir)/include/platforms \
           -I$(top_builddir)/include/platforms/unix
