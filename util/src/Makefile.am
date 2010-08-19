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
SUBDIRS = platforms/unix @ZLIBBUILD@
lib_LTLIBRARIES = libaxutil.la
libaxutil_la_SOURCES = hash.c \
                        allocator.c \
                        env.c \
                        error.c \
                        stream.c \
                        log.c \
                        string.c \
                        string_util.c \
                        qname.c \
                        array_list.c \
                        linked_list.c \
                        utils.c \
                        dir_handler.c \
                        file_handler.c \
                        class_loader.c\
                        network_handler.c \
                        file.c\
                        uuid_gen.c\
                        thread_pool.c \
                        property.c \
                        types.c \
                        param.c \
                        param_container.c \
                        dll_desc.c\
                        url.c\
                        stack.c \
                        generic_obj.c \
                        base64.c \
                        uri.c \
                        date_time.c \
                        base64_binary.c \
                        properties.c \
						rand.c \
						date_time_util.c \
						version.c \
						duration.c \
			md5.c \
			http_chunked_stream.c \
                        digest_calc.c 
                        
libaxutil_la_LIBADD =	$(top_builddir)/src/platforms/unix/libaxis2_unix.la \
			-lpthread \
			@ZLIBLIBS@

libaxutil_la_LDFLAGS = -version-info $(VERSION_NO)

INCLUDES = -I$(top_builddir)/include

EXTRA_DIST=platforms/windows string_util.c
