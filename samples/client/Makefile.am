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
# NTLM sample removed - NTLM support has been removed from Axis2/C for security reasons
# math and version are the two clients built on the SOAP stub API. src/core/
# clientapi/Makefile.am drops stub.c from libaxis2_clientapi when WITH_NGHTTP2
# is set -- HTTP/2 with SOAP is not a supported combination, see
# docs/HTTP2_CONDITIONAL_COMPILATION.md -- so in an HTTP/2 build the symbols
# they need do not exist and they fail at link with undefined references to
# axis2_stub_get_svc_client. Follow the same conditional here rather than
# leaving a build that cannot succeed.
if WITH_NGHTTP2
SUBDIRS = echo google notify mtom yahoo mtom_callback session
else
SUBDIRS = echo math google notify mtom yahoo version mtom_callback session
endif

# DIST_SUBDIRS stays complete so that a source distribution carries every
# sample regardless of the flags this tree happened to be configured with.
DIST_SUBDIRS = echo math google notify mtom yahoo version mtom_callback session
EXTRA_DIST= Makefile.am
