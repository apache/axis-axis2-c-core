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
# HTTP/2 JSON: Only Apache server needed for production HTTP/2 JSON system
# CGI transport removed (January 2026) - obsolete, use mod_axis2 instead
#
# The standalone server builds outside HTTP/2 mode. It is HTTP/1.1 and links
# http_receiver and http_sender, which the HTTP/2 JSON build does not produce,
# so it is gated the same way the HTTP/1.1 tests are. It left SUBDIRS entirely
# when HTTP/2 arrived, which stopped axis2_http_server being built in any
# configuration -- including the HTTP/1.1 SOAP one the quickstart describes.
if WITH_NGHTTP2
SIMPLE_SERVER_DIR =
else
SIMPLE_SERVER_DIR = simple_axis2_server
endif

SUBDIRS=@APACHE2BUILD@ $(SIMPLE_SERVER_DIR)

# Named unconditionally so make dist and make distclean recurse into it
# whatever the tree was configured with; its Makefile is generated either way.
DIST_SUBDIRS = apache2 simple_axis2_server

# HTTP/2 JSON system only needs Apache integration (mod_axis2)
# IIS/ISAPI integration removed (February 2026) - unmaintained since 2009
